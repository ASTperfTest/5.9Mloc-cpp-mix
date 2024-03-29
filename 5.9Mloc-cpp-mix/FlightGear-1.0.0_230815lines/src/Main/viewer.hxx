// viewer.hxx -- class for managing a viewer in the flightgear world.
//
// Written by Curtis Olson, started August 1997.
//                          overhaul started October 2000.
//   partially rewritten by Jim Wilson jim@kelcomaine.com using interface
//                          by David Megginson March 2002
//
// Copyright (C) 1997 - 2000  Curtis L. Olson  - http://www.flightgear.org/~curt
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
//
// $Id: viewer.hxx,v 1.16 2006-02-21 01:19:21 mfranz Exp $


#ifndef _VIEWER_HXX
#define _VIEWER_HXX


#ifndef __cplusplus                                                          
# error This library requires C++
#endif                                   

#include <simgear/compiler.h>
#include <simgear/constants.h>
#include <simgear/structure/subsystem_mgr.hxx>
#include <simgear/scene/model/location.hxx>

#include <plib/sg.h>		// plib include

#define FG_FOV_MIN 0.1
#define FG_FOV_MAX 179.9

enum fgViewType {
 FG_LOOKFROM = 0,
 FG_LOOKAT = 1
};

// Define a structure containing view information
class FGViewer : public SGSubsystem {

public:

    enum fgScalingType {  // nominal Field Of View actually applies to ...
	FG_SCALING_WIDTH,       // window width
	FG_SCALING_MAX          // max(width, height)
	// FG_SCALING_G_MEAN,      // geometric_mean(width, height)
	// FG_SCALING_INDEPENDENT  // whole screen
    };

    // Constructor
    FGViewer( fgViewType Type, bool from_model, int from_model_index,
              bool at_model, int at_model_index,
              double damp_roll, double damp_pitch, double damp_heading,
              double x_offset_m, double y_offset_m, double z_offset_m,
              double heading_offset_deg, double pitch_offset_deg,
              double roll_offset_deg,
              double fov_deg, double aspect_ratio_multiplier,
              double target_x_offset_m, double target_y_offset_m,
              double target_z_offset_m, double near_m, bool internal );

    // Destructor
    virtual ~FGViewer( void );

    //////////////////////////////////////////////////////////////////////
    // Part 1: standard SGSubsystem implementation.
    //////////////////////////////////////////////////////////////////////

    virtual void init ();
    virtual void bind ();
    virtual void unbind ();
    void update (double dt);


    //////////////////////////////////////////////////////////////////////
    // Part 2: user settings.
    //////////////////////////////////////////////////////////////////////

    virtual fgViewType getType() const { return _type; }
    virtual void setType( int type );

    virtual bool getInternal() const { return _internal; }
    virtual void setInternal( bool internal );

    // Reference geodetic position of view from position...
    //   These are the actual aircraft position (pilot in
    //   pilot view, model in model view).
    //   FIXME: the model view position (ie target positions) 
    //   should be in the model class.
    virtual double getLongitude_deg () const { return _lon_deg; }
    virtual double getLatitude_deg () const { return _lat_deg; }
    virtual double getAltitudeASL_ft () const { return _alt_ft; }
    virtual void setLongitude_deg (double lon_deg);
    virtual void setLatitude_deg (double lat_deg);
    virtual void setAltitude_ft (double alt_ft);
    virtual void setPosition (double lon_deg, double lat_deg, double alt_ft);

    // Reference geodetic target position...
    virtual double getTargetLongitude_deg () const { return _target_lon_deg; }
    virtual double getTargetLatitude_deg () const { return _target_lat_deg; }
    virtual double getTargetAltitudeASL_ft () const { return _target_alt_ft; }
    virtual void setTargetLongitude_deg (double lon_deg);
    virtual void setTargetLatitude_deg (double lat_deg);
    virtual void setTargetAltitude_ft (double alt_ft);
    virtual void setTargetPosition (double lon_deg, double lat_deg, double alt_ft);




