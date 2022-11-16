/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unsect.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:47:19 $
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


#ifndef _FMTCNTNT_HXX
#include <fmtcntnt.hxx>
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
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _ROLBCK_HXX
#include <rolbck.hxx>
#endif
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
#ifndef _DOCTXM_HXX
#include <doctxm.hxx>
#endif

/// OD 04.10.2002 #102894#
/// class Calc needed for calculation of the hidden condition of a section.
namespace binfilter {



/*N*/ SfxItemSet* lcl_GetAttrSet( const SwSection& rSect )
/*N*/ {
/*N*/ 	// Attribute des Formate sichern (Spalten, Farbe, ... )
/*N*/ 	// Cntnt- und Protect- Items interessieren nicht (stehen schon in der
/*N*/ 	// Section), muessen also entfernen werden
/*N*/ 	SfxItemSet* pAttr = 0;
/*N*/ 	if( rSect.GetFmt() )
/*N*/ 	{
/*N*/ 		USHORT nCnt = 1;
/*N*/ 		if( rSect.IsProtect() )
/*N*/ 			++nCnt;
/*N*/ 
/*N*/ 		if( nCnt < rSect.GetFmt()->GetAttrSet().Count() )
/*N*/ 		{
/*N*/ 			pAttr = new SfxItemSet( rSect.GetFmt()->GetAttrSet() );
/*N*/ 			pAttr->ClearItem( RES_PROTECT );
/*N*/ 			pAttr->ClearItem( RES_CNTNT );
/*N*/ 			if( !pAttr->Count() )
/*?*/ 				delete pAttr, pAttr = 0;
/*N*/ 		}
/*N*/ 	}
/*N*/ 	return pAttr;
/*N*/ }

/*N*/ SwUndoInsSection::SwUndoInsSection( const SwPaM& rPam, const SwSection& rNew,
/*N*/ 									const SfxItemSet* pSet )
/*N*/ 	: SwUndo( UNDO_INSSECTION ), SwUndRng( rPam ), nSectNodePos( 0 ),
/*N*/ 	pHistory( 0 ), pRedlData( 0 ), pAttr( 0 )
/*N*/ {
/*N*/ 	if( rNew.ISA( SwTOXBaseSection ))
/*N*/ 	{
/*N*/ 		const SwTOXBase& rBase = (SwTOXBaseSection&)rNew;
/*N*/ 		pSection = new SwTOXBaseSection( rBase );
/*N*/ 	}
/*N*/ 	else
/*N*/ 		pSection = new SwSection( rNew.GetType(), rNew.GetName() );
/*N*/ 	*pSection = rNew;
/*N*/ 
/*N*/ 	SwDoc& rDoc = *(SwDoc*)rPam.GetDoc();
/*N*/ 	if( rDoc.IsRedlineOn() )
/*N*/ 	{
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	pRedlData = new SwRedlineData( REDLINE_INSERT,
/*N*/ 	}
/*N*/ 
/*N*/ 	bSplitAtStt = FALSE;
/*N*/ 	bSplitAtEnd = FALSE;
/*N*/ 	bUpdateFtn = FALSE;
/*N*/ 
/*N*/ 	if( pSet && pSet->Count() )
/*N*/ 		pAttr = new SfxItemSet( *pSet );
/*N*/ 
/*N*/ 	if( !rPam.HasMark() )
/*N*/ 	{
/*N*/ 		const SwCntntNode* pCNd = rPam.GetPoint()->nNode.GetNode().GetCntntNode();
/*N*/ 		if( pCNd && pCNd->GetpSwAttrSet() && (
/*N*/ 			!rPam.GetPoint()->nContent.GetIndex() ||
/*N*/ 			rPam.GetPoint()->nContent.GetIndex() == pCNd->Len() ))
/*N*/ 		{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	SfxItemSet aBrkSet( rDoc.GetAttrPool(), aBreakSetRange );
/*N*/ 		}
/*N*/ 	}
/*N*/ }


/*N*/ SwUndoInsSection::~SwUndoInsSection()
/*N*/ {
/*N*/ 	delete pSection;
/*N*/ 	delete pAttr;
/*N*/ 
/*N*/ 	if( pHistory )
/*?*/ 		delete pHistory;
/*N*/ }











/*N*/ void SwUndoInsSection::SaveSplitNode( SwTxtNode* pTxtNd, BOOL bAtStt )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*N*/ }


// -----------------------------










/*N*/ SwUndoChgSection::SwUndoChgSection( const SwSectionFmt& rFmt, BOOL bOnlyAttr )
/*N*/ 	 : SwUndo( UNDO_CHGSECTION ), bOnlyAttrChgd( bOnlyAttr )
/*N*/ {
/*N*/ 	const SwSection& rSect = *rFmt.GetSection();
/*N*/ 	pSection = new SwSection( rSect.GetType(), rSect.GetName() );
/*N*/ 	*pSection = rSect;
/*N*/ 
/*N*/ 	pAttr = ::binfilter::lcl_GetAttrSet( rSect );
/*N*/ 
/*N*/ 	nSttNd = rFmt.GetCntnt().GetCntntIdx()->GetIndex();
/*N*/ }


/*N*/ SwUndoChgSection::~SwUndoChgSection()
/*N*/ {
/*N*/ 	delete pSection;
/*N*/ 	delete pAttr;
/*N*/ }














}
