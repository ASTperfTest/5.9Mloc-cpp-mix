/*

Jabber Protocol Plugin for Miranda IM
Copyright ( C ) 2002-04  Santithorn Bunchua
Copyright ( C ) 2005-09  George Hazan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or ( at your option ) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

File name      : $URL: https://miranda.googlecode.com/svn/branches/stable/miranda/protocols/JabberG/jabber_util.cpp $
Revision       : $Revision: 13026 $
Last change on : $Date: 2010-10-24 10:09:28 -0400 (Sun, 24 Oct 2010) $
Last change by : $Author: borkra $

*/

#include "jabber.h"
#include <richedit.h>

#include "jabber_list.h"
#include "jabber_caps.h"

#include "m_clistint.h"

extern CRITICAL_SECTION mutex;

extern int bSecureIM;

void CJabberProto::SerialInit( void )
{
	InitializeCriticalSection( &m_csSerial );
	m_nSerial = 0;
}

void CJabberProto::SerialUninit( void )
{
	DeleteCriticalSection( &m_csSerial );
}

int CJabberProto::SerialNext( void )
{
	unsigned int ret;

	EnterCriticalSection( &m_csSerial );
	ret = m_nSerial;
	m_nSerial++;
	LeaveCriticalSection( &m_csSerial );
	return ret;
}

void CJabberProto::Log( const char* fmt, ... )
{
	va_list vararg;
	va_start( vararg, fmt );
	char* str = ( char* )alloca( 32000 );
	mir_vsnprintf( str, 32000, fmt, vararg );
	va_end( vararg );

	JCallService( MS_NETLIB_LOG, ( WPARAM )m_hNetlibUser, ( LPARAM )str );
}

///////////////////////////////////////////////////////////////////////////////
// JabberChatRoomHContactFromJID - looks for the char room HCONTACT with required JID

HANDLE CJabberProto::ChatRoomHContactFromJID( const TCHAR* jid )
{
	if ( jid == NULL )
		return ( HANDLE )NULL;
	
	HANDLE hContactMatched = NULL;
	HANDLE hContact = ( HANDLE ) JCallService( MS_DB_CONTACT_FINDFIRST, 0, 0 );
	while ( hContact != NULL ) {
		char* szProto = ( char* )JCallService( MS_PROTO_GETCONTACTBASEPROTO, ( WPARAM ) hContact, 0 );
		if ( szProto != NULL && !strcmp( m_szModuleName, szProto )) {
			DBVARIANT dbv;
			int result = JGetStringT( hContact, "ChatRoomID", &dbv );
			if ( result )
				result = JGetStringT( hContact, "jid", &dbv );	

			if ( !result ) {
				int result;
				result = lstrcmpi( jid, dbv.ptszVal );
				JFreeVariant( &dbv );
				if ( !result && JGetByte( hContact, "ChatRoom", 0 ) != 0 ) {
					hContactMatched = hContact;
					break;
		}	}	}

		hContact = ( HANDLE ) JCallService( MS_DB_CONTACT_FINDNEXT, ( WPARAM ) hContact, 0 );
	}

	return hContactMatched;
}

///////////////////////////////////////////////////////////////////////////////
// JabberHContactFromJID - looks for the HCONTACT with required JID

HANDLE CJabberProto::HContactFromJID( const TCHAR* jid , BOOL bStripResource )
{
	if ( jid == NULL )
		return ( HANDLE )NULL;

	JABBER_LIST_ITEM* item = ListGetItemPtr( LIST_CHATROOM, jid );

	HANDLE hContactMatched = NULL;
	HANDLE hContact = ( HANDLE ) JCallService( MS_DB_CONTACT_FINDFIRST, 0, 0 );
	while ( hContact != NULL ) {
		char* szProto = ( char* )JCallService( MS_PROTO_GETCONTACTBASEPROTO, ( WPARAM ) hContact, 0 );
		if ( szProto != NULL && !strcmp( m_szModuleName, szProto )) {
			DBVARIANT dbv;
			int result = JGetStringT( hContact, "jid", &dbv );
			if ( result )
				result = JGetStringT( hContact, "ChatRoomID", &dbv );

			if ( !result ) {
				int result;
				if ( item != NULL )
					result = lstrcmpi( jid, dbv.ptszVal );
				else {
					if ( bStripResource == 3 ) {
						if (JGetByte(hContact, "ChatRoom", 0))
							result = lstrcmpi( jid, dbv.ptszVal );  // for chat room we have to have full contact matched
						else if ( TRUE )
							result = _tcsnicmp( jid, dbv.ptszVal, _tcslen(dbv.ptszVal));
						else
							result = JabberCompareJids( jid, dbv.ptszVal );
					}
					// most probably it should just look full matching contact
					else
						result = lstrcmpi( jid, dbv.ptszVal );

				}
				JFreeVariant( &dbv );
				if ( !result ) {
					hContactMatched = hContact;
					break;
		}	}	}

		hContact = ( HANDLE ) JCallService( MS_DB_CONTACT_FINDNEXT, ( WPARAM ) hContact, 0 );
	}

	return hContactMatched;
}

TCHAR* __stdcall JabberNickFromJID( const TCHAR* jid )
{
	if (!jid) return mir_tstrdup(_T(""));

	const TCHAR* p;
	TCHAR* nick;

	if (( p = _tcschr( jid, '@' )) == NULL )
		p = _tcschr( jid, '/' );

	if ( p != NULL ) {
		if (( nick=( TCHAR* )mir_alloc( sizeof(TCHAR)*( int( p-jid )+1 ))) != NULL ) {
			_tcsncpy( nick, jid, p-jid );
			nick[p-jid] = '\0';
		}
	}
	else nick = mir_tstrdup( jid );
	return nick;
}

JABBER_RESOURCE_STATUS* CJabberProto::ResourceInfoFromJID( const TCHAR* jid )
{
	if ( !jid )
		return NULL;

	JABBER_LIST_ITEM *item = NULL;
	if (( item = ListGetItemPtr( LIST_VCARD_TEMP, jid )) == NULL)
		item = ListGetItemPtr( LIST_ROSTER, jid );
	if ( item == NULL ) return NULL;

	const TCHAR* p = _tcschr( jid, '/' );
	if ( p == NULL )
		return &item->itemResource;
	if ( *++p == '\0' ) return NULL;

	JABBER_RESOURCE_STATUS *r = item->resource;
	if ( r == NULL ) return NULL;

	int i;
	for ( i=0; i<item->resourceCount && _tcscmp( r->resourceName, p ); i++, r++ );
	if ( i >= item->resourceCount )
		return NULL;

	return r;
}

TCHAR* JabberPrepareJid( LPCTSTR jid )
{
	if ( !jid ) return NULL;
	TCHAR* szNewJid = mir_tstrdup( jid );
	if ( !szNewJid ) return NULL;
	TCHAR* pDelimiter = _tcschr( szNewJid, _T('/') );
	if ( pDelimiter ) *pDelimiter = _T('\0');
	CharLower( szNewJid );
	if ( pDelimiter ) *pDelimiter = _T('/');
	return szNewJid;
}

void strdel( char* parBuffer, int len )
{
	char* p;
	for ( p = parBuffer+len; *p != 0; p++ )
		p[ -len ] = *p;

	p[ -len ] = '\0';
}

