/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unattr.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:45:23 $
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

#define _SVSTDARR_USHORTS
#define _SVSTDARR_USHORTSSORT

#ifndef _HINTIDS_HXX
#include <hintids.hxx>
#endif

#ifndef _SVX_TSTPITEM_HXX //autogen
#include <bf_svx/tstpitem.hxx>
#endif
#ifndef _SFXITEMITER_HXX //autogen
#include <svtools/itemiter.hxx>
#endif


#ifndef _TXTFTN_HXX //autogen
#include <txtftn.hxx>
#endif
#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _FMTANCHR_HXX //autogen
#include <fmtanchr.hxx>
#endif
#ifndef _FMTFSIZE_HXX //autogen
#include <fmtfsize.hxx>
#endif
#ifndef _FMTCNTNT_HXX //autogen
#include <fmtcntnt.hxx>
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
#ifndef _HINTS_HXX
#include <hints.hxx>
#endif
#ifndef _SWTABLE_HXX
#include <swtable.hxx>
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _ROLBCK_HXX
#include <rolbck.hxx>
#endif
#ifndef _DCONTACT_HXX
#include <dcontact.hxx>
#endif
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
#ifndef _SECTION_HXX
#include <section.hxx>
#endif
namespace binfilter {


// -----------------------------------------------------

/*?*/ _UndoFmtAttr::_UndoFmtAttr( SwFmt& rFmt, BOOL bSvDrwPt )
/*?*/ 	: SwClient( &rFmt ), pUndo( 0 ), bSaveDrawPt( bSvDrwPt )
/*?*/ {
/*?*/ }

/*?*/ void _UndoFmtAttr::Modify( SfxPoolItem* pOld, SfxPoolItem* pNew )
/*?*/ {
/*?*/ 	if( pOld && pNew )
/*?*/ 	{
/*?*/ 		if( POOLATTR_END >= pOld->Which() )
/*?*/ 		{
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 if( pUndo )
/*?*/ 		}
/*?*/ 		else if( RES_ATTRSET_CHG == pOld->Which() )
/*?*/ 		{
/*?*/ 			if( pUndo )
/*?*/ 			{
/*?*/ 				SfxItemIter aIter( *((SwAttrSetChg*)pOld)->GetChgSet() );
/*?*/ 				const SfxPoolItem* pItem = aIter.GetCurItem();
/*?*/ 				while( pItem )
/*?*/ 				{
/*?*/ 				DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	pUndo->PutAttr( *pItem );
/*?*/ 				}
/*?*/ 			}
/*?*/ 			else
/*?*/ 				pUndo = new SwUndoFmtAttr( *((SwAttrSetChg*)pOld)->GetChgSet(),
/*?*/ 										*(SwFmt*)pRegisteredIn, bSaveDrawPt );
/*?*/ 		}
/*?*/ 		else
/*?*/ 			SwClient::Modify( pOld, pNew );
/*?*/ 	}
/*?*/ 	else
/*?*/ 		SwClient::Modify( pOld, pNew );
/*?*/ }

/*N*/ SwUndoFmtAttr::SwUndoFmtAttr( const SfxItemSet& rSet, SwFmt& rChgFmt,
/*N*/ 								BOOL bSvDrwPt )
/*N*/ 	: SwUndo( UNDO_INSFMTATTR ), pFmt( &rChgFmt ),
/*N*/ 	nFmtWhich( rChgFmt.Which() ), nNode( 0 ), bSaveDrawPt( bSvDrwPt )
/*N*/ {
/*N*/ 	pOldSet = new SfxItemSet( rSet );
/*N*/ 	Init();
/*N*/ }

/*N*/ void SwUndoFmtAttr::Init()
/*N*/ {
/*N*/ 	// Ankerwechsel gesondert behandeln
/*N*/ 	if( SFX_ITEM_SET == pOldSet->GetItemState( RES_ANCHOR, FALSE ))
/*N*/ 		SaveFlyAnchor( bSaveDrawPt );
/*N*/ 	else if( RES_FRMFMT == nFmtWhich )
/*N*/ 	{
/*?*/ 		SwDoc* pDoc = pFmt->GetDoc();
/*?*/ 		if( USHRT_MAX !=
/*?*/ 			pDoc->GetTblFrmFmts()->GetPos( (const SwFrmFmtPtr)pFmt ))
/*?*/ 		{
/*?*/ 			// TabellenFormat -> Tabellen Index Position merken, TabellenFormate
/*?*/ 			// 						sind fluechtig!
/*?*/ 			SwClient* pTbl = SwClientIter( *pFmt ).First( TYPE( SwTable ));
/*?*/ 			if( pTbl )
/*?*/ 				nNode = ((SwTable*)pTbl)->GetTabSortBoxes()[ 0 ]->
/*?*/ 							GetSttNd()->FindTableNode()->GetIndex();
/*?*/ 		}
/*?*/ 		else if( USHRT_MAX !=
/*?*/ 			pDoc->GetSections().GetPos( (const SwSectionFmtPtr)pFmt ))
/*?*/ 			nNode = pFmt->GetCntnt().GetCntntIdx()->GetIndex();
/*N*/ 	}
/*N*/ }

/*N*/ SwUndoFmtAttr::~SwUndoFmtAttr()
/*N*/ {
/*N*/ 	delete pOldSet;
/*N*/ }



// prueft, ob es noch im Doc ist!




/*N*/ void SwUndoFmtAttr::SaveFlyAnchor( BOOL bSvDrwPt )
/*N*/ {
/*N*/ 	// das Format ist gueltig, sonst wuerde man gar bis hier kommen
/*N*/ 	if( bSvDrwPt )
/*N*/ 	{
/*N*/ 		if( RES_DRAWFRMFMT == pFmt->Which() )
/*N*/ 		{
/*?*/ 			Point aPt( ((SwFrmFmt*)pFmt)->FindSdrObject()->GetRelativePos() );
/*?*/ 			// den alten Wert als zwischenspeichern. Attribut dafuer benutzen,
/*?*/ 			// dadurch bleibt der SwUndoFmtAttr klein.
/*?*/ 			pOldSet->Put( SwFmtFrmSize( ATT_VAR_SIZE, aPt.X(), aPt.Y() ) );
/*N*/ 		}
/*		else
		{
			pOldSet->Put( pFmt->GetVertOrient() );
			pOldSet->Put( pFmt->GetHoriOrient() );
		}
*/	}

/*N*/ 	const SwFmtAnchor& rAnchor = (SwFmtAnchor&)pOldSet->Get(
/*N*/ 												RES_ANCHOR, FALSE );
/*N*/ 	if( !rAnchor.GetCntntAnchor() )
/*N*/ 		return;
/*N*/ 
/*N*/ 	xub_StrLen nCntnt = 0;
/*N*/ 	switch( rAnchor.GetAnchorId() )
/*N*/ 	{
/*N*/ 	case FLY_IN_CNTNT:
/*N*/ 	case FLY_AUTO_CNTNT:
/*N*/ 		nCntnt = rAnchor.GetCntntAnchor()->nContent.GetIndex();
/*N*/ 	case FLY_AT_CNTNT:
/*N*/ 	case FLY_AT_FLY:
/*N*/ 		nNode = rAnchor.GetCntntAnchor()->nNode.GetIndex();
/*N*/ 		break;
/*N*/ 	default:
/*N*/ 		return;
/*N*/ 	}
/*N*/ 
/*N*/ 	SwFmtAnchor aAnchor( rAnchor.GetAnchorId(), nCntnt );
/*N*/ 	pOldSet->Put( aAnchor );
/*N*/ }


/*  */









// -----------------------------------------------------



/*N*/ SwUndoAttr::SwUndoAttr( const SwPaM& rRange, const SfxPoolItem& rAttr,
/*N*/ 						USHORT nFlags )
/*N*/ 	: SwUndo( UNDO_INSATTR ), SwUndRng( rRange ),
/*N*/ 	aSet( rRange.GetDoc()->GetAttrPool(), rAttr.Which(), rAttr.Which() ),
/*N*/ 	nInsFlags( nFlags ), pHistory( new SwHistory ),
/*N*/ 	pRedlData( 0 ), pRedlSaveData( 0 ),
/*N*/ 	nNdIdx( ULONG_MAX )
/*N*/ {
/*N*/ 	aSet.Put( rAttr );
/*N*/ #ifdef COMPACT
/*N*/ 	(SwDoc*)rRange.GetDoc()->DelUndoGroups();
/*N*/ #endif
/*N*/ }

/*N*/ SwUndoAttr::SwUndoAttr( const SwPaM& rRange, const SfxItemSet& rSet,
/*N*/ 						USHORT nFlags )
/*N*/ 	: SwUndo( UNDO_INSATTR ), SwUndRng( rRange ), aSet( rSet ),
/*N*/ 	nInsFlags( nFlags ), pHistory( new SwHistory ),
/*N*/ 	pRedlData( 0 ), pRedlSaveData( 0 ),
/*N*/ 	nNdIdx( ULONG_MAX )
/*N*/ {
/*N*/ #ifdef COMPACT
/*?*/ 	(SwDoc*)rRange.GetDoc()->DelUndoGroups();
/*N*/ #endif
/*N*/ }

/*N*/ SwUndoAttr::~SwUndoAttr()
/*N*/ {
/*N*/ 	delete pHistory;
/*N*/ 	delete pRedlSaveData;
/*N*/ }








/*  */

/*N*/ SwUndoDefaultAttr::SwUndoDefaultAttr( const SfxItemSet& rSet )
/*N*/ 	: SwUndo( UNDO_SETDEFTATTR ), pOldSet( 0 ), pTabStop( 0 )
/*N*/ {
/*N*/ 	const SfxPoolItem* pItem;
/*N*/ 	if( rSet.GetItemState( RES_PARATR_TABSTOP, FALSE, &pItem ) )
/*N*/ 	{
/*N*/ 		pTabStop = (SvxTabStopItem*)pItem->Clone();		// gesondert merken, aendert sich !!!
/*N*/ 		if( 1 != rSet.Count() )			// gibts noch mehr Attribute ?
/*?*/ 			pOldSet = new SfxItemSet( rSet );
/*N*/ 	}
/*N*/ 	else
/*N*/ 		pOldSet = new SfxItemSet( rSet );
/*N*/ }

/*N*/ SwUndoDefaultAttr::~SwUndoDefaultAttr()
/*N*/ {
/*N*/ 	if( pOldSet )
/*N*/ 		delete pOldSet;
/*N*/ 	if( pTabStop )
/*N*/ 		delete pTabStop;
/*N*/ }



/*  */






/*  */

/*N*/ SwUndoChgFtn::SwUndoChgFtn( const SwPaM& rRange, const String& rTxt,
/*N*/ 							USHORT nNum, BOOL bIsEndNote )
/*N*/ 	: SwUndo( UNDO_CHGFTN ), SwUndRng( rRange ),
/*N*/ 	sTxt( rTxt ), nNo( nNum ), bEndNote( bIsEndNote ),
/*N*/ 	pHistory( new SwHistory() )
/*N*/ {
/*N*/ }

/*N*/ SwUndoChgFtn::~SwUndoChgFtn()
/*N*/ {
/*N*/ 	delete pHistory;
/*N*/ }





/*  */






/*  */





/*  */








}
