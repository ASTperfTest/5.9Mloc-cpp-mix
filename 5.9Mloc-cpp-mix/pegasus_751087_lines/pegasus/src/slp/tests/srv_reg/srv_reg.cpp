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


#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/../slp/slp_agent/peg_slp_agent.h>
#if defined(PEGASUS_OS_TYPE_WINDOWS)
#else
# include <unistd.h>
#endif
#include <Pegasus/Common/FileSystem.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

/** Notes -

The LONG_ATTRIBUTE_STRING defined below represents a typical list of attributes
that a Pegasus service registration will contain.

This test program uses a statically constructed Pegasus:slp_service_agent
object to create a background thread that creates and services four SLP service
registrations.

After 30 seconds the program stops the background thread and destroys the
service agent.

The way to use the Pegasus::slp_service_agent class is as follows:

1) call slp_service_agent::srv_register once for each service advertisement
2) when all the services are registered, call slp_service_agent::start_listener
   This kicks off a background thread that maintains each registration
   indefinately. Everything is done automatically, including finding a DA or,
   if one is not found, initializing an SA. Re-registration to prevent
   expiration of the service registration is also automatically completed.

3) When it is time to stop advertising services, call
   slp_service_agent::unregister.


************/

#define LONG_ATTRIBUTE_STRING "(service-hi-name=Blip subsystem CIMOM),\n \
    (service-hi-description=The Blip subsystem is managed by a \
    Blip-Version-0.92 CIMOM.),\n \
    (service-id=9a783b42-12783401-78FB92D0-9E82B83AA),\n \
    (service-location-tcp=http://example.com:38294),\n \
    (CommunicationMechanism=cim-xml),\n (cim-xmlProtocolVersion=1.0),\n \
    (ProfilesSupported=Basic Read, Basic Write, Schema Manipulation),\n \
    (MultipleOperationsSupported=true),\n \
    (AuthenticationMechanismSupported = basic),\n \
    (namespace=root,interop,pegasus),\n \
    (classinfo=cim27,cim27,cim27),\n \
    (CIM_InteropSchemaNamespace=/root/PG_Interop)\n"

// after this number of seconds, unregister and terminate.
    Uint32 testTimer = 300;
static Boolean verbose;

// static construct/destruct of our service agent object

slp_service_agent slp_agent;


int main(int argc, char **argv)
{

    verbose = getenv("PEGASUS_TEST_VERBOSE") ? true : false;
    /* test_registration will return zero if all the parameters have a valid
       syntax. positive error codes indicate which specific parameter was not
       parsable.
    */

    char * registrationType = "service:wbem.pegasus";
    char * slpScope = "DEFAULT";
    try
    {
        if (argc > 1)
        {
            for (int files = 1 ; files < argc; files++)
            {
                Buffer slpRegInfo;
                String fileName(argv[files]);
                String attributeString;
                String url;

                if (verbose)
                {
                    cout << "File: " << fileName << endl;
                }

                FileSystem::loadFileToMemory(slpRegInfo, fileName);

                // First line is the url
                Uint32 i = 0;

                for (i = 0; i < slpRegInfo.size(); i++)
                {
                    Char16 ch_in(slpRegInfo[i]);
                    Char16 ch_nl('\n');
                    Char16 ch_cr('\r');

                    if (ch_in != ch_nl && ch_in != ch_cr)
                    {
                        url.append(slpRegInfo[i]);
                    }
                    else
                    {
                        break;
                    }
                }

                // Every thing after the first line is attributes

                for (; i < slpRegInfo.size(); i++)
                {
                    attributeString.append(slpRegInfo[i]);
                }

                if (verbose)
                {
                    cout << "Register: " << url << endl;
                    cout << "Attributes: " << attributeString << endl;
                }

                if (!slp_agent.srv_register((const char *)url.getCString(),
                    (const char *)attributeString.getCString(),
                    registrationType,
                    slpScope,
                    0xffff))
                {
                    cout << "Registration error." << endl;
                    exit(1);
                }
            }
        }
        else
        {

            char * url1 =
                "service:serviceid:98432A98-B879E8FF-80342A89-43280B89C";
            if (verbose)
            {
                cout << "Test registration with url : " << url1
                    <<  "\n Attributes=\n"
                    << LONG_ATTRIBUTE_STRING
                    << " registration type= " << registrationType << endl;
            }
            slp_agent.test_registration(url1,
                LONG_ATTRIBUTE_STRING,
                registrationType,
                slpScope);

            // register 4 services.

            // this first registration is the only one that complies to
            // the DMTF template

            if (verbose)
            {
                cout << "Register: " << url1 << endl;
            }

            if (!slp_agent.srv_register(
                "service:url:98432A98-B879E8FF-80342A89-43280B89C",
                LONG_ATTRIBUTE_STRING,
                registrationType,
                slpScope,
                0xffff))
            {
                cout << "Registration error." << endl;
                exit(1);
            }

            char * url2 = "service:wbem.ibm://localhost";

            if (verbose)
            {
                cout << "Register: " << url2 << endl;
            }

            if (!slp_agent.srv_register(url2,
                "(nothing=1),(version=2),(authentication=basic)",
                registrationType,
                slpScope,
                0xffff))
            {
                cout << "Registration error." << endl;
                exit(1);
            }

            char * url3 = "service:wbem.ibm://192.168.2.100";

            if (verbose)
            {
                cout << "Register: " << url3 << endl;
            }

            if (!slp_agent.srv_register(url3,
                "(nothing=1),(version=2),(authentication=basic)",
                registrationType,
                slpScope,
                0xffff))
            {
                cout << "Registration error." << endl;
                exit(1);
            }

            char * url4 = "service:wbem.ibm://mday&192.168.2.100:5588";

            if (verbose)
            {
                cout << "Register: " << url4 << endl;
            }

            if (!slp_agent.srv_register(url4,
                "(nothing=1),(version=2),(authentication=basic)",
                "service:wbem.pegasus",
                slpScope,
                0xffff))
            {
                cout << "Registration error." << endl;
                exit(1);
            }
        }
        // start the background thread - nothing is actually advertised
        // until this function returns.

        if (verbose)
        {
            cout << "Start Listener and listen for " << testTimer
                << " seconds.";
        }

        slp_agent.start_listener();

        Uint32 finish, now, msec;
        System::getCurrentTime(now, msec);
        finish = now + testTimer;

        // wait for 30 seconds.

        while (finish > now)
        {
            Threads::sleep(1000);
            System::getCurrentTime(now, msec);
        }

        if (verbose)
        {
            cout << "Terminate and unregister services" << endl;
        }

        // shut down the background thread and remove all the
        // registration data.

        slp_agent.unregister();
    }
    catch (Exception& e)
    {
        cout << "Exception: " << e.getMessage() << endl;
        exit(1);
    }

    cout << argv[0] << " +++++ passed all tests" << endl;
    return 0;
}




