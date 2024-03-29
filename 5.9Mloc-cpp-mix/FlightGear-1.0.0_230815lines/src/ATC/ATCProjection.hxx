// ATCProjection.hxx - A convienience projection class for the ATC/AI system.
//
// Written by David Luff, started 2002.
//
// Copyright (C) 2002  David C Luff - david.luff@nottingham.ac.uk
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

#ifndef _FG_ATC_PROJECTION_HXX
#define _FG_ATC_PROJECTION_HXX

#include <simgear/math/point3d.hxx>

// FGATCProjection - a class to project an area local to an airport onto an orthogonal co-ordinate system
class FGATCProjection {

public:
    FGATCProjection();
    FGATCProjection(const Point3D& centre);
    ~FGATCProjection();

    void Init(const Point3D& centre);

    // Convert a lat/lon co-ordinate (degrees) to the local projection (meters)
    Point3D ConvertToLocal(const Point3D& pt);

    // Convert a local projection co-ordinate (meters) to lat/lon (degrees)
    Point3D ConvertFromLocal(const Point3D& pt);

private:
    Point3D _origin;	// lat/lon of local area origin
    double _correction_factor;	// Reduction in surface distance per degree of longitude due to latitude.  Saves having to do a cos() every call.

};


// FGATCAlignedProjection - a class to project an area local to a runway onto an orthogonal co-ordinate system
// with the origin at the threshold and the runway aligned with the y axis.
class FGATCAlignedProjection {

public:
    FGATCAlignedProjection();
    FGATCAlignedProjection(const Point3D& centre, double heading);
    ~FGATCAlignedProjection();

    void Init(const Point3D& centre, double heading);

    // Convert a lat/lon co-ordinate (degrees) to the local projection (meters)
    Point3D ConvertToLocal(const Point3D& pt);

    // Convert a local projection co-ordinate (meters) to lat/lon (degrees)
    Point3D ConvertFromLocal(const Point3D& pt);

private:
    Point3D _origin;	// lat/lon of local area origin (the threshold)
    double _theta;	// the rotation angle for alignment in radians
    double _correction_factor;	// Reduction in surface distance per degree of longitude due to latitude.  Saves having to do a cos() every call.

};

#endif	// _FG_ATC_PROJECTION_HXX
