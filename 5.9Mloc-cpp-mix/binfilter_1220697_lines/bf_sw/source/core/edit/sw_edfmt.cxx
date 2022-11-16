/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_edfmt.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:09:59 $
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

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#include "doc.hxx"
#include "editsh.hxx"

#ifndef _FCHRFMT_HXX //autogen
#include <fchrfmt.hxx>
#endif
namespace binfilter {

/*************************************
 * Formate
 *************************************/
// Char
// OPT: inline






/*N*/ SwCharFmt* SwEditShell::GetCurCharFmt() const
/*N*/ {
/*N*/ 	SwCharFmt *pFmt = 0;
/*N*/ 	SfxItemSet aSet( GetDoc()->GetAttrPool(), RES_TXTATR_CHARFMT,
/*N*/ 												RES_TXTATR_CHARFMT );
/*N*/ 	const SfxPoolItem* pItem;
/*N*/ 	if( GetAttr( aSet ) && SFX_ITEM_SET ==
/*N*/ 		aSet.GetItemState( RES_TXTATR_CHARFMT, FALSE, &pItem ) )
/*N*/ 		pFmt = ((SwFmtCharFmt*)pItem)->GetCharFmt();
/*N*/ 
/*N*/ 	return pFmt;
/*N*/ }



// Frm





//----------------------------------
// inlines im Product




	// return das geforderte automatische  Format - Basis-Klasse !















}
