/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: editobj2.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: obo $ $Date: 2007/07/17 11:31:34 $
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

#ifndef _EDITOBJ2_HXX
#define _EDITOBJ2_HXX

#include <editobj.hxx>
#include <editdoc.hxx>

#include <vcl/fontcvt.hxx>
class SfxStyleSheetPool; 
namespace binfilter {



class XEditAttribute
{
	friend class ContentInfo;	// fuer DTOR
	friend class BinTextObject;	// fuer DTOR

private:
	const SfxPoolItem*	pItem;
	USHORT 				nStart;
	USHORT 				nEnd;

						XEditAttribute();
						XEditAttribute( const XEditAttribute& rCopyFrom );

						~XEditAttribute();

public:
						XEditAttribute( const SfxPoolItem& rAttr, USHORT nStart, USHORT nEnd );

	const SfxPoolItem*	GetItem() const				{ return pItem; }

	USHORT&				GetStart()					{ return nStart; }
	USHORT&				GetEnd()					{ return nEnd; }

	USHORT				GetStart() const			{ return nStart; }
	USHORT				GetEnd() const				{ return nEnd; }

	USHORT				GetLen() const				{ return nEnd-nStart; }

	inline BOOL			IsFeature();
};

inline BOOL XEditAttribute::IsFeature()
{
	USHORT nWhich = pItem->Which();
	return 	( ( nWhich >= EE_FEATURE_START ) &&
			  ( nWhich <=  EE_FEATURE_END ) );
}

typedef XEditAttribute* XEditAttributePtr;
SV_DECL_PTRARR( XEditAttributeListImpl, XEditAttributePtr, 0, 4 )//STRIP008 ;

class XEditAttributeList : public XEditAttributeListImpl
{
public:
    XEditAttribute* FindAttrib( USHORT nWhich, USHORT nChar ) const;
};

struct XParaPortion
{
	long				nHeight;
	USHORT				nFirstLineOffset;

	EditLineList		aLines;
	TextPortionList		aTextPortions;
};

typedef XParaPortion* XParaPortionPtr;
SV_DECL_PTRARR( XBaseParaPortionList, XParaPortionPtr, 0, 4 )//STRIP008 ;

class XParaPortionList : public  XBaseParaPortionList
{
	ULONG 		nRefDevPtr;
	OutDevType	eRefDevType;
	MapMode		aRefMapMode;
	ULONG		nPaperWidth;


public:
			XParaPortionList( OutputDevice* pRefDev, ULONG nPW ) :
				aRefMapMode( pRefDev->GetMapMode() )
				{
					nRefDevPtr = (ULONG)pRefDev; nPaperWidth = nPW;
					eRefDevType = pRefDev->GetOutDevType();
				}

	ULONG			GetRefDevPtr() const 		{ return nRefDevPtr; }
	ULONG			GetPaperWidth() const 		{ return nPaperWidth; }
	OutDevType		GetRefDevType() const 		{ return eRefDevType; }
	const MapMode&	GetRefMapMode() const		{ return aRefMapMode; }
};

struct LoadStoreTempInfos
{
    ByteString              aOrgString_Load;

    FontToSubsFontConverter hOldSymbolConv_Store;
    BOOL                    bSymbolParagraph_Store;


    LoadStoreTempInfos() { bSymbolParagraph_Store = FALSE; hOldSymbolConv_Store = NULL; }
};

class ContentInfo
{
	friend class BinTextObject;

private:
	String				aText;
	String				aStyle;
	XEditAttributeList	aAttribs;
	SfxStyleFamily		eFamily;
	SfxItemSet			aParaAttribs;

    LoadStoreTempInfos* pTempLoadStoreInfos;

						ContentInfo( SfxItemPool& rPool );
						ContentInfo( const ContentInfo& rCopyFrom, SfxItemPool& rPoolToUse  );

public:
						~ContentInfo();

	const String&		GetText()			const	{ return aText; }
	const String&		GetStyle()			const	{ return aStyle; }
	const XEditAttributeList& GetAttribs()	const	{ return aAttribs; }
	const SfxItemSet&	GetParaAttribs()	const	{ return aParaAttribs; }
	SfxStyleFamily		GetFamily()			const	{ return eFamily; }

	String&				GetText()			{ return aText; }
	String&				GetStyle()			{ return aStyle; }
	XEditAttributeList&	GetAttribs()		{ return aAttribs; }
	SfxItemSet&			GetParaAttribs()	{ return aParaAttribs; }
	SfxStyleFamily&		GetFamily()			{ return eFamily; }

