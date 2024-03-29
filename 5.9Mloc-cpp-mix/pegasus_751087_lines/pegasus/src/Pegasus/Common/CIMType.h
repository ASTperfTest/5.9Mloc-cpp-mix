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

#ifndef Pegasus_Type_h
#define Pegasus_Type_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

/**
    The CIMType enumeration defines symbolic constants for the CIM data types.

    The table below shows each CIM type, its symbolic constant, and its
    representation type.

    <pre>
        CIM Type    CIMType Symbol       C++ Type
        ----------------------------------------------
        boolean     CIMTYPE_BOOLEAN      Boolean
        uint8       CIMTYPE_UINT8        Uint8
        sint8       CIMTYPE_SINT8        Sint8
        uint16      CIMTYPE_UINT16       Uint16
        sint16      CIMTYPE_SINT16       Sint16
        uint32      CIMTYPE_UINT32       Uint32
        sint32      CIMTYPE_SINT32       Sint32
        uint64      CIMTYPE_UINT64       Sint64
        sint64      CIMTYPE_SINT64       Sint64
        real32      CIMTYPE_REAL32       Real32
        real64      CIMTYPE_REAL64       Real64
        char16      CIMTYPE_CHAR16       Char16
        string      CIMTYPE_STRING       String
        datetime    CIMTYPE_DATETIME     CIMDateTime
        reference   CIMTYPE_REFERENCE    CIMObjectPath
        object      CIMTYPE_OBJECT       CIMObject (not a basic CIM type)
    </pre>
*/

enum CIMType
{
    CIMTYPE_BOOLEAN,
    CIMTYPE_UINT8,
    CIMTYPE_SINT8,
    CIMTYPE_UINT16,
    CIMTYPE_SINT16,
    CIMTYPE_UINT32,
    CIMTYPE_SINT32,
    CIMTYPE_UINT64,
    CIMTYPE_SINT64,
    CIMTYPE_REAL32,
    CIMTYPE_REAL64,
    CIMTYPE_CHAR16,
    CIMTYPE_STRING,
    CIMTYPE_DATETIME,
    CIMTYPE_REFERENCE,
    CIMTYPE_OBJECT
#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES
#ifdef PEGASUS_EMBEDDED_INSTANCE_SUPPORT
    , CIMTYPE_INSTANCE
#endif // PEGASUS_EMBEDDED_INSTANCE_SUPPORT
#endif // PEGASUS_USE_EXPERIMENTAL_INTERFACES
};

/**
    Gets a string representation of a CIMType.

    Note: the current implementation returns a string matching the first
    column in the table above, but that is subject to change in later
    revisions.
    @param type The CIMType to convert to string form.
    @return The string form of the specified CIMType.
*/
PEGASUS_COMMON_LINKAGE const char * cimTypeToString (
    const CIMType type);

PEGASUS_NAMESPACE_END

#endif /* Pegasus_Type_h */
