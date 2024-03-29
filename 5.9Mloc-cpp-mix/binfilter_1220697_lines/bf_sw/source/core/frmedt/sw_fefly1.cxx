/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_fefly1.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:15:15 $
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


#ifndef _FMTANCHR_HXX //autogen
#include <fmtanchr.hxx>
#endif
#ifndef _FMTCNTNT_HXX //autogen
#include <fmtcntnt.hxx>
#endif

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _FMTFSIZE_HXX //autogen
#include <fmtfsize.hxx>
#endif
#ifndef _FESH_HXX
#include <fesh.hxx>
#endif
#ifndef _ROOTFRM_HXX
#include <rootfrm.hxx>
#endif
#ifndef _PAGEFRM_HXX
#include <pagefrm.hxx>
#endif
#ifndef _TXTFRM_HXX
#include <txtfrm.hxx>
#endif
#ifndef _VIEWIMP_HXX
#include <viewimp.hxx>
#endif
#ifndef _VISCRS_HXX
#include <viscrs.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _DVIEW_HXX
#include <dview.hxx>
#endif
#ifndef _DFLYOBJ_HXX
#include <dflyobj.hxx>
#endif
#ifndef _FRMFMT_HXX
#include <frmfmt.hxx>
#endif
#ifndef _NDTXT_HXX
#include <ndtxt.hxx>
#endif
#ifndef _NDGRF_HXX
#include <ndgrf.hxx>
#endif
#ifndef _FLYFRMS_HXX
#include <flyfrms.hxx>
#endif
namespace binfilter {

using namespace ::rtl;
using namespace ::com::sun::star;

//Zum anmelden von Flys in Flys in ...
//definiert in layout/frmtool.cxx

/***********************************************************************
#*	Class	   	:  SwDoc
#*	Methode	   	:  UseSpzLayoutFmt
#*	Beschreibung:  Anhand des Request werden zu dem Format entsprechende
#*		Aenderungen an den Spezifischen Layouts vorgenommen.
#*	Datum	   	:  MA 23. Sep. 92
#*	Update	   	:  JP 09.03.98
#***********************************************************************/


/*N*/ BOOL lcl_FindAnchorPos( SwDoc& rDoc, const Point& rPt, const SwFrm& rFrm,
/*N*/ 						SfxItemSet& rSet )
/*N*/ {
/*N*/ 	BOOL bRet = TRUE;
/*N*/ 	SwFmtAnchor aNewAnch( (SwFmtAnchor&)rSet.Get( RES_ANCHOR ) );
/*N*/ 	RndStdIds nNew = aNewAnch.GetAnchorId();
/*N*/ 	const SwFrm *pNewAnch;
/*N*/ 
/*N*/ 	//Neuen Anker ermitteln
/*N*/ 	Point aTmpPnt( rPt );
/*N*/ 	switch( nNew )
/*N*/ 	{
/*N*/ 	case FLY_IN_CNTNT:	// sollte der nicht auch mit hinein?
/*N*/ 	case FLY_AT_CNTNT:
/*N*/ 	case FLY_AUTO_CNTNT: // LAYER_IMPL
/*N*/ 		{
/*N*/ 			//Ausgehend von der linken oberen Ecke des Fly den
/*N*/ 			//dichtesten CntntFrm suchen.
/*N*/ 			const SwFrm* pFrm = rFrm.IsFlyFrm() ? ((SwFlyFrm&)rFrm).GetAnchor()
/*N*/ 												: &rFrm;
/*N*/ 			pNewAnch = ::binfilter::FindAnchor( pFrm, aTmpPnt );
/*N*/ 			if( pNewAnch->IsProtected() )
/*N*/ 			{
/*N*/ 				bRet = FALSE;
/*N*/ 				break;
/*N*/ 			}
/*N*/ 
/*N*/ 			SwPosition aPos( *((SwCntntFrm*)pNewAnch)->GetNode() );
/*N*/ 			if( FLY_AUTO_CNTNT == nNew || FLY_IN_CNTNT == nNew )
/*N*/ 			{
/*N*/ 				// es muss ein TextNode gefunden werden, denn nur in diesen
/*N*/ 				// ist ein Inhaltsgebundene Frames zu verankern
/*N*/ 				SwCrsrMoveState aState( MV_SETONLYTEXT );
/*N*/ 				aTmpPnt.X() -= 1;					//nicht im Fly landen!!
/*N*/ 				if( !pNewAnch->GetCrsrOfst( &aPos, aTmpPnt, &aState ) )
/*N*/ 				{
/*N*/ 					SwCntntNode* pCNd = ((SwCntntFrm*)pNewAnch)->GetNode();
/*N*/ 					if( pNewAnch->Frm().Bottom() < aTmpPnt.Y() )
/*N*/ 						pCNd->MakeStartIndex( &aPos.nContent );
/*N*/ 					else
/*N*/ 						pCNd->MakeEndIndex( &aPos.nContent );
/*N*/ 				}
/*N*/ 			}
/*N*/ 			aNewAnch.SetAnchor( &aPos );
/*N*/ 		}
/*N*/ 		break;
/*N*/ 
/*N*/ 	case FLY_AT_FLY: // LAYER_IMPL
/*N*/ 		{
/*N*/ 			//Ausgehend von der linken oberen Ecke des Fly den
/*N*/ 			//dichtesten SwFlyFrm suchen.
/*N*/ 			SwCrsrMoveState aState( MV_SETONLYTEXT );
/*N*/ 			SwPosition aPos( rDoc.GetNodes() );
/*N*/ 			aTmpPnt.X() -= 1;					//nicht im Fly landen!!
/*N*/ 			rDoc.GetRootFrm()->GetCrsrOfst( &aPos, aTmpPnt, &aState );
/*N*/ 			pNewAnch = ::binfilter::FindAnchor(
/*N*/ 				aPos.nNode.GetNode().GetCntntNode()->GetFrm( 0, 0, sal_False ),
/*N*/ 				aTmpPnt )->FindFlyFrm();
/*N*/ 
/*N*/ 			if( pNewAnch && &rFrm != pNewAnch && !pNewAnch->IsProtected() )
/*N*/ 			{
/*N*/ 				aPos.nNode = *((SwFlyFrm*)pNewAnch)->GetFmt()->GetCntnt().
/*N*/ 								GetCntntIdx();
/*N*/ 				aNewAnch.SetAnchor( &aPos );
/*N*/ 				break;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
/*N*/ 		aNewAnch.SetType( nNew = FLY_PAGE );
/*N*/ 		// no break
/*N*/ 
/*N*/ 	case FLY_PAGE:
/*N*/ 		pNewAnch = rFrm.FindPageFrm();
/*N*/ 		aNewAnch.SetPageNum( pNewAnch->GetPhyPageNum() );
/*N*/ 		break;
/*N*/ 
/*N*/ 	default:
/*N*/ 		ASSERT( !&rDoc, "Falsche ID fuer neuen Anker." );
/*N*/ 	}
/*N*/ 
/*N*/ 	rSet.Put( aNewAnch );
/*N*/ 	return bRet;
/*N*/ }

//
//! also used in unoframe.cxx
//
/*N*/ sal_Bool lcl_ChkAndSetNewAnchor( const SwFlyFrm& rFly, SfxItemSet& rSet )
/*N*/ {
/*N*/ 	const SwFrmFmt& rFmt = *rFly.GetFmt();
/*N*/ 	const SwFmtAnchor &rOldAnch = rFmt.GetAnchor();
/*N*/ 	const RndStdIds nOld = rOldAnch.GetAnchorId();
/*N*/ 
/*N*/ 	RndStdIds nNew = ((SwFmtAnchor&)rSet.Get( RES_ANCHOR )).GetAnchorId();
/*N*/ 
/*N*/ 	if( nOld == nNew )
/*N*/ 		return sal_False;
/*N*/ 
/*N*/ 	SwDoc* pDoc = (SwDoc*)rFmt.GetDoc();
/*N*/ 
/*N*/ #ifndef PRODUCT
/*N*/ 	ASSERT( !(nNew == FLY_PAGE &&
/*N*/ 		(FLY_AT_CNTNT==nOld || FLY_AUTO_CNTNT==nOld || FLY_IN_CNTNT==nOld ) &&
/*N*/ 		pDoc->IsInHeaderFooter( rOldAnch.GetCntntAnchor()->nNode )),
/*N*/ 			"Unerlaubter Ankerwechsel in Head/Foot." );
/*N*/ #endif
/*N*/ 
/*N*/ 	return ::binfilter::lcl_FindAnchorPos( *pDoc, rFly.Frm().Pos(), rFly, rSet );
/*N*/ }


/*************************************************************************
|*
|*	SwFEShell::FindFlyFrm()
|*
|* 	Beschreibung		Liefert den Fly wenn einer Selektiert ist.
|*	Ersterstellung		MA 03. Nov. 92
|*	Letzte Aenderung	MA 05. Mar. 96
|*
*************************************************************************/

/*N*/ SwFlyFrm *SwFEShell::FindFlyFrm() const
/*N*/ {
/*N*/ 	if ( Imp()->HasDrawView() )
/*N*/ 	{
/*N*/ 		// Ein Fly ist genau dann erreichbar, wenn er selektiert ist.
/*N*/ 		const SdrMarkList &rMrkList = Imp()->GetDrawView()->GetMarkList();
/*N*/ 		if( rMrkList.GetMarkCount() != 1 )
/*N*/ 			return 0;
/*N*/ 
/*?*/ 		SdrObject *pO = rMrkList.GetMark( 0 )->GetObj();
/*?*/ 		return pO->IsWriterFlyFrame() ? ((SwVirtFlyDrawObj*)pO)->GetFlyFrm() : 0;
/*N*/ 	}
/*N*/ 	return 0;
/*N*/ }

/*************************************************************************
|*
|*	SwFEShell::IsFlyInFly()
|*
|* 	Beschreibung		Liefert sal_True, wenn der aktuelle Fly an einem anderen
|*						verankert werden koennte (also innerhalb ist)
|*	Ersterstellung		AMA 11. Sep. 97
|*	Letzte Aenderung	AMA 14. Jan. 98
|*
*************************************************************************/


/*************************************************************************
|*
|*	SwFEShell::SetFlyPos
|*
|*	Ersterstellung		MA 14. Jan. 93
|*	Letzte Aenderung	MA 14. Feb. 95
|*
*************************************************************************/


/*************************************************************************
|*
|*	SwFEShell::FindAnchorPos
|*
|*	Ersterstellung		AMA 24. Sep. 97
|*	Letzte Aenderung	AMA 24. Sep. 97
|*
*************************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  NewFlyFrm
#*	Beschreibung:
#*	Datum	   	:  MA 03. Nov. 92
#*	Update	   	:  JP 11. Aug. 93
#***********************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  Insert
#*	Datum	   	:  ??
#*	Update	   	:  MA 12. Sep. 94
#***********************************************************************/





/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  GetPageObjs
#*	Datum	   	:  ??
#*	Update	   	:  MA 11. Jan. 95
#***********************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  SetPageFlysNewPage
#*	Datum	   	:  ??
#*	Update	   	:  MA 14. Feb. 95
#***********************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  GetFlyFrmAttr
#*	Beschreibung:  Alle Attribute in dem 'Koerbchen' werden mit den
#*				   Attributen des aktuellen FlyFrms gefuellt.
#*				   Sind Attribute nicht zu fuellen weil fehl am Platz oder
#* 				   uneindeutig (Mehrfachtselektionen) so werden sie entfernt.
#*	Datum	   	:  MA 03. Nov. 92
#*	Update	   	:  MA 03. Feb. 94
#***********************************************************************/

/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  SetFlyFrmAttr
#*	Beschreibung:  Die Attribute des aktuellen Flys aendern sich.
#*	Datum	   	:  MA 03. Nov. 92
#*	Update	   	:  MA 01. Aug. 95
#***********************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  ResetFlyFrmAttr
#*	Beschreibung:  Das gewuenschte Attribut oder die im Set befindlichen
#*					werden zurueckgesetzt.
#*	Datum	   	:  MA 14. Mar. 97
#*	Update	   	:  MA 14. Mar. 97
#***********************************************************************/


/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  GetCurFrmFmt
#*	Beschreibung:  liefert wenn Rahmen, dann Rahmenvorlage, sonst 0
#*	Datum	   	:  ST 04. Jun. 93
#*	Update	   	:
#***********************************************************************/

/*N*/ SwFrmFmt* SwFEShell::GetCurFrmFmt() const
/*N*/ {
/*N*/ 	SwFrmFmt* pRet = 0;
/*N*/ 	SwLayoutFrm *pFly = FindFlyFrm();
/*N*/ 	if( pFly && ( pRet = (SwFrmFmt*)pFly->GetFmt()->DerivedFrom() ) ==
/*N*/ 											GetDoc()->GetDfltFrmFmt() )
/*?*/ 		pRet = 0;
/*N*/ 	return pRet;
/*N*/ }

/***********************************************************************
#*	Class	   	:  SwFEShell
#*	Methode	   	:  RequestObjectResize()
#*	Datum	   	:  MA 10. Feb. 95
#*	Update	   	:  MA 13. Jul. 95
#***********************************************************************/

/*N*/ void SwFEShell::RequestObjectResize( const SwRect &rRect, SvEmbeddedObject *pIPObj )
/*N*/ {
/*N*/ 	SwFlyFrm *pFly = FindFlyFrm( pIPObj );
/*N*/ 	if ( !pFly )
/*?*/ 		return;
/*N*/ 
/*N*/ 	StartAllAction();
/*N*/ 
/*N*/ 	//MA wir lassen den Fly nicht Clippen, damit die Ole-Server mit
/*N*/ 	//beliebigen Wuenschen kommen koennen. Die Formatierung uebernimmt das
/*N*/ 	//Clippen. Die richtige Darstellung wird per Scalierung erledigt.
/*N*/ 	//Die Scalierung wird von SwNoTxtFrm::Format durch einen Aufruf von
/*N*/ 	//SwWrtShell::CalcAndSetScale() erledigt.
/*N*/ 
/*N*/ 	if ( rRect.SSize() != pFly->Prt().SSize() )
/*N*/ 	{
/*N*/ 	 	Size aSz( rRect.SSize() );
/*N*/ 
/*N*/ 		//JP 28.02.2001: Task 74707 - ask for fly in fly with automatic size
/*N*/ 		//
/*N*/ 		const SwFrm* pAnchor;
/*N*/ 		const SwTxtNode* pTNd;
/*N*/ 		const SwpHints* pHts;
/*N*/ 		const SwFmtFrmSize& rFrmSz = pFly->GetFmt()->GetFrmSize();
/*N*/ 		if( bCheckForOLEInCaption &&
/*N*/ 			0 != rFrmSz.GetWidthPercent() &&
/*N*/ 			0 != (pAnchor = pFly->GetAnchor()) &&
/*N*/ 			pAnchor->IsTxtFrm() &&
/*N*/ 			!pAnchor->GetNext() && !pAnchor->GetPrev() &&
/*N*/ 			pAnchor->GetUpper()->IsFlyFrm() &&
/*?*/ 			0 != ( pTNd = ((SwTxtFrm*)pAnchor)->GetNode()->GetTxtNode()) &&
/*N*/ 			0 != ( pHts = pTNd->GetpSwpHints() ))
/*N*/ 		{
/*?*/ 			// search for a sequence field:
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 const SfxPoolItem* pItem;
/*N*/ 		}
/*N*/ 
/*N*/ 		// set the new Size at the fly themself
/*N*/ 		if ( pFly->Prt().Height() > 0 && pFly->Prt().Width() > 0 )
/*N*/ 		{
/*N*/ 		aSz.Width() += pFly->Frm().Width() - pFly->Prt().Width();
/*N*/ 		aSz.Height()+= pFly->Frm().Height()- pFly->Prt().Height();
/*N*/		}
/*N*/ 		pFly->ChgSize( aSz );
/*N*/		
/*N*/ 		//Wenn sich das Objekt aendert ist die Kontur hoechstwahrscheinlich daneben.
/*N*/ 		ASSERT( pFly->Lower()->IsNoTxtFrm(), "Request ohne NoTxt" );
/*N*/ 		SwNoTxtNode *pNd = ((SwCntntFrm*)pFly->Lower())->GetNode()->GetNoTxtNode();
/*N*/ 		ASSERT( pNd, "Request ohne Node" );
/*N*/ 		pNd->SetContour( 0 );
/*N*/ 		ClrContourCache();
/*N*/ 	}
/*N*/ 
/*N*/ 	//Wenn nur die Size angepasst werden soll, so wird eine Pos mit
/*N*/ 	//ausgezeichneten Werten transportiert.
/*N*/ 	Point aPt( pFly->Prt().Pos() );
/*N*/ 	aPt += pFly->Frm().Pos();
/*N*/ 	if ( rRect.Top() != LONG_MIN && rRect.Pos() != aPt )
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 aPt = rRect.Pos();
/*N*/ 	}
/*N*/ 	EndAllAction();
/*N*/ }

/*M*/ static USHORT SwFmtGetPageNum(const SwFlyFrmFmt * pFmt)
/*M*/ {DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 
/*M*/ }
} //namespace binfilter
#include <fmtcnct.hxx>
#if 0
#include <algorithm>
#include <iostream>
#include <iterator>


/*M*/ static ::std::ostream & operator << (::std::ostream & aStream,
/*M*/                                      const String & aString)
/*M*/ {
/*M*/     ByteString aByteString(aString, RTL_TEXTENCODING_ASCII_US);
/*M*/     aStream << aByteString.GetBuffer();
/*M*/ 
/*M*/     return aStream;
/*M*/ }

/*M*/ void lcl_PrintFrameChainPrev(const SwFrmFmt * pFmt)
/*M*/ {
/*M*/     if (pFmt != NULL)
/*M*/     {
/*M*/         lcl_PrintFrameChainPrev(pFmt->GetChain().GetPrev());
/*M*/ 
/*M*/         ::std::clog << pFmt->GetName() << "->";
/*M*/     }
/*M*/ }

/*M*/ void lcl_PrintFrameChainNext(const SwFrmFmt * pFmt)
/*M*/ {
/*M*/     if (pFmt != NULL)
/*M*/     {
/*M*/         ::std::clog << "->" << pFmt->GetName();
/*M*/ 
/*M*/         lcl_PrintFrameChainPrev(pFmt->GetChain().GetNext());
/*M*/     }
/*M*/ }

/*M*/ void lcl_PrintFrameChain(const SwFrmFmt & rFmt)
/*M*/ {
/*M*/     lcl_PrintFrameChainPrev(rFmt.GetChain().GetPrev());
/*M*/     ::std::clog << "(" <<  rFmt.GetName() << ")";
/*M*/     lcl_PrintFrameChainNext(rFmt.GetChain().GetNext());
/*M*/     ::std::clog << ::std::endl;
/*M*/ }

/*M*/ String lcl_GetChainableString(int nVal)
/*M*/ {
/*M*/     switch(nVal)
/*M*/     {
/*M*/     case SW_CHAIN_OK:
/*M*/         return String::CreateFromAscii("OK");
/*M*/ 
/*M*/     case SW_CHAIN_SOURCE_CHAINED:
/*M*/         return String::CreateFromAscii("source chained");
/*M*/ 
/*M*/     case SW_CHAIN_SELF:
/*M*/         return String::CreateFromAscii("self");
/*M*/ 
/*M*/     case SW_CHAIN_IS_IN_CHAIN:
/*M*/         return String::CreateFromAscii("in chain");
/*M*/ 
/*M*/     case SW_CHAIN_NOT_FOUND:
/*M*/         return String::CreateFromAscii("not found");
/*M*/ 
/*M*/     case SW_CHAIN_NOT_EMPTY:
/*M*/         return String::CreateFromAscii("not empty");
/*M*/ 
/*M*/     case SW_CHAIN_WRONG_AREA:
/*M*/         return String::CreateFromAscii("wrong area");
/*M*/ 
/*M*/     default:
/*M*/         return String::CreateFromAscii("??");
/*M*/ 
/*M*/     }
/*M*/ }
#endif


