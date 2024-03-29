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
// Author: Chip Vincent (cvincent@us.ibm.com)
//
// Modified By: David Dillard, VERITAS Software Corp.
//                  (david.dillard@veritas.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Client/CIMClient.h>
#include <Pegasus/Common/XmlWriter.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

static Boolean verbose;

static const String NAMESPACE("test/TestProvider");
static const String CLASSNAME("TST_Instance1");

Boolean isValid(const CIMObjectPath & cimObjectPath)
{
    if(!String::equalNoCase(cimObjectPath.getClassName().getString(), CLASSNAME))
    {
        return(false);
    }

    if(cimObjectPath.getKeyBindings().size() == 0)  // singleton?
    {
        return(false);
    }

    return(true);
}

// instance name normalization
void Test1(CIMClient& client)
{
    if(verbose)
    {
        cout << "Test1()" << endl;
    }

    {
        Array<CIMObjectPath> cimInstanceNames =
            client.enumerateInstanceNames(
                NAMESPACE,
                CLASSNAME);

        for(Uint32 i = 0, n = cimInstanceNames.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstanceNames[i]) ? "GOOD" : "BAD") << ": " << cimInstanceNames[i].toString() << endl;
            }
        }
    }
}

// get instance normalization (with request permutations)
void Test2(CIMClient& client)
{
    if(verbose)
    {
        cout << "Test2()" << endl;
    }

    Array<CIMObjectPath> cimInstanceNames =
        client.enumerateInstanceNames(
            NAMESPACE,
            CLASSNAME);

    {
        Boolean localOnly = false;
        Boolean includeQualifiers = false;
        Boolean includeClassOrigin = false;

        if(verbose)
        {
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        for(Uint32 i = 0, n = cimInstanceNames.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstanceNames[i]) ? "GOOD" : "BAD") << ": " << cimInstanceNames[i].toString() << endl;
            }

            if(isValid(cimInstanceNames[i]))
            {
                CIMInstance cimInstance;

                try
                {
                    cimInstance = client.getInstance(
                        NAMESPACE,
                        cimInstanceNames[i],
                        localOnly,
                        includeQualifiers,
                        includeClassOrigin,
                        CIMPropertyList());

                    if(verbose)
                    {
                        XmlWriter::printInstanceElement(cimInstance);
                    }
                }
                catch(CIMException & e)
                {
                    if(verbose)
                    {
                        cout << "CIMException(" << e.getCode() << "): " << e.getMessage() << endl;
                    }
                }
            }
            else
            {
                if(verbose)
                {
                    cout << "skipping getInstance() because of bad class name (tolerated for now)." << endl << endl;
                }
            }
        }
    }

    {
        Boolean localOnly = false;
        Boolean includeQualifiers = true;
        Boolean includeClassOrigin = false;

        if(verbose)
        {
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        for(Uint32 i = 0, n = cimInstanceNames.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstanceNames[i]) ? "GOOD" : "BAD") << ": " << cimInstanceNames[i].toString() << endl;
            }

            if(isValid(cimInstanceNames[i]))
            {
                try
                {
                    CIMInstance cimInstance =
                        client.getInstance(
                            NAMESPACE,
                            cimInstanceNames[i],
                            localOnly,
                            includeQualifiers,
                            includeClassOrigin,
                            CIMPropertyList());

                    if(verbose)
                    {
                        XmlWriter::printInstanceElement(cimInstance);
                    }
                }
                catch(CIMException & e)
                {
                    if(verbose)
                    {
                        cout << "CIMException(" << e.getCode() << "): " << e.getMessage() << endl;
                    }
                }
            }
            else
            {
                if(verbose)
                {
                    cout << "skipping getInstance() because of bad class name (tolerated for now)." << endl << endl;
                }
            }
        }
    }

    {
        Boolean localOnly = false;
        Boolean includeQualifiers = true;
        Boolean includeClassOrigin = true;

        if(verbose)
        {
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        for(Uint32 i = 0, n = cimInstanceNames.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstanceNames[i]) ? "GOOD" : "BAD") << ": " << cimInstanceNames[i].toString() << endl;
            }

            if(isValid(cimInstanceNames[i]))
            {
                try
                {
                    CIMInstance cimInstance =
                        client.getInstance(
                            NAMESPACE,
                            cimInstanceNames[i],
                            localOnly,
                            includeQualifiers,
                            includeClassOrigin,
                            CIMPropertyList());

                    if(verbose)
                    {
                        XmlWriter::printInstanceElement(cimInstance);
                    }
                }
                catch(CIMException & e)
                {
                    if(verbose)
                    {
                        cout << "CIMException(" << e.getCode() << "): " << e.getMessage() << endl;
                    }
                }
            }
            else
            {
                if(verbose)
                {
                    cout << "skipping getInstance() because of bad class name (tolerated for now)." << endl << endl;
                }
            }
        }
    }
}

