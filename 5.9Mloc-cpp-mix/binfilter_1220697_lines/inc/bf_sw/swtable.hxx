/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: swtable.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: kz $ $Date: 2006/11/08 13:13:21 $
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
#ifndef _SWTABLE_HXX
#define _SWTABLE_HXX

#ifndef _SVMEMPOOL_HXX //autogen
#include <tools/mempool.hxx>
#endif
#ifndef _TOOLS_REF_HXX
#include <tools/ref.hxx>
#endif
#ifndef _SVARRAY_HXX //autogen
#include <svtools/svarray.hxx>
#endif
#ifndef _TBLENUM_HXX
#include <tblenum.hxx>
#endif
#ifndef _CALBCK_HXX
#include <calbck.hxx>
#endif

#ifdef PRODUCT
#ifndef _NODE_HXX
#include <node.hxx>			// fuer StartNode->GetMyIndex
#endif
#else
namespace binfilter {
class SwStartNode;
} //namespace binfilter
#endif
class Color;
class SfxPoolItem;
namespace binfilter {


class SwFrmFmt;
class SwTableFmt;
class SwTableLineFmt;
class SwTableBoxFmt;
class SwHTMLTableLayout;
class SwTableLine;
class SwTableBox;
class SwTableNode;
class SwTabCols;
class SwDoc;
class SwSelBoxes;
class SwTblCalcPara;
class SwChartLines;
struct SwPosition;
class SwNodeIndex;

class SchMemChart;
class SwUndoTblMerge;
class SwUndo;
class SwTableBox_Impl;
class SwUndoTblCpyTbl;

#ifndef SW_DECL_SWSERVEROBJECT_DEFINED
#define SW_DECL_SWSERVEROBJECT_DEFINED
SV_DECL_REF( SwServerObject )
#endif

SV_DECL_PTRARR_DEL(SwTableLines, SwTableLine*, 10, 20)//STRIP008 ;
SV_DECL_PTRARR_DEL(SwTableBoxes, SwTableBox*, 25, 50)//STRIP008 ;

// speicher die Inhaltstragenden Box-Pointer zusaetzlich in einem
// sortierten Array (fuers rechnen in der Tabelle)
typedef SwTableBox* SwTableBoxPtr;
SV_DECL_PTRARR_SORT( SwTableSortBoxes, SwTableBoxPtr, 25, 50 )//STRIP008 ;

class SwTable: public SwClient			 //Client vom FrmFmt
{
protected:
	SwTableLines aLines;
	SwTableSortBoxes aSortCntBoxes;
 	SwServerObjectRef refObj;	// falls DataServer -> Pointer gesetzt

	SwHTMLTableLayout *pHTMLLayout;

//SOLL das fuer jede Tabelle einstellbar sein?
	TblChgMode	 eTblChgMode;

	USHORT		 nGrfsThatResize;	// Anzahl der Grfs, die beim HTML-Import
									// noch ein Resize der Tbl. anstossen

	BOOL		 bModifyLocked	:1;
	BOOL 		 bHeadlineRepeat:1;

	BOOL IsModifyLocked(){ return bModifyLocked;}

public:
	TYPEINFO();

	SwTable( SwTableFmt* );
	SwTable( const SwTable& rTable );		// kein Copy der Lines !!
	~SwTable();

	SwHTMLTableLayout *GetHTMLTableLayout() { return pHTMLLayout; }
	const SwHTMLTableLayout *GetHTMLTableLayout() const { return pHTMLLayout; }
    void SetHTMLTableLayout( SwHTMLTableLayout *p );    //Eigentumsuebergang!

	USHORT IncGrfsThatResize() { return ++nGrfsThatResize; }
	USHORT DecGrfsThatResize() { return nGrfsThatResize ? --nGrfsThatResize : 0; }

	void LockModify()	{ bModifyLocked = TRUE; }	//Muessen _immer_ paarig
	void UnlockModify()	{ bModifyLocked = FALSE;}	//benutzt werden!

	BOOL IsHeadlineRepeat() const { return bHeadlineRepeat; }
	void SetHeadlineRepeat( BOOL bSet ) { bHeadlineRepeat = bSet; }

		  SwTableLines &GetTabLines() { return aLines; }
	const SwTableLines &GetTabLines() const { return aLines; }

	SwFrmFmt* GetFrmFmt() 		{ return (SwFrmFmt*)pRegisteredIn; }
	SwFrmFmt* GetFrmFmt() const	{ return (SwFrmFmt*)pRegisteredIn; }

