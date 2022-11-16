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

#include <windows.h>
#include <objbase.h>

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

String Guid::getGuid(const String &prefix)
{
  GUID guid;
  String strUUID;
  HRESULT hres = CoCreateGuid(&guid);

  if (hres == S_OK)
    {
      WCHAR guid_strW[39] = { L"" };
      char guid_str[100];
      ::memset(&guid_str, 0, sizeof(guid_str));
      const Uint32 numChars =  sizeof(guid_strW)/sizeof(guid_strW[0]);
      if (StringFromGUID2(guid, guid_strW, numChars) > 0)
        {
          WideCharToMultiByte(
              CP_ACP,
              0,
              guid_strW,
              numChars,
              guid_str,
              sizeof(guid_str),
              NULL,
              NULL);
          // exclude the first and last chars (i.e., { and })
          for (Uint32 i=1; i<sizeof(guid_str); i++)
            {
              if (guid_str[i] != '}')
                {
                  strUUID.append(Char16(guid_str[i]));
                }
              else
                {
                  break;
                }
            }
        }
      if (prefix == String::EMPTY)
        return strUUID;
      else
        return (prefix + strUUID);
    }
  return (String::EMPTY);
}

PEGASUS_NAMESPACE_END
// END_OF_FILE