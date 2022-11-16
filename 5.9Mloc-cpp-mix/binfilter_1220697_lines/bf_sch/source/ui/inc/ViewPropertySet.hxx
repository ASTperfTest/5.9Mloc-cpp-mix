/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: ViewPropertySet.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:09:24 $
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
#ifndef _VIEWPROPERTYSET_HXX_
#define _VIEWPROPERTYSET_HXX_

#ifndef _CPPUHELPER_PROPSHLP_HXX
#include <cppuhelper/propshlp.hxx>
#endif

#ifndef _COM_SUN_STAR_AWT_RECTANGLE_HPP_
#include <com/sun/star/awt/Rectangle.hpp>
#endif
namespace binfilter {

namespace lcl_chart_view
{

/** This class provides a mutex.

    A Class that derives from this class can hand over this mutex to
    other base classes in its constructor.  This would not work, if
    the mutex was a class member
 */
class MutexContainer
{
protected:
    ::osl::Mutex m_aMutex;
};

}  // namespace lcl_chart_view

// ________________________________________

/** This class is used by ChXChartView to support XPropertySet.  This
    class uses the OPropertySetHelper which also provides other
    interfaces like XFastPropertySet and XMultiPropertySet

    Currently there are the following properties:

    <table>
     <tr><th>Name</th><th>Type</th><th>Attributes</th></tr>
     <tr>
      <td>VisibleArea</td> <td>awt::Rectangle</td> <td>/bound, read-only</td>
     </tr>
    </table>
 */
class ViewPropertySet :
	private ::binfilter::lcl_chart_view::MutexContainer,
    // needed for OPropertySetHelper to handle PropertyChangeEvents
    public ::cppu::OBroadcastHelper,
    // provides XPropertySet
    public ::cppu::OPropertySetHelper
{
public:
	ViewPropertySet();
	virtual ~ViewPropertySet();

    // XPropertySet
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo > SAL_CALL
        getPropertySetInfo()
            throw(::com::sun::star::uno::RuntimeException);

protected:
    // OPropertySetHelper
    // ------------------

	/** The InfoHelper table contains all property names and types of
        this object.

        @return the object that provides information for the
                PropertySetInfo
	 */
	virtual ::cppu::IPropertyArrayHelper & SAL_CALL getInfoHelper();
	
	/** Try to convert the value <code>rValue</code> to the type
        required by the property associated with <code>nHandle</code>.

        If the conversion changed  , </TRUE> is returned and the
        converted value is in <code>rConvertedValue</code>.  The
        former value is contained in <code>rOldValue</code>.

        After this call returns successfully, the vetoable listeners
        are notified.

        @throws IllegalArgumentException, if the conversion was not
                successful, or if there is no corresponding property
                to the given handle.
	  
        @param rConvertedValue the converted value. Only set if return
               is true.
        @param rOldValue the old value. Only set if return is true.
        @param nHandle the handle of the property.

        @return true, if the conversion was successful and converted
                value differs from the old value.
	 */
	virtual sal_Bool SAL_CALL convertFastPropertyValue
        ( ::com::sun::star::uno::Any & rConvertedValue,
          ::com::sun::star::uno::Any & rOldValue,
          sal_Int32 nHandle,
          const ::com::sun::star::uno::Any& rValue )
		throw (::com::sun::star::lang::IllegalArgumentException);
    
	/** The same as setFastProperyValue; nHandle is always valid.
        The changes must not be broadcasted in this method.
        The method is implemented in a derived class.

        @attention
        Although you are permitted to throw any UNO exception, only the following
        are valid for usage:
        -- ::com::sun::star::beans::UnknownPropertyException
        -- ::com::sun::star::beans::PropertyVetoException
        -- ::com::sun::star::lang::IllegalArgumentException
        -- ::com::sun::star::lang::WrappedTargetException
        -- ::com::sun::star::uno::RuntimeException
        
        @param nHandle handle
        @param rValue  value
    */
	virtual void SAL_CALL setFastPropertyValue_NoBroadcast
        ( sal_Int32 nHandle,
          const ::com::sun::star::uno::Any& rValue )
		throw (::com::sun::star::uno::Exception);
	/**
	   The same as getFastProperyValue, but return the value through rValue and nHandle
	   is always valid.
	   The method is not implemented in this class.
	 */
	virtual void SAL_CALL getFastPropertyValue
        ( ::com::sun::star::uno::Any& rValue,
          sal_Int32 nHandle ) const;

private:
    ::com::sun::star::awt::Rectangle  maLastVisArea;
};

} //namespace binfilter
#endif	// _VIEWPROPERTYSET_HXX_
