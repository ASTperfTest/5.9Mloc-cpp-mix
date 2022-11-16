/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: ipenv.cxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 13:46:49 $
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

#include <stdio.h>

#include <bf_so3/ipenv.hxx>
#include <bf_so3/iface.hxx>

#ifndef _DEBUG_HXX //autogen
#include <tools/debug.hxx>
#endif
#ifndef _SV_WRKWIN_HXX
#include <vcl/wrkwin.hxx>
#endif
#ifndef _SV_SVAPP_HXX
#include <vcl/svapp.hxx>
#endif
#ifndef _SV_ACCEL_HXX
#include <vcl/accel.hxx>
#endif
#include <bf_so3/so2dll.hxx>
#include <bf_so3/ipobj.hxx>
#include "bf_so3/ipclient.hxx"
#include "bf_so3/ipwin.hxx"
#include <ipmenu.hxx>

class INetURLObject;

//=========================================================================
//==========SvContainerEnvironment=========================================
//=========================================================================
/************************************************************************
|*	  SvPrint( ... )
|*
|*	  Beschreibung
*************************************************************************/
#ifdef DBG_UTIL
ByteString SvPrint( const SvBorder & rThis )
{
	ByteString aStr( "LTRB( " );
	aStr += ByteString::CreateFromInt32(rThis.Left());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rThis.Top());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rThis.Right());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rThis.Bottom());
	aStr += " )";
	return aStr;
}
ByteString SvPrint( const Point & rPos )
{
	ByteString aStr( "( " );
	aStr += ByteString::CreateFromInt32(rPos.X());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rPos.Y());
	aStr += " )";
	return aStr;
}
ByteString SvPrint( const Size & rSize )
{
	ByteString aStr( "( " );
	aStr += ByteString::CreateFromInt32(rSize.Width());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rSize.Height());
	aStr += " )";
	return aStr;
}

ByteString SvPrint( const Rectangle & rRect )
{
	ByteString aStr( "LTRB( " );
	aStr += ByteString::CreateFromInt32(rRect.Left());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rRect.Top());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rRect.Right());
	aStr += ", ";
	aStr += ByteString::CreateFromInt32(rRect.Bottom());
	aStr += " )";
	return aStr;
}
#endif

//=========================================================================
//=========================================================================
//=========================================================================
SO2_IMPL_INTERFACE1(SvAppFrame,SvObject)

IUnknown * SvAppFrame::GetMemberInterface( const SvGlobalName & )
{
	return NULL;
}

//=========================================================================
SvAppFrame::SvAppFrame( SvContainerEnvironment * pEnvP )
	: pObjI( NULL )
	, pEnv( pEnvP )
/*	[Beschreibung]

	Diese Klasse ist nur eine Hilfe, damit der InPlaceClient bei
	QueryInterface nicht den IOleInPlaceFrame zur"uckliefert.

	[Querverweise]

	<SvInPlaceClient>
*/
{
}

//=========================================================================
SvAppFrame::~SvAppFrame()
/*	[Beschreibung]

	Destruktor der Klasse SvAppFrame.

	[Querverweise]

	<SvInPlaceClient>
*/
{
}

//=========================================================================
//=========================================================================
//=========================================================================
SO2_IMPL_INTERFACE1(SvDocFrame,SvObject)

IUnknown * SvDocFrame::GetMemberInterface( const SvGlobalName & )
{
	return NULL;
}

//=========================================================================
SvDocFrame::SvDocFrame
(
	SvContainerEnvironment * pEnvP
)
	: pObjI( NULL )
	, pEnv( pEnvP )
/*	[Beschreibung]

	Diese Klasse ist nur eine Hilfe, damit der InPlaceClient bei
	QueryInterface nicht den IOleInPlaceFrame zur"uckliefert.

	[Querverweise]

	<SvInPlaceClient>
*/
{
}

//=========================================================================
SvDocFrame::~SvDocFrame()
/*	[Beschreibung]

	Destruktor der Klasse SvDocFrame.

	[Querverweise]

	<SvInPlaceClient>
*/
{
}

//=========================================================================
//==========SvContainerEnvironment=========================================
//=========================================================================
static void InsertInContList( SvContainerEnvironment * p )
{
	SoDll * pSoApp = SOAPP;
	if( !pSoApp->pContEnvList )
		pSoApp->pContEnvList = new SvContainerEnvironmentList( 2, 2 );
	pSoApp->pContEnvList->Insert( p, LIST_APPEND );
}

#define INIT_CTOR	 				\
	  pOleInfo		( NULL )		\
	, hOleMenuDesc	( 0 ) 			\
	, nCount1 		( 0 ) 			\
	, nCount2 		( 0 ) 			\
	, nCount3 		( 0 ) 			\
	, nMenuUseCount	( 0 ) 			\
	, pOleMenu		( NULL ) 		\
	, pAccel		( NULL )		\
    , aClipAreaPixel( 0, 0, 0x7FFF, 0x7FFF )	\
	, bDfltUIAction ( TRUE )		\
	, bDeleteTopWin ( FALSE )		\
	, bDeleteDocWin ( FALSE )		\
	, bDeleteEditWin ( FALSE )

TYPEINIT1(SvContainerEnvironment,SvClientData);

//=========================================================================
SvContainerEnvironment::SvContainerEnvironment
(
	SvInPlaceClient * pCl,
	WorkWindow * pTopWinP,
	WorkWindow * pDocWinP,
	Window * pEditWin_
)
	: SvClientData( pCl, pEditWin_ )
	, pIPEnv( NULL )
	, pObj( pCl )
	, pParent( NULL )
	, pChildList( NULL )
	, pTopWin( pTopWinP )
	, pDocWin( pDocWinP )
	, INIT_CTOR