    // Position offsets from reference
    //   These offsets position they "eye" in the scene according to a given
    //   location.  For example in pilot view they are used to position the 
    //   head inside the aircraft.
    //   Note that in pilot view these are applied "before" the orientation 
    //   rotations (see below) so that the orientation rotations have the 
    //   effect of the pilot staying in his seat and "looking out" in 
    //   different directions.
    //   In chase view these are applied "after" the application of the 
    //   orientation rotations listed below.  This has the effect of the 
    //   eye moving around and "looking at" the object (model) from 
    //   different angles.
    virtual double getXOffset_m () const { return _x_offset_m; }
    virtual double getYOffset_m () const { return _y_offset_m; }
    virtual double getZOffset_m () const { return _z_offset_m; }
    virtual double getTargetXOffset_m () const { return _target_x_offset_m; }
    virtual double getTargetYOffset_m () const { return _target_y_offset_m; }
    virtual double getTargetZOffset_m () const { return _target_z_offset_m; }
    virtual void setXOffset_m (double x_offset_m);
    virtual void setYOffset_m (double y_offset_m);
    virtual void setZOffset_m (double z_offset_m);
    virtual void setTargetXOffset_m (double x_offset_m);
    virtual void setTargetYOffset_m (double y_offset_m);
    virtual void setTargetZOffset_m (double z_offset_m);
    virtual void setPositionOffsets (double x_offset_m,
				     double y_offset_m,
				     double z_offset_m);




    // Reference orientation rotations...
    //   These are rotations that represent the plane attitude effect on
    //   the view (in Pilot view).  IE The view frustrum rotates as the plane
    //   turns, pitches, and rolls.
    //   In model view (lookat/chaseview) these end up changing the angle that
    //   the eye is looking at the ojbect (ie the model).
    //   FIXME: the FGModel class should have its own version of these so that
    //   it can generate it's own model rotations.
    virtual double getRoll_deg () const { return _roll_deg; }
    virtual double getPitch_deg () const {return _pitch_deg; }
    virtual double getHeading_deg () const {return _heading_deg; }
    virtual void setRoll_deg (double roll_deg);
    virtual void setPitch_deg (double pitch_deg);
    virtual void setHeading_deg (double heading_deg);
    virtual void setOrientation (double roll_deg, double pitch_deg, double heading_deg);
    virtual double getTargetRoll_deg () const { return _target_roll_deg; }
    virtual double getTargetPitch_deg () const {return _target_pitch_deg; }
    virtual double getTargetHeading_deg () const {return _target_heading_deg; }
    virtual void setTargetRoll_deg (double roll_deg);
    virtual void setTargetPitch_deg (double pitch_deg);
    virtual void setTargetHeading_deg (double heading_deg);
    virtual void setTargetOrientation (double roll_deg, double pitch_deg, double heading_deg);




    // Orientation offsets rotations from reference orientation.
    // Goal settings are for smooth transition from prior 
    // offset when changing view direction.
    //   These offsets are in ADDITION to the orientation rotations listed 
    //   above.
    //   In pilot view they are applied after the position offsets in order to
    //   give the effect of the pilot looking around.
    //   In lookat view they are applied before the position offsets so that
    //   the effect is the eye moving around looking at the object (ie the model)
    //   from different angles.
    virtual double getRollOffset_deg () const { return _roll_offset_deg; }
    virtual double getPitchOffset_deg () const { return _pitch_offset_deg; }
    virtual double getHeadingOffset_deg () const { return _heading_offset_deg; }
    virtual double getGoalRollOffset_deg () const { return _goal_roll_offset_deg; }
    virtual double getGoalPitchOffset_deg () const { return _goal_pitch_offset_deg; }
    virtual double getGoalHeadingOffset_deg () const {return _goal_heading_offset_deg; }
    virtual void setRollOffset_deg (double roll_offset_deg);
    virtual void setPitchOffset_deg (double pitch_offset_deg);
    virtual void setHeadingOffset_deg (double heading_offset_deg);
    virtual void setGoalRollOffset_deg (double goal_roll_offset_deg);
    virtual void setGoalPitchOffset_deg (double goal_pitch_offset_deg);
    virtual void setGoalHeadingOffset_deg (double goal_heading_offset_deg);
    virtual void setOrientationOffsets (double roll_offset_deg,
				     double heading_offset_deg,
				     double pitch_offset_deg);



