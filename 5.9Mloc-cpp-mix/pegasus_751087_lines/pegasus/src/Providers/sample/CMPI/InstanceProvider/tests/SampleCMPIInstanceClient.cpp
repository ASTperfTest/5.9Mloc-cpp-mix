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
#include <Pegasus/Client/CIMClient.h>
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <Pegasus/Common/CIMType.h>
#include <Pegasus/Common/PegasusAssert.h>


PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;


const CIMNamespaceName providerNamespace =
          CIMNamespaceName ("root/SampleProvider");
const CIMName CLASSNAME = CIMName ("CMPISample_InstanceProviderClass");

static Boolean verbose;
#define QUERIES 7
const char *queries[] = {"SELECT * FROM CMPISample_InstanceProviderClass",
    "SELECT * FROM CMPISample_InstanceProviderClass where Identifier<3",
    "select Identifier FROM CMPISample_InstanceProviderClass",
    "select Message FROM CMPISample_InstanceProviderClass",
    "select Identifier,Message FROM CMPISample_InstanceProviderClass",
    "select * from Sample_InstanceQueryProviderClass "
        " where Identifier=2 or Identifier=3",
    "select * from Sample_InstanceQueryProviderClass "
        "where Identifier=2 or Message=\"Hello world\"" };

void test01(CIMClient &client)
{
    CIMInstance cimInstance(CLASSNAME);
    CIMObjectPath instanceName;
    CIMObjectPath returnRef;
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append(
        CIMKeyBinding(CIMName("Identifier"), CIMValue(4)));

    //set the classname, provider name and keybindings to object-path
    instanceName.setNameSpace (providerNamespace);
    instanceName.setClassName (CLASSNAME);
    instanceName.setKeyBindings (keyBindings);

    //sets the object path for the instance.
    cimInstance.setPath(instanceName);

    //add properties to the instance.
    cimInstance.addProperty(CIMProperty(CIMName ("Identifier"),
        Uint8(4)));
    cimInstance.addProperty(CIMProperty(CIMName ("Message"),
        String("Hey John")));


    returnRef = client.createInstance(providerNamespace, cimInstance);

    CIMInstance cimInstance1(CLASSNAME);
    cimInstance1.setPath(instanceName);
    cimInstance1.addProperty(CIMProperty(CIMName ("Message"),
        String("Hey Mike")));
    Array<CIMName> propertyList;
    propertyList.append(CIMName ("Message"));
    client.modifyInstance(
        providerNamespace,
        cimInstance1,
        false,
        propertyList);
    client.deleteInstance(providerNamespace, instanceName);
}

#ifdef PEGASUS_ENABLE_EXECQUERY
void test02(CIMClient & client)
{
    Uint32 pos;
    CIMProperty theProperty;
    CIMValue theValue;
    String wql ("WQL");

    if (verbose)
    {
            cerr << "Querying " << queries << endl;
    }

    //evaluate each query from "queries" array
    for(Uint32 j =0 ; j < QUERIES; j++)
    {
        Array <CIMObject> objects = client.execQuery (
            providerNamespace,
            wql,
            queries[j]);
        if (objects.size () == 0)
        {
            if (verbose)
            {
                cerr << " No instance returned.. That is good" << endl;
            }
        }
        //for the recieved objects assert whether,
        //right results have been recieved
        for (Uint32 i = 0; i < objects.size (); i++)
        {
            if (objects[i].isInstance ())
            {
                CIMInstance instance (objects[i]);
                pos = instance.findProperty ("Identifier");
                if (pos != PEG_NOT_FOUND)
                {
                    theProperty = instance.getProperty (pos);
                    theValue = theProperty.getValue ();
                    PEGASUS_TEST_ASSERT (theValue.getType() == CIMTYPE_UINT8);
                }
                pos = instance.findProperty ("Message");
                if (pos != PEG_NOT_FOUND)
                {
                    theProperty = instance.getProperty (pos);
                    theValue = theProperty.getValue ();
                    PEGASUS_TEST_ASSERT (theValue.getType() == CIMTYPE_STRING);
                }
            }
        }
    }//QUERIES

}
#endif

int main(int argc, char** argv)
{
    verbose = (getenv ("PEGASUS_TEST_VERBOSE")) ? true : false;
    CIMClient client;
    try
    {
        client.connectLocal ();
    }
    catch (Exception & e)
    {
        cerr << e.getMessage () << endl;
        return -1;
    }

    try
    {
        test01(client);
#ifdef PEGASUS_ENABLE_EXECQUERY
        test02(client);
#endif
    }

    catch(Exception& e)
    {
        client.disconnect();
        PEGASUS_STD(cerr) << "Error: " << e.getMessage() << PEGASUS_STD(endl);
        PEGASUS_STD (cout) << argv[0] << " +++++  failed"
                           << PEGASUS_STD (endl);
        exit(-1);
    }

    PEGASUS_STD(cout) << argv[0] << " +++++ passed all tests"
                      << PEGASUS_STD(endl);

    return 0;
}


