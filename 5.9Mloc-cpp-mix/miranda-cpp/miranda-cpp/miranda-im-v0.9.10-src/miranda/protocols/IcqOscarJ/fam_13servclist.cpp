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
// File name      : $URL: https://miranda.googlecode.com/svn/branches/stable/miranda/protocols/IcqOscarJ/fam_13servclist.cpp $
// Revision       : $Revision: 12239 $
// Last change on : $Date: 2010-07-27 08:24:02 -0400 (Tue, 27 Jul 2010) $
// Last change by : $Author: borkra $
//
// DESCRIPTION:
//
//  Describe me here please...
//
// -----------------------------------------------------------------------------

#include "icqoscar.h"


static int unpackServerListItem(BYTE **pbuf, WORD *pwLen, char *pszRecordName, WORD *pwGroupId, WORD *pwItemId, WORD *pwItemType, WORD *pwTlvLength);


void CIcqProto::handleServCListFam(BYTE *pBuffer, WORD wBufferLength, snac_header* pSnacHeader, serverthread_info *info)
{
	switch (pSnacHeader->wSubtype) {

	case ICQ_LISTS_ACK: // UPDATE_ACK
		if (wBufferLength >= 2)
		{
			WORD wError;
			cookie_servlist_action* sc;

			unpackWord(&pBuffer, &wError);

			if (FindCookie(pSnacHeader->dwRef, NULL, (void**)&sc))
			{ // look for action cookie
#ifdef _DEBUG
				NetLog_Server("Received expected server list ack, action: %d, result: %d", sc->dwAction, wError);
#endif
				FreeCookie(pSnacHeader->dwRef); // release cookie

				if (sc->dwAction == SSA_ACTION_GROUP)
				{ // group cookie, handle sub-items
					int i;

#ifdef _DEBUG
					NetLog_Server("Server-List: Grouped action contains %d actions.", sc->dwGroupCount);
#endif
					pBuffer -= 2; // revoke unpack
					if (wBufferLength != 2*sc->dwGroupCount)
						NetLog_Server("Error: Server list ack does not contain expected amount of result codes (%u != %u)", wBufferLength/2, sc->dwGroupCount);

					for (i = 0; i < sc->dwGroupCount; i++)
					{
						if (wBufferLength >= 2)
						{ // get proper result code
							unpackWord(&pBuffer, &wError);
							wBufferLength -= 2;
						}
						else // missing result code, give some special
							wError = -1;

#ifdef _DEBUG
						NetLog_Server("Action: %d, ack result: %d", sc->pGroupItems[i]->dwAction, wError);
#endif
						// call normal ack handler
						handleServerCListAck(sc->pGroupItems[i], wError);
					}
					// Release cookie
					SAFE_FREE((void**)&sc->pGroupItems);
					SAFE_FREE((void**)&sc);
				}
				else // single ack
					handleServerCListAck(sc, wError);
			}
			else
			{
				NetLog_Server("Received unexpected server list ack %u", wError);
			}
		}
		break;

	case ICQ_LISTS_SRV_REPLYLISTS:
		{ /* received server-list rights */
      handleServerCListRightsReply(pBuffer, wBufferLength);

#ifdef _DEBUG
			NetLog_Server("Server sent SNAC(x13,x03) - SRV_REPLYLISTS");
#endif
		}
		break;

	case ICQ_LISTS_LIST: // SRV_REPLYROSTER
		{
			cookie_servlist_action* sc;
			BOOL blWork;

			blWork = bIsSyncingCL;
			bIsSyncingCL = TRUE; // this is not used if cookie takes place

			if (FindCookie(pSnacHeader->dwRef, NULL, (void**)&sc))
			{ // we do it by reliable cookie
				if (!sc->lParam)
				{ // is this first packet ?
					ResetSettingsOnListReload();
					sc->lParam = 1;
				}
				handleServerCListReply(pBuffer, wBufferLength, pSnacHeader->wFlags, info);
				if (!(pSnacHeader->wFlags & 0x0001))
				{ // was that last packet ?
					ReleaseCookie(pSnacHeader->dwRef); // yes, release cookie
				}
			}
			else
			{ // use old fake
				if (!blWork)
				{ // this can fail on some crazy situations
					ResetSettingsOnListReload();
				}
				handleServerCListReply(pBuffer, wBufferLength, pSnacHeader->wFlags, info);
			}
			break;
		}

	case ICQ_LISTS_UPTODATE: // SRV_REPLYROSTEROK
		{
			cookie_servlist_action* sc;

			bIsSyncingCL = FALSE;

			if (FindCookie(pSnacHeader->dwRef, NULL, (void**)&sc))
			{ // we requested servlist check
#ifdef _DEBUG
				NetLog_Server("Server stated roster is ok.");
#endif
				ReleaseCookie(pSnacHeader->dwRef);
				LoadServerIDs();
			}
			else
				NetLog_Server("Server sent unexpected SNAC(x13,x0F) - SRV_REPLYROSTEROK");

			// This will activate the server side list
			sendRosterAck(); // this must be here, cause of failures during cookie alloc
			handleServUINSettings(wListenPort, info);
			break;
		}

	case ICQ_LISTS_CLI_MODIFYSTART:
		NetLog_Server("Server sent SNAC(x13,x%02x) - %s", ICQ_LISTS_CLI_MODIFYSTART, "Server is modifying contact list");
		break;

	case ICQ_LISTS_CLI_MODIFYEND:
		NetLog_Server("Server sent SNAC(x13,x%02x) - %s", ICQ_LISTS_CLI_MODIFYEND, "End of server modification");
		break;

	case ICQ_LISTS_ADDTOLIST:
	case ICQ_LISTS_UPDATEGROUP:
	case ICQ_LISTS_REMOVEFROMLIST:
    {
      int nItems = 0;

		  while (wBufferLength >= 10)
		  {
			  WORD wGroupId, wItemId, wItemType, wTlvLen;
			  uid_str szRecordName;

			  if (unpackServerListItem(&pBuffer, &wBufferLength, szRecordName, &wGroupId, &wItemId, &wItemType, &wTlvLen))
			  {
          BYTE *buf = pBuffer;
          oscar_tlv_chain *pChain = NULL;

          nItems++;

          // parse possible item's data
          if (wBufferLength >= wTlvLen && wTlvLen > 0)
          {
            pChain = readIntoTLVChain(&buf, wTlvLen, 0);
            pBuffer += wTlvLen;
            wBufferLength -= wTlvLen;
          }
          else if (wTlvLen > 0)
            wBufferLength = 0;

          // process item change
          if (pSnacHeader->wSubtype == ICQ_LISTS_ADDTOLIST)
            handleServerCListItemAdd(szRecordName, wGroupId, wItemId, wItemType, pChain);
          else if (pSnacHeader->wSubtype == ICQ_LISTS_UPDATEGROUP)
            handleServerCListItemUpdate(szRecordName, wGroupId, wItemId, wItemType, pChain);
          else if (pSnacHeader->wSubtype == ICQ_LISTS_REMOVEFROMLIST)
            handleServerCListItemDelete(szRecordName, wGroupId, wItemId, wItemType, pChain);

          // release memory
          disposeChain(&pChain);
			  }
		  }
      { // log packet basics
        char *szChange;
        char szLogText[MAX_PATH];
		   
        if (pSnacHeader->wSubtype == ICQ_LISTS_ADDTOLIST)
          szChange = "Server added %u item(s) to list";
        else if (pSnacHeader->wSubtype == ICQ_LISTS_UPDATEGROUP)
          szChange = "Server updated %u item(s) on list";
        else if (pSnacHeader->wSubtype == ICQ_LISTS_REMOVEFROMLIST)
          szChange = "Server removed %u item(s) from list";

        null_snprintf(szLogText, MAX_PATH, szChange, nItems);
        NetLog_Server("Server sent SNAC(x13,x%02x) - %s", pSnacHeader->wSubtype, szLogText);
      }
    }
		break;

	case ICQ_LISTS_AUTHREQUEST:
		handleRecvAuthRequest(pBuffer, wBufferLength);
		break;

	case ICQ_LISTS_SRV_AUTHRESPONSE:
		handleRecvAuthResponse(pBuffer, wBufferLength);
		break;

	case ICQ_LISTS_AUTHGRANTED:
		NetLog_Server("Server sent SNAC(x13,x%02x) - %s", ICQ_LISTS_AUTHGRANTED, "User granted us future authorization");
		break;

	case ICQ_LISTS_YOUWEREADDED:
		handleRecvAdded(pBuffer, wBufferLength);
		break;

	case ICQ_LISTS_ERROR:
		if (wBufferLength >= 2)
		{
			WORD wError;
			cookie_servlist_action* sc;

			unpackWord(&pBuffer, &wError);

			if (FindCookie(pSnacHeader->dwRef, NULL, (void**)&sc))
			{ // look for action cookie
#ifdef _DEBUG
				NetLog_Server("Received server list error, action: %d, result: %d", sc->dwAction, wError);
#endif
				FreeCookie(pSnacHeader->dwRef); // release cookie

				if (sc->dwAction==SSA_CHECK_ROSTER)
				{ // the serv-list is unavailable turn it off
					icq_LogMessage(LOG_ERROR, LPGEN("Server contact list is unavailable, Miranda will use local contact list."));
					m_bSsiEnabled = 0;
					handleServUINSettings(wListenPort, info);
				}
        /// FIXME: properly release pending operations & cookie memory
				SAFE_FREE((void**)&sc);
			}
			else
			{
				LogFamilyError(ICQ_LISTS_FAMILY, wError);
			}
		}
		break;

	default:
		NetLog_Server("Warning: Ignoring SNAC(x%02x,x%02x) - Unknown SNAC (Flags: %u, Ref: %u)", ICQ_LISTS_FAMILY, pSnacHeader->wSubtype, pSnacHeader->wFlags, pSnacHeader->dwRef);
		break;
	}
}


