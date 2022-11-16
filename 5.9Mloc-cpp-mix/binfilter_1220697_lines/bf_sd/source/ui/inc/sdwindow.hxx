/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdwindow.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:31:58 $
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

#ifndef _SD_SDWINDOW_HXX
#define _SD_SDWINDOW_HXX

namespace binfilter {//STRIP009
class SdViewShell;
} //namespace binfilter

#ifndef _WINDOW_HXX //autogen
#include <vcl/window.hxx>
#endif
#ifndef _TRANSFER_HXX //autogen
#include <svtools/transfer.hxx>
#endif
namespace binfilter {

// Since we removed all old SV-stuff, there is no brush any more
// and so there is no BRUSH_SIZE defined in VCL.
// So I define it here
// #i2237#
// removed old stuff here which still forced zoom to be
// %BRUSH_SIZE which is outdated now
//#define BRUSH_SIZE		8

/** An SdWindow contains the actual working area of SdViewShell.

    <p>The zoom factor used by this class controls how much the page and the
    shapes on it are scaled down (<100%) or up (>100%) when displayed on the
    output device represented by the <type>OutputDevice</type>base class.  A
    zoom factor of 100% would result (with a correctly set DPI value for an
    output device) in a one to one mapping of the internal coordinates that
    are stored in 100th of mm.  The zoom factor is stored in the map mode
    member of the <type>OutputDevice</type> base class.  It is calculated to
    be an integer percent value.
*/
class SdWindow : public Window, public DropTargetHelper
{
 protected:
	SdWindow*	pShareWin;
	Point		aWinPos;
	Point		aViewOrigin;
	Size		aViewSize;
	USHORT		nMinZoom;
	USHORT		nMaxZoom;
    /** This flag tells whether to re-calculate the minimal zoom factor
        depening on the current zoom factor.  According to task #105436# its
        default value is now FALSE.
    */
	BOOL		bMinZoomAutoCalc;
	BOOL		bCalcMinZoomByMinSide;
	BOOL		bCenterAllowed;
	long		nTicks;
	BOOL		bDraggedFrom;

	SdViewShell* pViewShell;

	virtual void Resize();
	virtual void LoseFocus();

public:
	 SdWindow(Window* pParent);
	~SdWindow();

	void	SetViewShell(SdViewShell* pViewSh);


    /** Set the zoom factor to the specified value and center the display
        area arround the zoom center.  
        @param nZoom 
            The zoom factor is given as integral percent value.
    */

    /** This internally used method performs the actual adaption of the
        window's map mode to the specified zoom factor.  
        @param nZoom 
            The zoom factor is given as integral percent value.
        @return
            When the given zoom factor lies outside the valid range enclosed
            by the minimal zoom factor previously calculated by
            <member>CalcMinZoom</member> and a constant upper value it is
            forced into that interval.  Therefore the returned value is a
            valid zoom factor.
    */

    /** This method is called when the whole page shall be displayed in the
        window.  Position and zoom factor are set so that the given
        rectangle is displayed as large as possible in the window while at
        the same time maintaining the rectangle's aspect ratio and adding a
        small space at all its four sides (about 3% of width and height).
        As a result the this window's map mode is adapted accordingly.
        @param rZoomRect
            The rectangle is expected to be given relative to the upper left
            corner of the window in logical coordinates (100th of mm).
        @return 
            The new zoom factor is returned as integral percent value.
    */

	void	SetMinZoomAutoCalc(BOOL bAuto) { bMinZoomAutoCalc = bAuto; }
	void	SetCalcMinZoomByMinSide(BOOL bMin) { bCalcMinZoomByMinSide = bMin; }

    /** Calculate the minimal zoom factor as the value at which the
        application area would completely fill the window.  All values set
        manually or programatically are set to this value if they are
        smaller.  If the currently used zoom factor is smaller than the minimal zoom
        factor than set the minimal zoom factor as the new current zoom
        factor.  
    
        <p>This calculation is performed only when the
        <member>bMinZoomAutoCalc</member> is set (to <TRUE/>).</p>
    */
	void	CalcMinZoom();
	void	SetMinZoom(long nMin) { nMinZoom = (USHORT) nMin; }
	long	GetMinZoom() const { return nMinZoom; }
	void	SetMaxZoom(long nMax) { nMaxZoom = (USHORT) nMax; }
	long	GetMaxZoom() const { return nMaxZoom; }

	long	GetZoom() const
		{ return GetMapMode().GetScaleX().GetNumerator() * 100L /
				 GetMapMode().GetScaleX().GetDenominator(); }

	Point	GetWinViewPos() { return aWinPos; }
	Point	GetViewOrigin() { return aViewOrigin; }
	Size	GetViewSize()	{ return aViewSize; }
	void	SetWinViewPos(const Point& rPnt);
	void	SetViewOrigin(const Point& rPnt);
	void	SetViewSize(const Size& rSize);
	void	SetCenterAllowed(BOOL bIsAllowed) { bCenterAllowed = bIsAllowed; }

	void	UpdateMapOrigin(BOOL bInvalidate = TRUE);
	double	GetVisibleX();			// Interface fuer ScrollBars
	double	GetVisibleY();
	double	GetVisibleWidth();
	double	GetVisibleHeight();
	double	GetScrlLineWidth();
	double	GetScrlLineHeight();
	double	GetScrlPageWidth();
	double	GetScrlPageHeight();

	// DropTargetHelper

protected:
    /** Create an accessibility object that makes this window accessible.
        
        @return
            The returned reference is empty if an accessible object could
            not be created.
    */
};



} //namespace binfilter
#endif		// _SD_SDWINDOW_HXX
