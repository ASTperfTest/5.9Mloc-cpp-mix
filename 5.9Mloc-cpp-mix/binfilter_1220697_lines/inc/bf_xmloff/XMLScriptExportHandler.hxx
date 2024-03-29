/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: XMLScriptExportHandler.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 05:03:58 $
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

#ifndef _XMLOFF_XMLSCRIPTEXPORTHANDLER_HXX
#define _XMLOFF_XMLSCRIPTEXPORTHANDLER_HXX



#ifndef _RTL_USTRING_HXX_
#include "rtl/ustring.hxx"
#endif

#ifndef _XMLOFF_XMLEVENT_HXX
#include "xmlevent.hxx"
#endif

#include <map>

namespace com { namespace sun { namespace star {
	namespace beans { struct PropertyValue; } 
} } }
namespace binfilter {
class SvXMLExport;

class XMLScriptExportHandler : public XMLEventExportHandler
{
	const ::rtl::OUString sURL;

public:
	XMLScriptExportHandler();
	virtual ~XMLScriptExportHandler();

	virtual void Export(
		SvXMLExport& rExport,
		const ::rtl::OUString& rEventName,
		::com::sun::star::uno::Sequence<
			::com::sun::star::beans::PropertyValue> & rValues,
		sal_Bool bUseWhitespace);
};

}//end of namespace binfilter
#endif
