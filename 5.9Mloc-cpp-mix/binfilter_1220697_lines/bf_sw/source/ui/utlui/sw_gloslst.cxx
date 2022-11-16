/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_gloslst.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 13:13:33 $
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

#define _SVSTDARR_STRINGSDTOR
#define _SVSTDARR_STRINGSISORTDTOR
#define _SVSTDARR_STRINGS


#ifndef _SHELLIO_HXX
#include <shellio.hxx>
#endif
#ifndef _GLOSLST_HXX
#include <gloslst.hxx>
#endif

#ifndef _UTLUI_HRC
#include <utlui.hrc>
#endif
#ifndef _GLOSLST_HRC
#include <gloslst.hrc>
#endif
namespace binfilter {


#define STRING_DELIM (char)0x0A
#define GLOS_TIMEOUT 30000   // alle 30 s updaten
#define FIND_MAX_GLOS 20





/*-----------------21.01.97 13.25-------------------

--------------------------------------------------*/


/*-----------------21.01.97 13.25-------------------

--------------------------------------------------*/

/*-----------------21.01.97 13.29-------------------

--------------------------------------------------*/


/********************************************************************

********************************************************************/


/*N*/ SwGlossaryList::SwGlossaryList() :
/*N*/ 	bFilled(FALSE)
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	SvtPathOptions aPathOpt;
/*N*/ }

/********************************************************************

********************************************************************/


/*N*/ SwGlossaryList::~SwGlossaryList()
/*N*/ {
DBG_BF_ASSERT(0, "STRIP"); //STRIP001  	ClearGroups();
/*N*/ }

/********************************************************************
 * Wenn der GroupName bereits bekannt ist, dann wird nur
 * rShortName gefuellt, sonst wird rGroupName ebenfals gesetzt und
 * bei Bedarf nach der richtigen Gruppe gefragt
********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************

********************************************************************/



/********************************************************************
	Alle (nicht mehr als FIND_MAX_GLOS) gefunden Bausteine mit
	passendem Anfang zurueckgeben
********************************************************************/


/********************************************************************

********************************************************************/
/*N*/ void	SwGlossaryList::ClearGroups()
/*N*/ {
/*N*/ 	USHORT nCount = aGroupArr.Count();
/*N*/ 	for( USHORT i = 0; i < nCount; ++i )
/*N*/ 		delete aGroupArr.GetObject( i );
/*N*/ 
/*N*/ 	aGroupArr.Remove( 0, nCount );
/*N*/ 	bFilled = FALSE;
/*N*/ }


}
