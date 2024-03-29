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

#include <cstdio>
#include <cctype>
#include <ctime>

#if (defined(PEGASUS_OS_HPUX) || defined(PEGASUS_OS_LINUX)) \
    && defined(PEGASUS_USE_RELEASE_DIRS)
# include <unistd.h>
# include <errno.h>
#endif

#ifdef PEGASUS_OS_PASE
#include <as400_protos.h> // for systemCL()
#endif

#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Signal.h>
#include <Pegasus/Common/HTTPAcceptor.h>
#include <Pegasus/Common/Tracer.h>
#include <Pegasus/Common/Logger.h>
#include <Pegasus/Common/Cimom.h>
#include <Pegasus/Common/PegasusVersion.h>
#include <Pegasus/Common/SSLContextManager.h>
#include <Pegasus/Common/Time.h>
#include <Pegasus/Common/MessageLoader.h>
#include <Pegasus/Common/AuditLogger.h>

#include <Pegasus/Repository/CIMRepository.h>
#include <Pegasus/ExportServer/CIMExportRequestDispatcher.h>
#include <Pegasus/ExportServer/CIMExportResponseEncoder.h>
#include <Pegasus/ExportServer/CIMExportRequestDecoder.h>
#include <Pegasus/Config/ConfigManager.h>
#include <Pegasus/Security/UserManager/UserManager.h>
#include <Pegasus/HandlerService/IndicationHandlerService.h>
#include <Pegasus/IndicationService/IndicationService.h>
#include <Pegasus/ProviderManagerService/ProviderManagerService.h>
#include <Pegasus/ProviderManager2/Default/DefaultProviderManager.h>

#if defined PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
# include "ConsoleManager_zOS.h"
#endif

#ifdef PEGASUS_ENABLE_SLP
# include <Pegasus/Client/CIMClient.h>
#endif

// PEGASUS_SLP_REG_TIMEOUT is the time interval in minute for reregistration
// with SLP.
#ifdef PEGASUS_SLP_REG_TIMEOUT
#include "SLPAttrib.h"
#include <slp/slp_client/src/cmd-utils/slp_client/lslp-common-defs.h>
#include <slp/slp_client/src/cmd-utils/slp_client/slp_client.h>
#endif
#include "CIMServer.h"
#include "CIMOperationRequestDispatcher.h"
#include "CIMOperationResponseEncoder.h"
#include "CIMOperationRequestDecoder.h"
#include "CIMOperationRequestAuthorizer.h"
#include "HTTPAuthenticatorDelegator.h"
#include "ShutdownProvider.h"
#include "ShutdownService.h"
#include "BinaryMessageHandler.h"
#include <Pegasus/Common/ModuleController.h>
#include <Pegasus/ControlProviders/UserAuthProvider/UserAuthProvider.h>
#include <Pegasus/ControlProviders/ConfigSettingProvider/\
ConfigSettingProvider.h>
#include <Pegasus/ControlProviders/ProviderRegistrationProvider/\
ProviderRegistrationProvider.h>
#include <Pegasus/ControlProviders/NamespaceProvider/NamespaceProvider.h>

#ifndef PEGASUS_DISABLE_PERFINST
# include <Pegasus/ControlProviders/Statistic/CIMOMStatDataProvider.h>
#endif

#ifdef PEGASUS_HAS_SSL
# include <Pegasus/ControlProviders/CertificateProvider/CertificateProvider.h>
#endif

#ifndef PEGASUS_DISABLE_CQL
# include <Pegasus/ControlProviders/QueryCapabilitiesProvider/\
CIMQueryCapabilitiesProvider.h>
#endif

#if defined PEGASUS_ENABLE_INTEROP_PROVIDER
# include <Pegasus/ControlProviders/InteropProvider/InteropProvider.h>
#endif

PEGASUS_NAMESPACE_BEGIN
#ifdef PEGASUS_SLP_REG_TIMEOUT
ThreadReturnType PEGASUS_THREAD_CDECL registerPegasusWithSLP(void *parm);
// Configurable SLP port to be handeled in a separate bug.
# define SLP_DEFAULT_PORT 427
# define LOCALHOST_IP "127.0.0.1"
#endif

static CIMServer* _cimserver = NULL;

static Boolean _slpRegistrationComplete;

// Need a static method to act as a callback for the control provider.
// This doesn't belong here, but I don't have a better place to put it.
static Message* controlProviderReceiveMessageCallback(
    Message* message,
    void* instance)
{
    CIMRequestMessage* request = dynamic_cast<CIMRequestMessage*>(message);
    PEGASUS_ASSERT(request != 0);

    AcceptLanguageList* langs = new AcceptLanguageList(
        ((AcceptLanguageListContainer) request->operationContext.get(
            AcceptLanguageListContainer::NAME)).getLanguages());
    Thread::setLanguages(langs);

    ProviderMessageHandler* pmh =
        reinterpret_cast<ProviderMessageHandler*>(instance);
    return pmh->processMessage(request);
}

//
// Signal handler for shutdown signals, currently SIGHUP and SIGTERM
//
Boolean handleShutdownSignal = false;
void shutdownSignalHandler(int s_n, PEGASUS_SIGINFO_T* s_info, void* sig)
{
    PEG_METHOD_ENTER(TRC_SERVER, "shutdownSignalHandler");
    PEG_TRACE((TRC_SERVER, Tracer::LEVEL2, "Signal %d received.", s_n));

    CIMServer::shutdownSignal();

    PEG_METHOD_EXIT();
}

void CIMServer::shutdownSignal()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::shutdownSignal()");
    handleShutdownSignal = true;
    _cimserver->tickle_monitor();
    PEG_METHOD_EXIT();
}

#ifdef PEGASUS_OS_PASE
static void _synchronousSignalHandler(int s_n, PEGASUS_SIGINFO_T* s_info,
                                      void* sig)
{
    static bool mark = false;
    if (mark)
        return;

    mark = true;

    Logger::put_l(Logger::ERROR_LOG, "CIMServer", Logger::SEVERE,
        "Pegasus.Server.CIMServer.RECEIVE_SYN_SIGNAL.PEGASUS_OS_PASE", \
        "Synchronous signal received.");

    /* save job log */
    systemCL ("QSYS/CHGJOB JOB(*) LOG(4 00 *NOLIST)",
            SYSTEMCL_MSG_STDOUT | SYSTEMCL_MSG_STDERR);

    CIMServer::shutdownSignal();
}

