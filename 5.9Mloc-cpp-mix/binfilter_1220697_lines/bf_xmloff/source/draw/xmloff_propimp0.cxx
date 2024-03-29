/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmloff_propimp0.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 13:16:26 $
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

#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifndef _STRING_HXX
#include <tools/string.hxx>
#endif

#ifndef _RTL_USTRBUF_HXX_ 
#include <rtl/ustrbuf.hxx>
#endif

#ifndef _PROPIMP0_HXX
#include "propimp0.hxx"
#endif


#ifndef _COM_SUN_STAR_UTIL_DATETIME_HPP_
#include <com/sun/star/util/DateTime.hpp>
#endif


#ifndef _XMLOFF_XMLUCONV_HXX 
#include <xmluconv.hxx>
#endif
namespace binfilter {

using namespace ::rtl;
using namespace ::com::sun::star;

//////////////////////////////////////////////////////////////////////////////
// implementation of graphic property Stroke


//////////////////////////////////////////////////////////////////////////////
// implementation of presentation page property Change

//////////////////////////////////////////////////////////////////////////////
// implementation of an effect duration property handler


XMLDurationPropertyHdl::~XMLDurationPropertyHdl() 
{
}

sal_Bool XMLDurationPropertyHdl::importXML( 
	const OUString& rStrImpValue, 
	::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	util::DateTime aTime;
	SvXMLUnitConverter::convertTime( aTime,  rStrImpValue );

	const sal_Int32 nSeconds = ( aTime.Hours * 60 + aTime.Minutes ) * 60 + aTime.Seconds;
	rValue <<= nSeconds;

	return sal_True;
}

sal_Bool XMLDurationPropertyHdl::exportXML( 
	OUString& rStrExpValue, 
	const ::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	sal_Int32 nVal;

	if(rValue >>= nVal)
	{
		util::DateTime aTime( 0, (sal_uInt16)nVal, 0, 0, 0, 0, 0 );

		OUStringBuffer aOut;
		SvXMLUnitConverter::convertTime( aOut, aTime );
		rStrExpValue = aOut.makeStringAndClear();
		return sal_True;
	}

	return sal_False;
}

//////////////////////////////////////////////////////////////////////////////
// implementation of an opacity property handler


XMLOpacityPropertyHdl::~XMLOpacityPropertyHdl() 
{
}

sal_Bool XMLOpacityPropertyHdl::importXML( 
	const OUString& rStrImpValue, 
	::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	sal_Bool bRet = sal_False;
	sal_Int32 nValue = 0;

	if( rStrImpValue.indexOf( sal_Unicode('%') ) != -1 )
	{
		if( rUnitConverter.convertPercent( nValue, rStrImpValue ) )
		{
			rValue <<= sal_uInt16( nValue );
			bRet = sal_True;
		}
	}
	else
	{
		const String aStr( rStrImpValue );
		double fVal = aStr.ToDouble() * 100.0;
		rValue <<= sal_uInt16( fVal );
		bRet = sal_True;
	}

	return bRet;
}

sal_Bool XMLOpacityPropertyHdl::exportXML( 
	OUString& rStrExpValue, 
	const ::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	sal_Bool bRet = sal_False;
	sal_uInt16 nVal;

	if( rValue >>= nVal )
	{
		OUStringBuffer aOut;

		rUnitConverter.convertPercent( aOut, nVal );
		rStrExpValue = aOut.makeStringAndClear();
		bRet = sal_True;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////////
// implementation of an text animation step amount

XMLTextAnimationStepPropertyHdl::~XMLTextAnimationStepPropertyHdl()
{
}

sal_Bool XMLTextAnimationStepPropertyHdl::importXML( 
	const OUString& rStrImpValue, 
	::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	sal_Bool bRet = sal_False;
	sal_Int32 nValue = 0;

	const OUString aPX( RTL_CONSTASCII_USTRINGPARAM( "px" ) );
	sal_Int32 nPos = rStrImpValue.indexOf( aPX );
	if( nPos != -1 )
	{
		if( rUnitConverter.convertNumber( nValue, rStrImpValue.copy( 0, nPos ) ) )
		{
			rValue <<= sal_Int16( -nValue );
			bRet = sal_True;
		}
	}
	else
	{
		if( rUnitConverter.convertMeasure( nValue, rStrImpValue ) )
		{
			rValue <<= sal_Int16( nValue );
			bRet = sal_True;
		}
	}

	return bRet;
}

sal_Bool XMLTextAnimationStepPropertyHdl::exportXML( 
	OUString& rStrExpValue, 
	const ::com::sun::star::uno::Any& rValue, 
	const SvXMLUnitConverter& rUnitConverter ) const
{
	sal_Bool bRet = sal_False;
	sal_Int16 nVal;

	if( rValue >>= nVal )
	{
		OUStringBuffer aOut;

		if( nVal < 0 )
		{
			const OUString aPX( RTL_CONSTASCII_USTRINGPARAM( "px" ) );
			rUnitConverter.convertNumber( aOut, (sal_Int32)-nVal );
			aOut.append( aPX );
		}
		else
		{
			rUnitConverter.convertMeasure( aOut, nVal );
		}

		rStrExpValue = aOut.makeStringAndClear();
		bRet = sal_True;
	}

	return bRet;
}


}//end of namespace binfilter
