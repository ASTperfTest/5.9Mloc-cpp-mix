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
// Author: Marek Szermutzky (MSzermutzky@de.ibm.com) PEP#139 Stage2
//         Robert Kieninger, (KIENINGR@de.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef MCCATestClient_h
#define MCCATestClient_h

#include <Pegasus/Common/String.h>
#include <Pegasus/ManagedClient/CIMDefaultClientConnectionManager.h>
#include <Pegasus/ManagedClient/CIMManagedClient.h>
#include <Clients/cliutils/Command.h>
#include <Clients/cliutils/CommandException.h>

#define CLDEBUG(X) PEGASUS_STD(cout) << "MCCATestClient:" << X \
    << PEGASUS_STD(endl)
// #define CLDEBUG(X);

PEGASUS_NAMESPACE_BEGIN

class MCCATestClient
{
public:

   MCCATestClient (void);
   ~MCCATestClient (void);

   void initialiseConnectionManager(const String& host, const String& port);
   
   Uint32 enumerateInstancesNames(const String& _host, const String& _port);

   Uint32 enumerateInstances(const String& _host, const String& _port);

   // i represents a number as value for the key
   void createInstance(const String& _host,
                       const String& _port,
                       const CIMNamespaceName& nameSpace,
                       Uint32 i);

   CIMInstance associatorsTest(const String& _host,
                               const String& _port,
                               const CIMNamespaceName& fromNS,
                               const CIMNamespaceName& toNS,
                               Uint32 key);

   CIMInstance getInstance(const String& _host,
                           const String& _port,
                           const CIMNamespaceName& nameSpace,
                           Uint32 key);

   void deleteInstance(const CIMInstance& toDelete);

   CIMDefaultClientConnectionManager *cca_dccm;
   CIMManagedClient client;

private:

};

PEGASUS_NAMESPACE_END

#endif /* MCCATestClient_h */
