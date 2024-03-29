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

#ifndef Pegasus_WQLSimplePropertySource_h
#define Pegasus_WQLSimplePropertySource_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/WQL/WQLPropertySource.h>
#include <Pegasus/Common/HashTable.h>

PEGASUS_NAMESPACE_BEGIN

/** This class provides a simple property source implementation mainly for
    testing purposes only.

    It maintains a hash table which maps property names to values (represented
    using WQLOperand objects).

    This class implements getValue() by simply looking up the property name
    in the map and fetching the corresponding value.

    The addValue() method is used to add property-name/value pairs to the 
    internal map.

    Since this class is intended for testing purposes, all of the methods
    are inline to avoid making the library any larger.
*/
class WQLSimplePropertySource : public WQLPropertySource
{
public:

    /** Default constructor.
    */
    WQLSimplePropertySource()
    {
    }

    /** Virtual destructor.
    */
    virtual ~WQLSimplePropertySource()
    {
    }

    /** Get the value for the property with the given name. Simply looks up
        the property name in the internal map to fetch the value.
    
        @param propertyName name of property to be retrieved.
        @param value value of property.
        @return true is such a property exists and false otherwise.
    */
    virtual Boolean getValue(
    const CIMName& propertyName, 
    WQLOperand& value) const
    {
        return _map.lookup(propertyName.getString(), value);
    }

    /** Add the given propert-name/value pair to the internal map.
    
        @param propertyName name of property.
        @param value value of property.
        @return true if no such property already existed and false otherwise.
    */
    Boolean addValue(
    const CIMName& propertyName, 
    const WQLOperand& value)
    {
        return _map.insert(propertyName.getString(), value);
    }

private:

    typedef HashTable<
    String, WQLOperand, EqualFunc<String>, HashFunc<String> > Map;

    Map _map;
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_WQLSimplePropertySource_h */
