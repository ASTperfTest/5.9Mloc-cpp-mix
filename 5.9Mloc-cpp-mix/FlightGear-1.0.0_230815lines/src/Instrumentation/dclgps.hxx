// dclgps.hxx - a class to extend the operation of FG's current GPS
// code, and provide support for a KLN89-specific instrument.  It
// is envisioned that eventually this file and class will be split
// up between current FG code and new KLN89-specific code and removed.
//
// Written by David Luff, started 2005.
//
// Copyright (C) 2005 - David C Luff:  daveluff --AT-- ntlworld --D0T-- com
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
// $Id: dclgps.hxx,v 1.5 2006-03-13 23:01:48 daveluff Exp $

#ifndef _DCLGPS_HXX
#define _DCLGPS_HXX

#include "render_area_2d.hxx"
#include <string>
#include <list>
#include <vector>
#include <map>

#include <Navaids/navrecord.hxx>
#include <Navaids/navlist.hxx>
#include <Navaids/fixlist.hxx>
#include <Airports/simple.hxx>
#include <simgear/structure/subsystem_mgr.hxx>

using namespace std;

enum GPSDistanceUnits {
	GPS_DIST_UNITS_NM = 0,
	GPS_DIST_UNITS_KM
};

enum GPSSpeedUnits {
	GPS_VEL_UNITS_KT,
	GPS_VEL_UNITS_KPH
};

enum GPSAltitudeUnits {
	GPS_ALT_UNITS_FT,
	GPS_ALT_UNITS_M
};

enum GPSPressureUnits {
	GPS_PRES_UNITS_IN = 1,
	GPS_PRES_UNITS_MB,
	GPS_PRES_UNITS_HP
};

// --------------------- Waypoint / Flightplan stuff -----------------------------
// This should be merged with other similar stuff in FG at some point.

// NOTE - ORDERING IS IMPORTANT HERE - it matches the Bendix-King page ordering!
enum GPSWpType {
	GPS_WP_APT = 0,
	GPS_WP_VOR,
	GPS_WP_NDB,
	GPS_WP_INT,
	GPS_WP_USR,
	GPS_WP_VIRT		// Used for virtual waypoints, such as the start of DTO operation.
};

enum GPSAppWpType {
	GPS_IAF,		// Initial approach fix
	GPS_IAP,		// Waypoint on approach sequence that isn't any of the others.
	GPS_FAF,		// Final approach fix
	GPS_MAP,		// Missed approach point
	GPS_MAHP,		// Initial missed approach holding point.
    GPS_HDR,        // A virtual 'waypoint' to represent the approach header in the fpl page
    GPS_FENCE,      // A virtual 'waypoint' to represent the NO WPT SEQ fence.
    GPS_APP_NONE    // Not part of the approach sequence - the default.
};

ostream& operator << (ostream& os, GPSAppWpType type);

struct GPSWaypoint {
    GPSWaypoint();
    ~GPSWaypoint();
	string GetAprId();	// Returns the id with i, f, m or h added if appropriate. (Initial approach fix, final approach fix, etc)
	string id;
	float lat;	// Radians
	float lon;	// Radians
	GPSWpType type;
	GPSAppWpType appType;	// only used for waypoints that are part of an approach sequence
};

typedef vector < GPSWaypoint* > gps_waypoint_array;
typedef gps_waypoint_array::iterator gps_waypoint_array_iterator;
typedef map < string, gps_waypoint_array > gps_waypoint_map;
typedef gps_waypoint_map::iterator gps_waypoint_map_iterator;
typedef gps_waypoint_map::const_iterator gps_waypoint_map_const_iterator;

class GPSFlightPlan {
public:
	vector<GPSWaypoint*> waypoints;
	inline bool IsEmpty() { return(waypoints.size() == 0); }
};

// TODO - probably de-public the internals of the next 2 classes and add some methods!
// Instrument approach procedure base class
class FGIAP {
public:
	FGIAP();
	virtual ~FGIAP() = 0;
//protected:

