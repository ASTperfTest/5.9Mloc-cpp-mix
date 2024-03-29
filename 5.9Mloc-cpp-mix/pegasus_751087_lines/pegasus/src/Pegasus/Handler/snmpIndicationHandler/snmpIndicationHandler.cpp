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
#include <Pegasus/Common/PegasusVersion.h>

#include <iostream>
#include <Pegasus/Handler/CIMHandler.h>
#include <Pegasus/Repository/CIMRepository.h>
#include <Pegasus/Common/Tracer.h>

#include "snmpIndicationHandler.h"

#ifdef HPUX_EMANATE
# include "snmpDeliverTrap_emanate.h"
#elif defined (PEGASUS_USE_NET_SNMP)
# include "snmpDeliverTrap_netsnmp.h"
#else
# include "snmpDeliverTrap_stub.h"
#endif

#include <Pegasus/Common/MessageLoader.h>

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

snmpIndicationHandler::snmpIndicationHandler()
{
    PEG_METHOD_ENTER (TRC_IND_HANDLER,
        "snmpIndicationHandler::snmpIndicationHandler");

#ifdef HPUX_EMANATE
    _snmpTrapSender = new snmpDeliverTrap_emanate();
#elif defined (PEGASUS_USE_NET_SNMP)
    _snmpTrapSender = new snmpDeliverTrap_netsnmp();
#else
    _snmpTrapSender = new snmpDeliverTrap_stub();
#endif

    PEG_METHOD_EXIT();
}

void snmpIndicationHandler::initialize(CIMRepository* repository)
{
    PEG_METHOD_ENTER (TRC_IND_HANDLER,
        "snmpIndicationHandler::initialize");

    _repository = repository;

    _snmpTrapSender->initialize();

    PEG_METHOD_EXIT();
}

void snmpIndicationHandler::terminate()
{
    PEG_METHOD_ENTER(TRC_IND_HANDLER,
        "snmpIndicationHandler::terminate");

    _snmpTrapSender->terminate();

    PEG_METHOD_EXIT();
}

snmpIndicationHandler::~snmpIndicationHandler()
{
    PEG_METHOD_ENTER(TRC_IND_HANDLER,
        "snmpIndicationHandler::~snmpIndicationHandler");

    delete _snmpTrapSender;

    PEG_METHOD_EXIT();
}

