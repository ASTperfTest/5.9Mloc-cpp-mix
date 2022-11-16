/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: dlelstnr.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:37:59 $
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

#ifndef _DLELSTNR_HXX_
#define _DLELSTNR_HXX_


#ifndef _COM_SUN_STAR_LINGUISTIC2_XDICTIONARYLISTEVENTLISTENER_HPP_
#include <com/sun/star/linguistic2/XDictionaryListEventListener.hpp>
#endif
#ifndef _COM_SUN_STAR_LINGUISTIC2_XLINGUSERVICEEVENTLISTENER_HPP_
#include <com/sun/star/linguistic2/XLinguServiceEventListener.hpp>
#endif
#ifndef _COM_SUN_STAR_FRAME_XDESKTOP_HPP_
#include <com/sun/star/frame/XDesktop.hpp>
#endif

#ifndef _CPPUHELPER_IMPLBASE2_HXX_
#include <cppuhelper/implbase2.hxx>	// helper for implementations
#endif


namespace com { namespace sun { namespace star {
	namespace linguistic2 {
		class XDictionaryList;
		class XLinguServiceManager;
	}
	namespace frame {
		class XTerminateListener;
	}
} } }
namespace binfilter {

///////////////////////////////////////////////////////////////////////////
// SwLinguServiceEventListener
// is a EventListener that triggers spellchecking
// and hyphenation when relevant changes (to the
// dictionaries of the dictionary list, or properties) were made.
//

class SwLinguServiceEventListener : 
	public cppu::WeakImplHelper2
	<
		::com::sun::star::linguistic2::XLinguServiceEventListener,
		::com::sun::star::frame::XTerminateListener
	>
{
	::com::sun::star::uno::Reference<
		::com::sun::star::frame::XDesktop >					xDesktop;
	::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XLinguServiceManager >	xLngSvcMgr;

	// disallow use of copy-constructor and assignment operator
	SwLinguServiceEventListener(const SwLinguServiceEventListener &);
	SwLinguServiceEventListener & operator = (const SwLinguServiceEventListener &);

public:
	SwLinguServiceEventListener();
	virtual ~SwLinguServiceEventListener();

	// XEventListener
	virtual void SAL_CALL disposing( 
			const ::com::sun::star::lang::EventObject& rEventObj ) 
		throw(::com::sun::star::uno::RuntimeException);

	// XDictionaryListEventListener
    virtual void 	SAL_CALL processDictionaryListEvent(
			const ::com::sun::star::linguistic2::DictionaryListEvent& rDicListEvent) 
		throw( ::com::sun::star::uno::RuntimeException );

	// XLinguServiceEventListener
    virtual void SAL_CALL processLinguServiceEvent( 
			const ::com::sun::star::linguistic2::LinguServiceEvent& rLngSvcEvent ) 
		throw(::com::sun::star::uno::RuntimeException);

    // XTerminateListener
    virtual void SAL_CALL queryTermination( 
			const ::com::sun::star::lang::EventObject& rEventObj ) 
		throw(::com::sun::star::frame::TerminationVetoException, ::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL notifyTermination( 
			const ::com::sun::star::lang::EventObject& rEventObj ) 
		throw(::com::sun::star::uno::RuntimeException);
};


} //namespace binfilter
#endif