	string _id;		// The ID of the airport this approach is for
	string _name;	// The approach name, eg "VOR/DME OR GPS-B"
	string _abbrev;	// The abbreviation the GPS unit uses - eg "VOR/D" in this instance.  Possibly GPS model specific.
	string _rwyStr;	// The string used to specify the rwy - eg "B" in this instance.
	bool _precision;	// True for precision approach, false for non-precision.
};

// Non-precision instrument approach procedure
class FGNPIAP : public FGIAP {
public:
	FGNPIAP();
	~FGNPIAP();
//private:
public:
	vector<GPSWaypoint*> _IAF;	// The initial approach fix(es)
	vector<GPSWaypoint*> _IAP;	// The compulsory waypoints of the approach procedure (may duplicate one of the above).
								// _IAP includes the FAF and MAF.
	vector<GPSWaypoint*> _MAP;	// The missed approach procedure (doesn't include the MAF).
};

typedef vector < FGIAP* > iap_list_type;
typedef map < string, iap_list_type > iap_map_type;
typedef iap_map_type::iterator iap_map_iterator;

//	A class to encapsulate hr:min representation of time. 

class ClockTime {
public:
    ClockTime();
    ClockTime(int hr, int min);
    ~ClockTime();
    inline void set_hr(int hr) { _hr = hr; }
    inline int hr() const { return(_hr); } 
    inline void set_min(int min) { _min = min; }
    inline int min() const { return(_min); }
    
    ClockTime operator+ (const ClockTime& t) {
        int cumMin = _hr * 60 + _min + t.hr() * 60 + t.min();
        ClockTime t2(cumMin / 60, cumMin % 60);
        return(t2);
    }
    // Operator - has a max difference of 23:59,
    // and assumes the day has wrapped if the second operand
    // is larger that the first.
    // eg. 2:59 - 3:00 = 23:59
    ClockTime operator- (const ClockTime& t) {
        int diff = (_hr * 60 + _min) - (t.hr() * 60 + t.min());
        if(diff < 0) { diff += 24 * 60; }
        ClockTime t2(diff / 60, diff % 60);
        return(t2);
    }
    friend ostream& operator<< (ostream& out, const ClockTime& t);

private:
    int _hr;
    int _min;
};

// ------------------------------------------------------------------------------

class DCLGPS;

class GPSPage {
	
public:
	GPSPage(DCLGPS* parent);
	virtual ~GPSPage() = 0;
	virtual void Update(double dt);
	virtual void Knob1Left1();
	virtual void Knob1Right1();	
	virtual void Knob2Left1();
	virtual void Knob2Right1();	
	virtual void CrsrPressed();
	virtual void EntPressed();
	virtual void ClrPressed();
	virtual void DtoPressed();
	virtual void NrstPressed();
	virtual void AltPressed();
	virtual void OBSPressed();
	virtual void MsgPressed();
	
	// Sometimes a page needs to maintain state for some return paths,
	// but change it for others.  The CleanUp function can be used for
	// changing state for non-ENT return  paths in conjunction with
	// GPS::_cleanUpPage
	virtual void CleanUp();
	
	// The LooseFocus function is called when a page or subpage looses focus
	// and allows pages to clean up state that is maintained whilst focus is
	// retained, but lost on return.
	virtual void LooseFocus();
	
	// Allows pages that display info for a given ID to have it set/get if they implement these functions.
	virtual void SetId(const string& s);
	virtual const string& GetId()=0;
	
	inline int GetSubPage() { return(_subPage); }
	
	inline int GetNSubPages() { return(_nSubPages); }
	
	inline const string& GetName() { return(_name); }
	
protected:
	DCLGPS* _parent;
	string _name;	// eg. "APT", "NAV" etc
	int _nSubPages;
	// _subpage is zero based
	int _subPage;	// The subpage gets remembered when other pages are displayed
	string GPSitoa(int n);
};

/*-----------------------------------------------------------------------*/

