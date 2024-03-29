/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_SdUnoOutlineView.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:12:21 $
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

#include "SdUnoOutlineView.hxx"

#ifndef _COM_SUN_STAR_BEANS_PROPERTYATTRIBUTE_HPP_
#include <com/sun/star/beans/PropertyAttribute.hpp>
#endif



#ifndef _TOOLKIT_HELPER_VCLUNOHELPER_HXX_ 
#include <toolkit/helper/vclunohelper.hxx>
#endif

#include "sdwindow.hxx"
#include "outlnvsh.hxx"

#include "unohelp.hxx"
#include "unomodel.hxx"
#include "sdview.hxx"
namespace binfilter {

using namespace ::rtl;
using namespace ::vos;
using namespace ::cppu;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;

//----------------------------------------------------------------------
//------ SdUnoOutlineView--------------------------------------------------
//----------------------------------------------------------------------

SdUnoOutlineView::SdUnoOutlineView(SdView* pSdView, SdOutlineViewShell* pSdViewSh) throw()
:	SdUnoOutlineViewBase(),
	SfxBaseController((SfxViewShell*)pSdViewSh),
	OBroadcastHelper( ((SdUnoOutlineViewBase *)this)->aMutex ),
	OPropertySetHelper( *static_cast<OBroadcastHelperVar<OMultiTypeInterfaceContainerHelper, OMultiTypeInterfaceContainerHelper::keyType> *>(this)),
	mpView(pSdView),
	mpViewSh(pSdViewSh),
	mbDisposing(sal_False)
{
}

SdUnoOutlineView::~SdUnoOutlineView() throw()
{
}

//----------------------------------------------------------------------
//------ XInterface ----------------------------------------------------
//----------------------------------------------------------------------

Any SAL_CALL SdUnoOutlineView::queryInterface( const Type & rType )
	throw(RuntimeException)
{
	Any aAny;

	QUERYINT( view::XSelectionSupplier );
	else QUERYINT( drawing::XDrawView );
	else QUERYINT( lang::XServiceInfo );
	else QUERYINT( beans::XPropertySet );
	else if( rType == ITYPE(lang::XComponent) )
		aAny <<= uno::Reference< lang::XComponent >(static_cast<SfxBaseController*>(this));
	else QUERYINT( awt::XWindow );
	else
		return SfxBaseController::queryInterface(rType);

	return aAny;
}

//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::acquire()
	throw ( )
{
	SfxBaseController::acquire();
}

//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::release()
	throw ( )
{
	SfxBaseController::release();
}

//----------------------------------------------------------------------
//------ XTypeProvider -------------------------------------------------
//----------------------------------------------------------------------

Sequence< Type > SAL_CALL SdUnoOutlineView::getTypes()
	throw(RuntimeException)
{
	static Sequence< Type > aTypeSequence;
	if( 0 == aTypeSequence.getLength() )
	{
		::osl::MutexGuard guard( ::osl::Mutex::getGlobalMutex() );
		if( 0 == aTypeSequence.getLength() )
		{
			const Sequence< Type > aBaseTypes( SfxBaseController::getTypes() );
			const sal_Int32 nBaseTypes = aBaseTypes.getLength();
			const Type* pBaseTypes = aBaseTypes.getConstArray();

			const sal_Int32 nOwnTypes = 6;		// !DANGER! Keep this updated!

			aTypeSequence.realloc(  nBaseTypes + nOwnTypes );
			Type* pTypes = aTypeSequence.getArray();

			*pTypes++ = ITYPE(view::XSelectionSupplier);
			*pTypes++ = ITYPE(drawing::XDrawView);
			*pTypes++ = ITYPE(lang::XServiceInfo);
			*pTypes++ = ITYPE(beans::XPropertySet);
			*pTypes++ = ITYPE(lang::XComponent);
			*pTypes++ = ITYPE(awt::XWindow);

			for( sal_Int32 nType = 0; nType < nBaseTypes; nType++ )
				*pTypes++ = *pBaseTypes++;
		}
	}

	return aTypeSequence;
}

//----------------------------------------------------------------------

Sequence< sal_Int8 > SAL_CALL SdUnoOutlineView::getImplementationId()
	throw(RuntimeException)
{
	static Sequence< sal_Int8 > aId;
	if( aId.getLength() == 0 )
	{
		aId.realloc( 16 );
		rtl_createUuid( (sal_uInt8 *)aId.getArray(), 0, sal_True );
	}
	return aId;
}

//----------------------------------------------------------------------
//------ XComponent ----------------------------------------------------
//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::dispose()
	throw( RuntimeException )
{
	if( !mbDisposing )
	{
		OGuard aGuard( Application::GetSolarMutex() );

		if( !mbDisposing )
		{
			mbDisposing = sal_True;
			mpView = NULL,
			mpViewSh = NULL;

			SfxBaseController::dispose();
		}
	}
}

void SAL_CALL SdUnoOutlineView::addEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	SfxBaseController::addEventListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& aListener ) throw (::com::sun::star::uno::RuntimeException)
{
	SfxBaseController::removeEventListener( aListener );
}

