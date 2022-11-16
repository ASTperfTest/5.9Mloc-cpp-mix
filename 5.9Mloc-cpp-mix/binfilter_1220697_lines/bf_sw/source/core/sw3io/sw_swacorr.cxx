/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_swacorr.cxx,v $
 *
 *  $Revision: 1.9 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 14:06:48 $
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

#ifndef _SWACORR_HXX
#include <swacorr.hxx>
#endif
#ifndef _SWSERROR_H
#include <swerror.h>
#endif

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif
#ifndef _VISCRS_HXX
#include <viscrs.hxx>
#endif
#ifndef _EDITSH_HXX
#include <editsh.hxx>
#endif
#include "bf_so3/staticbaseurl.hxx"
namespace binfilter {



	//	- return den Ersetzungstext (nur fuer SWG-Format, alle anderen
	//		koennen aus der Wortliste herausgeholt werden!)
	//		rShort ist der Stream-Name - gecryptet!


	//	- Text mit Attributierung (kann nur der SWG - SWG-Format!)
	//		rShort ist der Stream-Name - gecryptet!


/*N*/ SwAutoCorrect::SwAutoCorrect( const SvxAutoCorrect& rACorr )
/*N*/ 	: SvxAutoCorrect( rACorr )
/*N*/ {
/*N*/ 	SwEditShell::SetAutoFmtFlags(&GetSwFlags());
/*N*/ }

/*N*/ SwAutoCorrect::~SwAutoCorrect()
/*N*/ {
/*N*/ }
}
