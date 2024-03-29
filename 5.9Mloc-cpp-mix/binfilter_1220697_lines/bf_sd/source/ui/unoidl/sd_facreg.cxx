/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_facreg.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:36:02 $
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


#ifndef _OSL_DIAGNOSE_H_
#include <osl/diagnose.h>
#endif

#include <cppuhelper/factory.hxx>
#include <uno/lbnames.h>
namespace binfilter {

using namespace rtl;
using namespace ::com::sun::star;

extern uno::Reference< uno::XInterface > SAL_CALL SdDrawingDocument_createInstance( const uno::Reference< lang::XMultiServiceFactory > & _rxFactory );
extern OUString SdDrawingDocument_getImplementationName() throw( uno::RuntimeException );
extern uno::Sequence< OUString > SAL_CALL SdDrawingDocument_getSupportedServiceNames() throw( uno::RuntimeException );

extern uno::Reference< uno::XInterface > SAL_CALL SdPresentationDocument_createInstance( const uno::Reference< lang::XMultiServiceFactory > & _rxFactory );
extern OUString SdPresentationDocument_getImplementationName() throw( uno::RuntimeException );
extern uno::Sequence< OUString > SAL_CALL SdPresentationDocument_getSupportedServiceNames() throw( uno::RuntimeException );


#ifdef __cplusplus
extern "C"
{
#endif

void SAL_CALL component_getImplementationEnvironment( const sal_Char ** ppEnvTypeName, uno_Environment ** ppEnv )
{
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}

void SAL_CALL writeInfo( registry::XRegistryKey * pRegistryKey, const OUString& rImplementationName, const uno::Sequence< OUString >& rServices )
{
	uno::Reference< registry::XRegistryKey > xNewKey(
		pRegistryKey->createKey(
			OUString( RTL_CONSTASCII_USTRINGPARAM("/") ) + rImplementationName + OUString(RTL_CONSTASCII_USTRINGPARAM( "/UNO/SERVICES") ) ) );

	for( sal_Int32 i = 0; i < rServices.getLength(); i++ )
		xNewKey->createKey( rServices.getConstArray()[i]);
}

sal_Bool SAL_CALL component_writeInfo( void * pServiceManager, void * pRegistryKey )
{
	if( pRegistryKey )
	{
		try
		{
			registry::XRegistryKey *pKey = reinterpret_cast< registry::XRegistryKey * >( pRegistryKey );

			writeInfo( pKey, SdDrawingDocument_getImplementationName(), SdDrawingDocument_getSupportedServiceNames() );
			writeInfo( pKey, SdPresentationDocument_getImplementationName(), SdPresentationDocument_getSupportedServiceNames() );
		}
		catch (registry::InvalidRegistryException &)
		{
			OSL_ENSURE( sal_False, "### InvalidRegistryException!" );
		}
	}
    return sal_True;
}

void * SAL_CALL component_getFactory( const sal_Char * pImplName, void * pServiceManager, void * pRegistryKey )
{
	void * pRet = 0;

	if( pServiceManager )
	{
		uno::Reference< lang::XMultiServiceFactory > xMSF( reinterpret_cast< lang::XMultiServiceFactory * >( pServiceManager ) );

		uno::Reference< lang::XSingleServiceFactory > xFactory;

		const sal_Int32 nImplNameLen = strlen( pImplName );
		if(0)//STRIP001 if( SdHtmlOptionsDialog_getImplementationName().equalsAsciiL( pImplName, nImplNameLen ) )
		{
		}
		else if( SdDrawingDocument_getImplementationName().equalsAsciiL( pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SdDrawingDocument_getImplementationName(),
				SdDrawingDocument_createInstance,
				SdDrawingDocument_getSupportedServiceNames() );
		}
		else if( SdPresentationDocument_getImplementationName().equalsAsciiL( pImplName, nImplNameLen ) )
		{
			xFactory = ::cppu::createSingleFactory( xMSF,
				SdPresentationDocument_getImplementationName(),
				SdPresentationDocument_createInstance,
				SdPresentationDocument_getSupportedServiceNames() );
		}


		if( xFactory.is())
		{
			xFactory->acquire();
			pRet = xFactory.get();
		}
	}

	return pRet;
}

}
}
