//%2003////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002  BMC Software, Hewlett-Packard Development
// Company, L. P., IBM Corp., The Open Group, Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L. P.;
// IBM Corp.; EMC Corporation, The Open Group.
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
// Author: Mike Brasher (mbrasher@bmc.com)
//
// Modified By: Rudy Schuet (rudy.schuet@compaq.com) 11/25/01
//              K. Schopmeyer
//              Carol Ann Krug Graves, Hewlett-Packard Company
//                (carolann_graves@hp.com)
//              David Eger (dteger@us.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_Config_h
#define Pegasus_Config_h

#if defined(PEGASUS_PLATFORM_WIN32_IX86_MSVC)
# include <Pegasus/Common/Platform_WIN32_IX86_MSVC.h>
#elif defined (PEGASUS_PLATFORM_LINUX_IX86_GNU)
# include <Pegasus/Common/Platform_LINUX_IX86_GNU.h>
#elif defined (PEGASUS_PLATFORM_AIX_RS_IBMCXX)
# include <Pegasus/Common/Platform_AIX_RS_IBMCXX.h>
#elif defined (PEGASUS_PLATFORM_HPUX_PARISC_ACC)
# include <Pegasus/Common/Platform_HPUX_PARISC_ACC.h>
#elif defined (PEGASUS_PLATFORM_HPUX_IA64_ACC)
# include <Pegasus/Common/Platform_HPUX_IA64_ACC.h>
#elif defined (PEGASUS_PLATFORM_TRU64_ALPHA_DECCXX)
# include <Pegasus/Common/Platform_TRU64_ALPHA_DECCXX.h>
#elif defined (PEGASUS_PLATFORM_SOLARIS_SPARC_GNU)
# include <Pegasus/Common/Platform_SOLARIS_SPARC_GNU.h>
#elif defined (PEGASUS_PLATFORM_SOLARIS_SPARC_CC)
# include <Pegasus/Common/Platform_SOLARIS_SPARC_CC.h>
#elif defined (PEGASUS_PLATFORM_ZOS_ZSERIES_IBM)
# include <Pegasus/Common/Platform_ZOS_ZSERIES_IBM.h>
#elif defined (PEGASUS_PLATFORM_NSK_NONSTOP_NMCPLUS)
# include <Pegasus/Common/Platform_NSK_NONSTOP_NMCPLUS.h>
#elif defined (PEGASUS_PLATFORM_LINUX_IA64_GNU)
# include <Pegasus/Common/Platform_LINUX_IA64_GNU.h>
#elif defined (PEGASUS_PLATFORM_LINUX_PPC_GNU)
# include <Pegasus/Common/Platform_LINUX_PPC_GNU.h>
#elif defined (PEGASUS_PLATFORM_LINUX_ZSERIES_GNU)
# include <Pegasus/Common/Platform_LINUX_ZSERIES_GNU.h>
#elif defined (__OS400__)
# if !defined (PEGASUS_PLATFORM_OS400_ISERIES_IBM)
#   define PEGASUS_PLATFORM_OS400_ISERIES_IBM 
# endif
# include <Pegasus/Common/Platform_OS400_ISERIES_IBM.h>
#else
# error "<Pegasus/Common/Config.h>: Unsupported Platform"
#endif




//<<< Sun Apr  6 19:28:00 2003 mdd >>>
//
// COMPILER Checks
//
// This is to allow a check for GCC > 3.2
// It needs to be the first thing we check because the next lines load
// further source files

#if defined(__GNUC__)
#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)

// To test for GCC > 3.2.0:
//     #if GCC_VERSION > 30200

#endif 


#include <cstdlib>

// used for Windows only
#ifndef PEGASUS_EXPORT
#define PEGASUS_EXPORT /* empty */
#endif

#ifdef PEGASUS_HAVE_NAMESPACES
# define PEGASUS_NAMESPACE_BEGIN namespace Pegasus {
# define PEGASUS_NAMESPACE_END }

#ifndef PEGASUS_HAVE_NO_STD
# define PEGASUS_STD(X) std::X
# define PEGASUS_USING_STD using namespace std
#else
# define PEGASUS_STD(X) X
# define PEGASUS_USING_STD
#endif
# define PEGASUS_USING_PEGASUS using namespace Pegasus
#else
# define PEGASUS_NAMESPACE_BEGIN /* empty */
# define PEGASUS_NAMESPACE_END /* empty */
# define PEGASUS_STD(X) X
# define PEGASUS_USING_STD
# define PEGASUS_USING_PEGASUS
#endif

#ifdef PEGASUS_HAVE_EXPLICIT
# define PEGASUS_EXPLICIT explicit
#else
# define PEGASUS_EXPLICIT /* empty */
#endif

#ifdef PEGASUS_HAVE_MUTABLE
# define PEGASUS_MUTABLE mutable
#else
# define PEGASUS_MUTABLE /* empty */
#endif

#ifndef PEGASUS_HAVE_FOR_SCOPE
# define for if (0) ; else for
#endif

#ifdef PEGASUS_HAVE_TEMPLATE_SPECIALIZATION
# define PEGASUS_TEMPLATE_SPECIALIZATION template <>
#else
# define PEGASUS_TEMPLATE_SPECIALIZATION
#endif

#ifdef PEGASUS_HAVE_IOS_BINARY
#if defined(PEGASUS_PLATFORM_OS400_ISERIES_IBM) 
#define PEGASUS_IOS_BINARY ,std::ios::binary,PEGASUS_STD(_CCSID_T(1208))
#else
#define PEGASUS_IOS_BINARY ,std::ios::binary
#endif
#define PEGASUS_OR_IOS_BINARY | std::ios::binary
#else
#define PEGASUS_IOS_BINARY /* empty */
#define PEGASUS_OR_IOS_BINARY /* empty */
#endif

#ifndef PEGASUS_SINT64_LITERAL
#define PEGASUS_SINT64_LITERAL(X) (Sint64 (X##LL))
#endif
#ifndef PEGASUS_UINT64_LITERAL
#define PEGASUS_UINT64_LITERAL(X) (Uint64 (X##ULL))
#endif

// Used in printf and scanf conversion strings for Uint64 and Sint64 arguments
#ifndef PEGASUS_64BIT_CONVERSION_WIDTH
#define PEGASUS_64BIT_CONVERSION_WIDTH "ll"
#endif

PEGASUS_NAMESPACE_BEGIN

typedef bool Boolean;  // If platform w/o bool, add flag and include Boolean.h
typedef unsigned char Uint8;
typedef char Sint8;
typedef unsigned short Uint16;
typedef short Sint16;
typedef unsigned int Uint32;
typedef int Sint32;
typedef float Real32;
typedef double Real64;
typedef PEGASUS_UINT64 Uint64;
typedef PEGASUS_SINT64 Sint64;

#define PEG_NOT_FOUND Uint32(-1)

PEGASUS_NAMESPACE_END

#ifdef PEGASUS_SUPPRESS_UNREACHABLE_STATEMENTS
# define PEGASUS_UNREACHABLE(CODE)
#else
# define PEGASUS_UNREACHABLE(CODE) CODE
#endif

#ifdef PEGASUS_HAVE_EBCDIC
# define PEGASUS_MAX_PRINTABLE_CHAR 255
#else
# define PEGASUS_MAX_PRINTABLE_CHAR 127
#endif

// used for zOS only
#ifndef PEGASUS_STATIC_CDECL
#define PEGASUS_STATIC_CDECL
#endif


#endif  /* Pegasus_Config_h */
