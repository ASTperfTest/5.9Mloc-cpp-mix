/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_txtinit.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:39:05 $
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


#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "fntcache.hxx"     // pFntCache  ( SwFont/ScrFont-PrtFont Cache )
#include "swfntcch.hxx"     // pSwFontCache  ( SwAttrSet/SwFont Cache )
#include "txtfrm.hxx"
#include "txtcache.hxx"
#include "porrst.hxx"

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#include "pordrop.hxx"
#include "txtfly.hxx"	// SwContourCache
#include "dbg_lay.hxx"  // Layout Debug Fileausgabe
namespace binfilter { 

/*N*/ SwCache *SwTxtFrm::pTxtCache = 0;
/*N*/ long SwTxtFrm::nMinPrtLine = 0;
/*N*/ SwContourCache *pContourCache = 0;

#ifndef PROFILE
// Code zum Initialisieren von Statics im eigenen Code-Segment
#ifdef _MSC_VER
#pragma code_seg( "SWSTATICS" )
#endif
#endif

/*N*/ IMPL_FIXEDMEMPOOL_NEWDEL( SwTxtLine, 	  50,  50 )
/*N*/ IMPL_FIXEDMEMPOOL_NEWDEL( SwParaPortion,  50,  50 )	//Absaetze
/*N*/ IMPL_FIXEDMEMPOOL_NEWDEL( SwLineLayout,  150, 150 )	//Zeilen
/*N*/ IMPL_FIXEDMEMPOOL_NEWDEL( SwHolePortion, 150, 150 )	//z.B. Blanks am Zeilenende
/*N*/ IMPL_FIXEDMEMPOOL_NEWDEL( SwTxtPortion,  200, 100 )	//Attributwechsel

#ifndef PROFILE
#ifdef _MSC_VER
#pragma code_seg()
#endif
#endif

/*************************************************************************
 *                  _TextInit(), _TextFinit()
 *************************************************************************/

// Werden _nur_ in init.cxx verwendet, dort stehen extern void _TextFinit()
// und extern void _TextInit(...)

/*N*/ void _TextInit()
/*N*/ {
/*N*/ 	pFntCache = new SwFntCache;
/*N*/ 	pSwFontCache = new SwFontCache;
/*N*/ 	pWaveCol = new Color( COL_GRAY );
/*N*/ 
/*N*/ 	//Pauschale groesse 250, plus 100 pro Shell
/*N*/ 	SwCache *pTxtCache = new SwCache( 250, 100
/*N*/ #ifndef PRODUCT
/*N*/ 	, "static SwTxtFrm::pTxtCache"
/*N*/ #endif
/*N*/ 	);
/*N*/ 	SwTxtFrm::SetTxtCache( pTxtCache );
/*N*/ 	PROTOCOL_INIT
/*N*/ }

/*N*/ void _TextFinit()
/*N*/ {
/*N*/ 	PROTOCOL_STOP
/*N*/ 	delete SwTxtFrm::GetTxtCache();
/*N*/ 	delete pSwFontCache;
/*N*/ 	delete pFntCache;
/*N*/ 	delete pWaveCol;
/*N*/ 	delete pContourCache;
/*N*/ }



}
