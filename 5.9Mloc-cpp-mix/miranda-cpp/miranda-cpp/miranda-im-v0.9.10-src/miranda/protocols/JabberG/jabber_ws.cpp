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

File name      : $URL: https://miranda.googlecode.com/svn/branches/stable/miranda/protocols/JabberG/jabber_ws.cpp $
Revision       : $Revision: 11748 $
Last change on : $Date: 2010-05-15 14:05:54 -0400 (Sat, 15 May 2010) $
Last change by : $Author: borkra $

*/

#include "jabber.h"

BOOL CJabberProto::WsInit( void )
{
	m_lastTicks = ::GetTickCount();

	TCHAR name[128];
	mir_sntprintf( name, SIZEOF(name), TranslateT("%s connection"), m_tszUserName);

	NETLIBUSER nlu = {0};
	nlu.cbSize = sizeof( nlu );
	nlu.flags = NUF_OUTGOING | NUF_INCOMING | NUF_HTTPCONNS | NUF_TCHAR;	// | NUF_HTTPGATEWAY;
	nlu.ptszDescriptiveName = name;
	nlu.szSettingsModule = m_szModuleName;
	//nlu.szHttpGatewayHello = "http://http.proxy.icq.com/hello";
	//nlu.szHttpGatewayUserAgent = "Mozilla/4.08 [en] ( WinNT; U ;Nav )";
	//nlu.pfnHttpGatewayInit = JabberHttpGatewayInit;
	//nlu.pfnHttpGatewayBegin = JabberHttpGatewayBegin;
	//nlu.pfnHttpGatewayWrapSend = JabberHttpGatewayWrapSend;
	//nlu.pfnHttpGatewayUnwrapRecv = JabberHttpGatewayUnwrapRecv;
	m_hNetlibUser = ( HANDLE ) JCallService( MS_NETLIB_REGISTERUSER, 0, ( LPARAM )&nlu );

	return m_hNetlibUser != NULL;
}

void CJabberProto::WsUninit( void )
{
	Netlib_CloseHandle( m_hNetlibUser );
	m_hNetlibUser = NULL;
}

JABBER_SOCKET CJabberProto::WsConnect( char* host, WORD port )
{
	NETLIBOPENCONNECTION nloc = { 0 };
	nloc.cbSize = sizeof( nloc );
	nloc.szHost = host;
	nloc.wPort = port;
	nloc.timeout = 6;
	return ( HANDLE )JCallService( MS_NETLIB_OPENCONNECTION, ( WPARAM ) m_hNetlibUser, ( LPARAM )&nloc );
}

int CJabberProto::WsSend( JABBER_SOCKET hConn, char* data, int datalen, int flags )
{
	m_lastTicks = ::GetTickCount();
	int len;

	if (( len = Netlib_Send( hConn, data, datalen, flags )) == SOCKET_ERROR || len != datalen ) {
		Log( "Netlib_Send() failed, error=%d", WSAGetLastError());
		return SOCKET_ERROR;
	}
	return len;
}

int CJabberProto::WsRecv( JABBER_SOCKET hConn, char* data, long datalen, int flags )
{
	int ret;

	ret = Netlib_Recv( hConn, data, datalen, flags );
	if ( ret == SOCKET_ERROR ) {
		Log( "Netlib_Recv() failed, error=%d", WSAGetLastError());
		return 0;
	}
	if ( ret == 0 ) {
		Log( "Connection closed gracefully" );
		return 0;
	}
	return ret;
}
