/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: frmids.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 10:05:27 $
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

#ifndef _FRMIDS_HXX
#define	_FRMIDS_HXX

// Aktuelle Version:
namespace binfilter {

#define	FRMVER_NEWFLY	0x0002		// Neue Ablage der absatzgebundenen Flys
#define	FRMVER_EXTINFO	0x0003		// Optionales Extended-Info-Byte
#define FRMVER_NEWALIGN 0x0004		// Alignment wurde Layoutseitig geaendert
									// es muss geeignet invalidiert werden.

#define	FRM_VERSION		0x0004

// ID-Byte als 1. Byte:

#define	FRMID_ROOT		'R'			// Root-Frame
#define	FRMID_PAGE		'P'			// Seite
#define	FRMID_HEADER	'H'			// Kopfzeilen
#define	FRMID_FOOTER	'F'			// Fusszeilen
#define	FRMID_BODY		'B'			// Arbeitsbereich
#define	FRMID_FTNCONT	'C'			// Fussnoten-Container
#define	FRMID_FTN		'N'			// Fussnote
#define	FRMID_TEXT 		'X'			// Text-Frame
#define	FRMID_GRAPHICS	'G'			// Grafik-Frame
#define	FRMID_OLE		'O'			// OLE-Frame (reserviert)
#define	FRMID_ATCNTNT	'A'			// am Inhalt gebundener Rahmen
#define	FRMID_INCNTNT	'I'			// im Inhalt gebundener Rahmen
#define	FRMID_FLYLAY	'L'			// Seiten/spaltengebundener Rahmen
#define	FRMID_COLUMN	'M'			// Spalten-Frame
#define	FRMID_TABLE		'T'			// Tabellen-Frame
#define	FRMID_TABROW	'r'			// Tabellen-Zeile
#define	FRMID_TABCELL	'c'			// Tabellen-Zelle

// Flag-Bits im 2. Byte:

#define	FRMF_PRINT		0x01		// Frame hat eigenen Print-Rahmen
#define	FRMF_HASFLY		0x02		// Frame hat FlyFrames
#define	FRMF_HASLOWER	0x04		// Frame hat Lower Frames
#define	FRMF_HASFOLLOW	0x08		// Frame *hat* Follow-Frames
#define	FRMF_ISFOLLOW	0x10		// Frame *ist* Follow-Frame
#define	FRMF_SECTID1	0x20		// Section-ID ist einstellig
#define	FRMF_NODEID1	0x40		// Node-ID ist einstellig
#define	FRMF_HASID		0x80		// Frame hat eigenen ID

// Zusatz-Bit, vom Reader in nFrmFlags zu setzen

#define	FRMF_INVALID	0x8000		// Frames ungueltig setzen


} //namespace binfilter
#endif