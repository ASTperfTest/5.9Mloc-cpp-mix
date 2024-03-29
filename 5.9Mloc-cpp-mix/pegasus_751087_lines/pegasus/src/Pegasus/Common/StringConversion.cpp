//%2006////////////////////////////////////////////////////////////////////////
//
// Copyright (c, 2 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c, 2 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c, 2 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c, 2 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c, 2 2006 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; Symantec Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software", 2, to
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
//%/////////////////////////////////////////////////////////////////////////////

#include "StringConversion.h"

#include <errno.h>

PEGASUS_NAMESPACE_BEGIN

struct Uint32ToStringElement
{
    const char* str;
    size_t size;
};

static Uint32ToStringElement _Uint32Strings[] =
{
    { "0", 1 },
    { "1", 1 },
    { "2", 1 },
    { "3", 1 },
    { "4", 1 },
    { "5", 1 },
    { "6", 1 },
    { "7", 1 },
    { "8", 1 },
    { "9", 1 },
    { "10", 2 },
    { "11", 2 },
    { "12", 2 },
    { "13", 2 },
    { "14", 2 },
    { "15", 2 },
    { "16", 2 },
    { "17", 2 },
    { "18", 2 },
    { "19", 2 },
    { "20", 2 },
    { "21", 2 },
    { "22", 2 },
    { "23", 2 },
    { "24", 2 },
    { "25", 2 },
    { "26", 2 },
    { "27", 2 },
    { "28", 2 },
    { "29", 2 },
    { "30", 2 },
    { "31", 2 },
    { "32", 2 },
    { "33", 2 },
    { "34", 2 },
    { "35", 2 },
    { "36", 2 },
    { "37", 2 },
    { "38", 2 },
    { "39", 2 },
    { "40", 2 },
    { "41", 2 },
    { "42", 2 },
    { "43", 2 },
    { "44", 2 },
    { "45", 2 },
    { "46", 2 },
    { "47", 2 },
    { "48", 2 },
    { "49", 2 },
    { "50", 2 },
    { "51", 2 },
    { "52", 2 },
    { "53", 2 },
    { "54", 2 },
    { "55", 2 },
    { "56", 2 },
    { "57", 2 },
    { "58", 2 },
    { "59", 2 },
    { "60", 2 },
    { "61", 2 },
    { "62", 2 },
    { "63", 2 },
    { "64", 2 },
    { "65", 2 },
    { "66", 2 },
    { "67", 2 },
    { "68", 2 },
    { "69", 2 },
    { "70", 2 },
    { "71", 2 },
    { "72", 2 },
    { "73", 2 },
    { "74", 2 },
    { "75", 2 },
    { "76", 2 },
    { "77", 2 },
    { "78", 2 },
    { "79", 2 },
    { "80", 2 },
    { "81", 2 },
    { "82", 2 },
    { "83", 2 },
    { "84", 2 },
    { "85", 2 },
    { "86", 2 },
    { "87", 2 },
    { "88", 2 },
    { "89", 2 },
    { "90", 2 },
    { "91", 2 },
    { "92", 2 },
    { "93", 2 },
    { "94", 2 },
    { "95", 2 },
    { "96", 2 },
    { "97", 2 },
    { "98", 2 },
    { "99", 2 },
    { "100", 3 },
    { "101", 3 },
    { "102", 3 },
    { "103", 3 },
    { "104", 3 },
    { "105", 3 },
    { "106", 3 },
    { "107", 3 },
    { "108", 3 },
    { "109", 3 },
    { "110", 3 },
    { "111", 3 },
    { "112", 3 },
    { "113", 3 },
    { "114", 3 },
    { "115", 3 },
    { "116", 3 },
    { "117", 3 },
    { "118", 3 },
    { "119", 3 },
    { "120", 3 },
    { "121", 3 },
    { "122", 3 },
    { "123", 3 },
    { "124", 3 },
    { "125", 3 },
    { "126", 3 },
    { "127", 3 },
};

template<class S, class U>
struct Converter
{
    static inline const char* uintToString(char buffer[22], U x, Uint32& size)
    {
        if (x < 128)
        {
            size = _Uint32Strings[x].size;
            return _Uint32Strings[x].str;
        }

        char* p = &buffer[21];
        *p = '\0';

        do
        {
            *--p = '0' + (x % 10);
            x = x / 10;
        }
        while (x);

        size = &buffer[21] - p;
        return p;
    }

    static inline const char* sintToString(char buffer[22], S x, Uint32& size)
    {
        if (x < 0)
        {
            char* p = &buffer[21];
            *p = '\0';

            U t = U(-x);

            do
            {
                *--p = '0' + (t % 10);
                t = t / 10;
            }
            while (t);

            *--p = '-';

            size = &buffer[21] - p;
            return p;
        }
        else
            return Converter<S, U>::uintToString(buffer, U(x), size);
    }
};