    //////////////////////////////////////////////////////////////////////
    // Part 3: output vectors and matrices in FlightGear coordinates.
    //////////////////////////////////////////////////////////////////////

    // Vectors and positions...

    // Get zero view_pos
    virtual float * get_view_pos() {if ( _dirty ) { recalc(); }	return _view_pos; }
    // Get the absolute view position in fgfs coordinates.
    virtual double * get_absolute_view_pos ();
    // Get zero elev
    virtual float * get_zero_elev() {if ( _dirty ) { recalc(); } return _zero_elev; }
    // Get world up vector
    virtual float *get_world_up() {if ( _dirty ) { recalc(); } return _world_up; }
    // Get the relative (to scenery center) view position in fgfs coordinates.
    virtual float * getRelativeViewPos ();
    // Get surface east vector
    virtual float *get_surface_east() {	if ( _dirty ) { recalc(); } return _surface_east; }
    // Get surface south vector
    virtual float *get_surface_south() {if ( _dirty ) { recalc(); } return _surface_south; }

    // Matrices...
    virtual const sgVec4 *get_VIEW() { if ( _dirty ) { recalc(); } return VIEW; }
    virtual const sgVec4 *get_UP() { if ( _dirty ) { recalc(); } return UP; }

    //////////////////////////////////////////////////////////////////////
    // Part 4: View and frustrum data setters and getters
    //////////////////////////////////////////////////////////////////////

    virtual void set_fov( double fov_deg ) {
	_fov_deg = fov_deg;
    }
    virtual double get_fov() const { return _fov_deg; }
    virtual double get_h_fov();    // Get horizontal fov, in degrees.
    virtual double get_v_fov();    // Get vertical fov, in degrees.

    virtual void set_aspect_ratio( double r ) {
	_aspect_ratio = r;
    }
    virtual double get_aspect_ratio() const { return _aspect_ratio; }

    virtual void set_aspect_ratio_multiplier( double m ) {
	_aspect_ratio_multiplier = m;
    }
    virtual double get_aspect_ratio_multiplier() const {
        return _aspect_ratio_multiplier;
    }

    virtual double getNear_m () const { return _ground_level_nearplane_m; }
    inline void setNear_m (double near_m) {
        _ground_level_nearplane_m = near_m;
    }

    //////////////////////////////////////////////////////////////////////
    // Part 5: misc setters and getters
    //////////////////////////////////////////////////////////////////////

    inline void set_dirty() { _dirty = true; }
    inline void set_clean() { _dirty = false; }
    
    // return eye location...
    virtual SGLocation * getSGLocation () const { return _location; }


private:

    //////////////////////////////////////////////////////////////////
    // private data                                                 //
    //////////////////////////////////////////////////////////////////

    // flag forcing a recalc of derived view parameters
    bool _dirty;

    mutable sgdVec3 _absolute_view_pos;
    mutable sgVec3 _relative_view_pos;

    double _lon_deg;
    double _lat_deg;
    double _alt_ft;
    double _target_lon_deg;
    double _target_lat_deg;
    double _target_alt_ft;

    double _roll_deg;
    double _pitch_deg;
    double _heading_deg;
    double _target_roll_deg;
    double _target_pitch_deg;
    double _target_heading_deg;

    double _damp_sync;
    double _damp_roll;
    double _damp_pitch;
    double _damp_heading;

