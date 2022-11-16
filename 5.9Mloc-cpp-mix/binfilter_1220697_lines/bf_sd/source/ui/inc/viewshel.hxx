/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: viewshel.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: obo $ $Date: 2007/07/17 10:07:28 $
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

#ifndef _SD_VIEWSHEL_HXX
#define _SD_VIEWSHEL_HXX


#ifndef _IMAGEBTN_HXX //autogen
#include <vcl/imagebtn.hxx>
#endif
#ifndef _SPLIT_HXX //autogen
#include <vcl/split.hxx>
#endif
#ifndef _SV_FIELD_HXX //autogen
#include <vcl/field.hxx>
#endif
#ifndef _SFXVIEWSH_HXX //autogen
#include <bf_sfx2/viewsh.hxx>
#endif
#ifndef _SV_FIELD_HXX //autogen
#include <vcl/field.hxx>
#endif
#ifndef _SV_PRNTYPES_HXX //autogen
#include <vcl/prntypes.hxx>
#endif
#ifndef _TRANSFER_HXX //autogen
#include <svtools/transfer.hxx>
#endif

#ifndef _SD_GLOB_HXX
#include "glob.hxx"
#endif
#include "pres.hxx"
#include "cfgids.hxx"

#include <memory>
#include <tools/table.hxx>

class ScrollBarBox;
class ScrollBar;
class MultiSelection;
namespace binfilter {

class FuPoor;
class SdView;
class SdWindow;
class SdPage;
class SvxRuler;
class FrameView;
class SdrOle2Obj;		// fuer die, die Teile von SVDRAW rausdefiniert haben
class FuSearch;
class SdDrawDocShell;
class SdDrawDocument;
class SdClient;
class FuSlideShow;
class ZoomList;
class FmFormShell;

class SdOptionsPrintItem;

/************************************************************************/

#define MAX_HSPLIT_CNT		2
#define MAX_VSPLIT_CNT		2
#define MIN_SCROLLBAR_SIZE	50

/************************************************************************/

extern String aEmptyStr;

/************************************************************************/

#define OUTPUT_DRAWMODE_COLOR		(DRAWMODE_DEFAULT)
#define OUTPUT_DRAWMODE_GRAYSCALE   (DRAWMODE_GRAYLINE | DRAWMODE_GRAYFILL | DRAWMODE_BLACKTEXT | DRAWMODE_GRAYBITMAP | DRAWMODE_GRAYGRADIENT)
#define OUTPUT_DRAWMODE_BLACKWHITE  (DRAWMODE_BLACKLINE | DRAWMODE_BLACKTEXT | DRAWMODE_WHITEFILL | DRAWMODE_GRAYBITMAP | DRAWMODE_WHITEGRADIENT)
#define OUTPUT_DRAWMODE_CONTRAST	(DRAWMODE_SETTINGSLINE | DRAWMODE_SETTINGSFILL | DRAWMODE_SETTINGSTEXT | DRAWMODE_SETTINGSGRADIENT)

namespace sd {
class WindowUpdater;
}


/*************************************************************************
|*
|* Basisklasse zur Verwaltung des Fenster-Interfaces
|*
\************************************************************************/

class SdViewShell : public SfxViewShell
{

	void	Construct();

    /** This window updater is used to keep all relevant windows up to date
        with reference to the digit langugage used to display digits in text
        shapes.
    */
	::std::auto_ptr< ::binfilter::sd::WindowUpdater> mpWindowUpdater;

protected:
	// #96090# Support methods for centralized UNDO/REDO

 protected:
	// zweidimensionales Zeigerarray (X/Y) mit Fensterzeigern fuer alle
	// moeglichen Splitter-Aufteilungen
	SdWindow*	pWinArray[MAX_HSPLIT_CNT][MAX_VSPLIT_CNT];
	// Zeigerarrays fuer die ScrollBars
	ScrollBar*	pHScrlArray[MAX_HSPLIT_CNT];
	ScrollBar*	pVScrlArray[MAX_VSPLIT_CNT];
	// Zeigerarrays fuer die Lineale
	SvxRuler*	pHRulerArray[MAX_HSPLIT_CNT];
	SvxRuler*	pVRulerArray[MAX_VSPLIT_CNT];
	ScrollBarBox* pScrlBox;

