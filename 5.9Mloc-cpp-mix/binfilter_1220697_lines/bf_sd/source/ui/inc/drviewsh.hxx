/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: drviewsh.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:25:41 $
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

#ifndef _SD_DRVIEWSH_HXX
#define _SD_DRVIEWSH_HXX

#ifndef _VIEWFAC_HXX //autogen
#include <bf_sfx2/viewfac.hxx>
#endif
#ifndef _SFXVIEWSH_HXX //autogen
#include <bf_sfx2/viewsh.hxx>
#endif

#ifndef _SD_TABCONTR_HXX
#include "tabcontr.hxx"
#endif
#ifndef _SD_LAYERTAB_HXX
#include "layertab.hxx"
#endif
#ifndef _SD_VIEWSHEL_HXX
#include "viewshel.hxx"
#endif
#ifndef _PRESENTATION_HXX
#include "pres.hxx"
#endif

#ifndef _COM_SUN_STAR_LANG_XEVENTLISTENER_HPP_
#include <com/sun/star/lang/XEventListener.hpp>
#endif
#ifndef _COM_SUN_STAR_SCANNER_XSCANNERMANAGER_HPP_
#include <com/sun/star/scanner/XScannerManager.hpp>
#endif
class TransferableDataHelper;
class TransferableClipboardListener;
namespace binfilter {

class SdPage;
class SdDrawView;
class SdDrawDocShell;
class SdAnimationWin;
class SdRuler;
class TabBar;
class SdrObject;
class XPolygon;
class SdrPageView;
class SdUnoDrawView;
class SvxNameDialog;
class SdrLayer;

#define SLOTARRAY_COUNT 	24
#define CHECK_RANGE(nMin, nValue, nMax) ((nValue >= nMin) && (nValue <= nMax))

/*************************************************************************
|*
|* Ableitung von SdViewShell fuer Drawing
|*
\************************************************************************/

class SdDrawViewShell : public SdViewShell
{
private:

	::com::sun::star::uno::Reference< ::com::sun::star::scanner::XScannerManager >	mxScannerManager;
	::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >		mxScannerListener;
	TransferableClipboardListener*                                                  pClipEvtLstnr;
	BOOL			                                                                bPastePossible;

protected:

	SdDrawView* 	pDrView;
	SdPage* 		pActualPage;
	SdUnoDrawView*	pController;
	UINT16          nLastSlot;
	Rectangle		aMarkRect;
	Point			aMousePos;
	BOOL			bMousePosFreezed;
	SdTabControl	aTabControl;
	SdLayerTab		aLayerTab;
	ImageButton 	aPageBtn;
	ImageButton 	aMasterPageBtn;
	ImageButton 	aLayerBtn;
	EditMode		eEditMode;
	BOOL			bLayerMode;
	PageKind		ePageKind;
	BOOL			bZoomOnPage;
	BOOL			bIsRulerDrag;
	XPolygon*		pXPolygon;
	USHORT			nPolygonIndex;
	BOOL			bLineError;
	BOOL			bLastWasLineTo;
	BOOL			bLastWasMoveTo;
	BOOL			bLastWasBezierTo;
    Color			aGradStartColor;
	Color			aGradEndColor;
	Color			aHatchColor;
	ULONG			nLockCount;
	Timer			aCloseTimer;
	BOOL 			bReadOnly;
	BOOL			bInEffectAssignment;
	USHORT*			pSlotArray;

    static BOOL 	bPipette;
	
	void	        Construct(SdDrawDocShell* pDocSh);

	                DECL_LINK( TabSplitHdl, TabBar * );
	                DECL_LINK( TabModeBtnHdl, Button * );


	virtual void    UpdateHRuler();
	virtual void    UpdateVRuler();

	virtual void    SFX_NOTIFY(SfxBroadcaster& rBC, const TypeId& rBCType,
							   const SfxHint& rHint, const TypeId& rHintType);

	void            DestroyPolygons();


	USHORT	        GetIdBySubId( USHORT nSId );
	void	        MapSlot( USHORT nSId );
	void	        UpdateToolboxImages( SfxItemSet &rSet, BOOL bPermanent = TRUE );
	USHORT	        GetArrayId( USHORT nSId );
                    
	void	        GetMenuStateSel(SfxItemSet& rSet);

 public:

	                TYPEINFO();