/*	[Beschreibung]

	[Querverweise]
*/
{
	InsertInContList( this );
}

//=========================================================================
SvContainerEnvironment::SvContainerEnvironment
(
	SvInPlaceClient * pCl,
	SvContainerEnvironment * pPar
)
	: SvClientData( pCl, pPar->pIPEnv->GetEditWin() )
	, pIPEnv( NULL )
	, pObj( pCl )
	, pParent( pPar )
	, pChildList( NULL )
	, pTopWin( pPar->GetTopWin() )
	, pDocWin( pPar->GetDocWin() )
	, INIT_CTOR
/*	[Beschreibung]

	[Querverweise]
*/
{
	if( !pParent->pChildList )
		pParent->pChildList = new SvContainerEnvironmentList( 2, 2 );
	pParent->pChildList->Insert( this, LIST_APPEND );

	InsertInContList( this );
}

//=========================================================================
SvContainerEnvironment::~SvContainerEnvironment()
/*	[Beschreibung]

	Im Destruktor werden alle selbsterzeugten Fenster zerst"ort.

	[Querverweise]
*/
{
	DBG_ASSERT( !pIPEnv, "IPEnv exist" )
	ResetChilds();

	if( bDeleteEditWin )
	{
		Window * pWin = SvClientData::GetEditWin();
		SetEditWin( NULL );
		delete pWin;
	}
	if( bDeleteDocWin )
		delete pDocWin;
	if( bDeleteTopWin )
		delete pTopWin;

	SoDll * pSoApp = SOAPP;
	pSoApp->pContEnvList->Remove( this );
	if( pParent )
		pParent->pChildList->Remove( this );
	delete pAccel;

	DBG_ASSERT( !xAppFrame.Is() || 1 == xAppFrame->GetRefCount(),
				"can't destroy xAppFrame" )
	DBG_ASSERT( !xDocFrame.Is() || 1 == xDocFrame->GetRefCount(),
				"can't destroy xDocFrame" )
}

//=========================================================================

//=========================================================================
SvContainerEnvironment * SvContainerEnvironment::GetChild
(
	ULONG nChildPos	/* Position des Childs in der Liste */
) const
/*	[Beschreibung]

	Die Methode liefert das Child-Env an von der angebenen Position
	zur"ck.

	[R"uckgabewert]

	SvContainerEnvironment * ,	wenn die Anzahl der Child's > nChildPos
								ist, wird das Child-Env zur"uckgegeben.
								Ansonsten wird NULL zur"uckgegeben.

	[Querverweise]

	<SvContainerEnvironment::()>
*/
{
	return pChildList ? pChildList->GetObject( nChildPos ) : NULL;
}

//=========================================================================
void SvContainerEnvironment::ResetChilds()
/*	[Beschreibung]

	Die Verbindung aller Client's, die in den Child-Environments
	stehen, werden abgebrochen.
*/
{
	if( pChildList )
	{
		// original Liste wird durch Disconnect ver"andert
		SvContainerEnvironmentList aList( *pChildList );
		SvContainerEnvironment * pChild = aList.First();
		while( pChild )
		{
			if( pChild->pObj )
				// Child disconnecten
				pChild->pObj->GetProtocol().Reset();
			pChild = aList.Next();
		}
	}
}

//=========================================================================
void SvContainerEnvironment::ResetChilds2IPActive()
/*	[Beschreibung]

	Die Methode setzt alle Childs auf IPActive zur"uck. Dabei
	arbeitet die Methode rekusiv. Das heisst Child's von Child's werden
	auch zur"uckgesetzt.

	[R"uckgabewert]

	SvContainerEnvironment * ,	wenn die Anzahl der Child's > nChildPos
								ist, wird das Child-Env zur"uckgegeben.
								Ansonsten wird NULL zur"uckgegeben.
*/
{
	// kein Child darf UI-Aktiv sein
	ULONG n = 0;
	SvContainerEnvironment * pChild;
	while( NULL != (pChild = GetChild( n++ ) ) )
	{
		if( pChild->GetIPClient() )
			pChild->GetIPClient()->GetProtocol().Reset2InPlaceActive();
		pChild->ResetChilds2IPActive();
	}
}

//=========================================================================
BOOL SvContainerEnvironment::IsChild
(
	SvContainerEnvironment * pEnv	/* Das Env, von dem festgestellt werden
									   soll, ob es ein Child ist */
) const
/*	[Beschreibung]

	Stellt fest, ob es sich um ein Child handelt.

	[R"uckgabewert]

	BOOL		TRUE, es ist ein Child.
				FALSE, es ist kein Child.
*/
{
	ULONG n = 0;
	SvContainerEnvironment * pChild;
	while( NULL != (pChild = GetChild( n++ ) ) )
	{
		if( pChild == pEnv || pChild->IsChild( pEnv ) )
			return TRUE;
	}
	return FALSE;
}

/************************************************************************
|*	  SvContainerEnvironment::IsStub()
|*
|*	  Beschreibung:
*************************************************************************/
BOOL SvContainerEnvironment::IsStub() const
{
	if( pParent )
		return pParent->IsStub();
	return pObj != NULL && !pObj->Owner();
}

/************************************************************************
|*	  SvContainerEnvironment::ShowUIByChildDeactivate()
|*
|*	  Beschreibung:
*************************************************************************/
void SvContainerEnvironment::ShowUIByChildDeactivate()
{
	if( pObj && pObj->GetProtocol().IsInPlaceActive() )
	{   // gibt es einen IP-Client und ist dieser IP-Active
		// dann UI aktivieren
		pObj->GetIPObj()->DoUIActivate( TRUE );
	}
	else if( pParent )
		pParent->ShowUIByChildDeactivate();
}

