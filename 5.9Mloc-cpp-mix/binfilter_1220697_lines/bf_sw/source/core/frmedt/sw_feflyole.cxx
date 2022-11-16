/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_feflyole.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:15:27 $
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

#ifndef _SCH_DLL_HXX
#include <bf_sch/schdll.hxx>
#endif


#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _FESH_HXX
#include <fesh.hxx>
#endif
#ifndef _FLYFRM_HXX
#include <flyfrm.hxx>
#endif
#ifndef _NOTXTFRM_HXX
#include <notxtfrm.hxx>
#endif
#ifndef _NDOLE_HXX
#include <ndole.hxx>
#endif
namespace binfilter {

/*N*/ SwFlyFrm *SwFEShell::FindFlyFrm( const SvEmbeddedObject *pIPObj ) const
/*N*/ {
/*N*/ 	SwFlyFrm *pFly = FindFlyFrm();
/*N*/ 	if ( pFly && pFly->Lower() && pFly->Lower()->IsNoTxtFrm() )
/*N*/ 	{
/*?*/ 		SwOLENode *pNd = ((SwNoTxtFrm*)pFly->Lower())->GetNode()->GetOLENode();
/*?*/ 		if ( !pNd || &pNd->GetOLEObj().GetOleRef() != pIPObj )
/*?*/ 			pFly = 0;
/*N*/ 	}
/*N*/ 	else
/*N*/ 		pFly = 0;
/*N*/ 
/*N*/ 	if ( !pFly )
/*N*/ 	{
/*N*/ 		//Kein Fly oder der falsche selektiert. Ergo muessen wir leider suchen.
/*N*/ 		BOOL bExist = FALSE;
/*N*/ 		SwStartNode *pStNd;
/*N*/ 		ULONG nSttIdx = GetNodes().GetEndOfAutotext().StartOfSectionIndex() + 1,
/*N*/ 			  nEndIdx = GetNodes().GetEndOfAutotext().GetIndex();
/*N*/ 		while( nSttIdx < nEndIdx &&
/*N*/ 				0 != (pStNd = GetNodes()[ nSttIdx ]->GetStartNode()) )
/*N*/ 		{
/*N*/ 			SwNode *pNd = GetNodes()[ nSttIdx+1 ];
/*N*/ 			if ( pNd->IsOLENode() &&
/*N*/ 				//do not load Objects! must not be neccessary here
/*N*/ 				 ((SwOLENode*)pNd)->GetOLEObj().IsOleRef() &&
/*N*/ 				 &((SwOLENode*)pNd)->GetOLEObj().GetOleRef() == pIPObj )
/*N*/ 			{
/*N*/ 				bExist = TRUE;
/*N*/ 				SwFrm *pFrm = ((SwOLENode*)pNd)->GetFrm();
/*N*/ 				if ( pFrm )
/*N*/ 					pFly = pFrm->FindFlyFrm();
/*N*/ 				break;
/*N*/ 			}
/*N*/ 			nSttIdx = pStNd->EndOfSectionIndex() + 1;
/*N*/ 		}
/*N*/ 
/*N*/ 		ASSERT( bExist, "OLE-Object unknown and FlyFrm not found." );
/*N*/ 	}
/*N*/ 	return pFly;
/*N*/ }

}