static void _asynchronousSignalHandler(int s_n, PEGASUS_SIGINFO_T* s_info,
                                       void* sig)
{
    static bool mark = false;
    if (mark)
        return;

    mark = true;

    Logger::put_l(Logger::ERROR_LOG, "CIMServer", Logger::SEVERE,
        "Pegasus.Server.CIMServer.RECEIVE_ASYN_SIGNAL.PEGASUS_OS_PASE", \
        "Asynchronous signal received.");

    CIMServer::shutdownSignal();
}
#endif


CIMServer::CIMServer()
    : _dieNow(false)
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::CIMServer()");
    _cimserver = this;
    _init();

    PEG_METHOD_EXIT();
}

void CIMServer::tickle_monitor()
{
    _monitor->tickle();
}

void CIMServer::_init()
{
    _monitor.reset(new Monitor());

#ifdef PEGASUS_ENABLE_SLP
    _runSLP = true;         // Boolean cannot be set in definition.
    _slpRegistrationComplete = false;
#endif

#if (defined(PEGASUS_OS_HPUX) || defined(PEGASUS_OS_LINUX)) \
    && defined(PEGASUS_USE_RELEASE_DIRS)
    if (chdir(PEGASUS_CORE_DIR) != 0)
    {
        PEG_TRACE((TRC_SERVER, Tracer::LEVEL2,
            "chdir(\"%s\") failed with errno %d.", PEGASUS_CORE_DIR, errno));
    }
#endif

    // -- Create a repository:

    String repositoryRootPath =
        ConfigManager::getHomedPath(
            ConfigManager::getInstance()->getCurrentValue("repositoryDir"));

#ifdef DO_NOT_CREATE_REPOSITORY_ON_STARTUP
    // If this code is enable, the CIMServer will fail to start
    // if the repository directory does not exist. If called,
    // the Repository will create an empty repository.

    // This check has been disabled to allow cimmof to call
    // the CIMServer to build the initial repository.
    if (!FileSystem::isDirectory(repositoryRootPath))
    {
        PEG_METHOD_EXIT();
        throw NoSuchDirectory(repositoryRootPath);
    }
#endif

    _repository = new CIMRepository(repositoryRootPath);

    // -- Create a UserManager object:

    UserManager* userManager = UserManager::getInstance(_repository);

    // -- Create a CIMServerState object:

    _serverState.reset(new CIMServerState());

    _providerRegistrationManager = new ProviderRegistrationManager(_repository);

    // -- Create queue inter-connections:

    _providerManager = new ProviderManagerService(
        _providerRegistrationManager,
        _repository,
        DefaultProviderManager::createDefaultProviderManagerCallback);

    // Create IndicationHandlerService:

    _handlerService = new IndicationHandlerService(_repository);

    _cimOperationRequestDispatcher = new CIMOperationRequestDispatcher(
        _repository, _providerRegistrationManager);
    _binaryMessageHandler =
        new BinaryMessageHandler(_cimOperationRequestDispatcher);

    // Create the control service
    _controlService = new ModuleController(PEGASUS_QUEUENAME_CONTROLSERVICE);

    // Jump this number up when there are more control providers.
    _controlProviders.reserveCapacity(16);

    // Create the Configuration control provider
    ProviderMessageHandler* configProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "ConfigSettingProvider",
        new ConfigSettingProvider(), 0, 0, false);

    _controlProviders.append(configProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_CONFIGPROVIDER,
        configProvider,
        controlProviderReceiveMessageCallback,
        0);

    // Create the User/Authorization control provider
    ProviderMessageHandler* userAuthProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "UserAuthProvider",
        new UserAuthProvider(_repository), 0, 0, false);
    _controlProviders.append(userAuthProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_USERAUTHPROVIDER,
        userAuthProvider,
        controlProviderReceiveMessageCallback,
        0);

    // Create the Provider Registration control provider
    ProviderMessageHandler* provRegProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "ProviderRegistrationProvider",
        new ProviderRegistrationProvider(_providerRegistrationManager),
        0, 0, false);
    // Warning: The ProviderRegistrationProvider destructor deletes
    // _providerRegistrationManager
    _controlProviders.append(provRegProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_PROVREGPROVIDER,
        provRegProvider,
        controlProviderReceiveMessageCallback,
        0);

    // Create the Shutdown control provider
    ProviderMessageHandler* shutdownProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "ShutdownProvider",
        new ShutdownProvider(this), 0, 0, false);
    _controlProviders.append(shutdownProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_SHUTDOWNPROVIDER,
        shutdownProvider,
        controlProviderReceiveMessageCallback,
        0);

    // Create the namespace control provider
    ProviderMessageHandler* namespaceProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "NamespaceProvider",
        new NamespaceProvider(_repository), 0, 0, false);
    _controlProviders.append(namespaceProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_NAMESPACEPROVIDER,
        namespaceProvider,
        controlProviderReceiveMessageCallback,
        0);

    //
    // Create a SSLContextManager object
    //
    _sslContextMgr = new SSLContextManager();

#ifdef PEGASUS_HAS_SSL
    // Because this provider allows management of the cimserver truststore
    // it needs to be available regardless of the value
    // of sslClientVerificationMode config property.
    ProviderMessageHandler* certificateProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "CertificateProvider",
        new CertificateProvider(_repository, _sslContextMgr),
        0, 0, false);
    _controlProviders.append(certificateProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_CERTIFICATEPROVIDER,
        certificateProvider,
        controlProviderReceiveMessageCallback,
        0);
#endif