static int unpackServerListItem(BYTE **pbuf, WORD *pwLen, char *pszRecordName, WORD *pwGroupId, WORD *pwItemId, WORD *pwItemType, WORD *pwTlvLength)
{
	WORD wRecordNameLen;

	// The name of the entry. If this is a group header, then this
	// is the name of the group. If it is a plain contact list entry,
	// then it's the UIN of the contact.
	unpackWord(pbuf, &wRecordNameLen);
	if (*pwLen < 10 + wRecordNameLen || wRecordNameLen >= MAX_PATH)
		return 0; // Failure

	unpackString(pbuf, pszRecordName, wRecordNameLen);
	if (pszRecordName)
		pszRecordName[wRecordNameLen] = '\0';

	// The group identifier this entry belongs to. If 0, this is meta information or
	// a contact without a group
	unpackWord(pbuf, pwGroupId);

	// The ID of this entry. Group headers have ID 0. Otherwise, this
	// is a random number generated when the user is added to the
	// contact list, or when the user is ignored. See CLI_ADDBUDDY.
	unpackWord(pbuf, pwItemId);

	// This field indicates what type of entry this is
	unpackWord(pbuf, pwItemType);

	// The length in bytes of the following TLV chain
	unpackWord(pbuf, pwTlvLength);

	*pwLen -= wRecordNameLen + 10;

	return 1; // Success
}


void CIcqProto::handleServerCListRightsReply(BYTE *buf, WORD wLen)
{	/* received list rights, store the item limits for future use */
  oscar_tlv_chain* chain;

  memset(m_wServerListLimits, -1, sizeof(m_wServerListLimits));
  m_wServerListGroupMaxContacts = 0;
  m_wServerListRecordNameMaxLength = 0xFFFF;

  if (chain = readIntoTLVChain(&buf, wLen, 0))
  {
    oscar_tlv* pTLV;

    // determine max number of contacts in a group
    m_wServerListGroupMaxContacts = chain->getWord(0x0C, 1);
    // determine length limit for server-list item's name
    m_wServerListRecordNameMaxLength = chain->getWord(0x06, 1);

    if (pTLV = chain->getTLV(0x04, 1))
    { // limits for item types
      int i;
      WORD *pLimits = (WORD*)pTLV->pData;

      for (i = 0; i < pTLV->wLen / 2; i++)
      {
        m_wServerListLimits[i] = (pLimits[i] & 0xFF) << 8 | (pLimits[i] >> 8);

        if (i + 1 >= SIZEOF(m_wServerListLimits)) break;
      }

      NetLog_Server("SSI: Max %d contacts (%d per group), %d groups, %d permit, %d deny, %d ignore items.", m_wServerListLimits[SSI_ITEM_BUDDY], m_wServerListGroupMaxContacts, m_wServerListLimits[SSI_ITEM_GROUP], m_wServerListLimits[SSI_ITEM_PERMIT], m_wServerListLimits[SSI_ITEM_DENY], m_wServerListLimits[SSI_ITEM_IGNORE]);
    }

    disposeChain(&chain);
  }
}


DWORD CIcqProto::updateServerGroupData(WORD wGroupId, void *groupData, int groupSize, DWORD dwOperationFlags)
{
	DWORD dwCookie;
	cookie_servlist_action* ack;

	ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
	if (!ack)
	{
		NetLog_Server("Updating of group on server list failed (malloc error)");
		return 0;
	}
	ack->dwAction = SSA_GROUP_UPDATE;
	ack->szGroupName = getServListGroupName(wGroupId);
	ack->wGroupId = wGroupId;
	dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_UPDATEGROUP, 0, ack);

	return icq_sendServerGroup(dwCookie, ICQ_LISTS_UPDATEGROUP, ack->wGroupId, ack->szGroupName, groupData, groupSize, dwOperationFlags);
}


