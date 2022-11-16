/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: slidvish.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: obo $ $Date: 2007/03/09 14:44:08 $
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

#ifndef _SD_SLIDVISH_HXX
#define _SD_SLIDVISH_HXX

#ifndef _SD_VIEWSHEL_HXX
#include "viewshel.hxx"
#endif
#ifndef _VIEWFAC_HXX //autogen
#include <bf_sfx2/viewfac.hxx>
#endif
#ifndef _SFXVIEWSH_HXX //autogen
#include <bf_sfx2/viewsh.hxx>
#endif
namespace binfilter {

class SdWindow;
class SdPage;
class SdUnoSlideView;

/*************************************************************************
|*
|*	  Beschreibung		SdSlideViewShell ist die ViewShell fuer den Diamodus
|*
\************************************************************************/

class SdSlideViewShell : public SdViewShell
{



	

public:

	                TYPEINFO();

	                SFX_DECL_VIEWFACTORY(SdSlideViewShell);
	                SFX_DECL_INTERFACE(SD_IF_SDSLIDEVIEWSHELL);

					SdSlideViewShell(SfxViewFrame *pFrame, const SdSlideViewShell& rShell): SdViewShell(pFrame, rShell){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;SdSlideViewShell(SfxViewFrame *pFrame, const SdSlideViewShell& rShell);
					SdSlideViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell): SdViewShell(pFrame, NULL ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;SdSlideViewShell(SfxViewFrame* pFrame, SfxViewShell* pOldShell);


    /** Set the position and size of the area which contains the GUI
        elements like rulers, sliders, and buttons as well as the document
        view.  Both size and position are expected to be in pixel
        coordinates.  The positions and sizes of the mentioned GUI elements
        are updated as well.

        <p> This method is implemented by first setting copying the given
        values to internal variables and then calling the
        <type>ArrangeGUIElements</type> method which performs the actual
        work of sizeing and arranging the UI elements accordingly.</p>
        @param rPos
            The position of the enclosing window relative to the document
            window.  This is only interesting if a Draw/Impress document
            view is embedded as OLE object into another document view.  For
            normal documents this position is (0,0).
        @param rSize
            The new size in pixel.
    */
                    
    /** Arrange and resize the GUI elements like rulers, sliders, and
        buttons as well as the actual document view according to the size of
        the enclosing window and current sizes of buttons, rulers, and
        sliders.
    */
                    

					virtual SdPage*	GetActualPage(){DBG_BF_ASSERT(0, "STRIP");return NULL;} //STRIP001 ;virtual SdPage*	GetActualPage();


					void            ExecCtrl(SfxRequest &rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;	void            ExecCtrl(SfxRequest &rReq);
					void            GetCtrlState(SfxItemSet &rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void            GetCtrlState(SfxItemSet &rSet);
					void            GetAttrState(SfxItemSet &rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;void            GetAttrState(SfxItemSet &rSet);


					void            ExecStatusBar(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;void            ExecStatusBar(SfxRequest& rReq);
					void            GetStatusBarState(SfxItemSet& rSet){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;void            GetStatusBarState(SfxItemSet& rSet);

					void            FuTemporary(SfxRequest &rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;	void            FuTemporary(SfxRequest &rReq);
					void            FuPermanent(SfxRequest &rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;void            FuPermanent(SfxRequest &rReq);
					void            FuSupport(SfxRequest &rReq){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 ;void            FuSupport(SfxRequest &rReq);




    /** Draw the rectangle arround the specified slide that indicates whether
        the slide is selected or not.  When not selected the rectangle is
        painted in the background color (WindowColor from the style settings)
        and is therefore not visible.  A selected slide is painted with the
        WindowTextColor from the style settings.  Painting takes place in
        all associated windows.  The line- and fill color of the windows are
        restored to their original values after the rectangle is painted.
        @param nPage
            When the page number is invalid then the call is ignored.
    */




};

} //namespace binfilter
#endif // _SD_SLIDVISH_HXX