char* __stdcall JabberUrlDecode( char* str )
{
	char* p, *q;

	if ( str == NULL )
		return NULL;

	for ( p=q=str; *p!='\0'; p++,q++ ) {
		if ( *p == '<' ) {
			// skip CDATA
			if ( !strncmp( p, "<![CDATA[", 9 ))
			{
				p += 9;
				char *tail = strstr(p, "]]>");
				size_t count = tail ? (tail-p) : strlen(p);
				memmove(q, p, count);
				q += count-1;
				p = (tail ? (tail+3) : (p+count)) - 1;
			} else
			{
				*q = *p;
			}
		} else
		if ( *p == '&' ) {
			if ( !strncmp( p, "&amp;", 5 )) {	*q = '&'; p += 4; }
			else if ( !strncmp( p, "&apos;", 6 )) { *q = '\''; p += 5; }
			else if ( !strncmp( p, "&gt;", 4 )) { *q = '>'; p += 3; }
			else if ( !strncmp( p, "&lt;", 4 )) { *q = '<'; p += 3; }
			else if ( !strncmp( p, "&quot;", 6 )) { *q = '"'; p += 5; }
			else { *q = *p;	}
		} else
		{
			*q = *p;
		}
	}
	*q = '\0';
	return str;
}

void __stdcall JabberUrlDecodeW( WCHAR* str )
{
	if ( str == NULL )
		return;

	WCHAR* p, *q;
	for ( p=q=str; *p!='\0'; p++,q++ ) {
		if ( *p == '&' ) {
			if ( !wcsncmp( p, L"&amp;", 5 )) {	*q = '&'; p += 4; }
			else if ( !wcsncmp( p, L"&apos;", 6 )) { *q = '\''; p += 5; }
			else if ( !wcsncmp( p, L"&gt;", 4 )) { *q = '>'; p += 3; }
			else if ( !wcsncmp( p, L"&lt;", 4 )) { *q = '<'; p += 3; }
			else if ( !wcsncmp( p, L"&quot;", 6 )) { *q = '"'; p += 5; }
			else { *q = *p;	}
		}
		else {
			*q = *p;
		}
	}
	*q = '\0';
}

char* __stdcall JabberUrlEncode( const char* str )
{
	char* s, *p, *q;
	int c;

	if ( str == NULL )
		return NULL;

	for ( c=0,p=( char* )str; *p!='\0'; p++ ) {
		switch ( *p ) {
			case '&': c += 5; break;
			case '\'': c += 6; break;
			case '>': c += 4; break;
			case '<': c += 4; break;
			case '"': c += 6; break;
			default: c++; break;
		}
	}
	if (( s=( char* )mir_alloc( c+1 )) != NULL ) {
		for ( p=( char* )str,q=s; *p!='\0'; p++ ) {
			switch ( *p ) {
			case '&': strcpy( q, "&amp;" ); q += 5; break;
			case '\'': strcpy( q, "&apos;" ); q += 6; break;
			case '>': strcpy( q, "&gt;" ); q += 4; break;
			case '<': strcpy( q, "&lt;" ); q += 4; break;
			case '"': strcpy( q, "&quot;" ); q += 6; break;
			default:
				if ( *p > 0 && *p < 32 ) {
					switch( *p ) {
					case '\r':
					case '\n':
					case '\t':
						*q = *p;
						break;
					default:
						*q = '?';
					}
				}
				else *q = *p; 
				q++; 
				break;
			}
		}
		*q = '\0';
	}

	return s;
}

void __stdcall JabberUtfToTchar( const char* pszValue, size_t cbLen, LPTSTR& dest )
{
	char* pszCopy = NULL;
	bool  bNeedsFree = false;
	__try
	{
		// this code can cause access violation when a stack overflow occurs
		pszCopy = ( char* )alloca( cbLen+1 );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		bNeedsFree = true;
		pszCopy = ( char* )malloc( cbLen+1 );
	}
	if ( pszCopy == NULL )
		return;

	memcpy( pszCopy, pszValue, cbLen );
	pszCopy[ cbLen ] = 0;

	JabberUrlDecode( pszCopy );

	#if defined( _UNICODE )
		mir_utf8decode( pszCopy, &dest );
	#else
		mir_utf8decode( pszCopy, NULL );
		dest = mir_strdup( pszCopy );
	#endif

	if ( bNeedsFree )
		free( pszCopy );
}

char* __stdcall JabberSha1( char* str )
{
	mir_sha1_ctx sha;
	mir_sha1_byte_t digest[20];
	char* result;
	int i;

	if ( str == NULL )
		return NULL;

	mir_sha1_init( &sha );
	mir_sha1_append( &sha, (mir_sha1_byte_t* )str, (int)strlen( str ));
	mir_sha1_finish( &sha, digest );
	if (( result=( char* )mir_alloc( 41 )) == NULL )
		return NULL;

	for ( i=0; i<20; i++ )
		sprintf( result+( i<<1 ), "%02x", digest[i] );
	return result;
}

TCHAR* __stdcall JabberStrFixLines( const TCHAR* str )
{
	if (!str) return NULL;

	const TCHAR *p;
	int add = 0;
	bool prev_r = false;
	bool prev_n = false;

	for (p = str; p && *p; ++p)
		if (*p == _T('\r') || *p == _T('\n'))
			++add;

	TCHAR *buf = (TCHAR *)mir_alloc((lstrlen(str) + add + 1) * sizeof(TCHAR));
	TCHAR *res = buf;

	for (p = str; p && *p; ++p)
	{
		if (*p == _T('\n') && !prev_r)
			*res++ = _T('\r');
		if (*p != _T('\r') && *p != _T('\n') && prev_r)
			*res++ = _T('\n');
		*res++ = *p;
		prev_r = *p == _T('\r');
		prev_n = *p == _T('\n');
	}
	*res = 0;

	return buf;
}

char* __stdcall JabberUnixToDos( const char* str )
{
	char* p, *q, *res;
	int extra;

	if ( str==NULL || str[0]=='\0' )
		return NULL;

	extra = 0;
	for ( p=( char* )str; *p!='\0'; p++ ) {
		if ( *p == '\n' )
			extra++;
	}
	if (( res=( char* )mir_alloc( strlen( str )+extra+1 )) != NULL ) {
		for ( p=( char* )str,q=res; *p!='\0'; p++,q++ ) {
			if ( *p == '\n' ) {
				*q = '\r';
				q++;
			}
			*q = *p;
		}
		*q = '\0';
	}
	return res;
}

WCHAR* __stdcall JabberUnixToDosW( const WCHAR* str )
{
	if ( str==NULL || str[0]=='\0' )
		return NULL;

	const WCHAR* p;
	WCHAR* q, *res;
	int extra = 0;

	for ( p = str; *p!='\0'; p++ ) {
		if ( *p == '\n' )
			extra++;
	}
	if (( res = ( WCHAR* )mir_alloc( sizeof( WCHAR )*( wcslen( str ) + extra + 1 )) ) != NULL ) {
		for ( p = str,q=res; *p!='\0'; p++,q++ ) {
			if ( *p == '\n' ) {
				*q = '\r';
				q++;
			}
			*q = *p;
		}
		*q = '\0';
	}
	return res;
}

