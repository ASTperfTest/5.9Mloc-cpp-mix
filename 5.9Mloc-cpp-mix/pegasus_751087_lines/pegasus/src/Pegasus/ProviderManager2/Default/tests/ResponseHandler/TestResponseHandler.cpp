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
#include <Pegasus/Common/CIMInstance.h>

#include <Pegasus/Common/ResponseHandler.h>
#include <Pegasus/ProviderManager2/SimpleResponseHandler.h>

PEGASUS_USING_PEGASUS;

#include <iostream>

PEGASUS_USING_STD;

int main(int argc, char** argv)
{

    // instantiate the primary response handler types
    {
        SimpleResponseHandler concreteHandler;
        ResponseHandler& handler = concreteHandler;

        handler.processing();
        handler.complete();
    }

    {
        SimpleObjectResponseHandler handler = SimpleObjectResponseHandler();

        handler.processing();
        handler.deliver(CIMObject());
        handler.complete();
    }

    {
        SimpleInstanceResponseHandler handler = SimpleInstanceResponseHandler();

        handler.processing();
        handler.deliver(CIMInstance());
        handler.complete();
    }

    {
        SimpleObjectPathResponseHandler handler =
            SimpleObjectPathResponseHandler();

        handler.processing();
        handler.deliver(CIMObjectPath());
        handler.complete();
    }

    {
        SimpleMethodResultResponseHandler handler =
            SimpleMethodResultResponseHandler();

        handler.processing();
        handler.deliverParamValue(CIMParamValue("param1", String("p1")));
        handler.deliverParamValue(CIMParamValue("param2", String("p2")));
        handler.deliver(CIMValue());
        handler.complete();
    }

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}