const char* Uint8ToString(char buffer[22], Uint8 x, Uint32& size)
{
    return Converter<Sint8, Uint8>::uintToString(buffer, x, size);
}

const char* Uint16ToString(char buffer[22], Uint16 x, Uint32& size)
{
    return Converter<Sint16, Uint16>::uintToString(buffer, x, size);
}

const char* Uint32ToString(char buffer[22], Uint32 x, Uint32& size)
{
    return Converter<Sint32, Uint32>::uintToString(buffer, x, size);
}

const char* Uint64ToString(char buffer[22], Uint64 x, Uint32& size)
{
    return Converter<Sint64, Uint64>::uintToString(buffer, x, size);
}

const char* Sint8ToString(char buffer[22], Sint8 x, Uint32& size)
{
    return Converter<Sint8, Uint8>::sintToString(buffer, x, size);
}

const char* Sint16ToString(char buffer[22], Sint16 x, Uint32& size)
{
    return Converter<Sint16, Uint16>::sintToString(buffer, x, size);
}

const char* Sint32ToString(char buffer[22], Sint32 x, Uint32& size)
{
    return Converter<Sint32, Uint32>::sintToString(buffer, x, size);
}

const char* Sint64ToString(char buffer[22], Sint64 x, Uint32& size)
{
    return Converter<Sint64, Uint64>::sintToString(buffer, x, size);
}

//------------------------------------------------------------------------------
//
// decimalStringToUint64
//
//      ( positiveDecimalDigit *decimalDigit | "0" )
//
//------------------------------------------------------------------------------

Boolean StringConversion::decimalStringToUint64(
    const char* stringValue,
    Uint64& x)
{
    x = 0;
    const char* p = stringValue;

    if (!p || !*p)
    {
        return false;
    }

    if (*p == '0')
    {
        // A decimal string that starts with '0' must be exactly "0".
        return p[1] == '\0';
    }

    // Add on each digit, checking for overflow errors
    while (isdigit(*p))
    {
        // Make sure we won't overflow when we multiply by 10
        if (x > PEGASUS_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF)/10)
        {
            return false;
        }
        x = 10 * x;

        // Make sure we won't overflow when we add the next digit
        Uint64 newDigit = (*p++ - '0');
        if (PEGASUS_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF) - x < newDigit)
        {
            return false;
        }
        x = x + newDigit;
    }

    // If we found a non-decimal digit, report an error
    return (!*p);
}

//------------------------------------------------------------------------------
//
// hexStringToUint64
//
//      ( "0x" | "0X" ) 1*hexDigit
//
//------------------------------------------------------------------------------

Boolean StringConversion::hexStringToUint64(
    const char* stringValue,
    Uint64& x)
{
    x = 0;
    const char* p = stringValue;

    if (!p || !*p)
    {
        return false;
    }

    if ((p[0] != '0') || ((p[1] != 'x') && (p[1] != 'X')))
    {
        return false;
    }

    // Skip over the "0x"
    p+=2;

    // At least one hexadecimal digit is required
    if (!*p)
    {
        return false;
    }

    // Add on each digit, checking for overflow errors
    while (isxdigit(*p))
    {
        // Make sure we won't overflow when we multiply by 16
        if (x & PEGASUS_UINT64_LITERAL(0xF000000000000000))
        {
            return false;
        }

        x = (x << 4) + Uint64(hexCharToNumeric(*p++));
    }

    // If we found a non-hexadecimal digit, report an error
    return (!*p);
}

//------------------------------------------------------------------------------
//
// octalStringToUint64
//
//      "0" 1*octalDigit
//
//------------------------------------------------------------------------------

Boolean StringConversion::octalStringToUint64(
    const char* stringValue,
    Uint64& x)
{
    x = 0;
    const char* p = stringValue;

    if (!p || !*p)
    {
        return false;
    }

    if (*p++ != '0')
    {
        return false;
    }

    // At least one octal digit is required
    if (!*p)
    {
        return false;
    }

    // Add on each digit, checking for overflow errors
    while ((*p >= '0') && (*p <= '7'))
    {
        // Make sure we won't overflow when we multiply by 8
        if (x & PEGASUS_UINT64_LITERAL(0xE000000000000000))
        {
            return false;
        }

        x = (x << 3) + Uint64(*p++ - '0');
    }

    // If we found a non-octal digit, report an error
    return (!*p);
}

//------------------------------------------------------------------------------
//
// binaryStringToUint64
//
//      1*binaryDigit ( "b" | "B" )
//
//------------------------------------------------------------------------------

