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

#ifndef Pegasus_Value_h
#define Pegasus_Value_h

#include <Pegasus/Common/String.h>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/CIMType.h>
#include <Pegasus/Common/CIMObjectPath.h>
#include <Pegasus/Common/CIMObject.h>
#include <Pegasus/Common/CIMDateTime.h>
#include <Pegasus/Common/Exception.h>
#include <Pegasus/Common/Array.h>
#include <Pegasus/Common/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

class CIMValueRep;
class CIMObject;

/**
    The CIMValue class represents a value of any of the CIM data types
    (see \Ref{CIMType} for a list of valid CIM data types). This class
    encapsulates a union which holds the current value. The class also
    has a type field indicating the type of that value.
*/
class PEGASUS_COMMON_LINKAGE CIMValue
{
public:
    /** Constructor - Creates an NULL CIMValue object set to null and 
        with type CIMType:none and !arraytype.
    */
    CIMValue();

    /** Constructor - Creates a NULL CIMValue object with the type and
        array indicator set as specified.
        @exception TypeMismatchException If the given type is not valid for a
        CIMValue object.
    */
    CIMValue(CIMType type, Boolean isArray, Uint32 arraySize = 0);

    /// Constructor.
    CIMValue(Boolean x);

    /// Constructor.
    CIMValue(Uint8 x);

    /// Constructor.
    CIMValue(Sint8 x);

    /// Constructor.
    CIMValue(Uint16 x);

    /// Constructor.
    CIMValue(Sint16 x);

    /// Constructor.
    CIMValue(Uint32 x);

    /// Constructor.
    CIMValue(Sint32 x);

    /// Constructor.
    CIMValue(Uint64 x);

    /// Constructor.
    CIMValue(Sint64 x);

    /// Constructor.
    CIMValue(Real32 x);

    /// Constructor.
    CIMValue(Real64 x);

    /// Constructor.
    CIMValue(const Char16& x);

    /// Constructor.
    CIMValue(const String& x);

    /// Constructor.
    CIMValue(const CIMDateTime& x);

    /// Constructor.
    CIMValue(const CIMObjectPath& x);

    /** Constructor.
        Note: Constructing a CIMValue with an uninitialized CIMObject is not 
        defined and results in a thrown UninitializedObjectException.
        Note: The input CIMObject will be cloned before putting it into the
        value of the constructed CIMValue. This is because CIMObjects use a
        shared representation model, but we don't want CIMObjects inside a 
        CIMValue to be altered by other external changes.
    */
    CIMValue(const CIMObject& x);

    /// Constructor.
    CIMValue(const Array<Boolean>& x);

    /// Constructor.
    CIMValue(const Array<Uint8>& x);

    /// Constructor.
    CIMValue(const Array<Sint8>& x);

    /// Constructor.
    CIMValue(const Array<Uint16>& x);

    /// Constructor.
    CIMValue(const Array<Sint16>& x);

    /// Constructor.
    CIMValue(const Array<Uint32>& x);

    /// Constructor.
    CIMValue(const Array<Sint32>& x);

    /// Constructor.
    CIMValue(const Array<Uint64>& x);

    /// Constructor.
    CIMValue(const Array<Sint64>& x);

    /// Constructor.
    CIMValue(const Array<Real32>& x);

    /// Constructor.
    CIMValue(const Array<Real64>& x);

    /// Constructor.
    CIMValue(const Array<Char16>& x);

    /// Constructor.
    CIMValue(const Array<String>& x);

    /// Constructor.
    CIMValue(const Array<CIMDateTime>& x);

    /// Constructor.
    CIMValue(const Array<CIMObjectPath>& x);

    /** Constructor.
        Note: Constructing a CIMValue with an uninitialized CIMObject anywhere
        in the input array is not defined and results in a thrown 
        UninitializedObjectException.
        Note: Each CIMObject in the input Array will be cloned before putting 
        the Array into the value of the constructed CIMValue. This is because
        CIMObjects use a shared representation model, but we don't want 
        CIMObjects inside a CIMValue to be altered by other external changes.
    */
    CIMValue(const Array<CIMObject>& x);

    /** Constructor.
        Note: If the input type is CIMObject, it/they will be cloned before
        putting it into the value of the constructed CIMValue. This is because
        CIMObjects use a shared representation model, but we don't want 
        CIMObjects inside a CIMValue to be altered by other external changes.
    */
    CIMValue(const CIMValue& x);

    /// Destructor.
    ~CIMValue();

    /** Operator =
        Note: If the right hand side type is CIMObject, it/they will be cloned
        before putting it into the value of the target CIMValue. This is because
        CIMObjects use a shared representation model, but we don't want 
        CIMObjects inside a CIMValue to be altered by other external changes.
    */
    CIMValue& operator=(const CIMValue& x);

