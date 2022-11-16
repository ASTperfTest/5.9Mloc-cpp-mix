/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_rolbck.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:45:10 $
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


#ifndef _SFXITEMITER_HXX //autogen
#include <svtools/itemiter.hxx>
#endif

#ifndef _FMTFTN_HXX //autogen
#include <fmtftn.hxx>
#endif
#ifndef _FCHRFMT_HXX //autogen
#include <fchrfmt.hxx>
#endif
#ifndef _FMTFLD_HXX //autogen
#include <fmtfld.hxx>
#endif
#ifndef _TXTFLD_HXX //autogen
#include <txtfld.hxx>
#endif
#ifndef _TXTFTN_HXX //autogen
#include <txtftn.hxx>
#endif
#ifndef _FMTPDSC_HXX //autogen
#include <fmtpdsc.hxx>
#endif
#ifndef _FRMFMT_HXX //autogen
#include <frmfmt.hxx>
#endif
#ifndef _FTNIDX_HXX
#include <ftnidx.hxx>
#endif
#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>              // SwDoc.GetNodes()
#endif
#ifndef _NDTXT_HXX
#include <ndtxt.hxx>			// SwTxtNode
#endif
#ifndef _PARATR_HXX
#include <paratr.hxx>			//
#endif
#ifndef _FLDBAS_HXX
#include <fldbas.hxx>			// fuer Felder
#endif
#ifndef _PAM_HXX
#include <pam.hxx>				// fuer SwPaM
#endif
#ifndef _HINTS_HXX
#include <hints.hxx> 			// fuer SwHytrSetAttrSet
#endif
#ifndef _ROLBCK_HXX
#include <rolbck.hxx>
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>			// fuer UndoDelete
#endif
#ifndef _BOOKMRK_HXX
#include <bookmrk.hxx> 			// fuer SwBookmark
#endif
namespace binfilter {




/*N*/ SV_IMPL_PTRARR( SwpHstry, SwHstryHintPtr)


/*N*/ SwSetFmtHint::SwSetFmtHint( const SfxPoolItem* pFmtHt, ULONG nNd )
/*N*/ 	:  SwHstryHint( HSTRY_SETFMTHNT ),
/*N*/ 	pAttr( pFmtHt->Clone() ), nNode( nNd ), nNumLvl( NO_NUMBERING ),
/*N*/ 	nSetStt( USHRT_MAX )
/*N*/ {
/*N*/ 	switch( pAttr->Which() )
/*N*/ 	{
/*N*/ 	case RES_PAGEDESC:
/*N*/ 		((SwFmtPageDesc*)pAttr)->ChgDefinedIn( 0 );
/*N*/ 		break;
/*N*/ 	case RES_PARATR_DROP:
/*N*/ 		((SwFmtDrop*)pAttr)->ChgDefinedIn( 0 );
/*N*/ 		break;
/*?*/ 	case RES_BOXATR_FORMULA:
/*?*/ 		{
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 //JP 30.07.98: Bug 54295 - Formeln immer im Klartext speichern
/*?*/ 		}
/*?*/ 		break;
/*?*/ 	case RES_PARATR_NUMRULE:
/*?*/ 		{
/*?*/ 			const SwModify* pMod = ((SwNumRuleItem*)pFmtHt)->GetDefinedIn();
/*?*/ 			const SwNodeNum* pNdNum;
/*?*/ 			if( pMod && pMod->ISA( SwTxtNode ) &&
/*?*/ 				0 != (pNdNum = ((SwTxtNode*)pMod)->GetNum() ) )
/*?*/ 			{
/*?*/ 				nNumLvl = pNdNum->GetLevel();
/*?*/ 				bNumStt = pNdNum->IsStart();
/*?*/ 				nSetStt = pNdNum->GetSetValue();
/*?*/ 			}
/*?*/ 			((SwNumRuleItem*)pAttr)->ChgDefinedIn( 0 );
/*?*/ 		}
/*?*/ 		break;
/*?*/ 	}
/*N*/ }




/*N*/ SwSetFmtHint::~SwSetFmtHint()
/*N*/ {
/*N*/ 	delete pAttr;
/*N*/ }


/*N*/ SwResetFmtHint::SwResetFmtHint( const SfxPoolItem* pFmtHt, ULONG nNd )
/*N*/ 	: SwHstryHint( HSTRY_RESETFMTHNT ),
/*N*/ 	nWhich( pFmtHt->Which() ), nNode( nNd )
/*N*/ {
/*N*/ }




/*N*/ SwSetTxtHint::SwSetTxtHint( SwTxtAttr* pTxtHt, ULONG nNodePos )
/*N*/ 	: SwHstryHint( HSTRY_SETTXTHNT )
/*N*/ {
/*N*/ 	// !! Achtung: folgende Attribute erzeugen keine FormatAttribute:
/*N*/ 	//  - NoLineBreak, NoHypen, Inserted, Deleted
/*N*/ 	// Dafuer muessen Sonderbehandlungen gemacht werden !!!
/*N*/ 
/*N*/ 	// ein bisschen kompliziert, aber ist Ok so: erst vom default
/*N*/ 	// eine Kopie und dann die Werte aus dem Text Attribut zuweisen
/*N*/ 	USHORT nWhich = pTxtHt->Which();
/*N*/ 	if( RES_TXTATR_CHARFMT == nWhich )
/*N*/ 		pAttr = new SwFmtCharFmt( pTxtHt->GetCharFmt().GetCharFmt() );
/*N*/ 	else
/*N*/ 		pAttr = pTxtHt->GetAttr().Clone();
/*N*/ 	nNode = nNodePos;
/*N*/ 	nStart = *pTxtHt->GetStart();
/*N*/ 	nEnd = *pTxtHt->GetAnyEnd();
/*N*/ }


/*N*/ SwSetTxtHint::~SwSetTxtHint()
/*N*/ {
/*N*/ 	delete pAttr;
/*N*/ }




/*N*/ SwSetTxtFldHint::SwSetTxtFldHint( SwTxtFld* pTxtFld, ULONG nNodePos )
/*N*/ 	: SwHstryHint( HSTRY_SETTXTFLDHNT ), pFldType( 0 )
/*N*/ {
/*N*/ 	pFld = new SwFmtFld( *pTxtFld->GetFld().GetFld() );
/*N*/ 
/*N*/ 	// nur kopieren wenn kein Sys-FieldType
/*N*/ 	SwDoc* pDoc = (SwDoc*)pTxtFld->GetTxtNode().GetDoc();
/*N*/ 
/*N*/ 	nFldWhich = pFld->GetFld()->GetTyp()->Which();
/*N*/ 	if( nFldWhich == RES_DBFLD ||
/*N*/ 		nFldWhich == RES_USERFLD ||
/*N*/ 		nFldWhich == RES_SETEXPFLD ||
/*N*/ 		nFldWhich == RES_DDEFLD ||
/*N*/ 		!pDoc->GetSysFldType( nFldWhich ))
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pFldType = pFld->GetFld()->GetTyp()->Copy();
/*N*/ 	}
/*N*/ 	nNode = nNodePos;
/*N*/ 	nPos = *pTxtFld->GetStart();
/*N*/ }


