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

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/CIMDateTime.h>

#include "IndicationProvider.h"

PEGASUS_USING_STD;

PEGASUS_USING_PEGASUS;

static IndicationResponseHandler* _handler = 0;
static Boolean _enabled = false;

void _generateIndication(
    IndicationResponseHandler* handler,
    const CIMName& methodName);

IndicationProvider::IndicationProvider() throw ()
{
}

IndicationProvider::~IndicationProvider() throw ()
{
}

void IndicationProvider::initialize(CIMOMHandle& cimom)
{
}

void IndicationProvider::terminate()
{
}

void IndicationProvider::enableIndications(
    IndicationResponseHandler& handler)
{
    _enabled = true;
    _handler = &handler;
}

void _generateIndication(
    IndicationResponseHandler* handler,
    const CIMName& methodName)
{
    if (_enabled)
    {
        CIMInstance indicationInstance(CIMName("RT_TestIndication"));

        CIMObjectPath path ;
        path.setNameSpace("SDKExamples/DefaultCXX");
        path.setClassName("RT_TestIndication");

        indicationInstance.setPath(path);

        char buffer[32];  // Should need 21 chars max
        sprintf(buffer,
            "%" PEGASUS_64BIT_CONVERSION_WIDTH "u",
            CIMDateTime::getCurrentDateTime().toMicroSeconds());
        indicationInstance.addProperty(
            CIMProperty("IndicationIdentifier", String(buffer)));

        CIMDateTime currentDateTime = CIMDateTime::getCurrentDateTime();
        indicationInstance.addProperty(
            CIMProperty("IndicationTime", currentDateTime));

        Array<String> correlatedIndications;
        indicationInstance.addProperty(
            CIMProperty("CorrelatedIndications", correlatedIndications));

        indicationInstance.addProperty(
            CIMProperty("MethodName", CIMValue(methodName.getString())));

        CIMIndication cimIndication(indicationInstance);

        handler->deliver(indicationInstance);
    }
}

void IndicationProvider::disableIndications()
{
    _enabled = false;
    _handler->complete();
}

void IndicationProvider::createSubscription(
    const OperationContext& context,
    const CIMObjectPath& subscriptionName,
    const Array <CIMObjectPath>& classNames,
    const CIMPropertyList& propertyList,
    const Uint16 repeatNotificationPolicy)
{
}

void IndicationProvider::modifySubscription(
    const OperationContext& context,
    const CIMObjectPath& subscriptionName,
    const Array <CIMObjectPath>& classNames,
    const CIMPropertyList& propertyList,
    const Uint16 repeatNotificationPolicy)
{
}

void IndicationProvider::deleteSubscription(
    const OperationContext& context,
    const CIMObjectPath& subscriptionName,
    const Array <CIMObjectPath>& classNames)
{
}

void IndicationProvider::invokeMethod(
    const OperationContext& context,
    const CIMObjectPath& objectReference,
    const CIMName& methodName,
    const Array<CIMParamValue>& inParameters,
    MethodResultResponseHandler& handler)
{
    Boolean sendIndication = false;
    handler.processing();

    if (objectReference.getClassName().equal("RT_TestIndication") &&
        _enabled)
    {
        if (methodName.equal("SendTestIndication"))
        {
            sendIndication = true;
            handler.deliver(CIMValue(0));
        }
    }

    else
    {
        handler.deliver(CIMValue(1));
        PEGASUS_STD(cout) << "Provider is not enabled." << PEGASUS_STD(endl);
    }

    handler.complete();

    if (sendIndication)
    {
        _generateIndication(_handler,"generateIndication");
    }
}
