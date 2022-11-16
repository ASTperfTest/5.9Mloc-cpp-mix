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
// Author: Amit K Arora, IBM (amitarora@in.ibm.com)  - PEP 193
//
// Modified By: Amit K Arora, IBM (amitarora@in.ibm.com) - Bug#2462,#2491
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_CIMQueryCapabilitiesProvider_h
#define Pegasus_CIMQueryCapabilitiesProvider_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/Exception.h>
#include <Pegasus/ControlProviders/QueryCapabilitiesProvider/Linkage.h>
#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <iostream>

#define PROPERTY_NAME_ELEMENT_NAME  "ElementName"
#define PROPERTY_NAME_DESCRIPTION   "Description"
#define PROPERTY_NAME_CAPTION       "Caption"
#define PROPERTY_NAME_INSTANCEID    "InstanceID"
#define PROPERTY_NAME_CQLFEATURES   "CQLFeatures"

#define INSTANCEID_VALUE "CIM:PegasusQueryCapabilities"
#define NUM_KEY_PROPERTIES  1

PEGASUS_NAMESPACE_BEGIN

class PEGASUS_CIMQUERYCAPPROVIDER_LINKAGE CIMQueryCapabilitiesProvider :
    public CIMInstanceProvider
{
public:
    CIMQueryCapabilitiesProvider();
    virtual ~CIMQueryCapabilitiesProvider();

    // CIMProvider interface
    // Note:  The initialize() and terminate() methods are not called for
    // Control Providers.
    virtual void initialize(CIMOMHandle& cimom) { }
    virtual void terminate() { }

    // CIMQueryCapabilitiesProvider interface
    virtual void getInstance(
        const OperationContext & context,
        const CIMObjectPath & ref,
                const Boolean includeQualifiers,
                const Boolean includeClassOrigin,
                const CIMPropertyList & propertyList,
        InstanceResponseHandler & handler);

    virtual void enumerateInstances(
        const OperationContext & context,
        const CIMObjectPath & ref,
                const Boolean includeQualifiers,
                const Boolean includeClassOrigin,
                const CIMPropertyList & propertyList,
        InstanceResponseHandler & handler);

    virtual void enumerateInstanceNames(
        const OperationContext & context,
        const CIMObjectPath & ref,
        ObjectPathResponseHandler & handler);

        virtual void modifyInstance(
                const OperationContext & context,
                const CIMObjectPath & ref,
                const CIMInstance & obj,
                const Boolean includeQualifiers,
                const CIMPropertyList & propertyList,
                ResponseHandler & handler);

        virtual void createInstance(
                const OperationContext & context,
                const CIMObjectPath & ref,
                const CIMInstance & obj,
                ObjectPathResponseHandler & handler);

        virtual void deleteInstance(
                const OperationContext & context,
                const CIMObjectPath & ref,
                ResponseHandler & handler);

        CIMInstance buildInstance(CIMObjectPath cimRef);

        static const Uint16 FeatureSet[];
        static const int NUM_QUERY_CAPABILITIES;

        static void getFeatureSet(Array<Uint16>& features)
        {
          for(int i=0; i<NUM_QUERY_CAPABILITIES; i++)
          {
            features[i] = FeatureSet[i];
          }
        }


   protected:
      CIMOMHandle _cimom;

};


PEGASUS_NAMESPACE_END

#endif
