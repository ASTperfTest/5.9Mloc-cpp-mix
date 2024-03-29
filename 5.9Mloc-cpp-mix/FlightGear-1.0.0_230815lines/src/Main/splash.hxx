// splash.hxx -- draws the initial splash screen
//
// Written by Curtis Olson, started July 1998.  (With a little looking
// at Freidemann's panel code.) :-)
//
// Copyright (C) 1997  Michele F. America  - nomimarketing@mail.telepac.pt
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
// $Id: splash.hxx,v 1.6.2.1 2007-05-02 18:49:34 mfranz Exp $


#ifndef _SPLASH_HXX
#define _SPLASH_HXX


#ifndef __cplusplus
# error This library requires C++
#endif


// Initialize the splash screen
void fgSplashInit ( const char *splash_texture );

// Update the splash screen with alpha specified from 0.0 to 1.0
void fgSplashUpdate ( float alpha );

// Set progress information
void fgSplashProgress ( const char *text );

// Free texture memory
void fgSplashExit ();


#endif // _SPLASH_HXX


