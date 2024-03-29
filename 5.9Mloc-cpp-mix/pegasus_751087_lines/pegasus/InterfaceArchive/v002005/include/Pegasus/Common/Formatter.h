//%2006////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2006 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; Symantec Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_Formatter_h
#define Pegasus_Formatter_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/Linkage.h>
#ifdef PEGASUS_INTERNALONLY
#include <Pegasus/Common/StrLit.h>
#endif

#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES

PEGASUS_NAMESPACE_BEGIN

/** <I><B>Experimental Interface</B></I><BR>
    Formatter is a class to build formatted strings from
    strings that contain variable defintions.  The
    variable definitions in the strings are of the form
    $<int>
    
    where <int> is a single digit integer (0 - 9).
    
    The variable subsituted may be String, Boolean Integer, Unsigned Integer
    or  real.
    
    The format subsitution may be escaped by preceding the
    $ with a \
    
    usage:
    Formatter::format (FormatString, variable0,.., variable9)
    
    Example:
    <pre>
    int total = 4;
    int count = 2;
    String name = "Output"
    String output = Formatter::format(
			"total $0 average $1 on $2", 
			total,
			total/count,
			name);
    produces the string
	 
      "total 4 average 2 on Output"
    
    </pre>
*/
class PEGASUS_COMMON_LINKAGE Formatter
{
public:

    class Arg
    {
    public:

	enum Type { VOIDT, STRING, CSTRLIT, BOOLEAN, INTEGER, 
	    UINTEGER, LINTEGER, ULINTEGER, REAL };

	Arg() : _type(VOIDT)
	{
	}

	Arg(const String& x) : _string(x), _type(STRING)
	{
	}

	Arg(const char* x) : _string(x), _type(STRING)
	{
	}

#ifdef PEGASUS_INTERNALONLY
	Arg(const StrLit& x) : _cstrlit(&x), _type(CSTRLIT)
	{
	}
#endif

	Arg(Boolean x) : _boolean(x), _type(BOOLEAN)
	{
	}

	Arg(Sint32 x) : _integer(x), _type(INTEGER)
	{
	}

	Arg(Uint32 x) : _uinteger(x), _type(UINTEGER)
	{
	}

	Arg(Sint64 x) : _lInteger(x), _type(LINTEGER)
	{
	}

	Arg(Uint64 x) : _lUInteger(x), _type(ULINTEGER)
	{
	}
	Arg(Real64 x) : _real(x), _type(REAL)
	{
	}

	String toString() const;

	void appendToString(String& out) const;
	
	friend class MessageLoader;  //l10n

    private:

	String _string;

	union
	{
	    Sint32 _integer;
	    Uint32 _uinteger;
	    Real64 _real;
	    int _boolean;
	    Sint64 _lInteger;
	    Uint64 _lUInteger;
#ifdef PEGASUS_INTERNALONLY
	    const StrLit* _cstrlit;
#else
	    const void* _unused;
#endif
	};

	Type _type;
    };

    /**	 Format function for the formatter
    */
    static String format(
	const String& formatString,
	const Arg& arg0 = Formatter::DEFAULT_ARG,
	const Arg& arg1 = Formatter::DEFAULT_ARG,
	const Arg& arg2 = Formatter::DEFAULT_ARG,
	const Arg& arg3 = Formatter::DEFAULT_ARG,
	const Arg& arg4 = Formatter::DEFAULT_ARG,
	const Arg& arg5 = Formatter::DEFAULT_ARG,
	const Arg& arg6 = Formatter::DEFAULT_ARG,
	const Arg& arg7 = Formatter::DEFAULT_ARG,
	const Arg& arg8 = Formatter::DEFAULT_ARG,
	const Arg& arg9 = Formatter::DEFAULT_ARG);

    static const Formatter::Arg DEFAULT_ARG;
};

PEGASUS_NAMESPACE_END

#endif /*  PEGASUS_USE_EXPERIMENTAL_INTERFACES */

#endif /* Pegasus_Formatter_h */
