/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fuslshow.hxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: obo $ $Date: 2007/03/09 14:40:43 $
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


#ifndef _SD_FUSLSHOW_HXX
#define _SD_FUSLSHOW_HXX

#ifndef _COM_SUN_STAR_PRESENTATION_ANIMATIONEFFECT_HPP_
#include <com/sun/star/presentation/AnimationEffect.hpp>
#endif
#ifndef _COM_SUN_STAR_PRESENTATION_FADEEFFECT_HPP_
#include <com/sun/star/presentation/FadeEffect.hpp>
#endif
#ifndef _COM_SUN_STAR_PRESENTATION_ANIMATIONSPEED_HPP_
#include <com/sun/star/presentation/AnimationSpeed.hpp>
#endif

#include <svtools/lstner.hxx>
#ifndef _SVDPAGE_HXX //autogen
#include <bf_svx/svdpage.hxx>
#endif
#ifndef _TIME_HXX //autogen
#include <tools/time.hxx>
#endif
#ifndef _TIMER_HXX //autogen
#include <vcl/timer.hxx>
#endif
#ifndef _WRKWIN_HXX //autogen
#include <vcl/wrkwin.hxx>
#endif
#ifndef _IMAGE_HXX //autogen
#include <vcl/image.hxx>
#endif
#ifndef _SOUND_HXX //autogen
#include <vcl/sound.hxx>
#endif
#ifndef _SD_FUPOOR_HXX
#include "fupoor.hxx"
#endif
#ifndef _SD_ANMINFO_HXX
#include "anminfo.hxx"
#endif
#ifndef _SD_MOVEDEF_HXX
#include "movedef.hxx"
#endif
#ifndef _SD_FADEDEF_H
#include "fadedef.h"
#endif
class PushButton;
class Button;
class Sound;		   // fuer die, die Sound "rausdefiniert" haben 
class SvInPlaceClientMemberList;
class SfxItemSet;
class VirtualDevice;
class BitmapEx;
class SvtSaveOptions;
namespace binfilter {

class SdrTextObj;
class SdrRectObj;
class SdrGrafObj;
class SdPage;
class OutlinerParaObject;
class SdrPaintInfoRec;
class ExtOutputDevice;
class DrawPortionInfo;
struct SdrPaintProcRec;
class SdrObject;
class SdrView;
class SdrAttrObj;
class SdrAttrObj;
class SdCustomShow;

// -----------
// - Defines -
// -----------

#define PAGE_NO_END 		65535
#define PAGE_NO_SOFTEND		(PAGE_NO_END - 1)
#define PAGE_NO_PAUSE 		(PAGE_NO_SOFTEND - 1)
#define PAGE_NO_FIRSTDEF	PAGE_NO_PAUSE // immer mit anpassen

#define PEN_THICKNESS		100
#define SLIDESHOW_MAGIC		0x12345678
#define LASER_ALL_PARAS		0xffff
#define VDEV_TOLERANCE		2

#define OBJIS_GRAFANIM( _def_obj ) ( _def_obj->ISA( SdrGrafObj ) && ( (SdrGrafObj*) _def_obj )->IsAnimated() )
#define OBJIS_TEXTANIM( _def_obj ) ( _def_obj->ISA( SdrTextObj ) && ( (SdrTextObj*) _def_obj )->IsTextAnimated() )

// -----------------
// - AnimationMode -
// -----------------

enum AnimationMode
{
	ANIMATIONMODE_SHOW,
	ANIMATIONMODE_PREVIEW,
	ANIMATIONMODE_VIEW
};

// ----------------
// - PageFadeMode -
// ----------------

enum PageFadeMode
{
    PAGE_FADE_NORMAL = 0,
    PAGE_FADE_BLACK = 1,
    PAGE_FADE_WHITE = 2
};

// ------------------
// - SdAnimPageList -
// ------------------


// -------------------
// - SdSlowObjRecord -
// -------------------


// ----------------
// - SdChangedObj -
// ----------------


// ----------------
// - SdShowStatus -
// ----------------


// ---------------
// - FuSlideShow -
// ---------------

class FuSlideShow : public FuPoor
{

public:
					FuSlideShow(SdViewShell* pViewSh, SdWindow* pWin, SdView* pView,SdDrawDocument* pDoc, SfxRequest& rReq):FuPoor(pViewSh, pWin, pView, pDoc, rReq) { DBG_ASSERT(0, "STRIP! FuSlideShow is deleted."); } //STRIP001 FuSlideShow(SdViewShell* pViewSh, SdWindow* pWin, SdView* pView,SdDrawDocument* pDoc, SfxRequest& rReq);
	virtual BOOL	KeyInput(const KeyEvent& rKEvt);
	BOOL			IsLivePresentation() const { DBG_BF_ASSERT(0, "STRIP"); return FALSE; } //STRIP001 BOOL			IsLivePresentation() const { return !bNativeMode; }
	AnimationMode   GetAnimationMode() const { DBG_BF_ASSERT(0, "STRIP"); return ANIMATIONMODE_SHOW;} //STRIP001 AnimationMode   GetAnimationMode() const { return eAnimationMode; }

	void			Terminate(){DBG_ASSERT(0, "STRIP! FuSlideShow is deleted.");} //STRIP001 void			Terminate();





};

} //namespace binfilter
#endif		// _SD_FUSLSHOW_HXX
