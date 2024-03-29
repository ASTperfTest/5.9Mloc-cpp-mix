/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sc_xmlannoi.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 10:56:51 $
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

#ifdef PCH
#endif

#ifdef _MSC_VER
#pragma hdrstop
#endif

// INCLUDE ---------------------------------------------------------------

#include "xmlannoi.hxx"
#include "xmlimprt.hxx"
#include "xmlcelli.hxx"
#ifndef SC_XMLCONTI_HXX
#include "xmlconti.hxx"
#endif

#include <bf_xmloff/nmspmap.hxx>
#ifndef _XMLOFF_XMLNMSPE_HXX
#include <bf_xmloff/xmlnmspe.hxx>
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace xmloff::token;

//------------------------------------------------------------------

ScXMLAnnotationContext::ScXMLAnnotationContext( ScXMLImport& rImport,
									  USHORT nPrfx,
									  const ::rtl::OUString& rLName,
									  const uno::Reference<xml::sax::XAttributeList>& xAttrList,
									  ScXMLTableRowCellContext* pTempCellContext) :
	SvXMLImportContext( rImport, nPrfx, rLName ),
	nParagraphCount(0),
	bDisplay(sal_False),
	bHasTextP(sal_False)
{
	pCellContext = pTempCellContext;
	sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
	const SvXMLTokenMap& rAttrTokenMap = GetScImport().GetTableAnnotationAttrTokenMap();
	for( sal_Int16 i=0; i < nAttrCount; i++ )
	{
		::rtl::OUString sAttrName = xAttrList->getNameByIndex( i );
		::rtl::OUString aLocalName;
		USHORT nPrefix = GetScImport().GetNamespaceMap().GetKeyByAttrName(
											sAttrName, &aLocalName );
		::rtl::OUString sValue = xAttrList->getValueByIndex( i );

		switch( rAttrTokenMap.Get( nPrefix, aLocalName ) )
		{
			case XML_TOK_TABLE_ANNOTATION_ATTR_AUTHOR:
			{
				sAuthor = sValue;
			}
			break;
			case XML_TOK_TABLE_ANNOTATION_ATTR_CREATE_DATE:
			{
				sCreateDate = sValue;
			}
			break;
			case XML_TOK_TABLE_ANNOTATION_ATTR_CREATE_DATE_STRING:
			{
				if (!sCreateDate.getLength())
					sCreateDate = sValue;
			}
			break;
			case XML_TOK_TABLE_ANNOTATION_ATTR_DISPLAY:
			{
				bDisplay = IsXMLToken(sValue, XML_TRUE);
			}
			break;
		}
	}
}

ScXMLAnnotationContext::~ScXMLAnnotationContext()
{
}

SvXMLImportContext *ScXMLAnnotationContext::CreateChildContext( USHORT nPrefix,
											const ::rtl::OUString& rLName,
											const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList )
{
	SvXMLImportContext *pContext = 0;

	if ((nPrefix == XML_NAMESPACE_TEXT) && IsXMLToken(rLName, XML_P) )
	{
		if (!bHasTextP)
		{
			bHasTextP = sal_True;
			sOUText.setLength(0);
		}
		if(nParagraphCount)
			sOUText.append(static_cast<sal_Unicode>('\n'));
		nParagraphCount++;
		pContext = new ScXMLContentContext( GetScImport(), nPrefix, rLName, xAttrList, sOUText);
	}

	if( !pContext )
		pContext = new SvXMLImportContext( GetImport(), nPrefix, rLName );

	return pContext;
}

void ScXMLAnnotationContext::Characters( const ::rtl::OUString& rChars )
{
	if (!bHasTextP)
		sOUText.append(rChars);
}

void ScXMLAnnotationContext::EndElement()
{
	ScMyImportAnnotation* pMyAnnotation = new ScMyImportAnnotation();
	pMyAnnotation->sAuthor = sAuthor;
	pMyAnnotation->sCreateDate = sCreateDate;
	pMyAnnotation->sText = sOUText.makeStringAndClear();
	pMyAnnotation->bDisplay = bDisplay;
	pCellContext->AddAnnotation(pMyAnnotation);
}

}
