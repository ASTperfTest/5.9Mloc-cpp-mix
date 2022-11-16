/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unnum.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:47:06 $
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
#ifndef _SWUNDO_HXX
#include <swundo.hxx>			// fuer die UndoIds
#endif
#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif
#ifndef _PAM_HXX
#include <pam.hxx>
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
namespace binfilter {

/*N*/ SwUndoInsNum::SwUndoInsNum( const SwPaM& rPam, const SwNumRule& rRule )
/*N*/ 	: SwUndo( UNDO_INSNUM ), SwUndRng( rPam ),
/*N*/ 	aNumRule( rRule ), pHistory( 0 ), nLRSavePos( 0 ),
/*N*/ 	nSttSet( ULONG_MAX ), pOldNumRule( 0 )
/*N*/ {
/*N*/ }


/*N*/ SwUndoInsNum::~SwUndoInsNum()
/*N*/ {
/*N*/ 	delete pHistory;
/*N*/ 	delete pOldNumRule;
/*N*/ }

/*  */

/*N*/ SwUndoNumUpDown::SwUndoNumUpDown( const SwPaM& rPam, short nOff )
/*N*/ 	: SwUndo( UNDO_NUMUPDOWN ), SwUndRng( rPam ), nOffset( nOff )
/*N*/ {
/*N*/ 	// nOffset: Down 	=>  1
/*N*/ 	//			Up 		=> -1
/*N*/ }

/*N*/ SwUndoNumRuleStart::SwUndoNumRuleStart( const SwPosition& rPos, USHORT nStt )
/*N*/ 	: SwUndo( UNDO_SETNUMRULESTART ),
/*N*/ 	nIdx( rPos.nNode.GetIndex() ), bSetSttValue( TRUE ),
/*N*/ 	nNewStt( nStt ), nOldStt( USHRT_MAX )
/*N*/ {
/*N*/ 	SwTxtNode* pTxtNd = rPos.nNode.GetNode().GetTxtNode();
/*N*/ 	if( pTxtNd && pTxtNd->GetNum() )
/*N*/ 		nOldStt = pTxtNd->GetNum()->GetSetValue();
/*N*/ }








}
