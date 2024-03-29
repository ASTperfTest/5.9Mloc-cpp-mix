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

#include "InstanceQueryProvider.h"

PEGASUS_USING_PEGASUS;

InstanceQueryProvider::InstanceQueryProvider()
{
}

InstanceQueryProvider::~InstanceQueryProvider()
{
}

void InstanceQueryProvider::initialize(CIMOMHandle& cimom)
{
    // create default instances
    CIMInstance instance1("Sample_InstanceQueryProviderClass");
    CIMObjectPath reference1("Sample_InstanceQueryProviderClass.Identifier=1");

    instance1.addProperty(CIMProperty("Identifier", Uint8(1)));   // key
    instance1.addProperty(CIMProperty("Message", String("Hello World")));

    _instances.append(instance1);
    _instanceNames.append(reference1);

    CIMInstance instance2("Sample_InstanceQueryProviderClass");
    CIMObjectPath reference2("Sample_InstanceQueryProviderClass.Identifier=2");

    instance2.addProperty(CIMProperty("Identifier", Uint8(2)));   // key
    instance2.addProperty(CIMProperty("Message", String("Yo Planet")));

    _instances.append(instance2);
    _instanceNames.append(reference2);

    CIMInstance instance3("Sample_InstanceQueryProviderClass");
    CIMObjectPath reference3("Sample_InstanceQueryProviderClass.Identifier=3");

    instance3.addProperty(CIMProperty("Identifier", Uint8(3)));   // key
    instance3.addProperty(CIMProperty("Message", String("Hey Earth")));

    _instances.append(instance3);
    _instanceNames.append(reference3);
}

void InstanceQueryProvider::terminate()
{
    delete this;
}

void InstanceQueryProvider::getInstance(
    const OperationContext& context,
    const CIMObjectPath& instanceReference,
    const Boolean includeQualifiers,
    const Boolean includeClassOrigin,
    const CIMPropertyList& propertyList,
    InstanceResponseHandler& handler)
{
    // convert a potential fully qualified reference into a local reference
    // (class name and keys only).
    CIMObjectPath localReference = CIMObjectPath(
        String(),
        CIMNamespaceName(),
        instanceReference.getClassName(),
        instanceReference.getKeyBindings());

    // begin processing the request
    handler.processing();

    // instance index corresponds to reference index
    for (Uint32 i = 0, n = _instances.size(); i < n; i++)
    {
        if (localReference == _instanceNames[i])
        {
            // deliver requested instance
            handler.deliver(_instances[i]);
            break;
        }
    }

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::enumerateInstances(
    const OperationContext& context,
    const CIMObjectPath& classReference,
    const Boolean includeQualifiers,
    const Boolean includeClassOrigin,
    const CIMPropertyList& propertyList,
    InstanceResponseHandler& handler)
{
    // begin processing the request
    handler.processing();

    for (Uint32 i = 0, n = _instances.size(); i < n; i++)
    {
        // deliver instance
        handler.deliver(_instances[i]);
    }

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::enumerateInstanceNames(
    const OperationContext& context,
    const CIMObjectPath& classReference,
    ObjectPathResponseHandler& handler)
{
    // begin processing the request
    handler.processing();

    for (Uint32 i = 0, n = _instances.size(); i < n; i++)
    {
        // deliver reference
        handler.deliver(_instanceNames[i]);
    }

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::modifyInstance(
    const OperationContext& context,
    const CIMObjectPath& instanceReference,
    const CIMInstance& instanceObject,
    const Boolean includeQualifiers,
    const CIMPropertyList& propertyList,
    ResponseHandler& handler)
{
    // convert a potential fully qualified reference into a local reference
    // (class name and keys only).
    CIMObjectPath localReference = CIMObjectPath(
        String(),
        CIMNamespaceName(),
        instanceReference.getClassName(),
        instanceReference.getKeyBindings());

    // begin processing the request
    handler.processing();

    // instance index corresponds to reference index
    for (Uint32 i = 0, n = _instances.size(); i < n; i++)
    {
        if (localReference == _instanceNames[i])
        {
            // overwrite existing instance
            _instances[i] = instanceObject;
            break;
        }
    }

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::createInstance(
    const OperationContext& context,
    const CIMObjectPath& instanceReference,
    const CIMInstance& instanceObject,
    ObjectPathResponseHandler& handler)
{
    // Validate the class name
    if (!instanceObject.getClassName().equal(
            "Sample_InstanceQueryProviderClass"))
    {
        throw CIMNotSupportedException(
            instanceObject.getClassName().getString());
    }

    // Find the key property
    Uint32 idIndex = instanceObject.findProperty("Identifier");
    if (idIndex == PEG_NOT_FOUND)
    {
        throw CIMInvalidParameterException("Missing key value");
    }

    // Create the new instance name
    CIMValue idValue = instanceObject.getProperty(idIndex).getValue();
    Array<CIMKeyBinding> keys;
    keys.append(CIMKeyBinding("Identifier", idValue));
    CIMObjectPath instanceName = CIMObjectPath(
        String(),
        CIMNamespaceName(),
        instanceObject.getClassName(),
        keys);

    // Determine whether this instance already exists
    for (Uint32 i = 0, n = _instanceNames.size(); i < n; i++)
    {
        if (instanceName == _instanceNames[i])
        {
            throw CIMObjectAlreadyExistsException(instanceName.toString());
        }
    }

    // begin processing the request
    handler.processing();

    // add the new instance to the array
    _instances.append(instanceObject);
    _instanceNames.append(instanceName);

    // deliver the new instance name
    handler.deliver(instanceName);

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::deleteInstance(
    const OperationContext& context,
    const CIMObjectPath& instanceReference,
    ResponseHandler& handler)
{
    // convert a potential fully qualified reference into a local reference
    // (class name and keys only).
    CIMObjectPath localReference = CIMObjectPath(
        String(),
        CIMNamespaceName(),
        instanceReference.getClassName(),
        instanceReference.getKeyBindings());

    // begin processing the request
    handler.processing();

    // instance index corresponds to reference index
    for (Uint32 i = 0, n = _instances.size(); i < n; i++)
    {
        if (localReference == _instanceNames[i])
        {
            // save the instance locally
            CIMInstance cimInstance(_instances[i]);

            // remove instance from the array
            _instances.remove(i);
            _instanceNames.remove(i);

            // exit loop
            break;
        }
    }

    // complete processing the request
    handler.complete();
}

void InstanceQueryProvider::execQuery(
    const OperationContext& context,
    const CIMObjectPath& objectPath,
    const QueryExpression& query,
    InstanceResponseHandler& handler)
{
    String lcQuery = query.getQuery();
    lcQuery.toLower();
    String id;
    Boolean tooComplex = false;

    handler.processing();

    for (unsigned int i = 0; i < _instances.size(); i++)
    {
        CIMInstance& ci = _instances[i];
        if (query.evaluate(ci))
        {
            handler.deliver(ci);
        }
    }

    // Notify processing is complete
    handler.complete();

    return;
}  // execQuery
