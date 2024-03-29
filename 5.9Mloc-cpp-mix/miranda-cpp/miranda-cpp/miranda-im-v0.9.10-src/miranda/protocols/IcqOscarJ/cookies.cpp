// ---------------------------------------------------------------------------80
//                ICQ plugin for Miranda Instant Messenger
//                ________________________________________
// 
// Copyright � 2000-2001 Richard Hughes, Roland Rabien, Tristan Van de Vreede
// Copyright � 2001-2002 Jon Keating, Richard Hughes
// Copyright � 2002-2004 Martin �berg, Sam Kothari, Robert Rainwater
// Copyright � 2004-2010 Joe Kucera
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// -----------------------------------------------------------------------------
//
// File name      : $URL: https://miranda.googlecode.com/svn/branches/stable/miranda/protocols/IcqOscarJ/cookies.cpp $
// Revision       : $Revision: 11974 $
// Last change on : $Date: 2010-06-12 18:57:21 -0400 (Sat, 12 Jun 2010) $
// Last change by : $Author: borkra $
//
// DESCRIPTION:
//
//  Handles packet & message cookies
//
// -----------------------------------------------------------------------------

#include "icqoscar.h"


#define INVALID_COOKIE_INDEX -1

void CIcqProto::RemoveExpiredCookies()
{
	time_t tNow = time(NULL);

	for (int i = cookies.getCount()-1; i >= 0; i--)
  {
    icq_cookie_info *cookie = cookies[i];

		if ((cookie->dwTime + COOKIE_TIMEOUT) < tNow)
    {
			cookies.remove(i);
      SAFE_FREE((void**)&cookie);
    }
  }
}


// Generate and allocate cookie
DWORD CIcqProto::AllocateCookie(BYTE bType, WORD wIdent, HANDLE hContact, void *pvExtra)
{
	icq_lock l(cookieMutex);

	DWORD dwThisSeq = wCookieSeq++;
	dwThisSeq &= 0x7FFF;
	dwThisSeq |= wIdent<<0x10;

	icq_cookie_info* p = (icq_cookie_info*)SAFE_MALLOC(sizeof(icq_cookie_info));
  if (p)
  {
	  p->bType = bType;
	  p->dwCookie = dwThisSeq;
	  p->hContact = hContact;
	  p->pvExtra = pvExtra;
	  p->dwTime = time(NULL);
	  cookies.insert(p);
  }
	return dwThisSeq;
}


DWORD CIcqProto::GenerateCookie(WORD wIdent)
{
  icq_lock l(cookieMutex);

  DWORD dwThisSeq = wCookieSeq++;
	dwThisSeq &= 0x7FFF;
	dwThisSeq |= wIdent<<0x10;

	return dwThisSeq;
}


int CIcqProto::GetCookieType(DWORD dwCookie)
{
	icq_lock l(cookieMutex);

	int i = cookies.getIndex(( icq_cookie_info* )&dwCookie );
	if ( i != INVALID_COOKIE_INDEX )
		i = cookies[i]->bType;

	return i;
}


int CIcqProto::FindCookie(DWORD dwCookie, HANDLE *phContact, void **ppvExtra)
{
	icq_lock l(cookieMutex);

	int i = cookies.getIndex(( icq_cookie_info* )&dwCookie );
	if (i != INVALID_COOKIE_INDEX)
	{
		if (phContact)
			*phContact = cookies[i]->hContact;
		if (ppvExtra)
			*ppvExtra = cookies[i]->pvExtra;

		// Cookie found
		return 1;
	}

	return 0;
}


int CIcqProto::FindCookieByData(void *pvExtra, DWORD *pdwCookie, HANDLE *phContact)
{
	icq_lock l(cookieMutex);

	for (int i = 0; i < cookies.getCount(); i++)
	{
		if (pvExtra == cookies[i]->pvExtra)
		{
			if (phContact)
				*phContact = cookies[i]->hContact;
			if (pdwCookie)
				*pdwCookie = cookies[i]->dwCookie;

			// Cookie found
			return 1;
		}
	}

	return 0;
}


int CIcqProto::FindCookieByType(BYTE bType, DWORD *pdwCookie, HANDLE *phContact, void** ppvExtra)
{
  icq_lock l(cookieMutex);

  for (int i = 0; i < cookies.getCount(); i++)
  {
    if (bType == cookies[i]->bType)
    {
      if (pdwCookie)
        *pdwCookie = cookies[i]->dwCookie;
      if (phContact)
        *phContact = cookies[i]->hContact;
      if (ppvExtra)
        *ppvExtra = cookies[i]->pvExtra;

      // Cookie found
      return 1;
    }
  }

  return 0;
}


