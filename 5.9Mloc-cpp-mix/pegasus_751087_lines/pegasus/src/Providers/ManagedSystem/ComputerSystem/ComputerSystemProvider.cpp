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

// This file should be generic, and while it allows for a
// platform-specific extension to the CIM classes (by
// refering to EXTENDED capabilities), it should never
// explicitly name the extension.

// This provider is registered to support operations at
// several class levels:
//
//  CIM_ComputerSystem
//  CIM_UnitaryComputerSystem
//  <platform>_ComputerSustem
//
// Enumeration operations always return instances from the
// deepest class available.  All other
// operations take note of the specified class.
//
// Operations currently supported:
//
//  enumerateInstanceNames()
//  enumerateInstances()
//  getInstance()

// ==========================================================================
// includes
// ==========================================================================

#include "ComputerSystemProvider.h"
#include "ComputerSystem.h"

// ==========================================================================
// defines
// ==========================================================================

#define NUMKEYS_COMPUTER_SYSTEM              2


PEGASUS_USING_STD;
PEGASUS_USING_PEGASUS;

ComputerSystemProvider::ComputerSystemProvider()
{
}

ComputerSystemProvider::~ComputerSystemProvider()
{
}

void ComputerSystemProvider::getInstance(
    const OperationContext& context,
    const CIMObjectPath& ref,
    const Boolean includeQualifiers,
    const Boolean includeClassOrigin,
    const CIMPropertyList& propertyList,
    InstanceResponseHandler &handler)
{
    CIMName className = ref.getClassName();
    _checkClass(className);

    Array<CIMKeyBinding> keys = ref.getKeyBindings();

    //-- make sure we're the right instance
    unsigned int keyCount = NUMKEYS_COMPUTER_SYSTEM;
    CIMName keyName;
    String keyValue;

    if (keys.size() != keyCount)
        throw CIMInvalidParameterException("Wrong number of keys");

    for (unsigned int ii = 0; ii < keys.size(); ii++)
    {
        keyName = keys[ii].getName();
        keyValue = keys[ii].getValue();

        if (keyName.equal(PROPERTY_CREATION_CLASS_NAME) &&
            (String::equalNoCase(keyValue,CLASS_CIM_COMPUTER_SYSTEM) ||
             String::equalNoCase(keyValue,CLASS_CIM_UNITARY_COMPUTER_SYSTEM) ||
             String::equalNoCase(keyValue,CLASS_EXTENDED_COMPUTER_SYSTEM) ||
             String::equalNoCase(keyValue,String::EMPTY)))
        {
            keyCount--;
        }
        else if (keyName.equal("Name") &&
                 String::equalNoCase(keyValue,_cs.getHostName()))
        {
            keyCount--;
        }
    }

    if (keyCount)
    {
        throw CIMInvalidParameterException(String::EMPTY);
    }

    // return instance of specified class
    CIMInstance instance = _cs.buildInstance(ref.getClassName());

    handler.processing();
    handler.deliver(instance);
    handler.complete();

    return;
}

void ComputerSystemProvider::enumerateInstances(
    const OperationContext& context,
    const CIMObjectPath& ref,
    const Boolean includeQualifiers,
    const Boolean includeClassOrigin,
    const CIMPropertyList& propertyList,
    InstanceResponseHandler& handler)
{
    CIMName className = ref.getClassName();
    _checkClass(className);

    handler.processing();

    // Deliver instance only if request was for leaf class
    if (className.equal (CLASS_EXTENDED_COMPUTER_SYSTEM))
    {
        Array<CIMKeyBinding> keys;
        keys.append(CIMKeyBinding(
            PROPERTY_CREATION_CLASS_NAME,
            CLASS_CIM_COMPUTER_SYSTEM,
            CIMKeyBinding::STRING));
        keys.append(CIMKeyBinding(
            PROPERTY_NAME,
            _cs.getHostName(),
            CIMKeyBinding::STRING));
        CIMObjectPath instanceName(
            String::EMPTY,       // Hostname not required
            CIMNamespaceName(),  // Namespace not required
            CLASS_EXTENDED_COMPUTER_SYSTEM,
            keys);
        CIMInstance instance =
            _cs.buildInstance(CLASS_EXTENDED_COMPUTER_SYSTEM);
        instance.setPath(instanceName);
        handler.deliver(instance);
    }

    handler.complete();
    return;
}

void ComputerSystemProvider::enumerateInstanceNames(
    const OperationContext& context,
    const CIMObjectPath &ref,
    ObjectPathResponseHandler& handler)
{
    CIMName className = ref.getClassName();
    _checkClass(className);

    handler.processing();

    // Deliver instance only if request was for leaf class
    if (className.equal(CLASS_EXTENDED_COMPUTER_SYSTEM))
    {
        Array<CIMKeyBinding> keys;

        keys.append(CIMKeyBinding(
            PROPERTY_CREATION_CLASS_NAME,
            CLASS_CIM_COMPUTER_SYSTEM,
            CIMKeyBinding::STRING));
        keys.append(CIMKeyBinding(
            PROPERTY_NAME,
            _cs.getHostName(),
            CIMKeyBinding::STRING));

        handler.deliver(CIMObjectPath(
            _cs.getHostName(),
            ref.getNameSpace(),
            CLASS_EXTENDED_COMPUTER_SYSTEM,
            keys));
    }

    handler.complete();
    return;
}

void ComputerSystemProvider::modifyInstance(
    const OperationContext& context,
    const CIMObjectPath& ref,
    const CIMInstance& instanceObject,
    const Boolean includeQualifiers,
    const CIMPropertyList& propertyList,
    ResponseHandler& handler)
{
    throw CIMNotSupportedException(String::EMPTY);
}

void ComputerSystemProvider::createInstance(
    const OperationContext& context,
    const CIMObjectPath& ref,
    const CIMInstance& instanceObject,
    ObjectPathResponseHandler& handler)
{
    throw CIMNotSupportedException(String::EMPTY);
}

void ComputerSystemProvider::deleteInstance(
    const OperationContext& context,
    const CIMObjectPath& ref,
    ResponseHandler& handler)
{
    throw CIMNotSupportedException(String::EMPTY);
}

void ComputerSystemProvider::initialize(CIMOMHandle& handle)
{
    _ch = handle;
    // platform-specific routine to initialize protected members
    _cs.initialize();
}


void ComputerSystemProvider::terminate()
{
    delete this;
}

void ComputerSystemProvider::_checkClass(CIMName& className)
{
    if (!className.equal(CLASS_CIM_COMPUTER_SYSTEM) &&
        !className.equal(CLASS_CIM_UNITARY_COMPUTER_SYSTEM) &&
        !className.equal(CLASS_EXTENDED_COMPUTER_SYSTEM))
    {
        throw CIMNotSupportedException(String::EMPTY);
    }
}
