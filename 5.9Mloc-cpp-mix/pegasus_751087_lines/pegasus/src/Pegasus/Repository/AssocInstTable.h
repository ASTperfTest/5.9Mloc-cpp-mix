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

#ifndef Pegasus_AssocInstTable_h
#define Pegasus_AssocInstTable_h

#include <iostream>
#include <fstream>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/CIMObjectPath.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/ArrayInternal.h>
#include <Pegasus/Repository/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

/** Maintains all associations for a given namesspace.
*/
class PEGASUS_REPOSITORY_LINKAGE AssocInstTable
{
public:

    /** Appends a row into the association table. There is no checking
        for duplicate entries (the caller ensures this). The case of
        the arguments doesn't matter. They are ignored during comparison.
    */
    static void append(
        PEGASUS_STD(ofstream)& os,
        const String& assocInstanceName,
        const CIMName& assocClassName,
        const String& fromInstanceName,
        const CIMName& fromClassName,
        const CIMName& fromPropertyName,
        const String& toInstanceName,
        const CIMName& toClassName,
        const CIMName& toPropertyName);

    /** Appends a row into the association table. There is no checking
        for duplicate entries (the caller ensures this). The case of the
        arguments doesn't matter. Case is ignored during comparison.
    */
    static void append(
        const String& path,
        const String& assocInstanceName,
        const CIMName& assocClassName,
        const String& fromInstanceName,
        const CIMName& fromClassName,
        const CIMName& fromPropertyName,
        const String& toInstanceName,
        const CIMName& toClassName,
        const CIMName& toPropertyName);

    /** Deletes the given association from the table by removing every entry
        with an assocInstanceName equal to the assocInstanceName parameter.
        @returns true if such an association was found.
    */
    static Boolean deleteAssociation(
        const String& path,
        const CIMObjectPath& assocInstanceName);

    /** Finds all associators of the given object. See
        CIMOperations::associators() for a full description.
    */
    static Boolean getAssociatorNames(
        const String& path,
        const CIMObjectPath& objectName,
        const Array<CIMName>& assocClassList,
        const Array<CIMName>& resultClassList,
        const String& role,
        const String& resultRole,
        Array<String>& associatorNames);

    /** Obtain all references (association instance names) in which the
        given object is involved. See CIMOperations::associators() for a
        full description.
    */
    static Boolean getReferenceNames(
        const String& path,
        const CIMObjectPath& objectName,
        const Array<CIMName>& resultClassList,
        const String& role,
        Array<String>& referenceNames);

private:

    AssocInstTable() { /* private */ }
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_AssocInstTable_h */
