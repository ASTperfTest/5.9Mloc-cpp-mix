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

#ifndef Pegasus_Platform_HPUX_IA64_ACC_h
#define Pegasus_Platform_HPUX_IA64_ACC_h

#ifdef __cplusplus
# include <cstddef>
#endif

#define PEGASUS_OS_TYPE_UNIX

#define PEGASUS_OS_HPUX

#define PEGASUS_COMPILER_ACC

#define PEGASUS_POINTER_64BIT

#define PEGASUS_UINT64 unsigned long long

#define PEGASUS_SINT64 long long

#ifdef HPUX_IA64_NATIVE_COMPILER
   #define PEGASUS_HAVE_NAMESPACES
#endif

#define PEGASUS_HAVE_TEMPLATE_SPECIALIZATION

#define PEGASUS_MAXHOSTNAMELEN  256

#define PEGASUS_DISABLE_INTERNAL_INLINES

#define PEGASUS_HAVE_PTHREADS

#define PEGASUS_HAVE_NANOSLEEP

#define PEGASUS_HAS_SIGNALS

#endif /* Pegasus_Platform_HPUX_IA64_ACC_h */