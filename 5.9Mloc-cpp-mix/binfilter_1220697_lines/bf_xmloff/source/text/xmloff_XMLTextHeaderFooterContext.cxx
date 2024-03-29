/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmloff_XMLTextHeaderFooterContext.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 02:17:42 $
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

#ifndef _COM_SUN_STAR_TEXT_XRELATIVETEXTCONTENTREMOVE_HPP_ 
#include <com/sun/star/text/XRelativeTextContentRemove.hpp>
#endif
#ifndef _XMLOFF_TEXTHEADERFOOTERCONTEXT_HXX_
#include "XMLTextHeaderFooterContext.hxx"
#endif
#ifndef _XMLOFF_XMLIMP_HXX
#include "xmlimp.hxx"
#endif
namespace binfilter {


using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::xml::sax;
//using namespace ::com::sun::star::style;
using namespace ::com::sun::star::text;
using namespace ::com::sun::star::beans;
//using namespace ::com::sun::star::container;
//using namespace ::com::sun::star::lang;
//using namespace ::com::sun::star::text;


TYPEINIT1( XMLTextHeaderFooterContext, SvXMLImportContext );

XMLTextHeaderFooterContext::XMLTextHeaderFooterContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
					   const OUString& rLName,
					   const uno::Reference<
							xml::sax::XAttributeList > & xAttrList,
						const Reference < XPropertySet > & rPageStylePropSet,
					   sal_Bool bFooter, sal_Bool bLft ) :
	SvXMLImportContext( rImport, nPrfx, rLName ),
	xPropSet( rPageStylePropSet ),
	sOn( OUString::createFromAscii( bFooter ? "FooterIsOn" : "HeaderIsOn" ) ),
	sShareContent( OUString::createFromAscii( bFooter ? "FooterIsShared"
													  : "HeaderIsShared" ) ),
	sText( OUString::createFromAscii( bFooter ? "FooterText" : "HeaderText" ) ),
	sTextLeft( OUString::createFromAscii( bFooter ? "FooterTextLeft"
													 : "HeaderTextLeft" ) ),
	bInsertContent( sal_True ),
	bLeft( bLft )
{
	if( bLeft )
	{
		Any aAny;

		aAny = xPropSet->getPropertyValue( sOn );
		sal_Bool bOn = *(sal_Bool *)aAny.getValue();

		if( bOn )
		{
			aAny = xPropSet->getPropertyValue( sShareContent );
			sal_Bool bShared = *(sal_Bool *)aAny.getValue();
			if( bShared )
			{
				// Don't share headers any longer
				bShared = sal_False;
				aAny.setValue( &bShared, ::getBooleanCppuType() );
				xPropSet->setPropertyValue( sShareContent, aAny );
			}
		}
		else
		{
			// If headers or footers are switched off, no content must be
			// inserted.
			bInsertContent = sal_False;
		}
	}
}

XMLTextHeaderFooterContext::~XMLTextHeaderFooterContext()
{
}

SvXMLImportContext *XMLTextHeaderFooterContext::CreateChildContext(
	sal_uInt16 nPrefix,
	const OUString& rLocalName,
	const uno::Reference< xml::sax::XAttributeList > & xAttrList )
{
	SvXMLImportContext *pContext = 0;
	if( bInsertContent )
	{
		if( !xOldTextCursor.is() )
		{
			sal_Bool bRemoveContent = sal_True;
			Any aAny;
			if( bLeft )
			{
				// Headers and footers are switched on already,
				// and they aren't shared.
				aAny = xPropSet->getPropertyValue( sTextLeft );
			}
			else
			{
				aAny = xPropSet->getPropertyValue( sOn );
				sal_Bool bOn = *(sal_Bool *)aAny.getValue();

				if( !bOn )
				{
					// Switch header on
					bOn = sal_True;
					aAny.setValue( &bOn, ::getBooleanCppuType() );
					xPropSet->setPropertyValue( sOn, aAny );

					// The content has not to be removed, because the header
					// or footer is empty already.
					bRemoveContent;
				}

				// If a header or footer is not shared, share it now.
				aAny = xPropSet->getPropertyValue( sShareContent );
				sal_Bool bShared = *(sal_Bool *)aAny.getValue();
				if( !bShared )
				{
					bShared = sal_True;
					aAny.setValue( &bShared, ::getBooleanCppuType() );
					xPropSet->setPropertyValue( sShareContent, aAny );
				}

				aAny = xPropSet->getPropertyValue( sText );
			}

			Reference < XText > xText;
			aAny >>= xText;

			if( bRemoveContent )
			{
				OUString sText;
				xText->setString( sText );
			}

			UniReference < XMLTextImportHelper > xTxtImport =
				GetImport().GetTextImport();

			xOldTextCursor = xTxtImport->GetCursor();
			xTxtImport->SetCursor( xText->createTextCursor() );
		}
		
		pContext = 
			GetImport().GetTextImport()->CreateTextChildContext(
				GetImport(), nPrefix, rLocalName, xAttrList,
				XML_TEXT_TYPE_HEADER_FOOTER );
	}
	if( !pContext )
		pContext = new SvXMLImportContext( GetImport(), nPrefix, rLocalName );
	
	return pContext;
}

void XMLTextHeaderFooterContext::EndElement()
{
	if( xOldTextCursor.is() )
	{
		GetImport().GetTextImport()->DeleteParagraph();
		GetImport().GetTextImport()->SetCursor( xOldTextCursor );
	}
	else if( !bLeft )
	{
		// If no content has been inserted inro the header or footer,
		// switch it off.
		sal_Bool bOn = sal_False;
		Any aAny;
		aAny.setValue( &bOn, ::getBooleanCppuType() );
		xPropSet->setPropertyValue( sOn, aAny );
	}
}

}//end of namespace binfilter
