/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: drawdoc.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:39:42 $
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
#ifndef _DRAWDOC_HXX
#define _DRAWDOC_HXX

#ifndef _FM_FMMODEL_HXX
#include <bf_svx/fmmodel.hxx>
#endif
class Window; 
class SbxValue; 
class SbxObject; 
namespace binfilter {

class SwDoc;
class SwDocShell;
class SjJSbxObject;


//==================================================================

class SwDrawDocument : public FmFormModel
{
	SwDoc* pDoc;
public:
	SwDrawDocument( SwDoc* pDoc );
    SwDrawDocument( SfxItemPool *pPool, SwDocShell *pDocSh );
	~SwDrawDocument();

	const SwDoc& GetDoc() const	{ return *pDoc; }
		  SwDoc& GetDoc()      	{ return *pDoc; }

	virtual SdrPage* AllocPage(FASTBOOL bMasterPage);

	// fuers "load on demand" von Grafiken im DrawingLayer
	virtual SvStream* GetDocumentStream( SdrDocumentStreamInfo& rInfo ) const;

	// fuers Speicher von Rechtecken als Control-Ersatz fuker Versionen < 5.0
};


} //namespace binfilter
#endif
