/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unspnd.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:47:33 $
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
#include "doc.hxx"
#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif
#include "ndtxt.hxx"
#include "swundo.hxx"           // fuer die UndoIds


#include "undobj.hxx"
#include "rolbck.hxx"
#include "redline.hxx"
namespace binfilter {




//------------------------------------------------------------------

// SPLITNODE


/*N*/ SwUndoSplitNode::SwUndoSplitNode( SwDoc* pDoc, const SwPosition& rPos,
/*N*/ 									BOOL bChkTable )
/*N*/ 	: SwUndo( UNDO_SPLITNODE ), nNode( rPos.nNode.GetIndex() ),
/*N*/ 		nCntnt( rPos.nContent.GetIndex() ), pHistory( 0 ),
/*N*/ 		bTblFlag( FALSE ), bChkTblStt( bChkTable ), pRedlData( 0 )
/*N*/ {
/*N*/ 	SwTxtNode* pTxtNd = pDoc->GetNodes()[ rPos.nNode ]->GetTxtNode();
/*N*/ 	ASSERT( pTxtNd, "nur beim TextNode rufen!" );
/*N*/ 	if( pTxtNd->GetpSwpHints() )
/*N*/ 	{
/*?*/ 		pHistory = new SwHistory;
/*?*/ 		pHistory->CopyAttr( pTxtNd->GetpSwpHints(), nNode, 0,
/*?*/ 							pTxtNd->GetTxt().Len(), FALSE );
/*?*/ 		if( !pHistory->Count() )
/*?*/ 			DELETEZ( pHistory );
/*N*/ 	}
/*N*/ 	// Redline beachten
/*N*/ 	if( pDoc->IsRedlineOn() )
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pRedlData = new SwRedlineData( REDLINE_INSERT, pDoc->GetRedlineAuthor() );
/*N*/ 	}
/*N*/ }




/*N*/ SwUndoSplitNode::~SwUndoSplitNode()
/*N*/ {
/*N*/ 	delete pHistory;
/*N*/ }








}
