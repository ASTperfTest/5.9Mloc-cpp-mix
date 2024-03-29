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

#ifndef ClientPerfDataStore_h
#define ClientPerfDataStore_h

#include <Pegasus/Common/CIMOperationType.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/TimeValue.h>
#include <Pegasus/Common/Message.h>
#include <Pegasus/Client/Linkage.h>
#include <Pegasus/Client/ClientOpPerformanceDataHandler.h>

PEGASUS_NAMESPACE_BEGIN

class ClientOpPerformanceDataHandler;
struct ClientOpPerformanceData;

/** The ClientPerfDataStore class is internal to pegasus. It has no API's
    that client apps can access. It's purpose is to collect and calculate
    the data that is returned to the client app in the
    ClientOpPerformanceData object.
*/
class PEGASUS_CLIENT_LINKAGE ClientPerfDataStore
{
public:

    ClientPerfDataStore();

    /** Resets all the data members to 0
    */
    void reset();

    /** Creates a ClientOpPerformanceData from the current values of of the
        private data members
    */
    ClientOpPerformanceData createPerfDataStruct();

    /** Checks the currentMessageID and _operationType data members against
        @param messageID and @param type if the values are equal
        true is returned.
        @param messageID
        @param type CIM message type of current message
        @return true if @param messageID equals currentMessageID data member
    */
    Boolean checkMessageIDandType(const String& messageID, MessageType type);

    /** Sets the server time data member
    */
    void setServerTime(Uint32 time);

    /** Sets the responsSize data member
    */
    void setResponseSize(Uint64 size);

    /** Sets the _requestSize data member
    */
    void setRequestSize(Uint64 size);

    /** Sets startNetworkTime data member
    */
    void setStartNetworkTime();

    /** Sets endNetworkTime data member
    */
    void setEndNetworkTime(TimeValue time);

    /** Sets validServerTime data member
    */
    void setServerTimeKnown(Boolean bol);

    /** Sets _operationType data member by translating message type given by
        @param type, into a CIMOperationType. That value is used to set the
        _operationType data member and is returned.
        @param type integer repesenting the message type
    */
    void setOperationType(MessageType type);

    //void setErrorCondition(Boolean bol);

    String toString() const;

    void setMessageID(String messageID);

    Boolean getStatError() const;

    void setClassRegistered(Boolean bol);

    static ClientPerfDataStore *current_Store;

    ClientOpPerformanceDataHandler* handler_prt;

    Boolean isClassRegistered() const;

protected:
    CIMOperationType _operationType;
    Boolean _serverTimeKnown;
    Boolean _errorCondition;
    Boolean _classRegistered;
    Uint64 _serverTime;
    Uint64 _requestSize;
    Uint64 _responseSize;
    String _messID;
    TimeValue _networkEndTime;
    TimeValue _networkStartTime;

private:
    // Make copy-constructor and assignment operator private
    ClientPerfDataStore(const ClientPerfDataStore & dataStore);
    ClientPerfDataStore& operator=(const ClientPerfDataStore& dataStore);
};


PEGASUS_NAMESPACE_END

#endif /* ClientPerfDataStore_h */

