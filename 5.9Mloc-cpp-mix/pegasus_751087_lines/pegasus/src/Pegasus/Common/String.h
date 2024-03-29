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

#ifndef Pegasus_String_h
#define Pegasus_String_h

#ifdef PEGASUS_OS_HPUX
# ifdef HPUX_IA64_NATIVE_COMPILER
#  include <iostream>
# else
#  include <iostream.h>
# endif
#else
# include <iostream>
#endif
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Char16.h>
#include <Pegasus/Common/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

class String;
struct StringRep;

/** The CString class provides access to an 8-bit String representation.
*/
class PEGASUS_COMMON_LINKAGE CString
{
public:

    /** Constructs a CString object with null values (default constructor).
    */
    CString();

    /** REVIEWERS: Describe method here.
    @param cstr Specifies the name of the CString instance to copy.
    */
    CString(const CString& cstr);

    /** CString destructor.
    */
    ~CString();

    /** Assigns the values of one CString instance to another.
    @param cstr Specifies the name of the CString instance whose values
    are assigned to CString.
    */
    CString& operator=(const CString& cstr);

    /** Gets a pointer to the CString's data.
    @return Returns a const char pointer to the CString's data.
    */
    operator const char*() const;

private:

    CString(char* cstr);

    friend class String;

    char* _rep;
};

/**
    The Pegasus String C++ Class implements the CIM string type.
    REVIEWERS: We need more definition here.
*/
class PEGASUS_COMMON_LINKAGE String
{
public:

    /** This member is used to represent an empty string. Using this
        member avoids construction of an empty string (for example, String()).
    */
    static const String EMPTY;

    /** Default constructor without parameters. This constructor creates a
    null string. For example,
    <pre>
        String test;
    </pre>
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String();

    /** Copy constructor.
    @param str Specifies the name of the String instance.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String(const String& str);

    /** Initialize with first <TT>n</TT> characters from <TT>str</TT>.
    @param str Specifies the name of the String instance.
    @param n Specifies Uint32 size to use for the length of the string object.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String(const String& str, Uint32 n);

    /** Initialize with str.
    @param str Specifies the name of the String instance.
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String(const Char16* str);

    /** Initialize with first n characters of str.
    @param str Specifies the name of the String instance.
    @param n Specifies the Uint32 size.
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String(const Char16* str, Uint32 n);

    /** Initialize from a plain C-String:
    @param str Specifies the name of the String instance.
    API supports UTF8
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    @exception Exception Thrown if str is invalid UTF8
    */
    String(const char* str);

    /** Initialize from the first <TT>n</TT> characters of a plain C-String:
    @param str Specifies the name of the String instance.
    @param u Specifies the Uint32 size.
    API supports UTF8
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    @exception Exception Thrown if str is invalid UTF8
    */
    String(const char* str, Uint32 n);

    /** String destructor.
    */
    ~String();

    /** Assign this string with str. For example,
    <pre>
        String t1 = "abc";
        String t2 = t1;
    </pre>
    String t2 is assigned the value of t1.
    @param str Specifies the name of the String to assign to another
    String instance.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& operator=(const String& str);

    /** Assign this string with String str.
    @param str String to assign.
    @return Returns the String.
    API supports UTF8
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& assign(const String& str);

    /** Assign this string with str.
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& assign(const Char16* str);

    /** Assign this string with first n characters of str.
    @param n REVIEWERS: Insert text here.
    @param str REVIEWERS: Insert text here.
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& assign(const Char16* str, Uint32 n);

    /** Assign this string with the plain C-String str.
    @param str REVIEWERS: Insert text here.
    API supports UTF8
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    @exception Exception Thrown if str is invalid UTF8
    */
    String& assign(const char* str);

    /** Assign this string with first n characters of the plain C-String str.
    @param str REVIEWERS: Insert text here.
    @param n REVIEWERS: Insert text here.
    API supports UTF8
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    @exception Exception Thrown if str is invalid UTF8
    */
    String& assign(const char* str, Uint32 n);

    /** Clear this string. After calling clear(), size() will return 0.
    <pre>
        String test = "abc";
        test.clear();
    </pre>
    */
    void clear();

    /** Reserves memory for capacity characters. Notice
    that this does not change the size of the string (size() returns
    what it did before).  If the capacity of the string is already
    greater or equal to the capacity argument, this method has no
    effect.  The capacity of a String object has no bearing on its
    external behavior.  The capacity of a String is set only for
    performance reasons.
    @param capacity Defines the capacity in characters to reserve.
    */
    void reserveCapacity(Uint32 capacity);

    /** Returns the length of the String object.
    @return Length of the String in characters. For example,
    <pre>
        String s = "abcd";
        assert(s.size() == 4);
    </pre>
        returns a value of 4 for the length.
    */
    Uint32 size() const;

    /** Returns a pointer to the first character in the
    null-terminated Char16 buffer of the String object.
    @return Pointer to the first character of the String object. For example,
    <pre>
        String test = "abc";
        const Char16* q = test.getChar16Data();
    </pre>
        points to the first character in the String instance named test.
    */
    const Char16* getChar16Data() const;

