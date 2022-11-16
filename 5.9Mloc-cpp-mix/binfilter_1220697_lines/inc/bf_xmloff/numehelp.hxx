/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: numehelp.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 18:52:54 $
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

#ifndef XMLOFF_NUMEHELP_HXX
#define XMLOFF_NUMEHELP_HXX

#ifndef _SAL_TYPES_H_
#include <sal/types.h>
#endif
#ifndef _COM_SUN_STAR_UTIL_XNUMBERFORMATSSUPPLIER_HPP_
#include <com/sun/star/util/XNumberFormatsSupplier.hpp>
#endif

#ifndef __SGI_STL_SET
#include <set>
#endif

namespace rtl
{
	class OUString;
}
namespace binfilter {
class SvXMLExport;

struct XMLNumberFormat
{
	::rtl::OUString	sCurrency;
	sal_Int32		nNumberFormat;
	sal_Int16		nType;
	sal_Bool		bIsStandard : 1;
	XMLNumberFormat() : nNumberFormat(0), nType(0) {}
	XMLNumberFormat(const ::rtl::OUString& sTempCurrency, sal_Int32 nTempFormat,
		sal_Int16 nTempType) : sCurrency(sTempCurrency), nNumberFormat(nTempFormat),
			nType(nTempType) {}
};

struct LessNumberFormat
{
	sal_Bool operator() (const XMLNumberFormat& rValue1, const XMLNumberFormat& rValue2) const
	{
		return rValue1.nNumberFormat < rValue2.nNumberFormat;
	}
};

typedef ::std::set<XMLNumberFormat, LessNumberFormat>	XMLNumberFormatSet;

class XMLNumberFormatAttributesExportHelper
{
	::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormats > xNumberFormats;
	SvXMLExport*		pExport;
    const ::rtl::OUString     sEmpty;
    const ::rtl::OUString sStandardFormat;
    const ::rtl::OUString sType;
    const ::rtl::OUString sAttrValueType;
    const ::rtl::OUString sAttrValue;
    const ::rtl::OUString sAttrDateValue;
    const ::rtl::OUString sAttrTimeValue;
    const ::rtl::OUString sAttrBooleanValue;
    const ::rtl::OUString sAttrStringValue;
    const ::rtl::OUString sAttrCurrency;
    XMLNumberFormatSet	aNumberFormats;
    sal_uInt16          nNamespace;
public :
	XMLNumberFormatAttributesExportHelper(::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier >& xNumberFormatsSupplier);
	XMLNumberFormatAttributesExportHelper(::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier >& xNumberFormatsSupplier,
                                            SvXMLExport& rExport, sal_uInt16 nNamespace);
	~XMLNumberFormatAttributesExportHelper();
    void SetExport(SvXMLExport* pExport) { this->pExport = pExport; }

	sal_Int16 GetCellType(const sal_Int32 nNumberFormat, ::rtl::OUString& sCurrency, sal_Bool& bIsStandard);

	static void WriteAttributes(SvXMLExport& rXMLExport,
								const sal_Int16 nTypeKey,
								const double& rValue,
								const ::rtl::OUString& rCurrencySymbol,
								sal_uInt16 nNamespace,
								sal_Bool bExportValue = sal_True);
	static sal_Bool GetCurrencySymbol(const sal_Int32 nNumberFormat, ::rtl::OUString& rCurrencySymbol,
		::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier > & xNumberFormatsSupplier);
	static sal_Int16 GetCellType(const sal_Int32 nNumberFormat, sal_Bool& bIsStandard,
		::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier > & xNumberFormatsSupplier);
	static void SetNumberFormatAttributes(SvXMLExport& rXMLExport,
										  const sal_Int32 nNumberFormat,
										  const double& rValue,
										  sal_uInt16 nNamespace,
										  sal_Bool bExportValue = sal_True);
	static void SetNumberFormatAttributes(SvXMLExport& rXMLExport,
										  const ::rtl::OUString& rValue,
										  const ::rtl::OUString& rCharacters,
										  sal_uInt16 nNamespace,
										  sal_Bool bExportValue = sal_True,
										  sal_Bool bExportTypeAttribute = sal_True);

    sal_Bool GetCurrencySymbol(const sal_Int32 nNumberFormat, ::rtl::OUString& rCurrencySymbol);
    sal_Int16 GetCellType(const sal_Int32 nNumberFormat, sal_Bool& bIsStandard);
    void WriteAttributes(const sal_Int16 nTypeKey,
                                          const double& rValue,
                                          const ::rtl::OUString& rCurrencySymbol,
                                          sal_Bool bExportValue = sal_True);
    void SetNumberFormatAttributes(const sal_Int32 nNumberFormat,
                                          const double& rValue,
                                          sal_Bool bExportValue = sal_True);
    void SetNumberFormatAttributes(const ::rtl::OUString& rValue,
                                          const ::rtl::OUString& rCharacters,
                                          sal_Bool bExportValue = sal_True,
                                          sal_Bool bExportTypeAttribute = sal_True);
};

}//end of namespace binfilter
#endif
