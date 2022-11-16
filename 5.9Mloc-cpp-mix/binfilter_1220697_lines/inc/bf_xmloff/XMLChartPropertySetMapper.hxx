/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: XMLChartPropertySetMapper.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 17:17:41 $
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
#ifndef _XMLOFF_CHARTPROPERTYSETMAPPER_HXX_
#define _XMLOFF_CHARTPROPERTYSETMAPPER_HXX_

#ifndef _UNIVERSALL_REFERENCE_HXX
#include "uniref.hxx"
#endif
#ifndef _XMLOFF_PROPERTYSETMAPPER_HXX
#include "xmlprmap.hxx"
#endif
#ifndef _XMLOFF_XMLEXPPR_HXX
#include "xmlexppr.hxx"
#endif
#ifndef _XMLOFF_XMLIMPPR_HXX
#include "xmlimppr.hxx"
#endif
#ifndef _XMLOFF_XMLIMP_HXX
#include "xmlimp.hxx"
#endif

namespace rtl { class OUString; }
namespace binfilter {

extern const XMLPropertyMapEntry aXMLChartPropMap[];

class SvXMLExport;

// ----------------------------------------

class XMLChartPropHdlFactory : public XMLPropertyHandlerFactory
{
private:
	const XMLPropertyHandler* GetShapePropertyHandler( sal_Int32 nType ) const;

public:
	virtual ~XMLChartPropHdlFactory();
	virtual const XMLPropertyHandler* GetPropertyHandler( sal_Int32 nType ) const;
};

// ----------------------------------------

class XMLChartPropertySetMapper : public XMLPropertySetMapper
{
public:
	XMLChartPropertySetMapper();
	~XMLChartPropertySetMapper();
};

// ----------------------------------------

class XMLChartExportPropertyMapper : public SvXMLExportPropertyMapper
{
private:
	const ::rtl::OUString msTrue;
	const ::rtl::OUString msFalse;

    SvXMLExport& mrExport;

protected:
	virtual void ContextFilter(
		::std::vector< XMLPropertyState >& rProperties,
		::com::sun::star::uno::Reference<
			::com::sun::star::beans::XPropertySet > rPropSet ) const;

private:
	/// this method is called for every item that has the MID_FLAG_ELEMENT_EXPORT flag set
	virtual void handleElementItem(
        SvXMLExport& rExport,
		const XMLPropertyState& rProperty, sal_uInt16 nFlags,
		const ::std::vector< XMLPropertyState > *pProperties = 0,
		sal_uInt32 nIdx = 0  ) const;

	/// this method is called for every item that has the MID_FLAG_SPECIAL_ITEM_EXPORT flag set
	virtual void handleSpecialItem(
		SvXMLAttributeList& rAttrList, const XMLPropertyState& rProperty,
		const SvXMLUnitConverter& rUnitConverter, const SvXMLNamespaceMap& rNamespaceMap,
		const ::std::vector< XMLPropertyState > *pProperties = 0,
		sal_uInt32 nIdx = 0  ) const;

public:
	XMLChartExportPropertyMapper( const UniReference< XMLPropertySetMapper >& rMapper,
                                  SvXMLExport& rExport );
	virtual ~XMLChartExportPropertyMapper();
};

// ----------------------------------------

class XMLChartImportPropertyMapper : public SvXMLImportPropertyMapper
{
private:
    SvXMLImport& mrImport;

public:
	XMLChartImportPropertyMapper( const UniReference< XMLPropertySetMapper >& rMapper,
                                  const SvXMLImport& rImport );
	virtual ~XMLChartImportPropertyMapper();

	virtual sal_Bool handleSpecialItem(
		XMLPropertyState& rProperty,
		::std::vector< XMLPropertyState >& rProperties,
		const ::rtl::OUString& rValue,
		const SvXMLUnitConverter& rUnitConverter,
		const SvXMLNamespaceMap& rNamespaceMap ) const;

	virtual void finished(
		::std::vector< XMLPropertyState >& rProperties,
		sal_Int32 nStartIndex, sal_Int32 nEndIndex ) const;
};

}//end of namespace binfilter
#endif	// _XMLOFF_CHARTPROPERTYSETMAPPER_HXX_