    LoadStoreTempInfos* GetLoadStoreTempInfos() const   { return pTempLoadStoreInfos; }
    void                CreateLoadStoreTempInfos();
    void                DestroyLoadStoreTempInfos();


};

typedef ContentInfo* ContentInfoPtr;
SV_DECL_PTRARR( ContentInfoList, ContentInfoPtr, 1, 4 )//STRIP008 ;

// MT 05/00: Sollte mal direkt EditTextObjekt werden => keine virtuellen Methoden mehr.

class BinTextObject : public EditTextObject
{
private:
	ContentInfoList			aContents;
	SfxItemPool*			pPool;
	BOOL					bOwnerOfPool;
	XParaPortionList*		pPortionInfo;
	
	sal_uInt32				nObjSettings;
	USHORT 					nMetric;
	USHORT 					nVersion;
	USHORT					nUserType;
	USHORT					nScriptType;

	BOOL					bVertical;
	BOOL					bStoreUnicodeStrings;

protected:
	void					DeleteContents();
	virtual void			StoreData( SvStream& rOStream ) const;
	virtual void			CreateData( SvStream& rIStream );
	BOOL					ImpChangeStyleSheets( const String& rOldName, SfxStyleFamily eOldFamily,
										const String& rNewName, SfxStyleFamily eNewFamily );

public:
							BinTextObject( SfxItemPool* pPool );
							BinTextObject( const BinTextObject& );
	virtual					~BinTextObject();

	virtual EditTextObject*	Clone() const;

	USHORT					GetUserType() const;
	void					SetUserType( USHORT n );


	BOOL					IsVertical() const;
	void					SetVertical( BOOL b );

	void					SetScriptType( USHORT nType );

	USHORT					GetVersion() const;	// Solange der Outliner keine Recordlaenge speichert
	void 					AdjustImportedLRSpaceItems( BOOL bTurnOfBullets );

	ContentInfo*			CreateAndInsertContent();
	XEditAttribute*			CreateAttrib( const SfxPoolItem& rItem, USHORT nStart, USHORT nEnd );
	void					DestroyAttrib( XEditAttribute* pAttr );

	ContentInfoList&		GetContents()			{ return aContents; }
	const ContentInfoList&	GetContents() const		{ return aContents; }
	SfxItemPool*			GetPool() const			{ return pPool; }
	XParaPortionList*		GetPortionInfo() const	{ return pPortionInfo; }
	void					SetPortionInfo( XParaPortionList* pP )
								{ pPortionInfo = pP; }

	virtual USHORT			GetParagraphCount() const;
	virtual String			GetText( USHORT nParagraph ) const;
	virtual void			Insert( const EditTextObject& rObj, USHORT nPara );

	virtual void			ClearPortionInfo();



	
	virtual void			MergeParaAttribs( const SfxItemSet& rAttribs, USHORT nStart, USHORT nEnd );

	virtual BOOL			HasField( TypeId Type = NULL ) const;

	SfxItemSet 				GetParaAttribs( USHORT nPara ) const;

	virtual void			GetStyleSheet( USHORT nPara, XubString& rName, SfxStyleFamily& eFamily ) const;
	virtual void			SetStyleSheet( USHORT nPara, const XubString& rName, const SfxStyleFamily& eFamily );
	virtual BOOL			ChangeStyleSheets( 	const XubString& rOldName, SfxStyleFamily eOldFamily,
												const String& rNewName, SfxStyleFamily eNewFamily );
	virtual void			ChangeStyleSheetName( SfxStyleFamily eFamily, const XubString& rOldName, const XubString& rNewName );

	void					CreateData300( SvStream& rIStream );

	BOOL					HasMetric() const			{ return nMetric != 0xFFFF; }
	USHORT					GetMetric() const			{ return nMetric; }
	void					SetMetric( USHORT n )		{ nMetric = n; }

	BOOL					IsOwnerOfPool() const		{ return bOwnerOfPool; }
	void					StoreUnicodeStrings( BOOL b ) { bStoreUnicodeStrings = b; }

	void 					PrepareStore( SfxStyleSheetPool* pStyleSheetPool );
	void 					FinishStore();
	void 					FinishLoad( SfxStyleSheetPool* pStyleSheetPool );
};

}//end of namespace binfilter
#endif	// _EDITOBJ2_HXX