/*N*/ SwSetTxtFldHint::~SwSetTxtFldHint()
/*N*/ {
/*N*/ 	delete pFld;
/*N*/ 	delete pFldType;
/*N*/ }















/*N*/ SwResetTxtHint::SwResetTxtHint( USHORT nWhich, xub_StrLen nAttrStt,
/*N*/ 								xub_StrLen nAttrEnd, ULONG nNodePos )
/*N*/ 	: SwHstryHint( HSTRY_RESETTXTHNT ),
/*N*/ 	nAttr( nWhich ), nNode( nNodePos ), nStart( nAttrStt ), nEnd( nAttrEnd )
/*N*/ {
/*N*/ }




/*N*/ SwSetFtnHint::SwSetFtnHint( SwTxtFtn* pTxtFtn, ULONG nNodePos )
/*N*/ 	: SwHstryHint( HSTRY_SETFTNHNT ),
/*N*/ 	nNode( nNodePos ), nStart( *pTxtFtn->GetStart() )
/*N*/ {
/*N*/ 	ASSERT( pTxtFtn->GetStartNode(), "Footnote ohne Section" );
/*N*/ 
/*N*/ 	aFtnStr = pTxtFtn->GetFtn().GetNumStr();
/*N*/ 	bEndNote = pTxtFtn->GetFtn().IsEndNote();
/*N*/ 
/*N*/ 	// merke die alte NodePos, denn wer weiss was alles in der SaveSection
/*N*/ 	// gespeichert (geloescht) wird
/*N*/ 	SwDoc* pDoc = (SwDoc*)pTxtFtn->GetTxtNode().GetDoc();
/*N*/ 	SwNode* pSaveNd = pDoc->GetNodes()[ nNode ];
/*N*/ 
/*N*/ 	//Pointer auf StartNode der FtnSection merken und erstmal den Pointer im
/*N*/ 	//Attribut zuruecksetzen -> Damit werden automatisch die Frms vernichtet.
/*N*/ 	SwNodeIndex aSttIdx( *pTxtFtn->GetStartNode() );
/*N*/ 	pTxtFtn->SetStartNode( 0, FALSE );
/*N*/ 
/*N*/ 	pUndo = new SwUndoSaveSection;
/*N*/ 	pUndo->SaveSection( pDoc, aSttIdx );
/*N*/ 	nNode = pSaveNd->GetIndex();
/*N*/ }

