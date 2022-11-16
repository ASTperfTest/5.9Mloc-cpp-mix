/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdxfer.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 13:36:36 $
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

#ifndef _SD_SDXFER_HXX
#define _SD_SDXFER_HXX

#ifndef _TRANSFER_HXX
#include <svtools/transfer.hxx>
#endif
#ifndef _EMBOBJ_HXX //autogen
#include <bf_so3/embobj.hxx>
#endif
#ifndef _SV_GRAPH_HXX //autogen
#include <vcl/graph.hxx>
#endif
class ImageMap;
class VirtualDevice;
namespace binfilter {

// ------------------
// - SdTransferable -
// ------------------

class SdView;
class SdDrawDocument;
class INetBookmark;
class SdDrawDocShell;

class SdTransferable : public TransferableHelper
{

									
public:								
									
	const SdView*					GetView() const {DBG_BF_ASSERT(0, "STRIP");return NULL; } //STRIP001 const SdView*					GetView() const { return pSdView; }
									
									
									
									



};

} //namespace binfilter
#endif // _SD_SDXFER_HXX
