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

#ifndef Pegasus_IndicationProvider_h
#define Pegasus_IndicationProvider_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Provider/CIMIndicationProvider.h>
#include <Pegasus/Provider/CIMMethodProvider.h>


class IndicationProvider :
    public PEGASUS_NAMESPACE(CIMMethodProvider),
    public PEGASUS_NAMESPACE(CIMIndicationProvider)
{
public:
    IndicationProvider() throw ();
    virtual ~IndicationProvider() throw ();

    // CIMProvider interface
    virtual void initialize(PEGASUS_NAMESPACE(CIMOMHandle)& cimom);
    virtual void terminate();

    // CIMIndicationProvider interface
    virtual void enableIndications(
        PEGASUS_NAMESPACE(IndicationResponseHandler)& handler);
    virtual void disableIndications();

    virtual void createSubscription(
        const PEGASUS_NAMESPACE(OperationContext)& context,
        const PEGASUS_NAMESPACE(CIMObjectPath)& subscriptionName,
        const PEGASUS_NAMESPACE(Array) <PEGASUS_NAMESPACE(CIMObjectPath)>&
            classNames,
        const PEGASUS_NAMESPACE(CIMPropertyList)& propertyList,
        const PEGASUS_NAMESPACE(Uint16) repeatNotificationPolicy);

    virtual void modifySubscription(
        const PEGASUS_NAMESPACE(OperationContext)& context,
        const PEGASUS_NAMESPACE(CIMObjectPath)& subscriptionName,
        const PEGASUS_NAMESPACE(Array) <PEGASUS_NAMESPACE(CIMObjectPath)>&
            classNames,
        const PEGASUS_NAMESPACE(CIMPropertyList)& propertyList,
        const PEGASUS_NAMESPACE(Uint16) repeatNotificationPolicy);

    virtual void deleteSubscription(
        const PEGASUS_NAMESPACE(OperationContext)& context,
        const PEGASUS_NAMESPACE(CIMObjectPath)& subscriptionName,
        const PEGASUS_NAMESPACE(Array) <PEGASUS_NAMESPACE(CIMObjectPath)>&
            classNames);

    // CIMMethodProvider Interface
    virtual void invokeMethod(
        const PEGASUS_NAMESPACE(OperationContext)& context,
        const PEGASUS_NAMESPACE(CIMObjectPath)& objectReference,
        const PEGASUS_NAMESPACE(CIMName)& methodName,
        const PEGASUS_NAMESPACE(Array)<PEGASUS_NAMESPACE(CIMParamValue)>&
            inParameters,
        PEGASUS_NAMESPACE(MethodResultResponseHandler)& handler);
};

#endif
