/*

Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2009 Miranda ICQ/IM project,
all portions of this codebase are copyrighted to the people
listed in contributors.txt.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "commonheaders.h"

#include "../clist/clc.h"

bool CheckProtocolOrder(void);
void 	BuildProtoMenus();

static BOOL bModuleInitialized = FALSE;

static int CompareAccounts( const PROTOACCOUNT* p1, const PROTOACCOUNT* p2 )
{
	return lstrcmpA( p1->szModuleName, p2->szModuleName );
}

LIST<PROTOACCOUNT> accounts( 10, CompareAccounts );

/////////////////////////////////////////////////////////////////////////////////////////

static int EnumDbModules(const char *szModuleName, DWORD ofsModuleName, LPARAM lParam)
{
	DBVARIANT dbv;
	if ( !DBGetContactSettingString( NULL, szModuleName, "AM_BaseProto", &dbv )) {
		if (!Proto_GetAccount( szModuleName )) {
			PROTOACCOUNT* pa = ( PROTOACCOUNT* )mir_calloc( sizeof( PROTOACCOUNT ));
			pa->cbSize = sizeof( *pa );
			pa->type = PROTOTYPE_PROTOCOL;
			pa->szModuleName = mir_strdup( szModuleName );
			pa->szProtoName = mir_strdup( dbv.pszVal );
			pa->tszAccountName = mir_a2t( szModuleName );
			pa->bIsVisible = TRUE;
			pa->bIsEnabled = FALSE;
			pa->iOrder = accounts.getCount();
			accounts.insert( pa );
		}
		DBFreeVariant( &dbv );
	}
	return 0;
}

void LoadDbAccounts(void)
{
	DBVARIANT dbv;
	int ver = DBGetContactSettingDword( NULL, "Protocols", "PrVer", -1 );
	int count = DBGetContactSettingDword( NULL, "Protocols", "ProtoCount", 0 ), i;

	for ( i=0; i < count; i++ ) {
		char buf[10];
		_itoa( i, buf, 10 );
		if ( DBGetContactSettingString( NULL, "Protocols", buf, &dbv ))
			continue;

		PROTOACCOUNT* pa = ( PROTOACCOUNT* )mir_calloc( sizeof( PROTOACCOUNT ));
		if ( pa == NULL ) {
			DBFreeVariant( &dbv );
			continue;
		}
		pa->cbSize = sizeof( *pa );
		pa->type = PROTOTYPE_PROTOCOL;
		pa->szModuleName = mir_strdup( dbv.pszVal );
		DBFreeVariant( &dbv );

		_itoa( OFFSET_VISIBLE+i, buf, 10 );
		pa->bIsVisible = DBGetContactSettingDword( NULL, "Protocols", buf, 1 );

		_itoa( OFFSET_PROTOPOS+i, buf, 10 );
		pa->iOrder = DBGetContactSettingDword( NULL, "Protocols", buf, 1 );

		if ( ver >= 4 ) {
			DBFreeVariant( &dbv );
			_itoa( OFFSET_NAME+i, buf, 10 );
			if ( !DBGetContactSettingTString( NULL, "Protocols", buf, &dbv )) {
				pa->tszAccountName = mir_tstrdup( dbv.ptszVal );
				DBFreeVariant( &dbv );
			}

			_itoa( OFFSET_ENABLED+i, buf, 10 );
			pa->bIsEnabled = DBGetContactSettingDword( NULL, "Protocols", buf, 1 );

			if ( !DBGetContactSettingString( NULL, pa->szModuleName, "AM_BaseProto", &dbv )) {
				pa->szProtoName = mir_strdup( dbv.pszVal );
				DBFreeVariant( &dbv );
			}
		}
		else pa->bIsEnabled = TRUE;

		if ( !pa->szProtoName ) {
			pa->szProtoName = mir_strdup( pa->szModuleName );
			DBWriteContactSettingString( NULL, pa->szModuleName, "AM_BaseProto", pa->szProtoName );
		}

		if ( !pa->tszAccountName )
			pa->tszAccountName = mir_a2t( pa->szModuleName );

		accounts.insert( pa );
	}

	if (CheckProtocolOrder()) WriteDbAccounts();

	int anum = accounts.getCount();
	CallService(MS_DB_MODULES_ENUM, 0, (LPARAM)EnumDbModules);
	if (anum != accounts.getCount()) WriteDbAccounts();
}

/////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int  arrlen;
	char **pszSettingName;
}
	enumDB_ProtoProcParam;

static int enumDB_ProtoProc( const char* szSetting, LPARAM lParam )
{
	if ( szSetting ) {
		enumDB_ProtoProcParam* p = ( enumDB_ProtoProcParam* )lParam;

		p->arrlen++;
		p->pszSettingName = ( char** )mir_realloc( p->pszSettingName, p->arrlen*sizeof( char* ));
		p->pszSettingName[ p->arrlen-1 ] = mir_strdup( szSetting );
	}
	return 0;
}

void WriteDbAccounts()
{
	int i;

	// enum all old settings to delete
	enumDB_ProtoProcParam param = { 0, NULL };

	DBCONTACTENUMSETTINGS dbces;
	dbces.pfnEnumProc = enumDB_ProtoProc;
	dbces.szModule = "Protocols";
	dbces.ofsSettings = 0;
	dbces.lParam = ( LPARAM )&param;
	CallService( MS_DB_CONTACT_ENUMSETTINGS, 0, ( LPARAM )&dbces );

	// delete all settings
	if ( param.arrlen ) {
		int i;
		for ( i=0; i < param.arrlen; i++ ) {
			DBDeleteContactSetting( 0, "Protocols", param.pszSettingName[i] );
			mir_free( param.pszSettingName[i] );
		}
		mir_free( param.pszSettingName );
	}

	// write new data
	for ( i=0; i < accounts.getCount(); i++ ) {
		PROTOACCOUNT* pa = accounts[i];

		char buf[ 20 ];
		_itoa( i, buf, 10 );
		DBWriteContactSettingString( NULL, "Protocols", buf, pa->szModuleName );

		_itoa( OFFSET_PROTOPOS+i, buf, 10 );
		DBWriteContactSettingDword( NULL, "Protocols", buf, pa->iOrder );

		_itoa( OFFSET_VISIBLE+i, buf, 10 );
		DBWriteContactSettingDword( NULL, "Protocols", buf, pa->bIsVisible );

		_itoa( OFFSET_ENABLED+i, buf, 10 );
		DBWriteContactSettingDword( NULL, "Protocols", buf, pa->bIsEnabled );

		_itoa( OFFSET_NAME+i, buf, 10 );
		DBWriteContactSettingTString( NULL, "Protocols", buf, pa->tszAccountName );
	}

	DBDeleteContactSetting( 0, "Protocols", "ProtoCount" );
	DBWriteContactSettingDword( 0, "Protocols", "ProtoCount", accounts.getCount() );
	DBWriteContactSettingDword( 0, "Protocols", "PrVer", 4 );
}

/////////////////////////////////////////////////////////////////////////////////////////

static int InitializeStaticAccounts( WPARAM, LPARAM )
{
	int count = 0;

	for ( int i = 0; i < accounts.getCount(); i++ ) {
		PROTOACCOUNT* pa = accounts[i];
		if ( !pa->ppro || !Proto_IsAccountEnabled( pa ))
			continue;

		pa->ppro->OnEvent( EV_PROTO_ONLOAD, 0, 0 );

		if ( !pa->bOldProto )
			count++;
	}

	BuildProtoMenus();

	if ( count == 0 && !DBGetContactSettingByte( NULL, "FirstRun", "AccManager", 0 )) {
		DBWriteContactSettingByte( NULL, "FirstRun", "AccManager", 1 );
		CallService( MS_PROTO_SHOWACCMGR, 0, 0 );
	}
	return 0;
}

static int UninitializeStaticAccounts( WPARAM, LPARAM )
{
	for ( int i = 0; i < accounts.getCount(); i++ ) {
		PROTOACCOUNT* pa = accounts[i];
		if ( !pa->ppro || !Proto_IsAccountEnabled( pa ))
			continue;

		pa->ppro->OnEvent( EV_PROTO_ONREADYTOEXIT, 0, 0 );
		pa->ppro->OnEvent( EV_PROTO_ONEXIT, 0, 0 );
	}
	return 0;
}

int LoadAccountsModule( void )
{
	int i;

	bModuleInitialized = TRUE;

	for ( i = 0; i < accounts.getCount(); i++ ) {
		PROTOACCOUNT* pa = accounts[i];
		pa->bDynDisabled = !Proto_IsProtocolLoaded( pa->szProtoName );
		if ( pa->ppro )
			continue;

		if (!Proto_IsAccountEnabled( pa )) {
			pa->type = PROTOTYPE_DISPROTO;
			continue;
		}

		if ( !ActivateAccount( pa )) {
			pa->bDynDisabled = TRUE;
			pa->type = PROTOTYPE_DISPROTO;
	}	}

	HookEvent( ME_SYSTEM_MODULESLOADED, InitializeStaticAccounts );
	HookEvent( ME_SYSTEM_PRESHUTDOWN, UninitializeStaticAccounts );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

static INT_PTR stub1( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->AddToList( wParam, (PROTOSEARCHRESULT*)lParam );
}

static INT_PTR stub2( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->AddToListByEvent( HIWORD(wParam), LOWORD(wParam), (HANDLE)lParam );
}

static INT_PTR stub3( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM )
{	return ( INT_PTR )ppi->Authorize(( HANDLE )wParam );
}

static INT_PTR stub4( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->AuthDeny(( HANDLE )wParam, StrConvT(( const char* )lParam ));
}

static INT_PTR stub7( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->ChangeInfo( wParam, ( void* )lParam );
}

static INT_PTR stub11( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	PROTOFILERESUME* pfr = ( PROTOFILERESUME* )lParam;
	return ( INT_PTR )ppi->FileResume(( HANDLE )wParam, &pfr->action, (const PROTOCHAR**)&pfr->szFilename );
}

static INT_PTR stub12( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->GetCaps( wParam, (HANDLE)lParam );
}

static INT_PTR stub13( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM )
{	return ( INT_PTR )ppi->GetIcon( wParam );
}

static INT_PTR stub15( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	return ( INT_PTR )ppi->SearchBasic( StrConvT(( char* )lParam ));
}

static INT_PTR stub16( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	return ( INT_PTR )ppi->SearchByEmail( StrConvT(( char* )lParam ));
}

static INT_PTR stub17( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	PROTOSEARCHBYNAME* psbn = ( PROTOSEARCHBYNAME* )lParam;
	return ( INT_PTR )ppi->SearchByName( StrConvT(( char* )psbn->pszNick ), 
		StrConvT(( char* )psbn->pszFirstName ), StrConvT(( char* )psbn->pszLastName ));
}

static INT_PTR stub18( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	return ( INT_PTR )ppi->SearchAdvanced(( HWND )lParam );
}

static INT_PTR stub19( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	return ( INT_PTR )ppi->CreateExtendedSearchUI (( HWND )lParam );
}

static INT_PTR stub22( PROTO_INTERFACE* ppi, WPARAM, LPARAM lParam )
{	CCSDATA *ccs = ( CCSDATA* )lParam;
	ppi->RecvMsg( ccs->hContact, ( PROTORECVEVENT* )ccs->lParam );
	return 0;
}

static INT_PTR stub29( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM )
{	return ( INT_PTR )ppi->SetStatus( wParam );
}

static INT_PTR stub33( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ( INT_PTR )ppi->SetAwayMsg( wParam, StrConvT(( const char* )lParam ));
}

static INT_PTR stub41( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	lstrcpynA(( char* )lParam, ppi->m_szModuleName, wParam );
	return 0;
}

static INT_PTR stub42( PROTO_INTERFACE* ppi, WPARAM wParam, LPARAM lParam )
{	return ppi->m_iStatus;
}

static HANDLE CreateProtoServiceEx( const char* szModule, const char* szService, MIRANDASERVICEOBJ pFunc, void* param )
{
	char tmp[100];
	mir_snprintf( tmp, sizeof( tmp ), "%s%s", szModule, szService );
	return CreateServiceFunctionObj( tmp, pFunc, param );
}

BOOL ActivateAccount( PROTOACCOUNT* pa )
{
	PROTO_INTERFACE* ppi;
	PROTOCOLDESCRIPTOR* ppd = Proto_IsProtocolLoaded( pa->szProtoName );
	if ( ppd == NULL )
		return FALSE;

	if ( ppd->fnInit == NULL )
		return FALSE;

	ppi = ppd->fnInit( pa->szModuleName, pa->tszAccountName );
	if ( ppi == NULL )
		return FALSE;

	pa->type = PROTOTYPE_PROTOCOL;
	pa->ppro = ppi;
	ppi->m_iDesiredStatus = ppi->m_iStatus = ID_STATUS_OFFLINE;
	CreateProtoServiceEx( pa->szModuleName, PS_ADDTOLIST, (MIRANDASERVICEOBJ)stub1, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_ADDTOLISTBYEVENT, (MIRANDASERVICEOBJ)stub2, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_AUTHALLOW, (MIRANDASERVICEOBJ)stub3, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_AUTHDENY, (MIRANDASERVICEOBJ)stub4, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_CHANGEINFO, (MIRANDASERVICEOBJ)stub7, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_FILERESUME, (MIRANDASERVICEOBJ)stub11, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_GETCAPS, (MIRANDASERVICEOBJ)stub12, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_LOADICON, (MIRANDASERVICEOBJ)stub13, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_BASICSEARCH, (MIRANDASERVICEOBJ)stub15, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_SEARCHBYEMAIL, (MIRANDASERVICEOBJ)stub16, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_SEARCHBYNAME, (MIRANDASERVICEOBJ)stub17, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_SEARCHBYADVANCED, (MIRANDASERVICEOBJ)stub18, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_CREATEADVSEARCHUI, (MIRANDASERVICEOBJ)stub19, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PSR_MESSAGE, (MIRANDASERVICEOBJ)stub22, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_SETSTATUS, (MIRANDASERVICEOBJ)stub29, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_SETAWAYMSG, (MIRANDASERVICEOBJ)stub33, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_GETNAME, (MIRANDASERVICEOBJ)stub41, pa->ppro );
	CreateProtoServiceEx( pa->szModuleName, PS_GETSTATUS, (MIRANDASERVICEOBJ)stub42, pa->ppro );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////

struct DeactivationThreadParam
{
	tagPROTO_INTERFACE* ppro;
	pfnUninitProto      fnUninit;
	bool                bIsDynamic;
	bool                bErase;
};

pfnUninitProto GetProtocolDestructor( char* szProto );

static int DeactivationThread( DeactivationThreadParam* param )
{
	tagPROTO_INTERFACE* p = ( tagPROTO_INTERFACE* )param->ppro;
	p->SetStatus(ID_STATUS_OFFLINE);

	char * szModuleName = NEWSTR_ALLOCA(p->m_szModuleName);

	if ( param->bIsDynamic ) {
		p->OnEvent( EV_PROTO_ONREADYTOEXIT, 0, 0 );
		p->OnEvent( EV_PROTO_ONEXIT, 0, 0 );
	}

	KillObjectThreads( p ); // waits for them before terminating
	KillObjectEventHooks( p ); // untie an object from the outside world

	if ( param->bErase )
		p->OnEvent( EV_PROTO_ONERASE, 0, 0 );

	if ( param->fnUninit )
		param->fnUninit( p );

	KillObjectServices( p );

	if ( param->bErase )
		EraseAccount( szModuleName );

	delete param;
	return 0;
}

void DeactivateAccount( PROTOACCOUNT* pa, bool bIsDynamic, bool bErase )
{
	if ( pa->ppro == NULL ) {
		if ( bErase )
			EraseAccount( pa->szModuleName );
		return;
	}

	if ( pa->hwndAccMgrUI ) {
		DestroyWindow(pa->hwndAccMgrUI);
		pa->hwndAccMgrUI = NULL;
		pa->bAccMgrUIChanged = FALSE;
	}

	DeactivationThreadParam* param = new DeactivationThreadParam;
	param->ppro = pa->ppro;
	param->fnUninit = GetProtocolDestructor( pa->szProtoName );
	param->bIsDynamic = bIsDynamic;
	param->bErase = bErase;
	pa->ppro = NULL;
	pa->type = PROTOTYPE_DISPROTO;
	if ( bIsDynamic )
		mir_forkthread(( pThreadFunc )DeactivationThread, param );
	else 
		DeactivationThread( param );
}

/////////////////////////////////////////////////////////////////////////////////////////

void EraseAccount( const char* pszModuleName )
{
	DBVARIANT dbv;
	DBCONTACTGETSETTING dbcgs;
	char szProtoName[32];

	dbcgs.pValue = &dbv;
	dbcgs.szModule = "Protocol";
	dbcgs.szSetting = "p";

	// remove protocol contacts first
	HANDLE hContact = ( HANDLE )CallService( MS_DB_CONTACT_FINDFIRST, 0, 0 );
	while ( hContact != NULL ) {
		HANDLE h1 = hContact;
		hContact = ( HANDLE )CallService( MS_DB_CONTACT_FINDNEXT, ( WPARAM )h1, 0 );

		dbv.type = DBVT_ASCIIZ;
		dbv.pszVal = szProtoName;
		dbv.cchVal = SIZEOF(szProtoName);

		if ( CallService( MS_DB_CONTACT_GETSETTINGSTATIC, ( WPARAM )h1, ( LPARAM )&dbcgs ))
			continue;

		if ( !lstrcmpA( szProtoName, pszModuleName ))
			CallService( MS_DB_CONTACT_DELETE, ( WPARAM )h1, 0 );
	}

	// remove all protocol settings
	CallService( MS_DB_MODULE_DELETE, 0, ( LPARAM )pszModuleName );
}


/////////////////////////////////////////////////////////////////////////////////////////

void UnloadAccount( PROTOACCOUNT* pa, bool bIsDynamic, bool bErase )
{
	DeactivateAccount( pa, bIsDynamic, bErase );

	mir_free( pa->tszAccountName );
	mir_free( pa->szProtoName );
	// szModuleName should be freed only on a program's exit.
	// otherwise many plugins dependand on static protocol names will crash!
	// do NOT fix this 'leak', please
	if ( !bIsDynamic ) {
		mir_free( pa->szModuleName );
		mir_free( pa );
	}
}

void UnloadAccountsModule()
{
	int i;

	if ( !bModuleInitialized ) return;

	for( i=accounts.getCount()-1; i >= 0; i-- ) {
		PROTOACCOUNT* pa = accounts[ i ];
		UnloadAccount( pa, false, false );
		accounts.remove(i);
	}

	accounts.destroy();
}

/////////////////////////////////////////////////////////////////////////////////////////

void BuildProtoMenus()
{
	for ( int i = 0; i < accounts.getCount(); i++ ) {
		PROTOACCOUNT* pa = accounts[ i ];
		if ( cli.pfnGetProtocolVisibility( pa->szModuleName ) == 0 )
			continue;

		if ( pa->ppro )
			pa->ppro->OnEvent( EV_PROTO_ONMENU, 0, 0 );
	}
}

void RebuildProtoMenus( int iNewValue )
{
	DBWriteContactSettingByte( NULL, "CList", "MoveProtoMenus", iNewValue );

	RebuildMenuOrder();
	BuildProtoMenus();
}
