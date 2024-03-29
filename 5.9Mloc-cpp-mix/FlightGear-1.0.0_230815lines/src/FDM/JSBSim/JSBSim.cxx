// JSBsim.cxx -- interface to the JSBsim flight model
//
// Written by Curtis Olson, started February 1999.
//
// Copyright (C) 1999  Curtis L. Olson  - curt@flightgear.org
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// $Id: JSBSim.cxx,v 1.39.2.5 2007-05-27 13:24:32 ehofman Exp $


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <simgear/compiler.h>

#include <stdio.h>	//	size_t
#ifdef SG_MATH_EXCEPTION_CLASH
#  include <math.h>
#endif

#include STL_STRING

#include <simgear/constants.h>
#include <simgear/debug/logstream.hxx>
#include <simgear/math/sg_geodesy.hxx>
#include <simgear/misc/sg_path.hxx>
#include <simgear/structure/commands.hxx>

#include <FDM/flight.hxx>

#include <Aircraft/aircraft.hxx>
#include <Aircraft/controls.hxx>
#include <Main/globals.hxx>
#include <Main/fg_props.hxx>

#include "JSBSim.hxx"
#include <FDM/JSBSim/FGFDMExec.h>
#include <FDM/JSBSim/FGJSBBase.h>
#include <FDM/JSBSim/FGState.h>
#include <FDM/JSBSim/initialization/FGInitialCondition.h>
#include <FDM/JSBSim/initialization/FGTrim.h>
#include <FDM/JSBSim/models/FGModel.h>
#include <FDM/JSBSim/models/FGAircraft.h>
#include <FDM/JSBSim/models/FGFCS.h>
#include <FDM/JSBSim/models/FGPropagate.h>
#include <FDM/JSBSim/models/FGAuxiliary.h>
#include <FDM/JSBSim/models/FGAtmosphere.h>
#include <FDM/JSBSim/models/FGMassBalance.h>
#include <FDM/JSBSim/models/FGAerodynamics.h>
#include <FDM/JSBSim/models/FGLGear.h>
#include <FDM/JSBSim/models/propulsion/FGEngine.h>
#include <FDM/JSBSim/models/propulsion/FGPiston.h>
#include <FDM/JSBSim/models/propulsion/FGTurbine.h>
#include <FDM/JSBSim/models/propulsion/FGTurboProp.h>
#include <FDM/JSBSim/models/propulsion/FGRocket.h>
#include <FDM/JSBSim/models/propulsion/FGElectric.h>
#include <FDM/JSBSim/models/propulsion/FGNozzle.h>
#include <FDM/JSBSim/models/propulsion/FGPropeller.h>
#include <FDM/JSBSim/models/propulsion/FGRotor.h>
#include <FDM/JSBSim/models/propulsion/FGTank.h>
#include <FDM/JSBSim/input_output/FGPropertyManager.h>
#include <FDM/JSBSim/input_output/FGGroundCallback.h>

using namespace JSBSim;

static inline double
FMAX (double a, double b)
{
  return a > b ? a : b;
}

class FGFSGroundCallback : public FGGroundCallback {
public:
  FGFSGroundCallback(FGInterface* ifc) : mInterface(ifc) {}
  virtual ~FGFSGroundCallback() {}

  /** Get the altitude above sea level depenent on the location. */
  virtual double GetAltitude(const FGLocation& l) const {
    double pt[3] = { SG_FEET_TO_METER*l(eX),
                     SG_FEET_TO_METER*l(eY),
                     SG_FEET_TO_METER*l(eZ) };
    double lat, lon, alt;
    sgCartToGeod( pt, &lat, &lon, &alt);
    return alt * SG_METER_TO_FEET;
  }

  /** Compute the altitude above ground. */
  virtual double GetAGLevel(double t, const FGLocation& l,
                            FGLocation& cont,
                            FGColumnVector3& n, FGColumnVector3& v) const {
    double loc_cart[3] = { l(eX), l(eY), l(eZ) };
    double contact[3], normal[3], vel[3], lc, ff, agl;
    int groundtype;
    mInterface->get_agl_ft(t, loc_cart, contact, normal, vel,
                           &groundtype, &lc, &ff, &agl);
    n = l.GetTec2l()*FGColumnVector3( normal[0], normal[1], normal[2] );
    v = l.GetTec2l()*FGColumnVector3( vel[0], vel[1], vel[2] );
    cont = FGColumnVector3( contact[0], contact[1], contact[2] );
    return agl;
  }
private:
  FGInterface* mInterface;
};

/******************************************************************************/

