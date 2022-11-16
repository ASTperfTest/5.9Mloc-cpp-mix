/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: transobj.hxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 13:32:27 $
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

#ifndef SC_TRANSOBJ_HXX
#define SC_TRANSOBJ_HXX

#ifndef _TRANSFER_HXX
#include <svtools/transfer.hxx>
#endif

#ifndef _EMBOBJ_HXX 
#include <bf_so3/embobj.hxx>
#endif

#ifndef SC_SCGLOB_HXX
#include "global.hxx"
#endif
namespace com { namespace sun { namespace star {
	namespace sheet {
		class XSheetCellRanges;
	}
}}}

namespace binfilter {

class ScDocShell;
class ScMarkData;

//STRIP008 namespace com { namespace sun { namespace star {
//STRIP008 	namespace sheet {
//STRIP008 		class XSheetCellRanges;
//STRIP008 	}
//STRIP008 }}}


class ScTransferObj : public TransferableHelper
{
private:
	ScDocument*						pDoc;
	ScRange							aBlock;
	USHORT							nNonFiltered;		// non-filtered rows
	TransferableDataHelper			aOleData;
	TransferableObjectDescriptor	aObjDesc;
	SvEmbeddedObjectRef				aDocShellRef;
	SvEmbeddedObjectRef				aDrawPersistRef;
	::com::sun::star::uno::Reference< ::com::sun::star::sheet::XSheetCellRanges> xDragSourceRanges;
	USHORT							nDragHandleX;
	USHORT							nDragHandleY;
	USHORT							nVisibleTab;
	USHORT							nDragSourceFlags;
	BOOL							bDragWasInternal;
	BOOL							bUsedForLink;


public:
			ScTransferObj( ScDocument* pClipDoc, const TransferableObjectDescriptor& rDesc ){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 ScTransferObj( ScDocument* pClipDoc, const TransferableObjectDescriptor& rDesc );
	virtual ~ScTransferObj(){DBG_BF_ASSERT(0, "STRIP");}; //STRIP001 virtual ~ScTransferObj();


	static ScTransferObj* GetOwnClipboard( Window* pUIWin );

};

} //namespace binfilter
#endif

