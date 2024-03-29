/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmlexp.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 03:46:01 $
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

#ifndef _XMLEXP_HXX
#define _XMLEXP_HXX

#ifndef _XMLOFF_XMLEXP_HXX
#include <bf_xmloff/xmlexp.hxx>
#endif

#ifndef _XMLITMAP_HXX
#include "xmlitmap.hxx"
#endif
#ifndef _UNIVERSALL_REFERENCE_HXX
#include <bf_xmloff/uniref.hxx>
#endif
#ifndef _XMLOFF_XMLTOKEN_HXX
#include <bf_xmloff/xmltoken.hxx>
#endif
namespace binfilter {

class SwPaM;
class SwFmt;
class SwFrmFmt;
class SvXMLUnitConverter; 
class SvXMLAutoStylePoolP; 
class XMLPropertySetMapper; 

class SvXMLExportItemMapper;

class SwTableLine;
class SwTableLines;
class SwTableBox;
class SwXMLTableColumn_Impl;
class SwXMLTableLines_Impl;
class SwXMLTableLinesCache_Impl;
class SwXMLTableColumnsSortByWidth_Impl;
class SwXMLTableFrmFmtsSort_Impl;
class SwXMLTableInfo_Impl;
class SwTableNode;


#ifndef XML_PROGRESS_REF_NOT_SET
#define XML_PROGRESS_REF_NOT_SET ((sal_Int32)-1)
#endif


class SwXMLExport : public SvXMLExport
{
	friend class SwXMLExpContext;

#ifdef XML_CORE_API
	SwPaM						*pCurPaM;		// the current PaM
	SwPaM						*pOrigPaM;		// the original PaM
#endif

	SvXMLUnitConverter			*pTwipUnitConv;

	SvXMLExportItemMapper		*pTableItemMapper;
	SwXMLTableLinesCache_Impl	*pTableLines;

	SvXMLItemMapEntriesRef 		xTableItemMap;
	SvXMLItemMapEntriesRef 		xTableRowItemMap;
	SvXMLItemMapEntriesRef 		xTableCellItemMap;
	UniReference < XMLPropertySetMapper > xParaPropMapper;

	sal_Bool					bExportWholeDoc : 1;// export whole document?
	sal_Bool					bBlock : 1;			// export text block?
	sal_Bool					bExportFirstTableOnly : 1;
	sal_Bool					bShowProgress : 1;
    sal_Bool                    bSavedShowChanges : 1;

	void _InitItemExport();
	void _FinitItemExport();
	void ExportTableLinesAutoStyles( const SwTableLines& rLines,
								 sal_uInt32 nAbsWidth,
								 sal_uInt32 nBaseWidth,
								 const ::rtl::OUString& rNamePrefix,
								 SwXMLTableColumnsSortByWidth_Impl& rExpCols,
								 SwXMLTableFrmFmtsSort_Impl& rExpRows,
								 SwXMLTableFrmFmtsSort_Impl& rExpCells,
								 SwXMLTableInfo_Impl& rTblInfo,
								 sal_Bool bTop=sal_False );


	void ExportFmt( const SwFmt& rFmt,  enum ::binfilter::xmloff::token::XMLTokenEnum eClass = ::binfilter::xmloff::token::XML_TOKEN_INVALID );
	void ExportTableFmt( const SwFrmFmt& rFmt, sal_uInt32 nAbsWidth );

	void ExportTableColumnStyle( const SwXMLTableColumn_Impl& rCol );
	void ExportTableBox( const SwTableBox& rBox, sal_uInt16 nColSpan,
						 SwXMLTableInfo_Impl& rTblInfo );
	void ExportTableLine( const SwTableLine& rLine,
			              const SwXMLTableLines_Impl& rLines,
						  SwXMLTableInfo_Impl& rTblInfo );
	void ExportTableLines( const SwTableLines& rLines,
						   SwXMLTableInfo_Impl& rTblInfo,
						   sal_Bool bHeadline=sal_False );

	virtual void _ExportMeta();
	virtual void _ExportFontDecls();
	virtual void _ExportStyles( sal_Bool bUsed );
	virtual void _ExportAutoStyles();
	virtual void _ExportMasterStyles();
	virtual void _ExportContent();
    virtual void GetViewSettings(::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& aProps);
	virtual void GetConfigurationSettings(::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& aProps);

#ifdef XML_CORE_API
	void SetCurPaM( SwPaM& rPaM, sal_Bool bWhole, sal_Bool bTabOnly );
#endif

	// string constants for table cell export
	const ::rtl::OUString sNumberFormat;
	const ::rtl::OUString sIsProtected;
	const ::rtl::OUString sCell;

protected:

	virtual XMLTextParagraphExport* CreateTextParagraphExport();
	virtual SvXMLAutoStylePoolP* CreateAutoStylePool();
	virtual XMLPageExport* CreatePageExport();
	virtual XMLShapeExport* CreateShapeExport();
	virtual XMLFontAutoStylePool* CreateFontAutoStylePool();

public:

	// #110680#
	SwXMLExport(
		const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > xServiceFactory,
		sal_uInt16 nExportFlags = EXPORT_ALL);

#ifdef XML_CORE_API
	// #110680#
	SwXMLExport( 
		const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > xServiceFactory,
		const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XModel > & rModel,
		SwPaM& rPaM, 
		const ::rtl::OUString& rFileName,
		const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XDocumentHandler > & rHandler,
		const ::com::sun::star::uno::Reference< ::com::sun::star::document::XGraphicObjectResolver > &,
		sal_Bool bExpWholeDoc, 
		sal_Bool bExpFirstTableOnly,
		sal_Bool bShowProgr );
#endif
	virtual ~SwXMLExport();

	void setBlockMode();

	virtual sal_uInt32 exportDoc( enum ::binfilter::xmloff::token::XMLTokenEnum eClass = ::binfilter::xmloff::token::XML_TOKEN_INVALID );

	inline const SvXMLUnitConverter& GetTwipUnitConverter() const;

	void ExportTableAutoStyles( const SwTableNode& rTblNd );
	void ExportTable( const SwTableNode& rTblNd );

	SvXMLExportItemMapper& GetTableItemMapper() { return *pTableItemMapper; }
	const UniReference < XMLPropertySetMapper >& GetParaPropMapper()
	{
		return xParaPropMapper;
	}

	sal_Bool IsShowProgress() const { return bShowProgress; }
	void SetShowProgress( sal_Bool b ) { bShowProgress = b; }
	sal_Bool IsBlockMode() const { return bBlock; }

	// XUnoTunnel
	static const ::com::sun::star::uno::Sequence< sal_Int8 > & getUnoTunnelId() throw();
    virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& aIdentifier ) throw(::com::sun::star::uno::RuntimeException);

    // XServiceInfo (override parent method)
    ::rtl::OUString SAL_CALL getImplementationName() 
        throw( ::com::sun::star::uno::RuntimeException );
};

inline const SvXMLUnitConverter& SwXMLExport::GetTwipUnitConverter() const
{
	return *pTwipUnitConv;
}


} //namespace binfilter
#endif	//  _XMLEXP_HXX

