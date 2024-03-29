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

File name      : $URL: https://miranda.googlecode.com/svn/branches/stable/miranda/protocols/JabberG/jabber_secur.h $
Revision       : $Revision: 11206 $
Last change on : $Date: 2010-01-31 09:13:26 -0500 (Sun, 31 Jan 2010) $
Last change by : $Author: borkra $

*/

#include "jabber.h"

// basic class - provides interface for various Jabber auth

class TJabberAuth
{

protected:  bool        bIsValid;
            const char* szName;
			unsigned	complete;
				ThreadData* info;

public:
            TJabberAuth( ThreadData* );
	virtual ~TJabberAuth();

	virtual	char* getInitialRequest();
	virtual	char* getChallenge( const TCHAR* challenge );

	inline   const char* getName() const
				{	return szName;
				}

	inline   bool isValid() const
   			{	return bIsValid;
   			}
};

// plain auth - the most simple one

class TPlainAuth : public TJabberAuth
{
	typedef TJabberAuth CSuper;


public:		TPlainAuth( ThreadData* );
	virtual ~TPlainAuth();

	virtual	char* getInitialRequest();
};

// md5 auth - digest-based authorization

class TMD5Auth : public TJabberAuth
{
	typedef TJabberAuth CSuper;

				int iCallCount;
public:		
				TMD5Auth( ThreadData* );
	virtual ~TMD5Auth();

	virtual	char* getChallenge( const TCHAR* challenge );
};

// ntlm auth - LanServer based authorization

class TNtlmAuth : public TJabberAuth
{
	typedef TJabberAuth CSuper;

				HANDLE hProvider;
public:		
				TNtlmAuth( ThreadData*, const char* mechanism );
	virtual ~TNtlmAuth();

	virtual	char* getInitialRequest();
	virtual	char* getChallenge( const TCHAR* challenge );

	bool getSpn( TCHAR* szSpn, size_t dwSpnLen );
};
