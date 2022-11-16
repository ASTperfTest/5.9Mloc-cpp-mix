/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: event.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 12:15:41 $
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
#ifndef _SFXEVENT_HXX
#define _SFXEVENT_HXX

#ifndef _STRING_HXX //autogen
#include <tools/string.hxx>
#endif
#ifndef _SFXHINT_HXX //autogen
#include <svtools/hint.hxx>
#endif

#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/beans/PropertyValue.hpp>
class PrintDialog;
class Printer;
namespace binfilter {

class SfxObjectShell;

//-------------------------------------------------------------------

class SfxEventHint : public SfxHint
{
	USHORT				nEventId;
	SfxObjectShell* 	pObjShell;
	String				_aArgs;
	BOOL				_bDummy;
	BOOL                _bAddToHistory;

public:
	TYPEINFO();
						SfxEventHint( USHORT nId,
									  const String& rArgs,
									  SfxObjectShell *pObj = 0  )
						:	nEventId(nId),
							pObjShell(pObj),
							_bAddToHistory(FALSE),
							_aArgs( rArgs )
						{}
						SfxEventHint( USHORT nId, SfxObjectShell *pObj = 0 )
						:	nEventId(nId),
							pObjShell(pObj),
							_bAddToHistory(FALSE)
						{}

	const String& GetArgs() const {	return _aArgs;}

	USHORT				GetEventId() const
						{ return nEventId; }

	SfxObjectShell* 	GetObjShell() const
						{ return pObjShell; }
};

//-------------------------------------------------------------------

class SfxNamedHint : public SfxHint
{
	String				_aEventName;
	SfxObjectShell* 	_pObjShell;
	String				_aArgs;
	BOOL				_bDummy;
	BOOL                _bAddToHistory;

public:
						TYPEINFO();

						SfxNamedHint( const String& rName,
									  const String& rArgs,
									  SfxObjectShell *pObj = 0  )
						:	_aEventName( rName ),
							_pObjShell( pObj),
							_bAddToHistory( FALSE ),
							_aArgs( rArgs )
						{}

						SfxNamedHint( const String& rName,
									  SfxObjectShell *pObj = 0 )
						:	_aEventName( rName ),
							_pObjShell( pObj ),
							_bAddToHistory( FALSE )
						{}

	const String&		GetArgs() const { return _aArgs;}
	const String&		GetName() const { return _aEventName; }
    SfxObjectShell*     GetObjShell() const { return _pObjShell; }
};

class SfxPrintingHint : public SfxHint
{
	PrintDialog*		pDialog;
	Printer*			pPrinter;
	sal_Int32			nWhich;
	::com::sun::star::uno::Sequence < ::com::sun::star::beans::PropertyValue > aOpts;
public:
						TYPEINFO();
						SfxPrintingHint( sal_Int32 nEvent, PrintDialog* pDlg, Printer* pPrt, const ::com::sun::star::uno::Sequence < ::com::sun::star::beans::PropertyValue >& rOpts )
							: pDialog( pDlg )
							, pPrinter( pPrt )
							, nWhich( nEvent )
							, aOpts( rOpts )
						{}

						SfxPrintingHint( sal_Int32 nEvent, PrintDialog* pDlg, Printer* pPrt )
							: pDialog( pDlg )
							, pPrinter( pPrt )
							, nWhich( nEvent )
						{}

	Printer*  			GetPrinter() const { return pPrinter; }
	PrintDialog*		GetPrintDialog() const { return pDialog; }
	sal_Int32			GetWhich() const { return nWhich; }
	const ::com::sun::star::uno::Sequence < ::com::sun::star::beans::PropertyValue >& GetAdditionalOptions() { return aOpts; }
};

}//end of namespace binfilter
#endif