	virtual void Modify( SfxPoolItem* pOld, SfxPoolItem* pNew );

	void GetTabCols( SwTabCols &rToFill, const SwTableBox *pStart,
					 FASTBOOL bHidden = FALSE, BOOL bCurRowOnly = FALSE ) const;

    BOOL DeleteSel( SwDoc*, const SwSelBoxes& rBoxes, SwUndo* pUndo = 0,
                            const BOOL bDelMakeFrms = TRUE,
                            const BOOL bCorrBorder = TRUE );
            BOOL Merge( SwDoc* pDoc, const SwSelBoxes& rBoxes,
                SwTableBox* pMergeBox, SwUndoTblMerge* = 0 );

		  SwTableSortBoxes& GetTabSortBoxes() 		{ return aSortCntBoxes; }
	const SwTableSortBoxes& GetTabSortBoxes() const { return aSortCntBoxes; }

		// gebe den Zellnamen zu der angebenen Row/Col zurueck. Das ist
		// nur fuer ausgeglichene Tabellen interessant, weil diese keine
		// "Sub"Boxen kennen. Es wird z.B. aus (0,0) ein "A1".
		// lese die 1. Nummer und loesche sie aus dem String
		// (wird von GetTblBox und SwTblFld benutzt)
	static USHORT _GetBoxNum( String& rStr, BOOL bFirst = FALSE );
		// suche die Inhaltstragende Box mit dem Namen
	const SwTableBox* GetTblBox( const String& rName ) const;
		// kopiere die selektierten Boxen in ein anderes Dokument.
		// kopiere die Tabelle in diese. (die Logik steht im TBLRWCL.CXX)
		// kopiere die Headline (mit Inhalt!) der Tabelle in eine andere

		// erfrage die Box, dessen Start-Index auf nBoxStt steht
		  SwTableBox* GetTblBox( ULONG nSttIdx );
	const SwTableBox* GetTblBox( ULONG nSttIdx ) const
						{	return ((SwTable*)this)->GetTblBox( nSttIdx );	}

	// returnt TRUE wenn sich in der Tabelle Verschachtelungen befinden
	BOOL IsTblComplex() const;

	//returnt TRUE wenn die Tabelle oder Selektion ausgeglichen ist
	BOOL IsTblComplexForChart( const String& rSel,
								SwChartLines* pGetCLines = 0  ) const;

	// suche alle Inhaltstragenden-Boxen der Grundline in der diese Box
	// steht. rBoxes auch als Return-Wert, um es gleich weiter zu benutzen
	//JP 31.01.97: bToTop = TRUE -> hoch bis zur Grundline,
	//						FALSE-> sonst nur die Line der Box
		// erfrage vom Client Informationen
	virtual BOOL GetInfo( SfxPoolItem& ) const;

		// suche im Format nach der angemeldeten Tabelle
	static SwTable* FindTable( SwFrmFmt* pFmt );

		// Struktur ein wenig aufraeumen
	void GCLines();
		// BorderLine ein wenig aufraeumen
	void GCBorderLines();

	SwTableNode* GetTableNode() const;

		// Daten Server-Methoden
 	void SetRefObject( SwServerObject* );
	const SwServerObject* GetObject() const		{  return &refObj; }
	  SwServerObject* GetObject() 			{  return &refObj; }

	//Daten fuer das Chart fuellen.
	SchMemChart *UpdateData( SchMemChart *pData,
							const String* pSelection = 0 ) const;

	TblChgMode GetTblChgMode() const 		{ return eTblChgMode; }
	void SetTblChgMode( TblChgMode eMode )	{ eTblChgMode = eMode; }

};

class SwTableLine: public SwClient		// Client vom FrmFmt
{
	SwTableBoxes aBoxes;
	SwTableBox *pUpper;

public:
	TYPEINFO();

	SwTableLine( SwTableLineFmt*, USHORT nBoxes, SwTableBox *pUp );
	~SwTableLine();

		  SwTableBoxes &GetTabBoxes() { return aBoxes; }
	const SwTableBoxes &GetTabBoxes() const { return aBoxes; }

		  SwTableBox *GetUpper() { return pUpper; }
	const SwTableBox *GetUpper() const { return pUpper; }
	void SetUpper( SwTableBox *pNew ) { pUpper = pNew; }