/*N*/ SwSetFtnHint::SwSetFtnHint( const SwTxtFtn &rTxtFtn ) :
/*N*/ 	SwHstryHint( HSTRY_SETFTNHNT ),
/*N*/ 	nNode( _SwTxtFtn_GetIndex( (&rTxtFtn) ) ),
/*N*/ 	nStart( *rTxtFtn.GetStart() ),
/*N*/ 	pUndo( 0 )
/*N*/ {
/*N*/ 	ASSERT( rTxtFtn.GetStartNode(), "Footnote ohne Section" );
/*N*/ 
/*N*/ 	aFtnStr = rTxtFtn.GetFtn().GetNumStr();
/*N*/ 	bEndNote = rTxtFtn.GetFtn().IsEndNote();
/*N*/ }


/*N*/ SwSetFtnHint::~SwSetFtnHint()
/*N*/ {
/*N*/ 	delete pUndo;
/*N*/ }




/*N*/ SwChgFmtColl::SwChgFmtColl( const SwFmtColl* pFmtColl, ULONG nNd,
/*N*/ 							BYTE nNodeWhich )
/*N*/ 	: SwHstryHint( HSTRY_CHGFMTCOLL ),
/*N*/ 	pColl( pFmtColl ), nNode( nNd ), nNdWhich( nNodeWhich ),
/*N*/ 	nNumLvl( NO_NUMBERING ), nSetStt( USHRT_MAX )
/*N*/ {
/*N*/ 	const SwDoc* pDoc = pFmtColl->GetDoc();
/*N*/ 	const SwTxtNode* pTxtNd = pDoc->GetNodes()[ nNode ]->GetTxtNode();
/*N*/ 	if( pTxtNd && pTxtNd->GetNum() )
/*N*/ 	{
/*?*/ 		nNumLvl = pTxtNd->GetNum()->GetLevel();
/*?*/ 		bNumStt = pTxtNd->GetNum()->IsStart();
/*?*/ 		nSetStt = pTxtNd->GetNum()->GetSetValue();
/*N*/ 	}
/*N*/ }


/*N*/SwHstryTxtFlyCnt::SwHstryTxtFlyCnt( SwFlyFrmFmt* pFlyFmt )
/*N*/	: SwHstryHint( HSTRY_FLYCNT )
/*N*/{
/*?*/DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	ASSERT( pFlyFmt, "kein Format" );
/*N*/}


/*N*/SwHstryTxtFlyCnt::~SwHstryTxtFlyCnt()
/*N*/{
/*N*/	delete pUndo;
/*N*/}