FGJSBsim::FGJSBsim( double dt )
  : FGInterface(dt)
{
    bool result;
                                // Set up the debugging level
                                // FIXME: this will not respond to
                                // runtime changes

                                // if flight is excluded, don't bother
    if ((logbuf::get_log_classes() & SG_FLIGHT) != 0) {

                                // do a rough-and-ready mapping to
                                // the levels documented in FGFDMExec.h
        switch (logbuf::get_log_priority()) {
        case SG_BULK:
            FGJSBBase::debug_lvl = 0x1f;
            break;
        case SG_DEBUG:
            FGJSBBase::debug_lvl = 0x0f;
        case SG_INFO:
            FGJSBBase::debug_lvl = 0x01;
            break;
        case SG_WARN:
        case SG_ALERT:
            FGJSBBase::debug_lvl = 0x00;
            break;
        }
    }

    fdmex = new FGFDMExec( (FGPropertyManager*)globals->get_props() );

    // Register ground callback.
    fdmex->SetGroundCallback( new FGFSGroundCallback(this) );

    State           = fdmex->GetState();
    Atmosphere      = fdmex->GetAtmosphere();
    FCS             = fdmex->GetFCS();
    MassBalance     = fdmex->GetMassBalance();
    Propulsion      = fdmex->GetPropulsion();
    Aircraft        = fdmex->GetAircraft();
    Propagate        = fdmex->GetPropagate();
    Auxiliary       = fdmex->GetAuxiliary();
    Aerodynamics    = fdmex->GetAerodynamics();
    GroundReactions = fdmex->GetGroundReactions();

    fgic=fdmex->GetIC();
    needTrim=true;

    SGPath aircraft_path( fgGetString("/sim/aircraft-dir") );

    SGPath engine_path( fgGetString("/sim/aircraft-dir") );
    engine_path.append( "Engine" );
    State->Setdt( dt );

    result = fdmex->LoadModel( aircraft_path.str(),
                               engine_path.str(),
                               fgGetString("/sim/aero"), false );

    if (result) {
      SG_LOG( SG_FLIGHT, SG_INFO, "  loaded aero.");
    } else {
      SG_LOG( SG_FLIGHT, SG_INFO,
              "  aero does not exist (you may have mis-typed the name).");
      throw(-1);
    }

    SG_LOG( SG_FLIGHT, SG_INFO, "" );
    SG_LOG( SG_FLIGHT, SG_INFO, "" );
    SG_LOG( SG_FLIGHT, SG_INFO, "After loading aero definition file ..." );

    int Neng = Propulsion->GetNumEngines();
    SG_LOG( SG_FLIGHT, SG_INFO, "num engines = " << Neng );

    if ( GroundReactions->GetNumGearUnits() <= 0 ) {
        SG_LOG( SG_FLIGHT, SG_ALERT, "num gear units = "
                << GroundReactions->GetNumGearUnits() );
        SG_LOG( SG_FLIGHT, SG_ALERT, "This is a very bad thing because with 0 gear units, the ground trimming");
        SG_LOG( SG_FLIGHT, SG_ALERT, "routine (coming up later in the code) will core dump.");
        SG_LOG( SG_FLIGHT, SG_ALERT, "Halting the sim now, and hoping a solution will present itself soon!");
        exit(-1);
    }

    init_gear();

    // Set initial fuel levels if provided.
    for (unsigned int i = 0; i < Propulsion->GetNumTanks(); i++) {
      SGPropertyNode * node = fgGetNode("/consumables/fuel/tank", i, true);
      if (node->getChild("level-gal_us", 0, false) != 0) {
        Propulsion->GetTank(i)->SetContents(node->getDoubleValue("level-gal_us") * 6.6);
      } else {
        node->setDoubleValue("level-lb", Propulsion->GetTank(i)->GetContents());
        node->setDoubleValue("level-gal_us", Propulsion->GetTank(i)->GetContents() / 6.6);
      }
    }
    Propulsion->SetFuelFreeze((fgGetNode("/sim/freeze/fuel",true))->getBoolValue());

    fgSetDouble("/fdm/trim/pitch-trim", FCS->GetPitchTrimCmd());
    fgSetDouble("/fdm/trim/throttle",   FCS->GetThrottleCmd(0));
    fgSetDouble("/fdm/trim/aileron",    FCS->GetDaCmd());
    fgSetDouble("/fdm/trim/rudder",     FCS->GetDrCmd());

    startup_trim = fgGetNode("/sim/presets/trim", true);

    trimmed = fgGetNode("/fdm/trim/trimmed", true);
    trimmed->setBoolValue(false);

    pitch_trim = fgGetNode("/fdm/trim/pitch-trim", true );
    throttle_trim = fgGetNode("/fdm/trim/throttle", true );
    aileron_trim = fgGetNode("/fdm/trim/aileron", true );
    rudder_trim = fgGetNode("/fdm/trim/rudder", true );

    stall_warning = fgGetNode("/sim/alarms/stall-warning",true);
    stall_warning->setDoubleValue(0);


    flap_pos_pct=fgGetNode("/surface-positions/flap-pos-norm",true);
    elevator_pos_pct=fgGetNode("/surface-positions/elevator-pos-norm",true);
    left_aileron_pos_pct
        =fgGetNode("/surface-positions/left-aileron-pos-norm",true);
    right_aileron_pos_pct
        =fgGetNode("/surface-positions/right-aileron-pos-norm",true);
    rudder_pos_pct=fgGetNode("/surface-positions/rudder-pos-norm",true);
    speedbrake_pos_pct
        =fgGetNode("/surface-positions/speedbrake-pos-norm",true);
    spoilers_pos_pct=fgGetNode("/surface-positions/spoilers-pos-norm",true);

    elevator_pos_pct->setDoubleValue(0);
    left_aileron_pos_pct->setDoubleValue(0);
    right_aileron_pos_pct->setDoubleValue(0);
    rudder_pos_pct->setDoubleValue(0);
    flap_pos_pct->setDoubleValue(0);
    speedbrake_pos_pct->setDoubleValue(0);
    spoilers_pos_pct->setDoubleValue(0);

    temperature = fgGetNode("/environment/temperature-degc",true);
    pressure = fgGetNode("/environment/pressure-inhg",true);
    density = fgGetNode("/environment/density-slugft3",true);
    turbulence_gain = fgGetNode("/environment/turbulence/magnitude-norm",true);
    turbulence_rate = fgGetNode("/environment/turbulence/rate-hz",true);

    wind_from_north= fgGetNode("/environment/wind-from-north-fps",true);
    wind_from_east = fgGetNode("/environment/wind-from-east-fps" ,true);
    wind_from_down = fgGetNode("/environment/wind-from-down-fps" ,true);

    slaved = fgGetNode("/sim/slaved/enabled", true);

    for (unsigned int i = 0; i < Propulsion->GetNumEngines(); i++) {
      SGPropertyNode * node = fgGetNode("engines/engine", i, true);
      Propulsion->GetEngine(i)->GetThruster()->SetRPM(node->getDoubleValue("rpm") /
                     Propulsion->GetEngine(i)->GetThruster()->GetGearRatio());
    }
}

/******************************************************************************/
FGJSBsim::~FGJSBsim(void)
{
  delete fdmex;
}

/******************************************************************************/

// Initialize the JSBsim flight model, dt is the time increment for
// each subsequent iteration through the EOM