#ifndef PEGASUS_DISABLE_PERFINST
    // Create the Statistical Data control provider
    ProviderMessageHandler* cimomstatdataProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "CIMOMStatDataProvider",
        new CIMOMStatDataProvider(), 0, 0, false);
    _controlProviders.append(cimomstatdataProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_CIMOMSTATDATAPROVIDER,
        cimomstatdataProvider,
        controlProviderReceiveMessageCallback,
        0);
#endif

#ifndef PEGASUS_DISABLE_CQL

    // Create the Query Capabilities control provider
    ProviderMessageHandler* cimquerycapprovider = new ProviderMessageHandler(
        "CIMServerControlProvider", "CIMQueryCapabilitiesProvider",
        new CIMQueryCapabilitiesProvider(),
        0, 0, false);
    _controlProviders.append(cimquerycapprovider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_CIMQUERYCAPPROVIDER,
        cimquerycapprovider,
        controlProviderReceiveMessageCallback,
        0);
#endif

#if defined PEGASUS_ENABLE_INTEROP_PROVIDER

    // Create the interop control provider
    ProviderMessageHandler* interopProvider = new ProviderMessageHandler(
        "CIMServerControlProvider", "InteropProvider",
        new InteropProvider(_repository), 0, 0, false);
    _controlProviders.append(interopProvider);
    ModuleController::register_module(
        PEGASUS_QUEUENAME_CONTROLSERVICE,
        PEGASUS_MODULENAME_INTEROPPROVIDER,
        interopProvider,
        controlProviderReceiveMessageCallback,
        0);
#endif

    _cimOperationResponseEncoder = new CIMOperationResponseEncoder;

    //
    // get the configured authentication and authorization flags
    //
    ConfigManager* configManager = ConfigManager::getInstance();

    Boolean enableAuthentication = ConfigManager::parseBooleanValue(
        configManager->getCurrentValue("enableAuthentication"));

    //
    // Create Authorization queue only if authentication is enabled
    //
    if ( enableAuthentication )
    {
        _cimOperationRequestAuthorizer = new CIMOperationRequestAuthorizer(
            _cimOperationRequestDispatcher);

        _cimOperationRequestDecoder = new CIMOperationRequestDecoder(
            _cimOperationRequestAuthorizer,
            _cimOperationResponseEncoder->getQueueId());
    }
    else
    {
        _cimOperationRequestAuthorizer = 0;

        _cimOperationRequestDecoder = new CIMOperationRequestDecoder(
            _cimOperationRequestDispatcher,
            _cimOperationResponseEncoder->getQueueId());
    }

    _cimExportRequestDispatcher = new CIMExportRequestDispatcher();

    _cimExportResponseEncoder = new CIMExportResponseEncoder;

    _cimExportRequestDecoder = new CIMExportRequestDecoder(
        _cimExportRequestDispatcher,
        _cimExportResponseEncoder->getQueueId());

    _httpAuthenticatorDelegator = new HTTPAuthenticatorDelegator(
        _cimOperationRequestDecoder->getQueueId(),
        _cimExportRequestDecoder->getQueueId(),
        _repository);

    // IMPORTANT-NU-20020513: Indication service must start after ExportService
    // otherwise HandlerService started by indicationService will never
    // get ExportQueue to export indications for existing subscriptions

    _indicationService = 0;
    if (ConfigManager::parseBooleanValue(
        configManager->getCurrentValue("enableIndicationService")))
    {
        _indicationService = new IndicationService(
            _repository, _providerRegistrationManager);
    }

    // Enable the signal handler to shutdown gracefully on SIGHUP and SIGTERM
    getSigHandle()->registerHandler(PEGASUS_SIGHUP, shutdownSignalHandler);
    getSigHandle()->activate(PEGASUS_SIGHUP);
    getSigHandle()->registerHandler(PEGASUS_SIGTERM, shutdownSignalHandler);
    getSigHandle()->activate(PEGASUS_SIGTERM);
#ifdef PEGASUS_OS_PASE
    getSigHandle()->registerHandler(SIGFPE, _synchronousSignalHandler);
    getSigHandle()->activate(SIGFPE);
    getSigHandle()->registerHandler(SIGILL, _synchronousSignalHandler);
    getSigHandle()->activate(SIGILL);
    getSigHandle()->registerHandler(SIGSEGV, _synchronousSignalHandler);
    getSigHandle()->activate(SIGSEGV);
    getSigHandle()->registerHandler(SIGIO, _asynchronousSignalHandler);
    getSigHandle()->activate(SIGIO);
#endif

    //
    // Set up an additional thread waiting for commands from the
    // system console
    //
#if defined PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
    ZOSConsoleManager::startConsoleWatchThread();
#endif

    // Load and initialize providers registed with AutoStart = true
    _providerRegistrationManager->initializeProviders();

#ifdef PEGASUS_ENABLE_AUDIT_LOGGER

    // Register audit logger initialize callback
    AuditLogger::setInitializeCallback(auditLogInitializeCallback);

    Boolean enableAuditLog = ConfigManager::parseBooleanValue(
        configManager->getCurrentValue("enableAuditLog"));

    if (enableAuditLog)
    {
        AuditLogger::setEnabled(enableAuditLog);
    }

#endif
}


