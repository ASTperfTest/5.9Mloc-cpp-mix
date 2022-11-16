/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_docundo.cxx,v $
 *
 *  $Revision: 1.9 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:44:56 $
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


#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif

#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif

#ifndef _SWUNDO_HXX
#include <swundo.hxx>		// fuer die UndoIds
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _DOCARY_HXX
#include <docary.hxx>
#endif
namespace binfilter {

USHORT SwDoc::nUndoActions = UNDO_ACTION_COUNT;		// anzahl von Undo-Action

// the undo array should never grow beyond this limit:
#define UNDO_ACTION_LIMIT (USHRT_MAX - 1000)



//#define _SHOW_UNDORANGE
#ifdef _SHOW_UNDORANGE









#endif


/*N*/ void SwDoc::AppendUndo( SwUndo* pUndo )
/*N*/ {
/*N*/ #ifdef COMPACT
/*N*/ 	DelUndoGroups( FALSE );		// nur die History loeschen !!
/*N*/ #endif
/*N*/ 
/*N*/ 	if( REDLINE_NONE == pUndo->GetRedlineMode() )
/*N*/ 		pUndo->SetRedlineMode( GetRedlineMode() );
/*N*/ 
/*N*/     // Unfortunately, the silly SvPtrArr can only store a little less than
/*N*/     // USHRT_MAX elements. Of course it doesn't see any necessity for asserting
/*N*/     // or even doing error handling. pUndos should definitely be replaced by an
/*N*/     // STL container that doesn't have this problem. cf #95884#
/*N*/     DBG_ASSERT( pUndos->Count() < USHRT_MAX - 16,
/*N*/                 "Writer will crash soon. I apologize for the inconvenience." );
/*N*/ 
/*N*/ 	pUndos->Insert( pUndo, nUndoPos );
/*N*/ 	++nUndoPos;
/*N*/ 	switch( pUndo->GetId() )
/*N*/ 	{
/*N*/ 	case UNDO_START:		++nUndoSttEnd;
/*N*/ 							break;
/*N*/ 
/*N*/ 	case UNDO_END:		    ASSERT( nUndoSttEnd, "Undo-Ende ohne Start" );
/*N*/ 							--nUndoSttEnd;
/*N*/ 							// kein break !!!
/*N*/ 	default:
/*N*/ 		if( pUndos->Count() != nUndoPos && UNDO_END != pUndo->GetId() )
/*N*/ 			ClearRedo();
/*N*/ 		else
/*N*/ 			ASSERT( pUndos->Count() == nUndoPos || UNDO_END == pUndo->GetId(),
/*N*/ 					"Redo history not deleted!" );
/*N*/ 		if( !nUndoSttEnd )
/*N*/ 			++nUndoCnt;
/*N*/ 		break;
/*N*/ 	}
/*N*/ 
/*N*/ #ifdef _SHOW_UNDORANGE
/*N*/ 	// zur Anzeige der aktuellen Undo-Groessen
/*N*/ 	if( !pUndoMsgWin )
/*N*/ 			pUndoMsgWin = new UndoArrStatus;
/*N*/ 	pUndoMsgWin->Set( pUndos->Count(), aUndoNodes.Count() );
/*N*/ #endif
/*N*/ 
/*N*/ 	// noch eine offene Klammerung, kann man sich den Rest schenken
/*N*/ 	if( nUndoSttEnd )
/*N*/ 		return;
/*N*/ 
/*N*/ 	// folgende Array-Grenzen muessen ueberwacht werden:
/*N*/ 	//	- Undo,				Grenze: fester Wert oder USHRT_MAX - 1000
/*N*/ 	//	- UndoNodes,		Grenze:  USHRT_MAX - 1000
/*N*/ 	//	- AttrHistory       Grenze:  USHRT_MAX - 1000
/*N*/     // (defined in UNDO_ACTION_LIMIT at the top of this file)
/*N*/ 
/*N*/ 	USHORT nEnde = UNDO_ACTION_LIMIT;
/*N*/ 
/*N*/ // nur zum Testen der neuen DOC-Member
/*N*/ #ifndef PRODUCT
/*N*/ {
/*N*/ 	USHORT nId, nUndosCnt = 0, nSttEndCnt = 0;
/*N*/ 	for( USHORT nCnt = 0; nCnt < nUndoPos; ++nCnt )
/*N*/ 	{
/*N*/ 		if( UNDO_START == ( nId = (*pUndos)[ nCnt ]->GetId() ))
/*N*/ 			++nSttEndCnt;
/*N*/ 		else if( UNDO_END == nId )
/*N*/ 			--nSttEndCnt;
/*N*/ 		if( !nSttEndCnt )
/*N*/ 			++nUndosCnt;
/*N*/ 	}
/*N*/ 	ASSERT( nSttEndCnt == nUndoSttEnd, "Start-Ende Count ungleich" );
/*N*/ 	ASSERT( nUndosCnt == nUndoCnt, "Undo Count ungleich" );
/*N*/ }
/*N*/ #endif
/*N*/ 
/*N*/ 	if( SwDoc::nUndoActions < nUndoCnt )
/*N*/ 		// immer 1/10 loeschen
/*N*/ 		//JP 23.09.95: oder wenn neu eingestellt wurde um die Differenz
/*N*/ 		//JP 29.5.2001: Task #83891#: remove only the overlapping actions
/*N*/ 		DelUndoObj( nUndoCnt - SwDoc::nUndoActions );
/*N*/ 	else
/*N*/ 	{
/*N*/ 		USHORT nUndosCnt = nUndoCnt;
/*N*/ 			// immer 1/10 loeschen bis der "Ausloeser" behoben ist
/*N*/ 		while( aUndoNodes.Count() && nEnde < aUndoNodes.Count() )
/*?*/ 			DelUndoObj( nUndosCnt / 10 );
/*N*/ 	}
/*N*/ }



/*N*/ void SwDoc::ClearRedo()
/*N*/ {
/*N*/ 	if( DoesUndo() && nUndoPos != pUndos->Count() )
/*N*/ 	{
/*N*/ //?? why ??		if( !nUndoSttEnd )
/*N*/ 		{
/*N*/ 			// setze UndoCnt auf den neuen Wert
/*N*/ 			SwUndo* pUndo;
/*?*/ 			for( USHORT nCnt = pUndos->Count(); nUndoPos < nCnt; --nUndoCnt )
/*?*/ 				// Klammerung ueberspringen
/*?*/ 				if( UNDO_END == (pUndo = (*pUndos)[ --nCnt ])->GetId() )
/*?*/ 					nCnt -= ((SwUndoEnd*)pUndo)->GetSttOffset();
/*?*/ 		}
/*?*/ 
/*?*/ 		// loesche die Undo-Aktionen (immer von hinten !)
/*?*/ 		pUndos->DeleteAndDestroy( nUndoPos, pUndos->Count() - nUndoPos );
/*N*/ 	}
/*N*/ }


