/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_RefreshListenerContainer.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 13:11:26 $
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

#ifndef _REFRESH_LISTENER_CONTAINER_HXX_
#include <RefreshListenerContainer.hxx>
#endif

#ifndef _COM_SUN_STAR_UTIL_XREFRESHLISTENER_HPP_
#include <com/sun/star/util/XRefreshListener.hpp>
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::util;

SwRefreshListenerContainer::SwRefreshListenerContainer( uno::XInterface* pxParent)
: SwEventListenerContainer ( pxParent )
{
}

void SwRefreshListenerContainer::Refreshed ()
{
	if(!pListenerArr)
		return;

	lang::EventObject aObj(pxParent);
	for(sal_uInt16 i = 0, nEnd = pListenerArr->Count(); i < nEnd ; i++)
	{
		Reference < XRefreshListener > xRefreshListener = Reference < XRefreshListener > ( *pListenerArr->GetObject(i), UNO_QUERY );
		xRefreshListener->refreshed(aObj);
	}
}
}
