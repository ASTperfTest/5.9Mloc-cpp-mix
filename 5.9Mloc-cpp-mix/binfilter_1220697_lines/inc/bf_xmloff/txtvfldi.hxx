/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: txtvfldi.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 17:36:29 $
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

/** @#file
 *
 *  XML import of all variable related text fields plus database display field
 */

#ifndef _XMLOFF_TXTVFLDI_HXX
#define _XMLOFF_TXTVFLDI_HXX

#ifndef _XMLOFF_TXTVFLDI_HXX
#include <bf_xmloff/txtvfldi.hxx>
#endif

#ifndef _XMLOFF_TXTFLDI_HXX
#include <bf_xmloff/txtfldi.hxx>
#endif

#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif

#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSETINFO_HPP_
#include <com/sun/star/beans/XPropertySetInfo.hpp>
#endif
namespace binfilter {

/// variable type (for XMLSetVarFieldImportContext)
enum VarType 
{ 
	VarTypeSimple, 
	VarTypeUserField, 
	VarTypeSequence
};

/** helper class: parses value-type and associated value attributes */
class XMLValueImportHelper
{

	const ::rtl::OUString sPropertyContent;
	const ::rtl::OUString sPropertyValue;
	const ::rtl::OUString sPropertyFormula;
	const ::rtl::OUString sPropertyNumberFormat;
    const ::rtl::OUString sPropertyIsFixedLanguage;

	SvXMLImport& rImport;
	XMLTextImportHelper& rHelper;

	::rtl::OUString sValue;		/// string value (only valid if bStringValueOK)
	double fValue;				/// double value (only valid if bFloatValueOK)
	sal_Int32 nFormatKey;		/// format key (only valid of bFormatOK)
	::rtl::OUString sFormula;	/// formula string
	::rtl::OUString sDefault;	/// default (see bStringDefault/bFormulaDef.)
    sal_Bool bIsDefaultLanguage;/// format (of nFormatKey) has system language?

	sal_Bool bStringType; 		/// is this a string (or a float) type?
	sal_Bool bFormatOK;			/// have we read a style:data-style-name attr.?
	sal_Bool bTypeOK;			/// have we read a value-type attribute?
	sal_Bool bStringValueOK;	/// have we read a string-value attr.?
	sal_Bool bFloatValueOK;		/// have we read any of the float attr.s?
	sal_Bool bFormulaOK;		/// have we read the formula attribute?

	const sal_Bool bSetType;	/// should PrepareField set the SetExp subtype?
	const sal_Bool bSetValue;	/// should PrepareField set content/value?
	const sal_Bool bSetStyle;	/// should PrepareField set NumberFormat?
	const sal_Bool bSetFormula;	/// should PrepareField set Formula?

	const sal_Bool bStringDefault;	/// default: string-value = content
	const sal_Bool bFormulaDefault;	/// default: formula = content

public:
	XMLValueImportHelper(
		SvXMLImport& rImprt,					/// XML Import
		XMLTextImportHelper& rHlp,				/// text import helper
		sal_Bool bType, 						/// process type (PrepareField)
		sal_Bool bStyle,						/// process data style (P.F.)
		sal_Bool bValue, 						/// process value (Prep.Field)
		sal_Bool bFormula);						/// process formula (Prep.F.)

	/// process attribute values
	virtual void ProcessAttribute( sal_uInt16 nAttrToken,
								   const ::rtl::OUString& sAttrValue );

	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);

	/// is value a string (rather than double)?
	inline sal_Bool IsStringValue() { return bStringType; }

	/// has format been read?
	inline sal_Bool IsFormatOK() { return bFormatOK; }

	inline void SetDefault(const ::rtl::OUString& sStr) { sDefault = sStr; }
};

/** 
 * abstract parent class for all variable related fields
 * - variable-set/get/decl		(not -decls), 
 * - user-field-get/decl		(not -decls),
 * - sequence/-decl				(not -decls),
 * - expression,
 * - text-input
 *
 * Processes the following attributes:
 * - name
 * - formula
 * - display
 * - value, value-type, data-style-name (via XMLValueImportHelper)
 * - description.
 *
 * Each attribute has a corresponding member, a bool variable to indicate
 * whether it was set or not, and a bool variable whether it should be set
 * using the standard property name.
 *
 * bValid is set true, when name is found!  
 * (Most variable related fields are valid, if a name is
 * found. However, some are always valid. In this case, setting bValid
 * does not matter.)  
 */