/*N*/ void SwHstryTxtFlyCnt::SetInDoc( SwDoc* pDoc, BOOL bTmpSet )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*N*/ }



// JP 21.03.94: jetzt auch die Bookmarks in die History aufnehmen
/*N*/ SwHstryBookmark::SwHstryBookmark( const SwBookmark& rBkmk, BYTE nType )
/*N*/ 	: SwHstryHint( HSTRY_BOOKMARK ),
/*N*/ 	nTyp( nType ), nNode1( 0 ), nCntnt1( 0 ),
/*N*/ 	nNode2( rBkmk.GetOtherPos() ? 0 : ULONG_MAX ), nCntnt2( 0 )
/*N*/ {
/*N*/ 	aName = rBkmk.GetName();
/*N*/ 	aShortName = rBkmk.GetShortName();
/*N*/ 	nKeyCode = rBkmk.GetKeyCode().GetCode() | rBkmk.GetKeyCode().GetModifier();
/*N*/ 
/*N*/ 	if( BKMK_POS & nTyp )
/*N*/ 	{
/*N*/ 		nNode1 = rBkmk.GetPos().nNode.GetIndex();
/*N*/ 		nCntnt1 = rBkmk.GetPos().nContent.GetIndex();
/*N*/ 	}
/*N*/ 	if( BKMK_OTHERPOS & nTyp )
/*N*/ 	{
/*?*/ 		nNode2 = rBkmk.GetOtherPos()->nNode.GetIndex();
/*?*/ 		nCntnt2 = rBkmk.GetOtherPos()->nContent.GetIndex();
/*N*/ 	}
/*N*/ }





/*************************************************************************/


/*N*/ SwHstrySetAttrSet::SwHstrySetAttrSet( const SfxItemSet& rSet, ULONG nNodePos,
/*N*/ 										const SvUShortsSort& rSetArr )
/*N*/ 	: SwHstryHint( HSTRY_SETATTRSET ),
/*N*/ 	nNode( nNodePos ), aOldSet( rSet ), nNumLvl( NO_NUMBERING ),
/*N*/ 	nSetStt( USHRT_MAX ), aResetArr( 0, 4 )
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	SfxItemIter aIter( aOldSet ), aOrigIter( rSet );
/*N*/ }


/*************************************************************************/


/*N*/ SwHstryResetAttrSet::SwHstryResetAttrSet( const SfxItemSet& rSet,
/*N*/ 					ULONG nNodePos, xub_StrLen nAttrStt, xub_StrLen nAttrEnd )
/*N*/ 	: SwHstryHint( HSTRY_RESETATTRSET ),
/*N*/ 	nNode( nNodePos ), nStart( nAttrStt ), nEnd( nAttrEnd ),
/*N*/ 	aArr( (BYTE)rSet.Count() )
/*N*/ {
/*N*/ 	SfxItemIter aIter( rSet );
/*N*/ 	while( TRUE )
/*N*/ 	{
/*N*/ 		aArr.Insert( aIter.GetCurItem()->Which() ,aArr.Count() );
/*N*/ 		if( aIter.IsAtEnd() )
/*N*/ 			break;
/*?*/ 		aIter.NextItem();
/*N*/ 	}
/*N*/ }




/*************************************************************************/


/*N*/SwHstryChgFlyAnchor::SwHstryChgFlyAnchor( const SwFrmFmt& rFmt )
/*N*/	: SwHstryHint( HSTRY_CHGFLYANCHOR ),
/*N*/	pFmt( (SwFrmFmt*)&rFmt )
/*N*/{
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	const SwFmtAnchor& rAnchor = rFmt.GetAnchor();
/*N*/ }




/*************************************************************************/






/*  */


/*N*/ SwHistory::SwHistory( USHORT nInitSz, USHORT nGrowSz )
/*N*/ 	: SwpHstry( (BYTE)nInitSz, (BYTE)nGrowSz ), nEndDiff( 0 )
/*N*/ {}


/*N*/ SwHistory::~SwHistory()
/*N*/ {
/*N*/ 	Delete( 0 );
/*N*/ }


