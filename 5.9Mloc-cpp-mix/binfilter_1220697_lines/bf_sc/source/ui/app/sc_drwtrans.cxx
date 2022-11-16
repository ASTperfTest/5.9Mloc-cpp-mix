/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sc_drwtrans.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:01:50 $
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

// INCLUDE ---------------------------------------------------------------

#ifdef PCH
#endif

#ifdef _MSC_VER
#pragma hdrstop
#endif




#include <svtools/itempool.hxx>

#include "drwtrans.hxx"
#include "scmod.hxx"

// #108584#
#include "scitems.hxx"

// #108584#

// #108584#
namespace binfilter {

using namespace ::com::sun::star;

// -----------------------------------------------------------------------

#define SCDRAWTRANS_TYPE_EMBOBJ         1
#define SCDRAWTRANS_TYPE_DRAWMODEL      2

// -----------------------------------------------------------------------

// -----------------------------------------------------------------------



// static
/*N*/ ScDrawTransferObj* ScDrawTransferObj::GetOwnClipboard( Window* )
/*N*/ {
/*N*/     ScDrawTransferObj* pObj = SC_MOD()->GetClipData().pDrawClipboard;
/*N*/     return pObj;
/*N*/ }














//
//  initialize aDocShellRef with a live document from the ClipDoc
//



}