CIMServer::~CIMServer ()
{
    PEG_METHOD_ENTER (TRC_SERVER, "CIMServer::~CIMServer()");

    // Wait until the Shutdown provider request has cleared through the
    // system.
    ShutdownService::getInstance(this)->waitUntilNoMoreRequests(false);

    // Ok, shutdown all the MQSs. This shuts their communication channel.
    ShutdownService::getInstance(this)->shutdownCimomServices();

    // Start deleting the objects.
    // The order is very important.

    // The HTTPAcceptor depends on HTTPAuthenticationDelegator
    for (Uint32 i = 0, n = _acceptors.size (); i < n; i++)
    {
        HTTPAcceptor* p = _acceptors[i];
        delete p;
    }

    // IndicationService depends on ProviderManagerService,
    // IndicationHandlerService, and ProviderRegistrationManager, and thus
    // should be deleted before the ProviderManagerService,
    // IndicationHandlerService, and ProviderRegistrationManager are deleted.
    delete _indicationService;

    // HTTPAuthenticationDelegator depends on CIMRepository,
    // CIMOperationRequestDecoder and CIMExportRequestDecoder
    delete _httpAuthenticatorDelegator;

    delete _cimExportRequestDecoder;

    delete _cimExportResponseEncoder;

    delete _cimExportRequestDispatcher;

    // CIMOperationRequestDecoder depends on CIMOperationRequestAuthorizer
    // and CIMOperationResponseEncoder
    delete _cimOperationRequestDecoder;

    delete _cimOperationResponseEncoder;

    // BinaryMessageHandler depends on CIMOperationRequestDispatcher
    delete _binaryMessageHandler;

    // CIMOperationRequestAuthorizer depends on
    // CIMOperationRequestDispatcher
    delete _cimOperationRequestAuthorizer;

    // IndicationHandlerService uses CIMOperationRequestDispatcher
    delete _handlerService;

    // CIMOperationRequestDispatcher depends on
    // CIMRepository and ProviderRegistrationManager.
    // CIMOperationRequestDispatcher keeps an internal list of control
    // providers. Must delete this before ModuleController.
    delete _cimOperationRequestDispatcher;

    // ModuleController takes care of deleting all wrappers around
    // the control providers.
    delete _controlService;

    // Find all of the control providers (module)
    // Must delete CIMOperationRequestDispatcher _before_ deleting each
    // of the control provider. The CIMOperationRequestDispatcher keeps
    // its own table of the internal providers (pointers).
    for (Uint32 i = 0, n = _controlProviders.size(); i < n; i++)
    {
        ProviderMessageHandler* p = _controlProviders[i];
        delete p->getProvider();
        delete p;
    }

    // The SSL control providers use the SSL context manager.
    delete _sslContextMgr;

    // ProviderManagerService depends on ProviderRegistrationManager.
    // Note that deleting the ProviderManagerService causes the
    // DefaultProviderManager (if loaded) to get unloaded.  Dynamically
    // unloading the DefaultProviderManager library affects (on HP-UX, at
    // least) the statically loaded version of this library used by the
    // ProviderMessageHandler wrapper for the control providers.  Deleting
    // the ProviderManagerService after the control providers is a
    // workaround for this problem.
    delete _providerManager;

    delete _providerRegistrationManager;

    // Almost everybody uses the CIMRepository.
    delete _repository;

    // Destroy the singleton services
    ConfigManager::destroy();
    UserManager::destroy();
    ShutdownService::destroy();

    PEG_METHOD_EXIT ();
}

void CIMServer::addAcceptor(
    Uint16 connectionType,
    Uint32 portNumber,
    Boolean useSSL)
{
    HTTPAcceptor* acceptor;

    acceptor = new HTTPAcceptor(
        _monitor.get(),
        _httpAuthenticatorDelegator,
        connectionType,
        portNumber,
        useSSL ? _getSSLContext() : 0,
        useSSL ? _sslContextMgr->getSSLContextObjectLock() : 0 );

    ConfigManager* configManager = ConfigManager::getInstance();
    String socketWriteConfigTimeout =
        configManager->getCurrentValue("socketWriteTimeout");
    // Set timeout value for server socket timeouts
    // depending on config option
    Uint32 socketWriteTimeout =
        strtol(socketWriteConfigTimeout.getCString(), (char**)0, 10);
    // equal what went wrong, there has to be a timeout
    if (socketWriteTimeout == 0)
        socketWriteTimeout = PEGASUS_DEFAULT_SOCKETWRITE_TIMEOUT_SECONDS;
    acceptor->setSocketWriteTimeout(socketWriteTimeout);

    _acceptors.append(acceptor);
}

void CIMServer::bind()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::bind()");

    if (_acceptors.size() == 0)
    {
        MessageLoaderParms mlp = MessageLoaderParms(
            "Server.CIMServer.BIND_FAILED",
            "No CIM Server connections are enabled.");

        throw BindFailedException(mlp);
    }

    for (Uint32 i=0; i<_acceptors.size(); i++)
    {
        _acceptors[i]->bind();
    }

    PEG_METHOD_EXIT();
}

void CIMServer::runForever()
{
    // Note: Trace code in this method will be invoked frequently.

    if (!_dieNow)
    {
        struct timeval now;
#ifdef PEGASUS_ENABLE_SLP
# ifdef PEGASUS_SLP_REG_TIMEOUT
        static struct timeval lastReregistrationTime = {0,0};
        Time::gettimeofday(&now);

        // If PEGASUS_SLP_REG_TIMEOUT (SLP registration timeout in minutes) is
        // defined, then when this SLP registration timeout is exceeded, we
        // need to call startSLPProvider() to update our registration with SLP.
        // Convert SLP registration timeout to seconds for this check.
        if (now.tv_sec - lastReregistrationTime.tv_sec > 
            (PEGASUS_SLP_REG_TIMEOUT * 60))
        {
            lastReregistrationTime.tv_sec = now.tv_sec;
# endif
            startSLPProvider();
# ifdef PEGASUS_SLP_REG_TIMEOUT
        }
# endif // PEGASUS_SLP_REG_TIMEOUT
#endif // PEGASUS_ENABLE_SLP

        _monitor->run(500000);

        static struct timeval lastIdleCleanupTime = {0, 0};
        Time::gettimeofday(&now);

        if (now.tv_sec - lastIdleCleanupTime.tv_sec > 300)
        {
            lastIdleCleanupTime.tv_sec = now.tv_sec;

            try
            {
                _providerManager->unloadIdleProviders();
                MessageQueueService::get_thread_pool()->cleanupIdleThreads();
            }
            catch (...)
            {
            }
        }

        if (handleShutdownSignal)
        {
            PEG_TRACE_CSTRING(TRC_SERVER, Tracer::LEVEL3,
                "CIMServer::runForever - signal received.  Shutting down.");
            ShutdownService::getInstance(this)->shutdown(true, 10, false);
            // Set to false must be after call to shutdown.  See
            // stopClientConnection.
            handleShutdownSignal = false;
        }
    }
}

