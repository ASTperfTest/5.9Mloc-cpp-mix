/*

Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2009 Miranda ICQ/IM project,
all portions of this codebase are copyrighted to the people
listed in contributors.txt.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "commonheaders.h"
#include "clc.h"

INT_PTR AddMainMenuItem(WPARAM wParam, LPARAM lParam);
INT_PTR AddContactMenuItem(WPARAM wParam, LPARAM lParam);
INT_PTR ContactChangeGroup(WPARAM wParam, LPARAM lParam);
int InitCListEvents(void);
void UninitCListEvents(void);
int ContactSettingChanged(WPARAM wParam, LPARAM lParam);
int ContactAdded(WPARAM wParam, LPARAM lParam);
int ContactDeleted(WPARAM wParam, LPARAM lParam);
INT_PTR GetContactDisplayName(WPARAM wParam, LPARAM lParam);
INT_PTR InvalidateDisplayName(WPARAM wParam, LPARAM lParam);
int InitGroupServices(void);
INT_PTR Docking_IsDocked(WPARAM wParam, LPARAM lParam);
void InitDisplayNameCache(void);
void FreeDisplayNameCache(void);
int LoadCLUIModule(void);
int InitClistHotKeys(void);

HANDLE hContactDoubleClicked, hContactIconChangedEvent;
HIMAGELIST hCListImages;
BOOL(WINAPI * MySetProcessWorkingSetSize) (HANDLE, SIZE_T, SIZE_T);

extern BYTE nameOrder[];

struct ProtoIconIndex
{
	char *szProto;
	int iIconBase;
};

OBJLIST<ProtoIconIndex> protoIconIndex(5);

static HANDLE hProtoAckHook;
static HANDLE hContactSettingChanged;

TCHAR* fnGetStatusModeDescription( int mode, int flags )
{
	static TCHAR szMode[64];
	TCHAR* descr;
	int    noPrefixReqd = 0;
	switch (mode) {
	case ID_STATUS_OFFLINE:
		descr = _T("Offline");
		noPrefixReqd = 1;
		break;
	case ID_STATUS_CONNECTING:
		descr = _T("Connecting");
		noPrefixReqd = 1;
		break;
	case ID_STATUS_ONLINE:
		descr = _T("Online");
		noPrefixReqd = 1;
		break;
	case ID_STATUS_AWAY:
		descr = _T("Away");
		break;
	case ID_STATUS_DND:
		descr = _T("DND");
		break;
	case ID_STATUS_NA:
		descr = _T("NA");
		break;
	case ID_STATUS_OCCUPIED:
		descr = _T("Occupied");
		break;
	case ID_STATUS_FREECHAT:
		descr = _T("Free for chat");
		break;
	case ID_STATUS_INVISIBLE:
		descr = _T("Invisible");
		break;
	case ID_STATUS_OUTTOLUNCH:
		descr = _T("Out to lunch");
		break;
	case ID_STATUS_ONTHEPHONE:
		descr = _T("On the phone");
		break;
	case ID_STATUS_IDLE:
		descr = _T("Idle");
		break;
	default:
		if (mode > ID_STATUS_CONNECTING && mode < ID_STATUS_CONNECTING + MAX_CONNECT_RETRIES) {
			const TCHAR* connFmt = _T("Connecting (attempt %d)");
			mir_sntprintf(szMode, SIZEOF(szMode), (flags&GSMDF_UNTRANSLATED)?connFmt:TranslateTS(connFmt), mode - ID_STATUS_CONNECTING + 1);
			return szMode;
		}
		return NULL;
	}
	if (noPrefixReqd || !(flags & GSMDF_PREFIXONLINE))
		return ( flags & GSMDF_UNTRANSLATED ) ? descr : TranslateTS( descr );

	lstrcpy( szMode, TranslateT( "Online" ));
	lstrcat( szMode, _T(": "));
	lstrcat( szMode, ( flags & GSMDF_UNTRANSLATED ) ? descr : TranslateTS( descr ));
	return szMode;
}

static INT_PTR GetStatusModeDescription(WPARAM wParam, LPARAM lParam)
{
	TCHAR* buf1 = cli.pfnGetStatusModeDescription( wParam, lParam );

	#ifdef UNICODE
		if ( !( lParam & GCMDF_TCHAR ))
		{
			static char szMode[64];
			char *buf2 = mir_u2a(buf1);
			mir_snprintf(szMode, SIZEOF(szMode), "%s", buf2);
			mir_free(buf2);
			return (INT_PTR)szMode;
		}
	#endif

	return (INT_PTR)buf1;
}

static int ProtocolAck(WPARAM, LPARAM lParam)
{
	ACKDATA *ack = (ACKDATA *) lParam;

	if (ack->type != ACKTYPE_STATUS)
		return 0;
	CallService(MS_CLUI_PROTOCOLSTATUSCHANGED, ack->lParam, (LPARAM) ack->szModule);

	if ((int) ack->hProcess < ID_STATUS_ONLINE && ack->lParam >= ID_STATUS_ONLINE) {
		DWORD caps;
		caps = (DWORD) CallProtoService(ack->szModule, PS_GETCAPS, PFLAGNUM_1, 0);
		if (caps & PF1_SERVERCLIST) {
			HANDLE hContact;
			char *szProto;

			hContact = (HANDLE) CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);
			while (hContact) {
				szProto = (char *) CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0);
				if (szProto != NULL && !strcmp(szProto, ack->szModule))
					if (DBGetContactSettingByte(hContact, "CList", "Delete", 0))
						CallService(MS_DB_CONTACT_DELETE, (WPARAM) hContact, 0);
				hContact = (HANDLE) CallService(MS_DB_CONTACT_FINDNEXT, (WPARAM) hContact, 0);
	}	}	}

	cli.pfnTrayIconUpdateBase(ack->szModule);
	return 0;
}

HICON fnGetIconFromStatusMode( HANDLE hContact, const char *szProto, int status )
{
	return ImageList_GetIcon( hCListImages, cli.pfnIconFromStatusMode( szProto, status, hContact ), ILD_NORMAL);
}

int fnIconFromStatusMode(const char *szProto, int status, HANDLE )
{
	int index, i;

	for ( index = 0; index < SIZEOF(statusModeList); index++ )
		if ( status == statusModeList[index] )
			break;

	if ( index == SIZEOF(statusModeList))
		index = 0;
	if (szProto == NULL)
		return index + 1;
	for ( i = 0; i < protoIconIndex.getCount(); i++ ) {
		if (strcmp(szProto, protoIconIndex[i].szProto) == 0)
		    return protoIconIndex[i].iIconBase + index;
	}
	return 1;
}

static INT_PTR GetContactIcon(WPARAM wParam, LPARAM)
{
	char *szProto = (char *) CallService(MS_PROTO_GETCONTACTBASEPROTO, wParam, 0);
	HANDLE hContact = (HANDLE)wParam;

	return cli.pfnIconFromStatusMode(szProto,
		szProto == NULL ? ID_STATUS_OFFLINE : DBGetContactSettingWord(hContact, szProto, "Status", ID_STATUS_OFFLINE), hContact);
}

static void AddProtoIconIndex( PROTOACCOUNT* pa )
{
    ProtoIconIndex *pii = new ProtoIconIndex;
    pii->szProto = pa->szModuleName;
	for (int i = 0; i < SIZEOF(statusModeList); i++) {
		int iImg = ImageList_AddIcon_ProtoIconLibLoaded(hCListImages, pa->szModuleName, statusModeList[i] );
		if (i == 0)
			pii->iIconBase = iImg;
	}
    protoIconIndex.insert(pii);
}

static void RemoveProtoIconIndex( PROTOACCOUNT* pa )
{
	for (int i = 0; i < protoIconIndex.getCount(); i++) 
        if (strcmp(protoIconIndex[i].szProto, pa->szModuleName) == 0) {
            protoIconIndex.remove(i);
            break;
        }
}

static int ContactListModulesLoaded(WPARAM, LPARAM)
{
	if ( !ServiceExists( MS_DB_CONTACT_GETSETTING_STR )) {
		MessageBox( NULL, TranslateT( "This plugin requires db3x plugin version 0.5.1.0 or later" ), _T("CList"), MB_OK );
		return 1;
	}

	RebuildMenuOrder();
	for (int i = 0; i < accounts.getCount(); i++)
		AddProtoIconIndex( accounts[i] );

	cli.pfnLoadContactTree();

	LoadCLUIModule();

	InitClistHotKeys();

	return 0;
}

static int ContactListAccountsChanged( WPARAM eventCode, LPARAM lParam )
{
    switch (eventCode) 
    { 
    case PRAC_ADDED:
        AddProtoIconIndex(( PROTOACCOUNT* )lParam );
        break;

    case PRAC_REMOVED:
        RemoveProtoIconIndex(( PROTOACCOUNT* )lParam );
        break;
    }
    cli.pfnReloadProtoMenus();
	cli.pfnTrayIconIconsChanged();
	cli.pfnClcBroadcast( INTM_RELOADOPTIONS, 0, 0 );
	cli.pfnClcBroadcast( INTM_INVALIDATE, 0, 0 );
	return 0;
}

static INT_PTR ContactDoubleClicked(WPARAM wParam, LPARAM)
{
	// Try to process event myself
	if ( cli.pfnEventsProcessContactDoubleClick(( HANDLE )wParam ) == 0 )
		return 0;

	// Allow third-party plugins to process a dblclick
	if ( NotifyEventHooks( hContactDoubleClicked, wParam, 0 ))
		return 0;

	// Otherwise try to execute the default action
	TryProcessDoubleClick(( HANDLE )wParam );
	return 0;
}

static INT_PTR GetIconsImageList(WPARAM, LPARAM)
{
	return (INT_PTR)hCListImages;
}

static INT_PTR ContactFilesDropped(WPARAM wParam, LPARAM lParam)
{
	CallService(MS_FILE_SENDSPECIFICFILES, wParam, lParam);
	return 0;
}

static int CListIconsChanged(WPARAM, LPARAM)
{
	int i, j;

	for (i = 0; i < SIZEOF(statusModeList); i++)
		ImageList_ReplaceIcon_IconLibLoaded(hCListImages, i + 1, LoadSkinIcon( skinIconStatusList[i] ));
	ImageList_ReplaceIcon_IconLibLoaded(hCListImages, IMAGE_GROUPOPEN, LoadSkinIcon( SKINICON_OTHER_GROUPOPEN ));
	ImageList_ReplaceIcon_IconLibLoaded(hCListImages, IMAGE_GROUPSHUT, LoadSkinIcon( SKINICON_OTHER_GROUPSHUT ));
	for (i = 0; i < protoIconIndex.getCount(); i++)
		for (j = 0; j < SIZEOF(statusModeList); j++)
			ImageList_ReplaceIcon_IconLibLoaded(hCListImages, protoIconIndex[i].iIconBase + j, LoadSkinProtoIcon(protoIconIndex[i].szProto, statusModeList[j] ));
	cli.pfnTrayIconIconsChanged();
	cli.pfnInvalidateRect( cli.hwndContactList, NULL, TRUE);
	return 0;
}

/*
Begin of Hrk's code for bug
*/
#define GWVS_HIDDEN 1
#define GWVS_VISIBLE 2
#define GWVS_COVERED 3
#define GWVS_PARTIALLY_COVERED 4