void FGJSBsim::init()
{
    double tmp;

    SG_LOG( SG_FLIGHT, SG_INFO, "Starting and initializing JSBsim" );

    // Explicitly call the superclass's
    // init method first.

    if (fgGetBool("/environment/params/control-fdm-atmosphere")) {
      Atmosphere->UseExternal();
      Atmosphere->SetExTemperature(
                  9.0/5.0*(temperature->getDoubleValue()+273.15) );
      Atmosphere->SetExPressure(pressure->getDoubleValue()*70.726566);
      Atmosphere->SetExDensity(density->getDoubleValue());

      tmp = turbulence_gain->getDoubleValue();
      Atmosphere->SetTurbGain(tmp * tmp * 100.0);

      tmp = turbulence_rate->getDoubleValue();
      Atmosphere->SetTurbRate(tmp);

    } else {
      Atmosphere->UseInternal();
    }

    fgic->SetVnorthFpsIC( wind_from_north->getDoubleValue() );
    fgic->SetVeastFpsIC( wind_from_east->getDoubleValue() );
    fgic->SetVdownFpsIC( wind_from_down->getDoubleValue() );

    //Atmosphere->SetExTemperature(get_Static_temperature());
    //Atmosphere->SetExPressure(get_Static_pressure());
    //Atmosphere->SetExDensity(get_Density());
    SG_LOG(SG_FLIGHT,SG_INFO,"T,p,rho: " << fdmex->GetAtmosphere()->GetTemperature()
     << ", " << fdmex->GetAtmosphere()->GetPressure()
     << ", " << fdmex->GetAtmosphere()->GetDensity() );

    if (fgGetBool("/sim/presets/running")) {
          for (int i=0; i < Propulsion->GetNumEngines(); i++) {
            SGPropertyNode * node = fgGetNode("engines/engine", i, true);
            node->setBoolValue("running", true);
            Propulsion->GetEngine(i)->SetRunning(true);
          }
    }

    common_init();

    copy_to_JSBsim();
    fdmex->RunIC();     //loop JSBSim once w/o integrating
    copy_from_JSBsim(); //update the bus

    SG_LOG( SG_FLIGHT, SG_INFO, "  Initialized JSBSim with:" );

    switch(fgic->GetSpeedSet()) {
    case setned:
        SG_LOG(SG_FLIGHT,SG_INFO, "  Vn,Ve,Vd= "
               << Propagate->GetVel(FGJSBBase::eNorth) << ", "
               << Propagate->GetVel(FGJSBBase::eEast) << ", "
               << Propagate->GetVel(FGJSBBase::eDown) << " ft/s");
    break;
    case setuvw:
        SG_LOG(SG_FLIGHT,SG_INFO, "  U,V,W= "
               << Propagate->GetUVW(1) << ", "
               << Propagate->GetUVW(2) << ", "
               << Propagate->GetUVW(3) << " ft/s");
    break;
    case setmach:
        SG_LOG(SG_FLIGHT,SG_INFO, "  Mach: "
               << Auxiliary->GetMach() );
    break;
    case setvc:
    default:
        SG_LOG(SG_FLIGHT,SG_INFO, "  Indicated Airspeed: "
               << Auxiliary->GetVcalibratedKTS() << " knots" );
    break;
    }

    stall_warning->setDoubleValue(0);

    SG_LOG( SG_FLIGHT, SG_INFO, "  Bank Angle: "
            << Propagate->GetEuler(FGJSBBase::ePhi)*RADTODEG << " deg" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  Pitch Angle: "
            << Propagate->GetEuler(FGJSBBase::eTht)*RADTODEG << " deg" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  True Heading: "
            << Propagate->GetEuler(FGJSBBase::ePsi)*RADTODEG << " deg" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  Latitude: "
            << Propagate->GetLocation().GetLatitudeDeg() << " deg" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  Longitude: "
            << Propagate->GetLocation().GetLongitudeDeg() << " deg" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  Altitude: "
            << Propagate->Geth() << " feet" );
    SG_LOG( SG_FLIGHT, SG_INFO, "  loaded initial conditions" );

    SG_LOG( SG_FLIGHT, SG_INFO, "  set dt" );

    SG_LOG( SG_FLIGHT, SG_INFO, "Finished initializing JSBSim" );

    SG_LOG( SG_FLIGHT, SG_INFO, "FGControls::get_gear_down()= " <<
                                  globals->get_controls()->get_gear_down() );
}

/******************************************************************************/

// Run an iteration of the EOM (equations of motion)