TCHAR* __stdcall JabberHttpUrlEncode( const TCHAR* str )
{
	TCHAR* p, *q, *res;

	if ( str == NULL ) return NULL;
	res = ( TCHAR* ) mir_alloc( 3*_tcslen( str ) + 1 );
	for ( p = ( TCHAR* )str, q = res; *p!='\0'; p++,q++ ) {
		if (( *p>='A' && *p<='Z' ) || ( *p>='a' && *p<='z' ) || ( *p>='0' && *p<='9' ) || strchr( "$-_.+!*'(),", *p )!=NULL ) {
			*q = *p;
		}
		else {
			wsprintf( q, _T("%%%02X"), *p );
			q += 2;
		}
	}
	*q = '\0';
	return res;
}

void __stdcall JabberHttpUrlDecode( TCHAR* str )
{
	TCHAR* p, *q;
	unsigned int code;

	if ( str == NULL ) return;
	for ( p = q = ( TCHAR* )str; *p!='\0'; p++,q++ ) {
		if ( *p=='%' && *( p+1 )!='\0' && isxdigit( *( p+1 )) && *( p+2 )!='\0' && isxdigit( *( p+2 )) ) {
			_stscanf(( TCHAR* )p+1, _T("%2x"), &code );
			*q = ( unsigned char ) code;
			p += 2;
		}
		else {
			*q = *p;
	}	}

	*q = '\0';
}

int __stdcall JabberCombineStatus( int status1, int status2 )
{
	// Combine according to the following priority ( high to low )
	// ID_STATUS_FREECHAT
	// ID_STATUS_ONLINE
	// ID_STATUS_DND
	// ID_STATUS_AWAY
	// ID_STATUS_NA
	// ID_STATUS_INVISIBLE ( valid only for TLEN_PLUGIN )
	// ID_STATUS_OFFLINE
	// other ID_STATUS in random order ( actually return status1 )
	if ( status1==ID_STATUS_FREECHAT || status2==ID_STATUS_FREECHAT )
		return ID_STATUS_FREECHAT;
	if ( status1==ID_STATUS_ONLINE || status2==ID_STATUS_ONLINE )
		return ID_STATUS_ONLINE;
	if ( status1==ID_STATUS_DND || status2==ID_STATUS_DND )
		return ID_STATUS_DND;
	if ( status1==ID_STATUS_AWAY || status2==ID_STATUS_AWAY )
		return ID_STATUS_AWAY;
	if ( status1==ID_STATUS_NA || status2==ID_STATUS_NA )
		return ID_STATUS_NA;
	if ( status1==ID_STATUS_INVISIBLE || status2==ID_STATUS_INVISIBLE )
		return ID_STATUS_INVISIBLE;
	if ( status1==ID_STATUS_OFFLINE || status2==ID_STATUS_OFFLINE )
		return ID_STATUS_OFFLINE;
	return status1;
}

struct tagErrorCodeToStr {
	int code;
	TCHAR* str;
}
static JabberErrorCodeToStrMapping[] = {
	{ JABBER_ERROR_REDIRECT,               _T("Redirect") },
	{ JABBER_ERROR_BAD_REQUEST,            _T("Bad request") },
	{ JABBER_ERROR_UNAUTHORIZED,           _T("Unauthorized") },
	{ JABBER_ERROR_PAYMENT_REQUIRED,       _T("Payment required") },
	{ JABBER_ERROR_FORBIDDEN,              _T("Forbidden") },
	{ JABBER_ERROR_NOT_FOUND,              _T("Not found") },
	{ JABBER_ERROR_NOT_ALLOWED,            _T("Not allowed") },
	{ JABBER_ERROR_NOT_ACCEPTABLE,         _T("Not acceptable") },
	{ JABBER_ERROR_REGISTRATION_REQUIRED,  _T("Registration required") },
	{ JABBER_ERROR_REQUEST_TIMEOUT,        _T("Request timeout") },
	{ JABBER_ERROR_CONFLICT,               _T("Conflict") },
	{ JABBER_ERROR_INTERNAL_SERVER_ERROR,  _T("Internal server error") },
	{ JABBER_ERROR_NOT_IMPLEMENTED,        _T("Not implemented") },
	{ JABBER_ERROR_REMOTE_SERVER_ERROR,    _T("Remote server error") },
	{ JABBER_ERROR_SERVICE_UNAVAILABLE,    _T("Service unavailable") },
	{ JABBER_ERROR_REMOTE_SERVER_TIMEOUT,  _T("Remote server timeout") },
	{ -1,                                  _T("Unknown error") }
};

TCHAR* __stdcall JabberErrorStr( int errorCode )
{
	int i;

	for ( i=0; JabberErrorCodeToStrMapping[i].code!=-1 && JabberErrorCodeToStrMapping[i].code!=errorCode; i++ );
	return JabberErrorCodeToStrMapping[i].str;
}

TCHAR* __stdcall JabberErrorMsg( HXML errorNode, int* pErrorCode )
{
	TCHAR* errorStr = ( TCHAR* )mir_alloc( 256 * sizeof( TCHAR ));
	if ( errorNode == NULL ) {
		if ( pErrorCode )
			*pErrorCode = -1;
		mir_sntprintf( errorStr, 256, _T("%s -1: %s"), TranslateT( "Error" ), TranslateT( "Unknown error message" ));
		return errorStr;
	}

	int errorCode = -1;
	const TCHAR *str;
	if (( str = xmlGetAttrValue( errorNode, _T("code"))) != NULL )
		errorCode = _ttoi( str );
	if (( str=xmlGetText( errorNode ) ) != NULL )
		mir_sntprintf( errorStr, 256, _T("%s %d: %s\r\n%s"), TranslateT( "Error" ), errorCode, TranslateTS( JabberErrorStr( errorCode )), str );
	else
		mir_sntprintf( errorStr, 256, _T("%s %d: %s"), TranslateT( "Error" ), errorCode, TranslateTS( JabberErrorStr( errorCode )) );

	if ( pErrorCode )
		*pErrorCode = errorCode;
	return errorStr;
}

void CJabberProto::SendVisibleInvisiblePresence( BOOL invisible )
{
	if ( !m_bJabberOnline ) return;

	LISTFOREACH(i, this, LIST_ROSTER)
	{
		JABBER_LIST_ITEM *item = ListGetItemPtrFromIndex( i );
		if ( item == NULL )
			continue;

		HANDLE hContact = HContactFromJID( item->jid );
		if ( hContact == NULL )
			continue;

		WORD apparentMode = JGetWord( hContact, "ApparentMode", 0 );
		if ( invisible==TRUE && apparentMode==ID_STATUS_OFFLINE )
			m_ThreadInfo->send( XmlNode( _T("presence" )) << XATTR( _T("to"), item->jid ) << XATTR( _T("type"), _T("invisible")));
		else if ( invisible==FALSE && apparentMode==ID_STATUS_ONLINE )
			SendPresenceTo( m_iStatus, item->jid, NULL );
}	}

/////////////////////////////////////////////////////////////////////////////////////////
// JabberBase64Encode

