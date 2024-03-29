/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: pordrop.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:58:30 $
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
#ifndef _PORDROP_HXX
#define _PORDROP_HXX

#include "portxt.hxx"
namespace binfilter {

class SwFont;

/*************************************************************************
 *                      class SwDropPortionPart
 *
 * A drop portion can consist of one or more parts in order to allow
 * attribute changes inside them.
 *************************************************************************/

class SwDropPortionPart
{
    SwDropPortionPart* pFollow;
    SwFont* pFnt;
    xub_StrLen nLen;
    USHORT nWidth;

public:
    SwDropPortionPart( SwFont& rFont, const xub_StrLen nL )
            : pFollow( 0 ), pFnt( &rFont ), nLen( nL ), nWidth( 0 ) {};
    ~SwDropPortionPart();

    inline SwDropPortionPart* GetFollow() const { return pFollow; };
    inline void SetFollow( SwDropPortionPart* pNew ) { pFollow = pNew; };
    inline SwFont& GetFont() const { return *pFnt; }
    inline xub_StrLen GetLen() const { return nLen; }
    inline USHORT GetWidth() const { return nWidth; }
    inline void SetWidth( USHORT nNew )  { nWidth = nNew; }
};

/*************************************************************************
 *						class SwDropPortion
 *************************************************************************/

class SwDropPortion : public SwTxtPortion
{
    SwDropPortionPart* pPart; // due to script / attribute changes
    MSHORT nLines;          // Anzahl der Zeilen
	KSHORT nDropHeight; 	// Hoehe
	KSHORT nDropDescent; 	// Abstand zur naechsten Zeile
	KSHORT nDistance;		// Abstand zum Text
	KSHORT nFix;			// Fixposition
	short nX;				// X-PaintOffset
    short nY;               // Y-Offset

	inline void Fix( const KSHORT nNew ) { nFix = nNew; }
public:
    SwDropPortion( const MSHORT nLineCnt,
                   const KSHORT nDropHeight,
                   const KSHORT nDropDescent,
                   const KSHORT nDistance );
	virtual ~SwDropPortion();


	inline MSHORT GetLines() const { return nLines; }
	inline KSHORT GetDistance() const { return nDistance; }
	inline KSHORT GetDropHeight() const { return nDropHeight; }
	inline KSHORT GetDropDescent() const { return nDropDescent; }
	inline KSHORT GetDropLeft() const { return Width() + nFix; }

    inline SwDropPortionPart* GetPart() const { return pPart; }
    inline void SetPart( SwDropPortionPart* pNew ) { pPart = pNew; }

    inline void SetY( short nNew )  { nY = nNew; }

    inline SwFont* GetFnt() { return pPart ? &pPart->GetFont() : NULL; }

	static void DeleteDropCapCache();

	OUTPUT_OPERATOR
};


} //namespace binfilter
#endif