void FGJSBsim::update( double dt )
{
    if (is_suspended())
      return;

    int multiloop = _calc_multiloop(dt);

    int i;

    // Compute the radius of the aircraft. That is the radius of a ball
    // where all gear units are in. At the moment it is at least 10ft ...
    double acrad = 10.0;
    int n_gears = GroundReactions->GetNumGearUnits();
    for (i=0; i<n_gears; ++i) {
      FGColumnVector3 bl = GroundReactions->GetGearUnit(i)->GetBodyLocation();
      double r = bl.Magnitude();
      if (acrad < r)
        acrad = r;
    }

    // Compute the potential movement of this aircraft and query for the
    // ground in this area.
    double groundCacheRadius = acrad + 2*dt*Propagate->GetUVW().Magnitude();
    double alt, slr, lat, lon;
    FGColumnVector3 cart = Auxiliary->GetLocationVRP();
    if ( needTrim && startup_trim->getBoolValue() ) {
      alt = fgic->GetAltitudeFtIC();
      slr = fgic->GetSeaLevelRadiusFtIC();
      lat = fgic->GetLatitudeDegIC() * SGD_DEGREES_TO_RADIANS;
      lon = fgic->GetLongitudeDegIC() * SGD_DEGREES_TO_RADIANS;
      cart = FGLocation(lon, lat, alt+slr);
    }
    double cart_pos[3] = { cart(1), cart(2), cart(3) };
    bool cache_ok = prepare_ground_cache_ft( State->Getsim_time(), cart_pos,
                                             groundCacheRadius );
    if (!cache_ok) {
      SG_LOG(SG_FLIGHT, SG_WARN,
             "FGInterface is being called without scenery below the aircraft!");
      SG_LOG(SG_FLIGHT, SG_WARN,
             "altitude         = " << alt);
      SG_LOG(SG_FLIGHT, SG_WARN,
            "sea level radius = " << slr);
      SG_LOG(SG_FLIGHT, SG_WARN,
            "latitude         = " << lat);
      SG_LOG(SG_FLIGHT, SG_WARN,
            "longitude        = " << lon);
      //return;
    }

    copy_to_JSBsim();

    trimmed->setBoolValue(false);

    if ( needTrim ) {
      if ( startup_trim->getBoolValue() ) {
        double contact[3], dummy[3], lc, ff, agl;
        int groundtype;
        get_agl_ft(State->Getsim_time(), cart_pos, contact,
                   dummy, dummy, &groundtype, &lc, &ff, &agl);
        double terrain_alt = sqrt(contact[0]*contact[0] + contact[1]*contact[1]
             + contact[2]*contact[2]) - fgic->GetSeaLevelRadiusFtIC();

        SG_LOG(SG_FLIGHT, SG_INFO,
          "Ready to trim, terrain altitude is: "
            << terrain_alt * SG_METER_TO_FEET );

        fgic->SetTerrainAltitudeFtIC( terrain_alt );
        do_trim();
      } else {
        fdmex->RunIC();  //apply any changes made through the set_ functions
      }
      needTrim = false;
    }

    for ( i=0; i < multiloop; i++ ) {
      fdmex->Run();
    }

    FGJSBBase::Message* msg;
    while (fdmex->SomeMessages()) {
      msg = fdmex->ProcessMessage();
      switch (msg->type) {
      case FGJSBBase::Message::eText:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text );
        break;
      case FGJSBBase::Message::eBool:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->bVal );
        break;
      case FGJSBBase::Message::eInteger:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->iVal );
        break;
      case FGJSBBase::Message::eDouble:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->dVal );
        break;
      default:
        SG_LOG( SG_FLIGHT, SG_INFO, "Unrecognized message type." );
        break;
      }
    }

    // translate JSBsim back to FG structure so that the
    // autopilot (and the rest of the sim can use the updated values
    copy_from_JSBsim();
}

/******************************************************************************/

// Convert from the FGInterface struct to the JSBsim generic_ struct

bool FGJSBsim::copy_to_JSBsim()
{
    double tmp;
    unsigned int i;

    // copy control positions into the JSBsim structure

    FCS->SetDaCmd( globals->get_controls()->get_aileron());
    FCS->SetRollTrimCmd( globals->get_controls()->get_aileron_trim() );
    FCS->SetDeCmd( globals->get_controls()->get_elevator());
    FCS->SetPitchTrimCmd( globals->get_controls()->get_elevator_trim() );
    FCS->SetDrCmd( -globals->get_controls()->get_rudder() );
    FCS->SetYawTrimCmd( -globals->get_controls()->get_rudder_trim() );
    // FIXME: make that get_steering work
//     FCS->SetDsCmd( globals->get_controls()->get_steering()/80.0 );
    FCS->SetDsCmd( globals->get_controls()->get_rudder() );
    FCS->SetDfCmd( globals->get_controls()->get_flaps() );
    FCS->SetDsbCmd( globals->get_controls()->get_speedbrake() );
    FCS->SetDspCmd( globals->get_controls()->get_spoilers() );

        // Parking brake sets minimum braking
        // level for mains.
    double parking_brake = globals->get_controls()->get_brake_parking();
    FCS->SetLBrake(FMAX(globals->get_controls()->get_brake_left(), parking_brake));
    FCS->SetRBrake(FMAX(globals->get_controls()->get_brake_right(), parking_brake));
    FCS->SetCBrake( 0.0 );
    // FCS->SetCBrake( globals->get_controls()->get_brake(2) );

    FCS->SetGearCmd( globals->get_controls()->get_gear_down());
    for (i = 0; i < Propulsion->GetNumEngines(); i++) {
      SGPropertyNode * node = fgGetNode("engines/engine", i, true);

      FCS->SetThrottleCmd(i, globals->get_controls()->get_throttle(i));
      FCS->SetMixtureCmd(i, globals->get_controls()->get_mixture(i));
      FCS->SetPropAdvanceCmd(i, globals->get_controls()->get_prop_advance(i));
      FCS->SetFeatherCmd(i, globals->get_controls()->get_feather(i));

      switch (Propulsion->GetEngine(i)->GetType()) {
      case FGEngine::etPiston:
        { // FGPiston code block
        FGPiston* eng = (FGPiston*)Propulsion->GetEngine(i);
        eng->SetMagnetos( globals->get_controls()->get_magnetos(i) );
        break;
        } // end FGPiston code block
      case FGEngine::etTurbine:
        { // FGTurbine code block
        FGTurbine* eng = (FGTurbine*)Propulsion->GetEngine(i);
        eng->SetAugmentation( globals->get_controls()->get_augmentation(i) );
        eng->SetReverse( globals->get_controls()->get_reverser(i) );
        eng->SetInjection( globals->get_controls()->get_water_injection(i) );
        eng->SetCutoff( globals->get_controls()->get_cutoff(i) );
        eng->SetIgnition( globals->get_controls()->get_ignition(i) );
        break;
        } // end FGTurbine code block
      case FGEngine::etRocket:
        { // FGRocket code block
        FGRocket* eng = (FGRocket*)Propulsion->GetEngine(i);
        break;
        } // end FGRocket code block
      case FGEngine::etTurboprop:
        { // FGTurboProp code block
        FGTurboProp* eng = (FGTurboProp*)Propulsion->GetEngine(i);
        eng->SetReverse( globals->get_controls()->get_reverser(i) );
        eng->SetCutoff( globals->get_controls()->get_cutoff(i) );
        eng->SetIgnition( globals->get_controls()->get_ignition(i) );

		eng->SetGeneratorPower( globals->get_controls()->get_generator_breaker(i) );
		eng->SetCondition( globals->get_controls()->get_condition(i) );
        break;
        } // end FGTurboProp code block
      }

      { // FGEngine code block
      FGEngine* eng = Propulsion->GetEngine(i);

      eng->SetStarter( globals->get_controls()->get_starter(i) );
      eng->SetRunning( node->getBoolValue("running") );
      } // end FGEngine code block
    }


    Propagate->SetSeaLevelRadius( get_Sea_level_radius() );

    Atmosphere->SetExTemperature(
                  9.0/5.0*(temperature->getDoubleValue()+273.15) );
    Atmosphere->SetExPressure(pressure->getDoubleValue()*70.726566);
    Atmosphere->SetExDensity(density->getDoubleValue());

    tmp = turbulence_gain->getDoubleValue();
    Atmosphere->SetTurbGain(tmp * tmp * 100.0);

    tmp = turbulence_rate->getDoubleValue();
    Atmosphere->SetTurbRate(tmp);

    Atmosphere->SetWindNED( wind_from_north->getDoubleValue(),
                            wind_from_east->getDoubleValue(),
                            wind_from_down->getDoubleValue() );
//    SG_LOG(SG_FLIGHT,SG_INFO, "Wind NED: "
//                  << get_V_north_airmass() << ", "
//                  << get_V_east_airmass()  << ", "
//                  << get_V_down_airmass() );

    for (i = 0; i < Propulsion->GetNumTanks(); i++) {
      SGPropertyNode * node = fgGetNode("/consumables/fuel/tank", i, true);
      FGTank * tank = Propulsion->GetTank(i);
      tank->SetContents(node->getDoubleValue("level-gal_us") * 6.6);
//       tank->SetContents(node->getDoubleValue("level-lb"));
    }
    SGPropertyNode* node = fgGetNode("/systems/refuel", true);
    Propulsion->SetRefuel(node->getDoubleValue("contact"));
    Propulsion->SetFuelFreeze((fgGetNode("/sim/freeze/fuel",true))->getBoolValue());
    fdmex->SetSlave(slaved->getBoolValue());

    return true;
}

