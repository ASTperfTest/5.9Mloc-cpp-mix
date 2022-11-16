/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svx_xmlexport.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:37:34 $
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



#ifndef _COM_SUN_STAR_XML_SAX_XPARSER_HPP_
#include <com/sun/star/xml/sax/XParser.hpp>
#endif




#ifndef _COM_SUN_STAR_IO_XACTIVEDATASOURCE_HPP_
#include <com/sun/star/io/XActiveDataSource.hpp>
#endif

#ifndef _COM_SUN_STAR_XML_SAX_SAXPARSEEXCEPTION_HPP_
#include <com/sun/star/xml/sax/SAXParseException.hpp>
#endif


#ifndef _COM_SUN_STAR_DOCUMENT_XFILTER_HPP_
#include <com/sun/star/document/XFilter.hpp>
#endif

#ifndef _COM_SUN_STAR_DOCUMENT_XEXPORTER_HPP_
#include <com/sun/star/document/XExporter.hpp>
#endif

#ifndef _COM_SUN_STAR_DOCUMENT_XIMPORTER_HPP_
#include <com/sun/star/document/XImporter.hpp>
#endif



#ifndef _SVDMODEL_HXX
#include <svdmodel.hxx>
#endif

#ifndef _XMLEOHLP_HXX
#include <xmleohlp.hxx>
#endif
#ifndef _XMLGRHLP_HXX
#include <xmlgrhlp.hxx>
#endif

