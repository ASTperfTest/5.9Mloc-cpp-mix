/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: gluectrl.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:28:09 $
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

#ifndef SD_GLUECTRL_HXX
#define SD_GLUECTRL_HXX

#ifndef _SV_LSTBOX_HXX //autogen
#include <vcl/lstbox.hxx>
#endif
#ifndef _SFXTBXCTRL_HXX //autogen
#include <bf_sfx2/tbxctrl.hxx>
#endif
namespace binfilter {

/*************************************************************************
|*
|* GluePointEscDirLB
|*
\************************************************************************/

/*************************************************************************
|*
|* Toolbox-Controller fuer Klebepunkte-Austrittsrichtung
|*
\************************************************************************/

class SdTbxCtlGlueEscDir: public SfxToolBoxControl
{
public:

			SFX_DECL_TOOLBOX_CONTROL();

			SdTbxCtlGlueEscDir( USHORT nId, ToolBox& rTbx, SfxBindings& rBindings );
};

} //namespace binfilter
#endif // SD_GLUECTRL_HXX

