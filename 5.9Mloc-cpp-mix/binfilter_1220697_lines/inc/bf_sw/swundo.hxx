/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: swundo.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:57:41 $
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
#ifndef _SWUNDO_HXX
#define _SWUNDO_HXX

class String; 
namespace binfilter {

// die Ids fuer StdAktionen
enum SwUndoStdId
{
	UNDO_STD_BEGIN = 1,
	UNDO_START = UNDO_STD_BEGIN,    		//  1
	UNDO_END,                               //  2
		REPEAT_START,				// alle UndoIds zwischen REPEAT_START und
									// REPEAT_END sind Repeat-Faehig !!
	UNDO_DELETE = REPEAT_START,            	//  3
	UNDO_INSERT,							//  4
	UNDO_OVERWRITE,                         //  5
	UNDO_SPLITNODE,                         //  6
	UNDO_INSATTR,                           //  7
	UNDO_SETFMTCOLL,                        //  8
	UNDO_RESETATTR,                         //  9
	UNDO_INSFMTATTR,                        // 10
	UNDO_INSDOKUMENT,                       // 11
	UNDO_COPY,                              // 12
	UNDO_INSTABLE,                          // 13
	UNDO_TABLETOTEXT,                       // 14
	UNDO_TEXTTOTABLE,                       // 15
	UNDO_SORT_TXT,                          // 16
	UNDO_INSLAYFMT,                         // 17
	UNDO_TABLEHEADLINE,                     // 18
	UNDO_INSSECTION,                        // 19
	UNDO_OUTLINE_LR,                        // 20
	UNDO_OUTLINE_UD,                        // 21
	UNDO_INSNUM,                            // 22
	UNDO_NUMUPDOWN,                         // 23
	UNDO_MOVENUM,                           // 24
	UNDO_INSDRAWFMT,                        // 25
	UNDO_NUMORNONUM,                        // 26
	UNDO_INC_LEFTMARGIN,                    // 27
	UNDO_DEC_LEFTMARGIN,                    // 28
	UNDO_INSERTLABEL,                       // 29
	UNDO_SETNUMRULESTART,                   // 30
	UNDO_CHGFTN,                            // 31
	UNDO_REDLINE,                           // 32
	UNDO_ACCEPT_REDLINE,                    // 33
	UNDO_REJECT_REDLINE,                    // 34
	UNDO_SPLIT_TABLE,                       // 35
	UNDO_DONTEXPAND,                        // 36
	UNDO_AUTOCORRECT,                    	// 37
	UNDO_MERGE_TABLE,                    	// 38
	UNDO_TRANSLITERATE,                     // 39

	UNDO_REPEAT_DUMMY_4,                    // 40
	UNDO_REPEAT_DUMMY_5,                    // 41
	UNDO_REPEAT_DUMMY_6,                    // 42
	UNDO_REPEAT_DUMMY_7,                    // 43
	UNDO_REPEAT_DUMMY_8,                    // 44
	UNDO_REPEAT_DUMMY_9,                    // 45
	REPEAT_END,

	UNDO_MOVE = REPEAT_END,                 // 46
	UNDO_INSGLOSSARY,                       // 47
	UNDO_DELBOOKMARK,                       // 48
	UNDO_INSBOOKMARK,                       // 49
	UNDO_SORT_TBL,                          // 50
	UNDO_DELLAYFMT,                         // 51
	UNDO_AUTOFORMAT,                        // 52
	UNDO_REPLACE,                           // 53
	UNDO_DELSECTION,                        // 54
	UNDO_CHGSECTION,                        // 55
	UNDO_CHGSECTIONPASSWD,                  // 56
	UNDO_SETDEFTATTR,                       // 57
	UNDO_DELNUM,                            // 58
	UNDO_DRAWUNDO,                          // 59
	UNDO_DRAWGROUP,                         // 60
	UNDO_DRAWUNGROUP,                       // 61
	UNDO_DRAWDELETE,                        // 62
	UNDO_REREAD,                            // 63
	UNDO_DELGRF,                            // 64
	UNDO_DELOLE,                            // 65
	UNDO_TABLE_ATTR,                        // 66
	UNDO_TABLE_AUTOFMT,                     // 67
	UNDO_TABLE_INSCOL,                      // 68
	UNDO_TABLE_INSROW,                      // 69
	UNDO_TABLE_DELBOX,                      // 70
	UNDO_TABLE_SPLIT,                       // 71
	UNDO_TABLE_MERGE,                       // 72
	UNDO_TBLNUMFMT,                         // 73
	UNDO_INSTOX,                            // 74
	UNDO_CLEARTOXRANGE,                     // 75
	UNDO_TBLCPYTBL,                         // 76
	UNDO_CPYTBL,                            // 77
	UNDO_INS_FROM_SHADOWCRSR,               // 78
	UNDO_CHAINE,                            // 79
	UNDO_UNCHAIN,                           // 80
	UNDO_FTNINFO,                           // 81
	UNDO_ENDNOTEINFO,                       // 82
	UNDO_COMPAREDOC,                        // 83
	UNDO_SETFLYFRMFMT,                      // 84
	UNDO_SETRUBYATTR,						// 85

	UNDO_TMPAUTOCORR,                       // 86 #102505#
	UNDO_DUMMY_3,                           // 87
	UNDO_DUMMY_4,                           // 88
	UNDO_DUMMY_5,                           // 89
	UNDO_DUMMY_6,                           // 90
	UNDO_DUMMY_7,                           // 91
	UNDO_DUMMY_8,                           // 92
	UNDO_DUMMY_9,                           // 93
	UNDO_STD_END                            // 94
};


#define INIT_UNDOIDS 20
#define GROW_UNDOIDS 32
// Das Array der verwendeten Undo-Ids




// Undo-Ids fuer die UI-Seite
enum SwUIUndoIds
{
	UIUNDO_REPLACE             =  UNDO_STD_END,
	UIUNDO_INSERT_PAGE_BREAK,
	UIUNDO_INSERT_COLUMN_BREAK,
	UIUNDO_PLAY_MACRO,
	UIUNDO_INSERT_ENVELOPE,
	UIUNDO_DRAG_AND_COPY,
	UIUNDO_DRAG_AND_MOVE,
	UIUNDO_INSERT_RULER,
	UIUNDO_INSERT_CHART,
	UIUNDO_INSERT_FOOTNOTE,
	UIUNDO_INSERT_URLBTN,
	UIUNDO_INSERT_URLTXT,
	UIUNDO_DELETE_INVISIBLECNTNT
};


} //namespace binfilter
#endif
