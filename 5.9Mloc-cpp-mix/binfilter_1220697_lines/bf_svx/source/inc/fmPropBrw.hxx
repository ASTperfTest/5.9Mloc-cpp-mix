/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fmPropBrw.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 21:07:39 $
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
#ifndef SVX_FMPROPBRW_HXX
#define SVX_FMPROPBRW_HXX

#ifndef _COM_SUN_STAR_AWT_XCONTROLCONTAINER_HPP_ 
#include <com/sun/star/awt/XControlContainer.hpp>
#endif
#ifndef _SFX_CHILDWIN_HXX 
#include <bf_sfx2/childwin.hxx>
#endif
namespace binfilter {

//========================================================================
class FmPropBrwMgr : public SfxChildWindow
{
protected:
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XControlContainer >
					m_xUnoRepresentation;
public:
	FmPropBrwMgr(Window *pParent, sal_uInt16 nId, SfxBindings *pBindings, SfxChildWinInfo *pInfo);
	SFX_DECL_CHILDWINDOW(FmPropBrwMgr);
};

class FmPropControl;
class SfxBindings;
//========================================================================
}//end of namespace binfilter
#endif //SVX_FMPROPBRW_HXX