//----------------------------------------------------------------------
//------ XServiceInfo --------------------------------------------------
//----------------------------------------------------------------------

sal_Char pImplSdUnoOutlineViewService[sizeof("com.sun.star.presentation.OutlineView")] = "com.sun.star.presentation.OutlineView";

OUString SAL_CALL SdUnoOutlineView::getImplementationName(  ) throw(RuntimeException)
{
	return OUString( RTL_CONSTASCII_USTRINGPARAM( "SdUnoOutlineView" ) );
}

//----------------------------------------------------------------------


sal_Bool SAL_CALL SdUnoOutlineView::supportsService( const OUString& ServiceName ) throw(RuntimeException)
{
	return ServiceName.equalsAscii( pImplSdUnoOutlineViewService );
}

//----------------------------------------------------------------------

Sequence< OUString > SAL_CALL SdUnoOutlineView::getSupportedServiceNames(  ) throw(RuntimeException)
{
	OUString aService( RTL_CONSTASCII_USTRINGPARAM( pImplSdUnoOutlineViewService ) );
	Sequence< OUString > aSeq( &aService, 1 );
	return aSeq;
}

//----------------------------------------------------------------------
//------ XSelectionSupplier --------------------------------------------
//----------------------------------------------------------------------

sal_Bool SAL_CALL SdUnoOutlineView::select( const Any& aSelection )
	throw(lang::IllegalArgumentException, RuntimeException)
{
/*
	OGuard aGuard( Application::GetSolarMutex() );

	SdrPageView *pPV = mpView->GetPageViewPvNum(0);

	if(pPV == NULL)
		return sal_False;

	// first deselect all
	mpView->UnmarkAllObj( pPV );

	// if the any is empty, just deselect all
	if( !aSelection.hasValue() )
		return sal_True;

	Reference< drawing::XShape > xShape;
	aSelection >>= xShape;

	// if the any is a shape, select it
	if(xShape.is())
	{
		SvxShape* pShape = SvxShape::getImplementation( xShape );
		if( pShape && pShape->GetSdrObject() )
		{
			SdrObject *pObj = pShape->GetSdrObject();
			if(pObj && pObj->GetPage() == pPV->GetPage())
			{
				mpView->MarkObj( pObj, pPV );
				return sal_True;
			}
		}
	}
	// else it must be a XShapes collection
	else
	{
		Reference< drawing::XShapes > xShapes;
		aSelection >>= xShapes;
		if( xShapes.is() )
		{
			const sal_uInt32 nCount = xShapes->getCount();
			for( sal_uInt32 i = 0; i < nCount; i++ )
			{
				xShapes->getByIndex(i) >>= xShape;
				if( xShape.is() )
				{
					SvxShape* pShape = SvxShape::getImplementation(xShape);
					if( pShape )
					{
						SdrObject *pObj = pShape->GetSdrObject();
						if(pObj && pObj->GetPage() == pPV->GetPage())
						{
							mpView->MarkObj( pObj, pPV );
							return sal_True;
						}
					}
				}
			}
		}
	}
*/
	// todo: add selections for text ranges
	return sal_False;
}

//----------------------------------------------------------------------