	                SFX_DECL_VIEWFACTORY(SdDrawViewShell);
	                SFX_DECL_INTERFACE(SD_IF_SDDRAWVIEWSHELL);

	                SdDrawViewShell(SfxViewFrame* pFrame, const SdDrawViewShell& rShell);
	                SdDrawViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell);
	                ~SdDrawViewShell();


    /** Set the position and size of the area which contains the GUI
        elements like rulers, sliders, and buttons as well as the document
        view.  Both size and position are expected to be in pixel
        coordinates.  The positions and sizes of the mentioned GUI elements
        are updated as well.

        <p> This method is implemented by first setting copying the given
        values to internal variables and then calling the
        <type>ArrangeGUIElements</type> method which performs the actual
        work of sizeing and arranging the UI elements accordingly.</p>
        @param rPos
            The position of the enclosing window relative to the document
            window.  This is only interesting if a Draw/Impress document
            view is embedded as OLE object into another document view.  For
            normal documents this position is (0,0).
        @param rSize
            The new size in pixel.
    */

    /** Arrange and resize the GUI elements like rulers, sliders, and
        buttons as well as the actual document view according to the size of
        the enclosing window and current sizes of buttons, rulers, and
        sliders.
    */
    
    void 	        HidePage(SdrPageView* pPV);

                    



	void	        ChangeEditMode(EditMode eMode, BOOL bLMode);




	void		    SelectionHasChanged();
	void		    ModelHasChanged();
	virtual void    Deactivate(BOOL IsMDIActivate);

	void	        ExecCtrl(SfxRequest& rReq);
	void	        GetCtrlState(SfxItemSet& rSet);
	void	        GetMenuState(SfxItemSet& rSet);
	void	        GetAttrState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001  void	        GetAttrState(SfxItemSet& rSet);
	void	        GetSnapItemState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetSnapItemState(SfxItemSet& rSet);
                    
