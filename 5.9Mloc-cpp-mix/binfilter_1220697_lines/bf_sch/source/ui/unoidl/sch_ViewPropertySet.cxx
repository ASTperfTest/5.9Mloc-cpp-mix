/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sch_ViewPropertySet.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:09:57 $
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
#include "ViewPropertySet.hxx"

#ifndef _COM_SUN_STAR_BEANS_PROPERTYATTRIBUTE_HPP_
#include <com/sun/star/beans/PropertyAttribute.hpp>
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::com::sun::star::beans;
using namespace ::cppu;

// ________________________________________

#define CHART_VIEW_PROPERTY_VISIBLE_AREA  1

static const sal_Int32 lcl_nPropertyTableSize = 1;

static Property * lcl_getPropertyTable( ::osl::Mutex & aMutex )
{
	static Property *pTable = 0;

	if( NULL != pTable )
    {
		::osl::MutexGuard guard( aMutex );
		if( NULL != pTable )
        {
			static Property aTable[ lcl_nPropertyTableSize ] =
			{
				Property( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "VisibleArea" )),
                          CHART_VIEW_PROPERTY_VISIBLE_AREA,
                          ::getCppuType(( const ::com::sun::star::awt::Rectangle * )0),
                          PropertyAttribute::BOUND | PropertyAttribute::READONLY )
			};
			pTable = aTable;
		}
	}
	return pTable;
}

// ________________________________________

IPropertyArrayHelper & SAL_CALL ViewPropertySet::getInfoHelper()
{
	static OPropertyArrayHelper aInfo( lcl_getPropertyTable( m_aMutex ),
                                       lcl_nPropertyTableSize,
                                       sal_True /* is sorted */ );
	return aInfo;
}

// ________________________________________

uno::Reference< beans::XPropertySetInfo >  ViewPropertySet::getPropertySetInfo()
    throw (uno::RuntimeException)
{
    ::osl::MutexGuard guard( m_aMutex );

	static uno::Reference< beans::XPropertySetInfo >  xInfo( createPropertySetInfo( getInfoHelper() ) );
	return xInfo;
}

// ________________________________________

ViewPropertySet::ViewPropertySet() :
        OBroadcastHelper( m_aMutex ),
        // this line causes a warning.  I have no solution to avoid this yet
        OPropertySetHelper( static_cast< OBroadcastHelper & >( *this ))
{
}

ViewPropertySet::~ViewPropertySet()
{}

// ____________________ protected methods ____________________

// OPropertySetHelper

sal_Bool SAL_CALL ViewPropertySet::convertFastPropertyValue(
    uno::Any & rConvertedValue,
	uno::Any & rOldValue,
    sal_Int32 nHandle,
    const uno::Any& rValue )
    throw (lang::IllegalArgumentException)
{
    // currently there are no properties that may be applied to
    return sal_False;
}

void SAL_CALL ViewPropertySet::setFastPropertyValue_NoBroadcast
    ( sal_Int32 nHandle,
      const uno::Any& rValue )
    throw (uno::Exception)
{
    // currently there are no properties that may be applied to
}

void SAL_CALL ViewPropertySet::getFastPropertyValue
    ( uno::Any& rValue,
      sal_Int32 nHandle ) const
{
    if( CHART_VIEW_PROPERTY_VISIBLE_AREA == nHandle )
        rValue <<= maLastVisArea;
}

}
