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
//%////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Shutdown Provider
///////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Signal.h>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Tracer.h>
#include <Pegasus/Common/MessageLoader.h>
#include <Pegasus/Provider/CIMMethodProvider.h>
#include <Pegasus/Server/ShutdownService.h>
#include "ShutdownProvider.h"

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

static const CIMName METHOD_NAME_SHUTDOWN = CIMName ("shutdown");

//
// Invoke method used to shutdown cimom.
//
void ShutdownProvider::invokeMethod(
    const OperationContext& context,
    const CIMObjectPath& objectReference,
    const CIMName& methodName,
    const Array<CIMParamValue>& inParameters,
    MethodResultResponseHandler& handler)
{
    PEG_METHOD_ENTER(TRC_SHUTDOWN, "ShutdownProvider::invokeMethod()");

    // Check to see if the method name is correct
    if (!methodName.equal(METHOD_NAME_SHUTDOWN))
    {
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION(
            CIM_ERR_METHOD_NOT_AVAILABLE, String::EMPTY);
    }

    // Get user name
    String userName;
    if (context.contains(IdentityContainer::NAME))
    {
        IdentityContainer container = context.get(IdentityContainer::NAME);
        userName = container.getUserName();
    }
    else
    {
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION(CIM_ERR_FAILED, String::EMPTY);
    }

#ifndef PEGASUS_ZOS_SECURITY
    // Only privileged user can execute this operation
    if ((userName != String::EMPTY) && !System::isPrivilegedUser(userName))
    {
        PEG_METHOD_EXIT();
        MessageLoaderParms parms(
            "ControlProviders.UserAuthProvider.MUST_BE_PRIVILEGED_USER",
            "Must be a privileged user to execute this CIM operation.");
        throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_ACCESS_DENIED, parms);
    }
#endif

    // Begin processing the request
    handler.processing();

    // Check if the input parameters are passed.
    if (inParameters.size() < 2)
    {
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_INVALID_PARAMETER,
            MessageLoaderParms(
                "Server.ShutdownProvider.INPUT_NOT_VALID",
                "Input parameters are not valid."));
    }

    Boolean force = false;
    Uint32 timeoutValue = 0;

    // Get the input parameter values
    for (Uint32 i = 0; i < inParameters.size(); i++)
    {
        String parmName = inParameters[i].getParameterName();
        if (String::equalNoCase(parmName, "force"))
        {
            //
            // get the force parameter
            //
            inParameters[i].getValue().get(force);
        }
        else
        {
            if (String::equalNoCase(parmName, "timeout"))
            {
                //
                // get the timeout value
                //
                inParameters[i].getValue().get(timeoutValue);
            }
            else
            {
                PEG_METHOD_EXIT();
                throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_INVALID_PARAMETER,
                    MessageLoaderParms(
                        "Server.ShutdownProvider.INPUT_NOT_VALID",
                        "Input parameters are not valid."));
            }
        }
    }

    try
    {
        _shutdownService->shutdown(force, timeoutValue, true);
    }
    catch (CIMException& e)
    {
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION(CIM_ERR_FAILED, e.getMessage());
    }
    catch (Exception& e)
    {
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION(CIM_ERR_FAILED, e.getMessage());
    }

    handler.deliver(CIMValue(0));
    handler.complete();
    PEG_METHOD_EXIT();
}

PEGASUS_NAMESPACE_END
