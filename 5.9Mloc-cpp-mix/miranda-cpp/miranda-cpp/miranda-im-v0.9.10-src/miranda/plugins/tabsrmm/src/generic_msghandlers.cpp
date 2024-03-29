/*
 * astyle --force-indent=tab=4 --brackets=linux --indent-switches
 *		  --pad=oper --one-line=keep-blocks  --unpad=paren
 *
 * Miranda IM: the free IM client for Microsoft* Windows*
 *
 * Copyright 2000-2009 Miranda ICQ/IM project,
 * all portions of this codebase are copyrighted to the people
 * listed in contributors.txt.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * you should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * part of tabSRMM messaging plugin for Miranda.
 *
 * (C) 2005-2010 by silvercircle _at_ gmail _dot_ com and contributors
 *
 * $Id: generic_msghandlers.cpp 12979 2010-10-17 22:48:09Z silvercircle $
 *
 * these are generic message handlers which are used by the message dialog window procedure.
 * calling them directly instead of using SendMessage() is faster.
 * also contains various callback functions for custom buttons
 */


#include "commonheaders.h"

extern RECT	  			rcLastStatusBarClick;


/**
 * Save message log for given session as RTF document
 */
void TSAPI DM_SaveLogAsRTF(const TWindowData* dat)
{
	TCHAR szFilename[MAX_PATH];
	OPENFILENAME ofn = {0};
	EDITSTREAM stream = { 0 };
	TCHAR szFilter[MAX_PATH];

	if (dat && dat->hwndIEView != 0) {
		IEVIEWEVENT event = {0};

		event.cbSize = sizeof(IEVIEWEVENT);
		event.hwnd = dat->hwndIEView;
		event.hContact = dat->hContact;
		event.iType = IEE_SAVE_DOCUMENT;
		event.dwFlags = 0;
		event.count = 0;
		event.codepage = 0;
		CallService(MS_IEVIEW_EVENT, 0, (LPARAM)&event);
	} else if(dat) {
		TCHAR  szInitialDir[MAX_PATH + 2];

		_tcsncpy(szFilter, _T("Rich Edit file\0*.rtf\0\0"), MAX_PATH);
		mir_sntprintf(szFilename, MAX_PATH, _T("%s.rtf"), dat->cache->getNick());

		Utils::sanitizeFilename(szFilename);

		mir_sntprintf(szInitialDir, MAX_PATH, _T("%s%s\\"), M->getDataPath(), _T("\\Saved message logs"));
		CallService(MS_UTILS_CREATEDIRTREET, 0, (LPARAM)szInitialDir);
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = dat->hwnd;
		ofn.lpstrFile = szFilename;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrInitialDir = szInitialDir;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_HIDEREADONLY;
		ofn.lpstrDefExt = _T("rtf");
		if (GetSaveFileName(&ofn)) {
			stream.dwCookie = (DWORD_PTR)szFilename;
			stream.dwError = 0;
			stream.pfnCallback = Utils::StreamOut;
			SendDlgItemMessage(dat->hwnd, dat->bType == SESSIONTYPE_IM ? IDC_LOG : IDC_CHAT_LOG, EM_STREAMOUT, SF_RTF | SF_USECODEPAGE, (LPARAM) & stream);
		}
	}
}

/**
 * This is broadcasted by the container to all child windows to check if the
 * container can be autohidden or -closed.
 *
 * wParam is the autohide timeout (in seconds)
 * lParam points to a BOOL and a session which wants to prevent auto-hiding
 * the container must set it to FALSE.
 *
 * If no session in the container disagrees, the container will be hidden.
 */
void TSAPI DM_CheckAutoHide(const TWindowData* dat, WPARAM wParam, LPARAM lParam)
{
	if(dat && lParam) {
		BOOL	*fResult = (BOOL *)lParam;

		if(GetWindowTextLengthA(GetDlgItem(dat->hwnd, dat->bType == SESSIONTYPE_IM ? IDC_MESSAGE : IDC_CHAT_MESSAGE)) > 0) {
			*fResult = FALSE;
			return;				// text entered in the input area -> prevent autohide/cose
		}
		if(dat->dwUnread) {
			*fResult = FALSE;
			return;				// unread events, do not hide or close the container
		}
		if(((GetTickCount() - dat->dwLastActivity) / 1000) <= wParam)
			*fResult = FALSE;		// time since last activity did not yet reach the threshold.
	}
}
/**
 * checks if the balloon tooltip can be dismissed (usually called by
 * WM_MOUSEMOVE events
 */

void TSAPI DM_DismissTip(TWindowData *dat, const POINT& pt)
{
	RECT rc;

	if(!IsWindowVisible(dat->hwndTip))
		return;

	GetWindowRect(dat->hwndTip, &rc);
	if(PtInRect(&rc, pt))
		return;

	if(abs(pt.x - dat->ptTipActivation.x) > 5 || abs(pt.y - dat->ptTipActivation.y) > 5) {
		SendMessage(dat->hwndTip, TTM_TRACKACTIVATE, FALSE, 0);
		dat->ptTipActivation.x = dat->ptTipActivation.y = 0;
	}
}

/**
 * initialize the balloon tooltip for message window notifications
 */
void TSAPI DM_InitTip(TWindowData *dat)
{
	dat->hwndTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_BALLOON, CW_USEDEFAULT, CW_USEDEFAULT,
								  CW_USEDEFAULT, CW_USEDEFAULT, dat->hwnd, NULL, g_hInst, (LPVOID) NULL);

	ZeroMemory((void *)&dat->ti, sizeof(dat->ti));
	dat->ti.cbSize = sizeof(dat->ti);
	dat->ti.lpszText = PluginConfig.m_szNoStatus;
	dat->ti.hinst = g_hInst;
	dat->ti.hwnd = dat->hwnd;
	dat->ti.uFlags = TTF_TRACK | TTF_IDISHWND | TTF_TRANSPARENT;
	dat->ti.uId = (UINT_PTR)dat->hwnd;
	SendMessageA(dat->hwndTip, TTM_ADDTOOLA, 0, (LPARAM)&dat->ti);
	SetWindowPos(dat->hwndTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
}

/**
 * checks generic hotkeys valid for both IM and MUC sessions
 *
 * returns 1 for handled hotkeys, 0 otherwise.
 */
LRESULT TSAPI DM_GenericHotkeysCheck(MSG *message, TWindowData *dat)
{
	LRESULT mim_hotkey_check = CallService(MS_HOTKEY_CHECK, (WPARAM)message, (LPARAM)(TABSRMM_HK_SECTION_GENERIC));
	HWND	hwndDlg = dat->hwnd;

	switch(mim_hotkey_check) {
		case TABSRMM_HK_PASTEANDSEND:
			HandlePasteAndSend(dat);
			return(1);
		case TABSRMM_HK_HISTORY:
			SendMessage(hwndDlg, WM_COMMAND, IDC_HISTORY, 0);
			return(1);
		case TABSRMM_HK_CONTAINEROPTIONS:
			if (dat->pContainer->hWndOptions == 0)
				CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_CONTAINEROPTIONS), dat->pContainer->hwnd,
								  DlgProcContainerOptions, (LPARAM)dat->pContainer);
			return(1);
		case TABSRMM_HK_SEND:
			if (!(GetWindowLongPtr(GetDlgItem(hwndDlg, IDC_MESSAGE), GWL_STYLE) & ES_READONLY)) {
				PostMessage(hwndDlg, WM_COMMAND, IDOK, 0);
				return(1);
			}
			break;
		case TABSRMM_HK_TOGGLEINFOPANEL:
			dat->Panel->setActive(dat->Panel->isActive() ? FALSE : TRUE);
			dat->Panel->showHide();
			return(1);
		case TABSRMM_HK_EMOTICONS:
			SendMessage(hwndDlg, WM_COMMAND, IDC_SMILEYBTN, 0);
			return(1);
		case TABSRMM_HK_TOGGLETOOLBAR:
			SendMessage(hwndDlg, WM_COMMAND, IDC_TOGGLETOOLBAR, 0);
			return(1);
		case TABSRMM_HK_CLEARLOG:
			ClearLog(dat);
			return(1);
		case TABSRMM_HK_TOGGLESIDEBAR:
			if(dat->pContainer->SideBar->isActive())
				SendMessage(hwndDlg, WM_COMMAND, IDC_TOGGLESIDEBAR, 0);
			return(1);
		default:
			break;
	}
	return(0);
}

