/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: basesh.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: ihi $ $Date: 2006/11/14 12:15:40 $
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
#ifndef _SWBASESH_HXX
#define _SWBASESH_HXX


#ifndef _SHELLID_HXX
#include <shellid.hxx>
#endif

#define _SVSTDARR_USHORTSSORT
#define _SVSTDARR_USHORTS

#ifndef _LINK_HXX //autogen
#include <tools/link.hxx>
#endif
#ifndef _SV_TIMER_HXX //autogen
#include <vcl/timer.hxx>
#endif
#ifndef _SFXMODULE_HXX //autogen
#include <bf_sfx2/module.hxx>
#endif
#ifndef _SFX_SHELL_HXX //autogen
#include <bf_sfx2/shell.hxx>
#endif
#include <svtools/svstdarr.hxx>
class GraphicFilter; 
class SfxItemSet; 
class Graphic; 

namespace binfilter {

class SwWrtShell;
class SwCrsrShell;
class SwView;

class SwFlyFrmAttrMgr;

extern void DisableAllItems(SfxItemSet &);
struct DBTextStruct_Impl;
class SwBaseShell: public SfxShell
{
	SwView      &rView;

	// DragModus
	static USHORT nFrameMode;

	// Bug 75078 - if in GetState the asynch call of GetGraphic returns
	//				synch, the set the state directly into the itemset
	SfxItemSet* 		pGetStateSet;

	//Update-Timer fuer Graphic
	SvUShortsSort aGrfUpdateSlots;

	DECL_LINK( GraphicArrivedHdl, SwCrsrShell* );

protected:

	SwWrtShell& GetShell   ();

	SwView&     GetView() { return rView; }

	SwFlyFrmAttrMgr *pFrmMgr;

    DECL_STATIC_LINK( SwBaseShell, InsertDBTextHdl, DBTextStruct_Impl* );

	void SetGetStateSet( SfxItemSet* p )			{ pGetStateSet = p; }
	BOOL AddGrfUpdateSlot( USHORT nSlot )
								{ return aGrfUpdateSlots.Insert( nSlot ); }

public:
	SwBaseShell(SwView &rShell);
	virtual     ~SwBaseShell();
	SFX_DECL_INTERFACE(SW_BASESHELL);
	TYPEINFO();

	void        ExecDelete(SfxRequest &){DBG_BF_ASSERT(0, "STRIP");} ;//STRIP001 void        ExecDelete(SfxRequest &);

	void        ExecClpbrd(SfxRequest &){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void        ExecClpbrd(SfxRequest &);
	void        StateClpbrd(SfxItemSet &);

	void        ExecUndo(SfxRequest &){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void        ExecUndo(SfxRequest &);
	void        StateUndo(SfxItemSet &);

	void        Execute(SfxRequest &){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void        Execute(SfxRequest &);
	void        GetState(SfxItemSet &);
	void        StateStyle(SfxItemSet &);

	void		ExecuteGallery(SfxRequest&){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void		ExecuteGallery(SfxRequest&);
	void		GetGalleryState(SfxItemSet&){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void		GetGalleryState(SfxItemSet&);

	void        ExecDlg(SfxRequest &){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void        ExecDlg(SfxRequest &);

	void        StateStatusLine(SfxItemSet &rSet);
	void        ExecTxtCtrl(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} ;//STRIP001 	void        ExecTxtCtrl(SfxRequest& rReq);
	void        GetTxtFontCtrlState(SfxItemSet& rSet);
	void        GetTxtCtrlState(SfxItemSet& rSet);
	void 		GetBorderState(SfxItemSet &rSet){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void 		GetBorderState(SfxItemSet &rSet);
	void        GetBckColState(SfxItemSet &rSet);

 	void        ExecBckCol(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void        ExecBckCol(SfxRequest& rReq);
	void		SetWrapMode( USHORT nSlot );

	void		StateDisableItems(SfxItemSet &){DBG_BF_ASSERT(0, "STRIP");};//STRIP001 void		StateDisableItems(SfxItemSet &);

	void		EditRegionDialog(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP"); };//STRIP001 void		EditRegionDialog(SfxRequest& rReq);
	void		InsertRegionDialog(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP"); };//STRIP001 void		InsertRegionDialog(SfxRequest& rReq);

	void		ExecField(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 void		ExecField(SfxRequest& rReq);

	static void   _SetFrmMode( USHORT nMode )   { nFrameMode = nMode; }
	static USHORT  GetFrmMode()                 { return nFrameMode;  }

	//public fuer D&D

};


} //namespace binfilter
#endif
