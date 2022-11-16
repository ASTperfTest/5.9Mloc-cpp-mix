/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_unofreg.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: obo $ $Date: 2007/07/17 12:12:19 $
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

#include <string.h>

#ifndef _COM_SUN_STAR_REGISTRY_XREGISTRYKEY_HPP_
#include <com/sun/star/registry/XRegistryKey.hpp>
#endif

#ifndef _OSL_DIAGNOSE_H_
#include <osl/diagnose.h>
#endif

#include <cppuhelper/factory.hxx>
#include <uno/lbnames.h>
#ifndef _SWXFILTEROPTIONS_HXX
#include <SwXFilterOptions.hxx>
#endif
namespace binfilter {

using namespace rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::lang;

// writer documents
extern uno::Sequence< OUString > SAL_CALL SwTextDocument_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwTextDocument_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwTextDocument_createInstance( const uno::Reference< XMultiServiceFactory > &rSMgr ) throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwWebDocument_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwWebDocument_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwWebDocument_createInstance( const uno::Reference< XMultiServiceFactory > &rSMgr ) throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwGlobalDocument_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwGlobalDocument_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwGlobalDocument_createInstance( const uno::Reference< XMultiServiceFactory > &rSMgr ) throw( uno::Exception );

// xml import
extern uno::Sequence< OUString > SAL_CALL SwXMLImport_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLImport_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLImport_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLImportStyles_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLImportStyles_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLImportStyles_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLImportContent_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLImportContent_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLImportContent_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );

extern uno::Sequence< OUString > SAL_CALL     SwXMLImportMeta_getSupportedServiceNames() throw();
extern OUString SAL_CALL     SwXMLImportMeta_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLImportMeta_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );

extern uno::Sequence< OUString > SAL_CALL SwXMLImportSettings_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLImportSettings_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLImportSettings_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );

// xml export
extern uno::Sequence< OUString > SAL_CALL SwXMLExport_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLExport_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLExport_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLExportContent_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLExportContent_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLExportContent_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLExportStyles_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLExportStyles_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLExportStyles_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLExportMeta_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLExportMeta_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLExportMeta_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );
extern uno::Sequence< OUString > SAL_CALL SwXMLExportSettings_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXMLExportSettings_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXMLExportSettings_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr)	throw( uno::Exception );

extern uno::Sequence< OUString > SAL_CALL SwXModule_getSupportedServiceNames() throw();
extern OUString SAL_CALL SwXModule_getImplementationName() throw();
extern uno::Reference< uno::XInterface > SAL_CALL SwXModule_createInstance(const uno::Reference< XMultiServiceFactory > & rSMgr) throw( uno::Exception );

