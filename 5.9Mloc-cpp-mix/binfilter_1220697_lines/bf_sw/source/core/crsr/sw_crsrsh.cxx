/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_crsrsh.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:54:06 $
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



#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifndef _HINTIDS_HXX
#include <hintids.hxx>
#endif


#ifdef BIDI
#ifndef _SVX_FRMDIRITEM_HXX
#include <bf_svx/frmdiritem.hxx>
#endif
#endif

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _ROOTFRM_HXX
#include <rootfrm.hxx>
#endif
#ifndef _PAGEFRM_HXX
#include <pagefrm.hxx>
#endif
#ifndef _VIEWIMP_HXX
#include <viewimp.hxx>
#endif
#ifndef _FLYFRM_HXX
#include <flyfrm.hxx>
#endif
#ifndef _DVIEW_HXX
#include <dview.hxx>
#endif
#ifndef _VIEWOPT_HXX
#include <viewopt.hxx>
#endif
#ifndef _CRSRSH_HXX
#include <crsrsh.hxx>
#endif
#ifndef _TXTFRM_HXX
#include <txtfrm.hxx>
#endif
#ifndef _CALLNK_HXX
#include <callnk.hxx>
#endif
#ifndef _VISCRS_HXX
#include <viscrs.hxx>
#endif
#ifndef _SECTION_HXX
#include <section.hxx>
#endif
#ifndef _SWGLOBDOCSH_HXX //autogen
#include <globdoc.hxx>
#endif
#ifndef _FMTEIRO_HXX //autogen
#include <fmteiro.hxx>
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::com::sun::star::util;

/*N*/ TYPEINIT2(SwCrsrShell,ViewShell,SwModify);


// Funktion loescht, alle ueberlappenden Cursor aus einem Cursor-Ring
/*N*/ void CheckRange( SwCursor* );

//-----------------------------------------------------------------------

/*
 * Ueberpruefe ob der pCurCrsr in einen schon bestehenden Bereich zeigt.
 * Wenn ja, dann hebe den alten Bereich auf.
 */


/*N*/ void CheckRange( SwCursor* pCurCrsr )
/*N*/ {
/*N*/ 	const SwPosition *pStt = pCurCrsr->Start(),
/*N*/ 		*pEnd = pCurCrsr->GetPoint() == pStt ? pCurCrsr->GetMark() : pCurCrsr->GetPoint();
/*N*/ 
/*N*/ 	SwPaM *pTmpDel = 0,
/*N*/ 		  *pTmp = (SwPaM*)pCurCrsr->GetNext();
/*N*/ 
/*N*/ 	// durchsuche den gesamten Ring
/*N*/ 	while( pTmp != pCurCrsr )
/*N*/ 	{
/*?*/ 		const SwPosition *pTmpStt = pTmp->Start(),
/*?*/ 						*pTmpEnd = pTmp->GetPoint() == pTmpStt ?
/*?*/ 										pTmp->GetMark() : pTmp->GetPoint();
/*?*/ 		if( *pStt <= *pTmpStt )
/*?*/ 		{
/*?*/ 			if( *pEnd > *pTmpStt ||
/*?*/ 				( *pEnd == *pTmpStt && *pEnd == *pTmpEnd ))
/*?*/ 				pTmpDel = pTmp;
/*?*/ 		}
/*?*/ 		else
/*?*/ 			if( *pStt < *pTmpEnd )
/*?*/ 				pTmpDel = pTmp;
		/*
		 * liegt ein SPoint oder GetMark innerhalb vom Crsr-Bereich
		 * muss der alte Bereich aufgehoben werden.
		 * Beim Vergleich ist darauf zu achten, das SPoint nicht mehr zum
		 * Bereich gehoert !
		 */
/*?*/ 		pTmp = (SwPaM*)pTmp->GetNext();
/*?*/ 		if( pTmpDel )
/*?*/ 		{
/*?*/ 			delete pTmpDel;         // hebe alten Bereich auf
/*?*/ 			pTmpDel = 0;
/*?*/ 		}
/*N*/ 	}
/*N*/ }

// -------------- Methoden von der SwCrsrShell -------------


// gebe den aktuellen zurueck

/*N*/ SwPaM* SwCrsrShell::GetCrsr( FASTBOOL bMakeTblCrsr ) const
/*N*/ {
/*N*/ 	if( pTblCrsr )
/*N*/ 	{
DBG_BF_ASSERT(0, "STRIP"); //STRIP001  /*?*/ 		if( bMakeTblCrsr && pTblCrsr->IsCrsrMovedUpdt() )
/*N*/ 	}
/*N*/ 	return pCurCrsr;
/*N*/ }


/*N*/ void SwCrsrShell::StartAction()
/*N*/ {
/*N*/ 	if( !ActionPend() )
/*N*/ 	{
/*N*/ 		// fuer das Update des Ribbon-Bars merken
/*N*/ 		const SwNode& rNd = pCurCrsr->GetPoint()->nNode.GetNode();
/*N*/ 		nAktNode = rNd.GetIndex();
/*N*/ 		nAktCntnt = pCurCrsr->GetPoint()->nContent.GetIndex();
/*N*/ 		nAktNdTyp = rNd.GetNodeType();
/*N*/       bAktSelection = *pCurCrsr->GetPoint() != *pCurCrsr->GetMark();
/*N*/ 		if( ND_TEXTNODE & nAktNdTyp )
/*N*/ 			nLeftFrmPos = SwCallLink::GetFrm( (SwTxtNode&)rNd, nAktCntnt, TRUE );
/*N*/ 		else
/*?*/ 			nLeftFrmPos = 0;
/*N*/ 	}
/*N*/ 	ViewShell::StartAction();           // zur ViewShell
/*N*/ }


