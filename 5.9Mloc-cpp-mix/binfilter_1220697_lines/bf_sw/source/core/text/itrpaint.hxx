/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: itrpaint.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 23:05:02 $
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
#ifndef _ITRPAINT_HXX
#define _ITRPAINT_HXX

#include "itrtxt.hxx"
namespace binfilter {

class SwSaveClip;          // SwTxtPainter
class SwMultiPortion;

/*************************************************************************
 *                      class SwTxtPainter
 *************************************************************************/

class SwTxtPainter : public SwTxtCursor
{
	sal_Bool bPaintDrop;

protected:
	void CtorInit( SwTxtFrm *pFrm, SwTxtPaintInfo *pInf );
	inline SwTxtPainter() { }
public:
	inline SwTxtPainter( SwTxtFrm *pFrm, SwTxtPaintInfo *pInf )
		   { CtorInit( pFrm, pInf ); }
    // if PaintMultiPortion is called recursively, we have to pass the
    // surrounding SwBidiPortion
	inline void SetPaintDrop( const sal_Bool bNew ) { bPaintDrop = bNew; }
	inline int IsPaintDrop() const { return bPaintDrop; }
	inline SwTxtPaintInfo &GetInfo()
		{ return (SwTxtPaintInfo&)SwTxtIter::GetInfo(); }
	inline const SwTxtPaintInfo &GetInfo() const
		{ return (const SwTxtPaintInfo&)SwTxtIter::GetInfo(); }
};



} //namespace binfilter
#endif