    /** Assigns one CIMValue object to another CIMValue object.
        @param x - CIMValue object to be used for assignment.
        Note: If the input type is CIMObject, it/they will be cloned before 
        putting it into the value of the target CIMValue. This is because 
        CIMObjects use a shared representation model, but we don't want  
        CIMObjects inside a CIMValue to be altered by other external changes.
    */
    void assign(const CIMValue& x);

    /** Clears the attributes and value of the CIMValue object.
    */
    void clear();

    /** Compares the types of two CIMValues. This
        compares the type field and the array indicators.
        @return true if both are of the same type and both are either arrays
        or not, false otherwise.
        <pre>
            CIMValue a(Boolean(true);
            CIMValue b = a;
            if b.typeCompatible(a)
                ...
        </pre>
    */
    Boolean typeCompatible(const CIMValue& x) const;

    /** Determines if the value is an array.
        @return true if the value is an array, false otherwise.
    */
    Boolean isArray() const;

    /** Determines whether the CIMvalue object is Null. 
        Null is the specific condition where no value has
        yet been set. If a CIMValue object is Null, any get on that 
        object will create an exception.
        @return true if the CIMValue object is Null, false otherwise.
    */
    Boolean isNull() const;

    /** Gets the size of an Array CIMValue.
        @return The number of entries in the array.
    */
    Uint32 getArraySize() const;

    /** Gets the CIMType attribute for the CIMValue.
        @return the CIMType value.
    */
    CIMType getType() const;

    /** Sets the CIMValue a NULL, but with valid CIMType and array 
        characteristics.
        @param type - CIMType for this CIMValue.
        @param isArray - Boolean indicating whether this is an array CIMValue.
        @param arraySize - Optional parameter indicating the array size.
        <pre>
            CIMValue value;             
            value.setNullValue(CIMType::BOOLEAN, false); 
        </pre>
    */
    void setNullValue(CIMType type, Boolean isArray, Uint32 arraySize = 0);

    /** Sets the type, Array attribute and puts the value provided
        into the value of the target CIMValue. 
        <pre>
            CIMValue x;
            x.set(Uint16(9));
        </pre>
        @exception UninitializedObjectException If the given type is CIMObject, 
        and the input CIMObject parameter is uninitialized or at least one entry 
        in the Array of CIMObjects is uninitialized.
        Note: If the input type is CIMObject, it/they will be cloned before
        putting it into the value of the target CIMValue. This is because 
        CIMObjects use a shared representation model, but we don't want
        CIMObjects inside a CIMValue to be altered by other external changes.
    */    
    void set(Boolean x);

    /// 
    void set(Uint8 x);
    ///
    void set(Sint8 x);
    ///
    void set(Uint16 x);
    ///
    void set(Sint16 x);
    ///
    void set(Uint32 x);
    ///
    void set(Sint32 x);
    ///
    void set(Uint64 x);
    ///
    void set(Sint64 x);
    ///
    void set(Real32 x);
    ///
    void set(Real64 x);
    ///
    void set(const Char16& x);
    ///
    void set(const String& x);
    ///
    void set(const CIMDateTime& x);
    ///
    void set(const CIMObjectPath& x);
    ///
    void set(const CIMObject& x);
    ///
    void set(const Array<Boolean>& x);
    ///
    void set(const Array<Uint8>& x);
    ///
    void set(const Array<Sint8>& x);
    ///
    void set(const Array<Uint16>& x);
    ///
    void set(const Array<Sint16>& x);
    ///
    void set(const Array<Uint32>& x);
    ///
    void set(const Array<Sint32>& x);
    ///
    void set(const Array<Uint64>& x);
    ///
    void set(const Array<Sint64>& x);
    ///
    void set(const Array<Real32>& x);
    ///
    void set(const Array<Real64>& x);
    ///
    void set(const Array<Char16>& x);
    ///
    void set(const Array<String>& x);
    ///
    void set(const Array<CIMDateTime>& x);
    ///
    void set(const Array<CIMObjectPath>& x);
    ///
    void set(const Array<CIMObject>& x);