// l10n - note: ignoring indication language
void snmpIndicationHandler::handleIndication(
    const OperationContext& context,
    const String nameSpace,
    CIMInstance& indication,
    CIMInstance& handler,
    CIMInstance& subscription,
    ContentLanguageList & contentLanguages)
{
    Array<String> propOIDs;
    Array<String> propTYPEs;
    Array<String> propVALUEs;

    Array<String> mapStr;

    PEG_METHOD_ENTER(TRC_IND_HANDLER,
        "snmpIndicationHandler::handleIndication");

    try
    {
        PEG_TRACE ((TRC_INDICATION_GENERATION, Tracer::LEVEL3,
            "snmpIndicationHandler %s:%s.%s processing %s Indication",
           (const char*)(nameSpace.getCString()),
           (const char*)(handler.getClassName().getString().getCString()),
           (const char*)(handler.getProperty(
           handler.findProperty(PEGASUS_PROPERTYNAME_NAME)).
           getValue().toString().getCString()),
           (const char*)(indication.getClassName().getString().
           getCString())));
        CIMClass indicationClass = _repository->getClass(
            nameSpace, indication.getClassName(), false, true,
            false, CIMPropertyList());

        Uint32 propertyCount = indication.getPropertyCount();

        for (Uint32 i=0; i < propertyCount; i++)
        {
            CIMProperty prop = indication.getProperty(i);

            Uint32 propDeclPos = indicationClass.findProperty(prop.getName());
            if (propDeclPos != PEG_NOT_FOUND)
            {
                CIMProperty propDecl = indicationClass.getProperty(propDeclPos);

                Uint32 qualifierPos =
                    propDecl.findQualifier(CIMName("MappingStrings"));
                if (qualifierPos != PEG_NOT_FOUND)
                {
                    //
                    // We are looking for following fields:
                    // MappingStrings {"OID.IETF | SNMP." oidStr, 
                    //     "DataType.IETF |" dataType}
                    // oidStr is the object identifier (e.g. "1.3.6.1.2.1.5..."
                    // dataType is either Integer, or OctetString, 
                    // or OID
                    // Following is one example:
                    // MappingStrings {"OID.IETF | SNMP.1.3.6.6.3.1.1.5.2",
                    //    "DataType.IETF | Integer"}
                    //

                    propDecl.getQualifier(qualifierPos).getValue().get(
                        mapStr);
 
                    String oidStr, dataType;
                    String mapStr1, mapStr2;
                    Boolean isValidAuthority = false;
                    Boolean isValidDataType = false;

                    for (Uint32 j=0; j < mapStr.size(); j++)
                    {
                        Uint32 barPos = mapStr[j].find("|");
                            
                        if (barPos != PEG_NOT_FOUND) 
                        {
                            mapStr1 = mapStr[j].subString(0, barPos);
                            mapStr2 = mapStr[j].subString(barPos + 1);

                            _trimWhitespace(mapStr1);
                            _trimWhitespace(mapStr2);
                                
                            if ((mapStr1 == "OID.IETF") &&
                                (String::compare(mapStr2, 
                                 String("SNMP."), 5) == 0))
                            {
                                isValidAuthority = true;
                                oidStr = mapStr2.subString(5);
                            }
                            else if (mapStr1 == "DataType.IETF")
                            {
                                isValidDataType = true;
                                dataType = mapStr2;
                            }

                            if (isValidAuthority && isValidDataType) 
                            {
                                propOIDs.append(oidStr);
                                propTYPEs.append(dataType);
                                propVALUEs.append(prop.getValue().toString());

                                break;
                            }
                        }
                    }
                }
            }
        }

        // Collected complete data in arrays and ready to send the trap.
        // trap destination and SNMP type are defined in handlerInstance
        // and passing this instance as it is to deliverTrap() call

        Uint32 targetHostPos = handler.findProperty(CIMName("TargetHost"));
        Uint32 targetHostFormatPos =
            handler.findProperty(CIMName("TargetHostFormat"));
        Uint32 otherTargetHostFormatPos =
            handler.findProperty(CIMName("OtherTargetHostFormat"));
        Uint32 portNumberPos = handler.findProperty(CIMName("PortNumber"));
        Uint32 snmpVersionPos = handler.findProperty(CIMName("SNMPVersion"));
        Uint32 securityNamePos =
            handler.findProperty(CIMName("SNMPSecurityName"));
        Uint32 engineIDPos = handler.findProperty(CIMName("SNMPEngineID"));

        if (targetHostPos == PEG_NOT_FOUND)
        {
            PEG_TRACE((TRC_DISCARDED_DATA, Tracer::LEVEL3,
                "Target host is not set for IndicationHandlerSNMPMapper %s"
                " Indication.",
                (const char*)(indication.getClassName().getString().
                getCString())));
            PEG_METHOD_EXIT();
            throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED, MessageLoaderParms(
                "Handler.snmpIndicationHandler.snmpIndicationHandler."
                "INVALID_SNMP_INSTANCE",
                "Invalid IndicationHandlerSNMPMapper instance"));
        }
        if (targetHostFormatPos == PEG_NOT_FOUND)
        {
            PEG_TRACE((TRC_DISCARDED_DATA, Tracer::LEVEL3,
                "Target host format is not set for IndicationHandlerSNMPMapper"
                " %s Indication.",
                (const char*)(indication.getClassName().getString().
                getCString())));
            PEG_METHOD_EXIT();
            throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED, MessageLoaderParms(
                "Handler.snmpIndicationHandler.snmpIndicationHandler."
                "INVALID_SNMP_INSTANCE",
                "Invalid IndicationHandlerSNMPMapper instance"));
        }
        if (snmpVersionPos == PEG_NOT_FOUND)
        {
            PEG_TRACE((TRC_DISCARDED_DATA, Tracer::LEVEL3,
                "SNMP Version is not set for IndicationHandlerSNMPMapper %s"
                " Indication.",
                (const char*)(indication.getClassName().getString().
                getCString())));
            PEG_METHOD_EXIT();
            throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED, MessageLoaderParms(
                "Handler.snmpIndicationHandler.snmpIndicationHandler."
                "INVALID_SNMP_INSTANCE",
                "Invalid IndicationHandlerSNMPMapper instance"));
        }
        else
        {
            // properties from the handler instance
            String targetHost;
            String otherTargetHostFormat = String();
            String securityName = String();
            String engineID = String();
            Uint16 targetHostFormat = 0;
            Uint16 snmpVersion = 0;
            Uint32 portNumber;

            String trapOid;
            Boolean trapOidAvailable = false;
            String exceptionStr;
            //
            //  Get snmpTrapOid from context
            //
            if (context.contains(SnmpTrapOidContainer::NAME))
            {
                SnmpTrapOidContainer trapContainer =
                    context.get(SnmpTrapOidContainer::NAME);

                trapOid = trapContainer.getSnmpTrapOid();
                trapOidAvailable = true;
            }
            else
            {
                // get trapOid from indication Class

                Uint32 pos =
                    indicationClass.findQualifier(CIMName("MappingStrings"));
                if (pos != PEG_NOT_FOUND)
                {
                    Array<String> classMapStr;
                    indicationClass.getQualifier(pos).getValue().
                        get(classMapStr);

                    for (Uint32 i=0; i < classMapStr.size(); i++)
                    {
                        Uint32 barPos = classMapStr[i].find("|");

                        if (barPos != PEG_NOT_FOUND) 
                        {
                            String authorityName = 
                                classMapStr[i].subString(0, barPos);
                            String oidStr = classMapStr[i].subString(
                                barPos+1, PEG_NOT_FOUND);

                            _trimWhitespace(authorityName);
                            _trimWhitespace(oidStr);

                            if ((authorityName == "OID.IETF") &&
                                (String::compare(oidStr, 
                                 String("SNMP."), 5) == 0))
                            {
                                trapOid = oidStr.subString(5); 
                                trapOidAvailable = true;
                                break;
                            }
                        }
                    }

                    if (!trapOidAvailable)
                    {
                        exceptionStr = "No MappingStrings for snmp trap"
                            "is specified for class: ";
                      
                        exceptionStr.append(
                            indication.getClassName().getString());

                        PEG_TRACE_STRING(TRC_IND_HANDLER, Tracer::LEVEL4,
                            exceptionStr);
                        PEG_METHOD_EXIT();

                        throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED,
                            MessageLoaderParms(
                                "Handler.snmpIndicationHandler."
                                "snmpIndicationHandler.NO_MS_FOR_SNMP_TRAP",
                                exceptionStr));
                    }
                }
                else
                {
                    PEG_TRACE_CSTRING(TRC_IND_HANDLER, Tracer::LEVEL4,
                        "Qualifier MappingStrings can not be found.");
                    PEG_METHOD_EXIT();
                    MessageLoaderParms parms(
                        "Handler.snmpIndicationHandler.snmpIndicationHandler."
                            "QUALIFIER_MAPPINGS_NOT_FOUND",
                        "Qualifier MappingStrings can not be found");
                    throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED, parms);
                }
            }

            handler.getProperty(targetHostPos).getValue().get(targetHost);
            handler.getProperty(targetHostFormatPos).getValue().get(
                targetHostFormat);
            if (otherTargetHostFormatPos != PEG_NOT_FOUND)
            {
                handler.getProperty(otherTargetHostFormatPos).getValue().get(
                    otherTargetHostFormat);
            }
            if (portNumberPos != PEG_NOT_FOUND)
            {
                handler.getProperty(portNumberPos).getValue().get(portNumber);
            }
            else
            {
                // default port
                portNumber = SNMP_TRAP_DEFAULT_PORT;
            }

            handler.getProperty(snmpVersionPos).getValue().get(snmpVersion);
            if (securityNamePos != PEG_NOT_FOUND)
            {
                handler.getProperty(securityNamePos).getValue().get(
                    securityName);
            }
            if (engineIDPos != PEG_NOT_FOUND)
            {
                handler.getProperty(engineIDPos).getValue().get(engineID);
            }

            PEG_TRACE ((TRC_INDICATION_GENERATION, Tracer::LEVEL3,
               "snmpIndicationHandler sending %s Indication trap %s to target"
               " host %s target port %d",
               (const char*)(indication.getClassName().getString().
               getCString()),
               (const char*)(trapOid.getCString()),
               (const char*)(targetHost.getCString()),portNumber));
           _snmpTrapSender->deliverTrap(
                trapOid,
                securityName,
                targetHost,
                targetHostFormat,
                otherTargetHostFormat,
                portNumber,
                snmpVersion,
                engineID,
                propOIDs,
                propTYPEs,
                propVALUEs);

        PEG_TRACE ((TRC_INDICATION_GENERATION, Tracer::LEVEL3,
           "%s Indication trap %s sent to target host %s target port %d "
           "successfully",
           (const char*)(indication.getClassName().getString().getCString()),
           (const char*)(trapOid.getCString()),
           (const char*)(targetHost.getCString()),portNumber));
        }
    }
    catch (CIMException& c)
    {
        PEG_TRACE_STRING(TRC_IND_HANDLER, Tracer::LEVEL4, c.getMessage());
        PEG_METHOD_EXIT();
        throw PEGASUS_CIM_EXCEPTION(CIM_ERR_FAILED, c.getMessage());
    }
    catch (Exception& e)
    {
        PEG_TRACE_STRING(TRC_IND_HANDLER, Tracer::LEVEL4, e.getMessage());
        PEG_METHOD_EXIT();

        throw PEGASUS_CIM_EXCEPTION(CIM_ERR_FAILED, e.getMessage());
    }
    catch (...)
    {
        PEG_TRACE_CSTRING(TRC_IND_HANDLER, Tracer::LEVEL4,
            "Failed to deliver trap.");
        PEG_METHOD_EXIT();

        throw PEGASUS_CIM_EXCEPTION_L(CIM_ERR_FAILED, MessageLoaderParms(
            "Handler.snmpIndicationHandler.snmpIndicationHandler."
                "FAILED_TO_DELIVER_TRAP",
            "Failed to deliver trap."));
    }
    PEG_METHOD_EXIT();
}

void snmpIndicationHandler::_trimWhitespace(
    String & nameStr)
{
    PEG_METHOD_ENTER(TRC_IND_HANDLER,
        "snmpIndicationHandler::_trimWhitespace");

    Uint32 ps = 0;
    // skip begining whitespace
    for (ps = 0; ps < nameStr.size(); ps++)
    {
        if (nameStr[ps] != ' ')
        {
            break;
        }
    }

    if (ps != 0)
    {
        nameStr.remove(0, ps);
    }

    // skip the appended whitespace
    for (ps = nameStr.size(); ps != 0; ps--)
    {
        if (nameStr[ps-1] != ' ')
        {
            break;
        }
    }

    if (ps !=  nameStr.size())
    {
        nameStr.remove(ps);
    }

    PEG_METHOD_EXIT();
}

PEGASUS_NAMESPACE_END

PEGASUS_USING_PEGASUS;

// This is the entry point into this dynamic module.

extern "C" PEGASUS_EXPORT CIMHandler* PegasusCreateHandler(
    const String& handlerName)
{
    if (handlerName == "snmpIndicationHandler")
    {
        return new snmpIndicationHandler;
    }

    return 0;
}
