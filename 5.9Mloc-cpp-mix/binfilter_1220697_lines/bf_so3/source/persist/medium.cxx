/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: medium.cxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: vg $ $Date: 2007/10/23 13:52:32 $
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

 #include <medium.hxx>
 #include <bf_so3/svstor.hxx>

 /********************** SvMedium ******************************************
 **************************************************************************/
 #define STM_STD_STREAM  "standard stream"

 /*************************************************************************
 |*    SvMedium::SvMedium()
 |*
 |*    Beschreibung
 |*    Ersterstellung    MM 19.10.94
 |*    Letzte Aenderung  MM 19.10.94
 *************************************************************************/
 #define IMPL_CTOR()                         \
	 eError(SVSTREAM_OK),                    \
											 \
	 bDirect(TRUE),                          \
	 bSetFilter(FALSE),                      \
											 \
	 nStorOpenMode(STREAM_STD_READWRITE),    \
	 pInStream(0),                           \
	 pOutStream(0)

 SvMedium::SvMedium() :
	 bRoot(FALSE),
	 IMPL_CTOR()
 {
 }

 /*************************************************************************
 |*    SvMedium::SvMedium()
 |*
 |*    Beschreibung
 |*    Ersterstellung    MM 19.10.94
 |*    Letzte Aenderung  MM 19.10.94
 *************************************************************************/
 SvMedium::SvMedium( const DirEntry &rName,
					 StreamMode nOpenMode,
					 BOOL bDirectP ) :
	 bRoot(TRUE),
	 aName( rName.GetFull() ),
	 IMPL_CTOR()
 {
	 bDirect       = bDirectP;
	 nStorOpenMode = nOpenMode;
 }

 /*************************************************************************
 |*    SvMedium::SvMedium()
 |*
 |*    Beschreibung
 *************************************************************************/
 SvMedium::SvMedium( SvStorage *pStorage, BOOL bRootP ) :
	 bRoot(bRootP),
	 aStorage(pStorage),
	 IMPL_CTOR()
 {
	 if ( pStorage )
	 {
		 aName = pStorage->GetName();
		 if( pStorage->IsRoot() )
			 // Wenn Storage Root, dann auf jedenfall Root
			 bRoot = TRUE;
	 }
	 else
		 bRoot = TRUE;
 }

 /*************************************************************************
 |*    SvMedium::~SvMedium()
 |*
 |*    Beschreibung
 *************************************************************************/
 SvMedium::~SvMedium()
 {
	 delete pOutStream;
	 delete pInStream;
 }

 /*************************************************************************
 |*    SvMedium::GetClassFilter()
 |*
 |*    Beschreibung
 *************************************************************************/
 const SvGlobalName&  SvMedium::GetClassFilter()
 {
	 if( !bSetFilter && GetStorage() )
		 SetClassFilter( GetStorage()->GetClassName() );
	 return aFilterClass;
 }

 /*************************************************************************
 |*    SvMedium::ResetError()
 |*
 |*    Beschreibung
 *************************************************************************/
 void SvMedium::ResetError()
 {
	 eError = SVSTREAM_OK;
	 if( aStorage.Is() )
		 aStorage->ResetError();
	 if( pInStream )
		 pInStream->ResetError();
	 if( pOutStream )
		 pOutStream->ResetError();
 }


 ULONG SvMedium::GetErrorCode() const
 {
	 ULONG lError=eError;
	 if(!lError && pInStream)
		 lError=pInStream->GetErrorCode();
	 if(!lError && pOutStream)
		 lError=pOutStream->GetErrorCode();
	 if(!lError && aStorage.Is())
		 lError=aStorage->GetErrorCode();
	 return lError;
 }

 /*************************************************************************
 |*    SvMedium::GetInStream()
 |*
 |*    Beschreibung
 *************************************************************************/
 SvStream* SvMedium::GetInStream()
 {
	 if( !pInStream && !aStorage.Is() )
	 {
		 pInStream = new SvFileStream( aName, nStorOpenMode );
		 eError = pInStream->GetError();
		 if( !eError && (nStorOpenMode & STREAM_WRITE)
			&& ! pInStream->IsWritable())
			 eError = ERRCODE_IO_ACCESSDENIED;

		 if( eError != SVSTREAM_OK )
		 {
			 delete pInStream;
			 pInStream = NULL;
		 }
	 }
	 return pInStream;
 }

 /*************************************************************************
 |*    SvMedium::ReleaseInStream()
 |*
 |*    Beschreibung
 *************************************************************************/
 void SvMedium::CloseInStream()
 {
	 delete pInStream;
	 pInStream = NULL;
 }

 /*************************************************************************
 |*    SvMedium::GetOutStream()
 |*
 |*    Beschreibung
 *************************************************************************/
 SvStream* SvMedium::GetOutStream()
 {
	 if( !pOutStream && !aStorage.Is() )
	 {
		 StreamMode nMode = nStorOpenMode | STREAM_SHARE_DENYREAD;
		 DirEntry aFile( aName );
		 if ( pInStream )
			 pInStream->Close();
		 pOutStream = new SvFileStream( aFile.GetFull(), nMode );
		 eError = pOutStream->GetError();
		 if( !eError && (nStorOpenMode & STREAM_WRITE)
			&& ! pOutStream->IsWritable())
			 eError = ERRCODE_IO_ACCESSDENIED;
		 if( eError != SVSTREAM_OK )
		 {
			 delete pOutStream;
			 pOutStream = NULL;
		 }

		 return pOutStream;
	 }
	 // auf dem gleichen Stream arbeiten
	 return GetInStream();
 }

 /*************************************************************************
 |*    SvMedium::CloseOutStream()
 |*
 |*    Beschreibung
 *************************************************************************/
 BOOL SvMedium::CloseOutStream()
 {
	 if( pOutStream )
	 {
		 // Der Stream muss weiter existieren, damit
		 // GetStorage() weiter unten nicht einen
		 // Stroage liefert, der dann beim Committen die
		 // Datei breitmacht!
		 pOutStream->Close();
		 if ( pInStream )
		 {
			 pInStream->ReOpen();
			 eError = pInStream->GetError();
		 }
	 }

	 return BOOL( GetError() == SVSTREAM_OK );
 }

 /*************************************************************************
 |*    SvMedium::Commit()
 |*
 |*    Beschreibung
 *************************************************************************/
 BOOL SvMedium::Commit()
 {
	 if( !bDirect )
	 {
		 if( aStorage.Is() )
		 {
			 // StorageStream immer direkt
			 if( !aStorage->Commit() )
				 eError = aStorage->GetError();
		 }
		 // MD: Wenn ich das richtig sehe,ist der Commit ein
		 // reiner Flush, also kann der Output-Stream hier nicht
		 // geschlossen werden!

		 // TLX: Er muss doch geschlossen werden, denn offenbar ist
		 // es so gedacht: Arbeit mit dem InStream -> Arbeit mit dem
		 // OutStream -> Commit -> von vorn.

		 else if( pOutStream && pOutStream->IsOpen() )
		 {
			 pOutStream->Flush();
			 CloseOutStream();
		 }
		 else if( bDirect && pInStream && pInStream->IsOpen() )
			 pInStream->Flush();
	}
	return GetError() == SVSTREAM_OK;
}

