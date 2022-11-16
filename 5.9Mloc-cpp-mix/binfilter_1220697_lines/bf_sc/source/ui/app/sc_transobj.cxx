/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sc_transobj.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:03:11 $
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

#include "scitems.hxx"
#define ITEMID_FIELD EE_FEATURE_FIELD


#include <bf_sfx2/app.hxx>

#include "transobj.hxx"
#include "scmod.hxx"

// for InitDocShell
#include "cellsuno.hxx"
namespace binfilter {

using namespace ::com::sun::star;

// -----------------------------------------------------------------------

#define SCTRANS_TYPE_IMPEX			1
#define SCTRANS_TYPE_EDIT_RTF		2
#define SCTRANS_TYPE_EDIT_BIN		3
#define SCTRANS_TYPE_EMBOBJ			4

// -----------------------------------------------------------------------

// static

// static

// -----------------------------------------------------------------------



// static
/*N*/ ScTransferObj* ScTransferObj::GetOwnClipboard( Window* pUIWin )
/*N*/ {
/*N*/ 	ScTransferObj* pObj = SC_MOD()->GetClipData().pCellClipboard;
/*N*/ 	if ( pObj && pUIWin )
/*N*/ 	{
/*?*/ 		//	check formats to see if pObj is really in the system clipboard
/*?*/ 
/*?*/ 		//	pUIWin is NULL when called from core (IsClipboardSource),
/*?*/ 		//	in that case don't access the system clipboard, because the call
/*?*/ 		//	may be from other clipboard operations (like flushing, #86059#)
/*?*/ 
/*?*/ 		TransferableDataHelper aDataHelper( TransferableDataHelper::CreateFromSystemClipboard( pUIWin ) );
/*?*/ 		if ( !aDataHelper.HasFormat( SOT_FORMATSTR_ID_DIF ) )
/*?*/ 		{
/*?*/ //			DBG_ERROR("ScTransferObj wasn't released");
/*?*/ 			pObj = NULL;
/*?*/ 		}
/*N*/ 	}
/*N*/ 	return pObj;
/*N*/ }
}
