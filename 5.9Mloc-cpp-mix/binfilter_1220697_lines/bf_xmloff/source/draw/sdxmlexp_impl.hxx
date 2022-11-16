/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdxmlexp_impl.hxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 07:50:36 $
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

#ifndef _SDXMLEXP_IMPL_HXX
#define _SDXMLEXP_IMPL_HXX

#ifndef _XMLOFF_XMLEXP_HXX
#include "xmlexp.hxx"
#endif


#ifndef _COM_SUN_STAR_FRAME_XMODEL_HPP_
#include <com/sun/star/frame/XModel.hpp>
#endif

#ifndef _COM_SUN_STAR_TASK_XSTATUSINDICATOR_HPP_
#include <com/sun/star/task/XStatusIndicator.hpp>
#endif

#ifndef _COM_SUN_STAR_CONTAINER_XNAMEACCESS_HPP_
#include <com/sun/star/container/XNameAccess.hpp>
#endif

#ifndef _COM_SUN_STAR_DRAWING_XDRAWPAGE_HPP_
#include <com/sun/star/drawing/XDrawPage.hpp>
#endif
class SfxPoolItem;
class SfxItemSet;
class Rectangle;
namespace binfilter {

//////////////////////////////////////////////////////////////////////////////

class SvXMLUnitConverter;
class SvXMLExportItemMapper;
class OUStrings_Impl;
class OUStringsSort_Impl;

class ImpPresPageDrawStylePropMapper;
class ImpXMLEXPPageMasterList;
class ImpXMLEXPPageMasterInfo;
class ImpXMLDrawPageInfoList;
class ImpXMLAutoLayoutInfoList;
class SvXMLAutoStylePoolP;
class XMLSdPropHdlFactory;
class ImpXMLShapeStyleInfo;
class XMLShapeExportPropertyMapper;
class XMLPageExportPropertyMapper;

//////////////////////////////////////////////////////////////////////////////

enum XmlPlaceholder
{
	XmlPlaceholderTitle,
	XmlPlaceholderOutline,
	XmlPlaceholderSubtitle,
	XmlPlaceholderText,
	XmlPlaceholderGraphic,
	XmlPlaceholderObject,
	XmlPlaceholderChart,
	XmlPlaceholderOrgchart,
	XmlPlaceholderTable,
	XmlPlaceholderPage,
	XmlPlaceholderNotes,
	XmlPlaceholderHandout,
	XmlPlaceholderVerticalTitle,
	XmlPlaceholderVerticalOutline
};

//////////////////////////////////////////////////////////////////////////////

class SdXMLExport : public SvXMLExport
{
	::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess > mxDocStyleFamilies;
	::com::sun::star::uno::Reference< ::com::sun::star::container::XIndexAccess > mxDocMasterPages;
	::com::sun::star::uno::Reference< ::com::sun::star::container::XIndexAccess > mxDocDrawPages;
	sal_Int32					mnDocMasterPageCount;
	sal_Int32					mnDocDrawPageCount;
	sal_uInt32					mnShapeStyleInfoIndex;
	sal_uInt32					mnObjectCount;

	// temporary infos
	ImpXMLEXPPageMasterList*	mpPageMasterInfoList;
	ImpXMLEXPPageMasterList*	mpPageMasterUsageList;
	ImpXMLEXPPageMasterList*	mpNotesPageMasterUsageList;
	ImpXMLEXPPageMasterInfo*	mpHandoutPageMaster;
	ImpXMLAutoLayoutInfoList*	mpAutoLayoutInfoList;

	::com::sun::star::uno::Sequence< ::rtl::OUString > maDrawPagesAutoLayoutNames;

	::std::vector< ::rtl::OUString >		maDrawPagesStyleNames;
	::std::vector< ::rtl::OUString >		maMasterPagesStyleNames;

	XMLSdPropHdlFactory*				mpSdPropHdlFactory;
	XMLShapeExportPropertyMapper*		mpPropertySetMapper;
	XMLPageExportPropertyMapper*		mpPresPagePropsMapper;

	sal_uInt32					mnUsedDateStyles;			// this is a bitfield of the used formatings for date fields
	sal_uInt32					mnUsedTimeStyles;			// this is a bitfield of the used formatings for time fields