typedef vector<GPSPage*> gps_page_list_type;
typedef gps_page_list_type::iterator gps_page_list_itr;

// TODO - merge generic GPS functions instead and split out KLN specific stuff.
class DCLGPS : public SGSubsystem {
	
	friend class GPSPage;
	
public:
	DCLGPS(RenderArea2D* instrument);
	virtual ~DCLGPS() = 0;
	
	virtual void draw();
	
	virtual void init();
	virtual void bind();
	virtual void unbind();
	virtual void update(double dt);

	// Render string s in display field field at position x, y
	// WHERE POSITION IS IN CHARACTER UNITS!
	// zero y at bottom?
	virtual void DrawText(const string& s, int field, int px, int py, bool bold = false);
	
	// Render a char at a given position as above
	virtual void DrawChar(char c, int field, int px, int py, bool bold = false);
	
	virtual void Knob1Right1();
	virtual void Knob1Left1();
	virtual void Knob2Right1();
	virtual void Knob2Left1();
	virtual void CrsrPressed();
	virtual void EntPressed();
	virtual void ClrPressed();
	virtual void DtoPressed();
	virtual void NrstPressed();
	virtual void AltPressed();
	virtual void OBSPressed();
	virtual void MsgPressed();
	
	// Set the number of fields
	inline void SetNumFields(int n) { _nFields = (n > _maxFields ? _maxFields : (n < 1 ? 1 : n)); }
	
	// Set Units
	// m if true, ft if false
	inline void SetAltUnitsSI(bool b) { _altUnits = (b ? GPS_ALT_UNITS_M : GPS_ALT_UNITS_FT); }
	// Returns true if alt units are SI (m), false if ft
	inline bool GetAltUnitsSI() { return(_altUnits == GPS_ALT_UNITS_M ? true : false); }
	// km and k/h if true, nm and kt if false
	inline void SetDistVelUnitsSI(bool b) { _distUnits = (b ? GPS_DIST_UNITS_KM : GPS_DIST_UNITS_NM); _velUnits = (b ? GPS_VEL_UNITS_KPH : GPS_VEL_UNITS_KT); }
	// Returns true if dist/vel units are SI
	inline bool GetDistVelUnitsSI() { return(_distUnits == GPS_DIST_UNITS_KM && _velUnits == GPS_VEL_UNITS_KPH ? true : false); }
	// Set baro units - 1 = in, 2 = mB, 3 = hP   Wrapping if for the convienience of the GPS setter.
	void SetBaroUnits(int n, bool wrap = false);
	// Get baro units: 1 = in, 2 = mB, 3 = hP
	inline int GetBaroUnits() { return((int)_baroUnits); }
	
	// It is expected that specific GPS units will override these functions.
	// Increase the CDI full-scale deflection (ie. increase the nm per dot) one (GPS unit dependent) increment.  Wraps if necessary (GPS unit dependent).
	virtual void CDIFSDIncrease();
	// Ditto for decrease the distance per dot
	virtual void CDIFSDDecrease();
	
	// Host specifc
	////inline void SetOverlays(Overlays* overlays) { _overlays = overlays; }
	
	virtual void CreateDefaultFlightPlans();
	
	void SetOBSFromWaypoint();
	
	inline GPSWaypoint* GetActiveWaypoint() { return &_activeWaypoint; }
	// Get the (zero-based) position of the active waypoint in the active flightplan
	// Returns -1 if no active waypoint.
	int GetActiveWaypointIndex();
	// Ditto for an arbitrary waypoint id
	int GetWaypointIndex(const string& id);
	
	// Returns meters
	inline float GetDistToActiveWaypoint() { return _dist2Act; }
	// Returns degrees (magnetic)
	float GetHeadingToActiveWaypoint();
	// Returns degrees (magnetic)
	float GetHeadingFromActiveWaypoint();
	// Get the time to the active waypoint in seconds.
	// Returns -1 if groundspeed < 30 kts
	double GetTimeToActiveWaypoint();
	// Get the time to the final waypoint in seconds.
	// Returns -1 if groundspeed < 30 kts
	double GetETE();
	// Get the time to a given waypoint (spec'd by ID) in seconds.
	// returns -1 if groundspeed is less than 30kts.
	// If the waypoint is an unreached part of the active flight plan the time will be via each leg.
	// otherwise it will be a direct-to time.
	double GetTimeToWaypoint(const string& id);
	
