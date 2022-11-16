// HUD_runway.cxx -- An instrument that renders a virtual runway on the HUD
//
// Written by Aaron Wilson & Phillip Merritt, Nov 2004.
//
// Copyright (C) 2004 Aaron Wilson, Aaron.I.Wilson@nasa.gov
// Copyright (C) 2004 Phillip Merritt, Phillip.M.Merritt@nasa.gov
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <simgear/compiler.h>
#include <simgear/math/sg_geodesy.hxx>
#include <simgear/math/polar3d.hxx>
#include SG_GLU_H

#include <Main/globals.hxx>
#include <Scenery/scenery.hxx>
#include <Aircraft/aircraft.hxx>
#include <Environment/environment.hxx>
#include <Environment/environment_mgr.hxx>
#include <Main/viewer.hxx>
#include <Main/viewmgr.hxx>
#include <ATC/ATCutils.hxx>

#include "HUD.hxx"


HUD::Runway::Runway(HUD *hud, const SGPropertyNode *node, float x, float y) :
    Item(hud, node, x, y),
    _agl(fgGetNode("/position/altitude-agl-ft", true)),
    _arrow_scale(node->getDoubleValue("arrow-scale", 1.0)),
    _arrow_radius(node->getDoubleValue("arrow-radius")),
    _line_scale(node->getDoubleValue("line-scale", 1.0)),
    _scale_dist(node->getDoubleValue("scale-dist-nm")),
    _default_pitch(fgGetDouble("/sim/view[0]/config/pitch-pitch-deg", 0.0)),
    _default_heading(fgGetDouble("/sim/view[0]/config/pitch-heading-deg", 0.0)),
    _cockpit_view(globals->get_viewmgr()->get_view(0)),
    _stipple_out(node->getIntValue("outer_stipple", 0xFFFF)),
    _stipple_center(node->getIntValue("center-stipple", 0xFFFF)),
    _draw_arrow(_arrow_scale > 0 ? true : false),
    _draw_arrow_always(_arrow_scale > 0 ? node->getBoolValue("arrow-always") : false)
{
    _view[0] = 0;
    _view[1] = 0;
    _view[2] = 640;
    _view[3] = 480;

    _center_x = _view[2] / 2;
    _center_y = _view[3] / 2;

    _left = _center_x - (_w / 2) + _x;
    _right = _center_x + (_w / 2) + _x;
    _bottom = _center_y - (_h / 2) + _y;
    _top = _center_y + (_h / 2) + _y;
}


