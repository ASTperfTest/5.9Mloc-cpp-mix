/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_untbl.cxx,v $
 *
 *  $Revision: 1.9 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:47:46 $
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

#define ITEMID_BOXINFO      SID_ATTR_BORDER_INNER
#ifndef _HINTIDS_HXX
#include <hintids.hxx>
#endif


#ifndef _FRMFMT_HXX
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
#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif
#ifndef _TBLSEL_HXX
#include <tblsel.hxx>
#endif
#ifndef _HINTS_HXX
#include <hints.hxx>
#endif
#ifndef _SWUNDO_HXX
#include <swundo.hxx>			// fuer die UndoIds
#endif
#ifndef _UNDOBJ_HXX
#include <undobj.hxx>
#endif
#ifndef _DDEFLD_HXX
#include <ddefld.hxx>
#endif
#ifndef _CELLFRM_HXX
#include <cellfrm.hxx>
#endif
#ifndef _SWCACHE_HXX
#include <swcache.hxx>
#endif
#ifndef _CELLATR_HXX
#include <cellatr.hxx>
#endif
#ifndef _SWTBLFMT_HXX
#include <swtblfmt.hxx>
#endif
#ifndef _REDLINE_HXX
#include <redline.hxx>
#endif
namespace binfilter {


typedef SfxItemSet* SfxItemSetPtr;
SV_DECL_PTRARR_DEL( SfxItemSets, SfxItemSetPtr, 10, 5 )

typedef SwUndoSaveSection* SwUndoSaveSectionPtr;
SV_DECL_PTRARR_DEL( SwUndoSaveSections, SwUndoSaveSectionPtr, 0, 10 )

class _SaveBox;
class _SaveLine;

class _SaveTable
{
    friend class _SaveBox;
    friend class _SaveLine;
    _SaveLine* pLine;
    const SwTable* pSwTable;
    SfxItemSets aSets;
    SwFrmFmts aFrmFmts;
    SfxItemSet aTblSet;
    USHORT nLineCount;
    BOOL bModifyBox : 1;
    BOOL bSaveFormula : 1;

public:
    _SaveTable( const SwTable& rTbl, USHORT nLnCnt = USHRT_MAX,
                BOOL bSaveFml = TRUE );
    ~_SaveTable();

    USHORT AddFmt( SwFrmFmt* pFmt );
};

class _SaveLine
{
    friend class _SaveTable;
    friend class _SaveBox;

    _SaveLine* pNext;
    _SaveBox* pBox;
    USHORT nItemSet;

public:

    _SaveLine( _SaveLine* pPrev, const SwTableLine& rLine, _SaveTable& rSTbl );
    ~_SaveLine();
    void CreateNew( SwTable& rTbl, SwTableBox& rParent, _SaveTable& rSTbl  );
};

class _SaveBox
{
    friend class _SaveLine;

    _SaveBox* pNext;
    ULONG nSttNode;
    USHORT nItemSet;
    union
    {
        SfxItemSets* pCntntAttrs;
        _SaveLine* pLine;
    } Ptrs;

public:
    _SaveBox( _SaveBox* pPrev, const SwTableBox& rBox, _SaveTable& rSTbl );
    ~_SaveBox();
    void CreateNew( SwTable& rTbl, SwTableLine& rParent, _SaveTable& rSTbl );
};

void InsertSort( SvULongs& rArr, ULONG nIdx, USHORT* pInsPos = 0 );


SV_IMPL_PTRARR( SfxItemSets, SfxItemSetPtr )
SV_IMPL_PTRARR( SwUndoSaveSections, SwUndoSaveSectionPtr )

/*N*/ SwUndoInsTbl::SwUndoInsTbl( const SwPosition& rPos, USHORT nCl, USHORT nRw,
/*N*/ 							USHORT nAdj, USHORT nInsert,
/*N*/ 							const SwTableAutoFmt* ,
/*N*/ 							const SvUShorts* pColArr )
/*N*/ 	: SwUndo( UNDO_INSTABLE ), nSttNode( rPos.nNode.GetIndex() ),
/*N*/ 	nRows( nRw ), nCols( nCl ), nAdjust( nAdj ), pDDEFldType( 0 ),
/*N*/ 	nInsTblFlags( nInsert ), pColWidth( 0 ), pRedlData( 0 ), pAutoFmt( 0 )
/*N*/ {
/*N*/ 	if( pColArr )
/*N*/ 	{
/*?*/ 		pColWidth = new SvUShorts( 0, 1 );
/*?*/ 		pColWidth->Insert( pColArr, 0 );
/*N*/ 	}
/*N*/
/*N*/ 	// Redline beachten
/*N*/ 	SwDoc& rDoc = *rPos.nNode.GetNode().GetDoc();
/*N*/ 	if( rDoc.IsRedlineOn() )
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 pRedlData = new SwRedlineData( REDLINE_INSERT, rDoc.GetRedlineAuthor() );
/*N*/ 	}
/*N*/ }