/******************************************************************************/

// Convert from the JSBsim generic_ struct to the FGInterface struct

bool FGJSBsim::copy_from_JSBsim()
{
    unsigned int i, j;
/*
    _set_Inertias( MassBalance->GetMass(),
                   MassBalance->GetIxx(),
                   MassBalance->GetIyy(),
                   MassBalance->GetIzz(),
                   MassBalance->GetIxz() );
*/
    _set_CG_Position( MassBalance->GetXYZcg(1),
                      MassBalance->GetXYZcg(2),
                      MassBalance->GetXYZcg(3) );

    _set_Accels_Body( Aircraft->GetBodyAccel(1),
                      Aircraft->GetBodyAccel(2),
                      Aircraft->GetBodyAccel(3) );

    _set_Accels_CG_Body_N ( Aircraft->GetNcg(1),
                            Aircraft->GetNcg(2),
                            Aircraft->GetNcg(3) );

    _set_Accels_Pilot_Body( Auxiliary->GetPilotAccel(1),
                            Auxiliary->GetPilotAccel(2),
                            Auxiliary->GetPilotAccel(3) );

    _set_Nlf( Aircraft->GetNlf() );

    // Velocities

    _set_Velocities_Local( Propagate->GetVel(FGJSBBase::eNorth),
                           Propagate->GetVel(FGJSBBase::eEast),
                           Propagate->GetVel(FGJSBBase::eDown) );

    _set_Velocities_Wind_Body( Propagate->GetUVW(1),
                               Propagate->GetUVW(2),
                               Propagate->GetUVW(3) );

    // Make the HUD work ...
    _set_Velocities_Ground( Propagate->GetVel(FGJSBBase::eNorth),
                            Propagate->GetVel(FGJSBBase::eEast),
                            -Propagate->GetVel(FGJSBBase::eDown) );

    _set_V_rel_wind( Auxiliary->GetVt() );

    _set_V_equiv_kts( Auxiliary->GetVequivalentKTS() );

    _set_V_calibrated_kts( Auxiliary->GetVcalibratedKTS() );

    _set_V_ground_speed( Auxiliary->GetVground() );

    _set_Omega_Body( Propagate->GetPQR(FGJSBBase::eP),
                     Propagate->GetPQR(FGJSBBase::eQ),
                     Propagate->GetPQR(FGJSBBase::eR) );

    _set_Euler_Rates( Auxiliary->GetEulerRates(FGJSBBase::ePhi),
                      Auxiliary->GetEulerRates(FGJSBBase::eTht),
                      Auxiliary->GetEulerRates(FGJSBBase::ePsi) );

    _set_Mach_number( Auxiliary->GetMach() );

    // Positions of Visual Reference Point
    FGLocation l = Auxiliary->GetLocationVRP();
    _updateGeocentricPosition( l.GetLatitude(), l.GetLongitude(),
                               l.GetRadius() - get_Sea_level_radius() );

    _set_Altitude_AGL( Propagate->GetDistanceAGL() );
    {
      double loc_cart[3] = { l(FGJSBBase::eX), l(FGJSBBase::eY), l(FGJSBBase::eZ) };
      double contact[3], d[3], sd, t;
      int id;
      is_valid_m(&t, d, &sd);
      get_agl_ft(t, loc_cart, contact, d, d, &id, &sd, &sd, &sd);
      double rwrad
        = FGColumnVector3( contact[0], contact[1], contact[2] ).Magnitude();
      _set_Runway_altitude( rwrad - get_Sea_level_radius() );
    }

    _set_Euler_Angles( Propagate->GetEuler(FGJSBBase::ePhi),
                       Propagate->GetEuler(FGJSBBase::eTht),
                       Propagate->GetEuler(FGJSBBase::ePsi) );

    _set_Alpha( Auxiliary->Getalpha() );
    _set_Beta( Auxiliary->Getbeta() );


    _set_Gamma_vert_rad( Auxiliary->GetGamma() );

    _set_Earth_position_angle( Auxiliary->GetEarthPositionAngle() );

    _set_Climb_Rate( Propagate->Gethdot() );

    const FGMatrix33& Tl2b = Propagate->GetTl2b();
    for ( i = 1; i <= 3; i++ ) {
        for ( j = 1; j <= 3; j++ ) {
            _set_T_Local_to_Body( i, j, Tl2b(i,j) );
        }
    }

    // Copy the engine values from JSBSim.
    for ( i=0; i < Propulsion->GetNumEngines(); i++ ) {
      SGPropertyNode * node = fgGetNode("engines/engine", i, true);
      char buf[30];
      sprintf(buf, "engines/engine[%d]/thruster", i);
      SGPropertyNode * tnode = fgGetNode(buf, true);
      FGThruster * thruster = Propulsion->GetEngine(i)->GetThruster();

      switch (Propulsion->GetEngine(i)->GetType()) {
      case FGEngine::etPiston:
        { // FGPiston code block
        FGPiston* eng = (FGPiston*)Propulsion->GetEngine(i);
        node->setDoubleValue("egt-degf", eng->getExhaustGasTemp_degF());
        node->setDoubleValue("oil-temperature-degf", eng->getOilTemp_degF());
        node->setDoubleValue("oil-pressure-psi", eng->getOilPressure_psi());
        node->setDoubleValue("mp-osi", eng->getManifoldPressure_inHg());
        node->setDoubleValue("cht-degf", eng->getCylinderHeadTemp_degF());
        node->setDoubleValue("rpm", eng->getRPM());
        } // end FGPiston code block
        break;
      case FGEngine::etRocket:
        { // FGRocket code block
        FGRocket* eng = (FGRocket*)Propulsion->GetEngine(i);
        } // end FGRocket code block
        break;
      case FGEngine::etTurbine:
        { // FGTurbine code block
        FGTurbine* eng = (FGTurbine*)Propulsion->GetEngine(i);
        node->setDoubleValue("n1", eng->GetN1());
        node->setDoubleValue("n2", eng->GetN2());
        node->setDoubleValue("egt_degf", 32 + eng->GetEGT()*9/5);
        node->setBoolValue("augmentation", eng->GetAugmentation());
        node->setBoolValue("water-injection", eng->GetInjection());
        node->setBoolValue("ignition", eng->GetIgnition());
        node->setDoubleValue("nozzle-pos-norm", eng->GetNozzle());
        node->setDoubleValue("inlet-pos-norm", eng->GetInlet());
        node->setDoubleValue("oil-pressure-psi", eng->getOilPressure_psi());
        node->setBoolValue("reversed", eng->GetReversed());
        node->setBoolValue("cutoff", eng->GetCutoff());
        node->setDoubleValue("epr", eng->GetEPR());
        globals->get_controls()->set_reverser(i, eng->GetReversed() );
        globals->get_controls()->set_cutoff(i, eng->GetCutoff() );
        globals->get_controls()->set_water_injection(i, eng->GetInjection() );
        globals->get_controls()->set_augmentation(i, eng->GetAugmentation() );
        } // end FGTurbine code block
        break;
      case FGEngine::etTurboprop:
        { // FGTurboProp code block
        FGTurboProp* eng = (FGTurboProp*)Propulsion->GetEngine(i);
        node->setDoubleValue("n1", eng->GetN1());
        //node->setDoubleValue("n2", eng->GetN2());
        node->setDoubleValue("itt_degf", 32 + eng->GetITT()*9/5);
        node->setBoolValue("ignition", eng->GetIgnition());
        node->setDoubleValue("nozzle-pos-norm", eng->GetNozzle());
        node->setDoubleValue("inlet-pos-norm", eng->GetInlet());
        node->setDoubleValue("oil-pressure-psi", eng->getOilPressure_psi());
        node->setBoolValue("reversed", eng->GetReversed());
        node->setBoolValue("cutoff", eng->GetCutoff());
        node->setBoolValue("starting", eng->GetEngStarting());
        node->setBoolValue("generator-power", eng->GetGeneratorPower());
        node->setBoolValue("damaged", eng->GetCondition());
        node->setBoolValue("ielu-intervent", eng->GetIeluIntervent());
        node->setDoubleValue("oil-temperature-degf", eng->getOilTemp_degF());
//        node->setBoolValue("onfire", eng->GetFire());
        globals->get_controls()->set_reverser(i, eng->GetReversed() );
        globals->get_controls()->set_cutoff(i, eng->GetCutoff() );
        } // end FGTurboProp code block
        break;
      case FGEngine::etElectric:
        { // FGElectric code block
        FGElectric* eng = (FGElectric*)Propulsion->GetEngine(i);
        node->setDoubleValue("rpm", eng->getRPM());
        } // end FGElectric code block
        break;
      }

      { // FGEngine code block
      FGEngine* eng = Propulsion->GetEngine(i);
      node->setDoubleValue("fuel-flow-gph", eng->getFuelFlow_gph());
      node->setDoubleValue("thrust_lb", thruster->GetThrust());
      node->setDoubleValue("fuel-flow_pph", eng->getFuelFlow_pph());
      node->setBoolValue("running", eng->GetRunning());
      node->setBoolValue("starter", eng->GetStarter());
      node->setBoolValue("cranking", eng->GetCranking());
      globals->get_controls()->set_starter(i, eng->GetStarter() );
      } // end FGEngine code block

      switch (thruster->GetType()) {
      case FGThruster::ttNozzle:
        { // FGNozzle code block
        FGNozzle* noz = (FGNozzle*)thruster;
        } // end FGNozzle code block
        break;
      case FGThruster::ttPropeller:
        { // FGPropeller code block
        FGPropeller* prop = (FGPropeller*)thruster;
        tnode->setDoubleValue("rpm", thruster->GetRPM());
        tnode->setDoubleValue("pitch", prop->GetPitch());
        tnode->setDoubleValue("torque", prop->GetTorque());
        tnode->setBoolValue("feathered", prop->GetFeather());
        } // end FGPropeller code block
        break;
      case FGThruster::ttRotor:
        { // FGRotor code block
        FGRotor* rotor = (FGRotor*)thruster;
        } // end FGRotor code block
        break;
      case FGThruster::ttDirect:
        { // Direct code block
        } // end Direct code block
        break;
      }

    }

    // Copy the fuel levels from JSBSim if fuel
    // freeze not enabled.
    if ( ! Propulsion->GetFuelFreeze() ) {
      for (i = 0; i < Propulsion->GetNumTanks(); i++) {
        SGPropertyNode * node = fgGetNode("/consumables/fuel/tank", i, true);
        FGTank* tank = Propulsion->GetTank(i);
        double contents = tank->GetContents();
        double temp = tank->GetTemperature_degC();
        node->setDoubleValue("level-gal_us", contents/6.6);
        node->setDoubleValue("level-lb", contents);
        if (temp != -9999.0) node->setDoubleValue("temperature_degC", temp);
      }
    }

    update_gear();

    stall_warning->setDoubleValue( Aerodynamics->GetStallWarn() );

    elevator_pos_pct->setDoubleValue( FCS->GetDePos(ofNorm) );
    left_aileron_pos_pct->setDoubleValue( FCS->GetDaLPos(ofNorm) );
    right_aileron_pos_pct->setDoubleValue( FCS->GetDaRPos(ofNorm) );
    rudder_pos_pct->setDoubleValue( -1*FCS->GetDrPos(ofNorm) );
    flap_pos_pct->setDoubleValue( FCS->GetDfPos(ofNorm) );
    speedbrake_pos_pct->setDoubleValue( FCS->GetDsbPos(ofNorm) );
    spoilers_pos_pct->setDoubleValue( FCS->GetDspPos(ofNorm) );

    // force a sim reset if crashed (altitude AGL < 0)
    if (get_Altitude_AGL() < -100.0) {
         fgSetBool("/sim/crashed", true);
         SGPropertyNode* node = fgGetNode("/sim/presets", true);
         globals->get_commands()->execute("old-reinit-dialog", node);
    }

    return true;
}


