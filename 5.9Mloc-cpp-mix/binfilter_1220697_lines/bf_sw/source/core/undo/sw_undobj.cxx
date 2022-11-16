/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_undobj.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:46:00 $
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

#ifndef _TXTFTN_HXX //autogen
#include <txtftn.hxx>
#endif
#ifndef _FMTANCHR_HXX //autogen
#include <fmtanchr.hxx>
#endif
#ifndef _FTNIDX_HXX //autogen
#include <ftnidx.hxx>
#endif
#ifndef _FRMFMT_HXX //autogen
#include <frmfmt.hxx>
#endif
#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _DOCARY_HXX
#include <docary.hxx>
#endif
#ifndef _SWUNDO_HXX
#include <swundo.hxx>			// fuer die UndoIds
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
#ifndef _BOOKMRK_HXX
#include <bookmrk.hxx>
#endif
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
namespace binfilter {


/*N*/ class SwRedlineSaveData : public SwUndRng, public SwRedlineData,
/*N*/ 						  private SwUndoSaveSection
/*N*/ {
/*N*/ public:
/*N*/ 	SwRedlineSaveData( SwComparePosition eCmpPos,
/*N*/ 						const SwPosition& rSttPos, const SwPosition& rEndPos,
/*N*/ 						SwRedline& rRedl, BOOL bCopyNext );
~SwRedlineSaveData(){DBG_BF_ASSERT(0, "STRIP"); };//STRIP001 /*N*/ 	~SwRedlineSaveData();
/*N*/ };

/*N*/ SV_IMPL_PTRARR( SwUndos, SwUndo*)
/*N*/ SV_IMPL_PTRARR( SwRedlineSaveDatas, SwRedlineSaveDataPtr )


//------------------------------------------------------------

// Diese Klasse speichert den Pam als USHORT's und kann diese wieder zu

// einem PaM zusammensetzen
/*?*/ SwUndRng::SwUndRng()
/*?*/ 	: nSttNode( 0 ), nEndNode( 0 ), nSttCntnt( 0 ), nEndCntnt( 0 )
/*?*/ {
/*?*/ }

/*N*/ SwUndRng::SwUndRng( const SwPaM& rPam )
/*N*/ {
/*N*/ 	SetValues( rPam );
/*N*/ }

/*N*/ void SwUndRng::SetValues( const SwPaM& rPam )
/*N*/ {
/*N*/ 	const SwPosition *pStt = rPam.Start();
/*N*/ 	if( rPam.HasMark() )
/*N*/ 	{
/*N*/ 		const SwPosition *pEnd = rPam.GetPoint() == pStt
/*N*/ 						? rPam.GetMark()
/*N*/ 						: rPam.GetPoint();
/*N*/ 		nEndNode = pEnd->nNode.GetIndex();
/*N*/ 		nEndCntnt = pEnd->nContent.GetIndex();
/*N*/ 	}
/*N*/ 	else
/*N*/ 		// keine Selektion !!
/*N*/ 		nEndNode = 0, nEndCntnt = STRING_MAXLEN;
/*N*/ 
/*N*/ 	nSttNode = pStt->nNode.GetIndex();
/*N*/ 	nSttCntnt = pStt->nContent.GetIndex();
/*N*/ }



//------------------------------------------------------------





/*N*/ SwUndo::~SwUndo()
/*N*/ {
/*N*/ }


//------------------------------------------------------------

/*N*/ SwUndoSaveCntnt::SwUndoSaveCntnt()
/*N*/ 	: pHistory( 0 )
/*N*/ {}

/*N*/ SwUndoSaveCntnt::~SwUndoSaveCntnt()
/*N*/ {
/*N*/ 	delete pHistory;
/*N*/ }

	// wird fuer das Loeschen von Inhalt benoetigt. Fuer das ReDo werden
	// Inhalte in das UndoNodesArray verschoben. Diese Methoden fuegen
	// am Ende eines TextNodes fuer die Attribute einen Trenner ein.
	// Dadurch werden die Attribute nicht expandiert.
	// MoveTo.. 	verschiebt aus dem NodesArray in das UndoNodesArray
	// MoveFrom..	verschiebt aus dem UndoNodesArray in das NodesArray

