/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_wrt_fn.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 13:01:48 $
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

#ifndef _SFXITEMITER_HXX //autogen
#include <svtools/itemiter.hxx>
#endif
#ifndef _SFX_WHITER_HXX //autogen
#include <svtools/whiter.hxx>
#endif


#include "shellio.hxx"
#include "wrt_fn.hxx"

#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif

#include "node.hxx"
namespace binfilter {



Writer& Out( const SwAttrFnTab pTab, const SfxPoolItem& rHt, Writer & rWrt )
{
    USHORT nId = rHt.Which();
    ASSERT(  nId < POOLATTR_END && nId >= POOLATTR_BEGIN, "SwAttrFnTab::Out()" );
    FnAttrOut pOut;
    if( 0 != ( pOut = pTab[ nId - RES_CHRATR_BEGIN] ))
        (*pOut)( rWrt, rHt );
    return rWrt;

}

Writer& Out_SfxItemSet( const SwAttrFnTab pTab, Writer& rWrt,
                        const SfxItemSet& rSet, BOOL bDeep,
                        BOOL bTstForDefault )
{
    // erst die eigenen Attribute ausgeben
    const SfxItemPool& rPool = *rSet.GetPool();
    const SfxItemSet* pSet = &rSet;
    if( !pSet->Count() )        // Optimierung - leere Sets
    {
        if( !bDeep )
            return rWrt;
        while( 0 != ( pSet = pSet->GetParent() ) && !pSet->Count() )
            ;
        if( !pSet )
            return rWrt;
    }
    const SfxPoolItem* pItem;
    FnAttrOut pOut;
    if( !bDeep || !pSet->GetParent() )
    {
        ASSERT( rSet.Count(), "Wurde doch schon behandelt oder?" );
        SfxItemIter aIter( *pSet );
        pItem = aIter.GetCurItem();
        do {
            if( 0 != ( pOut = pTab[ pItem->Which() - RES_CHRATR_BEGIN] ))
                    (*pOut)( rWrt, *pItem );
        } while( !aIter.IsAtEnd() && 0 != ( pItem = aIter.NextItem() ) );
    }
    else
    {
        SfxWhichIter aIter( *pSet );
        register USHORT nWhich = aIter.FirstWhich();
        while( nWhich )
        {
            if( SFX_ITEM_SET == pSet->GetItemState( nWhich, bDeep, &pItem ) &&
                ( !bTstForDefault || (
                    *pItem != rPool.GetDefaultItem( nWhich )
                    || ( pSet->GetParent() &&
                        *pItem != pSet->GetParent()->Get( nWhich ))
                )) && 0 != ( pOut = pTab[ nWhich - RES_CHRATR_BEGIN] ))
                    (*pOut)( rWrt, *pItem );
            nWhich = aIter.NextWhich();
        }
    }
    return rWrt;
}

/*N*/ Writer& Out( const SwNodeFnTab pTab, SwNode& rNode, Writer & rWrt )
/*N*/ {
/*N*/ 	// es muss ein CntntNode sein !!
/*N*/ 	SwCntntNode * pCNd = rNode.GetCntntNode();
/*N*/ 	if( !pCNd )
/*?*/ 		return rWrt;
/*N*/
/*N*/ 	USHORT nId = RES_TXTNODE;
/*N*/ 	switch( pCNd->GetNodeType() )
/*N*/ 	{
/*N*/ 	case ND_TEXTNODE:   nId = RES_TXTNODE;  break;
/*?*/ 	case ND_GRFNODE:    nId = RES_GRFNODE;  break;
/*?*/ 	case ND_OLENODE:    nId = RES_OLENODE;  break;
/*?*/ 	default:
/*?*/ 		ASSERT( FALSE, "was fuer ein Node ist es denn nun?" );
/*N*/ 	}
/*N*/ 	FnNodeOut pOut;
/*N*/ 	if( 0 != ( pOut = pTab[ nId - RES_NODE_BEGIN ] ))
/*N*/ 		(*pOut)( rWrt, *pCNd );
/*N*/ 	return rWrt;
/*N*/ }


}
