//%2003////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002  BMC Software, Hewlett-Packard Development
// Company, L. P., IBM Corp., The Open Group, Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L. P.;
// IBM Corp.; EMC Corporation, The Open Group.
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

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

int main(int argc, char** argv)
{
    const CIMNamespaceName NAMESPACE = CIMNamespaceName ("SDKExamples/DefaultCXX");
    const CIMName          CLASSNAME = CIMName ("Sample_InstanceProviderClass");

    try
    {
	Boolean 		deepInheritance = true;
	Boolean 		localOnly = true;
	Boolean 		includeQualifiers = false;
	Boolean 		includeClassOrigin = false;
	Array<CIMInstance> 	cimInstances;
	CIMClient 		client;

        //
        // The connectLocal Client API creates a connection to the server for
        // local clients. The connection is automatically authenticated
        // for the current user. The connect Client API, can be used to create
        // an HTTP connection with the server defined by the URL in address.
        // User name and Password information can be passed
        // using the connect Client API.
        //
	client.connectLocal();

        //
        // Enumerate Instances.
        //
        cimInstances = client.enumerateInstances( 
                                NAMESPACE,  
                                CLASSNAME, 
                                deepInheritance,
				localOnly,  
                                includeQualifiers,
			        includeClassOrigin );

        cout << "Total Number of Instances: " << cimInstances.size() << endl;
    }
    catch(Exception& e)
    {
	cerr << "Error: " << e.getMessage() << endl;
        exit(1);
    }

    return 0;
}