	// Return true if waypoint alerting is occuring
	inline bool GetWaypointAlert() const { return(_waypointAlert); }
	// Return true if in OBS mode
	inline bool GetOBSMode() const { return(_obsMode); }
	// Return true if in Leg mode
	inline bool GetLegMode() const { return(!_obsMode); }
	
	// Clear a flightplan
	void ClearFlightPlan(int n);
	void ClearFlightPlan(GPSFlightPlan* fp);
	
	// Returns true if an approach is loaded/armed/active in the active flight plan
	inline bool ApproachLoaded() const { return(_approachLoaded); }
	inline bool GetApproachArm() const { return(_approachArm); }
	inline bool GetApproachActive() const { return(_approachActive); }
	double GetCDIDeflection() const;
	inline bool GetToFlag() const { return(_headingBugTo); }
	
	// Initiate Direct To operation to the supplied ID.
	void DtoInitiate(const string& id);
	// Cancel Direct To operation
	void DtoCancel();
	
protected:
	// Maximum number of display fields for this device
	int _maxFields;
	// Current number of on-screen fields
	int _nFields;
	// Full x border
	int _xBorder;
	// Full y border
	int _yBorder;
	// Lower (y) border per field
	int _yFieldBorder[4];
	// Left (x) border per field
	int _xFieldBorder[4];
	// Field start in x dir (border is part of field since it is the normal char border - sometimes map mode etc draws in it)
	int _xFieldStart[4];
	// Field start in y dir (for completeness - KLN89 only has vertical divider.
	int _yFieldStart[4];
	
	// The number of pages on the cyclic knob control
	unsigned int _nPages;
	// The current page we're on (Not sure how this ties in with extra pages such as direct or nearest).
	unsigned int _curPage;
	
	// 2D rendering area
	RenderArea2D* _instrument;
	
	// The actual pages
	gps_page_list_type _pages;
	
	// The currently active page
	GPSPage* _activePage;
	// And a facility to save the immediately preceeding active page
	GPSPage* _lastActivePage;
	
	// Units
	GPSSpeedUnits _velUnits;
	GPSDistanceUnits _distUnits;
	GPSPressureUnits _baroUnits;
	GPSAltitudeUnits _altUnits;
	
	// CDI full-scale deflection, specified either as an index into a vector of values (standard values) or as a double precision float (intermediate values).
	// This will influence how an externally driven CDI will display as well as the NAV1 page.
	// Hence the variables are located here, not in the nav page class.
	vector<float> _cdiScales;
	unsigned int _currentCdiScaleIndex;
	bool _cdiScaleTransition;		// Set true when the floating CDI value is used during transitions
	double _currentCdiScale;	// The floating value to use.
	unsigned int _targetCdiScaleIndex;	// The target indexed value to attain during a transition.
	unsigned int _sourceCdiScaleIndex;	// The source indexed value during a transition - so we know which way we're heading!
	// Timers to handle the transitions - not sure if we need these.
	double _apprArmTimer;
	double _apprActvTimer;
	double _cdiTransitionTime;	// Time for transition to occur in - normally 30sec but may be quicker if time to FAF < 30sec?
	// 
	
	// Data and lookup functions
	// All waypoints mapped by id.
	gps_waypoint_map _waypoints;
private:
	// Worker function for the below.
	const GPSWaypoint* ActualFindFirstById(const string& id, bool exact = false);
protected:
	// Find first of any type of waypoint by id.  (TODO - Possibly we should return multiple waypoints here).
	const GPSWaypoint* FindFirstById(const string& id, bool &multi, bool exact = false); 
	FGNavRecord* FindFirstVorById(const string& id, bool &multi, bool exact = false);
	FGNavRecord* FindFirstNDBById(const string& id, bool &multi, bool exact = false);
	const FGAirport* FindFirstAptById(const string& id, bool &multi, bool exact = false);
	const FGFix* FindFirstIntById(const string& id, bool &multi, bool exact = false);
	// Find the closest VOR to a position in RADIANS.
	FGNavRecord* FindClosestVor(double lat_rad, double lon_rad);

