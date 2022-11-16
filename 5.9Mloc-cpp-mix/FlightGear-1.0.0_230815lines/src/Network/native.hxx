// native.hxx -- FGFS "Native" protocal class
//
// Written by Curtis Olson, started November 1999.
//
// Copyright (C) 1999  Curtis L. Olson - http://www.flightgear.org/~curt
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
// $Id: native.hxx,v 1.3 2006-02-21 01:19:47 mfranz Exp $


#ifndef _FG_NATIVE_HXX
#define _FG_NATIVE_HXX


#include <simgear/compiler.h>

#include STL_STRING

#include <FDM/flight.hxx>

#include "protocol.hxx"

SG_USING_STD(string);


class FGNative : public FGProtocol {

    FGInterface buf;
    int length;

public:

    FGNative();
    ~FGNative();

    // open hailing frequencies
    bool open();

    // process work for this port
    bool process();

    // close the channel
    bool close();
};


#endif // _FG_NATIVE_HXX