void CIcqProto::handleServerCListAck(cookie_servlist_action* sc, WORD wError)
{
	switch (sc->dwAction)
	{
	case SSA_VISIBILITY: 
		{
			if (wError)
				NetLog_Server("Server visibility update failed, error %d", wError);
			break;
		}
	case SSA_CONTACT_UPDATE:
		{
      servlistPendingRemoveContact(sc->hContact, sc->wContactId, sc->wGroupId, wError ? PENDING_RESULT_FAILED : PENDING_RESULT_SUCCESS);
			if (wError)
			{
				NetLog_Server("Updating of server contact failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Updating of server contact failed."));
			}
			break;
		}
	case SSA_PRIVACY_ADD: 
		{
			if (wError)
			{
				NetLog_Server("Adding of privacy item to server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Adding of privacy item to server list failed."));
			}
			break;
		}
	case SSA_PRIVACY_REMOVE: 
		{
			if (wError)
			{
				NetLog_Server("Removing of privacy item from server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Removing of privacy item from server list failed."));
			}
			FreeServerID(sc->wContactId, SSIT_ITEM); // release server id
			break;
		}
	case SSA_CONTACT_ADD:
		{
			if (wError)
			{
				if (wError == 0xE) // server refused to add contact w/o auth, add with
				{
					DWORD dwCookie;

					NetLog_Server("Contact could not be added without authorization, add with await auth flag.");

					setSettingByte(sc->hContact, "Auth", 1); // we need auth
					dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_ADDTOLIST, sc->hContact, sc);
					icq_sendServerContact(sc->hContact, dwCookie, ICQ_LISTS_ADDTOLIST, sc->wGroupId, sc->wContactId, SSOP_ITEM_ACTION | SSOF_CONTACT, 500, NULL);

					sc = NULL; // we do not want it to be freed now
					break;
				}
				FreeServerID(sc->wContactId, SSIT_ITEM);

				NetLog_Server("Adding of contact to server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Adding of contact to server list failed."));

        servlistPendingRemoveContact(sc->hContact, 0, sc->wGroupId, PENDING_RESULT_FAILED);

        servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100); // end server modifications here
			}
			else
			{
				void* groupData;
				int groupSize;

				setSettingWord(sc->hContact, DBSETTING_SERVLIST_ID, sc->wContactId);
				setSettingWord(sc->hContact, DBSETTING_SERVLIST_GROUP, sc->wGroupId);

        servlistPendingRemoveContact(sc->hContact, sc->wContactId, sc->wGroupId, PENDING_RESULT_SUCCESS);

				if (groupData = collectBuddyGroup(sc->wGroupId, &groupSize))
				{ // the group is not empty, just update it
					updateServerGroupData(sc->wGroupId, groupData, groupSize, SSOF_END_OPERATION);
					SAFE_FREE((void**)&groupData);
				}
				else
				{ // this should never happen
					NetLog_Server("Group update failed.");
          servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100); // end server modifications here
				}
			}
			break;
		}
	case SSA_GROUP_ADD:
		{
			if (wError)
			{
				FreeServerID(sc->wGroupId, SSIT_GROUP);
				NetLog_Server("Adding of group to server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Adding of group to server list failed."));

        servlistPendingRemoveGroup(sc->szGroup, 0, PENDING_RESULT_FAILED);
			}
			else // group added, we need to update master group
			{
				void* groupData;
				int groupSize;
				cookie_servlist_action* ack;
				DWORD dwCookie;

				setServListGroupName(sc->wGroupId, sc->szGroupName); // add group to namelist
				{ // add group to known
					char *szCListGroup = getServListGroupCListPath(sc->wGroupId);
          
          // create link to the original CList group
					setServListGroupLinkID(sc->szGroup, sc->wGroupId);

          servlistPendingRemoveGroup(sc->szGroup, sc->wGroupId, PENDING_RESULT_SUCCESS);
          SAFE_FREE((void**)&szCListGroup);
				}

				groupData = collectGroups(&groupSize);
				groupData = SAFE_REALLOC(groupData, groupSize+2);
				*(((WORD*)groupData)+(groupSize>>1)) = sc->wGroupId; // add this new group id
				groupSize += 2;

				ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
				if (ack)
				{
					ack->dwAction = SSA_GROUP_UPDATE;
					dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_UPDATEGROUP, 0, ack);

					icq_sendServerGroup(dwCookie, ICQ_LISTS_UPDATEGROUP, 0, ack->szGroupName, groupData, groupSize, SSOF_END_OPERATION);
				}
        else // end server modifications here
          servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100);

				SAFE_FREE((void**)&groupData);
			}
      if (sc->szGroup != sc->szGroupName)
        SAFE_FREE((void**)&sc->szGroup);

			SAFE_FREE((void**)&sc->szGroupName);
			break;
		}
	case SSA_CONTACT_REMOVE:
		{
			if (!wError)
			{
				void* groupData;
				int groupSize;

				setSettingWord(sc->hContact, DBSETTING_SERVLIST_ID, 0); // clear the values
				setSettingWord(sc->hContact, DBSETTING_SERVLIST_GROUP, 0);

				FreeServerID(sc->wContactId, SSIT_ITEM); 

        servlistPendingRemoveContact(sc->hContact, 0, sc->wGroupId, PENDING_RESULT_SUCCESS);

				if (groupData = collectBuddyGroup(sc->wGroupId, &groupSize))
				{ // the group is still not empty, just update it
					updateServerGroupData(sc->wGroupId, groupData, groupSize, SSOF_END_OPERATION);
				}
				else // the group is empty, delete it
				{
          char *szGroup = getServListGroupCListPath(sc->wGroupId);

          servlistRemoveGroup(szGroup, sc->wGroupId);
          SAFE_FREE((void**)&szGroup);
				}
				SAFE_FREE((void**)&groupData); // free the memory
			}
			else
			{
				NetLog_Server("Removing of contact from server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Removing of contact from server list failed."));

        servlistPendingRemoveContact(sc->hContact, sc->wContactId, sc->wGroupId, PENDING_RESULT_FAILED);

        servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100); // end server modifications here
			}
			break;
		}
	case SSA_GROUP_UPDATE:
		{
			if (wError)
			{
				NetLog_Server("Updating of group on server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Updating of group on server list failed."));
			}
			SAFE_FREE((void**)&sc->szGroupName);
			break;
		}
	case SSA_GROUP_REMOVE:
		{
			SAFE_FREE((void**)&sc->szGroupName);
			if (wError)
			{
				NetLog_Server("Removing of group from server list failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Removing of group from server list failed."));

        servlistPendingRemoveGroup(sc->szGroup, 0, PENDING_RESULT_FAILED);

        servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100); // end server modifications here
        SAFE_FREE((void**)&sc->szGroup);
			}
			else // group removed, we need to update master group
			{
				void* groupData;
				int groupSize;
				DWORD dwCookie;

				setServListGroupName(sc->wGroupId, NULL); // clear group from namelist
				FreeServerID(sc->wGroupId, SSIT_GROUP);
				removeGroupPathLinks(sc->wGroupId);

        servlistPendingRemoveGroup(sc->szGroup, 0, PENDING_RESULT_SUCCESS);
        SAFE_FREE((void**)&sc->szGroup);

				groupData = collectGroups(&groupSize);
				sc->wGroupId = 0;
				sc->dwAction = SSA_GROUP_UPDATE;
				sc->szGroupName = NULL;
				dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_UPDATEGROUP, 0, sc);

				icq_sendServerGroup(dwCookie, ICQ_LISTS_UPDATEGROUP, 0, sc->szGroupName, groupData, groupSize, SSOF_END_OPERATION);
				// end server modifications here

				sc = NULL; // we do not want to be freed here

				SAFE_FREE((void**)&groupData);
			}
			break;
		}
	case SSA_CONTACT_SET_GROUP:
		{ // we moved contact to another group
			if (sc->lParam == -1)
			{ // the first was an error
				break;
			}
			if (wError)
			{
				if (wError == 0x0E && sc->lParam == 1)
				{ // second ack - adding failed with error 0x0E, try to add with AVAIT_AUTH flag
					DWORD dwCookie;

					if (!getSettingByte(sc->hContact, "Auth", 0))
					{ // we tried without AWAIT_AUTH, try again with it
						NetLog_Server("Contact could not be added without authorization, add with await auth flag.");

						setSettingByte(sc->hContact, "Auth", 1); // we need auth
					}
					else
					{ // we tried with AWAIT_AUTH, try again without
						NetLog_Server("Contact count not be added awaiting authorization, try authorized.");

						setSettingByte(sc->hContact, "Auth", 0);
					}
					dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_ADDTOLIST, sc->hContact, sc);
					icq_sendServerContact(sc->hContact, dwCookie, ICQ_LISTS_ADDTOLIST, sc->wNewGroupId, sc->wNewContactId, SSOP_ITEM_ACTION | SSOF_CONTACT, 400, NULL);

					sc->lParam = 2; // do not cycle
					sc = NULL; // we do not want to be freed here
					break;
				}
        FreeServerID(sc->wNewContactId, SSIT_ITEM);
				NetLog_Server("Moving of user to another group on server list failed, error %d", wError);
				icq_LogMessage(LOG_ERROR, LPGEN("Moving of user to another group on server list failed."));

        servlistPendingRemoveContact(sc->hContact, 0, (WORD)(sc->lParam ? sc->wGroupId : sc->wNewGroupId), PENDING_RESULT_FAILED);

        servlistPostPacket(NULL, 0, SSO_END_OPERATION, 100); // end server modifications here

				if (!sc->lParam) // is this first ack ?
				{
					sc->lParam = -1;
					sc = NULL; // this can't be freed here
				}
				break;
			}
			if (sc->lParam) // is this the second ack ?
			{
				void* groupData;
				int groupSize;
				int bEnd = 1; // shall we end the sever modifications

				setSettingWord(sc->hContact, DBSETTING_SERVLIST_ID, sc->wNewContactId);
				setSettingWord(sc->hContact, DBSETTING_SERVLIST_GROUP, sc->wNewGroupId);

        servlistPendingRemoveContact(sc->hContact, sc->wNewContactId, sc->wNewGroupId, PENDING_RESULT_SUCCESS);

				if (groupData = collectBuddyGroup(sc->wGroupId, &groupSize)) // update the group we moved from
				{ // the group is still not empty, just update it
					updateServerGroupData(sc->wGroupId, groupData, groupSize, 0);
					SAFE_FREE((void**)&groupData); // free the memory
				}
				else
        { // the group is empty, delete it
          char* szGroup = getServListGroupCListPath(sc->wGroupId);

          servlistRemoveGroup(szGroup, sc->wGroupId);
          SAFE_FREE((void**)&szGroup);
          bEnd = 0; // here the modifications go on
				}

				groupData = collectBuddyGroup(sc->wNewGroupId, &groupSize); // update the group we moved to
        updateServerGroupData(sc->wNewGroupId, groupData, groupSize, bEnd ? SSOF_END_OPERATION : 0);
        // end server modifications here
				SAFE_FREE((void**)&groupData);

			}
			else // contact was deleted from server-list
			{
				deleteSetting(sc->hContact, DBSETTING_SERVLIST_ID);
				deleteSetting(sc->hContact, DBSETTING_SERVLIST_GROUP);
        FreeServerID(sc->wContactId, SSIT_ITEM); // release old contact id
				sc->lParam = 1;
				sc = NULL; // wait for second ack
			}
			break;
		}
	case SSA_CONTACT_FIX_AUTH:
		{
			if (wError)
			{ // FIXME: something failed, we should handle it properly
			}
			break;
		}
	case SSA_GROUP_RENAME:
		{
			if (wError)
			{
				NetLog_Server("Renaming of server group failed, error %d", wError);
				icq_LogMessage(LOG_WARNING, LPGEN("Renaming of server group failed."));

        servlistPendingRemoveGroup(sc->szGroup, sc->wGroupId, PENDING_RESULT_FAILED);
			}
			else
			{ 
				setServListGroupName(sc->wGroupId, sc->szGroupName);
				removeGroupPathLinks(sc->wGroupId);
				{ // add group to known
					char *szCListGroup = getServListGroupCListPath(sc->wGroupId);

          /// FIXME: need to create link to the new group name before unique item name correction as well
					setServListGroupLinkID(szCListGroup, sc->wGroupId);
					SAFE_FREE((void**)&szCListGroup);
				}
        servlistPendingRemoveGroup(sc->szGroup, sc->wGroupId, PENDING_RESULT_SUCCESS);
			}
			SAFE_FREE((void**)&sc->szGroupName);
      SAFE_FREE((void**)&sc->szGroup);
			break;
		}
	case SSA_SETAVATAR:
		{
			if (wError)
			{
				NetLog_Server("Uploading of avatar hash failed.");
				if (sc->wGroupId) // is avatar added or updated?
				{
					FreeServerID(sc->wContactId, SSIT_ITEM);
					deleteSetting(NULL, DBSETTING_SERVLIST_AVATAR); // to fix old versions
				}
			}
			else
			{
				setSettingWord(NULL, DBSETTING_SERVLIST_AVATAR, sc->wContactId);
			}
			break;
		}
	case SSA_REMOVEAVATAR:
		{
			if (wError)
				NetLog_Server("Removing of avatar hash failed.");
			else
			{
				FreeServerID(sc->wContactId, SSIT_ITEM);
				deleteSetting(NULL, DBSETTING_SERVLIST_AVATAR);
			}
			break;
		}
	case SSA_SERVLIST_ACK:
		{
			BroadcastAck(sc->hContact, ICQACKTYPE_SERVERCLIST, wError?ACKRESULT_FAILED:ACKRESULT_SUCCESS, (HANDLE)sc->lParam, wError);
			break;
		}
	case SSA_IMPORT:
		{
			if (wError)
				NetLog_Server("Re-starting import sequence failed, error %d", wError);
			else
			{
				setSettingWord(NULL, "SrvImportID", 0);
				deleteSetting(NULL, "ImportTS");
			}
			break;
		}
	default:
		NetLog_Server("Server ack cookie type (%d) not recognized.", sc->dwAction);
	}
	SAFE_FREE((void**)&sc); // free the memory

	return;
}


HANDLE CIcqProto::HContactFromRecordName(const char* szRecordName, int *bAdded)
{
	HANDLE hContact = INVALID_HANDLE_VALUE;

	if (!IsStringUIN(szRecordName))
	{ // probably AIM contact
		hContact = HContactFromUID(0, szRecordName, bAdded);
	}
	else
	{ // this should be ICQ number
		DWORD dwUin = atoi(szRecordName);

		hContact = HContactFromUIN(dwUin, bAdded);
	}
	return hContact;
}


