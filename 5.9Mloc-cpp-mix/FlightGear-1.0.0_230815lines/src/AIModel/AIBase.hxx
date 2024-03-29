// FGAIBase.hxx - abstract base class for AI objects
// Written by David Culp, started Nov 2003, based on
// David Luff's FGAIEntity class.
// - davidculp2@comcast.net
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

#ifndef _FG_AIBASE_HXX
#define _FG_AIBASE_HXX

#include <string>
#include <list>

#include <simgear/constants.h>
#include <simgear/math/SGMath.hxx>
#include <simgear/math/point3d.hxx>
#include <simgear/scene/model/placement.hxx>
#include <simgear/misc/sg_path.hxx>
#include <simgear/structure/ssgSharedPtr.hxx>
#include <simgear/structure/SGReferenced.hxx>

#include <Main/fg_props.hxx>


SG_USING_STD(string);
SG_USING_STD(list);

class FGAIManager;
class FGAIFlightPlan;

class FGAIBase : public SGReferenced {

public:
    enum object_type { otNull = 0, otAircraft, otShip, otCarrier, otBallistic,
                       otRocket, otStorm, otThermal, otStatic, otMultiplayer,
                       MAX_OBJECTS };	// Needs to be last!!!

    FGAIBase(object_type ot);
    virtual ~FGAIBase();

    virtual void readFromScenario(SGPropertyNode* scFileNode);

    virtual bool init(bool search_in_AI_path=false);
    virtual void update(double dt);
    virtual void bind();
    virtual void unbind();
    virtual void reinit() {}

    void setManager(FGAIManager* mgr, SGPropertyNode* p);
    void setPath( const char* model );
    void setSMPath( const string& p );
    void setCallSign( const string& );
    void setSpeed( double speed_KTAS );
    void setAltitude( double altitude_ft );
    void setHeading( double heading );
    void setLatitude( double latitude );
    void setLongitude( double longitude );
    void setBank( double bank );
    void setPitch( double newpitch );
    void setRadius ( double radius );
    void setXoffset( double x_offset );
    void setYoffset( double y_offset );
    void setZoffset( double z_offset );
    void setServiceable ( bool serviceable );
    void setDie( bool die );
    void setCollisionData( bool i, double lat, double lon, double elev );
    void setImpactData( bool d );
    void setImpactLat( double lat );
    void setImpactLon( double lon );
    void setImpactElev( double e );

    int getID() const;
    int _getSubID() const;

    bool getDie();

    SGVec3d getCartPosAt(const SGVec3d& off) const;
    SGVec3d getCartPos() const;

    double _getCartPosX() const;
    double _getCartPosY() const;
    double _getCartPosZ() const;

    string _path;
    string _callsign;
    string _submodel;
    string _name;

protected:

    SGPropertyNode_ptr props;
    SGPropertyNode_ptr trigger_node;
    SGPropertyNode_ptr model_removed; // where to report model removal
    FGAIManager* manager;

    // these describe the model's actual state
    SGGeod pos;	// WGS84 lat & lon in degrees, elev above sea-level in meters
    double hdg;		// True heading in degrees
    double roll;	// degrees, left is negative
    double pitch;	// degrees, nose-down is negative
    double speed;       // knots true airspeed
    double altitude_ft; // feet above sea level
    double vs;          // vertical speed, feet per minute
    double speed_north_deg_sec;
    double speed_east_deg_sec;
    double turn_radius_ft; // turn radius ft at 15 kts rudder angle 15 degrees

    double ft_per_deg_lon;
    double ft_per_deg_lat;

    // these describe the model's desired state
    double tgt_heading;     // target heading, degrees true
    double tgt_altitude_ft; // target altitude, *feet* above sea level
    double tgt_speed;       // target speed, KTAS
    double tgt_roll;
    double tgt_pitch;
    double tgt_yaw;
    double tgt_vs;

    // these describe radar information for the user
    bool in_range;       // true if in range of the radar, otherwise false
    double bearing;      // true bearing from user to this model
    double elevation;    // elevation in degrees from user to this model
    double range;        // range from user to this model, nm
    double rdot;         // range rate, in knots
    double horiz_offset; // look left/right from user to me, deg
    double vert_offset;  // look up/down from user to me, deg
    double x_shift;      // value used by radar display instrument
    double y_shift;      // value used by radar display instrument
    double rotation;     // value used by radar display instrument
    double ht_diff;		 // value used by radar display instrument

    string model_path;	   //Path to the 3D model
    ssgSharedPtr<ssgBranch> model; //The 3D model object
    SGModelPlacement aip;

