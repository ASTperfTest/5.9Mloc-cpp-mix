/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmloff_opaquhdl.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 02:03:43 $
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

#ifndef _XMLOFF_PROPERTYHANDLER_OPAQUETYPES_HXX
#include "opaquhdl.hxx"
#endif



// --


#ifndef _XMLOFF_XMLUCONV_HXX 
#include "xmluconv.hxx"
#endif
namespace binfilter {

using namespace ::rtl;
using namespace ::com::sun::star::uno;
using namespace ::binfilter::xmloff::token;


///////////////////////////////////////////////////////////////////////////////
//
// class XMLOpaquePropHdl
//

XMLOpaquePropHdl::~XMLOpaquePropHdl()
{
	// nothing to do
}

sal_Bool XMLOpaquePropHdl::importXML( const OUString& rStrImpValue, Any& rValue, const SvXMLUnitConverter& rUnitConverter ) const
{ 
	sal_Bool bRet = sal_True;

	sal_Bool bValue = IsXMLToken( rStrImpValue, XML_OPAQUE_FOREGROUND );
	rValue <<= sal_Bool(bValue);
	bRet = sal_True;

	return bRet; 
}

sal_Bool XMLOpaquePropHdl::exportXML( OUString& rStrExpValue, const Any& rValue, const SvXMLUnitConverter& rUnitConverter ) const
{ 
	sal_Bool bRet = sal_False;
	sal_Bool bValue;

	if (rValue >>= bValue)
	{
		if( bValue )
			rStrExpValue = GetXMLToken( XML_OPAQUE_FOREGROUND );
		else
			rStrExpValue = GetXMLToken( XML_OPAQUE_BACKGROUND );

		bRet = sal_True;
	}
	
	return bRet;
}
}//end of namespace binfilter