class XMLVarFieldImportContext : public XMLTextFieldImportContext
{
protected:
	const ::rtl::OUString sPropertyContent;
	const ::rtl::OUString sPropertyHint;
	const ::rtl::OUString sPropertyIsVisible;
	const ::rtl::OUString sPropertyIsDisplayFormula;
//STRIP013	const ::rtl::OUString sPropertyCurrentPresentation;

private:
	::rtl::OUString sName;				/// name attribute
	::rtl::OUString sFormula;			/// formula attribute
	::rtl::OUString sDescription;		/// description
	XMLValueImportHelper aValueHelper;	/// value, value-type, and style
	sal_Bool bDisplayFormula;			/// display formula?(rather than value)
	sal_Bool bDisplayNone;				/// hide field?

	sal_Bool bNameOK;					/// sName was set
	sal_Bool bFormulaOK;				/// sFormula was set
	sal_Bool bDescriptionOK;			/// sDescription was set
	sal_Bool bDisplayOK;				/// sDisplayFormula/-None were set

	sal_Bool bSetName;					/// set sName with ???-property
	sal_Bool bSetFormula;				/// set Formula property
	sal_Bool bSetFormulaDefault;		/// use content as default for formula
	sal_Bool bSetDescription;			/// set sDescription with Hint-property
	sal_Bool bSetVisible;				/// set IsVisible
	sal_Bool bSetDisplayFormula;		/// set DisplayFormula (sub type???)
//STRIP013	sal_Bool bSetPresentation;			/// set presentation frm elem. content?

public:

	TYPEINFO();

	XMLVarFieldImportContext(
		// for XMLTextFieldImportContext:
		SvXMLImport& rImport,			/// XML Import
		XMLTextImportHelper& rHlp,		/// text import helper
		const sal_Char* pServiceName,	/// name of SO API service
		sal_uInt16 nPrfx,				/// namespace prefix
		const ::rtl::OUString& rLocalName,	/// element name w/o prefix
		// config variables for PrepareField behavior:
		sal_Bool bName,					/// set sName with ???-property
		sal_Bool bFormula,				/// set Formula property
		sal_Bool bFormulaDefault,		/// use content as default for formula
		sal_Bool bDescription,			/// set sDescription with Hint-property
		sal_Bool bVisible,				/// set IsVisible (display attr)
		sal_Bool bDisplayFormula,		/// set ??? (display attr.)
		sal_Bool bType,					/// set value type with ???-property
		sal_Bool bStyle,				/// set data style (NumberFormat-Prop.)
		sal_Bool bValue);				/// set value with Content/Value-Prop.
//STRIP013		,sal_Bool bPresentation);		/// set presentation from elem. content

protected:
	/// process attribute values
	virtual void ProcessAttribute( sal_uInt16 nAttrToken,
								   const ::rtl::OUString& sAttrValue );

	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);

	// various accessor methods:
	inline ::rtl::OUString GetName()	{ return sName; }
	inline sal_Bool IsStringValue()		{ return aValueHelper.IsStringValue();}
	inline sal_Bool IsNameOK()			{ return bNameOK; }
	inline sal_Bool IsFormulaOK()		{ return bFormulaOK; }
	inline sal_Bool IsDescriptionOK()	{ return bDescriptionOK; }
	inline sal_Bool IsDisplayOK()		{ return bDisplayOK; }
};

/** import variable get fields (<text:variable-get>) */
class XMLVariableGetFieldImportContext : public XMLVarFieldImportContext
{
	const ::rtl::OUString sPropertySubType;

public:

	TYPEINFO();

	XMLVariableGetFieldImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

protected:
	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/** import expression fields (<text:expression>) */
class XMLExpressionFieldImportContext : public XMLVarFieldImportContext
{
	const ::rtl::OUString sPropertySubType;

public:

