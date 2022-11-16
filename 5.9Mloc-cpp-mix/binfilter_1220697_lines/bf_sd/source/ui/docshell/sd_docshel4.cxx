/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_docshel4.cxx,v $
 *
 *  $Revision: 1.11 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 13:35:36 $
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

#ifndef _COM_SUN_STAR_DOCUMENT_PRINTERINDEPENDENTLAYOUT_HPP_
#include <com/sun/star/document/PrinterIndependentLayout.hpp>
#endif

#ifndef _URLOBJ_HXX
#include <tools/urlobj.hxx>
#endif
#ifndef _SVXIDS_HRC
#include <bf_svx/svxids.hrc>
#endif
#define ITEMID_FONTLIST     SID_ATTR_CHAR_FONTLIST
#ifndef _SVX_FLSTITEM_HXX
#include <bf_svx/flstitem.hxx>
#endif
#ifndef _SFXFLAGITEM_HXX //autogen
#include <svtools/flagitem.hxx>
#endif
#ifndef _SFXDOCFILE_HXX //autogen
#include <bf_sfx2/docfile.hxx>
#endif
#ifndef _SFX_DOCFILT_HACK_HXX //autogen
#include <bf_sfx2/docfilt.hxx>
#endif
#ifndef _SFX_PRINTER_HXX //autogen
#include <bf_sfx2/printer.hxx>
#endif
#ifndef _CTRLTOOL_HXX //autogen
#include <svtools/ctrltool.hxx>
#endif
#ifndef _SO_CLSIDS_HXX
#include <comphelper/classids.hxx>
#endif
#ifdef TF_STARONE
#endif
#ifndef _OFF_APP_HXX
#include <bf_offmgr/app.hxx>
#endif

#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "app.hrc"
#include "glob.hrc"
#include "strings.hrc"
#include "strmname.h"
#include "optsitem.hxx"
#include "sdoutl.hxx"
#include "sdattr.hxx"
#include "drawdoc.hxx"
#include "sdview.hxx"
#include "sdpage.hxx"
#include "sdresid.hxx"
#include "sdxmlwrp.hxx"
#include "sdbinfilter.hxx"
#include "docshell.hxx"