	// Position, orientation and velocity.
	// These should be read from FG's built-in GPS logic if possible.
	// Use the property node pointers below to do this.
    SGPropertyNode_ptr _lon_node;
    SGPropertyNode_ptr _lat_node;
    SGPropertyNode_ptr _alt_node;
	SGPropertyNode_ptr _grnd_speed_node;
	SGPropertyNode_ptr _true_track_node;
	SGPropertyNode_ptr _mag_track_node;
	// Present position. (Radians)
	double _lat, _lon;
	// Present altitude (ft). (Yuk! but it saves converting ft->m->ft every update).
	double _alt;
	// Reported position as measured by GPS.  For now this is the same
	// as present position, but in the future we might want to model
	// GPS lat and lon errors.
	// Note - we can depriciate _gpsLat and _gpsLon if we implement error handling in FG
	// gps code and not our own.
	double _gpsLat, _gpsLon;  //(Radians)
	// Hack - it seems that the GPS gets initialised before FG's initial position is properly set.
	// By checking for abnormal slew in the position we can force a re-initialisation of active flight
	// plan leg and anything else that might be affected.
	// TODO - sort FlightGear's initialisation order properly!!!
	double _checkLat, _checkLon;	// (Radians)
	double _groundSpeed_ms;	// filtered groundspeed (m/s)
	double _groundSpeed_kts;	// ditto in knots
	double _track;			// filtered true track (degrees)
	double _magTrackDeg;	// magnetic track in degrees calculated from true track above
	
	// _navFlagged is set true when GPS navigation is either not possible or not logical.
	// This includes not receiving adequate signals, and not having an active flightplan entered.
	bool _navFlagged;
	
	// Positional functions copied from ATCutils that might get replaced
	// INPUT in RADIANS, returns DEGREES!
	// Magnetic
	double GetMagHeadingFromTo(double latA, double lonA, double latB, double lonB);
	// True
	//double GetHeadingFromTo(double latA, double lonA, double latB, double lonB);
	
	// Given two positions (lat & lon in RADIANS), get the HORIZONTAL separation (in meters)
	//double GetHorizontalSeparation(double lat1, double lon1, double lat2, double lon2);
	
	// Proper great circle positional functions from The Aviation Formulary
	// Returns distance in Nm, input in RADIANS.
	double GetGreatCircleDistance(double lat1, double lon1, double lat2, double lon2) const;
	
	// Input in RADIANS, output in DEGREES.
	// True
	double GetGreatCircleCourse(double lat1, double lon1, double lat2, double lon2) const;
	
	// Return a position on a radial from wp1 given distance d (nm) and magnetic heading h (degrees)
	// Note that d should be less that 1/4 Earth diameter!
	GPSWaypoint GetPositionOnMagRadial(const GPSWaypoint& wp1, double d, double h);
	
	// Return a position on a radial from wp1 given distance d (nm) and TRUE heading h (degrees)
	// Note that d should be less that 1/4 Earth diameter!
	GPSWaypoint GetPositionOnRadial(const GPSWaypoint& wp1, double d, double h);
	
	// Calculate the current cross-track deviation in nm.
	// Returns zero if a sensible value cannot be calculated.
	double CalcCrossTrackDeviation() const;
	
	// Calculate the cross-track deviation between 2 arbitrary waypoints in nm.
	// Returns zero if a sensible value cannot be calculated.
	double CalcCrossTrackDeviation(const GPSWaypoint& wp1, const GPSWaypoint& wp2) const;
	