/*N*/ SwUndoInsTbl::~SwUndoInsTbl()
/*N*/ {
/*N*/ 	delete pDDEFldType;
/*N*/ 	delete pColWidth;
/*N*/ }

// -----------------------------------------------------

/*  */

_SaveTable::_SaveTable( const SwTable& rTbl, USHORT nLnCnt, BOOL bSaveFml )
    : aTblSet( *rTbl.GetFrmFmt()->GetAttrSet().GetPool(), aTableSetRange ),
    nLineCount( nLnCnt ), pSwTable( &rTbl ), bSaveFormula( bSaveFml )
{
    bModifyBox = FALSE;
    aTblSet.Put( rTbl.GetFrmFmt()->GetAttrSet() );
    pLine = new _SaveLine( 0, *rTbl.GetTabLines()[ 0 ], *this );

    _SaveLine* pLn = pLine;
    if( USHRT_MAX == nLnCnt )
        nLnCnt = rTbl.GetTabLines().Count();
    for( USHORT n = 1; n < nLnCnt; ++n )
        pLn = new _SaveLine( pLn, *rTbl.GetTabLines()[ n ], *this );

    aFrmFmts.Remove( 0, aFrmFmts.Count() );
    pSwTable = 0;
}


_SaveTable::~_SaveTable()
{
    delete pLine;
}


USHORT _SaveTable::AddFmt( SwFrmFmt* pFmt )
{
    USHORT nRet = aFrmFmts.GetPos( pFmt );
    if( USHRT_MAX == nRet )
    {
        // Kopie vom ItemSet anlegen
        SfxItemSet* pSet = new SfxItemSet( *pFmt->GetAttrSet().GetPool(),
                                            aTableBoxSetRange );
        pSet->Put( pFmt->GetAttrSet() );
        //JP 20.04.98: Bug 49502 - wenn eine Formel gesetzt ist, nie den
        //              Value mit sichern. Der muss gegebenfalls neu
        //              errechnet werden!
        //JP 30.07.98: Bug 54295 - Formeln immer im Klartext speichern
        const SfxPoolItem* pItem;
        if( SFX_ITEM_SET == pSet->GetItemState( RES_BOXATR_FORMULA, TRUE, &pItem ))
        {
            pSet->ClearItem( RES_BOXATR_VALUE );
            if( pSwTable && bSaveFormula )
            {
                SwTableFmlUpdate aMsgHnt( pSwTable );
                aMsgHnt.eFlags = TBL_BOXNAME;
                ((SwTblBoxFormula*)pItem)->ChgDefinedIn( pFmt );
                ((SwTblBoxFormula*)pItem)->ChangeState( &aMsgHnt );
                ((SwTblBoxFormula*)pItem)->ChgDefinedIn( 0 );
            }
        }
        aSets.Insert( pSet, (nRet = aSets.Count() ) );
        aFrmFmts.Insert( pFmt, nRet );
    }
    return nRet;
}


_SaveLine::_SaveLine( _SaveLine* pPrev, const SwTableLine& rLine, _SaveTable& rSTbl )
    : pNext( 0 )
{
    if( pPrev )
        pPrev->pNext = this;

    nItemSet = rSTbl.AddFmt( rLine.GetFrmFmt() );

    pBox = new _SaveBox( 0, *rLine.GetTabBoxes()[ 0 ], rSTbl );
    _SaveBox* pBx = pBox;
    for( USHORT n = 1; n < rLine.GetTabBoxes().Count(); ++n )
        pBx = new _SaveBox( pBx, *rLine.GetTabBoxes()[ n ], rSTbl );
}


_SaveLine::~_SaveLine()
{
    delete pBox;
    delete pNext;
}

