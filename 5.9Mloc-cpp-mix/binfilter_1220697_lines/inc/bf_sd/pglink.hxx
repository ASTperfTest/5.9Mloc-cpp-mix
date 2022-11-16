/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: pglink.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 14:20:16 $
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

#ifndef _PGLINK_HXX
#define _PGLINK_HXX

#ifndef SVX_LIGHT

#ifndef _LNKBASE_HXX
#include <bf_so3/lnkbase.hxx>
#endif
#include <tools/debug.hxx> //for STRIPE
namespace binfilter {
class SdPage;



class SdPageLink : public ::so3::SvBaseLink
{
	SdPageLink(SdPage* pPg, const String& rFileName, const String& rBookmarkName) { DBG_ASSERT(0,"STRIP! SdPageLink Class is deleted");} //STRIP001 	SdPageLink(SdPage* pPg, const String& rFileName, const String& rBookmarkName);
};

#else

class SdPageLink;

#endif

} //namespace binfilter
#endif	   // _PGLINK_HXX