bool FGJSBsim::ToggleDataLogging(void)
{
  // ToDo: handle this properly
  fdmex->DisableOutput();
  return false;
}


bool FGJSBsim::ToggleDataLogging(bool state)
{
    if (state) {
      fdmex->EnableOutput();
      return true;
    } else {
      fdmex->DisableOutput();
      return false;
    }
}


//Positions
void FGJSBsim::set_Latitude(double lat)
{
    static SGConstPropertyNode_ptr altitude = fgGetNode("/position/altitude-ft");
    double alt;
    double sea_level_radius_meters, lat_geoc;

    // In case we're not trimming
    FGInterface::set_Latitude(lat);

    if ( altitude->getDoubleValue() > -9990 ) {
      alt = altitude->getDoubleValue();
    } else {
      alt = 0.0;
    }

    update_ic();
    SG_LOG(SG_FLIGHT,SG_INFO,"FGJSBsim::set_Latitude: " << lat );
    SG_LOG(SG_FLIGHT,SG_INFO," cur alt (ft) =  " << alt );

    sgGeodToGeoc( lat, alt * SG_FEET_TO_METER,
                      &sea_level_radius_meters, &lat_geoc );
    _set_Sea_level_radius( sea_level_radius_meters * SG_METER_TO_FEET  );
    fgic->SetSeaLevelRadiusFtIC( sea_level_radius_meters * SG_METER_TO_FEET  );
    fgic->SetLatitudeRadIC( lat_geoc );
    needTrim=true;
}


