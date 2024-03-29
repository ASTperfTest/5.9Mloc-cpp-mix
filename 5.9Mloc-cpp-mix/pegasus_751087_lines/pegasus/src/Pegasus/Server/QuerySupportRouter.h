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

#ifndef Pegasus_QuerySupportRouter_h
#define Pegasus_QuerySupportRouter_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>

#include <Pegasus/Server/CIMOperationRequestDispatcher.h>

PEGASUS_NAMESPACE_BEGIN

/**
    This class is the basis for encapsulating Query support such that
    CIMOperationRequestDispatcher.cpp is not to be changed for adding
    new/additional types of query support.

    New types of query support (like CQL) need to subclass
    CIMOperationRequestDispatcher as can be seen in
    WQLCIMOperatioRequestDispatcher.

    Also, QuerySupportRouter.cpp must be extended to recognize the
    CQL query language identifier and route the requests to the
    appropriate query language processor. The new query language processor
    has to be added to the pegserver library.

    In addition, ProviderManager2/QueryExpressionFactory.h has to be
    extended similarly.
*/
class QuerySupportRouter
{
public:
    static Boolean routeHandleExecQueryRequest(
        CIMOperationRequestDispatcher* opThis,
        CIMExecQueryRequestMessage* msg);

    static void routeHandleExecQueryResponseAggregation(
        CIMOperationRequestDispatcher* opThis,
        OperationAggregate* poA);

    static void routeApplyQueryToEnumeration(
        CIMOperationRequestDispatcher* opThis,
        CIMResponseMessage* msg,
        QueryExpressionRep* query);
};

PEGASUS_NAMESPACE_END

#endif