void HUD::Runway::draw()
{
    if (!get_active_runway(_runway))
        return;

    glPushAttrib(GL_LINE_STIPPLE | GL_LINE_STIPPLE_PATTERN | GL_LINE_WIDTH);
    float modelView[4][4], projMat[4][4];
    bool anyLines;
    //Get the current view
    FGViewer* curr_view = globals->get_viewmgr()->get_current_view();
    int curr_view_id = globals->get_viewmgr()->get_current();
    double gpo = curr_view->getGoalPitchOffset_deg();
    double gho = curr_view->getGoalHeadingOffset_deg();
    double po = curr_view->getPitchOffset_deg();
    double ho = curr_view->getHeadingOffset_deg();

    double yaw = -(_cockpit_view->getHeadingOffset_deg() - _default_heading) * SG_DEGREES_TO_RADIANS;
    double pitch = (_cockpit_view->getPitchOffset_deg() - _default_pitch) * SG_DEGREES_TO_RADIANS;
    //double roll = fgGetDouble("/sim/view[0]/config/roll-offset-deg",0.0) //TODO: adjust for default roll offset
    double sPitch = sin(pitch), cPitch = cos(pitch),
           sYaw = sin(yaw), cYaw = cos(yaw);

    //Assuming that the "Cockpit View" is always at position zero!!!
    if (curr_view_id != 0) {
        globals->get_viewmgr()->set_view(0);
        globals->get_viewmgr()->copyToCurrent();
    }
    //Set the camera to the cockpit view to get the view of the runway from the cockpit
    ssgSetCamera((sgVec4 *)_cockpit_view->get_VIEW());
    get_rwy_points(_points3d);
    //Get the current project matrix
    ssgGetProjectionMatrix(projMat);
//    const sgVec4 *viewMat = globals->get_current_view()->get_VIEW();
    //Get the current model view matrix (cockpit view)
    ssgGetModelviewMatrix(modelView);
    //Create a rotation matrix to correct for any offsets (other than default offsets) to the model view matrix
    sgMat4 xy; //rotation about the Rxy, negate the sin's on Ry
    xy[0][0] = cYaw,         xy[1][0] = 0.0f,   xy[2][0] = -sYaw,        xy[3][0] = 0.0f;
    xy[0][1] = sPitch*-sYaw, xy[1][1] = cPitch, xy[2][1] = -sPitch*cYaw, xy[3][1] = 0.0f;
    xy[0][2] = cPitch*sYaw,  xy[1][2] = sPitch, xy[2][2] = cPitch*cYaw,  xy[3][2] = 0.0f;
    xy[0][3] = 0.0f,         xy[1][3] = 0.0f,   xy[2][3] = 0.0f,         xy[3][3] = 1.0f;
    //Re-center the model view
    sgPostMultMat4(modelView,xy);
    //copy float matrices to double
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int idx = (i * 4) + j;
            _mm[idx] = (double)modelView[i][j];
            _pm[idx] = (double)projMat[i][j];
        }
    }

    //Calculate the 2D points via gluProject
    int result = GL_TRUE;
    for (int i = 0; i < 6; i++) {
        result = gluProject(_points3d[i][0], _points3d[i][1], _points3d[i][2], _mm,
                _pm, _view, &_points2d[i][0], &_points2d[i][1], &_points2d[i][2]);
    }
    //set the line width based on our distance from the runway
    setLineWidth();
    //Draw the runway lines on the HUD
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, _stipple_out);
    anyLines =
            drawLine(_points3d[0], _points3d[1], _points2d[0], _points2d[1]) | //draw top
            drawLine(_points3d[2], _points3d[1], _points2d[2], _points2d[1]) | //draw right
            drawLine(_points3d[2], _points3d[3], _points2d[2], _points2d[3]) | //draw bottom
            drawLine(_points3d[3], _points3d[0], _points2d[3], _points2d[0]);  //draw left

    glLineStipple(1, _stipple_center);
    anyLines |= drawLine(_points3d[5], _points3d[4], _points2d[5], _points2d[4]); //draw center

    //Check to see if arrow needs drawn
    if ((!anyLines && _draw_arrow) || _draw_arrow_always) {
        drawArrow(); //draw indication arrow
    }

    //Restore the current view and any offsets
    if (curr_view_id != 0) {
        globals->get_viewmgr()->set_view(curr_view_id);
        globals->get_viewmgr()->copyToCurrent();
        curr_view->setHeadingOffset_deg(ho);
        curr_view->setPitchOffset_deg(po);
        curr_view->setGoalHeadingOffset_deg(gho);
        curr_view->setGoalPitchOffset_deg(gpo);
    }
    //Set the camera back to the current view
    ssgSetCamera((sgVec4 *)curr_view);
    glPopAttrib();
}


bool HUD::Runway::get_active_runway(FGRunway& runway)
{
    FGEnvironment stationweather =
            ((FGEnvironmentMgr *)globals->get_subsystem("environment"))->getEnvironment();
    double hdg = stationweather.get_wind_from_heading_deg();
    return globals->get_runways()->search(fgGetString("/sim/presets/airport-id"), int(hdg), &runway);
}


void HUD::Runway::get_rwy_points(sgdVec3 *_points3d)
{
    double alt = current_aircraft.fdm_state->get_Runway_altitude() * SG_FEET_TO_METER;
    double length = (_runway._length / 2.0) * SG_FEET_TO_METER;
    double width = (_runway._width / 2.0) * SG_FEET_TO_METER;
    double frontLat, frontLon, backLat, backLon,az, tempLat, tempLon;

    geo_direct_wgs_84(alt, _runway._lat, _runway._lon, _runway._heading, length, &backLat, &backLon, &az);
    sgGeodToCart(backLat * SG_DEGREES_TO_RADIANS, backLon * SG_DEGREES_TO_RADIANS, alt, _points3d[4]);

    geo_direct_wgs_84(alt, _runway._lat, _runway._lon, _runway._heading + 180, length, &frontLat, &frontLon, &az);
    sgGeodToCart(frontLat * SG_DEGREES_TO_RADIANS, frontLon * SG_DEGREES_TO_RADIANS, alt, _points3d[5]);

    geo_direct_wgs_84(alt, backLat, backLon, _runway._heading + 90, width, &tempLat, &tempLon, &az);
    sgGeodToCart(tempLat * SG_DEGREES_TO_RADIANS, tempLon * SG_DEGREES_TO_RADIANS, alt, _points3d[0]);

    geo_direct_wgs_84(alt, backLat, backLon, _runway._heading - 90, width, &tempLat, &tempLon, &az);
    sgGeodToCart(tempLat * SG_DEGREES_TO_RADIANS, tempLon * SG_DEGREES_TO_RADIANS, alt, _points3d[1]);

    geo_direct_wgs_84(alt, frontLat, frontLon, _runway._heading - 90, width, &tempLat, &tempLon, &az);
    sgGeodToCart(tempLat * SG_DEGREES_TO_RADIANS, tempLon * SG_DEGREES_TO_RADIANS, alt, _points3d[2]);

    geo_direct_wgs_84(alt, frontLat, frontLon, _runway._heading + 90, width, &tempLat, &tempLon, &az);
    sgGeodToCart(tempLat * SG_DEGREES_TO_RADIANS, tempLon * SG_DEGREES_TO_RADIANS, alt, _points3d[3]);

    //Get the current tile center
    SGVec3d tileCenter = globals->get_scenery()->get_center();
    for (int i = 0; i < 6; i++) {
        _points3d[i][0] -= tileCenter.x();
        _points3d[i][1] -= tileCenter.y();
        _points3d[i][2] -= tileCenter.z();
    }
}