static char b64table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* __stdcall JabberBase64Encode( const char* buffer, int bufferLen )
{
	if ( buffer==NULL || bufferLen<=0 )
		return NULL;

	char* res = (char*)mir_alloc(((( bufferLen+2 )*4 )/3 ) + 1);
	if ( res == NULL )
		return NULL;

	unsigned char igroup[3];
	char *r = res;
	const char* peob = buffer + bufferLen;
	for ( const char* p = buffer; p < peob; ) {
		igroup[ 0 ] = igroup[ 1 ] = igroup[ 2 ] = 0;
		int n;
		for ( n=0; n<3; n++ ) {
			if ( p >= peob ) break;
			igroup[n] = ( unsigned char ) *p;
			p++;
		}

		if ( n > 0 ) {
			r[0] = b64table[ igroup[0]>>2 ];
			r[1] = b64table[ (( igroup[0]&3 )<<4 ) | ( igroup[1]>>4 ) ];
			r[2] = b64table[ (( igroup[1]&0xf )<<2 ) | ( igroup[2]>>6 ) ];
			r[3] = b64table[ igroup[2]&0x3f ];

			if ( n < 3 ) {
				r[3] = '=';
				if ( n < 2 )
					r[2] = '=';
			}
			r += 4;
	}	}

	*r = '\0';

	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////
// JabberBase64Decode

static unsigned char b64rtable[256];

char* __stdcall JabberBase64Decode( const TCHAR* str, int *resultLen )
{
	char* res;
	unsigned char* r, igroup[4], a[4];
	int n, num, count;

	if ( str==NULL || resultLen==NULL ) return NULL;
	if (( res=( char* )mir_alloc(( ( _tcslen( str )+3 )/4 )*3 )) == NULL ) return NULL;

	for ( n=0; n<256; n++ )
		b64rtable[n] = ( unsigned char ) 0x80;
	for ( n=0; n<26; n++ )
		b64rtable['A'+n] = n;
	for ( n=0; n<26; n++ )
		b64rtable['a'+n] = n + 26;
	for ( n=0; n<10; n++ )
		b64rtable['0'+n] = n + 52;
	b64rtable['+'] = 62;
	b64rtable['/'] = 63;
	b64rtable['='] = 0;
	count = 0;
	for ( r=( unsigned char* )res; *str != '\0'; ) {
		for ( n=0; n<4; n++ ) {
			if ( BYTE(*str) == '\r' || BYTE(*str) == '\n' ) {
				n--; str++;
				continue;
			}

			if ( BYTE(*str)=='\0' ) {
				if ( n == 0 )
					goto LBL_Exit;
				mir_free( res );
				return NULL;
			}

			if ( b64rtable[BYTE(*str)]==0x80 ) {
				mir_free( res );
				return NULL;
			}

			a[n] = BYTE(*str);
			igroup[n] = b64rtable[BYTE(*str)];
			str++;
		}
		r[0] = igroup[0]<<2 | igroup[1]>>4;
		r[1] = igroup[1]<<4 | igroup[2]>>2;
		r[2] = igroup[2]<<6 | igroup[3];
		r += 3;
		num = ( a[2]=='='?1:( a[3]=='='?2:3 ));
		count += num;
		if ( num < 3 ) break;
	}
LBL_Exit:
	*resultLen = count;
	return res;
}

time_t __stdcall JabberIsoToUnixTime( const TCHAR* stamp )
{
	struct tm timestamp;
	TCHAR date[9];
	int i, y;
	time_t t;

	if ( stamp == NULL ) return ( time_t ) 0;

	const TCHAR *p = stamp;

	// Get the date part
	for ( i=0; *p!='\0' && i<8 && isdigit( *p ); p++,i++ )
		date[i] = *p;

	// Parse year
	if ( i == 6 ) {
		// 2-digit year ( 1970-2069 )
		y = ( date[0]-'0' )*10 + ( date[1]-'0' );
		if ( y < 70 ) y += 100;
	}
	else if ( i == 8 ) {
		// 4-digit year
		y = ( date[0]-'0' )*1000 + ( date[1]-'0' )*100 + ( date[2]-'0' )*10 + date[3]-'0';
		y -= 1900;
	}
	else
		return ( time_t ) 0;
	timestamp.tm_year = y;
	// Parse month
	timestamp.tm_mon = ( date[i-4]-'0' )*10 + date[i-3]-'0' - 1;
	// Parse date
	timestamp.tm_mday = ( date[i-2]-'0' )*10 + date[i-1]-'0';

	// Skip any date/time delimiter
	for ( ; *p!='\0' && !isdigit( *p ); p++ );

	// Parse time
	if ( _stscanf( p, _T("%d:%d:%d"), &timestamp.tm_hour, &timestamp.tm_min, &timestamp.tm_sec ) != 3 )
		return ( time_t ) 0;

	timestamp.tm_isdst = 0;	// DST is already present in _timezone below
	t = mktime( &timestamp );

	_tzset();
	t -= _timezone;

	if ( t >= 0 )
		return t;
	else
		return ( time_t ) 0;
}

void CJabberProto::SendPresenceTo( int status, TCHAR* to, HXML extra, TCHAR *msg )
{
	if ( !m_bJabberOnline ) return;

	// Send <presence/> update for status ( we won't handle ID_STATUS_OFFLINE here )
	short iPriority = (short)JGetWord( NULL, "Priority", 0 );
	UpdatePriorityMenu(iPriority);

	TCHAR szPriority[40];
	_itot( iPriority, szPriority, 10 );

	XmlNode p( _T("presence")); p << XCHILD( _T("priority"), szPriority );
	if ( to != NULL )
		p << XATTR( _T("to"), to );

	if ( extra )
		xmlAddChild( p, extra );

	// XEP-0115:Entity Capabilities
	HXML c = p << XCHILDNS( _T("c"), _T(JABBER_FEAT_ENTITY_CAPS)) << XATTR( _T("node"), _T(JABBER_CAPS_MIRANDA_NODE)) 
		<< XATTR( _T("ver"), _T(__VERSION_STRING));

	TCHAR szExtCaps[ 512 ];
	szExtCaps[ 0 ] = _T('\0');

	if ( bSecureIM )
		_tcscat( szExtCaps, _T(JABBER_EXT_SECUREIM) );

	if ( m_options.EnableRemoteControl ) {
		if ( szExtCaps[0] )
			_tcscat( szExtCaps, _T(" "));
		_tcscat( szExtCaps, _T(JABBER_EXT_COMMANDS) );
	}

	if ( m_options.EnableUserMood ) {
		if ( szExtCaps[0] )
			_tcscat( szExtCaps, _T(" "));
		_tcscat( szExtCaps, _T(JABBER_EXT_USER_MOOD) );
	}

	if ( m_options.EnableUserTune ) {
		if ( szExtCaps[0] )
			_tcscat( szExtCaps, _T(" "));
		_tcscat( szExtCaps, _T(JABBER_EXT_USER_TUNE) );
	}

	if ( m_options.EnableUserActivity ) {
		if ( szExtCaps[0] )
			_tcscat( szExtCaps, _T(" "));
		_tcscat( szExtCaps, _T(JABBER_EXT_USER_ACTIVITY) );
	}

	if ( m_options.AcceptNotes ) {
		if ( szExtCaps[0] )
			_tcscat( szExtCaps, _T(" "));
		_tcscat( szExtCaps, _T(JABBER_EXT_MIR_NOTES) );
	}

	// add features enabled through IJabberNetInterface::AddFeatures()
	for ( int i = 0; i < m_lstJabberFeatCapPairsDynamic.getCount(); i++ ) {
		if ( m_uEnabledFeatCapsDynamic & m_lstJabberFeatCapPairsDynamic[i]->jcbCap ) {
			if ( szExtCaps[0] )
				_tcscat( szExtCaps, _T(" "));
			_tcscat( szExtCaps, m_lstJabberFeatCapPairsDynamic[i]->szExt );
		}
	}

	if ( szExtCaps[0] )
		xmlAddAttr( c, _T("ext"), szExtCaps );

	if ( m_options.EnableAvatars ) {
		char hashValue[ 50 ];
		if ( !JGetStaticString( "AvatarHash", NULL, hashValue, sizeof( hashValue ))) {
			// XEP-0153: vCard-Based Avatars
			HXML x = p << XCHILDNS( _T("x"), _T("vcard-temp:x:update"));
			x << XCHILD( _T("photo"), _A2T(hashValue));
		} else {
			HXML x = p << XCHILDNS( _T("x"), _T("vcard-temp:x:update"));
			x << XCHILD( _T("photo"));
		}
	}

	EnterCriticalSection( &m_csModeMsgMutex );
	switch ( status ) {
	case ID_STATUS_ONLINE:
		if ( msg || m_modeMsgs.szOnline )
			p << XCHILD( _T("status"), msg ? msg : m_modeMsgs.szOnline );
		break;
	case ID_STATUS_INVISIBLE:
		p << XATTR( _T("type"), _T("invisible"));
		break;
	case ID_STATUS_AWAY:
	case ID_STATUS_ONTHEPHONE:
	case ID_STATUS_OUTTOLUNCH:
		p << XCHILD( _T("show"), _T("away"));
		if ( msg || m_modeMsgs.szAway )
			p << XCHILD( _T("status"), msg ? msg : m_modeMsgs.szAway );
		break;
	case ID_STATUS_NA:
		p << XCHILD( _T("show"), _T("xa"));
		if ( msg || m_modeMsgs.szNa )
			p << XCHILD( _T("status"), msg ? msg : m_modeMsgs.szNa );
		break;
	case ID_STATUS_DND:
	case ID_STATUS_OCCUPIED:
		p << XCHILD( _T("show"), _T("dnd"));
		if ( msg || m_modeMsgs.szDnd )
			p << XCHILD( _T("status"), msg ? msg : m_modeMsgs.szDnd );
		break;
	case ID_STATUS_FREECHAT:
		p << XCHILD( _T("show"), _T("chat"));
		if ( msg || m_modeMsgs.szFreechat )
			p << XCHILD( _T("status"), msg ? msg : m_modeMsgs.szFreechat );
		break;
	default:
		// Should not reach here
		break;
	}
	LeaveCriticalSection( &m_csModeMsgMutex );
	m_ThreadInfo->send( p );
}

void CJabberProto::SendPresence( int status, bool bSendToAll )
{
	SendPresenceTo( status, NULL, NULL );
	SendVisibleInvisiblePresence( status == ID_STATUS_INVISIBLE );

	// Also update status in all chatrooms
	if ( bSendToAll ) {
		LISTFOREACH(i, this, LIST_CHATROOM)
		{
			JABBER_LIST_ITEM *item = ListGetItemPtrFromIndex( i );
			if ( item != NULL )
				SendPresenceTo( status == ID_STATUS_INVISIBLE ? ID_STATUS_ONLINE : status, item->jid, NULL );
}	}	}

void __stdcall JabberStringAppend( char* *str, int *sizeAlloced, const char* fmt, ... )
{
	va_list vararg;
	char* p;
	size_t size, len;

	if ( str == NULL ) return;

	if ( *str==NULL || *sizeAlloced<=0 ) {
		*sizeAlloced = 2048;
        size = 2048;
		*str = ( char* )mir_alloc( size );
		len = 0;
	}
	else {
		len = strlen( *str );
		size = *sizeAlloced - strlen( *str );
	}

	p = *str + len;
	va_start( vararg, fmt );
	while ( _vsnprintf( p, size, fmt, vararg ) == -1 ) {
		size += 2048;
		( *sizeAlloced ) += 2048;
		*str = ( char* )mir_realloc( *str, *sizeAlloced );
		p = *str + len;
	}
	va_end( vararg );
}

///////////////////////////////////////////////////////////////////////////////
// JabberGetPacketID - converts the xml id attribute into an integer

int __stdcall JabberGetPacketID( HXML n )
{
	int result = -1;

	const TCHAR* str = xmlGetAttrValue( n, _T("id"));
	if ( str )
		if ( !_tcsncmp( str, _T(JABBER_IQID), SIZEOF( JABBER_IQID )-1 ))
			result = _ttoi( str + SIZEOF( JABBER_IQID )-1 );

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// JabberGetClientJID - adds a resource postfix to a JID

TCHAR* CJabberProto::GetClientJID( const TCHAR* jid, TCHAR* dest, size_t destLen )
{
	if ( jid == NULL )
		return NULL;

	size_t len = _tcslen( jid );
	if ( len >= destLen )
		len = destLen-1;

	_tcsncpy( dest, jid, len );
	dest[ len ] = '\0';

	TCHAR* p = _tcschr( dest, '/' );

	JABBER_LIST_ITEM* LI = ListGetItemPtr( LIST_ROSTER, jid );
	if ( LI && LI->resourceCount == 1 && LI->resource[ 0 ].szCapsNode &&
		_tcsicmp( LI->resource[ 0 ].szCapsNode, _T( "http://talk.google.com/xmpp/bot/caps")) == 0)
	{
		if ( p ) *p = 0; 
		return dest;
	}

	if ( p == NULL ) {
		TCHAR* resource = ListGetBestResourceNamePtr( jid );
		if ( resource != NULL )
			mir_sntprintf( dest+len, destLen-len-1, _T("/%s"), resource );
	}

	return dest;
}

///////////////////////////////////////////////////////////////////////////////
// JabberStripJid - strips a resource postfix from a JID

TCHAR* __stdcall JabberStripJid( const TCHAR* jid, TCHAR* dest, size_t destLen )
{
	if ( jid == NULL )
		*dest = 0;
	else {
		size_t len = _tcslen( jid );
		if ( len >= destLen )
			len = destLen-1;

		memcpy( dest, jid, len * sizeof( TCHAR ));
		dest[ len ] = 0;

		TCHAR* p = _tcschr( dest, '/' );
		if ( p != NULL )
			*p = 0;
	}

	return dest;
}

/////////////////////////////////////////////////////////////////////////////////////////
// JabberGetPictureType - tries to autodetect the picture type from the buffer

int __stdcall JabberGetPictureType( const char* buf )
{
	if ( buf != NULL ) {
		if ( memcmp( buf, "GIF8", 4 ) == 0 )     return PA_FORMAT_GIF;
		if ( memcmp( buf, "\x89PNG", 4 ) == 0 )  return PA_FORMAT_PNG;
		if ( memcmp( buf, "BM", 2 ) == 0 )       return PA_FORMAT_BMP;
		if ( memcmp( buf, "\xFF\xD8", 2 ) == 0 ) return PA_FORMAT_JPEG;
	}

	return PA_FORMAT_UNKNOWN;
}

/////////////////////////////////////////////////////////////////////////////////////////
// TStringPairs class members

TStringPairs::TStringPairs( char* buffer ) :
	elems( NULL )
{
   TStringPairsElem tempElem[ 100 ];

	char* token = strtok( buffer, "," );

	for ( numElems=0; token != NULL; numElems++ ) {
		char* p = strchr( token, '=' ), *p1;
		if ( p == NULL )
			break;

		while( isspace( *token ))
			token++;

		tempElem[ numElems ].name = rtrim( token );
		*p++ = 0;
		if (( p1 = strchr( p, '\"' )) != NULL ) {
			*p1 = 0;
			p = p1+1;
		}

		if (( p1 = strrchr( p, '\"' )) != NULL )
			*p1 = 0;

		tempElem[ numElems ].value = rtrim( p );
		token = strtok( NULL, "," );
	}

	if ( numElems ) {
		elems = new TStringPairsElem[ numElems ];
		memcpy( elems, tempElem, sizeof(tempElem[0]) * numElems );
}	}

TStringPairs::~TStringPairs()
{
	delete[] elems;
}

const char* TStringPairs::operator[]( const char* key ) const
{
	for ( int i = 0; i < numElems; i++ )
		if ( !strcmp( elems[i].name, key ))
			return elems[i].value;

	return "";
}

////////////////////////////////////////////////////////////////////////
// Manage combo boxes with recent item list

void CJabberProto::ComboLoadRecentStrings(HWND hwndDlg, UINT idcCombo, char *param, int recentCount)
{
	for (int i = 0; i < recentCount; ++i) {
		DBVARIANT dbv;
		char setting[MAXMODULELABELLENGTH];
		mir_snprintf(setting, sizeof(setting), "%s%d", param, i);
		if (!JGetStringT(NULL, setting, &dbv)) {
			SendDlgItemMessage(hwndDlg, idcCombo, CB_ADDSTRING, 0, (LPARAM)dbv.ptszVal);
			JFreeVariant(&dbv);
	}	}
	if (!SendDlgItemMessage(hwndDlg, idcCombo, CB_GETCOUNT, 0, 0))
		SendDlgItemMessage(hwndDlg, idcCombo, CB_ADDSTRING, 0, (LPARAM)_T(""));
}

void CJabberProto::ComboAddRecentString(HWND hwndDlg, UINT idcCombo, char *param, TCHAR *string, int recentCount)
{
	if (!string || !*string)
		return;
	if (SendDlgItemMessage(hwndDlg, idcCombo, CB_FINDSTRING, (WPARAM)-1, (LPARAM)string) != CB_ERR)
		return;

	int id;
	SendDlgItemMessage(hwndDlg, idcCombo, CB_ADDSTRING, 0, (LPARAM)string);
	if ((id = SendDlgItemMessage(hwndDlg, idcCombo, CB_FINDSTRING, (WPARAM)-1, (LPARAM)_T(""))) != CB_ERR)
		SendDlgItemMessage(hwndDlg, idcCombo, CB_DELETESTRING, id, 0);

	id = JGetByte(NULL, param, 0);
	char setting[MAXMODULELABELLENGTH];
	mir_snprintf(setting, sizeof(setting), "%s%d", param, id);
	JSetStringT(NULL, setting, string);
	JSetByte(NULL, param, (id+1)%recentCount);
}

/////////////////////////////////////////////////////////////////////////////////////////
// jabber frame maintenance code

static VOID CALLBACK sttRebuildInfoFrameApcProc( DWORD_PTR param )
{
	CJabberProto *ppro = (CJabberProto *)param;
	if (!ppro->m_pInfoFrame)
		return;

	ppro->m_pInfoFrame->LockUpdates();
	if (!ppro->m_bJabberOnline)
	{
		ppro->m_pInfoFrame->RemoveInfoItem("$/PEP");
		ppro->m_pInfoFrame->RemoveInfoItem("$/Transports");
		ppro->m_pInfoFrame->UpdateInfoItem("$/JID", LoadSkinnedIconHandle(SKINICON_OTHER_USERDETAILS), TranslateT("Offline"));
	} else
	{
		ppro->m_pInfoFrame->UpdateInfoItem("$/JID", LoadSkinnedIconHandle(SKINICON_OTHER_USERDETAILS), ppro->m_szJabberJID);

		if (!ppro->m_bPepSupported)
		{
			ppro->m_pInfoFrame->RemoveInfoItem("$/PEP");
		} else
		{
			ppro->m_pInfoFrame->RemoveInfoItem("$/PEP/");
			ppro->m_pInfoFrame->CreateInfoItem("$/PEP", false);
			ppro->m_pInfoFrame->UpdateInfoItem("$/PEP", ppro->GetIconHandle(IDI_PL_LIST_ANY), TranslateT("Advanced Status"));

			ppro->m_pInfoFrame->CreateInfoItem("$/PEP/mood", true);
			ppro->m_pInfoFrame->SetInfoItemCallback("$/PEP/mood", &CJabberProto::InfoFrame_OnUserMood);
			ppro->m_pInfoFrame->UpdateInfoItem("$/PEP/mood", LoadSkinnedIconHandle(SKINICON_OTHER_SMALLDOT), TranslateT("Set mood..."));

			ppro->m_pInfoFrame->CreateInfoItem("$/PEP/activity", true);
			ppro->m_pInfoFrame->SetInfoItemCallback("$/PEP/activity", &CJabberProto::InfoFrame_OnUserActivity);
			ppro->m_pInfoFrame->UpdateInfoItem("$/PEP/activity", LoadSkinnedIconHandle(SKINICON_OTHER_SMALLDOT), TranslateT("Set activity..."));
		}

		ppro->m_pInfoFrame->RemoveInfoItem("$/Transports/");
		ppro->m_pInfoFrame->CreateInfoItem("$/Transports", false);
		ppro->m_pInfoFrame->UpdateInfoItem("$/Transports", ppro->GetIconHandle(IDI_TRANSPORT), TranslateT("Transports"));

		JABBER_LIST_ITEM *item = NULL;
		LISTFOREACH(i, ppro, LIST_ROSTER)
		{
			if (( item=ppro->ListGetItemPtrFromIndex( i )) != NULL ) {
				if ( _tcschr( item->jid, '@' )==NULL && _tcschr( item->jid, '/' )==NULL && item->subscription!=SUB_NONE ) {
					HANDLE hContact = ppro->HContactFromJID( item->jid );
					if ( hContact == NULL ) continue;

					char name[128];
					char *jid_copy = mir_t2a(item->jid);
					mir_snprintf(name, SIZEOF(name), "$/Transports/%s", jid_copy);
					ppro->m_pInfoFrame->CreateInfoItem(name, true, (LPARAM)hContact);
					ppro->m_pInfoFrame->UpdateInfoItem(name, ppro->GetIconHandle(IDI_TRANSPORTL), (TCHAR *)item->jid);
					ppro->m_pInfoFrame->SetInfoItemCallback(name, &CJabberProto::InfoFrame_OnTransport);
					mir_free(jid_copy);
			}	}
		}
	}
	ppro->m_pInfoFrame->Update();
}

void CJabberProto::RebuildInfoFrame()
{
	QueueUserAPC(sttRebuildInfoFrameApcProc, hMainThread, (ULONG_PTR)this);
}

////////////////////////////////////////////////////////////////////////
// case-insensitive _tcsstr
const TCHAR *JabberStrIStr( const TCHAR *str, const TCHAR *substr)
{
	TCHAR *str_up = NEWTSTR_ALLOCA(str);
	TCHAR *substr_up = NEWTSTR_ALLOCA(substr);

	CharUpperBuff(str_up, lstrlen(str_up));
	CharUpperBuff(substr_up, lstrlen(substr_up));

	TCHAR* p = _tcsstr(str_up, substr_up);
	return p ? (str + (p - str_up)) : NULL;
}

////////////////////////////////////////////////////////////////////////
// clipboard processing
void JabberCopyText(HWND hwnd, TCHAR *text)
{
	if (!hwnd || !text) return;

	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(TCHAR)*(lstrlen(text)+1));
	TCHAR *s = (TCHAR *)GlobalLock(hMem);
	lstrcpy(s, text);
	GlobalUnlock(hMem);
#ifdef UNICODE
	SetClipboardData(CF_UNICODETEXT, hMem);
#else
	SetClipboardData(CF_TEXT, hMem);
#endif
	CloseClipboard();
}

/////////////////////////////////////////////////////////////////////////////////////////
// One string entry dialog

struct JabberEnterStringParams
{
	CJabberProto* ppro;

	int type;
	TCHAR* caption;
	TCHAR* result;
	size_t resultLen;
	char *windowName;
	int recentCount;
	int timeout;

	int idcControl;
	int height;
};

static int sttEnterStringResizer(HWND, LPARAM, UTILRESIZECONTROL *urc)
{
	switch (urc->wId)
	{
	case IDC_TXT_MULTILINE:
	case IDC_TXT_COMBO:
	case IDC_TXT_RICHEDIT:
		return RD_ANCHORX_LEFT|RD_ANCHORY_TOP|RD_ANCHORX_WIDTH|RD_ANCHORY_HEIGHT;
	case IDOK:
	case IDCANCEL:
		return RD_ANCHORX_RIGHT|RD_ANCHORY_BOTTOM;
	}
	return RD_ANCHORX_LEFT|RD_ANCHORY_TOP;
}

static INT_PTR CALLBACK sttEnterStringDlgProc( HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	JabberEnterStringParams *params = (JabberEnterStringParams *)GetWindowLongPtr( hwndDlg, GWLP_USERDATA );

	switch ( msg ) {
	case WM_INITDIALOG:
	{
		//SetWindowPos( hwndDlg, HWND_TOPMOST ,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE );
		TranslateDialogDefault( hwndDlg );
		SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadSkinnedIconBig(SKINICON_OTHER_RENAME));
		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadSkinnedIcon(SKINICON_OTHER_RENAME));
		JabberEnterStringParams *params = (JabberEnterStringParams *)lParam;
		SetWindowLongPtr( hwndDlg, GWLP_USERDATA, ( LONG_PTR )params );
		SetWindowText( hwndDlg, params->caption );

		RECT rc; GetWindowRect(hwndDlg, &rc);
		switch (params->type)
		{
			case JES_PASSWORD:
			{
				params->idcControl = IDC_TXT_PASSWORD;
				params->height = rc.bottom-rc.top;
				break;
			}
			case JES_MULTINE:
			{
				params->idcControl = IDC_TXT_MULTILINE;
				params->height = 0;
				rc.bottom += (rc.bottom-rc.top) * 2;
				SetWindowPos(hwndDlg, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE|SWP_NOREPOSITION);
				break;
			}
			case JES_COMBO:
			{
				params->idcControl = IDC_TXT_COMBO;
				params->height = rc.bottom-rc.top;
				if (params->windowName && params->recentCount)
					params->ppro->ComboLoadRecentStrings(hwndDlg, IDC_TXT_COMBO, params->windowName, params->recentCount);
				break;
			}
			case JES_RICHEDIT:
			{
				params->idcControl = IDC_TXT_RICHEDIT;
				SendDlgItemMessage(hwndDlg, IDC_TXT_RICHEDIT, EM_AUTOURLDETECT, TRUE, 0);
				SendDlgItemMessage(hwndDlg, IDC_TXT_RICHEDIT, EM_SETEVENTMASK, 0, ENM_LINK);
				params->height = 0;
				rc.bottom += (rc.bottom-rc.top) * 2;
				SetWindowPos(hwndDlg, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE|SWP_NOREPOSITION);
				break;
			}
		}

		ShowWindow(GetDlgItem(hwndDlg, params->idcControl), SW_SHOW);
		SetDlgItemText( hwndDlg, params->idcControl, params->result );

		if (params->windowName)
			Utils_RestoreWindowPosition(hwndDlg, NULL, params->ppro->m_szModuleName, params->windowName);

		SetTimer(hwndDlg, 1000, 50, NULL);

		if (params->timeout > 0)
		{
			SetTimer(hwndDlg, 1001, 1000, NULL);
			TCHAR buf[128];
			mir_sntprintf(buf, SIZEOF(buf), _T("%s (%d)"), TranslateT("OK"), params->timeout);
			SetDlgItemText(hwndDlg, IDOK, buf);
		}

		return TRUE;
	}
	case WM_DESTROY:
		WindowFreeIcon( hwndDlg );
		break;
	case WM_TIMER:
	{
		switch (wParam)
		{
			case 1000:
				KillTimer(hwndDlg,1000);
				EnableWindow(GetParent(hwndDlg), TRUE);
				return TRUE;

			case 1001:
			{
				TCHAR buf[128];
				mir_sntprintf(buf, SIZEOF(buf), _T("%s (%d)"), TranslateT("OK"), --params->timeout);
				SetDlgItemText(hwndDlg, IDOK, buf);

				if (params->timeout < 0)
				{
					KillTimer(hwndDlg, 1001);
					UIEmulateBtnClick(hwndDlg, IDOK);
				}

				return TRUE;
			}
		}
	}
	case WM_SIZE:
	{
		UTILRESIZEDIALOG urd = {0};
		urd.cbSize = sizeof(urd);
		urd.hInstance = hInst;
		urd.hwndDlg = hwndDlg;
		urd.lpTemplate = MAKEINTRESOURCEA(IDD_GROUPCHAT_INPUT);
		urd.pfnResizer = sttEnterStringResizer;
		CallService(MS_UTILS_RESIZEDIALOG, 0, (LPARAM)&urd);
		break;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
		if (params && params->height)
			lpmmi->ptMaxSize.y = lpmmi->ptMaxTrackSize.y = params->height;
		break;
	}
	case WM_NOTIFY:
	{
		ENLINK *param = (ENLINK *)lParam;
		if (param->nmhdr.idFrom != IDC_TXT_RICHEDIT) break;
		if (param->nmhdr.code != EN_LINK) break;
		if (param->msg != WM_LBUTTONUP) break;

		CHARRANGE sel;
		SendMessage(param->nmhdr.hwndFrom, EM_EXGETSEL, 0, (LPARAM) & sel);
		if (sel.cpMin != sel.cpMax) break; // allow link selection

		TEXTRANGE tr;
		tr.chrg = param->chrg;
		tr.lpstrText = (TCHAR *)mir_alloc(sizeof(TCHAR)*(tr.chrg.cpMax - tr.chrg.cpMin + 2));
        SendMessage(param->nmhdr.hwndFrom, EM_GETTEXTRANGE, 0, (LPARAM) & tr);

		char *tmp = mir_t2a(tr.lpstrText);
		CallService(MS_UTILS_OPENURL, 1, (LPARAM)tmp);
        mir_free(tmp);
        mir_free(tr.lpstrText);
        return TRUE;
	}
	case WM_COMMAND:
		switch ( LOWORD( wParam )) {
		case IDOK:
			GetDlgItemText( hwndDlg, params->idcControl, params->result, (int)params->resultLen );
			params->result[ params->resultLen-1 ] = 0;

			if ((params->type == JES_COMBO) && params->windowName && params->recentCount)
				params->ppro->ComboAddRecentString(hwndDlg, IDC_TXT_COMBO, params->windowName, params->result, params->recentCount);
			if (params->windowName)
				Utils_SaveWindowPosition(hwndDlg, NULL, params->ppro->m_szModuleName, params->windowName);

			EndDialog( hwndDlg, 1 );
			break;

		case IDCANCEL:
			if (params->windowName)
				Utils_SaveWindowPosition(hwndDlg, NULL, params->ppro->m_szModuleName, params->windowName);

			EndDialog( hwndDlg, 0 );
			break;

		case IDC_TXT_MULTILINE:
		case IDC_TXT_RICHEDIT:
			if ((HIWORD(wParam) != EN_SETFOCUS) && (HIWORD(wParam) != EN_KILLFOCUS))
			{
				SetDlgItemText(hwndDlg, IDOK, TranslateT("OK"));
				KillTimer(hwndDlg, 1001);
			}
			break;

		case IDC_TXT_COMBO:
			if ((HIWORD(wParam) != CBN_SETFOCUS) && (HIWORD(wParam) != CBN_KILLFOCUS))
			{
				SetDlgItemText(hwndDlg, IDOK, TranslateT("OK"));
				KillTimer(hwndDlg, 1001);
			}
			break;
	}	}

	return FALSE;
}