    double _damped_roll_deg;
    double _damped_pitch_deg;
    double _damped_heading_deg;

    // Position offsets from FDM origin.  The X axis is positive
    // out the tail, Y is out the right wing, and Z is positive up.
    // distance in meters
    double _x_offset_m;
    double _y_offset_m;
    double _z_offset_m;

    // Target offsets from FDM origin (for "lookat" targets) The X
    // axis is positive out the tail, Y is out the right wing, and Z
    // is positive up.  distance in meters
    double _target_x_offset_m;
    double _target_y_offset_m;
    double _target_z_offset_m;


    // orientation offsets from reference (_goal* are for smoothed transitions)
    double _roll_offset_deg;
    double _pitch_offset_deg;
    double _heading_offset_deg;
    double _goal_roll_offset_deg;
    double _goal_pitch_offset_deg;
    double _goal_heading_offset_deg;

    // used to set nearplane when at ground level for this view
    double _ground_level_nearplane_m;

    fgViewType _type;
    fgScalingType _scaling_type;

    // internal view (e.g. cockpit) flag
    bool _internal;

    // view is looking from a model
    bool _from_model;
    int _from_model_index;  // number of model (for multi model)

    // view is looking at a model
    bool _at_model;
    int _at_model_index;  // number of model (for multi model)

    SGLocation * _location;
    SGLocation * _target_location;

    // the nominal field of view (angle, in degrees)
    double _fov_deg; 

    // Ratio of window width and height; height = width *
    // aspect_ratio.  This value is automatically calculated based on
    // window dimentions.
    double _aspect_ratio;       

    // default = 1.0, this value is user configurable and is
    // multiplied into the aspect_ratio to get the actual vertical fov
    double _aspect_ratio_multiplier;

    bool _reverse_view_offset;

    // view position in opengl world coordinates (this is the
    // abs_view_pos translated to scenery.center)
    sgVec3 _view_pos;

    // cartesion coordinates of current lon/lat if at sea level
    // translated to scenery.center
    sgVec3 _zero_elev;

    // surface vector heading south
    sgVec3 _surface_south;

    // surface vector heading east (used to unambiguously align sky
    // with sun)
    sgVec3 _surface_east;

    // world up vector (normal to the plane tangent to the earth's
    // surface at the spot we are directly above
    sgVec3 _world_up;

    // up vector for the view (usually point straight up through the
    // top of the aircraft
    sgVec3 _view_up;

    // sg versions of our friendly matrices
    sgMat4 VIEW, UP;
    sgMat4 LOCAL, ATLOCAL, TRANS, LARC_TO_SSG;

    // Transformation matrix for the view direction offset relative to
    // the AIRCRAFT matrix
    sgMat4 VIEW_OFFSET;

    //////////////////////////////////////////////////////////////////
    // private functions                                            //
    //////////////////////////////////////////////////////////////////

    void recalc ();
    void recalcLookFrom();
    void recalcLookAt();
    void copyLocationData();
    void updateFromModelLocation (SGLocation * location);
    void updateAtModelLocation (SGLocation * location);
    void recalcOurOwnLocation (SGLocation * location, double lon_deg, double lat_deg, double alt_ft,
                 double roll_deg, double pitch_deg, double heading_deg);
    void dampEyeData (double &roll_deg, double &pitch_deg, double &heading_deg);

    // add to _heading_offset_deg
    inline void incHeadingOffset_deg( double amt ) {
	set_dirty();
	_heading_offset_deg += amt;
    }

    // add to _pitch_offset_deg
    inline void incPitchOffset_deg( double amt ) {
	set_dirty();
	_pitch_offset_deg += amt;
    }

    // add to _roll_offset_deg
    inline void incRollOffset_deg( double amt ) {
	set_dirty();
	_roll_offset_deg += amt;
    }

    inline void set_reverse_view_offset( bool val ) {
	_reverse_view_offset = val;
    }

};


#endif // _VIEWER_HXX