void CIMServer::stopClientConnection()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::stopClientConnection()");

    // tell Monitor to stop listening for client connections
    if (handleShutdownSignal)
        // If shutting down, this is in the same thread as runForever.
        // No need to wait for the thread to see the stop flag.
        _monitor->stopListeningForConnections(false);
    else
        // If not shutting down, this is not in the same thread as runForever.
        // Need to wait for the thread to see the stop flag.
        _monitor->stopListeningForConnections(true);

    //
    // Wait 150 milliseconds to allow time for the Monitor to stop
    // listening for client connections.
    //
    // This wait time is the timeout value for the select() call
    // in the Monitor's run() method (currently set to 100
    // milliseconds) plus a delta of 50 milliseconds.  The reason
    // for the wait here is to make sure that the Monitor entries
    // are updated before closing the connection sockets.
    //
    // PEG_TRACE_CSTRING(TRC_SERVER, Tracer::LEVEL4, "Wait 150 milliseconds.");
    //  Threads::sleep(150);  not needed anymore due to the semaphore
    // in the monitor

    for (Uint32 i=0; i<_acceptors.size(); i++)
    {
        _acceptors[i]->closeConnectionSocket();
    }

    PEG_METHOD_EXIT();
}

void CIMServer::shutdown()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::shutdown()");

#ifdef PEGASUS_DEBUG
    _repository->DisplayCacheStatistics();
#endif

    _dieNow = true;
    _cimserver->tickle_monitor();

    PEG_METHOD_EXIT();
}

void CIMServer::resume()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::resume()");

    for (Uint32 i=0; i<_acceptors.size(); i++)
    {
        _acceptors[i]->reopenConnectionSocket();
    }

    PEG_METHOD_EXIT();
}

void CIMServer::setState(Uint32 state)
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::setState()");

    _serverState->setState(state);

    //
    // get the configured authentication and authorization flags
    //
    ConfigManager* configManager = ConfigManager::getInstance();

    Boolean enableAuthentication = ConfigManager::parseBooleanValue(
        configManager->getCurrentValue("enableAuthentication"));
    Boolean enableNamespaceAuthorization = ConfigManager::parseBooleanValue(
        configManager->getCurrentValue("enableNamespaceAuthorization"));

    if (state == CIMServerState::TERMINATING)
    {
        // tell decoder that CIMServer is terminating
        _cimOperationRequestDecoder->setServerTerminating(true);
        _cimExportRequestDecoder->setServerTerminating(true);

        // tell authorizer that CIMServer is terminating ONLY if
        // authentication and authorization are enabled
        //
        if ( enableAuthentication && enableNamespaceAuthorization )
        {
            _cimOperationRequestAuthorizer->setServerTerminating(true);
        }
    }
    else
    {
        // tell decoder that CIMServer is not terminating
        _cimOperationRequestDecoder->setServerTerminating(false);
        _cimExportRequestDecoder->setServerTerminating(false);

        // tell authorizer that CIMServer is terminating ONLY if
        // authentication and authorization are enabled
        //
        if ( enableAuthentication && enableNamespaceAuthorization )
        {
            _cimOperationRequestAuthorizer->setServerTerminating(false);
        }
    }
    PEG_METHOD_EXIT();
}

Uint32 CIMServer::getOutstandingRequestCount()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::getOutstandingRequestCount()");

    Uint32 requestCount = 0;

    for (Uint32 i=0; i<_acceptors.size(); i++)
    {
        requestCount += _acceptors[i]->getOutstandingRequestCount();
    }

    PEG_METHOD_EXIT();
    return requestCount;
}

