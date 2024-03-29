//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
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
// Author: Alex Dunfey (dunfey_alexander@emc.com)
//
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_EmbeddedInstanceProvider_h
#define Pegasus_EmbeddedInstanceProvider_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/AutoPtr.h>
#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <Pegasus/Provider/CIMIndicationProvider.h>
#include <Pegasus/Provider/CIMMethodProvider.h>

PEGASUS_USING_PEGASUS;

class EmbeddedInstanceProvider :
    public CIMInstanceProvider,
    public CIMMethodProvider,
    public CIMIndicationProvider
{
public:
    EmbeddedInstanceProvider();
    virtual ~EmbeddedInstanceProvider();

    // CIMProvider interface
    void initialize (CIMOMHandle & cimom);
    void terminate();

    // CIMIndicationProvider interface
    void enableIndications (IndicationResponseHandler & handler);
    void disableIndications();

    void createSubscription (
	    const OperationContext & context,
	    const CIMObjectPath & subscriptionName,
	    const Array <CIMObjectPath> & classNames,
	    const CIMPropertyList & propertyList,
	    const Uint16 repeatNotificationPolicy);

    void modifySubscription (
	    const OperationContext & context,
	    const CIMObjectPath & subscriptionName,
	    const Array <CIMObjectPath> & classNames,
	    const CIMPropertyList & propertyList,
	    const Uint16 repeatNotificationPolicy);

    void deleteSubscription (
	    const OperationContext & context,
	    const CIMObjectPath & subscriptionName,
	    const Array <CIMObjectPath> & classNames);

    // CIMMethodProvider Interface
    void invokeMethod(
         const OperationContext & context,
         const CIMObjectPath & objectReference,
         const CIMName & methodName,
         const Array<CIMParamValue> & inParameters,
         MethodResultResponseHandler & handler);

    // CIMInstanceProvider interface
	void getInstance(
		const OperationContext & context,
		const CIMObjectPath & ref,
		const Boolean includeQualifiers,
		const Boolean includeClassOrigin,
		const CIMPropertyList & propertyList,
		InstanceResponseHandler & handler);

	void enumerateInstances(
		const OperationContext & context,
		const CIMObjectPath & ref,
		const Boolean includeQualifiers,
		const Boolean includeClassOrigin,
		const CIMPropertyList & propertyList,
		InstanceResponseHandler & handler);

	void enumerateInstanceNames(
		const OperationContext & context,
		const CIMObjectPath & ref,
		ObjectPathResponseHandler & handler);

	void modifyInstance(
		const OperationContext & context,
		const CIMObjectPath & ref,
		const CIMInstance & obj,
		const Boolean includeQualifiers,
		const CIMPropertyList & propertyList,
		ResponseHandler & handler);

	void createInstance(
		const OperationContext & context,
		const CIMObjectPath & ref,
		const CIMInstance & obj,
		ObjectPathResponseHandler & handler);

	void deleteInstance(
		const OperationContext & context,
		const CIMObjectPath & ref,
		ResponseHandler & handler);


private:
    IndicationResponseHandler *indicationHandler;
    AutoPtr<CIMInstance> errorInstance;
    AutoPtr<CIMInstance> indicationInstance;
    CIMNamespaceName STATIC_REPOSITORY;
    CIMObjectPath repositoryPath;
    CIMOMHandle cimom;
};

#endif

