/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_sddll1.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:21:46 $
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

#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "sddll.hxx"
#include "sdmod.hxx"
#include "app.hrc"
#include "drawdoc.hxx"
#include "grdocsh.hxx"

namespace binfilter {



/*************************************************************************
|*
|* Register all Factorys
|*
\************************************************************************/


/*N*/ void SdDLL::RegisterFactorys()
/*N*/ {
/*N*/ }



/*************************************************************************
|*
|* Register all Interfaces
|*
\************************************************************************/


/*N*/ void SdDLL::RegisterInterfaces()
/*N*/ {
/*N*/ 	// Modul
/*N*/ 	SfxModule* pMod = SD_MOD();
/*N*/ 	SdModule::RegisterInterface(pMod);
/*N*/ 
/*N*/ 	// DocShells
/*N*/ 	SdDrawDocShell::RegisterInterface(pMod);
/*N*/ 	SdGraphicDocShell::RegisterInterface(pMod);
/*N*/ 
/*N*/ }
}
