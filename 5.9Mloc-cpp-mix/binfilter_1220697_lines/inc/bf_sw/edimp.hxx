/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: edimp.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:40:08 $
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

#ifndef _EDIMP_HXX
#define _EDIMP_HXX

#include "crsrsh.hxx"
namespace binfilter {

/*
 * MACROS um ueber alle Bereiche zu iterieren
 */
#define PCURCRSR (_pStartCrsr)

#define FOREACHPAM_START(pCURSH) \
	{\
		SwPaM *_pStartCrsr = (pCURSH)->GetCrsr(), *__pStartCrsr = _pStartCrsr; \
		do {

#define FOREACHPAM_END() \
		} while( (_pStartCrsr=(SwPaM *)_pStartCrsr->GetNext()) != __pStartCrsr ); \
	}


#define FOREACHCURSOR_START(pCURSH) \
	{\
		SwShellCrsr *_pStartCrsr = *(pCURSH)->GetSwCrsr(), *__pStartCrsr = _pStartCrsr; \
		do {

#define FOREACHCURSOR_END() \
		} while( (_pStartCrsr=*(SwCursor*)_pStartCrsr->GetNext()) != __pStartCrsr ); \
	}


struct SwPamRange
{
	ULONG nStart, nEnd;

	SwPamRange() : nStart( 0 ), nEnd( 0 )	{}
	SwPamRange( ULONG nS, ULONG nE ) : nStart( nS ), nEnd( nE )	{}

	BOOL operator==( const SwPamRange& rRg )
		{ return nStart == rRg.nStart ? TRUE : FALSE; }
	BOOL operator<( const SwPamRange& rRg )
		{ return nStart < rRg.nStart ? TRUE : FALSE; }
};

SV_DECL_VARARR_SORT( _SwPamRanges, SwPamRange, 0, 1 )

class SwPamRanges : private _SwPamRanges
{
public:
	SwPamRanges( const SwPaM& rRing ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 SwPamRanges( const SwPaM& rRing );

	SwPaM& SetPam( USHORT nArrPos, SwPaM& rPam ){DBG_BF_ASSERT(0, "STRIP"); return rPam;} //STRIP001 SwPaM& SetPam( USHORT nArrPos, SwPaM& rPam );

	USHORT Count() const
				{	return _SwPamRanges::Count(); }
};


} //namespace binfilter
#endif
