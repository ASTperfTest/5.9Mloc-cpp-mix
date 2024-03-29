/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fudraw.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:26:16 $
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

#ifndef _SD_FUDRAW_HXX
#define _SD_FUDRAW_HXX

#ifndef _SV_POINTR_HXX //autogen
#include <vcl/pointr.hxx>
#endif

#ifndef _SD_FUPOOR_HXX
#include "fupoor.hxx"
#endif
namespace binfilter {

struct SdrViewEvent;
class SdrObject;


/*************************************************************************
|*
|* Basisklasse fuer alle Drawmodul-spezifischen Funktionen
|*
\************************************************************************/

class FuDraw : public FuPoor
{
 protected:
	Pointer aNewPointer;
	Pointer aOldPointer;
	BOOL	bMBDown;
	BOOL	bDragHelpLine;
	USHORT	nHelpLine;
	BOOL    bPermanent;

 public:
	TYPEINFO();

	FuDraw(SdViewShell* pViewSh, SdWindow* pWin, SdView* pView,
			SdDrawDocument* pDoc, SfxRequest& rReq);
	virtual ~FuDraw();



	virtual void Activate();
	virtual void Deactivate();

	virtual void ForcePointer(const MouseEvent* pMEvt = NULL);



	void    SetPermanent(BOOL bSet) { bPermanent = bSet; }

	/** is called when the currenct function should be aborted. <p>
		This is used when a function gets a KEY_ESCAPE but can also
		be called directly.

		@returns true if a active function was aborted
	*/
};



} //namespace binfilter
#endif		// _SD_FUDRAW_HXX
