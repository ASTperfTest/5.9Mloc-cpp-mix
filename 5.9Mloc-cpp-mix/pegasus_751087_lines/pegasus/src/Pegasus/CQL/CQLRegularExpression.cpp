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

#include "CQLRegularExpression.h"
#include <Pegasus/Common/Char16.h>
#include <Pegasus/Common/CommonUTF.h>

PEGASUS_NAMESPACE_BEGIN

CQLRegularExpression::CQLRegularExpression()
{
}

CQLRegularExpression::~CQLRegularExpression()
{
}


Boolean CQLRegularExpression::match(const String& string,
                                    const String& pattern)
{
    Uint32 patIndex = 0;
    Uint32 strIndex = 0;

    // if either pattern or string are "EMPTY" you have an invalid String
    if (pattern == String::EMPTY)
    {
        return false;
    }

    if (string == String::EMPTY)
    {
        return false;
    }

    while (true)
    {
        if ( (string.size() == strIndex) && (pattern.size() == patIndex))
        {
            return true;
        }
        else if ((string.size() == strIndex) || (pattern.size() == patIndex))
        {
            return false;
        }

        //  Check if pattern equal to  '.'
        if (pattern[patIndex] == '.')
        {
            //assumes a valid multi-byte pair has been passed
            if ((((Uint16)pattern[patIndex] >= FIRST_HIGH_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_HIGH_SURROGATE)) ||
                (((Uint16)pattern[patIndex] >= FIRST_LOW_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_LOW_SURROGATE)))
            {
                patIndex ++;
                strIndex ++;
            }

            strIndex ++;
            patIndex ++;

            // Check if pattern equal to '*'
        }
        else if (pattern[patIndex] == '*')
        {
            if(patIndex == 0)
                return false;
            if (pattern[patIndex-1] == '.')
            {
                if ((patIndex > 1) && pattern[patIndex-2] =='\\')
                {
                    if (string[strIndex] != '.')
                    {
                        return false;
                    }
                }
                else if (pattern.size()-1 == patIndex)
                {
                    return true;
                }
                else if (string.size()-1 == strIndex)
                {
                    return false;
                }
            }
            else if (pattern[patIndex-1] == '\\')
            {
                if (pattern[patIndex-2] == '.')
                {
                    if (string[strIndex] != '*')
                    {
                        return false;
                    }
                }
            }
            else if (
                 (((Uint16)pattern[patIndex-2] >= FIRST_HIGH_SURROGATE) && 
                    ((Uint16)pattern[patIndex-2] <= LAST_HIGH_SURROGATE)) ||
                 (((Uint16)pattern[patIndex-2] >= FIRST_LOW_SURROGATE) && 
                    ((Uint16)pattern[patIndex-2] <= LAST_LOW_SURROGATE)))
            {

                if (pattern[patIndex-2] != string[strIndex])
                {
                    return false;
                } else if (pattern[patIndex-1] != string[strIndex+1])
                {
                    return false;
                }
                else
                {
                    strIndex ++;
                }
            }
            else if (pattern[patIndex-1] != string[strIndex])
            {
                return false;
            }
            while (true)
            {
                strIndex ++;

                if (pattern[patIndex-1] == '.')
                {
                    if ((patIndex > 1) && (pattern[patIndex-2] =='\\'))
                    {
                        if (string[strIndex] != '.')
                        {
                            patIndex ++;
                            break;
                        }
                    }
                    else if (pattern[patIndex+1] == string[strIndex])
                    {
                        //make copies of the indexes in case you do not reach
                        //the end of the string
                        int stringOrig = strIndex;
                        int patternOrig = patIndex;
                        patIndex++;

                        if (strIndex == string.size()-1 && 
                                patIndex == pattern.size()-1)
                        {
                            return true;
                        }
                        while (true)
                        {
                            strIndex++;
                            patIndex ++;
                            if (pattern[patIndex] != string[strIndex])
                            {
                                strIndex = stringOrig + 1;
                                patIndex = patternOrig;
                                break;
                            } else if (strIndex == string.size()-1 && 
                                   patIndex == pattern.size()-1)
                            {
                                break;
                            }
                            patIndex++;
                        }
                    }
                }
                else if (pattern[patIndex-1] == '\\')
                {
                    if (pattern[patIndex-2] == '.')
                    {
                        if (string[strIndex] != '*')
                        {
                            patIndex ++;
                            break;
                        }
                        if (strIndex == string.size()-1 && 
                            patIndex == pattern.size()-1)
                        {
                            return true;
                        }
                        while (true) {
                            strIndex ++;
                            if (string[strIndex] != '*')
                            {
                                patIndex ++;
                                break;
                            }
                            if (strIndex == string.size()-1 && 
                                patIndex == pattern.size()-1)
                            {
                                return true;
                            }
                        }
                    }
                }
                else if (
                     (((Uint16)pattern[patIndex-2] >= FIRST_HIGH_SURROGATE) && 
                        ((Uint16)pattern[patIndex-2] <= LAST_HIGH_SURROGATE)) ||
                     (((Uint16)pattern[patIndex-2] >= FIRST_LOW_SURROGATE) && 
                        ((Uint16)pattern[patIndex-2] <= LAST_LOW_SURROGATE)))
                {
                    if (pattern[patIndex-2] != string[strIndex])
                    {
                        patIndex ++;
                        break;
                    }
                    else if (pattern[patIndex-1] != string[strIndex+1])
                    {
                        patIndex ++;
                        break;
                    }
                    else
                    {
                        strIndex ++;
                    }
                }
                else if (pattern[patIndex-1] != string[strIndex])
                {
                    patIndex ++;
                    break;
                }

                if (strIndex == string.size()-1 && 
                    patIndex == pattern.size()-1)
                {
                    return true;
                } else if (strIndex == string.size()-1)
                {
                    return false;
                }

            }
            // check if pattern equal to '\'
        }
        else if (pattern[patIndex] == '\\')
        {
            patIndex ++;
            if ((((Uint16)pattern[patIndex] >= FIRST_HIGH_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_HIGH_SURROGATE)) ||
                (((Uint16)pattern[patIndex] >= FIRST_LOW_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_LOW_SURROGATE)))
            {

                if (pattern[patIndex] != string[strIndex])
                {
                    return false;
                }
                else if (pattern[patIndex+1] != string[strIndex+1])
                {
                    return false;
                }
                else
                {
                    patIndex ++;
                    strIndex ++;
                }
            }
            else
            {
                if (pattern[patIndex] != string[strIndex]) {
                    return false;
                }
                if (strIndex == string.size()-1 && 
                    patIndex == pattern.size()-1)
                {
                    return true;
                }
                strIndex ++;
                patIndex ++;

            }  

            //default 
        }
        else
        {
            if ((((Uint16)pattern[patIndex] >= FIRST_HIGH_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_HIGH_SURROGATE)) ||
                (((Uint16)pattern[patIndex] >= FIRST_LOW_SURROGATE) && 
                 ((Uint16)pattern[patIndex] <= LAST_LOW_SURROGATE)))
            {
                if (pattern[patIndex] != string[strIndex])
                {
                    return false;
                }
                else if (pattern[patIndex+1] != string[strIndex+1])
                {
                    return false;
                } else {
                    patIndex ++;
                    strIndex ++;
                }
            }
            else if (pattern[patIndex] != string[strIndex])
            {
                return false;
            }
            patIndex ++;
            strIndex ++;
        }
    }
    PEGASUS_UNREACHABLE( return false; )
}

PEGASUS_NAMESPACE_END


