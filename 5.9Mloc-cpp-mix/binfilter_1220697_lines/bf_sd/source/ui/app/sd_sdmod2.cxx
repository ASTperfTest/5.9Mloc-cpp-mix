/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_sdmod2.cxx,v $
 *
 *  $Revision: 1.9 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:22:23 $
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

#ifndef _EEITEM_HXX //autogen
#include <bf_svx/eeitem.hxx>
#endif

#define ITEMID_FIELD    EE_FEATURE_FIELD


#ifndef _SFXPOOLITEM_HXX
#include <svtools/poolitem.hxx>
#endif

#ifndef _SVX_ITEMDATA_HXX
#include <bf_svx/itemdata.hxx>
#endif

#ifndef _DATE_HXX
#include <tools/date.hxx>
#endif

#ifndef _TOOLS_TIME_HXX
#include <tools/time.hxx>
#endif

#ifndef _INETHIST_HXX //autogen
#include <svtools/inethist.hxx>
#endif
#ifndef _SFXPOOLITEM_HXX //autogen
#include <svtools/poolitem.hxx>
#endif
#ifndef _SFXDOCFILE_HXX //autogen
#include <bf_sfx2/docfile.hxx>
#endif

#include <bf_svx/svdfield.hxx>

#define ITEMID_SPELLCHECK   0
#include <bf_svx/dialogs.hrc>


#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifndef _OFF_OFAIDS_HRC
#include <bf_offmgr/ofaids.hrc>
#endif

#define _SD_DLL                 // fuer SD_MOD()
#include "app.hrc"
#include "glob.hrc"
#include "strings.hrc"
#include "res_bmp.hrc"
#include "frmview.hxx"
#include "drawdoc.hxx"
#include "sdoutl.hxx"
#include "sdmod.hxx"
#include "docshell.hxx"

