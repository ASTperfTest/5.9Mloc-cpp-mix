/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: tabwin.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 21:12:56 $
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
#ifndef _SVX_TABWIN_HXX
#define _SVX_TABWIN_HXX




#ifndef _SFX_CHILDWIN_HXX //autogen
#include <bf_sfx2/childwin.hxx>
#endif



//#ifndef _SVX_FMTOOLS_HXX
//#include "fmtools.hxx"
//#endif

namespace binfilter {
//end of namespace binfilter
//==================================================================
class FmFieldWin;

//========================================================================
class FmFormShell;

//========================================================================

//========================================================================
class FmFieldWinMgr : public SfxChildWindow
{
public:
	FmFieldWinMgr(Window *pParent, sal_uInt16 nId,
		SfxBindings *pBindings, SfxChildWinInfo *pInfo);
	SFX_DECL_CHILDWINDOW(FmFieldWinMgr);
};


}
#endif