//
SSLContext* CIMServer::_getSSLContext()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::_getSSLContext()");

    static const String PROPERTY_NAME__SSL_CERT_FILEPATH =
        "sslCertificateFilePath";
    static const String PROPERTY_NAME__SSL_KEY_FILEPATH = "sslKeyFilePath";
    static const String PROPERTY_NAME__SSL_TRUST_STORE = "sslTrustStore";
    static const String PROPERTY_NAME__SSL_CRL_STORE = "crlStore";
    static const String PROPERTY_NAME__SSL_CLIENT_VERIFICATION =
        "sslClientVerificationMode";
    static const String PROPERTY_NAME__SSL_AUTO_TRUST_STORE_UPDATE =
        "enableSSLTrustStoreAutoUpdate";
    static const String PROPERTY_NAME__SSL_TRUST_STORE_USERNAME =
        "sslTrustStoreUserName";
    static const String PROPERTY_NAME__HTTP_ENABLED =
        "enableHttpConnection";

    String verifyClient;
    String trustStore;
    SSLContext* sslContext = 0;

    //
    // Get a config manager instance
    //
    ConfigManager* configManager = ConfigManager::getInstance();

    // Note that if invalid values were set for either sslKeyFilePath,
    // sslCertificateFilePath, crlStore or sslTrustStore, the invalid
    // paths would have been detected in SecurityPropertyOwner and
    // terminated the server startup. This happens regardless of whether
    // or not HTTPS is enabled (not a great design, but that seems to
    // be how other properties are validated as well)
    //
    // Get the sslClientVerificationMode property from the Config
    // Manager.
    //
    verifyClient = configManager->getCurrentValue(
        PROPERTY_NAME__SSL_CLIENT_VERIFICATION);

    //
    // Get the sslTrustStore property from the Config Manager.
    //
    trustStore = configManager->getCurrentValue(
        PROPERTY_NAME__SSL_TRUST_STORE);

    if (trustStore != String::EMPTY)
    {
        trustStore = ConfigManager::getHomedPath(trustStore);
    }

    PEG_TRACE_STRING(TRC_SERVER, Tracer::LEVEL4,
        "Server trust store name: " + trustStore);

    //
    // Get the sslTrustStoreUserName property from the Config Manager.
    //
    String trustStoreUserName;
    trustStoreUserName = configManager->getCurrentValue(
        PROPERTY_NAME__SSL_TRUST_STORE_USERNAME);

    if (!String::equal(verifyClient, "disabled"))
    {
        //
        // 'required' and 'optional' settings must have a valid truststore
        //
        if (trustStore == String::EMPTY)
        {
            MessageLoaderParms parms(
                "Pegasus.Server.SSLContextManager."
                    "SSL_CLIENT_VERIFICATION_EMPTY_TRUSTSTORE",
                "The \"sslTrustStore\" configuration property must be set "
                    "if \"sslClientVerificationMode\" is 'required' or "
                    "'optional'.");
            PEG_METHOD_EXIT();
            throw SSLException(parms);
        }

#ifdef PEGASUS_DISABLE_LOCAL_DOMAIN_SOCKET
        //
        // ATTN: 'required' setting must have http port enabled.
        // If only https is enabled, and a call to shutdown the
        // cimserver is given, the call will hang and a forced shutdown
        // will ensue. This is because the CIMClient::connectLocal call
        // cannot specify a certificate for authentication against
        // the local server.  This limitation is being investigated.
        // See Bugzilla 2995.
        //
        if (String::equal(verifyClient, "required"))
        {
            if (!ConfigManager::parseBooleanValue(
                configManager->getCurrentValue(
                    PROPERTY_NAME__HTTP_ENABLED)))
            {
                MessageLoaderParms parms(
                    "Pegasus.Server.SSLContextManager."
                        "INVALID_CONF_HTTPS_REQUIRED",
                    "The \"sslClientVerificationMode\" property cannot be "
                        "set to \"required\" if HTTP is disabled, as the "
                        "cimserver will be unable to properly shutdown.  "
                        "The recommended course of action is to change "
                        "the property value to \"optional\".");
                PEG_METHOD_EXIT();
                throw SSLException(parms);
            }
        }
#endif
        //
        // A truststore username must be specified if
        // sslClientVerificationMode is enabled and the truststore is a
        // single CA file.  If the truststore is a directory, then the
        // CertificateProvider should be used to register users with
        // certificates.
        //
        if ((trustStore != String::EMPTY) &&
            (!FileSystem::isDirectory(trustStore)))
        {
            if (trustStoreUserName == String::EMPTY)
            {
                MessageLoaderParms parms(
                    "Pegasus.Server.SSLContextManager."
                        "SSL_CLIENT_VERIFICATION_EMPTY_USERNAME",
                    "The \"sslTrustStoreUserName\" property must specify a "
                        "valid username if \"sslClientVerificationMode\" is "
                        "'required' or 'optional' and the truststore is a "
                        "single CA file. To register individual certificates "
                        "to users, you must use a truststore directory along "
                        "with the CertificateProvider.");
                PEG_METHOD_EXIT();
                throw SSLException(parms);
            }
        }
    }

#ifdef PEGASUS_ENABLE_SSL_CRL_VERIFICATION
    //
    // Get the crlStore property from the Config Manager.
    //
    String crlStore = configManager->getCurrentValue(
        PROPERTY_NAME__SSL_CRL_STORE);

    if (crlStore != String::EMPTY)
    {
        crlStore = ConfigManager::getHomedPath(crlStore);
    }
#else
    String crlStore;
#endif

    //
    // Get the sslCertificateFilePath property from the Config Manager.
    //
    String certPath;
    certPath = ConfigManager::getHomedPath(
        configManager->getCurrentValue(PROPERTY_NAME__SSL_CERT_FILEPATH));

    //
    // Get the sslKeyFilePath property from the Config Manager.
    //
    String keyPath;
    keyPath = ConfigManager::getHomedPath(
        configManager->getCurrentValue(PROPERTY_NAME__SSL_KEY_FILEPATH));

    String randFile;

#ifdef PEGASUS_SSL_RANDOMFILE
    // NOTE: It is technically not necessary to set up a random file on
    // the server side, but it is easier to use a consistent interface
    // on the client and server than to optimize out the random file on
    // the server side.
    randFile = ConfigManager::getHomedPath(PEGASUS_SSLSERVER_RANDOMFILE);
#endif

    //
    // Create the SSLContext defined by the configuration properties
    //
    if (String::equal(verifyClient, "required"))
    {
        PEG_TRACE_CSTRING(TRC_SERVER, Tracer::LEVEL2,
            "SSL Client verification REQUIRED.");

        _sslContextMgr->createSSLContext(
            trustStore, certPath, keyPath, crlStore, false, randFile);
    }
    else if (String::equal(verifyClient, "optional"))
    {
        PEG_TRACE_CSTRING(TRC_SERVER, Tracer::LEVEL2,
            "SSL Client verification OPTIONAL.");

        _sslContextMgr->createSSLContext(
            trustStore, certPath, keyPath, crlStore, true, randFile);
    }
    else if (String::equal(verifyClient, "disabled") ||
             verifyClient == String::EMPTY)
    {
        PEG_TRACE_CSTRING(TRC_SERVER, Tracer::LEVEL2,
            "SSL Client verification DISABLED.");

        _sslContextMgr->createSSLContext(
            String::EMPTY, certPath, keyPath, crlStore, false, randFile);
    }
    sslContext = _sslContextMgr->getSSLContext();

    PEG_METHOD_EXIT();
    return sslContext;
}

void CIMServer::auditLogInitializeCallback()
{
#ifdef PEGASUS_ENABLE_AUDIT_LOGGER

    Array<String> propertyNames;
    Array<String> propertyValues;

    // Get all current property names and values
    ConfigManager* configManager = ConfigManager::getInstance();

    configManager->getAllPropertyNames(propertyNames, false);

    for (Uint32 i = 0; i < propertyNames.size(); i++)
    {
        propertyValues.append(configManager->getCurrentValue(propertyNames[i]));
    }

    AuditLogger::logCurrentConfig(propertyNames, propertyValues);

    // get currently registered provider module instances
    Array<CIMInstance> moduleInstances;

    moduleInstances =
        _cimserver->_providerRegistrationManager->enumerateInstancesForClass(
        CIMObjectPath("PG_ProviderModule"));

    AuditLogger::logCurrentRegProvider(moduleInstances);

    AuditLogger::logCurrentEnvironmentVar();

#endif
}

#ifdef PEGASUS_ENABLE_SLP

