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
//
// This class is the interface between the cimmof compiler, in its various
// forms, and the various Pegasus repository interfaces which as the
// time this class was created were CIMRepository and CIMClient.
//
// This class supports only the operations that the compiler needs, which
// are
//     addClass()
//     addInstance()
//     addQualifier()
//     createNameSpace()
//
// If we create compiler-like tools to do mass changes to the repository,
// then I expect that we will add methods to deal with the modification.
// This class is intended to be very light, basically making it easy
// to choose what repository and what repository interface to use.
// It includes both, since there's nothing to be saved by splitting them.
// Anything that the client or repository interface throws gets passed
// to the cimmofParser level, which is equipped to handle the exceptions
//

#ifndef CIMMOF_CLIENT_H_
#define CIMMOF_CLIENT_H_

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include "compilerCommonDefs.h"
#include <Pegasus/Client/CIMClient.h>
#include <Pegasus/Compiler/Linkage.h>

#define CIMMOFCLIENT_TIMEOUT_DEFAULT 120000

PEGASUS_NAMESPACE_BEGIN

// Forward declarations;
class CIMClass;
class CIMQualifierDecl;
class CIMInstance;

class PEGASUS_COMPILER_LINKAGE cimmofClient
{
    private:
        compilerCommonDefs::operationType _ot;
        CIMClient *_client;
    public:
        cimmofClient();
        virtual ~cimmofClient();
        void init(String &location,
                compilerCommonDefs::operationType ot);
        virtual void addClass(
                const CIMNamespaceName &nameSpace,
                CIMClass &Class) const;
        virtual void addQualifier(
                const CIMNamespaceName &nameSpace,
                CIMQualifierDecl &qual) const;
        virtual void addInstance(
                const CIMNamespaceName &nameSpace,
                CIMInstance &instance) const;
        virtual CIMQualifierDecl getQualifierDecl(
                const CIMNamespaceName &nameSpace,
                const CIMName &qualifierName) const;
        virtual CIMClass getClass(
                const CIMNamespaceName &nameSpace,
                const CIMName &className) const;
        virtual void modifyClass(
                const CIMNamespaceName &nameSpace,
                CIMClass &Class) const;
        virtual void createNameSpace(const CIMNamespaceName &nameSpace) const;
};

PEGASUS_NAMESPACE_END

#endif




