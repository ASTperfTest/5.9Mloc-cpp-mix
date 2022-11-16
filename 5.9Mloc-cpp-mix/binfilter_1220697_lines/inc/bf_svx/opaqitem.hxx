/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: opaqitem.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 03:47:14 $
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
#ifndef _SVX_OPAQITEM_HXX
#define _SVX_OPAQITEM_HXX

// include ---------------------------------------------------------------

#ifndef _SFXENUMITEM_HXX //autogen
#include <svtools/eitem.hxx>
#endif

namespace rtl
{
	class OUString;
}
namespace binfilter {
class SvXMLUnitConverter;
// class SvxOpaqueItem ---------------------------------------------------

/*
[Beschreibung]
Dieses Item beschreibt eine logische Variable "Undurchsichtig ja oder nein".
*/

class SvxOpaqueItem : public SfxBoolItem
{
public:
	TYPEINFO();

	SvxOpaqueItem( const USHORT nId = ITEMID_OPAQUE, const BOOL bOpa = TRUE );
	inline SvxOpaqueItem &operator=( const SvxOpaqueItem &rCpy );

	// "pure virtual Methoden" vom SfxPoolItem
	virtual SfxPoolItem*    Clone( SfxItemPool *pPool = 0 ) const;
	virtual SfxPoolItem*    Create(SvStream &, USHORT) const;
	virtual SvStream&		Store(SvStream &, USHORT nItemVersion ) const;

};

inline SvxOpaqueItem::SvxOpaqueItem( const USHORT nId, const BOOL bOpa )
	: SfxBoolItem( nId, bOpa )
{}

inline SvxOpaqueItem &SvxOpaqueItem::operator=( const SvxOpaqueItem &rCpy )
{
	SetValue( rCpy.GetValue() );
	return *this;
}

}//end of namespace binfilter
#endif
