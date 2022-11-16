/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: swevent.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/09 16:29:28 $
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

#ifndef _SWEVENT_HXX
#define _SWEVENT_HXX

// #include *****************************************************************

#ifndef _SOLAR_H
#include <tools/solar.h>
#endif

#ifndef _SFX_HRC
#include <bf_sfx2/sfx.hrc>
#endif
namespace binfilter {

#define     SW_EVENT_OBJECT_SELECT        ( EVENT_APP_START + 0 )
#define     SW_EVENT_START_INS_GLOSSARY   ( EVENT_APP_START + 1 )
#define     SW_EVENT_END_INS_GLOSSARY     ( EVENT_APP_START + 2 )
#define     SW_EVENT_MAIL_MERGE           ( EVENT_APP_START + 3 )
#define     SW_EVENT_FRM_KEYINPUT_ALPHA   ( EVENT_APP_START + 4 )
#define     SW_EVENT_FRM_KEYINPUT_NOALPHA ( EVENT_APP_START + 5 )
#define     SW_EVENT_FRM_RESIZE           ( EVENT_APP_START + 6 )
#define     SW_EVENT_FRM_MOVE             ( EVENT_APP_START + 7 )
#define     SW_EVENT_PAGE_COUNT			  ( EVENT_APP_START + 8 )
#define     SW_EVENT_MAIL_MERGE_END		  ( EVENT_APP_START + 9 )


class SwFrmFmt;
class SwFmtINetFmt;
class IMapObject;

// enum fuer Objecte die Events ins Basic oder in JavaScript Callen
enum SwCallEventObjectType
{
	EVENT_OBJECT_NONE = 0,			// Null ist garnichts
	EVENT_OBJECT_IMAGE,
	EVENT_OBJECT_INETATTR,
	EVENT_OBJECT_URLITEM,
	EVENT_OBJECT_IMAGEMAP
};

// structur fuer den Austausch zwischen UI/CORE

struct SwCallMouseEvent
{
	SwCallEventObjectType eType;
	union
	{
		// EVENT_OBJECT_IMAGE/EVENT_OBJECT_URLITEM
		const SwFrmFmt* pFmt;

		// EVENT_OBJECT_INETATTR
		const SwFmtINetFmt* pINetAttr;

		// EVENT_OBJECT_IMAGEMAP
		struct
		{
			const SwFrmFmt* pFmt;
			const IMapObject* pIMapObj;
		} IMAP;
	} PTR;

	SwCallMouseEvent()
		: eType( EVENT_OBJECT_NONE )
		{ PTR.pFmt = 0; PTR.IMAP.pIMapObj = 0; }

	void Set( SwCallEventObjectType eTyp, const SwFrmFmt* pFmt )
		{ eType = eTyp; PTR.pFmt = pFmt; PTR.IMAP.pIMapObj = 0; }

	void Set( const SwFrmFmt* pFmt, const IMapObject* pIMapObj )
		{ eType = EVENT_OBJECT_IMAGEMAP; PTR.pFmt = pFmt; PTR.IMAP.pIMapObj = pIMapObj; }

	void Set( const SwFmtINetFmt* pINetAttr )
		{ eType = EVENT_OBJECT_INETATTR; PTR.pINetAttr = pINetAttr; PTR.IMAP.pIMapObj = 0; }

	int operator==( const SwCallMouseEvent& rEvent ) const
		{
			return eType == rEvent.eType &&
					PTR.pFmt == rEvent.PTR.pFmt &&
					PTR.IMAP.pIMapObj == rEvent.PTR.IMAP.pIMapObj;
		}
	int operator!=( const SwCallMouseEvent& rEvent ) const
		{	return !( *this == rEvent );	}

	void Clear()
		{ eType = EVENT_OBJECT_NONE; PTR.pFmt = 0; PTR.IMAP.pIMapObj = 0; }

	BOOL HasEvent() const { return EVENT_OBJECT_NONE != eType; }
};


} //namespace binfilter
#endif
