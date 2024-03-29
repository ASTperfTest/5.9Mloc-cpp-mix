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

#include "ConsumerModule.h"

#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/MessageLoader.h> //l10n
#include <Pegasus/Common/Tracer.h>
#include <Pegasus/Common/ThreadPool.h>

#include <Pegasus/Provider/CIMIndicationConsumerProvider.h>
#include <Pegasus/DynListener/ConsumerManager.h>

PEGASUS_USING_STD;
PEGASUS_NAMESPACE_BEGIN


ConsumerModule::ConsumerModule()
{
}

ConsumerModule::~ConsumerModule(void)
{
}

// The caller assumes the repsonsibility of making sure the libraryPath is correctly formatted
CIMIndicationConsumerProvider* ConsumerModule::load(const String & consumerName, const String & libraryPath)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerModule::load");

    //check whether the module is cached; if it's not already in memory, load it
    if (!_library.isLoaded())
    {
        if (!FileSystem::exists(libraryPath) || !FileSystem::canRead(libraryPath))
        {
            throw Exception(MessageLoaderParms("DynListener.ConsumerModule.INVALID_LIBRARY_PATH",
               "The library ($0:$1) does not exist or cannot be read.",
               libraryPath,
               consumerName));
        }

        _library = DynamicLibrary(libraryPath);
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Loading library: " + consumerName);

    if (!_library.load())
    {
        throw Exception(MessageLoaderParms("DynListener.ConsumerModule.CANNOT_LOAD_LIBRARY",
                                   "Cannot load consumer library ($0:$1), load error $2",
                                   _library.getFileName(),
                                   consumerName,
                                   _library.getLoadErrorMessage()));
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Successfully loaded library " + consumerName);

    // locate the entry point
    CIMProvider* (*createProvider)(const String&) =
        (CIMProvider* (*)(const String&))
            _library.getSymbol("PegasusCreateProvider");

    if (!createProvider)
    {
        _library.unload();
        throw Exception(MessageLoaderParms("DynListener.ConsumerModule.ENTRY_POINT_NOT_FOUND",
               "The entry point for consumer library ($0:$1) cannot be found.",
               libraryPath,
               consumerName));
    }

    // create the consumer provider
    CIMProvider* providerRef = createProvider(consumerName);

    if(!providerRef)
    {
        _library.unload();
        throw Exception(MessageLoaderParms("DynListener.ConsumerModule.CREATE_PROVIDER_FAILED",
               "createProvider failed for consumer library ($0:$1)",
               libraryPath,
               consumerName));
    }

    // test for the appropriate interface
    CIMIndicationConsumerProvider* consumerRef = dynamic_cast<CIMIndicationConsumerProvider *>(providerRef);
    if(!consumerRef)
    {
        _library.unload();
        throw Exception(MessageLoaderParms("DynListener.ConsumerModule.CONSUMER_IS_NOT_A",
            "Consumer ($0:$1) is not a CIMIndicationConsumerProvider.",
            libraryPath,
            consumerName));
    }

    PEG_METHOD_EXIT();
    return consumerRef;
}

void ConsumerModule::unloadModule(void)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerModule::unloadModule");

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3,
        "Unloading module " + _library.getFileName());
    _library.unload();

    PEG_METHOD_EXIT();
}

PEGASUS_NAMESPACE_END