int CIcqProto::getServerDataFromItemTLV(oscar_tlv_chain* pChain, unsigned char *buf) /// FIXME: need to keep original order
{ // get server-list item's TLV data
	oscar_tlv_chain* list = pChain;
	int datalen = 0;
	icq_packet pBuf;

	// Initialize our handy data buffer
	pBuf.wPlace = 0;
	pBuf.pData = buf;

	while (list)
	{ // collect non-standard TLVs and save them to DB
		if (list->tlv.wType != SSI_TLV_AWAITING_AUTH &&
			list->tlv.wType != SSI_TLV_NAME &&
			list->tlv.wType != SSI_TLV_COMMENT &&
      list->tlv.wType != SSI_TLV_METAINFO_TOKEN &&
      list->tlv.wType != SSI_TLV_METAINFO_TIME)
		{ // only TLVs which we do not handle on our own
			packTLV(&pBuf, list->tlv.wType, list->tlv.wLen, list->tlv.pData);

			datalen += list->tlv.wLen + 4;
		}
		list = list->next;
	}
	return datalen;
}


void CIcqProto::handleServerCListReply(BYTE *buf, WORD wLen, WORD wFlags, serverthread_info *info)
{
	BYTE bySSIVersion;
	WORD wRecordCount;
	WORD wRecord;
	WORD wGroupId;
	WORD wItemId;
	WORD wTlvType;
	WORD wTlvLength;
	BOOL bIsLastPacket;
	uid_str szRecordName;
	oscar_tlv_chain* pChain = NULL;
	oscar_tlv* pTLV = NULL;
  char *szActiveSrvGroup = NULL;
  WORD wActiveSrvGroupId = -1;


	// If flag bit 1 is set, this is not the last
	// packet. If it is 0, this is the last packet
	// and there will be a timestamp at the end.
	if (wFlags & 0x0001)
		bIsLastPacket = FALSE;
	else
		bIsLastPacket = TRUE;

	if (wLen < 3)
		return;

	// Version number of SSI protocol?
	unpackByte(&buf, &bySSIVersion);
	wLen -= 1;

	// Total count of following entries. This is the size of the server
	// side contact list and should be saved and sent with CLI_CHECKROSTER.
	// NOTE: When the entries are split up in several packets, each packet
	// has it's own count and they must be added to get the total size of
	// server list.
	unpackWord(&buf, &wRecordCount);
	wLen -= 2;
	NetLog_Server("SSI: number of entries is %u, version is %u", wRecordCount, bySSIVersion);


	// Loop over all items in the packet
	for (wRecord = 0; wRecord < wRecordCount; wRecord++)
	{
		NetLog_Server("SSI: parsing record %u", wRecord + 1);

		if (wLen < 10)
		{ // minimum: name length (zero), group ID, item ID, empty TLV
			NetLog_Server("Warning: SSI parsing error (%d)", 0);
			break;
		}

		if (!unpackServerListItem(&buf, &wLen, szRecordName, &wGroupId, &wItemId, &wTlvType, &wTlvLength))
		{ // unpack basic structure
			NetLog_Server("Warning: SSI parsing error (%d)", 1);
			break;
		}

		NetLog_Server("Name: '%s', GroupID: %u, EntryID: %u, EntryType: %u, TLVlength: %u",
			szRecordName, wGroupId, wItemId, wTlvType, wTlvLength);

		if (wLen < wTlvLength)
		{
			NetLog_Server("Warning: SSI parsing error (%d)", 2);
			break;
		}

		// Initialize the tlv chain
		if (wTlvLength > 0)
		{
			pChain = readIntoTLVChain(&buf, wTlvLength, 0);
			wLen -= wTlvLength;
		}
		else
		{
			pChain = NULL;
		}


		switch (wTlvType)
		{

		case SSI_ITEM_BUDDY:
			{
				/* this is a contact */
				HANDLE hContact;
				int bAdded;

				hContact = HContactFromRecordName(szRecordName, &bAdded);

				if (hContact != INVALID_HANDLE_VALUE)
				{
					int bRegroup = 0;
					int bNicked = 0;

					if (bAdded)
					{ // Not already on list: added
						NetLog_Server("SSI added new %s contact '%s'", "ICQ", szRecordName);

						AddJustAddedContact(hContact);
					}
					else
					{ // we should add new contacts and this contact was just added, show it
						if (IsContactJustAdded(hContact))
						{
							setContactHidden(hContact, 0);
							bAdded = 1; // we want details for new contacts
						}
						else
							NetLog_Server("SSI ignoring existing contact '%s'", szRecordName);
						// Contact on server is always on list
						DBWriteContactSettingByte(hContact, "CList", "NotOnList", 0);
					}

					// Save group and item ID
					setSettingWord(hContact, DBSETTING_SERVLIST_ID, wItemId);
					setSettingWord(hContact, DBSETTING_SERVLIST_GROUP, wGroupId);
					ReserveServerID(wItemId, SSIT_ITEM, 0);

					if (!bAdded && getSettingByte(NULL, "LoadServerDetails", DEFAULT_SS_LOAD))
					{ // check if the contact has been moved on the server
            if (wActiveSrvGroupId != wGroupId || !szActiveSrvGroup)
            {
              SAFE_FREE(&szActiveSrvGroup);
						  szActiveSrvGroup = getServListGroupCListPath(wGroupId);
              wActiveSrvGroupId = wGroupId;
            }
            char *szLocalGroup = getContactCListGroup(hContact);

            if (!strlennull(szLocalGroup))
            { // no CListGroup
							SAFE_FREE(&szLocalGroup);

							szLocalGroup = null_strdup(DEFAULT_SS_GROUP);
						}

						if (strcmpnull(szActiveSrvGroup, szLocalGroup) && 
               (strlennull(szActiveSrvGroup) >= strlennull(szLocalGroup) || _strnicmp(szActiveSrvGroup, szLocalGroup, strlennull(szLocalGroup))))
						{ // contact moved to new group or sub-group or not to master group
							bRegroup = 1;
						}
						if (bRegroup && !stricmpnull(DEFAULT_SS_GROUP, szActiveSrvGroup)) /// TODO: invent something more clever for "root" group
						{ // is it the default "General" group ?
							bRegroup = 0; // if yes, do not move to it - cause it would hide the contact
						}
						SAFE_FREE(&szLocalGroup);
					}

					if (bRegroup || bAdded)
					{ // if we should load server details or contact was just added, update its group
            if (wActiveSrvGroupId != wGroupId || !szActiveSrvGroup)
            {
              SAFE_FREE(&szActiveSrvGroup);
						  szActiveSrvGroup = getServListGroupCListPath(wGroupId);
              wActiveSrvGroupId = wGroupId;
            }

						if (szActiveSrvGroup)
						{ // try to get Miranda Group path from groupid, if succeeded save to db
              moveContactToCListGroup(hContact, szActiveSrvGroup);
						}
					}

					if (pChain)
					{ // Look for nickname TLV and copy it to the db if necessary
						if (pTLV = pChain->getTLV(SSI_TLV_NAME, 1))
						{
							if (pTLV->pData && (pTLV->wLen > 0))
							{
								char *pszNick;
								WORD wNickLength;

								wNickLength = pTLV->wLen;

								pszNick = (char*)SAFE_MALLOC(wNickLength + 1);
								// Copy buffer to utf-8 buffer
								memcpy(pszNick, pTLV->pData, wNickLength);
								pszNick[wNickLength] = 0; // Terminate string

								NetLog_Server("Nickname is '%s'", pszNick);

								bNicked = 1;

								// Write nickname to database
								if (getSettingByte(NULL, "LoadServerDetails", DEFAULT_SS_LOAD) || bAdded)
								{ // if just added contact, save details always - does no harm
									char *szOldNick;

									if (szOldNick = getSettingStringUtf(hContact, "CList", "MyHandle", NULL))
									{
										if ((strcmpnull(szOldNick, pszNick)) && (strlennull(pszNick) > 0))
										{ // check if the truncated nick changed, i.e. do not overwrite locally stored longer nick
											if (strlennull(szOldNick) <= strlennull(pszNick) || strncmp(szOldNick, pszNick, null_strcut(szOldNick, MAX_SSI_TLV_NAME_SIZE)))
											{
												// Yes, we really do need to delete it first. Otherwise the CLUI nick
												// cache isn't updated (I'll look into it)
												DBDeleteContactSetting(hContact,"CList","MyHandle");
												setSettingStringUtf(hContact, "CList", "MyHandle", pszNick);
											}
										}
										SAFE_FREE(&szOldNick);
									}
									else if (strlennull(pszNick) > 0)
									{
										DBDeleteContactSetting(hContact,"CList","MyHandle");
										setSettingStringUtf(hContact, "CList", "MyHandle", pszNick);
									}
								}
								SAFE_FREE(&pszNick);
							}
							else
							{
								NetLog_Server("Invalid nickname");
							}
						}
						if (bAdded && !bNicked)
							icq_QueueUser(hContact); // queue user without nick for fast auto info update

						// Look for comment TLV and copy it to the db if necessary
						if (pTLV = pChain->getTLV(SSI_TLV_COMMENT, 1))
						{
							if (pTLV->pData && (pTLV->wLen > 0))
							{
								char *pszComment;
								WORD wCommentLength;


								wCommentLength = pTLV->wLen;

								pszComment = (char*)SAFE_MALLOC(wCommentLength + 1);
								// Copy buffer to utf-8 buffer
								memcpy(pszComment, pTLV->pData, wCommentLength);
								pszComment[wCommentLength] = 0; // Terminate string

								NetLog_Server("Comment is '%s'", pszComment);

								// Write comment to database
								if (getSettingByte(NULL, "LoadServerDetails", DEFAULT_SS_LOAD) || bAdded)
								{ // if just added contact, save details always - does no harm
									char *szOldComment;

									if (szOldComment = getSettingStringUtf(hContact, "UserInfo", "MyNotes", NULL))
									{
										if ((strcmpnull(szOldComment, pszComment)) && (strlennull(pszComment) > 0))
										{ // check if the truncated comment changed, i.e. do not overwrite locally stored longer comment
											if (strlennull(szOldComment) <= strlennull(pszComment) || strncmp((char*)szOldComment, (char*)pszComment, null_strcut(szOldComment, MAX_SSI_TLV_COMMENT_SIZE)))
											{
												setSettingStringUtf(hContact, "UserInfo", "MyNotes", pszComment);
											}
										}
										SAFE_FREE((void**)&szOldComment);
									}
									else if (strlennull(pszComment) > 0)
									{
										setSettingStringUtf(hContact, "UserInfo", "MyNotes", pszComment);
									}
								}
								SAFE_FREE((void**)&pszComment);
							}
							else
							{
								NetLog_Server("Invalid comment");
							}
						}

						// Look for need-authorization TLV
						if (pChain->getTLV(SSI_TLV_AWAITING_AUTH, 1))
						{
							setSettingByte(hContact, "Auth", 1);
							NetLog_Server("SSI contact need authorization");
						}
						else
						{
							setSettingByte(hContact, "Auth", 0);
						}

            if (pTLV = pChain->getTLV(SSI_TLV_METAINFO_TOKEN, 1))
            {
              setSettingBlob(hContact, DBSETTING_METAINFO_TOKEN, pTLV->pData, pTLV->wLen);
              if (pChain->getTLV(SSI_TLV_METAINFO_TIME, 1))
                setSettingDouble(hContact, DBSETTING_METAINFO_TIME, pChain->getDouble(SSI_TLV_METAINFO_TIME, 1));
              NetLog_Server("SSI contact has meta info token");
            }
            else
            {
              deleteSetting(hContact, DBSETTING_METAINFO_TOKEN);
              deleteSetting(hContact, DBSETTING_METAINFO_TIME);
            }

						{ // store server-list item's TLV data
							BYTE* data = (BYTE*)SAFE_MALLOC(wTlvLength);
							int datalen = getServerDataFromItemTLV(pChain, data);

							if (datalen > 0)
								setSettingBlob(hContact, DBSETTING_SERVLIST_DATA, data, datalen);
							else
								deleteSetting(hContact, DBSETTING_SERVLIST_DATA);

							SAFE_FREE((void**)&data);
						}
					}
				}
				else
				{ // failed to add or other error
					NetLog_Server("SSI failed to handle %s Item '%s'", "Buddy", szRecordName);
				}
			}
			break;

		case SSI_ITEM_GROUP:
			if ((wGroupId == 0) && (wItemId == 0))
			{
				/* list of groups. wTlvType=1, data is TLV(C8) containing list of WORDs which */
				/* is the group ids
				/* we don't need to use this. Our processing is on-the-fly */
				/* this record is always sent first in the first packet only, */
			}
			else if (wGroupId != 0)
			{
				/* wGroupId != 0: a group record */
				if (wItemId == 0)
				{ /* no item ID: this is a group */
					/* pszRecordName is the name of the group */
					ReserveServerID(wGroupId, SSIT_GROUP, 0);

					setServListGroupName(wGroupId, szRecordName);

					NetLog_Server("Group %s added to known groups.", szRecordName);

					/* demangle full grouppath, set it to known */
          SAFE_FREE(&szActiveSrvGroup);
          szActiveSrvGroup = getServListGroupCListPath(wGroupId);
          wActiveSrvGroupId = wGroupId;

					/* TLV contains a TLV(C8) with a list of WORDs of contained contact IDs */
					/* our processing is good enough that we don't need this duplication */
				}
				else
				{
					NetLog_Server("Unhandled type 0x01, wItemID != 0");
				}
			}
			else
			{
				NetLog_Server("Unhandled type 0x01");
			}
			break;

		case SSI_ITEM_PERMIT:
			{
				/* item on visible list */
				/* wItemId not related to contact ID */
				/* pszRecordName is the UIN */
				HANDLE hContact;
				int bAdded;

				hContact = HContactFromRecordName(szRecordName, &bAdded);

				if (hContact != INVALID_HANDLE_VALUE)
				{
					if (bAdded)
					{
						NetLog_Server("SSI added new %s contact '%s'", "Permit", szRecordName);
						// It wasn't previously in the list, we hide it so it only appears in the visible list
						setContactHidden(hContact, 1);
						// Add it to the list, so it can be added properly if proper contact
						AddJustAddedContact(hContact);
					}
					else
						NetLog_Server("SSI %s contact already exists '%s'", "Permit", szRecordName);

					// Save permit ID
					setSettingWord(hContact, DBSETTING_SERVLIST_PERMIT, wItemId);
					ReserveServerID(wItemId, SSIT_ITEM, 0);
					// Set apparent mode
					setSettingWord(hContact, "ApparentMode", ID_STATUS_ONLINE);
					NetLog_Server("Visible-contact (%s)", szRecordName);
				}
				else
				{ // failed to add or other error
					NetLog_Server("SSI failed to handle %s Item '%s'", "Permit", szRecordName);

          ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
				}
			}
			break;

		case SSI_ITEM_DENY:
			{
				/* Item on invisible list */
				/* wItemId not related to contact ID */
				/* pszRecordName is the UIN */
				HANDLE hContact;
				int bAdded;

				hContact = HContactFromRecordName(szRecordName, &bAdded);

				if (hContact != INVALID_HANDLE_VALUE)
				{
					if (bAdded)
					{
						/* not already on list: added */
						NetLog_Server("SSI added new %s contact '%s'", "Deny", szRecordName);
						// It wasn't previously in the list, we hide it so it only appears in the visible list
						setContactHidden(hContact, 1);
						// Add it to the list, so it can be added properly if proper contact
						AddJustAddedContact(hContact);
					}
					else
						NetLog_Server("SSI %s contact already exists '%s'", "Deny", szRecordName);

					// Save Deny ID
					setSettingWord(hContact, DBSETTING_SERVLIST_DENY, wItemId);
					ReserveServerID(wItemId, SSIT_ITEM, 0);

					// Set apparent mode
					setSettingWord(hContact, "ApparentMode", ID_STATUS_OFFLINE);
					NetLog_Server("Invisible-contact (%s)", szRecordName);
				}
				else
				{ // failed to add or other error
					NetLog_Server("SSI failed to handle %s Item '%s'", "Deny", szRecordName);

          ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
				}
			}
			break;

		case SSI_ITEM_VISIBILITY: /* My visibility settings */
			{
				BYTE bVisibility;

				// Look for visibility TLV
				if (bVisibility = pChain->getByte(SSI_TLV_VISIBILITY, 1))
				{ // found it, store the id, we do not need current visibility - we do not rely on it
					setSettingWord(NULL, DBSETTING_SERVLIST_PRIVACY, wItemId);
					ReserveServerID(wItemId, SSIT_ITEM, 0);

					NetLog_Server("Visibility is %u", bVisibility);
				}
        else
					ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
			}
			break;

		case SSI_ITEM_IGNORE: 
			{
				/* item on ignore list */
				/* wItemId not related to contact ID */
				/* pszRecordName is the UIN */
				HANDLE hContact;
				int bAdded;

				hContact = HContactFromRecordName(szRecordName, &bAdded);

				if (hContact != INVALID_HANDLE_VALUE)
				{
					if (bAdded)
					{
						/* not already on list: add */
						NetLog_Server("SSI added new %s contact '%s'", "Ignore", szRecordName);
						// It wasn't previously in the list, we hide it
						setContactHidden(hContact, 1);
						// Add it to the list, so it can be added properly if proper contact
						AddJustAddedContact(hContact);
					}
					else
						NetLog_Server("SSI %s contact already exists '%s'", "Ignore", szRecordName);

					// Save Ignore ID
					setSettingWord(hContact, DBSETTING_SERVLIST_IGNORE, wItemId);
					ReserveServerID(wItemId, SSIT_ITEM, 0);

					// Set apparent mode & ignore
					setSettingWord(hContact, "ApparentMode", ID_STATUS_OFFLINE);
					// set ignore all events
					CallService(MS_IGNORE_IGNORE, (WPARAM)hContact, IGNOREEVENT_ALL);
					NetLog_Server("Ignore-contact (%s)", szRecordName);
				}
				else
				{ // failed to add or other error
					NetLog_Server("SSI failed to handle %s Item '%s'", "Ignore", szRecordName);

          ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
				}
			}
			break;

		case SSI_ITEM_UNKNOWN2:
			NetLog_Server("SSI unknown type 0x11");

      ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
			break;

		case SSI_ITEM_IMPORTTIME:
			if (wGroupId == 0)
			{
				/* time our list was first imported */
				/* pszRecordName is "Import Time" */
				/* data is TLV(13) {TLV(D4) {time_t importTime}} */
				setSettingDword(NULL, "ImportTS", pChain->getDWord(SSI_TLV_TIMESTAMP, 1));
				setSettingWord(NULL, "SrvImportID", wItemId);
        ReserveServerID(wItemId, SSIT_ITEM, 0);
				NetLog_Server("SSI %s item recognized", "first import");
			}
			break;

		case SSI_ITEM_BUDDYICON:
			if (wGroupId == 0)
			{
				/* our avatar MD5-hash */
				/* pszRecordName is "1" */
				/* data is TLV(D5) hash */
				/* we ignore this, just save the id */
				/* cause we get the hash again after login */
				if (!strcmpnull(szRecordName, "12"))
				{ // need to handle Photo Item separately
					setSettingWord(NULL, DBSETTING_SERVLIST_PHOTO, wItemId);
					NetLog_Server("SSI %s item recognized", "Photo");
				}
				else
				{
					setSettingWord(NULL, DBSETTING_SERVLIST_AVATAR, wItemId);
					NetLog_Server("SSI %s item recognized", "Avatar");
				}
				ReserveServerID(wItemId, SSIT_ITEM, 0);
			}
			break;

    case SSI_ITEM_METAINFO:
      if (wGroupId == 0)
      {
        /* our meta info token & last update time */
        /* pszRecordName is "ICQ-MDIR" */
        /* data is TLV(15C) and TLV(15D) */
        oscar_tlv* pToken = pChain->getTLV(SSI_TLV_METAINFO_TOKEN, 1);
        oscar_tlv* pTime = pChain->getTLV(SSI_TLV_METAINFO_TIME, 1);
        if (pToken)
          setSettingBlob(NULL, DBSETTING_METAINFO_TOKEN, pToken->pData, pToken->wLen);
        if (pTime)
          setSettingDouble(NULL, DBSETTING_METAINFO_TIME, pChain->getDouble(SSI_TLV_METAINFO_TIME, 1));

        setSettingWord(NULL, DBSETTING_SERVLIST_METAINFO, wItemId);
        ReserveServerID(wItemId, SSIT_ITEM, 0);

        NetLog_Server("SSI %s item recognized", "Meta info");
      }
      break;

		case SSI_ITEM_CLIENTDATA:
			if (wGroupId == 0)
			{
				/* ICQ2k ShortcutBar Items */
				/* data is TLV(CD) text */
        if (wItemId)
          ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
			}

		default:
			NetLog_Server("SSI unhandled item %2x", wTlvType);

      if (wItemId)
        ReserveServerID(wItemId, SSIT_ITEM, SSIF_UNHANDLED);
			break;
		}

    disposeChain(&pChain);
	} // end for

  // Release Memory
  SAFE_FREE(&szActiveSrvGroup);

	NetLog_Server("Bytes left: %u", wLen);

	setSettingWord(NULL, "SrvRecordCount", (WORD)(wRecord + getSettingWord(NULL, "SrvRecordCount", 0)));

	if (bIsLastPacket)
	{
		// No contacts left to sync
		bIsSyncingCL = FALSE;

    StoreServerIDs();

		icq_RescanInfoUpdate();

		if (wLen >= 4)
		{
			DWORD dwLastUpdateTime;

			/* finally we get a time_t of the last update time */
			unpackDWord(&buf, &dwLastUpdateTime);
			setSettingDword(NULL, "SrvLastUpdate", dwLastUpdateTime);
			NetLog_Server("Last update of server list was (%u) %s", dwLastUpdateTime, time2text(dwLastUpdateTime));

			sendRosterAck();
			handleServUINSettings(wListenPort, info);

      servlistProcessLogin();
		}
		else
		{
			NetLog_Server("Last packet missed update time...");
		}
		if (getSettingWord(NULL, "SrvRecordCount", 0) == 0)
		{ // we got empty serv-list, create master group
			cookie_servlist_action* ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
			if (ack)
			{ 
				DWORD dwCookie;

				ack->dwAction = SSA_GROUP_UPDATE;
				ack->szGroupName = null_strdup("");
				dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_ADDTOLIST, 0, ack);
				icq_sendServerGroup(dwCookie, ICQ_LISTS_ADDTOLIST, 0, ack->szGroupName, NULL, 0, 0);
			}
		}
		// serv-list sync finished, clear just added contacts
		FlushJustAddedContacts(); 
	}
	else
	{
		NetLog_Server("Waiting for more packets");
	}
}


