/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: prvwshll.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:30:25 $
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

#ifndef _SD_PRVWSHLL_HXX
#define _SD_PRVWSHLL_HXX

#include "drviewsh.hxx"
namespace binfilter {

/**
   The class SdPreviewViewShell provides a convenient way to preview the
   first page of a Draw/Impress document. No user interaction is possible, 
   although a living document is displayed. The view is inherited from 
   the SdDrawViewShell.
 */

class SdPreviewViewShell : public SdDrawViewShell
{
public:		
	TYPEINFO();
    
	SFX_DECL_VIEWFACTORY( SdPreviewViewShell );
	SFX_DECL_INTERFACE( SD_IF_SDPREVIEWVIEWSHELL );
    
	SdPreviewViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell) : SdDrawViewShell( pFrame, pOldShell ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 	SdPreviewViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell);

	// ensure usage of full window for preview

	// own functions for user input to prevent interaction

	// prevent display of rulers 
};

} //namespace binfilter
#endif // _SD_PRVWSHLL_HXX
