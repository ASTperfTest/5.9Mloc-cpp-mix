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

#ifndef Pegasus_ClientRep_h
#define Pegasus_ClientRep_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/SSLContext.h>
#include <Pegasus/Common/CIMObject.h>
#include <Pegasus/Common/CIMClass.h>
#include <Pegasus/Common/CIMInstance.h>
#include <Pegasus/Common/CIMObjectPath.h>
#include <Pegasus/Common/CIMValue.h>
#include <Pegasus/Common/CIMParamValue.h>
#include <Pegasus/Common/CIMPropertyList.h>
#include <Pegasus/Common/CIMQualifierDecl.h>
#include <Pegasus/Common/Exception.h>
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Monitor.h>
#include <Pegasus/Common/HTTPConnector.h>
#include <Pegasus/Common/CIMMessage.h>
#include <Pegasus/Common/HTTPConnection.h>
#include <Pegasus/Common/XmlWriter.h>
#include <Pegasus/Common/TimeValue.h>
#include <Pegasus/Common/AutoPtr.h>
#include <Pegasus/Client/CIMClientException.h>
#include <Pegasus/Client/Linkage.h>
#include <Pegasus/Client/CIMClientInterface.h>
#include <Pegasus/Client/ClientPerfDataStore.h>

#include "CIMOperationResponseDecoder.h"
#include "CIMOperationRequestEncoder.h"


PEGASUS_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
//
// CIMClientRep
//
///////////////////////////////////////////////////////////////////////////////
class PEGASUS_CLIENT_LINKAGE CIMClientRep : public CIMClientInterface
{
public:

    // Timeout value defines the time the CIMClient will wait for a response
    // to an outstanding request.  If a request times out, the connection
    // gets reset (disconnected and reconnected).
    CIMClientRep(Uint32 timeoutMilliseconds =
                 PEGASUS_DEFAULT_CLIENT_TIMEOUT_MILLISECONDS);

    ~CIMClientRep();

    virtual void handleEnqueue();

    virtual Uint32 getTimeout() const
    {
        return _timeoutMilliseconds;
    }

    virtual void setTimeout(Uint32 timeoutMilliseconds)
    {
        _timeoutMilliseconds = timeoutMilliseconds;
        if ((_connected) && (_httpConnection != 0))
           _httpConnection->setSocketWriteTimeout(_timeoutMilliseconds/1000+1);
    }

    AcceptLanguageList getRequestAcceptLanguages() const;
    ContentLanguageList getRequestContentLanguages() const;
    ContentLanguageList getResponseContentLanguages() const;
    void setRequestAcceptLanguages(const AcceptLanguageList& langs);
    void setRequestContentLanguages(const ContentLanguageList& langs);
    void setRequestDefaultLanguages();

    void connect(
        const String& host,
        const Uint32 portNumber,
        const String& userName,
        const String& password
    );

    void connect(
        const String& host,
        const Uint32 portNumber,
        const SSLContext& sslContext,
        const String& userName,
        const String& password
    );

    void connectLocal();

    void disconnect();

    Boolean isConnected() const throw();