    /** Gets the value of a CIMValue.
        Note: Before using get, the caller should use getType () and isNull ()
        to ensure that the value is not null, and is of the correct type.
     
        The behavior of get is undefined when the value is null.

        @param Variable in which to return the value.
        @exception TypeMismatchException If the CIMValue type is not compatible
                   with the type of the output parameter.
        <pre>
            Uint32 v;
            CIMValue value(CIMValue::UINT32, UINT32(99));
            value.get(v);
        </pre>

        <pre>
            Uint32 v;
            CIMValue value = property.getValue ();
            if ((value.getType () == CIMTYPE_UINT32) && (!value.isNull ()))
                value.get (v);
        </pre>
    */
    void get(Boolean& x) const;
    ///
    void get(Uint8& x) const;
    ///
    void get(Sint8& x) const;
    ///
    void get(Uint16& x) const;
    ///
    void get(Sint16& x) const;
    ///
    void get(Uint32& x) const;
    ///
    void get(Sint32& x) const;
    ///
    void get(Uint64& x) const;
    ///
    void get(Sint64& x) const;
    ///
    void get(Real32& x) const;
    ///
    void get(Real64& x) const;
    ///
    void get(Char16& x) const;
    ///
    void get(String& x) const;
    ///
    void get(CIMDateTime& x) const;
    ///
    void get(CIMObjectPath& x) const;
    ///
    void get(CIMObject& x) const;
    ///
    void get(Array<Boolean>& x) const;
    ///
    void get(Array<Uint8>& x) const;
    ///
    void get(Array<Sint8>& x) const;
    ///
    void get(Array<Uint16>& x) const;
    ///
    void get(Array<Sint16>& x) const;
    ///
    void get(Array<Uint32>& x) const;
    ///
    void get(Array<Sint32>& x) const;
    ///
    void get(Array<Uint64>& x) const;
    ///
    void get(Array<Sint64>& x) const;
    ///
    void get(Array<Real32>& x) const;
    ///
    void get(Array<Real64>& x) const;
    ///
    void get(Array<Char16>& x) const;
    ///
    void get(Array<String>& x) const;
    ///
    void get(Array<CIMDateTime>& x) const; 
    ///
    void get(Array<CIMObjectPath>& x) const; 
    ///
    void get(Array<CIMObject>& x) const; 

    /** Compares with another CIMValue object for equality.
        @param x - CIMValue to compare with.
        @return true if they are identical in type, attribute and value,
        false otherwise.
    */
    Boolean equal(const CIMValue& x) const;

    /** Converts the CIMvalue to a string.  Should only be
        used for output purposes.  To get an actual String value, use
        get(String &).
        @return  String output for CIMValue.
        <PRE>
            String test;
            CIMValue value(Boolean(true));
            test = value.toString();  // puts "TRUE" into test
        </PRE>
    */
    String toString() const;

#ifdef PEGASUS_USE_DEPRECATED_INTERFACES
    /**
        <I><B>Deprecated Interface</B></I><BR>
        Constructor.  (Note: This constructor exists solely to support binary
        compatibility with a previous definition of the Sint8 type.)
    */
    CIMValue(char x);

    /**
        <I><B>Deprecated Interface</B></I><BR>
        Constructor.  (Note: This constructor exists solely to support binary
        compatibility with a previous definition of the Sint8 type.)
    */
    CIMValue(const Array<char>& x);

    /**
        <I><B>Deprecated Interface</B></I><BR>
        Sets an Sint8 value.  (Note: This method exists solely to support
        binary compatibility with a previous definition of the Sint8 type.)
    */
    void set(char x);

    /**
        <I><B>Deprecated Interface</B></I><BR>
        Sets an Sint8 array value.  (Note: This method exists solely to support
        binary compatibility with a previous definition of the Sint8 type.)
    */
    void set(const Array<char>& x);

    /**
        <I><B>Deprecated Interface</B></I><BR>
        Gets an Sint8 value.  (Note: This method exists solely to support
        binary compatibility with a previous definition of the Sint8 type.)
    */
    void get(char& x) const;

    /**
        <I><B>Deprecated Interface</B></I><BR>
        Gets an Sint8 array value.  (Note: This method exists solely to support
        binary compatibility with a previous definition of the Sint8 type.)
    */
    void get(Array<char>& x) const;
#endif

private:

    void _get(const String*& data, Uint32& size) const;

    CIMValueRep* _rep;

    friend class CIMMethodRep;
    friend class CIMParameterRep;
    friend class CIMPropertyRep;
    friend class CIMQualifierRep;
    friend class CIMQualifierDeclRep;
    friend class BinaryStreamer;
    friend class XmlWriter;
};

/** operator == compares two CIMValue objects for equality.
    @param x - First CIMValue to compare
    @param y - Second CIMValue to compare
    @return true if they are identical in type, attribute and value,
    false otherwise.
*/
PEGASUS_COMMON_LINKAGE Boolean operator==(const CIMValue& x, const CIMValue& y);

/** operator != compares two CIMValue objects for inequality.
    @param x - First CIMValue to compare
    @param y - Second CIMValue to compare
    @return true if they are NOT identical in type, attribute or value,
    false otherwise.
*/
PEGASUS_COMMON_LINKAGE Boolean operator!=(const CIMValue& x, const CIMValue& y);

#define PEGASUS_ARRAY_T CIMValue
# include <Pegasus/Common/ArrayInter.h>
#undef PEGASUS_ARRAY_T

PEGASUS_NAMESPACE_END

#ifdef PEGASUS_INTERNALONLY
#include <Pegasus/Common/CIMValueInline.h>
#endif

#endif /* Pegasus_Value_h */
