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
// Author: Heather Sterling (hsterl@us.ibm.com)
//
// Modified By: 
//
//%/////////////////////////////////////////////////////////////////////////////

#include "DynamicListener.h"
#include "ListenerService.h"
#include "ConsumerManager.h"

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Exception.h>
#include <Pegasus/Common/SSLContext.h>
#include <Pegasus/Common/Monitor.h>
#include <Pegasus/Common/HTTPAcceptor.h>
#include <Pegasus/Common/PegasusVersion.h>
#include <Pegasus/Common/Tracer.h>

#include <Pegasus/Common/HashTable.h>
#include <Pegasus/Common/FileSystem.h>

#include <Pegasus/ExportServer/CIMExportResponseEncoder.h>
#include <Pegasus/ExportServer/CIMExportRequestDecoder.h>

PEGASUS_NAMESPACE_BEGIN
PEGASUS_USING_STD;


const Boolean DynamicListener::DEFAULT_CONSUMER_UNLOAD = true;
const Uint32 DynamicListener::DEFAULT_IDLE_TIMEOUT = 300000; //ms
const Boolean DynamicListener::DEFAULT_FORCE_SHUTDOWN = false;
const Uint32 DynamicListener::DEFAULT_SHUTDOWN_TIMEOUT = 10000; //ms


/////////////////////////////////////////////////////////////////////////////
// DynamicListenerRep
/////////////////////////////////////////////////////////////////////////////

//This class is used to stabilize the DynamicListener external interface.

class DynamicListenerRep
{
public:

    DynamicListenerRep(Uint32 portNumber, 
                       const String& consumerDir, 
                       const String& consumerConfigDir,
                       SSLContext* sslContext,
                       Boolean enableConsumerUnload, 
                       Uint32 consumerIdleTimeout, 
                       Uint32 shutdownTimeout);

    ~DynamicListenerRep();

    void start();

    void stop(Boolean forceShutdown);

    Boolean isAlive();

    Boolean addConsumer(const String& consumerName, const String& location = String::EMPTY);

    Boolean removeConsumer(const String& consumerName);

    Uint32 getPortNumber();

    String getConsumerDir();

    String getConsumerConfigDir();

    void setEnableConsumerUnload(Boolean consumerUnload);

    Boolean getEnableConsumerUnload();

    void setIdleTimeout(Uint32 idleTimeout);

    Uint32 getIdleTimeout(); 

private:

    // core components
    ListenerService* _listenerService; 
    ConsumerManager* _consumerManager;

    // config properties -- do we want to separate these out????
    Uint32 _port;
    SSLContext* _sslContext;
};

DynamicListenerRep::DynamicListenerRep(Uint32 portNumber, 
                                       const String& consumerDir,           //consumer mgr  
                                       const String& consumerConfigDir,     //consumer mgr
                                       SSLContext* sslContext,              //listener svc
                                       Boolean enableConsumerUnload,        //consumer mgr
                                       Uint32 consumerIdleTimeout,          //consumer mgr
                                       Uint32 shutdownTimeout) :            //???
_port(portNumber),
_sslContext(sslContext)             
{
    PEG_METHOD_ENTER(TRC_LISTENER, "DynamicListenerRep::DynamicListenerRep");

    _consumerManager = new ConsumerManager(consumerDir, consumerConfigDir, enableConsumerUnload, consumerIdleTimeout);

    _listenerService = new ListenerService(_consumerManager);

    PEG_METHOD_EXIT();
}

DynamicListenerRep::~DynamicListenerRep()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "DynamicListenerRep::~DynamicListenerRep");

    delete _consumerManager;

    delete _listenerService;

    PEG_METHOD_EXIT();
}

void DynamicListenerRep::start()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "DynamicListenerRep::start");

    if (_sslContext)
    {
        _listenerService->initializeListener(_port, true, _sslContext);

    } else
    {
        _listenerService->initializeListener(_port, false, 0);
    }

    _listenerService->runListener();

    PEG_METHOD_EXIT();  
}

void DynamicListenerRep::stop(Boolean forceShutdown)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "DynamicListenerRep::stop");

    _listenerService->shutdownListener();

    PEG_METHOD_EXIT();
}

Boolean DynamicListenerRep::isAlive()
{
    return _listenerService->isAlive();
}

//TODO: 
Boolean DynamicListenerRep::addConsumer(const String& consumerName, const String& location)
{
    return true;
    //return _consumerManager->addConsumer(consumerName, location);
}

//TODO:
Boolean DynamicListenerRep::removeConsumer(const String& consumerName)
{
    return true;
    //return _consumerManager->unloadConsumer(consumerName);
}

Uint32 DynamicListenerRep::getPortNumber() 
{
    return _listenerService->getPortNumber();
}

String DynamicListenerRep::getConsumerDir()
{
    return _consumerManager->getConsumerDir();
}

