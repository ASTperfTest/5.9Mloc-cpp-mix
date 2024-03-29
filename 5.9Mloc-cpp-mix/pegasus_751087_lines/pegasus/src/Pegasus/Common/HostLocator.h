//%2007////////////////////////////////////////////////////////////////////////
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
 
#ifndef Pegasus_HostLocator_h
#define Pegasus_HostLocator_h

#include <Pegasus/Common/HostAddress.h>

PEGASUS_NAMESPACE_BEGIN

/**
    This class is used to store the host locator. HostLocator can be Hostname or
    IPv4 address or IPv6 address with optional port. IPv6 address must be 
    enclosed in brackets.
*/

class PEGASUS_COMMON_LINKAGE HostLocator
{
public:

    /**
        Port Numbers. Values for Max, valid and unspecified ports. 
    */
    enum
    {
        PORT_UNSPECIFIED =  Uint32(-2),
        PORT_INVALID = Uint32(-1),
        MAX_PORT_NUMBER = 65535
    };

    HostLocator();
    ~HostLocator();

    HostLocator(const String &locator);
    HostLocator(const HostLocator &rhs);
    HostLocator& operator =(const HostLocator &rhs);
 
    void setHostLocator(const String &locator);

    /**
        Returns true if the address is valid. If vaild it can be HostName
        or IPv4Address or IPv6Address.
    */
    Boolean isValid();

    /**
        Returns HostName or IPv4Address or IPv6Address. Removes port number
        from HostLocator if present. This returns empty string if HostLocator
        is not valid. Check if HostLocator is valid by using isValid() method
        before making any calls on HostLocator object.
    */
    String getHost();

    /**
        Returns port number assosiated with this HostLocator.
    */
    Uint32 getPort();

    /**
        Returns port number in the string form.
    */
    String getPortString();

    /*
        Returns true if port is specified as part of HostAddress and it is
        valid.
    */
    Boolean isPortSpecified ();

    /**
       Returns address type. It can be AT_IPV4, AT_IPV6 or AT_HOSTNAME of
       HostAddress.
    */
    Uint16 getAddressType();

private:
    void _init();
    void _parseLocator(const String &locator);
    HostAddress _hostAddr;
    Boolean _isValid;
    Uint32 _portNumber;
};

PEGASUS_NAMESPACE_END

#endif //Pegasus_HostLocator_h
