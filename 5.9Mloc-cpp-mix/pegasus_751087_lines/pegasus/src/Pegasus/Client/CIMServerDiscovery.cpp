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

#include "CIMServerDiscovery.h"

#include <string.h> // for strdup

PEGASUS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////////////////////////////
// SLPClientOptions
// Filthy hack. User is responsible for adjusting values,
// and for new/delete of any struct used
/////////////////////////////////////////////////////////////////////////////

SLPClientOptions::SLPClientOptions()
{
    //initialise everything
    target_address  = NULL;
    local_interface  = NULL;
    target_port = 427;
    spi = NULL;
    scopes = strdup("DEFAULT");
    service_type = strdup("service:wbem");
    predicate = NULL;
    use_directory_agent = false;
}

SLPClientOptions::~SLPClientOptions()
{
    // free all pointers
    free(target_address);
    free(local_interface);
    free(spi);
    free(scopes);
    free(service_type);
    free(predicate);
}

void SLPClientOptions::print() const
{
    printf("target_address %s\n",target_address!=NULL?target_address:"NULL");
    printf("local_interface %s\n",local_interface!=NULL?local_interface:"NULL");
    printf("target_port %d\n",target_port);
    printf("spi %s\n",spi!=NULL?spi:"NULL");
    printf("scopes %s\n",scopes!=NULL?scopes:"NULL");
    printf("service_type %s\n",service_type!=NULL?service_type:"NULL");
    printf("predicate %s\n",predicate!=NULL?predicate:"NULL");
    printf("use_directory_agent %s\n",use_directory_agent==0?"false":"true");
}

PEGASUS_NAMESPACE_END

#ifdef PEGASUS_SLP_CLIENT_INTERFACE_WRAPPER
# include "PegasusSLPWrapper.cpp"
#elif OPENSLP_SLP_CLIENT_INTERFACE_WRAPPER
# include "OpenSLPWrapper.cpp"
#else

// No interface wrapper selected

PEGASUS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////////////////////////////
// CIMServerDiscoveryRep
/////////////////////////////////////////////////////////////////////////////

CIMServerDiscoveryRep::CIMServerDiscoveryRep()
{
}

CIMServerDiscoveryRep::~CIMServerDiscoveryRep()
{
}

Array<CIMServerDescription> CIMServerDiscoveryRep::lookup(
    const Array<Attribute>& criteria,
    const SLPClientOptions* options)
{
    Array<CIMServerDescription> connections;
    return connections;
}

PEGASUS_NAMESPACE_END
#endif

PEGASUS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////////////////////////////
// CIMServerDiscovery
/////////////////////////////////////////////////////////////////////////////
CIMServerDiscovery::CIMServerDiscovery()
    :_rep(new CIMServerDiscoveryRep())
{
}

CIMServerDiscovery::~CIMServerDiscovery()
{
    delete _rep;
}

Array<CIMServerDescription> CIMServerDiscovery::lookup(
    const SLPClientOptions* options)
{
    Array<Attribute> criteria;
    return _rep->lookup(criteria,options);
}

Array<CIMServerDescription> CIMServerDiscovery::lookup(
    const Array<Attribute>& criteria,
    const SLPClientOptions* options)
{
    return _rep->lookup(criteria,options);
}

PEGASUS_NAMESPACE_END
