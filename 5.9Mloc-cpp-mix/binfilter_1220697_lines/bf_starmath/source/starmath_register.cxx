/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: starmath_register.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 04:04:26 $
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

#ifndef _COM_SUN_STAR_LANG_XSERVICEINFO_HPP_
#include <com/sun/star/lang/XServiceInfo.hpp>
#endif
#ifndef _COM_SUN_STAR_REGISTRY_XREGISTRYKEY_HPP_
#include <com/sun/star/registry/XRegistryKey.hpp>
#endif
#ifndef _COM_SUN_STAR_UNO_SEQUENCE_H_
#include <com/sun/star/uno/Sequence.h>
#endif
#ifndef _RTL_USTRING_HXX_
#include <rtl/ustring.hxx>
#endif

#include "smdll.hxx"
#include "document.hxx"

#ifndef _CPPUHELPER_FACTORY_HXX_ 
#include <cppuhelper/factory.hxx>
#endif

namespace binfilter {

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;

//Math document
extern Sequence< OUString > SAL_CALL    
        SmDocument_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmDocument_getImplementationName() throw();
extern Reference< XInterface >SAL_CALL 
        SmDocument_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );

//MathML import
extern Sequence< OUString > SAL_CALL
        SmXMLImport_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLImport_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLImport_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );
extern Sequence< OUString > SAL_CALL
        SmXMLImportMeta_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLImportMeta_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLImportMeta_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );
extern Sequence< OUString > SAL_CALL
        SmXMLImportSettings_getSupportedServiceNames() throw();
extern OUString SAL_CALL SmXMLImportSettings_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLImportSettings_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );

//MathML export
extern Sequence< OUString > SAL_CALL
        SmXMLExport_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLExport_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLExport_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );
extern Sequence< OUString > SAL_CALL
        SmXMLExportMeta_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLExportMeta_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLExportMeta_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );
extern Sequence< OUString > SAL_CALL
        SmXMLExportSettings_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLExportSettings_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLExportSettings_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );
extern Sequence< OUString > SAL_CALL
        SmXMLExportContent_getSupportedServiceNames() throw();
extern OUString SAL_CALL 
        SmXMLExportContent_getImplementationName() throw();
extern Reference< XInterface > SAL_CALL 
        SmXMLExportContent_createInstance(const Reference< XMultiServiceFactory > & rSMgr) throw( Exception );


extern "C" {

void SAL_CALL component_getImplementationEnvironment(
        const  sal_Char**   ppEnvironmentTypeName,
        uno_Environment**   ppEnvironment           )
{
	*ppEnvironmentTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME ;
}

sal_Bool SAL_CALL component_writeInfo(	void*	pServiceManager	,
										void*	pRegistryKey	)
{
    Reference< registry::XRegistryKey >
            xKey( reinterpret_cast< registry::XRegistryKey* >( pRegistryKey ) ) ;

    OUString aDelimiter( RTL_CONSTASCII_USTRINGPARAM("/") );
    OUString aUnoServices( RTL_CONSTASCII_USTRINGPARAM( "/UNO/SERVICES") );

    // Eigentliche Implementierung und ihre Services registrieren
	sal_Int32 i;
    Reference< registry::XRegistryKey >  xNewKey;

    xNewKey = xKey->createKey( aDelimiter + SmXMLImport_getImplementationName() +
                               aUnoServices );

    Sequence< OUString > aServices = SmXMLImport_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLExport_getImplementationName() +
                               aUnoServices );

    aServices = SmXMLExport_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLImportMeta_getImplementationName() +
                               aUnoServices );

    aServices = SmXMLImportMeta_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLExportMeta_getImplementationName() +
                               aUnoServices );

    aServices = SmXMLExportMeta_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLImportSettings_getImplementationName() + 
                               aUnoServices );

    aServices = SmXMLImportSettings_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLExportSettings_getImplementationName() + 
                               aUnoServices );

    aServices = SmXMLExportSettings_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmXMLExportContent_getImplementationName() + 
                               aUnoServices );

    aServices = SmXMLExportContent_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    xNewKey = xKey->createKey( aDelimiter + SmDocument_getImplementationName() + 
                               aUnoServices );

    aServices = SmDocument_getSupportedServiceNames();
    for(i = 0; i < aServices.getLength(); i++ )
        xNewKey->createKey( aServices.getConstArray()[i] );

    return sal_True;
}

void* SAL_CALL component_getFactory( const sal_Char* pImplementationName,
                                     void* pServiceManager,
                                     void* pRegistryKey )
{
	// Set default return value for this operation - if it failed.
	void* pReturn = NULL ;

	if	(
			( pImplementationName	!=	NULL ) &&
			( pServiceManager		!=	NULL )
		)
	{
		// Define variables which are used in following macros.
        Reference< XSingleServiceFactory >   xFactory                                                                                                ;
        Reference< XMultiServiceFactory >    xServiceManager( reinterpret_cast< XMultiServiceFactory* >( pServiceManager ) ) ;

		if( SmXMLImport_getImplementationName().equalsAsciiL(
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLImport_getImplementationName(),
			SmXMLImport_createInstance,
			SmXMLImport_getSupportedServiceNames() );
		}
		else if( SmXMLExport_getImplementationName().equalsAsciiL(
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLExport_getImplementationName(),
			SmXMLExport_createInstance,
			SmXMLExport_getSupportedServiceNames() );
		}
		else if( SmXMLImportMeta_getImplementationName().equalsAsciiL(
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLImportMeta_getImplementationName(),
			SmXMLImportMeta_createInstance,
			SmXMLImportMeta_getSupportedServiceNames() );
		}
		else if( SmXMLExportMeta_getImplementationName().equalsAsciiL(
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLExportMeta_getImplementationName(),
			SmXMLExportMeta_createInstance,
			SmXMLExportMeta_getSupportedServiceNames() );
		}
		else if( SmXMLImportSettings_getImplementationName().equalsAsciiL( 
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLImportSettings_getImplementationName(),
			SmXMLImportSettings_createInstance, 
			SmXMLImportSettings_getSupportedServiceNames() );
		}
		else if( SmXMLExportSettings_getImplementationName().equalsAsciiL( 
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLExportSettings_getImplementationName(),
			SmXMLExportSettings_createInstance, 
			SmXMLExportSettings_getSupportedServiceNames() );
		}
	    else if( SmXMLExportContent_getImplementationName().equalsAsciiL( 
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmXMLExportContent_getImplementationName(),
			SmXMLExportContent_createInstance, 
			SmXMLExportContent_getSupportedServiceNames() );
		}
	    else if( SmDocument_getImplementationName().equalsAsciiL( 
			pImplementationName, strlen(pImplementationName)) )
		{
			xFactory = ::cppu::createSingleFactory( xServiceManager,
			SmDocument_getImplementationName(),
			SmDocument_createInstance, 
			SmDocument_getSupportedServiceNames() );
		}


		// Factory is valid - service was found.
		if ( xFactory.is() )
		{
			xFactory->acquire();
			pReturn = xFactory.get();
		}
	}

	// Return with result of this operation.
	return pReturn ;
}
} // extern "C"



}