ThreadReturnType PEGASUS_THREAD_CDECL _callSLPProvider(void* parm);

// This is a control function that starts a new thread which issues a
// cim operation to start the slp provider.
//
// If PEGASUS_ENABLE_SLP is defined, then startSLPProvider is called for
// each iteration of the runForever "loop". The _runSLP variable will be
// set from the "slp" configuration variable, and if true, SLP registration
// will occur, but only once. After registering, _runSLP is set to false
// and remains so for the life of this process.
//
// If *both* PEGASUS_ENABLE_SLP and PEGASUS_SLP_REG_TIMEOUT are defined,
// then the first call to startSLPProvider is for the initial SLP
// registration, and each subsequent call is to "reregister" or update our
// SLP registration based on the timeout interval. In this case _runSLP
// will still be set from the "slp" configuration variable, and tested,
// but it will retain this setting for the life of this process.
//
// Note that the SLP registration needs to be performed from within the
// runForever "loop" because it will actually call connectLocal() and
// client.invokeMethod() to perform the SLP registration, so the CIM
// Server needs to be able to accept connectLocal requests

void CIMServer::startSLPProvider()
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::startSLPProvider");

#ifndef PEGASUS_SLP_REG_TIMEOUT
    // This is a onetime function.  If already issued, or config is not to
    // use simply return
    if (!_runSLP)
    {
        return;
    }
#endif

    // Get Config parameter to determine if we should start SLP.
    ConfigManager* configManager = ConfigManager::getInstance();
    _runSLP = ConfigManager::parseBooleanValue(
         configManager->getCurrentValue("slp"));

    // If false, do not start slp provider
    if (!_runSLP)
    {
        return;
    }
#ifndef PEGASUS_SLP_REG_TIMEOUT
    //SLP startup is onetime function; reset the switch so this
    // function does not get called a second time.
    _runSLP = false;
#endif
    // Start SLPProvider for Built-in SA and Open SLP SA. If the
    // PEGASUS_SLP_REG_TIMEOUT is defined and if Open SLP is not used, start a
    // thread which advertises CIMOM with a external SLP SA( i.e . IBM SA).
#if defined( PEGASUS_SLP_REG_TIMEOUT ) && !defined( PEGASUS_USE_OPENSLP )
     Thread SLPThread(registerPegasusWithSLP,0,true);
     SLPThread.run();
#else
    // Create a separate thread, detach and call function to execute the
    // startup.
    Thread t( _callSLPProvider, 0, true );
    t.run();
#endif

    PEG_METHOD_EXIT();
    return;
}


// startSLPProvider is a function to get the slp provider kicked off
// during startup.  It is placed in the provider manager simply because
// the provider manager is the only component of the system is
// driven by a timer after startup.  It should never be here and must be
// moved to somewhere more logical or really replaced. We simply needed
// something that was run shortly after system startup.
// This function is assumed to operate in a separate thread and
// KS 15 February 2004.

ThreadReturnType PEGASUS_THREAD_CDECL _callSLPProvider(void* parm)
{
    PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::_callSLPProvider()");

    //
    // Create CIMClient object
    //
    CIMClient client;

    //
    // open connection to CIMOM
    //
    String hostStr = System::getHostName();

    try
    {
        //
        client.connectLocal();

        //
        // set client timeout to 2 seconds
        //
        client.setTimeout(40000);
        // construct CIMObjectPath
        //
        String referenceStr = "//";
        referenceStr.append(hostStr);
        referenceStr.append("/");
        referenceStr.append(PEGASUS_NAMESPACENAME_INTERNAL.getString());
        referenceStr.append(":");
        referenceStr.append(PEGASUS_CLASSNAME_WBEMSLPTEMPLATE.getString());
        CIMObjectPath reference(referenceStr);

        //
        // issue the invokeMethod request on the register method
        //
        Array<CIMParamValue> inParams;
        Array<CIMParamValue> outParams;

        CIMValue retValue = client.invokeMethod(
            PEGASUS_NAMESPACENAME_INTERNAL,
            reference,
            CIMName("register"),
            inParams,
            outParams);

        _slpRegistrationComplete = true;

        Logger::put_l(
            Logger::STANDARD_LOG, System::CIMSERVER, Logger::INFORMATION,
            "Pegasus.Server.SLP.SLP_REGISTRATION_INITIATED",
            "SLP Registration Initiated");
    }

    catch(Exception& e)
    {
        Logger::put_l(Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
            "Pegasus.Server.SLP.INTERNAL_SLP_REGISTRATION_FAILED_EXCEPTION",
            "CIM Server registration with Internal SLP Failed. Exception: $0",
            e.getMessage());
    }

    catch(...)
    {
        Logger::put_l(Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
            "Pegasus.Server.SLP.INTERNAL_SLP_REGISTRATION_FAILED_ERROR",
            "CIM Server registration with Internal SLP Failed.");
    }

    client.disconnect();

    PEG_METHOD_EXIT();
    return( (ThreadReturnType)32 );
}