BOOL CJabberProto::EnterString(TCHAR *result, size_t resultLen, TCHAR *caption, int type, char *windowName, int recentCount, int timeout)
{
	bool free_caption = false;
	if (!caption || (caption==result))
	{
		free_caption = true;
		caption = mir_tstrdup( result );
		result[ 0 ] = _T('\0');
	}

	JabberEnterStringParams params = { this, type, caption, result, resultLen, windowName, recentCount, timeout };

	BOOL bRetVal = DialogBoxParam( hInst, MAKEINTRESOURCE( IDD_GROUPCHAT_INPUT ), GetForegroundWindow(), sttEnterStringDlgProc, LPARAM( &params ));

	if (free_caption) mir_free( caption );

	return bRetVal;
}

////////////////////////////////////////////////////////////////////////
// Premultiply bitmap channels for 32-bit bitmaps
void JabberBitmapPremultiplyChannels(HBITMAP hBitmap)
{
	BITMAP bmp;
	DWORD dwLen;
	BYTE *p;
	int x, y;

	GetObject(hBitmap, sizeof(bmp), &bmp);

	if (bmp.bmBitsPixel != 32)
		return;

	dwLen = bmp.bmWidth * bmp.bmHeight * (bmp.bmBitsPixel / 8);
	p = (BYTE *)malloc(dwLen);
	if (p == NULL)
		return;
	memset(p, 0, dwLen);

	GetBitmapBits(hBitmap, dwLen, p);

	for (y = 0; y < bmp.bmHeight; ++y)
	{
		BYTE *px = p + bmp.bmWidth * 4 * y;

		for (x = 0; x < bmp.bmWidth; ++x)
		{
			px[0] = px[0] * px[3] / 255;
			px[1] = px[1] * px[3] / 255;
			px[2] = px[2] * px[3] / 255;

			px += 4;
		}
	}

	SetBitmapBits(hBitmap, dwLen, p);

	free(p);
}

