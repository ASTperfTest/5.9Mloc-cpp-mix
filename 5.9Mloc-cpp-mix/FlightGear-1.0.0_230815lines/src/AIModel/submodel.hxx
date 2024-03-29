// submodel.hxx - models a releasable submodel.
// Written by Dave Culp, started Aug 2004
//
// This file is in the Public Domain and comes with no warranty.


#ifndef __SYSTEMS_SUBMODEL_HXX
#define __SYSTEMS_SUBMODEL_HXX 1

#ifndef __cplusplus
# error This library requires C++
#endif

#include <simgear/props/props.hxx>
#include <simgear/structure/subsystem_mgr.hxx>
#include <AIModel/AIBase.hxx>
#include <vector>
#include <string>

#include <Main/fg_props.hxx>

SG_USING_STD(vector);
SG_USING_STD(string);
SG_USING_STD(list);

class FGAIBase;

class FGSubmodelMgr : public SGSubsystem
{

public:

    typedef struct {
        SGPropertyNode_ptr trigger_node;
        SGPropertyNode_ptr prop;
        SGPropertyNode_ptr contents_node;
        SGPropertyNode_ptr submodel_node;
        SGPropertyNode_ptr speed_node;

        string             name;
        string             model;
        double             speed;
        bool               slaved;
        bool               repeat;
        double             delay;
        double             timer;
        int                count;
        double             x_offset;
        double             y_offset;
        double             z_offset;
        double             yaw_offset;
        double             pitch_offset;
        double             drag_area;
        double             life;
        double             buoyancy;
        bool               wind;
        bool               first_time;
        double             cd;
        double             weight;
        double             contents;
        bool               aero_stabilised;
        int                id;
        bool               no_roll;
        bool               serviceable;
        bool               collision;
        bool               impact;
        string             impact_report;
        double             fuse_range;
        string             submodel;
        int                sub_id;
    }   submodel;

    typedef struct {
        double     lat;
        double     lon;
        double     alt;
        double     roll;
        double     azimuth;
        double     elevation;
        double     speed;
        double     wind_from_east;
        double     wind_from_north;
        double     speed_down_fps;
        double     speed_east_fps;
        double     speed_north_fps;
        double     total_speed_down;
        double     total_speed_east;
        double     total_speed_north;
        double     mass;
        int        id;
        bool       no_roll;
    }   IC_struct;

    FGSubmodelMgr();
    ~FGSubmodelMgr();

    void load();
    void init();
    void postinit();
    void bind();
    void unbind();
    void update(double dt);
    void updatelat(double lat);

private:

    typedef vector <submodel*> submodel_vector_type;
    typedef submodel_vector_type::iterator submodel_vector_iterator;

    submodel_vector_type       submodels;
    submodel_vector_type       subsubmodels;
    submodel_vector_iterator   submodel_iterator, subsubmodel_iterator;

    float trans[3][3];
    float in[3];
    float out[3];

    double Rx, Ry, Rz;
    double Sx, Sy, Sz;
    double Tx, Ty, Tz;

    float cosRx, sinRx;
    float cosRy, sinRy;
    float cosRz, sinRz;

    int index;

    double ft_per_deg_longitude;
    double ft_per_deg_latitude;

    double x_offset, y_offset, z_offset;
    double pitch_offset, yaw_offset;

    double _parent_lat;
    double _parent_lon;
    double _parent_elev;
    double _parent_hdg;
    double _parent_pitch;
    double _parent_roll;
    double _parent_speed;

    static const double lbs_to_slugs; //conversion factor

    double contrail_altitude;

    bool _impact;
    bool _hit;

    SGPropertyNode_ptr _serviceable_node;
    SGPropertyNode_ptr _user_lat_node;
    SGPropertyNode_ptr _user_lon_node;
    SGPropertyNode_ptr _user_heading_node;
    SGPropertyNode_ptr _user_alt_node;
    SGPropertyNode_ptr _user_pitch_node;
    SGPropertyNode_ptr _user_roll_node;
    SGPropertyNode_ptr _user_yaw_node;
    SGPropertyNode_ptr _user_alpha_node;
    SGPropertyNode_ptr _user_speed_node;
    SGPropertyNode_ptr _user_wind_from_east_node;
    SGPropertyNode_ptr _user_wind_from_north_node;
    SGPropertyNode_ptr _user_speed_down_fps_node;
    SGPropertyNode_ptr _user_speed_east_fps_node;
    SGPropertyNode_ptr _user_speed_north_fps_node;
    SGPropertyNode_ptr _contrail_altitude_node;
    SGPropertyNode_ptr _contrail_trigger;
    SGPropertyNode_ptr _count_node;
    SGPropertyNode_ptr _trigger_node;
    SGPropertyNode_ptr props;

    FGAIManager* ai;
    IC_struct  IC;

    // A list of pointers to AI objects
    typedef list <SGSharedPtr<FGAIBase> > sm_list_type;
    typedef sm_list_type::iterator sm_list_iterator;
    typedef sm_list_type::const_iterator sm_list_const_iterator;

    sm_list_type sm_list;


    void loadAI();
    void loadSubmodels();
    void setData(int id, string& path, bool serviceable);
    void setSubData(int id, string& path, bool serviceable);
    void valueChanged (SGPropertyNode *);
    void transform(submodel *);

    bool release(submodel *, double dt);

    double getRange(double lat, double lon, double lat2, double lon2) const;

    int _count;

};

#endif // __SYSTEMS_SUBMODEL_HXX
