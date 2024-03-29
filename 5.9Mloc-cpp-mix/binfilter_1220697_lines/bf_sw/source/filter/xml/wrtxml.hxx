/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: wrtxml.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: ihi $ $Date: 2007/11/26 18:57:38 $
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

#ifndef _WRTXML_HXX
#define _WRTXML_HXX

#ifndef _SHELLIO_HXX
#include <shellio.hxx>
#endif
namespace com { namespace sun { namespace start {
	namespace uno { template<class A> class Reference; }
	namespace uno { template<class A> class Sequence; }
	namespace uno { class Any; }
	namespace lang { class XComponent; }
	namespace lang { class XMultiServiceFactory; }
	namespace beans { struct PropertyValue; }
} } }
namespace binfilter {

class SwDoc;
class SwPaM;
class SfxMedium;
	

class SwXMLWriter : public StgWriter
{
	sal_uInt32 _Write();

protected:
	virtual ULONG WriteStorage();

public:

	SwXMLWriter();
	virtual ~SwXMLWriter();

	virtual ULONG Write( SwPaM&, SfxMedium&, const String* = 0 );

private:

	// helper methods to write XML streams

	/// write a single XML stream into the package
	sal_Bool WriteThroughComponent(
		/// the component we export
		const ::com::sun::star::uno::Reference<
			::com::sun::star::lang::XComponent> & xComponent,
		const sal_Char* pStreamName,		/// the stream name
		/// service factory for pServiceName
		const ::com::sun::star::uno::Reference<
			::com::sun::star::lang::XMultiServiceFactory> & rFactory,
		const sal_Char* pServiceName,		/// service name of the component
		/// the argument (XInitialization)
		const ::com::sun::star::uno::Sequence<
			::com::sun::star::uno::Any> & rArguments,
		/// output descriptor
		const ::com::sun::star::uno::Sequence<
			::com::sun::star::beans::PropertyValue> & rMediaDesc,
		sal_Bool bPlainStream );			/// neither compress nor encrypt

	/// write a single output stream
	/// (to be called either directly or by WriteThroughComponent(...))
	sal_Bool WriteThroughComponent(
		const ::com::sun::star::uno::Reference<
			::com::sun::star::io::XOutputStream> & xOutputStream,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::lang::XComponent> & xComponent,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::lang::XMultiServiceFactory> & rFactory,
		const sal_Char* pServiceName,
		const ::com::sun::star::uno::Sequence<
			::com::sun::star::uno::Any> & rArguments,
		const ::com::sun::star::uno::Sequence<
			::com::sun::star::beans::PropertyValue> & rMediaDesc );
};


} //namespace binfilter
#endif	//  _WRTXML_HXX