// enumerate instances normalization (with request permutations)
void Test3(CIMClient& client)
{
    if(verbose)
    {
        cout << "Test3()" << endl;
    }

    {
        Boolean deepInheritance = false;
        Boolean localOnly = false;
        Boolean includeQualifiers = false;
        Boolean includeClassOrigin = false;

        if(verbose)
        {
            cout << "deepInheritance = " << (deepInheritance ? "true" : "false") << endl;
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        Array<CIMInstance> cimInstances =
            client.enumerateInstances(
                NAMESPACE,
                CLASSNAME,
                deepInheritance,
                localOnly,
                includeQualifiers,
                includeClassOrigin,
                CIMPropertyList());

        for(Uint32 i = 0, n = cimInstances.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstances[i].getPath()) ? "GOOD" : "BAD") << ": " << cimInstances[i].getPath().toString() << endl;

                XmlWriter::printInstanceElement(cimInstances[i]);
            }
        }
    }

    {
        Boolean deepInheritance = true;
        Boolean localOnly = false;
        Boolean includeQualifiers = false;
        Boolean includeClassOrigin = false;

        if(verbose)
        {
            cout << "deepInheritance = " << (deepInheritance ? "true" : "false") << endl;
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        Array<CIMInstance> cimInstances =
            client.enumerateInstances(
                NAMESPACE,
                CLASSNAME,
                deepInheritance,
                localOnly,
                includeQualifiers,
                includeClassOrigin,
                CIMPropertyList());

        for(Uint32 i = 0, n = cimInstances.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstances[i].getPath()) ? "GOOD" : "BAD") << ": " << cimInstances[i].getPath().toString() << endl;

                XmlWriter::printInstanceElement(cimInstances[i]);
            }
        }
    }

    {
        Boolean deepInheritance = true;
        Boolean localOnly = false;
        Boolean includeQualifiers = true;
        Boolean includeClassOrigin = false;

        if(verbose)
        {
            cout << "deepInheritance = " << (deepInheritance ? "true" : "false") << endl;
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        Array<CIMInstance> cimInstances =
            client.enumerateInstances(
                NAMESPACE,
                CLASSNAME,
                deepInheritance,
                localOnly,
                includeQualifiers,
                includeClassOrigin,
                CIMPropertyList());

        for(Uint32 i = 0, n = cimInstances.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstances[i].getPath()) ? "GOOD" : "BAD") << ": " << cimInstances[i].getPath().toString() << endl;

                XmlWriter::printInstanceElement(cimInstances[i]);
            }
        }
    }

    {
        Boolean deepInheritance = true;
        Boolean localOnly = false;
        Boolean includeQualifiers = true;
        Boolean includeClassOrigin = true;

        if(verbose)
        {
            cout << "deepInheritance = " << (deepInheritance ? "true" : "false") << endl;
            cout << "localOnly = " << (localOnly ? "true" : "false") << endl;
            cout << "includeQualifiers = " << (includeQualifiers ? "true" : "false") << endl;
            cout << "includeClassOrigin = " << (includeClassOrigin ? "true" : "false") << endl;
        }

        Array<CIMInstance> cimInstances =
            client.enumerateInstances(
                NAMESPACE,
                CLASSNAME,
                deepInheritance,
                localOnly,
                includeQualifiers,
                includeClassOrigin,
                CIMPropertyList());

        for(Uint32 i = 0, n = cimInstances.size(); i < n; i++)
        {
            if(verbose)
            {
                cout << (isValid(cimInstances[i].getPath()) ? "GOOD" : "BAD") << ": " << cimInstances[i].getPath().toString() << endl;

                XmlWriter::printInstanceElement(cimInstances[i]);
            }
        }
    }
}

int main(int argc, char** argv)
{
    verbose = getenv("PEGASUS_TEST_VERBOSE") ? true : false;

    CIMClient client;

    try
    {
        client.connectLocal();
        Test1(client);
        Test2(client);
        Test3(client);
    }
    catch(const CIMException & e)
    {
        client.disconnect();

        cout << "CIMException: " << e.getCode() << " " << e.getMessage() << endl;

        return(1);
    }
    catch(const Exception & e)
    {
        client.disconnect();

        cout << "Exception: " << e.getMessage() << endl;

        return(1);
    }
    catch(...)
    {
        client.disconnect();

        cout << "unknown exception" << endl;

        return(1);
    }

    client.disconnect();

    cout << argv[0] << " +++++ passed all tests" << endl;

    return(0);
}
