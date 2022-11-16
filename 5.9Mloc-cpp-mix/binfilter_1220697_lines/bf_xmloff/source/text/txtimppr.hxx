/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: txtimppr.hxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 10:14:35 $
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
#ifndef _XMLOFF_TXTIMPPR_HXX
#define _XMLOFF_TXTIMPPR_HXX

#ifndef _XMLOFF_XMLIMPPR_HXX
#include "xmlimppr.hxx"
#endif
#ifndef _XMLOFF_XMLICTXT_HXX 
#include "xmlictxt.hxx"
#endif
namespace binfilter {

class XMLFontStylesContext;

class XMLTextImportPropertyMapper : public SvXMLImportPropertyMapper
{
	sal_Int32 nSizeTypeIndex;
	SvXMLImportContextRef xFontDecls;

	void FontFinished(
				XMLPropertyState *pFontFamilyNameState,
				XMLPropertyState *pFontStyleNameState,
				XMLPropertyState *pFontFamilyState,
				XMLPropertyState *pFontPitchState,
				XMLPropertyState *pFontCharsetState ) const;

	void FontDefaultsCheck(
				XMLPropertyState*pFontFamilyName,
				XMLPropertyState* pFontStyleName,
				XMLPropertyState* pFontFamily,
				XMLPropertyState* pFontPitch,
				XMLPropertyState* pFontCharSet,
				XMLPropertyState** ppNewFontStyleName,
				XMLPropertyState** ppNewFontFamily,
				XMLPropertyState** ppNewFontPitch,
				XMLPropertyState** ppNewFontCharSet ) const;

protected:
	virtual sal_Bool handleSpecialItem(
			XMLPropertyState& rProperty,
			::std::vector< XMLPropertyState >& rProperties,
			const ::rtl::OUString& rValue,
			const SvXMLUnitConverter& rUnitConverter,
			const SvXMLNamespaceMap& rNamespaceMap ) const;
public:
	XMLTextImportPropertyMapper(
			const UniReference< XMLPropertySetMapper >& rMapper,
            SvXMLImport& rImport,
			XMLFontStylesContext *pFontDecls = 0 );
	virtual ~XMLTextImportPropertyMapper();

	void SetFontDecls( XMLFontStylesContext *pFontDecls );

	/** This method is called when all attributes have benn processed. It may be used to remove items that are incomplete */
	virtual void finished(
			::std::vector< XMLPropertyState >& rProperties,
			sal_Int32 nStartIndex, sal_Int32 nEndIndex ) const;
};

}//end of namespace binfilter
#endif	//  _XMLOFF_XMLIMPPR_HXX