#ifdef Brauch_ich_nicht_mehr
//=========================================================================
void SvContainerEnvironment::DeleteWindows_Impl()
/*	[Beschreibung]

	Die Fenster pEditWin, pDocWin und pTopWin werden in dieser
	Reihenfolge gel"oscht.
*/
{
	if( bDeleteEditWin )
	{
		// erst austragen, dann l"oschen
		Window * pWin = SvClientData::GetEditWin();
		SetEditWin( NULL );
		delete pWin;
	}
	if( bDeleteDocWin )
		delete pDocWin;
	if( bDeleteTopWin )
		delete pTopWin;
	bDeleteEditWin = bDeleteDocWin = bDeleteTopWin = FALSE;
}
#endif

//=========================================================================
WorkWindow * SvContainerEnvironment::GetTopWin() const
{
	return pTopWin;
}

//=========================================================================
WorkWindow * SvContainerEnvironment::GetDocWin() const
{
	return pDocWin;
}

//=========================================================================
Window * SvContainerEnvironment::GetEditWin() const
{
	return SvClientData::GetEditWin();
}

/************************************************************************
|*	  SvContainerEnvironment::SetMenu()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::DeleteMenu()
{
	delete pOleMenu;
	pOleMenu = NULL;
}

/************************************************************************
|*	  SvContainerEnvironment::SetMenu()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetMenu( MenuBar * pOleMenuP, USHORT nCount1P,
								USHORT nCount2P, USHORT nCount3P )
{
	DBG_ASSERT( !nMenuUseCount, "SvContainerEnvironment::SetMenu - protocol error: menu selected" )
	pOleMenu = pOleMenuP;
	nCount1  = nCount1P;
	nCount2  = nCount2P;
	nCount3  = nCount3P;
}

/************************************************************************
|*	  SvContainerEnvironment::DoQueryMenu()
|*
|*	  Beschreibung
*************************************************************************/
MenuBar * SvContainerEnvironment::DoQueryMenu( USHORT * p0, USHORT * p1, USHORT * p2 )
{
	nMenuUseCount++;
	return QueryMenu( p0, p1, p2 );
}

/************************************************************************
|*	  SvContainerEnvironment::QueryMenu()
|*
|*	  Beschreibung
*************************************************************************/
MenuBar * SvContainerEnvironment::QueryMenu
(
	USHORT * p0,
	USHORT * p1,
	USHORT * p2
)
{
	*p0 = *p1 = *p2 = 0;
	if( pObj && !pObj->Owner() )
	{
		if( pOleMenu )
		{
			delete pOleMenu;
			pOleMenu = NULL;
			nCount1 = nCount2 = nCount3 = 0;
		}
	}

	if( pOleMenu )
	{
		*p0 = nCount1;
		*p1 = nCount2;
		*p2 = nCount3;
	}
	else if( pParent )
		return pParent->QueryMenu( p0, p1, p2 );
	return pOleMenu;
}

/************************************************************************
|*	  SvContainerEnvironment::SetInPlaceMenu()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetInPlaceMenu( MenuBar * pIPMenu, BOOL bSet )
{
	if( !bDfltUIAction )
		return;

	if( pParent )
	{
		pParent->SetInPlaceMenu( pIPMenu, bSet );
		return;
	}

	if( !pObj || pObj->Owner() )
	{
		WorkWindow * pW = GetDocWin();
		if( !pW )
			pW = GetTopWin();
		if( bSet )
		{
			if( pW )
				pW->SetMenuBar( pIPMenu );
		}
		else
		{
			if( pW && (pIPMenu == pW->GetMenuBar()) )
				pW->SetMenuBar( NULL );
		}
	}
}

/************************************************************************
|*	  SvContainerEnvironment::DoMenuReleased()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::DoMenuReleased( MenuBar * pOldAppMenu )
{
	if( nMenuUseCount )
	{
		nMenuUseCount--;
		if( pOldAppMenu )
			SetInPlaceMenu( pOldAppMenu, FALSE );
		MenuReleased();
	}
}

/************************************************************************
|*	  SvContainerEnvironment::MenuReleased()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::MenuReleased()
{
}

/************************************************************************
|*	  SvContainerEnvironment::UIToolsShown()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::UIToolsShown( BOOL bShow )
{
	(void)bShow;
	//if( !IsStub() && !bShow )
	//	SvSO::SetAppBorder( 0, 0, 0, 0 );
}

/*************************************************************************
|*	  SvContainerEnvironment::SetAccel()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetAccel( const Accelerator & rAccel )
{
	if( pAccel )
		*pAccel = rAccel;
	else
		pAccel = new Accelerator( rAccel );
}

/*************************************************************************
|*	  SvContainerEnvironment::GetAccel()
|*
|*	  Beschreibung
*************************************************************************/
Accelerator * SvContainerEnvironment::GetAccel()
{
	DBG_ASSERT( !pObj || pObj->Owner(),
				 "cannot convert HACCEL to Accelerator (not implemented)" )
	if( !pAccel && pParent )
		return pParent->GetAccel();
	return pAccel;
}

/*************************************************************************
|*	  SvContainerEnvironment::DispatchAccel()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::DispatchAccel( const KeyCode & rCode )
{
	if( pParent )
		return pParent->DispatchAccel( rCode );

	return FALSE;
}

/*************************************************************************
|*    SvContainerEnvironment::OutDevScaleChanged()
|*
|*    Beschreibung
*************************************************************************/
void SvContainerEnvironment::OutDevScaleChanged()
{
	if( pIPEnv )
		pIPEnv->DoRectsChanged();
}

