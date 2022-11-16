/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: unopracc.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 21:13:52 $
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

#ifndef _SVX_UNOPRACC_HXX
#define _SVX_UNOPRACC_HXX


#ifndef _SVX_UNOTEXT_HXX
#include "unotext.hxx"
#endif
namespace binfilter {


class SvxEditSource;

/** Wraps SvxUnoTextRangeBase and provides us with the text properties
 
    Inherits from SvxUnoTextRangeBase and provides XPropertySet and
    XMultiPropertySet interfaces. Just set the selection to the
    required text range and return a reference to a XPropertySet.
 */
class SvxAccessibleTextPropertySet : public SvxUnoTextRangeBase, 
                                     public ::com::sun::star::lang::XTypeProvider,
                                     public ::cppu::OWeakObject
{
public:
	SvxAccessibleTextPropertySet( const SvxEditSource*, const SfxItemPropertyMap* );
	virtual ~SvxAccessibleTextPropertySet() throw();

    // XTextRange
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::text::XText > SAL_CALL getText() throw (::com::sun::star::uno::RuntimeException);
 
	// uno::XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryAggregation( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	virtual void SAL_CALL acquire() throw();
	virtual void SAL_CALL release() throw();

	// lang::XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ) throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames() throw (::com::sun::star::uno::RuntimeException);

	// lang::XTypeProvider
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);

	// XServiceName
    ::rtl::OUString SAL_CALL getServiceName() throw (::com::sun::star::uno::RuntimeException);
};

}//end of namespace binfilter
#endif