	// Flightplans
	// GPS can have up to _maxFlightPlans flightplans stored, PLUS an active FP which may or my not be one of the stored ones.
	// This is from KLN89, but is probably not far off the mark for most if not all GPS.
	vector<GPSFlightPlan*> _flightPlans;
	unsigned int _maxFlightPlans;
	GPSFlightPlan* _activeFP;
	
	// Modes of operation.
	// This is currently somewhat Bendix-King specific, but probably applies fundamentally to other units as well
	// Mode defaults to leg, but is OBS if _obsMode is true.
	bool _obsMode;
	// _dto is set true for DTO operation
	bool _dto;
	// In leg mode, we need to know if we are displaying a from and to waypoint, or just the to waypoint (eg. when OBS mode is cancelled).
	bool _fullLegMode;
	// In OBS mode we need to know the set OBS heading
	int _obsHeading;
	
	// Operational variables
	GPSWaypoint _activeWaypoint;
	GPSWaypoint _fromWaypoint;
	float _dist2Act;
	float _crosstrackDist;	// UNITS ??????????
	double _eta;	// ETA in SECONDS to active waypoint.
	// Desired track for active leg, true and magnetic, in degrees
	double _dtkTrue, _dtkMag;
	bool _headingBugTo;		// Set true when the heading bug is TO, false when FROM.
	bool _waypointAlert;	// Set true when waypoint alerting is happening. (This is a variable NOT a user-setting).
	bool _departed;		// Set when groundspeed first exceeds 30kts.
	string _departureTimeString;	// Ditto.
	double _elapsedTime;	// Elapsed time in seconds since departure
	ClockTime _powerOnTime;		// Time (hr:min) of unit power-up.
	bool _powerOnTimerSet;		// Indicates that we have set the above following power-up.
	void SetPowerOnTimer();
public:
	void ResetPowerOnTimer();
	// Set the alarm to go off at a given time.
	inline void SetAlarm(int hr, int min) {
		_alarmTime.set_hr(hr);
		_alarmTime.set_min(min);
		_alarmSet = true;
	}
protected:
	ClockTime _alarmTime;
	bool _alarmSet;
	
	// Configuration that affects flightplan operation
	bool _turnAnticipationEnabled;
	
	// Configuration that affects general operation
	bool _suaAlertEnabled;		// Alert user to potential SUA entry
	bool _altAlertEnabled;		// Alert user to min safe alt violation
        
	// Magvar stuff.  Might get some of this stuff (such as time) from FG in future.
	SGTime* _time;
	
	list<string> _messageStack;
	
	virtual void CreateFlightPlan(GPSFlightPlan* fp, vector<string> ids, vector<GPSWpType> wps);
	
	// Orientate the GPS unit to a flightplan - ie. figure out from current position
	// and possibly orientation which leg of the FP we are on.
	virtual void OrientateToFlightPlan(GPSFlightPlan* fp);
	
	// Ditto for active fp.  Probably all we need really!
	virtual void OrientateToActiveFlightPlan();
	
	int _cleanUpPage;	// -1 => no cleanup required.
	
	// IAP stuff
	iap_map_type _np_iap;	// Non-precision approaches
	iap_map_type _pr_iap;	// Precision approaches
	bool _approachLoaded;	// Set true when an approach is loaded in the active flightplan
	bool _approachArm;		// Set true when in approach-arm mode
	bool _approachReallyArmed;	// Apparently, approach-arm mode can be set from an external GPS-APR switch outside 30nm from airport,
								// but the CDI scale change doesn't happen until 30nm from airport.  Bizarre that it can be armed without
								// the scale change, but it's in the manual...
	bool _approachActive;	// Set true when in approach-active mode
	GPSFlightPlan* _approachFP;	// Current approach - not necessarily loaded.
	string _approachID;		// ID of the airport we have an approach loaded for - bit of a hack that can hopefully be removed in future.
	// More hackery since we aren't actually storing an approach class... Doh!
	string _approachAbbrev;
	string _approachRwyStr;
};

#endif  // _DCLGPS_HXX