bool HUD::Runway::drawLine(const sgdVec3& a1, const sgdVec3& a2, const sgdVec3& point1, const sgdVec3& point2)
{
    sgdVec3 p1, p2;
    sgdCopyVec3(p1, point1);
    sgdCopyVec3(p2, point2);
    bool p1Inside = (p1[0] >= _left && p1[0] <= _right && p1[1] >= _bottom && p1[1] <= _top);
    bool p1Insight = (p1[2] >= 0.0 && p1[2] < 1.0);
    bool p1Valid = p1Insight && p1Inside;
    bool p2Inside = (p2[0] >= _left && p2[0] <= _right && p2[1] >= _bottom && p2[1] <= _top);
    bool p2Insight = (p2[2] >= 0.0 && p2[2] < 1.0);
    bool p2Valid = p2Insight && p2Inside;

    if (p1Valid && p2Valid) { //Both project points are valid, draw the line
        glBegin(GL_LINES);
        glVertex2d(p1[0],p1[1]);
        glVertex2d(p2[0],p2[1]);
        glEnd();

    } else if (p1Valid) { //p1 is valid and p2 is not, calculate a new valid point
        sgdVec3 vec = {a2[0] - a1[0], a2[1] - a1[1], a2[2] - a1[2]};
        //create the unit vector
        sgdScaleVec3(vec, 1.0 / sgdLengthVec3(vec));
        sgdVec3 newPt;
        sgdCopyVec3(newPt, a1);
        sgdAddVec3(newPt, vec);
        if (gluProject(newPt[0], newPt[1], newPt[2], _mm, _pm, _view, &p2[0], &p2[1], &p2[2])
                && (p2[2] > 0 && p2[2] < 1.0)) {
            boundPoint(p1, p2);
            glBegin(GL_LINES);
            glVertex2d(p1[0], p1[1]);
            glVertex2d(p2[0], p2[1]);
            glEnd();
        }

    } else if (p2Valid) { //p2 is valid and p1 is not, calculate a new valid point
        sgdVec3 vec = {a1[0] - a2[0], a1[1] - a2[1], a1[2] - a2[2]};
        //create the unit vector
        sgdScaleVec3(vec, 1.0 / sgdLengthVec3(vec));
        sgdVec3 newPt;
        sgdCopyVec3(newPt, a2);
        sgdAddVec3(newPt, vec);
        if (gluProject(newPt[0], newPt[1], newPt[2], _mm, _pm, _view, &p1[0], &p1[1], &p1[2])
                && (p1[2] > 0 && p1[2] < 1.0)) {
            boundPoint(p2, p1);
            glBegin(GL_LINES);
            glVertex2d(p2[0], p2[1]);
            glVertex2d(p1[0], p1[1]);
            glEnd();
        }

    } else if (p1Insight && p2Insight) { //both points are insight, but not inside
        bool v = boundOutsidePoints(p1, p2);
        if (v) {
            glBegin(GL_LINES);
                glVertex2d(p1[0], p1[1]);
                glVertex2d(p2[0], p2[1]);
            glEnd();
        }
        return v;
    }
    //else both points are not insight, don't draw anything
    return (p1Valid && p2Valid);
}


void HUD::Runway::boundPoint(const sgdVec3& v, sgdVec3& m)
{
    double y = v[1];
    if (m[1] < v[1])
        y = _bottom;
    else if (m[1] > v[1])
        y = _top;

    if (m[0] == v[0]) {
        m[1] = y;
        return;  //prevent divide by zero
    }

    double slope = (m[1] - v[1]) / (m[0] - v[0]);
    m[0] = (y - v[1]) / slope + v[0];
    m[1] = y;

    if (m[0] < _left) {
        m[0] = _left;
        m[1] = slope * (_left - v[0]) + v[1];

    } else if (m[0] > _right) {
        m[0] = _right;
        m[1] = slope * (_right - v[0]) + v[1];
    }
}


