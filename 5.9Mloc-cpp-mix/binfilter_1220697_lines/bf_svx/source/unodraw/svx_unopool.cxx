/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svx_unopool.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 21:56:34 $
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


#include <comphelper/propertysetinfo.hxx>

#ifndef _RTL_UUID_H_ 
#include <rtl/uuid.h>
#endif

#ifndef _SV_SVAPP_HXX 
#include <vcl/svapp.hxx>
#endif

#ifndef _SVX_UNOPOOL_HXX_
#include "unopool.hxx"
#endif
#ifndef _SVDMODEL_HXX 
#include "svdmodel.hxx"
#endif
#ifndef _SVDOBJ_HXX 
#include "svdobj.hxx"
#endif
#ifndef _SVX_UNOSHPRP_HXX 
#include "unoshprp.hxx"
#endif
#ifndef _SVX_XFLBSTIT_HXX 
#include "xflbstit.hxx"
#endif
#ifndef _SVX_XFLBMTIT_HXX 
#include "xflbmtit.hxx"
#endif
#ifndef _SVDETC_HXX 
#include "svdetc.hxx"
#endif
#ifndef _MyEDITENG_HXX 
#include "editeng.hxx"
#endif

#include "unoapi.hxx"
#include <memory>
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::beans;
using namespace ::rtl;
using namespace ::cppu;

SvxUnoDrawPool::SvxUnoDrawPool( SdrModel* pModel, sal_Int32 nServiceId ) throw()
: PropertySetHelper( SvxPropertySetInfoPool::getOrCreate( nServiceId ) ), mpModel( pModel )
{
	init();
}

/* deprecated */
SvxUnoDrawPool::SvxUnoDrawPool( SdrModel* pModel ) throw()
: PropertySetHelper( SvxPropertySetInfoPool::getOrCreate( SVXUNO_SERVICEID_COM_SUN_STAR_DRAWING_DEFAULTS ) ), mpModel( pModel )
{
	init();
}

SvxUnoDrawPool::~SvxUnoDrawPool() throw()
{
	delete mpDefaultsPool;
}

void SvxUnoDrawPool::init()
{
	mpDefaultsPool = new SdrItemPool(SDRATTR_START, SDRATTR_END );
	SfxItemPool* pOutlPool=EditEngine::CreatePool();
	mpDefaultsPool->SetSecondaryPool(pOutlPool);

//	mpDefaultsPool = new SdrItemPool( SdrObject::GetGlobalDrawObjectItemPool() );
	SdrModel::SetTextDefaults( mpDefaultsPool, SdrEngineDefaults::GetFontHeight() );
	mpDefaultsPool->SetDefaultMetric((SfxMapUnit)SdrEngineDefaults::GetMapUnit());
	mpDefaultsPool->FreezeIdRanges();
}

SfxItemPool* SvxUnoDrawPool::getModelPool( sal_Bool bReadOnly ) throw()
{
	if( mpModel	)
	{
		return &mpModel->GetItemPool();
	}
	else
	{
		if( bReadOnly )
			return mpDefaultsPool;
		else
			return NULL;
	}
}

void SvxUnoDrawPool::getAny( SfxItemPool* pPool, const comphelper::PropertyMapEntry* pEntry, Any& rValue )
	throw(UnknownPropertyException)
{
	switch( pEntry->mnHandle )
	{
	case OWN_ATTR_FILLBMP_MODE:
		{
			XFillBmpStretchItem* pStretchItem = (XFillBmpStretchItem*)&pPool->GetDefaultItem(XATTR_FILLBMP_STRETCH);
			XFillBmpTileItem* pTileItem = (XFillBmpTileItem*)&pPool->GetDefaultItem(XATTR_FILLBMP_TILE);
			if( pTileItem && pTileItem->GetValue() )
			{
				rValue <<= drawing::BitmapMode_REPEAT;
			}
			else if( pStretchItem && pStretchItem->GetValue() )
			{
				rValue <<= drawing::BitmapMode_STRETCH;
			}
			else
			{
				rValue <<= drawing::BitmapMode_NO_REPEAT;
			}
			break;
		}
	default:
		{
			const SfxMapUnit eMapUnit = pPool ? pPool->GetMetric((USHORT)pEntry->mnHandle) : SFX_MAPUNIT_100TH_MM;

			BYTE nMemberId = pEntry->mnMemberId & (~SFX_METRIC_ITEM);
			if( eMapUnit == SFX_MAPUNIT_100TH_MM )
				nMemberId &= (~CONVERT_TWIPS);

			pPool->GetDefaultItem( (USHORT)pEntry->mnHandle ).QueryValue( rValue, nMemberId );
		}
	}


	// check for needed metric translation
	const SfxMapUnit eMapUnit = pPool->GetMetric((USHORT)pEntry->mnHandle);
	if(pEntry->mnMemberId & SFX_METRIC_ITEM && eMapUnit != SFX_MAPUNIT_100TH_MM)
	{
		SvxUnoConvertToMM( eMapUnit, rValue );
	}
	// convert int32 to correct enum type if needed
	else if ( pEntry->mpType->getTypeClass() == TypeClass_ENUM && rValue.getValueType() == ::getCppuType((const sal_Int32*)0) )
	{
		sal_Int32 nEnum;
		rValue >>= nEnum;

		rValue.setValue( &nEnum, *pEntry->mpType );
	}
}