void FGJSBsim::set_Longitude(double lon)
{
    SG_LOG(SG_FLIGHT,SG_INFO,"FGJSBsim::set_Longitude: " << lon );

    // In case we're not trimming
    FGInterface::set_Longitude(lon);

    update_ic();
    fgic->SetLongitudeRadIC( lon );
    needTrim=true;
}

void FGJSBsim::set_Altitude(double alt)
{
    static SGConstPropertyNode_ptr latitude = fgGetNode("/position/latitude-deg");

    double sea_level_radius_meters,lat_geoc;

    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Altitude: " << alt );
    SG_LOG(SG_FLIGHT,SG_INFO, "  lat (deg) = " << latitude->getDoubleValue() );

    // In case we're not trimming
    FGInterface::set_Altitude(alt);

    update_ic();
    sgGeodToGeoc( latitude->getDoubleValue() * SGD_DEGREES_TO_RADIANS, alt,
                  &sea_level_radius_meters, &lat_geoc);
    _set_Sea_level_radius( sea_level_radius_meters * SG_METER_TO_FEET  );
    fgic->SetSeaLevelRadiusFtIC( sea_level_radius_meters * SG_METER_TO_FEET );
    SG_LOG(SG_FLIGHT, SG_INFO,
          "Terrain altitude: " << cur_fdm_state->get_Runway_altitude() * SG_METER_TO_FEET );
    fgic->SetLatitudeRadIC( lat_geoc );
    fgic->SetAltitudeFtIC(alt);
    needTrim=true;
}

void FGJSBsim::set_V_calibrated_kts(double vc)
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_V_calibrated_kts: " <<  vc );

    // In case we're not trimming
    FGInterface::set_V_calibrated_kts(vc);

    update_ic();
    fgic->SetVcalibratedKtsIC(vc);
    needTrim=true;
}

void FGJSBsim::set_Mach_number(double mach)
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Mach_number: " <<  mach );

    // In case we're not trimming
    FGInterface::set_Mach_number(mach);

    update_ic();
    fgic->SetMachIC(mach);
    needTrim=true;
}

void FGJSBsim::set_Velocities_Local( double north, double east, double down )
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Velocities_Local: "
       << north << ", " <<  east << ", " << down );

    // In case we're not trimming
    FGInterface::set_Velocities_Local(north, east, down);

    update_ic();
    fgic->SetVnorthFpsIC(north);
    fgic->SetVeastFpsIC(east);
    fgic->SetVdownFpsIC(down);
    needTrim=true;
}

