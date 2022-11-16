/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fupoor.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:26:50 $
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

#ifndef _SD_FUPOOR_HXX
#define _SD_FUPOOR_HXX

#ifndef _RTTI_HXX //autogen
#include <tools/rtti.hxx>
#endif
#ifndef _SV_TIMER_HXX //autogen
#include <vcl/timer.hxx>
#endif
#ifndef _SV_EVENT_HXX //autogen
#include <vcl/event.hxx>
#endif
class Dialog;
namespace binfilter {

class SdView;
class SdViewShell;
class SdWindow;
class SdDrawDocument;
class SdDrawDocShell;
class SfxRequest;
class SdrObject;

#define HITPIX	  2 				   // Hit-Toleranz in Pixel
#define DRGPIX	  2 				   // Drag MinMove in Pixel


/*************************************************************************
|*
|* Basisklasse fuer alle Funktionen
|*
\************************************************************************/

class FuPoor
{
 protected:
	SdView* 		pView;
	SdViewShell*	pViewShell;
	SdWindow*		pWindow;
	SdDrawDocShell* pDocSh;
	SdDrawDocument* pDoc;

	USHORT			nSlotId;
	USHORT			nSlotValue;

	Dialog* 		pDialog;

	Timer			aScrollTimer;			// fuer Autoscrolling
	DECL_LINK( ScrollHdl, Timer * );

	Timer			aDragTimer; 			// fuer Drag&Drop
	DECL_LINK( DragHdl, Timer * );
	BOOL			bIsInDragMode;
	Point			aMDPos; 				// Position von MouseButtonDown

	// Flag, um AutoScrolling zu verhindern, bis von ausserhalb in das
	// Fenster hinein gedragt wurde
	BOOL			bNoScrollUntilInside;

	// Timer um das scrolling zu verzoegern, wenn aus dem fenster
	// herausgedraggt wird (ca. 1 sec.)
	Timer			aDelayToScrollTimer;	// fuer Verzoegerung bis scroll
	BOOL			bScrollable;
	BOOL			bDelayActive;
	BOOL			bFirstMouseMove;

	// #95491# member to hold state of the mouse buttons for creation
	// of own MouseEvents (like in ScrollHdl)
private:
	sal_uInt16		mnCode;

protected:
 	DECL_LINK( DelayHdl, Timer * );

 public:
	TYPEINFO();

	FuPoor(SdViewShell* pViewSh, SdWindow* pWin, SdView* pView,
		   SdDrawDocument* pDoc, SfxRequest& rReq);
	virtual ~FuPoor();

	// #95491# see member
	void SetMouseButtonCode(sal_uInt16 nNew) { if(nNew != mnCode) mnCode = nNew; }
	const sal_uInt16 GetMouseButtonCode() const { return mnCode; }

	SdDrawDocShell* GetDocSh() { return pDocSh; }
	SdDrawDocument* GetDoc() { return pDoc; }


	// Mouse- & Key-Events; Returnwert=TRUE: Event wurde bearbeitet
	virtual BOOL MouseMove(const MouseEvent& rMEvt) { return FALSE; }

	// #95491# moved from inline to *.cxx
	
	virtual void Paint(const Rectangle& rRect, SdWindow* pWin) {}
	virtual void ReceiveRequest(SfxRequest& rReq);

	virtual void Activate();		// Function aktivieren
	virtual void Deactivate();		// Function deaktivieren

	virtual void ScrollStart() {}	// diese Funktionen werden von
	virtual void ScrollEnd() {} 	// ForceScroll aufgerufen

	void SetWindow(SdWindow* pWin) { pWindow = pWin; }

	// #97016# II
	virtual void SelectionHasChanged();

	USHORT	GetSlotID() const { return( nSlotId ); }
	USHORT	GetSlotValue() const { return( nSlotValue ); }

	void	SetNoScrollUntilInside(BOOL bNoScroll = TRUE)
			{ bNoScrollUntilInside = bNoScroll; }


	// #97016#

	/** is called when the currenct function should be aborted. <p>
		This is used when a function gets a KEY_ESCAPE but can also
		be called directly.

		@returns true if a active function was aborted
	*/

protected:

    /** Switch to another layer.  The layer to switch to is specified by an
        offset relative to the active layer.  With respect to the layer bar
        control at the lower left of the document window positive values
        move to the right and negative values move to the left.
    
        <p>Switching the layer is independant of the view's layer mode.  The
        layers are switched even when the layer mode is turned off and the
        layer control is not visible.</p>
        @param nOffset
           If the offset is positive skip that many layers in selecting the
           next layer.  If it is negative then select a previous one.  An
           offset or zero does not change the current layer.  If the
           resulting index lies outside the valid range of indices then it
           is set to either the minimal or maximal valid index, whitchever
           is nearer.
    */
};



} //namespace binfilter
#endif		// _SD_FUPOOR_HXX