int fnGetWindowVisibleState(HWND hWnd, int iStepX, int iStepY)
{
	RECT rc, rcWin, rcWorkArea;
	POINT pt;
	register int i, j, width, height, iCountedDots = 0, iNotCoveredDots = 0;
	BOOL bPartiallyCovered = FALSE;
	HWND hAux = 0;

	if (hWnd == NULL) {
		SetLastError(0x00000006);       //Wrong handle
		return -1;
	}
	//Some defaults now. The routine is designed for thin and tall windows.
	if (iStepX <= 0)
		iStepX = 4;
	if (iStepY <= 0)
		iStepY = 16;

	if (IsIconic(hWnd) || !IsWindowVisible(hWnd))
		return GWVS_HIDDEN;
	else 
	{
		if (CallService(MS_CLIST_DOCKINGISDOCKED, 0, 0))
			return GWVS_VISIBLE;

		GetWindowRect(hWnd, &rcWin);

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, FALSE);
		if (MyMonitorFromWindow)
		{
			HMONITOR hMon = MyMonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			if (MyGetMonitorInfo(hMon, &mi))
				rcWorkArea = mi.rcWork;
		}

		IntersectRect(&rc, &rcWin, &rcWorkArea);

		width = rc.right - rc.left;
		height = rc.bottom - rc.top;

		for (i = rc.top; i < rc.bottom; i += (height / iStepY)) {
			pt.y = i;
			for (j = rc.left; j < rc.right; j += (width / iStepX)) {
				pt.x = j;
				hAux = WindowFromPoint(pt);
				while (GetParent(hAux) != NULL)
					hAux = GetParent(hAux);
				if (hAux != hWnd && hAux != NULL)       //There's another window!
					bPartiallyCovered = TRUE;
				else
					iNotCoveredDots++;  //Let's count the not covered dots.
				iCountedDots++; //Let's keep track of how many dots we checked.
			}
		}
		if (iNotCoveredDots == iCountedDots)    //Every dot was not covered: the window is visible.
			return GWVS_VISIBLE;
		else if (iNotCoveredDots == 0)  //They're all covered!
			return GWVS_COVERED;
		else                    //There are dots which are visible, but they are not as many as the ones we counted: it's partially covered.
			return GWVS_PARTIALLY_COVERED;
	}
}

