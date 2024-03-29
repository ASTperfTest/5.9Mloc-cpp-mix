/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: cellranges.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 11:41:48 $
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
#ifndef _SCH_CELLRANGES_HXX_
#define _SCH_CELLRANGES_HXX_

#ifndef _SAL_TYPES_H_
#include <sal/types.h>
#endif
#ifndef _RTL_USTRING_HXX_
#include <rtl/ustring.hxx>
#endif
#include <vector>
namespace binfilter {

// Structures for CellRangeAddresses from container documents (Writer/Calc)
// ========================================================================

struct SchSingleCell
{
    sal_Int32 mnColumn, mnRow;
    sal_Bool  mbRelativeColumn : 1;     // if true the $-sign before the address is skipped in the XML format
    sal_Bool  mbRelativeRow    : 1;     // see above

    SchSingleCell() :
            mnColumn( -1 ),
            mnRow( -1 ),
            mbRelativeColumn( sal_False ),
            mbRelativeRow( sal_False )
        {}
};

struct SchCellAddress
{
    ::std::vector< SchSingleCell > maCells;      // to handle subtables (Writer)
};

struct SchCellRangeAddress
{
    SchCellAddress maUpperLeft;
    SchCellAddress maLowerRight;    // range is one cell if this vector is empty (or equal to aUpperLeft)
    ::rtl::OUString msTableName;
    sal_Int32 mnTableNumber;        // to reconstruct old address string in Calc

    SchCellRangeAddress() :
            mnTableNumber( -1 )
        {}
};

struct SchChartRange
{
    ::std::vector< SchCellRangeAddress > maRanges;    // a number of ranges describing the complete source data for the chart
    sal_Bool mbFirstColumnContainsLabels;
    sal_Bool mbFirstRowContainsLabels;
    sal_Bool mbKeepCopyOfData;                        // if this is true, the chart will export a table containing the current data

    SchChartRange() :
            mbFirstColumnContainsLabels( sal_False ),
            mbFirstRowContainsLabels( sal_False ),
            mbKeepCopyOfData( sal_True )
        {}
};

} //namespace binfilter
#endif	// _SCH_CELLRANGES_HXX_