int CIcqProto::FindMessageCookie(DWORD dwMsgID1, DWORD dwMsgID2, DWORD *pdwCookie, HANDLE *phContact, cookie_message_data **ppvExtra)
{
	icq_lock l(cookieMutex);

	for (int i = 0; i < cookies.getCount(); i++)
	{
		if (cookies[i]->bType == CKT_MESSAGE || cookies[i]->bType == CKT_FILE || cookies[i]->bType == CKT_REVERSEDIRECT)
		{ // message cookie found
			cookie_message_data *pCookie = (cookie_message_data*)cookies[i]->pvExtra;

			if (pCookie->dwMsgID1 == dwMsgID1 && pCookie->dwMsgID2 == dwMsgID2)
			{
				if (phContact)
					*phContact = cookies[i]->hContact;
				if (pdwCookie)
					*pdwCookie = cookies[i]->dwCookie;
				if (ppvExtra)
					*ppvExtra = pCookie;

				// Cookie found
				return 1;
			}
		}
	}

	return 0;
}


void CIcqProto::FreeCookie(DWORD dwCookie)
{
	icq_lock l(cookieMutex);

	int i = cookies.getIndex((icq_cookie_info*)&dwCookie);
	if (i != INVALID_COOKIE_INDEX)
  {	// Cookie found, remove from list
    icq_cookie_info *cookie = cookies[i];

		cookies.remove(i);
    SAFE_FREE((void**)&cookie);
  }

	RemoveExpiredCookies();
}


void CIcqProto::FreeCookieByData(BYTE bType, void *pvExtra)
{
	icq_lock l(cookieMutex);

	for (int i = 0; i < cookies.getCount(); i++)
  {
    icq_cookie_info *cookie = cookies[i];

		if (bType == cookie->bType && pvExtra == cookie->pvExtra)
		{ // Cookie found, remove from list
			cookies.remove(i);
      SAFE_FREE((void**)&cookie);
			break;
		}
  }

	RemoveExpiredCookies();
}


void CIcqProto::ReleaseCookie(DWORD dwCookie)
{
	icq_lock l(cookieMutex);

	int i = cookies.getIndex(( icq_cookie_info* )&dwCookie );
	if (i != INVALID_COOKIE_INDEX)
	{ // Cookie found, remove from list
    icq_cookie_info *cookie = cookies[i];

		cookies.remove(i);
		SAFE_FREE((void**)&cookie->pvExtra);
    SAFE_FREE((void**)&cookie);
	}
	RemoveExpiredCookies();
}


void CIcqProto::InitMessageCookie(cookie_message_data *pCookie)
{
	DWORD dwMsgID1;
	DWORD dwMsgID2;

	do
	{ // ensure that message ids are unique
		dwMsgID1 = time(NULL);
		dwMsgID2 = RandRange(0, 0x0FFFF);
	} while (FindMessageCookie(dwMsgID1, dwMsgID2, NULL, NULL, NULL));

	if (pCookie)
	{
		pCookie->dwMsgID1 = dwMsgID1;
		pCookie->dwMsgID2 = dwMsgID2;
	}
}


cookie_message_data* CIcqProto::CreateMessageCookie(WORD bMsgType, BYTE bAckType)
{
	cookie_message_data *pCookie = (cookie_message_data*)SAFE_MALLOC(bMsgType == MTYPE_PLAIN ? sizeof(cookie_message_data_ext) : sizeof(cookie_message_data));
	if (pCookie)
	{
		pCookie->bMessageType = bMsgType;
		pCookie->nAckType = bAckType;

		InitMessageCookie(pCookie);
	}
	return pCookie;
}


cookie_message_data* CIcqProto::CreateMessageCookieData(BYTE bMsgType, HANDLE hContact, DWORD dwUin, int bUseSrvRelay)
{
	BYTE bAckType;
	WORD wStatus = getContactStatus(hContact);

	if (!getSettingByte(NULL, "SlowSend", DEFAULT_SLOWSEND) || (!dwUin && wStatus == ID_STATUS_OFFLINE))
		bAckType = ACKTYPE_NONE;
	else if ((bUseSrvRelay && ((!dwUin) || (!CheckContactCapabilities(hContact, CAPF_SRV_RELAY)) ||
		(wStatus == ID_STATUS_OFFLINE))) || getSettingByte(NULL, "OnlyServerAcks", DEFAULT_ONLYSERVERACKS))
		bAckType = ACKTYPE_SERVER;
	else
		bAckType = ACKTYPE_CLIENT;

	return CreateMessageCookie(bMsgType, bAckType);
}