void CIcqProto::handleServerCListItemAdd(const char *szRecordName, WORD wGroupId, WORD wItemId, WORD wItemType, oscar_tlv_chain *pItemData)
{
  if (wItemType == SSI_ITEM_IMPORTTIME)
  {
    if (pItemData) 
    {
      setSettingDword(NULL, "ImportTS", pItemData->getDWord(SSI_TLV_TIMESTAMP, 1));
      setSettingWord(NULL, "SrvImportID", wItemId);
      ReserveServerID(wItemId, SSIT_ITEM, 0);

			NetLog_Server("Server added Import timestamp to list");

      return;
    }
  }
  // Reserve server-list ID
  ReserveServerID(wItemId, wItemType == SSI_ITEM_GROUP ? SSIT_GROUP : SSIT_ITEM, SSIF_UNHANDLED);
}


void CIcqProto::handleServerCListItemUpdate(const char *szRecordName, WORD wGroupId, WORD wItemId, WORD wItemType, oscar_tlv_chain *pItemData)
{
  HANDLE hContact = (wItemType == SSI_ITEM_BUDDY || wItemType == SSI_ITEM_DENY || wItemType == SSI_ITEM_PERMIT || wItemType == SSI_ITEM_IGNORE) ? HContactFromRecordName(szRecordName, NULL) : NULL;

  if (hContact != INVALID_HANDLE_VALUE && wItemType == SSI_ITEM_BUDDY)
  { // a contact was updated on server
    if (pItemData) 
    {
      oscar_tlv* pAuth = pItemData->getTLV(SSI_TLV_AWAITING_AUTH, 1);
      BYTE bAuth = getSettingByte(hContact, "Auth", 0);

      if (bAuth && !pAuth)
      { // server authorized our contact
        char str[MAX_PATH];
        char msg[MAX_PATH];
        char *nick = NickFromHandleUtf(hContact);

        setSettingByte(hContact, "Auth", 0);
        null_snprintf(str, MAX_PATH, ICQTranslateUtfStatic(LPGEN("Contact \"%s\" was authorized in the server list."), msg, MAX_PATH), nick);
        icq_LogMessage(LOG_WARNING, str);
        SAFE_FREE(&nick);
      }
      else if (!bAuth && pAuth)
      { // server took away authorization of our contact
        char str[MAX_PATH];
        char msg[MAX_PATH];
        char *nick = NickFromHandleUtf(hContact);

        setSettingByte(hContact, "Auth", 1);
        null_snprintf(str, MAX_PATH, ICQTranslateUtfStatic(LPGEN("Contact \"%s\" lost its authorization in the server list."), msg, MAX_PATH), nick);
        icq_LogMessage(LOG_WARNING, str);
        SAFE_FREE(&nick);
      }

      { // update metainfo data
        DBVARIANT dbv = {0};
        oscar_tlv *pToken = pItemData->getTLV(SSI_TLV_METAINFO_TOKEN, 1);
        oscar_tlv *pTime = pItemData->getTLV(SSI_TLV_METAINFO_TIME, 1);
                
        if (!getSetting(hContact, DBSETTING_METAINFO_TOKEN, &dbv))
        {
          if (!pToken || dbv.cpbVal != pToken->wLen || memcmp(dbv.pbVal, pToken->pData, dbv.cpbVal))
          {
            if (!pToken)
              NetLog_Server("Contact %s, meta info token removed", szRecordName);
            else
              NetLog_Server("Contact %s, meta info token changed", szRecordName);

            // user info was changed, refresh
            if (IsMetaInfoChanged(hContact))
              icq_QueueUser(hContact);
          }

          ICQFreeVariant(&dbv);
        }
        else if (pToken)
        {
          NetLog_Server("Contact %s, meta info token added", szRecordName);

          // user info was changed, refresh
          if (IsMetaInfoChanged(hContact))
            icq_QueueUser(hContact);
        }

        if (pToken)
          setSettingBlob(hContact, DBSETTING_METAINFO_TOKEN, pToken->pData, pToken->wLen);
        if (pTime)
          setSettingDouble(hContact, DBSETTING_METAINFO_TIME, pItemData->getDouble(SSI_TLV_METAINFO_TIME, 1));
      }

      { // update server's data - otherwise consequent operations can fail with 0x0E
        BYTE *data = (BYTE*)_alloca(pItemData->getChainLength());
        int datalen = getServerDataFromItemTLV(pItemData, data);

        if (datalen > 0)
          setSettingBlob(hContact, DBSETTING_SERVLIST_DATA, data, datalen);
        else
          deleteSetting(hContact, DBSETTING_SERVLIST_DATA);
      }
    }
  }
  else if (wItemType == SSI_ITEM_METAINFO)
  { // owner MetaInfo data updated
    if (pItemData)
    {
      DBVARIANT dbv = {0};
      oscar_tlv *pToken = pItemData->getTLV(SSI_TLV_METAINFO_TOKEN, 1);
      oscar_tlv *pTime = pItemData->getTLV(SSI_TLV_METAINFO_TIME, 1);

      if (!getSetting(hContact, DBSETTING_METAINFO_TOKEN, &dbv))
      {
        if (!pToken || dbv.cpbVal != pToken->wLen || memcmp(dbv.pbVal, pToken->pData, dbv.cpbVal))
        {
          if (!pToken)
            NetLog_Server("Owner meta info token removed");
          else
            NetLog_Server("Owner meta info token changed");
        }

        ICQFreeVariant(&dbv);
      }

      if (pToken)
        setSettingBlob(hContact, DBSETTING_METAINFO_TOKEN, pToken->pData, pToken->wLen);
      if (pTime)
        setSettingDouble(hContact, DBSETTING_METAINFO_TIME, pItemData->getDouble(SSI_TLV_METAINFO_TIME, 1));
    }
  }
  else if (wItemType == SSI_ITEM_GROUP)
  { // group updated
    NetLog_Server("Server updated our group \"%s\" on list", szRecordName);
  }
}


