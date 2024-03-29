/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: drbezob.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 00:36:36 $
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

#ifndef _SD_DRBEZOB_HXX
#define _SD_DRBEZOB_HXX


#ifndef _SFXMODULE_HXX //autogen
#include <bf_sfx2/module.hxx>
#endif
#ifndef _SFX_SHELL_HXX //autogen
#include <bf_sfx2/shell.hxx>
#endif

#ifndef _SD_GLOB_HXX
#include "glob.hxx"
#endif
namespace binfilter {

class SdViewShell;
class SdView;

/************************************************************************/

class SdDrawBezierObjectBar : public SfxShell
{
protected:
	SdView*       pView;
	SdViewShell*  pViewSh;

public:
	TYPEINFO();
	SFX_DECL_INTERFACE(SD_IF_SDDRAWBEZIEROBJECTBAR);

	 SdDrawBezierObjectBar(SdViewShell* pSdViewShell, SdView* pSdView);
	~SdDrawBezierObjectBar();

	void GetAttrState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void GetAttrState(SfxItemSet& rSet);
	void Execute(SfxRequest &rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void Execute(SfxRequest &rReq);
};



} //namespace binfilter
#endif			// _SD_DRBEZOB_HXX
