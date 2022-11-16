/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_undraw.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:46:26 $
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

#include <rtl/string.h>

#ifndef _RTL_MEMORY_H
#include <rtl/memory.h>
#endif


#ifndef _RTL_STRING_H
#include <rtl/string.h>
#endif

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif
#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _FRAME_HXX
#include <frame.hxx>
#endif
namespace binfilter {

#ifdef _MSC_VER
#pragma optimize("elg",off)
#endif




// Draw-Objecte

/*N*/ IMPL_LINK( SwDoc, AddDrawUndo, SdrUndoAction *, pUndo )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*N*/  	return 0;
/*N*/  }






//--------------------------------------------











// ------------------------------






//-------------------------------------






}