	void	        GetState (SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void	        GetState (SfxItemSet& rSet);
    void            Execute (SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void            Execute (SfxRequest& rReq);

	void	        ExecStatusBar(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void	        ExecStatusBar(SfxRequest& rReq);
	void	        GetStatusBarState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void	        GetStatusBarState(SfxItemSet& rSet);
                    
	void	        ExecOptionsBar(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecOptionsBar(SfxRequest& rReq);
	void	        GetOptionsBarState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetOptionsBarState(SfxItemSet& rSet);
                    
	void	        ExecRuler(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void	        ExecRuler(SfxRequest& rReq);
	void	        GetRulerState(SfxItemSet& rSet) {DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 ;	void	        GetRulerState(SfxItemSet& rSet);
                    
	void	        ExecFormText(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecFormText(SfxRequest& rReq);
	void	        GetFormTextState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetFormTextState(SfxItemSet& rSet);
                    
                    
	void	        ExecAnimationWin(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecAnimationWin(SfxRequest& rReq);
	void	        GetAnimationWinState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetAnimationWinState(SfxItemSet& rSet);
                    
	void	        ExecNavigatorWin(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecNavigatorWin(SfxRequest& rReq);
	void	        GetNavigatorWinState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetNavigatorWinState(SfxItemSet& rSet);
                    
	void	        ExecEffectWin(SfxRequest& rReq);
	void	        GetEffectWinState(SfxItemSet& rSet);
	void	        UpdateEffectWindow();
                    
	void	        Update3DWindow();
                    
	void	        ExecGallery(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecGallery(SfxRequest& rReq);
	void	        GetGalleryState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetGalleryState(SfxItemSet& rSet);
                    
	void	        ExecBmpMask( SfxRequest& rReq ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecBmpMask( SfxRequest& rReq );
	void	        GetBmpMaskState( SfxItemSet& rSet ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetBmpMaskState( SfxItemSet& rSet );
                    
	void	        ExecIMap( SfxRequest& rReq ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        ExecIMap( SfxRequest& rReq );
	void	        GetIMapState( SfxItemSet& rSet ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        GetIMapState( SfxItemSet& rSet );
                    
	void	        FuTemporary(SfxRequest& rReq);
	void	        FuPermanent(SfxRequest& rReq);
	void	        FuSupport(SfxRequest& rReq) {DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 void	        FuSupport(SfxRequest& rReq);
                    
	void	        AttrExec (SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        AttrExec (SfxRequest& rReq);
	void	        AttrState (SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	void	        AttrState (SfxItemSet& rSet);
                    

	virtual USHORT  PrepareClose( BOOL bUI = TRUE, BOOL bForBrowsing = FALSE );

	PageKind	    GetPageKind() { return ePageKind; }

	Point		    GetMousePos() { return aMousePos; }
	BOOL 		    IsMousePosFreezed() { return bMousePosFreezed; }
	void 		    SetMousePosFreezed( BOOL bIn ) { bMousePosFreezed = bIn; }
                    
	EditMode	    GetEditMode() const { return eEditMode; }
	virtual SdPage*	GetActualPage() { return pActualPage; }

	void		    ResetActualPage();
	void		    ResetActualLayer();
	BOOL		    SwitchPage(USHORT nPage);
	BOOL		    IsSwitchPageAllowed() const;
                    
                    
	virtual void    ReadFrameViewData(FrameView* pView);
	virtual void    WriteFrameViewData();


	void		    SetZoomOnPage( BOOL bZoom = TRUE ) { bZoomOnPage = bZoom; }
	BOOL		    IsZoomOnPage() { return bZoomOnPage; }
	void		    CheckLineTo (SfxRequest& rReq);
	void		    FuTemp01(SfxRequest& rReq);
	void		    FuTemp02(SfxRequest& rReq);
	void		    FuTemp03(SfxRequest& rReq);
	void		    FuTemp04(SfxRequest& rReq);
	void		    SetChildWindowState( SfxItemSet& rSet );
                    
                    
	BOOL		    IsInputLocked() const { return nLockCount > 0UL; }
                    
	USHORT		    GetCurPageId() { return( aTabControl.GetCurPageId() ); }
                    
                    

	SdUnoDrawView*  GetController() const { return pController; }
	BOOL			GetLayerMode() const { return bLayerMode; }

	USHORT*	    	GetSlotArray() const { return pSlotArray; }


    virtual void    WriteUserDataSequence ( ::com::sun::star::uno::Sequence < ::com::sun::star::beans::PropertyValue >&, sal_Bool bBrowse = sal_False );

	virtual void    VisAreaChanged(const Rectangle& rRect);

    /** Create an accessible object representing the specified window.
	    @param pWindow
	        The returned object makes the document displayed in this window
	        accessible.
        @return
	        Returns an <type>AccessibleDrawDocumentView</type> object.
   */

    /** Return the number of layers managed by the layer tab control.  This
        will usually differ from the number of layers managed by the layer
        administrator.
        @return
            The number of layers managed by the layer tab control.  The
            returned value is independent of whether the layer modus is
            currently active and the tab control is visible.
    */
    virtual int GetTabLayerCount (void) const;

    /** Return the numerical id of the currently active layer as seen by the
        layer tab control.
        @return
            The returned id is a number between zero (inclusive) and the
            number of layers as returned by the
            <member>GetTabLayerCount</member> method (exclusive).
    */
    virtual int GetActiveTabLayerIndex (void) const;
    
    /** Set the active layer at the layer tab control and update the control
        accordingly to reflect the change on screen.
        @param nId
            The id is expected to be a number between zero (inclusive) and
            the number of layers as returned by the
            <member>GetTabLayerCount</member> method (exclusive).  Note that
            Invalid values are ignored.  No excpetion is thrown in that case.
    */
    virtual void SetActiveTabLayerIndex (int nId);

    /** Return a pointer to the tab control for pages.
    */
	SdTabControl* GetPageTabControl (void);

    /** Return a pointer to the tab control for layers.
    */
	SdLayerTab* GetLayerTabControl (void);

    /** Renames the given slide using an SvxNameDialog

        @param nPageId the index of the page in the SdTabControl.
        @param rName the new name of the slide.

        @return false, if the new name is invalid for some reason.

        <p>Implemented in <code>drviews8.cxx</code>.</p>
     */

	/** modifies the given layer with the given values */
	void ModifyLayer( SdrLayer* pLayer, String& rLayerName, bool bIsVisible, bool bIsLocked, bool bIsPrintable );

private:
    /** Depending on the given request create a new page or duplicate an
        existing one.
        @param rReq
            The request as passed to 
    */
    void CreateOrDuplicatePage (SfxRequest& rReq);
};

} //namespace binfilter
#endif		// _SD_DRVIEWSH_HXX
