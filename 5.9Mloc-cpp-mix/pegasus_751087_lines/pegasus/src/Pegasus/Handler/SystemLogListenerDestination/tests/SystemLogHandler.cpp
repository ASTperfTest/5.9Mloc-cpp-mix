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
#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Client/CIMClient.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;


// Uses interop namespace defined by PEGASUS_NAMESPACENAME_INTEROP in
// Constants.h

CIMObjectPath CreateHandler1Instance(CIMClient& client)
{
    CIMInstance handlerInstance(PEGASUS_CLASSNAME_LSTNRDST_SYSTEM_LOG);
    handlerInstance.addProperty (CIMProperty (CIMName
        ("SystemCreationClassName"), System::getSystemCreationClassName()));
    handlerInstance.addProperty (CIMProperty (CIMName ("SystemName"),
        System::getFullyQualifiedHostName()));
    handlerInstance.addProperty (CIMProperty (CIMName ("CreationClassName"),
        PEGASUS_CLASSNAME_LSTNRDST_SYSTEM_LOG.getString()));
    handlerInstance.addProperty(CIMProperty(CIMName ("Name"),
        String("Handler1")));

    CIMObjectPath Ref = client.createInstance(PEGASUS_NAMESPACENAME_INTEROP,
        handlerInstance);
    return Ref;
}

CIMObjectPath CreateFilterInstance(CIMClient& client,
    const String & query,
    const String & name)
{
    CIMInstance filterInstance(PEGASUS_CLASSNAME_INDFILTER);
    filterInstance.addProperty (CIMProperty (CIMName
        ("SystemCreationClassName"), System::getSystemCreationClassName()));
    filterInstance.addProperty (CIMProperty (CIMName ("SystemName"),
        System::getFullyQualifiedHostName()));
    filterInstance.addProperty(CIMProperty(CIMName ("CreationClassName"),
        PEGASUS_CLASSNAME_INDFILTER.getString()));
    filterInstance.addProperty(CIMProperty(CIMName ("Name"), name));
    filterInstance.addProperty (CIMProperty(CIMName ("Query"), query));
    filterInstance.addProperty (CIMProperty(CIMName ("QueryLanguage"),
        String("WQL")));
    filterInstance.addProperty (CIMProperty(CIMName ("SourceNamespace"),
        String("test/TestProvider")));

    CIMObjectPath Ref = client.createInstance(PEGASUS_NAMESPACENAME_INTEROP,
        filterInstance);
    return Ref;
}

CIMObjectPath CreateFormattedSubscriptionIns(CIMClient& client,
    const CIMObjectPath & handlerRef,
    const CIMObjectPath & filterRef,
    const String & textFormat,
    const Array<String> & textFormatParams)
{
    CIMInstance subscriptionInstance
        (PEGASUS_CLASSNAME_FORMATTEDINDSUBSCRIPTION);
    subscriptionInstance.addProperty(CIMProperty(CIMName ("Filter"),
        filterRef, 0, PEGASUS_CLASSNAME_INDFILTER));
    subscriptionInstance.addProperty(CIMProperty(CIMName ("Handler"),
        handlerRef, 0, PEGASUS_CLASSNAME_LSTNRDST_SYSTEM_LOG));
    subscriptionInstance.addProperty (CIMProperty
        (CIMName ("SubscriptionState"), CIMValue ((Uint16) 2)));
    subscriptionInstance.addProperty (CIMProperty
        (CIMName ("TextFormat"), textFormat));
    subscriptionInstance.addProperty (CIMProperty
        (CIMName ("TextFormatParameters"), textFormatParams));

    CIMObjectPath Ref = client.createInstance(
        PEGASUS_NAMESPACENAME_INTEROP, subscriptionInstance);
    return Ref;
}

void generateIndication(CIMClient& client)
{
    CIMInstance indicationInstance (CIMName("Test_IndicationProviderClass"));

    CIMObjectPath path;
    path.setNameSpace("test/TestProvider");
    path.setClassName("Test_IndicationProviderClass");

    indicationInstance.setPath(path);

    Array<CIMParamValue> inParams;
    Array<CIMParamValue> outParams;

    CIMValue ret_value = client.invokeMethod(
        "test/TestProvider",
        path,
        "SendTestIndication",
        inParams,
        outParams);
}

void DeleteInstance(CIMClient& client, const CIMObjectPath Ref)
{
    client.deleteInstance(PEGASUS_NAMESPACENAME_INTEROP, Ref);
}

int main(int argc, char** argv)
{
    CIMClient client;
    try
    {
        client.connectLocal();
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "Client connects local failed"
                           << PEGASUS_STD (endl);
        return -1;
    }

    CIMObjectPath Handler1Ref;
    CIMObjectPath Filter1Ref, Filter2Ref;
    CIMObjectPath Subscription1Ref, Subscription2Ref;

    try
    {
        Handler1Ref = CreateHandler1Instance (client);
    }
    catch (Exception& e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "create handler instance failed"
                           << PEGASUS_STD (endl);
        return -1;
    }

    PEGASUS_STD (cout) << "+++++ handler instances created"
                       << PEGASUS_STD (endl);
    try
    {
        String query="SELECT * FROM Test_IndicationProviderClass";
        String name1 = "TestFilter01";
        Filter1Ref = CreateFilterInstance (client, query, name1);
    }
    catch (Exception& e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "create filter instances failed"
                           << PEGASUS_STD (endl);
        return -1;
    }

    PEGASUS_STD (cout) << "+++++ filter instances created"
                       << PEGASUS_STD (endl);
    try
    {
        Array<String> textFormatParams;
        textFormatParams.append("IndicationTime");
        textFormatParams.append("IndicationIdentifier");

        // create a formatted subscription
        Subscription1Ref =
          CreateFormattedSubscriptionIns (client, Handler1Ref, Filter1Ref,
          "The indication occured at {0, datetime} with Indication ID {1}.",
          textFormatParams);

    }
    catch (Exception& e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "create subscription instance failed"
                           << PEGASUS_STD (endl);
        return -1;
    }

    PEGASUS_STD (cout) << "+++++ subscription instances created"
                       << PEGASUS_STD (endl);

    try
    {
        // generate indication

        generateIndication(client);
    }
    catch (Exception& e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "generate indication failed"
                           << PEGASUS_STD (endl);
        return -1;
    }

    PEGASUS_STD (cout) << "+++++ indications generated"
                       << PEGASUS_STD (endl);

    try
    {
        DeleteInstance (client, Subscription1Ref);
        DeleteInstance (client, Filter1Ref);
        DeleteInstance (client, Handler1Ref);
    }
    catch (Exception& e)
    {
        PEGASUS_STD (cerr) << "Exception: " << e.getMessage()
                           << PEGASUS_STD (endl);
        PEGASUS_STD (cerr) << "delete instance failed"
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ instances deleted"
                       << PEGASUS_STD (endl);

    PEGASUS_STD (cout) << "+++++ TestSyslogHandler passed all tests"
                       << PEGASUS_STD (endl);

}
