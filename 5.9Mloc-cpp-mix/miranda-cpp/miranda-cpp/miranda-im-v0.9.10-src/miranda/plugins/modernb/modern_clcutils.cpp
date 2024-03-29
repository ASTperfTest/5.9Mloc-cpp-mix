/*

Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2008 Miranda ICQ/IM project, 
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
#include "hdr/modern_commonheaders.h"
#include "m_clc.h"
#include "hdr/modern_clc.h"
#include "hdr/modern_commonprototypes.h"
#include "hdr/modern_defsettings.h"

//loads of stuff that didn't really fit anywhere else

BOOL RectHitTest(RECT *rc, int testx, int testy)
{
	return testx >= rc->left && testx < rc->right && testy >= rc->top && testy < rc->bottom;
}

int cliHitTest(HWND hwnd,struct ClcData *dat,int testx,int testy,struct ClcContact **contact,struct ClcGroup **group,DWORD *flags)
{
	struct ClcContact *hitcontact=NULL;
	struct ClcGroup *hitgroup=NULL;
	int hit=-1;
	RECT clRect;
	if (CLUI_TestCursorOnBorders()!=0)
	{
		if(flags) *flags=CLCHT_NOWHERE;
		return -1;
	}
	if(flags) *flags=0;
	GetClientRect(hwnd,&clRect);
	if(testx<0 || testy<0 || testy>=clRect.bottom || testx>=clRect.right) {
		if(flags) {
			if(testx<0) *flags|=CLCHT_TOLEFT;
			else if(testx>=clRect.right) *flags|=CLCHT_TORIGHT;
			if(testy<0) *flags|=CLCHT_ABOVE;
			else if(testy>=clRect.bottom) *flags|=CLCHT_BELOW;
		}
		return -1;
	}
	if(testx<dat->leftMargin) {
		if(flags) *flags|=CLCHT_INLEFTMARGIN|CLCHT_NOWHERE;
		return -1;
	}

	// Get hit item 
	hit = cliRowHitTest(dat, dat->yScroll + testy);

	if (hit != -1) 
		hit = cliGetRowByIndex(dat, hit, &hitcontact, &hitgroup);

	if(hit==-1) {
		if(flags) *flags|=CLCHT_NOWHERE|CLCHT_BELOWITEMS;
		return -1;
	}
	if(contact) *contact=hitcontact;
	if(group) *group=hitgroup;
	/////////

	if ( ((testx<hitcontact->pos_indent) && !dat->text_rtl) ||
		((testx>clRect.right-hitcontact->pos_indent) && dat->text_rtl) ) 
	{
		if(flags) *flags|=CLCHT_ONITEMINDENT;
		return hit;
	}

	if (RectHitTest(&hitcontact->pos_check, testx, testy)) 
	{
		if(flags) *flags|=CLCHT_ONITEMCHECK;
		return hit;
	}

	if (RectHitTest(&hitcontact->pos_avatar, testx, testy)) 
	{
		if(flags) *flags|=CLCHT_ONITEMICON;
		return hit;
	}

	if (RectHitTest(&hitcontact->pos_icon, testx, testy)) 
	{
		if(flags) *flags|=CLCHT_ONITEMICON;
		return hit;
	}

	//	if (testx>hitcontact->pos_extra) {
	//		if(flags)
	{
		//			int c = -1;
		int i;
		for(i = 0; i < dat->extraColumnsCount; i++) 
		{  
			if (RectHitTest(&hitcontact->pos_extra[i], testx, testy)) 
			{
				if(flags) *flags|=CLCHT_ONITEMEXTRA|(i<<24);
				return hit;
			}
		}
	}

	if (dat->HiLightMode==1)
	{
		if(flags) *flags|=CLCHT_ONITEMLABEL;
		return hit;
	}

	if (RectHitTest(&hitcontact->pos_label, testx, testy)) 
	{
		if(flags) *flags|=CLCHT_ONITEMLABEL;
		return hit;
	}

	if(flags) *flags|=CLCHT_NOWHERE;
	return hit;
}

void cliScrollTo(HWND hwnd,struct ClcData *dat,int desty,int noSmooth)
{
	DWORD startTick,nowTick;
	int oldy=dat->yScroll;
	RECT clRect,rcInvalidate;
	int maxy,previousy;

	if(dat->iHotTrack!=-1 && dat->yScroll != desty) {
		pcli->pfnInvalidateItem(hwnd,dat,dat->iHotTrack);
		dat->iHotTrack=-1;
		ReleaseCapture();
	}
	GetClientRect(hwnd,&clRect);
	rcInvalidate=clRect;
	//maxy=dat->rowHeight*GetGroupContentsCount(&dat->list,2)-clRect.bottom;
	maxy=cliGetRowTotalHeight(dat)-clRect.bottom;
	if(desty>maxy) desty=maxy;
	if(desty<0) desty=0;
	if(abs(desty-dat->yScroll)<4) noSmooth=1;
	if(!noSmooth && dat->exStyle&CLS_EX_NOSMOOTHSCROLLING) noSmooth=1;
	previousy=dat->yScroll;
	if(!noSmooth) {
		startTick=GetTickCount();
		for(;;) {
			nowTick=GetTickCount();
			if(nowTick>=startTick+dat->scrollTime) break;
			dat->yScroll=oldy+(desty-oldy)*(int)(nowTick-startTick)/dat->scrollTime;
			if(/*dat->backgroundBmpUse&CLBF_SCROLL || dat->hBmpBackground==NULL &&*/FALSE)
				ScrollWindowEx(hwnd,0,previousy-dat->yScroll,NULL,NULL,NULL,NULL,SW_INVALIDATE);
			else
			{
				CallService(MS_SKINENG_UPTATEFRAMEIMAGE,(WPARAM) hwnd, (LPARAM) 0); 
				//InvalidateRectZ(hwnd,NULL,FALSE);
			}
			previousy=dat->yScroll;
			SetScrollPos(hwnd,SB_VERT,dat->yScroll,TRUE);
			CallService(MS_SKINENG_UPTATEFRAMEIMAGE,(WPARAM) hwnd, (LPARAM) 0); 
			UpdateWindow(hwnd);
		}
	}
	dat->yScroll=desty;
	if((dat->backgroundBmpUse&CLBF_SCROLL || dat->hBmpBackground==NULL) && FALSE)
		ScrollWindowEx(hwnd,0,previousy-dat->yScroll,NULL,NULL,NULL,NULL,SW_INVALIDATE);
	else
		CLUI__cliInvalidateRect(hwnd,NULL,FALSE);
	SetScrollPos(hwnd,SB_VERT,dat->yScroll,TRUE);
}


