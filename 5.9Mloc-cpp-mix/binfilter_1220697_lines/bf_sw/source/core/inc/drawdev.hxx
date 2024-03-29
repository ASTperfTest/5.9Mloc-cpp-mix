/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: drawdev.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 09:27:10 $
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

#ifndef _DRAWDEV_HXX
#define _DRAWDEV_HXX

#include "swrect.hxx"

#ifndef _OUTDEV_HXX //autogen
#include <vcl/outdev.hxx>
#endif
namespace binfilter {

/*************************************************************************
 *							class SwDrawDev
 *
 * Alle Draw-Methoden werden um den Offset *pPos verschoben.
 *************************************************************************/

class SwDrawDev
{
	OutputDevice  *pOut;
	const Point *pPos;

public:
	inline SwDrawDev( OutputDevice *pOut, const Point *pPos )
		:pOut(pOut), pPos(pPos) { }

	inline OutputDevice *GetOut() { return pOut; }

	// Ausgabemethoden
	inline void DrawText( const Point& rStart, const String& rTxt,
						  const USHORT nIdx = 0,
						  const USHORT nLen = STRING_LEN );
	inline void DrawStretchText( const Point& rStart, USHORT nWidth,
						  const String& rTxt,
						  const USHORT nIdx = 0,
						  const USHORT nLen = STRING_LEN );
	inline void DrawTextArray( const Point& rStart,
						  const String& rTxt,
						  long *pKernArray = 0,
						  const USHORT nIdx = 0,
						  const USHORT nLen = STRING_LEN);
	inline void DrawLine( const Point& rStart, const Point& rEnd );
	inline void DrawRect( const SwRect& rRect,
						  const USHORT nHorzRount = 0,
						  const USHORT nVertRound = 0 );

	inline const Point *GetOrigin() const {return pPos; }
};

/*************************************************************************
 *						SwDrawDev::DrawText
 *************************************************************************/

inline void SwDrawDev::DrawText( const Point& rStart, const String& rTxt,
								 const USHORT nIdx, const USHORT nLen )
{
	if( !pPos )
		pOut->DrawText( rStart, rTxt, nIdx, nLen );
	else
		pOut->DrawText( rStart - *pPos, rTxt, nIdx, nLen );
}

/*************************************************************************
 *						SwDrawDev::DrawStretchText
 *************************************************************************/

inline void SwDrawDev::DrawStretchText( const Point& rStart, USHORT nWidth,
	   const String& rTxt, const USHORT nIdx, const USHORT nLen )
{
	if( !pPos )
		pOut->DrawStretchText( rStart, nWidth, rTxt, nIdx, nLen );
	else
		pOut->DrawStretchText( rStart - *pPos, nWidth, rTxt, nIdx, nLen );
}

/*************************************************************************
 *						SwDrawDev::DrawTextArray
 *************************************************************************/

inline void SwDrawDev::DrawTextArray( const Point& rStart, const String& rTxt,
			long *pKernArray, const USHORT nIdx, const USHORT nLen )
{
	if( !pPos )
		pOut->DrawTextArray( rStart, rTxt, pKernArray, nIdx, nLen );
	else
		pOut->DrawTextArray( rStart - *pPos, rTxt, pKernArray, nIdx, nLen );
}

/*************************************************************************
 *						SwDrawDev::DrawLine
 *************************************************************************/

inline void SwDrawDev::DrawLine( const Point& rStart, const Point& rEnd )
{
	if( !pPos )
		pOut->DrawLine( rStart, rEnd );
	else
		pOut->DrawLine( rStart - *pPos, rEnd - *pPos );
}

/*************************************************************************
 *						SwDrawDev::DrawRect
 *************************************************************************/

inline void SwDrawDev::DrawRect( const SwRect& rRect,
					  const USHORT nHorzRound, const USHORT nVertRound )
{
	SwRect aRect( rRect );
	if( pPos )
		aRect.Pos() -= *pPos;
	pOut->DrawRect( aRect.SVRect(), nHorzRound, nVertRound );
}


} //namespace binfilter
#endif