	sal_Bool					mbIsDraw;
	sal_Bool					mbFamilyGraphicUsed;
	sal_Bool					mbFamilyPresentationUsed;

	const ::rtl::OUString			msZIndex;
	const ::rtl::OUString			msEmptyPres;
	const ::rtl::OUString			msModel;
	const ::rtl::OUString			msStartShape;
	const ::rtl::OUString			msEndShape;
	const ::rtl::OUString			msPageLayoutNames;

	virtual void _ExportStyles(BOOL bUsed);
	virtual void _ExportAutoStyles();
	virtual void _ExportMasterStyles();
	virtual void _ExportContent();
	// #82003#
	virtual void _ExportMeta();

	ImpXMLEXPPageMasterInfo* ImpGetOrCreatePageMasterInfo( ::com::sun::star::uno::Reference< ::com::sun::star::drawing::XDrawPage > xMasterPage );
	void ImpPrepPageMasterInfos();
	void ImpPrepDrawMasterInfos();
	void ImpWritePageMasterInfos();
	void ImpPrepAutoLayoutInfos();

	ImpXMLEXPPageMasterInfo* ImpGetPageMasterInfoByName(const ::rtl::OUString& rName);

	void ImpPrepDrawPageInfos();
	void ImpPrepMasterPageInfos();
	void ImpWritePresentationStyles();

	BOOL ImpPrepAutoLayoutInfo(const ::com::sun::star::uno::Reference< ::com::sun::star::drawing::XDrawPage >& xPage, ::rtl::OUString& rName);
	void ImpWriteAutoLayoutInfos();
	void ImpWriteAutoLayoutPlaceholder(XmlPlaceholder ePl, const Rectangle& rRect);

	void exportFormsElement( ::com::sun::star::uno::Reference< ::com::sun::star::drawing::XDrawPage > xDrawPage );
	void exportPresentationSettings();

	// #82003# helper function for recursive object count
	sal_uInt32 ImpRecursiveObjectCount( ::com::sun::star::uno::Reference< ::com::sun::star::drawing::XShapes > xShapes);

protected:
	virtual void GetViewSettings( ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& aProps);
	virtual void GetConfigurationSettings( ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& aProps);

public:
	// #110680#
	SdXMLExport( 
		const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& xServiceFactory,
		sal_Bool bIsDraw, sal_uInt16 nExportFlags = EXPORT_ALL );
	virtual ~SdXMLExport();

	void SetProgress(sal_Int32 nProg);

	// XExporter
	virtual void SAL_CALL setSourceDocument( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& xDoc ) throw(::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

	// get factories and mappers
	XMLSdPropHdlFactory* GetSdPropHdlFactory() const { return mpSdPropHdlFactory; }
	XMLShapeExportPropertyMapper* GetPropertySetMapper() const { return mpPropertySetMapper; }
	XMLPageExportPropertyMapper* GetPresPagePropsMapper() const { return mpPresPagePropsMapper; }

	BOOL IsDraw() const { return mbIsDraw; }
	BOOL IsImpress() const { return !mbIsDraw; }

	BOOL IsFamilyGraphicUsed() const { return mbFamilyGraphicUsed; }
	void SetFamilyGraphicUsed() { mbFamilyGraphicUsed = TRUE; }
	BOOL IsFamilyPresentationUsed() const { return mbFamilyPresentationUsed; }
	void SetFamilyPresentationUsed() { mbFamilyPresentationUsed = TRUE; }

	virtual void addDataStyle(const sal_Int32 nNumberFormat, sal_Bool bTimeFormat = sal_False );
	virtual void exportDataStyles();
	virtual void exportAutoDataStyles();
	virtual ::rtl::OUString getDataStyleName(const sal_Int32 nNumberFormat, sal_Bool bTimeFormat = sal_False ) const;

    // XServiceInfo ( : SvXMLExport )
    virtual ::rtl::OUString SAL_CALL getImplementationName() throw( ::com::sun::star::uno::RuntimeException );
};

}//end of namespace binfilter
#endif	//  _SDXMLEXP_HXX
