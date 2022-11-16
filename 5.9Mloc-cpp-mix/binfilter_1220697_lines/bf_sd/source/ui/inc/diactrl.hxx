/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: diactrl.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: obo $ $Date: 2007/03/09 14:39:27 $
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

#ifndef _SD_DIACTRL_HXX
#define _SD_DIACTRL_HXX

#ifndef _SFXINTITEM_HXX //autogen
#include <svtools/intitem.hxx>
#endif

#ifndef _SFX_BINDINGS_HXX
#include <bf_sfx2/bindings.hxx>
#endif
#ifndef _SV_FIXED_HXX //autogen
#include <vcl/fixed.hxx>
#endif

#ifndef _SV_FIELD_HXX //autogen
#include <vcl/field.hxx>
#endif

#ifndef _SV_TOOLBOX_HXX //autogen
#include <vcl/toolbox.hxx>
#endif

#ifndef _SFXTBXCTRL_HXX //autogen
#include <bf_sfx2/tbxctrl.hxx>
#endif
namespace binfilter {


#ifdef _SD_DIACTRL_CXX

/*************************************************************************
|*
|* Toolbox-Controller fuer Diaeffekte
|*
\************************************************************************/

class SdTbxCtlDiaEffect: public SfxToolBoxControl
{
public:
			SFX_DECL_TOOLBOX_CONTROL();
			SdTbxCtlDiaEffect( USHORT nId, ToolBox& rTbx, SfxBindings& rBindings );
};

class SdTbxCtlDiaSpeed: public SfxToolBoxControl
{
public:
			SFX_DECL_TOOLBOX_CONTROL();
			SdTbxCtlDiaSpeed( USHORT nId, ToolBox& rTbx, SfxBindings& rBindings );
};

class SdTbxCtlDiaAuto: public SfxToolBoxControl
{
public: 
			SFX_DECL_TOOLBOX_CONTROL();

			SdTbxCtlDiaAuto( USHORT nId, ToolBox& rTbx, SfxBindings& rBindings );
};

class SdTbxCtlDiaTime: public SfxToolBoxControl
{
public:
			SFX_DECL_TOOLBOX_CONTROL();

			SdTbxCtlDiaTime( USHORT nId, ToolBox& rTbx, SfxBindings& rBindings );
};

//========================================================================
// SdTbxCtlDiaPages:
//========================================================================

class SdTbxCtlDiaPages : public SfxToolBoxControl
{
public:

	SFX_DECL_TOOLBOX_CONTROL();

	SdTbxCtlDiaPages( USHORT nId, ToolBox& rTbx, SfxBindings& rBind );
};

#endif // _SD_DIACTRL_CXX

} //namespace binfilter
#endif // _SD_TBXCTRLS_HXX



