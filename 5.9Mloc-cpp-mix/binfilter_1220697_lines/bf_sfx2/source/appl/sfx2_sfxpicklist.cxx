/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sfx2_sfxpicklist.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: obo $ $Date: 2007/07/17 10:39:04 $
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

#ifndef INCLUDED_SVTOOLS_HISTORYOPTIONS_HXX
#include <svtools/historyoptions.hxx>
#endif

#ifndef INCLUDED_SVTOOLS_USEROPTIONS_HXX
#include <svtools/useroptions.hxx>
#endif

#ifndef _URLOBJ_HXX
#include <tools/urlobj.hxx>
#endif

#ifndef __FRAMEWORK_CLASSES_MENUCONFIGURATION_HXX_
#include <framework/menuconfiguration.hxx>
#endif

#ifndef _INETHIST_HXX
#include <svtools/inethist.hxx>
#endif

#ifndef _SFXSTRITEM_HXX
#include <svtools/stritem.hxx>
#endif

#ifndef _SFXENUMITEM_HXX
#include <svtools/eitem.hxx>
#endif

#ifndef _OSL_FILE_HXX_
#include <osl/file.hxx>
#endif

#ifndef _UNOTOOLS_LOCALFILEHELPER_HXX
#include <unotools/localfilehelper.hxx>
#endif

#ifndef _CPPUHELPER_IMPLBASE1_HXX_
#include <cppuhelper/implbase1.hxx>
#endif

#include <shell/systemshell.hxx>

// ----------------------------------------------------------------------------

#include "app.hxx"
#include "sfxpicklist.hxx"
#include "sfxuno.hxx"
#include "sfxtypes.hxx"
#include "request.hxx"
#include "sfxsids.hrc"
#include "sfx.hrc"
#include "event.hxx"
#include "objsh.hxx"
#include "docinf.hxx"
#include "referers.hxx"
#include "docfile.hxx"
#include "objshimp.hxx"
#include "docfilt.hxx"

#include <algorithm>
namespace binfilter {

// ----------------------------------------------------------------------------

using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::util;

// ----------------------------------------------------------------------------

osl::Mutex*		SfxPickList::pMutex = 0;
SfxPickList*	SfxPickList::pUniqueInstance = 0;

// ----------------------------------------------------------------------------

class StringLength : public ::cppu::WeakImplHelper1< XStringWidth >
{
	public:
		StringLength() {}
		virtual ~StringLength() {}