// Last resource map
void CJabberProto::CleanLastResourceMap()
{
	EnterCriticalSection( &m_csLastResourceMap );

	m_dwResourceMapPointer = 0;
	ZeroMemory( m_ulpResourceToDbEventMap, sizeof( m_ulpResourceToDbEventMap ));

	while ( m_pLastResourceList ) {
		void *pNext = (( void ** )m_pLastResourceList )[ 0 ];
		mir_free( m_pLastResourceList );
		m_pLastResourceList = pNext;
	}

	LeaveCriticalSection( &m_csLastResourceMap );
}

// lock CS before use
BOOL CJabberProto::IsLastResourceExists( void *pResource )
{
	if ( !pResource )
		return FALSE;

	void *pOurResource = m_pLastResourceList;
	while ( pOurResource ) {
		if ( pOurResource == pResource )
			return TRUE;
		pOurResource = (( void ** )pOurResource)[ 0 ];
	}
	return FALSE;
}

// lock CS before use
void* CJabberProto::AddToLastResourceMap( LPCTSTR szFullJid )
{
	// detach resource from full jid
	const TCHAR* szResource = _tcschr( szFullJid, '/' );
	if ( szResource == NULL )
		return NULL;
	if ( *++szResource == '\0' )
		return NULL;

	DWORD dwResourceCount = 0;

	void *pNewTailResource = NULL;
	void *pOurResource = m_pLastResourceList;
	while ( pOurResource ) {
		dwResourceCount++;

		if ( !_tcscmp(( TCHAR * )(( BYTE * )pOurResource + sizeof( void * )), szResource ))
			return pOurResource;

		void *pTmp = (( void ** )pOurResource )[ 0 ];
		if ( pTmp && !((( void ** )pTmp )[ 0 ]))
			pNewTailResource = pOurResource;
		pOurResource = pTmp;
	}

	if ( pNewTailResource && ( dwResourceCount > ( SIZEOF( m_ulpResourceToDbEventMap ) / 2 ))) {
		void *pTmp = (( void ** )pNewTailResource )[ 0 ];
		(( void ** )pNewTailResource )[ 0 ] = NULL;
		mir_free( pTmp );
	}

	void *pNewResource = mir_alloc( sizeof( void * ) + sizeof( TCHAR ) * ( _tcslen( szResource ) + 1 ));
	if ( !pNewResource )
		return NULL;

	(( void ** )pNewResource)[ 0 ] = m_pLastResourceList;
	_tcscpy(( TCHAR * )(( BYTE * )pNewResource + sizeof( void * )), szResource );

	m_pLastResourceList = pNewResource;

	return pNewResource;
}

