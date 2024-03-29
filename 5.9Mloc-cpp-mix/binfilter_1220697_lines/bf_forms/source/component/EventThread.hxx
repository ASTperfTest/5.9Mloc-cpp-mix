/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: EventThread.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 13:56:40 $
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

#ifndef _FRM_EVENT_THREAD_HXX_
#define _FRM_EVENT_THREAD_HXX_

#include <com/sun/star/awt/XControl.hpp>
#include <vos/thread.hxx>


#ifndef _OSL_CONDITN_HXX_
#include <osl/conditn.hxx>
#endif
#ifndef _CPPUHELPER_COMPONENT_HXX_
#include <cppuhelper/component.hxx>
#endif

#ifndef _COMPHELPER_STLTYPES_HXX_
#include <comphelper/stl_types.hxx>
#endif
#ifndef _COMPHELPER_GUARDING_HXX_
#include <comphelper/guarding.hxx>
#endif
#ifndef _COMPHELPER_UNO3_HXX_
#include <comphelper/uno3.hxx>
#endif
namespace binfilter {
	using namespace ::comphelper;

//.........................................................................
namespace frm
{
//.........................................................................

// ***************************************************************************************************
// ***************************************************************************************************

typedef ::vos::OThread	OComponentEventThread_TBASE;
class OComponentEventThread
			:public OComponentEventThread_TBASE
			,public ::com::sun::star::lang::XEventListener
			,public ::cppu::OWeakObject
{
	DECLARE_STL_VECTOR(::com::sun::star::lang::EventObject*, ThreadEvents);
	DECLARE_STL_VECTOR(::com::sun::star::uno::Reference< ::com::sun::star::uno::XAdapter> , ThreadObjects);
	DECLARE_STL_VECTOR(sal_Bool,	ThreadBools);

	::osl::Mutex					m_aMutex;
	::osl::Condition 				m_aCond;			// Queue gefuellt?
	ThreadEvents 					m_aEvents;			// Event-Queue
	ThreadObjects	 				m_aControls;		// Control fuer Submit
	ThreadBools						m_aFlags;			// Flags fuer Submit/Reset

	::cppu::OComponentHelper*					m_pCompImpl;	// Implementierung des Controls
	::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent>	m_xComp;		// ::com::sun::star::lang::XComponent des Controls

protected:

	// XThread
	virtual void SAL_CALL run();

	virtual void SAL_CALL kill();
	virtual void SAL_CALL onTerminated();

	// Die folgende Methode wird gerufen um das Event unter Beruecksichtigung
	// seines Typs zu duplizieren.
	virtual ::com::sun::star::lang::EventObject* cloneEvent(const ::com::sun::star::lang::EventObject* _pEvt) const = 0;

	// Ein Event bearbeiten. Der Mutex ist dabei nicht gelockt, pCompImpl
	// bleibt aber in jedem Fall gueltig. Bei pEvt kann es sich auch um
	// einen abgeleiteten Typ handeln, naemlich den, den cloneEvent
	// zurueckgibt. rControl ist nur gesetzt, wenn beim addEvent ein
	// Control uebergeben wurde. Da das Control nur als WeakRef gehalten
	// wird kann es auch zwischenzeitlich verschwinden.
	virtual void processEvent( ::cppu::OComponentHelper* _pCompImpl,
							   const ::com::sun::star::lang::EventObject* _pEvt,
							   const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XControl>& _rControl,
							   sal_Bool _bFlag) = 0;

public:

	// UNO Anbindung
	DECLARE_UNO3_DEFAULTS(OComponentEventThread, OWeakObject);
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface(const ::com::sun::star::uno::Type& _rType) throw (::com::sun::star::uno::RuntimeException);

	OComponentEventThread(::cppu::OComponentHelper* pCompImpl);
	virtual ~OComponentEventThread();

	void addEvent( const ::com::sun::star::lang::EventObject* _pEvt, sal_Bool bFlag = sal_False );
	void addEvent( const ::com::sun::star::lang::EventObject* _pEvt, const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XControl>& rControl,
				   sal_Bool bFlag = sal_False );

	// ::com::sun::star::lang::XEventListener
	virtual void SAL_CALL disposing(const ::com::sun::star::lang::EventObject& _rSource ) throw (::com::sun::star::uno::RuntimeException);

/* resolve ambiguity : both OWeakObject and OObject have these memory operators */
	void * SAL_CALL operator new( size_t size ) throw() { return OThread::operator new(size); }
	void SAL_CALL operator delete( void * p ) throw() { OThread::operator delete(p); }

private:
	void	implStarted( );
	void	implTerminated( );
};

//.........................................................................
}	// namespace frm
//.........................................................................

}//end of namespace binfilter
#endif // _FRM_EVENT_THREAD_HXX_

