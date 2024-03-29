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

#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Common/Tracer.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/HandlerService/HandlerTable.h>
#include <Pegasus/Repository/CIMRepository.h>
#include <Pegasus/Config/ConfigManager.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

static CIMInstance CreateHandlerInstance()
{
    CIMInstance handlerInstance(PEGASUS_CLASSNAME_LSTNRDST_EMAIL);
    handlerInstance.addProperty (CIMProperty (CIMName
        ("SystemCreationClassName"), System::getSystemCreationClassName ()));
    handlerInstance.addProperty (CIMProperty (CIMName ("SystemName"),
        System::getFullyQualifiedHostName ()));
    handlerInstance.addProperty (CIMProperty (CIMName ("CreationClassName"),
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML.getString ()));
    handlerInstance.addProperty(CIMProperty(CIMName ("Name"),
        String("Handler1")));
    return (handlerInstance);
}

static void TestDestination(
    CIMHandler* handler,
    CIMInstance indicationHandlerInstance,
    CIMStatusCode statusCode)
{
    OperationContext context;
    CIMInstance indicationInstance;
    CIMInstance indicationSubscriptionInstance;
    ContentLanguageList contentLanguages;

    Boolean exceptionCaught;
    CIMException testException;

    exceptionCaught = false;
    try
    {
        handler->handleIndication(context,
            PEGASUS_NAMESPACENAME_INTEROP.getString(),
            indicationInstance,
            indicationHandlerInstance,
            indicationSubscriptionInstance,
            contentLanguages);
    }
    catch (CIMException& e)
    {
        exceptionCaught = true;
        testException = e;
    }
    PEGASUS_TEST_ASSERT(exceptionCaught &&
        testException.getCode() == statusCode);
}

static void TestDestinationExceptionHandling(CIMHandler* handler)
{
    CIMInstance indicationHandlerInstance;

    // Test "missing destination property" exception
    indicationHandlerInstance = CreateHandlerInstance();
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    // Test "invalid destination property type" exception
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), Uint32(10)));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    //destination       = "http" ["s"] ":" "//" authority ["/" pathSegment]
    //authority         = hostname [":" portnumber]

    // Test "missing colon" exception
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http//localhost")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    // Test "invalid scheme type" exception
    // A valid destination must begin with "http:" or "https:"
    // Processing of "localhost" destination is handled by the Handler Service.
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("invalid://localhost:1234")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("https2://localhost:1234")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    // Test "missing double slash" exception
    // The ":" must be followed by a "double slash"
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http:localhost")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http:/localhost:2345")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http:l//localhost:2345")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    // Test "invalid host name or port number"
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http://")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http://:6758")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http://localhost:1234EEEE")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);

    // Test "connection failure" exception
    indicationHandlerInstance = CreateHandlerInstance();
    indicationHandlerInstance.addProperty(CIMProperty(
        CIMName("destination"), String("http://localhost")));
    TestDestination(handler, indicationHandlerInstance, CIM_ERR_FAILED);
}


int main(int argc, char** argv)
{

bool enableTrace=false;

   if ((argc == 2) && !strcmp(argv[1], "trace"))
   {
      enableTrace=true;
   }
   const char* pegasusHome = getenv("PEGASUS_HOME");
   if (!pegasusHome)
   {
        cerr << "PEGASUS_HOME environment variable not set" << endl;
        exit(1);
    }
    String repositoryRoot = pegasusHome;
    repositoryRoot.append("/repository");
    CIMRepository* repository = new CIMRepository(repositoryRoot);

    ConfigManager::setPegasusHome(pegasusHome);

    if (enableTrace)
    {
        const char* tmpDir = getenv("PEGASUS_TMP");
        if (tmpDir == NULL)
        {
            tmpDir = ".";
        }
        String traceFile(tmpDir);
        traceFile.append("/TestCIMxmlIndicationHandler.trc");

        Tracer::setTraceFile(traceFile.getCString());
        Tracer::setTraceComponents("DiscardedData,IndHandler");
        Tracer::setTraceLevel(Tracer::LEVEL4);
    }

    try
    {
        HandlerTable handlerTable;
        String handlerId = "CIMxmlIndicationHandler";
        CIMHandler* handler = handlerTable.getHandler(handlerId, repository);
        PEGASUS_TEST_ASSERT(handler != 0);

        TestDestinationExceptionHandling(handler);

        delete repository;
    }
    catch(Exception& e)
    {
        PEGASUS_STD(cerr) << "Error: " << e.getMessage() << PEGASUS_STD(endl);
        exit(1);
    }

    cout << "+++++ passed all tests" << endl;
    return 0;
}
