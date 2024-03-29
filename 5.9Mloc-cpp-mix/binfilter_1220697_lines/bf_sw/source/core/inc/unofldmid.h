/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: unofldmid.h,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 09:42:20 $
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
#ifndef SW_UNOFLDMID_H
#define SW_UNOFLDMID_H

class String;
namespace com { namespace sun { namespace star { namespace uno {
	class Any;
} } } }

/******************************************************************************
 *
 ******************************************************************************/
//Abbildung der Properties auf den Descriptor
#define FIELD_PROP_PAR1   			10
#define FIELD_PROP_PAR2             11
#define FIELD_PROP_PAR3             12
#define FIELD_PROP_FORMAT           13
#define FIELD_PROP_SUBTYPE          14
#define FIELD_PROP_BOOL1			15
#define FIELD_PROP_BOOL2			16
#define FIELD_PROP_DATE				17
#define FIELD_PROP_USHORT1          18
#define FIELD_PROP_USHORT2          19
#define FIELD_PROP_BYTE1			20
#define FIELD_PROP_DOUBLE           21
#define FIELD_PROP_BOOL3			22
#define FIELD_PROP_PAR4             23
#define FIELD_PROP_SHORT1           24
#define FIELD_PROP_DATE_TIME		25
#define FIELD_PROP_PROP_SEQ			26
#define FIELD_PROP_LOCALE			27
#define FIELD_PROP_BOOL4            28
#define FIELD_PROP_STRINGS          29

String& GetString( const com::sun::star::uno::Any&, String& rStr );

#endif