void FGJSBsim::set_Velocities_Wind_Body( double u, double v, double w)
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Velocities_Wind_Body: "
       << u << ", " <<  v << ", " <<  w );

    // In case we're not trimming
    FGInterface::set_Velocities_Wind_Body(u, v, w);

    update_ic();
    fgic->SetUBodyFpsIC(u);
    fgic->SetVBodyFpsIC(v);
    fgic->SetWBodyFpsIC(w);
    needTrim=true;
}

//Euler angles
void FGJSBsim::set_Euler_Angles( double phi, double theta, double psi )
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Euler_Angles: "
       << phi << ", " << theta << ", " << psi );

    // In case we're not trimming
    FGInterface::set_Euler_Angles(phi, theta, psi);

    update_ic();
    fgic->SetThetaRadIC(theta);
    fgic->SetPhiRadIC(phi);
    fgic->SetPsiRadIC(psi);
    needTrim=true;
}

//Flight Path
void FGJSBsim::set_Climb_Rate( double roc)
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Climb_Rate: " << roc );

    // In case we're not trimming
    FGInterface::set_Climb_Rate(roc);

    update_ic();
    //since both climb rate and flight path angle are set in the FG
    //startup sequence, something is needed to keep one from cancelling
    //out the other.
    if( !(fabs(roc) > 1 && fabs(fgic->GetFlightPathAngleRadIC()) < 0.01) ) {
      fgic->SetClimbRateFpsIC(roc);
    }
    needTrim=true;
}

void FGJSBsim::set_Gamma_vert_rad( double gamma)
{
    SG_LOG(SG_FLIGHT,SG_INFO, "FGJSBsim::set_Gamma_vert_rad: " << gamma );

    update_ic();
    if( !(fabs(gamma) < 0.01 && fabs(fgic->GetClimbRateFpsIC()) > 1) ) {
      fgic->SetFlightPathAngleRadIC(gamma);
    }
    needTrim=true;
}

void FGJSBsim::init_gear(void )
{
    FGGroundReactions* gr=fdmex->GetGroundReactions();
    int Ngear=GroundReactions->GetNumGearUnits();
    for (int i=0;i<Ngear;i++) {
      FGLGear *gear = gr->GetGearUnit(i);
      SGPropertyNode * node = fgGetNode("gear/gear", i, true);
      node->setDoubleValue("xoffset-in", gear->GetBodyLocation()(1));
      node->setDoubleValue("yoffset-in", gear->GetBodyLocation()(2));
      node->setDoubleValue("zoffset-in", gear->GetBodyLocation()(3));
      node->setBoolValue("wow", gear->GetWOW());
      node->setBoolValue("has-brake", gear->GetBrakeGroup() > 0);
      node->setDoubleValue("position-norm", FCS->GetGearPos());
      node->setDoubleValue("tire-pressure-norm", gear->GetTirePressure());
      node->setDoubleValue("compression-norm", gear->GetCompLen());
      if ( gear->GetSteerable() )
        node->setDoubleValue("steering-norm", gear->GetSteerNorm());
    }
}

void FGJSBsim::update_gear(void)
{
    FGGroundReactions* gr=fdmex->GetGroundReactions();
    int Ngear=GroundReactions->GetNumGearUnits();
    for (int i=0;i<Ngear;i++) {
      FGLGear *gear = gr->GetGearUnit(i);
      SGPropertyNode * node = fgGetNode("gear/gear", i, true);
      node->getChild("wow", 0, true)->setBoolValue( gear->GetWOW());
      node->getChild("position-norm", 0, true)->setDoubleValue(FCS->GetGearPos());
      gear->SetTirePressure(node->getDoubleValue("tire-pressure-norm"));
      node->setDoubleValue("compression-norm", gear->GetCompLen());
      if ( gear->GetSteerable() )
        node->setDoubleValue("steering-norm", gear->GetSteerNorm());
    }
}

void FGJSBsim::do_trim(void)
{
  FGTrim *fgtrim;

  if ( fgGetBool("/sim/presets/onground") )
  {
    fgic->SetVcalibratedKtsIC(0.0);
    fgtrim = new FGTrim(fdmex,tGround);
  } else {
    fgtrim = new FGTrim(fdmex,tLongitudinal);
  }

  if ( !fgtrim->DoTrim() ) {
    fgtrim->Report();
    fgtrim->TrimStats();
  } else {
    trimmed->setBoolValue(true);
  }
#if 0
  if (FGJSBBase::debug_lvl > 0)
      State->ReportState();	/* FIXME: Function not implemented */
#endif

  delete fgtrim;

  pitch_trim->setDoubleValue( FCS->GetPitchTrimCmd() );
  throttle_trim->setDoubleValue( FCS->GetThrottleCmd(0) );
  aileron_trim->setDoubleValue( FCS->GetDaCmd() );
  rudder_trim->setDoubleValue( FCS->GetDrCmd() );

  globals->get_controls()->set_elevator_trim(FCS->GetPitchTrimCmd());
  globals->get_controls()->set_elevator(FCS->GetDeCmd());
  globals->get_controls()->set_throttle(FGControls::ALL_ENGINES,
  FCS->GetThrottleCmd(0));

  globals->get_controls()->set_aileron(FCS->GetDaCmd());
  globals->get_controls()->set_rudder( FCS->GetDrCmd());

  SG_LOG( SG_FLIGHT, SG_INFO, "  Trim complete" );
}

void FGJSBsim::update_ic(void)
{
   if ( !needTrim ) {
     fgic->SetLatitudeRadIC(get_Lat_geocentric() );
     fgic->SetLongitudeRadIC( get_Longitude() );
     fgic->SetAltitudeFtIC( get_Altitude() );
     fgic->SetVcalibratedKtsIC( get_V_calibrated_kts() );
     fgic->SetThetaRadIC( get_Theta() );
     fgic->SetPhiRadIC( get_Phi() );
     fgic->SetPsiRadIC( get_Psi() );
     fgic->SetClimbRateFpsIC( get_Climb_Rate() );
   }
}