/*************************************************************************
|*
|*    void SwHistory::Add()
|*
|*    Beschreibung		Dokument 1.0
|*    Ersterstellung    JP 18.02.91
|*    Letzte Aenderung  JP 18.02.91
|*
*************************************************************************/


/*N*/ void SwHistory::Add( const SfxPoolItem* pOldValue, const SfxPoolItem* pNewValue,
/*N*/ 					ULONG nNodeIdx )
/*N*/ {
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 
/*N*/ 	USHORT nWhich = pNewValue->Which();
/*N*/ 	if( (nWhich >= POOLATTR_END) || (nWhich == RES_TXTATR_FIELD) )
/*N*/ 		return;
/*N*/ 
/*N*/ 	// kein default Attribut ??
/*N*/ 	SwHstryHint * pHt;
/*N*/ #ifdef JP_NEWCORE
/*N*/ 	if( pOldValue && pOldValue->GetFmt() &&
/*N*/ 		( !pNewValue->GetFmt() || pOldValue->GetFmt()==pNewValue->GetFmt()) )
/*N*/ #else
/*N*/ 	if( pOldValue && pOldValue != GetDfltAttr( pOldValue->Which() ) )
/*N*/ #endif
/*N*/ 		pHt = new SwSetFmtHint( (SfxPoolItem*)pOldValue, nNodeIdx );
/*N*/ 	else
/*N*/ 		pHt = new SwResetFmtHint( (SfxPoolItem*)pNewValue, nNodeIdx );
/*N*/ 	Insert( pHt, Count() );
/*N*/ }


/*N*/ void SwHistory::Add( const SwTxtAttr* pHint, ULONG nNodeIdx, BOOL bNewAttr )
/*N*/ {
/*N*/ 
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 
/*N*/ 	SwHstryHint * pHt;
/*N*/ 	USHORT nAttrWhich = pHint->Which();
/*N*/ 
/*N*/ 	if( !bNewAttr )
/*N*/ 	{
/*N*/ 		switch ( nAttrWhich )
/*N*/ 		{
/*N*/ 		case RES_TXTATR_FTN:
/*N*/ 			pHt = new SwSetFtnHint( (SwTxtFtn*)pHint, nNodeIdx );
/*N*/ 			break;
/*N*/ 		case RES_TXTATR_FLYCNT:
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	pHt = new SwHstryTxtFlyCnt( (SwTxtFlyCnt*)pHint );
/*?*/ 			break;
/*?*/ 		case RES_TXTATR_FIELD:
/*N*/ 			pHt = new SwSetTxtFldHint( (SwTxtFld*)pHint, nNodeIdx );
/*?*/ 			break;
/*?*/ 		case RES_TXTATR_TOXMARK:
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	pHt = new SwSetTOXMarkHint( (SwTxtTOXMark*)pHint, nNodeIdx );
/*?*/ 			break;
/*?*/ 		case RES_TXTATR_REFMARK:
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pHt = new SwSetRefMarkHint( (SwTxtRefMark*)pHint, nNodeIdx );
/*?*/ 			break;
/*?*/ 		default:
/*N*/ 			pHt = new SwSetTxtHint((SwTxtAttr*)pHint, nNodeIdx );
/*N*/ 		}
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*N*/ 		pHt = new SwResetTxtHint( pHint->Which(), *pHint->GetStart(),
/*N*/ 									*pHint->GetAnyEnd(), nNodeIdx );
/*N*/ 	}
/*N*/ 	Insert( pHt, Count() );
/*N*/ }


/*N*/ void SwHistory::Add( const SwFmtColl* pColl, ULONG nNodeIdx, BYTE nWhichNd )
/*N*/ {
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 	SwHstryHint * pHt = new SwChgFmtColl( pColl, nNodeIdx, nWhichNd );
/*N*/ 	Insert( pHt, Count() );
/*N*/ }


