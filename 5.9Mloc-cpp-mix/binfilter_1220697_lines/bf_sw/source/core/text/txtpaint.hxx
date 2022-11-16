/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: txtpaint.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 23:15:43 $
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
#ifndef _TXTPAINT_HXX
#define _TXTPAINT_HXX

#ifndef _SV_OUTDEV_HXX //autogen
#include <vcl/outdev.hxx>
#endif


class SwRect;				// SwSaveClip

#ifdef VERTICAL_LAYOUT
#endif
namespace binfilter {

/*************************************************************************
 *						class SwSaveClip
 *************************************************************************/

class SwSaveClip
{
	Region	 aClip;
	const sal_Bool	   bOn;
		  sal_Bool	   bChg;
protected:
	OutputDevice *pOut;
public:
	inline SwSaveClip( OutputDevice *pOut );
	inline ~SwSaveClip();
	void Reset(){DBG_BF_ASSERT(0, "STRIP");} ;//STRIP001 		   void Reset();
	inline sal_Bool IsOn()	const { return bOn; }
	inline sal_Bool IsChg() const { return bChg; }
	inline sal_Bool IsOut() const { return 0 != pOut; }
	inline OutputDevice *GetOut() { return pOut; }
};

inline SwSaveClip::SwSaveClip( OutputDevice *pOut ) :
	pOut(pOut),
	bOn( pOut && pOut->IsClipRegion() ),
	bChg( sal_False )
{}

inline SwSaveClip::~SwSaveClip()
{
	Reset();
}

#ifndef PRODUCT

/*************************************************************************
 *							class SwDbgOut
 *************************************************************************/

class SwDbgOut
{
protected:
		OutputDevice *pOut;
public:
		inline SwDbgOut( OutputDevice *pOutDev, const sal_Bool bOn = sal_True );
};

/*************************************************************************
 *							class DbgPen
 *************************************************************************/

//class DbgPen : public SwDbgOut
//{
//		Pen aPen;
//public:
//		inline DbgPen( OutputDevice *pOutDev, const sal_Bool bOn = sal_True,
//					   const ColorName eColor = COL_BLACK );
//		inline ~DbgPen();
//};

/*************************************************************************
 *							class DbgColor
 *************************************************************************/

class DbgColor
{
	Font *pFnt;
	Color aColor;
public:
		inline DbgColor( Font *pFont, const sal_Bool bOn = sal_True,
						 const ColorData eColor = COL_BLUE );
		inline ~DbgColor();
};

/*************************************************************************
 *							class DbgBrush
 *************************************************************************/

class DbgBackColor : public SwDbgOut
{
		Color 	aOldFillColor;
public:
		DbgBackColor( OutputDevice *pOut, const sal_Bool bOn = sal_True,
				  ColorData nColor = COL_YELLOW );
	   ~DbgBackColor();
};

/*************************************************************************
 *							class DbgRect
 *************************************************************************/

class DbgRect : public SwDbgOut
{
public:
		DbgRect( OutputDevice *pOut, const Rectangle &rRect,
				 const sal_Bool bOn = sal_True,
				 ColorData eColor = COL_LIGHTBLUE );
};

/*************************************************************************
 *						Inline-Implementierung
 *************************************************************************/

inline SwDbgOut::SwDbgOut( OutputDevice *pOutDev, const sal_Bool bOn )
			   :pOut( bOn ? pOutDev : 0 )
{ }

//inline DbgPen::DbgPen( OutputDevice *pOutDev, const sal_Bool bOn,
//			   const ColorName eColor )
//	: SwDbgOut( pOutDev, bOn)
//{
//	if( pOut )
//	{
//		const Color aColor( eColor );
//		Pen aTmpPen( aColor );
//		aPen = pOut->GetPen( );
//		pOut->SetPen( aTmpPen );
//	}
//}

//inline DbgPen::~DbgPen()
//{
//	if( pOut )
//		pOut->SetPen(aPen);
//}

inline DbgColor::DbgColor( Font *pFont, const sal_Bool bOn,
				 const ColorData eColor )
	:pFnt( bOn ? pFont : 0 )
{
	if( pFnt )
	{
		aColor = pFnt->GetColor();
		pFnt->SetColor( Color( eColor ) );
	}
}

inline DbgColor::~DbgColor()
{
	if( pFnt )
		pFnt->SetColor( aColor );
}

inline DbgBackColor::DbgBackColor( OutputDevice *pOutDev, const sal_Bool bOn,
						   ColorData eColor )
	:SwDbgOut( pOutDev, bOn )
{
	if( pOut )
	{
		aOldFillColor = pOut->GetFillColor();
		pOut->SetFillColor( Color(eColor) );
	}
}

inline DbgBackColor::~DbgBackColor()
{
	if( pOut )
	{
		pOut->SetFillColor( aOldFillColor );
	}
}

inline DbgRect::DbgRect( OutputDevice *pOutDev, const Rectangle &rRect,
						 const sal_Bool bOn,
						 ColorData eColor )
	: SwDbgOut( pOutDev, bOn )
{
	if( pOut )
	{
		const Color aColor( eColor );
		Color aLineColor = pOut->GetLineColor();
		pOut->SetLineColor( aColor );
		Color aFillColor = pOut->GetFillColor();
		pOut->SetFillColor( Color(COL_TRANSPARENT) );
		pOut->DrawRect( rRect );
		pOut->SetLineColor( aLineColor );
		pOut->SetFillColor( aFillColor );
	}
}

#endif



} //namespace binfilter
#endif