String DynamicListenerRep::getConsumerConfigDir()
{
    return _consumerManager->getConsumerConfigDir();
}

Boolean DynamicListenerRep::getEnableConsumerUnload()
{
    return _consumerManager->getEnableConsumerUnload();
}

void DynamicListenerRep::setEnableConsumerUnload(Boolean enableConsumerUnload)
{
//do nothing for now
}

Uint32 DynamicListenerRep::getIdleTimeout()
{
    return _consumerManager->getIdleTimeout();
}

void DynamicListenerRep::setIdleTimeout(Uint32 idleTimeout)
{
//do nothing for now
}


/////////////////////////////////////////////////////////////////////////////
// DynamicListener
/////////////////////////////////////////////////////////////////////////////

DynamicListener::DynamicListener(Uint32 portNumber, 
                                 const String& consumerDir, 
                                 const String& consumerConfigDir,
                                 Boolean enableConsumerUnload, 
                                 Uint32 consumerIdleTimeout, 
                                 Uint32 shutdownTimeout) 
{
    _rep = new DynamicListenerRep(portNumber, 
                                  consumerDir,           
                                  consumerConfigDir,     
                                  0,             
                                  enableConsumerUnload,   
                                  consumerIdleTimeout,  
                                  shutdownTimeout);
}

#ifdef PEGASUS_HAS_SSL
DynamicListener::DynamicListener(Uint32 portNumber, 
                                 const String& consumerDir,
                                 const String& consumerConfigDir,
                                 Boolean useSSL, 
                                 const String& keyPath, 
                                 const String& certPath,
                                 Boolean enableConsumerUnload, 
                                 Uint32 consumerIdleTimeout, 
                                 Uint32 shutdownTimeout)     //ONLY IF PEGASUS_HAS_SSL
{
    SSLContext* sslContext = 0;
    if (useSSL)
    {
        sslContext = new SSLContext(String::EMPTY, certPath, keyPath, 0, String::EMPTY);//randFile);
    }

    _rep = new DynamicListenerRep(portNumber, 
                                  consumerDir,           
                                  consumerConfigDir,     
                                  sslContext,             
                                  enableConsumerUnload,   
                                  consumerIdleTimeout,  
                                  shutdownTimeout);
}

DynamicListener::DynamicListener(Uint32 portNumber, 
                                 const String& consumerDir,
                                 const String& consumerConfigDir,
                                 Boolean useSSL, 
                                 SSLContext* sslContext,
                                 Boolean enableConsumerUnload, 
                                 Uint32 consumerIdleTimeout, 
                                 Uint32 shutdownTimeout)     //ONLY IF PEGASUS_HAS_SSL
{
    _rep = new DynamicListenerRep(portNumber, 
                                  consumerDir,           
                                  consumerConfigDir,     
                                  sslContext,             
                                  enableConsumerUnload,   
                                  consumerIdleTimeout,  
                                  shutdownTimeout);
}
#endif

DynamicListener::~DynamicListener()
{
    delete static_cast<DynamicListenerRep*>(_rep);
}

void DynamicListener::start()
{
    static_cast<DynamicListenerRep*>(_rep)->start();
}

void DynamicListener::stop(Boolean forceShutdown)
{
    static_cast<DynamicListenerRep*>(_rep)->stop(forceShutdown);
}

Boolean DynamicListener::isAlive()
{
    return static_cast<DynamicListenerRep*>(_rep)->isAlive();
}

Boolean DynamicListener::addConsumer(const String& consumerName, const String& location)
{
    return static_cast<DynamicListenerRep*>(_rep)->addConsumer(consumerName, location);
}

Boolean DynamicListener::removeConsumer(const String& consumerName)
{
    return static_cast<DynamicListenerRep*>(_rep)->removeConsumer(consumerName);
}

Uint32 DynamicListener::getPortNumber() 
{
    return static_cast<DynamicListenerRep*>(_rep)->getPortNumber();
}

String DynamicListener::getConsumerDir() 
{
    return static_cast<DynamicListenerRep*>(_rep)->getConsumerDir();
} 

String DynamicListener::getConsumerConfigDir() 
{
    return static_cast<DynamicListenerRep*>(_rep)->getConsumerConfigDir();
} 

void DynamicListener::setEnableConsumerUnload(const Boolean consumerUnload)
{
    static_cast<DynamicListenerRep*>(_rep)->setEnableConsumerUnload(consumerUnload);
}

Boolean DynamicListener::getEnableConsumerUnload()
{
    return static_cast<DynamicListenerRep*>(_rep)->getEnableConsumerUnload();
}

Uint32 DynamicListener::getIdleTimeout() 
{
    return static_cast<DynamicListenerRep*>(_rep)->getIdleTimeout();
}

void DynamicListener::setIdleTimeout(Uint32 idleTimeout)
{
    static_cast<DynamicListenerRep*>(_rep)->setIdleTimeout(idleTimeout);
}


PEGASUS_NAMESPACE_END