	TYPEINFO();

	XMLExpressionFieldImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& sLocalName);		/// element name w/o prefix

protected:
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/*** import text input fields (<text:text-input>) */
class XMLTextInputFieldImportContext : public XMLVarFieldImportContext
{
	const ::rtl::OUString sPropertyContent;

public:

	TYPEINFO();

	XMLTextInputFieldImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& sLocalName);		/// element name w/o prefix

protected:
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/**
 * uperclass for variable/user-set, var/user-input, and sequence fields
 * inds field master of appropriate type and attaches field to it.
 */
class XMLSetVarFieldImportContext : public XMLVarFieldImportContext
{
	const VarType eFieldType;

public:

	TYPEINFO();

	XMLSetVarFieldImportContext( 
		// for XMLTextFieldImportContext:
		SvXMLImport& rImport,			/// see XMLTextFieldImportContext
		XMLTextImportHelper& rHlp,		/// see XMLTextFieldImportContext
		const sal_Char* pServiceName,	/// see XMLTextFieldImportContext
		sal_uInt16 nPrfx,				/// see XMLTextFieldImportContext
		const ::rtl::OUString& rLocalName, /// see XMLTextFieldImportContext
		// for finding appropriate field master (see EndElement())
		VarType eVarType,				/// variable type
		// config variables:
		sal_Bool bName,					/// see XMLTextFieldImportContext
		sal_Bool bFormula,				/// see XMLTextFieldImportContext
		sal_Bool bFormulaDefault,		/// see XMLTextFieldImportContext
		sal_Bool bDescription,			/// see XMLTextFieldImportContext
		sal_Bool bVisible,				/// see XMLTextFieldImportContext
		sal_Bool bDisplayFormula,		/// see XMLTextFieldImportContext
		sal_Bool bType,					/// see XMLTextFieldImportContext
		sal_Bool bStyle,				/// see XMLTextFieldImportContext
		sal_Bool bValue);				/// see XMLTextFieldImportContext
//STRIP013		,sal_Bool bPresentation);		/// see XMLTextFieldImportContext

protected:

	/// create XTextField, attach master and insert into document; 
	/// also calls PrepareTextField
	virtual void EndElement();

	/// find appropriate field master
	sal_Bool FindFieldMaster(
		::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xMaster);
};

/** import variable set fields (<text:variable-set>) */
class XMLVariableSetFieldImportContext : public XMLSetVarFieldImportContext
{
	const ::rtl::OUString sPropertySubType;

public:

	TYPEINFO();

	XMLVariableSetFieldImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

protected:
	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/** variable input fields (<text:variable-input>) */
class XMLVariableInputFieldImportContext : public XMLSetVarFieldImportContext
{
	const ::rtl::OUString sPropertySubType;
	const ::rtl::OUString sPropertyIsInput;

public:

	TYPEINFO();

	XMLVariableInputFieldImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx, 						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

protected:

	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/** user fields (<text:user-field-get>) */
class XMLUserFieldImportContext : public XMLSetVarFieldImportContext
{

public:

	TYPEINFO();

	XMLUserFieldImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix
};

/** user input fields (<text:user-field-input>) */
class XMLUserFieldInputImportContext : public XMLVarFieldImportContext
{

public:

	TYPEINFO();

	XMLUserFieldInputImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/** sequence fields (<text:sequence>) */
class XMLSequenceFieldImportContext : public XMLSetVarFieldImportContext
{
	const ::rtl::OUString sPropertyFormula;
	const ::rtl::OUString sPropertyNumberFormat;
	const ::rtl::OUString sPropertySequenceValue;
	::rtl::OUString sFormula;
	::rtl::OUString sNumFormat;
	::rtl::OUString sNumFormatSync;
	::rtl::OUString sRefName;

	sal_Bool bRefNameOK;

public:

	TYPEINFO();

	XMLSequenceFieldImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// Text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

protected:

