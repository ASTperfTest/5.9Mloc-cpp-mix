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
//=============================================================================
//
// Author: Barbara Packard (barbara_packard@hp.com)
//
// Modified By:    Adriano Zanuz (adriano.zanuz@hp.com)
//
//%////////////////////////////////////////////////////////////////////////////


#ifndef Pegasus_WMIInstanceProvider_h
#define Pegasus_WMIInstanceProvider_h

#include <WMIMapper/WMIProvider/WMIBaseProvider.h>

PEGASUS_NAMESPACE_BEGIN

class WMICollector;

class PEGASUS_WMIPROVIDER_LINKAGE WMIInstanceProvider : public WMIBaseProvider 
{
public:
    WMIInstanceProvider(void);
    virtual ~WMIInstanceProvider(void);

    // CIMInstanceProvider interface
    virtual CIMInstance getInstance(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMObjectPath& instanceName,
        Boolean localOnly = true,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList());

   virtual Array<CIMInstance> enumerateInstances(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const String& className,
        Boolean deepInheritance = true,
        Boolean localOnly = true,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList());

    virtual Array<CIMObjectPath> enumerateInstanceNames(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const String& className);

    virtual CIMValue getProperty(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMObjectPath& instanceName,
        const String& propertyName);

    virtual void modifyInstance(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMInstance& modifiedInstance,
        Boolean includeQualifiers = true,
        const CIMPropertyList& propertylist = CIMPropertyList());

    virtual CIMObjectPath createInstance(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMInstance& newInstance);

    virtual void deleteInstance(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMObjectPath& instanceName);

    virtual void setProperty(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMObjectPath& instanceName,
        const String& propertyName,
        const CIMValue& newValue = CIMValue());

private:
    
    String WMIInstanceProvider::getHostName();

//////////////////////////////////////////////////////////////////////
// TERRY: ADDED: helper function, maybe should go in a utilities or base class?
//////////////////////////////////////////////////////////////////////
    bool GetKeyBindingsFromCIMInstance(const CIMInstance& newInstance, 
                                       BSTR* pbsKeyBindings);

};

PEGASUS_NAMESPACE_END

#endif