namespace binfilter {


/*************************************************************************
|*
|* Options-Dialog
|*
\************************************************************************/



/*************************************************************************
|*
|* Link fuer CalcFieldValue des Outliners
|*
\************************************************************************/

/*N*/ IMPL_LINK(SdModule, CalcFieldValueHdl, EditFieldInfo*, pInfo)
/*N*/ {
/*N*/ 	if (pInfo)
/*N*/ 	{
/*N*/ 		const SvxFieldData* pField = pInfo->GetField().GetField();
/*N*/         SdDrawDocShell*     pDocShell = NULL;
/*N*/ 
/*N*/         if( pInfo->GetOutliner() )
/*N*/         {
/*N*/             const SdrTextObj* pTextObj = static_cast< SdrOutliner* >( pInfo->GetOutliner() )->GetTextObj();
/*N*/ 
/*N*/             if( pTextObj && pTextObj->GetModel() && pTextObj->GetModel()->ISA( SdDrawDocument ) )
/*N*/                 pDocShell = static_cast< SdDrawDocument* >( pTextObj->GetModel() )->GetDocSh();
/*N*/         }
/*N*/ 
/*N*/         if( !pDocShell )
/*N*/             pDocShell = PTR_CAST( SdDrawDocShell, SfxObjectShell::Current() );
/*N*/ 
/*N*/ 		if (pField && pField->ISA(SvxDateField))
/*N*/ 		{
			/******************************************************************
			* Date-Field
			******************************************************************/
/*N*/ 			LanguageType eLang = pInfo->GetOutliner()->GetLanguage( pInfo->GetPara(), pInfo->GetPos() );
/*N*/ 			pInfo->SetRepresentation( ((const SvxDateField*) pField)->GetFormatted( *GetNumberFormatter(), eLang ) );
/*N*/ 		}
/*N*/ 		else if( pField && pField->ISA( SvxExtTimeField ) )
		{
 			/******************************************************************
 			* Time-Field
 			******************************************************************/
/*N*/			LanguageType eLang = pInfo->GetOutliner()->GetLanguage( pInfo->GetPara(), pInfo->GetPos() );
/*N*/ 			pInfo->SetRepresentation( ( (const SvxExtTimeField*) pField)->GetFormatted( *GetNumberFormatter(), eLang ) );
		}
/*N*/ 		else if( pField && pField->ISA( SvxExtFileField ) )
		{
 			/******************************************************************
 			* File-Field
 			******************************************************************/
				/*N*/ const SvxExtFileField* pFileField = (const SvxExtFileField*) pField;
/*N*/ 			String aFile;
/*N*/ 			if( pFileField->GetType() == SVXFILETYPE_FIX )
/*N*/ 				aFile =  pFileField->GetFormatted();
/*N*/ 			else if( pDocShell )
/*N*/ 			{
/*N*/ 				String aName;
/*N*/ 				if( pDocShell->HasName() )
/*N*/ 					aName = pDocShell->GetMedium()->GetName();
/*N*/ 				else
/*N*/ 					aName = pDocShell->GetName();
/*N*/ 
/*N*/                 // #92496# Set new content also for living field
/*N*/ 				const_cast< SvxExtFileField* >(pFileField)->SetFile( aName );
/*N*/ 				aFile = pFileField->GetFormatted();
/*N*/ 			}

/*N*/ 			pInfo->SetRepresentation( aFile );
/*N*/ 
		}
/*N*/ 		else if( pField && pField->ISA( SvxAuthorField ) )
		{
	 			/******************************************************************
	 			* Author-Field
	 			******************************************************************/
/*N*/			const SvxAuthorField* pAuthorField = (const SvxAuthorField*) pField;
/*N*/ 			String aAuthor;
/*N*/ 			if( pAuthorField->GetType() == SVXAUTHORTYPE_FIX )
/*N*/ 				aAuthor = pAuthorField->GetFormatted();
/*N*/ 			else
/*N*/ 			{
					aAuthor = String( RTL_CONSTASCII_USTRINGPARAM( "<author>" ) );
/*N*/ 			}
/*N*/ 			pInfo->SetRepresentation( aAuthor );
/*N*/ 
		}
/*N*/ 		else if( pField && pField->ISA( SvxPageField ) )
		{
	 			/******************************************************************
	 			* Page-Field
	 			******************************************************************/
/*N*/ 			String aRepresentation;
/*N*/ 			aRepresentation += sal_Unicode( ' ' );
/*N*/ 			pInfo->SetRepresentation( aRepresentation );
		}
/*N*/ 		else if (pField && pField->ISA(SvxURLField))
/*N*/ 		{
			/******************************************************************
			* URL-Field
			******************************************************************/

/*N*/  switch ( ((const SvxURLField*) pField)->GetFormat() )
/*N*/ 			{
/*N*/ 				case SVXURLFORMAT_APPDEFAULT: //!!! einstellbar an App???
/*N*/ 				case SVXURLFORMAT_REPR:
/*N*/ 				{
/*N*/ 					pInfo->SetRepresentation(
/*N*/ 						   ((const SvxURLField*)pField)->GetRepresentation() );
/*N*/ 				}
/*N*/ 				break;
/*N*/ 
/*N*/ 				case SVXURLFORMAT_URL:
/*N*/ 				{
/*N*/ 					pInfo->SetRepresentation(
/*N*/ 						   ((const SvxURLField*)pField)->GetURL() );
/*N*/ 				}
/*N*/ 				break;
/*N*/ 			}
/*N*/ 
/*N*/ 			String aURL = ((const SvxURLField*)pField)->GetURL();
/*N*/ 
/*N*/             svtools::ColorConfig aConfig;
/*N*/             svtools::ColorConfigEntry eEntry =
/*N*/                 INetURLHistory::GetOrCreate()->QueryUrl( aURL ) ? svtools::LINKSVISITED : svtools::LINKS;
/*N*/ 			pInfo->SetTxtColor( aConfig.GetColorValue(eEntry).nColor );
/*N*/ 		}
/*N*/ 		else if (pField && pField->ISA(SdrMeasureField))
		{
			/******************************************************************
			* Measure-Field
			******************************************************************/
/*N*/ 			pInfo->ClearFldColor();
/*N*/ 		}
/*N*/ 		else
		{
/*?*/ 			DBG_ERROR("unbekannter Feldbefehl");
/*?*/ 			String aStr;
/*?*/ 			aStr += sal_Unicode( '?' );
/*?*/ 			pInfo->SetRepresentation( aStr );
/*?*/ 		}
/*?*/ 	}
/*N*/ 
/*N*/ 	return(0);
/*N*/ }



/*************************************************************************
|*
|* virt. Methoden fuer Optionendialog
|*
\************************************************************************/
}
