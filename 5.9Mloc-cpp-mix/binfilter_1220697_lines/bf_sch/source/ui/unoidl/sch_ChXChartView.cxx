/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sch_ChXChartView.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:09:46 $
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

#include "ChXChartAxis.hxx"
#include "ChXChartView.hxx"
#include "ChXDataPoint.hxx"
#include "ChXDataRow.hxx"

#ifndef _COMPHELPER_EXTRACT_HXX_
#include <comphelper/extract.hxx>
#endif

// header for class OGuard

#include "viewshel.hxx"
#include "schview.hxx"
#include "mapprov.hxx"
#include "datarow.hxx"
#include "datapoin.hxx"
#include "objid.hxx"

#ifndef _COM_SUN_STAR_CHART_XCHARTDOCUMENT_HPP_ 
#include <com/sun/star/chart/XChartDocument.hpp>
#endif

#include "ChXDiagram.hxx"
#ifndef _RTL_UUID_H_
#include <rtl/uuid.h>
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::vos;
using namespace ::cppu;

extern SchUnoPropertyMapProvider aSchMapProvider;

ChXChartView::ChXChartView( SchViewShell* pSchViewShell ) :
    SfxBaseController( pSchViewShell ),
    mpViewShell( pSchViewShell ),
    maSelectionListenerManager( m_aViewMutex )
{
}

ChXChartView::~ChXChartView()
{
}

// XSelectionSupplier
sal_Bool SAL_CALL ChXChartView::select( const uno::Any& aSelection )
	throw( lang::IllegalArgumentException, uno::RuntimeException )
{
    SchViewShell* pViewShell = NULL;
    {
        ::osl::MutexGuard aGuard( m_aViewMutex );
        pViewShell = mpViewShell;
    }
    if( ! pViewShell )
        return sal_False;

	// extract an XInterface from aSelection
	uno::Reference< uno::XInterface > xInterface;
	if( ! cppu::extractInterface( xInterface, aSelection ) || ! xInterface.is() )
		return sal_False;

    OGuard aSolarGuard( Application::GetSolarMutex());
	SchView* pView = pViewShell->GetView();
	SdrObject* pObj = NULL;
	long nObjId = 0, nCol = 0, nRow = 0;

	// try to get a chart object
	uno::Reference< lang::XUnoTunnel > xUnoTunnel( xInterface, uno::UNO_QUERY );
	if( xUnoTunnel.is())
	{
		// do not use getImplementation - xInterface could be a derived class
		ChXChartObject* pXObject = (ChXChartObject*)xUnoTunnel->getSomething( ChXChartObject::getUnoTunnelId());
		if( pXObject )
			nObjId = pXObject->GetId();
	}

	if( pView )
	{
		ChartModel& aModel = pView->GetDoc();
		if( nObjId )
		{
			pObj   = aModel.GetObjectWithId( nObjId );
		}
		else
		{
			// no ChXChartObject and no ChXChartAxis, try DataRow, DataPoint
			ChXDataRow*	pXRow = ChXDataRow::getImplementation( xInterface );
			if( pXRow )
			{
				pObj = aModel.GetDataRowObj( pXRow->GetRow() );
			}
			else 
			{
				ChXDataPoint* pXPoint = ChXDataPoint::getImplementation( xInterface );
				if( pXPoint )
				{
					pObj = aModel.GetDataPointObj( pXPoint->GetCol(), pXPoint->GetRow() );
				}
			}

			if( pObj )
			{
				SchObjectId* pId = GetObjectId( *pObj );
				if( pId )
					nObjId = pId->GetObjId();
			}
		}
	}

	if( pObj )			// we found an implementation object
	{
        SuspendSelectionChangeBroadcasting aGuard( mpViewShell );
		pView->UnmarkAll ();
		pView->LeaveAllGroup ();

		pView->SetMarkHdlHidden( TRUE );
		pView->MarkObj( pObj, pView->GetPageViewPvNum( 0 ));
		pView->SetMarkHdlHidden( FALSE );

		return sal_True;
	}
	else
	{
		DBG_WARNING( "ChXChartView::select - selected object couldn't be resolved" );
	}
	return sal_False;
}

