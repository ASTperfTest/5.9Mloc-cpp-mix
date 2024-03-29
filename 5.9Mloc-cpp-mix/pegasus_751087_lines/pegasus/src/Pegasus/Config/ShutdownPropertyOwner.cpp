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


///////////////////////////////////////////////////////////////////////////////
//
// This file has implementation for the timeout property owner class.
//
///////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Constants.h>
#include "ShutdownPropertyOwner.h"


PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
//  ShutdownPropertyOwner
//
//  The shutdownTimeout property is the timeout value in seconds that the
//  cimom uses to wait for all the outstanding CIM operations to complete
//  before shutting down the cimserver.  The default value is 30 seconds.
//
//  When a new timeout property is added, make sure to add the property name
//  and the default attributes of that property in the table below.
//
///////////////////////////////////////////////////////////////////////////////

static struct ConfigPropertyRow properties[] =
{
    {"shutdownTimeout", PEGASUS_DEFAULT_SHUTDOWN_TIMEOUT_SECONDS_STRING,
         IS_DYNAMIC, 0, 0, IS_VISIBLE},
};

const Uint32 NUM_PROPERTIES = sizeof(properties) / sizeof(properties[0]);

static long MIN_SHUTDOWN_TIMEOUT = 2;

/** Constructor  */
ShutdownPropertyOwner::ShutdownPropertyOwner()
{
    _shutdownTimeout.reset(new ConfigProperty);
}


/**
    Initialize the config properties.
*/
void ShutdownPropertyOwner::initialize()
{
    for (Uint32 i = 0; i < NUM_PROPERTIES; i++)
    {
        //
        // Initialize the properties with default values
        //
        if (String::equalNoCase(properties[i].propertyName, "shutdownTimeout"))
        {
            _shutdownTimeout->propertyName = properties[i].propertyName;
            _shutdownTimeout->defaultValue = properties[i].defaultValue;
            _shutdownTimeout->currentValue = properties[i].defaultValue;
            _shutdownTimeout->plannedValue = properties[i].defaultValue;
            _shutdownTimeout->dynamic = properties[i].dynamic;
            _shutdownTimeout->domain = properties[i].domain;
            _shutdownTimeout->domainSize = properties[i].domainSize;
            _shutdownTimeout->externallyVisible =
                properties[i].externallyVisible;
        }
    }
}

struct ConfigProperty* ShutdownPropertyOwner::_lookupConfigProperty(
    const String& name) const
{
    if (String::equalNoCase(_shutdownTimeout->propertyName, name))
    {
        return _shutdownTimeout.get();
    }
    else
    {
        throw UnrecognizedConfigProperty(name);
    }
}

/**
    Get information about the specified property.
*/
void ShutdownPropertyOwner::getPropertyInfo(
    const String& name,
    Array<String>& propertyInfo) const
{
    propertyInfo.clear();
    struct ConfigProperty * configProperty = _lookupConfigProperty(name);

    propertyInfo.append(configProperty->propertyName);
    propertyInfo.append(configProperty->defaultValue);
    propertyInfo.append(configProperty->currentValue);
    propertyInfo.append(configProperty->plannedValue);
    if (configProperty->dynamic)
    {
        propertyInfo.append(STRING_TRUE);
    }
    else
    {
        propertyInfo.append(STRING_FALSE);
    }
    if (configProperty->externallyVisible)
    {
        propertyInfo.append(STRING_TRUE);
    }
    else
    {
        propertyInfo.append(STRING_FALSE);
    }
}

/**
    Get default value of the specified property.
*/
String ShutdownPropertyOwner::getDefaultValue(const String& name) const
{
    struct ConfigProperty * configProperty = _lookupConfigProperty(name);
    return configProperty->defaultValue;
}

/**
    Get current value of the specified property.
*/
String ShutdownPropertyOwner::getCurrentValue(const String& name) const
{
    struct ConfigProperty * configProperty = _lookupConfigProperty(name);
    return configProperty->currentValue;
}

/**
    Get planned value of the specified property.
*/
String ShutdownPropertyOwner::getPlannedValue(const String& name) const
{
    struct ConfigProperty * configProperty = _lookupConfigProperty(name);
    return configProperty->plannedValue;
}

/**
    Init current value of the specified property to the specified value.
*/
void ShutdownPropertyOwner::initCurrentValue(
    const String& name,
    const String& value)
{
    struct ConfigProperty* configProperty = _lookupConfigProperty(name);
    configProperty->currentValue = value;
}


/**
    Init planned value of the specified property to the specified value.
*/
void ShutdownPropertyOwner::initPlannedValue(
    const String& name,
    const String& value)
{
    struct ConfigProperty* configProperty = _lookupConfigProperty(name);
    configProperty->plannedValue = value;
}

/**
    Update current value of the specified property to the specified value.
*/
void ShutdownPropertyOwner::updateCurrentValue(
    const String& name,
    const String& value)
{
    //
    // make sure the property is dynamic before updating the value.
    //
    if (!isDynamic(name))
    {
        throw NonDynamicConfigProperty(name);
    }

    //
    // Since the validations done in initCurrrentValue are sufficient and
    // no additional validations required for update, we will call
    // initCurrrentValue.
    //
    initCurrentValue(name, value);
}


/**
    Update planned value of the specified property to the specified value.
*/
void ShutdownPropertyOwner::updatePlannedValue(
    const String& name,
    const String& value)
{
    //
    // Since the validations done in initPlannedValue are sufficient and
    // no additional validations required for update, we will call
    // initPlannedValue.
    //
    initPlannedValue(name, value);
}

/**
    Checks to see if the given value is valid or not.
*/
Boolean ShutdownPropertyOwner::isValid(
    const String& name,
    const String& value) const
{
    //
    // convert timeout string to integer
    //
    long timeoutValue = strtol(value.getCString(), (char **)0, 10);

    if (String::equalNoCase(_shutdownTimeout->propertyName, name))
    {
        // Check if the timeout value is greater than the minimum allowed
        //
        if ( timeoutValue > MIN_SHUTDOWN_TIMEOUT )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        throw UnrecognizedConfigProperty(name);
    }
}

/**
    Checks to see if the specified property is dynamic or not.
*/
Boolean ShutdownPropertyOwner::isDynamic(const String& name) const
{
    struct ConfigProperty* configProperty = _lookupConfigProperty(name);
    return (configProperty->dynamic == IS_DYNAMIC);
}

PEGASUS_NAMESPACE_END
