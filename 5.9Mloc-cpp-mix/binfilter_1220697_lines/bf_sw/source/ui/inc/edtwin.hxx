/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: edtwin.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 00:39:21 $
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
#ifndef _EDTWIN_HXX
#define _EDTWIN_HXX

#ifndef _LINK_HXX
#include <tools/link.hxx>
#endif
#ifndef _TIMER_HXX
#include <vcl/timer.hxx>
#endif
#ifndef _WINDOW_HXX
#include <vcl/window.hxx>
#endif
#ifndef _TRANSFER_HXX
#include <svtools/transfer.hxx>
#endif

#ifndef _SWEVENT_HXX
#include <swevent.hxx>
#endif

#define _SVSTDARR_STRINGSISORTDTOR
#include <svtools/svstdarr.hxx>
namespace binfilter {

class	SwWrtShell;
class 	SwView;
class 	SwRect;
class 	SwDrawBase;
class	ViewShell;
class	SdrViewUserMarker;
class	SdrObject;
class	SwShadowCursor;
class	DataChangedEvent;
class   OfaAutoCorrCfg;
class   SvxAutoCorrect;
struct	SwApplyTemplate;
struct 	QuickHelpData;

/*--------------------------------------------------------------------
	Beschreibung:	Eingabe-Fenster
 --------------------------------------------------------------------*/

class SwEditWin: public Window,
				public DropTargetHelper, public DragSourceHelper
{
friend void 	ScrollMDI(ViewShell* pVwSh, const SwRect&,
						  USHORT nRangeX, USHORT nRangeY);
friend BOOL 	IsScrollMDI(ViewShell* pVwSh, const SwRect&);

friend void 	SizeNotify(ViewShell* pVwSh, const Size &);

friend void 	PageNumNotify( 	ViewShell* pVwSh,
								USHORT nPhyNum,
								USHORT nVirtNum,
								const String& rPg );

	static  QuickHelpData* pQuickHlpData;

	static	BOOL	bReplaceQuote;
	static	long 	nDDStartPosX, nDDStartPosY;

	static 	Color	aTextColor; 	//Textfarbe, fuer die Giesskanne
	static 	BOOL	bTransparentBackColor; // Hintergrund transparent
	static 	Color	aTextBackColor; //Texthintergrundfarbe, fuer die Giesskanne

	/*
	 * Timer und Handler fuer das Weiterscrollen, wenn der
	 * Mauspointer innerhalb eines Drag-Vorgangs ausserhalb des
	 * EditWin stehen bleibt.  In regelmaessigen Intervallen wird
	 * die Selektion in Richtung der Mausposition vergroessert.
	 */
	AutoTimer 		aTimer;
	// Timer fuer verschachtelte KeyInputs (z.B. fuer Tabellen)
	Timer 			aKeyInputTimer;
	// timer for ANY-KeyInut question without a following KeyInputEvent
	Timer 			aKeyInputFlushTimer;

	String			aInBuffer;
	Point			aStartPos;
	Point			aMovePos;
	Point			aRszMvHdlPt;
	Timer 			aTemplateTimer;

	// Type/Objecte ueber dem der MousePointer steht
	SwCallMouseEvent aSaveCallEvent;

	SwApplyTemplate 	*pApplyTempl;

	SdrViewUserMarker	*pUserMarker;
	SdrObject			*pUserMarkerObj;

	SwView		   &rView;

	int		 		aActHitType;	// aktueller Mauspointer

	ULONG 			nDropFormat;	//Format aus dem letzten QueryDrop
	USHORT			nDropAction;	//Action aus dem letzten QueryDrop
	USHORT			nDropDestination;	//Ziel aus dem letzten QueryDrop

	UINT16			eDrawMode;
	UINT16			eBezierMode;
	UINT16			nInsFrmColCount; //Spaltenzahl fuer interaktiven Rahmen
	BOOL			bLinkRemoved	: 1,
					bMBPressed		: 1,
					bInsDraw 		: 1,
					bInsFrm 		: 1,
					bIsInMove		: 1,
					bIsInDrag		: 1, //StartExecuteDrag nich doppelt ausfuehren
					bOldIdle		: 1, //Zum abschalten des Idle'ns
					bOldIdleSet		: 1, //waehrend QeueryDrop
					bTblInsDelMode	: 1, //
					bTblIsInsMode	: 1, //
					bTblIsColMode	: 1, //
					bChainMode		: 1, //Rahmen verbinden
					bWasShdwCrsr	: 1, //ShadowCrsr war im MouseButtonDown an
					bLockInput		: 1; //Lock waehrend die Rechenleiste aktiv ist





	//Hilfsfunktionen fuer D&D

	/*
	 * Handler fuer das Weiterscrollen, wenn der Mauspointer innerhalb eines
	 * Drag-Vorgangs ausserhalb des EditWin stehen bleibt. In regelmaessigen
	 * Intervallen wird die Selektion in Richtung der Mausposition
	 * vergroessert.
	 */
	DECL_LINK( TimerHandler, Timer * );

	// timer for ANY-KeyInut question without a following KeyInputEvent
	DECL_LINK( KeyInputFlushHandler, Timer * );

	// Timer fuer verschachtelte KeyInputs (z.B. fuer Tabellen)
	DECL_LINK( KeyInputTimerHandler, Timer * );

	// Timer fuer das ApplyTemplates per Maus (verkapptes Drag&Drop)
	DECL_LINK( TemplateTimerHdl, Timer* );

protected:






								// Drag & Drop Interface

    void    ShowAutoTextCorrectQuickHelp( const String& rWord, OfaAutoCorrCfg* pACfg, SvxAutoCorrect* pACorr );
public:

	void			UpdatePointer(const Point &, USHORT nButtons = 0);

	BOOL			IsDrawAction() 					{ return (bInsDraw); }
	void			SetDrawAction(BOOL bFlag) 		{ bInsDraw = bFlag; }
	inline UINT16	GetDrawMode(BOOL bBuf = FALSE) const { return eDrawMode; }
	inline void		SetDrawMode(UINT16 eDrwMode)	{ eDrawMode = eDrwMode; }
	BOOL			IsFrmAction() 					{ return (bInsFrm); }
	inline UINT16	GetBezierMode() 				{ return eBezierMode; }
	void			SetBezierMode(UINT16 eBezMode)	{ eBezierMode = eBezMode; }
	UINT16			GetFrmColCount() const {return nInsFrmColCount;} //Spaltenzahl fuer interaktiven Rahmen


	BOOL			IsChainMode() const				{ return bChainMode; }


	static	void 	SetReplaceQuote(BOOL bOn = TRUE) { bReplaceQuote = bOn; }
	static	BOOL 	IsReplaceQuote() { return bReplaceQuote; }

	SwApplyTemplate* GetApplyTemplate() const { return pApplyTempl; }

	USHORT			GetDropAction() const { return nDropAction; }
	ULONG			GetDropFormat() const { return nDropFormat; }

	Color 			GetTextColor() { return aTextColor; }
	void 			SetTextColor(const Color& rCol ) { aTextColor = rCol; }

	Color 			GetTextBackColor()
											{ return aTextBackColor; }
	void 			SetTextBackColor(const Color& rCol )
											{ aTextBackColor = rCol; }
	void			SetTextBackColorTransparent(BOOL bSet)
									{ bTransparentBackColor = bSet; }
	BOOL			IsTextBackColorTransparent()
									{ return bTransparentBackColor; }
	void			LockKeyInput(BOOL bSet){bLockInput = bSet;}

	const SwView &GetView() const { return rView; }
		  SwView &GetView() 	  { return rView; }

#ifdef ACCESSIBLE_LAYOUT
#endif

	// Tipfenster loeschen

	static inline long GetDDStartPosX() { return nDDStartPosX; }
	static inline long GetDDStartPosY() { return nDDStartPosY; }

	static void	_InitStaticData();
	static void	_FinitStaticData();
    
    //#i3370# remove quick help to prevent saving of autocorrection suggestions
    void StopQuickHelp();

	SwEditWin(Window *pParent, SwView &);
	virtual ~SwEditWin();
};


} //namespace binfilter
#endif