void SvxUnoDrawPool::putAny( SfxItemPool* pPool, const comphelper::PropertyMapEntry* pEntry, const Any& rValue )
	throw(UnknownPropertyException, IllegalArgumentException)
{
	Any aValue( rValue );

	const SfxMapUnit eMapUnit = pPool->GetMetric((USHORT)pEntry->mnHandle);
	if(pEntry->mnMemberId & SFX_METRIC_ITEM && eMapUnit != SFX_MAPUNIT_100TH_MM)
	{
		SvxUnoConvertFromMM( eMapUnit, aValue );
	}

	const sal_uInt16 nWhich = (sal_uInt16)pEntry->mnHandle;
	switch( nWhich )
	{
		case OWN_ATTR_FILLBMP_MODE:
			do
			{
				drawing::BitmapMode eMode;
				if(!(aValue >>= eMode) )
				{
					sal_Int32 nMode;
					if(!(aValue >>= nMode))
						throw IllegalArgumentException();

					eMode = (drawing::BitmapMode)nMode;
				}

				pPool->SetPoolDefaultItem( XFillBmpStretchItem( eMode == drawing::BitmapMode_STRETCH ) );
				pPool->SetPoolDefaultItem( XFillBmpTileItem( eMode == drawing::BitmapMode_REPEAT ) );
				return;
			}
			while(0);
			break;

	default:
		{
			::std::auto_ptr<SfxPoolItem> pNewItem( pPool->GetDefaultItem( nWhich ).Clone() );

			const SfxMapUnit eMapUnit = pPool ? pPool->GetMetric(nWhich) : SFX_MAPUNIT_100TH_MM;

			BYTE nMemberId = pEntry->mnMemberId & (~SFX_METRIC_ITEM);
			if( eMapUnit == SFX_MAPUNIT_100TH_MM )
				nMemberId &= (~CONVERT_TWIPS);

			if( !pNewItem->PutValue( aValue, nMemberId ) )
				throw IllegalArgumentException();

			pPool->SetPoolDefaultItem( *pNewItem );
		}
	}
}

void SvxUnoDrawPool::_setPropertyValues( const comphelper::PropertyMapEntry** ppEntries, const Any* pValues )
	throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	SfxItemPool* pPool = getModelPool( sal_False );

	DBG_ASSERT( pPool, "I need a SfxItemPool!" );
	if( NULL == pPool )
		throw UnknownPropertyException();

	while( *ppEntries )
		putAny( pPool, *ppEntries++, *pValues++ );
}

void SvxUnoDrawPool::_getPropertyValues( const comphelper::PropertyMapEntry** ppEntries, Any* pValue )
	throw(UnknownPropertyException, WrappedTargetException )
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	SfxItemPool* pPool = getModelPool( sal_True );

	DBG_ASSERT( pPool, "I need a SfxItemPool!" );
	if( NULL == pPool )
		throw UnknownPropertyException();

	while( *ppEntries )
		getAny( pPool, *ppEntries++, *pValue++ );
}

void SvxUnoDrawPool::_getPropertyStates( const comphelper::PropertyMapEntry** ppEntries, PropertyState* pStates )
	throw(UnknownPropertyException )
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	SfxItemPool* pPool = getModelPool( sal_True );

	if( pPool && pPool != mpDefaultsPool )
	{
		while( *ppEntries )
		{
			const sal_uInt16 nWhich = (sal_uInt16)(*ppEntries)->mnHandle;

			switch( nWhich )
			{
			case OWN_ATTR_FILLBMP_MODE:
				{
					if( pPool->GetDefaultItem( XATTR_FILLBMP_STRETCH ) == mpDefaultsPool->GetDefaultItem( XATTR_FILLBMP_STRETCH ) ||
						pPool->GetDefaultItem( XATTR_FILLBMP_TILE ) == mpDefaultsPool->GetDefaultItem( XATTR_FILLBMP_TILE ) )
					{
						*pStates = beans::PropertyState_DEFAULT_VALUE;
					}
					else
					{
						*pStates = beans::PropertyState_DIRECT_VALUE;
					}
				}
				break;
			default:
				const SfxPoolItem& r1 = pPool->GetDefaultItem( nWhich );
				const SfxPoolItem& r2 = mpDefaultsPool->GetDefaultItem( nWhich );

				if( r1 == r2 )
				{
					*pStates = PropertyState_DEFAULT_VALUE;
				}
				else
				{
					*pStates = PropertyState_DIRECT_VALUE;
				}
			}

			pStates++;
			ppEntries++;
		}
	}
	else
	{
		// as long as we have no model, all properties are default
		while( *ppEntries++ )
			*pStates++ = PropertyState_DEFAULT_VALUE;
		return;
	}
}