void CIcqProto::handleServerCListItemDelete(const char *szRecordName, WORD wGroupId, WORD wItemId, WORD wItemType, oscar_tlv_chain *pItemData)
{
  HANDLE hContact = (wItemType == SSI_ITEM_BUDDY || wItemType == SSI_ITEM_DENY || wItemType == SSI_ITEM_PERMIT || wItemType == SSI_ITEM_IGNORE) ? HContactFromRecordName(szRecordName, NULL) : NULL;

  if (hContact != INVALID_HANDLE_VALUE && wItemType == SSI_ITEM_BUDDY)
	{ // a contact was removed from our list
    if (getSettingWord(hContact, DBSETTING_SERVLIST_ID, 0) == wItemId)
    {
      deleteSetting(hContact, DBSETTING_SERVLIST_ID);
      deleteSetting(hContact, DBSETTING_SERVLIST_GROUP);
      deleteSetting(hContact, "Auth");

      {
        char str[MAX_PATH];
        char msg[MAX_PATH];
        char *nick = NickFromHandleUtf(hContact);

        null_snprintf(str, MAX_PATH, ICQTranslateUtfStatic(LPGEN("User \"%s\" was removed from server list."), msg, MAX_PATH), nick);
        icq_LogMessage(LOG_WARNING, str);
        SAFE_FREE(&nick);
      }
    }
  }
  // Release server-list ID
  FreeServerID(wItemId, wItemType == SSI_ITEM_GROUP ? SSIT_GROUP : SSIT_ITEM);
}