void cliRecalcScrollBar(HWND hwnd,struct ClcData *dat)
{
	SCROLLINFO si={0};
	RECT clRect;
	NMCLISTCONTROL nm;
	if (LOCK_RECALC_SCROLLBAR) return;

	RowHeights_CalcRowHeights(dat, hwnd);

	GetClientRect(hwnd,&clRect);
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	si.nMin=0;
	si.nMax=cliGetRowTotalHeight(dat)-1;
	si.nPage=clRect.bottom;
	si.nPos=dat->yScroll;

	nm.hdr.code=CLN_LISTSIZECHANGE;
	nm.hdr.hwndFrom=hwnd;
	nm.hdr.idFrom=0;//GetDlgCtrlID(hwnd);
	nm.pt.y=si.nMax;

	SendMessage(GetParent(hwnd),WM_NOTIFY,0,(LPARAM)&nm);       //post

	GetClientRect(hwnd,&clRect);
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	si.nMin=0;
	si.nMax=cliGetRowTotalHeight(dat)-1;
	si.nPage=clRect.bottom;
	si.nPos=dat->yScroll;

	if ( GetWindowLong(hwnd,GWL_STYLE)&CLS_CONTACTLIST ) {
		if ( dat->noVScrollbar==0 ) SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
		//else SetScrollInfo(hwnd,SB_VERT,&si,FALSE);
	} 
	else 
		SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
	g_mutex_bSizing=1;
	cliScrollTo(hwnd,dat,dat->yScroll,1);
	g_mutex_bSizing=0;
}


static WNDPROC OldRenameEditWndProc;
static LRESULT CALLBACK RenameEditSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_KEYDOWN:
		switch(wParam) 
		{
		case VK_RETURN:
			pcli->pfnEndRename(GetParent(hwnd),(struct ClcData*)GetWindowLongPtr(hwnd,GWLP_USERDATA),1);
			return 0;
		case VK_ESCAPE:
			pcli->pfnEndRename(GetParent(hwnd),(struct ClcData*)GetWindowLongPtr(hwnd,GWLP_USERDATA),0);
			return 0;
		}
		break;
	case WM_GETDLGCODE:
		if(lParam) 
		{
			MSG *msg=(MSG*)lParam;
			if(msg->message==WM_KEYDOWN && msg->wParam==VK_TAB) return 0;
			if(msg->message==WM_CHAR && msg->wParam=='\t') return 0;
		}
		return DLGC_WANTMESSAGE;
	case WM_KILLFOCUS:
		pcli->pfnEndRename(GetParent(hwnd),(struct ClcData*)GetWindowLongPtr(hwnd,GWLP_USERDATA),1);
		SendMessage(pcli->hwndContactTree,WM_SIZE,0,0);
		return 0;
	}
	return CallWindowProc(OldRenameEditWndProc,hwnd,msg,wParam,lParam);
}