void _SaveLine::CreateNew( SwTable& rTbl, SwTableBox& rParent, _SaveTable& rSTbl )
{
    SwTableLineFmt* pFmt = (SwTableLineFmt*)rSTbl.aFrmFmts[ nItemSet ];
    if( !pFmt )
    {
        SwDoc* pDoc = rTbl.GetFrmFmt()->GetDoc();
        pFmt = pDoc->MakeTableLineFmt();
        pFmt->SetAttr( *rSTbl.aSets[ nItemSet ] );
        rSTbl.aFrmFmts.Replace( pFmt, nItemSet );
    }
    SwTableLine* pNew = new SwTableLine( pFmt, 1, &rParent );
    rParent.GetTabLines().C40_INSERT( SwTableLine, pNew, rParent.GetTabLines().Count() );

    pBox->CreateNew( rTbl, *pNew, rSTbl );

    if( pNext )
        pNext->CreateNew( rTbl, rParent, rSTbl );
}


_SaveBox::_SaveBox( _SaveBox* pPrev, const SwTableBox& rBox, _SaveTable& rSTbl )
    : nSttNode( ULONG_MAX ), pNext( 0 )
{
    Ptrs.pLine = 0;

    if( pPrev )
        pPrev->pNext = this;

    nItemSet = rSTbl.AddFmt( rBox.GetFrmFmt() );

    if( rBox.GetSttNd() )
        nSttNode = rBox.GetSttIdx();
    else
    {
        Ptrs.pLine = new _SaveLine( 0, *rBox.GetTabLines()[ 0 ], rSTbl );

        _SaveLine* pLn = Ptrs.pLine;
        for( USHORT n = 1; n < rBox.GetTabLines().Count(); ++n )
            pLn = new _SaveLine( pLn, *rBox.GetTabLines()[ n ], rSTbl );
    }
}


_SaveBox::~_SaveBox()
{
    if( ULONG_MAX == nSttNode )     // keine EndBox
        delete Ptrs.pLine;
    else
        delete Ptrs.pCntntAttrs;
    delete pNext;
}

void _SaveBox::CreateNew( SwTable& rTbl, SwTableLine& rParent, _SaveTable& rSTbl )
{
    SwTableBoxFmt* pFmt = (SwTableBoxFmt*)rSTbl.aFrmFmts[ nItemSet ];
    if( !pFmt )
    {
        SwDoc* pDoc = rTbl.GetFrmFmt()->GetDoc();
        pFmt = pDoc->MakeTableBoxFmt();
        pFmt->SetAttr( *rSTbl.aSets[ nItemSet ] );
        rSTbl.aFrmFmts.Replace( pFmt, nItemSet );
    }

    if( ULONG_MAX == nSttNode )     // keine EndBox
    {
        SwTableBox* pNew = new SwTableBox( pFmt, 1, &rParent );
        rParent.GetTabBoxes().C40_INSERT( SwTableBox, pNew, rParent.GetTabBoxes().Count() );

        Ptrs.pLine->CreateNew( rTbl, *pNew, rSTbl );
    }
    else
    {
        // Box zum StartNode in der alten Tabelle suchen
        SwTableBox* pBox = rTbl.GetTblBox( nSttNode );
        ASSERT( pBox, "Wo ist meine TabellenBox geblieben?" );

        SwFrmFmt* pOld = pBox->GetFrmFmt();
        pFmt->Add( pBox );
        if( !pOld->GetDepends() )
            delete pOld;

        SwTableBoxes* pTBoxes = &pBox->GetUpper()->GetTabBoxes();
        pTBoxes->Remove( pTBoxes->C40_GETPOS( SwTableBox, pBox ) );

        pBox->SetUpper( &rParent );
        pTBoxes = &rParent.GetTabBoxes();
        pTBoxes->C40_INSERT( SwTableBox, pBox, pTBoxes->Count() );
    }

    if( pNext )
        pNext->CreateNew( rTbl, rParent, rSTbl );
}

/*  */

// UndoObject fuer Attribut Aenderung an der Tabelle

/*  */

// UndoObject fuer AutoFormat an der Tabelle

/*  */


SwUndoTblNdsChg::SwUndoTblNdsChg( USHORT nAction,
                                    const SwSelBoxes& rBoxes,
                                    const SwTableNode& rTblNd )
    : SwUndo( nAction ),
    nSttNode( rTblNd.GetIndex() ),
    aBoxes( rBoxes.Count() < 255 ? (BYTE)rBoxes.Count() : 255, 10 ),
    nCount( 0 ), nRelDiff( 0 ), nAbsDiff( 0 ),
    nSetColType( USHRT_MAX ), nCurrBox( 0 ),
    bFlag( FALSE )
{
    Ptrs.pNewSttNds = 0;

    const SwTable& rTbl = rTblNd.GetTable();
    pSaveTbl = new _SaveTable( rTbl );

    // und die Selektion merken
    for( USHORT n = 0; n < rBoxes.Count(); ++n )
        aBoxes.Insert( rBoxes[n]->GetSttIdx(), n );
}