    bool delete_me;
    bool invisible;
    bool no_roll;
    bool serviceable;
    int _subID;

    double life;

    FGAIFlightPlan *fp;

    bool _impact_reported;
    bool _collision_reported;

    double _impact_lat;
    double _impact_lon;
    double _impact_elev;
    double _impact_hdg;
    double _impact_pitch;
    double _impact_roll;
    double _impact_speed;

    void Transform();
    void CalculateMach();
    double UpdateRadar(FGAIManager* manager);

    static int _newAIModelID();

private:
    int _refID;
    object_type _otype;
    bool _initialized;

public:
    object_type getType();

    virtual const char* getTypeString(void) const { return "null"; }

    bool isa( object_type otype );

    void _setVS_fps( double _vs );
    void _setAltitude( double _alt );
    void _setLongitude( double longitude );
    void _setLatitude ( double latitude );
    void _setSubID( int s );

    double _getVS_fps() const;
    double _getAltitude() const;
    double _getLongitude() const;
    double _getLatitude() const;
    double _getBearing() const;
    double _getElevationFt() const;
    double _getRdot() const;
    double _getH_offset() const;
    double _getV_offset() const;
    double _getX_shift() const;
    double _getY_shift() const;
    double _getRotation() const;
    double _getSpeed() const;
    double _getRoll() const;
    double _getPitch() const;
    double _getHeading() const;
    double _get_speed_east_fps() const;
    double _get_speed_north_fps() const;
    double _get_SubPath() const;
    double _getImpactLat() const;
    double _getImpactLon() const;
    double _getImpactElevFt() const;
    double _getImpactHdg() const;
    double _getImpactPitch() const;
    double _getImpactRoll() const;
    double _getImpactSpeed() const;

    //unsigned int _getCount() const;

    bool   _getServiceable() const;
    bool   _getFirstTime() const;
    bool   _getImpact();
    bool   _getImpactData();
    bool   _getCollisionData();

    SGPropertyNode* _getProps() const;

    const char* _getPath() const;
    const char* _getSMPath() const;
    const char* _getCallsign() const;
    const char* _getTriggerNode() const;
    const char* _getName() const;
    const char* _getSubmodel() const;


    // These are used in the Mach number calculations

    double rho;
    double T;                             // temperature, degs farenheit
    double p;                             // pressure lbs/sq ft
    double a;                             // speed of sound at altitude (ft/s)
    double Mach;                          // Mach number

    static const double e;
    static const double lbs_to_slugs;

    inline double _getRange() { return range; };
    inline double _getBearing() { return bearing; };

    ssgBranch * load3DModel(const string& fg_root,
                            const string &path,
                            SGPropertyNode *prop_root,
                            double sim_time_sec);

    static bool _isNight();
};

inline void FGAIBase::setManager(FGAIManager* mgr, SGPropertyNode* p) {
    manager = mgr;
    props = p;
}

inline void FGAIBase::setPath(const char* model ) {
    model_path.append(model);
}

inline void FGAIBase::setSMPath(const string& p) {
    _path = p;
}

inline void FGAIBase::setServiceable(bool s) {
    serviceable = s;
}

inline void FGAIBase::setSpeed( double speed_KTAS ) {
    speed = tgt_speed = speed_KTAS;
}

inline void FGAIBase::setRadius( double radius ) {
    turn_radius_ft = radius;
}

inline void FGAIBase::setHeading( double heading ) {
    hdg = tgt_heading = heading;
}

inline void FGAIBase::setAltitude( double alt_ft ) {
    altitude_ft = tgt_altitude_ft = alt_ft;
    pos.setElevationFt(altitude_ft);
}

inline void FGAIBase::setBank( double bank ) {
    roll = tgt_roll = bank;
    no_roll = false;
}

inline void FGAIBase::setPitch( double newpitch ) {
    pitch = tgt_pitch = newpitch;
}

inline void FGAIBase::setLongitude( double longitude ) {
    pos.setLongitudeDeg( longitude );
}

inline void FGAIBase::setLatitude ( double latitude ) {
    pos.setLatitudeDeg( latitude );
}

inline void FGAIBase::setCallSign(const string& s) {
    _callsign = s;
}


inline void FGAIBase::setDie( bool die ) { delete_me = die; }

inline bool FGAIBase::getDie() { return delete_me; }

inline FGAIBase::object_type FGAIBase::getType() { return _otype; }

#endif	// _FG_AIBASE_HXX
