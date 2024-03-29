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

#include "WQLOperation.h"

PEGASUS_NAMESPACE_BEGIN

const char* WQLOperationToString(WQLOperation x)
{
    switch (x)
    {
        case WQL_OR: return "WQL_OR";
        case WQL_AND: return "WQL_AND";
        case WQL_NOT: return "WQL_NOT";
        case WQL_EQ: return "WQL_EQ";
        case WQL_NE: return "WQL_NE";
        case WQL_LT: return "WQL_LT";
        case WQL_LE: return "WQL_LE";
        case WQL_GT: return "WQL_GT";
        case WQL_GE: return "WQL_GE";
        case WQL_IS_NULL: return "WQL_IS_NULL";
        case WQL_IS_TRUE: return "WQL_IS_TRUE";
        case WQL_IS_FALSE: return "WQL_IS_FALSE";
        case WQL_IS_NOT_NULL: return "WQL_IS_NOT_NULL";
        case WQL_IS_NOT_TRUE: return "WQL_IS_NOT_TRUE";
        case WQL_IS_NOT_FALSE: return "WQL_IS_NOT_FALSE";
        default: return "UNKNOWN OPERATION";
    }
}

PEGASUS_NAMESPACE_END