	/// process attribute values
	virtual void ProcessAttribute( sal_uInt16 nAttrToken,
								   const ::rtl::OUString& sAttrValue );

	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/**
 * variable declaration container for all variable fields
 * 		(variable-decls, user-field-decls, sequence-decls)
 */
class XMLVariableDeclsImportContext : public SvXMLImportContext
{
	enum VarType eVarDeclsContextType;
	XMLTextImportHelper& rImportHelper;

public:

	TYPEINFO();

	XMLVariableDeclsImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName,		/// element name w/o prefix
		enum VarType eVarType);					/// variable type

	virtual SvXMLImportContext *CreateChildContext( 
		sal_uInt16 nPrefix,
		const ::rtl::OUString& rLocalName,
		const ::com::sun::star::uno::Reference< 
		::com::sun::star::xml::sax::XAttributeList> & xAttrList );
};

/**
 * variable field declarations
 * 		(variable-decl, user-field-decl, sequence-decl)
 */
class XMLVariableDeclImportContext : public SvXMLImportContext
{
	const ::rtl::OUString sPropertyName;
	const ::rtl::OUString sPropertySubType;
	const ::rtl::OUString sPropertyNumberingLevel;
	const ::rtl::OUString sPropertyNumberingSeparator;
	const ::rtl::OUString sPropertyIsExpression;

	::rtl::OUString sName;
	XMLValueImportHelper aValueHelper;
	sal_Int8 nNumLevel;
	sal_Unicode cSeparationChar;

public:

	TYPEINFO();

	XMLVariableDeclImportContext( 
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName,		/// element name w/o prefix
		const ::com::sun::star::uno::Reference< /// list of element attributes
		::com::sun::star::xml::sax::XAttributeList> & xAttrList,
		enum VarType eVarType);					/// variable type

	/// get field master for name and rename if appropriate
	static sal_Bool FindFieldMaster(::com::sun::star::uno::Reference<
									::com::sun::star::beans::XPropertySet> & xMaster,
									SvXMLImport& rImport, 
									XMLTextImportHelper& rHelper,
									const ::rtl::OUString& sVarName,
									enum VarType eVarType);
};

/** import table formula fields (deprecated; for Writer 2.0 compatibility) */
class XMLTableFormulaImportContext : public XMLTextFieldImportContext
{
	const ::rtl::OUString sPropertyNumberFormat;
	const ::rtl::OUString sPropertyContent;
    const ::rtl::OUString sPropertyIsShowFormula;
//STRIP013	const ::rtl::OUString sPropertyCurrentPresentation;

	XMLValueImportHelper aValueHelper;

	::rtl::OUString sFormula;
	sal_Bool bFormulaOK;

    sal_Bool bIsShowFormula;

public:

	TYPEINFO();

	XMLTableFormulaImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix
    virtual ~XMLTableFormulaImportContext();

protected:

	/// process attribute values
	virtual void ProcessAttribute( sal_uInt16 nAttrToken,
								   const ::rtl::OUString& sAttrValue );

	/// prepare XTextField for insertion into document
	virtual void PrepareField( 
		const ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet> & xPropertySet);
};

/** import database display fields (<text:database-display>) */
class XMLDatabaseDisplayImportContext : public XMLDatabaseFieldImportContext
{
	const ::rtl::OUString sPropertyColumnName;
	const ::rtl::OUString sPropertyDatabaseFormat;
//STRIP013	const ::rtl::OUString sPropertyCurrentPresentation;
    const ::rtl::OUString sPropertyIsVisible;

	XMLValueImportHelper aValueHelper;

	::rtl::OUString sColumnName;
	sal_Bool bColumnOK;

    sal_Bool bDisplay;
    sal_Bool bDisplayOK;

public:

	TYPEINFO();

	XMLDatabaseDisplayImportContext(
		SvXMLImport& rImport,					/// XML Import
		XMLTextImportHelper& rHlp,				/// text import helper
		sal_uInt16 nPrfx,						/// namespace prefix
		const ::rtl::OUString& rLocalName);		/// element name w/o prefix

protected:

	/// process attribute values
	virtual void ProcessAttribute( sal_uInt16 nAttrToken,
								   const ::rtl::OUString& sAttrValue );

	/// create, prepare and insert database field master and database field
	virtual void EndElement();
};

}//end of namespace binfilter
#endif
