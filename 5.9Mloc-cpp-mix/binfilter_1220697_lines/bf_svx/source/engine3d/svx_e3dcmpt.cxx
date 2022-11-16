/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svx_e3dcmpt.cxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 05:30:27 $
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

#ifndef _DEBUG_HXX //autogen
#include <tools/debug.hxx>
#endif

#ifndef _E3D_E3DIOCMPT_HXX
#include "e3dcmpt.hxx"
#endif
namespace binfilter {

/*************************************************************************
|*
|* Konstruktor, schreibt bzw. liest Versionsnummer
|*
\************************************************************************/

/*N*/ E3dIOCompat::E3dIOCompat(SvStream& rNewStream, USHORT nNewMode, UINT16 nVer)
/*N*/ : SdrDownCompat(rNewStream, nNewMode, TRUE),
/*N*/   nVersion(nVer)
/*N*/ {
/*N*/ 	if (nNewMode == STREAM_WRITE)
/*N*/ 	{
/*N*/ 		DBG_ASSERT(nVer != E3DIOCOMPAT_VERSIONDONTKNOW,
/*N*/ 				   "kann unbekannte Version nicht schreiben");
/*N*/ 		rNewStream << nVersion;
/*N*/ 	}
/*N*/ 	else if (nNewMode == STREAM_READ)
/*N*/ 	{
/*N*/ 		DBG_ASSERT(nVer == E3DIOCOMPAT_VERSIONDONTKNOW,
/*N*/ 				   "Lesen mit Angabe der Version ist Quatsch!");
/*N*/ 		rNewStream >> nVersion;
/*N*/ 	}
/*N*/ }


}