/*************************************************************************
|*    SvClientData::SetSizeScale()
|*
|*    Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetSizeScale( const Fraction & rScaleWidth,
								const Fraction & rScaleHeight )
{
	if(	GetScaleWidth() != rScaleWidth || GetScaleHeight() != rScaleHeight )
	{
		SvClientData::SetSizeScale( rScaleWidth, rScaleHeight );
		OutDevScaleChanged();
	}
}

/*************************************************************************
|*    SvContainerEnvironment::SetClipAreaPixel()
|*
|*    Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetClipAreaPixel( const Rectangle & rRect )
{
	aClipAreaPixel = rRect;
	if( pIPEnv )
		pIPEnv->DoRectsChanged();
}

/*************************************************************************
|*    SvInPLaceClient::SetObjArea()
|*
|*    Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::SetObjArea( const Rectangle & rArea )
{
	if( SvClientData::SetObjArea( rArea ) )
	{
		if( pIPEnv )
			pIPEnv->DoRectsChanged();
		return TRUE;
	}
	return FALSE;
}

/*************************************************************************
|*    SvContainerEnvironment::GetObjArea()
|*
|*    Beschreibung
*************************************************************************/
Rectangle SvContainerEnvironment::GetObjArea() const
{
	return SvClientData::GetObjArea();
}

/*************************************************************************
|*	  SvContainerEnvironment::PixelObjVisAreaToLogic()
|*
|*	  Beschreibung: Der Return ist in logischen Koordinaten des Objektes
*************************************************************************/
Rectangle SvContainerEnvironment::PixelObjVisAreaToLogic(
									const Rectangle & rObjRect ) const
{
	DBG_ASSERT( pIPEnv, "no InPlaceEnvironment" )

	SvInPlaceObject * pIPObj = pIPEnv->GetIPObj();

	Window * pWin;
	if( !pObj || pObj->Owner() )
		pWin = GetEditWin();
	else
		pWin = pIPEnv->GetEditWin();

	MapMode aClientMap( pWin->GetMapMode().GetMapUnit() );
	MapMode aObjMap( pIPObj->GetMapUnit() );

	// VisArea entsprechend der Groessenaenderung anpassen
	Rectangle aRect = pIPObj->GetVisArea();

	// Wenn die linke obere Ecke verschoben wird, auch die linke
	// obere Ecke der VisArea verschieben. Dies aber nur, wenn gleichzeitig
    // die Groesse geaendert wird.
	Rectangle aOldArea = GetObjAreaPixel();
	long nWidth = aOldArea.GetWidth();
	if( nWidth && nWidth != rObjRect.GetWidth() )
	{
		// Differenz berechnen
		long n = rObjRect.Left() - aOldArea.Left();
		// Breite in Pixel des Orginal
		// 3 Satz, Linke Seite entsprechend Vergroessern
		aRect.Left() += n * aRect.GetWidth() / nWidth;
	}
	long nHeight = aOldArea.GetHeight();
	if( nHeight && nHeight != rObjRect.GetHeight() )
	{
		long n = rObjRect.Top() - aOldArea.Top();
		aRect.Top() += n * aRect.GetHeight() / nHeight;
	}

	Size aSize;
	// Scale und OutDev richtig
	aSize = pWin->PixelToLogic( rObjRect.GetSize() );
	// Skalierung bei der Visarea nicht beruecksichtigen
	aSize = pWin->LogicToLogic( aSize, &aClientMap, &aObjMap );
	aSize.Width()  = Fraction( aSize.Width() )	/ GetScaleWidth();
	aSize.Height() = Fraction( aSize.Height() ) / GetScaleHeight();

	aRect.SetSize( aSize );
	return aRect;
}