	// loescht die gesamten UndoObjecte
/*N*/ void SwDoc::DelAllUndoObj()
/*N*/ {
/*N*/ 	ClearRedo();
/*N*/ 
/*N*/ 	DoUndo( FALSE );
/*N*/ 
/*N*/ 	// Offene Undo-Klammerungen erhalten !!
/*N*/ 	SwUndo* pUndo;
/*N*/ 	USHORT nSize = pUndos->Count();
/*N*/ 	while( nSize )
/*N*/ 		if( UNDO_START != ( pUndo = (*pUndos)[ --nSize ] )->GetId() ||
/*N*/ 			((SwUndoStart*)pUndo)->GetEndOffset() )
/*N*/ 			// keine offenen Gruppierung ?
/*N*/ 			pUndos->DeleteAndDestroy( nSize, 1 );
/*N*/ 
/*N*/ 	nUndoCnt = 0;
/*N*/ 	nUndoPos = pUndos->Count();

/*
	while( nUndoPos )
		aUndos.DelDtor( --nUndoPos, 1 );
	nUndoCnt = nUndoSttEnd = nUndoPos = 0;
*/
/*N*/ 	nUndoSavePos = USHRT_MAX;
/*N*/ 	DoUndo( TRUE );
/*N*/ }


	// loescht alle UndoObjecte vom Anfang bis zum angegebenen Ende
/*N*/ BOOL SwDoc::DelUndoObj( USHORT nEnde )
/*N*/ {
/*N*/ 	if( !nEnde )					// sollte mal 0 uebergeben werden,
/*N*/ 	{
/*?*/ 		if( !pUndos->Count() )
/*?*/ 			return FALSE;
/*?*/ 		++nEnde;            		// dann korrigiere es auf 1
/*N*/ 	}
/*N*/ 
/*N*/ 	DoUndo( FALSE );
/*N*/ 
/*N*/ 	// pruefe erstmal, wo das Ende steht
/*N*/ 	USHORT nId = 0, nSttEndCnt = 0;
		USHORT nCnt = 0;
/*N*/ 	for( nCnt = 0; nEnde && nCnt < nUndoPos; ++nCnt )
/*N*/ 	{
/*N*/ 		if( UNDO_START == ( nId = (*pUndos)[ nCnt ]->GetId() ))
/*N*/ 			++nSttEndCnt;
/*N*/ 		else if( UNDO_END == nId )
/*N*/ 			--nSttEndCnt;
/*N*/ 		if( !nSttEndCnt )
/*N*/ 			--nEnde, --nUndoCnt;
/*N*/ 	}
/*N*/ 
/*N*/ 	ASSERT( nCnt < nUndoPos || nUndoPos == pUndos->Count(),
/*N*/ 			"Undo-Del-Ende liegt in einer Redo-Aktion" );
/*N*/ 
/*N*/ 	// dann setze ab Ende bis Undo-Ende bei allen Undo-Objecte die Werte um
/*N*/ 	nSttEndCnt = nCnt;			// Position merken
/*N*/ 	if( nUndoSavePos < nSttEndCnt )		// SavePos wird aufgegeben
/*N*/ 		nUndoSavePos = USHRT_MAX;
/*N*/ 	else if( nUndoSavePos != USHRT_MAX )
/*?*/ 		nUndoSavePos -= nSttEndCnt;
/*N*/ 
/*N*/ 	while( nSttEndCnt )
/*N*/ 		pUndos->DeleteAndDestroy( --nSttEndCnt, 1 );
/*N*/ 	nUndoPos = pUndos->Count();
/*N*/ 
/*N*/ 	DoUndo( TRUE );
/*N*/ 	return TRUE;
/*N*/ }

/**************** UNDO ******************/






// setzt Undoklammerung auf, liefert nUndoId der Klammerung


/*N*/ USHORT SwDoc::StartUndo( USHORT nUndoId )
/*N*/ {
/*N*/ 	if( !bUndo )
/*N*/ 		return 0;
/*N*/ 
/*N*/ 	if( !nUndoId )
/*N*/ 		nUndoId = UNDO_START;
/*N*/ 
/*N*/ 	AppendUndo( new SwUndoStart( nUndoId ));
/*N*/ 	return nUndoId;
/*N*/ }
// schliesst Klammerung der nUndoId, nicht vom UI benutzt


/*N*/ USHORT SwDoc::EndUndo(USHORT nUndoId)
/*N*/ {
/*N*/ 	USHORT nSize = nUndoPos;
/*N*/ 	if( !bUndo || !nSize-- )
/*N*/ 		return 0;
/*N*/ 
/*N*/ 	if( UNDO_START == nUndoId || !nUndoId )
/*N*/ 		nUndoId = UNDO_END;
/*N*/ 
/*N*/ 	SwUndo* pUndo = (*pUndos)[ nSize ];
/*N*/ 	if( UNDO_START == pUndo->GetId() )
/*N*/ 	{
/*N*/ 		// leere Start/End-Klammerung ??
/*N*/ 		pUndos->DeleteAndDestroy( nSize );
/*N*/ 		--nUndoPos;
/*N*/ 		--nUndoSttEnd;
/*N*/ 		return 0;
/*N*/ 	}
/*N*/ 
/*N*/ 	// exist above any redo objects? If yes, delete them
/*N*/ 	if( nUndoPos != pUndos->Count() )
/*N*/ 	{
/*N*/ 		// setze UndoCnt auf den neuen Wert
/*?*/ 		for( USHORT nCnt = pUndos->Count(); nUndoPos < nCnt; --nUndoCnt )
/*?*/ 			// Klammerung ueberspringen
/*?*/ 			if( UNDO_END == (pUndo = (*pUndos)[ --nCnt ])->GetId() )
/*?*/ 				nCnt -= ((SwUndoEnd*)pUndo)->GetSttOffset();
/*?*/ 
/*?*/ 		pUndos->DeleteAndDestroy( nUndoPos, pUndos->Count() - nUndoPos );
/*N*/ 	}
/*N*/ 
/*N*/ 	// suche den Anfang dieser Klammerung
/*N*/ 	USHORT nId;
/*N*/ 	while( nSize )
/*N*/ 		if( UNDO_START == ( nId = (pUndo = (*pUndos)[ --nSize ] )->GetId()) &&
/*N*/ 			!((SwUndoStart*)pUndo)->GetEndOffset() )
/*N*/ 			break;		// Start gefunden
/*N*/ 
/*N*/ 	if( nId != UNDO_START )
/*N*/ 	{
/*?*/ 		// kann eigentlich nur beim Abspielen von Macros passieren, die
/*?*/ 		// Undo/Redo/Repeat benutzen und die eine exitierende Selection
/*?*/ 		// durch Einfuegen loeschen
/*?*/ 		ASSERT( !this, "kein entsprechendes Ende gefunden" );
/*?*/ 		// kein entsprechenden Start gefunden -> Ende nicht einfuegen
/*?*/ 		// und die Member am Doc updaten
/*?*/ 
/*?*/ 		nUndoSttEnd = 0;
/*?*/ 		nUndoCnt = 0;
/*?*/ 		// setze UndoCnt auf den neuen Wert
/*?*/ 		SwUndo* pUndo;
/*?*/ 		for( USHORT nCnt = 0; nCnt < pUndos->Count(); ++nCnt, ++nUndoCnt )
/*?*/ 			// Klammerung ueberspringen
/*?*/ 			if( UNDO_START == (pUndo = (*pUndos)[ nCnt ])->GetId() )
/*?*/ 				nCnt += ((SwUndoStart*)pUndo)->GetEndOffset();
/*?*/ 		return 0;
/*?*/ 
/*N*/ 	}

	// Klammerung um eine einzelne Action muss nicht sein!
	// Aussnahme: es ist eine eigene ID definiert
/*N*/ 	if(  2 == pUndos->Count() - nSize &&
/*N*/ 		(UNDO_END == nUndoId || nUndoId == (*pUndos)[ nSize+1 ]->GetId() ))
/*N*/ 	{
/*N*/ 		pUndos->DeleteAndDestroy( nSize );
/*N*/ 		nUndoPos = pUndos->Count();
/*N*/ 		if( !--nUndoSttEnd )
/*N*/ 		{
/*N*/ 			++nUndoCnt;
/*N*/ 			if( SwDoc::nUndoActions < nUndoCnt )
/*N*/ 				// immer 1/10 loeschen
/*N*/ 				//JP 23.09.95: oder wenn neu eingestellt wurde um die Differenz
/*N*/ 				//JP 29.5.2001: Task #83891#: remove only the overlapping actions
/*N*/ 				DelUndoObj( nUndoCnt - SwDoc::nUndoActions );
/*N*/ 			else
/*N*/ 			{
/*N*/ 				USHORT nEnde = USHRT_MAX - 1000;
/*N*/ 				USHORT nUndosCnt = nUndoCnt;
/*N*/ 					// immer 1/10 loeschen bis der "Ausloeser" behoben ist
/*N*/ 				while( aUndoNodes.Count() && nEnde < aUndoNodes.Count() )
/*?*/ 					DelUndoObj( nUndosCnt / 10 );
/*N*/ 			}
/*N*/ 		}
/*N*/ 		return nUndoId;
/*N*/ 	}
/*N*/ 
/*N*/ 	// setze die Klammerung am Start/End-Undo
/*N*/ 	nSize = pUndos->Count() - nSize;
/*N*/ 	((SwUndoStart*)pUndo)->SetEndOffset( nSize );
/*N*/ 
/*N*/ 	SwUndoEnd* pUndoEnd = new SwUndoEnd( nUndoId );
/*N*/ 	pUndoEnd->SetSttOffset( nSize );
/*N*/ 
/*N*/ // nur zum Testen der Start/End-Verpointerung vom Start/End Undo
/*N*/ #ifndef PRODUCT
/*N*/ {
/*N*/ 	USHORT nEndCnt = 1, nCnt = pUndos->Count(), nId;
/*N*/ 	while( nCnt )
/*N*/ 	{
/*N*/ 		if( UNDO_START == ( nId = (*pUndos)[ --nCnt ]->GetId()) )
/*N*/ 		{
/*N*/ 			if( !nEndCnt )		// falls mal ein Start ohne Ende vorhanden ist
/*N*/ 				continue;
/*N*/ 			--nEndCnt;
/*N*/ 			if( !nEndCnt )		// hier ist der Anfang
/*N*/ 				break;
/*N*/ 		}
/*N*/ 		else if( UNDO_END == nId )
/*N*/ 			++nEndCnt;
/*N*/ 		else if( !nEndCnt )
/*N*/ 			break;
/*N*/ 	}
/*N*/ 	ASSERT( nCnt == pUndos->Count() - nSize, "Start-Ende falsch geklammert" );
/*N*/ }
/*N*/ #endif
/*N*/ 
/*N*/ 	AppendUndo( pUndoEnd );
/*N*/ 	return nUndoId;
/*N*/ }

}
