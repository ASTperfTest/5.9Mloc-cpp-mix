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
#include <Pegasus/Common/Config.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

#ifdef PEGASUS_OS_TYPE_WINDOWS
# include <direct.h>
#else
# include <unistd.h>
#endif

PEGASUS_USING_STD;

bool RemoveFile(const string& path)
{
#ifdef OS_TYPE_WINDOWS
    return _unlink(path.c_str()) == 0;
#else
    return unlink(path.c_str()) == 0;
#endif
}

bool CopyFile(const string& fromFile, const string& toFile)
{
    // Open input file:

    ifstream is(fromFile.c_str() PEGASUS_IOS_BINARY);

    if (!is)
        return false;

    // Open output file:

    ofstream os(toFile.c_str() PEGASUS_IOS_BINARY);

    if (!os)
        return false;

    char c;

    while (is.get(c))
        os.put(c);

    return true;
}

const char* arg0 = "";

void ProcessFile(const char* fileName)
{
    ifstream is(fileName PEGASUS_IOS_BINARY);

    if (!is)
    {
        cerr << arg0 << "warning: failed to open \"" << fileName << "\"";
        cerr << endl;
        return;
    }

    const char TEMP_FILE_NAME[] = "stripcrs.tmp";

    ofstream os(TEMP_FILE_NAME PEGASUS_IOS_BINARY);

    if (!os)
    {
        cerr << arg0 << ": failed to open \"";
        cerr << TEMP_FILE_NAME << "\"" << endl;
        exit(1);
    }

    char c;

    while (is.get(c))
    {
        if (c != '\r')
            os.put(c);
    }

    is.close();
    os.close();

    if (!CopyFile(TEMP_FILE_NAME, fileName))
    {
        cerr << arg0 << ": failed to copy back file" << endl;
        exit(1);
    }

    RemoveFile(TEMP_FILE_NAME);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " filenames..." << endl;
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        cout << argv[i] << endl;
        ProcessFile(argv[i]);
    }

    return 0;
}
