/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fusel.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:27:13 $
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

#ifndef _SD_FUSEL_HXX
#define _SD_FUSEL_HXX

#ifndef _SD_FUDRAW_HXX
#include "fudraw.hxx"
#endif
class Sound;
namespace binfilter {

class SdrHdl;
class SdrObject;


/*************************************************************************
|*
|* Basisklasse fuer alle Funktionen
|*
\************************************************************************/

class FuSelection : public FuDraw
{
 protected:
	BOOL			bTempRotation;
	BOOL            bSelectionChanged;
	BOOL            bHideAndAnimate;
	SdrHdl*         pHdl;
	BOOL            bSuppressChangesOfSelection;
	BOOL            bMirrorSide0;
	USHORT          nEditMode;
	Sound*          pSound;

					DECL_STATIC_LINK( FuSelection, StaticSoundHasStoppedHdl, Sound* );

 public:
	TYPEINFO();

	FuSelection(SdViewShell* pViewSh, SdWindow* pWin, SdView* pView,
				SdDrawDocument* pDoc, SfxRequest& rReq);

	virtual ~FuSelection();
									   // Mouse- & Key-Events

	virtual void Activate();		   // Function aktivieren
	virtual void Deactivate();		   // Function deaktivieren

	virtual void SelectionHasChanged();

	USHORT  GetEditMode() { return nEditMode; }


	/** is called when the currenct function should be aborted. <p>
		This is used when a function gets a KEY_ESCAPE but can also
		be called directly.

		@returns true if a active function was aborted
	*/

private:
    /** This pointer stores a canidate for assigning a style in the water
        can mode between mouse button down and mouse button up.
    */
    SdrObject* pWaterCanCandidate;

    /** Find the object under the given test point without selecting it.
        @param rTestPoint
            The coordinates at which to search for a shape.
        @return
            The shape at the test point.  When there is no shape at this
            position then NULL is returned.
    */
    SdrObject* pickObject (const Point& rTestPoint);
};



} //namespace binfilter
#endif		// _SD_FUSEL_HXX

