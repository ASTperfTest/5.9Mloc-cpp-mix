/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_swblocks.cxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 12:31:02 $
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



#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif


#ifndef _ERRHDL_HXX
#include <errhdl.hxx>
#endif

#ifndef _SHELLIO_HXX
#include <shellio.hxx>
#endif
#ifndef _SWBLOCKS_HXX
#include <swblocks.hxx>
#endif
#ifndef _NDTXT_HXX
#include <ndtxt.hxx>
#endif

#ifndef _STATSTR_HRC
#include <statstr.hrc>
#endif
#ifndef _SWSWERROR_H
#include <swerror.h>
#endif
namespace binfilter {


//////////////////////////////////////////////////////////////////////////

// Hash-Code errechnen (muss nicht eindeutig sein)






// Ist die angegebene Datei ein Storage oder gibt es sie nicht?






// Loeschen des Inhaltes des Dokuments


// Erzeugen eines PaMs, der das ganze Dokument umfasst



// Case Insensitive


















////////////////////////////////////////////////////////////////////////////


/*N*/ SwTextBlocks::SwTextBlocks( const String& rFile )
/*N*/ 	: pImp( 0 ), nErr( 0 )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*N*/ }


/*N*/ SwTextBlocks::~SwTextBlocks()
/*N*/ {
/*N*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 delete pImp;
/*N*/ }


/*N*/ const String& SwTextBlocks::GetName()
/*N*/ {
/*?*/	return pImp ? pImp->aName : aEmptyStr;
/*N*/ }


/*N*/ void SwTextBlocks::SetName( const String& r )
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 if( pImp )
/*N*/ }


/*N*/ BOOL SwTextBlocks::IsOld() const
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*N*/ 	return FALSE;
/*N*/ }


/*N*/ ULONG SwTextBlocks::ConvertToNew()
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 
/*N*/ }


/*N*/ USHORT SwTextBlocks::GetCount() const
/*N*/ {
/*?*/ DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 	return pImp ? pImp->GetCount() : 0;
/*N*/ }


/*N*/ USHORT SwTextBlocks::GetIndex( const String& r ) const
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 return pImp ? pImp->GetIndex( r ) : (USHORT) -1;
/*N*/ }


/*N*/ USHORT SwTextBlocks::GetLongIndex( const String& r ) const
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP");return 0; //STRIP001 return pImp ? (USHORT)(pImp->GetLongIndex( r )) : (USHORT) -1;
/*N*/ }


/*N*/ const String& SwTextBlocks::GetShortName( USHORT n ) const
/*N*/ {
/*?*/ DBG_BF_ASSERT(0, "STRIP"); //STRIP001 	if( pImp )
/*?*/ 	return aEmptyStr;
/*N*/ }


/*N*/ const String& SwTextBlocks::GetLongName( USHORT n ) const
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 if( pImp )
/*?*/ 	return aEmptyStr;
/*N*/ }


/*N*/ BOOL SwTextBlocks::Delete( USHORT n )
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 if( pImp && !pImp->bInPutMuchBlocks )
/*?*/ 	return FALSE;
/*N*/ }


/*N*/ USHORT SwTextBlocks::Rename( USHORT n, const String* s, const String* l )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 
/*N*/ }






/*N*/ BOOL SwTextBlocks::BeginPutDoc( const String& s, const String& l )
/*N*/ {DBG_BF_ASSERT(0, "STRIP"); return FALSE;//STRIP001 
/*N*/ }


/*N*/ USHORT SwTextBlocks::PutDoc()
/*N*/ {DBG_BF_ASSERT(0, "STRIP");return FALSE; //STRIP001 
/*N*/ }




/*N*/ USHORT SwTextBlocks::PutText( const String& rShort, const String& rName,
/*N*/ 							  const String& rTxt )
/*?*/ {DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 
/*N*/ }


/*N*/ SwDoc* SwTextBlocks::GetDoc()
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); return NULL;//STRIP001 if( pImp )
/*N*/ }


/*N*/ void SwTextBlocks::ClearDoc()
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP"); //STRIP001 if( pImp )
/*N*/ }


/*N*/ const String& SwTextBlocks::GetFileName() const
/*N*/ {
/*?*/ 	 return pImp->GetFileName();
/*N*/ }





/*N*/ BOOL SwTextBlocks::GetMacroTable( USHORT nIdx, SvxMacroTableDtor& rMacroTbl )
/*N*/ {
/*?*/ 	DBG_BF_ASSERT(0, "STRIP");return FALSE; //STRIP001 return ( pImp && !pImp->bInPutMuchBlocks )
/*N*/ }

/*N*/ BOOL SwTextBlocks::SetMacroTable( USHORT nIdx,
/*N*/ 								const SvxMacroTableDtor& rMacroTbl )
/*N*/ {
/*?*/ DBG_BF_ASSERT(0, "STRIP"); return FALSE;//STRIP001 	return ( pImp && !pImp->bInPutMuchBlocks )
/*N*/ }
/*N*/ 






}
