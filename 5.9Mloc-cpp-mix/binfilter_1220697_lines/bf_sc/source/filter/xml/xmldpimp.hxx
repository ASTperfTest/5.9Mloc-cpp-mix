/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmldpimp.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/07 18:36:19 $
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
#ifndef SC_XMLDPIMP_HXX
#define SC_XMLDPIMP_HXX

#ifndef _XMLOFF_XMLICTXT_HXX
#include <bf_xmloff/xmlictxt.hxx>
#endif
#ifndef _XMLOFF_XMLIMP_HXX
#include <bf_xmloff/xmlimp.hxx>
#endif

#include "global.hxx"
#include "dpobject.hxx"
#include "dpsave.hxx"
namespace binfilter {

class ScXMLImport;

enum ScMySourceType
{
	SQL,
	TABLE,
	QUERY,
	SERVICE,
	CELLRANGE
};

class ScXMLDataPilotTablesContext : public SvXMLImportContext
{

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotTablesContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
										::com::sun::star::xml::sax::XAttributeList>& xAttrList);

	virtual ~ScXMLDataPilotTablesContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotTableContext : public SvXMLImportContext
{
	ScDocument*		pDoc;
	ScDPObject*		pDPObject;
	ScDPSaveData*	pDPSave;
	::rtl::OUString	sDataPilotTableName;
	::rtl::OUString	sApplicationData;
	::rtl::OUString	sGrandTotal;
	::rtl::OUString	sDatabaseName;
	::rtl::OUString	sSourceObject;
	::rtl::OUString	sServiceName;
	::rtl::OUString	sServiceSourceName;
	::rtl::OUString	sServiceSourceObject;
	::rtl::OUString	sServiceUsername;
	::rtl::OUString	sServicePassword;
	::rtl::OUString	sButtons;
	ScRange			aSourceCellRangeAddress;
	ScRange			aTargetRangeAddress;
	ScRange			aFilterSourceRange;
	ScAddress		aFilterOutputPosition;
	ScQueryParam	aSourceQueryParam;
	ScMySourceType	nSourceType;
	sal_Bool		bIsNative : 1;
	sal_Bool		bIgnoreEmptyRows : 1;
	sal_Bool		bIdentifyCategories : 1;
	sal_Bool		bUseRegularExpression : 1;
	sal_Bool		bIsCaseSensitive : 1;
	sal_Bool		bSkipDuplicates : 1;
	sal_Bool		bFilterCopyOutputData : 1;
	sal_Bool		bTargetRangeAddress : 1;
	sal_Bool		bSourceCellRange : 1;


	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotTableContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
										::com::sun::star::xml::sax::XAttributeList>& xAttrList);

	virtual ~ScXMLDataPilotTableContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();

	void SetDatabaseName(const ::rtl::OUString& sValue) { sDatabaseName = sValue; }
	void SetSourceObject(const ::rtl::OUString& sValue) { sSourceObject = sValue; }
	void SetNative(const sal_Bool bValue) { bIsNative = bValue; }
	void SetServiceName(const ::rtl::OUString& sValue) { sServiceName = sValue; }
	void SetServiceSourceName(const ::rtl::OUString& sValue) { sServiceSourceName = sValue; }
	void SetServiceSourceObject(const ::rtl::OUString& sValue) { sServiceSourceObject = sValue; }
	void SetServiceUsername(const ::rtl::OUString& sValue) { sServiceUsername = sValue; }
	void SetServicePassword(const ::rtl::OUString& sValue) { sServicePassword = sValue; }
	void SetSourceCellRangeAddress(const ScRange& aValue) { aSourceCellRangeAddress = aValue; bSourceCellRange = sal_True; }
	void SetSourceQueryParam(const ScQueryParam& aValue) { aSourceQueryParam = aValue; }
//	void SetFilterUseRegularExpressions(const sal_Bool bValue) { aSourceQueryParam.bRegExp = bValue; }
	void SetFilterOutputPosition(const ScAddress& aValue) { aFilterOutputPosition = aValue; }
	void SetFilterCopyOutputData(const sal_Bool bValue) { bFilterCopyOutputData = bValue; }
	void SetFilterSourceRange(const ScRange& aValue) { aFilterSourceRange = aValue; }
//	void SetFilterIsCaseSensitive(const sal_Bool bValue) { aSourceQueryParam.bCaseSens = bValue; }
//	void SetFilterSkipDuplicates(const sal_Bool bValue) { aSourceQueryParam.bDuplicate = !bValue; }
	void AddDimension(ScDPSaveDimension* pDim);
	void SetButtons();
};

class ScXMLDPSourceSQLContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceSQLContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceSQLContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDPSourceTableContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceTableContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceTableContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDPSourceQueryContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceQueryContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceQueryContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLSourceServiceContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLSourceServiceContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLSourceServiceContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLSourceCellRangeContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLSourceCellRangeContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLSourceCellRangeContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotFieldContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;
	ScDPSaveDimension*			pDim;

	sal_Int32					nUsedHierarchy;
	sal_Int16					nFunction;
	sal_Int16					nOrientation;
	sal_Bool					bShowEmpty : 1;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotFieldContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDataPilotFieldContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();

	void SetShowEmpty(const sal_Bool bValue) { if (pDim) pDim->SetShowEmpty(bValue); }
	void SetSubTotals(const sal_uInt16* pFunctions, const sal_Int16 nCount) { if(pDim) pDim->SetSubTotals(nCount, pFunctions); }
	void AddMember(ScDPSaveMember* pMember) { if (pDim) pDim->AddMember(pMember); }
};

class ScXMLDataPilotLevelContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotLevelContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotLevelContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotSubTotalsContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext* pDataPilotField;

	sal_Int16	nFunctionCount;
	sal_uInt16*	pFunctions;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotFieldContext* GetDataPilotField() { return pDataPilotField; }

	ScXMLDataPilotSubTotalsContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotSubTotalsContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
	void AddFunction(sal_Int16 nFunction);
};

class ScXMLDataPilotSubTotalContext : public SvXMLImportContext
{
	ScXMLDataPilotSubTotalsContext*	pDataPilotSubTotals;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotSubTotalContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotSubTotalsContext* pDataPilotSubTotals);

	virtual ~ScXMLDataPilotSubTotalContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotMembersContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext* pDataPilotField;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotMembersContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotMembersContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotMemberContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	::rtl::OUString sName;
	sal_Bool	bDisplay : 1;
	sal_Bool	bDisplayDetails : 1;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotMemberContext( ScXMLImport& rImport, USHORT nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotMemberContext();

	virtual SvXMLImportContext *CreateChildContext( USHORT nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

} //namespace binfilter
#endif

