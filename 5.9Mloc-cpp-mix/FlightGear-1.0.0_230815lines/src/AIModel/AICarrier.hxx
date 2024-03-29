// FGAICarrier - AIShip-derived class creates an AI aircraft carrier
//
// Written by David Culp, started October 2004.
//
// Copyright (C) 2004  David P. Culp - davidculp2@comcast.net
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef _FG_AICARRIER_HXX
#define _FG_AICARRIER_HXX

#include <string>
#include <list>
#include <plib/ssg.h>
#include <simgear/compiler.h>

SG_USING_STD(string);
SG_USING_STD(list);

#include "AIShip.hxx"

#include "AIManager.hxx"
#include "AIBase.hxx"

class FGAIManager;
class FGAICarrier;

class FGAICarrierHardware : public ssgBase {
public:

    enum Type { Catapult, Wire, Solid };

    FGAICarrier *carrier;
    int id;
    Type type;

    static FGAICarrierHardware* newCatapult(FGAICarrier *c) {
        FGAICarrierHardware* ch = new FGAICarrierHardware;
        ch->carrier = c;
        ch->type = Catapult;
        ch->id = unique_id++;
        return ch;
    }
    static FGAICarrierHardware* newWire(FGAICarrier *c) {
        FGAICarrierHardware* ch = new FGAICarrierHardware;
        ch->carrier = c;
        ch->type = Wire;
        ch->id = unique_id++;
        return ch;
    }
    static FGAICarrierHardware* newSolid(FGAICarrier *c) {
        FGAICarrierHardware* ch = new FGAICarrierHardware;
        ch->carrier = c;
        ch->type = Solid;
        ch->id = unique_id++;
        return ch;
    }

private:
    static int unique_id;
};


class FGAICarrier  : public FGAIShip {
public:

    FGAICarrier();
    virtual ~FGAICarrier();

    virtual void readFromScenario(SGPropertyNode* scFileNode);

    void setSign(const string& );
    void setTACANChannelID(const string &);

    void getVelocityWrtEarth(sgdVec3& v, sgdVec3& omega, sgdVec3& pivot);
    virtual void bind();
    virtual void unbind();
    void UpdateWind ( double dt );
    void setWind_from_east( double fps );
    void setWind_from_north( double fps );
    void setMaxLat( double deg );
    void setMinLat( double deg );
    void setMaxLong( double deg );
    void setMinLong( double deg );
    void TurnToLaunch();
    void TurnToBase();
    void ReturnToBox();
    float Horizon(float h);
    bool OutsideBox();

    bool init(bool search_in_AI_path=false);

    virtual const char* getTypeString(void) const { return "carrier"; }

    bool getParkPosition(const string& id, SGGeod& geodPos,
                         double& hdng, SGVec3d& uvw);

private:
  /// Is sufficient to be private, stores a possible position to place an
  /// aircraft on start
  struct ParkPosition {
    ParkPosition(const ParkPosition& pp)
      : name(pp.name), offset(pp.offset), heading_deg(pp.heading_deg)
    {}
    ParkPosition(const string& n, const SGVec3d& off = SGVec3d(), double heading = 0)
      : name(n), offset(off), heading_deg(heading)
    {}
    string name;
    SGVec3d offset;
    double heading_deg;
  };


    void update(double dt);
    void mark_nohot(ssgEntity*);

    bool mark_wires(ssgEntity*, const list<string>&, bool = false);
    bool mark_cat(ssgEntity*, const list<string>&, bool = false);
    bool mark_solid(ssgEntity*, const list<string>&, bool = false);
    double wind_from_east;  // fps
    double wind_from_north; // fps
    double rel_wind_speed_kts;
    double rel_wind_from_deg;


    list<string> solid_objects;       // List of solid object names
    list<string> wire_objects;        // List of wire object names
    list<string> catapult_objects;    // List of catapult object names
    list<ParkPosition> ppositions;    // List of positions where an aircraft can start.
    string sign;                      // The sign of this carrier.

    // Velocity wrt earth.
    SGVec3d vel_wrt_earth;
    SGVec3d rot_wrt_earth;
    SGVec3d rot_pivot_wrt_earth;

    // these describe the flols
    SGVec3d flols_off;

    double dist;            // the distance of the eyepoint from the flols
    double angle;
    int source;             // the flols light which is visible at the moment
    bool wave_off_lights;

    // these are for maneuvering the carrier
    SGGeod mOpBoxPos;

    double wind_speed_from_north_kts ;
    double wind_speed_from_east_kts  ;
    double wind_speed_kts;  //true wind speed
    double wind_from_deg;   //true wind direction
    double rel_wind;
    double max_lat, min_lat, max_long, min_long;
    double base_course, base_speed;

    bool turn_to_launch_hdg;
    bool returning;      // set if the carrier is returning to an operating box
    bool InToWind();     // set if the carrier is in to wind


    SGPropertyNode_ptr _longitude_node;
    SGPropertyNode_ptr _latitude_node;
    SGPropertyNode_ptr _altitude_node;
    SGPropertyNode_ptr _surface_wind_from_deg_node;
    SGPropertyNode_ptr _surface_wind_speed_node;
    SGPropertyNode_ptr _launchbar_state_node;
    // this is for tacan

    string TACAN_channel_id;

    // these are for moving the elevators
    void UpdateElevator( double dt, double transition_time);
    double pos_norm, raw_pos_norm;
    double transition_time, time_constant;
    bool elevators;

    // these are for moving the jet blast deflectors
    void UpdateJBD( double dt, double jbd_transition_time);
    double jbd_pos_norm, raw_jbd_pos_norm;
    double jbd_transition_time, jbd_time_constant;
    bool jbd;
    bool _initialized;
};

#endif  // _FG_AICARRIER_HXX
