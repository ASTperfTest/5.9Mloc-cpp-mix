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
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_ProviderRegistrationProvider_h
#define Pegasus_ProviderRegistrationProvider_h
#include <Pegasus/Common/Config.h>
#include <Pegasus/ControlProviders/ProviderRegistrationProvider/Linkage.h>

#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <Pegasus/Provider/CIMMethodProvider.h>
#include <Pegasus/Common/ModuleController.h>

#include <Pegasus/Common/AcceptLanguageList.h>

#include \
    <Pegasus/Server/ProviderRegistrationManager/ProviderRegistrationManager.h>

PEGASUS_NAMESPACE_BEGIN

class PEGASUS_PROVREGPROVIDER_LINKAGE ProviderRegistrationProvider :
    public CIMInstanceProvider,
    public CIMMethodProvider
{
public:

      class callback_data
      {
         public:

            Message *reply;
            Semaphore client_sem;
            ProviderRegistrationProvider & cimom_handle;

            callback_data(ProviderRegistrationProvider *handle)
               : reply(0), client_sem(0), cimom_handle(*handle)
            {
            }
            ~callback_data()
            {
               delete reply;
            }

            Message *get_reply(void)
            {
               Message *ret = reply;
               reply = NULL;
               return ret;
            }

         private:
            callback_data(void);
      };


    ProviderRegistrationProvider(
        ProviderRegistrationManager* providerRegistrationManager);
    virtual ~ProviderRegistrationProvider(void);

ProviderRegistrationProvider & operator=
    (const ProviderRegistrationProvider & handle);

    // CIMProvider interface
    // Note:  The initialize() and terminate() methods are not called for
    // Control Providers.
    virtual void initialize(CIMOMHandle& cimom) { }
    virtual void terminate() { }

    // CIMInstanceProvider interface
    virtual void getInstance(
        const OperationContext & context,
        const CIMObjectPath & instanceReference,
        const Boolean includeQualifiers,
        const Boolean includeClassOrigin,
        const CIMPropertyList & propertyList,
        InstanceResponseHandler & handler);

    virtual void enumerateInstances(
        const OperationContext & context,
        const CIMObjectPath & classReference,
        const Boolean includeQualifiers,
        const Boolean includeClassOrigin,
        const CIMPropertyList & propertyList,
        InstanceResponseHandler & handler);

    virtual void enumerateInstanceNames(
        const OperationContext & context,
        const CIMObjectPath & classReference,
        ObjectPathResponseHandler & handler);

    virtual void modifyInstance(
        const OperationContext & context,
        const CIMObjectPath & instanceReference,
        const CIMInstance & instanceObject,
        const Boolean includeQualifiers,
        const CIMPropertyList & propertyList,
        ResponseHandler & handler);

    virtual void createInstance(
        const OperationContext & context,
        const CIMObjectPath & instanceReference,
        const CIMInstance & instanceObject,
        ObjectPathResponseHandler & handler);

    virtual void deleteInstance(
        const OperationContext & context,
        const CIMObjectPath & instanceReference,
        ResponseHandler & handler);

    // CIMMethodProvider interface
    virtual void invokeMethod(
        const OperationContext & context,
        const CIMObjectPath & objectReference,
        const CIMName & methodName,
        const Array<CIMParamValue> & inParameters,
        MethodResultResponseHandler & handler);

protected:

    ModuleController * _controller;

    ProviderRegistrationManager* _providerRegistrationManager;

    MessageQueueService * _getProviderManagerService();
    Array<Uint16> _sendDisableMessageToProviderManager(
        CIMDisableModuleRequestMessage * notify_req);
    Array<Uint16> _sendEnableMessageToProviderManager(
        CIMEnableModuleRequestMessage * notify_req);
    MessageQueueService * _getIndicationService();
    void _sendTerminationMessageToSubscription(
        const CIMObjectPath & ref, const String & moduleName,
        const Boolean disableProviderOnly,
        const AcceptLanguageList & al);

    Sint16 _disableModule(const CIMObjectPath & moduleRef, 
                          const String & moduleName,
                          Boolean disableProviderOnly,
                      const AcceptLanguageList & al);

    //
    // If the provider is an indication provider, return true
    // otherwise, return false
    //
    Boolean _isIndicationProvider(const String & moduleName,
                                  const CIMInstance & instance);

    Sint16 _enableModule(const CIMObjectPath & moduleRef, 
                         const String & moduleName,
                         const AcceptLanguageList & al); 

    void _sendEnableMessageToSubscription(
        const CIMInstance & mInstance,
        const CIMInstance & pInstance,
        const Array<CIMInstance> & capInstances,
        const AcceptLanguageList & al);

     //
    // get all the indication capability instances which belongs
    // to the provider
    //
    Array<CIMInstance>  _getIndicationCapInstances(
        const String & moduleName,
        const CIMInstance & instance,
        const CIMObjectPath & providerRef);

};

PEGASUS_NAMESPACE_END

#endif
