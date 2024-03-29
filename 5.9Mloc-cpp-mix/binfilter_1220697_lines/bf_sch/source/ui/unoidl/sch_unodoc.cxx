/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sch_unodoc.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 17:59:57 $
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

// System - Includes -----------------------------------------------------

#include <bf_sfx2/docfac.hxx>


#include "schmod.hxx"

#ifndef _VOS_MUTEX_HXX_
#include <vos/mutex.hxx>
#endif
#ifndef _SV_SVAPP_HXX
#include <vcl/svapp.hxx>
#endif
namespace binfilter {

using namespace ::com::sun::star;

::rtl::OUString SAL_CALL SchDocument_getImplementationName() throw()
{
	return ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.comp.Chart.ChartDocument" ) );
}

uno::Sequence< ::rtl::OUString > SAL_CALL SchDocument_getSupportedServiceNames() throw()
{
	uno::Sequence< ::rtl::OUString > aSeq( 3 );
	aSeq[0] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.chart.ChartDocument" ) );
	aSeq[1] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.chart.ChartTableAddressSupplier" ) );
	aSeq[2] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.xml.UserDefinedAttributeSupplier" ) );

	return aSeq;
}

uno::Reference< uno::XInterface > SAL_CALL SchDocument_createInstance(
				const uno::Reference< lang::XMultiServiceFactory > & rSMgr ) throw( uno::Exception )
{
    ::vos::OGuard aGuard( Application::GetSolarMutex() );

	// to create the service the SW_MOD should be already initialized
	DBG_ASSERT( SCH_MOD1(), "No StarChart module!" );
	
	if ( SCH_MOD1() )
	{
		::rtl::OUString aFactoryURL( RTL_CONSTASCII_USTRINGPARAM ( "private:factory/schart" ) );
		const SfxObjectFactory* pFactory = SfxObjectFactory::GetFactory( aFactoryURL );
		if ( pFactory )
		{
			SfxObjectShell* pShell = pFactory->CreateObject();
			if( pShell )
				return uno::Reference< uno::XInterface >( pShell->GetModel() );
		}
	}

	return uno::Reference< uno::XInterface >();
}


}