	// 2.8.93:	ist pEndNdIdx angebenen, wird vom Undo/Redo -Ins/DelFly
	//			aufgerufen. Dann soll die gesamte Section verschoben werden.

/*N*/void SwUndoSaveCntnt::MoveToUndoNds( SwPaM& rPaM, SwNodeIndex* pNodeIdx,
/*N*/					SwIndex* pCntIdx, ULONG* pEndNdIdx, xub_StrLen* pEndCntIdx )
/*N*/{
DBG_BF_ASSERT(0, "STRIP"); //STRIP001  	SwDoc& rDoc = *rPaM.GetDoc();
/*N*/ }


// diese beiden Methoden bewegen den Point vom Pam zurueck/vor. Damit
// kann fuer ein Undo/Redo ein Bereich aufgespannt werden. (Der
// Point liegt dann vor dem manipuliertem Bereich !!)
// Das Flag gibt an, ob noch vorm Point Inhalt steht.




/*
   JP 21.03.94: loesche alle Objecte, die ContentIndizies auf den ang.
				Bereich besitzen.
				Zur Zeit gibts folgende Objecte
					- Fussnoten
					- Flys
					- Bookmarks
					- Verzeichnisse
*/

/*N*/ void SwUndoSaveCntnt::DelCntntIndex( const SwPosition& rMark,
/*N*/ 								     const SwPosition& rPoint,
/*N*/ 									 DelCntntType nDelCntntType )
/*N*/ {
/*N*/ 	const SwPosition *pStt = rMark < rPoint ? &rMark : &rPoint,
/*N*/ 					*pEnd = &rMark == pStt ? &rPoint : &rMark;
/*N*/ 
/*N*/ 	SwDoc* pDoc = rMark.nNode.GetNode().GetDoc();
/*N*/ 	BOOL bDoesUndo = pDoc->DoesUndo();
/*N*/ 	pDoc->DoUndo( FALSE );
/*N*/ 
/*N*/ 	// 1. Fussnoten
/*N*/ 	if( DELCNT_FTN & nDelCntntType )
/*N*/ 	{
/*N*/ 		SwFtnIdxs& rFtnArr = pDoc->GetFtnIdxs();
/*N*/ 		if( rFtnArr.Count() )
/*N*/ 		{
/*N*/ 			const SwNode* pFtnNd;
/*N*/ 			USHORT nPos;
/*N*/ 			rFtnArr.SeekEntry( pStt->nNode, &nPos );
/*N*/ 			SwTxtFtn* pSrch;
/*N*/ 
/*N*/ 			// loesche erstmal alle, die dahinter stehen
/*N*/ 			while( nPos < rFtnArr.Count() && ( pFtnNd =
/*N*/ 				&( pSrch = rFtnArr[ nPos ] )->GetTxtNode())->GetIndex()
/*N*/ 						<= pEnd->nNode.GetIndex() )
/*N*/ 			{
/*?*/ 				xub_StrLen nFtnSttIdx = *pSrch->GetStart();
/*?*/ 				if( (DELCNT_CHKNOCNTNT & nDelCntntType )
/*?*/ 					? (&pEnd->nNode.GetNode() == pFtnNd )
/*?*/ 					: (( &pStt->nNode.GetNode() == pFtnNd &&
/*?*/ 					pStt->nContent.GetIndex() > nFtnSttIdx) ||
/*?*/ 					( &pEnd->nNode.GetNode() == pFtnNd &&
/*?*/ 					nFtnSttIdx >= pEnd->nContent.GetIndex() )) )
/*?*/ 				{
/*?*/ 					++nPos;		// weiter suchen
/*?*/ 					continue;
/*?*/ 				}
/*?*/ 
/*?*/ 				// es muss leider ein Index angelegt werden. Sonst knallts im
/*?*/ 				// TextNode, weil im DTOR der SwFtn dieser geloescht wird !!
/*?*/ 				SwTxtNode* pTxtNd = (SwTxtNode*)pFtnNd;
/*?*/ 				if( !pHistory )
/*?*/ 					pHistory = new SwHistory;
/*?*/ 				SwTxtAttr* pFtnHnt = pTxtNd->GetTxtAttr( nFtnSttIdx );
/*?*/ 				ASSERT( pFtnHnt, "kein FtnAttribut" );
/*?*/ 				SwIndex aIdx( pTxtNd, nFtnSttIdx );
/*?*/ 				pHistory->Add( pFtnHnt, pTxtNd->GetIndex(), FALSE );
/*?*/ 				pTxtNd->Erase( aIdx, 1 );
/*N*/ 			}
/*N*/ 
/*N*/ 			while( nPos-- && ( pFtnNd = &( pSrch = rFtnArr[ nPos ] )->
/*N*/ 					GetTxtNode())->GetIndex() >= pStt->nNode.GetIndex() )
/*N*/ 			{
/*?*/ 				xub_StrLen nFtnSttIdx = *pSrch->GetStart();
/*?*/ 				if( !(DELCNT_CHKNOCNTNT & nDelCntntType) && (
/*?*/ 					( &pStt->nNode.GetNode() == pFtnNd &&
/*?*/ 					pStt->nContent.GetIndex() > nFtnSttIdx ) ||
/*?*/ 					( &pEnd->nNode.GetNode() == pFtnNd &&
/*?*/ 					nFtnSttIdx >= pEnd->nContent.GetIndex() )))
/*?*/ 					continue;				// weiter suchen
/*?*/ 
/*?*/ 				// es muss leider ein Index angelegt werden. Sonst knallts im
/*?*/ 				// TextNode, weil im DTOR der SwFtn dieser geloescht wird !!
/*?*/ 				SwTxtNode* pTxtNd = (SwTxtNode*)pFtnNd;
/*?*/ 				if( !pHistory )
/*?*/ 					pHistory = new SwHistory;
/*?*/ 				SwTxtAttr* pFtnHnt = pTxtNd->GetTxtAttr( nFtnSttIdx );
/*?*/ 				ASSERT( pFtnHnt, "kein FtnAttribut" );
/*?*/ 				SwIndex aIdx( pTxtNd, nFtnSttIdx );
/*?*/ 				pHistory->Add( pFtnHnt, pTxtNd->GetIndex(), FALSE );
/*?*/ 				pTxtNd->Erase( aIdx, 1 );
/*?*/ 			}
/*N*/ 		}
/*N*/ 	}

	// 2. Flys
/*N*/ 	if( DELCNT_FLY & nDelCntntType )
/*N*/ 	{
/*N*/ 		USHORT nChainInsPos = pHistory ? pHistory->Count() : 0;
/*N*/ 		const SwSpzFrmFmts& rSpzArr = *pDoc->GetSpzFrmFmts();
/*N*/ 		if( rSpzArr.Count() )
/*N*/ 		{
/*N*/ 			const BOOL bDelFwrd = rMark.nNode.GetIndex() <= rPoint.nNode.GetIndex();
/*N*/ 			SwFlyFrmFmt* pFmt;
/*N*/ 			const SwFmtAnchor* pAnchor;
/*N*/ 			USHORT n = rSpzArr.Count();
/*N*/ 			const SwPosition* pAPos;
/*N*/ 
/*N*/ 			while( n && rSpzArr.Count() )
/*N*/ 			{
/*N*/ 				pFmt = (SwFlyFrmFmt*)rSpzArr[--n];
/*N*/ 				pAnchor = &pFmt->GetAnchor();
/*N*/ 				switch( pAnchor->GetAnchorId() )
/*N*/ 				{
/*N*/ 				case FLY_IN_CNTNT:
/*N*/ 					if( 0 != (pAPos = pAnchor->GetCntntAnchor() ) &&
/*N*/ 						(( DELCNT_CHKNOCNTNT & nDelCntntType )
/*N*/ 						? ( pStt->nNode <= pAPos->nNode &&
/*N*/ 							pAPos->nNode < pEnd->nNode )
/*N*/ 						: ( *pStt <= *pAPos && *pAPos < *pEnd )) )
/*N*/ 					{
/*?*/ 						if( !pHistory )
/*?*/ 							pHistory = new SwHistory;
/*?*/ 						SwTxtNode* pTxtNd = pDoc->GetNodes()[ pAPos->nNode]->GetTxtNode();
/*?*/ 						SwTxtAttr* pFlyHnt = pTxtNd->GetTxtAttr( pAPos->nContent.GetIndex());
/*?*/ 						ASSERT( pFlyHnt, "kein FlyAttribut" );
/*?*/ 						pHistory->Add( pFlyHnt, 0, FALSE );
/*?*/ 						// n wieder zurueck, damit nicht ein Format uebesprungen wird !
/*?*/ 						n = n >= rSpzArr.Count() ? rSpzArr.Count() : n+1;
/*N*/ 					}
/*N*/ 					break;
/*N*/ 				case FLY_AT_CNTNT:
/*N*/ 
/*N*/ 					if( 0 != (pAPos = pAnchor->GetCntntAnchor() ) &&
/*N*/ 						(( DELCNT_CHKNOCNTNT & nDelCntntType )
/*N*/ 						? ( pStt->nNode <= pAPos->nNode &&
/*N*/ 							pAPos->nNode < pEnd->nNode )
/*N*/ 						: ( bDelFwrd
/*N*/ 							? rMark.nNode < pAPos->nNode &&
/*N*/ 										pAPos->nNode <= rPoint.nNode
/*N*/ 							: rPoint.nNode <= pAPos->nNode &&
/*N*/ 										pAPos->nNode < rMark.nNode )) )
/*N*/ 					{
/*?*/ 						if( !pHistory )
/*?*/ 							pHistory = new SwHistory;
/*?*/ 
/*?*/ 						// nur den Anker verchieben ??
/*?*/ 						if( !( DELCNT_CHKNOCNTNT & nDelCntntType ) &&
/*?*/ 							rPoint.nNode.GetIndex() == pAPos->nNode.GetIndex())
/*?*/ 						{
/*?*/ 							pHistory->Add( *pFmt );
/*?*/ 
/*?*/ 							SwFmtAnchor aAnch( *pAnchor );
/*?*/ 							SwPosition aPos( rMark.nNode );
/*?*/ 							aAnch.SetAnchor( &aPos );
/*?*/ 							pFmt->SetAttr( aAnch );
/*?*/ 						}
/*?*/ 						else
/*?*/ 						{
/*?*/ 							pHistory->Add( *pFmt, nChainInsPos );
/*?*/ 							// n wieder zurueck, damit nicht ein Format uebesprungen wird !
/*?*/ 							n = n >= rSpzArr.Count() ? rSpzArr.Count() : n+1;
/*?*/ 						}
/*N*/ 					}
/*N*/ 					break;
/*N*/ 				case FLY_AUTO_CNTNT:
/*?*/ 					if( 0 != (pAPos = pAnchor->GetCntntAnchor() ) &&
/*?*/ 						(( DELCNT_CHKNOCNTNT & nDelCntntType )
/*?*/ 						? ( pStt->nNode <= pAPos->nNode &&
/*?*/ 							pAPos->nNode < pEnd->nNode )
/*?*/ 						: ( *pStt <= *pAPos && *pAPos < *pEnd )) )
/*?*/ 					{
/*?*/ 						if( !pHistory )
/*?*/ 							pHistory = new SwHistory;
/*?*/ 						if( ( DELCNT_CHKNOCNTNT & nDelCntntType ) ||
/*?*/ 							( ( pAPos->nNode < pEnd->nNode ) &&
/*?*/ 								( pStt->nNode < pAPos->nNode ||
/*?*/ 								  !pStt->nContent.GetIndex() ) ) )
/*?*/ 						{
/*?*/ 							pHistory->Add( *pFmt, nChainInsPos );
/*?*/ 							n = n >= rSpzArr.Count() ? rSpzArr.Count() : n+1;
/*?*/ 						}
/*?*/ 						else
/*?*/ 						{
/*?*/ 							pHistory->Add( *pFmt );
/*?*/ 
/*?*/ 							SwFmtAnchor aAnch( *pAnchor );
/*?*/ 							aAnch.SetAnchor( &rMark );
/*?*/ 							pFmt->SetAttr( aAnch );
/*?*/ 						}
/*?*/ 					}
/*?*/ 					break;
/*?*/ 				case FLY_AT_FLY:
/*?*/ 
/*?*/ 					if( 0 != (pAPos = pAnchor->GetCntntAnchor() ) &&
/*?*/ 						pStt->nNode == pAPos->nNode )
/*?*/ 					{
/*?*/ 						if( !pHistory )
/*?*/ 							pHistory = new SwHistory;
/*?*/ 
/*?*/ 						pHistory->Add( *pFmt, nChainInsPos );
/*?*/ 
/*?*/ 						// n wieder zurueck, damit nicht ein Format uebesprungen wird !
/*?*/ 						n = n >= rSpzArr.Count() ? rSpzArr.Count() : n+1;
/*?*/ 					}
/*?*/ 					break;
/*?*/ 				}
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}

	// 3. Bookmarks
/*N*/ 	if( DELCNT_BKM & nDelCntntType )
/*N*/ 	{
/*N*/ 		const SwBookmarks& rBkmkTbl = pDoc->GetBookmarks();
/*N*/ 		if( rBkmkTbl.Count() )
/*N*/ 		{
/*N*/ 			const SwBookmark* pBkmk;
/*N*/ 
/*N*/ 			for( USHORT n = 0; n < rBkmkTbl.Count(); ++n )
/*N*/ 			{
/*N*/ 				BYTE nTyp = 0;
/*N*/ 				if( ( DELCNT_CHKNOCNTNT & nDelCntntType )
/*N*/ 					? (pStt->nNode <= ( pBkmk = rBkmkTbl[ n ])->GetPos().nNode &&
/*N*/ 									pBkmk->GetPos().nNode < pEnd->nNode )
/*N*/ 					: (*pStt <= ( pBkmk = rBkmkTbl[ n ])->GetPos() &&
/*N*/ 									pBkmk->GetPos() < *pEnd ) )
/*N*/ 					nTyp = SwHstryBookmark::BKMK_POS;
/*N*/ 
/*N*/ 				if( pBkmk->GetOtherPos() &&
/*N*/ 					(( DELCNT_CHKNOCNTNT & nDelCntntType )
/*N*/ 					? (pStt->nNode <= pBkmk->GetOtherPos()->nNode &&
/*N*/ 						pBkmk->GetOtherPos()->nNode < pEnd->nNode )
/*N*/ 					: ( *pStt <= *pBkmk->GetOtherPos() &&
/*N*/ 						*pBkmk->GetOtherPos() < *pEnd )) )
/*?*/ 					nTyp |= SwHstryBookmark::BKMK_OTHERPOS;
/*N*/ 
/*N*/ 				if( nTyp )
/*N*/ 				{
/*N*/ 					if( !pHistory )
/*?*/ 						pHistory = new SwHistory;
/*N*/ 
/*N*/ 					pHistory->Add( *pBkmk, nTyp );
/*N*/ 					if( (SwHstryBookmark::BKMK_OTHERPOS|
/*N*/ 						SwHstryBookmark::BKMK_POS) == nTyp ||
/*N*/ 						( SwHstryBookmark::BKMK_POS == nTyp
/*N*/ 							&& !pBkmk->GetOtherPos() ))
/*N*/ 						pDoc->DelBookmark( n-- );
/*N*/ 				}
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ 
/*N*/ 	pDoc->DoUndo( bDoesUndo );
/*N*/ }


// sicher eine vollstaendige Section im Undo-Nodes-Array

/*N*/ SwUndoSaveSection::SwUndoSaveSection()
/*N*/ 	: nStartPos( ULONG_MAX ), pMvStt( 0 ), nMvLen( 0 ), pRedlSaveData( 0 )
/*N*/ {
/*N*/ }

/*N*/ SwUndoSaveSection::~SwUndoSaveSection()
/*N*/ {
/*N*/ 	if( pMvStt )		// loesche noch den Bereich aus dem UndoNodes Array
/*N*/ 	{
/*?*/ 		// SaveSection speichert den Inhalt in der PostIt-Section
/*?*/ 		SwNodes& rUNds = pMvStt->GetNode().GetNodes();
/*?*/ 		rUNds.Delete( *pMvStt, nMvLen );
/*?*/ 
/*?*/ 		delete pMvStt;
/*N*/ 	}
/*N*/ 	delete pRedlSaveData;
/*N*/ }

/*N*/ void SwUndoSaveSection::SaveSection( SwDoc* pDoc, const SwNodeIndex& rSttIdx )
/*N*/ {
/*N*/ 	SwNodeRange aRg( rSttIdx.GetNode(), *rSttIdx.GetNode().EndOfSectionNode() );
/*N*/ 	SaveSection( pDoc, aRg );
/*N*/ }


/*N*/ void SwUndoSaveSection::SaveSection( SwDoc* pDoc, const SwNodeRange& rRange )
/*N*/ {
/*N*/ 	SwPaM aPam( rRange.aStart, rRange.aEnd );
/*N*/ 
/*N*/ 	// loesche alle Fussnoten / FlyFrames / Bookmarks / Verzeichnisse
/*N*/ 	DelCntntIndex( *aPam.GetMark(), *aPam.GetPoint() );
/*N*/ 
/*N*/ 	pRedlSaveData = new SwRedlineSaveDatas;
/*N*/ 	if( !SwUndo::FillSaveData( aPam, *pRedlSaveData, TRUE, TRUE ))
/*N*/ 		delete pRedlSaveData, pRedlSaveData = 0;
/*N*/ 
/*N*/ 	nStartPos = rRange.aStart.GetIndex();
/*N*/ 
/*N*/ 	aPam.GetPoint()->nNode--;
/*N*/ 	aPam.GetMark()->nNode++;
/*N*/ 
/*N*/ 	SwCntntNode* pCNd = aPam.GetCntntNode( FALSE );
/*N*/ 	if( pCNd )
/*N*/ 		aPam.GetMark()->nContent.Assign( pCNd, 0 );
/*N*/ 	if( 0 != ( pCNd = aPam.GetCntntNode( TRUE )) )
/*N*/ 		aPam.GetPoint()->nContent.Assign( pCNd, pCNd->Len() );
/*N*/ 
/*N*/ 	// Positionen als SwIndex merken, damit im DTOR dieser Bereich
/*N*/ 	// entfernt werden kann !!
/*N*/ 	ULONG nEnd = 0;
/*N*/ 	pMvStt = new SwNodeIndex( rRange.aStart );
/*N*/ 	MoveToUndoNds( aPam, pMvStt, 0, &nEnd, 0 );
/*N*/ 	nMvLen = nEnd - pMvStt->GetIndex() + 1;
/*N*/ }




// START
/*N*/ SwUndoStart::SwUndoStart( USHORT nId )
/*N*/ 	: SwUndo( UNDO_START ), nUserId( nId ), nEndOffset( 0 )
/*N*/ {
/*N*/ }





// END
/*N*/ SwUndoEnd::SwUndoEnd( USHORT nId )
/*N*/ 	: SwUndo( UNDO_END ), nUserId( nId ), nSttOffset( 0 )
/*N*/ {
/*N*/ }




/*  */
		// sicher und setze die RedlineDaten

/*N*/ SwRedlineSaveData::SwRedlineSaveData( SwComparePosition eCmpPos,
/*N*/ 										const SwPosition& rSttPos,
/*N*/ 										const SwPosition& rEndPos,
/*N*/ 										SwRedline& rRedl,
/*N*/ 										BOOL bCopyNext )
/*N*/ 	: SwUndRng( rRedl ),
/*N*/ 	SwRedlineData( rRedl.GetRedlineData(), bCopyNext )
/*N*/ {
			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 //STRIP001 	ASSERT( POS_OUTSIDE == eCmpPos ||
/*N*/ }



/*N*/ BOOL SwUndo::FillSaveData( const SwPaM& rRange, SwRedlineSaveDatas& rSData,
/*N*/ 							BOOL bDelRange, BOOL bCopyNext )
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); return FALSE;//STRIP001 	if( rSData.Count() )
/*N*/ }





}
