/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_undobj1.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:46:13 $
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



#ifndef _FMTANCHR_HXX //autogen
#include <fmtanchr.hxx>
#endif
#ifndef _FRMFMT_HXX //autogen
#include <frmfmt.hxx>
#endif
#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _SWUNDO_HXX
#include <swundo.hxx>			// fuer die UndoIds
#endif
#ifndef _PAM_HXX
#include <pam.hxx>
#endif
// OD 26.06.2003 #108784#
namespace binfilter {

// Inline Methode vom UndoIter

//---------------------------------------------------------------------

/*N*/ SwUndoFlyBase::SwUndoFlyBase( SwFrmFmt* pFormat, USHORT nUndoId )
/*N*/ 	: SwUndo( nUndoId ), pFrmFmt( pFormat )
/*N*/ {
/*N*/ #ifdef COMPACT
/*N*/ 	pFormat->GetDoc()->DelUndoGroups();
/*N*/ #endif
/*N*/ }

/*N*/ SwUndoFlyBase::~SwUndoFlyBase()
/*N*/ {
/*N*/ 	if( bDelFmt )		// loeschen waehrend eines Undo's ??
/*?*/ 		delete pFrmFmt;
/*N*/ }



// ----- Undo-InsertFly ------

/*N*/ SwUndoInsLayFmt::SwUndoInsLayFmt( SwFrmFmt* pFormat )
/*N*/ 	: SwUndoFlyBase( pFormat,
/*N*/ 			RES_DRAWFRMFMT == pFormat->Which() ? UNDO_INSDRAWFMT : UNDO_INSLAYFMT )
/*N*/ {
/*N*/ 	const SwFmtAnchor& rAnchor = pFrmFmt->GetAnchor();
/*N*/ 	nRndId = rAnchor.GetAnchorId();
/*N*/ 	bDelFmt = FALSE;
/*N*/ 	switch( nRndId )
/*N*/ 	{
/*N*/ 	case FLY_PAGE:
/*N*/ 		nNdPgPos = rAnchor.GetPageNum();
/*N*/ 		break;
/*N*/ 	case FLY_AT_CNTNT:
/*N*/ 	case FLY_AT_FLY:
/*N*/ 		nNdPgPos = rAnchor.GetCntntAnchor()->nNode.GetIndex();
/*N*/ 		break;
/*N*/ 	case FLY_IN_CNTNT:
/*N*/ 	case FLY_AUTO_CNTNT:
/*N*/ 		{
/*N*/ 			const SwPosition* pPos = rAnchor.GetCntntAnchor();
/*N*/ 			nCntPos = pPos->nContent.GetIndex();
/*N*/ 			nNdPgPos = pPos->nNode.GetIndex();
/*N*/ 		}
/*N*/ 		break;
/*N*/ 	default:
/*N*/ 		ASSERT( FALSE, "Was denn fuer ein FlyFrame?" );
/*N*/ 	}
/*N*/ }




// ----- Undo-DeleteFly ------

/*N*/ SwUndoDelLayFmt::SwUndoDelLayFmt( SwFrmFmt* pFormat )
/*N*/ 	: SwUndoFlyBase( pFormat, UNDO_DELLAYFMT ), bShowSelFrm( TRUE )
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	SwDoc* pDoc = pFormat->GetDoc();
/*N*/ }




/*  */

/*N*/ SwUndoSetFlyFmt::SwUndoSetFlyFmt( SwFrmFmt& rFlyFmt, SwFrmFmt& rNewFrmFmt )
/*N*/ 	: SwUndo( UNDO_SETFLYFRMFMT ), SwClient( &rFlyFmt ), pFrmFmt( &rFlyFmt ),
/*N*/ 	pNewFmt( &rNewFrmFmt ), pOldFmt( (SwFrmFmt*)rFlyFmt.DerivedFrom() ),
/*N*/ 	pItemSet( new SfxItemSet( *rFlyFmt.GetAttrSet().GetPool(),
/*N*/ 								rFlyFmt.GetAttrSet().GetRanges() )),
/*N*/ 	nOldNode( 0 ), nOldCntnt( 0 ), nOldAnchorTyp( 0 ),
/*N*/ 	nNewNode( 0 ), nNewCntnt( 0 ), nNewAnchorTyp( 0 ), bAnchorChgd( FALSE )
/*N*/ {
/*N*/ }

/*N*/ SwUndoSetFlyFmt::~SwUndoSetFlyFmt()
/*N*/ {
/*N*/ 	delete pItemSet;
/*N*/ }






}
