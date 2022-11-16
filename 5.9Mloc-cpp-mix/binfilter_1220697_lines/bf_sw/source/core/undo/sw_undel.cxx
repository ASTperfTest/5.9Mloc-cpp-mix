/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_undel.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:45:48 $
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

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _SWUNDO_HXX
#include <swundo.hxx>			// fuer die UndoIds
#endif
#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif
#ifndef _NDTXT_HXX
#include <ndtxt.hxx>
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _ROLBCK_HXX
#include <rolbck.hxx>
#endif
#ifndef _MVSAVE_HXX
#include <mvsave.hxx>
#endif
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
#ifndef _DOCARY_HXX
#include <docary.hxx>
#endif
namespace binfilter {



// DELETE

/*N*/ SwUndoDelete::SwUndoDelete( SwPaM& rPam, BOOL bFullPara )
/*N*/ 	: SwUndo(UNDO_DELETE), SwUndRng( rPam ),
/*N*/ 	pMvStt( 0 ), pSttStr(0), pEndStr(0), nNode( 0 ), nSectDiff( 0 ),
/*N*/ 	pRedlData( 0 ), pRedlSaveData( 0 )
/*N*/ {
/*N*/ 	bMvAroundSectNd = bSectNdFnd = bGroup = bBackSp = bTblDelLastNd =
/*N*/ 		bResetPgDesc = bResetPgBrk = FALSE;
/*N*/ 
/*N*/ 	bDelFullPara = bFullPara;
/*N*/ 
/*N*/ 	SwDoc * pDoc = rPam.GetDoc();
/*N*/ #ifdef COMPACT
/*N*/ 	pDoc->DelUndoGroups();
/*N*/ #endif
/*N*/ 
/*N*/ 	if( !pDoc->IsIgnoreRedline() && pDoc->GetRedlineTbl().Count() )
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pRedlSaveData = new SwRedlineSaveDatas;
/*N*/ 	}
/*N*/ 
/*N*/ 	if( !pHistory )
/*N*/ 		pHistory = new SwHistory;
/*N*/ 
/*N*/ 	// loesche erstmal alle Fussnoten
/*N*/ 	const SwPosition *pStt = rPam.Start(),
/*N*/ 					*pEnd = rPam.GetPoint() == pStt
/*N*/ 						? rPam.GetMark()
/*N*/ 						: rPam.GetPoint();
/*N*/ 
/*N*/ 	if( bDelFullPara )
/*N*/ 	{
/*N*/ 		ASSERT( rPam.HasMark(), "PaM ohne Mark" );
/*N*/ 		DelCntntIndex( *rPam.GetMark(), *rPam.GetPoint(),
/*N*/ 						DelCntntType(DELCNT_ALL | DELCNT_CHKNOCNTNT) );
/*N*/ 
/*N*/ 		BOOL bDoesUndo = pDoc->DoesUndo();
/*N*/ 		pDoc->DoUndo( FALSE );
/*N*/ 		_DelBookmarks( pStt->nNode, pEnd->nNode );
/*N*/ 		pDoc->DoUndo( bDoesUndo );
/*N*/ 	}
/*N*/ 	else
/*N*/ 		DelCntntIndex( *rPam.GetMark(), *rPam.GetPoint() );
/*N*/ 	nSetPos = pHistory ? pHistory->Count() : 0;
/*N*/ 
/*N*/ 	// wurde schon was geloescht ??
/*N*/ 	nNdDiff = nSttNode - pStt->nNode.GetIndex();
/*N*/ 
/*N*/ 	bJoinNext = !bFullPara && pEnd == rPam.GetPoint();
/*N*/ 	bBackSp = !bFullPara && !bJoinNext;
/*N*/ 
/*N*/ 	SwTxtNode *pSttTxtNd = 0, *pEndTxtNd = 0;
/*N*/ 	if( !bFullPara )
/*N*/ 	{
/*N*/ 		pSttTxtNd = pStt->nNode.GetNode().GetTxtNode();
/*N*/ 		pEndTxtNd = nSttNode == nEndNode
/*N*/ 					? pSttTxtNd
/*N*/ 					: pEnd->nNode.GetNode().GetTxtNode();
/*N*/ 	}
/*N*/ 
/*N*/ 	BOOL bMoveNds = *pStt == *pEnd      // noch ein Bereich vorhanden ??
/*N*/ 				? FALSE
/*N*/ 				: SaveCntnt( pStt, pEnd, pSttTxtNd, pEndTxtNd );
/*N*/ 
/*N*/ 	if( pSttTxtNd && pEndTxtNd && pSttTxtNd != pEndTxtNd )
/*N*/ 	{
/*N*/ 		// zwei unterschiedliche TextNodes, also speicher noch die
/*N*/ 		// TextFormatCollection fuers
/*?*/ 		pHistory->Add( pSttTxtNd->GetTxtColl(),pStt->nNode.GetIndex(), ND_TEXTNODE );
/*?*/ 		pHistory->Add( pEndTxtNd->GetTxtColl(),pEnd->nNode.GetIndex(), ND_TEXTNODE );
/*?*/ 
/*?*/ 		if( !bJoinNext )	 	// Selection von Unten nach Oben
/*?*/ 		{
/*?*/ 			// Beim JoinPrev() werden die AUTO-PageBreak's richtig
/*?*/ 			// kopiert. Um diese beim Undo wieder herzustellen, muss das
/*?*/ 			// Auto-PageBreak aus dem EndNode zurueckgesetzt werden.
/*?*/ 			// - fuer die PageDesc, ColBreak dito !
/*?*/ 			if( pEndTxtNd->GetpSwAttrSet() )
/*?*/ 			{
/*?*/ 				SwRegHistory aRegHist( *pEndTxtNd, pHistory );
/*?*/ 				if( SFX_ITEM_SET == pEndTxtNd->GetpSwAttrSet()->GetItemState(
/*?*/ 						RES_BREAK, FALSE ) )
/*?*/ 					pEndTxtNd->ResetAttr( RES_BREAK );
/*?*/ 				if( pEndTxtNd->GetpSwAttrSet() &&
/*?*/ 					SFX_ITEM_SET == pEndTxtNd->GetpSwAttrSet()->GetItemState(
/*?*/ 						RES_PAGEDESC, FALSE ) )
/*?*/ 					pEndTxtNd->ResetAttr( RES_PAGEDESC );
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ 

	// verschiebe jetzt noch den PaM !!!
	// der SPoint steht am Anfang der SSelection
/*N*/ 	if( pEnd == rPam.GetPoint() && pSttTxtNd )
/*N*/ 		rPam.Exchange();
/*N*/ 
/*N*/ 	if( !pSttTxtNd && !pEndTxtNd )
/*N*/ 		rPam.GetPoint()->nNode--;
/*N*/ 	rPam.DeleteMark();			// der SPoint ist aus dem Bereich
/*N*/ 
/*N*/ 	if( !pEndTxtNd )
/*N*/ 		nEndCntnt = 0;
/*N*/ 	if( !pSttTxtNd )
/*N*/ 		nSttCntnt = 0;
/*N*/ 
/*N*/ 	if( bMoveNds )		// sind noch Nodes zu verschieben ?
/*N*/ 	{
/*N*/ 		// verschiebe jetzt den Rest, also die vollstaendigen Nodes
/*N*/ 		// ACHTUNG: pStt und pEnd koennen durch die Pam-Korrektur schon
/*N*/ 		// ungueltig sein !!
/*N*/ 		int nNdOff = 0;
/*N*/ 		if( pSttTxtNd && ( pEndTxtNd || pSttTxtNd->GetTxt().Len() ))
/*N*/ 			nNdOff++;
/*N*/ 
/*N*/ 		SwNodeRange aRg( pDoc->GetNodes(), nSttNode - nNdDiff + nNdOff,
/*N*/ 						 pDoc->GetNodes(), nEndNode - nNdDiff );
/*N*/ 		if( !bFullPara && !pEndTxtNd &&
/*N*/ 			&aRg.aEnd.GetNode() != &pDoc->GetNodes().GetEndOfContent() )
/*?*/ 			aRg.aEnd++;
/*N*/ 
/*N*/ 		SwNodes& rNds = (SwNodes&)*pDoc->GetUndoNds();
/*N*/ 		SwNodes& rDocNds = pDoc->GetNodes();
/*N*/ 		nNode = rNds.GetEndOfContent().GetIndex();
/*N*/ 
/*N*/ 		// habe wir SectionNodes (Start/Ende) als 1. oder letzten
/*N*/ 		// Nodes in der Selection ?
/*N*/ 		SwNode* pTmpNd;
/*N*/ 		if( bJoinNext )		// Selektion von oben -> unten
/*N*/ 		{
/*N*/ 			// Bedingung: - SectionNode und dessen Ende ist der naechste Node
/*N*/ 			//			  - EndNode einer Section und der Start steht ausserhalb
/*?*/ 			if( pSttTxtNd && pEndTxtNd )
/*?*/ 			{
/*?*/ 				// am Ende erfolgt ein JoinNext, teste auf leere Sections
/*?*/ 				// Bedingung: - hinter dem EndTextNode steht ein SectionEndNd
/*?*/ 				//				dessen Start im Bereich liegt
/*?*/ 				//			  - SectionSttNd und der Start steht ausserhalb
/*?*/ 				if( aRg.aEnd.GetIndex()+1 < rDocNds.Count() &&
/*?*/ 					( (pTmpNd = rDocNds[ aRg.aEnd.GetIndex()+1 ])->IsEndNode()
/*?*/ 					&& pTmpNd->FindStartNode()->IsSectionNode()
/*?*/ 					&& pTmpNd->StartOfSectionIndex() >= aRg.aStart.GetIndex())
/*?*/ 				)
/*?*/ 				{
/*?*/ 					aRg.aEnd++;
/*?*/ 					bSectNdFnd = TRUE;
/*?*/ 				}
/*?*/ 				else if( ((pTmpNd = rDocNds[ aRg.aEnd.GetIndex()-1 ])->IsSectionNode()
/*?*/ 						&& pTmpNd->EndOfSectionIndex()-1 == aRg.aEnd.GetIndex())
/*?*/ 					|| ( pTmpNd->IsEndNode() &&
/*?*/ 					pTmpNd->FindStartNode()->IsSectionNode() &&
/*?*/ 					pTmpNd->FindStartNode()->GetIndex() < aRg.aStart.GetIndex())
/*?*/ 					)
/*?*/ 				{
/*?*/ 					aRg.aEnd++;
/*?*/ 					bSectNdFnd = TRUE;
/*?*/ 				}
/*?*/ 			}
/*?*/ 			while( aRg.aEnd.GetIndex() < rDocNds.Count()-1 &&
/*?*/ 				// entstehen leere Sections ???
/*?*/ 				( (pTmpNd = &aRg.aEnd.GetNode())->IsEndNode() &&
/*?*/ 				pTmpNd->FindStartNode()->IsSectionNode() &&
/*?*/ 				pTmpNd->FindStartNode()->GetIndex()+1 >= aRg.aStart.GetIndex())
/*?*/ 				)
/*?*/ 			{
/*?*/ 				aRg.aEnd++;
/*?*/ 				bSectNdFnd = TRUE;
/*?*/ 			}
/*N*/ 		}
/*N*/ 		else
/*N*/ 		{
/*N*/ 			if( pSttTxtNd && pEndTxtNd )
/*N*/ 			{
/*N*/ 				// am Ende erfolgt ein JoinPrev, teste auf leere Sections
/*N*/ 				// Bedingung: - vor dem StartTextNode steht ein SectionSttNd
/*N*/ 				//				dessen Ende im Bereich liegt
/*N*/ 				//			  - SectionEndNd und der Start steht ausserhalb
/*N*/ 				if( 2 < aRg.aStart.GetIndex() &&
/*N*/ 					( (pTmpNd = rDocNds[ aRg.aStart.GetIndex()-2 ])
/*N*/ 						->IsSectionNode() &&
/*N*/ 					pTmpNd->EndOfSectionIndex() < aRg.aEnd.GetIndex())
/*N*/ 				)
/*N*/ 				{
/*?*/ 					aRg.aStart = *pTmpNd;
/*?*/ 					bSectNdFnd = TRUE;
/*?*/ 					nSectDiff++;
/*N*/ 				}
/*N*/ 				else if( aRg.aStart.GetIndex() &&
/*N*/ 					((pTmpNd = &aRg.aStart.GetNode())->IsSectionNode()
/*N*/ 					&& pTmpNd->EndOfSectionIndex() > aRg.aEnd.GetIndex() ) ||
/*N*/ 					(pTmpNd->IsEndNode() && pTmpNd->FindStartNode()->IsSectionNode() &&
/*N*/ 					pTmpNd->FindStartNode()->GetIndex() < aRg.aStart.GetIndex() )
/*N*/ 					)
/*N*/ 				{
/*N*/ 					aRg.aStart--;
/*N*/ 					bSectNdFnd = TRUE;
/*N*/ 				}
/*N*/ 			}
/*N*/ 			while( 1 < aRg.aStart.GetIndex() &&
/*N*/ 				// entstehen leere Sections ???
/*N*/ 				( (pTmpNd = rDocNds[ aRg.aStart.GetIndex()-1 ])->IsSectionNode() &&
/*N*/ 				pTmpNd->EndOfSectionIndex() < aRg.aEnd.GetIndex())
/*N*/ 				)
/*N*/ 			{
/*?*/ 				aRg.aStart--;
/*?*/ 				bSectNdFnd = TRUE;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
		// ein Index auf den Start-/End-ContentNode, der mit verschoben wird,
		// um wieder an der Position eine Kopie anzulegen.
/*N*/ 		if( bSectNdFnd && ( bJoinNext ? pEndTxtNd : pSttTxtNd ))
/*N*/ 		{
/*N*/ 			if( bJoinNext )
/*N*/ 			{
/*?*/ 				SwNodeRange aMvRg( *pEndTxtNd, 0, *pEndTxtNd, 1 );
/*?*/ 				rDocNds.MakeTxtNode( aMvRg.aStart, pEndTxtNd->GetTxtColl() );
/*?*/ 				rDocNds._MoveNodes( aMvRg, rDocNds, aRg.aStart );
/*N*/ 			}
/*N*/ 			else
/*N*/ 			{
/*N*/ 				SwNodeRange aMvRg( *pSttTxtNd, 0, *pSttTxtNd, 1 );
/*N*/ 				SwNode* pNew = rDocNds.MakeTxtNode( aMvRg.aEnd,
/*N*/ 											pSttTxtNd->GetTxtColl() );
/*N*/ 				if( nSectDiff )
/*N*/ 				{
/*?*/ 					aMvRg.aEnd--;
/*?*/ 					rDocNds._MoveNodes( aMvRg, rDocNds, aRg.aEnd );
/*?*/ 					aRg.aEnd--;
/*N*/ 				}
/*N*/ 				else
/*N*/ 					aRg.aStart = *pNew;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
/*N*/ 		rDocNds._MoveNodes( aRg, rNds, SwNodeIndex( rNds.GetEndOfContent() ));
/*N*/ 		pMvStt = new SwNodeIndex( rNds, nNode );
/*N*/ 		bMvAroundSectNd = FALSE;
/*N*/ 
/*N*/ 		if( !bSectNdFnd )
/*N*/ 		{
/*N*/ 			nSectDiff = aRg.aEnd.GetIndex() - aRg.aStart.GetIndex();
/*N*/ 			bMvAroundSectNd = 0 != nSectDiff;
/*N*/ 		}
/*N*/ 
/*N*/ 		nNode = rNds.GetEndOfContent().GetIndex() - nNode;		// Differenz merken !
/*N*/ 	}
/*N*/ 	else
/*N*/ 		nNode = 0;		// kein Node verschoben -> keine Differenz zum Ende
/*N*/ 
/*N*/ 	// wurden davor noch Nodes geloescht ?? (FootNotes haben ContentNodes!)
/*N*/ 	if( !pSttTxtNd && !pEndTxtNd )
/*N*/ 	{
/*N*/ 		nNdDiff = nSttNode - rPam.GetPoint()->nNode.GetIndex() - (bFullPara ? 0 : 1);
/*N*/ 		rPam.Move( fnMoveForward, fnGoNode );
/*N*/ 	}
/*N*/ 	else
/*N*/ 		nNdDiff = nSttNode - rPam.GetPoint()->nNode.GetIndex();
/*N*/ 	if( bSectNdFnd )
/*N*/ 		nNdDiff -= nSectDiff;
/*N*/ 
/*N*/ 	if( !rPam.GetNode()->IsCntntNode() )
/*N*/ 		rPam.GetPoint()->nContent.Assign( 0, 0 );
/*N*/ 
/*N*/ 	// wird die History ueberhaupt benoetigt ??
/*N*/ 	if( pHistory && !pHistory->Count() )
/*N*/ 		DELETEZ( pHistory );
/*N*/ }

/*N*/ BOOL SwUndoDelete::SaveCntnt( const SwPosition* pStt, const SwPosition* pEnd,
/*N*/ 					SwTxtNode* pSttTxtNd, SwTxtNode* pEndTxtNd )
/*N*/ {
/*N*/ 	ULONG nNdIdx = pStt->nNode.GetIndex();
/*N*/ 	// 1 - kopiere den Anfang in den Start-String
/*N*/ 	if( pSttTxtNd )
/*N*/ 	{
/*N*/ 		BOOL bOneNode = nSttNode == nEndNode;
/*N*/ 		xub_StrLen nLen = bOneNode ? nEndCntnt - nSttCntnt
/*N*/ 								: pSttTxtNd->GetTxt().Len() - nSttCntnt;
/*N*/ 		SwRegHistory aRHst( *pSttTxtNd, pHistory );
/*N*/ 		// immer alle TextAttribute sichern; ist fuers Undo mit voll-
/*N*/ 		// staendiger Attributierung am besten, wegen den evt.
/*N*/ 		// Ueberlappenden Bereichen von An/Aus.
/*N*/ 		pHistory->CopyAttr( pSttTxtNd->GetpSwpHints(), nNdIdx,
/*N*/ 							0, pSttTxtNd->GetTxt().Len(), TRUE );
/*N*/ 		if( !bOneNode && pSttTxtNd->GetpSwAttrSet() )
/*?*/ 			{DBG_BF_ASSERT(0, "STRIP"); }//STRIP001 	pHistory->CopyFmtAttr( *pSttTxtNd->GetpSwAttrSet(), nNdIdx );
/*N*/ 
/*N*/ 		// die Laenge kann sich veraendert haben (!!Felder!!)
/*N*/ 		nLen = ( bOneNode ? pEnd->nContent.GetIndex() : pSttTxtNd->GetTxt().Len() )
/*N*/ 				- pStt->nContent.GetIndex();
/*N*/ 
/*N*/ 		// loesche jetzt noch den Text (alle Attribut-Aenderungen kommen in
/*N*/ 		// die Undo-History
/*N*/ 		pSttStr = (String*)new String( pSttTxtNd->GetTxt().Copy( nSttCntnt, nLen ));
/*N*/ 		pSttTxtNd->Erase( pStt->nContent, nLen );
/*N*/ 		if( pSttTxtNd->GetpSwpHints() )
/*N*/ 			pSttTxtNd->GetpSwpHints()->DeRegister();
/*N*/ 
/*N*/ 		if( bOneNode )
/*N*/ 			return FALSE;           // keine Nodes mehr verschieben
/*N*/ 	}
/*N*/ 
/*N*/ 
/*N*/ 	// 2 - kopiere das Ende in den End-String
/*N*/ 	if( pEndTxtNd )
/*N*/ 	{
/*N*/ 		SwIndex aEndIdx( pEndTxtNd );
/*N*/ 		nNdIdx = pEnd->nNode.GetIndex();
/*N*/ 		SwRegHistory aRHst( *pEndTxtNd, pHistory );

		// immer alle TextAttribute sichern; ist fuers Undo mit voll-
		// staendiger Attributierung am besten, wegen den evt.
		// Ueberlappenden Bereichen von An/Aus.
/*N*/ 		pHistory->CopyAttr( pEndTxtNd->GetpSwpHints(), nNdIdx, 0,
/*N*/ 							pEndTxtNd->GetTxt().Len(), TRUE );
/*N*/ 
/*N*/ 		if( pEndTxtNd->GetpSwAttrSet() )
/*?*/ 			{DBG_BF_ASSERT(0, "STRIP");} //STRIP001 pHistory->CopyFmtAttr( *pEndTxtNd->GetpSwAttrSet(), nNdIdx );

		// loesche jetzt noch den Text (alle Attribut-Aenderungen kommen in
		// die Undo-History
/*N*/ 		pEndStr = (String*)new String( pEndTxtNd->GetTxt().Copy( 0,
/*N*/ 									pEnd->nContent.GetIndex() ));
/*N*/ 		pEndTxtNd->Erase( aEndIdx, pEnd->nContent.GetIndex() );
/*N*/ 		if( pEndTxtNd->GetpSwpHints() )
/*?*/ 			pEndTxtNd->GetpSwpHints()->DeRegister();
/*N*/ 	}

	// sind es nur zwei Nodes, dann ist schon alles erledigt.
/*N*/ 	if( ( pSttTxtNd || pEndTxtNd ) && nSttNode + 1 == nEndNode )
/*N*/ 		return FALSE;           // keine Nodes mehr verschieben

/*N*/ 	return TRUE;                // verschiebe die dazwischen liegenden Nodes
/*N*/ }


/*N*/ BOOL SwUndoDelete::CanGrouping( SwDoc* pDoc, const SwPaM& rDelPam )
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	// ist das Undo groesser als 1 Node ? (sprich: Start und EndString)
/*N*/ 	return TRUE;
/*N*/ }



/*N*/ SwUndoDelete::~SwUndoDelete()
/*N*/ {
/*N*/ 	delete pSttStr;
/*N*/ 	delete pEndStr;
/*N*/ 	if( pMvStt )		// loesche noch den Bereich aus dem UndoNodes Array
/*N*/ 	{
/*N*/ 		// Insert speichert den Inhalt in der IconSection
/*N*/ 		pMvStt->GetNode().GetNodes().Delete( *pMvStt, nNode );
/*N*/ 		delete pMvStt;
/*N*/ 	}
/*N*/ 	delete pRedlSaveData;
/*N*/ }







}
