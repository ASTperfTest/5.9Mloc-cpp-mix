/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmlevent.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 05:06:19 $
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

#ifndef _XMLOFF_XMLEVENT_HXX
#define _XMLOFF_XMLEVENT_HXX

#ifndef _COM_SUN_STAR_UNO_SEQUENCE_HXX_
#include <com/sun/star/uno/Sequence.hxx>
#endif


/**
 * @#file
 *
 * Several definition used in im- and export of events
 */

namespace com { namespace sun { namespace star {
	namespace xml { namespace sax { class XAttributeList; } }
	namespace beans { struct PropertyValue; }
} } }
namespace rtl { class OUString; }
namespace binfilter {

class SvXMLExport;
class SvXMLImportContext;
class SvXMLImport;
class XMLEventsImportContext;

/**
 * XMLEventNameTranslation: define tables that translate between event names
 * as used in the XML file format and in the StarOffice API.
 * The last entry in the table must be { NULL, NULL }.
 */
struct XMLEventNameTranslation
{
	const sal_Char* sAPIName;
	const sal_Char* sXMLName;
};

/// a translation table for the events defined in the XEventsSupplier service
/// (implemented in XMLEventExport.cxx)
extern const XMLEventNameTranslation aStandardEventTable[];

/** 
 * Handle export of an event for a certain event type (event type as
 * defined by the PropertyValue "EventType" in API).
 *
 * The Handler has to generate the full <script:event> element.
 */
class XMLEventExportHandler
{
public:
    virtual ~XMLEventExportHandler() {};

	virtual void Export( 
		SvXMLExport& rExport,					/// the current XML export
		const ::rtl::OUString& rEventName,		/// the XML name of the event
		::com::sun::star::uno::Sequence<		/// the values for the event
			::com::sun::star::beans::PropertyValue> & rValues,
	 	sal_Bool bUseWhitespace) = 0;	/// create whitespace around elements?
};

/** 
 * Handle import of an event for a certain event type (as defined by
 * the PropertyValue "EventType" in the API).
 *
 * EventContextFactories must be registered with the EventImportHelper
 * that is attached to the SvXMLImport.
 *
 * The factory has to create an import context for a <script:event>
 * element.  The context has to call the
 * EventsImportContext::AddEventValues() method to fave its event
 * registered with the enclosing element. For events consisting only
 * of attributes (and an empty element) an easy solution is to handle
 * all attributes in the CreateContext()-method and return a default
 * context.
 *
 * EventContextFactory objects have to be registered with the
 * EventsImportHelper.
 */
class XMLEventContextFactory
{
public:
    virtual ~XMLEventContextFactory() {};

	virtual SvXMLImportContext* CreateContext(
		SvXMLImport& rImport,				/// import context
		sal_uInt16 nPrefix,					/// element: namespace prefix
		const ::rtl::OUString& rLocalName,	/// element: local name
		const ::com::sun::star::uno::Reference< 	/// attribute list
			::com::sun::star::xml::sax::XAttributeList> & xAttrList,
		/// the context for the enclosing <script:events> element
		XMLEventsImportContext* rEvents, 
		/// the event name (as understood by the API)
		const ::rtl::OUString& rApiEventName,
		/// the event type name (as registered)
		const ::rtl::OUString& rLanguage) = 0;	
};

}//end of namespace binfilter
#endif