void CIcqProto::handleRecvAuthRequest(unsigned char *buf, WORD wLen)
{
	DWORD dwUin;
	uid_str szUid;
	int bAdded;

	if (!unpackUID(&buf, &wLen, &dwUin, &szUid)) return;

	if (dwUin && IsOnSpammerList(dwUin))
	{
		NetLog_Server("Ignored Message from known Spammer");
		return;
	}

	WORD wReasonLen;
	unpackWord(&buf, &wReasonLen);
	wLen -= 2;
	if (wReasonLen > wLen)
		return;

	HANDLE hContact = HContactFromUID(dwUin, szUid, &bAdded);
	CCSDATA ccs;
	PROTORECVEVENT pre;

	ccs.szProtoService = PSR_AUTH;
	ccs.hContact = hContact;
	ccs.wParam = 0;
	ccs.lParam = (LPARAM)&pre;
	pre.flags = 0;
	pre.timestamp = time(NULL);
	pre.lParam = sizeof(DWORD) + sizeof(HANDLE) + 5;
	// Prepare reason
	char *szReason = (char*)SAFE_MALLOC(wReasonLen + 1);
	int nReasonLen = 0;
	if (szReason)
	{
		memcpy(szReason, buf, wReasonLen);
		szReason[wReasonLen] = '\0';
		nReasonLen = strlennull(szReason);

		char *temp = (char*)_alloca(nReasonLen + 2);
		if (!IsUSASCII(szReason, nReasonLen) && UTF8_IsValid(szReason) && utf8_decode_static(szReason, temp, nReasonLen + 1)) 
			pre.flags |= PREF_UTF;
	}
	// Read nick name from DB
	char *szNick = NULL;
	if (dwUin)
	{
		DBVARIANT dbv = { 0 };
		if (pre.flags & PREF_UTF)
			szNick = getSettingStringUtf(hContact, "Nick", NULL);
		else if (!getSettingString(hContact, "Nick", &dbv))
		{
			szNick = null_strdup(dbv.pszVal);
			ICQFreeVariant(&dbv);
		}
	}
	else
		szNick = null_strdup(szUid);
	int nNickLen = strlennull(szNick);

	pre.lParam += nNickLen + nReasonLen;

	setSettingByte(ccs.hContact, "Grant", 1);

	/*blob is: uin(DWORD), hcontact(HANDLE), nick(ASCIIZ), first(ASCIIZ), last(ASCIIZ), email(ASCIIZ), reason(ASCIIZ)*/
	char *szBlob = (char *)_alloca(pre.lParam);
	char *pCurBlob = szBlob;
	memcpy(pCurBlob, &dwUin, sizeof(DWORD)); pCurBlob += sizeof(DWORD);
	memcpy(pCurBlob, &hContact, sizeof(HANDLE)); pCurBlob += sizeof(HANDLE);
	if (nNickLen) 
	{ // if we have nick we add it, otherwise keep trailing zero
		memcpy(pCurBlob, szNick, nNickLen);
		pCurBlob += nNickLen;
	}
	*pCurBlob = 0; pCurBlob++; // Nick
	*pCurBlob = 0; pCurBlob++; // FirstName
	*pCurBlob = 0; pCurBlob++; // LastName
	*pCurBlob = 0; pCurBlob++; // email
	if (nReasonLen)
	{
		memcpy(pCurBlob, szReason, nReasonLen);
		pCurBlob += nReasonLen;
	}
	*pCurBlob = 0; // Reason
	pre.szMessage = szBlob;

	// TODO: Change for new auth system, include all known informations
	CallService(MS_PROTO_CHAINRECV,0,(LPARAM)&ccs);

	SAFE_FREE(&szNick);
	SAFE_FREE(&szReason);
	return;
}


void CIcqProto::handleRecvAdded(unsigned char *buf, WORD wLen)
{
	DWORD dwUin;
	uid_str szUid;
	DWORD cbBlob;
	PBYTE pBlob,pCurBlob;
	int bAdded;
	char* szNick;
	int nNickLen;
	DBVARIANT dbv = {0};

	if (!unpackUID(&buf, &wLen, &dwUin, &szUid)) return;

	if (dwUin && IsOnSpammerList(dwUin))
	{
		NetLog_Server("Ignored Message from known Spammer");
		return;
	}

	HANDLE hContact = HContactFromUID(dwUin, szUid, &bAdded);

	deleteSetting(hContact, "Grant");

	cbBlob=sizeof(DWORD)+sizeof(HANDLE)+4;

	if (dwUin)
	{
		if (getSettingString(hContact, "Nick", &dbv))
			nNickLen = 0;
		else
		{
			szNick = dbv.pszVal;
			nNickLen = strlennull(szNick);
		}
	}
	else
		nNickLen = strlennull(szUid);

	cbBlob += nNickLen;

	pCurBlob=pBlob=(PBYTE)_alloca(cbBlob);
	/*blob is: uin(DWORD), hContact(HANDLE), nick(ASCIIZ), first(ASCIIZ), last(ASCIIZ), email(ASCIIZ) */
	memcpy(pCurBlob,&dwUin,sizeof(DWORD)); pCurBlob+=sizeof(DWORD);
	memcpy(pCurBlob,&hContact,sizeof(HANDLE)); pCurBlob+=sizeof(HANDLE);
	if (nNickLen && dwUin) 
	{ // if we have nick we add it, otherwise keep trailing zero
		memcpy(pCurBlob, szNick, nNickLen);
		pCurBlob+=nNickLen;
	}
	else
	{
		memcpy(pCurBlob, szUid, nNickLen);
		pCurBlob+=nNickLen;
	}
	*(char *)pCurBlob = 0; pCurBlob++;
	*(char *)pCurBlob = 0; pCurBlob++;
	*(char *)pCurBlob = 0; pCurBlob++;
	*(char *)pCurBlob = 0;
	// TODO: Change for new auth system

	AddEvent(NULL, EVENTTYPE_ADDED, time(NULL), 0, cbBlob, pBlob);
}


void CIcqProto::handleRecvAuthResponse(unsigned char *buf, WORD wLen)
{
	DWORD dwUin;
	uid_str szUid;
	char* szNick = NULL;
	WORD nReasonLen;
	char* szReason;
	int bAdded;

	BYTE bResponse = 0xFF;

	if (!unpackUID(&buf, &wLen, &dwUin, &szUid)) return;

	if (dwUin && IsOnSpammerList(dwUin))
	{
		NetLog_Server("Ignored Message from known Spammer");
		return;
	}

	HANDLE hContact = HContactFromUID(dwUin, szUid, &bAdded);

	if (hContact != INVALID_HANDLE_VALUE) szNick = NickFromHandle(hContact);

	if (wLen > 0)
	{
		unpackByte(&buf, &bResponse);
		wLen -= 1;
	}
	if (wLen >= 2)
	{
		unpackWord(&buf, &nReasonLen);
		wLen -= 2;
		if (wLen >= nReasonLen)
		{
			szReason = (char*)_alloca(nReasonLen+1);
			unpackString(&buf, szReason, nReasonLen);
			szReason[nReasonLen] = '\0';
		}
	}

	switch (bResponse)
	{

	case 0:
		NetLog_Server("Authorization request %s by %s", "denied", strUID(dwUin, szUid));
		// TODO: Add to system history as soon as new auth system is ready
		break;

	case 1:
		setSettingByte(hContact, "Auth", 0);
		NetLog_Server("Authorization request %s by %s", "granted", strUID(dwUin, szUid));
		// TODO: Add to system history as soon as new auth system is ready
		break;

	default:
		NetLog_Server("Unknown Authorization request response (%u) from %s", bResponse, strUID(dwUin, szUid));
		break;

	}
	SAFE_FREE(&szNick);
}


