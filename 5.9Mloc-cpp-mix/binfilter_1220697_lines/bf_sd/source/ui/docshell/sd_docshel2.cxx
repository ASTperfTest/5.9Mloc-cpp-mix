/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sd_docshel2.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: kz $ $Date: 2007/09/06 11:22:48 $
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


#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "helpids.h"
#include "docshell.hxx"
#include "drawdoc.hxx"
#include "sdpage.hxx"
#include "sdview.hxx"
#include "strings.hrc"
#include "res_bmp.hrc"
#include "strmname.h"
namespace binfilter {

/*************************************************************************
|*
|* Zeichnen der DocShell (mittels der Hilfsklasse SdDrawViewShell)
|*
\************************************************************************/


/*************************************************************************
|*
|*
|*
\************************************************************************/

/*N*/ void SdDrawDocShell::SetVisArea(const Rectangle& rRect)
/*N*/ {
//    SfxInPlaceObject::SetVisArea(rRect);

/*N*/ 	if (GetCreateMode() == SFX_CREATE_MODE_EMBEDDED)
/*N*/ 	{
/*N*/ 		// Setzt das Modified-Flag
/*N*/ 		SfxInPlaceObject::SetVisArea(rRect);
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*N*/ 		SvEmbeddedObject::SetVisArea(rRect);
/*N*/ 	}
/*N*/ }

/*************************************************************************
|*
|*
|*
\************************************************************************/

/*N*/ Rectangle SdDrawDocShell::GetVisArea(USHORT nAspect) const
/*N*/ {
/*N*/ 	Rectangle aVisArea;
/*N*/ 
/*N*/ 	if( ( ASPECT_THUMBNAIL == nAspect ) || ( ASPECT_DOCPRINT == nAspect ) )
/*N*/ 	{
/*N*/ 		// Groesse der ersten Seite herausgeben
/*?*/ 		MapMode aSrcMapMode(MAP_PIXEL);
/*?*/ 		MapMode aDstMapMode(MAP_100TH_MM);
/*?*/ 		Size aSize = pDoc->GetSdPage(0, PK_STANDARD)->GetSize();
/*?*/ 		aSrcMapMode.SetMapUnit(MAP_100TH_MM);
/*?*/ 
/*?*/ 		aSize = Application::GetDefaultDevice()->LogicToLogic(aSize, &aSrcMapMode, &aDstMapMode);
/*?*/ 		aVisArea.SetSize(aSize);
/*N*/ 	}
/*N*/ 	else
/*N*/ 	{
/*N*/ 		aVisArea = SfxInPlaceObject::GetVisArea(nAspect);
/*N*/ 	}
/*N*/ 
/*N*/ 	return (aVisArea);
/*N*/ }

/*N*/ FrameView* SdDrawDocShell::GetFrameView()
/*N*/ {
/*N*/ 	return NULL;
/*N*/ }

/*************************************************************************
|*
|* Groesse der ersten Seite zurueckgeben
|*
\************************************************************************/


/*************************************************************************
|*
|*
|*
\************************************************************************/


/*************************************************************************
|*
|* Bitmap einer beliebigen Seite erzeugen
|*
\************************************************************************/



/*************************************************************************
|*
|* Pruefen, ob die Seite vorhanden ist und dann den Anwender zwingen einen
|* noch nicht vorhandenen Namen einzugeben. Wird FALSE zurueckgegeben,
|* wurde die Aktion vom Anwender abgebrochen.
|*
\************************************************************************/



}