int fnShowHide(WPARAM, LPARAM)
{
	BOOL bShow = FALSE;

	int iVisibleState = cli.pfnGetWindowVisibleState(cli.hwndContactList, 0, 0);

	//bShow is FALSE when we enter the switch.
	switch (iVisibleState) {
	case GWVS_PARTIALLY_COVERED:
		//If we don't want to bring it to top, we can use a simple break. This goes against readability ;-) but the comment explains it.
		if (!DBGetContactSettingByte(NULL, "CList", "BringToFront", SETTING_BRINGTOFRONT_DEFAULT))
			break;
	case GWVS_COVERED:     //Fall through (and we're already falling)
	case GWVS_HIDDEN:
		bShow = TRUE;
		break;
	case GWVS_VISIBLE:     //This is not needed, but goes for readability.
		bShow = FALSE;
		break;
	case -1:               //We can't get here, both cli.hwndContactList and iStepX and iStepY are right.
		return 0;
	}
	if (bShow == TRUE) {
		RECT rcWindow;

		ShowWindow(cli.hwndContactList, SW_RESTORE);
		if (!DBGetContactSettingByte(NULL, "CList", "OnTop", SETTING_ONTOP_DEFAULT))
			SetWindowPos(cli.hwndContactList, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		else
			SetWindowPos(cli.hwndContactList, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		SetForegroundWindow(cli.hwndContactList);
		DBWriteContactSettingByte(NULL, "CList", "State", SETTING_STATE_NORMAL);

		//this forces the window onto the visible screen
		GetWindowRect(cli.hwndContactList, &rcWindow);
		if (Utils_AssertInsideScreen(&rcWindow) == 1)
		{
			MoveWindow(cli.hwndContactList, rcWindow.left, rcWindow.top, 
				rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
		}
	}
	else {                      //It needs to be hidden
		if (DBGetContactSettingByte(NULL, "CList", "ToolWindow", SETTING_TOOLWINDOW_DEFAULT) || 
			DBGetContactSettingByte(NULL, "CList", "Min2Tray", SETTING_MIN2TRAY_DEFAULT))
		{
			ShowWindow(cli.hwndContactList, SW_HIDE);
			DBWriteContactSettingByte(NULL, "CList", "State", SETTING_STATE_HIDDEN);
		}
		else
		{
			ShowWindow(cli.hwndContactList, SW_MINIMIZE);
			DBWriteContactSettingByte(NULL, "CList", "State", SETTING_STATE_MINIMIZED);
		}

		if (MySetProcessWorkingSetSize != NULL && DBGetContactSettingByte(NULL, "CList", "DisableWorkingSet", 1))
			MySetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// old evil code. hopefully it will be deleted soon, cause nobody uses it now

#define SAFESTRING(a) a?a:""

int GetStatusModeOrdering(int statusMode);
extern int sortByStatus, sortByProto;

static INT_PTR CompareContacts( WPARAM wParam, LPARAM lParam )
{
	HANDLE a = (HANDLE) wParam, b = (HANDLE) lParam;
	TCHAR namea[128], *nameb;
	int statusa, statusb;
	char *szProto1, *szProto2;
	int rc;

	szProto1 = (char *) CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) a, 0);
	szProto2 = (char *) CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) b, 0);
	statusa = DBGetContactSettingWord((HANDLE) a, SAFESTRING(szProto1), "Status", ID_STATUS_OFFLINE);
	statusb = DBGetContactSettingWord((HANDLE) b, SAFESTRING(szProto2), "Status", ID_STATUS_OFFLINE);

	if (sortByProto) {
		/* deal with statuses, online contacts have to go above offline */
		if ((statusa == ID_STATUS_OFFLINE) != (statusb == ID_STATUS_OFFLINE)) {
			return 2 * (statusa == ID_STATUS_OFFLINE) - 1;
		}
		/* both are online, now check protocols */
		rc = strcmp(SAFESTRING(szProto1), SAFESTRING(szProto2));        /* strcmp() doesn't like NULL so feed in "" as needed */
		if (rc != 0 && (szProto1 != NULL && szProto2 != NULL))
			return rc;
		/* protocols are the same, order by display name */
	}

	if (sortByStatus) {
		int ordera, orderb;
		ordera = GetStatusModeOrdering(statusa);
		orderb = GetStatusModeOrdering(statusb);
		if (ordera != orderb)
			return ordera - orderb;
	}
	else {
		//one is offline: offline goes below online
		if ((statusa == ID_STATUS_OFFLINE) != (statusb == ID_STATUS_OFFLINE)) {
			return 2 * (statusa == ID_STATUS_OFFLINE) - 1;
		}
	}

	nameb = cli.pfnGetContactDisplayName( a, 0);
	_tcsncpy(namea, nameb, SIZEOF(namea));
	namea[ SIZEOF(namea)-1 ] = 0;
	nameb = cli.pfnGetContactDisplayName( b, 0);

	//otherwise just compare names
	return _tcsicmp(namea, nameb);
}

/***************************************************************************************/

static INT_PTR TrayIconProcessMessageStub( WPARAM wParam, LPARAM lParam ) {	return cli.pfnTrayIconProcessMessage( wParam, lParam ); }
static INT_PTR TrayIconPauseAutoHideStub( WPARAM wParam, LPARAM lParam ) { return cli.pfnTrayIconPauseAutoHide( wParam, lParam ); }
static INT_PTR ShowHideStub( WPARAM wParam, LPARAM lParam ) { return cli.pfnShowHide( wParam, lParam ); }
static INT_PTR SetHideOfflineStub( WPARAM wParam, LPARAM lParam ) { return cli.pfnSetHideOffline( wParam, lParam ); }
static INT_PTR Docking_ProcessWindowMessageStub( WPARAM wParam, LPARAM lParam ) { return cli.pfnDocking_ProcessWindowMessage( wParam, lParam ); }
static INT_PTR HotkeysProcessMessageStub( WPARAM wParam, LPARAM lParam ) { return cli.pfnHotkeysProcessMessage( wParam, lParam ); }

int LoadContactListModule2(void)
{
	HookEvent(ME_SYSTEM_MODULESLOADED, ContactListModulesLoaded);
	HookEvent(ME_PROTO_ACCLISTCHANGED, ContactListAccountsChanged);
	hContactSettingChanged = HookEvent(ME_DB_CONTACT_SETTINGCHANGED, ContactSettingChanged);
	HookEvent(ME_DB_CONTACT_ADDED, ContactAdded);
	HookEvent(ME_DB_CONTACT_DELETED, ContactDeleted);
	hProtoAckHook = (HANDLE) HookEvent(ME_PROTO_ACK, ProtocolAck);
	hContactDoubleClicked = CreateHookableEvent(ME_CLIST_DOUBLECLICKED);
	hContactIconChangedEvent = CreateHookableEvent(ME_CLIST_CONTACTICONCHANGED);
	CreateServiceFunction(MS_CLIST_CONTACTDOUBLECLICKED, ContactDoubleClicked);
	CreateServiceFunction(MS_CLIST_CONTACTFILESDROPPED, ContactFilesDropped);
	CreateServiceFunction(MS_CLIST_GETSTATUSMODEDESCRIPTION, GetStatusModeDescription);
	CreateServiceFunction(MS_CLIST_GETCONTACTDISPLAYNAME, GetContactDisplayName);
	CreateServiceFunction(MS_CLIST_INVALIDATEDISPLAYNAME, InvalidateDisplayName);
	CreateServiceFunction(MS_CLIST_TRAYICONPROCESSMESSAGE, TrayIconProcessMessageStub );
	CreateServiceFunction(MS_CLIST_PAUSEAUTOHIDE, TrayIconPauseAutoHideStub);
	CreateServiceFunction(MS_CLIST_CONTACTSCOMPARE, CompareContacts);
	CreateServiceFunction(MS_CLIST_CONTACTCHANGEGROUP, ContactChangeGroup);
	CreateServiceFunction(MS_CLIST_SHOWHIDE, ShowHideStub);
	CreateServiceFunction(MS_CLIST_SETHIDEOFFLINE, SetHideOfflineStub);
	CreateServiceFunction(MS_CLIST_DOCKINGPROCESSMESSAGE, Docking_ProcessWindowMessageStub);
	CreateServiceFunction(MS_CLIST_DOCKINGISDOCKED, Docking_IsDocked);
	CreateServiceFunction(MS_CLIST_HOTKEYSPROCESSMESSAGE, HotkeysProcessMessageStub);
	CreateServiceFunction(MS_CLIST_GETCONTACTICON, GetContactIcon);
	MySetProcessWorkingSetSize = (BOOL(WINAPI *) (HANDLE, SIZE_T, SIZE_T)) GetProcAddress(GetModuleHandleA("kernel32"), "SetProcessWorkingSetSize");
	InitDisplayNameCache();
	InitCListEvents();
	InitGroupServices();
	cli.pfnInitTray();

	hCListImages = ImageList_Create(16, 16, ILC_MASK | (IsWinVerXPPlus()? ILC_COLOR32 : ILC_COLOR16), 13, 0);
	HookEvent(ME_SKIN_ICONSCHANGED, CListIconsChanged);
	CreateServiceFunction(MS_CLIST_GETICONSIMAGELIST, GetIconsImageList);

	ImageList_AddIcon_NotShared(hCListImages, MAKEINTRESOURCE(IDI_BLANK));

	{
		int i;
		//now all core skin icons are loaded via icon lib. so lets release them
		for (i = 0; i < SIZEOF(statusModeList); i++)
			ImageList_AddIcon_IconLibLoaded(hCListImages, skinIconStatusList[i] );
	}

	//see IMAGE_GROUP... in clist.h if you add more images above here
	ImageList_AddIcon_IconLibLoaded(hCListImages, SKINICON_OTHER_GROUPOPEN );
	ImageList_AddIcon_IconLibLoaded(hCListImages, SKINICON_OTHER_GROUPSHUT );
	return 0;
}

void UnloadContactListModule()
{
	if ( hCListImages ) {
		//remove transitory contacts
		HANDLE hContact = (HANDLE) CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);
		while (hContact != NULL) {
			HANDLE hNext = (HANDLE) CallService(MS_DB_CONTACT_FINDNEXT, (WPARAM) hContact, 0);
			if (DBGetContactSettingByte(hContact, "CList", "NotOnList", 0))
				CallService(MS_DB_CONTACT_DELETE, (WPARAM) hContact, 0);
			hContact = hNext;
		}
		ImageList_Destroy(hCListImages);
		UnhookEvent(hProtoAckHook);
		UninitCListEvents();
		protoIconIndex.destroy();
		DestroyHookableEvent(hContactDoubleClicked);
		UnhookEvent(hContactSettingChanged);
}	}
