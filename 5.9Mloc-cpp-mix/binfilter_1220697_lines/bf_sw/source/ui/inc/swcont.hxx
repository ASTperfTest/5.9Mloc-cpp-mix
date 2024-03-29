/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: swcont.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 00:54:19 $
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

#ifndef _SWCONT_HXX
#define _SWCONT_HXX

#ifndef _STRING_HXX //autogen
#include <tools/string.hxx>
#endif
namespace binfilter {

class SwContentType;

//Reihenfolge und Anzahl mit ResIds abgleichen!!
#define CONTENT_TYPE_OUTLINE		0
#define CONTENT_TYPE_TABLE 			1
#define CONTENT_TYPE_FRAME          2
#define CONTENT_TYPE_GRAPHIC        3
#define CONTENT_TYPE_OLE            4
#define CONTENT_TYPE_BOOKMARK       5
#define CONTENT_TYPE_REGION         6
#define CONTENT_TYPE_URLFIELD		7
#define CONTENT_TYPE_REFERENCE      8
#define CONTENT_TYPE_INDEX			9
#define CONTENT_TYPE_POSTIT			10
#define CONTENT_TYPE_DRAWOBJECT     11
#define CONTENT_TYPE_MAX 			CONTENT_TYPE_DRAWOBJECT +1


// Typen fuer das Globaldokument
#define GLOBAL_CONTENT_REGION		100
#define GLOBAL_CONTENT_INDEX        101
#define GLOBAL_CONTENT_TEXT        	102
#define GLOBAL_CONTENT_MAX			3

// Strings fuer Kontextmenue
#define CONTEXT_COUNT 	12
#define GLOBAL_CONTEXT_COUNT 14

// Modi fuer Drag 'n Drop
#define REGION_MODE_NONE    	0
#define REGION_MODE_LINK        1
#define REGION_MODE_EMBEDDED    2

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//mini rtti
 class SwTypeNumber
 {
 	BYTE nTypeId;

 	public:
 		SwTypeNumber(BYTE nId) :nTypeId(nId){}
 		virtual ~SwTypeNumber();

 		virtual BYTE	GetTypeId();
 };
//----------------------------------------------------------------------------

class SwContent : public SwTypeNumber
{
	const SwContentType*  	pParent;
	String 					sContentName;
	long					nYPosition;
	BOOL					bInvisible;
public:
		SwContent(const SwContentType* pCnt, const String& rName, long nYPos );

	const SwContentType* 	GetParent() const {return pParent;}
	const String&			GetName() 	const {return sContentName;}
	int						operator==(const SwContent& rCont) const
								{
									//gleich sind sie nie, sonst fallen sie aus dem Array
									return FALSE;
								}
	int						operator<(const SwContent& rCont) const
								{
									//zuerst nach Position dann nach Name sortieren
									return nYPosition != rCont.nYPosition ?
										nYPosition < rCont.nYPosition :
											sContentName < rCont.sContentName;;
								}

	long		GetYPos() const {return nYPosition;}

	BOOL		IsInvisible() const {return bInvisible;}
	void		SetInvisible(){ bInvisible = TRUE;}
};

} //namespace binfilter
#endif
