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

#ifndef Pegasus_CIMServerDescription_h
#define Pegasus_CIMServerDescription_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/Attribute.h>
#include <Pegasus/Common/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

class PEGASUS_COMMON_LINKAGE CIMServerDescription
{
public:

    /** Constructor for a CIMServerDescription object.
    */
    CIMServerDescription();

    /** Constructor for a CIMServerDescription object.
        @param url Url that identifies this connection description.
    */
    CIMServerDescription(const String& url);

    /** Destructor for a CIMServerDescription object.
    */
    ~CIMServerDescription();

    /** Returns the URL for this description.
        @return  String that contains the Url for this connection description.
    */
    String getUrl();

    /** Returns an attribute value as a string.
        @param  attributeName Attribute name of attribute.
        @param  defaultValue Default attribute value to return if empty or
        malformed.
        @return String that contains an attribute value.
    */
    String getValue(
        const String& attributeName,
        const String& defaultValue) const;

    /** Returns an attribute value Array
        @param  attributeName Attribute name of attribute.
        @param  attributeValues Array<String> into which results are copied
        @return True if attributeName was found, false otherwise.
        Note, a true return value with an empty attributeValues array
        indicates the tag had no attribute values (ie a keyword).
    */
    Boolean getValues(
        const String& attributeName,
        Array<String>& attributeValues) const;

    /** Returns an array of attribute.
        @return Array that contains contains attributes for this connection
        description.
    */
    Array<Attribute> getAttributes() const;

    /** Sets an array of attributes for this description.
        @param attributes Array that contains attributes for this connection
        description.
    */
    void setAttributes(const Array<Attribute>& attributes);

private:
    String _serviceLocationTcp;
    String _host;
    Uint32 _port;
    Array<Attribute> _attributes;
};

#define PEGASUS_ARRAY_T CIMServerDescription
#include <Pegasus/Common/ArrayInter.h>
#undef PEGASUS_ARRAY_T

PEGASUS_NAMESPACE_END

#endif /* Pegasus_CIMServerDescription_h */