void cliBeginRenameSelection(HWND hwnd,struct ClcData *dat)
{
	struct ClcContact *contact;
	struct ClcGroup *group;
	int indent,x,y,subident, h,w;
	RECT clRect;
	RECT r;


	KillTimer(hwnd,TIMERID_RENAME);
	ReleaseCapture();
	dat->iHotTrack=-1;
	dat->selection=cliGetRowByIndex(dat,dat->selection,&contact,&group);
	if(dat->selection==-1) return;
	if(contact->type!=CLCIT_CONTACT && contact->type!=CLCIT_GROUP) return;

	if (contact->type==CLCIT_CONTACT && contact->isSubcontact)
		subident=dat->subIndent;
	else 
		subident=0;

	for(indent=0;group->parent;indent++,group=group->parent);
	GetClientRect(hwnd,&clRect);
	x=indent*dat->groupIndent+dat->iconXSpace-2+subident;
	w=clRect.right-x;
	y=cliGetRowTopY(dat, dat->selection)-dat->yScroll;
	h=dat->row_heights[dat->selection];
	{
		int i;
		for (i=0; i<=FONTID_MODERN_MAX; i++)
			if (h<dat->fontModernInfo[i].fontHeight+4) h=dat->fontModernInfo[i].fontHeight+4;
	}
	//TODO contact->pos_label 


	{

		RECT rectW;      
		int h2;
		GetWindowRect(hwnd,&rectW);
		//       w=contact->pos_full_first_row.right-contact->pos_full_first_row.left;
		//       h=contact->pos_full_first_row.bottom-contact->pos_full_first_row.top;
		//w=clRect.right-x;
		//w=clRect.right-x;
		//x+=rectW.left;//+contact->pos_full_first_row.left;
		//y+=rectW.top;//+contact->pos_full_first_row.top;
		x=contact->pos_rename_rect.left+rectW.left;
		y=contact->pos_label.top+rectW.top;
		w=contact->pos_rename_rect.right-contact->pos_rename_rect.left;
		h2=contact->pos_label.bottom-contact->pos_label.top+4;
		h=h2;//max(h,h2);

	}

	{
		int a=0;
		if (contact->type==CLCIT_GROUP)
		{
			if (dat->row_align_group_mode==1) a|=ES_CENTER;
			else if (dat->row_align_group_mode==2) a|=ES_RIGHT;
		}
		if (dat->text_rtl) a|=EN_ALIGN_RTL_EC;
		if (contact->type==CLCIT_GROUP)
			dat->hwndRenameEdit=CreateWindow(TEXT("EDIT"),contact->szText,WS_POPUP|WS_BORDER|ES_AUTOHSCROLL|a,x,y,w,h,hwnd,NULL,g_hInst,NULL);
		else
			dat->hwndRenameEdit=CreateWindow(TEXT("EDIT"),pcli->pfnGetContactDisplayName(contact->hContact,0),WS_POPUP|WS_BORDER|ES_AUTOHSCROLL|a,x,y,w,h,hwnd,NULL,g_hInst,NULL);
	}
	SetWindowLong(dat->hwndRenameEdit,GWL_STYLE,GetWindowLong(dat->hwndRenameEdit,GWL_STYLE)&(~WS_CAPTION)|WS_BORDER);
	SetWindowLongPtr(dat->hwndRenameEdit,GWLP_USERDATA,(LONG_PTR)dat);
	OldRenameEditWndProc=(WNDPROC)SetWindowLongPtr(dat->hwndRenameEdit,GWLP_WNDPROC,(LONG_PTR)RenameEditSubclassProc);
	SendMessage(dat->hwndRenameEdit,WM_SETFONT,(WPARAM)(contact->type==CLCIT_GROUP?dat->fontModernInfo[FONTID_OPENGROUPS].hFont:dat->fontModernInfo[FONTID_CONTACTS].hFont),0);
	SendMessage(dat->hwndRenameEdit,EM_SETMARGINS,EC_LEFTMARGIN|EC_RIGHTMARGIN|EC_USEFONTINFO,0);
	SendMessage(dat->hwndRenameEdit,EM_SETSEL,0,(LPARAM)(-1));
	// SetWindowLongPtr(dat->hwndRenameEdit,GWLP_USERDATA,(LONG_PTR)hwnd);
	r.top=1;
	r.bottom=h-1;
	r.left=0;
	r.right=w;

	//ES_MULTILINE

	SendMessage(dat->hwndRenameEdit,EM_SETRECT,0,(LPARAM)(&r));

	CLUI_ShowWindowMod(dat->hwndRenameEdit,SW_SHOW);
	SetWindowPos(dat->hwndRenameEdit,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	SetFocus(dat->hwndRenameEdit);
}

int GetDropTargetInformation(HWND hwnd,struct ClcData *dat,POINT pt)
{
	RECT clRect;
	int hit;
	struct ClcContact *contact=NULL,*movecontact=NULL;
	struct ClcGroup *group,*movegroup;
	DWORD hitFlags;
	int nSetSelection=-1;

	GetClientRect(hwnd,&clRect);
	dat->selection=dat->iDragItem;
	dat->iInsertionMark=-1;
	dat->nInsertionLevel=0;
	if(!PtInRect(&clRect,pt)) return DROPTARGET_OUTSIDE;

	hit=cliHitTest(hwnd,dat,pt.x,pt.y,&contact,&group,&hitFlags);
	cliGetRowByIndex(dat,dat->iDragItem,&movecontact,&movegroup);
	if(hit==dat->iDragItem) return DROPTARGET_ONSELF;
	if(hit==-1 || hitFlags&CLCHT_ONITEMEXTRA || !movecontact) return DROPTARGET_ONNOTHING;

	if(movecontact->type==CLCIT_GROUP) {
		struct ClcContact *bottomcontact=NULL,*topcontact=NULL;
		struct ClcGroup *topgroup=NULL, *bottomgroup=NULL;
		int topItem=-1,bottomItem=-1;
		int ok=0;
		if(pt.y+dat->yScroll<cliGetRowTopY(dat,hit)+dat->insertionMarkHitHeight || contact->type!=CLCIT_GROUP) {
			//could be insertion mark (above)
			topItem=hit-1; bottomItem=hit;
			bottomcontact=contact;
			bottomgroup=group;
			topItem=cliGetRowByIndex(dat,topItem,&topcontact,&topgroup);
			ok=1;
		} else if ((pt.y+dat->yScroll>=cliGetRowTopY(dat,hit+1)-dat->insertionMarkHitHeight)
			||(contact->type==CLCIT_GROUP && contact->group->expanded && contact->group->cl.count>0)) 
		{
			//could be insertion mark (below)
			topItem=hit; bottomItem=hit+1;
			topcontact=contact; topgroup=group;
			bottomItem=cliGetRowByIndex(dat,bottomItem,&bottomcontact,&bottomgroup);
			ok=1;	
		}
		if (ok)
		{
			if (bottomItem==-1 && contact->type==CLCIT_GROUP)
			{
				bottomItem=topItem+1;
			} 
			else 
			{
				if (bottomItem==-1 && contact->type!=CLCIT_GROUP && contact->groupId==0)
				{
					if (contact->type!=CLCIT_GROUP && contact->groupId==0)
					{
						bottomItem=topItem;
						cliGetRowByIndex(dat,bottomItem,&bottomcontact,&bottomgroup);
					}
				}
				if (bottomItem!=-1 && bottomcontact->type!=CLCIT_GROUP)
				{
					struct ClcGroup * gr=bottomgroup;
					do 
					{
						bottomItem=cliGetRowByIndex(dat,bottomItem-1,&bottomcontact,&bottomgroup);}
					while (bottomItem>=0 && bottomcontact->type!=CLCIT_GROUP && bottomgroup==gr);
					nSetSelection=bottomItem;
					bottomItem=cliGetRowByIndex(dat,bottomItem+1,&bottomcontact,&bottomgroup);
				}
			}

			if (bottomItem==-1)	bottomItem=topItem+1;
			{
				int bi=cliGetRowByIndex(dat,bottomItem,&bottomcontact,&bottomgroup);
				if (bi!=-1)
				{
					group=bottomgroup;                    
					if (bottomcontact==movecontact || group==movecontact->group)	return DROPTARGET_ONSELF;
					dat->nInsertionLevel=-1; // decreasing here
					for(;group;group=group->parent)
					{   
						dat->nInsertionLevel++;
						if (group==movecontact->group) return DROPTARGET_ONSELF;
					}
				}
			}           
			dat->iInsertionMark=bottomItem;
			dat->selection=nSetSelection;
			return DROPTARGET_INSERTION;
		}
	}
	if(contact->type==CLCIT_GROUP) 
	{
		if(dat->iInsertionMark==-1) 
		{
			if(movecontact->type==CLCIT_GROUP) 
			{	 //check not moving onto its own subgroup
				dat->iInsertionMark=hit+1;
				for(;group;group=group->parent) 
				{
					dat->nInsertionLevel++;
					if(group==movecontact->group) return DROPTARGET_ONSELF;
				}
			}
			dat->selection=hit;            
			return DROPTARGET_ONGROUP;
		}
	}
	dat->selection=hit;

	if (g_szMetaModuleName && !mir_strcmp(contact->proto,g_szMetaModuleName)&& (ServiceExists(MS_MC_ADDTOMETA))) return DROPTARGET_ONMETACONTACT;
	if (contact->isSubcontact && (ServiceExists(MS_MC_ADDTOMETA))) return DROPTARGET_ONSUBCONTACT;
	return DROPTARGET_ONCONTACT;
}
COLORREF sttGetColor(char * module, char * color, COLORREF defColor)
{
	BOOL useWinColor=ModernGetSettingByte(NULL, module, "UseWinColours",CLCDEFAULT_USEWINDOWSCOLOURS);
	if (useWinColor) return defColor;
	else return ModernGetSettingDword(NULL, module, color, defColor);
}
void RegisterCLUIFonts( void );
void LoadCLCFonts( HWND hwnd, struct ClcData *dat )
{
	RegisterCLUIFonts();
	
	HDC hdc=GetDC(hwnd);
	HFONT holdfont = (HFONT)GetCurrentObject( hdc, OBJ_FONT );
	
	for( int i = 0 ; i <= FONTID_MODERN_MAX; i++ ) 
	{
		if( !dat->fontModernInfo[i].changed && dat->fontModernInfo[i].hFont )
		{
			DeleteObject(dat->fontModernInfo[i].hFont);
		}
		LOGFONT lf;

		// Issue 40: Do not reload font colors for embedded clists
		// Parent window is responsible to re-set fonts colors if needed
		GetFontSetting( i, &lf, dat->force_in_dialog ? NULL: &dat->fontModernInfo[i].colour, &dat->fontModernInfo[i].effect, &dat->fontModernInfo[i].effectColour1, &dat->fontModernInfo[i].effectColour2 );
		dat->fontModernInfo[i].hFont = CreateFontIndirect( &lf );
		dat->fontModernInfo[i].changed = 0;

		SelectObject( hdc, dat->fontModernInfo[i].hFont );
		SIZE fontSize;
		GetTextExtentPoint32A( hdc, "x", 1, &fontSize );
		dat->fontModernInfo[i].fontHeight = fontSize.cy;
	}
	SelectObject( hdc, holdfont );
	ReleaseDC( hwnd, hdc );
}

void LoadCLCOptions(HWND hwnd, struct ClcData *dat )
{ 
	int i;

	g_CluiData.fDisableSkinEngine=ModernGetSettingByte(NULL,"ModernData","DisableEngine", SETTING_DISABLESKIN_DEFAULT);
	
	LoadCLCFonts( hwnd, dat );

	g_CluiData.bSortByOrder[0]=ModernGetSettingByte(NULL,"CList","SortBy1",SETTING_SORTBY1_DEFAULT);
	g_CluiData.bSortByOrder[1]=ModernGetSettingByte(NULL,"CList","SortBy2",SETTING_SORTBY2_DEFAULT);
	g_CluiData.bSortByOrder[2]=ModernGetSettingByte(NULL,"CList","SortBy3",SETTING_SORTBY3_DEFAULT);
	g_CluiData.fSortNoOfflineBottom=ModernGetSettingByte(NULL,"CList","NoOfflineBottom",SETTING_NOOFFLINEBOTTOM_DEFAULT);

	// Row
	dat->row_min_heigh = ModernGetSettingWord(NULL,"CList","MinRowHeight",CLCDEFAULT_ROWHEIGHT);
	dat->row_border = ModernGetSettingWord(NULL,"CList","RowBorder",SETTING_ROWBORDER_DEFAULT);
	dat->row_before_group_space =((hwnd!=pcli->hwndContactTree&&pcli->hwndContactTree!=NULL) 
		|| !ModernGetSettingByte(NULL,"ModernData","UseAdvancedRowLayout",SETTING_ROW_ADVANCEDLAYOUT_DEFAULT))?0:ModernGetSettingWord(NULL,"ModernSkin","SpaceBeforeGroup",SKIN_SPACEBEFOREGROUP_DEFAULT);
	dat->row_variable_height = ModernGetSettingByte(NULL,"CList","VariableRowHeight",SETTING_VARIABLEROWHEIGHT_DEFAULT);
	dat->row_align_left_items_to_left = ModernGetSettingByte(NULL,"CList","AlignLeftItemsToLeft",SETTING_ALIGNLEFTTOLEFT_DEFAULT);
	dat->row_hide_group_icon = ModernGetSettingByte(NULL,"CList","HideGroupsIcon",SETTING_HIDEGROUPSICON_DEFAULT);
	dat->row_align_right_items_to_right = ModernGetSettingByte(NULL,"CList","AlignRightItemsToRight",SETTING_ALIGNRIGHTORIGHT_DEFAULT);
	//TODO: Add to settings
	dat->row_align_group_mode=ModernGetSettingByte(NULL,"CList","AlignGroupCaptions",SETTING_ALIGNGROPCAPTION_DEFAULT);
	if (pcli->hwndContactTree==NULL || dat->hWnd==pcli->hwndContactTree)
	{

		int defItemsOrder[NUM_ITEM_TYPE] = SETTINS_ROWITEMORDER_DEFAULT;
		for (i = 0 ; i < NUM_ITEM_TYPE ; i++)
		{
			char tmp[128];
			mir_snprintf(tmp, sizeof(tmp), "RowPos%d", i);
			dat->row_items[i] = ModernGetSettingWord(NULL, "CList", tmp, defItemsOrder[i]);
		}
	}
	else
	{
		int defItems[]= {ITEM_ICON, ITEM_TEXT, ITEM_EXTRA_ICONS,};
		for (i = 0 ; i < NUM_ITEM_TYPE; i++)
			dat->row_items[i]=(i<SIZEOF(defItems)) ? defItems[i] : -1;
	}

	// Avatar
	if (pcli->hwndContactTree == hwnd  || pcli->hwndContactTree==NULL)
	{
		dat->avatars_show = ModernGetSettingByte(NULL,"CList","AvatarsShow",SETTINGS_SHOWAVATARS_DEFAULT);
		dat->avatars_draw_border = ModernGetSettingByte(NULL,"CList","AvatarsDrawBorders",SETTINGS_AVATARDRAWBORDER_DEFAULT);
		dat->avatars_border_color = (COLORREF)ModernGetSettingDword(NULL,"CList","AvatarsBorderColor",SETTINGS_AVATARBORDERCOLOR_DEFAULT);
		dat->avatars_round_corners = ModernGetSettingByte(NULL,"CList","AvatarsRoundCorners",SETTINGS_AVATARROUNDCORNERS_DEFAULT);
		dat->avatars_use_custom_corner_size = ModernGetSettingByte(NULL,"CList","AvatarsUseCustomCornerSize",SETTINGS_AVATARUSECUTOMCORNERSIZE_DEFAULT);
		dat->avatars_custom_corner_size = ModernGetSettingWord(NULL,"CList","AvatarsCustomCornerSize",SETTINGS_AVATARCORNERSIZE_DEFAULT);
		dat->avatars_ignore_size_for_row_height = ModernGetSettingByte(NULL,"CList","AvatarsIgnoreSizeForRow",SETTINGS_AVATARIGNORESIZEFORROW_DEFAULT);
		dat->avatars_draw_overlay = ModernGetSettingByte(NULL,"CList","AvatarsDrawOverlay",SETTINGS_AVATARDRAWOVERLAY_DEFAULT);
		dat->avatars_overlay_type = ModernGetSettingByte(NULL,"CList","AvatarsOverlayType",SETTINGS_AVATAROVERLAYTYPE_DEFAULT);
		dat->avatars_maxheight_size = ModernGetSettingWord(NULL,"CList","AvatarsSize",SETTING_AVATARHEIGHT_DEFAULT);
		dat->avatars_maxwidth_size = ModernGetSettingWord(NULL,"CList","AvatarsWidth",SETTING_AVATARWIDTH_DEFAULT);
	}
	else
	{
		dat->avatars_show = 0;
		dat->avatars_draw_border = 0;
		dat->avatars_border_color = 0;
		dat->avatars_round_corners = 0;
		dat->avatars_use_custom_corner_size = 0;
		dat->avatars_custom_corner_size = 4;
		dat->avatars_ignore_size_for_row_height = 0;
		dat->avatars_draw_overlay = 0;
		dat->avatars_overlay_type = SETTING_AVATAR_OVERLAY_TYPE_NORMAL;
		dat->avatars_maxheight_size = 30;
		dat->avatars_maxwidth_size = 0;
	}

	// Icon
	if (pcli->hwndContactTree == hwnd|| pcli->hwndContactTree==NULL)
	{
		dat->icon_hide_on_avatar = ModernGetSettingByte(NULL,"CList","IconHideOnAvatar",SETTING_HIDEICONONAVATAR_DEFAULT);
		dat->icon_draw_on_avatar_space = ModernGetSettingByte(NULL,"CList","IconDrawOnAvatarSpace",SETTING_ICONONAVATARPLACE_DEFAULT);
		dat->icon_ignore_size_for_row_height = ModernGetSettingByte(NULL,"CList","IconIgnoreSizeForRownHeight",SETTING_ICONIGNORESIZE_DEFAULT);
	}
	else
	{
		dat->icon_hide_on_avatar = 0;
		dat->icon_draw_on_avatar_space = 0;
		dat->icon_ignore_size_for_row_height = 0;
	}

	// Contact time
	if (pcli->hwndContactTree == hwnd|| pcli->hwndContactTree==NULL)
	{
		dat->contact_time_show = ModernGetSettingByte(NULL,"CList","ContactTimeShow",SETTING_SHOWTIME_DEFAULT);
		dat->contact_time_show_only_if_different = ModernGetSettingByte(NULL,"CList","ContactTimeShowOnlyIfDifferent",SETTING_SHOWTIMEIFDIFF_DEFAULT);
	}
	else
	{
		dat->contact_time_show = 0;
		dat->contact_time_show_only_if_different = 0;
	}

	// Text
	dat->text_rtl = ModernGetSettingByte(NULL,"CList","TextRTL",SETTING_TEXT_RTL_DEFAULT);
	dat->text_align_right = ModernGetSettingByte(NULL,"CList","TextAlignToRight",SETTING_TEXT_RIGHTALIGN_DEFAULT);
	dat->text_replace_smileys = ModernGetSettingByte(NULL,"CList","TextReplaceSmileys",SETTING_TEXT_SMILEY_DEFAULT);
	dat->text_resize_smileys = ModernGetSettingByte(NULL,"CList","TextResizeSmileys",SETTING_TEXT_RESIZESMILEY_DEFAULT);
	dat->text_smiley_height = 0;
	dat->text_use_protocol_smileys = ModernGetSettingByte(NULL,"CList","TextUseProtocolSmileys",SETTING_TEXT_PROTOSMILEY_DEFAULT);

	if (pcli->hwndContactTree == hwnd|| pcli->hwndContactTree==NULL)
	{
		dat->text_ignore_size_for_row_height = ModernGetSettingByte(NULL,"CList","TextIgnoreSizeForRownHeight",SETTING_TEXT_IGNORESIZE_DEFAULT);
	}
	else
	{
		dat->text_ignore_size_for_row_height = 0;
	}

	// First line
	dat->first_line_draw_smileys = ModernGetSettingByte(NULL,"CList","FirstLineDrawSmileys",SETTING_FIRSTLINE_SMILEYS_DEFAULT);
	dat->first_line_append_nick = ModernGetSettingByte(NULL,"CList","FirstLineAppendNick",SETTING_FIRSTLINE_APPENDNICK_DEFAULT);
	gl_TrimText=ModernGetSettingByte(NULL,"CList","TrimText",SETTING_FIRSTLINE_TRIMTEXT_DEFAULT);

	// Second line
	if (pcli->hwndContactTree == hwnd || pcli->hwndContactTree==NULL)
	{
		dat->second_line_show = ModernGetSettingByte(NULL,"CList","SecondLineShow",SETTING_SECONDLINE_SHOW_DEFAULT);
		dat->second_line_top_space = ModernGetSettingWord(NULL,"CList","SecondLineTopSpace",SETTING_SECONDLINE_TOPSPACE_DEFAULT);
		dat->second_line_draw_smileys = ModernGetSettingByte(NULL,"CList","SecondLineDrawSmileys",SETTING_SECONDLINE_SMILEYS_DEFAULT);
		dat->second_line_type = ModernGetSettingWord(NULL,"CList","SecondLineType",SETTING_SECONDLINE_TYPE_DEFAULT);
		{
			DBVARIANT dbv={0};

			if (!ModernGetSettingTString(NULL, "CList","SecondLineText", &dbv))
			{
				lstrcpyn(dat->second_line_text, dbv.ptszVal, SIZEOF(dat->second_line_text)-1);
				dat->second_line_text[SIZEOF(dat->second_line_text)-1] = _T('\0');
				ModernDBFreeVariant(&dbv);
			}
			else
			{
				dat->second_line_text[0] = _T('\0');
			}
		}
		dat->second_line_xstatus_has_priority = ModernGetSettingByte(NULL,"CList","SecondLineXStatusHasPriority",SETTING_SECONDLINE_XSTATUS_DEFAULT);
		dat->second_line_show_status_if_no_away=ModernGetSettingByte(NULL,"CList","SecondLineShowStatusIfNoAway",SETTING_SECONDLINE_STATUSIFNOAWAY_DEFAULT);
		dat->second_line_show_listening_if_no_away=ModernGetSettingByte(NULL,"CList","SecondLineShowListeningIfNoAway",SETTING_SECONDLINE_LISTENINGIFNOAWAY_DEFAULT);
		dat->second_line_use_name_and_message_for_xstatus = ModernGetSettingByte(NULL,"CList","SecondLineUseNameAndMessageForXStatus",SETTING_SECONDLINE_XSTATUSNAMETEXT_DEFAULT);
	}
	else
	{
		dat->second_line_show = 0;
		dat->second_line_top_space = 0;
		dat->second_line_draw_smileys = 0;
		dat->second_line_type = 0;
		dat->second_line_text[0] = _T('\0');
		dat->second_line_xstatus_has_priority = 0;
		dat->second_line_use_name_and_message_for_xstatus = 0;
	}


	// Third line
	if (pcli->hwndContactTree == hwnd || pcli->hwndContactTree==NULL)
	{
		dat->third_line_show = ModernGetSettingByte(NULL,"CList","ThirdLineShow",SETTING_THIRDLINE_SHOW_DEFAULT);
		dat->third_line_top_space = ModernGetSettingWord(NULL,"CList","ThirdLineTopSpace",SETTING_THIRDLINE_TOPSPACE_DEFAULT);
		dat->third_line_draw_smileys = ModernGetSettingByte(NULL,"CList","ThirdLineDrawSmileys",SETTING_THIRDLINE_SMILEYS_DEFAULT);
		dat->third_line_type = ModernGetSettingWord(NULL,"CList","ThirdLineType",SETTING_THIRDLINE_TYPE_DEFAULT);
		{
			DBVARIANT dbv={0};

			if (!ModernGetSettingTString(NULL, "CList","ThirdLineText", &dbv))
			{
				lstrcpyn(dat->third_line_text, dbv.ptszVal, SIZEOF(dat->third_line_text)-1);
				dat->third_line_text[SIZEOF(dat->third_line_text)-1] = _T('\0');
				ModernDBFreeVariant(&dbv);
			}
			else
			{
				dat->third_line_text[0] = _T('\0');
			}
		}
		dat->third_line_xstatus_has_priority = ModernGetSettingByte(NULL,"CList","ThirdLineXStatusHasPriority",SETTING_THIRDLINE_XSTATUS_DEFAULT);
		dat->third_line_show_status_if_no_away=ModernGetSettingByte(NULL,"CList","ThirdLineShowStatusIfNoAway",SETTING_THIRDLINE_STATUSIFNOAWAY_DEFAULT);
		dat->third_line_show_listening_if_no_away=ModernGetSettingByte(NULL,"CList","ThirdLineShowListeningIfNoAway",SETTING_THIRDLINE_LISTENINGIFNOAWAY_DEFAULT);
		dat->third_line_use_name_and_message_for_xstatus = ModernGetSettingByte(NULL,"CList","ThirdLineUseNameAndMessageForXStatus",SETTING_THIRDLINE_XSTATUSNAMETEXT_DEFAULT);
	}
	else
	{
		dat->third_line_show = 0;
		dat->third_line_top_space = 0;
		dat->third_line_draw_smileys = 0;
		dat->third_line_type = TEXT_STATUS_MESSAGE;
		dat->third_line_text[0] = _T('\0');
		dat->third_line_xstatus_has_priority = 1;
		dat->third_line_use_name_and_message_for_xstatus = 0;
	}

	dat->leftMargin=ModernGetSettingByte(NULL,"CLC","LeftMargin",CLCDEFAULT_LEFTMARGIN);
	dat->rightMargin=ModernGetSettingByte(NULL,"CLC","RightMargin",CLCDEFAULT_RIGHTMARGIN);
	dat->exStyle=ModernGetSettingDword(NULL,"CLC","ExStyle",GetDefaultExStyle());
	dat->scrollTime=ModernGetSettingWord(NULL,"CLC","ScrollTime",CLCDEFAULT_SCROLLTIME);
	dat->force_in_dialog=(pcli->hwndContactTree)?(hwnd!=pcli->hwndContactTree):0;
	dat->groupIndent=ModernGetSettingByte(NULL,"CLC","GroupIndent",CLCDEFAULT_GROUPINDENT);
	dat->subIndent=ModernGetSettingByte(NULL,"CLC","SubIndent",CLCDEFAULT_GROUPINDENT);
	dat->gammaCorrection=ModernGetSettingByte(NULL,"CLC","GammaCorrect",CLCDEFAULT_GAMMACORRECT);
	dat->showIdle=ModernGetSettingByte(NULL,"CLC","ShowIdle",CLCDEFAULT_SHOWIDLE);
	dat->noVScrollbar=ModernGetSettingByte(NULL,"CLC","NoVScrollBar",CLCDEFAULT_NOVSCROLL);
	SendMessage(hwnd,INTM_SCROLLBARCHANGED,0,0);
	
	if (dat->hBmpBackground) {DeleteObject(dat->hBmpBackground); dat->hBmpBackground=NULL;}
	if (dat->hMenuBackground) {DeleteObject(dat->hMenuBackground); dat->hMenuBackground=NULL;}
	
	dat->useWindowsColours = ModernGetSettingByte(NULL, "CLC", "UseWinColours", CLCDEFAULT_USEWINDOWSCOLOURS);

	if (g_CluiData.fDisableSkinEngine)
	{
		DBVARIANT dbv;
		if(!dat->bkChanged) 
		{
			dat->bkColour=sttGetColor("CLC","BkColour",GetSysColor(COLOR_3DFACE));
			{	
				if(ModernGetSettingByte(NULL,"CLC","UseBitmap",CLCDEFAULT_USEBITMAP)) 
				{
					if(!ModernGetSettingString(NULL,"CLC","BkBitmap",&dbv)) 
					{
						dat->hBmpBackground=(HBITMAP)CallService(MS_UTILS_LOADBITMAP,0,(LPARAM)dbv.pszVal);				
						ModernDBFreeVariant(&dbv);						
					}
				}
			}
			dat->backgroundBmpUse=ModernGetSettingWord(NULL,"CLC","BkBmpUse",CLCDEFAULT_BKBMPUSE);
		}		
		dat->MenuBkColor=sttGetColor("Menu","BkColour",CLCDEFAULT_BKCOLOUR);
		dat->MenuBkHiColor=sttGetColor("Menu","SelBkColour",CLCDEFAULT_SELBKCOLOUR);

		dat->MenuTextColor=sttGetColor("Menu","TextColour",CLCDEFAULT_TEXTCOLOUR);
		dat->MenuTextHiColor=sttGetColor("Menu","SelTextColour",CLCDEFAULT_MODERN_SELTEXTCOLOUR);
		
		if(ModernGetSettingByte(NULL,"Menu","UseBitmap",CLCDEFAULT_USEBITMAP)) {
			if(!ModernGetSettingString(NULL,"Menu","BkBitmap",&dbv)) {
				dat->hMenuBackground=(HBITMAP)CallService(MS_UTILS_LOADBITMAP,0,(LPARAM)dbv.pszVal);
				//mir_free_and_nill(dbv.pszVal);
				ModernDBFreeVariant(&dbv);
			}
		}
		dat->MenuBmpUse=ModernGetSettingWord(NULL,"Menu","BkBmpUse",CLCDEFAULT_BKBMPUSE);
	}
	
	dat->greyoutFlags=ModernGetSettingDword(NULL,"CLC","GreyoutFlags",CLCDEFAULT_GREYOUTFLAGS);
	dat->offlineModes=ModernGetSettingDword(NULL,"CLC","OfflineModes",CLCDEFAULT_OFFLINEMODES);
	dat->selBkColour=sttGetColor("CLC","SelBkColour",CLCDEFAULT_SELBKCOLOUR);
	dat->selTextColour=ModernGetSettingDword(NULL,"CLC","SelTextColour",CLCDEFAULT_MODERN_SELTEXTCOLOUR);
	dat->hotTextColour=ModernGetSettingDword(NULL,"CLC","HotTextColour",CLCDEFAULT_MODERN_HOTTEXTCOLOUR);
	dat->quickSearchColour=ModernGetSettingDword(NULL,"CLC","QuickSearchColour",CLCDEFAULT_MODERN_QUICKSEARCHCOLOUR);
	if(!g_szMetaModuleName && ServiceExists(MS_MC_GETPROTOCOLNAME)) g_szMetaModuleName = (char *)CallService(MS_MC_GETPROTOCOLNAME, 0, 0);
	dat->IsMetaContactsEnabled=(!(GetWindowLong(hwnd,GWL_STYLE)&CLS_MANUALUPDATE)) &&
		g_szMetaModuleName && ModernGetSettingByte(NULL,g_szMetaModuleName,"Enabled",1) && ServiceExists(MS_MC_GETDEFAULTCONTACT);

	if (pcli->hwndContactTree==NULL || dat->hWnd==pcli->hwndContactTree)
		dat->MetaIgnoreEmptyExtra=ModernGetSettingByte(NULL,"CLC","MetaIgnoreEmptyExtra",SETTING_METAIGNOREEMPTYEXTRA_DEFAULT);
	else
		dat->MetaIgnoreEmptyExtra=FALSE;

	dat->expandMeta=ModernGetSettingByte(NULL,"CLC","MetaExpanding",SETTING_METAEXPANDING_DEFAULT);
	dat->useMetaIcon=ModernGetSettingByte(NULL,"CLC","Meta",SETTING_USEMETAICON_DEFAULT);
	
	dat->drawOverlayedStatus=ModernGetSettingByte(NULL,"CLC","DrawOverlayedStatus",SETTING_DRAWOVERLAYEDSTATUS_DEFAULT);

	dat->dbbMetaHideExtra=ModernGetSettingByte(NULL,"CLC","MetaHideExtra",SETTING_METAHIDEEXTRA_DEFAULT);
	dat->dbbBlendInActiveState=ModernGetSettingByte(NULL,"CLC","BlendInActiveState",SETTING_BLENDINACTIVESTATE_DEFAULT);
	dat->dbbBlend25=ModernGetSettingByte(NULL,"CLC","Blend25%",SETTING_BLENDINACTIVESTATE_DEFAULT);
	dat->bCompactMode=ModernGetSettingByte(NULL,"CLC","CompactMode",SETTING_COMPACTMODE_DEFAULT);
	if ((pcli->hwndContactTree == hwnd || pcli->hwndContactTree==NULL))
	{
		IvalidateDisplayNameCache(16);

	}

	{
		NMHDR hdr;
		hdr.code=CLN_OPTIONSCHANGED;
		hdr.hwndFrom=hwnd;
		hdr.idFrom=0;//GetDlgCtrlID(hwnd);
		SendMessage(GetParent(hwnd),WM_NOTIFY,0,(LPARAM)&hdr);
	}
	SendMessage(hwnd,WM_SIZE,0,0);

}

int ExpandMetaContact(HWND hwnd, struct ClcContact * contact, struct ClcData * dat, BOOL bExpand)
{
	struct ClcContact * ht=NULL;
	KillTimer(hwnd,TIMERID_SUBEXPAND);
	if (contact->type!=CLCIT_CONTACT ||contact->SubAllocated==0 || contact->SubExpanded==bExpand || !ModernGetSettingByte(NULL,"CLC","MetaExpanding",SETTING_METAEXPANDING_DEFAULT)) return 0;
	contact->SubExpanded=bExpand;
	ModernWriteSettingByte(contact->hContact,"CList","Expanded",contact->SubExpanded);
	dat->NeedResort=1;
	pcli->pfnSortCLC(hwnd,dat,1);		
	cliRecalcScrollBar(hwnd,dat);
	return contact->SubExpanded;
}

int cliFindRowByText(HWND hwnd, struct ClcData *dat, const TCHAR *text, int prefixOk)
{
	struct ClcGroup *group = &dat->list;
	int testlen = lstrlen(text);
	struct ClcContact *contact=NULL;
	int SubCount=0;

	group->scanIndex = 0;
	for (;;) {
		if (group->scanIndex == group->cl.count) {
			group = group->parent;
			if (group == NULL)
				break;
			group->scanIndex++;
			continue;
		}
		contact=group->cl.items[group->scanIndex];
		if (contact->type != CLCIT_DIVIDER) 
		{			
			if ((prefixOk && !_tcsnicmp(text, contact->szText, testlen)) ||
				(!prefixOk && !lstrcmpi(text, contact->szText))) 
			{
				struct ClcGroup *contactGroup = group;
				int contactScanIndex = group->scanIndex;
				for (; group; group = group->parent)
					pcli->pfnSetGroupExpand(hwnd, dat, group, 1);
				return pcli->pfnGetRowsPriorTo(&dat->list, contactGroup, contactScanIndex+SubCount);
			}
			if (group->cl.items[group->scanIndex]->type == CLCIT_GROUP) 
			{
				if (!(dat->exStyle & CLS_EX_QUICKSEARCHVISONLY) || group->cl.items[group->scanIndex]->group->expanded) {
					group = group->cl.items[group->scanIndex]->group;
					group->scanIndex = 0;
					SubCount=0;
					continue;
				}
			}
		}
		if (contact->type==CLCIT_CONTACT && contact->SubAllocated)			
		{
			if (!(dat->exStyle & CLS_EX_QUICKSEARCHVISONLY) || contact->SubExpanded )
			{
				int i=0;
				for (i=0; i<contact->SubAllocated; i++)
				{
					struct ClcContact * subcontact=&(contact->subcontacts[i]);
					if ((prefixOk && !_tcsnicmp(text, subcontact->szText, testlen)) ||
						(!prefixOk && !lstrcmpi(text, subcontact->szText))) 
					{
						struct ClcGroup *contactGroup = group;
						int contactScanIndex = group->scanIndex;
						for (; group; group = group->parent)
							pcli->pfnSetGroupExpand(hwnd, dat, group, 1);
						if (!contact->SubExpanded)
							ExpandMetaContact(hwnd, contact, dat, 1 );
						return pcli->pfnGetRowsPriorTo(&dat->list, contactGroup, contactScanIndex+SubCount+i+1);
					}
				}
			}
		}
		if (contact->type==CLCIT_CONTACT && contact->SubAllocated && contact->SubExpanded)
			SubCount+=contact->SubAllocated;
		group->scanIndex++;
	}
	return -1;
}