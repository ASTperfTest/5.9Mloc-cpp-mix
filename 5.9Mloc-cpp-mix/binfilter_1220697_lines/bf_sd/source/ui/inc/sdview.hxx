/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdview.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:31:46 $
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

#ifndef _SD_SDVIEW_HXX
#define _SD_SDVIEW_HXX

#ifndef _PRESENTATION_HXX
#include "pres.hxx"
#endif
#ifndef _GEN_HXX //autogen
#include <tools/gen.hxx>
#endif
#ifndef _TRANSFER_HXX //autogen
#include <svtools/transfer.hxx>
#endif
#ifndef _SVX_FMVIEW_HXX
#include <bf_svx/fmview.hxx>
#endif
#ifndef _SVDMARK_HXX //autogen
#include <bf_svx/svdmark.hxx>
#endif
#ifndef _SVDVMARK_HXX //autogen
#include <bf_svx/svdvmark.hxx>
#endif
#ifndef _SVDPAGE_HXX //autogen
#include <bf_svx/svdpage.hxx>
#endif
class OutputDevice;
class VirtualDevice;
class ImageMap;
class Point;
class Graphic;
class TransferableDataHelper;
namespace binfilter {

class SdWindow;
class SdDrawDocument;
class SdDrawDocShell;
class SdViewShell;
class SdrOle2Obj;
class SdrGrafObj;
class SdrOutliner;
struct StyleRequestData;
struct SdNavigatorDropEvent;

// -------------------
// - SdViewRedrawRec -
// -------------------

struct SdViewRedrawRec
{
	OutputDevice* pOut;
	Rectangle	  aRect;
};

// ----------
// - SdView -
// ----------

class SdView : public FmFormView
{
protected:

	SdDrawDocument* 	    pDoc;
	SdDrawDocShell* 	    pDocSh;
	SdViewShell*		    pViewSh;
	SdrMarkList*		    pDragSrcMarkList;
	SdrObject*			    pDropMarkerObj;
	SdrViewUserMarker*	    pDropMarker;
	USHORT				    nDragSrcPgNum;
	Point				    aDropPos;
	::std::vector< String >	aDropFileVector;
	sal_Int8			    nAction;
	Timer				    aDropErrorTimer;
	Timer				    aDropInsertFileTimer;
	USHORT				    nLockRedrawSmph;
	List*				    pLockedRedraws;
	BOOL				    bIsDropAllowed;

					        DECL_LINK( DropErrorHdl, Timer* );
					        DECL_LINK( DropInsertFileHdl, Timer* );
public:

	                        TYPEINFO();

	                        SdView( SdDrawDocument* pDrawDoc, OutputDevice* pOutDev, SdViewShell* pViewSh=NULL );
	virtual                 ~SdView();


	virtual void	        MarkListHasChanged();
	virtual BOOL	        HasMarkedObj() const { return SdrView::HasMarkedObj(); }
	virtual void	        ModelHasChanged();


    void                    UpdateSelectionClipboard( BOOL bForceDeselect );

	SdDrawDocShell*         GetDocSh() const { return pDocSh; }
	SdDrawDocument*         GetDoc() const { return pDoc; }







	BOOL	                IsMorphingAllowed() const;
	BOOL	                IsVectorizeAllowed() const;




};

} //namespace binfilter
#endif // _SD_SDVIEW_HXX