#include "unomodel.hxx"
#ifndef _LEGACYBINFILTERMGR_HXX
#include <legacysmgr/legacy_binfilters_smgr.hxx>	//STRIP002 
#endif
namespace binfilter {
using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;

/*N*/ sal_Bool SvxDrawingLayerExport( SdrModel* pModel, uno::Reference<io::XOutputStream> xOut, Reference< lang::XComponent > xComponent )
/*N*/ {
/*N*/ 	return SvxDrawingLayerExport( pModel, xOut, xComponent, "com.sun.star.comp.DrawingLayer.XMLExporter" );
/*N*/ }
/*N*/ 
/*N*/ sal_Bool SvxDrawingLayerExport( SdrModel* pModel, uno::Reference<io::XOutputStream> xOut, Reference< lang::XComponent > xComponent, const char* pExportService )
/*N*/ {
/*N*/ 	sal_Bool bDocRet = xOut.is();
/*N*/ 
/*N*/ 	Reference< document::XGraphicObjectResolver > xGraphicResolver;
/*N*/ 	SvXMLGraphicHelper *pGraphicHelper = 0;
/*N*/ 
/*N*/ 	Reference< document::XEmbeddedObjectResolver > xObjectResolver;
/*N*/ 	SvXMLEmbeddedObjectHelper *pObjectHelper = 0;
/*N*/ 
/*N*/ 	try
/*N*/ 	{
/*N*/ 		if( !xComponent.is() )
/*N*/ 		{
/*N*/ 			xComponent = new SvxUnoDrawingModel( pModel );
/*N*/ 			pModel->setUnoModel( Reference< XInterface >::query( xComponent ) );
/*N*/ 		}
/*N*/ 
/*N*/ 		uno::Reference< lang::XMultiServiceFactory> xServiceFactory( ::legacy_binfilters::getLegacyProcessServiceFactory() );
/*N*/ 		if( !xServiceFactory.is() )
/*N*/ 		{
/*N*/ 			DBG_ERROR( "got no service manager" );
/*N*/ 			bDocRet = sal_False;
/*N*/ 		}
/*N*/ 
/*N*/ 		if( bDocRet )
/*N*/ 		{
/*N*/ 			uno::Reference< uno::XInterface > xWriter( xServiceFactory->createInstance( OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.xml.sax.Writer" ) ) ) );
/*N*/ 			if( !xWriter.is() )
/*N*/ 			{
/*N*/ 				DBG_ERROR( "com.sun.star.xml.sax.Writer service missing" );
/*N*/ 				bDocRet = sal_False;
/*N*/ 			}
/*N*/ 
/*N*/ 			// init resolver
/*N*/ 			SvPersist *pPersist = pModel->GetPersist();
/*N*/ 			if( pPersist )
/*N*/ 			{
/*N*/ 				pObjectHelper = SvXMLEmbeddedObjectHelper::Create( *pPersist, EMBEDDEDOBJECTHELPER_MODE_WRITE );
/*N*/ 				xObjectResolver = pObjectHelper;
/*N*/ 			}
/*N*/ 
/*N*/ 			pGraphicHelper = SvXMLGraphicHelper::Create( GRAPHICHELPER_MODE_WRITE );
/*N*/ 			xGraphicResolver = pGraphicHelper;
/*N*/ 
/*N*/ 			if( bDocRet )
/*N*/ 			{
/*N*/ 				uno::Reference<xml::sax::XDocumentHandler>	xHandler( xWriter, uno::UNO_QUERY );
/*N*/ 
/*N*/ 				// doc export
/*N*/ 				uno::Reference< io::XActiveDataSource > xDocSrc( xWriter, uno::UNO_QUERY );
/*N*/ 				xDocSrc->setOutputStream( xOut );
/*N*/ 
/*N*/ 				uno::Sequence< uno::Any > aArgs( xObjectResolver.is() ? 3 : 2 );
/*N*/ 				aArgs[0] <<= xHandler;
/*N*/ 				aArgs[1] <<= xGraphicResolver;
/*N*/ 				if( xObjectResolver.is() )
/*N*/ 					aArgs[2] <<= xObjectResolver;
/*N*/ 
/*N*/ 				uno::Reference< document::XFilter > xFilter( xServiceFactory->createInstanceWithArguments( OUString::createFromAscii( pExportService ), aArgs ), uno::UNO_QUERY );
/*N*/ 				if( !xFilter.is() )
/*N*/ 				{
/*N*/ 					DBG_ERROR( "com.sun.star.comp.Draw.XMLExporter service missing" );
/*N*/ 					bDocRet = sal_False;
/*N*/ 				}
/*N*/ 
/*N*/ 				if( bDocRet )
/*N*/ 				{
/*N*/ 					uno::Reference< document::XExporter > xExporter( xFilter, uno::UNO_QUERY );
/*N*/ 					if( xExporter.is() )
/*N*/ 					{
/*N*/ 						xExporter->setSourceDocument( xComponent );
/*N*/ 
/*N*/ 						uno::Sequence< beans::PropertyValue > aDescriptor( 0 );
/*N*/ 						bDocRet = xFilter->filter( aDescriptor );
/*N*/ 					}
/*N*/ 				}
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ 	catch(uno::Exception e)
/*N*/ 	{
/*N*/ #if OSL_DEBUG_LEVEL > 1
/*N*/ 		ByteString aError( "uno Exception caught while exporting:\n" );
/*N*/ 		aError += ByteString( String( e.Message), RTL_TEXTENCODING_ASCII_US );
/*N*/ 		DBG_ERROR( aError.GetBuffer() );
/*N*/ #endif
/*N*/ 		bDocRet = sal_False;
/*N*/ 	}
/*N*/ 
/*N*/ 	if( pGraphicHelper )
/*N*/ 		SvXMLGraphicHelper::Destroy( pGraphicHelper );
/*N*/ 	xGraphicResolver = 0;
/*N*/ 
/*N*/ 	if( pObjectHelper )
/*N*/ 	    SvXMLEmbeddedObjectHelper::Destroy( pObjectHelper );
/*N*/ 	xObjectResolver = 0;
/*N*/ 
/*N*/ 	return bDocRet;
/*N*/ }
/*N*/ 
/*N*/ //-////////////////////////////////////////////////////////////////////
/*N*/ 
/*N*/ sal_Bool SvxDrawingLayerImport( SdrModel* pModel, uno::Reference<io::XInputStream> xInputStream, Reference< lang::XComponent > xComponent )
/*N*/ {
/*N*/ 	return SvxDrawingLayerImport( pModel, xInputStream, xComponent, "com.sun.star.comp.Draw.XMLImporter" );
/*N*/ }
/*N*/ 
/*N*/ sal_Bool SvxDrawingLayerImport( SdrModel* pModel, uno::Reference<io::XInputStream> xInputStream, Reference< lang::XComponent > xComponent, const char* pImportService  )
/*N*/ {
/*N*/ 	sal_uInt32	nRet = 0;
/*N*/ 
/*N*/ 	Reference< document::XGraphicObjectResolver > xGraphicResolver;
/*N*/ 	SvXMLGraphicHelper *pGraphicHelper = 0;
/*N*/ 
/*N*/ 	Reference< document::XEmbeddedObjectResolver > xObjectResolver;
/*N*/ 	SvXMLEmbeddedObjectHelper *pObjectHelper = 0;
/*N*/ 
/*N*/ 	if( !xComponent.is() )
/*N*/ 	{
/*N*/ 		xComponent = new SvxUnoDrawingModel( pModel );
/*N*/ 		pModel->setUnoModel( Reference< XInterface >::query( xComponent ) );
/*N*/ 	}
/*N*/ 
/*N*/ 	Reference< frame::XModel > xModel( xComponent, UNO_QUERY );
/*N*/ 
/*N*/ 	try
/*N*/ 	{
/*N*/ 		// Get service factory
/*N*/ 		Reference< lang::XMultiServiceFactory > xServiceFactory = ::legacy_binfilters::getLegacyProcessServiceFactory();
/*N*/ 		DBG_ASSERT( xServiceFactory.is(), "XMLReader::Read: got no service manager" );
/*N*/ 
/*N*/ 		if( !xServiceFactory.is() )
/*N*/ 			nRet = 1;
/*N*/ 
/*N*/ 		if( 0 == nRet )
/*N*/ 		{
/*N*/ 			xModel->lockControllers();
/*N*/ 
/*N*/ 			// -------------------------------------
/*N*/ 
/*N*/ 			pGraphicHelper = SvXMLGraphicHelper::Create( GRAPHICHELPER_MODE_READ );
/*N*/ 			xGraphicResolver = pGraphicHelper;
/*N*/ 
/*N*/ 			SvPersist *pPersist = pModel->GetPersist();
/*N*/ 			if( pPersist )
/*N*/ 			{
/*N*/ 				pObjectHelper = SvXMLEmbeddedObjectHelper::Create(
/*N*/ 											*pPersist,
/*N*/ 											EMBEDDEDOBJECTHELPER_MODE_READ );
/*N*/ 				xObjectResolver = pObjectHelper;
/*N*/ 			}
/*N*/ 		}
/*N*/ 
/*N*/ 		// -------------------------------------
/*N*/ 
/*N*/ 		if( 0 == nRet )
/*N*/ 		{
/*N*/ 
/*N*/ 			// parse
/*N*/ 			// prepare ParserInputSrouce
/*N*/ 			xml::sax::InputSource aParserInput;
/*N*/ 			aParserInput.aInputStream = xInputStream;
/*N*/ 
/*N*/ 			// get parser
/*N*/ 			Reference< xml::sax::XParser > xParser( xServiceFactory->createInstance( OUString::createFromAscii("com.sun.star.xml.sax.Parser") ), UNO_QUERY );
/*N*/ 			DBG_ASSERT( xParser.is(), "Can't create parser" );
/*N*/ 
/*N*/ 			// prepare filter arguments
/*N*/ 			Sequence<Any> aFilterArgs( 2 );
/*N*/ 			Any *pArgs = aFilterArgs.getArray();
/*N*/ 			*pArgs++ <<= xGraphicResolver;
/*N*/ 			*pArgs++ <<= xObjectResolver;
/*N*/ 
/*N*/ 			// get filter
/*N*/ 			Reference< xml::sax::XDocumentHandler > xFilter( xServiceFactory->createInstanceWithArguments( OUString::createFromAscii( pImportService ), aFilterArgs), UNO_QUERY );
/*N*/ 			DBG_ASSERT( xFilter.is(), "Can't instantiate filter component." );
/*N*/ 
/*N*/ 			if( !xParser.is() || !xFilter.is() )
/*N*/ 			{
/*N*/ 				nRet = 1;
/*N*/ 			}
/*N*/ 			else
/*N*/ 			{
/*N*/ 				// connect parser and filter
/*N*/ 				xParser->setDocumentHandler( xFilter );
/*N*/ 
/*N*/ 				// connect model and filter
/*N*/ 				uno::Reference < document::XImporter > xImporter( xFilter, UNO_QUERY );
/*N*/ 				xImporter->setTargetDocument( xComponent );
/*N*/ 
/*N*/ 				// finally, parser the stream
/*N*/ 				xParser->parseStream( aParserInput );
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ 	catch( xml::sax::SAXParseException& r )
/*N*/ 	{
/*N*/ #if OSL_DEBUG_LEVEL > 1
/*N*/ 		ByteString aError( "SAX parse exception catched while importing:\n" );
/*N*/ 		aError += ByteString( String( r.Message), RTL_TEXTENCODING_ASCII_US );
/*N*/ 		DBG_ERROR( aError.GetBuffer() );
/*N*/ #endif
/*N*/ 	}
/*N*/ 	catch( xml::sax::SAXException& r )
/*N*/ 	{
/*N*/ #if OSL_DEBUG_LEVEL > 1
/*N*/ 		ByteString aError( "SAX exception catched while importing:\n" );
/*N*/ 		aError += ByteString( String( r.Message), RTL_TEXTENCODING_ASCII_US );
/*N*/ 		DBG_ERROR( aError.GetBuffer() );
/*N*/ #endif
/*N*/ 	}
/*N*/ 	catch( io::IOException& r )
/*N*/ 	{
/*N*/ #if OSL_DEBUG_LEVEL > 1
/*N*/ 		ByteString aError( "IO exception catched while importing:\n" );
/*N*/ 		aError += ByteString( String( r.Message), RTL_TEXTENCODING_ASCII_US );
/*N*/ 		DBG_ERROR( aError.GetBuffer() );
/*N*/ #endif
/*N*/ 	}
/*N*/ 	catch( uno::Exception& r )
/*N*/ 	{
/*N*/ #if OSL_DEBUG_LEVEL > 1
/*N*/ 		ByteString aError( "uno exception catched while importing:\n" );
/*N*/ 		aError += ByteString( String( r.Message), RTL_TEXTENCODING_ASCII_US );
/*N*/ 		DBG_ERROR( aError.GetBuffer() );
/*N*/ #endif
/*N*/ 	}
/*N*/ 
/*N*/ 	if( pGraphicHelper )
/*N*/ 		SvXMLGraphicHelper::Destroy( pGraphicHelper );
/*N*/ 	xGraphicResolver = 0;
/*N*/ 
/*N*/ 	if( pObjectHelper )
/*N*/ 		SvXMLEmbeddedObjectHelper::Destroy( pObjectHelper );
/*N*/ 	xObjectResolver = 0;
/*N*/ 
/*N*/ 	if( xModel.is() )
/*N*/ 		xModel->unlockControllers();
/*N*/ 
/*N*/ 	return nRet == 0;
/*N*/ }
}
