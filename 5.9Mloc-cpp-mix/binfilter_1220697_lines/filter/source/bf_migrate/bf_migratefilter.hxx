/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: bf_migratefilter.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 10:41:34 $
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

#ifndef _BF_MIGRATEFILTER_HXX
#define _BF_MIGRATEFILTER_HXX

#ifndef _COM_SUN_STAR_DOCUMENT_XFILTER_HPP_
#include <com/sun/star/document/XFilter.hpp>
#endif

#ifndef _COM_SUN_STAR_DOCUMENT_XEXPORTER_HPP_
#include <com/sun/star/document/XExporter.hpp>
#endif

#ifndef _COM_SUN_STAR_DOCUMENT_XIMPORTER_HPP_
#include <com/sun/star/document/XImporter.hpp>
#endif

#ifndef _COM_SUN_STAR_LANG_XINITIALIZATION_HPP_
#include <com/sun/star/lang/XInitialization.hpp>
#endif

#ifndef _COM_SUN_STAR_LANG_XSERVICEINFO_HPP_
#include <com/sun/star/lang/XServiceInfo.hpp>
#endif

#ifndef _CPPUHELPER_IMPLBASE5_HXX_
#include <cppuhelper/implbase5.hxx>
#endif
namespace binfilter {

enum FilterType 
{
	FILTER_IMPORT,
	FILTER_EXPORT
};

/* This component will be instantiated for both import or export. Whether it calls
 * setSourceDocument or setTargetDocument determines which Impl function the filter
 * member calls */

class bf_MigrateFilter : public cppu::WeakImplHelper5 
< 
	com::sun::star::document::XFilter,
	com::sun::star::document::XExporter,
	com::sun::star::document::XImporter,
	com::sun::star::lang::XInitialization,
	com::sun::star::lang::XServiceInfo
>
{
protected:
    ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > mxMSF;
	::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent > mxDoc;
	::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > mxLegServFact;
	::rtl::OUString msFilterName;
	FilterType meType;

    sal_Bool exportImpl(const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aDescriptor) 
		throw (::com::sun::star::uno::RuntimeException);

	sal_Bool importImpl(const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aDescriptor) 
		throw (::com::sun::star::uno::RuntimeException);

	sal_Bool getContactToLegacyProcessServiceFactory()
		throw (::com::sun::star::uno::RuntimeException);

public:
	bf_MigrateFilter(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > &rxMSF)
	:	mxMSF(rxMSF) 
	{
	}

	virtual ~bf_MigrateFilter() 
	{
	}

	// XFilter
    virtual sal_Bool SAL_CALL filter(const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aDescriptor) 
		throw (::com::sun::star::uno::RuntimeException);

	virtual void SAL_CALL cancel() 
		throw (::com::sun::star::uno::RuntimeException);

	// XExporter
    virtual void SAL_CALL setSourceDocument(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& xDoc) 
		throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

	// XImporter
    virtual void SAL_CALL setTargetDocument(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& xDoc) 
		throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

	// XInitialization
    virtual void SAL_CALL initialize(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& aArguments) 
		throw (::com::sun::star::uno::Exception, ::com::sun::star::uno::RuntimeException);

	// XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName() 
		throw (::com::sun::star::uno::RuntimeException);

	virtual sal_Bool SAL_CALL supportsService(const ::rtl::OUString& ServiceName) 
		throw (::com::sun::star::uno::RuntimeException);

	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames() 
		throw (::com::sun::star::uno::RuntimeException);
};

::rtl::OUString bf_MigrateFilter_getImplementationName()
	throw ( ::com::sun::star::uno::RuntimeException );

sal_Bool SAL_CALL bf_MigrateFilter_supportsService(const ::rtl::OUString& ServiceName) 
	throw ( ::com::sun::star::uno::RuntimeException );

::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL bf_MigrateFilter_getSupportedServiceNames() 
	throw ( ::com::sun::star::uno::RuntimeException );

::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >
	SAL_CALL bf_MigrateFilter_createInstance(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > & rSMgr)
	throw ( ::com::sun::star::uno::Exception );

}//end of namespace binfilter
#endif // _BF_MIGRATEFILTER_HXX
// eof