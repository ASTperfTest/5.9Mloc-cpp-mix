/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_atrfld.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:41:07 $
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

#include "doc.hxx"			// Update fuer UserFields

#ifndef _FMTFLD_HXX //autogen
#include <fmtfld.hxx>
#endif
#ifndef _TXTFLD_HXX //autogen
#include <txtfld.hxx>
#endif
#include "reffld.hxx"
#include "ddefld.hxx"
#include "usrfld.hxx"
#include "expfld.hxx"
#include "ndtxt.hxx"        // SwTxtNode
#include "hints.hxx"
namespace binfilter {

/*N*/ TYPEINIT2( SwFmtFld, SfxPoolItem, SwClient )

/****************************************************************************
 *
 *  class SwFmtFld
 *
 ****************************************************************************/

	// Konstruktor fuers Default vom Attribut-Pool
/*N*/ SwFmtFld::SwFmtFld()
/*N*/ 	: SfxPoolItem( RES_TXTATR_FIELD ),
/*N*/ 	SwClient( 0 ),
/*N*/ 	pField( 0 ),
/*N*/ 	pTxtAttr( 0 )
/*N*/ {
/*N*/ }

/*N*/ SwFmtFld::SwFmtFld( const SwField &rFld )
/*N*/ 	: SfxPoolItem( RES_TXTATR_FIELD ),
/*N*/ 	SwClient( rFld.GetTyp() ),
/*N*/ 	pTxtAttr( 0 )
/*N*/ {
/*N*/ 	pField = rFld.Copy();
/*N*/ }

/*N*/ SwFmtFld::SwFmtFld( const SwFmtFld& rAttr )
/*N*/ 	: SfxPoolItem( RES_TXTATR_FIELD ),
/*N*/ 	SwClient( rAttr.GetFld()->GetTyp() ),
/*N*/ 	pTxtAttr( 0 )
/*N*/ {
/*N*/ 	pField = rAttr.GetFld()->Copy();
/*N*/ }

/*N*/ SwFmtFld::~SwFmtFld()
/*N*/ {
/*N*/ 	SwFieldType* pType = pField ? pField->GetTyp() : 0;
/*N*/ 
/*N*/ 	if (pType && pType->Which() == RES_DBFLD)
/*N*/ 		pType = 0;	// DB-Feldtypen zerstoeren sich selbst
/*N*/ 
/*N*/ 	delete pField;
/*N*/ 
/*N*/ 	// bei einige FeldTypen muessen wir den FeldTypen noch loeschen
/*N*/ 	if( pType && pType->IsLastDepend() )
/*N*/ 	{
/*N*/ 		BOOL bDel = FALSE;
/*N*/ 		switch( pType->Which() )
/*N*/ 		{
/*N*/ 		case RES_USERFLD:
/*N*/ 			bDel = ((SwUserFieldType*)pType)->IsDeleted();
/*N*/ 			break;
/*N*/ 
/*N*/ 		case RES_SETEXPFLD:
/*N*/ 			bDel = ((SwSetExpFieldType*)pType)->IsDeleted();
/*N*/ 			break;
/*N*/ 
/*N*/ 		case RES_DDEFLD:
/*N*/ 			bDel = ((SwDDEFieldType*)pType)->IsDeleted();
/*N*/ 			break;
/*N*/ 		}
/*N*/ 
/*N*/ 		if( bDel )
/*N*/ 		{
/*N*/ 			// vorm loeschen erstmal austragen
/*?*/ 			pType->Remove( this );
/*?*/ 			delete pType;
/*N*/ 		}
/*N*/ 	}
/*N*/ }

int SwFmtFld::operator==( const SfxPoolItem& rAttr ) const
{
		DBG_BF_ASSERT(0, "STRIP"); return 0; //STRIP001 	ASSERT( SfxPoolItem::operator==( rAttr ), "keine gleichen Attribute" );
}

/*N*/ SfxPoolItem* SwFmtFld::Clone( SfxItemPool* ) const
/*N*/ {
/*N*/ 	return new SwFmtFld( *this );
/*N*/ }

/*N*/ void SwFmtFld::Modify( SfxPoolItem* pOld, SfxPoolItem* pNew )
/*N*/ {
/*N*/ 	if( !pTxtAttr )
/*N*/ 		return;
/*N*/ 
/*N*/ 	SwTxtNode* pTxtNd = (SwTxtNode*)&pTxtAttr->GetTxtNode();
/*N*/ 	ASSERT( pTxtNd, "wo ist denn mein Node?" );
/*N*/ 	if( pNew )
/*N*/ 	{
/*N*/ 		switch( pNew->Which() )
/*N*/ 		{
/*?*/ 		case RES_TXTATR_FLDCHG:
/*?*/ 				// "Farbe hat sich geaendert !"
/*?*/ 				// this, this fuer "nur Painten"
/*?*/ 				pTxtNd->Modify( this, this );
/*?*/ 				return;
/*?*/ 		case RES_REFMARKFLD_UPDATE:
/*?*/ 				// GetReferenz-Felder aktualisieren
/*?*/ 				if( RES_GETREFFLD == GetFld()->GetTyp()->Which() )
/*?*/ 					((SwGetRefField*)GetFld())->UpdateField();
/*?*/ 				break;
/*N*/ 		case RES_DOCPOS_UPDATE:
/*N*/ 				// Je nach DocPos aktualisieren (SwTxtFrm::Modify())
/*N*/ 				pTxtNd->Modify( pNew, this );
/*N*/ 				return;
/*N*/ 
/*N*/ 		case RES_ATTRSET_CHG:
/*N*/ 		case RES_FMT_CHG:
/*?*/ 				pTxtNd->Modify( pOld, pNew );
/*?*/ 				return;
/*N*/ 		}
/*N*/ 	}
/*N*/ 
/*N*/ 	switch (GetFld()->GetTyp()->Which())
/*N*/ 	{
/*?*/ 		case RES_HIDDENPARAFLD:
/*?*/ 			if( !pOld || RES_HIDDENPARA_PRINT != pOld->Which() )
/*?*/ 				break;
/*?*/ 		case RES_DBSETNUMBERFLD:
/*?*/ 		case RES_DBNUMSETFLD:
/*?*/ 		case RES_DBNEXTSETFLD:
/*?*/ 		case RES_DBNAMEFLD:
/*?*/ 			pTxtNd->Modify( 0, pNew);
/*?*/ 			return;
/*N*/ 	}
/*N*/ 
/*N*/ 	if( RES_USERFLD == GetFld()->GetTyp()->Which() )
/*N*/ 	{
/*?*/ 		SwUserFieldType* pType = (SwUserFieldType*)GetFld()->GetTyp();
/*?*/ 		if(!pType->IsValid())
/*?*/ 		{
/*?*/ 			DBG_BF_ASSERT(0, "STRIP"); //STRIP001 SwCalc aCalc( *pTxtNd->GetDoc() );
/*?*/ 		}
/*N*/ 	}
/*N*/ 	pTxtAttr->Expand();
/*N*/ }

/*N*/ BOOL SwFmtFld::GetInfo( SfxPoolItem& rInfo ) const
/*N*/ {
/*N*/ 	const SwTxtNode* pTxtNd;
/*N*/ 	if( RES_AUTOFMT_DOCNODE != rInfo.Which() ||
/*N*/ 		!pTxtAttr || 0 == ( pTxtNd = pTxtAttr->GetpTxtNode() ) ||
/*N*/ 		&pTxtNd->GetNodes() != ((SwAutoFmtGetDocNode&)rInfo).pNodes )
/*N*/ 		return TRUE;
/*N*/ 
/*N*/ 	((SwAutoFmtGetDocNode&)rInfo).pCntntNode = pTxtNd;
/*N*/ 	return FALSE;
/*N*/ }


/*N*/ BOOL SwFmtFld::IsFldInDoc() const
/*N*/ {
/*N*/ 	const SwTxtNode* pTxtNd;
/*N*/ 	return pTxtAttr && 0 != ( pTxtNd = pTxtAttr->GetpTxtNode() ) &&
/*N*/ 			pTxtNd->GetNodes().IsDocNodes();
/*N*/ }


/*************************************************************************
|*
|*                SwTxtFld::SwTxtFld()
|*
|*    Beschreibung      Attribut fuer automatischen Text, Ctor
|*    Ersterstellung    BP 30.04.92
|*    Letzte Aenderung	JP 15.08.94
|*
*************************************************************************/

/*N*/ SwTxtFld::SwTxtFld( const SwFmtFld& rAttr, xub_StrLen nStart )
/*N*/ 	: SwTxtAttr( rAttr, nStart ),
/*N*/ 	aExpand( rAttr.GetFld()->Expand() ),
/*N*/ 	pMyTxtNd( 0 )
/*N*/ {
/*N*/ 	((SwFmtFld&)rAttr).pTxtAttr = this;
/*N*/ }

/*N*/ SwTxtFld::~SwTxtFld( )
/*N*/ {
/*N*/ }

/*************************************************************************
|*
|*                SwTxtFld::Expand()
|*
|*    Beschreibung      exandiert das Feld und tauscht den Text im Node
|*    Ersterstellung    BP 30.04.92
|*    Letzte Aenderung	JP 15.08.94
|*
*************************************************************************/

/*N*/ void SwTxtFld::Expand()
/*N*/ {
/*N*/ 	// Wenn das expandierte Feld sich nicht veraendert hat, wird returnt
/*N*/ 	ASSERT( pMyTxtNd, "wo ist denn mein Node?" );
/*N*/ 
/*N*/ 	const SwField* pFld = GetFld().GetFld();
/*N*/ 	XubString aNewExpand( pFld->Expand() );
/*N*/ 
/*N*/ 	if( aNewExpand == aExpand )
/*N*/ 	{
/*N*/ 		// Bei Seitennummernfeldern
/*N*/ 		const USHORT nWhich = pFld->GetTyp()->Which();
/*N*/ 		if( RES_CHAPTERFLD != nWhich && RES_PAGENUMBERFLD != nWhich &&
/*N*/ 			RES_REFPAGEGETFLD != nWhich &&
/*N*/ 			( RES_GETEXPFLD != nWhich ||
/*N*/ 				((SwGetExpField*)pFld)->IsInBodyTxt() ) )
/*N*/ 		{
/*N*/ 			// BP: das muesste man noch optimieren!
/*N*/ 			//JP 12.06.97: stimmt, man sollte auf jedenfall eine Status-
/*N*/ 			//				aenderung an die Frames posten
/*N*/ 			if( pMyTxtNd->CalcVisibleFlag() )
/*?*/ 				pMyTxtNd->Modify( 0, 0 );
/*N*/ 			return;
/*N*/ 		}
/*N*/ 	}
/*N*/ 
/*N*/ 	aExpand = aNewExpand;
/*N*/ 
/*N*/ 	// 0, this fuer Formatieren
/*N*/ 	pMyTxtNd->Modify( 0, (SfxPoolItem*)&GetFld() );
/*N*/ }

/*************************************************************************
 *                      SwTxtFld::CopyFld()
 *************************************************************************/



}