//
#ifdef __cplusplus
extern "C"
{
#endif

void SAL_CALL component_getImplementationEnvironment(
		const sal_Char ** ppEnvTypeName,
		uno_Environment ** ppEnv )
{
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}

void SAL_CALL lcl_uno_writeInfo(
		registry::XRegistryKey * pRegistryKey,
		const OUString& rImplementationName,
		const uno::Sequence< OUString >& rServices )
{
	uno::Reference< registry::XRegistryKey > xNewKey(
		pRegistryKey->createKey(
			OUString( RTL_CONSTASCII_USTRINGPARAM("/") ) + rImplementationName + OUString(RTL_CONSTASCII_USTRINGPARAM( "/UNO/SERVICES") ) ) );

	for( sal_Int32 i = 0; i < rServices.getLength(); i++ )
		xNewKey->createKey( rServices.getConstArray()[i]);
}

sal_Bool SAL_CALL component_writeInfo(
		void * pServiceManager,
		void * pRegistryKey )
{
	if( pRegistryKey )
	{
		try
		{
			registry::XRegistryKey *pKey =
				reinterpret_cast< registry::XRegistryKey * >( pRegistryKey );


			// xml filter
			lcl_uno_writeInfo( pKey, SwXMLImport_getImplementationName(),
							   SwXMLImport_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLImportStyles_getImplementationName(),
							   SwXMLImportStyles_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey,SwXMLImportContent_getImplementationName(),
							   SwXMLImportContent_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLImportMeta_getImplementationName(),
							   SwXMLImportMeta_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLExport_getImplementationName(),
							   SwXMLExport_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLExportStyles_getImplementationName(),
							   SwXMLExportStyles_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey,SwXMLExportContent_getImplementationName(),
							   SwXMLExportContent_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLExportMeta_getImplementationName(),
							   SwXMLExportMeta_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLExportSettings_getImplementationName(),
							   SwXMLExportSettings_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwXMLImportSettings_getImplementationName(),
							   SwXMLImportSettings_getSupportedServiceNames() );
            //Filter options
            lcl_uno_writeInfo( pKey, SwXFilterOptions::getImplementationName_Static(),
                               SwXFilterOptions::getSupportedServiceNames_Static() );

			// documents
			lcl_uno_writeInfo( pKey, SwTextDocument_getImplementationName(),
							   SwTextDocument_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey, SwWebDocument_getImplementationName(),
							   SwWebDocument_getSupportedServiceNames() );
			lcl_uno_writeInfo( pKey,SwGlobalDocument_getImplementationName(),
							   SwGlobalDocument_getSupportedServiceNames() );

        }
		catch (registry::InvalidRegistryException &)
		{
			OSL_ENSURE( sal_False, "### InvalidRegistryException!" );
		}
	}
    return sal_True;
}

void * SAL_CALL component_getFactory( const sal_Char * pImplName,
									  void * pServiceManager,
									  void * pRegistryKey )
{
	void * pRet = 0;
	if( pServiceManager )
	{
		uno::Reference< XMultiServiceFactory > xMSF(
			reinterpret_cast< XMultiServiceFactory * >( pServiceManager ) );

		uno::Reference< XSingleServiceFactory > xFactory;

		const sal_Int32 nImplNameLen = strlen( pImplName );
		if( SwXMLImport_getImplementationName().equalsAsciiL( pImplName,
															  nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLImport_getImplementationName(),
				SwXMLImport_createInstance,
				SwXMLImport_getSupportedServiceNames() );
		}
		else if( SwXMLImportStyles_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLImportStyles_getImplementationName(),
				SwXMLImportStyles_createInstance,
				SwXMLImportStyles_getSupportedServiceNames() );
		}
		else if( SwXMLImportContent_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLImportContent_getImplementationName(),
				SwXMLImportContent_createInstance,
				SwXMLImportContent_getSupportedServiceNames() );
		}
		else if( SwXMLImportMeta_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLImportMeta_getImplementationName(),
				SwXMLImportMeta_createInstance,
				SwXMLImportMeta_getSupportedServiceNames() );
		}
		else if( SwXMLExport_getImplementationName().equalsAsciiL( pImplName,
															nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLExport_getImplementationName(),
				SwXMLExport_createInstance,
				SwXMLExport_getSupportedServiceNames() );
		}
		else if( SwXMLExportStyles_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLExportStyles_getImplementationName(),
				SwXMLExportStyles_createInstance,
				SwXMLExportStyles_getSupportedServiceNames() );
		}
		else if( SwXMLExportContent_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLExportContent_getImplementationName(),
				SwXMLExportContent_createInstance,
				SwXMLExportContent_getSupportedServiceNames() );
		}
		else if( SwXMLExportMeta_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLExportMeta_getImplementationName(),
				SwXMLExportMeta_createInstance,
				SwXMLExportMeta_getSupportedServiceNames() );
		}
		else if( SwXMLExportSettings_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLExportSettings_getImplementationName(),
				SwXMLExportSettings_createInstance,
				SwXMLExportSettings_getSupportedServiceNames() );
		}
		else if( SwXMLImportSettings_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SwXMLImportSettings_getImplementationName(),
				SwXMLImportSettings_createInstance,
				SwXMLImportSettings_getSupportedServiceNames() );
		}
        else if( SwXModule_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
                SwXModule_getImplementationName(),
                SwXModule_createInstance,
                SwXModule_getSupportedServiceNames() );
		}
        else if( SwXFilterOptions::getImplementationName_Static().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
                SwXFilterOptions::getImplementationName_Static(),
                SwXFilterOptions_createInstance,
                SwXFilterOptions::getSupportedServiceNames_Static() );
		}
        else if( SwTextDocument_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
                SwTextDocument_getImplementationName(),
                SwTextDocument_createInstance,
                SwTextDocument_getSupportedServiceNames() );
		}
        else if( SwWebDocument_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
                SwWebDocument_getImplementationName(),
                SwWebDocument_createInstance,
                SwWebDocument_getSupportedServiceNames() );
		}
        else if( SwGlobalDocument_getImplementationName().equalsAsciiL(
													pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
                SwGlobalDocument_getImplementationName(),
                SwGlobalDocument_createInstance,
                SwGlobalDocument_getSupportedServiceNames() );
		}

        if( xFactory.is())
		{
			xFactory->acquire();
			pRet = xFactory.get();
		}
	}
	return pRet;
}

#ifdef __cplusplus
}
#endif
}