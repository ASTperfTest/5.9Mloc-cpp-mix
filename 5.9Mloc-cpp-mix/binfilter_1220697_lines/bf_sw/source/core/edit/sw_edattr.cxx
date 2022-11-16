/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_edattr.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:09:28 $
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

#ifndef _HINTIDS_HXX
#include <hintids.hxx>
#endif

#ifndef _SVX_TSTPITEM_HXX //autogen
#include <bf_svx/tstpitem.hxx>
#endif
#ifndef _SVX_LRSPITEM_HXX //autogen
#include <bf_svx/lrspitem.hxx>
#endif
#ifndef _SVX_SCRIPTTYPEITEM_HXX
#include <bf_svx/scripttypeitem.hxx>
#endif
#ifndef _COM_SUN_STAR_I18N_SCRIPTTYPE_HDL_
#include <com/sun/star/i18n/ScriptType.hdl>
#endif


#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _EDITSH_HXX
#include <editsh.hxx>
#endif
#ifndef _EDIMP_HXX
#include <edimp.hxx>	// fuer MACROS
#endif
#ifndef _NDTXT_HXX
#include <ndtxt.hxx>
#endif
#ifndef _EXPFLD_HXX
#include <expfld.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _PAM_HXX
#include <pam.hxx>
#endif
#ifndef _BREAKIT_HXX
#include <breakit.hxx>
#endif
#ifndef _TXTFLD_HXX
#include <txtfld.hxx>
#endif
#ifndef _FMTFLD_HXX
#include <fmtfld.hxx>
#endif
#ifndef _CRSSKIP_HXX
#include <crsskip.hxx>
#endif
#ifndef _TXTFRM_HXX
#include <txtfrm.hxx>		// SwTxtFrm
#endif
#ifndef _DRAWFONT_HXX
#include <drawfont.hxx>
#endif
namespace binfilter {

using namespace ::com::sun::star::i18n;

/*************************************
 * harte Formatierung (Attribute)
 *************************************/

// wenn Selektion groesser Max Nodes oder mehr als Max Selektionen
// => keine Attribute
/*N*/ const USHORT& getMaxLookup()
/*N*/ {
/*N*/ 	static const USHORT nMaxLookup = 1000;
/*N*/ 	return nMaxLookup;
/*N*/ }

/*N*/ BOOL SwEditShell::GetAttr( SfxItemSet& rSet ) const
/*N*/ {
/*N*/ 	if( GetCrsrCnt() > getMaxLookup() )
/*N*/ 	{
/*?*/ 		rSet.InvalidateAllItems();
/*?*/ 		return FALSE;
/*N*/ 	}
/*N*/ 
/*N*/ 	SfxItemSet aSet( *rSet.GetPool(), rSet.GetRanges() );
/*N*/ 	SfxItemSet *pSet = &rSet;
/*N*/ 
/*N*/ 	FOREACHPAM_START(this)
/*N*/ 
/*N*/ 		ULONG nSttNd = PCURCRSR->GetMark()->nNode.GetIndex(),
/*N*/ 			  nEndNd = PCURCRSR->GetPoint()->nNode.GetIndex();
/*N*/ 		xub_StrLen nSttCnt = PCURCRSR->GetMark()->nContent.GetIndex(),
/*N*/ 				   nEndCnt = PCURCRSR->GetPoint()->nContent.GetIndex();
/*N*/ 
/*N*/ 		if( nSttNd > nEndNd || ( nSttNd == nEndNd && nSttCnt > nEndCnt ))
/*N*/ 		{
/*?*/ 			ULONG nTmp = nSttNd; nSttNd = nEndNd; nEndNd = nTmp;
/*?*/ 			nTmp = nSttCnt; nSttCnt = nEndCnt; nEndCnt = (xub_StrLen)nTmp;
/*N*/ 		}
/*N*/ 
/*N*/ 		if( nEndNd - nSttNd >= getMaxLookup() )
/*N*/ 		{
/*?*/ 			rSet.ClearItem();
/*?*/ 			rSet.InvalidateAllItems();
/*?*/ 			return FALSE;
/*N*/ 		}
/*N*/ 
/*N*/ 		// beim 1.Node traegt der Node die Werte in den GetSet ein (Initial)
/*N*/ 		// alle weiteren Nodes werden zum GetSet zu gemergt
/*N*/ 		for( ULONG n = nSttNd; n <= nEndNd; ++n )
/*N*/ 		{
/*N*/ 			SwNode* pNd = GetDoc()->GetNodes()[ n ];
/*N*/ 			switch( pNd->GetNodeType() )
/*N*/ 			{
/*N*/ 			case ND_TEXTNODE:
/*N*/ 				{
/*N*/ 					xub_StrLen nStt = n == nSttNd ? nSttCnt : 0,
/*N*/ 						   	   nEnd = n == nEndNd ? nEndCnt
/*N*/ 										: ((SwTxtNode*)pNd)->GetTxt().Len();
/*N*/ 					((SwTxtNode*)pNd)->GetAttr( *pSet, nStt, nEnd );
/*N*/ 				}
/*N*/ 				break;
/*N*/ 			case ND_GRFNODE:
/*N*/ 			case ND_OLENODE:
/*?*/ 				((SwCntntNode*)pNd)->GetAttr( *pSet );
/*?*/ 				break;
/*?*/ 
/*?*/ 			default:
/*?*/ 				pNd = 0;
/*N*/ 			}
/*N*/ 
/*N*/ 			if( pNd )
/*N*/ 			{
/*N*/ 				if( pSet != &rSet )
/*?*/ 					rSet.MergeValues( aSet );
/*N*/ 
/*N*/ 				if( aSet.Count() )
/*?*/ 					aSet.ClearItem();
/*N*/ 
/*N*/ #ifdef JP_NEWCORE
/*N*/ 			// vieleicht sollte man hier noch erfragen, ob schon alle Attribute
/*N*/ 			// "DontCare" sind. Dann kann man abbrechen!
/*N*/ #endif
/*N*/ 			}
/*N*/ 			pSet = &aSet;
/*N*/ 		}
/*N*/ 
/*N*/ 	FOREACHPAM_END()
/*N*/ 
/*N*/ 	return TRUE;
/*N*/ }




/*N*/ SwTxtFmtColl* SwEditShell::GetCurTxtFmtColl() const
/*N*/ {
/*N*/ 	SwTxtFmtColl *pFmt = 0;
/*N*/ 
/*N*/ 	if ( GetCrsrCnt() > getMaxLookup() )
/*?*/ 		return 0;
/*N*/ 
/*N*/ 	FOREACHPAM_START(this)
/*N*/ 
/*N*/ 		ULONG nSttNd = PCURCRSR->GetMark()->nNode.GetIndex(),
/*N*/ 			  nEndNd = PCURCRSR->GetPoint()->nNode.GetIndex();
/*N*/ 		xub_StrLen nSttCnt = PCURCRSR->GetMark()->nContent.GetIndex(),
/*N*/ 				   nEndCnt = PCURCRSR->GetPoint()->nContent.GetIndex();
/*N*/ 
/*N*/ 		if( nSttNd > nEndNd || ( nSttNd == nEndNd && nSttCnt > nEndCnt ))
/*N*/ 		{
/*?*/ 			ULONG nTmp = nSttNd; nSttNd = nEndNd; nEndNd = nTmp;
/*?*/ 			nTmp = nSttCnt; nSttCnt = nEndCnt; nEndCnt = (xub_StrLen)nTmp;
/*N*/ 		}
/*N*/ 
/*N*/ 		if( nEndNd - nSttNd >= getMaxLookup() )
/*N*/ 		{
/*?*/ 			pFmt = 0;
/*?*/ 			break;
/*N*/ 		}
/*N*/ 
/*N*/ 		for( ULONG n = nSttNd; n <= nEndNd; ++n )
/*N*/ 		{
/*N*/ 			SwNode* pNd = GetDoc()->GetNodes()[ n ];
/*N*/ 			if( pNd->IsTxtNode() )
/*N*/ 			{
/*N*/ 				if( !pFmt )
/*N*/ 					pFmt = ((SwTxtNode*)pNd)->GetTxtColl();
/*N*/ 				else if( pFmt == ((SwTxtNode*)pNd)->GetTxtColl() ) // ???
/*?*/ 					break;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
/*N*/ 	FOREACHPAM_END()
/*N*/ 	return pFmt;
/*N*/ }
}
