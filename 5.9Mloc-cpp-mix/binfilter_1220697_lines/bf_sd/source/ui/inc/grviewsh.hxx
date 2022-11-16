/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: grviewsh.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:28:20 $
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

#ifndef _SD_GRVIEWSH_HXX
#define _SD_GRVIEWSH_HXX

#ifndef _SD_DRVIEWSH_HXX
#include "drviewsh.hxx"
#endif
namespace binfilter {

/*************************************************************************
|*
|* Ableitung von SdDrawViewShell fuer Drawing (Graphic)
|*
\************************************************************************/

class SdGraphicViewShell : public SdDrawViewShell
{
protected:

 public:
	TYPEINFO();

	SFX_DECL_VIEWFACTORY(SdGraphicViewShell);
	SFX_DECL_INTERFACE(SD_IF_SDGRAPHICVIEWSHELL);

	 SdGraphicViewShell( SfxViewFrame* pFrame, const SdDrawViewShell& rShell );
	 SdGraphicViewShell( SfxViewFrame* pFrame, SfxViewShell* pOldShell );
	~SdGraphicViewShell();

	//virtual BOOL KeyInput(const KeyEvent& rKEvt, SdWindow* pWin);
	//virtual void MouseMove(const MouseEvent& rMEvt, SdWindow* pWin);
	//virtual void MouseButtonUp(const MouseEvent& rMEvt, SdWindow* pWin);
	//virtual void MouseButtonDown(const MouseEvent& rMEvt, SdWindow* pWin);
	//virtual void Command(const CommandEvent& rCEvt, SdWindow* pWin);

	//virtual void OuterResizePixel(const Point& rPos, const Size& rSize);

	//virtual PrintDialog*	CreatePrintDialog( Window *pParent );
	//virtual SfxTabPage* 	CreatePrintOptionsPage( Window *pParent,
	//									const SfxItemSet &rOptions );
	//virtual USHORT			Print(SfxProgress& rProgress, PrintDialog* pDialog);

	//void	FuTemporary(SfxRequest& rReq);
	//void	FuPermanent(SfxRequest& rReq);
	//void	FuSupport(SfxRequest& rReq);

	//void	AttrExec (SfxRequest& rReq);
	//void	AttrState (SfxItemSet& rSet);

	//virtual void ReadFrameViewData(FrameView* pView);
	//virtual void WriteFrameViewData();

};



} //namespace binfilter
#endif		// _SD_GRVIEWSH_HXX

