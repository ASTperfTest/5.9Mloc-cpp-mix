/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: itabenum.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 16:12:51 $
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
#ifndef _ITABENUM_HXX
#define _ITABENUM_HXX
namespace binfilter {

enum SwTableInsMode
{
	DEFAULT_BORDER     = 0x01,
	HEADLINE           = 0x02,
	REPEAT             = 0x04,
	HEADLINE_REPEAT    = 0x06,	// Headline + Repeat
	SPLIT_LAYOUT       = 0x08,
	HEADLINE_NO_BORDER = HEADLINE_REPEAT|SPLIT_LAYOUT,
	ALL_TBL_INS_ATTR   = DEFAULT_BORDER|HEADLINE_REPEAT|SPLIT_LAYOUT
};


} //namespace binfilter
#endif