/*N*/ void SwHistory::Add( const SwFmt* pFmt, ULONG nNodeIdx, BYTE nWhichNd )
/*N*/ {
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 	SwHstryHint * pHt;
/*N*/ 	const USHORT nWh = pFmt->Which();
/*N*/ 	if( RES_FLYFRMFMT == nWh || RES_DRAWFRMFMT == nWh )
/*N*/ 	{
/*N*/ 		pHt = new SwHstryTxtFlyCnt( (SwFlyFrmFmt*)pFmt );
/*N*/ 		Insert( pHt, Count() );
/*N*/ 	}
/*N*/ }



// JP 21.03.94: Bookmarks jetzt auch in die History mitaufnehmen
/*N*/ void SwHistory::Add( const SwBookmark& rBkmk, BYTE nTyp )
/*N*/ {
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 	SwHstryHint * pHt = new SwHstryBookmark( rBkmk, nTyp );
/*N*/ 	Insert( pHt, Count() );
/*N*/ }


/*N*/ void SwHistory::Add( const SwFrmFmt& rFmt )
/*N*/ {
/*N*/ 	SwHstryHint * pHt = new SwHstryChgFlyAnchor( rFmt );
/*N*/ 	Insert( pHt, Count() );
/*N*/ }

/*N*/ void SwHistory::Add( const SwFlyFrmFmt& rFmt, USHORT& rSetPos )
/*N*/ {
/*N*/ 	ASSERT( !nEndDiff, "nach REDO wurde die History noch nicht geloescht" );
/*N*/ 	SwHstryHint * pHt;
/*N*/ 	const USHORT nWh = rFmt.Which();
/*N*/ 	if( RES_FLYFRMFMT == nWh || RES_DRAWFRMFMT == nWh )
/*N*/ 	{
/*N*/ 		pHt = new SwHstryTxtFlyCnt( (SwFlyFrmFmt*)&rFmt );
/*N*/ 		Insert( pHt, Count() );
/*N*/ 
/*N*/ 		const SwFmtChain* pChainItem;
/*N*/ 		if( SFX_ITEM_SET == rFmt.GetItemState( RES_CHAIN, FALSE,
/*N*/ 			(const SfxPoolItem**)&pChainItem ))
/*N*/ 		{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	if( pChainItem->GetNext() || pChainItem->GetPrev() )
/*N*/ 		}
/*N*/ 	}
/*N*/ }

/*N*/ void SwHistory::Add( const SwTxtFtn& rFtn )
/*N*/ {
/*N*/ 	SwHstryHint *pHt = new SwSetFtnHint( rFtn );
/*N*/ 	Insert( pHt, Count() );
/*N*/ }


/*************************************************************************
|*
|*    BOOL SwHistory::Rollback()
|*
|*    Beschreibung		Dokument 1.0
|*    Ersterstellung    JP 18.02.91
|*    Letzte Aenderung  JP 18.02.91
|*
*************************************************************************/







/*N*/ void SwHistory::Delete( USHORT nStart )
/*N*/ {
/*N*/ 	for( USHORT n = Count(); n > nStart; )
/*N*/ 		SwpHstry::DeleteAndDestroy( --n, 1 );
/*N*/ 	nEndDiff = 0;
/*N*/ }




