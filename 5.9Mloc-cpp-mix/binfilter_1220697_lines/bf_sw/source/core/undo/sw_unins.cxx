/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unins.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:46:53 $
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

#ifndef _UNOTOOLS_CHARCLASS_HXX
#include <unotools/charclass.hxx>
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
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
#ifndef _DOCARY_HXX
#include <docary.hxx>
#endif
namespace binfilter {




//------------------------------------------------------------------


// zwei Zugriffs-Funktionen

//------------------------------------------------------------

// INSERT

/*N*/ SwUndoInsert::SwUndoInsert( const SwNodeIndex& rNd, xub_StrLen nCnt,
/*N*/ 							xub_StrLen nL, BOOL bWDelim )
/*N*/ 	: SwUndo(UNDO_INSERT), nNode( rNd.GetIndex() ), nCntnt(nCnt), nLen(nL),
/*N*/ 		bIsWordDelim( bWDelim ), bIsAppend( FALSE ), pPos( 0 ), pTxt( 0 ),
/*N*/ 		pRedlData( 0 )
/*N*/ {
/*N*/ 	// Redline beachten
/*N*/ 	SwDoc& rDoc = *rNd.GetNode().GetDoc();
/*N*/ 	if( rDoc.IsRedlineOn() )
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pRedlData = new SwRedlineData( REDLINE_INSERT,
/*N*/ 	}
/*N*/ }

/*N*/ SwUndoInsert::SwUndoInsert( const SwNodeIndex& rNd )
/*N*/ 	: SwUndo(UNDO_INSERT), nNode( rNd.GetIndex() ), nCntnt(0), nLen(1),
/*N*/ 		bIsWordDelim( FALSE ), bIsAppend( TRUE ), pPos( 0 ), pTxt( 0 ),
/*N*/ 		pRedlData( 0 )
/*N*/ {
/*N*/ 	// Redline beachten
/*N*/ 	SwDoc& rDoc = *rNd.GetNode().GetDoc();
/*N*/ 	if( rDoc.IsRedlineOn() )
/*N*/ 	{
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	pRedlData = new SwRedlineData( REDLINE_INSERT,
/*N*/ 	}
/*N*/ }

// stelle fest, ob das naechste Insert mit dem aktuellen zusammengefasst
// werden kann. Wenn ja, dann aender die Laenge und die InsPos.
// Dann wird von SwDoc::Insert kein neues Object in die Undoliste gestellt.

/*N*/ BOOL SwUndoInsert::CanGrouping( const SwPosition& rInsPos, sal_Unicode cIns )
/*N*/ {
/*N*/ 	BOOL bRet = FALSE;
/*N*/ 	if( !bIsAppend )
/*N*/ 	{
/*N*/ 		++nCntnt;
/*N*/ 		bRet = CanGrouping( rInsPos );
/*N*/ 		--nCntnt;
/*N*/ 		if( bRet )
/*N*/ 			bRet = CanGrouping( cIns );
/*N*/ 	}
/*N*/ 	return bRet;
/*N*/ }


/*N*/ BOOL SwUndoInsert::CanGrouping( sal_Unicode cIns )
/*N*/ {
/*N*/ 	if( !bIsAppend && bIsWordDelim ==
/*N*/ 		!GetAppCharClass().isLetterNumeric( String( cIns )) )
/*N*/ 	{
/*N*/ 		nLen++;
/*N*/ 		nCntnt++;
/*N*/ 		return TRUE;
/*N*/ 	}
/*N*/ 	return FALSE;
/*N*/ }

/*N*/ BOOL SwUndoInsert::CanGrouping( const SwPosition& rPos )
/*N*/ {
/*N*/ 	BOOL bRet = FALSE;
/*N*/ 	if( nNode == rPos.nNode.GetIndex() &&
/*N*/ 		nCntnt == rPos.nContent.GetIndex() )
/*N*/ 	{
/*N*/ 		// Redline beachten
/*N*/ 		SwDoc& rDoc = *rPos.nNode.GetNode().GetDoc();
/*N*/ 		if( ( ~REDLINE_SHOW_MASK & rDoc.GetRedlineMode() ) ==
/*N*/ 			( ~REDLINE_SHOW_MASK & GetRedlineMode() ) )
/*N*/ 		{
/*N*/ 			bRet = TRUE;
/*N*/ 
/*N*/ 			// dann war oder ist noch Redline an:
/*N*/ 			// pruefe, ob an der InsPosition ein anderer Redline
/*N*/ 			// rumsteht. Wenn der gleiche nur einmalig vorhanden ist,
/*N*/ 			// kann zusammen gefasst werden.
/*N*/ 			const SwRedlineTbl& rTbl = rDoc.GetRedlineTbl();
/*N*/ 			if( rTbl.Count() )
/*N*/ 			{
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	SwRedlineData aRData( REDLINE_INSERT, rDoc.GetRedlineAuthor() );
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ 	return bRet;
/*N*/ }

/*N*/ SwUndoInsert::~SwUndoInsert()
/*N*/ {
/*N*/ 	if( pPos )      // loesche noch den Bereich aus dem UndoNodes Array
/*N*/ 	{
/*?*/ 		// Insert speichert den Inhalt in der IconSection
/*?*/ 		SwNodes& rUNds = pPos->nNode.GetNode().GetNodes();
/*?*/ 		if( pPos->nContent.GetIndex() )         // nicht den gesamten Node loeschen
/*?*/ 		{
/*?*/ 			SwTxtNode* pTxtNd = pPos->nNode.GetNode().GetTxtNode();
/*?*/ 			ASSERT( pTxtNd, "kein TextNode, aus dem geloescht werden soll" );
/*?*/ 			pTxtNd->Erase( pPos->nContent );
/*?*/ 			pPos->nNode++;
/*?*/ 		}
/*?*/ 		pPos->nContent.Assign( 0, 0 );
/*?*/ 		rUNds.Delete( pPos->nNode, rUNds.GetEndOfExtras().GetIndex() -
/*?*/ 									pPos->nNode.GetIndex() );
/*?*/ 		delete pPos;
/*N*/ 	}
/*N*/ 	else if( pTxt )		// der eingefuegte Text
/*?*/ 		delete pTxt;
/*N*/ }









/*  */














/*  */













/*  */









}