// Updates the visibility code used while in SSI mode. If a server ID is
// not stored in the local DB, a new ID will be added to the server list.
//
// Possible values are:
//   01 - Allow all users to see you
//   02 - Block all users from seeing you
//   03 - Allow only users in the permit list to see you
//   04 - Block only users in the invisible list from seeing you
//   05 - Allow only users in the buddy list to see you
//
void CIcqProto::updateServVisibilityCode(BYTE bCode)
{
	icq_packet packet;
	WORD wVisibilityID;
	WORD wCommand;

	if ((bCode > 0) && (bCode < 6))
	{
		cookie_servlist_action* ack;
		DWORD dwCookie;
		BYTE bVisibility = getSettingByte(NULL, "SrvVisibility", 0);

		if (bVisibility == bCode) // if no change was made, not necescary to update that
			return;
		setSettingByte(NULL, "SrvVisibility", bCode);

		// Do we have a known server visibility ID? We should, unless we just subscribed to the serv-list for the first time
		if ((wVisibilityID = getSettingWord(NULL, DBSETTING_SERVLIST_PRIVACY, 0)) == 0)
		{
			// No, create a new random ID
			wVisibilityID = GenerateServerID(SSIT_ITEM, 0);
			setSettingWord(NULL, DBSETTING_SERVLIST_PRIVACY, wVisibilityID);
			wCommand = ICQ_LISTS_ADDTOLIST;
#ifdef _DEBUG
			NetLog_Server("Made new srvVisibilityID, id is %u, code is %u", wVisibilityID, bCode);
#endif
		}
		else
		{
#ifdef _DEBUG
			NetLog_Server("Reused srvVisibilityID, id is %u, code is %u", wVisibilityID, bCode);
#endif
			wCommand = ICQ_LISTS_UPDATEGROUP;
		}

		ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
		if (!ack) 
		{
			NetLog_Server("Cookie alloc failure.");
			return; // out of memory, go away
		}
		ack->dwAction = SSA_VISIBILITY; // update visibility
		dwCookie = AllocateCookie(CKT_SERVERLIST, wCommand, 0, ack); // take cookie

		// Build and send packet
		serverPacketInit(&packet, 25);
		packFNACHeader(&packet, ICQ_LISTS_FAMILY, wCommand, 0, dwCookie);
		packWord(&packet, 0);                   // Name (null)
		packWord(&packet, 0);                   // GroupID (0 if not relevant)
		packWord(&packet, wVisibilityID);       // EntryID
		packWord(&packet, SSI_ITEM_VISIBILITY); // EntryType
		packWord(&packet, 5);                   // Length in bytes of following TLV
		packTLV(&packet, SSI_TLV_VISIBILITY, 1, &bCode);  // TLV (Visibility)
		sendServPacket(&packet);
		// There is no need to send ICQ_LISTS_CLI_MODIFYSTART or
		// ICQ_LISTS_CLI_MODIFYEND when modifying the visibility code
	}
}

// Updates the avatar hash used while in SSI mode. If a server ID is
// not stored in the local DB, a new ID will be added to the server list.
void CIcqProto::updateServAvatarHash(BYTE *pHash, int size)
{
  void** pDoubleObject = NULL;
  void* doubleObject = NULL;
  DWORD dwOperationFlags = 0;
	WORD wAvatarID;
	WORD wCommand;
	DBVARIANT dbvHash;
	int bResetHash = 0;
  char szItemName[2] = {0, 0};

	if (!getSetting(NULL, "AvatarHash", &dbvHash))
	{
		szItemName[0] = 0x30 + dbvHash.pbVal[1];

		if (memcmp(pHash, dbvHash.pbVal, 2) != 0)
		{
			/** add code to remove old hash from server */
			bResetHash = 1;
		}
		ICQFreeVariant(&dbvHash);
	}

	if (bResetHash) // start update session
  { // pair the packets (need to be send in the correct order
		dwOperationFlags |= SSOF_BEGIN_OPERATION | SSOF_END_OPERATION;
    pDoubleObject = &doubleObject;
  }

	if (bResetHash || !pHash)
	{
		cookie_servlist_action* ack;
		DWORD dwCookie;

		// Do we have a known server avatar ID?
		if (wAvatarID = getSettingWord(NULL, DBSETTING_SERVLIST_AVATAR, 0))
		{
			ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
			if (!ack) 
			{
				NetLog_Server("Cookie alloc failure.");
				return; // out of memory, go away
			}
			ack->dwAction = SSA_REMOVEAVATAR; // update avatar hash
			ack->wContactId = wAvatarID;
			dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_REMOVEFROMLIST, 0, ack); // take cookie

      icq_sendServerItem(dwCookie, ICQ_LISTS_REMOVEFROMLIST, 0, wAvatarID, szItemName, NULL, 0, SSI_ITEM_BUDDYICON, SSOP_ITEM_ACTION | dwOperationFlags, 400, pDoubleObject);
		}
	}

	if (pHash)
	{
		cookie_servlist_action* ack;
		DWORD dwCookie;
    WORD wTLVlen;
    icq_packet pBuffer;
		WORD hashsize = size - 2;

		// Do we have a known server avatar ID? We should, unless we just subscribed to the serv-list for the first time
		if (bResetHash || (wAvatarID = getSettingWord(NULL, DBSETTING_SERVLIST_AVATAR, 0)) == 0)
		{
			// No, create a new random ID
			wAvatarID = GenerateServerID(SSIT_ITEM, 0);
			wCommand = ICQ_LISTS_ADDTOLIST;
#ifdef _DEBUG
			NetLog_Server("Made new srvAvatarID, id is %u", wAvatarID);
#endif
		}
		else
		{
#ifdef _DEBUG
			NetLog_Server("Reused srvAvatarID, id is %u", wAvatarID);
#endif
			wCommand = ICQ_LISTS_UPDATEGROUP;
		}

		ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action));
		if (!ack) 
		{
			NetLog_Server("Cookie alloc failure.");
			return; // out of memory, go away
		}
		ack->dwAction = SSA_SETAVATAR; // update avatar hash
		ack->wContactId = wAvatarID;
		dwCookie = AllocateCookie(CKT_SERVERLIST, wCommand, 0, ack); // take cookie

    szItemName[0] = 0x30 + pHash[1];

  	// Build the packet
	  wTLVlen = 8 + hashsize;

	  // Initialize our handy data buffer
	  pBuffer.wPlace = 0;
	  pBuffer.pData = (BYTE *)_alloca(wTLVlen);
	  pBuffer.wLen = wTLVlen;

		packTLV(&pBuffer, SSI_TLV_NAME, 0, NULL);                    // TLV (Name)
		packTLV(&pBuffer, SSI_TLV_AVATARHASH, hashsize, pHash + 2);  // TLV (Hash)

    icq_sendServerItem(dwCookie, wCommand, 0, wAvatarID, szItemName, pBuffer.pData, wTLVlen, SSI_ITEM_BUDDYICON, SSOP_ITEM_ACTION | dwOperationFlags, 400, pDoubleObject);
		// There is no need to send ICQ_LISTS_CLI_MODIFYSTART or
		// ICQ_LISTS_CLI_MODIFYEND when modifying the avatar hash
	}
}

// Should be called before the server list is modified. When all
// modifications are done, call icq_sendServerEndOperation().
// Called automatically thru server-list update board!
void CIcqProto::icq_sendServerBeginOperation(int bImport)
{
	icq_packet packet;
	WORD wImportID = getSettingWord(NULL, "SrvImportID", 0);

	if (bImport && wImportID)
	{ // we should be importing, check if already have import item
		if (getSettingDword(NULL, "ImportTS", 0) + 604800 < getSettingDword(NULL, "LogonTS", 0))
		{ // is the timestamp week older, clear it and begin new import
			DWORD dwCookie;
			cookie_servlist_action* ack;

			if (ack = (cookie_servlist_action*)SAFE_MALLOC(sizeof(cookie_servlist_action)))
			{ // we have cookie good, go on
				ack->dwAction = SSA_IMPORT;
				dwCookie = AllocateCookie(CKT_SERVERLIST, ICQ_LISTS_REMOVEFROMLIST, 0, ack);

				icq_sendSimpleItem(dwCookie, ICQ_LISTS_REMOVEFROMLIST, 0, "ImportTime", 0, wImportID, SSI_ITEM_IMPORTTIME, SSOP_ITEM_ACTION | SSOF_SEND_DIRECTLY, 100);
			}
		}
	}

	serverPacketInit(&packet, (WORD)(bImport?14:10));
	packFNACHeader(&packet, ICQ_LISTS_FAMILY, ICQ_LISTS_CLI_MODIFYSTART);
	if (bImport) packDWord(&packet, 1<<0x10); 
	sendServPacket(&packet);
}

// Should be called after the server list has been modified to inform
// the server that we are done.
// Called automatically thru server-list update board!
void CIcqProto::icq_sendServerEndOperation()
{
	icq_packet packet;

	serverPacketInit(&packet, 10);
	packFNACHeader(&packet, ICQ_LISTS_FAMILY, ICQ_LISTS_CLI_MODIFYEND);
	sendServPacket(&packet);
}

// Sent when the last roster packet has been received
void CIcqProto::sendRosterAck(void)
{
	icq_packet packet;

	serverPacketInit(&packet, 10);
	packFNACHeader(&packet, ICQ_LISTS_FAMILY, ICQ_LISTS_GOTLIST);
	sendServPacket(&packet);

#ifdef _DEBUG
	NetLog_Server("Sent SNAC(x13,x07) - CLI_ROSTERACK");
#endif
}