/*N*/ void SwHistory::CopyAttr( const SwpHints* pHts, ULONG nNodeIdx,
/*N*/ 						  xub_StrLen nStt, xub_StrLen nEnd, BOOL bFields )
/*N*/ {
/*N*/ 	if( !pHts  )
/*N*/ 		return;
/*N*/ 
/*N*/ 	// kopiere alle Attribute aus dem TextNode die
/*N*/ 	// im Bereich von Position nStt bis Len liegen
/*N*/ 	const SwTxtAttr* pHt;
/*N*/ 	xub_StrLen nAttrStt;
/*N*/ 	const xub_StrLen * pEndIdx;
/*N*/ 	for( USHORT n = 0; n < pHts->Count(); n++ )
/*N*/ 	{
/*N*/ 		// BP: nAttrStt muss auch bei !pEndIdx gesetzt werden
/*N*/ 		nAttrStt = *(pHt = (*pHts)[n])->GetStart();
/*N*/ // JP: ???? wieso nAttrStt >= nEnd
/*N*/ //		if( 0 != ( pEndIdx = pHt->GetEnd() ) && nAttrStt >= nEnd )
/*N*/ 		if( 0 != ( pEndIdx = pHt->GetEnd() ) && nAttrStt > nEnd )
/*N*/ 			break;
/*N*/ 
/*N*/ 		// Flys und Ftn nie kopieren !!
/*N*/ 		BOOL bNextAttr = FALSE;
/*N*/ 		switch( pHt->Which() )
/*N*/ 		{
/*N*/ 		case RES_TXTATR_FIELD:
/*N*/ 		case RES_TXTATR_HARDBLANK:
/*N*/ 			// keine Felder, .. kopieren ??
/*N*/ 			if( !bFields )
/*N*/ 				bNextAttr = TRUE;
/*N*/ 			break;
/*N*/ 		case RES_TXTATR_FLYCNT:
/*N*/ 		case RES_TXTATR_FTN:
/*N*/ 			bNextAttr = TRUE;
/*N*/ 			break;
/*N*/ 		}
/*N*/ 
/*N*/ 		if( bNextAttr )
/*N*/ 		   continue;
/*N*/ 
/*N*/ 		// alle Attribute, die irgendwie in diesem Bereich liegen speichern
/*N*/ 		if( nStt <= nAttrStt )
/*N*/ 		{
/*N*/ 			if( nEnd > nAttrStt
/*N*/ // JP: ???? wieso nAttrStt >= nEnd
/*N*/ //				|| (nEnd == nAttrStt && (!pEndIdx || nEnd == pEndIdx->GetIndex()))
/*N*/ 			)
/*N*/ 				Add( pHt, nNodeIdx, FALSE );
/*N*/ 		}
/*N*/ 		else if( pEndIdx && nStt < *pEndIdx )
/*N*/ 			Add( pHt, nNodeIdx, FALSE );
/*N*/ 	}
/*N*/ }


/*************************************************************************/

// Klasse zum Registrieren der History am Node, Format, HintsArray, ...

/*N*/ SwRegHistory::SwRegHistory( SwModify* pRegIn, const SwNode& rNd,
/*N*/ 							SwHistory* pHst )
/*N*/ 	: SwClient( pRegIn ), pHstry( pHst ), nNodeIdx( rNd.GetIndex() )
/*N*/ {
/*N*/ 	if( pHst )
/*N*/ 		_MakeSetWhichIds();
/*N*/ }



/*N*/ SwRegHistory::SwRegHistory( const SwNode& rNd, SwHistory* pHst )
/*N*/ 	: SwClient( 0 ), pHstry( pHst ), nNodeIdx( rNd.GetIndex() )
/*N*/ {
/*N*/ 	if( pHstry )
/*N*/ 		_MakeSetWhichIds();
/*N*/ }



/*N*/ void SwRegHistory::Modify( SfxPoolItem* pOld, SfxPoolItem* pNew )
/*N*/ {
/*N*/ 	// Attribut erfragen ?? ( mal sehen )
/*N*/ 	if( pHstry && ( pOld || pNew ) )
/*N*/ 	{
/*N*/ 		if( pNew->Which() < POOLATTR_END )
/*N*/ 			pHstry->Add( pOld, pNew, nNodeIdx );
/*N*/ 		else if( RES_ATTRSET_CHG == pNew->Which() )
/*N*/ 		{
/*N*/ 			SwHstryHint* pNewHstr;
/*N*/ 			const SfxItemSet& rSet = *((SwAttrSetChg*)pOld)->GetChgSet();
/*N*/ 			if( 1 < rSet.Count() )
/*N*/ 				pNewHstr = new SwHstrySetAttrSet( rSet, nNodeIdx,
/*N*/ 													aSetWhichIds );
/*N*/ 			else
/*N*/ 			{
/*N*/ 				const SfxPoolItem* pItem = SfxItemIter( rSet ).FirstItem();
/*N*/ 				if( aSetWhichIds.Seek_Entry( pItem->Which() ) )
/*N*/ 					pNewHstr = new SwSetFmtHint( pItem, nNodeIdx );
/*N*/ 				else
/*N*/ 					pNewHstr = new SwResetFmtHint( pItem, nNodeIdx );
/*N*/ 			}
/*N*/ 			pHstry->Insert( pNewHstr, pHstry->Count() );
/*N*/ 		}
/*N*/ 	}
/*N*/ }