LRESULT TSAPI DM_MsgWindowCmdHandler(HWND hwndDlg, TContainerData *m_pContainer, TWindowData *dat, UINT cmd, WPARAM wParam, LPARAM lParam)
{
	HWND	hwndContainer = m_pContainer->hwnd;

	switch(cmd) {
		case IDC_FONTBOLD:
		case IDC_FONTITALIC:
		case IDC_FONTUNDERLINE:
		case IDC_FONTSTRIKEOUT:	{
			CHARFORMAT2 cf, cfOld;
			int cmd = LOWORD(wParam);
			BOOL isBold, isItalic, isUnderline, isStrikeout;

			if (dat->SendFormat == 0)           // dont use formatting if disabled
				break;

			ZeroMemory(&cf, sizeof(CHARFORMAT2));
			ZeroMemory(&cfOld, sizeof(CHARFORMAT2));
			cfOld.cbSize = cf.cbSize = sizeof(CHARFORMAT2);
			cfOld.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
			SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfOld);
			isBold = (cfOld.dwEffects & CFE_BOLD) && (cfOld.dwMask & CFM_BOLD);
			isItalic = (cfOld.dwEffects & CFE_ITALIC) && (cfOld.dwMask & CFM_ITALIC);
			isUnderline = (cfOld.dwEffects & CFE_UNDERLINE) && (cfOld.dwMask & CFM_UNDERLINE);
			isStrikeout = (cfOld.dwEffects & CFM_STRIKEOUT) && (cfOld.dwMask & CFM_STRIKEOUT);

			if (cmd == IDC_FONTBOLD && !IsWindowEnabled(GetDlgItem(hwndDlg, IDC_FONTBOLD)))
				break;
			if (cmd == IDC_FONTITALIC && !IsWindowEnabled(GetDlgItem(hwndDlg, IDC_FONTITALIC)))
				break;
			if (cmd == IDC_FONTUNDERLINE && !IsWindowEnabled(GetDlgItem(hwndDlg, IDC_FONTUNDERLINE)))
				break;
			if (cmd == IDC_FONTSTRIKEOUT && !IsWindowEnabled(GetDlgItem(hwndDlg, IDC_FONTSTRIKEOUT)))
				break;
			if (cmd == IDC_FONTBOLD) {
				cf.dwEffects = isBold ? 0 : CFE_BOLD;
				cf.dwMask = CFM_BOLD;
				CheckDlgButton(hwndDlg, IDC_FONTBOLD, !isBold);
			} else if (cmd == IDC_FONTITALIC) {
				cf.dwEffects = isItalic ? 0 : CFE_ITALIC;
				cf.dwMask = CFM_ITALIC;
				CheckDlgButton(hwndDlg, IDC_FONTITALIC, !isItalic);
			} else if (cmd == IDC_FONTUNDERLINE) {
				cf.dwEffects = isUnderline ? 0 : CFE_UNDERLINE;
				cf.dwMask = CFM_UNDERLINE;
				CheckDlgButton(hwndDlg, IDC_FONTUNDERLINE, !isUnderline);
			} else if (cmd == IDC_FONTSTRIKEOUT) {
				cf.dwEffects = isStrikeout ? 0 : CFM_STRIKEOUT;
				cf.dwMask = CFM_STRIKEOUT;
				CheckDlgButton(hwndDlg, IDC_FONTSTRIKEOUT, !isStrikeout);
			}
			SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
			break;
		}
		case IDC_FONTFACE: {
			HMENU submenu = GetSubMenu(m_pContainer->hMenuContext, 7);
			RECT rc;
			int iSelection, i;
			CHARFORMAT2 cf;

			ZeroMemory(&cf, sizeof(CHARFORMAT2));
			cf.cbSize = sizeof(CHARFORMAT2);
			cf.dwMask = CFM_COLOR;
			cf.dwEffects = 0;

			GetWindowRect(GetDlgItem(hwndDlg, IDC_FONTFACE), &rc);
			iSelection = TrackPopupMenu(submenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);
			if (iSelection == ID_FONT_CLEARALLFORMATTING) {
				cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
				cf.crTextColor = M->GetDword(FONTMODULE, "Font16Col", 0);
				SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
				break;
			}
			if (iSelection == ID_FONT_DEFAULTCOLOR) {
				int i = 0;
				cf.crTextColor = M->GetDword(FONTMODULE, "Font16Col", 0);
				for (i = 0; i < Utils::rtf_ctable_size; i++) {
					if (Utils::rtf_ctable[i].clr == cf.crTextColor)
						cf.crTextColor = RGB(GetRValue(cf.crTextColor), GetGValue(cf.crTextColor), GetBValue(cf.crTextColor) == 0 ? GetBValue(cf.crTextColor) + 1 : GetBValue(cf.crTextColor) - 1);
				}
				SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
				break;
			}
			for (i = 0; i < RTF_CTABLE_DEFSIZE; i++) {
				if (Utils::rtf_ctable[i].menuid == iSelection) {
					cf.crTextColor = Utils::rtf_ctable[i].clr;
					SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
				}
			}
			break;
		}

		case IDCANCEL: {
			ShowWindow(hwndContainer, SW_MINIMIZE);
			return FALSE;
		}

		case IDC_SAVE:
			SendMessage(hwndDlg, WM_CLOSE, 1, 0);
			break;

		case IDC_NAME: {
			if (GetKeyState(VK_SHIFT) & 0x8000)   // copy UIN
				SendMessage(hwndDlg, DM_UINTOCLIPBOARD, 0, 0);
			else {
				CallService(MS_USERINFO_SHOWDIALOG, (WPARAM) (dat->cache->getActiveContact()), 0);
			}
			break;
		}

		case IDC_HISTORY:
			CallService(MS_HISTORY_SHOWCONTACTHISTORY, (WPARAM) dat->hContact, 0);
			break;

		case IDC_SMILEYBTN:
			if (dat->doSmileys && PluginConfig.g_SmileyAddAvail) {
				RECT rc;
				HANDLE hContact = dat->cache->getActiveContact();

				if (CheckValidSmileyPack(dat->cache->getActiveProto(), hContact) != 0) {
					SMADD_SHOWSEL3 smaddInfo = {0};

					if (lParam == 0)
						GetWindowRect(GetDlgItem(hwndDlg, IDC_SMILEYBTN), &rc);
					else
						GetWindowRect((HWND)lParam, &rc);
					smaddInfo.cbSize = sizeof(SMADD_SHOWSEL3);
					smaddInfo.hwndTarget = GetDlgItem(hwndDlg, IDC_MESSAGE);
					smaddInfo.targetMessage = EM_REPLACESEL;
					smaddInfo.targetWParam = TRUE;
					smaddInfo.Protocolname = const_cast<char *>(dat->cache->getActiveProto());
					smaddInfo.Direction = 0;
					smaddInfo.xPosition = rc.left;
					smaddInfo.yPosition = rc.top + 24;
					smaddInfo.hwndParent = hwndContainer;
					smaddInfo.hContact = hContact;
					CallService(MS_SMILEYADD_SHOWSELECTION, (WPARAM)hwndContainer, (LPARAM) &smaddInfo);
				}
			}
			break;
		case IDC_TIME: {
			RECT rc;
			HMENU submenu = GetSubMenu(m_pContainer->hMenuContext, 2);
			int iSelection, isHandled;
			DWORD dwOldFlags = dat->dwFlags;
			DWORD dwOldEventIsShown = dat->dwFlagsEx;

			MsgWindowUpdateMenu(dat, submenu, MENU_LOGMENU);

			GetWindowRect(GetDlgItem(hwndDlg, IDC_TIME), &rc);

			iSelection = TrackPopupMenu(submenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);
			isHandled = MsgWindowMenuHandler(dat, iSelection, MENU_LOGMENU);
			return(isHandled);
		}
		case IDC_PROTOMENU: {
			RECT rc;
			HMENU submenu = GetSubMenu(m_pContainer->hMenuContext, 4);
			int iSelection;
			int iOldGlobalSendFormat = PluginConfig.m_SendFormat;

			if (dat->hContact) {
				int iLocalFormat = M->GetDword(dat->hContact, "sendformat", 0);
				int iNewLocalFormat = iLocalFormat;

				GetWindowRect(GetDlgItem(hwndDlg, IDC_PROTOCOL), &rc);

				CheckMenuItem(submenu, ID_MODE_GLOBAL, MF_BYCOMMAND | (!(dat->dwFlagsEx & MWF_SHOW_SPLITTEROVERRIDE) ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(submenu, ID_MODE_PRIVATE, MF_BYCOMMAND | (dat->dwFlagsEx & MWF_SHOW_SPLITTEROVERRIDE ? MF_CHECKED : MF_UNCHECKED));

				/*
				 * formatting menu..
				 */

				CheckMenuItem(submenu, ID_GLOBAL_BBCODE, MF_BYCOMMAND | ((PluginConfig.m_SendFormat) ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(submenu, ID_GLOBAL_OFF, MF_BYCOMMAND | ((PluginConfig.m_SendFormat == SENDFORMAT_NONE) ? MF_CHECKED : MF_UNCHECKED));

				CheckMenuItem(submenu, ID_THISCONTACT_GLOBALSETTING, MF_BYCOMMAND | ((iLocalFormat == SENDFORMAT_NONE) ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(submenu, ID_THISCONTACT_BBCODE, MF_BYCOMMAND | ((iLocalFormat > 0) ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(submenu, ID_THISCONTACT_OFF, MF_BYCOMMAND | ((iLocalFormat == -1) ? MF_CHECKED : MF_UNCHECKED));

				iSelection = TrackPopupMenu(submenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);
				switch (iSelection) {
					case ID_MODE_GLOBAL:
						dat->dwFlagsEx &= ~(MWF_SHOW_SPLITTEROVERRIDE);
						M->WriteByte(dat->hContact, SRMSGMOD_T, "splitoverride", 0);
						LoadSplitter(dat);
						AdjustBottomAvatarDisplay(dat);
						DM_RecalcPictureSize(dat);
						SendMessage(hwndDlg, WM_SIZE, 0, 0);
						break;
					case ID_MODE_PRIVATE:
						dat->dwFlagsEx |= MWF_SHOW_SPLITTEROVERRIDE;
						M->WriteByte(dat->hContact, SRMSGMOD_T, "splitoverride", 1);
						LoadSplitter(dat);
						AdjustBottomAvatarDisplay(dat);
						DM_RecalcPictureSize(dat);
						SendMessage(hwndDlg, WM_SIZE, 0, 0);
						break;
					case ID_GLOBAL_BBCODE:
						PluginConfig.m_SendFormat = SENDFORMAT_BBCODE;
						break;
					case ID_GLOBAL_OFF:
						PluginConfig.m_SendFormat = SENDFORMAT_NONE;
						break;
					case ID_THISCONTACT_GLOBALSETTING:
						iNewLocalFormat = 0;
						break;
					case ID_THISCONTACT_BBCODE:
						iNewLocalFormat = SENDFORMAT_BBCODE;
						break;
					case ID_THISCONTACT_OFF:
						iNewLocalFormat = -1;
						break;
				}
				if (iNewLocalFormat == 0)
					DBDeleteContactSetting(dat->hContact, SRMSGMOD_T, "sendformat");
				else if (iNewLocalFormat != iLocalFormat)
					M->WriteDword(dat->hContact, SRMSGMOD_T, "sendformat", iNewLocalFormat);

				if (PluginConfig.m_SendFormat != iOldGlobalSendFormat)
					M->WriteByte(SRMSGMOD_T, "sendformat", (BYTE)PluginConfig.m_SendFormat);
				if (iNewLocalFormat != iLocalFormat || PluginConfig.m_SendFormat != iOldGlobalSendFormat) {
					dat->SendFormat = M->GetDword(dat->hContact, "sendformat", PluginConfig.m_SendFormat);
					if (dat->SendFormat == -1)          // per contact override to disable it..
						dat->SendFormat = 0;
					M->BroadcastMessage(DM_CONFIGURETOOLBAR, 0, 1);
				}
			}
			break;
		}
		case IDC_TOGGLETOOLBAR:
			if (lParam == 1)
				ApplyContainerSetting(m_pContainer, CNT_NOMENUBAR, m_pContainer->dwFlags & CNT_NOMENUBAR ? 0 : 1, true);
			else
				ApplyContainerSetting(m_pContainer, CNT_HIDETOOLBAR, m_pContainer->dwFlags & CNT_HIDETOOLBAR ? 0 : 1, true);
			break;
		case IDC_INFOPANELMENU: {
			RECT 	rc;
			int 	iSelection;

			HMENU submenu = GetSubMenu(m_pContainer->hMenuContext, 9);
			GetWindowRect(GetDlgItem(hwndDlg, IDC_NAME), &rc);

			EnableMenuItem(submenu, ID_FAVORITES_ADDCONTACTTOFAVORITES, !dat->cache->isFavorite() ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(submenu, ID_FAVORITES_REMOVECONTACTFROMFAVORITES, !dat->cache->isFavorite() ? MF_GRAYED : MF_ENABLED);

			iSelection = TrackPopupMenu(submenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);

			switch(iSelection) {
				case ID_FAVORITES_ADDCONTACTTOFAVORITES:
					DBWriteContactSettingByte(dat->hContact, SRMSGMOD_T, "isFavorite", 1);
					AddContactToFavorites(dat->hContact, dat->cache->getNick(), dat->cache->getActiveProto(), dat->szStatus, dat->wStatus, LoadSkinnedProtoIcon(dat->cache->getActiveProto(), dat->cache->getActiveStatus()), 1, PluginConfig.g_hMenuFavorites);
					break;
				case ID_FAVORITES_REMOVECONTACTFROMFAVORITES:
					DBWriteContactSettingByte(dat->hContact, SRMSGMOD_T, "isFavorite", 0);
					DeleteMenu(PluginConfig.g_hMenuFavorites, (UINT_PTR)dat->hContact, MF_BYCOMMAND);
					break;
				default:
					break;
			}
			dat->cache->updateFavorite();
			break;
		}
		case IDC_SENDMENU: {
			RECT rc;
			HMENU submenu = GetSubMenu(m_pContainer->hMenuContext, 3);
			int iSelection;

			GetWindowRect(GetDlgItem(hwndDlg, IDOK), &rc);
			CheckMenuItem(submenu, ID_SENDMENU_SENDTOMULTIPLEUSERS, MF_BYCOMMAND | (dat->sendMode & SMODE_MULTIPLE ? MF_CHECKED : MF_UNCHECKED));
			CheckMenuItem(submenu, ID_SENDMENU_SENDDEFAULT, MF_BYCOMMAND | (dat->sendMode == 0 ? MF_CHECKED : MF_UNCHECKED));
			CheckMenuItem(submenu, ID_SENDMENU_SENDTOCONTAINER, MF_BYCOMMAND | (dat->sendMode & SMODE_CONTAINER ? MF_CHECKED : MF_UNCHECKED));
			CheckMenuItem(submenu, ID_SENDMENU_FORCEANSISEND, MF_BYCOMMAND | (dat->sendMode & SMODE_FORCEANSI ? MF_CHECKED : MF_UNCHECKED));
			CheckMenuItem(submenu, ID_SENDMENU_SENDLATER, MF_BYCOMMAND | (dat->sendMode & SMODE_SENDLATER ? MF_CHECKED : MF_UNCHECKED));
			CheckMenuItem(submenu, ID_SENDMENU_SENDWITHOUTTIMEOUTS, MF_BYCOMMAND | (dat->sendMode & SMODE_NOACK ? MF_CHECKED : MF_UNCHECKED));
			{
				const char *szFinalProto = dat->cache->getActiveProto();
				char szServiceName[128];

				mir_snprintf(szServiceName, 128, "%s/SendNudge", szFinalProto);
				EnableMenuItem(submenu, ID_SENDMENU_SENDNUDGE, MF_BYCOMMAND | ((ServiceExists(szServiceName) && ServiceExists(MS_NUDGE_SEND)) ? MF_ENABLED : MF_GRAYED));
			}
			if (lParam)
				iSelection = TrackPopupMenu(submenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);
			else
				iSelection = HIWORD(wParam);

			switch (iSelection) {
				case ID_SENDMENU_SENDTOMULTIPLEUSERS:
					dat->sendMode ^= SMODE_MULTIPLE;
					if (dat->sendMode & SMODE_MULTIPLE)
						HWND hwndClist = DM_CreateClist(dat);
					else {
						if (IsWindow(GetDlgItem(hwndDlg, IDC_CLIST)))
							DestroyWindow(GetDlgItem(hwndDlg, IDC_CLIST));
					}
					break;
				case ID_SENDMENU_SENDNUDGE:
					SendNudge(dat);
					break;
				case ID_SENDMENU_SENDDEFAULT:
					dat->sendMode = 0;
					break;
				case ID_SENDMENU_SENDTOCONTAINER:
					dat->sendMode ^= SMODE_CONTAINER;
					RedrawWindow(hwndDlg, 0, 0, RDW_ERASENOW|RDW_UPDATENOW);
					break;
				case ID_SENDMENU_FORCEANSISEND:
					dat->sendMode ^= SMODE_FORCEANSI;
					break;
				case ID_SENDMENU_SENDLATER:
					if(sendLater->isAvail())
						dat->sendMode ^= SMODE_SENDLATER;
					else
						CWarning::show(CWarning::WARN_NO_SENDLATER, MB_OK|MB_ICONINFORMATION, CTranslator::get(CTranslator::QMGR_ERROR_NOMULTISEND));
					break;
				case ID_SENDMENU_SENDWITHOUTTIMEOUTS:
					dat->sendMode ^= SMODE_NOACK;
					if (dat->sendMode & SMODE_NOACK)
						M->WriteByte(dat->hContact, SRMSGMOD_T, "no_ack", 1);
					else
						DBDeleteContactSetting(dat->hContact, SRMSGMOD_T, "no_ack");
					break;
			}
			M->WriteByte(dat->hContact, SRMSGMOD_T, "no_ack", (BYTE)(dat->sendMode & SMODE_NOACK ? 1 : 0));
			M->WriteByte(dat->hContact, SRMSGMOD_T, "forceansi", (BYTE)(dat->sendMode & SMODE_FORCEANSI ? 1 : 0));
			SetWindowPos(GetDlgItem(hwndDlg, IDC_MESSAGE), 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
			if (dat->sendMode & SMODE_MULTIPLE || dat->sendMode & SMODE_CONTAINER) {
				SetWindowPos(GetDlgItem(hwndDlg, IDC_MESSAGE), 0, 0, 0, 0, 0, SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOZORDER|
							 SWP_NOMOVE|SWP_NOSIZE|SWP_NOCOPYBITS);
				RedrawWindow(hwndDlg, 0, 0, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ALLCHILDREN);
			}
			else {
				if (IsWindow(GetDlgItem(hwndDlg, IDC_CLIST)))
					DestroyWindow(GetDlgItem(hwndDlg, IDC_CLIST));
				SetWindowPos(GetDlgItem(hwndDlg, IDC_MESSAGE), 0, 0, 0, 0, 0, SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOZORDER|
							 SWP_NOMOVE|SWP_NOSIZE|SWP_NOCOPYBITS);
				RedrawWindow(hwndDlg, 0, 0, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ALLCHILDREN);
			}
			SendMessage(hwndContainer, DM_QUERYCLIENTAREA, 0, (LPARAM)&rc);
			SendMessage(hwndDlg, WM_SIZE, 0, 0);
			DM_ScrollToBottom(dat, 1, 1);
			Utils::showDlgControl(hwndDlg, IDC_MULTISPLITTER, (dat->sendMode & SMODE_MULTIPLE) ? SW_SHOW : SW_HIDE);
			Utils::showDlgControl(hwndDlg, IDC_CLIST, (dat->sendMode & SMODE_MULTIPLE) ? SW_SHOW : SW_HIDE);
			break;
		}
		case IDC_TOGGLESIDEBAR: {
			SendMessage(m_pContainer->hwnd, WM_COMMAND, IDC_TOGGLESIDEBAR, 0);
			break;
		}
		case IDC_PIC: {
			RECT rc;
			GetClientRect(hwndDlg, &rc);

			dat->fEditNotesActive = !dat->fEditNotesActive;
			if(dat->fEditNotesActive) {
				int iLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_MESSAGE));
				if(iLen != 0) {
					SendMessage(hwndDlg, DM_ACTIVATETOOLTIP, IDC_MESSAGE, (LPARAM)CTranslator::get(CTranslator::GEN_MSG_NO_EDIT_NOTES));
					dat->fEditNotesActive = false;
					break;
				}

				if(!dat->fIsAutosizingInput) {
					dat->iSplitterSaved = dat->splitterY;
					dat->splitterY = rc.bottom / 2;
					SendMessage(hwndDlg, WM_SIZE, 1, 1);
				}

				DBVARIANT dbv = {0};

				if(0 == M->GetTString(dat->hContact, "UserInfo", "MyNotes", &dbv)) {
					SetDlgItemText(hwndDlg, IDC_MESSAGE, dbv.ptszVal);
					mir_free(dbv.ptszVal);
				}
			}
			else {
				int iLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_MESSAGE));

				TCHAR *buf = (TCHAR *)mir_alloc((iLen + 2) * sizeof(TCHAR));
				GetDlgItemText(hwndDlg, IDC_MESSAGE, buf, iLen + 1);
				M->WriteTString(dat->hContact, "UserInfo", "MyNotes", buf);
				SetDlgItemText(hwndDlg, IDC_MESSAGE, _T(""));
				
				if(!dat->fIsAutosizingInput) {
					dat->splitterY = dat->iSplitterSaved;
					SendMessage(hwndDlg, WM_SIZE, 0, 0);
					DM_ScrollToBottom(dat, 0, 1);
				}
			}
			SetWindowPos(GetDlgItem(hwndDlg, IDC_MESSAGE), 0, 0, 0, 0, 0, SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOZORDER|
						 SWP_NOMOVE|SWP_NOSIZE|SWP_NOCOPYBITS);
			RedrawWindow(hwndDlg, 0, 0, RDW_INVALIDATE|RDW_ERASE|RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN);

			if(dat->fEditNotesActive)
				CWarning::show(CWarning::WARN_EDITUSERNOTES, MB_OK|MB_ICONINFORMATION);
			break;
		}
		case IDM_CLEAR:
			ClearLog(dat);
			break;
		case IDC_PROTOCOL:	{
			RECT rc;
			int  iSel = 0;

			HMENU hMenu = (HMENU) CallService(MS_CLIST_MENUBUILDCONTACT, (WPARAM) dat->hContact, 0);
			if(lParam == 0)
				GetWindowRect(GetDlgItem(hwndDlg, IDC_PROTOCOL/*IDC_NAME*/), &rc);
			else
				GetWindowRect((HWND)lParam, &rc);
			iSel = TrackPopupMenu(hMenu, TPM_RETURNCMD, rc.left, rc.bottom, 0, hwndDlg, NULL);

			if(iSel)
				CallService(MS_CLIST_MENUPROCESSCOMMAND, MAKEWPARAM(LOWORD(iSel), MPCF_CONTACTMENU), (LPARAM) dat->hContact);

			DestroyMenu(hMenu);
			break;
		}
		// error control
		case IDC_CANCELSEND:
			SendMessage(hwndDlg, DM_ERRORDECIDED, MSGERROR_CANCEL, 0);
			break;
		case IDC_RETRY:
			SendMessage(hwndDlg, DM_ERRORDECIDED, MSGERROR_RETRY, 0);
			break;
		case IDC_MSGSENDLATER:
			SendMessage(hwndDlg, DM_ERRORDECIDED, MSGERROR_SENDLATER, 0);
			break;
		case IDC_SELFTYPING:
			if (dat->hContact) {
				int iCurrentTypingMode = M->GetByte(dat->hContact, SRMSGMOD, SRMSGSET_TYPING, M->GetByte(SRMSGMOD, SRMSGSET_TYPINGNEW, SRMSGDEFSET_TYPINGNEW));

				if (dat->nTypeMode == PROTOTYPE_SELFTYPING_ON && iCurrentTypingMode) {
					DM_NotifyTyping(dat, PROTOTYPE_SELFTYPING_OFF);
					dat->nTypeMode = PROTOTYPE_SELFTYPING_OFF;
				}
				M->WriteByte(dat->hContact, SRMSGMOD, SRMSGSET_TYPING, (BYTE)!iCurrentTypingMode);
			}
			break;
		default:
			return(0);
	}
	return(1);
}

LRESULT TSAPI DM_ContainerCmdHandler(TContainerData *pContainer, UINT cmd, WPARAM wParam, LPARAM lParam)
{
	if(!pContainer)
		return(0);

	HWND hwndDlg = pContainer->hwnd;

	switch(cmd) {
		case IDC_CLOSE:
			SendMessage(hwndDlg, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case IDC_MINIMIZE:
			PostMessage(hwndDlg, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		case IDC_MAXIMIZE:
			SendMessage(hwndDlg, WM_SYSCOMMAND, IsZoomed(hwndDlg) ? SC_RESTORE : SC_MAXIMIZE, 0);
			break;
		case IDOK:
			SendMessage(pContainer->hwndActive, WM_COMMAND, wParam, lParam);      // pass the IDOK command to the active child - fixes the "enter not working
			break;
		case ID_FILE_SAVEMESSAGELOGAS:
			SendMessage(pContainer->hwndActive, DM_SAVEMESSAGELOG, 0, 0);
			break;
		case ID_FILE_CLOSEMESSAGESESSION:
			PostMessage(pContainer->hwndActive, WM_CLOSE, 0, 1);
			break;
		case ID_FILE_CLOSE:
			PostMessage(hwndDlg, WM_CLOSE, 0, 1);
			break;
		case ID_VIEW_SHOWSTATUSBAR:
			ApplyContainerSetting(pContainer, CNT_NOSTATUSBAR, pContainer->dwFlags & CNT_NOSTATUSBAR ? 0 : 1, true);
			break;
		case ID_VIEW_VERTICALMAXIMIZE:
			ApplyContainerSetting(pContainer, CNT_VERTICALMAX, pContainer->dwFlags & CNT_VERTICALMAX ? 0 : 1, false);
			break;
		case ID_VIEW_BOTTOMTOOLBAR:
			ApplyContainerSetting(pContainer, CNT_BOTTOMTOOLBAR, pContainer->dwFlags & CNT_BOTTOMTOOLBAR ? 0 : 1, false);
			M->BroadcastMessage(DM_CONFIGURETOOLBAR, 0, 1);
			return 0;
		case ID_VIEW_SHOWTOOLBAR:
			ApplyContainerSetting(pContainer, CNT_HIDETOOLBAR, pContainer->dwFlags & CNT_HIDETOOLBAR ? 0 : 1, false);
			M->BroadcastMessage(DM_CONFIGURETOOLBAR, 0, 1);
			return 0;
		case ID_VIEW_SHOWMENUBAR:
			ApplyContainerSetting(pContainer, CNT_NOMENUBAR, pContainer->dwFlags & CNT_NOMENUBAR ? 0 : 1, true);
			break;
		case ID_VIEW_SHOWTITLEBAR:
			ApplyContainerSetting(pContainer, CNT_NOTITLE, pContainer->dwFlags & CNT_NOTITLE ? 0 : 1, true);
			break;
		case ID_VIEW_TABSATBOTTOM:
			ApplyContainerSetting(pContainer, CNT_TABSBOTTOM, pContainer->dwFlags & CNT_TABSBOTTOM ? 0 : 1, false);
			break;
		case ID_VIEW_SHOWMULTISENDCONTACTLIST:
			SendMessage(pContainer->hwndActive, WM_COMMAND, MAKEWPARAM(IDC_SENDMENU, ID_SENDMENU_SENDTOMULTIPLEUSERS), 0);
			break;
		case ID_VIEW_STAYONTOP:
			SendMessage(hwndDlg, WM_SYSCOMMAND, IDM_STAYONTOP, 0);
			break;
		case ID_CONTAINER_CONTAINEROPTIONS:
			SendMessage(hwndDlg, WM_SYSCOMMAND, IDM_MOREOPTIONS, 0);
			break;
		case ID_EVENTPOPUPS_DISABLEALLEVENTPOPUPS:
			ApplyContainerSetting(pContainer, (CNT_DONTREPORT | CNT_DONTREPORTUNFOCUSED | CNT_ALWAYSREPORTINACTIVE), 0, false);
			return 0;
		case ID_EVENTPOPUPS_SHOWPOPUPSIFWINDOWISMINIMIZED:
			ApplyContainerSetting(pContainer, CNT_DONTREPORT, pContainer->dwFlags & CNT_DONTREPORT ? 0 : 1, false);
			return 0;
		case ID_EVENTPOPUPS_SHOWPOPUPSIFWINDOWISUNFOCUSED:
			ApplyContainerSetting(pContainer, CNT_DONTREPORTUNFOCUSED, pContainer->dwFlags & CNT_DONTREPORTUNFOCUSED ? 0 : 1, false);
			return 0;
		case ID_EVENTPOPUPS_SHOWPOPUPSFORALLINACTIVESESSIONS:
			ApplyContainerSetting(pContainer, CNT_ALWAYSREPORTINACTIVE, pContainer->dwFlags & CNT_ALWAYSREPORTINACTIVE ? 0 : 1, false);
			return 0;
		case ID_WINDOWFLASHING_DISABLEFLASHING:
			ApplyContainerSetting(pContainer, CNT_NOFLASH, 1, false);
			ApplyContainerSetting(pContainer, CNT_FLASHALWAYS, 0, false);
			return 0;
		case ID_WINDOWFLASHING_FLASHUNTILFOCUSED:
			ApplyContainerSetting(pContainer, CNT_NOFLASH, 0, false);
			ApplyContainerSetting(pContainer, CNT_FLASHALWAYS, 1, false);
			return 0;
		case ID_WINDOWFLASHING_USEDEFAULTVALUES:
			ApplyContainerSetting(pContainer, (CNT_NOFLASH | CNT_FLASHALWAYS), 0, false);
			return 0;
		case ID_OPTIONS_SAVECURRENTWINDOWPOSITIONASDEFAULT: {
			WINDOWPLACEMENT wp = {0};

			wp.length = sizeof(wp);
			if (GetWindowPlacement(hwndDlg, &wp)) {
				M->WriteDword(SRMSGMOD_T, "splitx", wp.rcNormalPosition.left);
				M->WriteDword(SRMSGMOD_T, "splity", wp.rcNormalPosition.top);
				M->WriteDword(SRMSGMOD_T, "splitwidth", wp.rcNormalPosition.right - wp.rcNormalPosition.left);
				M->WriteDword(SRMSGMOD_T, "splitheight", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
			}
			return 0;
		}
		case ID_VIEW_INFOPANEL: {
			TWindowData *dat = (TWindowData *)GetWindowLongPtr(pContainer->hwndActive, GWLP_USERDATA);
			if(dat) {
				RECT	rc;
				POINT	pt;
				GetWindowRect(pContainer->hwndActive, &rc);
				pt.x = rc.left + 10;
				pt.y = rc.top + dat->Panel->getHeight() - 10;
				dat->Panel->invokeConfigDialog(pt);
			}
			return(0);
		}
		/*
		 * commands from the message log popup will be routed to the
		 * message log menu handler
		 */
		case ID_MESSAGELOGSETTINGS_FORTHISCONTACT:
		case ID_MESSAGELOGSETTINGS_GLOBAL: {
			struct TWindowData *dat = (struct TWindowData *)GetWindowLongPtr(pContainer->hwndActive, GWLP_USERDATA);

			if(dat) {
				MsgWindowMenuHandler(dat, (int)LOWORD(wParam), MENU_LOGMENU);
				return(1);
			}
			break;
		}
		case ID_HELP_ABOUTTABSRMM:
			CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), 0, DlgProcAbout, 0);
			break;
		default:
			return(0); 		// not handled
	}
	return(1);				// handled
}

/**
 * initialize rich edit control (log and edit control) for both MUC and
 * standard IM session windows.
 */
void TSAPI DM_InitRichEdit(TWindowData *dat)

{
	char*				szStreamOut = NULL;
	SETTEXTEX 			stx = {ST_DEFAULT, CP_UTF8};
	COLORREF 			colour;
	COLORREF 			inputcharcolor;
	CHARFORMAT2A 		cf2;
	LOGFONTA 			lf;
	int 				i = 0;
	bool				fIsChat = ((dat->bType == SESSIONTYPE_CHAT) ? true : false);
	HWND				hwndLog = GetDlgItem(dat->hwnd, !fIsChat ? IDC_LOG : IDC_CHAT_LOG);
	HWND				hwndEdit= GetDlgItem(dat->hwnd, !fIsChat ? IDC_MESSAGE : IDC_CHAT_MESSAGE);
	HWND				hwndDlg = dat->hwnd;

	ZeroMemory(&cf2, sizeof(CHARFORMAT2A));

	dat->inputbg = fIsChat ? M->GetDword(FONTMODULE, "inputbg", SRMSGDEFSET_BKGCOLOUR) : dat->pContainer->theme.inputbg;
	colour = fIsChat ? M->GetDword(FONTMODULE, SRMSGSET_BKGCOLOUR_MUC, SRMSGDEFSET_BKGCOLOUR) : dat->pContainer->theme.bg;

	if(!fIsChat) {
		if (GetWindowTextLengthA(hwndEdit) > 0)
			szStreamOut = Message_GetFromStream(hwndEdit, dat, (CP_UTF8 << 16) | (SF_RTFNOOBJS | SFF_PLAINRTF | SF_USECODEPAGE));
	}

	SendMessage(hwndLog, EM_SETBKGNDCOLOR, 0, colour);
	SendMessage(hwndEdit, EM_SETBKGNDCOLOR, 0, dat->inputbg);

	if(fIsChat)
		LoadLogfont(MSGFONTID_MESSAGEAREA, &lf, &inputcharcolor, FONTMODULE);
	else {
		lf = dat->pContainer->theme.logFonts[MSGFONTID_MESSAGEAREA];
		inputcharcolor = dat->pContainer->theme.fontColors[MSGFONTID_MESSAGEAREA];
	}
	/*
	 * correct the input area text color to avoid a color from the table of usable bbcode colors
	 */
	if(!fIsChat) {
		for (i = 0; i < Utils::rtf_ctable_size; i++) {
			if (Utils::rtf_ctable[i].clr == inputcharcolor)
				inputcharcolor = RGB(GetRValue(inputcharcolor), GetGValue(inputcharcolor), GetBValue(inputcharcolor) == 0 ? GetBValue(inputcharcolor) + 1 : GetBValue(inputcharcolor) - 1);
		}
	}
	if(fIsChat) {
		cf2.dwMask = CFM_COLOR | CFM_FACE | CFM_CHARSET | CFM_SIZE | CFM_WEIGHT | CFM_ITALIC | CFM_BACKCOLOR;
		cf2.cbSize = sizeof(cf2);
		cf2.crTextColor = inputcharcolor;
		cf2.bCharSet = lf.lfCharSet;
		cf2.crBackColor = dat->inputbg;
		strncpy(cf2.szFaceName, lf.lfFaceName, LF_FACESIZE);
		cf2.dwEffects = 0;
		cf2.wWeight = (WORD)lf.lfWeight;
		cf2.bPitchAndFamily = lf.lfPitchAndFamily;
		cf2.yHeight = abs(lf.lfHeight) * 15;
		SetWindowText(hwndEdit, _T(""));
		SendMessage(hwndEdit, EM_SETCHARFORMAT, 0, (LPARAM)&cf2);
	}
	else {
		cf2.dwMask = CFM_COLOR | CFM_FACE | CFM_CHARSET | CFM_SIZE | CFM_WEIGHT | CFM_BOLD | CFM_ITALIC;
		cf2.cbSize = sizeof(cf2);
		cf2.crTextColor = inputcharcolor;
		cf2.bCharSet = lf.lfCharSet;
		strncpy(cf2.szFaceName, lf.lfFaceName, LF_FACESIZE);
		cf2.dwEffects = ((lf.lfWeight >= FW_BOLD) ? CFE_BOLD : 0) | (lf.lfItalic ? CFE_ITALIC : 0)|(lf.lfUnderline ? CFE_UNDERLINE : 0)|(lf.lfStrikeOut ? CFE_STRIKEOUT : 0);
		cf2.wWeight = (WORD)lf.lfWeight;
		cf2.bPitchAndFamily = lf.lfPitchAndFamily;
		cf2.yHeight = abs(lf.lfHeight) * 15;
		SendMessageA(hwndEdit, EM_SETCHARFORMAT, 0, (LPARAM)&cf2);
	}

	/*
	 * setup the rich edit control(s)
	 * LOG is always set to RTL, because this is needed for proper bidirectional operation later.
	 * The real text direction is then enforced by the streaming code which adds appropiate paragraph
	 * and textflow formatting commands to the
	 */

	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));

	pf2.cbSize = sizeof(pf2);

	pf2.wEffects = PFE_RTLPARA;
	pf2.dwMask = PFM_RTLPARA;
	if (Utils::FindRTLLocale(dat))
		SendMessage(hwndEdit, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
	if (!(dat->dwFlags & MWF_LOG_RTL)) {
		pf2.wEffects = 0;
		SendMessage(hwndEdit, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
	}
	SendMessage(hwndEdit, EM_SETLANGOPTIONS, 0, (LPARAM) SendMessage(hwndEdit, EM_GETLANGOPTIONS, 0, 0) & ~IMF_AUTOKEYBOARD);
	pf2.wEffects = PFE_RTLPARA;
	pf2.dwMask |= PFM_OFFSET;
	if (dat->dwFlags & MWF_INITMODE) {
		pf2.dwMask |= (PFM_RIGHTINDENT | PFM_OFFSETINDENT);
		pf2.dxStartIndent = 30;
		pf2.dxRightIndent = 30;
	}
	pf2.dxOffset = dat->pContainer->theme.left_indent + 30;
	if(!fIsChat) {
		SetWindowText(hwndLog, _T(""));
		SendMessage(hwndLog, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
		SendMessage(hwndLog, EM_SETLANGOPTIONS, 0, (LPARAM) SendDlgItemMessage(hwndDlg, IDC_LOG, EM_GETLANGOPTIONS, 0, 0) & ~IMF_AUTOKEYBOARD);
	}

	/*
	 * set the scrollbars etc to RTL/LTR (only for manual RTL mode)
	 */

	if(!fIsChat) {
		if (dat->dwFlags & MWF_LOG_RTL) {
			SetWindowLongPtr(hwndEdit, GWL_EXSTYLE, GetWindowLongPtr(hwndEdit, GWL_EXSTYLE) | WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR);
			SetWindowLongPtr(hwndLog, GWL_EXSTYLE, GetWindowLongPtr(hwndLog, GWL_EXSTYLE) | WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR);
		} else {
			SetWindowLongPtr(hwndEdit, GWL_EXSTYLE, GetWindowLongPtr(hwndEdit, GWL_EXSTYLE) &~(WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR));
			SetWindowLongPtr(hwndLog, GWL_EXSTYLE, GetWindowLongPtr(hwndLog, GWL_EXSTYLE) &~(WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR));
		}
		SetWindowText(hwndEdit, _T(""));
	}
	if (szStreamOut != NULL) {
		SendMessage(hwndEdit, EM_SETTEXTEX, (WPARAM)&stx, (LPARAM)szStreamOut);
		free(szStreamOut);
	}
}

/*
* action and callback procedures for the stock button objects
*/

static void BTN_StockAction(ButtonItem *item, HWND hwndDlg, struct TWindowData *dat, HWND hwndBtn)
{
	if (item->dwStockFlags & SBI_HANDLEBYCLIENT && IsWindow(hwndDlg) && dat)
		SendMessage(hwndDlg, WM_COMMAND, MAKELONG(item->uId, BN_CLICKED), (LPARAM)hwndBtn);
	else {
		switch (item->uId) {
			case IDC_SBAR_CANCEL:
				PostMessage(hwndDlg, WM_COMMAND, MAKELONG(IDC_SAVE, BN_CLICKED), (LPARAM)hwndBtn);
				break;
			case IDC_SBAR_SLIST:
				SendMessage(PluginConfig.g_hwndHotkeyHandler, DM_TRAYICONNOTIFY, 101, WM_LBUTTONUP);
				break;
			case IDC_SBAR_FAVORITES: {
				POINT pt;
				int iSelection;
				GetCursorPos(&pt);
				iSelection = TrackPopupMenu(PluginConfig.g_hMenuFavorites, TPM_RETURNCMD, pt.x, pt.y, 0, PluginConfig.g_hwndHotkeyHandler, NULL);
				HandleMenuEntryFromhContact(iSelection);
				break;
			}
			case IDC_SBAR_RECENT: {
				POINT pt;
				int iSelection;
				GetCursorPos(&pt);
				iSelection = TrackPopupMenu(PluginConfig.g_hMenuRecent, TPM_RETURNCMD, pt.x, pt.y, 0, PluginConfig.g_hwndHotkeyHandler, NULL);
				HandleMenuEntryFromhContact(iSelection);
				break;
			}
			case IDC_SBAR_USERPREFS: {
				HANDLE hContact = 0;
				SendMessage(hwndDlg, DM_QUERYHCONTACT, 0, (LPARAM)&hContact);
				if (hContact != 0)
					CallService(MS_TABMSG_SETUSERPREFS, (WPARAM)hContact, 0);
				break;
			}
			case IDC_SBAR_TOGGLEFORMAT: {
				if (dat) {
					if (IsDlgButtonChecked(hwndDlg, IDC_SBAR_TOGGLEFORMAT) == BST_UNCHECKED) {
						dat->SendFormat = 0;
						GetSendFormat(dat, 0);
					} else {
						dat->SendFormat = SENDFORMAT_BBCODE;
						GetSendFormat(dat, 0);
					}
				}
				break;
			}
		}
	}
}

static void BTN_StockCallback(ButtonItem *item, HWND hwndDlg, struct TWindowData *dat, HWND hwndBtn)
{
}

/*
* predefined button objects for customizeable buttons
*/

static struct SIDEBARITEM sbarItems[] = {
	0, 0, 0, 0, 0, _T(""), NULL, NULL, _T("")
};

int TSAPI BTN_GetStockItem(ButtonItem *item, const TCHAR *szName)
{
	int i = 0;

	while (sbarItems[i].uId) {
		if (!_tcsicmp(sbarItems[i].szName, szName)) {
			item->uId = sbarItems[i].uId;
			//item->dwFlags |= BUTTON_ISSIDEBAR;
			//myGlobals.m_SideBarEnabled = TRUE;
			if (item->dwFlags & BUTTON_ISSIDEBAR) {
				if (sbarItems[i].dwFlags & SBI_TOP)
					item->yOff = 0;
				else if (sbarItems[i].dwFlags & SBI_BOTTOM)
					item->yOff = -1;
			}
			item->dwStockFlags = sbarItems[i].dwFlags;
			item->dwFlags = sbarItems[i].dwFlags & SBI_TOGGLE ? item->dwFlags | BUTTON_ISTOGGLE : item->dwFlags & ~BUTTON_ISTOGGLE;
			item->pfnAction = sbarItems[i].pfnAction;
			item->pfnCallback = sbarItems[i].pfnCallback;
			lstrcpyn(item->szTip, sbarItems[i].tszTip, 256);
			item->szTip[255] = 0;
			if (sbarItems[i].hIcon) {
				item->normalGlyphMetrics[0] = (LONG_PTR)sbarItems[i].hIcon;
				item->dwFlags |= BUTTON_NORMALGLYPHISICON;
			}
			if (sbarItems[i].hIconPressed) {
				item->pressedGlyphMetrics[0] = (LONG_PTR)sbarItems[i].hIconPressed;
				item->dwFlags |= BUTTON_PRESSEDGLYPHISICON;
			}
			if (sbarItems[i].hIconHover) {
				item->hoverGlyphMetrics[0] = (LONG_PTR)sbarItems[i].hIconHover;
				item->dwFlags |= BUTTON_HOVERGLYPHISICON;
			}
			return 1;
		}
		i++;
	}
	return 0;
}

/*
* set the states of defined database action buttons (only if button is a toggle)
*/

void TSAPI DM_SetDBButtonStates(HWND hwndChild, struct TWindowData *dat)
{
	ButtonItem *buttonItem = dat->pContainer->buttonItems;
	HANDLE hContact = dat->hContact, hFinalContact = 0;
	char *szModule, *szSetting;
	HWND hwndContainer = dat->pContainer->hwnd;;

	while (buttonItem) {
		BOOL result = FALSE;
		HWND hWnd = GetDlgItem(hwndContainer, buttonItem->uId);

		if (buttonItem->pfnCallback)
			buttonItem->pfnCallback(buttonItem, hwndChild, dat, hWnd);

		if (!(buttonItem->dwFlags & BUTTON_ISTOGGLE && buttonItem->dwFlags & BUTTON_ISDBACTION)) {
			buttonItem = buttonItem->nextItem;
			continue;
		}
		szModule = buttonItem->szModule;
		szSetting = buttonItem->szSetting;
		if (buttonItem->dwFlags & BUTTON_DBACTIONONCONTACT || buttonItem->dwFlags & BUTTON_ISCONTACTDBACTION) {
			if (hContact == 0) {
				SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
				buttonItem = buttonItem->nextItem;
				continue;
			}
			if (buttonItem->dwFlags & BUTTON_ISCONTACTDBACTION)
				szModule = (char *)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM)hContact, 0);
			hFinalContact = hContact;
		} else
			hFinalContact = 0;

		if (buttonItem->type == DBVT_ASCIIZ) {
			DBVARIANT dbv = {0};

			if (!DBGetContactSettingString(hFinalContact, szModule, szSetting, &dbv)) {
				result = !strcmp((char *)buttonItem->bValuePush, dbv.pszVal);
				DBFreeVariant(&dbv);
			}
		} else {
			switch (buttonItem->type) {
				case DBVT_BYTE: {
					BYTE val = M->GetByte(hFinalContact, szModule, szSetting, 0);
					result = (val == buttonItem->bValuePush[0]);
					break;
				}
				case DBVT_WORD: {
					WORD val = DBGetContactSettingWord(hFinalContact, szModule, szSetting, 0);
					result = (val == *((WORD *) & buttonItem->bValuePush));
					break;
				}
				case DBVT_DWORD: {
					DWORD val = M->GetDword(hFinalContact, szModule, szSetting, 0);
					result = (val == *((DWORD *) & buttonItem->bValuePush));
					break;
				}
			}
		}
		SendMessage(hWnd, BM_SETCHECK, (WPARAM)result, 0);
		buttonItem = buttonItem->nextItem;
	}
}

LRESULT TSAPI DM_ScrollToBottom(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	if (dat) {

		if (dat->dwFlagsEx & MWF_SHOW_SCROLLINGDISABLED)
			return 0;

		if (IsIconic(dat->pContainer->hwnd))
			dat->dwFlags |= MWF_DEFERREDSCROLL;

		if (dat->hwndIEView) {
			PostMessage(dat->hwnd, DM_SCROLLIEVIEW, 0, 0);
			return 0;
		} else if (dat->hwndHPP) {
			SendMessage(dat->hwnd, DM_SCROLLIEVIEW, 0, 0);
			return 0;
		} else {
			HWND hwnd = GetDlgItem(dat->hwnd, dat->bType == SESSIONTYPE_IM ? IDC_LOG : IDC_CHAT_LOG);
			if (lParam)
				SendMessage(hwnd, WM_SIZE, 0, 0);

			if (wParam == 1 && lParam == 1) {
				RECT rc;
				int len;

				GetClientRect(hwnd, &rc);
				len = GetWindowTextLengthA(hwnd);
				SendMessage(hwnd, EM_SETSEL, len - 1, len - 1);
			}
			if (wParam)
				SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
			else
				PostMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
			if (lParam)
				InvalidateRect(hwnd, NULL, FALSE);
		}
	}
	return 0;
}

static unsigned __stdcall LoadKLThread(LPVOID vParam)
{
	HANDLE 		hContact = reinterpret_cast<HANDLE>(vParam);
	DBVARIANT 	dbv = {0};

	LRESULT res = M->GetTString(hContact, SRMSGMOD_T, "locale", &dbv);
	if (res == 0) {
		HKL hkl = LoadKeyboardLayout(dbv.ptszVal, 0);
		PostMessage(PluginConfig.g_hwndHotkeyHandler, DM_SETLOCALE, (WPARAM)hContact, (LPARAM)hkl);
		DBFreeVariant(&dbv);
	}
	return(0);
}


LRESULT TSAPI DM_LoadLocale(TWindowData *dat)
{
	/*
	* set locale if saved to contact
	*/
	if (dat) {
		if (dat->dwFlags & MWF_WASBACKGROUNDCREATE)
			return 0;

		if (PluginConfig.m_AutoLocaleSupport) {
			DBVARIANT dbv;
			int res;
			TCHAR szKLName[KL_NAMELENGTH+1];
			UINT  flags = KLF_ACTIVATE;

			res = DBGetContactSettingTString(dat->hContact, SRMSGMOD_T, "locale", &dbv);
			if (res == 0) {

				/*
				dat->hkl = LoadKeyboardLayout(dbv.ptszVal, KLF_REPLACELANG | KLF_NOTELLSHELL);
				GetLocaleID(dat, dbv.ptszVal);
				PostMessage(dat->hwnd, DM_SETLOCALE, 0, 0);*/
				DBFreeVariant(&dbv);
				CloseHandle((HANDLE)mir_forkthreadex(LoadKLThread, reinterpret_cast<void *>(dat->hContact), 16000, NULL));
			} else {
				if(!PluginConfig.m_dontUseDefaultKbd) {
					TCHAR	szBuf[20];

					GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, szBuf, 20);
					mir_sntprintf(szKLName, KL_NAMELENGTH, _T("0000%s"), szBuf);
					M->WriteTString(dat->hContact, SRMSGMOD_T, "locale", szKLName);
				}
				else {
					GetKeyboardLayoutName(szKLName);
					M->WriteTString(dat->hContact, SRMSGMOD_T, "locale", szKLName);
				}
				/*dat->hkl = LoadKeyboardLayout(szKLName, KLF_NOTELLSHELL | KLF_REPLACELANG);
				GetLocaleID(dat, szKLName);
				PostMessage(dat->hwnd, DM_SETLOCALE, 0, 0);*/
				CloseHandle((HANDLE)mir_forkthreadex(LoadKLThread, reinterpret_cast<void *>(dat->hContact), 16000, NULL));
			}
		}
	}
	return 0;
}

LRESULT TSAPI DM_RecalcPictureSize(TWindowData *dat)
{
	BITMAP bminfo;
	HBITMAP hbm;

	if (dat) {
		hbm = ((dat->Panel->isActive()) && dat->pContainer->avatarMode != 3) ? dat->hOwnPic : (dat->ace ? dat->ace->hbmPic : PluginConfig.g_hbmUnknown);

		if (hbm == 0) {
			dat->pic.cy = dat->pic.cx = 60;
			return 0;
		}
		GetObject(hbm, sizeof(bminfo), &bminfo);
		CalcDynamicAvatarSize(dat, &bminfo);
		SendMessage(dat->hwnd, WM_SIZE, 0, 0);
	}
	return 0;
}

LRESULT TSAPI DM_UpdateLastMessage(const TWindowData *dat)
{
	if (dat) {
		if (dat->pContainer->hwndStatus == 0)
			return 0;
		if (dat->pContainer->hwndActive != dat->hwnd)
			return 0;
		if (dat->showTyping) {
			TCHAR szBuf[80];

			mir_sntprintf(szBuf, safe_sizeof(szBuf), CTranslator::get(CTranslator::GEN_MTN_STARTWITHNICK), dat->cache->getNick());
			SendMessage(dat->pContainer->hwndStatus, SB_SETTEXT, 0, (LPARAM) szBuf);
			SendMessage(dat->pContainer->hwndStatus, SB_SETICON, 0, (LPARAM) PluginConfig.g_buttonBarIcons[ICON_DEFAULT_TYPING]);
			return 0;
		}
		else
			SendMessage(dat->pContainer->hwndStatus, SB_SETICON, 0, 0);

		if (dat->lastMessage || dat->pContainer->dwFlags & CNT_UINSTATUSBAR) {
			TCHAR date[64], time[64];

			if (!(dat->pContainer->dwFlags & CNT_UINSTATUSBAR)) {
				tmi.printTimeStamp(NULL, dat->lastMessage, _T("d"), date, safe_sizeof(date), 0); 
				if (dat->pContainer->dwFlags & CNT_UINSTATUSBAR && lstrlen(date) > 6)
					date[lstrlen(date) - 5] = 0;
				tmi.printTimeStamp(NULL, dat->lastMessage, _T("t"), time, safe_sizeof(time), 0); 
			}
			if (dat->pContainer->dwFlags & CNT_UINSTATUSBAR) {
				TCHAR fmt[100];
				mir_sntprintf(fmt, safe_sizeof(fmt), _T("UID: %s"), dat->cache->getUIN());
				SendMessage(dat->pContainer->hwndStatus, SB_SETTEXT, 0, (LPARAM)fmt);
			} else {
				TCHAR fmt[100];
				mir_sntprintf(fmt, safe_sizeof(fmt), CTranslator::get(CTranslator::GEN_SBAR_LASTRECEIVED), date, time);
				SendMessage(dat->pContainer->hwndStatus, SB_SETTEXT, 0, (LPARAM) fmt);
			}
		} else
			SendMessageA(dat->pContainer->hwndStatus, SB_SETTEXTA, 0, (LPARAM) "");
	}
	return 0;
}

/*
* save current keyboard layout for the given contact
*/

LRESULT TSAPI DM_SaveLocale(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	if (dat) {
		if (PluginConfig.m_AutoLocaleSupport && dat->hContact && dat->pContainer->hwndActive == dat->hwnd) {
			TCHAR szKLName[KL_NAMELENGTH + 1];
			if ((HKL)lParam != dat->hkl) {
				dat->hkl = (HKL)lParam;
				ActivateKeyboardLayout(dat->hkl, 0);
				GetKeyboardLayoutName(szKLName);
				M->WriteTString(dat->hContact, SRMSGMOD_T, "locale", szKLName);
				GetLocaleID(dat, szKLName);
				UpdateReadChars(dat);
			}
		}
	}
	return 0;
}

/*
* generic handler for the WM_COPY message in message log/chat history richedit control(s).
* it filters out the invisible event boundary markers from the text copied to the clipboard.
*/

LRESULT TSAPI DM_WMCopyHandler(HWND hwnd, WNDPROC oldWndProc, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CallWindowProc(oldWndProc, hwnd, WM_COPY, wParam, lParam);

	if (OpenClipboard(hwnd)) {
		HANDLE hClip = GetClipboardData(CF_UNICODETEXT);
		if (hClip) {
			HGLOBAL hgbl;
			TCHAR *tszLocked;
			TCHAR *tszText = (TCHAR *)malloc((lstrlen((TCHAR *)hClip) + 2) * sizeof(TCHAR));

			lstrcpy(tszText, (TCHAR *)hClip);
			Utils::FilterEventMarkers(tszText);
			EmptyClipboard();

			hgbl = GlobalAlloc(GMEM_MOVEABLE, (lstrlen(tszText) + 1) * sizeof(TCHAR));
			tszLocked = (TCHAR *)GlobalLock(hgbl);
			lstrcpy(tszLocked, tszText);
			GlobalUnlock(hgbl);
			SetClipboardData(CF_UNICODETEXT, hgbl);
			if (tszText)
				free(tszText);
		}
		CloseClipboard();
	}
	return result;
}

/*
* create embedded contact list control
*/

HWND TSAPI DM_CreateClist(TWindowData *dat)
{
	if(!sendLater->isAvail()) {
		CWarning::show(CWarning::WARN_NO_SENDLATER, MB_OK|MB_ICONINFORMATION, CTranslator::get(CTranslator::QMGR_ERROR_NOMULTISEND));
		dat->sendMode &= ~SMODE_MULTIPLE;
		return(0);
	}
	HWND hwndClist = CreateWindowExA(0, "CListControl", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | 0x248,
									 184, 0, 30, 30, dat->hwnd, (HMENU)IDC_CLIST, g_hInst, NULL);

	//MAD: fix for little bug, when following code didn't work (another hack :) )
	HANDLE hItem;
	SendMessage(hwndClist, WM_TIMER, 14, 0);
	//
	hItem = (HANDLE) SendMessage(hwndClist, CLM_FINDCONTACT, (WPARAM) dat->hContact, 0);

	SetWindowLongPtr(hwndClist, GWL_EXSTYLE, GetWindowLongPtr(hwndClist, GWL_EXSTYLE) & ~CLS_EX_TRACKSELECT);
	SetWindowLongPtr(hwndClist, GWL_EXSTYLE, GetWindowLongPtr(hwndClist, GWL_EXSTYLE) | (CLS_EX_NOSMOOTHSCROLLING | CLS_EX_NOTRANSLUCENTSEL));
	//MAD: show offline contacts in multi-send
	if (!PluginConfig.m_AllowOfflineMultisend)
		SetWindowLongPtr(hwndClist, GWL_STYLE, GetWindowLongPtr(hwndClist, GWL_STYLE) | CLS_HIDEOFFLINE);
	//
	if (hItem)
		SendMessage(hwndClist, CLM_SETCHECKMARK, (WPARAM) hItem, 1);

	if (CallService(MS_CLUI_GETCAPS, 0, 0) & CLUIF_DISABLEGROUPS && !M->GetByte("CList", "UseGroups", SETTING_USEGROUPS_DEFAULT))
		SendMessage(hwndClist, CLM_SETUSEGROUPS, (WPARAM) FALSE, 0);
	else
		SendMessage(hwndClist, CLM_SETUSEGROUPS, (WPARAM) TRUE, 0);
	if (CallService(MS_CLUI_GETCAPS, 0, 0) & CLUIF_HIDEEMPTYGROUPS && M->GetByte("CList", "HideEmptyGroups", SETTING_USEGROUPS_DEFAULT))
		SendMessage(hwndClist, CLM_SETHIDEEMPTYGROUPS, (WPARAM) TRUE, 0);
	else
		SendMessage(hwndClist, CLM_SETHIDEEMPTYGROUPS, (WPARAM) FALSE, 0);
	SendMessage(hwndClist, CLM_FIRST + 106, 0, 1);
	SendMessage(hwndClist, CLM_AUTOREBUILD, 0, 0);
	if(hwndClist)
		RedrawWindow(hwndClist, 0, 0, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
	return hwndClist;
}

LRESULT TSAPI DM_MouseWheelHandler(HWND hwnd, HWND hwndParent, struct TWindowData *mwdat, WPARAM wParam, LPARAM lParam)
{
	RECT rc, rc1;
	POINT pt;
	TCHITTESTINFO hti;
	HWND hwndTab;
	UINT uID = mwdat->bType == SESSIONTYPE_IM ? IDC_LOG : IDC_CHAT_LOG;
	UINT uIDMsg = mwdat->bType == SESSIONTYPE_IM ? IDC_MESSAGE : IDC_CHAT_MESSAGE;

	GetCursorPos(&pt);
	GetWindowRect(hwnd, &rc);
	if (PtInRect(&rc, pt))
		return 1;
	if (mwdat->pContainer->dwFlags & CNT_SIDEBAR) {
		GetWindowRect(GetDlgItem(mwdat->pContainer->hwnd, IDC_SIDEBARUP), &rc);
		GetWindowRect(GetDlgItem(mwdat->pContainer->hwnd, IDC_SIDEBARDOWN), &rc1);
		rc.bottom = rc1.bottom;
		if (PtInRect(&rc, pt)) {
			short amount = (short)(HIWORD(wParam));
			SendMessage(mwdat->pContainer->hwnd, WM_COMMAND, MAKELONG(amount > 0 ? IDC_SIDEBARUP : IDC_SIDEBARDOWN, 0), (LPARAM)uIDMsg);
			return 0;
		}
	}
	if (mwdat->bType == SESSIONTYPE_CHAT) {					// scroll nick list by just hovering it
		RECT	rcNicklist;
		GetWindowRect(GetDlgItem(mwdat->hwnd, IDC_LIST), &rcNicklist);
		if (PtInRect(&rcNicklist, pt)) {
			SendMessage(GetDlgItem(mwdat->hwnd, IDC_LIST), WM_MOUSEWHEEL, wParam, lParam);
			return(0);
		}
	}
	if (mwdat->hwndIEView)
		GetWindowRect(mwdat->hwndIEView, &rc);
	else if (mwdat->hwndHPP)
		GetWindowRect(mwdat->hwndHPP, &rc);
	else
		GetWindowRect(GetDlgItem(hwndParent, uID), &rc);
	if (PtInRect(&rc, pt)) {
		HWND hwnd = (mwdat->hwndIEView || mwdat->hwndHPP) ? mwdat->hwndIWebBrowserControl : GetDlgItem(hwndParent, uID);
		short wDirection = (short)HIWORD(wParam);

		if (hwnd == 0)
			hwnd = WindowFromPoint(pt);

		if (LOWORD(wParam) & MK_SHIFT || M->GetByte("fastscroll", 0)) {
			if (wDirection < 0)
				SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
			else if (wDirection > 0)
				SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
		} else
			SendMessage(hwnd, WM_MOUSEWHEEL, wParam, lParam);
		return 0;
	}
	hwndTab = GetDlgItem(mwdat->pContainer->hwnd, IDC_MSGTABS);
	GetCursorPos(&hti.pt);
	ScreenToClient(hwndTab, &hti.pt);
	hti.flags = 0;
	if (TabCtrl_HitTest(hwndTab, &hti) != -1) {
		SendMessage(hwndTab, WM_MOUSEWHEEL, wParam, -1);
		return 0;
	}
	return 1;
}

void TSAPI DM_FreeTheme(TWindowData *dat)
{
	if(dat) {
		if (CMimAPI::m_pfnCloseThemeData) {
			if(dat->hTheme) {
				CMimAPI::m_pfnCloseThemeData(dat->hTheme);
				dat->hTheme = 0;
			}
			if(dat->hThemeIP) {
				CMimAPI::m_pfnCloseThemeData(dat->hThemeIP);
				dat->hThemeIP = 0;
			}
			if(dat->hThemeToolbar) {
				CMimAPI::m_pfnCloseThemeData(dat->hThemeToolbar);
				dat->hThemeToolbar = 0;
			}
		}
	}
}

LRESULT TSAPI DM_ThemeChanged(TWindowData *dat)
{
	CSkinItem *item_log = &SkinItems[ID_EXTBKHISTORY];
	CSkinItem *item_msg = &SkinItems[ID_EXTBKINPUTAREA];

	HWND	hwnd = dat->hwnd;

	dat->hTheme = (M->isVSAPIState() && CMimAPI::m_pfnOpenThemeData) ? CMimAPI::m_pfnOpenThemeData(hwnd, L"EDIT") : 0;

	if (dat->bType == SESSIONTYPE_IM) {
		if (dat->hTheme != 0 || (CSkin::m_skinEnabled && !item_log->IGNORED))
			SetWindowLongPtr(GetDlgItem(hwnd, IDC_LOG), GWL_EXSTYLE, GetWindowLongPtr(GetDlgItem(hwnd, IDC_LOG), GWL_EXSTYLE) & ~WS_EX_STATICEDGE);
		if (dat->hTheme != 0 || (CSkin::m_skinEnabled && !item_msg->IGNORED))
			SetWindowLongPtr(GetDlgItem(hwnd, IDC_MESSAGE), GWL_EXSTYLE, GetWindowLongPtr(GetDlgItem(hwnd, IDC_MESSAGE), GWL_EXSTYLE) & ~WS_EX_STATICEDGE);
	} else {
		if (dat->hTheme != 0 || (CSkin::m_skinEnabled && !item_log->IGNORED)) {
			SetWindowLongPtr(GetDlgItem(hwnd, IDC_CHAT_LOG), GWL_EXSTYLE, GetWindowLongPtr(GetDlgItem(hwnd, IDC_CHAT_LOG), GWL_EXSTYLE) & ~WS_EX_STATICEDGE);
			SetWindowLongPtr(GetDlgItem(hwnd, IDC_LIST), GWL_EXSTYLE, GetWindowLongPtr(GetDlgItem(hwnd, IDC_LIST), GWL_EXSTYLE) & ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
		}
		if (dat->hTheme != 0 || (CSkin::m_skinEnabled && !item_msg->IGNORED))
			SetWindowLongPtr(GetDlgItem(hwnd, IDC_CHAT_MESSAGE), GWL_EXSTYLE, GetWindowLongPtr(GetDlgItem(hwnd, IDC_CHAT_MESSAGE), GWL_EXSTYLE) & ~WS_EX_STATICEDGE);
	}
	dat->hThemeIP = M->isAero() ? CMimAPI::m_pfnOpenThemeData(hwnd, L"ButtonStyle") : 0;
	dat->hThemeToolbar = (M->isAero() || (!CSkin::m_skinEnabled && M->isVSThemed())) ? CMimAPI::m_pfnOpenThemeData(hwnd, L"REBAR") : 0;

	return 0;
}

/**
 * send out message typing notifications (MTN) when the
 * user is typing/editing text in the messgae input area.
 */
void TSAPI DM_NotifyTyping(struct TWindowData *dat, int mode)
{
	DWORD 	protoStatus;
	DWORD 	protoCaps;
	DWORD 	typeCaps;
	const 	char* szProto = 0;
	HANDLE 	hContact = 0;

	if (dat && dat->hContact) {
		DeletePopupsForContact(dat->hContact, PU_REMOVE_ON_TYPE);

		if(dat->bIsMeta){
			szProto = dat->cache->getActiveProto();
			hContact = dat->cache->getActiveContact();
		}
		else {
			szProto = dat->szProto;
			hContact = dat->hContact;
		}

		/*
		 * editing user notes or preparing a message for queued delivery -> don't send MTN
		 */
		if(dat->fEditNotesActive || dat->sendMode & SMODE_SENDLATER)
			return;

		/*
		 * allow supression of sending out TN for the contact (NOTE: for metacontacts, do NOT use the subcontact handle)
		 */
		if (!M->GetByte(dat->hContact, SRMSGMOD, SRMSGSET_TYPING, M->GetByte(SRMSGMOD, SRMSGSET_TYPINGNEW, SRMSGDEFSET_TYPINGNEW)))
			return;

		if (!dat->szProto)			// should not, but who knows...
			return;

		/*
		 * check status and capabilities of the protocol
		 */
		protoStatus = CallProtoService(szProto, PS_GETSTATUS, 0, 0);
		protoCaps = CallProtoService(szProto, PS_GETCAPS, PFLAGNUM_1, 0);
		typeCaps = CallProtoService(szProto, PS_GETCAPS, PFLAGNUM_4, 0);

		if (!(typeCaps & PF4_SUPPORTTYPING))
			return;
		if (protoStatus < ID_STATUS_ONLINE)
			return;

		/*
		 * check visibility/invisibility lists to not "accidentially" send MTN to contacts who
		 * should not see them (privacy issue)
		 */
		if (protoCaps & PF1_VISLIST && DBGetContactSettingWord(hContact, szProto, "ApparentMode", 0) == ID_STATUS_OFFLINE)
			return;

		if (protoCaps & PF1_INVISLIST && protoStatus == ID_STATUS_INVISIBLE && DBGetContactSettingWord(hContact, szProto, "ApparentMode", 0) != ID_STATUS_ONLINE)
			return;

		/*
		 * don't send to contacts which are not permanently added to the contact list,
		 * unless the option to ignore added status is set.
		 */
		if (M->GetByte(dat->hContact, "CList", "NotOnList", 0)
				&& !M->GetByte(SRMSGMOD, SRMSGSET_TYPINGUNKNOWN, SRMSGDEFSET_TYPINGUNKNOWN))
			return;
		// End user check
		dat->nTypeMode = mode;
		CallService(MS_PROTO_SELFISTYPING, (WPARAM) hContact, dat->nTypeMode);
	}
}

void TSAPI DM_OptionsApplied(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	if(dat == 0)
		return;

	HWND 				hwndDlg = dat->hwnd;
	TContainerData *m_pContainer = dat->pContainer;

	dat->szMicroLf[0] = 0;
	if (!(dat->pContainer->theme.isPrivate)) {
		LoadThemeDefaults(dat->pContainer);
		dat->dwFlags = dat->pContainer->theme.dwFlags;
	}
	LoadLocalFlags(hwndDlg, dat);

	LoadTimeZone(dat);

	if (dat->hContact && dat->szProto != NULL && dat->bIsMeta) {
		DWORD dwForcedContactNum = 0;
		CallService(MS_MC_GETFORCESTATE, (WPARAM)dat->hContact, (LPARAM)&dwForcedContactNum);
		M->WriteDword(dat->hContact, SRMSGMOD_T, "tabSRMM_forced", dwForcedContactNum);
	}

	dat->showUIElements = m_pContainer->dwFlags & CNT_HIDETOOLBAR ? 0 : 1;

	dat->dwFlagsEx = M->GetByte(SRMSGSET_SHOWURLS, SRMSGDEFSET_SHOWURLS) ? MWF_SHOW_URLEVENTS : 0;
	dat->dwFlagsEx |= M->GetByte(SRMSGSET_SHOWFILES, SRMSGDEFSET_SHOWFILES) ? MWF_SHOW_FILEEVENTS : 0;
	dat->dwFlagsEx |= M->GetByte(dat->hContact, "splitoverride", 0) ? MWF_SHOW_SPLITTEROVERRIDE : 0;
	dat->Panel->getVisibility();

	// small inner margins (padding) for the text areas

	SendDlgItemMessage(hwndDlg, IDC_LOG, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(0, 0));
	SendDlgItemMessage(hwndDlg, IDC_MESSAGE, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(3, 3));

	GetSendFormat(dat, 1);
	SetDialogToType(hwndDlg);
	SendMessage(hwndDlg, DM_CONFIGURETOOLBAR, 0, 0);

	DM_InitRichEdit(dat);
	if (hwndDlg == m_pContainer->hwndActive)
		SendMessage(m_pContainer->hwnd, WM_SIZE, 0, 0);
	InvalidateRect(GetDlgItem(hwndDlg, IDC_MESSAGE), NULL, FALSE);
	if (!lParam)
		SendMessage(hwndDlg, DM_REMAKELOG, 0, 0);

	ShowWindow(dat->hwndPanelPicParent, PluginConfig.g_bDisableAniAvatars ? SW_HIDE : SW_SHOW);
	EnableWindow(dat->hwndPanelPicParent, PluginConfig.g_bDisableAniAvatars ? FALSE : TRUE);

	SendMessage(hwndDlg, DM_UPDATEWINICON, 0, 0);
}


void TSAPI DM_Typing(TWindowData *dat, bool fForceOff)
{
	if(dat == 0)
		return;

	HWND	hwndDlg = dat->hwnd;
	HWND    hwndContainer = dat->pContainer->hwnd;
	HWND	hwndStatus = dat->pContainer->hwndStatus;

	if (dat->nTypeMode == PROTOTYPE_SELFTYPING_ON && GetTickCount() - dat->nLastTyping > TIMEOUT_TYPEOFF) {
		DM_NotifyTyping(dat, PROTOTYPE_SELFTYPING_OFF);
	}
	if (dat->showTyping == 1) {
		if (dat->nTypeSecs > 0) {
			dat->nTypeSecs--;
			if (GetForegroundWindow() == hwndContainer)
				SendMessage(hwndDlg, DM_UPDATEWINICON, 0, 0);
		} else {
			struct TWindowData *dat_active = NULL;
			
			if(!fForceOff) {
				dat->showTyping = 2;
				dat->nTypeSecs = 86400;

				mir_sntprintf(dat->szStatusBar, safe_sizeof(dat->szStatusBar),
						  CTranslator::get(CTranslator::GEN_MTN_STOPPED), dat->cache->getNick());
				if(hwndStatus && dat->pContainer->hwndActive == hwndDlg)
					SendMessage(hwndStatus, SB_SETTEXT, 0, (LPARAM) dat->szStatusBar);
			}
			SendMessage(hwndDlg, DM_UPDATEWINICON, 0, 0);
			HandleIconFeedback(dat, (HICON) - 1);
			dat_active = (struct TWindowData *)GetWindowLongPtr(dat->pContainer->hwndActive, GWLP_USERDATA);
			if (dat_active && dat_active->bType == SESSIONTYPE_IM)
				SendMessage(hwndContainer, DM_UPDATETITLE, 0, 0);
			else
				SendMessage(hwndContainer, DM_UPDATETITLE, (WPARAM)dat->pContainer->hwndActive, (LPARAM)1);
			if (!(dat->pContainer->dwFlags & CNT_NOFLASH) && PluginConfig.m_FlashOnMTN)
				ReflashContainer(dat->pContainer);
		}
	} else if(dat->showTyping == 2) {
		if (dat->nTypeSecs > 0)
			dat->nTypeSecs--;
		else {
			dat->szStatusBar[0] = 0;
			dat->showTyping = 0;
		}
		UpdateStatusBar(dat);
	}
	else {
		if (dat->nTypeSecs > 0) {
			mir_sntprintf(dat->szStatusBar, safe_sizeof(dat->szStatusBar), CTranslator::get(CTranslator::GEN_MTN_STARTWITHNICK), dat->cache->getNick());

			dat->nTypeSecs--;
			if (hwndStatus && dat->pContainer->hwndActive == hwndDlg) {
				SendMessage(hwndStatus, SB_SETTEXT, 0, (LPARAM) dat->szStatusBar);
				SendMessage(hwndStatus, SB_SETICON, 0, (LPARAM) PluginConfig.g_buttonBarIcons[ICON_DEFAULT_TYPING]);
			}
			if (IsIconic(hwndContainer) || GetForegroundWindow() != hwndContainer || GetActiveWindow() != hwndContainer) {
				SetWindowText(hwndContainer, dat->szStatusBar);
				dat->pContainer->dwFlags |= CNT_NEED_UPDATETITLE;
				if (!(dat->pContainer->dwFlags & CNT_NOFLASH) && PluginConfig.m_FlashOnMTN)
					ReflashContainer(dat->pContainer);
			}
			if (dat->pContainer->hwndActive != hwndDlg) {
				if (dat->mayFlashTab)
					dat->iFlashIcon = PluginConfig.g_IconTypingEvent;
				HandleIconFeedback(dat, PluginConfig.g_IconTypingEvent);
			} else {         // active tab may show icon if status bar is disabled
				if (!hwndStatus) {
					if (TabCtrl_GetItemCount(GetParent(hwndDlg)) > 1 || !(dat->pContainer->dwFlags & CNT_HIDETABS)) {
						HandleIconFeedback(dat, PluginConfig.g_IconTypingEvent);
					}
				}
			}
			if ((GetForegroundWindow() != hwndContainer) || (dat->pContainer->hwndStatus == 0) || (dat->pContainer->hwndActive != hwndDlg))
				SendMessage(hwndContainer, DM_SETICON, (WPARAM)dat, (LPARAM) PluginConfig.g_buttonBarIcons[ICON_DEFAULT_TYPING]);
			dat->showTyping = 1;
		}
	}
}

/**
 * sync splitter position for all open sessions.
 * This cares about private / per container / MUC <> IM splitter syncing and everything.
 * called from IM and MUC windows via DM_SPLITTERGLOBALEVENT
 */
int TSAPI DM_SplitterGlobalEvent(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	RECT 			rcWin;
	short 			newMessagePos;
	LONG			newPos;
	TWindowData*	srcDat = PluginConfig.lastSPlitterPos.pSrcDat;
	TContainerData*	srcCnt = PluginConfig.lastSPlitterPos.pSrcContainer;
	bool			fCntGlobal = (!dat->pContainer->settings->fPrivate ? true : false);

#if defined(__FEAT_EXP_AUTOSPLITTER)
	if(dat->fIsAutosizingInput)
		return(0);
#endif

	GetWindowRect(dat->hwnd, &rcWin);

	if(wParam == 0 && lParam == 0) {
		if((dat->dwFlagsEx & MWF_SHOW_SPLITTEROVERRIDE) && dat != srcDat)
			return(0);

		if(srcDat->bType == dat->bType)
			newPos = PluginConfig.lastSPlitterPos.pos;
		else if(srcDat->bType == SESSIONTYPE_IM && dat->bType == SESSIONTYPE_CHAT)
			newPos = PluginConfig.lastSPlitterPos.pos + PluginConfig.lastSPlitterPos.off_im;
		else if(srcDat->bType == SESSIONTYPE_CHAT && dat->bType == SESSIONTYPE_IM)
			newPos = PluginConfig.lastSPlitterPos.pos + PluginConfig.lastSPlitterPos.off_im;

		if(dat == srcDat) {
			if(dat->bType == SESSIONTYPE_IM) {
				dat->pContainer->settings->splitterPos = dat->splitterY;
				if(fCntGlobal) {
					SaveSplitter(dat);
					if(PluginConfig.lastSPlitterPos.bSync)
						g_Settings.iSplitterY = dat->splitterY - DPISCALEY_S(23);
				}
			}
			if(dat->bType == SESSIONTYPE_CHAT) {
				SESSION_INFO *si = dat->si;
				if(si) {
					dat->pContainer->settings->splitterPos = si->iSplitterY + DPISCALEY_S(23);
					if(fCntGlobal) {
						g_Settings.iSplitterY = si->iSplitterY;
						if(PluginConfig.lastSPlitterPos.bSync)
							M->WriteDword(SRMSGMOD_T, "splitsplity", (DWORD)si->iSplitterY + DPISCALEY_S(23));
					}
				}
			}
			return(0);
		}

		if(!fCntGlobal && dat->pContainer != srcCnt)
			return(0);
		if(srcCnt->settings->fPrivate && dat->pContainer != srcCnt)
			return(0);

		if(!PluginConfig.lastSPlitterPos.bSync && dat->bType != srcDat->bType)
			return(0);

		/*
		 * for inactive sessions, delay the splitter repositioning until they become
		 * active (faster, avoid redraw/resize problems for minimized windows)
		 */
		if (IsIconic(dat->pContainer->hwnd) || dat->pContainer->hwndActive != dat->hwnd) {
			dat->dwFlagsEx |= MWF_EX_DELAYEDSPLITTER;
			dat->wParam = newPos;
			dat->lParam = PluginConfig.lastSPlitterPos.lParam;
			return(0);
		}
	}
	else
		newPos = wParam;

	newMessagePos = (short)rcWin.bottom - (short)newPos;

	if(dat->bType == SESSIONTYPE_IM) {
		LoadSplitter(dat);
		AdjustBottomAvatarDisplay(dat);
		DM_RecalcPictureSize(dat);
		SendMessage(dat->hwnd, WM_SIZE, 0, 0);
		DM_ScrollToBottom(dat, 1,1);
		if(dat != srcDat)
			CSkin::UpdateToolbarBG(dat);
	}
	else {
		SESSION_INFO *si = dat->si;
		if(si) {
			si->iSplitterY = g_Settings.iSplitterY;
			SendMessage(dat->hwnd, WM_SIZE, 0, 0);
		}
	}
	return(0);
}

/**
 * incoming event handler
 */

void TSAPI DM_EventAdded(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	TContainerData *m_pContainer = dat->pContainer;
	DBEVENTINFO 		dbei = {0};
	DWORD 				dwTimestamp = 0;
	BOOL  				fIsStatusChangeEvent = FALSE, fIsNotifyEvent = FALSE;
	HWND				hwndDlg = dat->hwnd, hwndContainer = m_pContainer->hwnd, hwndTab = GetParent(dat->hwnd);

	dbei.cbSize = sizeof(dbei);
	dbei.cbBlob = 0;

	CallService(MS_DB_EVENT_GET, lParam, (LPARAM) & dbei);
	if (dat->hDbEventFirst == NULL)
		dat->hDbEventFirst = (HANDLE) lParam;

	fIsStatusChangeEvent = IsStatusEvent(dbei.eventType);
	fIsNotifyEvent = (dbei.eventType == EVENTTYPE_MESSAGE || dbei.eventType == EVENTTYPE_FILE);

	if (!fIsStatusChangeEvent) {
		int heFlags = HistoryEvents_GetFlags(dbei.eventType);
		if (heFlags != -1 && !(heFlags & HISTORYEVENTS_FLAG_DEFAULT) && !(heFlags & HISTORYEVENTS_FLAG_FLASH_MSG_WINDOW))
			fIsStatusChangeEvent = TRUE;
	}

	if (dbei.eventType == EVENTTYPE_MESSAGE && (dbei.flags & DBEF_READ))
		return;

	if (DbEventIsShown(dat, &dbei)) {
		if (dbei.eventType == EVENTTYPE_MESSAGE && !(dbei.flags & (DBEF_SENT))) {
			dat->lastMessage = dbei.timestamp;
			dat->szStatusBar[0] = 0;
			if(dat->showTyping) {
				dat->nTypeSecs = 0;
				DM_Typing(dat, true);
				dat->showTyping = 0;
			}
			HandleIconFeedback(dat, (HICON)-1);
			if(m_pContainer->hwndStatus)
				PostMessage(hwndDlg, DM_UPDATELASTMESSAGE, 0, 0);
		}
		/*
		* set the message log divider to mark new (maybe unseen) messages, if the container has
		* been minimized or in the background.
		*/
		if (!(dbei.flags & DBEF_SENT) && !fIsStatusChangeEvent) {

			if (PluginConfig.m_DividersUsePopupConfig && PluginConfig.m_UseDividers) {
				if (!MessageWindowOpened((WPARAM)dat->hContact, 0))
					SendMessage(hwndDlg, DM_ADDDIVIDER, 0, 0);
			} else if (PluginConfig.m_UseDividers) {
				if ((GetForegroundWindow() != hwndContainer || GetActiveWindow() != hwndContainer))
					SendMessage(hwndDlg, DM_ADDDIVIDER, 0, 0);
				else {
					if (m_pContainer->hwndActive != hwndDlg)
						SendMessage(hwndDlg, DM_ADDDIVIDER, 0, 0);
				}
			}
			tabSRMM_ShowPopup(wParam, lParam, dbei.eventType, m_pContainer->fHidden ? 0 : 1, m_pContainer, hwndDlg, dat->cache->getActiveProto(), dat);
			if(IsWindowVisible(m_pContainer->hwnd))
				m_pContainer->fHidden = false;
		}
		dat->cache->updateStats(TSessionStats::UPDATE_WITH_LAST_RCV, 0);

		if ((HANDLE) lParam != dat->hDbEventFirst) {
			HANDLE nextEvent = (HANDLE) CallService(MS_DB_EVENT_FINDNEXT, lParam, 0);
			if (1 || nextEvent == 0) {
				if (!(dat->dwFlagsEx & MWF_SHOW_SCROLLINGDISABLED))
					SendMessage(hwndDlg, DM_APPENDTOLOG, lParam, 0);
				else {
					TCHAR szBuf[100];

					if (dat->iNextQueuedEvent >= dat->iEventQueueSize) {
						dat->hQueuedEvents = (HANDLE *)realloc(dat->hQueuedEvents, (dat->iEventQueueSize + 10) * sizeof(HANDLE));
						dat->iEventQueueSize += 10;
					}
					dat->hQueuedEvents[dat->iNextQueuedEvent++] = (HANDLE)lParam;
					mir_sntprintf(szBuf, safe_sizeof(szBuf), CTranslator::get(CTranslator::GEN_MSG_LOGFROZENQUEUED),
								  dat->iNextQueuedEvent);
					SetDlgItemText(hwndDlg, IDC_LOGFROZENTEXT, szBuf);
					RedrawWindow(GetDlgItem(hwndDlg, IDC_LOGFROZENTEXT), NULL, NULL, RDW_INVALIDATE);
				}
			} else
				SendMessage(hwndDlg, DM_REMAKELOG, 0, 0);
		} else
			SendMessage(hwndDlg, DM_REMAKELOG, 0, 0);

		// handle tab flashing

		if ((TabCtrl_GetCurSel(hwndTab) != dat->iTabID) && !(dbei.flags & DBEF_SENT) && !fIsStatusChangeEvent) {
			switch (dbei.eventType) {
				case EVENTTYPE_MESSAGE:
					dat->iFlashIcon = PluginConfig.g_IconMsgEvent;
					break;
				case EVENTTYPE_FILE:
					dat->iFlashIcon = PluginConfig.g_IconFileEvent;
					break;
				default:
					dat->iFlashIcon = PluginConfig.g_IconMsgEvent;
					break;
			}
			SetTimer(hwndDlg, TIMERID_FLASHWND, TIMEOUT_FLASHWND, NULL);
			dat->mayFlashTab = TRUE;
		}
		/*
		* try to flash the contact list...
		*/

		FlashOnClist(hwndDlg, dat, (HANDLE)lParam, &dbei);
		/*
		* autoswitch tab if option is set AND container is minimized (otherwise, we never autoswitch)
		* never switch for status changes...
		*/
		if (!(dbei.flags & DBEF_SENT) && !fIsStatusChangeEvent) {
			if(PluginConfig.haveAutoSwitch() && m_pContainer->hwndActive != hwndDlg) {
				if ((IsIconic(hwndContainer) && !IsZoomed(hwndContainer)) || (PluginConfig.m_HideOnClose && !IsWindowVisible(m_pContainer->hwnd))) {
					int iItem = GetTabIndexFromHWND(GetParent(hwndDlg), hwndDlg);
					if (iItem >= 0) {
						TabCtrl_SetCurSel(GetParent(hwndDlg), iItem);
						ShowWindow(m_pContainer->hwndActive, SW_HIDE);
						m_pContainer->hwndActive = hwndDlg;
						SendMessage(hwndContainer, DM_UPDATETITLE, (WPARAM)dat->hContact, 0);
						m_pContainer->dwFlags |= CNT_DEFERREDTABSELECT;
					}
				}
			}
		}
		/*
		* flash window if it is not focused
		*/
		if ((GetActiveWindow() != hwndContainer || GetForegroundWindow() != hwndContainer || dat->pContainer->hwndActive != hwndDlg) && !(dbei.flags & DBEF_SENT) && !fIsStatusChangeEvent) {
			if (!(m_pContainer->dwFlags & CNT_NOFLASH) && (GetActiveWindow() != hwndContainer || GetForegroundWindow() != hwndContainer))
				FlashContainer(m_pContainer, 1, 0);
			SendMessage(hwndContainer, DM_SETICON, (WPARAM)dat, (LPARAM)LoadSkinnedIcon(SKINICON_EVENT_MESSAGE));
			m_pContainer->dwFlags |= CNT_NEED_UPDATETITLE;
		}
		/*
		* play a sound
		*/
		if (dbei.eventType == EVENTTYPE_MESSAGE && !(dbei.flags & (DBEF_SENT)))
			PostMessage(hwndDlg, DM_PLAYINCOMINGSOUND, 0, 0);

		if(dat->pWnd)
			dat->pWnd->Invalidate();
	}
}

void TSAPI DM_HandleAutoSizeRequest(TWindowData *dat, REQRESIZE* rr)
{
	if(dat && rr && GetForegroundWindow() == dat->pContainer->hwnd) {
		if(dat->fIsAutosizingInput && dat->iInputAreaHeight != -1) {
			LONG heightLimit = M->GetDword("autoSplitMinLimit", 0);
			LONG iNewHeight = rr->rc.bottom - rr->rc.top;

			if(CSkin::m_skinEnabled && !SkinItems[ID_EXTBKINPUTAREA].IGNORED) 
				iNewHeight += (SkinItems[ID_EXTBKINPUTAREA].MARGIN_TOP + SkinItems[ID_EXTBKINPUTAREA].MARGIN_BOTTOM - 2);

			if(heightLimit && iNewHeight < heightLimit)
				iNewHeight = heightLimit;

			if(iNewHeight != dat->iInputAreaHeight) {
				RECT	rc;

				GetClientRect(dat->hwnd, &rc);
				LONG cy = rc.bottom - rc.top;
				LONG panelHeight = (dat->Panel->isActive() ? dat->Panel->getHeight() : 0);

				if(iNewHeight > (cy - panelHeight) / 2)
					iNewHeight = (cy - panelHeight) / 2;

				if(dat->bType == SESSIONTYPE_IM) {
					dat->dynaSplitter = rc.bottom - (rc.bottom - iNewHeight + DPISCALEY_S(2));
					if(dat->pContainer->dwFlags & CNT_BOTTOMTOOLBAR)
						dat->dynaSplitter += DPISCALEY_S(22);
					dat->splitterY = dat->dynaSplitter + DPISCALEY_S(34);
					DM_RecalcPictureSize(dat);
				}
				else if (dat->si) {
					dat->si->iSplitterY = (rc.bottom - (rc.bottom - iNewHeight + DPISCALEY_S(3))) + DPISCALEY_S(34);
					if(!(dat->pContainer->dwFlags & CNT_BOTTOMTOOLBAR))
						dat->si->iSplitterY -= DPISCALEY_S(22);
					SendMessage(dat->hwnd, WM_SIZE, 0, 0);
				}
				dat->iInputAreaHeight = iNewHeight;
				CSkin::UpdateToolbarBG(dat);
				DM_ScrollToBottom(dat, 1, 0);
			}
		}
	}
}

void TSAPI DM_UpdateTitle(TWindowData *dat, WPARAM wParam, LPARAM lParam)
{
	TCHAR 					newtitle[128];
	TCHAR*					pszNewTitleEnd;
	TCHAR 					newcontactname[128];
	TCITEM 					item;
	DWORD 					dwOldIdle = dat->idle;
	const char*				szActProto = 0;
	HANDLE 					hActContact = 0;

	HWND 					hwndDlg = dat->hwnd;
	HWND					hwndTab = GetParent(hwndDlg);
	HWND					hwndContainer = dat->pContainer->hwnd;
	TContainerData*	m_pContainer = dat->pContainer;

	ZeroMemory((void *)newcontactname,  sizeof(newcontactname));
	dat->szStatus[0] = 0;

	pszNewTitleEnd = _T("Message Session");

	if (dat->iTabID == -1)
		return;

	ZeroMemory((void *)&item, sizeof(item));
	if (dat->hContact) {
		int 			iHasName;
		TCHAR 			fulluin[256];
		const TCHAR*	szNick = dat->cache->getNick();

		if (dat->szProto) {

			szActProto = dat->cache->getActiveProto();
			hActContact = dat->hContact;

			iHasName = (dat->cache->getUIN()[0] != 0);
			dat->idle = dat->cache->getIdleTS();
			dat->dwFlagsEx =  dat->idle ? dat->dwFlagsEx | MWF_SHOW_ISIDLE : dat->dwFlagsEx & ~MWF_SHOW_ISIDLE;

			dat->wStatus = dat->cache->getStatus();
			mir_sntprintf(dat->szStatus, safe_sizeof(dat->szStatus), _T("%s"), (char *) CallService(MS_CLIST_GETSTATUSMODEDESCRIPTION, dat->szProto == NULL ? ID_STATUS_OFFLINE : dat->wStatus, GCMDF_TCHAR));

			if (lParam != 0) {
				if (PluginConfig.m_CutContactNameOnTabs)
					CutContactName(szNick, newcontactname, safe_sizeof(newcontactname));
				else
					lstrcpyn(newcontactname, szNick, safe_sizeof(newcontactname));

				Utils::DoubleAmpersands(newcontactname);

				if (lstrlen(newcontactname) != 0 && dat->szStatus != NULL) {
					if (PluginConfig.m_StatusOnTabs)
						mir_sntprintf(newtitle, 127, _T("%s (%s)"), newcontactname, dat->szStatus);
					else
						mir_sntprintf(newtitle, 127, _T("%s"), newcontactname);
				} else
					mir_sntprintf(newtitle, 127, _T("%s"), _T("Forward"));

				item.mask |= TCIF_TEXT;
			}
			SendMessage(hwndDlg, DM_UPDATEWINICON, 0, 0);
			if (dat->bIsMeta)
				mir_sntprintf(fulluin, safe_sizeof(fulluin),
							  CTranslator::get(CTranslator::GEN_MSG_UINCOPY),
							  iHasName ? dat->cache->getUIN() : CTranslator::get(CTranslator::GEN_MSG_NOUIN));
			else
				mir_sntprintf(fulluin, safe_sizeof(fulluin),
							  CTranslator::get(CTranslator::GEN_MSG_UINCOPY_NOMC),
							  iHasName ? dat->cache->getUIN() : CTranslator::get(CTranslator::GEN_MSG_NOUIN));

			SendMessage(GetDlgItem(hwndDlg, IDC_NAME), BUTTONADDTOOLTIP, /*iHasName ?*/ (WPARAM)fulluin /*: (WPARAM)_T("")*/, 0);
		}
	} else
		lstrcpyn(newtitle, pszNewTitleEnd, safe_sizeof(newtitle));

	if (dat->idle != dwOldIdle || lParam != 0) {

		if (item.mask & TCIF_TEXT) {
			item.pszText = newtitle;
			_tcsncpy(dat->newtitle, newtitle, safe_sizeof(dat->newtitle));
			dat->newtitle[127] = 0;
			item.cchTextMax = 127;
			if(dat->pWnd)
				dat->pWnd->updateTitle(dat->cache->getNick());
		}
		if (dat->iTabID  >= 0) {
			TabCtrl_SetItem(hwndTab, dat->iTabID, &item);
			if(m_pContainer->dwFlags & CNT_SIDEBAR)
				m_pContainer->SideBar->updateSession(dat);
		}
		if (m_pContainer->hwndActive == hwndDlg && lParam)
			SendMessage(hwndContainer, DM_UPDATETITLE, (WPARAM)dat->hContact, 0);

		UpdateTrayMenuState(dat, TRUE);
		if (dat->cache->isFavorite())
			AddContactToFavorites(dat->hContact, dat->cache->getNick(), szActProto, dat->szStatus, dat->wStatus,
								  LoadSkinnedProtoIcon(dat->cache->getActiveProto(), dat->cache->getActiveStatus()), 0, PluginConfig.g_hMenuFavorites);
		if (dat->cache->isRecent()) {
			AddContactToFavorites(dat->hContact, dat->cache->getNick(), szActProto, dat->szStatus, dat->wStatus,
								  LoadSkinnedProtoIcon(dat->cache->getActiveProto(), dat->cache->getActiveStatus()), 0, PluginConfig.g_hMenuRecent);
		}

		dat->Panel->Invalidate();
		if(dat->pWnd)
			dat->pWnd->Invalidate();

		if (PluginConfig.g_FlashAvatarAvail) {
			FLASHAVATAR fa = {0};

			fa.hContact = dat->hContact;
			fa.hWindow = 0;
			fa.id = 25367;
			fa.cProto = dat->szProto;

			CallService(MS_FAVATAR_GETINFO, (WPARAM)&fa, 0);
			dat->hwndFlash = fa.hWindow;
			if (dat->hwndFlash) {
				bool isInfoPanel = dat->Panel->isActive();
				SetParent(dat->hwndFlash, isInfoPanel ? dat->hwndPanelPicParent : GetDlgItem(hwndDlg, IDC_CONTACTPIC));
			}
		}
	}
	// care about MetaContacts and update the statusbar icon with the currently "most online" contact...
	if (dat->bIsMeta) {
		PostMessage(hwndDlg, DM_UPDATEMETACONTACTINFO, 0, 0);
		PostMessage(hwndDlg, DM_OWNNICKCHANGED, 0, 0);
		if (m_pContainer->dwFlags & CNT_UINSTATUSBAR)
			DM_UpdateLastMessage(dat);
	}
}

/*
* status icon stuff (by sje, used for indicating encryption status in the status bar
* this is now part of the message window api
*/

static HANDLE hHookIconPressedEvt;
struct TStatusBarIconNode *status_icon_list = 0;
int status_icon_list_size = 0;

static INT_PTR SI_AddStatusIcon(WPARAM wParam, LPARAM lParam)
{
	StatusIconData *sid = (StatusIconData *)lParam;
	struct TStatusBarIconNode *siln = (struct TStatusBarIconNode *)mir_alloc(sizeof(struct TStatusBarIconNode));

	siln->sid.cbSize = sid->cbSize;
	siln->sid.szModule = mir_strdup(sid->szModule);
	siln->sid.dwId = sid->dwId;
	siln->sid.hIcon = sid->hIcon;
	siln->sid.hIconDisabled = sid->hIconDisabled;
	siln->sid.flags = sid->flags;
	if (sid->szTooltip) siln->sid.szTooltip = mir_strdup(sid->szTooltip);
	else siln->sid.szTooltip = 0;

	siln->next = status_icon_list;
	status_icon_list = siln;
	status_icon_list_size++;

	M->BroadcastMessage(DM_STATUSICONCHANGE, 0, 0);
	return 0;
}

static INT_PTR SI_RemoveStatusIcon(WPARAM wParam, LPARAM lParam)
{
	StatusIconData *sid = (StatusIconData *)lParam;
	struct TStatusBarIconNode *current = status_icon_list, *prev = 0;

	while (current) {
		if (strcmp(current->sid.szModule, sid->szModule) == 0 && current->sid.dwId == sid->dwId) {
			if (prev) prev->next = current->next;
			else status_icon_list = current->next;

			status_icon_list_size--;

			mir_free(current->sid.szModule);
			DestroyIcon(current->sid.hIcon);
			if (current->sid.hIconDisabled) DestroyIcon(current->sid.hIconDisabled);
			if (current->sid.szTooltip) mir_free(current->sid.szTooltip);
			mir_free(current);
			M->BroadcastMessage(DM_STATUSICONCHANGE, 0, 0);
			return 0;
		}

		prev = current;
		current = current->next;
	}
	return 1;
}

static void SI_RemoveAllStatusIcons(void)
{
	struct TStatusBarIconNode *current;

	while (status_icon_list) {
		current = status_icon_list;
		status_icon_list = status_icon_list->next;
		status_icon_list_size--;

		mir_free(current->sid.szModule);
		DestroyIcon(current->sid.hIcon);
		if (current->sid.hIconDisabled) DestroyIcon(current->sid.hIconDisabled);
		if (current->sid.szTooltip) mir_free(current->sid.szTooltip);
		mir_free(current);
	}
	M->BroadcastMessage(DM_STATUSICONCHANGE, 0, 0);
}

static INT_PTR SI_ModifyStatusIcon(WPARAM wParam, LPARAM lParam)
{
	HANDLE hContact = (HANDLE)wParam;

	StatusIconData *sid = (StatusIconData *)lParam;
	struct TStatusBarIconNode *current = status_icon_list;

	while (current) {
		if (strcmp(current->sid.szModule, sid->szModule) == 0 && current->sid.dwId == sid->dwId) {
			if (!hContact) {
				current->sid.flags = sid->flags;
				if (sid->hIcon) {
					DestroyIcon(current->sid.hIcon);
					current->sid.hIcon = sid->hIcon;
				}
				if (sid->hIconDisabled) {
					DestroyIcon(current->sid.hIconDisabled);
					current->sid.hIconDisabled = sid->hIconDisabled;
				}
				if (sid->szTooltip) {
					if (current->sid.szTooltip) mir_free(current->sid.szTooltip);
					current->sid.szTooltip = mir_strdup(sid->szTooltip);
				}

				M->BroadcastMessage(DM_STATUSICONCHANGE, 0, 0);
			} else {
				char buff[256];
				HWND hwnd;
				if (!(sid->flags&MBF_OWNERSTATE)) {
					sprintf(buff, "SRMMStatusIconFlags%d", (int)sid->dwId);
					M->WriteByte(hContact, sid->szModule, buff, (BYTE)sid->flags);
				}
				if ((hwnd = M->FindWindow(hContact))) {
					if (sid->flags&MBF_OWNERSTATE) {

						struct TStatusBarIconNode *siln = NULL;
						struct TWindowData *dat = (struct TWindowData *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
						struct TStatusBarIconNode *psi = dat->pSINod;
						while (psi) {
							if (strcmp(psi->sid.szModule, sid->szModule) == 0 && psi->sid.dwId == sid->dwId) {
								siln = psi;
								break;
							}
							psi = psi->next;
						}
						if (!siln) {
							siln = (struct TStatusBarIconNode *)mir_alloc(sizeof(struct TStatusBarIconNode));
							siln->sid.szModule = mir_strdup(sid->szModule);
							siln->sid.dwId = sid->dwId;
							siln->sid.hIcon = sid->hIcon;
							siln->sid.hIconDisabled = sid->hIconDisabled;
							siln->sid.flags = sid->flags;

							if (sid->szTooltip) siln->sid.szTooltip = mir_strdup(sid->szTooltip);
							else siln->sid.szTooltip = 0;

							siln->next = dat->pSINod;
							dat->pSINod = siln;
						} else {
							siln->sid.hIcon = sid->hIcon;
							siln->sid.hIconDisabled = sid->hIconDisabled;
							siln->sid.flags = sid->flags;
							if (siln->sid.szTooltip) mir_free(siln->sid.szTooltip);

							if (sid->szTooltip) siln->sid.szTooltip = mir_strdup(sid->szTooltip);
							else siln->sid.szTooltip = 0;

						}


						PostMessage(hwnd, DM_STATUSICONCHANGE, 0, 0);
					} else
						PostMessage(hwnd, DM_STATUSICONCHANGE, 0, 0);
				}
			}
			return 0;
		}
		current = current->next;
	}
	return 1;
}

void DrawStatusIcons(struct TWindowData *dat, HDC hDC, RECT r, int gap)
{
	TStatusBarIconNode*	current = status_icon_list;
	HICON 				hIcon = NULL;
	char 				buff[256];
	int 				flags;
	int 				x = r.left;
	LONG 				cx_icon = PluginConfig.m_smcxicon;
	LONG				cy_icon = PluginConfig.m_smcyicon;
	LONG				y = (r.top + r.bottom - cx_icon) >> 1;

	SetBkMode(hDC, TRANSPARENT);
	while (current) {
		if (current->sid.flags&MBF_OWNERSTATE) {
			struct TStatusBarIconNode *currentSIN = dat->pSINod;
			flags = current->sid.flags;
			hIcon = current->sid.hIcon;
			while (currentSIN) {
				if (strcmp(currentSIN->sid.szModule, current->sid.szModule) == 0 && currentSIN->sid.dwId == current->sid.dwId) {
					flags = currentSIN->sid.flags;
					hIcon = currentSIN->sid.hIcon;
					break;
				}
				currentSIN = currentSIN->next;
			}
		} else {
			sprintf(buff, "SRMMStatusIconFlags%d", (int)current->sid.dwId);
			flags = M->GetByte(dat->hContact, current->sid.szModule, buff, current->sid.flags);
		}

		if (!(flags & MBF_HIDDEN)) {
			if (!(flags&MBF_OWNERSTATE) && (flags & MBF_DISABLED) && current->sid.hIconDisabled)
				hIcon = current->sid.hIconDisabled;
			else if (!(flags&MBF_OWNERSTATE))
				hIcon = current->sid.hIcon;

			if (flags & MBF_DISABLED && current->sid.hIconDisabled == (HICON)0)
				CSkin::DrawDimmedIcon(hDC, x, y, cx_icon, cy_icon, hIcon, 50);
			else
				DrawIconEx(hDC, x, y, hIcon, 16, 16, 0, NULL, DI_NORMAL);

			x += 16 + gap;
		}
		current = current->next;
	}
	DrawIconEx(hDC, x, y, PluginConfig.g_buttonBarIcons[ICON_DEFAULT_SOUNDS],
			   cx_icon, cy_icon, 0, NULL, DI_NORMAL);

	DrawIconEx(hDC, x, y, dat->pContainer->dwFlags & CNT_NOSOUND ?
			   PluginConfig.g_iconOverlayDisabled : PluginConfig.g_iconOverlayEnabled,
			   cx_icon, cy_icon, 0, NULL, DI_NORMAL);

	x += (cx_icon + gap);

	if (dat->bType == SESSIONTYPE_IM) {
		DrawIconEx(hDC, x, y,
				   PluginConfig.g_buttonBarIcons[ICON_DEFAULT_TYPING], cx_icon, cy_icon, 0, NULL, DI_NORMAL);
		DrawIconEx(hDC, x, y, M->GetByte(dat->hContact, SRMSGMOD, SRMSGSET_TYPING, M->GetByte(SRMSGMOD, SRMSGSET_TYPINGNEW, SRMSGDEFSET_TYPINGNEW)) ?
				   PluginConfig.g_iconOverlayEnabled : PluginConfig.g_iconOverlayDisabled, cx_icon, cy_icon, 0, NULL, DI_NORMAL);
	}
	else
		CSkin::DrawDimmedIcon(hDC, x, y, cx_icon, cy_icon,
					   PluginConfig.g_buttonBarIcons[ICON_DEFAULT_TYPING], 50);

	x += (cx_icon + gap);
	DrawIconEx(hDC, x, y, PluginConfig.g_sideBarIcons[0], cx_icon, cy_icon, 0, NULL, DI_NORMAL);
}

void SI_CheckStatusIconClick(struct TWindowData *dat, HWND hwndFrom, POINT pt, RECT r, int gap, int code)
{
	StatusIconClickData sicd;
	struct TStatusBarIconNode *current = status_icon_list;
	struct TStatusBarIconNode *clicked = NULL;

	unsigned int iconNum = (pt.x - (r.left + 0)) / (PluginConfig.m_smcxicon + gap);
	unsigned int list_icons = 0;
	char         buff[100];
	DWORD		 flags;

	if (dat && (code == NM_CLICK || code == NM_RCLICK)) {
		POINT	ptScreen;

		GetCursorPos(&ptScreen);
		if (!PtInRect(&rcLastStatusBarClick, ptScreen))
			return;
	}
	while (current && dat) {
		if (current->sid.flags&MBF_OWNERSTATE) {
			struct TStatusBarIconNode *currentSIN = dat->pSINod;
			flags = current->sid.flags;
			while (currentSIN) {
				if (strcmp(currentSIN->sid.szModule, current->sid.szModule) == 0 && currentSIN->sid.dwId == current->sid.dwId) {
					flags = currentSIN->sid.flags;
					break;
				}
				currentSIN = currentSIN->next;
			}
		} else  {
			sprintf(buff, "SRMMStatusIconFlags%d", (int)current->sid.dwId);
			flags = M->GetByte(dat->hContact, current->sid.szModule, buff, current->sid.flags);
		}
		if (!(flags & MBF_HIDDEN)) {
			if (list_icons++ == iconNum)
				clicked = current;
		}
		current = current->next;
	}

	if ((int)iconNum == list_icons && code != NM_RCLICK) {
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			struct TContainerData *piContainer = pFirstContainer;

			while (piContainer) {
				piContainer->dwFlags = ((dat->pContainer->dwFlags & CNT_NOSOUND) ? piContainer->dwFlags | CNT_NOSOUND : piContainer->dwFlags & ~CNT_NOSOUND);
				InvalidateRect(dat->pContainer->hwndStatus, NULL, TRUE);
				piContainer = piContainer->pNextContainer;
			}
		} else {
			dat->pContainer->dwFlags ^= CNT_NOSOUND;
			InvalidateRect(dat->pContainer->hwndStatus, NULL, TRUE);
		}
	} else if ((int)iconNum == list_icons + 1 && code != NM_RCLICK && dat->bType == SESSIONTYPE_IM) {
		SendMessage(dat->pContainer->hwndActive, WM_COMMAND, IDC_SELFTYPING, 0);
		InvalidateRect(dat->pContainer->hwndStatus, NULL, TRUE);
	} else if ((int)iconNum == list_icons + 2) {
		if(code == NM_CLICK)
			PostMessage(PluginConfig.g_hwndHotkeyHandler, DM_TRAYICONNOTIFY, 101, WM_LBUTTONUP);
		else if(code == NM_RCLICK)
			PostMessage(PluginConfig.g_hwndHotkeyHandler, DM_TRAYICONNOTIFY, 101, WM_RBUTTONUP);
	} else if (clicked) {
		sicd.cbSize = sizeof(StatusIconClickData);
		GetCursorPos(&sicd.clickLocation);
		sicd.dwId = clicked->sid.dwId;
		sicd.szModule = clicked->sid.szModule;
		sicd.flags = (code == NM_RCLICK ? MBCF_RIGHTBUTTON : 0);
		NotifyEventHooks(hHookIconPressedEvt, (WPARAM)dat->hContact, (LPARAM)&sicd);
		InvalidateRect(dat->pContainer->hwndStatus, NULL, TRUE);
	}
}

static HANDLE SI_hServiceIcon[3];

int SI_InitStatusIcons()
{
	SI_hServiceIcon[0] = CreateServiceFunction(MS_MSG_ADDICON, SI_AddStatusIcon);
	SI_hServiceIcon[1] = CreateServiceFunction(MS_MSG_REMOVEICON, SI_RemoveStatusIcon);
	SI_hServiceIcon[2] = CreateServiceFunction(MS_MSG_MODIFYICON, SI_ModifyStatusIcon);
	hHookIconPressedEvt = CreateHookableEvent(ME_MSG_ICONPRESSED);

	return 0;
}


int SI_DeinitStatusIcons()
{
	int i;
	DestroyHookableEvent(hHookIconPressedEvt);
	for (i = 0; i < 3; i++)
		DestroyServiceFunction(SI_hServiceIcon[i]);
	SI_RemoveAllStatusIcons();
	return 0;
}

int SI_GetStatusIconsCount()
{
	return status_icon_list_size;
}