/*************************************************************************
|*	  SvContainerEnvironment::RequestObjAreaPixel()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::RequestObjAreaPixel( const Rectangle & rObjRect )
{
	// IP-Win wurde veraendert, Client anpassen
	// Position im Client setzen
	if( !pIPEnv )
	{
		SetObjAreaPixel( rObjRect );
		return;
	}

	Rectangle aOldAreaPixel = GetObjAreaPixel();
	if( rObjRect == aOldAreaPixel )
		return;

	pIPEnv->LockRectsChanged();

	Rectangle aOldArea = GetObjArea();
	Rectangle aArea = PixelObjAreaToLogic( rObjRect );
	// Dies muss vor dem setzen der neuen ObjArea erfolgen, da die
	// alte ObjArea benoetigt wird
	// Ist in Objekt Koordinaten
	Rectangle aObjVisArea = PixelObjVisAreaToLogic( rObjRect );
	SvInPlaceObjectRef xIPObj = pIPEnv->GetIPObj();
	Rectangle aOldObjVisArea = xIPObj->GetVisArea();
	if( rObjRect.GetSize() == aOldAreaPixel.GetSize() )
	{
        // Die Grosse aendert sich nur durch Pixel-Rundung,
		// deshalb die alte logische Groesse nehmen
		aObjVisArea.SetSize( aOldObjVisArea.GetSize() );
		aArea.SetSize( aOldArea.GetSize() );
	}

	if( rObjRect.TopLeft() == aOldAreaPixel.TopLeft() )
	{
        // Die Position aendert sich nur durch Pixel-Rundung,
		// deshalb die alte logische Position nehmen
		aObjVisArea.SetPos( aOldObjVisArea.TopLeft() );
		aArea.SetPos( aOldArea.TopLeft() );
	}


	// Dadurch wird eine zu fruehe Ausgabe unterbunden
	BOOL bInval = IsInvalidate();
	if( xIPObj->GetProtocol().IsInPlaceActive() )
		SetInvalidate( FALSE );

	SetObjArea( aArea );
	SetInvalidate( bInval );

	xIPObj->SetVisArea( aObjVisArea );

	// innerhalb der Verhandlung nicht neu updaten
	pIPEnv->UnlockRectsChanged();
	pIPEnv->DoRectsChanged();
}

/*************************************************************************
|*	  SvContainerEnvironment::Scroll()
|*
|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::Scroll( const Size & rSize )
{
	if( !rSize.Width() && !rSize.Height() )
		// doch nicht gescrollt
		return;

	if( !pObj || pObj->Owner() )
	{
		// Da in Scroll der Origin versetzt wird,
		// aendern sich die Pixel Koordinaten
		pIPEnv->DoRectsChanged();
	}
}

/************************************************************************
|*	  SvContainerEnvironment::SetTopToolFramePixel()

|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetTopToolFramePixel( const SvBorder & rBorder )
{
	if( rBorder != aTopBorder )
	{
		aTopBorder = rBorder;
		if( pIPEnv )
			// InPlace-Objekt fragt nach rOuter, deshalb vorher setzen
			pIPEnv->DoTopWinResize();

		// Alle Childs benachrichtigen
		ULONG n = 0;
		SvContainerEnvironment * pChild;
		while( NULL != (pChild = GetChild( n++ ) ) )
			pChild->SetTopToolFramePixel( aTopBorder );
	}
}

/************************************************************************
|*	  SvContainerEnvironment::SetDocToolFramePixel()

|*	  Beschreibung
*************************************************************************/
void SvContainerEnvironment::SetDocToolFramePixel( const SvBorder & rBorder )
{
	if( rBorder != aDocBorder )
	{
		aDocBorder = rBorder;
		if( pIPEnv )
			// InPlace-Objekt fragt nach rOuter, deshalb vorher setzen
			pIPEnv->DoDocWinResize();

		// Alle Childs benachrichtigen
		ULONG n = 0;
		SvContainerEnvironment * pChild;
		while( NULL != (pChild = GetChild( n++ ) ) )
			pChild->SetDocToolFramePixel( aDocBorder );
	}
}

/************************************************************************
|*	  SvContainerEnvironment::RequestTopToolSpacePixel()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::RequestTopToolSpacePixel( const SvBorder & rInner )
{
	if( pParent )
		return pParent->RequestTopToolSpacePixel( rInner );

	BOOL bRet = FALSE;
	if( !pObj || pObj->Owner() )
	{
		bRet = TRUE;
	}
	return bRet;
}

/************************************************************************
|*	  SvContainerEnvironment::RequestDocToolSpacePixel()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::RequestDocToolSpacePixel( const SvBorder & rInner )
{
	if( pParent )
		return pParent->RequestDocToolSpacePixel( rInner );

	BOOL bRet = FALSE;
	if( !pObj || pObj->Owner() )
	{
		bRet = TRUE;
	}
	return bRet;
}

/************************************************************************
|*	  SvContainerEnvironment::SetTopToolSpacePixel()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::SetTopToolSpacePixel( const SvBorder & rInner )
{
	if( pParent )
		return pParent->SetTopToolSpacePixel( rInner );

	if( !pObj || pObj->Owner() )
	{
		if( RequestTopToolSpacePixel( rInner ) )
		{
			// Sind Top- und EditWin unterschiedlich, dann muss das EditWin
			// neu angeordnet werden. Dies muss von dem abgeleiteten
			// Environment geleistet werden.
			// Sind sie gleich, kann die Anordnung automatisch geschehen
			return GetEditWin() == GetTopWin();
		}
		return FALSE;
	}
	return FALSE;
}

/************************************************************************
|*	  SvContainerEnvironment::SetDocToolSpacePixel()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::SetDocToolSpacePixel( const SvBorder & rInner )
{
	if( pParent )
		return pParent->SetDocToolSpacePixel( rInner );

	if( !pObj || pObj->Owner() )
	{
		if( RequestDocToolSpacePixel( rInner ) )
		{
			// Sind Doc- und EditWin unterschiedlich, dann muss das EditWin
			// neu angeordnet werden. Dies muss von dem abgeleiteten
			// Environment geleistet werden.
			// Sind sie gleich, kann die Anordnung automatisch geschehen
			return GetEditWin() == GetDocWin();
		}
	}
	return FALSE;
}

/************************************************************************
|*	  SvContainerEnvironment::GetTopOuterRectPixel()
|*
|*	  Beschreibung
*************************************************************************/
Rectangle SvContainerEnvironment::GetTopOuterRectPixel() const
{
	if( pParent )
		return pParent->GetTopOuterRectPixel();

	Rectangle aOuter;
	if( !pObj || pObj->Owner() )
	{
		aOuter = Rectangle( Point(), pTopWin->GetOutputSizePixel() );
		aOuter -= aTopBorder;
	}

	return aOuter;
}

/************************************************************************
|*	  SvContainerEnvironment::GetDocOuterRectPixel()
|*
|*	  Beschreibung
*************************************************************************/
Rectangle SvContainerEnvironment::GetDocOuterRectPixel() const
{
	if( pParent )
		return pParent->GetDocOuterRectPixel();

	Rectangle aOuter;
	if( !pObj || pObj->Owner() )
	{
		aOuter = Rectangle( Point(), pDocWin->GetOutputSizePixel() );
		aOuter -= aDocBorder;
	}

	return aOuter;
}


