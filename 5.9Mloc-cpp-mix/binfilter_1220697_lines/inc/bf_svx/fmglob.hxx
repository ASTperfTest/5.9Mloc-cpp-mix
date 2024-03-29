/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fmglob.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 13:27:16 $
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

#ifndef _SVX_FMGLOB_HXX
#define _SVX_FMGLOB_HXX

#include <tools/solar.h>

#ifndef _SVDOBJ_HXX
#include <bf_svx/svdobj.hxx>
#endif

#ifndef _COM_SUN_STAR_FORM_FORMCOMPONENTTYPE_HPP_
#include <com/sun/star/form/FormComponentType.hpp>
#endif
namespace binfilter {

const sal_uInt32 FmFormInventor = sal_uInt32('F')*0x00000001+
							  sal_uInt32('M')*0x00000100+
							  sal_uInt32('0')*0x00010000+
							  sal_uInt32('1')*0x01000000;

const sal_uInt16 nOffset = (sal_uInt16)OBJ_UNO;	// Offset, damit Ids nicht mit Ids aus SvDraw kollidieren

const sal_uInt16 OBJ_FM_CONTROL			=	::com::sun::star::form::FormComponentType::CONTROL+nOffset;		// allgemeiner Kennzeichner
																	// fuer FormularKomponenten
const sal_uInt16 OBJ_FM_EDIT			=	::com::sun::star::form::FormComponentType::TEXTFIELD+nOffset;
const sal_uInt16 OBJ_FM_BUTTON			=	::com::sun::star::form::FormComponentType::COMMANDBUTTON+nOffset;
const sal_uInt16 OBJ_FM_FIXEDTEXT		=	::com::sun::star::form::FormComponentType::FIXEDTEXT+nOffset;
const sal_uInt16 OBJ_FM_LISTBOX			=	::com::sun::star::form::FormComponentType::LISTBOX+nOffset;
const sal_uInt16 OBJ_FM_CHECKBOX		=	::com::sun::star::form::FormComponentType::CHECKBOX+nOffset;
const sal_uInt16 OBJ_FM_COMBOBOX		=	::com::sun::star::form::FormComponentType::COMBOBOX+nOffset;
const sal_uInt16 OBJ_FM_RADIOBUTTON		=	::com::sun::star::form::FormComponentType::RADIOBUTTON+nOffset;
const sal_uInt16 OBJ_FM_GROUPBOX		=	::com::sun::star::form::FormComponentType::GROUPBOX+nOffset;
const sal_uInt16 OBJ_FM_GRID			=	::com::sun::star::form::FormComponentType::GRIDCONTROL+nOffset;
const sal_uInt16 OBJ_FM_IMAGEBUTTON		=	::com::sun::star::form::FormComponentType::IMAGEBUTTON+nOffset;
const sal_uInt16 OBJ_FM_FILECONTROL		=	::com::sun::star::form::FormComponentType::FILECONTROL+nOffset;
const sal_uInt16 OBJ_FM_DATEFIELD		=	::com::sun::star::form::FormComponentType::DATEFIELD+nOffset;
const sal_uInt16 OBJ_FM_TIMEFIELD		=	::com::sun::star::form::FormComponentType::TIMEFIELD+nOffset;
const sal_uInt16 OBJ_FM_NUMERICFIELD	=	::com::sun::star::form::FormComponentType::NUMERICFIELD+nOffset;
const sal_uInt16 OBJ_FM_CURRENCYFIELD	=	::com::sun::star::form::FormComponentType::CURRENCYFIELD+nOffset;
const sal_uInt16 OBJ_FM_PATTERNFIELD	=	::com::sun::star::form::FormComponentType::PATTERNFIELD+nOffset;
const sal_uInt16 OBJ_FM_HIDDEN			=	::com::sun::star::form::FormComponentType::HIDDENCONTROL+nOffset;
const sal_uInt16 OBJ_FM_IMAGECONTROL	=	::com::sun::star::form::FormComponentType::IMAGECONTROL+nOffset;
const sal_uInt16 OBJ_FM_FORMATTEDFIELD	=	::com::sun::star::form::FormComponentType::PATTERNFIELD+nOffset+1;
	// keine eigene ID fuer formatted field, sondern groesste bekannte + 1

}//end of namespace binfilter
#endif      // _FM_FMGLOB_HXX

