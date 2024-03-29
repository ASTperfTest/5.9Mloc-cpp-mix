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
// Author: Michael E. Brasher
//
//%=============================================================================

#include "EchoCmd.h"
#include <iostream>

int EchoCmd(const vector<string>& args)
{
    for (size_t i = 1; i < args.size(); i++)
    {
        cout << args[i];

        if (i + 1 != args.size())
            cout << ' ';
    }

    cout << endl;

    return 0;
}


// EchoWithEscapeCmd (echo-e) supports the following
// limited set of escape sequences.
//       \a     alert (BEL)
//       \b     backspace
//       \c     suppress trailing newline
//       \n     new line
//       \r     carriage return
//       \t     horizontal tab

int EchoWithEscapeCmd(const vector<string>& args)
{
    for (size_t i = 1; i < args.size(); i++)
    {
        string text = args[i];
        int textLen = static_cast<int>(text.size());

        int j = 0;
        while (j < textLen - 1)
        {
             if ((text[j] == '\\') && (text[j+1] != '\\'))
             {
                   j++;
                   switch (text[j])
                   {
                       case 'a':
                           cout << "\a";
                           break;
                       case 'b':
                           cout << "\b";
                           break;
                       case 'c':
                           return 0;
                       case 'n':
                           cout << "\n";
                           break;
                       case 'r':
                           cout << "\r";
                           break;
                       case 't':
                           cout << "\t";
                           break;
                       default:
                           cout << "\\" << text[j];
                    }
             }
             else
             {
                 cout << text[j];
             }
             j++;
        }
        if (j == textLen - 1)
           cout << text[j];

        if (i + 1 != args.size())
           cout << ' ';
     }
     cout << endl;
     return 0;
}
