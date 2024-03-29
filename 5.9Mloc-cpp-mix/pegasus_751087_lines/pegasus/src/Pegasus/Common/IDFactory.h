
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

#ifndef Pegasus_IDFactory_h
#define Pegasus_IDFactory_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Mutex.h>
#include <Pegasus/Common/Stack.h>
#include <Pegasus/Common/Magic.h>

PEGASUS_NAMESPACE_BEGIN


/** The IDFactory provides a thread-safe interface for assigning unique
    integer identifiers. The getID() method is used to obtain the next unique
    identifier. The putID(), returns a unique identifier to the id pool. Note
    that if putID() is never called, then getID() will assigns a sequence of
    monotonically increasing ids.
*/
class PEGASUS_COMMON_LINKAGE IDFactory
{
public:

    /** Constructor. The firstID argument is the id that is first returned
        by getID().
    */
    IDFactory(Uint32 firstID = 1);

    /** Destructor.
    */
    ~IDFactory();

    /** Obtain the next id.
    */
    Uint32 getID() const;

    /** Return an id to the pool (optional).
    */
    void putID(Uint32 id);

private:

    Magic<0x94E91236> _magic;
    Stack<Uint32> _pool;
    Uint32 _firstID;
    Uint32 _nextID;
    Mutex _mutex;
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_IDFactory_h */