Boolean StringConversion::binaryStringToUint64(
    const char* stringValue,
    Uint64& x)
{
    x = 0;
    const char* p = stringValue;

    if (!p || !*p)
    {
        return false;
    }

    // At least two characters are required
    if (!*(p+1))
    {
        return false;
    }

    // Add on each digit, checking for overflow errors
    while ((*p == '0') || (*p == '1'))
    {
        // Make sure we won't overflow when we multiply by 2
        if (x & PEGASUS_UINT64_LITERAL(0x8000000000000000))
        {
            return false;
        }

        // We can't overflow when we add the next digit
        x = (x << 1) + Uint64(*p++ - '0');
    }

    if ((*p != 'b') && (*p != 'B'))
    {
        return false;
    }

    // No additional characters are permitted
    return (!*++p);
}

Boolean StringConversion::checkUintBounds(
    Uint64 x,
    CIMType type)
{
    switch (type)
    {
        case CIMTYPE_UINT8:
            return !(x & PEGASUS_UINT64_LITERAL(0xFFFFFFFFFFFFFF00));
        case CIMTYPE_UINT16:
            return !(x & PEGASUS_UINT64_LITERAL(0xFFFFFFFFFFFF0000));
        case CIMTYPE_UINT32:
            return !(x & PEGASUS_UINT64_LITERAL(0xFFFFFFFF00000000));
        case CIMTYPE_UINT64:
            return true;
        default:
            break;
    }

    return false;
}

//------------------------------------------------------------------------------
//
// stringToSint64
//
//      [ "+" | "-" ] (unsigned integer format)
//
//------------------------------------------------------------------------------

Boolean StringConversion::stringToSint64(
    const char* stringValue,
    Boolean (*uint64Converter)(const char*, Uint64&),
    Sint64& x)
{
    x = 0;

    if (!stringValue)
    {
        return false;
    }

    // Skip optional sign

    Boolean negative = *stringValue == '-';

    if (negative || *stringValue == '+')
    {
        stringValue++;
    }

    // Convert the remaining unsigned integer

    Uint64 uint64Value = 0;
    if (!(uint64Converter(stringValue, uint64Value)))
    {
        return false;
    }

    // Convert the unsigned integer to a signed integer

    if (negative)
    {
        if (uint64Value > PEGASUS_UINT64_LITERAL(0x8000000000000000))
        {
            return false;
        }
        x = -Sint64(uint64Value);
    }
    else
    {
        if (uint64Value > PEGASUS_UINT64_LITERAL(0x7FFFFFFFFFFFFFFF))
        {
            return false;
        }
        x = Sint64(uint64Value);
    }

    return true;
}

Boolean StringConversion::checkSintBounds(
    Sint64 x,
    CIMType type)
{
    switch (type)
    {
        case CIMTYPE_SINT8:
            return (((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFFFF80)) == 0) ||
                    ((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFFFF80)) ==
                          PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFFFF80)));
        case CIMTYPE_SINT16:
            return (((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFF8000)) == 0) ||
                    ((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFF8000)) ==
                          PEGASUS_SINT64_LITERAL(0xFFFFFFFFFFFF8000)));
        case CIMTYPE_SINT32:
            return (((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFF80000000)) == 0) ||
                    ((x & PEGASUS_SINT64_LITERAL(0xFFFFFFFF80000000)) ==
                          PEGASUS_SINT64_LITERAL(0xFFFFFFFF80000000)));
        case CIMTYPE_SINT64:
            return true;
        default:
            break;
    }

    return false;
}

//------------------------------------------------------------------------------
//
// stringToReal64
//
//      [ "+" | "-" ] *decimalDigit "." 1*decimalDigit
//          [ ( "e" | "E" ) [ "+" | "-" ] 1*decimalDigit ]
//
//------------------------------------------------------------------------------

Boolean StringConversion::stringToReal64(
    const char* stringValue,
    Real64& x)
{
    //
    // Check the string against the DMTF-defined grammar
    //
    const char* p = stringValue;

    if (!p || !*p)
        return false;

    // Skip optional sign:

    if (*p == '+' || *p  == '-')
        p++;

    // Skip optional first set of digits:

    while (isdigit(*p))
        p++;

    // Test required dot:

    if (*p++ != '.')
        return false;

    // One or more digits required:

    if (!isdigit(*p++))
        return false;

    while (isdigit(*p))
        p++;

    // If there is an exponent now:

    if (*p)
    {
        // Test exponent:

        if (*p != 'e' && *p != 'E')
            return false;

        p++;

        // Skip optional sign:

        if (*p == '+' || *p  == '-')
            p++;

        // One or more digits required:

        if (!isdigit(*p++))
            return false;

        while (isdigit(*p))
            p++;
    }

    if (*p)
        return false;

    //
    // Do the conversion
    //
    char* end;

    errno = 0;
    x = strtod(stringValue, &end);

    return (!*end && (errno != ERANGE));
}

PEGASUS_NAMESPACE_END
