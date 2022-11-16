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
// Author: Mike Brasher (mbrasher@bmc.com)
//       : Denise Eckstein (denise_eckstein@hp.com)
//
// Modified By:  Susan Campbell, Hewlett-Packard Company <scampbell@hp.com>
//               k. v. le   <ikhanh@us.ibm.com>
//              Sean Keenan, Hewlett-Packard Company (sean.keenan@hp.com)
//              Jim Wunderlich (Jim_Wunderlich@prodigy.net)
//               Marek Szermutzky, IBM, (mszermutzky@de.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////
#include <Pegasus/Common/Config.h>

PEGASUS_USING_PEGASUS;

#if defined(PEGASUS_PLATFORM_HPUX_ACC)
# include "OperatingSystem_HPUX.cpp"
#elif defined (PEGASUS_OS_TYPE_WINDOWS)
# include "OperatingSystem_Windows.cpp"
#elif defined (PEGASUS_PLATFORM_LINUX_GENERIC_GNU)
# include "OperatingSystem_Linux.cpp"
#elif defined (PEGASUS_OS_AIX)
# include "OperatingSystem_AIX.cpp"
#elif defined (PEGASUS_OS_DARWIN)
#include "OperatingSystem_DARWIN.cpp"
#elif defined (PEGASUS_OS_VMS)
# include "OperatingSystem_Vms.cpp"
#elif defined (PEGASUS_OS_SOLARIS)
# include "OperatingSystem_Solaris.cpp"
#elif defined (PEGASUS_OS_ZOS)
#include "OperatingSystem_zOS.cpp"
#else
# include "OperatingSystem_Stub.cpp"
#endif