	BOOL		bIsHSplit;
	BOOL		bIsVSplit;
	BOOL		bHasRuler;
	// aktives Fenster
	SdWindow*	pWindow;
	SdView* 	pView;
	FrameView*	pFrameView;
	SdDrawDocShell* pDocSh;
	SdDrawDocument* pDoc;

	FuPoor* 	 pFuActual;
	FuPoor* 	 pFuOld;
	FuSearch*	 pFuSearch;
	FuSlideShow* pFuSlideShow;
	ZoomList*	 pZoomList;

	Splitter	aHSplit;
	Splitter	aVSplit;

	ImageButton aDrawBtn;
	ImageButton aOutlineBtn;
	ImageButton aSlideBtn;
	ImageButton aNotesBtn;
	ImageButton aHandoutBtn;
	ImageButton aPresentationBtn;

	Point		aViewPos;
	Size		aViewSize;
	Size		aScrBarWH;

	Table		aShellTable;			  // fuer die ObjectBarShells
	USHORT		nCurrentObjectBar;		  // ResourceID des aktuellen Object Bar
	BOOL		bObjectBarSwitchEnabled;  // zum Unterdruecken des Switchens

	BOOL		bCenterAllowed; 		  // wird an Fenster weitergegeben

	BOOL		bStartShowWithDialog;	// Praesentation wurde ueber Dialog gestartet
	USHORT      nPrintedHandoutPageNum; // Seitennummer der zu durckenden Handzettelseite

	BOOL		bPrintDirectSelected;		// Print only selected objects in direct print
	String		sPageRange;					// pagerange if selected objects in direct print

	// Bereich aller Fenster, wenn Splitter aktiv sind
	Rectangle	aAllWindowRect;

	DECL_LINK( SplitHdl, Splitter * );
	DECL_LINK( HScrollHdl, ScrollBar * );
	DECL_LINK( VScrollHdl, ScrollBar * );
	DECL_LINK( ModeBtnHdl, Button * );

	// virt. Scroll-Handler, hier koennen sich abgeleitete Klassen einklinken

	// virtuelle Funktionen fuer Lineal-Handling
	virtual SvxRuler* CreateHRuler(SdWindow* pWin, BOOL bIsFirst) { return NULL; }
	virtual SvxRuler* CreateVRuler(SdWindow* pWin) { return NULL; }
	virtual void UpdateHRuler() {}
	virtual void UpdateVRuler() {}

	// Zeiger auf ein zusaetzliches Control im horizontalen ScrollBar
	// abgeleiteter Klassen (z.B. ein TabBar) zurueckgeben
	virtual long GetHCtrlWidth() { return 0; }

	virtual void Deactivate(BOOL IsMDIActivate);







 public:
	TYPEINFO();

	// SFX_DECL_INTERFACE(SD_IF_SDVIEWSHELL); SOH

	 SdViewShell(SfxViewFrame *pFrame, Window *pParent,
				 BOOL bAllowCenter = TRUE);
	SdViewShell(SfxViewFrame *pFrame, const SdViewShell& rShell);
	~SdViewShell();

	void Cancel();
	void CancelSearching();

	virtual SdView*  GetView() const { return (pView); }
	virtual SdrView* GetDrawView() const { return ( (SdrView*) pView); } // fuer den Sfx
	SdDrawDocShell*  GetDocSh() { return pDocSh; }
	SdDrawDocument*  GetDoc() { return pDoc; }
	FmFormShell*	 GetFormShell() { return( ( FmFormShell*) aShellTable.Get( RID_FORMLAYER_TOOLBOX ) ); }

