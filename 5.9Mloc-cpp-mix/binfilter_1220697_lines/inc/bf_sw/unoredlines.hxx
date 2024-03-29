/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: unoredlines.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 16:47:30 $
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
#ifndef _UNOREDLINES_HXX
#define _UNOREDLINES_HXX

#ifndef _UNOCOLL_HXX
#include <unocoll.hxx>
#endif
#ifndef _UNOBASECLASS_HXX
#include <unobaseclass.hxx>
#endif

#ifndef _COM_SUN_STAR_CONTAINER_XENUMERATIONACCESS_HPP_
#include <com/sun/star/container/XEnumerationAccess.hpp>
#endif
namespace com{ namespace sun{ namespace star{
		namespace beans{
			class XPropertySet;
		}
}}}
namespace binfilter {

class SwRedline;
typedef
cppu::WeakImplHelper3
<
	::com::sun::star::container::XIndexAccess,
	::com::sun::star::container::XEnumerationAccess,
	::com::sun::star::lang::XServiceInfo
>
SwRedlinesBaseClass;
class SwXRedlines : public SwRedlinesBaseClass,
	public SwUnoCollection
{
protected:
	virtual ~SwXRedlines();
public:
	SwXRedlines(SwDoc* pDoc);
	

	//XIndexAccess
    virtual sal_Int32 SAL_CALL getCount(  ) throw(::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Any SAL_CALL getByIndex(sal_Int32 nIndex) throw( ::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException );

	//XEnumerationAccess - frueher XParagraphEnumerationAccess
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::container::XEnumeration >  SAL_CALL createEnumeration(void) throw( ::com::sun::star::uno::RuntimeException );

	//XElementAccess
    virtual ::com::sun::star::uno::Type SAL_CALL getElementType(  ) throw(::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL hasElements(  ) throw(::com::sun::star::uno::RuntimeException);

	//XServiceInfo
	virtual ::rtl::OUString SAL_CALL getImplementationName(void) throw( ::com::sun::star::uno::RuntimeException );
	virtual BOOL SAL_CALL supportsService(const ::rtl::OUString& ServiceName) throw( ::com::sun::star::uno::RuntimeException );
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames(void) throw( ::com::sun::star::uno::RuntimeException );

	static ::com::sun::star::beans::XPropertySet* 			GetObject( SwRedline& rRedline, SwDoc& rDoc );
};
/* -----------------------------12.01.01 14:58--------------------------------

 ---------------------------------------------------------------------------*/
class SwXRedlineEnumeration : public SwSimpleEnumerationBaseClass,
	public SwClient
{
	SwDoc* pDoc;
	USHORT nCurrentIndex;
protected:
	virtual ~SwXRedlineEnumeration();
public:
	SwXRedlineEnumeration(SwDoc& rDoc);
	

	//XEnumeration
	virtual BOOL SAL_CALL hasMoreElements(void) throw( ::com::sun::star::uno::RuntimeException );
	virtual ::com::sun::star::uno::Any SAL_CALL nextElement(void) throw( ::com::sun::star::container::NoSuchElementException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException );

	//XServiceInfo
	virtual ::rtl::OUString SAL_CALL getImplementationName(void) throw( ::com::sun::star::uno::RuntimeException );
	virtual BOOL SAL_CALL supportsService(const ::rtl::OUString& ServiceName) throw( ::com::sun::star::uno::RuntimeException );
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames(void) throw( ::com::sun::star::uno::RuntimeException );

	//SwClient
	virtual void 			Modify( SfxPoolItem *pOld, SfxPoolItem *pNew);
};


} //namespace binfilter
#endif