namespace binfilter {

/*************************************************************************
|*
|* SfxPrinter ggf. erzeugen und zurueckgeben
|*
\************************************************************************/

/*N*/ SfxPrinter* SdDrawDocShell::GetPrinter(BOOL bCreate)
/*N*/ {
/*N*/ 	if (bCreate && !pPrinter)
/*N*/ 	{
/*N*/ 		// ItemSet mit speziellem Poolbereich anlegen
/*N*/ 		SfxItemSet* pSet = new SfxItemSet( GetPool(),
/*N*/ 							SID_PRINTER_NOTFOUND_WARN,	SID_PRINTER_NOTFOUND_WARN,
/*N*/ 							SID_PRINTER_CHANGESTODOC,	SID_PRINTER_CHANGESTODOC,
/*N*/ 							ATTR_OPTIONS_PRINT, 		ATTR_OPTIONS_PRINT,
/*N*/ 							0 );
/*N*/ 		// PrintOptionsSet setzen
/*N*/ 		SdOptionsPrintItem aPrintItem( ATTR_OPTIONS_PRINT,
/*N*/ 							SD_MOD()->GetSdOptions(pDoc->GetDocumentType()));
/*N*/ 		SfxFlagItem aFlagItem( SID_PRINTER_CHANGESTODOC );
/*N*/ 		USHORT		nFlags = 0;
/*N*/ 
/*N*/ 		nFlags =  (aPrintItem.IsWarningSize() ? SFX_PRINTER_CHG_SIZE : 0) |
/*N*/ 				(aPrintItem.IsWarningOrientation() ? SFX_PRINTER_CHG_ORIENTATION : 0);
/*N*/ 		aFlagItem.SetValue( nFlags );
/*N*/ 
/*N*/ 		pSet->Put( aPrintItem );
/*N*/ 		pSet->Put( SfxBoolItem( SID_PRINTER_NOTFOUND_WARN, aPrintItem.IsWarningPrinter() ) );
/*N*/ 		pSet->Put( aFlagItem );
/*N*/ 
/*N*/ 		pPrinter = new SfxPrinter(pSet);
/*N*/ 		bOwnPrinter = TRUE;
/*N*/ 
/*N*/ 		// Ausgabequalitaet setzen
/*N*/ 		UINT16 nQuality = aPrintItem.GetOutputQuality();
/*N*/ 
/*N*/ 		ULONG nMode = DRAWMODE_DEFAULT;
/*N*/ 		
/*N*/         if( nQuality == 1 )
/*N*/ 			nMode = DRAWMODE_GRAYLINE | DRAWMODE_GRAYFILL | DRAWMODE_BLACKTEXT | DRAWMODE_GRAYBITMAP | DRAWMODE_GRAYGRADIENT;
/*N*/ 		else if( nQuality == 2 )
/*N*/ 			nMode = DRAWMODE_BLACKLINE | DRAWMODE_BLACKTEXT | DRAWMODE_WHITEFILL | DRAWMODE_GRAYBITMAP | DRAWMODE_WHITEGRADIENT;
/*N*/ 
/*N*/ 		pPrinter->SetDrawMode( nMode );
/*N*/ 
/*N*/ 		MapMode aMM (pPrinter->GetMapMode());
/*N*/ 		aMM.SetMapUnit(MAP_100TH_MM);
/*N*/ 		pPrinter->SetMapMode(aMM);
/*N*/         UpdateRefDevice();
/*N*/ 	}
/*N*/ 	return pPrinter;
/*N*/ }

/*************************************************************************
|*
|* neuen SfxPrinter setzen (Eigentuemeruebergang)
|*
\************************************************************************/

/*N*/ void SdDrawDocShell::SetPrinter(SfxPrinter *pNewPrinter)
/*N*/ {
/*N*/ 	if ( pPrinter && bOwnPrinter && (pPrinter != pNewPrinter) )
/*N*/ 	{
/*N*/ 		delete pPrinter;
/*N*/ 	}
/*N*/ 
/*N*/ 	pPrinter = pNewPrinter;
/*N*/ 	bOwnPrinter = TRUE;
/*N*/ 
/*N*/ 	delete pFontList;
/*N*/ 	pFontList = new FontList( GetPrinter(TRUE), Application::GetDefaultDevice(), FALSE );
/*N*/ 	SvxFontListItem aFontListItem( pFontList );
/*N*/ 	PutItem( aFontListItem );
/*N*/     UpdateRefDevice();
/*N*/ }

/*************************************************************************
|*
|*
|*
\************************************************************************/
/*N*/ Printer* SdDrawDocShell::GetDocumentPrinter()
/*N*/ {
/*N*/ 	return GetPrinter(FALSE);
/*N*/ }

/*************************************************************************
|*
|*
|*
\************************************************************************/

/*************************************************************************
|*
|*
|*
\************************************************************************/
/*N*/ void SdDrawDocShell::UpdateRefDevice()
/*N*/ {
/*N*/ 	if( pDoc )
/*N*/ 	{
/*N*/         // Determine the device for which the output will be formatted.
/*N*/         OutputDevice* pRefDevice = NULL;
/*N*/         switch (pDoc->GetPrinterIndependentLayout())
/*N*/         {
/*N*/             case ::com::sun::star::document::PrinterIndependentLayout::DISABLED:
/*N*/                 pRefDevice = pPrinter;
/*N*/                 break;
/*N*/ 
/*N*/             case ::com::sun::star::document::PrinterIndependentLayout::ENABLED:
/*N*/                 pRefDevice = SD_MOD()->GetVirtualRefDevice();
/*N*/                 break;
/*N*/ 
/*N*/             default:
/*N*/                 // We are confronted with an invalid or un-implemented
/*N*/                 // layout mode.  Use the printer as formatting device
/*N*/                 // as a fall-back.
/*N*/                 DBG_ASSERT(false, "SdDrawDocShell::UpdateRefDevice(): Unexpected printer layout mode");
/*N*/                 
/*N*/                 pRefDevice = pPrinter;
/*N*/                 break;
/*N*/         }
/*N*/ 		pDoc->SetRefDevice( pRefDevice );
/*N*/ 
/*N*/ 		SdOutliner* pOutl = pDoc->GetOutliner( FALSE );
/*N*/ 
/*N*/ 		if( pOutl )
/*?*/ 			pOutl->SetRefDevice( pRefDevice );
/*N*/ 
/*N*/ 		SdOutliner* pInternalOutl = pDoc->GetInternalOutliner( FALSE );
/*N*/ 
/*N*/ 		if( pInternalOutl )
/*N*/ 			pInternalOutl->SetRefDevice( pRefDevice );
/*N*/ 	}
/*N*/ }

/*************************************************************************
|*
|* InitNew, (Dokument wird neu erzeugt): Streams oeffnen
|*
\************************************************************************/

/*N*/ BOOL SdDrawDocShell::InitNew( SvStorage * pStor )
/*N*/ {
/*N*/ 	BOOL bRet = FALSE;
/*N*/ 
/*N*/ 	bRet = SfxInPlaceObject::InitNew( pStor );
/*N*/ 
/*N*/ 	Rectangle aVisArea( Point(0, 0), Size(14100, 10000) );
/*N*/ 	SetVisArea(aVisArea);
/*N*/ 
/*N*/ 	if (bRet)
/*N*/ 	{
/*N*/ 		if( !bSdDataObj )
/*N*/ 			pDoc->NewOrLoadCompleted(NEW_DOC);  // otherwise calling
/*N*/ 			                                    // NewOrLoadCompleted(NEW_LOADED) in
/*N*/ 												// SdDrawDocument::AllocModel()
/*N*/ 	}
/*N*/ 	return bRet;
/*N*/ }

/*************************************************************************
|*
|* Load: Pools und Dokument laden
|*
\************************************************************************/


/*************************************************************************
|*
|* Load: Pools und Dokument laden
|*
\************************************************************************/

/*N*/ BOOL SdDrawDocShell::Load( SvStorage* pStore )
/*N*/ {
/*N*/     mbNewDocument = sal_False;
/*N*/ 
/*N*/ 	ULONG	nStoreVer = pStore->GetVersion();
/*N*/ 	BOOL	bRet = FALSE;
/*N*/ 	BOOL	bXML = ( nStoreVer >= SOFFICE_FILEFORMAT_60 );
/*N*/ 	BOOL	bBinary = ( nStoreVer < SOFFICE_FILEFORMAT_60 );
/*N*/ 	bool	bStartPresentation = false;
/*N*/ 
/*N*/ 	if( bBinary || bXML )
/*N*/ 	{
/*N*/ 		SfxItemSet* pSet = GetMedium()->GetItemSet();
/*N*/ 
/*N*/ 
/*N*/ 		if( pSet )
/*N*/ 		{
/*N*/ 			if( (  SFX_ITEM_SET == pSet->GetItemState(SID_PREVIEW ) ) && ( (SfxBoolItem&) ( pSet->Get( SID_PREVIEW ) ) ).GetValue() )
/*N*/ 			{
/*N*/ 				pDoc->SetStarDrawPreviewMode( TRUE );
/*N*/ 			}
/*N*/ 
/*N*/ 			if( SFX_ITEM_SET == pSet->GetItemState(SID_DOC_STARTPRESENTATION)&&
/*N*/ 				( (SfxBoolItem&) ( pSet->Get( SID_DOC_STARTPRESENTATION ) ) ).GetValue() )
/*N*/ 			{
/*?*/ 				DBG_BF_ASSERT(0, "STRIP"); //STRIP001 bStartPresentation = true;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
/*N*/ 		bRet = SfxInPlaceObject::Load( pStore );
/*N*/ 
/*N*/ 		if( bRet )
/*N*/ 		{
/*N*/ 			SdFilter*	pFilter = NULL;
/*N*/ 			SfxMedium* pMedium = 0L;
/*N*/ 
/*N*/ 			if( bBinary )
/*N*/ 			{
/*N*/ 				pMedium = new SfxMedium( pStore );
/*N*/ 				pFilter = new SdBINFilter( *pMedium, *this, sal_True );
/*N*/ 			}
/*N*/ 			else if( bXML )
/*N*/ 			{
/*N*/ 				// #80365# use the medium from the DrawDocShell, do not construct an own one
/*?*/ 				pFilter = new SdXMLFilter( *GetMedium(), *this, sal_True );
/*N*/ 			}
/*N*/ 
/*N*/ 			bRet = pFilter ? pFilter->Import() : FALSE;
/*N*/ 
/*N*/ 
/*N*/ 			if(pFilter)
/*N*/ 				delete pFilter;
/*N*/ 			if(pMedium)
/*N*/ 				delete pMedium;
/*N*/ 		}
/*N*/ 	}
/*N*/ 	else
/*?*/ 		pStore->SetError( SVSTREAM_WRONGVERSION );
/*N*/ 
/*N*/ 	if( bRet )
/*N*/ 	{
/*N*/ 		UpdateTablePointers();
/*N*/ 
        // #108451# If we're an embedded OLE object, use tight bounds
        // for our visArea. No point in showing the user lots of empty
        // space. Had to remove the check for empty VisArea below,
        // since XML load always sets a VisArea before.
/*N*/ 		if( ( GetCreateMode() == SFX_CREATE_MODE_EMBEDDED ) && SfxInPlaceObject::GetVisArea( ASPECT_CONTENT ).IsEmpty() )
/*N*/ 		{
/*?*/ 			SdPage* pPage = pDoc->GetSdPage( 0, PK_STANDARD );
/*?*/ 
/*?*/ 			if( pPage )
/*?*/ 				SetVisArea( Rectangle( pPage->GetAllObjBoundRect() ) );
/*N*/ 		}
/*N*/ 
/*N*/ 		FinishedLoading( SFX_LOADED_ALL );
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*?*/ 		if( pStore->GetError() == ERRCODE_IO_BROKENPACKAGE )
/*?*/ 			SetError( ERRCODE_IO_BROKENPACKAGE );
/*?*/ 
/*?*/ 		pStore->SetError( SVSTREAM_WRONGVERSION );
/*N*/ 	}
/*N*/ 
/*N*/ 	// tell SFX to change viewshell when in preview mode
/*N*/ 	if( IsPreview() || bStartPresentation )
/*N*/ 	{
/*?*/ 		SfxItemSet *pSet = GetMedium()->GetItemSet();
/*?*/ 
/*?*/ 		if( pSet )
/*?*/ 			pSet->Put( SfxUInt16Item( SID_VIEW_ID, bStartPresentation ? 1 : 5 ) );
/*N*/ 	}
/*N*/ 
/*N*/ 	return bRet;
/*N*/ }

/*************************************************************************
|*
|* LoadFrom: Inhalte fuer Organizer laden
|*
\************************************************************************/


/*************************************************************************
|*
|* ConvertFrom: aus Fremdformat laden
|*
\************************************************************************/


/*************************************************************************
|*
|* Save: Pools und Dokument in die offenen Streams schreiben
|*
\************************************************************************/

/*N*/  BOOL SdDrawDocShell::Save()
/*N*/  {
/*N*/  	pDoc->StopWorkStartupDelay();
/*N*/  
/*N*/  	if( GetCreateMode() == SFX_CREATE_MODE_STANDARD )
/*N*/  		SvInPlaceObject::SetVisArea( Rectangle() );
/*N*/  
/*N*/  	BOOL bRet = SfxInPlaceObject::Save();
/*N*/  
/*N*/  	if( bRet )
/*N*/  	{
/*N*/  		SvStorage*	pStore = GetStorage();
/*N*/  		SfxMedium	aMedium( pStore );
/*N*/  		SdFilter*	pFilter = NULL;
/*N*/  
/*N*/  		if( pStore->GetVersion() >= SOFFICE_FILEFORMAT_60 )
/*N*/  			pFilter = new SdXMLFilter( aMedium, *this, sal_True );
/*N*/  		else
/*N*/  			pFilter = new SdBINFilter( aMedium, *this, sal_True );
/*N*/  
/*N*/  		// #86834# Call UpdateDocInfoForSave() before export
/*N*/  		UpdateDocInfoForSave();
/*N*/  
/*N*/  		bRet = pFilter ? pFilter->Export() : FALSE;
/*N*/  		delete pFilter;
/*N*/  	}
/*N*/  
/*N*/  	return bRet;
/*N*/  }

/*************************************************************************
|*
|* SaveAs: Pools und Dokument in den angegebenen Storage sichern
|*
\************************************************************************/

/*N*/ BOOL SdDrawDocShell::SaveAs( SvStorage* pStore )
/*N*/ {
/*N*/ 	pDoc->StopWorkStartupDelay();
/*N*/ 
/*N*/ 	if( GetCreateMode() == SFX_CREATE_MODE_STANDARD )
/*N*/ 		SvInPlaceObject::SetVisArea( Rectangle() );
/*N*/ 
/*N*/ 	UINT32	nVBWarning = ERRCODE_NONE;
/*N*/ 	BOOL	bRet = SfxInPlaceObject::SaveAs( pStore );
/*N*/ 
/*N*/ 	if( bRet )
/*N*/ 	{
/*N*/ 		SdFilter* pFilter = NULL;
/*N*/ 
/*N*/ 		if( pStore->GetVersion() >= SOFFICE_FILEFORMAT_60 )
/*N*/ 		{
/*N*/ 			SfxMedium aMedium( pStore );
/*N*/ 			pFilter = new SdXMLFilter( aMedium, *this, sal_True );
/*N*/ 
/*N*/ 			// #86834# Call UpdateDocInfoForSave() before export
/*N*/ 			UpdateDocInfoForSave();
/*N*/ 			
/*N*/ 			bRet = pFilter->Export();
/*N*/ 		}
/*N*/ 		else
/*N*/ 		{
/*N*/ 
/*N*/ 			SfxMedium aMedium( pStore );
/*N*/ 			pFilter = new SdBINFilter( aMedium, *this, sal_True );
/*N*/ 
/*N*/ 			// #86834# Call UpdateDocInfoForSave() before export
/*N*/ 			UpdateDocInfoForSave();
/*N*/ 
/*N*/ 			const ULONG	nOldSwapMode = pDoc->GetSwapGraphicsMode();
/*N*/ 			pDoc->SetSwapGraphicsMode( SDR_SWAPGRAPHICSMODE_TEMP );
/*N*/ 			if( !( bRet = pFilter->Export() ) )
/*?*/ 				pDoc->SetSwapGraphicsMode( nOldSwapMode );
/*N*/ 
/*N*/ 		}
/*N*/ 
/*N*/ 		delete pFilter;
/*N*/ 	}
/*N*/ 
/*N*/ 	if( GetError() == ERRCODE_NONE )
/*N*/ 		SetError( nVBWarning );
/*N*/ 
/*N*/ 	return bRet;
/*N*/ }

/*************************************************************************
|*
|* ConvertTo: im Fremdformat speichern
|*
\************************************************************************/


/*************************************************************************
|*
|* SaveCompleted: die eigenen Streams wieder oeffnen, damit kein anderer
|*								  sie "besetzt"
|*
\************************************************************************/

/*N*/ BOOL SdDrawDocShell::SaveCompleted( SvStorage * pStor )
/*N*/ {
/*N*/ 	BOOL bRet = FALSE;
/*N*/ 
/*N*/ 	if( SfxInPlaceObject::SaveCompleted(pStor) )
/*N*/ 	{
/*N*/ 		pDoc->NbcSetChanged( FALSE );
/*N*/ 
/*N*/ 		bRet = TRUE;
/*N*/ 
/*N*/         // throw away old streams
/*N*/ 	    if( pDoc )
/*N*/ 		    pDoc->HandsOff();
/*N*/ 	}
/*N*/ 	return bRet;
/*N*/ }

/*************************************************************************
|*
|*
|*
\************************************************************************/

/*N*/ void SdDrawDocShell::HandsOff()
/*N*/ {
/*N*/ 	SfxInPlaceObject::HandsOff();
/*N*/ 
/*N*/ #ifndef SVX_LIGHT
/*N*/     // throw away old streams
/*N*/ 	if( pDoc )
/*N*/ 		pDoc->HandsOff();
/*N*/ #endif
/*N*/ }

/*************************************************************************
|*
|* Referenz auf Dokument
|*
\************************************************************************/

/*N*/ SdDrawDocument* SdDrawDocShell::GetDoc()
/*N*/ {
/*N*/ 	return pDoc;
/*N*/ }

/*************************************************************************
|*
|* Referenz auf Dokument
|*
\************************************************************************/

/*N*/ SfxStyleSheetBasePool* SdDrawDocShell::GetStyleSheetPool()
/*N*/ {
/*N*/ 	return( (SfxStyleSheetBasePool*) pDoc->GetStyleSheetPool() );
/*N*/ }

/*************************************************************************
|*
|* Sprung zu Bookmark
|*
\************************************************************************/


/*************************************************************************
|*
|* SaveAsOwnFormat: wenn es eine Dokumentvorlage werden soll,
|*
\************************************************************************/
#ifndef _URLOBJ_HXX 
#include <tools/urlobj.hxx>
#endif

/*N*/ BOOL SdDrawDocShell::SaveAsOwnFormat( SfxMedium& rMedium )
/*N*/ {
/*N*/ 
/*N*/ 	const SfxFilter* pFilter = rMedium.GetFilter();
/*N*/ 
/*N*/ 	if (pFilter->IsOwnTemplateFormat())
/*N*/ 	{
/*N*/ 		// jetzt die StarDraw-Spezialitaeten:
/*N*/ 		// die Layoutvorlagen der ersten Seite werden mit dem jetzt
/*N*/ 		// bekannten Layoutnamen versehen, die Layoutnamen der betroffenen
/*N*/ 		// Masterpages und Seiten werden gesetzt;
/*N*/ 		// alle Textobjekte der betroffenen Standard-, Notiz- und
/*N*/ 		// Masterpages werden ueber die Namensaenderung informiert
/*N*/ 
/*N*/ 		String aLayoutName;
/*N*/ 
/*N*/ 		SfxStringItem* pLayoutItem;
/*N*/ 		if( rMedium.GetItemSet()->GetItemState(SID_TEMPLATE_NAME, FALSE, (const SfxPoolItem**) & pLayoutItem ) == SFX_ITEM_SET )
/*N*/ 		{
/*?*/ 			aLayoutName = pLayoutItem->GetValue();
/*N*/ 		}
/*N*/ 		else
/*N*/ 		{
/*N*/ 			INetURLObject aURL( rMedium.GetName() );
/*N*/ 			aURL.removeExtension();
/*N*/ 			aLayoutName = aURL.getName();
/*N*/ 		}
/*N*/ 
/*N*/ 		if( aLayoutName.Len() )
/*N*/ 		{
/*?*/ 			String aOldPageLayoutName = pDoc->GetSdPage(0, PK_STANDARD)->GetLayoutName();
/*?*/ 			pDoc->RenameLayoutTemplate(aOldPageLayoutName, aLayoutName);
/*N*/ 		}
/*N*/  	}
/*N*/ 
/*N*/ 	return SfxObjectShell::SaveAsOwnFormat(rMedium);
/*N*/ }

/*************************************************************************
|*
|* FillClass
|*
\************************************************************************/

/*N*/ void SdDrawDocShell::FillClass(SvGlobalName* pClassName,
/*N*/ 										ULONG*  pFormat,
/*N*/ 										String* pAppName,
/*N*/ 										String* pFullTypeName,
/*N*/ 										String* pShortTypeName,
/*N*/ 										long    nFileFormat) const
/*N*/ {
/*N*/ 	SfxInPlaceObject::FillClass(pClassName, pFormat, pAppName, pFullTypeName,
/*N*/ 								pShortTypeName, nFileFormat);
/*N*/ 
/*N*/ 	if (nFileFormat == SOFFICE_FILEFORMAT_31)
/*N*/ 	{
/*N*/ 		*pClassName = SvGlobalName(BF_SO3_SIMPRESS_CLASSID_30);
/*N*/ 		*pFormat = SOT_FORMATSTR_ID_STARDRAW;
/*N*/ 		*pAppName = String(RTL_CONSTASCII_USTRINGPARAM("Sdraw 3.1"));
/*N*/ 		*pFullTypeName = String(SdResId(STR_IMPRESS_DOCUMENT_FULLTYPE_31));;
/*N*/ 		*pShortTypeName = String(SdResId(STR_IMPRESS_DOCUMENT));
/*N*/ 	}
/*N*/ 	else if (nFileFormat == SOFFICE_FILEFORMAT_40)
/*N*/ 	{
/*N*/ 		*pClassName = SvGlobalName(BF_SO3_SIMPRESS_CLASSID_40);
/*N*/ 		*pFormat = SOT_FORMATSTR_ID_STARDRAW_40;
/*N*/ 		*pFullTypeName = String(SdResId(STR_IMPRESS_DOCUMENT_FULLTYPE_40));
/*N*/ 		*pShortTypeName = String(SdResId(STR_IMPRESS_DOCUMENT));
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*N*/ 		if (nFileFormat == SOFFICE_FILEFORMAT_50)
/*N*/ 		{
/*N*/ 			if (eDocType == DOCUMENT_TYPE_DRAW)
/*N*/ 			{
/*N*/ 				*pClassName = SvGlobalName(BF_SO3_SDRAW_CLASSID_50);
/*N*/ 				*pFormat = SOT_FORMATSTR_ID_STARDRAW_50;
/*N*/ 				*pFullTypeName = String(SdResId(STR_GRAPHIC_DOCUMENT_FULLTYPE_50));
/*N*/ 			}
/*N*/ 			else
/*N*/ 			{
/*N*/ 				*pClassName = SvGlobalName(BF_SO3_SIMPRESS_CLASSID_50);
/*N*/ 				*pFormat = SOT_FORMATSTR_ID_STARIMPRESS_50;
/*N*/ 				*pFullTypeName = String(SdResId(STR_IMPRESS_DOCUMENT_FULLTYPE_50));
/*N*/ 			}
/*N*/ 		}
/*N*/ 		else if (nFileFormat == SOFFICE_FILEFORMAT_CURRENT)
/*N*/ 		{
/*N*/ 			*pFullTypeName = String(SdResId( (eDocType == DOCUMENT_TYPE_DRAW) ?
/*N*/ 											  STR_GRAPHIC_DOCUMENT_FULLTYPE_60 : STR_IMPRESS_DOCUMENT_FULLTYPE_60 ));

				// for binfilter, we need the FormatIDs to be set. Not setting them
				// has always been an error (!)
				if(eDocType == DOCUMENT_TYPE_DRAW)
				{
					*pClassName = SvGlobalName(BF_SO3_SDRAW_CLASSID_60);
					*pFormat = SOT_FORMATSTR_ID_STARDRAW_60;
				}
				else
				{
					*pClassName = SvGlobalName(BF_SO3_SIMPRESS_CLASSID_60);
					*pFormat = SOT_FORMATSTR_ID_STARIMPRESS_60;
				}

/*N*/ 		}
/*N*/ 
/*N*/ 		*pShortTypeName = String(SdResId( (eDocType == DOCUMENT_TYPE_DRAW) ?
/*N*/ 										  STR_GRAPHIC_DOCUMENT : STR_IMPRESS_DOCUMENT ));
/*N*/ 	}
/*N*/ }




/*N*/ OutputDevice* SdDrawDocShell::GetDocumentRefDev (void)
/*N*/ {
/*N*/     OutputDevice* pReferenceDevice = SfxInPlaceObject::GetDocumentRefDev ();
/*N*/     // Only when our parent does not have a reference device then we return
/*N*/     // our own.
/*N*/     if (pReferenceDevice == NULL && pDoc != NULL)
/*N*/         pReferenceDevice = pDoc->GetRefDevice ();
/*N*/     return pReferenceDevice;
/*N*/ }
}