Any SAL_CALL SdUnoOutlineView::getSelection()
	throw(RuntimeException)
{
	Any aAny;
	return aAny;
}

//----------------------------------------------------------------------

inline const ::com::sun::star::uno::Type & getSelectionTypeIdentifier()
{
	return ::getCppuType( (Reference< view::XSelectionChangeListener > *)0 );
}

//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::addSelectionChangeListener( const Reference< view::XSelectionChangeListener >& xListener )
	throw(RuntimeException)
{
	addListener( getSelectionTypeIdentifier(), xListener );
}

//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::removeSelectionChangeListener( const Reference< view::XSelectionChangeListener >& xListener ) throw(RuntimeException)
{
	removeListener( getSelectionTypeIdentifier( ), xListener );
}

//----------------------------------------------------------------------
//------ The Properties of this implementation -------------------------
//----------------------------------------------------------------------

// Id must be the index into the array
enum properties
{
	PROPERTY_WORKAREA = 0,

	PROPERTY_COUNT
};

/**
 * All Properties of this implementation. Must be sorted by name.
 */
static beans::Property * getBasicProps()
{
	static beans::Property *pTable = 0;

	if( ! pTable ) 
	{
		::osl::MutexGuard guard( ::osl::Mutex::getGlobalMutex() );
		if( ! pTable )
		{

			static beans::Property aBasicProps[PROPERTY_COUNT] =
			{
				beans::Property( OUString( RTL_CONSTASCII_USTRINGPARAM("VisibleArea") ),		PROPERTY_WORKAREA,			::getCppuType((const ::com::sun::star::awt::Rectangle*)0), beans::PropertyAttribute::BOUND | beans::PropertyAttribute::READONLY )
			};
			pTable = aBasicProps;
		}
	}
	return pTable;
}

//----------------------------------------------------------------------
//------ XPropertySet & OPropertySetHelper -----------------------------
//----------------------------------------------------------------------

/**
 * Create a table that map names to index values.
 */
IPropertyArrayHelper & SdUnoOutlineView::getInfoHelper()
{
	OGuard aGuard( Application::GetSolarMutex() );

	static OPropertyArrayHelper aInfo( getBasicProps(), PROPERTY_COUNT );
	return aInfo;
}

//----------------------------------------------------------------------

Reference < beans::XPropertySetInfo >  SdUnoOutlineView::getPropertySetInfo() throw ( ::com::sun::star::uno::RuntimeException)
{
	OGuard aGuard( Application::GetSolarMutex() );

	static Reference < beans::XPropertySetInfo >  xInfo( createPropertySetInfo( getInfoHelper() ) );
	return xInfo;
}

//----------------------------------------------------------------------

// Return sal_True, value changed
sal_Bool SdUnoOutlineView::convertFastPropertyValue
(
	Any & rConvertedValue, 
	Any & rOldValue, 
	sal_Int32 nHandle, 
	const Any& rValue
) throw ( ::com::sun::star::lang::IllegalArgumentException)
{
	return sal_False;
}

//----------------------------------------------------------------------

/** 
 * only set the value.
 */
void SdUnoOutlineView::setFastPropertyValue_NoBroadcast
(
	sal_Int32 nHandle, 
	const Any& rValue
) throw ( ::com::sun::star::uno::Exception)
{
/*
	switch( nHandle )
	{
		case PROPERTY_MASTERPAGEMODE:
			{
			sal_Bool bOldValue = getMasterPageMode();
			sal_Bool b;
			convertPropertyValue( b , rValue );
			if( b != bOldValue )
			{				
				
				rConvertedValue.setValue( &b , ::getCppuBooleanType()  );
				rOldValue.setValue( & bOldValue , ::getCppuBooleanType() );
				return sal_True;
			}
			else
				return sal_False;
			}

	}
*/
}

//----------------------------------------------------------------------

void SdUnoOutlineView::getFastPropertyValue( Any & rRet, sal_Int32 nHandle ) const
{
	switch( nHandle )
	{
		case PROPERTY_WORKAREA:
			rRet <<= awt::Rectangle( maLastVisArea.Left(), maLastVisArea.Top(), maLastVisArea.GetWidth(), maLastVisArea.GetHeight() );
			break;
	}
}