uno::Any SAL_CALL ChXChartView::getSelection() throw( uno::RuntimeException )
{
    uno::Reference< uno::XInterface > xIntf;
	uno::Any aAny;
	aAny <<= xIntf;

    SchViewShell* pViewShell = NULL;
    {
        ::osl::MutexGuard aGuard( m_aViewMutex );
        pViewShell = mpViewShell;
    }
    if( ! pViewShell )
        return aAny;

	OGuard aSolarGuard( Application::GetSolarMutex());
	SchView* pView = pViewShell->GetView();
	if( ! pView )
		return aAny;

	const SdrMarkList& rMarkList  = pView->GetMarkList();
	SdrObject*   		pObj	  = NULL;
	SchObjectId*		pObjId	  = NULL;
	ChartModel*			pModel	  = NULL;
	SfxObjectShell*		pDocShell = NULL;
	SdrMark*			pMark	  = rMarkList.GetMark( 0 );

	if( pMark )
	{
		pObj = pMark->GetObj();

		if( pObj )
		{
			pObjId = GetObjectId( *pObj );
			pModel = (ChartModel*)(pObj->GetModel());
			if( pModel )
				pDocShell = pModel->GetObjectShell();
		}
	}

	if( ! pObjId || ! pDocShell )
		return aAny;

	uno::Reference< chart::XChartDocument > xDoc( pDocShell->GetBaseModel(), uno::UNO_QUERY );
	if( xDoc.is())
	{
		long nObjId = pObjId->GetObjId();

		switch( nObjId )
		{
			// provided by XChartDocument
			case CHOBJID_TITLE_MAIN:
				aAny <<= xDoc->getTitle();		break;
			case CHOBJID_TITLE_SUB:
				aAny <<= xDoc->getSubTitle();	break;
			case CHOBJID_LEGEND:
				aAny <<= xDoc->getLegend();		break;
			case CHOBJID_DIAGRAM_AREA:
				aAny <<= xDoc->getArea();		break;
			case CHOBJID_DIAGRAM:
				aAny <<= xDoc->getDiagram();	break;

			// data rows
			case CHOBJID_DIAGRAM_SPECIAL_GROUP:
			case CHOBJID_DIAGRAM_ROWGROUP:
			case CHOBJID_LEGEND_SYMBOL_ROW:
				{
					SchDataRow* pDataRow = GetDataRow( *pObj );

					if( pDataRow )
					{
						long nRow = pDataRow->GetRow();
						uno::Reference< beans::XPropertySet > xRef( new ChXDataRow( nRow, pModel ));
						aAny <<= xRef;
					}
					else
						DBG_ERROR( "Couldn't resolve data row object from SdrObject" );
				}
				break;

			// data points
			case CHOBJID_DIAGRAM_DATA :
			case CHOBJID_LEGEND_SYMBOL_COL:
				{
					SchDataPoint* pDataPoint = GetDataPoint( *pObj );

					if( pDataPoint )
					{
						long nRow = pDataPoint->GetRow();
						long nCol = pDataPoint->GetCol();
						uno::Reference< beans::XPropertySet > xRef( new ChXDataPoint( nCol, nRow, pModel ));
						aAny <<= xRef;
					}
					else
						DBG_ERROR( "Couldn't resolve data point object from SdrObject" );
				}
				break;

			case CHOBJID_DIAGRAM_STACKEDGROUP:
			case CHOBJID_DIAGRAM_STATISTICS_GROUP:
			case CHOBJID_DIAGRAM_ROWSLINE:
			case CHOBJID_DIAGRAM_ROWS:
			case CHOBJID_DIAGRAM_AVERAGEVALUE :
			case CHOBJID_DIAGRAM_REGRESSION :
			case CHOBJID_DIAGRAM_ERROR :
				DBG_WARNING1( "Object with Id %d is not supported in ChXChartView::getSelection()", nObjId );
				break;

			// objects available by ChXDiagram
			default:
				{
					ChXDiagram* pDia = ChXDiagram::getImplementation( xDoc->getDiagram() );
					if( pDia )
					{
						switch( nObjId )
						{
							case CHOBJID_DIAGRAM_TITLE_X_AXIS:
								aAny <<= pDia->getXAxisTitle();		break;
							case CHOBJID_DIAGRAM_TITLE_Y_AXIS:
								aAny <<= pDia->getYAxisTitle();		break;
							case CHOBJID_DIAGRAM_TITLE_Z_AXIS:
								aAny <<= pDia->getZAxisTitle();		break;
							case CHOBJID_DIAGRAM_X_AXIS:
								aAny <<= pDia->getXAxis();			break;
							case CHOBJID_DIAGRAM_Y_AXIS:
								aAny <<= pDia->getYAxis();			break;
							case CHOBJID_DIAGRAM_Z_AXIS:
								aAny <<= pDia->getZAxis();			break;
							case CHOBJID_DIAGRAM_A_AXIS:
								aAny <<= pDia->getSecondaryXAxis();	break;
							case CHOBJID_DIAGRAM_B_AXIS:
								aAny <<= pDia->getSecondaryYAxis();	break;
							case CHOBJID_DIAGRAM_FLOOR:
								aAny <<= pDia->getFloor();			break;
							case CHOBJID_DIAGRAM_WALL:
								aAny <<= pDia->getWall();			break;

							case CHOBJID_DIAGRAM_Y_GRID_MAIN:
							case CHOBJID_DIAGRAM_Y_GRID_MAIN_GROUP:
								// attention: x and y grids are interchanged
								aAny <<= pDia->getXMainGrid();		break;
							case CHOBJID_DIAGRAM_X_GRID_MAIN:
							case CHOBJID_DIAGRAM_X_GRID_MAIN_GROUP:
								// attention: x and y grids are interchanged
								aAny <<= pDia->getYMainGrid();		break;
							case CHOBJID_DIAGRAM_Z_GRID_MAIN:
							case CHOBJID_DIAGRAM_Z_GRID_MAIN_GROUP:
								aAny <<= pDia->getZMainGrid();		break;
							case CHOBJID_DIAGRAM_Y_GRID_HELP:
							case CHOBJID_DIAGRAM_Y_GRID_HELP_GROUP:
								// attention: x and y grids are interchanged
								aAny <<= pDia->getXHelpGrid();		break;
							case CHOBJID_DIAGRAM_X_GRID_HELP:
							case CHOBJID_DIAGRAM_X_GRID_HELP_GROUP:
								// attention: x and y grids are interchanged
								aAny <<= pDia->getYHelpGrid();		break;
							case CHOBJID_DIAGRAM_Z_GRID_HELP:
							case CHOBJID_DIAGRAM_Z_GRID_HELP_GROUP:
								aAny <<= pDia->getZHelpGrid();		break;

							default:
								DBG_WARNING1( "Unknown object id %d", nObjId );
								break;
						}
					}
				}
		}
	}

	return aAny;
}