/************************************************************************
|*	  SvContainerEnvironment::SetStatusText()
|*
|*	  Beschreibung
*************************************************************************/
BOOL SvContainerEnvironment::SetStatusText( const String & rText )
{
	if( pParent )
		return pParent->SetStatusText( rText );

	return FALSE;
}

//=========================================================================
void SvContainerEnvironment::ShowDocument
(
	const INetURLObject &,		/* Die URL, die angezeigt werden soll. */
	const XubString &			/* Spezifiert den Frame, in dem das Dokument
								   angezeigt werden soll.
								   "_self",	show in current frame.
								   "_parent", show in parent frame.
								   "_top", show in top-most frame.
								   "_blank", show in new unnamed top-level
								   window.
								   name, show in a new top-level named name.
								 */
)
/*	[Beschreibung]

	Show a new document in a target window or frame. This may be ignored
	by  the applet context.

	[Querverweise]

	<SjAppletContext::ShowDocument>
*/
{
}

/************** SvInPlaceEnvironment ***********************************************
**************************************************************************/
SvInPlaceEnvironment::SvInPlaceEnvironment( SvContainerEnvironment * pCl,
								SvInPlaceObject * pObjP )
	: nChangeRectsLockCount( 0 )
	, bShowUITools( FALSE )
	, bTopWinResize( FALSE )
	, bDocWinResize( FALSE )
	, bDeleteEditWin( FALSE )
	, pUIMenu( NULL )
	, pClientMenu( NULL )
	, pObjMenu( NULL )
	, pClipWin( NULL )
	, pBorderWin( NULL )
	, pEditWin( NULL )
	, pContEnv( pCl )
	, pObj( pObjP )
{
	pContEnv->SetIPEnv( this );
}

/*************************************************************************/
SvInPlaceEnvironment::~SvInPlaceEnvironment()
{
	DBG_ASSERT( !bShowUITools, "UI-Tools werden noch angezeigt" )
	DoShowUITools( FALSE );
	if( bDeleteEditWin )
		delete pEditWin;
	pContEnv->ResetIPEnv();
}

//=========================================================================
void SvInPlaceEnvironment::DeleteObjMenu()
/*	[Beschreibung]

	Das Menu welches mit <SvInPLaceEnvironment::SetObjMenu> eingesetzt
	wurde, wird gel"oscht. Die Methode darf auch gerufen werden, wenn
	vorher kein Menu eingesetzt wurde.
*/
{
	delete pObjMenu;
	pObjMenu = NULL;
}

//=========================================================================
void SvInPlaceEnvironment::DeleteClipWin()
{
	delete pClipWin;
	pClipWin = NULL;
}

//=========================================================================
void SvInPlaceEnvironment::DeleteBorderWin()
{
	delete pBorderWin;
	pBorderWin = NULL;
}

//=========================================================================
Window * SvInPlaceEnvironment::GetEditWin()
{
	return pEditWin;
}

//=========================================================================
void SvInPlaceEnvironment::DeleteEditWin()
{
	delete pEditWin;
	pEditWin = NULL;
	bDeleteEditWin = FALSE;
}

//=========================================================================
MenuBar * SvInPlaceEnvironment::QueryMenu
(
	USHORT * pCount0,
	USHORT * pCount1,
	USHORT * pCount2
)
{
	if( pObjMenu )
		return pObjMenu->GetBlocks( pCount0, pCount1, pCount2 );
	return NULL;
}

/*************************************************************************/
void SvInPlaceEnvironment::MergeMenus()
{
	if( !pUIMenu )
	{ // kein Menu mehr da

		USHORT    nC[ 3 ], nO[ 3 ];

		pClientMenu = pContEnv->QueryMenu( &nC[0], &nC[1], &nC[2] );
		// Es muss ein Objekt-Menu zurueckgegeben werden
		pUIMenu    = QueryMenu( &nO[0], &nO[1], &nO[2] );
		if( pClientMenu && pUIMenu )
		{
			USHORT nOff = 0;
			USHORT  nCOff = 0;

			for( USHORT i = 0; i < 3; i++ )
			{
				for( USHORT n = 0; n < nC[ i ]; n++ )
				{
					USHORT nId = pClientMenu->GetItemId( nCOff++ );
					PopupMenu * pPopup = pClientMenu->GetPopupMenu( nId );
					// Gegen falsche Items oder Counts absichern
					if( pPopup )
					{
						pUIMenu->InsertItem( nId,
							  pClientMenu->GetItemText( nId ), 0, nOff++ );
						pUIMenu->SetPopupMenu( nId, pPopup );
					}
					else
						break; // Menu oder Counts sind falsch
				}
				nOff = nOff + nO[ i ]; // um Objekt-Menublock weiterzaehlen
			}
		}
	}
}

/*************************************************************************/
void SvInPlaceEnvironment::ReleaseClientMenu()
{
	if( pClientMenu && pUIMenu )
	{ // Client-Items aus OleMenu entfernen
		for( USHORT i = 0; i < pClientMenu->GetItemCount(); i++ )
		{
			USHORT nId =  pClientMenu->GetItemId( i );
			USHORT nPos = pUIMenu->GetItemPos( nId  );
			pUIMenu->RemoveItem( nPos );
		}
	}
	if( pClientMenu )
	{ // Client benachrichtigen, dass das Menu freigegeben werden kann
		pContEnv->DoMenuReleased( pUIMenu );
		pClientMenu = NULL;
	}
	pUIMenu = NULL;
}

/*************************************************************************/
void SvInPlaceEnvironment::DoShowIPObj( BOOL bShow )
{
	if( bShow )
		DoRectsChanged();
	ShowIPObj( bShow );
}

