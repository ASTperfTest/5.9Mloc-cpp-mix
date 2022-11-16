/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_fedesc.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:15:03 $
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

#ifndef _FESH_HXX
#include <fesh.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _PAGEFRM_HXX
#include <pagefrm.hxx>
#endif
#ifndef _ROOTFRM_HXX
#include <rootfrm.hxx>
#endif
#ifndef _CNTFRM_HXX
#include <cntfrm.hxx>
#endif
namespace binfilter {
/*************************************************************************
|*
|*	SwFEShell::GetPageDescCnt()
|*
|*	Ersterstellung		MA 25. Jan. 93
|*	Letzte Aenderung	MA 25. Jan. 93
|*
|*************************************************************************/

/*N*/ USHORT SwFEShell::GetPageDescCnt() const
/*N*/ {
/*N*/ 	return GetDoc()->GetPageDescCnt();
/*N*/ }

/*************************************************************************
|*
|*	SwFEShell::ChgCurPageDesc()
|*
|*	Ersterstellung		ST ??
|*	Letzte Aenderung	MA 01. Aug. 94
|*
|*************************************************************************/


/*************************************************************************
|*
|*	SwFEShell::ChgPageDesc()
|*
|*	Ersterstellung		MA 25. Jan. 93
|*	Letzte Aenderung	MA 24. Jan. 95
|*
|*************************************************************************/


/*************************************************************************
|*
|*	SwFEShell::GetPageDesc(), GetCurPageDesc()
|*
|*	Ersterstellung		MA 25. Jan. 93
|*	Letzte Aenderung	MA 23. Apr. 93
|
|*************************************************************************/

/*N*/ const SwPageDesc& SwFEShell::GetPageDesc( USHORT i ) const
/*N*/ {
/*N*/ 	return GetDoc()->GetPageDesc( i );
/*N*/ }


/*M*/ USHORT SwFEShell::GetCurPageDesc( const BOOL bCalcFrm ) const
/*M*/ {
/*M*/ 	const SwFrm *pFrm = GetCurrFrm( bCalcFrm );
/*M*/ 	if ( pFrm )
/*M*/ 	{
/*M*/ 		const SwPageFrm *pPage = pFrm->FindPageFrm();
/*M*/ 		if ( pPage )
/*M*/ 		{
/*M*/ 			SwDoc *pDoc = GetDoc();
/*M*/ 			for ( USHORT i = 0; i < GetDoc()->GetPageDescCnt(); ++i )
/*M*/ 			{
/*M*/ 				if ( pPage->GetPageDesc() == &pDoc->GetPageDesc(i) )
/*M*/ 					return i;
/*M*/ 			}
/*M*/ 		}
/*M*/ 	}
/*M*/ 	return 0;
/*M*/ }

// if inside all selection only one PageDesc, return this.
// Otherwise return 0 pointer



}