#ifdef PEGASUS_SLP_REG_TIMEOUT
// This thread advertises pegasus to a listening SA. The attributes for
// the Pegasus advertisement is obtained from CIM classes with the help
// of SLPAttrib class methods.
ThreadReturnType PEGASUS_THREAD_CDECL registerPegasusWithSLP(void* parm)
{

     PEG_METHOD_ENTER(TRC_SERVER, "CIMServer::registerPegasusWithSLP()");
     Boolean foundHttpProtocol=false, foundHttpsProtocol=false;
     SLPAttrib SLPHttpAttribObj;
     SLPAttrib SLPHttpsAttribObj;
     struct slp_client *client;
     const char *scopes = "DEFAULT";
     Uint16 life = PEGASUS_SLP_REG_TIMEOUT * 60, port=SLP_DEFAULT_PORT;
     const char *addr = LOCALHOST_IP;
     const char *iface = NULL;
     CString type, httpUrl, httpsUrl, httpAttrs, httpsAttrs;

     try
     {
         // Get all the SLP attributes and data for the Pegasus cimserver.
         foundHttpProtocol = SLPHttpAttribObj.fillData("http");
         foundHttpsProtocol = SLPHttpsAttribObj.fillData("https");
         if (!foundHttpProtocol && !foundHttpsProtocol)
         {
             Logger::put_l(
                 Logger::STANDARD_LOG, 
                 System::CIMSERVER, 
                 Logger::WARNING,
                 "Pegasus.Server.SLP.PROTOCOLS_NOT_ENABLED",
                 "Both Http and Https protocols are disabled, "
                     "SLP registration skipped.");
             PEG_METHOD_EXIT();
             return( (ThreadReturnType)32 );
         }

         // Populate datastructures required for registering a service with SLP
         if (foundHttpProtocol)
         {
             SLPHttpAttribObj.formAttributes();
             type = SLPHttpAttribObj.getServiceType().getCString();
             httpUrl = SLPHttpAttribObj.getServiceUrl().getCString();
             httpAttrs = SLPHttpAttribObj.getAttributes().getCString();
         }
         if (foundHttpsProtocol)
         {
             SLPHttpsAttribObj.formAttributes();
             if (!foundHttpProtocol)
             {
                 type = SLPHttpsAttribObj.getServiceType().getCString();
             }
             httpsUrl = SLPHttpsAttribObj.getServiceUrl().getCString();
             httpsAttrs = SLPHttpsAttribObj.getAttributes().getCString();
         }

         if (NULL != (client = create_slp_client(addr,
                                                 iface,
                                                 SLP_DEFAULT_PORT,
                                                 "DSA",
                                                 scopes,
                                                 FALSE,
                                                 FALSE)))
         {
             if (foundHttpProtocol &&
                 (!client->srv_reg_local(client, 
                                         (const char*)httpUrl,
                                         (const char*)httpAttrs, 
                                         (const char*)type, 
                                         scopes, 
                                         life)))
             {
                 Logger::put_l(
                     Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
                     "Pegasus.Server.SLP."
                         "EXTERNAL_SLP_REGISTRATION_FAILED_ERROR",
                     "CIM Server registration with External SLP Failed.");
             }

             if (foundHttpsProtocol &&
                 (!client->srv_reg_local(client, 
                                         (const char*)httpsUrl,
                                         (const char*)httpsAttrs, 
                                         (const char*)type, 
                                         scopes, 
                                         life)))
             {
                 Logger::put_l(
                     Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
                     "Pegasus.Server.SLP."
                         "EXTERNAL_SLP_REGISTRATION_FAILED_ERROR",
                     "CIM Server registration with External SLP Failed.");
             }
         }
     }

     catch(Exception& e)
     {
         Logger::put_l(
             Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
             "Pegasus.Server.SLP.EXTERNAL_SLP_REGISTRATION_FAILED_EXCEPTION",
             "CIM Server registration with External SLP Failed. Exception: $0",
             e.getMessage());
     }

     catch(...)
     {
         Logger::put_l(
             Logger::STANDARD_LOG, System::CIMSERVER, Logger::WARNING,
             "Pegasus.Server.SLP.EXTERNAL_SLP_REGISTRATION_FAILED_ERROR",
             "CIM Server registration with External SLP Failed.");
     }

     destroy_slp_client(client);

    PEG_METHOD_EXIT();
    return (ThreadReturnType)32;
}

// This routine deregisters the CIM Server registration with external SLP SA.

void PEGASUS_SERVER_LINKAGE unregisterPegasusFromSLP()
{
    PEG_METHOD_ENTER(TRC_SERVER, "unregisterPegasusFromSLP()");
    Boolean foundHttpProtocol=false, foundHttpsProtocol=false;
    SLPAttrib SLPHttpAttribObj;
    SLPAttrib SLPHttpsAttribObj;
    struct slp_client *client;
    const char *scopes = "DEFAULT";
    Uint16 port=SLP_DEFAULT_PORT;
    const char *addr = LOCALHOST_IP;
    const char *iface = NULL;
    CString type, httpUrl, httpsUrl, httpAttrs, httpsAttrs;

    // If Pegasus did not successfully register with SLP, just return
    if (!_slpRegistrationComplete)
    {
        PEG_METHOD_EXIT();
        return;
    }

    // Get all the SLP attributes and data for the Pegasus cimserver.
    foundHttpProtocol = SLPHttpAttribObj.fillData("http");
    foundHttpsProtocol = SLPHttpsAttribObj.fillData("https");
    if (!foundHttpProtocol && !foundHttpsProtocol)
    {
        PEG_METHOD_EXIT();
        return;
    }
    if (foundHttpProtocol)
    {
        SLPHttpAttribObj.formAttributes();
        type = SLPHttpAttribObj.getServiceType().getCString();
        httpUrl = SLPHttpAttribObj.getServiceUrl().getCString();
        httpAttrs = SLPHttpAttribObj.getAttributes().getCString();
    }
    if (foundHttpsProtocol)
    {
        SLPHttpsAttribObj.formAttributes();
        if (!foundHttpProtocol)
        {
            type = SLPHttpsAttribObj.getServiceType().getCString();
        }
        httpsUrl = SLPHttpsAttribObj.getServiceUrl().getCString();
        httpsAttrs = SLPHttpsAttribObj.getAttributes().getCString();
    }

    if (NULL != (client = create_slp_client(addr,
                                            iface,
                                            SLP_DEFAULT_PORT,
                                            "DSA",
                                            scopes,
                                            FALSE,
                                            FALSE)))
    {
        if (foundHttpProtocol)
        {
            client->srv_reg_local(
                client,
                (const char*)httpUrl,
                (const char*)httpAttrs,
                (const char*)type,
                scopes,
                0);
        }

        if (foundHttpsProtocol)
        {
            client->srv_reg_local(
                client,
                (const char*)httpsUrl,
                (const char*)httpsAttrs,
                (const char*)type,
                scopes,
                0);
        }

        destroy_slp_client(client);
    }

    PEG_METHOD_EXIT();
    return;
 }
#endif // PEGASUS_SLP_REG_TIMEOUT
#endif

PEGASUS_NAMESPACE_END