//----------------------------------------------------------------------
//------ XDrawView -----------------------------------------------------
//----------------------------------------------------------------------

void SAL_CALL SdUnoOutlineView::setCurrentPage( const Reference< drawing::XDrawPage >& xPage )
	throw(RuntimeException)
{
/*
	OGuard aGuard( Application::GetSolarMutex() );

	if(mpView)
	{
		SvxDrawPage* pDrawPage = SvxDrawPage::getImplementation( xPage );
		SdrPage *pSdrPage = pDrawPage ? pDrawPage->GetSdrPage() : NULL;

		if(pSdrPage)
		{
			setMasterPageMode( pSdrPage->IsMasterPage() );
			mpViewSh->SwitchPage( (pSdrPage->GetPageNum() - 1) >> 1 );
		}
	}
*/
}

//----------------------------------------------------------------------

Reference< drawing::XDrawPage > SAL_CALL SdUnoOutlineView::getCurrentPage()
	throw(RuntimeException)
{
	OGuard aGuard( Application::GetSolarMutex() );

	Reference< drawing::XDrawPage >  xPage;

/*
	if(mpView)
	{
		SdXImpressDocument* pModel = getModel();
		SdrPageView *pPV = mpView->GetPageViewPvNum(0);
		SdrPage* pPage = pPV ? pPV->GetPage() : NULL;

		if(pPage)
			xPage = Reference< drawing::XDrawPage >::query( pPage->getUnoPage() );
	}
*/
	return xPage;
}

//----------------------------------------------------------------------

Reference< ::com::sun::star::awt::XWindow > SdUnoOutlineView::getWindow()
{
	Reference< ::com::sun::star::awt::XWindow > xWindow;
	if( mpViewSh && mpViewSh->GetActiveWindow() )
		xWindow = VCLUnoHelper::GetInterface( mpViewSh->GetActiveWindow() );
	return xWindow;
}

void SAL_CALL SdUnoOutlineView::setPosSize( sal_Int32 X, sal_Int32 Y, sal_Int32 Width, sal_Int32 Height, sal_Int16 Flags ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->setPosSize( X, Y, Width, Height, Flags );
}

::com::sun::star::awt::Rectangle SAL_CALL SdUnoOutlineView::getPosSize(  ) throw (::com::sun::star::uno::RuntimeException)
{
	::com::sun::star::awt::Rectangle aRect;

	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		aRect = xWindow->getPosSize();

	return aRect;
}

void SAL_CALL SdUnoOutlineView::setVisible( sal_Bool Visible ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->setVisible( Visible );
}

void SAL_CALL SdUnoOutlineView::setEnable( sal_Bool Enable ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->setEnable( Enable );
}

void SAL_CALL SdUnoOutlineView::setFocus(  ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->setFocus();
}

void SAL_CALL SdUnoOutlineView::addWindowListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addWindowListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeWindowListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removeWindowListener( xListener );
}

void SAL_CALL SdUnoOutlineView::addFocusListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XFocusListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addFocusListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeFocusListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XFocusListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removeFocusListener( xListener );
}

void SAL_CALL SdUnoOutlineView::addKeyListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XKeyListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addKeyListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeKeyListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XKeyListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removeKeyListener( xListener );
}

void SAL_CALL SdUnoOutlineView::addMouseListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addMouseListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeMouseListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removeMouseListener( xListener );
}

void SAL_CALL SdUnoOutlineView::addMouseMotionListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseMotionListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addMouseMotionListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removeMouseMotionListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseMotionListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removeMouseMotionListener( xListener );
}

void SAL_CALL SdUnoOutlineView::addPaintListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XPaintListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->addPaintListener( xListener );
}

void SAL_CALL SdUnoOutlineView::removePaintListener( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XPaintListener >& xListener ) throw (::com::sun::star::uno::RuntimeException)
{
	Reference< ::com::sun::star::awt::XWindow > xWindow( getWindow() );
	if( xWindow.is() )
		xWindow->removePaintListener( xListener );
}

}