bool HUD::Runway::boundOutsidePoints(sgdVec3& v, sgdVec3& m)
{
    bool pointsInvalid = (v[1] > _top && m[1] > _top) ||
                         (v[1] < _bottom && m[1] < _bottom) ||
                         (v[0] > _right && m[0] > _right) ||
                         (v[0] < _left && m[0] < _left);
    if (pointsInvalid)
        return false;

    if (m[0] == v[0]) {//x's are equal, vertical line
        if (m[1] > v[1]) {
            m[1] = _top;
            v[1] = _bottom;
        } else {
            v[1] = _top;
            m[1] = _bottom;
        }
        return true;
    }

    if (m[1] == v[1]) { //y's are equal, horizontal line
        if (m[0] > v[0]) {
            m[0] = _right;
            v[0] = _left;
        } else {
            v[0] = _right;
            m[0] = _left;
        }
        return true;
    }

    double slope = (m[1] - v[1]) / (m[0] - v[0]);
    double b = v[1] - (slope * v[0]);
    double y1 = slope * _left + b;
    double y2 = slope * _right + b;
    double x1 = (_bottom - b) / slope;
    double x2 = (_top - b) / slope;
    int counter = 0;

    if (y1 >= _bottom && y1 <= _top) {
        v[0] = _left;
        v[1] = y1;
        counter++;
    }

    if (y2 >= _bottom && y2 <= _top) {
        if (counter > 0) {
            m[0] = _right;
            m[1] = y2;
        } else {
            v[0] = _right;
            v[1] = y2;
        }
        counter++;
    }

    if (x1 >= _left && x1 <= _right) {
        if (counter > 0) {
            m[0] = x1;
            m[1] = _bottom;
        } else {
            v[0] = x1;
            v[1] = _bottom;
        }
        counter++;
    }

    if (x2 >= _left && x2 <= _right) {
        m[0] = x1;
        m[1] = _bottom;
        counter++;
    }
    return (counter == 2);
}


void HUD::Runway::drawArrow()
{
    Point3D ac(0.0), rwy(0.0);
    ac.setlat(current_aircraft.fdm_state->get_Latitude_deg());
    ac.setlon(current_aircraft.fdm_state->get_Longitude_deg());
    rwy.setlat(_runway._lat);
    rwy.setlon(_runway._lon);
    float theta = GetHeadingFromTo(ac, rwy);
    theta -= fgGetDouble("/orientation/heading-deg");
    theta = -theta;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated((_right + _left) / 2.0, (_top + _bottom) / 2.0, 0.0);
    glRotated(theta, 0.0, 0.0, 1.0);
    glTranslated(0.0, _arrow_radius, 0.0);
    glScaled(_arrow_scale, _arrow_scale, 0.0);

    glBegin(GL_TRIANGLES);
    glVertex2d(-5.0, 12.5);
    glVertex2d(0.0, 25.0);
    glVertex2d(5.0, 12.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2d(-2.5, 0.0);
    glVertex2d(-2.5, 12.5);
    glVertex2d(2.5, 12.5);
    glVertex2d(2.5, 0.0);
    glEnd();
    glPopMatrix();
}


void HUD::Runway::setLineWidth()
{
    //Calculate the distance from the runway, A
    double course, distance;
    calc_gc_course_dist(Point3D(_runway._lon * SGD_DEGREES_TO_RADIANS,
            _runway._lat * SGD_DEGREES_TO_RADIANS, 0.0),
            Point3D(current_aircraft.fdm_state->get_Longitude(),
            current_aircraft.fdm_state->get_Latitude(), 0.0 ),
            &course, &distance);
    distance *= SG_METER_TO_NM;
    //Get altitude above runway, B
    double alt_nm = _agl->getDoubleValue();

    if (_hud->getUnits() == FEET)
        alt_nm *= SG_FEET_TO_METER;

    alt_nm *= SG_METER_TO_NM;

    //Calculate distance away from runway, C = v(A�+B�)
    distance = sqrt(alt_nm * alt_nm + distance*distance);
    if (distance < _scale_dist)
        glLineWidth(1.0 + ((_line_scale - 1) * ((_scale_dist - distance) / _scale_dist)));
    else
        glLineWidth(1.0);

}