/*************************************************************************/
void SvInPlaceEnvironment::ShowIPObj( BOOL bShow )
{
	if( pClipWin )
	{
		if( bShow )
			GetClipWin()->Show();
		else
			GetClipWin()->Hide();
	}
}

/*************************************************************************/
void SvInPlaceEnvironment::DoShowUITools( BOOL bShow )
{

	//DBG_ASSERT( !bShow || !bShowUITools, "bShowUITools && bShow" )
	if( bShow == bShowUITools )
		// bei soviel Activate/Deactivate ist es besser dies zu gestatten
		return;

	if( bShow && !GetContainerEnv()->IsStub() )
	{
		SoDll * pSoApp = SOAPP;
		// SvInPlaceEnvironment * pUIEnv = pSoApp->pUIShowIPEnv;
		// unbedingt zuerst setzen
		pSoApp->pUIShowIPEnv = this;

        // this is only for MDI apps!
//        if( pUIEnv )
//            pUIEnv->DoShowUITools( FALSE );

		// kein Child darf UI-Aktiv sein
		SvContainerEnvironment * pEnv = GetContainerEnv();
		pEnv->ResetChilds2IPActive();

		// kein Parent darf UI-Aktiv sein
		SvContainerEnvironment * pPar = pEnv->GetParent();
		while( pPar && pPar->GetIPClient() )
		{
			pPar->GetIPClient()->GetProtocol().Reset2InPlaceActive();
			pPar = pPar->GetParent();
		}
/*
		SvContainerEnvironmentList * pList = pSoApp->pContEnvList;
		if( pList )
		{
			for( ULONG i = 0; i < pList->Count(); i++ )
			{
				SvContainerEnvironment * pFrm = pList->GetObject( i );
				if( !pFrm->IsStub() && pFrm->GetIPEnv()
				  && pFrm->GetIPEnv()->IsShowUITools() )
				{
					SvInPlaceClient * pCl = pFrm->GetIPClient();
					if( pCl && pCl->GetProtocol().IsUIActive() )
						// Objekte-UI-Tools ueber UIActivate( FALSE ) wegnehmen
						pCl->GetProtocol().Reset2InPlaceActive();
					else
						pFrm->GetIPEnv()->DoShowUITools( FALSE );
				}
			}
		}
*/
	}
	else if( !bShow && !GetContainerEnv()->IsStub() )
	{
		SoDll * pSoApp = SOAPP;
		if( pSoApp->pUIShowIPEnv == this )
			pSoApp->pUIShowIPEnv = NULL;
	}

	if( bShow )
		// bei Show == TRUE zuerst den Conatiner benachrichtigen
		GetContainerEnv()->UIToolsShown( bShow );
	// Flag muss gesetzt werden, son kommt Resize nicht durch
	bShowUITools = bShow;

	if( GetIPObj()->Owner() && pUIMenu )
		GetContainerEnv()->SetInPlaceMenu( pUIMenu, bShow );
	if( bShow )
	{
		if( GetIPObj()->Owner() )
		{
			if( !bTopWinResize )
				// Anordnen der Tools anstossen
				DoTopWinResize();
			if( !bDocWinResize )
				// Anordnen der Tools anstossen
				DoDocWinResize();
		}
	}
	else
	{
		// Anordnen der Tools anstossen
		bTopWinResize = FALSE;
		bDocWinResize = FALSE;
	}

	ShowUITools( bShow );
	if( !bShow )
		// bei Show == FALSE zuletzt den Conatiner benachrichtigen
		GetContainerEnv()->UIToolsShown( bShow );
}

/*************************************************************************/
void SvInPlaceEnvironment::ShowUITools( BOOL )
{
}

/*************************************************************************/
void SvInPlaceEnvironment::MakeUI( BOOL bMake )
{
	if( bMake )
	{
		if( !GetContainerEnv()->IsStub() )
			// sonst nur bei Ole-SetInPlaceActiveObj
			GetIPObj()->DoMergePalette();
		DoShowIPObj( bMake );
		MergeMenus();
	}
	else
	{
		DoShowIPObj( bMake );
		ReleaseClientMenu();
	}
}

//=========================================================================
void SvInPlaceEnvironment::DoTopWinResize()
/*	[Beschreibung]

	Es wird nur die Methode <SvInPlaceEnvironment::TopWinResize>
	gerufen, wenn die UI-Tools angezeigt werden. Ausserdem wird
	ein Status gespeichert, der Anzeigt, ob vor dem
	<SvInPlaceEnvironment::ShowUITools> ein TopWinResize gerufen werden
	muss.
*/
{
	if( bShowUITools )
	{
		bTopWinResize = TRUE;
		TopWinResize();
	}
	else
		bTopWinResize = FALSE;
}

//=========================================================================
void SvInPlaceEnvironment::TopWinResize()
/*	[Beschreibung]

	Die Gr"osse des TopWin hat sich ge"andert. Die Methode wird vor
	ShowUITools( TRUE ) mindestens einmal gerufen.
	Die Defaultaktion ist:
		GetContainerEnv()->SetTopToolSpacePixel( SvBorder() );

	[!Owner]

	Die Methode wird an den Server weitergeleitet. Dies geschieht nur
	wenn das pActiveObj gesetzt ist.

	[Querverweise]

	<IOleInPlaceActiveObject::ResizeBorder>
*/
{
	bTopWinResize = TRUE;

	if( pObj->Owner() )
		GetContainerEnv()->SetTopToolSpacePixel( SvBorder() );
}

