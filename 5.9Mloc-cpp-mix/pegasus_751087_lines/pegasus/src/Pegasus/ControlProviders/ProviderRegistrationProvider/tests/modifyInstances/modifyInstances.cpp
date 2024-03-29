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
#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Client/CIMClient.h>
#include <Pegasus/Repository/CIMRepository.h>
#include <Pegasus/Common/Constants.h>
// NOCHKSRC
#include <Pegasus/Server/ProviderRegistrationManager/ProviderRegistrationManager.h>
// DOCHKSRC

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

const CIMName CLASSNAME = CIMName ("PG_ProviderModule");
const CIMName CLASSNAME2 = CIMName ("PG_Provider");
const CIMName CLASSNAME3 = CIMName ("PG_ProviderCapabilities");

void TestModifyInstances(CIMClient& client)
{
    //
    // create Provider module instances
    //
    CIMObjectPath returnRef;

    CIMClass cimClass(CLASSNAME);

    CIMInstance cimInstance(CLASSNAME);

    cimInstance.addProperty(CIMProperty(CIMName ("Name"),
        String("providersModule1")));
    cimInstance.addProperty(CIMProperty(CIMName ("Vendor"), String("HP")));
    cimInstance.addProperty(CIMProperty(CIMName ("Version"), String("2.0")));
    cimInstance.addProperty(CIMProperty(CIMName ("InterfaceType"),
        String("C++Default")));
    cimInstance.addProperty(CIMProperty(CIMName ("InterfaceVersion"),
        String("2.1.0")));
    cimInstance.addProperty(CIMProperty(CIMName ("Location"),
        String("/tmp/module1")));

    CIMObjectPath instanceName = cimInstance.buildPath(cimClass);

    instanceName.setNameSpace(PEGASUS_NAMESPACENAME_INTEROP);
    instanceName.setClassName(CLASSNAME);

    returnRef =
        client.createInstance(PEGASUS_NAMESPACENAME_INTEROP, cimInstance);

    // create PG_Provider instances

    CIMObjectPath returnRef2;

    CIMClass cimClass2(CLASSNAME2);

    CIMInstance cimInstance2(CLASSNAME2);

    cimInstance2.addProperty(CIMProperty(CIMName ("ProviderModuleName"),
        String("providersModule1")));
    cimInstance2.addProperty(CIMProperty(CIMName ("Name"),
        String("PG_ProviderInstance1")));

    CIMObjectPath instanceName2 = cimInstance2.buildPath(cimClass2);

    instanceName2.setNameSpace(PEGASUS_NAMESPACENAME_INTEROP);
    instanceName2.setClassName(CLASSNAME2);

    returnRef2 =
        client.createInstance(PEGASUS_NAMESPACENAME_INTEROP, cimInstance2);

    //
    // create provider capability instances
    //

    Array <String> namespaces;
    Array <Uint16> providerType;
    Array <String> supportedMethods;
    Array <String> supportedProperties;

    namespaces.append("root/cimv2");
    namespaces.append("root/cimv3");

    providerType.append(4);
    providerType.append(5);

    supportedMethods.append("test_method1");
    supportedMethods.append("test_method2");

    supportedProperties.append("PkgStatus");
    supportedProperties.append("PkgIndex");

    CIMObjectPath returnRef3;

    CIMClass cimClass3(CLASSNAME3);

    CIMInstance cimInstance3(CLASSNAME3);

    cimInstance3.addProperty(CIMProperty(CIMName ("ProviderModuleName"),
        String("providersModule1")));
    cimInstance3.addProperty(CIMProperty(CIMName ("ProviderName"),
        String("PG_ProviderInstance1")));
    cimInstance3.addProperty(CIMProperty(CIMName ("CapabilityID"),
        String("capability1")));
    cimInstance3.addProperty(CIMProperty(CIMName ("ClassName"),
        String("TestSoftwarePkg")));
    cimInstance3.addProperty(CIMProperty(CIMName ("Namespaces"), namespaces));
    cimInstance3.addProperty(CIMProperty(CIMName ("ProviderType"),
        providerType));
    cimInstance3.addProperty(CIMProperty(CIMName ("SupportedMethods"),
        supportedMethods));
    cimInstance3.addProperty(CIMProperty(CIMName ("SupportedProperties"),
        supportedProperties));

    CIMObjectPath instanceName3 = cimInstance3.buildPath(cimClass3);

    instanceName3.setNameSpace(PEGASUS_NAMESPACENAME_INTEROP);
    instanceName3.setClassName(CLASSNAME3);

    returnRef3 =
        client.createInstance(PEGASUS_NAMESPACENAME_INTEROP, cimInstance3);

    // create CIMObjectPath
    Array<CIMKeyBinding> keys;
    CIMKeyBinding kb1(CIMName ("ProviderModuleName"), "providersModule1",
        CIMKeyBinding::STRING);
    CIMKeyBinding kb2(CIMName ("ProviderName"), "PG_ProviderInstance1",
        CIMKeyBinding::STRING);
    CIMKeyBinding kb3(CIMName ("CapabilityID"), "capability1",
        CIMKeyBinding::STRING);

    keys.append(kb1);
    keys.append(kb2);
    keys.append(kb3);

    instanceName3.setKeyBindings(keys);

    Array <String> supportedMethods2;
    Array <String> namespaces2;
    Array<CIMName> propertyList;

    // create new instance which will replace cimInstance3
    CIMInstance cimInstance4(CLASSNAME3);
    namespaces2.append("root/cimv4");
    namespaces2.append("root/cimv5");

    supportedMethods2.append("test_method2");
    supportedMethods2.append("test_method3");

    cimInstance4.addProperty(CIMProperty(CIMName ("Namespaces"), namespaces2));
    cimInstance4.addProperty(CIMProperty(CIMName ("SupportedMethods"),
        supportedMethods2));

    propertyList.append(CIMName ("Namespaces"));
    propertyList.append(CIMName ("SupportedMethods"));

    CIMInstance modifyedInstance(cimInstance4);
    modifyedInstance.setPath (instanceName3);

    client.modifyInstance(
        PEGASUS_NAMESPACENAME_INTEROP,
        modifyedInstance,
        false,
        propertyList);

    CIMKeyBinding kbm1(CIMName ("Name"), "providersModule1",
        CIMKeyBinding::STRING);
    Array<CIMKeyBinding> keyms;
    keyms.append(kbm1);

    instanceName.setKeyBindings(keyms);
    client.deleteInstance(PEGASUS_NAMESPACENAME_INTEROP, instanceName);
}

int main(int argc, char** argv)
{

    CIMClient client;

    try
    {
        client.connectLocal();
        TestModifyInstances(client);
    }

    catch(Exception& e)
    {
        PEGASUS_STD(cerr) << "Error: " << e.getMessage() << PEGASUS_STD(endl);
        PEGASUS_STD (cout) << "+++++ modify instances failed"
                           << PEGASUS_STD (endl);
        exit(-1);
    }

    PEGASUS_STD(cout) << "+++++ passed all tests" << PEGASUS_STD(endl);

    return 0;
}
