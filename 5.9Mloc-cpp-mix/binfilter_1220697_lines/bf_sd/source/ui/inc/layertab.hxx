/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: layertab.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:28:31 $
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

#ifndef _SD_LAYERTAB_HXX
#define _SD_LAYERTAB_HXX


#ifndef _TABBAR_HXX //autogen wg. TabBar
#include <svtools/tabbar.hxx>
#endif
#ifndef _TRANSFER_HXX
#include <svtools/transfer.hxx>
#endif
namespace binfilter {

/*************************************************************************
|*
|* TabBar fuer die Layerverwaltung
|*
\************************************************************************/

class SdDrawViewShell;

class SdLayerTab : public TabBar, public DropTargetHelper
{
protected:

	SdDrawViewShell*	pDrViewSh;

	// TabBar
						
						
						

	// DropTargetHelper

 public:

						SdLayerTab(SdDrawViewShell* pDrViewSh, Window* pParent);
						~SdLayerTab();

    /** Inform all listeners of this control that the current layer has been
        activated.  Call this method after switching the current layer and is
        not done elsewhere (like when using ctrl + page up/down keys).
    */
    void SendActivatePageEvent (void);

    /** Inform all listeners of this control that the current layer has been
        deactivated.  Call this method before switching the current layer
        and is not done elsewhere (like when using ctrl page up/down keys).
    */
    void SendDeactivatePageEvent (void);
};

} //namespace binfilter
#endif // _SD_LAYERTAB_HXX