	SwFrmFmt* GetFrmFmt()		{ return (SwFrmFmt*)pRegisteredIn; }
	SwFrmFmt* GetFrmFmt() const	{ return (SwFrmFmt*)pRegisteredIn; }

	//Macht ein eingenes FrmFmt wenn noch mehr Lines von ihm abhaengen.
	SwFrmFmt* ClaimFrmFmt();
    void ChgFrmFmt( SwTableLineFmt* pNewFmt );

	// suche nach der naechsten/vorherigen Box mit Inhalt
    SwTableBox* FindNextBox( const SwTable&, const SwTableBox* =0,
                            BOOL bOvrTblLns=TRUE ) const;
    SwTableBox* FindPreviousBox( const SwTable&, const SwTableBox* =0,
                            BOOL bOvrTblLns=TRUE ) const;

};

class SwTableBox: public SwClient		//Client vom FrmFmt
{
	friend class SwNodes;			// um den Index umzusetzen !
	friend void DelBoxNode(SwTableSortBoxes&);  // um den StartNode* zu loeschen !
	friend class SwXMLTableContext;

	//nicht (mehr) implementiert.
	SwTableBox( const SwTableBox & );

	SwTableLines aLines;
	const SwStartNode * pSttNd;
	SwTableLine *pUpper;
	SwTableBox_Impl* pImpl;

	SwTableBox &operator=( const SwTableBox &);	//gibts nicht.
	// falls das Format schon Formeln/Values enthaelt, muss ein neues
	// fuer die neue Box erzeugt werden.
	SwTableBoxFmt* CheckBoxFmt( SwTableBoxFmt* );

public:
	TYPEINFO();

	SwTableBox( SwTableBoxFmt*, USHORT nLines, SwTableLine *pUp = 0 );
	SwTableBox( SwTableBoxFmt*, const SwStartNode&, SwTableLine *pUp = 0 );
	SwTableBox( SwTableBoxFmt*, const SwNodeIndex&, SwTableLine *pUp = 0 );
	~SwTableBox();

		  SwTableLines &GetTabLines() { return aLines; }
	const SwTableLines &GetTabLines() const { return aLines; }

		  SwTableLine *GetUpper() { return pUpper; }
	const SwTableLine *GetUpper() const { return pUpper; }
	void SetUpper( SwTableLine *pNew ) { pUpper = pNew; }

	SwFrmFmt* GetFrmFmt()		{ return (SwFrmFmt*)pRegisteredIn; }
	SwFrmFmt* GetFrmFmt() const	{ return (SwFrmFmt*)pRegisteredIn; }

	//Macht ein eingenes FrmFmt wenn noch mehr Boxen von ihm abhaengen.
	SwFrmFmt* ClaimFrmFmt();
	void ChgFrmFmt( SwTableBoxFmt *pNewFmt );

	const SwStartNode *GetSttNd() const { return pSttNd; }
	ULONG GetSttIdx() const
#ifdef PRODUCT
		{ return pSttNd ? pSttNd->GetIndex() : 0; }
#else
		;
#endif

	// suche nach der naechsten/vorherigen Box mit Inhalt
	// gebe den Namen dieser Box zurueck. Dieser wird dynamisch bestimmt
	// und ergibt sich aus der Position in den Lines/Boxen/Tabelle
	String GetName() const;
	// gebe den "Wert" der Box zurueck (fuers rechnen in der Tabelle)
	double GetValue( SwTblCalcPara& rPara ) const;

	BOOL IsInHeadline( const SwTable* pTbl = 0 ) const;

	// enthaelt die Box Inhalt, der als Nummer formatiert werden kann?
	ULONG IsValidNumTxtNd( BOOL bCheckAttr = TRUE ) const;
	// teste ob der BoxInhalt mit der Nummer uebereinstimmt, wenn eine
	// Tabellenformel gesetzt ist. (fuers Redo des Change vom NumFormat!)

	// ist das eine FormelBox oder eine Box mit numerischen Inhalt (AutoSum)
	// Was es ist, besagt der ReturnWert - die WhichId des Attributes
	// Leere Boxen haben den ReturnWert USHRT_MAX !!

	// fuers Laden - tauscht bei Value Zellen den Inhalt aus, falls sie
	// fuer die Sprache System formatiert sind.
	void ChgByLanguageSystem();

	DECL_FIXEDMEMPOOL_NEWDEL(SwTableBox)

	// zugriff auf interne Daten - z.Z. benutzt fuer den NumFormatter
};

} //namespace binfilter
#endif	//_SWTABLE_HXX