/*N*/ void SwRegHistory::Add( SwTxtAttr* pHt, const BOOL bNew )
/*N*/ {
/*N*/ 	pHstry->Add( pHt, nNodeIdx, bNew );
/*N*/ }


/*N*/ SwRegHistory::SwRegHistory( SwTxtNode* pTxtNode, const SfxItemSet& rSet,
/*N*/ 							xub_StrLen nStart, xub_StrLen nEnd, USHORT nFlags,
/*N*/ 							SwHistory* pHst )
/*N*/ 	: SwClient( pTxtNode ), pHstry( pHst ), nNodeIdx( pTxtNode->GetIndex() )
/*N*/ {
/*N*/ 	if( !rSet.Count() )
/*N*/ 		return;
/*N*/ 
/*N*/ 	register BOOL bInsert;
/*N*/ 
/*N*/ 	if( pTxtNode->GetpSwpHints() && pHst )
/*N*/ 	{
/*N*/ 		pTxtNode->GetpSwpHints()->Register( this );
/*N*/ 		bInsert = pTxtNode->SetAttr( rSet, nStart, nEnd, nFlags );
/*N*/ 		// Achtung: Durch das Einfuegen eines Attributs kann das Array
/*N*/ 		// geloescht werden!!! Wenn das einzufuegende zunaechst ein vorhandenes
/*N*/ 		// loescht, selbst aber nicht eingefuegt werden braucht, weil die
/*N*/ 		// Absatzattribute identisch sind( -> bForgetAttr in SwpHints::Insert )
/*N*/ 		if ( pTxtNode->GetpSwpHints() )
/*N*/ 			pTxtNode->GetpSwpHints()->DeRegister();
/*N*/ 	}
/*N*/ 	else
/*N*/ 		bInsert = pTxtNode->SetAttr( rSet, nStart, nEnd, nFlags );
/*N*/ 
/*N*/ 	if( pHst && bInsert )
/*N*/ 	{
/*N*/ 		SwHstryHint* pNewHstr = new SwHstryResetAttrSet( rSet,
/*N*/ 									pTxtNode->GetIndex(), nStart, nEnd );
 		// !!! ----                     /|\
		// !!! ----                    /|||\
		// der NodeIndex kann verschoben sein !!
/*N*/ 
/*N*/ 		pHst->Insert( pNewHstr, pHst->Count() );
/*N*/ 	}
/*N*/ }


/*N*/ void SwRegHistory::_MakeSetWhichIds()
/*N*/ {
/*N*/ 	if( aSetWhichIds.Count() )
/*?*/ 		aSetWhichIds.Remove( 0, aSetWhichIds.Count() );
/*N*/ 
/*N*/ 	if( GetRegisteredIn() )
/*N*/ 	{
/*N*/ 		const SfxItemSet* pSet = 0;
/*N*/ 		if( GetRegisteredIn()->ISA( SwCntntNode ) )
/*?*/ 			pSet = ((SwCntntNode*)GetRegisteredIn())->GetpSwAttrSet();
/*N*/ 		else if( GetRegisteredIn()->ISA( SwFmt ) )
/*N*/ 			pSet = &((SwFmt*)GetRegisteredIn())->GetAttrSet();
/*N*/ 		if( pSet && pSet->Count() )
/*N*/ 		{
/*N*/ 			SfxItemIter aIter( *pSet );
/*N*/ 			USHORT nW = aIter.FirstItem()->Which();
/*N*/ 			while( TRUE )
/*N*/ 			{
/*N*/ 				aSetWhichIds.Insert( nW );
/*N*/ 				if( aIter.IsAtEnd() )
/*N*/ 					break;
/*N*/ 				nW = aIter.NextItem()->Which();
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ }



}