	// diese Funktionen werden gerufen, wenn Fenster gesplittet wird
	// bzw., wenn Splitting aufgehoben wird
	virtual void AddWindow(SdWindow* pWin) {}
	virtual void RemoveWindow(SdWindow* pWin) {}

	SdWindow* GetActiveWindow() { return pWindow; }
	void	  SetActiveWindow(SdWindow* pWindow);


	// Mouse- & Key-Events
	virtual void Paint(const Rectangle& rRect, SdWindow* pWin) {}

	virtual void Draw(OutputDevice &rDev, const Region &rReg) {}

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

	BOOL	HasRuler() { return bHasRuler; }

	void	UpdateScrollBars();
	void	InitWindows(const Point& rViewOrigin, const Size& rViewSize,
						const Point& rWinPos, BOOL bUpdate = FALSE);
	void	InvalidateWindows();
	virtual void 	UpdatePreview( SdPage* pPage, BOOL bInit = FALSE );
	USHORT  SwitchObjectBar(USHORT nSdResId);
	USHORT	GetObjectBar() { return nCurrentObjectBar; }


	ZoomList*	  GetZoomList() { return pZoomList; }

	FrameView*	  GetFrameView() { return pFrameView; }
	virtual void  WriteFrameViewData();


	virtual SdPage*	GetActualPage() = 0;

	virtual SfxPrinter*     GetPrinter(BOOL bCreate = FALSE);


	USHORT					SetPrinterOptDlg(SfxPrinter* pNewPrinter,
											 USHORT nDiffFlags = SFX_PRINTER_ALL,
											 BOOL _bShowDialog = TRUE){DBG_BF_ASSERT(0, "STRIP"); return 0; } //STRIP001 BOOL _bShowDialog = TRUE);
					// kann auch NULL sein
	FuPoor* GetOldFunction() const	  { return pFuOld; }
	FuPoor* GetActualFunction() const { return pFuActual; }
	FuSlideShow* GetSlideShow() const { return pFuSlideShow; }


	void	UpdateSlideChangeWindow();

	void	SetStartShowWithDialog( BOOL bIn = TRUE ) { bStartShowWithDialog = bIn; }
	BOOL	IsStartShowWithDialog() const { return bStartShowWithDialog; }

	USHORT  GetPrintedHandoutPageNum() const { return nPrintedHandoutPageNum; }

	virtual USHORT PrepareClose( BOOL bUI = TRUE, BOOL bForBrowsing = FALSE );

	void GetMenuState(SfxItemSet& rSet);


    virtual void WriteUserDataSequence ( ::com::sun::star::uno::Sequence < ::com::sun::star::beans::PropertyValue >&, sal_Bool bBrowse = sal_False );

	/** this method is called when the visible area of the view from this viewshell is changed */
	virtual void VisAreaChanged(const Rectangle& rRect);

    /** Create an accessible object representing the specified window.
	    Overload this method to provide view mode specific objects.  The
	    default implementation returns an empty reference.
        @param pWindow
            Make the document displayed in this window accessible.
        @return 
            This default implementation returns an empty reference.
    */

	/** makes the SdPreviewChildWindow visible or invisible
		@param bVisible	if set to true, the preview is shown, if set to false, the preview is hidden
	*/
	void SetPreview( bool bVisible );

	void SetWinViewPos(const Point& rWinPos, bool bUpdate){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void SetWinViewPos(const Point& rWinPos, bool bUpdate);
	Point GetWinViewPos() const{DBG_BF_ASSERT(0, "STRIP"); Point a; return a;} //STRIP001 Point GetWinViewPos() const;
	Point GetViewOrigin() const{DBG_BF_ASSERT(0, "STRIP"); Point a; return a;} //STRIP001 Point GetViewOrigin() const;

    /** Return the window updater of this view shell.
        @return
            In rare circumstances the returned pointer may be <null/>,
            i.e. when no memory is available anymore.
    */
};

} //namespace binfilter
#endif			// _SD_VIEWSHEL_HXX
