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
//=============================================================================
#include <iostream>
#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Common/HashTable.h>
#include <Pegasus/Common/String.h>

PEGASUS_USING_STD;
PEGASUS_USING_PEGASUS;

static Boolean verbose;

void test01()
{
    typedef HashTable<String, Uint32, EqualFunc<String>, HashFunc<String> > HT;
    HT ht;

    PEGASUS_TEST_ASSERT(ht.insert("Red", 100));
    PEGASUS_TEST_ASSERT(ht.insert("Green", 200));
    PEGASUS_TEST_ASSERT(ht.insert("Blue", 300));
    PEGASUS_TEST_ASSERT(ht.size() == 3);

    Uint32 sum = 0;

    HT::Iterator i = ht.start();

    for (; i; i++)
    {
    if (verbose)
        {
            cout << "key[" << i.key() << "]" << endl;
            cout << "value[" << i.value() << "]" << endl;
        }
    sum += i.value();
    }

    HT ht2;
    ht2 = ht;
    sum = 0;

    for (HT::Iterator i = ht.start(); i; i++)
    {
    if (verbose)
        {
            cout << "key[" << i.key() << "]" << endl;
            cout << "value[" << i.value() << "]" << endl;
        }
    sum += i.value();
    }

    PEGASUS_TEST_ASSERT(sum == 600);

    Uint32 value = 0;
    PEGASUS_TEST_ASSERT(ht.lookup("Blue", value));
    PEGASUS_TEST_ASSERT(value == 300);

    PEGASUS_TEST_ASSERT(ht.remove("Red"));
    PEGASUS_TEST_ASSERT(ht.remove("Green"));
    PEGASUS_TEST_ASSERT(ht.size() == 1);

    ht.clear();
    PEGASUS_TEST_ASSERT(ht.size() == 0);

}

void test02()
{
    typedef HashTable<Uint32, Uint32, EqualFunc<Uint32>, HashFunc<Uint32> > HT;
    HT ht;

    Uint32 sum = 0;
    const Uint32 N = 10000;

    for (Uint32 i = 0; i < N; i++)
    {
    sum += 2 * i;
    ht.insert(i, i);
    }

    HT ht2 = ht;

    PEGASUS_TEST_ASSERT(ht2.size() == N);

    Uint32 expectedSum = 0;
    Uint32 n = 0;

    for (HT::Iterator i = ht2.start(); i; i++)
    {
    n++;
    expectedSum += i.value() + i.key();
    }

    PEGASUS_TEST_ASSERT(expectedSum == sum);
    PEGASUS_TEST_ASSERT(n == N);
}

void test03()
{
    // Test iteration of an empty hash table:

    typedef HashTable<Uint32, Uint32, EqualFunc<Uint32>, HashFunc<Uint32> > HT;
    HT ht;
    Uint32 n = 0;

    for (HT::Iterator i = ht.start(); i; i++)
    n++;

    PEGASUS_TEST_ASSERT(n == 0);
}

int main(int argc, char** argv)
{
    verbose = getenv("PEGASUS_TEST_VERBOSE") ? true : false;

    test01();
    test02();
    test03();

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}
