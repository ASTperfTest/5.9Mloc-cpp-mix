/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: ChXChartView.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/07 22:41:22 $
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

#ifndef _CHART_VIEW_HXX
#define _CHART_VIEW_HXX

#ifndef _COM_SUN_STAR_VIEW_XSELECTIONSUPPLIER_HPP_
#include <com/sun/star/view/XSelectionSupplier.hpp>
#endif
#ifndef _SFX_SFXBASECONTROLLER_HXX_
#include <bf_sfx2/sfxbasecontroller.hxx>
#endif
#ifndef _CPPUHELPER_INTERFACECONTAINER_HXX_
#include <cppuhelper/interfacecontainer.hxx>
#endif
#ifndef _CPPUHELPER_PROPSHLP_HXX
#include <cppuhelper/propshlp.hxx>
#endif

#include "ViewPropertySet.hxx"
namespace binfilter {

class SchViewShell;

class ChXChartView :
	public ::com::sun::star::view::XSelectionSupplier,
    // provides XController, XComponent
	public SfxBaseController,
    public ViewPropertySet
{
private:
    ::osl::Mutex m_aViewMutex;

    SchViewShell*	 mpViewShell;
	::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > maTypeSequence;
    ::cppu::OBroadcastHelper  maSelectionListenerManager;

public:
	ChXChartView( SchViewShell* pSchViewShell );
	virtual ~ChXChartView();

    /** this method should be called (from the core) whenever the current
        selection has changed.  It will then notify all selection change
        listeners registered at this view
     */
	void fireSelectionChangeListener() throw();

	// XSelectionSupplier
    virtual sal_Bool SAL_CALL select( const ::com::sun::star::uno::Any& aSelection )
		throw( ::com::sun::star::lang::IllegalArgumentException,
			   ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Any SAL_CALL getSelection()
		throw( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL addSelectionChangeListener(
		const ::com::sun::star::uno::Reference< ::com::sun::star::view::XSelectionChangeListener >& xListener )
		throw( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL removeSelectionChangeListener(
		const ::com::sun::star::uno::Reference< ::com::sun::star::view::XSelectionChangeListener >& xListener )
		throw( ::com::sun::star::uno::RuntimeException );

	// XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType )
		throw( ::com::sun::star::uno::RuntimeException );	
    virtual void SAL_CALL acquire() throw();
    virtual void SAL_CALL release() throw();

	// XTypeProvider ( ::SfxBaseController )
	virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes()
		throw( ::com::sun::star::uno::RuntimeException );
    virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId()
		throw( ::com::sun::star::uno::RuntimeException );

    // XComponent ( ::XController::SfxBaseController )
	virtual void SAL_CALL dispose() throw( ::com::sun::star::uno::RuntimeException );
};

} //namespace binfilter
#endif	// _CHART_VIEW_HXX

