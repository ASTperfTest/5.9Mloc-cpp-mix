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
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_IndicationConstants_h
#define Pegasus_IndicationConstants_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/CIMName.h>

PEGASUS_NAMESPACE_BEGIN

/**
    Values for the Repeat Notification Policy property of the Subscription
    class, as defined in the CIM Events MOF
 */
enum RepeatNotificationPolicy {_POLICY_UNKNOWN = 0, _POLICY_OTHER = 1,
     _POLICY_NONE = 2, _POLICY_SUPPRESS = 3, _POLICY_DELAY = 4};

/**
    Values for the On Fatal Error Policy property of the Subscription
    class, as defined in the CIM Events MOF
 */
enum OnFatalErrorPolicy {_ERRORPOLICY_OTHER = 1, _ERRORPOLICY_IGNORE = 2,
    _ERRORPOLICY_DISABLE = 3, _ERRORPOLICY_REMOVE = 4};

/**
    Values for the Probable Cause property of the Alert Indication
    class, as defined in the CIM Events MOF
    Note: not all possible values have been included
 */
enum ProbableCause {_CAUSE_UNKNOWN = 0, _CAUSE_OTHER = 1};

/**
    Values for the Alert Type property of the Alert Indication class,
    as defined in the CIM Events MOF
 */
enum AlertType {_TYPE_OTHER = 1, _TYPE_COMMUNICATIONS = 2, _TYPE_QOS = 3,
     _TYPE_PROCESSING = 4, _TYPE_DEVICE = 5, _TYPE_ENVIRONMENTAL = 6,
     _TYPE_MODELCHANGE = 7, _TYPE_SECURITY = 8};


//
//  Class names
//

/**
    The name of the CIMOM Shutdown alert indication class
 */
//
//  ATTN: Update once CimomShutdownAlertIndication has been defined
//
extern const CIMName _CLASS_CIMOM_SHUTDOWN_ALERT;

/**
    The name of the No Provider alert indication class
 */
//
//  ATTN: Update once NoProviderAlertIndication has been defined
//
extern const CIMName _CLASS_NO_PROVIDER_ALERT;

/**
    The name of the CIMOM shutdown alert indication class
 */
//
//  ATTN: Update once ProviderTerminatedAlertIndication has been defined
//
extern const CIMName _CLASS_PROVIDER_TERMINATED_ALERT;


//
//  Property names
//

/**
    The name of the Other Subscription State property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_OTHERSTATE;

/**
    The name of the Repeat Notification Policy property for indication
    subscription class
 */
extern const CIMName _PROPERTY_REPEATNOTIFICATIONPOLICY;

/**
    The name of the Other Repeat Notification Policy property for
    indication subscription class
 */
extern const CIMName _PROPERTY_OTHERREPEATNOTIFICATIONPOLICY;

/**
    The name of the On Fatal Error Policy property for Indication Subscription
    class
 */
extern const CIMName _PROPERTY_ONFATALERRORPOLICY;

/**
    The name of the Other On Fatal Error Policy property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_OTHERONFATALERRORPOLICY;

/**
    The name of the Failure Trigger Time Interval property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_FAILURETRIGGERTIMEINTERVAL;

/**
    The name of the Time Of Last State Change property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_LASTCHANGE;

/**
    The name of the Subscription Start Time property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_STARTTIME;

/**
    The name of the Subscription Duration property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_DURATION;

/**
    The name of the Subscription Time Remaining property for Indication
    Subscription class
 */
extern const CIMName _PROPERTY_TIMEREMAINING;

/**
    The name of the Repeat Notification Interval property for indication
    subscription class
 */
extern const CIMName _PROPERTY_REPEATNOTIFICATIONINTERVAL;

/**
    The name of the Repeat Notification Gap property for indication
    subscription class
 */
extern const CIMName _PROPERTY_REPEATNOTIFICATIONGAP;

/**
    The name of the Repeat Notification Count property for indication
    subscription class
 */
extern const CIMName _PROPERTY_REPEATNOTIFICATIONCOUNT;

/**
    The name of the TextFormatOwningEntity property for Formatted Indication
    Subscription class
*/
extern const CIMName _PROPERTY_TEXTFORMATOWNINGENTITY;

/**
    The name of the TextFormatID property for Formatted Indication
    Subscription class
*/
extern const CIMName _PROPERTY_TEXTFORMATID;

/**
    The name of the Caption property for Managed Element class
 */
extern const CIMName _PROPERTY_CAPTION;

/**
    The name of the Description property for Managed Element class
 */
extern const CIMName _PROPERTY_DESCRIPTION;

/**
    The name of the ElementName property for Managed Element class
 */
extern const CIMName _PROPERTY_ELEMENTNAME;

/**
    The name of the Source Namespace property for indication filter class
 */
extern const CIMName _PROPERTY_SOURCENAMESPACE;

/**
    The name of the System Name property for indication filter and indications
    handler classes
 */
extern const CIMName _PROPERTY_SYSTEMNAME;

/**
    The name of the System Creation Class Name property for indication filter
    and indications handler classes
 */
extern const CIMName _PROPERTY_SYSTEMCREATIONCLASSNAME;

/**
    The name of the Other Persistence Type property for Indication Handler
    class
 */
extern const CIMName _PROPERTY_OTHERPERSISTENCETYPE;

/**
    The name of the Owner property for Indication Handler class
 */
extern const CIMName _PROPERTY_OWNER;

/**
    The name of the TargetHostFormat property for SNMP Mapper Indication
    Handler subclass
 */
extern const CIMName _PROPERTY_TARGETHOSTFORMAT;

/**
    The name of the OtherTargetHostFormat property for SNMP Mapper Indication
    Handler subclass
 */
extern const CIMName _PROPERTY_OTHERTARGETHOSTFORMAT;

/**
    The name of the Port Number property for SNMP Mapper Indication Handler
    subclass
 */
extern const CIMName _PROPERTY_PORTNUMBER;

/**
    The name of the SNMP Security Name property for SNMP Mapper Indication
    Handler subclass
 */
extern const CIMName _PROPERTY_SNMPSECURITYNAME;

/**
    The name of the SNMP Engine ID property for SNMP Mapper Indication Handler
    subclass
 */
extern const CIMName _PROPERTY_SNMPENGINEID;

/**
    The name of the Alert Type property for Alert Indication class
 */
extern const CIMName _PROPERTY_ALERTTYPE;

/**
    The name of the Other Alert Type property for Alert Indication class
 */
extern const CIMName _PROPERTY_OTHERALERTTYPE;

/**
    The name of the Perceived Severity property for Alert Indication class
 */
extern const CIMName _PROPERTY_PERCEIVEDSEVERITY;

/**
    The name of the Probable Cause property for Alert Indication class
 */
extern const CIMName _PROPERTY_PROBABLECAUSE;

//
//  Qualifier names
//

/**
    The name of the Indication qualifier for classes
 */
extern const CIMName _QUALIFIER_INDICATION;


//
//  Other literal values
//

/**
    The WHERE keyword in WQL
 */
extern const char _QUERY_WHERE[];

/**
    A zero value CIMDateTime interval
 */
extern const char _ZERO_INTERVAL_STRING[];

PEGASUS_NAMESPACE_END

#endif  /* Pegasus_IndicationConstants_h */