SwUndoTblNdsChg::~SwUndoTblNdsChg()
{
    delete pSaveTbl;

    if( UNDO_TABLE_DELBOX == GetId() )
        delete Ptrs.pDelSects;
    else
        delete Ptrs.pNewSttNds;
}

void SwUndoTblNdsChg::SaveSection( SwStartNode* pSttNd )
{
    ASSERT( UNDO_TABLE_DELBOX == GetId(), "falsche Action" );
    if( !Ptrs.pDelSects )
        Ptrs.pDelSects = new SwUndoSaveSections( 10, 5 );

    SwTableNode* pTblNd = pSttNd->FindTableNode();
    SwUndoSaveSection* pSave = new SwUndoSaveSection;
    pSave->SaveSection( pSttNd->GetDoc(), SwNodeIndex( *pSttNd ));

    Ptrs.pDelSects->Insert( pSave, Ptrs.pDelSects->Count() );
    nSttNode = pTblNd->GetIndex();
}

/*  */


/*N*/  SwUndoTblMerge::SwUndoTblMerge( const SwPaM& rTblSel )
/*N*/  	: SwUndo( UNDO_TABLE_MERGE ), SwUndRng( rTblSel ), pHistory( 0 )
/*N*/  {
        const SwTableNode* pTblNd = rTblSel.GetNode()->FindTableNode();
        nTblNode = pTblNd->GetIndex();
/*N*/ }


/*N*/  SwUndoTblMerge::~SwUndoTblMerge()
/*N*/  {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	delete pSaveTbl;
/*N*/ }



 void SwUndoTblMerge::SetSelBoxes( const SwSelBoxes& rBoxes )
 {
    // die Selektion merken
    for( USHORT n = 0; n < rBoxes.Count(); ++n )
        InsertSort( aBoxes, rBoxes[n]->GetSttIdx() );

    // als Trennung fuers einfuegen neuer Boxen nach dem Verschieben!
    aNewSttNds.Insert( (ULONG)0, aNewSttNds.Count() );

    nTblNode = rBoxes[ 0 ]->GetSttNd()->FindTableNode()->GetIndex();
 }

/*  */

/** switch the RedlineMode on the given document, using
 * SetRedlineMode_intern. This class set the mode in the constructor,
 * and changes it back in the destructor, i.e. it uses the
 * initialization-is-resource-acquisition idiom.
 */

/*  */

/*N*/ void InsertSort( SvUShorts& rArr, USHORT nIdx, USHORT* pInsPos )
/*N*/ {
/*N*/ 	register USHORT nO	= rArr.Count(), nM, nU = 0;
/*N*/ 	if( nO > 0 )
/*N*/ 	{
/*N*/ 		nO--;
/*N*/ 		while( nU <= nO )
/*N*/ 		{
/*N*/ 			nM = nU + ( nO - nU ) / 2;
/*N*/ 			if( *(rArr.GetData() + nM) == nIdx )
/*N*/ 			{
/*N*/ 				ASSERT( FALSE, "Index ist schon vorhanden, darf nie sein!" );
/*N*/ 				return;
/*N*/ 			}
/*N*/ 			if( *(rArr.GetData() + nM) < nIdx )
/*N*/ 				nU = nM + 1;
/*N*/ 			else if( nM == 0 )
/*N*/ 				break;
/*N*/ 			else
/*N*/ 				nO = nM - 1;
/*N*/ 		}
/*N*/ 	}
/*N*/ 	rArr.Insert( nIdx, nU );
/*N*/ 	if( pInsPos )
/*N*/ 		*pInsPos = nU;
/*N*/ }

 void InsertSort( SvULongs& rArr, ULONG nIdx, USHORT* pInsPos )
 {
    register USHORT nO  = rArr.Count(), nM, nU = 0;
    if( nO > 0 )
    {
        nO--;
        while( nU <= nO )
        {
            nM = nU + ( nO - nU ) / 2;
            if( *(rArr.GetData() + nM) == nIdx )
            {
                ASSERT( FALSE, "Index ist schon vorhanden, darf nie sein!" );
                return;
            }
            if( *(rArr.GetData() + nM) < nIdx )
                nU = nM + 1;
            else if( nM == 0 )
                break;
            else
                nO = nM - 1;
        }
    }
    rArr.Insert( nIdx, nU );
    if( pInsPos )
        *pInsPos = nU;
 }

}
