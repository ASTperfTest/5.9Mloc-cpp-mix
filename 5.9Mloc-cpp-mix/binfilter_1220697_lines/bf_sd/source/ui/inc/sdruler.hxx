/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdruler.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:31:12 $
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

#ifndef _SD_SDRULER_HXX
#define _SD_SDRULER_HXX


#ifndef _SVX_RULER_HXX //autogen
#include <bf_svx/ruler.hxx>
#endif
namespace binfilter {

class SdRulerCtrlItem;
class SdView;
class SdWindow;
class SdDrawViewShell;


/*************************************************************************
|*
|* das Fenster der Diashow leitet Mouse- und Key-Events an die SdSlideViewShell
|*
\************************************************************************/

class SdRuler : public SvxRuler
{
 protected:
	SdView* 			pSdView;
	SdWindow*			pSdWin;
	SdDrawViewShell*	pDrViewShell;
	SdRulerCtrlItem*	pCtrlItem;
	BOOL				bHorz;


 public:
	SdRuler(SdDrawViewShell& rViewSh, Window* pParent, SdWindow* pWin,
			USHORT nRulerFlags, SfxBindings& rBindings,
			WinBits nWinStyle);
	~SdRuler();

	void SetNullOffset(const Point& rOffset);

};



} //namespace binfilter
#endif		// _SD_SDRULER_HXX
