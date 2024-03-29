/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sfx2_sfxdll.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:25:40 $
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/

#ifdef WIN
#ifndef _SVWIN_H
#include <svwin.h>
#endif
#endif

#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifdef WIN
namespace binfilter {

// Statische DLL-Verwaltungs-Variablen
static HINSTANCE hDLLInst = 0;

//==========================================================================

/*N*/ extern "C" int CALLBACK LibMain( HINSTANCE hDLL, WORD, WORD nHeap, LPSTR )
/*N*/ {
/*N*/ #ifndef WNT
/*N*/ 	if ( nHeap )
/*N*/ 		UnlockData( 0 );
/*N*/ #endif
/*N*/ 
/*N*/ 	hDLLInst = hDLL;
/*N*/ 
/*N*/ 	return TRUE;
/*N*/ }


//--------------------------------------------------------------------------

/*N*/ extern "C" int CALLBACK WEP( int )
/*N*/ {
/*N*/ 	return 1;
/*N*/ }



//==========================================================================
}
#endif



