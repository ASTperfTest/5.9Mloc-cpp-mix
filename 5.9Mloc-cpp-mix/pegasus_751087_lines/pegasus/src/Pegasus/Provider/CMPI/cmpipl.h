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
//=============================================================================
//
//%////////////////////////////////////////////////////////////////////////////

#ifndef _CMPIPL_H_
#define _CMPIPL_H_


#ifdef PEGASUS_PLATFORM_LINUX_GENERIC_GNU
# define CMPI_PLATFORM_LINUX_GENERIC_GNU 1
#endif

#ifdef PEGASUS_PLATFORM_HPUX_ACC
# define CMPI_PLATFORM_HPUX_ACC 1
#endif

#if defined(PEGASUS_PLATFORM_WIN64_IA64_MSVC) || \
    defined(PEGASUS_PLATFORM_WIN64_X86_64_MSVC) || \
    defined(PEGASUS_PLATFORM_WIN32_IX86_MSVC)
# define CMPI_PLATFORM_WIN32_IX86_MSVC 1
#endif

#ifdef PEGASUS_PLATFORM_SOLARIS_SPARC_GNU
# define CMPI_PLATFORM_SOLARIS_SPARC_GNU 1
#endif

#ifdef PEGASUS_PLATFORM_SOLARIS_SPARC_CC
# define CMPI_PLATFORM_SOLARIS_SPARC_CC 1
#endif

#ifdef PEGASUS_PLATFORM_AIX_RS_IBMCXX
# define CMPI_PLATFORM_AIX_RS_IBMCXX 1
#endif

#   ifdef PEGASUS_PLATFORM_PASE_ISERIES_IBMCXX
#      define CMPI_PLATFORM_PASE_ISERIES_IBMCXX 1
#   endif

#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
# define CMPI_PLATFORM_ZOS_ZSERIES_IBM 1
#endif

#ifdef PEGASUS_PLATFORM_TRU64_ALPHA_DECCXX 
# define CMPI_PLATFORM_TRU64_ALPHA_DECCXX 1
#endif

#ifdef PEGASUS_PLATFORM_DARWIN_PPC_GNU
# define CMPI_PLATFORM_DARWIN_PPC_GNU 1
#endif

#ifdef PEGASUS_PLATFORM_DARWIN_IX86_GNU
# define CMPI_PLATFORM_DARWIN_IX86_GNU 1
#endif

#ifdef PEGASUS_PLATFORM_VMS_ALPHA_DECCXX
# define CMPI_PLATFORM_VMS_ALPHA_DECCXX 1
#endif

#ifdef PEGASUS_PLATFORM_VMS_IA64_DECCXX
# define CMPI_PLATFORM_VMS_IA64_DECCXX 1
#endif

#endif
