/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: filter.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 02:35:55 $
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

#ifndef SC_FILTER_HXX
#define SC_FILTER_HXX

#ifndef _STRING_HXX //autogen
#include <tools/string.hxx>
#endif
#ifndef _RTL_TEXTENC_H
#include <rtl/textenc.h>
#endif
class SvStream;
namespace binfilter {

class SfxMedium;

class ScAddress;
class ScDocument;
class ScRange;

// Return-Werte Im-/Exportfilter	(ULONG)

typedef ULONG FltError;
//enum FltError {

#define eERR_OK  		ERRCODE_NONE				// kein Fehler, alles OK
#define	eERR_OPEN		SCERR_IMPORT_OPEN			// ...
#define	eERR_UNBEK		SCERR_IMPORT_UNKNOWN		// unbekannter Fehler, auch historische Bedeutung
#define	eERR_FORMAT		SCERR_IMPORT_FORMAT			// Formatfehler beim Lesen (kein Formel-Fehler!)
#define	eERR_NI			SCERR_IMPORT_NI				// Nicht implementierter Filter
#define	eERR_RNGOVRFLW	SCWARN_IMPORT_RANGE_OVERFLOW// ueberlauf der Zellkoordinaten:
													//	Tabelle abgschnitten auf erlaubtem Bereich
// mehr Fehlercodes siehe scerrors.hxx

//	};


// fuer Import
enum EXCIMPFORMAT { EIF_AUTO, EIF_BIFF5, EIF_BIFF8, EIF_BIFF_LE4 };

// fuer Export
enum ExportFormatLotus { ExpWK1, ExpWK3, ExpWK4 };
enum ExportFormatExcel { ExpBiff2, ExpBiff3, ExpBiff4, ExpBiff4W, ExpBiff5 };


// Optionen fuer DIF-Im-/Export (Kombination ueber '|')
#define	SC_DIFOPT_PLAIN		0x00000000
#define SC_DIFOPT_DATE		0x00000001
#define SC_DIFOPT_TIME		0x00000002
#define SC_DIFOPT_CURRENCY	0x00000004

#define SC_DIFOPT_EXCEL		(SC_DIFOPT_DATE|SC_DIFOPT_TIME|SC_DIFOPT_CURRENCY)


// ***********************************************************************
// Diverse Importfilter
// ***********************************************************************

FltError ScImportLotus123( SfxMedium&, ScDocument*, CharSet eSrc = RTL_TEXTENCODING_DONTKNOW );

FltError ScImportExcel( SvStream&, ScDocument* );

FltError ScImportExcel( SfxMedium&, ScDocument* );

FltError ScImportExcel( SfxMedium&, ScDocument*, const EXCIMPFORMAT );
		// eFormat == EIF_AUTO	-> passender Filter wird automatisch verwendet
		// eFormat == EIF_BIFF5	-> nur Biff5-Stream fuehrt zum Erfolg (auch wenn in einem Excel97-Doc)
		// eFormat == EIF_BIFF8	-> nur Biff8-Stream fuehrt zum Erfolg (nur in Excel97-Docs)
		// eFormat == EIF_BIFF_LE4 -> nur Nicht-Storage-Dateien _koennen_ zum Erfolg fuehren

FltError ScImportStarCalc10( SvStream&, ScDocument* );

FltError ScImportDif( SvStream&, ScDocument*, const ScAddress& rInsPos,
						const CharSet eSrc = RTL_TEXTENCODING_DONTKNOW, UINT32 nDifOption = SC_DIFOPT_EXCEL );

FltError ScImportRTF( SvStream&, ScDocument*, ScRange& rRange );

FltError ScImportHTML( SvStream&, ScDocument*, ScRange& rRange, double nOutputFactor = 1.0, BOOL bCalcWidthHeight = TRUE );

// ***********************************************************************
// Diverse Exportfilter
// ***********************************************************************

FltError ScExportLotus123( SvStream&, ScDocument*, ExportFormatLotus, CharSet eDest );

FltError ScExportExcel234( SvStream&, ScDocument*, ExportFormatExcel, CharSet eDest );

FltError ScExportExcel5( SfxMedium&, ScDocument*, const BOOL bTuerk, CharSet eDest );

FltError ScExportDif( SvStream&, ScDocument*, const ScAddress& rOutPos, const CharSet eDest,
						UINT32 nDifOption = SC_DIFOPT_EXCEL );

FltError ScExportDif( SvStream&, ScDocument*, const ScRange& rRange, const CharSet eDest,
						UINT32 nDifOption = SC_DIFOPT_EXCEL );

FltError ScExportHTML( SvStream&, ScDocument*, const ScRange& rRange, const CharSet eDest, BOOL bAll,
                        const String& rStreamPath, String& rNonConvertibleChars );

FltError ScExportRTF( SvStream&, ScDocument*, const ScRange& rRange, const CharSet eDest );

} //namespace binfilter
#endif

