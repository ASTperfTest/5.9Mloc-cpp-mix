/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: presvish.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:29:53 $
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

#ifndef _SD_PRESVISH_HXX
#define _SD_PRESVISH_HXX

#include "drviewsh.hxx"
namespace binfilter {

/*************************************************************************
|*
|* Beschreibung SdPresViewShell ist die ViewShell fuer die Praes.
|*
\************************************************************************/

class SdPresViewShell : public SdDrawViewShell
{
private:

    Rectangle       maOldVisArea;
    sal_Bool        mbShowStarted;
                    
                    
	SdPresViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell) : SdDrawViewShell( pFrame, pOldShell ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 		            SdPresViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell);


public:
		
		            TYPEINFO();
                    
		            SFX_DECL_VIEWFACTORY( SdPresViewShell );
		            SFX_DECL_INTERFACE( SD_IF_SDPRESVIEWSHELL );
                    
};

} //namespace binfilter
#endif // _SD_SLIDVISH_HXX
