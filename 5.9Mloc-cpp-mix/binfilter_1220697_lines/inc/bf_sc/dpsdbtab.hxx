/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: dpsdbtab.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 02:34:27 $
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

#ifndef SC_DPSDBTAB_HXX
#define SC_DPSDBTAB_HXX


namespace com { namespace sun { namespace star {
	namespace lang {
		class XMultiServiceFactory;
	}
}}}

#ifndef SC_DPTABDAT_HXX
#include "dptabdat.hxx"
#endif
namespace binfilter {

// --------------------------------------------------------------------
//
//	implementation of ScDPTableData with database data
//

struct ScImportSourceDesc
{
	String	aDBName;
	String	aObject;
	USHORT	nType;			// enum DataImportMode
	BOOL	bNative;

	BOOL operator==	( const ScImportSourceDesc& rOther ) const
		{ return aDBName == rOther.aDBName &&
				 aObject == rOther.aObject &&
				 nType   == rOther.nType &&
				 bNative == rOther.bNative; }
};





} //namespace binfilter
#endif