    virtual CIMClass getClass(
        const CIMNamespaceName& nameSpace,
        const CIMName& className,
        Boolean localOnly = true,
        Boolean includeQualifiers = true,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual CIMInstance getInstance(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& instanceName,
        Boolean localOnly = true,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual void deleteClass(
        const CIMNamespaceName& nameSpace,
        const CIMName& className
    );

    virtual void deleteInstance(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& instanceName
    );

    virtual void createClass(
        const CIMNamespaceName& nameSpace,
        const CIMClass& newClass
    );

    virtual CIMObjectPath createInstance(
        const CIMNamespaceName& nameSpace,
        const CIMInstance& newInstance
    );

    virtual void modifyClass(
        const CIMNamespaceName& nameSpace,
        const CIMClass& modifiedClass
    );

    virtual void modifyInstance(
        const CIMNamespaceName& nameSpace,
        const CIMInstance& modifiedInstance,
        Boolean includeQualifiers = true,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual Array<CIMClass> enumerateClasses(
        const CIMNamespaceName& nameSpace,
        const CIMName& className = CIMName(),
        Boolean deepInheritance = false,
        Boolean localOnly = true,
        Boolean includeQualifiers = true,
        Boolean includeClassOrigin = false
    );

    virtual Array<CIMName> enumerateClassNames(
        const CIMNamespaceName& nameSpace,
        const CIMName& className = CIMName(),
        Boolean deepInheritance = false
    );

    virtual Array<CIMInstance> enumerateInstances(
        const CIMNamespaceName& nameSpace,
        const CIMName& className,
        Boolean deepInheritance = true,
        Boolean localOnly = true,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual Array<CIMObjectPath> enumerateInstanceNames(
        const CIMNamespaceName& nameSpace,
        const CIMName& className
    );

    virtual Array<CIMObject> execQuery(
        const CIMNamespaceName& nameSpace,
        const String& queryLanguage,
        const String& query
    );

    virtual Array<CIMObject> associators(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& objectName,
        const CIMName& assocClass = CIMName(),
        const CIMName& resultClass = CIMName(),
        const String& role = String::EMPTY,
        const String& resultRole = String::EMPTY,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual Array<CIMObjectPath> associatorNames(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& objectName,
        const CIMName& assocClass = CIMName(),
        const CIMName& resultClass = CIMName(),
        const String& role = String::EMPTY,
        const String& resultRole = String::EMPTY
    );

    virtual Array<CIMObject> references(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& objectName,
        const CIMName& resultClass = CIMName(),
        const String& role = String::EMPTY,
        Boolean includeQualifiers = false,
        Boolean includeClassOrigin = false,
        const CIMPropertyList& propertyList = CIMPropertyList()
    );

    virtual Array<CIMObjectPath> referenceNames(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& objectName,
        const CIMName& resultClass = CIMName(),
        const String& role = String::EMPTY
    );

    virtual CIMValue getProperty(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& instanceName,
        const CIMName& propertyName
    );

    virtual void setProperty(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& instanceName,
        const CIMName& propertyName,
        const CIMValue& newValue = CIMValue()
    );

    virtual CIMQualifierDecl getQualifier(
        const CIMNamespaceName& nameSpace,
        const CIMName& qualifierName
    );

    virtual void setQualifier(
        const CIMNamespaceName& nameSpace,
        const CIMQualifierDecl& qualifierDeclaration
    );

    virtual void deleteQualifier(
        const CIMNamespaceName& nameSpace,
        const CIMName& qualifierName
    );

    virtual Array<CIMQualifierDecl> enumerateQualifiers(
        const CIMNamespaceName& nameSpace
    );

    virtual CIMValue invokeMethod(
        const CIMNamespaceName& nameSpace,
        const CIMObjectPath& instanceName,
        const CIMName& methodName,
        const Array<CIMParamValue>& inParameters,
        Array<CIMParamValue>& outParameters
    );

    void registerClientOpPerformanceDataHandler(
        ClientOpPerformanceDataHandler & handler);

    void deregisterClientOpPerformanceDataHandler();

private:

    void _connect();
    void _disconnect();

    Message* _doRequest(
        AutoPtr<CIMRequestMessage>& request,
        MessageType expectedResponseMessageType);

    AutoPtr<Monitor> _monitor;
    AutoPtr<HTTPConnector> _httpConnector;
    HTTPConnection* _httpConnection;

    Uint32 _timeoutMilliseconds;
    Boolean _connected;
    /**
        The CIMExportClient uses a lazy reconnect algorithm.  A reconnection
        is necessary when the server (listener) sends a Connection: Close
        header in the HTTP response or when a connection timeout occurs
        while waiting for a response.  In these cases, a disconnect is
        performed immediately and the _doReconnect flag is set.  The
        connection is re-established only when required to perform another
        operation.  Note that in the case of a connection timeout, the
        challenge status must be reset in the ClientAuthenticator to allow
        authentication to be performed properly on the new connection.
    */
    Boolean _doReconnect;

    AutoPtr<CIMOperationResponseDecoder> _responseDecoder;
    AutoPtr<CIMOperationRequestEncoder> _requestEncoder;
    ClientAuthenticator _authenticator;
    String _connectHost;
    Uint32 _connectPortNumber;
    AutoPtr<SSLContext> _connectSSLContext;
    ClientPerfDataStore perfDataStore;

    AcceptLanguageList requestAcceptLanguages;
    ContentLanguageList requestContentLanguages;
    ContentLanguageList responseContentLanguages;
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_ClientRep_h */