void SvxUnoDrawPool::_setPropertyToDefault( const comphelper::PropertyMapEntry* pEntry )
	throw(UnknownPropertyException )
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	SfxItemPool* pPool = getModelPool( sal_True );

	if( pPool && pPool != mpDefaultsPool )
		pPool->Put( mpDefaultsPool->GetDefaultItem( (USHORT)pEntry->mnHandle ), (USHORT)pEntry->mnHandle );
}

Any SvxUnoDrawPool::_getPropertyDefault( const comphelper::PropertyMapEntry* pEntry )
	throw(UnknownPropertyException, WrappedTargetException )
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	Any aAny;
	getAny( mpDefaultsPool, pEntry, aAny );
	return aAny;
}

// XInterface

Any SAL_CALL SvxUnoDrawPool::queryInterface( const Type & rType )
	throw( RuntimeException )
{
	return OWeakAggObject::queryInterface( rType );
}

Any SAL_CALL SvxUnoDrawPool::queryAggregation( const Type & rType )
	throw(RuntimeException)
{
	Any aAny;

	if( rType == ::getCppuType((const Reference< XServiceInfo >*)0) )
		aAny <<= Reference< XServiceInfo >(this);
	else if( rType == ::getCppuType((const Reference< XTypeProvider >*)0) )
		aAny <<= Reference< XTypeProvider >(this);
	else if( rType == ::getCppuType((const Reference< XPropertySet >*)0) )
		aAny <<= Reference< XPropertySet >(this);
	else if( rType == ::getCppuType((const Reference< XPropertyState >*)0) )
		aAny <<= Reference< XPropertyState >(this);
	else if( rType == ::getCppuType((const Reference< XMultiPropertySet >*)0) )
		aAny <<= Reference< XMultiPropertySet >(this);
	else
		aAny <<= OWeakAggObject::queryAggregation( rType );

	return aAny;
}

void SAL_CALL SvxUnoDrawPool::acquire() throw ( )
{
	OWeakAggObject::acquire();
}

void SAL_CALL SvxUnoDrawPool::release() throw ( )
{
	OWeakAggObject::release();
}

uno::Sequence< uno::Type > SAL_CALL SvxUnoDrawPool::getTypes()
	throw (uno::RuntimeException)
{
	uno::Sequence< uno::Type > aTypes( 6 );
	uno::Type* pTypes = aTypes.getArray();

	*pTypes++ = ::getCppuType((const uno::Reference< XAggregation>*)0);
	*pTypes++ = ::getCppuType((const uno::Reference< XServiceInfo>*)0);
	*pTypes++ = ::getCppuType((const uno::Reference< XTypeProvider>*)0);
	*pTypes++ = ::getCppuType((const uno::Reference< XPropertySet>*)0);
	*pTypes++ = ::getCppuType((const uno::Reference< XPropertyState>*)0);
	*pTypes++ = ::getCppuType((const uno::Reference< XMultiPropertySet>*)0);

	return aTypes;
}

uno::Sequence< sal_Int8 > SAL_CALL SvxUnoDrawPool::getImplementationId()
	throw (uno::RuntimeException)
{
	vos::OGuard aGuard( Application::GetSolarMutex() );

	static uno::Sequence< sal_Int8 > aId;
	if( aId.getLength() == 0 )
	{
		aId.realloc( 16 );
		rtl_createUuid( (sal_uInt8 *)aId.getArray(), 0, sal_True );
	}
	return aId;
}

// XServiceInfo

sal_Bool SAL_CALL SvxUnoDrawPool::supportsService( const  OUString& ServiceName ) throw(RuntimeException)
{
    Sequence< OUString > aSNL( getSupportedServiceNames() );
    const OUString * pArray = aSNL.getConstArray();

    for( INT32 i = 0; i < aSNL.getLength(); i++ )
        if( pArray[i] == ServiceName )
            return TRUE;

    return FALSE;
}

OUString SAL_CALL SvxUnoDrawPool::getImplementationName() throw( RuntimeException )
{
	return OUString( RTL_CONSTASCII_USTRINGPARAM("SvxUnoDrawPool") );
}

Sequence< OUString > SAL_CALL SvxUnoDrawPool::getSupportedServiceNames(  )
	throw( RuntimeException )
{
    Sequence< OUString > aSNS( 1 );
    aSNS.getArray()[0] = OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.drawing.Defaults" ));
    return aSNS;
}
}
