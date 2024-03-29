/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: document.hxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 14:29:22 $
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
#ifndef DOCUMENT_HXX
#define DOCUMENT_HXX

#define SMDLL	1

#ifndef _SVSTOR_HXX //autogen
#include <bf_so3/svstor.hxx>
#endif
#ifndef _SOT_SOTREF_HXX //autogen
#include <sot/sotref.hxx>
#endif
#ifndef _SFX_OBJSH_HXX //autogen
#include <bf_sfx2/objsh.hxx>
#endif
#ifndef _SFX_INTERNO_HXX //autogen
#include <bf_sfx2/interno.hxx>
#endif
#ifndef _SFXLSTNER_HXX //autogen
#include <svtools/lstner.hxx>
#endif
#ifndef _SFX_OBJFAC_HXX //autogen
#include <bf_sfx2/docfac.hxx>
#endif
#ifndef _SV_VIRDEV_HXX //autogen
#include <vcl/virdev.hxx>
#endif

#ifndef _FORMAT_HXX
#include "format.hxx"
#endif
#ifndef PARSE_HXX
#include "parse.hxx"
#endif
#ifndef SMMOD_HXX
#include "smmod.hxx"
#endif
class Printer; 
namespace binfilter {

class SmNode;
class SmSymSetManager;
class SfxPrinter;


#ifndef SO2_DECL_SVSTORAGESTREAM_DEFINED
#define SO2_DECL_SVSTORAGESTREAM_DEFINED
SO2_DECL_REF(SvStorageStream)
#endif

#define HINT_DATACHANGED	1004

#define SM30BIDENT	 ((sal_uInt32)0x534D3033L)
#define SM30IDENT	 ((sal_uInt32)0x30334d53L)
#define SM304AIDENT  ((sal_uInt32)0x34303330L)
#define SM30VERSION  ((sal_uInt32)0x00010000L)
#define SM50VERSION	 ((sal_uInt32)0x00010001L)	//Unterschied zur SM30VERSION ist
											//der neue Border im Format.

#define FRMIDENT	((sal_uInt32)0x03031963L)
#define FRMVERSION	((sal_uInt32)0x00010001L)

#define STAROFFICE_XML	"StarOffice XML (Math)"
#define MATHML_XML		"MathML XML (Math)"

/* Zugriff auf den Drucker sollte ausschliesslich ueber diese Klasse erfolgen
 * ==========================================================================
 *
 * Der Drucker kann dem Dokument oder auch dem OLE-Container gehoeren. Wenn
 * das Dokument also eine OLE-Dokument ist, so gehoert der Drucker auch
 * grundsaetzlich dem Container. Der Container arbeitet aber eventuell mit
 * einer anderen MapUnit als der Server. Der Drucker wird bezueglich des MapMode
 * im Konstruktor entsprechend eingestellt und im Destruktor wieder restauriert.
 * Das bedingt natuerlich, das diese Klasse immer nur kurze Zeit existieren darf
 * (etwa waehrend des Paints).
 * Die Kontrolle darueber ob der Drucker selbst angelegt, vom Server besorgt
 * oder dann auch NULL ist, uebernimmt die DocShell in der Methode GetPrt(),
 * fuer die der Access auch Friend der DocShell ist.
*/

class SmDocShell;
class EditEngine;
class EditEngineItemPool;

class SmPrinterAccess
{
    Printer* pPrinter;
    OutputDevice* pRefDev;
public:
	SmPrinterAccess( SmDocShell &rDocShell );
	~SmPrinterAccess();
    Printer* GetPrinter()  { return pPrinter; }
    OutputDevice* GetRefDev()  { return pRefDev; }
};


////////////////////////////////////////////////////////////

class SmDocShell : public SfxObjectShell, public SfxInPlaceObject,
					public SfxListener
{
	friend class SmPrinterAccess;

	String				aText;
	SmFormat   			aFormat;
	SmParser	  		aInterpreter;
	SvStorageStreamRef	aDocStream;
    String              aAccText;
	SmSymSetManager	   *pSymSetMgr;
	SmNode		  	   *pTree;
	SvInPlaceMenuBar   *pMenuBar;
	SfxMenuBarManager  *pMenuMgr;
	SfxItemPool		   *pEditEngineItemPool;
	EditEngine		   *pEditEngine;
	SfxPrinter		   *pPrinter;		//Siehe Kommentar zum SmPrinter Access!
	Printer			   *pTmpPrinter;	//ebenfalls
	long				nLeftBorder,
						nRightBorder,
						nTopBorder,
						nBottomBorder;
	USHORT				nModifyCount;
	BOOL				bIsFormulaArranged;





	BOOL		Try3x( SvStorage *pStor, StreamMode eMode);
    BOOL        Try2x( SvStorage *pStor, StreamMode eMode);


	virtual void		FillClass(SvGlobalName* pClassName,
								  ULONG*  pFormat,
								  String* pAppName,
								  String* pFullTypeName,
								  String* pShortTypeName,
								  long	  nFileFormat = SOFFICE_FILEFORMAT_CURRENT) const;

	virtual ULONG		GetMiscStatus() const;
	virtual void		OnDocumentPrinterChanged( Printer * );
	virtual BOOL		InitNew(SvStorage *);
	virtual BOOL		Load(SvStorage *);
			void		ImplSave(  SvStorageStreamRef xStrm  );
	virtual BOOL		Save();
	virtual BOOL		SaveAs( SvStorage *pNewStor );
	virtual BOOL		SaveCompleted( SvStorage *pNewStor );
	virtual void		HandsOff();

    Printer             *GetPrt();
    OutputDevice*       GetRefDev();

    // used to convert the formula text between different office versions
    void                ConvertText( String &rText, SmConvert eConv );

	BOOL				IsFormulaArranged() const { return bIsFormulaArranged; }
	void				SetFormulaArranged(BOOL bVal) { bIsFormulaArranged = bVal; }
	void				ArrangeFormula();



    void                UpdateText();

public:
	TYPEINFO();
    SFX_DECL_OBJECTFACTORY_DLL(SmDocShell, SM_MOD());

				SmDocShell(SfxObjectCreateMode eMode = SFX_CREATE_MODE_EMBEDDED);
	virtual		~SmDocShell();


	//Zugriff fuer die View. Diese Zugriffe sind nur fuer den nicht OLE-Fall!
	//und fuer die Kommunikation mit dem SFX!
	//Alle internen Verwendungen des Printers sollten ausschlieslich uber
	//den SmPrinterAccess funktionieren.
	BOOL		HasPrinter()	{ return 0 != pPrinter; }
	SfxPrinter *GetPrinter()	{ GetPrt(); return pPrinter; }
	void	    SetPrinter( SfxPrinter * );


	void		SetText(const String& rBuffer);
	String& 	GetText() { return (aText); }
	SmFormat&	GetFormat() { return (aFormat); }

	void			Parse();
	SmParser &		GetParser() { return aInterpreter; }
	const SmNode *	GetFormulaTree() const 	{ return pTree; }
	void 			SetFormulaTree(SmNode *&rTree) { pTree = rTree; }


	EditEngine & 	GetEditEngine();

	SmSymSetManager &		GetSymSetManager();
	const SmSymSetManager &	GetSymSetManager() const
	{
		return ((SmDocShell *) this)->GetSymSetManager();
	}

	void		Draw(OutputDevice &rDev, Point &rPosition);
	Size		GetSize();

	void		Resize();


	virtual 	SfxItemPool& GetPool();

	virtual void SetVisArea (const Rectangle & rVisArea);

	virtual void SetModified(BOOL bModified);
};


} //namespace binfilter
#endif