//=========================================================================
void SvInPlaceEnvironment::DoDocWinResize()
{
/*	[Beschreibung]

	Es wird nur die Methode <SvInPlaceEnvironment::DocWinResize>
	gerufen, wenn die UI-Tools angezeigt werden. Ausserdem wird
	ein Status gespeichert, der Anzeigt, ob vor dem
	<SvInPlaceEnvironment::ShowUITools> ein DocWinResize gerufen werden
	muss.
*/

	if( bShowUITools )
	{
		bDocWinResize = TRUE;
		DocWinResize();
	}
	else
		bDocWinResize = FALSE;
}

//=========================================================================
void SvInPlaceEnvironment::DocWinResize()
/*	[Beschreibung]

	Die Gr"osse des DocWin hat sich ge"andert. Die Methode wird vor
	ShowUITools( TRUE ) mindestens einmal gerufen.

	[!Owner]
	Die Methode wird an den Server weitergeleitet. Dies geschieht nur
	wenn das pActiveObj gesetzt ist.

	[Querverweise]

	<IOleInPlaceActiveObject::ResizeBorder>
*/
{
	bDocWinResize = TRUE;
}

/*************************************************************************/
void SvInPlaceEnvironment::RectsChangedPixel( const Rectangle & rObjRect,
										const Rectangle & rClip )
{
	if( pObj->Owner() )
	{
		if( pClipWin )
			pClipWin->SetRectsPixel( rObjRect, rClip );
	}
}

/*************************************************************************/
void SvInPlaceEnvironment::MakeScale( const Size & rVisAreaSize,
								 MapUnit nVisAreaUnit,
								 const Size & rObjSizePixel )
{
	DBG_ASSERT( pEditWin, "no Edit-Win" )
	Size aVS = pEditWin->LogicToPixel( rVisAreaSize, nVisAreaUnit );
	if( aVS.Width() && aVS.Height() )
	{
		Fraction aSX( rObjSizePixel.Width(), aVS.Width() );
		Fraction aSY( rObjSizePixel.Height(), aVS.Height() );
		MapMode aMod = pEditWin->GetMapMode();
		aMod.SetScaleX( aSX );
		aMod.SetScaleY( aSY );
		pEditWin->SetMapMode( aMod );
		pEditWin->Invalidate();

		ULONG n = 0;
		SvContainerEnvironment * pChild;
		while( NULL != (pChild = pContEnv->GetChild( n++ ) ) )
			if( pChild->GetEditWin() == pEditWin )
				// Das Child von ContEnv muss auf dem Edit-Win basieren
				pChild->OutDevScaleChanged();
	}
}

/**************************************************************************/
void SvInPlaceEnvironment::MakeWindows()
{
	// Fenster fuers Clipping erzeugen
	pClipWin = new SvInPlaceClipWindow( pContEnv->GetEditWin() );
	// Fenster fuer den Border Erzeugen
	pBorderWin = new SvInPlaceWindow( pClipWin, this );
	pBorderWin->Show();

	// Fenstergroesse mit Beachtung der Border setzen
	Rectangle aRect( pContEnv->GetObjAreaPixel() );
	pBorderWin->SetInnerPosSizePixel( aRect.TopLeft(), aRect.GetSize() );

	// Fenster zuordnen (fuers Resize)
	pClipWin->SetResizeWin( pBorderWin );
}

/**************************************************************************/
void SvInPlaceEnvironment::DeleteWindows()
{
	// Fenster zuordnen (fuers Resize)
	pClipWin->Hide();
	pClipWin->SetResizeWin( NULL );
	delete pBorderWin;
	pBorderWin = NULL;
	delete pClipWin;
	pClipWin = NULL;
}

/************************************************************************
|*    SvInPlaceEnvironment::QueryRectsChanged()
|*
|*    Beschreibung
*************************************************************************/
void SvInPlaceEnvironment::QueryObjAreaPixel( Rectangle & ) const
{
}

/************************************************************************
|*    SvInPlaceEnvironment::LockRectsChanged();
|*    SvInPlaceEnvironment::UnlockRectsChanged();
|*
|*    Beschreibung
*************************************************************************/
void SvInPlaceEnvironment::LockRectsChanged()
{
	nChangeRectsLockCount++;
}

void SvInPlaceEnvironment::UnlockRectsChanged()
{
	nChangeRectsLockCount--;
}

/************************************************************************
|*  SvInPlaceEnvironment::DoRectsChanged()
|*
|*  Beschreibung:	Die Methode darf den Handler nur rufen, wenn die
|*                  Bereiche Werte groesser als Null haben. Ausserdem muss
|*                  sichergestellt sein, das der Handler fuer dieselben
|*					Werte nicht zweimal gerufen wird.
*************************************************************************/
void SvInPlaceEnvironment::DoRectsChanged( BOOL bIfEqual )
{
	if( nChangeRectsLockCount ) return;

	Rectangle aClipAreaPixel = pContEnv->GetClipAreaPixel();
	if( aClipAreaPixel.GetWidth() > 0 && aClipAreaPixel.GetHeight() > 0 )
	{
		Rectangle aObjRect = pContEnv->GetObjAreaPixel();

		if( aObjRect.GetWidth() > 0 && aObjRect.GetHeight() > 0 )
			if( bIfEqual ||
			  (aOldObjAreaPixel != aObjRect
			   || aClipAreaPixel != aOldClipAreaPixel) )
			{
				aOldObjAreaPixel = aObjRect;
				aOldClipAreaPixel = aClipAreaPixel;

				RectsChangedPixel( aObjRect, aClipAreaPixel );
			}
	}
}

/*************************************************************************
|*    SvInPlaceEnvironment::DispatchAccel()
|*
|*    Beschreibung
*************************************************************************/
BOOL SvInPlaceEnvironment::DispatchAccel( const KeyCode & )
{
	return FALSE;
}
