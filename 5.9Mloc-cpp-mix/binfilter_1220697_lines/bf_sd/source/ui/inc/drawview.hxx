/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: drawview.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:25:18 $
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

#ifndef _SD_DRAWVIEW_HXX
#define _SD_DRAWVIEW_HXX


//#ifndef _SD_SDVIEW_HXX
#include "sdview.hxx"
namespace binfilter {
//#endif

class SdDrawDocShell;
class SdDrawViewShell;
class FuSlideShow;

#define SDDRAWVIEW_MAGIC  0x456789BA

/*************************************************************************
|*
|* Ableitung von SdView; enthaelt auch einen Zeiger auf das Dokument
|*
\************************************************************************/

class SdDrawView : public SdView
{
	SdDrawDocShell* 	pDocShell;
	SdDrawViewShell*	pDrawViewShell;
	VirtualDevice*      pVDev;

	USHORT				nPOCHSmph;	// zum blockieren des PageOrderChangedHint
	USHORT				nPresPaintSmph; 	// zum Blockieren des Zeichnens
											// in der Diashow
	BOOL                bPixelMode;
	FuSlideShow*        pSlideShow;
	BOOL                bInAnimation;
	ULONG               nMagic;

	BOOL                bActionMode;

 protected:
	virtual void ModelHasChanged();

 public:
	TYPEINFO();

	 SdDrawView(SdDrawDocShell* pDocSh, OutputDevice* pOutDev,
				SdDrawViewShell* pShell);
	~SdDrawView();

	virtual void MarkListHasChanged();


	virtual void SFX_NOTIFY(SfxBroadcaster& rBC, const TypeId& rBCType,
							const SfxHint& rHint, const TypeId& rHintType);



	BOOL	IsPresPaintAllowed() const { return nPresPaintSmph == 0; }
	BOOL    IsPixelMode() const { return bPixelMode; }
	void    SetActionMode(BOOL bOn) { bActionMode = bOn; }
	BOOL    IsActionMode() const { return bActionMode; }

	virtual void HidePage(SdrPageView* pPV);


	BOOL    IsInAnimation() { return bInAnimation; }
	FuSlideShow* GetSlideShow() { return pSlideShow; }

};



} //namespace binfilter
#endif		// _SD_DRAWVIEW_HXX