/// type that must be supported by selection listeners
inline const ::com::sun::star::uno::Type & lcl_getSelectionTypeIdentifier()
{
	return ::getCppuType( (uno::Reference< view::XSelectionChangeListener > *)0 );
}

void SAL_CALL ChXChartView::addSelectionChangeListener( const uno::Reference< view::XSelectionChangeListener >& xListener )
	throw( uno::RuntimeException )
{
	maSelectionListenerManager.addListener( lcl_getSelectionTypeIdentifier(), xListener );
}

void SAL_CALL ChXChartView::removeSelectionChangeListener( const uno::Reference< view::XSelectionChangeListener >& xListener )
	throw( uno::RuntimeException )
{
	maSelectionListenerManager.removeListener( lcl_getSelectionTypeIdentifier(), xListener );
}

void ChXChartView::fireSelectionChangeListener() throw()
{
	::cppu::OInterfaceContainerHelper * pLC = maSelectionListenerManager.getContainer( lcl_getSelectionTypeIdentifier() );
	if( pLC != NULL )
	{
		uno::Reference< uno::XInterface > xSource( (uno::XWeak*)this );
		const lang::EventObject aEvent( xSource );

		// send an event to all listeners
		::cppu::OInterfaceIteratorHelper aIt( *pLC );
		while( aIt.hasMoreElements() )
		{
			view::XSelectionChangeListener * pL = static_cast< view::XSelectionChangeListener * >( aIt.next() );
			pL->selectionChanged( aEvent );
		}
	}
}

// XInterface
uno::Any SAL_CALL ChXChartView::queryInterface( const uno::Type & rType ) throw( uno::RuntimeException )
{
    // ask base classes for interface
	uno::Any aAny = SfxBaseController::queryInterface( rType );
	if( aAny.hasValue() )
		return aAny;

    aAny = ViewPropertySet::queryInterface( rType );
	if( aAny.hasValue() )
		return aAny;

    // own interface
	return ::cppu::queryInterface( rType, ( view::XSelectionSupplier* ) this );
}

void SAL_CALL ChXChartView::acquire() throw()
{
	SfxBaseController::acquire();
}

void SAL_CALL ChXChartView::release() throw()
{
	SfxBaseController::release();
}

// XTypeProvider ( ::SfxBaseController )
uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL ChXChartView::getTypes()
	throw( uno::RuntimeException )
{
    ::osl::MutexGuard aGuard( m_aViewMutex );
	if( maTypeSequence.getLength() == 0 )
	{
		const uno::Sequence< uno::Type > aBaseTypes( SfxBaseController::getTypes() );
		const sal_Int32 nBaseTypes = aBaseTypes.getLength();
		const uno::Type* pBaseTypes = aBaseTypes.getConstArray();

		maTypeSequence.realloc( nBaseTypes + 1 );		// Note: Keep the size updated !!
		uno::Type* pTypes = maTypeSequence.getArray();

		*pTypes++ = ::getCppuType((const uno::Reference< view::XSelectionSupplier >*)0);

		for( sal_Int32 nType = 0; nType < nBaseTypes; nType++ )
			*pTypes++ = *pBaseTypes++;
	}

	return maTypeSequence;
}

uno::Sequence< sal_Int8 > SAL_CALL ChXChartView::getImplementationId()
	throw( uno::RuntimeException )
{
	static uno::Sequence< sal_Int8 > aId;
	if( aId.getLength() == 0 )
	{
		aId.realloc( 16 );
		rtl_createUuid( (sal_uInt8 *)aId.getArray(), 0, sal_True );
	}
	return aId;
}

void SAL_CALL ChXChartView::dispose() throw( uno::RuntimeException )
{
    ::osl::MutexGuard aGuard( m_aViewMutex );
    mpViewShell = NULL;

    SfxBaseController::dispose();
}
}