		// XStringWidth
		sal_Int32 SAL_CALL queryStringWidth( const ::rtl::OUString& aString )
			throw (::com::sun::star::uno::RuntimeException)
		{
			return aString.getLength();
		}
};

// ----------------------------------------------------------------------------

/*N*/ osl::Mutex*	SfxPickList::GetOrCreateMutex()
/*N*/ {
/*N*/ 	if ( !pMutex )
/*N*/ 	{
/*N*/ 		::osl::MutexGuard aGuard( ::osl::Mutex::getGlobalMutex() );
/*N*/ 		if ( !pMutex )
/*N*/ 			pMutex = new osl::Mutex;
/*N*/ 	}
/*N*/ 
/*N*/ 	return pMutex;
/*N*/ }




/*N*/ SfxPickList*	SfxPickList::GetOrCreate( const sal_uInt32 nMenuSize )
/*N*/ {
/*N*/ 	if ( !pUniqueInstance )
/*N*/ 	{
/*N*/ 		::osl::MutexGuard aGuard( GetOrCreateMutex() );
/*N*/ 		if ( !pUniqueInstance )
/*N*/ 			pUniqueInstance = new SfxPickList( nMenuSize );
/*N*/ 	}
/*N*/ 
/*N*/ 	return pUniqueInstance;
/*N*/ }



/*N*/ SfxPickList::SfxPickList( sal_uInt32 nAllowedMenuSize ) :
/*N*/ 	m_nAllowedMenuSize( nAllowedMenuSize )
/*N*/ {
/*N*/ 	m_xStringLength = new StringLength;
/*N*/ 	m_nAllowedMenuSize = ::std::min( m_nAllowedMenuSize, (sal_uInt32)PICKLIST_MAXSIZE );
/*N*/ 	StartListening( *SFX_APP() );
/*N*/ }







/*N*/ void SfxPickList::Notify( SfxBroadcaster& rBC, const SfxHint& rHint )
/*N*/ {
/*N*/ 	if ( rHint.IsA( TYPE( SfxStringHint )))
/*N*/ 	{
/*N*/ 		SfxStringHint* pStringHint = (SfxStringHint*) &rHint;
/*N*/ 
/*N*/ 		if ( pStringHint->GetId() == SID_OPENURL )
/*N*/ 			INetURLHistory::GetOrCreate()->PutUrl( INetURLObject( pStringHint->GetObject() ));
/*N*/ 	}
/*N*/ 
/*N*/ 	if ( rHint.IsA( TYPE( SfxEventHint )))
/*N*/ 	{
/*N*/ 		SfxEventHint* pEventHint = PTR_CAST(SfxEventHint,&rHint);
/*N*/ 		// nur ObjectShell-bezogene Events mit Medium interessieren
/*N*/ 		SfxObjectShell* pDocSh = pEventHint->GetObjShell();
/*N*/ 		if( !pDocSh )
/*N*/ 			return;
/*N*/ 
/*N*/ 		switch ( pEventHint->GetEventId() )
/*N*/ 		{
/*N*/ 			case SFX_EVENT_CREATEDOC:
/*N*/ 			{
/*N*/ 				pDocSh->GetDocInfo().SetCreated( SvtUserOptions().GetFullName() );
/*N*/ 				break;
/*N*/ 			}
/*N*/ 
/*N*/             case SFX_EVENT_OPENDOC:
/*N*/             {
/*N*/                 SfxMedium *pMed = pDocSh->GetMedium();
/*N*/                 if( !pMed )
/*N*/                     return;
/*N*/ 
/*N*/                 // unbenannt-Docs und embedded-Docs nicht in History
/*N*/                 if ( !pDocSh->HasName() ||
/*N*/                      SFX_CREATE_MODE_STANDARD != pDocSh->GetCreateMode() )
/*N*/                     return;
/*N*/ 
/*N*/                 // Hilfe nicht in History
/*N*/ 				INetURLObject aURL( pMed->GetOrigURL() );
/*N*/                 if ( aURL.GetProtocol() != INET_PROT_VND_SUN_STAR_HELP )
/*N*/                 {
/*N*/                     ::rtl::OUString     aTitle = pDocSh->GetTitle(SFX_TITLE_PICKLIST);
/*N*/                     ::rtl::OUString     aFilter;
/*N*/ 
/*N*/                     const SfxFilter*    pFilter = pMed->GetOrigFilter();
/*N*/                     if ( pFilter )
/*N*/                         aFilter = pFilter->GetFilterName();
/*N*/ 
/*N*/                     // add to svtool history options
/*N*/                     SvtHistoryOptions().AppendItem( eHISTORY,
/*N*/                             aURL.GetURLNoPass( INetURLObject::NO_DECODE ),
/*N*/                             aFilter,
/*N*/                             aTitle,
/*N*/                             SfxStringEncode( aURL.GetPass() ) );
/*N*/                 }
/*N*/                 break;
/*N*/             }
/*N*/ 
/*N*/ 			case SFX_EVENT_CLOSEDOC:
/*N*/ 			{
/*N*/ 				SfxMedium *pMed = pDocSh->GetMedium();
/*N*/ 				if( !pMed )
/*N*/ 					return;
/*N*/ 
/*N*/ 				// unbenannt-Docs und embedded-Docs nicht in Pickliste
/*N*/ 				if ( !pDocSh->HasName() ||
/*N*/ 					 SFX_CREATE_MODE_STANDARD != pDocSh->GetCreateMode() )
/*N*/ 					return;
/*N*/ 
/*N*/ 				if( pDocSh->Get_Impl()->bWaitingForPicklist &&
/*N*/ 					!pDocSh->Get_Impl()->bIsHelpObjSh )
/*N*/ 				{
/*N*/ 					// only add r/w document into picklist
/*N*/ 					SfxMedium *pMed = pDocSh->GetMedium();
/*N*/ 					if( !pMed || pDocSh->IsReadOnly() || !pMed->IsUpdatePickList() )
/*N*/ 						return;
/*N*/ 
/*N*/ 					// add no document that forbids this (for example Message-Body)
/*?*/ 					DBG_BF_ASSERT(0, "STRIP"); //STRIP001 SFX_ITEMSET_ARG( pMed->GetItemSet(), pPicklistItem, SfxBoolItem, SID_PICKLIST, sal_False );
/*?*/ 				}
/*?*/ 				break;
/*N*/ 			}
/*N*/ 		}
/*N*/ 	}
/*N*/ }
}
