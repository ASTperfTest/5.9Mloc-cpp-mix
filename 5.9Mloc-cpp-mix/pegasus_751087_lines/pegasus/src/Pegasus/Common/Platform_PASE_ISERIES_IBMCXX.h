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

#ifndef Pegasus_Platform_PASE_ISERIES_IBMCXX_h
#define Pegasus_Platform_PASE_ISERIES_IBMCXX_h

#define PASE_DEFAULT_PEGASUS_HOME "/QOpenSys/QIBM/UserData/UME/Pegasus"

#define PASE_PROD_HOME "/QOpenSys/QIBM/ProdData/UME/Pegasus"

#define PASE_DEFAULT_LOG_DIRECTORY "/QOpenSys/QIBM/UserData/UME/Pegasus/logs"

#define PASE_DEFAULT_MESSAGE_SOURCE "/QOpenSys/QIBM/ProdData/UME/Pegasus/msg"

#define PEGASUS_OS_TYPE_UNIX

#define PEGASUS_OS_PASE

#define PEGASUS_ARCHITECTURE_ISERIES

#define PEGASUS_COMPILER_IBMCXX

#define PEGASUS_UINT64 unsigned long long

#define PEGASUS_SINT64 signed long long  

#define PEGASUS_HAVE_NAMESPACES

#define PEGASUS_HAVE_FOR_SCOPE

#define PEGASUS_HAVE_TEMPLATE_SPECIALIZATION

#define PEGASUS_NO_PASSWORDFILE

// This enables the default pluggable provider manager.
// See ProviderManagerService.cpp
//#define PEGASUS_ENABLE_DEFAULT_PROVIDER_MANAGER

#define PEGASUS_USE_RELEASE_CONFIG_OPTIONS

#define PEGASUS_USE_RELEASE_DIRS

#define PEGASUS_SUPPORT_UTF8_FILENAME

//#define PEGASUS_REPOSITORY_NOT_NORMALIZED

#include <sys/param.h> // For MAXHOSTNAMELEN
#define PEGASUS_MAXHOSTNAMELEN (MAXHOSTNAMELEN + 1)

#ifndef PEGASUS_USE_EXPERIMENTAL_INTERFACES
#define PEGASUS_USE_EXPERIMENTAL_INTERFACES
#endif

/* environment variable to store pase ccsid */
#define PEGASUS_ORIG_PASE_CCSID "PEGASUS_ORIG_PASE_CCSID"

#define PEGASUS_HAVE_PTHREADS
#endif /* Pegasus_Platform_PASE_ISERIES_IBMCXX_h */