// lock CS before use
TCHAR* CJabberProto::FindLastResourceByDbEvent( HANDLE hDbEvent )
{
	for ( int i = 0; i < SIZEOF( m_ulpResourceToDbEventMap ); i += 2 ) {
		if ( m_ulpResourceToDbEventMap[ i ] == ( ULONG_PTR )hDbEvent ) {
			TCHAR *szRetVal = ( TCHAR * )( m_ulpResourceToDbEventMap[ i + 1 ] + sizeof( void * ));
			m_ulpResourceToDbEventMap[ i ] = 0;
			m_ulpResourceToDbEventMap[ i + 1 ] = 0;
			return szRetVal;
		}
	}
	return NULL;
}

BOOL CJabberProto::IsMyOwnJID( LPCTSTR szJID )
{
	if ( !m_ThreadInfo )
		return FALSE;

	TCHAR* szFrom = JabberPrepareJid( szJID );
	if ( !szFrom )
		return FALSE;

	TCHAR* szTo = JabberPrepareJid( m_ThreadInfo->fullJID );
	if ( !szTo ) {
		mir_free( szFrom );
		return FALSE;
	}

	TCHAR* pDelimiter = _tcschr( szFrom, _T('/') );
	if ( pDelimiter ) *pDelimiter = _T('\0');

	pDelimiter = _tcschr( szTo, _T('/') );
	if ( pDelimiter ) *pDelimiter = _T('\0');

	BOOL bRetVal = _tcscmp( szFrom, szTo ) == 0;

	mir_free( szFrom );
	mir_free( szTo );

	return bRetVal;
}