/*N*/ void SwCrsrShell::EndAction( const BOOL bIdleEnd )
/*N*/ {
/*
//OS: Wird z.B. eine Basic-Action im Hintergrund ausgefuehrt, geht es so nicht
	if( !bHasFocus )
	{
		// hat die Shell nicht den Focus, dann nur das EndAction an
		// die ViewShell weitergeben.
		ViewShell::EndAction( bIdleEnd );
		return;
	}
*/

/*N*/ 	FASTBOOL bVis = bSVCrsrVis;

	// Idle-Formatierung ?
/*N*/ 	if( bIdleEnd && Imp()->GetRegion() )
/*N*/ 	{
/*?*/ 		pCurCrsr->Hide();

/*?*/ #ifdef SHOW_IDLE_REGION
/*?*/ if( GetWin() )
/*?*/ {
/*?*/ 	GetWin()->Push();
/*?*/ 	GetWin()->ChangePen( Pen( Color( COL_YELLOW )));
/*?*/ 	for( USHORT n = 0; n < aPntReg.Count(); ++n )
/*?*/ 	{
/*?*/ 		SwRect aIRect( aPntReg[n] );
/*?*/ 		GetWin()->DrawRect( aIRect.SVRect() );
/*?*/ 	}
/*?*/ 	GetWin()->Pop();
/*?*/ }
/*?*/ #endif

/*N*/ 	}

	// vor der letzten Action alle invaliden Numerierungen updaten
/*N*/ 	if( 1 == nStartAction )
/*N*/ 		GetDoc()->UpdateNumRule();
/*N*/ 
/*N*/ 	// Task: 76923: dont show the cursor in the ViewShell::EndAction() - call.
/*N*/ 	//				Only the UpdateCrsr shows the cursor.
/*N*/ 	BOOL bSavSVCrsrVis = bSVCrsrVis;
/*N*/ 	bSVCrsrVis = FALSE;
/*N*/ 
/*N*/ 	ViewShell::EndAction( bIdleEnd );	//der ViewShell den Vortritt lassen
/*N*/ 
/*N*/ 	bSVCrsrVis = bSavSVCrsrVis;
/*N*/ 
/*N*/ 	if( ActionPend() )
/*N*/ 	{
/*N*/ 		if( bVis )    // auch SV-Cursor wieder anzeigen
/*N*/ 			pVisCrsr->Show();
/*N*/ 
/*N*/ 		// falls noch ein ChgCall vorhanden ist und nur noch die Basic
/*N*/ 		// Klammerung vorhanden ist, dann rufe ihn. Dadurch wird die interne
/*N*/ 		// mit der Basic-Klammerung entkoppelt; die Shells werden umgeschaltet
/*N*/ 		if( !BasicActionPend() )
/*N*/ 		{
/*?*/ 			//JP 12.01.98: Bug #46496# - es muss innerhalb einer BasicAction
/*?*/ 			//				der Cursor geupdatet werden; um z.B. den
/*?*/ 			//				TabellenCursor zu erzeugen. Im UpdateCrsr wird
/*?*/ 			//				das jetzt beruecksichtigt!
DBG_BF_ASSERT(0, "STRIP"); //STRIP001  /*?*/ 			UpdateCrsr( SwCrsrShell::CHKRANGE, bIdleEnd );
/*N*/ 		}
/*N*/ 		return;
/*N*/ 	}

/*N*/ 	USHORT nParm = SwCrsrShell::CHKRANGE;
/*N*/ 	if ( !bIdleEnd )
/*N*/ 		nParm |= SwCrsrShell::SCROLLWIN;
/*N*/ 	UpdateCrsr( nParm, bIdleEnd );		// Cursor-Aenderungen anzeigen
/*N*/ 
/*N*/ 	{
/*N*/ 		SwCallLink aLk( *this );        // Crsr-Moves ueberwachen,
/*N*/ 		aLk.nNode = nAktNode;           // evt. Link callen
/*N*/ 		aLk.nNdTyp = (BYTE)nAktNdTyp;
/*N*/ 		aLk.nCntnt = nAktCntnt;
/*N*/ 		aLk.nLeftFrmPos = nLeftFrmPos;
/*N*/ 
/*N*/ 		if( !nCrsrMove ||
/*N*/ 			( 1 == nCrsrMove && bInCMvVisportChgd ) )
/*N*/ 			ShowCrsrs( bSVCrsrVis ? TRUE : FALSE );    // Cursor & Selektionen wieder anzeigen
/*N*/ 	}
/*N*/ 	// falls noch ein ChgCall vorhanden ist, dann rufe ihn
/*N*/ 	if( bCallChgLnk && bChgCallFlag && aChgLnk.IsSet() )
/*N*/ 	{
/*N*/ 		aChgLnk.Call( this );
/*N*/ 		bChgCallFlag = FALSE;		// Flag zuruecksetzen
/*N*/ 	}
/*N*/ }


/*?*/ #if !defined( PRODUCT )
/*?*/ 
/*?*/ void SwCrsrShell::SttCrsrMove()
/*?*/ {
/*?*/ 	ASSERT( nCrsrMove < USHRT_MAX, "To many nested CrsrMoves." );
/*?*/ 	++nCrsrMove;
/*?*/ 	StartAction();
/*?*/ }
/*?*/ 
/*?*/ void SwCrsrShell::EndCrsrMove( const BOOL bIdleEnd )
/*?*/ {
/*?*/ 	ASSERT( nCrsrMove, "EndCrsrMove() ohne SttCrsrMove()." );
/*?*/ 	EndAction( bIdleEnd );
/*?*/ 	if( !--nCrsrMove )
/*?*/ 		bInCMvVisportChgd = FALSE;
/*?*/ }
/*?*/ 
/*?*/ #endif


/*N*/ void SwCrsrShell::UpdateCrsrPos()
/*N*/ {
/*N*/ 	SET_CURR_SHELL( this );
/*N*/ 	++nStartAction;
/*N*/ 	Size aOldSz( GetLayout()->Frm().SSize() );
/*N*/ 	SwCntntNode *pCNode = pCurCrsr->GetCntntNode();
/*N*/ 	SwCntntFrm  *pFrm = pCNode ?
/*N*/ 		pCNode->GetFrm( &pCurCrsr->GetPtPos(), pCurCrsr->GetPoint() ) :0;
/*N*/ 	if( !pFrm || (pFrm->IsTxtFrm() && ((SwTxtFrm*)pFrm)->IsHiddenNow()) )
/*N*/ 	{
/*?*/ 		SwCrsrMoveState aTmpState( MV_NONE );
/*?*/ 		aTmpState.bSetInReadOnly = IsReadOnlyAvailable();
/*?*/ 		GetLayout()->GetCrsrOfst( pCurCrsr->GetPoint(), pCurCrsr->GetPtPos(),
/*?*/ 									 &aTmpState );
/*?*/ 		if( pCurCrsr->HasMark())
/*?*/ 			pCurCrsr->DeleteMark();
/*N*/ 	}
/*N*/ 	--nStartAction;
/*N*/ 	if( aOldSz != GetLayout()->Frm().SSize() )
/*N*/ 		SizeChgNotify( GetLayout()->Frm().SSize() );
/*N*/ }



