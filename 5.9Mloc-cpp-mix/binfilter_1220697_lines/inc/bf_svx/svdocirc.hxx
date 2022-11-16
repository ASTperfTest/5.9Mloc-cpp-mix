/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svdocirc.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:07:51 $
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

#ifndef _SVDOCIRC_HXX
#define _SVDOCIRC_HXX

#ifndef _SVDORECT_HXX
#include <bf_svx/svdorect.hxx>
#endif
namespace binfilter {

//************************************************************
//   Vorausdeklarationen
//************************************************************

class SdrCircSetItem;

//************************************************************
//   Hilfsklasse SdrCircObjGeoData
//************************************************************

class SdrCircObjGeoData : public SdrRectObjGeoData
{
public:
	long						nStartWink;
	long						nEndWink;
};

//************************************************************
//   SdrCircObj
//************************************************************

class SdrCircObj : public SdrRectObj
{
protected:
	SdrObjKind					eKind;
	long						nStartWink;
	long						nEndWink;
	Point						aPnt1;
	Point						aPnt2;

private:
	XPolygon ImpCalcXPoly(const Rectangle& rRect1, long nStart, long nEnd, FASTBOOL bContour=FALSE) const;
	void ImpSetAttrToCircInfo(); // Werte vom Pool kopieren
	void ImpSetCircInfoToAttr(); // Werte in den Pool kopieren

	// Liefert TRUE, wenn das Painten ein XPolygon erfordert.
	FASTBOOL PaintNeedsXPoly() const;
	virtual void RecalcXPoly();

protected:
	virtual void SFX_NOTIFY(SfxBroadcaster& rBC, const TypeId& rBCType, const SfxHint& rHint, const TypeId& rHintType);
	virtual void ForceDefaultAttr();

public:
	TYPEINFO();
	SdrCircObj(SdrObjKind eNewKind); // Circ, CArc, Sect oder CCut
	SdrCircObj(SdrObjKind eNewKind, const Rectangle& rRect);

	// 0=0.00Deg=3h 9000=90.00Deg=12h 18000=180.00Deg=9h 27000=270.00Deg=6h
	// Der Verlauf des Kreises von StartWink nach EndWink ist immer entgegen
	// dem Uhrzeigersinn.
	// Wenn nNewStartWink==nNewEndWink hat der Kreisbogen einen Verlaufswinkel
	// von 0 Grad. Bei nNewStartWink+36000==nNewEndWink ist der Verlaufswinkel
	// 360.00 Grad.
	SdrCircObj(SdrObjKind eNewKind, const Rectangle& rRect, long nNewStartWink, long nNewEndWink);
	virtual ~SdrCircObj();

	virtual UINT16 GetObjIdentifier() const;
	virtual void RecalcBoundRect();
	virtual void TakeUnrotatedSnapRect(Rectangle& rRect) const;
	virtual FASTBOOL Paint(ExtOutputDevice& rOut, const SdrPaintInfoRec& rInfoRec) const;


	virtual void operator=(const SdrObject& rObj);
	virtual void RecalcSnapRect();
	virtual void NbcSetSnapRect(const Rectangle& rRect);
	virtual void TakeXorPoly(XPolyPolygon& rXPoly, FASTBOOL bDetail) const;
	virtual void TakeContour(XPolyPolygon& rXPoly) const;




	virtual void NbcMove(const Size& aSiz);
	virtual void NbcResize(const Point& rRef, const Fraction& xFact, const Fraction& yFact);
	virtual void NbcShear (const Point& rRef, long nWink, double tn, FASTBOOL bVShear);
	virtual SdrObject* DoConvertToPolyObj(BOOL bBezier) const;

protected:
public:
	virtual void WriteData(SvStream& rOut) const;
	virtual void ReadData(const SdrObjIOHeader& rHead, SvStream& rIn);

	long GetStartWink() const { return nStartWink; }
	long GetEndWink() const { return nEndWink; }
	virtual void NbcSetStyleSheet(SfxStyleSheet* pNewStyleSheet, FASTBOOL bDontRemoveHardAttr);

	// ItemSet access
	virtual SfxItemSet* CreateNewItemSet(SfxItemPool& rPool);

	// private support routines for ItemSet access. NULL pointer means clear item.
	virtual void ItemSetChanged(const SfxItemSet& rSet);

	// pre- and postprocessing for objects for saving
	virtual void PreSave();
	virtual void PostSave();
};

}//end of namespace binfilter
#endif //_SVDOCIRC_HXX
