/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmloff_XMLIndexTabStopEntryContext.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 02:13:58 $
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


#ifndef _XMLOFF_XMLINDEXTABSTOPENTRYCONTEXT_HXX_
#include "XMLIndexTabStopEntryContext.hxx"
#endif

#ifndef _XMLOFF_XMLINDEXTEMPLATECONTEXT_HXX_
#include "XMLIndexTemplateContext.hxx"
#endif


#ifndef _XMLOFF_XMLIMP_HXX
#include "xmlimp.hxx"
#endif


#ifndef _XMLOFF_NMSPMAP_HXX 
#include "nmspmap.hxx"
#endif

#ifndef _XMLOFF_XMLNMSPE_HXX
#include "xmlnmspe.hxx"
#endif


#ifndef _XMLOFF_XMLUCONV_HXX
#include "xmluconv.hxx"
#endif


#ifndef _TOOLS_DEBUG_HXX
#include <tools/debug.hxx>
#endif
namespace binfilter {


using namespace ::binfilter::xmloff::token;

using ::rtl::OUString;
using ::com::sun::star::uno::Sequence;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::Any;
using ::com::sun::star::beans::PropertyValue;
using ::com::sun::star::xml::sax::XAttributeList;


TYPEINIT1( XMLIndexTabStopEntryContext, XMLIndexSimpleEntryContext );

XMLIndexTabStopEntryContext::XMLIndexTabStopEntryContext(
	SvXMLImport& rImport, 
	XMLIndexTemplateContext& rTemplate,
	sal_uInt16 nPrfx,
	const OUString& rLocalName ) :
		XMLIndexSimpleEntryContext(rImport, rTemplate.sTokenTabStop, 
                                   rTemplate, nPrfx, rLocalName),
		sLeaderChar(),
		nTabPosition(0),
		bTabPositionOK(sal_False),
		bTabRightAligned(sal_False),
		bLeaderCharOK(sal_False)
{
}

XMLIndexTabStopEntryContext::~XMLIndexTabStopEntryContext()
{
}

void XMLIndexTabStopEntryContext::StartElement(
	const Reference<XAttributeList> & xAttrList)
{
	// process three attributes: type, position, leader char
	sal_Int16 nLength = xAttrList->getLength();
	for(sal_Int16 nAttr = 0; nAttr < nLength; nAttr++)
	{
		OUString sLocalName;
		sal_uInt16 nPrefix = GetImport().GetNamespaceMap().
			GetKeyByAttrName( xAttrList->getNameByIndex(nAttr), 
							  &sLocalName );
		OUString sAttr = xAttrList->getValueByIndex(nAttr);
		if (XML_NAMESPACE_STYLE == nPrefix)
		{
			if ( IsXMLToken( sLocalName, XML_TYPE ) )
			{
				// if it's neither left nor right, value is
				// ignored. Since left is default, we only need to
				// check for right
				bTabRightAligned = IsXMLToken( sAttr, XML_RIGHT );
			}
			else if ( IsXMLToken( sLocalName, XML_POSITION ) )
			{
				sal_Int32 nTmp;
				if (GetImport().GetMM100UnitConverter().
										convertMeasure(nTmp, sAttr))
				{
					nTabPosition = nTmp;
					bTabPositionOK = sal_True;
				}
			}
			else if ( IsXMLToken( sLocalName, XML_LEADER_CHAR ) )
			{
				sLeaderChar = sAttr;
				// valid only, if we have a char!
				bLeaderCharOK = (sAttr.getLength() > 0);
			}
			// else: unknown style: attribute -> ignore
		}
		// else: no style attribute -> ignore
	}

	// how many entries?
    nValues += 1 + (bTabPositionOK ? 1 : 0) + (bLeaderCharOK ? 1 : 0);

    // now try parent class (for character style)
    XMLIndexSimpleEntryContext::StartElement( xAttrList );
}

void XMLIndexTabStopEntryContext::FillPropertyValues(
    Sequence<PropertyValue> & rValues)
{
    // fill vlues from parent class (type + style name)
    XMLIndexSimpleEntryContext::FillPropertyValues(rValues);

    // get values array and next entry to be written;
	sal_Int32 nNextEntry = bCharStyleNameOK ? 2 : 1;
    PropertyValue* pValues = rValues.getArray();

	// right aligned?
	pValues[nNextEntry].Name = rTemplateContext.sTabStopRightAligned;
	pValues[nNextEntry].Value.setValue( &bTabRightAligned, 
                                        ::getBooleanCppuType());
	nNextEntry++;

	// position
	if (bTabPositionOK)
	{
		pValues[nNextEntry].Name = rTemplateContext.sTabStopPosition;
		pValues[nNextEntry].Value <<= nTabPosition;
		nNextEntry++;
	}

	// leader char
	if (bLeaderCharOK)
	{
		pValues[nNextEntry].Name = rTemplateContext.sTabStopFillCharacter;
		pValues[nNextEntry].Value <<= sLeaderChar;
		nNextEntry++;
	}

    // check whether we really filled all elements of the sequence
    DBG_ASSERT( nNextEntry == rValues.getLength(), 
                "length incorrectly precumputed!" );
}
}//end of namespace binfilter