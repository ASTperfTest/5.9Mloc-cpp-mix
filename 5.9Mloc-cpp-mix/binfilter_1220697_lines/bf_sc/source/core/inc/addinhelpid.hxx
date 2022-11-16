/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: addinhelpid.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/07 16:55:56 $
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

#ifndef SC_ADDINHELPID_HXX
#define SC_ADDINHELPID_HXX

#ifndef _RTL_USTRING_HXX_
#include <rtl/ustring.hxx>
#endif
namespace binfilter {


// ============================================================================

struct ScUnoAddInHelpId;

/** Generates help IDs for standard Calc AddIns. */
class ScUnoAddInHelpIdGenerator
{
private:
    const ScUnoAddInHelpId*     pCurrHelpIds;       /// Array of function names and help IDs.
    sal_uInt32                  nArrayCount;        /// Count of array entries.

public:
                                ScUnoAddInHelpIdGenerator();
                                ScUnoAddInHelpIdGenerator( const ::rtl::OUString& rServiceName );

    /** Sets service name of the AddIn. Has to be done before requesting help IDs. */
    void                        SetServiceName( const ::rtl::OUString& rServiceName );

    /** @return  The help ID of the function with given built-in name or 0 if not found. */
    sal_uInt16                  GetHelpId( const ::rtl::OUString& rFuncName ) const;
};


// ============================================================================

} //namespace binfilter
#endif