// JP 30.04.99: Bug 65475 - falls Point/Mark in versteckten Bereichen
//				stehen, so mussen diese daraus verschoben werden

/*M*/ void SwCrsrShell::UpdateCrsr( USHORT eFlags, BOOL bIdleEnd )
/*M*/ {
/*M*/ 	SET_CURR_SHELL( this );
/*M*/ 
/*N*/     ClearUpCrsrs();
/*M*/ 
/*M*/ 	// erfrage den Count fuer die Start-/End-Actions und ob die Shell
/*M*/ 	// ueberhaupt den Focus hat
/*M*/ //	if( ActionPend() /*|| !bHasFocus*/ )
/*M*/ 	//JP 12.01.98: Bug #46496# - es muss innerhalb einer BasicAction der
/*M*/ 	//				Cursor geupdatet werden; um z.B. den TabellenCursor zu
/*M*/ 	//				erzeugen. Im EndAction wird jetzt das UpdateCrsr gerufen!
/*M*/ 	if( ActionPend() && BasicActionPend() )
/*M*/ 	{
/*M*/ 		if ( eFlags & SwCrsrShell::READONLY )
/*M*/ 			bIgnoreReadonly = TRUE;
/*M*/ 		return;             // wenn nicht, dann kein Update !!
/*M*/ 	}
/*M*/ 
/*M*/ 	if ( bIgnoreReadonly )
/*M*/ 	{
/*M*/ 		bIgnoreReadonly = FALSE;
/*M*/ 		eFlags |= SwCrsrShell::READONLY;
/*M*/ 	}
/*M*/ 
/*M*/ 	if( eFlags & SwCrsrShell::CHKRANGE )	// alle Cursor-Bewegungen auf
/*M*/ 		CheckRange( pCurCrsr );     	// ueberlappende Bereiche testen
/*M*/ 
/*M*/ 	if( !bIdleEnd )
/*M*/ 		CheckTblBoxCntnt();
/*M*/ 
/*M*/ 	// steht der akt. Crsr in einer Tabelle und in unterschiedlichen Boxen
/*M*/ 	// (oder ist noch TabellenMode), dann gilt der Tabellen Mode
/*M*/ 	SwPaM* pTstCrsr = pTblCrsr ? pTblCrsr : pCurCrsr;
/*M*/ 	if( ( pTstCrsr->HasMark() &&
/*M*/ 		  pDoc->IsIdxInTbl( pTstCrsr->GetPoint()->nNode ) &&
/*M*/ 		  ( pTblCrsr ||
/*M*/ 			pTstCrsr->GetNode( TRUE )->FindStartNode() !=
/*M*/ 			pTstCrsr->GetNode( FALSE )->FindStartNode() ))
/*M*/ 		/*|| ( !pTblCrsr && lcl_IsInValueBox( *pTstCrsr, *this ) )*/ )
/*M*/ 	{DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*M*/ 	}
/*M*/ 
/*M*/ 	if( pTblCrsr )
/*M*/ 	{
/*M*/ 		// Cursor Ring loeschen
/*M*/ 		while( pCurCrsr->GetNext() != pCurCrsr )
/*M*/ 			delete pCurCrsr->GetNext();
/*M*/ 		pCurCrsr->DeleteMark();
/*M*/ 		*pCurCrsr->GetPoint() = *pTblCrsr->GetPoint();
/*M*/ 		pCurCrsr->GetPtPos() = pTblCrsr->GetPtPos();
/*M*/ 		delete pTblCrsr, pTblCrsr = 0;
/*M*/ 	}
/*M*/ 
/*M*/ 	pVisCrsr->Hide();       // sichtbaren Cursor immer verstecken
/*M*/ 
/*M*/ 	// sind wir vielleicht in einer geschuetzten/versteckten Section ?
/*M*/ 	{
/*M*/ 		BOOL bChgState = TRUE;
/*M*/ 		const SwSectionNode* pSectNd = pCurCrsr->GetNode()->FindSectionNode();
/*M*/ 		if( pSectNd && ( pSectNd->GetSection().IsHiddenFlag() ||
/*M*/ 			( !IsReadOnlyAvailable() &&
/*M*/ 			  pSectNd->GetSection().IsProtectFlag() &&
/*M*/ 			 ( !pDoc->GetDocShell() ||
/*M*/ 			   !pDoc->GetDocShell()->IsReadOnly() || bAllProtect )) ) )
/*M*/ 		{DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*M*/ 		}
/*M*/ 		if( bChgState )
/*M*/ 		{
/*M*/ 			BOOL bWasAllProtect = bAllProtect;
/*M*/ 			bAllProtect = FALSE;
/*M*/ 			if( bWasAllProtect && GetDoc()->GetDocShell() &&
/*M*/ 				GetDoc()->GetDocShell()->IsReadOnlyUI() )
/*M*/ 			{DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*M*/ 			}
/*M*/ 		}
/*M*/ 	}
/*M*/ 
/*M*/ 	UpdateCrsrPos();
/*M*/ 
/*M*/ 
/*M*/     // #100722# The cursor must always point into content; there's some code
/*M*/     // that relies on this. (E.g. in SwEditShell::GetScriptType, which always
/*M*/     // loops _behind_ the last node in the selection, which always works if you
/*M*/     // are in content.) To achieve this, we'll force cursor(s) to point into
/*M*/     // content, if UpdateCrsrPos() hasn't already done so.
/*M*/     SwPaM* pCmp = pCurCrsr;
/*M*/     do
/*M*/     {
/*M*/         // start will move forwards, end will move backwards
/*M*/         bool bPointIsStart = ( pCmp->Start() == pCmp->GetPoint() );
/*M*/ 
/*M*/         // move point; forward if it's the start, backwards if it's the end
/*M*/         if( ! pCmp->GetPoint()->nNode.GetNode().IsCntntNode() )
/*M*/             pCmp->Move( bPointIsStart ? fnMoveForward : fnMoveBackward,
/*M*/                         fnGoCntnt );
/*M*/ 
/*M*/         // move mark (if exists); forward if it's the start, else backwards
/*M*/         if( pCmp->HasMark() )
/*M*/         {
/*M*/             if( ! pCmp->GetMark()->nNode.GetNode().IsCntntNode() )
/*M*/             {
/*M*/                 pCmp->Exchange();
/*M*/                 pCmp->Move( !bPointIsStart ? fnMoveForward : fnMoveBackward,
/*M*/                             fnGoCntnt );
/*M*/                 pCmp->Exchange();
/*M*/             }
/*M*/         }
/*M*/ 
/*M*/         // iterate to next PaM in ring
/*M*/         pCmp = static_cast<SwPaM*>( pCmp->GetNext() );
/*M*/     }
/*M*/     while( pCmp != pCurCrsr );
/*M*/ 
/*M*/ 
/*M*/ 	SwRect aOld( aCharRect );
/*M*/ 	FASTBOOL bFirst = TRUE;
/*M*/ 	SwCntntFrm *pFrm;
/*M*/ 	int nLoopCnt = 100;
/*M*/ 
/*M*/ 	do {
/*M*/ 		BOOL bAgainst;
/*M*/ 		do {
/*M*/ 			bAgainst = FALSE;
/*M*/ 			pFrm = pCurCrsr->GetCntntNode()->GetFrm(
/*M*/ 						&pCurCrsr->GetPtPos(), pCurCrsr->GetPoint() );
/*M*/ 			// ist der Frm nicht mehr vorhanden, dann muss das gesamte Layout
/*M*/ 			// erzeugt werden, weil ja mal hier einer vorhanden war !!
/*M*/ 			if ( !pFrm )
/*M*/ 			{
/*M*/ 				do
/*M*/ 				{
/*M*/ 					CalcLayout();
/*M*/ 					pFrm = pCurCrsr->GetCntntNode()->GetFrm(
/*M*/ 								&pCurCrsr->GetPtPos(), pCurCrsr->GetPoint() );
/*M*/ 				}  while( !pFrm );
/*M*/ 			}
/*M*/ 			else if ( Imp()->IsIdleAction() )
/*M*/ 				//Wir stellen sicher, dass anstaendig Formatiert wurde #42224#
/*?*/ 				{DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 pFrm->PrepareCrsr();
/*M*/ 
/*M*/ 			// im geschuetzten Fly? aber bei Rahmenselektion ignorieren
/*M*/ 			if( !IsReadOnlyAvailable() && pFrm->IsProtected() &&
/*M*/ 				( !Imp()->GetDrawView() ||
/*M*/ 				  !Imp()->GetDrawView()->GetMarkList().GetMarkCount() ) &&
/*M*/ 				(!pDoc->GetDocShell() ||
/*M*/ 				 !pDoc->GetDocShell()->IsReadOnly() || bAllProtect ) )
/*M*/ 			{DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*M*/ 			}
/*M*/ 		} while( bAgainst );
/*M*/ 
/*M*/ 		if( !( eFlags & SwCrsrShell::NOCALRECT ))
/*M*/ 		{
/*M*/ 			SwCrsrMoveState aTmpState( eMvState );
/*M*/ 			aTmpState.bSetInReadOnly = IsReadOnlyAvailable();
/*M*/             aTmpState.bRealHeight = TRUE;
/*M*/ 			aTmpState.bRealWidth = IsOverwriteCrsr();
/*N*/             aTmpState.nCursorBidiLevel = pCurCrsr->GetCrsrBidiLevel();
/*M*/ 			if( !pFrm->GetCharRect( aCharRect, *pCurCrsr->GetPoint(), &aTmpState ) )
/*M*/ 			{ 
/*N*/ 				Point& rPt = pCurCrsr->GetPtPos();
/*N*/ 				rPt = aCharRect.Center();
/*N*/ 				pFrm->GetCrsrOfst( pCurCrsr->GetPoint(), rPt, &aTmpState );
/*N*/ #ifndef VERTICAL_LAYOUT
/*N*/                 if ( !pFrm->GetCharRect(aCharRect, *pCurCrsr->GetPoint(), &aTmpState) )
/*N*/ 					ASSERT( !this, "GetCharRect failed." );
/*N*/ #endif
/*M*/ 			}
/*M*/ //			ALIGNRECT( aCharRect );
/*M*/ 
/*M*/             if( !pCurCrsr->HasMark() )
/*M*/ 				aCrsrHeight = aTmpState.aRealHeight;
/*M*/ 			else
/*M*/ 			{
/*M*/ 				aCrsrHeight.X() = 0;
/*M*/                 aCrsrHeight.Y() = aTmpState.aRealHeight.Y() < 0 ?
/*M*/                                   -aCharRect.Width() : aCharRect.Height();
/*M*/ 			}
/*M*/ 		}
/*M*/ 		else
/*M*/ 		{
/*M*/ 			aCrsrHeight.X() = 0;
/*M*/ 			aCrsrHeight.Y() = aCharRect.Height();
/*M*/ 		}
/*M*/ 
/*M*/ 		if( !bFirst && aOld == aCharRect )
/*M*/ 			break;
/*M*/ 
/*M*/ 		// falls das Layout meint, nach dem 100 durchlauf ist man immer noch
/*M*/ 		// im Fluss, sollte man die akt. Pos. als gegeben hinnehmen!
/*M*/ 		// siehe Bug: 29658
/*M*/ 		if( !--nLoopCnt )
/*M*/ 		{
/*M*/ 			ASSERT( !this, "Endlosschleife? CharRect != OldCharRect ");
/*M*/ 			break;
/*M*/ 		}
/*M*/ 		aOld = aCharRect;
/*M*/ 		bFirst = FALSE;
/*M*/ 
/*M*/ 		// Cursor-Points auf die neuen Positionen setzen
/*M*/ 		pCurCrsr->GetPtPos().X() = aCharRect.Left();
/*M*/ 		pCurCrsr->GetPtPos().Y() = aCharRect.Top();
/*M*/ 
/*M*/ 		if( !(eFlags & SwCrsrShell::UPDOWN ))	// alte Pos. von Up/Down loeschen
/*M*/ 		{
/*M*/ 			pFrm->Calc();
/*M*/ #ifdef VERTICAL_LAYOUT
/*M*/             nUpDownX = pFrm->IsVertical() ?
/*M*/                        aCharRect.Top() - pFrm->Frm().Top() :
/*M*/                        aCharRect.Left() - pFrm->Frm().Left();
/*M*/ #else
/*M*/ 			nUpDownX = aCharRect.Left() - pFrm->Frm().Left();
/*M*/ #endif
/*M*/ 		}
/*M*/ 
/*M*/ 		// Curosr in den sichtbaren Bereich scrollen
/*M*/         if( bHasFocus && eFlags & SwCrsrShell::SCROLLWIN &&
/*M*/ 			(HasSelection() || eFlags & SwCrsrShell::READONLY ||
/*M*/              !IsCrsrReadonly() || GetViewOptions()->IsSelectionInReadonly()) )
/*M*/ 		{
/*M*/ 			//JP 30.04.99:  damit das EndAction, beim evtuellen Scrollen, den
/*M*/ 			//		SV-Crsr nicht wieder sichtbar macht, wird hier das Flag
/*M*/ 			//		gesichert und zurueckgesetzt.
/*M*/ 			BOOL bSav = bSVCrsrVis; bSVCrsrVis = FALSE;
/*M*/ 			MakeSelVisible();
/*M*/ 			bSVCrsrVis = bSav;
/*M*/ 		}
/*M*/ 
/*M*/ 	} while( eFlags & SwCrsrShell::SCROLLWIN );
/*M*/ 
/*M*/ 	if( !bIdleEnd && bHasFocus && !bBasicHideCrsr )
/*M*/ 		pCurCrsr->SwSelPaintRects::Show();
/*M*/ 
/*M*/ 	//Ggf. gescrollten Bereicht korrigieren (Alignment).
/*M*/ 	//Nur wenn gescrollt wurde, und wenn keine Selektion existiert.
/*M*/ 	if( pFrm && Imp()->IsScrolled() &&
/*M*/ 			pCurCrsr->GetNext() == pCurCrsr && !pCurCrsr->HasMark() )
/*?*/ 		{DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 Imp()->RefreshScrolledArea( aCharRect );
/*M*/ 
/*M*/ 
/*M*/ 	eMvState = MV_NONE;		// Status fuers Crsr-Travelling - GetCrsrOfst
/*M*/ 
/*M*/ #ifdef ACCESSIBLE_LAYOUT
/*M*/ #endif
/*M*/ 
/*M*/ #ifndef REMOTE_APPSERVER
/*M*/ 
/*M*/     // switch from blinking cursor to read-only-text-selection cursor
/*M*/     long nBlinkTime = GetOut()->GetSettings().GetStyleSettings().
/*M*/                       GetCursorBlinkTime();
/*M*/ 
/*M*/     if ( (IsCrsrReadonly() && GetViewOptions()->IsSelectionInReadonly()) ==
/*M*/         ( nBlinkTime != STYLE_CURSOR_NOBLINKTIME ) )
/*M*/     {
/*M*/         // non blinking cursor in read only - text selection mode
/*M*/         AllSettings aSettings = GetOut()->GetSettings();
/*M*/         StyleSettings aStyleSettings = aSettings.GetStyleSettings();
/*M*/         long nNewBlinkTime = nBlinkTime == STYLE_CURSOR_NOBLINKTIME ?
/*M*/                              500 :
/*M*/                              STYLE_CURSOR_NOBLINKTIME;
/*M*/         aStyleSettings.SetCursorBlinkTime( nNewBlinkTime );
/*M*/         aSettings.SetStyleSettings( aStyleSettings );
/*M*/         GetOut()->SetSettings( aSettings );
/*M*/     }
/*M*/ 
/*M*/ #endif
/*M*/ 
/*M*/ 	if( bSVCrsrVis )
/*M*/ 		pVisCrsr->Show();           // wieder anzeigen
/*M*/ }



// erzeuge eine Kopie vom Cursor und speicher diese im Stack



/*
 *  Loescht einen Cursor (gesteuert durch bOldCrsr)
 *      - vom Stack oder    ( bOldCrsr = TRUE )
 *      - den aktuellen und der auf dem Stack stehende wird zum aktuellen
 *
 *  Return:  es war auf dem Stack noch einer vorhanden
 */



/*
 * Verbinde zwei Cursor miteinander.
 * Loesche vom Stack den obersten und setzen dessen GetMark im Aktuellen.
 */







/*N*/ void SwCrsrShell::ShowCrsrs( BOOL bCrsrVis )
/*N*/ {
/*N*/ 	if( !bHasFocus || bAllProtect || bBasicHideCrsr )
/*?*/ 		return;

/*N*/ 	SET_CURR_SHELL( this );
/*N*/ 	SwShellCrsr* pAktCrsr = pTblCrsr ? pTblCrsr : pCurCrsr;
/*N*/ 	pAktCrsr->Show();
/*N*/ 
/*N*/ 	if( bSVCrsrVis && bCrsrVis )    // auch SV-Cursor wieder anzeigen
/*N*/ 		pVisCrsr->Show();
/*N*/ }


/*N*/ SwCntntFrm *SwCrsrShell::GetCurrFrm( const BOOL bCalcFrm ) const
/*N*/ {
/*N*/ 	SET_CURR_SHELL( (ViewShell*)this );
/*N*/ 	SwCntntFrm *pRet = 0;
/*N*/ 	SwCntntNode *pNd = pCurCrsr->GetCntntNode();
/*N*/ 	if ( pNd )
/*N*/ 	{
/*N*/ 		if ( bCalcFrm )
/*N*/ 		{
/*N*/ 			const USHORT* pST = &nStartAction;
/*N*/ 			++(*((USHORT*)pST));
/*N*/ 			const Size aOldSz( GetLayout()->Frm().SSize() );
/*N*/ 			pRet = pNd->GetFrm( &pCurCrsr->GetPtPos(), pCurCrsr->GetPoint() );
/*N*/ 			--(*((USHORT*)pST));
/*N*/ 			if( aOldSz != GetLayout()->Frm().SSize() )
/*N*/ 				((SwCrsrShell*)this)->SizeChgNotify( GetLayout()->Frm().SSize() );
/*N*/ 		}
/*N*/ 		else
/*N*/ 			pRet = pNd->GetFrm( &pCurCrsr->GetPtPos(), pCurCrsr->GetPoint(), FALSE);
/*N*/ 	}
/*N*/ 	return pRet;
/*N*/ }


// alle Attribut/Format-Aenderungen am akt. Node werden an den
// Link weitergeleitet.


/*N*/ void SwCrsrShell::Modify( SfxPoolItem* pOld, SfxPoolItem* pNew )
/*N*/ {
/*N*/ 	USHORT nWhich = pOld ? pOld->Which()
/*N*/ 						 : pNew ? pNew->Which()
/*N*/ 						 		: RES_MSG_BEGIN;
/*N*/ 	if( bCallChgLnk &&
/*N*/ 		( nWhich < RES_MSG_BEGIN || nWhich >= RES_MSG_END ||
/*N*/ 			nWhich == RES_FMT_CHG || nWhich == RES_UPDATE_ATTR ||
/*N*/ 			nWhich == RES_ATTRSET_CHG ))
/*N*/ 		// die Messages werden nicht weitergemeldet
/*N*/ 		//MA 07. Apr. 94 fix(6681): RES_UPDATE_ATTR wird implizit vom
/*N*/ 		//SwTxtNode::Insert(SwTxtHint*, USHORT) abgesetzt; hier wird reagiert und
/*N*/ 		//vom Insert brauch nicht mehr die Keule RES_FMT_CHG versandt werden.
/*N*/ 		CallChgLnk();
/*N*/ 
/*N*/ 	if( aGrfArrivedLnk.IsSet() &&
/*N*/ 		( RES_GRAPHIC_ARRIVED == nWhich || RES_GRAPHIC_SWAPIN == nWhich ))
/*?*/ 		aGrfArrivedLnk.Call( this );
/*N*/ }


// Abfrage, ob der aktuelle Cursor eine Selektion aufspannt,
// also, ob GetMark gesetzt und SPoint und GetMark unterschiedlich sind.


/*N*/ FASTBOOL SwCrsrShell::HasSelection() const
/*N*/ {
/*N*/ 	SwPaM* pCrsr = IsTableMode() ? pTblCrsr : pCurCrsr;
/*N*/ 	return( IsTableMode() || ( pCurCrsr->HasMark() &&
/*N*/ 			*pCurCrsr->GetPoint() != *pCrsr->GetMark())
/*N*/ 		? TRUE : FALSE );
/*N*/ }


/*N*/ void SwCrsrShell::CallChgLnk()
/*N*/ {
/*N*/ 	// innerhalb von Start-/End-Action kein Call, sondern nur merken,
/*N*/ 	// das sich etwas geaendert hat. Wird bei EndAction beachtet.
/*N*/ 	if( BasicActionPend() )
/*N*/ 		bChgCallFlag = TRUE;		// das Change merken
/*N*/ 	else if( aChgLnk.IsSet() )
/*N*/ 	{
/*N*/ 		if( bCallChgLnk )
/*N*/ 			aChgLnk.Call( this );
/*N*/ 		bChgCallFlag = FALSE;		// Flag zuruecksetzen
/*N*/ 	}
/*N*/ }

// returne den am akt.Cursor selektierten Text eines Nodes.



// gebe nur den Text ab der akt. Cursor Position zurueck (bis zum NodeEnde)



// retrurne die Anzahl der selektierten Zeichen.
// Falls keine Selektion vorliegt entscheided nType was selektiert wird
// bIntrnlChar besagt ob interne Zeichen erhalten bleiben (TRUE) oder
// ob sie expandiert werden (z.B Felder/...)


// hole vom Start/Ende der akt. SSelection das nte Zeichen

// erweiter die akt. SSelection am Anfang/Ende um n Zeichen



// setze nur den sichtbaren Cursor an die angegebene Dokument-Pos.
// returnt FALSE: wenn der SPoint vom Layout korrigiert wurde.




	// returne die Anzahl der Cursor im Ring (Flag besagt ob man nur
	// aufgepspannte haben will - sprich etwas selektiert ist (Basic))
/*N*/ USHORT SwCrsrShell::GetCrsrCnt( BOOL bAll ) const
/*N*/ {
/*N*/ 	Ring* pTmp = GetCrsr()->GetNext();
/*N*/ 	USHORT n = (bAll || ( pCurCrsr->HasMark() &&
/*N*/ 					*pCurCrsr->GetPoint() != *pCurCrsr->GetMark())) ? 1 : 0;
/*N*/ 	while( pTmp != pCurCrsr )
/*N*/ 	{
/*?*/ 		if( bAll || ( ((SwPaM*)pTmp)->HasMark() &&
/*?*/ 				*((SwPaM*)pTmp)->GetPoint() != *((SwPaM*)pTmp)->GetMark()))
/*?*/ 			++n;
/*?*/ 		pTmp = pTmp->GetNext();
/*N*/ 	}
/*N*/ 	return n;
/*N*/ }






// loesche alle erzeugten Crsr, setze den Tabellen-Crsr und den letzten
// Cursor auf seinen TextNode (oder StartNode?).
// Beim naechsten ::GetCrsr werden sie wieder alle erzeugt
// Wird fuers Drag&Drop / ClipBorad-Paste in Tabellen benoetigt.

/***********************************************************************
#*	Class		:  SwCrsrShell
#*	Methode 	:  ParkCrsr
#*	Beschreibung:  Vernichtet Selektionen und zus. Crsr aller Shell der
#*				   verbleibende Crsr der Shell wird geparkt.
#*	Datum		:  MA 05. Nov. 92
#*	Update		:  JP 19.09.97
#***********************************************************************/


/*
 * der normale Constructor
 */

/*N*/ SwCrsrShell::SwCrsrShell( SwDoc& rDoc, Window *pWin, SwRootFrm *pRoot,
/*N*/ 							const SwViewOption *pOpt )
/*N*/ 	: ViewShell( rDoc, pWin, pOpt ),
/*N*/ 	SwModify( 0 )
/*N*/ {
/*N*/ 	SET_CURR_SHELL( this );
	/*
 	 * Erzeugen des initialen Cursors, wird auf die erste
	 * Inhaltsposition gesetzt
	 */
/*N*/ 	SwNodes& rNds = rDoc.GetNodes();
/*N*/ 
/*N*/ 	SwNodeIndex aNodeIdx( *rNds.GetEndOfContent().StartOfSectionNode() );
/*N*/ 	SwCntntNode* pCNd = rNds.GoNext( &aNodeIdx ); // gehe zum 1. ContentNode
/*N*/ 
/*N*/ 	pCurCrsr = new SwShellCrsr( *this, SwPosition( aNodeIdx, SwIndex( pCNd, 0 )));
/*N*/ 	pCrsrStk = 0;
/*N*/ 	pTblCrsr = 0;
/*N*/ 
/*N*/ 	nBasicActionCnt = 0;
/*N*/ 
/*N*/ 	pBoxIdx = 0;
/*N*/ 	pBoxPtr = 0;
/*N*/ 
/*N*/ 	// melde die Shell beim akt. Node als abhaengig an, dadurch koennen alle
/*N*/ 	// Attribut-Aenderungen ueber den Link weiter gemeldet werden.
/*N*/ 	pCNd->Add( this );
/*N*/ 
 	/*
 	 * setze die initiale Spalten-Position fuer Up / Down
	 */
/*N*/ 	nCrsrMove = 0;
/*N*/ 	bAllProtect = bVisPortChgd = bChgCallFlag = bInCMvVisportChgd =
/*N*/ 	bGCAttr = bIgnoreReadonly = bSelTblCells = bBasicHideCrsr =
/*N*/ 	bOverwriteCrsr = FALSE;
/*N*/ 	bCallChgLnk = bHasFocus = bSVCrsrVis = bAutoUpdateCells = TRUE;
/*N*/ 	bSetCrsrInReadOnly = TRUE;
/*N*/ 	eMvState = MV_NONE;		// Status fuers Crsr-Travelling - GetCrsrOfst
/*N*/ 
/*N*/ 	pVisCrsr = new SwVisCrsr( this );
/*N*/ //	UpdateCrsr( 0 );
/*N*/     // OD 11.02.2003 #100556#
/*N*/     mbMacroExecAllowed = true;
/*N*/ }



/*N*/ SwCrsrShell::~SwCrsrShell()
/*N*/ {
/*N*/ 	// wenn es nicht die letzte View so sollte zu mindest das
/*N*/ 	// Feld noch geupdatet werden.
/*N*/ 	if( GetNext() != this )
/*?*/ 		CheckTblBoxCntnt( pCurCrsr->GetPoint() );
/*N*/ 	else
/*N*/ 		ClearTblBoxCntnt();
/*N*/ 
/*N*/ 	delete pVisCrsr;
/*N*/ 	delete pTblCrsr;
/*N*/ 
 	/*
	 * Freigabe der Cursor
 	 */
/*N*/ 	while(pCurCrsr->GetNext() != pCurCrsr)
/*?*/ 		delete pCurCrsr->GetNext();
/*N*/ 	delete pCurCrsr;
/*N*/ 
/*N*/ 	// Stack freigeben
/*N*/ 	if( pCrsrStk )
/*N*/ 	{
/*?*/ 		while( pCrsrStk->GetNext() != pCrsrStk )
/*?*/ 			delete pCrsrStk->GetNext();
/*?*/ 		delete pCrsrStk;
/*N*/ 	}

	// JP 27.07.98: Bug 54025 - ggfs. den HTML-Parser, der als Client in
	// 				der CursorShell haengt keine Chance geben, sich an den
	//				TextNode zu haengen.
/*N*/ 	if( GetRegisteredIn() )
/*N*/ 		pRegisteredIn->Remove( this );
/*N*/ }



//Sollte fuer das Clipboard der WaitPtr geschaltet werden?
//Warten bei TableMode, Mehrfachselektion und mehr als x Selektieren Absaetzen.





// steht der Curor auf einem "Symbol"-Zeichen


// zeige das akt. selektierte "Object" an
/*N*/ void SwCrsrShell::MakeSelVisible()
/*N*/ {
/*N*/ 	ASSERT( bHasFocus, "kein Focus aber Cursor sichtbar machen?" );
/*N*/ 	if( aCrsrHeight.Y() < aCharRect.Height() && aCharRect.Height() > VisArea().Height() )
/*N*/ 	{
/*N*/ 		SwRect aTmp( aCharRect );
/*N*/ 		long nDiff = aCharRect.Height() - VisArea().Height();
/*N*/ 		if( nDiff < aCrsrHeight.X() )
/*?*/ 			aTmp.Top( nDiff + aCharRect.Top() );
/*N*/ 		else
/*N*/ 		{
/*N*/ 			aTmp.Top( aCrsrHeight.X() + aCharRect.Top() );
/*N*/ 			aTmp.Height( aCrsrHeight.Y() );
/*N*/ 		}
/*N*/         if( !aTmp.HasArea() )
/*N*/         {
/*?*/             aTmp.SSize().Height() += 1;
/*?*/             aTmp.SSize().Width() += 1;
/*N*/         }
/*N*/ 		MakeVisible( aTmp );
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*N*/ 		if( aCharRect.HasArea() )
/*N*/ 			MakeVisible( aCharRect );
/*N*/ 		else
/*N*/ 		{
/*?*/ 			SwRect aTmp( aCharRect );
/*?*/ 			aTmp.SSize().Height() += 1; aTmp.SSize().Width() += 1;
/*?*/ 			MakeVisible( aTmp );
/*N*/ 		}
/*N*/ 	}
/*N*/ }


// suche eine gueltige ContentPosition (nicht geschuetzt/nicht versteckt)




/*N*/ FASTBOOL SwCrsrShell::IsCrsrReadonly() const
/*N*/ {
/*N*/ 	if ( GetViewOptions()->IsReadonly() )
/*N*/ 	{
/*N*/ 		SwFrm *pFrm = GetCurrFrm( FALSE );
/*N*/ 		SwFlyFrm *pFly;
/*N*/ 		if( pFrm && pFrm->IsInFly() &&
/*N*/ 			 (pFly = pFrm->FindFlyFrm())->GetFmt()->GetEditInReadonly().GetValue() &&
/*N*/ 			 pFly->Lower() &&
/*N*/ 			 !pFly->Lower()->IsNoTxtFrm() &&
/*N*/ 			 !GetDrawView()->GetMarkList().GetMarkCount() )
/*N*/ 		{
/*N*/ 			return FALSE;
/*N*/ 		}
/*N*/ 		return TRUE;
/*N*/ 	}
/*N*/ 	return FALSE;
/*N*/ }

// SwCursor - Methode !!!!
/*N*/ FASTBOOL SwCursor::IsReadOnlyAvailable() const
/*N*/ {
/*N*/ 	const SwShellCrsr* pShCrsr = *this;
/*N*/ 	const SwUnoCrsr* pUnoCrsr = *this;
/*N*/ 	return pShCrsr ? pShCrsr->GetShell()->IsReadOnlyAvailable() :
/*N*/ 		pUnoCrsr ? TRUE : FALSE;
/*N*/ }


#if !defined(PRODUCT) || defined(WIN)



/*N*/ SwCursor* SwCrsrShell::GetSwCrsr( FASTBOOL bMakeTblCrsr ) const
/*N*/ {
/*N*/ 	return (SwCursor*)GetCrsr( bMakeTblCrsr );
/*N*/ }

// gebe den Stack Cursor zurueck
/*N*/ SwPaM * SwCrsrShell::GetStkCrsr() const			{ return pCrsrStk; }

// gebe den TabellenCrsr zurueck
/*N*/ const	SwPaM* SwCrsrShell::GetTblCrs() const	{ return pTblCrsr; }
/*N*/ 		SwPaM* SwCrsrShell::GetTblCrs()			{ return pTblCrsr; }

// Abfrage, ob ueberhaupt eine Selektion existiert, sprich der akt. Cursor
// aufgespannt oder nicht der einzigste ist.

/*N*/ FASTBOOL SwCrsrShell::IsSelection() const
/*N*/ {
/*N*/ 	return IsTableMode() || pCurCrsr->HasMark() ||
/*N*/ 			pCurCrsr->GetNext() != pCurCrsr;
/*N*/ }
// returns if multiple cursors are available
/*N*/ FASTBOOL SwCrsrShell::IsMultiSelection() const
/*N*/ {
/*N*/     return pCurCrsr->GetNext() != pCurCrsr;
/*N*/ }        

// pruefe ob vom aktuellen Crsr der SPoint/Mark in einer Tabelle stehen
/*N*/ const SwTableNode* SwCrsrShell::IsCrsrInTbl( BOOL bIsPtInTbl ) const
/*N*/ {
/*N*/ 	return pCurCrsr->GetNode( bIsPtInTbl )->FindTableNode();
/*N*/ }


/*?*/ FASTBOOL SwCrsrShell::IsCrsrPtAtEnd() const
/*?*/ {
/*?*/ 	return pCurCrsr->End() == pCurCrsr->GetPoint();
/*?*/ }


/*?*/ Point& SwCrsrShell::GetCrsrDocPos( BOOL bPoint ) const
/*?*/ {
/*?*/ 	return bPoint ? pCurCrsr->GetPtPos() : pCurCrsr->GetMkPos();
/*?*/ }


/*?*/ void SwCrsrShell::UnSetVisCrsr()
/*?*/ {
/*?*/ 	pVisCrsr->Hide();
/*?*/ 	pVisCrsr->SetDragCrsr( FALSE );
/*?*/ }


/*?*/ FASTBOOL SwCrsrShell::IsSelOnePara() const
/*?*/ {
/*?*/ 	return pCurCrsr == pCurCrsr->GetNext() &&
/*?*/ 		   pCurCrsr->GetPoint()->nNode ==
/*?*/ 		   pCurCrsr->GetMark()->nNode;
/*?*/ }

/*?*/ SwMoveFnCollection* SwCrsrShell::MakeFindRange(
/*?*/ 							USHORT nStt, USHORT nEnd, SwPaM* pPam ) const
/*?*/ {
/*?*/ DBG_BF_ASSERT(0, "STRIP"); return NULL;//STRIP001 	return pCurCrsr->MakeFindRange( (SwDocPositions)nStt,
/*?*/ }
#endif

/**
   Checks if a position is valid. To be valid the position's node must
   be a content node and the content must not be unregistered.

   @param aPos the position to check.
*/
/*N*/ static bool lcl_PosOk(SwPosition & aPos)
/*N*/ {
/*N*/     bool bResult = true;
/*N*/     SwPosition aTmpPos(aPos);
/*N*/     aTmpPos.nContent.Assign(0, 0);
/*N*/ 
/*N*/     if (aPos.nNode.GetNode().GetCntntNode() == NULL ||
/*N*/         aPos.nContent.GetIdxReg() == aTmpPos.nContent.GetIdxReg())
/*N*/         bResult = false;
/*N*/ 
/*N*/     return bResult;
/*N*/ }

/**
   Checks if a PaM is valid. For a PaM to be valid its point must be
   valid. Additionaly if the PaM has a mark this has to be valid, too.

   @param aPam the PaM to check
*/
/*N*/ static bool lcl_CrsrOk(SwPaM & aPam)
/*N*/ {
/*N*/     return lcl_PosOk(*aPam.GetPoint()) && (! aPam.HasMark()
/*N*/         || lcl_PosOk(*aPam.GetMark()));
/*N*/ }

/*N*/ void SwCrsrShell::ClearUpCrsrs()
/*N*/ {
/*N*/     // start of the ring
/*N*/     SwPaM * pStartCrsr = GetCrsr();
/*N*/     // start loop with second entry of the ring
/*N*/     SwPaM * pCrsr = (SwPaM *) pStartCrsr->GetNext();
/*N*/     SwPaM * pTmpCrsr;
/*N*/     bool bChanged = false;
/*N*/ 
    /*
       For all entries in the ring except the start entry delete the
        entry if it is invalid.
    */
/*N*/     while (pCrsr != pStartCrsr)
/*N*/     {
/*?*/         pTmpCrsr = (SwPaM *) pCrsr->GetNext();
/*?*/ 
/*?*/         if ( ! lcl_CrsrOk(*pCrsr))
/*?*/         {
/*?*/             delete pCrsr;
/*?*/ 
/*?*/             bChanged = true;
/*N*/         }
/*N*/ 
/*N*/         pCrsr = pTmpCrsr;
/*N*/     }
/*N*/ 
    /*
      If the start entry of the ring is invalid replace it with a
      cursor pointing to the beginning of the first content node in
      the document.
    */
/*N*/     if (! lcl_CrsrOk(*pStartCrsr))
/*N*/     {
/*?*/         SwNodes & aNodes = GetDoc()->GetNodes();
/*?*/         SwNodeIndex aIdx(*(aNodes.GetEndOfContent().StartOfSectionNode()));
/*?*/ 
/*?*/         SwNode * pNode = aNodes.GoNext(&aIdx);
/*?*/         bool bFound = (pNode != NULL);
/*?*/ 
/*?*/         ASSERT(bFound, "no content node found");
/*?*/ 
/*?*/         if (bFound)
/*?*/         {
/*?*/             SwPaM aTmpPam(*pNode);
/*?*/             *pStartCrsr = aTmpPam;
/*?*/         }
/*?*/ 
/*?*/         bChanged = true;
/*N*/     }

    /*
      If at least one of the cursors in the ring have been deleted or
      replaced, remove the table cursor.
    */
/*N*/     if (pTblCrsr != NULL && bChanged)
/*?*/         {DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 TblCrsrToCursor();
/*N*/ }

}
