/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: cellfrm.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 22:42:48 $
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
#ifndef _CELLFRM_HXX
#define _CELLFRM_HXX

#ifndef _SVMEMPOOL_HXX //autogen
#include <tools/mempool.hxx>
#endif

#include "layfrm.hxx"
namespace binfilter {

class SwTableBox;
struct SwCrsrMoveState;
class SwBorderAttrs;

class SwCellFrm: public SwLayoutFrm
{
	const SwTableBox *pTabBox;

protected:
	virtual void Format( const SwBorderAttrs *pAttrs = 0 );

public:
	SwCellFrm( const SwTableBox & );
	~SwCellFrm();

	virtual	void   Modify( SfxPoolItem*, SfxPoolItem* );

	const SwTableBox *GetTabBox() const { return pTabBox; }
	DECL_FIXEDMEMPOOL_NEWDEL(SwCellFrm)
};

} //namespace binfilter
#endif
