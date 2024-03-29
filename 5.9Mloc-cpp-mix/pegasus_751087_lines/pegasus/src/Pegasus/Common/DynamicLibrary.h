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

#ifndef Pegasus_DynamicLibrary_h
#define Pegasus_DynamicLibrary_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Exception.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/Mutex.h>

#include <Pegasus/Common/Linkage.h>

#if defined(PEGASUS_OS_TYPE_WINDOWS)
# include <Pegasus/Common/Network.h>
# include <windows.h>
#endif

PEGASUS_NAMESPACE_BEGIN

class PEGASUS_COMMON_LINKAGE DynamicLibrary
{
public:
#if defined(PEGASUS_OS_TYPE_WINDOWS)
    typedef HMODULE DynamicLibraryHandle;
    typedef FARPROC DynamicSymbolHandle;
#else
    typedef void * DynamicLibraryHandle;
    typedef void * DynamicSymbolHandle;
#endif

public:
    DynamicLibrary();
    DynamicLibrary(const DynamicLibrary & library);
    explicit DynamicLibrary(const String & fileName);
    virtual ~DynamicLibrary();

    DynamicLibrary & operator=(const DynamicLibrary & library);

    Boolean load();
    const String& getLoadErrorMessage() const;
    void unload();

    Boolean isLoaded() const;

    const String& getFileName() const;
    DynamicSymbolHandle getSymbol(const String & symbolName);

private:
    Boolean _load();
    void _unload();

    String _fileName;
    DynamicLibraryHandle _handle;
    String _loadErrorMessage;
    Uint32 _referenceCount;
    Mutex _loadMutex;
};

PEGASUS_NAMESPACE_END

#endif
