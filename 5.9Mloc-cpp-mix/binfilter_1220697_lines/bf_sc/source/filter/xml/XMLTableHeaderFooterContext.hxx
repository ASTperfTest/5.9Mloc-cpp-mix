/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: XMLTableHeaderFooterContext.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/07 18:17:54 $
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
#ifndef _SC_XMLTABLEHEADERFOOTERCONTEXT_HXX_
#define _SC_XMLTABLEHEADERFOOTERCONTEXT_HXX_


#ifndef _XMLOFF_XMLICTXT_HXX_
#include <bf_xmloff/xmlictxt.hxx>
#endif

#ifndef _XMLOFF_XMLIMP_HXX_
#include <bf_xmloff/xmlimp.hxx>
#endif
#ifndef _COM_SUN_STAR_SHEET_XHEADERFOOTERCONTENT_HPP_
#include <com/sun/star/sheet/XHeaderFooterContent.hpp>
#endif
namespace binfilter {

namespace com { namespace sun { namespace star {
	namespace text { class XTextCursor; }
	namespace beans { class XPropertySet; }
} } }

class XMLTableHeaderFooterContext: public SvXMLImportContext
{
	::com::sun::star::uno::Reference <
		::com::sun::star::text::XTextCursor > xTextCursor;
	::com::sun::star::uno::Reference <
		::com::sun::star::text::XTextCursor > xOldTextCursor;
	::com::sun::star::uno::Reference <
		::com::sun::star::beans::XPropertySet > xPropSet;
	::com::sun::star::uno::Reference <
		::com::sun::star::sheet::XHeaderFooterContent > xHeaderFooterContent;

	const ::rtl::OUString	sOn;
	const ::rtl::OUString	sShareContent;
	const ::rtl::OUString	sContent;
	const ::rtl::OUString	sContentLeft;
	const ::rtl::OUString	sEmpty;
	::rtl::OUString			sCont;

	sal_Bool	bDisplay : 1;
	sal_Bool	bInsertContent : 1;
	sal_Bool 	bLeft : 1;
	sal_Bool	bContainsLeft : 1;
	sal_Bool	bContainsRight : 1;
	sal_Bool	bContainsCenter : 1;

public:
	TYPEINFO();

	XMLTableHeaderFooterContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
			const ::rtl::OUString& rLName,
		    const ::com::sun::star::uno::Reference<
					::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			const ::com::sun::star::uno::Reference <
		   			::com::sun::star::beans::XPropertySet > & rPageStylePropSet,
			   sal_Bool bFooter, sal_Bool bLft );

	virtual ~XMLTableHeaderFooterContext();

	virtual SvXMLImportContext *CreateChildContext(
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual void EndElement();
};

class XMLHeaderFooterRegionContext: public SvXMLImportContext
{
private:
	::com::sun::star::uno::Reference <
		::com::sun::star::text::XTextCursor >& xTextCursor;
	::com::sun::star::uno::Reference <
		::com::sun::star::text::XTextCursor > xOldTextCursor;

public:
	TYPEINFO();

	XMLHeaderFooterRegionContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
			const ::rtl::OUString& rLName,
		    const ::com::sun::star::uno::Reference<
					::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			::com::sun::star::uno::Reference< ::com::sun::star::text::XTextCursor >& xCursor );

	virtual ~XMLHeaderFooterRegionContext();

	virtual SvXMLImportContext *CreateChildContext(
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual void EndElement();
};


} //namespace binfilter
#endif