    /** Create an 8-bit representation of this String object. For example,

    @return CString object that provides access to the UTF8 String
    representation.

    <pre>
        String test = "abc";
            printf("test = %s\n", (const char*)test.getCString());

            USAGE WARNING:  Do not do the following:

              const char * p = (const char *)test.getCString();

            The pointer p will be invalid.  This is because
            the Compiler casues the CString object to be created on the
            callers stack as a temporary object. The deletion is therefore
            also the responsibility of the Compiler. The Compiler may cause
            it to be deleted at anytime after the return. Typically it is
            done at the closeof the next scope. When it is deleted the
            "const char *p" above will become a dangling pointer.

            The correct usage to achieve the "const char * p" is
            as follows:

              String str = "hello";
              ...
              CString cstr = str.getCString();

              const char* p = (const char*)cstr;

            This tells the compiler to create a CString object on the callers
            stack that is the deleted at the discretion of the caller rather
            than the compiler. The "const char *p" above will be good until
            the caller explicity deletes the CString object.


    </pre>
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    CString getCString() const;

    /** Returns the specified character of the String object.
    @param index Index of the character to access.
    @return Specified character of the String object.
    @exception IndexOutOfBoundsException If <TT>index</TT>
    is outside the bounds of the String.
    <pre>
        String test = "abc;
        Char16 c = test[1];
    </pre>
    */
    Char16& operator[](Uint32 index);

    /** Returns the specified character of the String object (const version).
    @param index Index of the character to access.
    @return Specified character of the String object.
    @exception IndexOutOfBoundsException If <TT>index</TT>
    is outside the bounds of the String.
    */
    const Char16 operator[](Uint32 index) const;

    /** Append the given character to this String.
    @param c Character to append.
    @return This String.
    <pre>
        String test = "abc";
        test.append(Char16('d'));
        assert(test == "abcd");
    </pre>
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& append(const Char16& c);

    /** Append n characters from str to this String.
    @param str REVIEWERS: Insert text here.
    @param n REVIEWERS: Insert text here.
    @exception NullPointer Thrown if str is NULL.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& append(const Char16* str, Uint32 n);

    /** Append the given String to this String.
    @param str String to append.
    @return This String.
    <pre>
        String test = "abc";
        test.append("def");
        assert(test == "abcdef");
    </pre>
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String& append(const String& str);

    /** Remove size characters from the string starting at the given
    index. If size is PEG_NOT_FOUND, then all characters after
    <TT>index</TT> are removed.
    @param index Position in string to start remove.
    @param size Number of characters to remove. Default is PEG_NOT_FOUND
    which causes all characters after <TT>index</TT> to be removed.
    <pre>
        String s;
        s = "abc";
        s.remove(0, 1);
        assert(String::equal(s, "bc"));
        assert(s.size() == 2);
        s.remove(0);
        assert(String::equal(s, ""));
        assert(s.size() == 0);
    </pre>
    @exception IndexOutOfBoundsException If size is greater than
    length of String plus starting index for remove.
    */
    void remove(Uint32 index, Uint32 size = PEG_NOT_FOUND);

    /** Return a new String which is initialzed with <TT>length</TT>
    characters from this string starting at <TT>index</TT>.
    @param index Specifies the index in string to start getting the
    substring.
    @param length Specifies the number of characters to get. If length
    is PEG_NOT_FOUND, then all characters after index are added to the new
    string.
    @return String Specifies the Sting with the defined substring.
    @exception bad_alloc Thrown if there is insufficient memory.
    */
    String subString(Uint32 index, Uint32 length = PEG_NOT_FOUND) const;

    /** Find the index of the first occurrence of the character c.
    If the character is not found, PEG_NOT_FOUND is returned.
    @param c Char to be found in the String.
    @return Position of the character in the string or PEG_NOT_FOUND if not
    found.
    */
    Uint32 find(Char16 c) const;

    /** Find the index of the first occurence of the character c.
    If the character is not found, PEG_NOT_FOUND is returned.
    Searching begins from the specified index.
    @param c Char to be found in the String.
    @return Position of the character in the string or PEG_NOT_FOUND if the
    character is not found.
    */
    Uint32 find(Uint32 index, Char16 c) const;

    /** Find the index of the first occurence of the string object.
    This function finds one string inside another.
    If the matching substring is not found, PEG_NOT_FOUND is returned.
    @param s String object to be found in the String.
    @return Position of the substring in the String or PEG_NOT_FOUND if the
    substring is not found.
    */
    Uint32 find(const String& s) const;

    /** Same as find() but start looking in reverse (last character first).
    @param c Char16 character to find in String.
    @return Position of the character in the string or PEG_NOT_FOUND if the
    character is not found.
    */
    Uint32 reverseFind(Char16 c) const;

    /** Converts all characters in this string to lowercase characters,
    */
    void toLower();

#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES
    /** <I><B>Experimental Interface</B></I><BR>
        Converts all characters in this string to uppercase characters.
    */
    void toUpper();
#endif