/*************************************************************************
|*    SvMedium::IsStorage()
|*
|*    Beschreibung
*************************************************************************/
BOOL SvMedium::IsStorage() const
{
	return aStorage.Is()
		   || SvStorage::IsStorageFile( GetName() );
}

/*************************************************************************
|*    SvMedium::GetStorage()
|*
|*    Beschreibung
*************************************************************************/
SvStorage* SvMedium::GetStorage()
{
	if( !aStorage.Is() && GetError() == SVSTREAM_OK && !pInStream && !pOutStream )
	{
		aStorage = new SvStorage( aName, nStorOpenMode, bDirect );
		eError = aStorage->GetError();
		if( eError != SVSTREAM_OK )
			aStorage.Clear();
	}
	return aStorage;
}

/*************************************************************************
|*    SvMedium::CloseStorage()
|*
|*    Beschreibung: damit externe Filter an das file kommen
*************************************************************************/
void SvMedium::CloseStorage()
{
	aStorage.Clear();
}

/*************************************************************************
|*    SvMedium::SetOpenMode()
|*
|*    Beschreibung
*************************************************************************/
void SvMedium::SetOpenMode( StreamMode nStorOpen, BOOL bDirectP )
{
	aStorage.Clear();
	DELETEZ( pOutStream );
	DELETEZ( pInStream );
	nStorOpenMode	= nStorOpen;
	bDirect 		= bDirectP;
	bSetFilter  	= FALSE;
}