    /**
        Compares the first n characters of two String objects.
        @param s1 The first String to compare.
        @param s2 The second String to compare.
        @param n The maximum number of characters to compare.
        @return Returns a negative integer if the first n characters of s1
        are lexographically less than s2, 0 if the first n characters of s1
        and s2 are equal, and a positive integer otherwise.
    */
    static int compare(const String& s1, const String& s2, Uint32 n);

    /**
        Compares two String objects.
        @param s1 The first String to compare.
        @param s2 The second String to compare.
        @return Returns a negative integer if s1 is lexographically less
        than s2, 0 if s1 and s2 are equal, and a positive integer otherwise.

        NOTE: Use the comparison operators <,<= > >= to compare
        String objects.
    */
    static int compare(const String& s1, const String& s2);

    /**
        Compares two String objects, ignoring case differences.
        @param s1 The first String to compare.
        @param s2 The second String to compare.
        @return Returns a negative integer if s1 is lexographically less
        than s2, 0 if s1 and s2 are equal, and a positive integer otherwise.
        (Case differences are ignored in all cases.)
    */
    static int compareNoCase(const String& s1, const String& s2);

    /** Compare two String objects for equality.
    @param s1 First <TT>String</TT> for comparison.
    @param s2 Second <TT>String</TT> for comparison.

    @return true If the two strings are equal; otherwise, false. For example,
    <pre>
        String s1 = "Hello World";
        String s2 = s1;
        String s3(s2);
        assert(String::equal(s1, s3));
    </pre>
    */
    static Boolean equal(const String& str1, const String& str2);

    /** Compares two strings and returns true if they
    are equal indepedent of case of the characters.
    @param str1 First String parameter.
    @param str2 Second String parameter.
    @return true If strings are equal independent of case, flase
        otherwise.
    */
    static Boolean equalNoCase(const String& str1, const String& str2);

#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES

    String(const String& s1, const String& s2);

    String(const String& s1, const char* s2);

    String(const char* s1, const String& s2);

    String& operator=(const char* str);

    Uint32 find(const char* s) const;

    static Boolean equal(const String& s1, const char* s2);

    static int compare(const String& s1, const char* s2);

    String& append(const char* str);

    String& append(const char* str, Uint32 size);

    static Boolean equalNoCase(const String& s1, const char* s2);

#endif /* PEGASUS_USE_EXPERIMENTAL_INTERFACES */

private:

    StringRep* _rep;
};

/** String operator == tests for equality between two strings of any of the
    types String or char*.
    @return true If the strings are equal; otherwise, false.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator==(
    const String& str1,
    const String& str2);

/** String operator ==. Test for equality between two strings.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator==(const String& str1, const char* str2);

/** String operator ==. Test for equality between two strings.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator==(const char* str1, const String& str2);

/** String operator ==. Test for equality between two strings.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator!=(
    const String& str1,
    const String& str2);

/** REVIEWERS: Insert description here.
    @param str REVIEWERS: Insert description here.
    @param os REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE PEGASUS_STD(ostream)& operator<<(
    PEGASUS_STD(ostream)& os,
    const String& str);

/** This overload operator (+) concatenates String objects. For example,
    <pre>
        String t1 = "abc";
        String t2;
        t2 = t1 + "def"
        assert(t2 == "abcdef");
    </pre>
*/
PEGASUS_COMMON_LINKAGE String operator+(const String& str1, const String& str2);

/** The overload operator (<) compares String obects.
    <pre>
        String t1 = "def";
        String t2 = "a";
        assert (t2 < t1);
    </pre>
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator<(
    const String& str1,
    const String& str2);

/** The overload operator (<=) compares String objects.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator<=(
    const String& str1,
    const String& str2);

/** The overload operator (>) compares String objects.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator>(
    const String& str1,
    const String& str2);

/** The overload operator (>=) compares String objects.
    @param str1 REVIEWERS: Insert description here.
    @param str2 REVIEWERS: Insert description here.
*/
PEGASUS_COMMON_LINKAGE Boolean operator>=(
    const String& str1,
    const String& str2);

#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES

PEGASUS_COMMON_LINKAGE Boolean operator==(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator==(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator==(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator!=(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator!=(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator!=(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator<(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator<(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator<(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator>(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator>(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator>(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator<=(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator<=(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator<=(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator>=(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE Boolean operator>=(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE Boolean operator>=(const char* s1, const String& s2);

PEGASUS_COMMON_LINKAGE String operator+(const String& s1, const String& s2);

PEGASUS_COMMON_LINKAGE String operator+(const String& s1, const char* s2);

PEGASUS_COMMON_LINKAGE String operator+(const char* s1, const String& s2);

#endif /* PEGASUS_USE_EXPERIMENTAL_INTERFACES */

PEGASUS_NAMESPACE_END

#if defined(PEGASUS_INTERNALONLY)
# include "StringInline.h"
#endif

#endif /* Pegasus_String_h */
