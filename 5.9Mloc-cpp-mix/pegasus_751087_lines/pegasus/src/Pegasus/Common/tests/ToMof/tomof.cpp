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
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Common/CIMValue.h>
#include <Pegasus/Common/CIMProperty.h>
#include <Pegasus/Common/CIMClass.h>
#include <Pegasus/Common/CIMInstance.h>
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/CIMQualifierDecl.h>
#include <Pegasus/Common/MofWriter.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

static Boolean verbose;

// Local function to compare created buffer and a result char array

bool resultTest(const Buffer& buffer, const char * result)
{
    if (strcmp(buffer.getData(), result) == 0)
    {
        return true;
    }

   else
    {
        int maxLen = (int)strlen(result);
        if (strlen(buffer.getData()) != strlen(result))
        {
            cout << "Size diff error. str1 len = " << strlen(buffer.getData())
                 << " str2 len = " << strlen(result) << endl;
        }
        cout << "Created Buffer\n<" << buffer.getData()
            << ">\nTest Result\n<" << result << ">" << endl;
        return(false);
    }
}
template<class T>
void test01(const T& x, const char* result)
{
    CIMValue v(x);
    CIMValue v2(v);
    CIMValue v3;
    v3 = v2;

    Buffer tmp1;
    MofWriter::appendValueElement(tmp1, v3);

    try
    {
        T t;
        v3.get(t);
        PEGASUS_TEST_ASSERT(t == x);
    }
    catch(Exception& e)
    {
        cerr << "Error: " << e.getMessage() << endl;
        exit(1);
    }
    PEGASUS_TEST_ASSERT(resultTest(tmp1, result));
}


// test property generation
void test03()
{
    // Test Property mof generation for string property.
    {
        CIMProperty p1(CIMName ("message"), String("Hi There"));
        Buffer tmp;
        // Test the mof creation for a class declaration
        MofWriter::appendPropertyElement(true, tmp, p1);
        const char * declCompare = "\nstring message = \"Hi There\";";
        if (verbose)
            cout << "Property MOF = " << tmp.getData()
                << "\n out \n" << declCompare << endl;

        PEGASUS_TEST_ASSERT(resultTest(tmp,declCompare));

        // Test the mof creation for instance definition
        Buffer tmp1;
        MofWriter::appendPropertyElement(false, tmp1, p1);
        const char * instanceCompare = "\nmessage = \"Hi There\";";

        PEGASUS_TEST_ASSERT(resultTest(tmp1,instanceCompare));
    }

    // Test Property mof generation for boolean property.
    {
        CIMProperty p1(CIMName ("message"), Boolean(true));
        Buffer tmp;
        // Test the mof creation for a class declaration
        MofWriter::appendPropertyElement(true, tmp, p1);
        const char * declCompare = "\nboolean message = TRUE;";

        PEGASUS_TEST_ASSERT(resultTest(tmp,declCompare));

        // Test the mof creation for instance definition
        Buffer tmp1;
        MofWriter::appendPropertyElement(false, tmp1, p1);
        const char * instanceCompare = "\nmessage = TRUE;";

        PEGASUS_TEST_ASSERT(resultTest(tmp1,instanceCompare));
    }

    // Test Property mof generation for Uint32 property.
    {
        CIMProperty p1(CIMName ("message"), Uint32(9999));
        Buffer tmp;
        // Test the mof creation for a class declaration
        MofWriter::appendPropertyElement(true, tmp, p1);
        const char * declCompare = "\nuint32 message = 9999;";


        PEGASUS_TEST_ASSERT(resultTest(tmp,declCompare));

        // Test the mof creation for instance definition
        Buffer tmp1;
        MofWriter::appendPropertyElement(false, tmp1, p1);
        const char * instanceCompare = "\nmessage = 9999;";

        PEGASUS_TEST_ASSERT(resultTest(tmp1,instanceCompare));
    }
}


// test mof generation for Qualifier declaration

/*
Qualifier flavors are indicated by an optional clause after the qualifier
and preceded by a colon.
They consist of some combination of the key words EnableOverride,
DisableOverride,
ToSubclass and Restricted, indicating the applicable propagation
and override rules.

Handle is designated as writable for the Process class and for every subclass of
this class.
class CIM_Process:CIM_LogicalElement
{
uint32 Priority;
[Write(true):DisableOverride ToSubclass]
string Handle;
};
*/
void test04()
{
    //Qualifier declaration Test
    try
    {
        CIMQualifierDecl qual1(
            CIMName ("CIMTYPE"),
            String(),
            CIMScope::PROPERTY,
            CIMFlavor::TOINSTANCE);

        //Qualifier Abstract : boolean = false, Scope(class, association,
        //indication), Flavor(DisableOverride, Restricted);

        CIMQualifierDecl q2(
            CIMName ("Abstract"),
            true,
            CIMScope::CLASS,
            CIMFlavor::OVERRIDABLE);
        ;

        Buffer tmp;
        MofWriter::appendQualifierDeclElement(tmp, qual1);

        char cimTypeQualifierDecl[] = "\nQualifier CIMTYPE :"
                                      " string = \"\", Scope(property);\n";
        PEGASUS_TEST_ASSERT(resultTest(tmp, cimTypeQualifierDecl));

        Buffer tmp1;
        MofWriter::appendQualifierDeclElement(tmp1, q2);
        char cimAbstractQualDecl[] = "\nQualifier Abstract :"
                                     " boolean = TRUE, Scope(class);\n";
        PEGASUS_TEST_ASSERT(resultTest(tmp1, cimAbstractQualDecl));
    }
    catch(Exception& e)
    {
        cerr << "Exception: " << e.getMessage() << endl;
    }
}


// Test MOF generation of Classes and Instances.
void test05()
{
    // Test CimClass not Association
    {
        if (verbose)
           cout << "Class test\n";
    CIMClass class1(CIMName ("MyClass"), CIMName ("YourClass"));

    Array<String> valueMap;
    valueMap.append("Abend");
    valueMap.append("Other");
    valueMap.append("Unknown");
    CIMValue v(valueMap);

    CIMProperty p(CIMName ("errorType"), CIMValue(CIMTYPE_STRING,false));
    p.addQualifier(CIMQualifier(CIMName("valueMap"),v));

    class1
        .addQualifier(CIMQualifier(CIMName ("q1"), Uint32(55)))
        .addQualifier(CIMQualifier(CIMName ("q2"), String("Hello")))
        .addProperty(CIMProperty(CIMName ("message"), String("Hello")))
        .addProperty(CIMProperty(CIMName ("count"), Uint32(77)))
        .addProperty(CIMProperty(p))
        .addMethod(CIMMethod(CIMName ("isActive"), CIMTYPE_BOOLEAN)
            .addParameter(CIMParameter(CIMName ("hostname"),
                    CIMTYPE_STRING))
            .addParameter(CIMParameter(CIMName ("port"), CIMTYPE_UINT32)));
    if (verbose)
        MofWriter::printClassElement(class1);
    Buffer tmp;
    MofWriter::appendClassElement(tmp, class1);

    // compare with the following result.
        char classCompare[] =
            "\n//    Class MyClass\n\n"
            "[q1 (55) : DisableOverride, Restricted, \n"
            "q2 (\"Hello\") : DisableOverride, Restricted]\n"
            "class MyClass : YourClass\n"
            "{\n"
            "string message = \"Hello\";\n"
            "uint32 count = 77;\n"
            "[valueMap {\"Abend\", \"Other\", \"Unknown\"} :"
                " DisableOverride, Restricted]\n"
            "string errorType;\n"
            "boolean isActive(string hostname, uint32 port);\n"
            "};\n";
        PEGASUS_TEST_ASSERT(resultTest(tmp, classCompare));

        CIMInstance instance1 =
            class1.buildInstance(false, false, CIMPropertyList());
        Uint32 idx = instance1.findProperty(CIMName("errorType"));
        instance1.removeProperty(idx);
        instance1.addProperty(
            CIMProperty(CIMName("errorType"), String("Abend")));
        Buffer tmpInstance;
        MofWriter::appendInstanceElement(tmpInstance, instance1);

        char instanceCompare[] =
            "\n//Instance of MyClass\n"
            "instance of MyClass\n"
            "{\n"
            "message = \"Hello\";\n"
            "count = 77;\n"
            "errorType = \"Abend\";\n"
            "};\n";
        PEGASUS_TEST_ASSERT(resultTest(tmpInstance, instanceCompare));
    }


    // Test CimClass This is not really an association class. We
    // simply used the
    {
        if (verbose)
            cout << "Class test\n";
        CIMClass class1(CIMName ("MyClass"), CIMName ("YourClass"));

        Array<String> arr;
        arr.append("One");
        arr.append("Two");
        arr.append("Three");
        CIMValue v(arr);

        class1
            .addQualifier(CIMQualifier(CIMName ("association"), true))
            .addQualifier(CIMQualifier(CIMName ("q1"), Uint32(55)))
            .addQualifier(CIMQualifier(CIMName ("q2"), String("Hello")))
            .addProperty(CIMProperty(CIMName ("message"),
                                     CIMValue(CIMTYPE_STRING, false)))
            .addProperty(CIMProperty(CIMName ("count"),
                                     CIMValue(CIMTYPE_UINT32, false)))
            .addProperty(CIMProperty(CIMName ("booltest"),
                                     CIMValue(CIMTYPE_BOOLEAN, false)))
            .addProperty(CIMProperty(CIMName ("arraytest"), v))
            .addMethod(CIMMethod(CIMName ("isActive"), CIMTYPE_BOOLEAN)
                .addParameter(CIMParameter(CIMName ("hostname"),
                        CIMTYPE_STRING))
                .addParameter(CIMParameter(CIMName ("port"), CIMTYPE_UINT32)));
        if (verbose)
            MofWriter::printClassElement(class1);
        Buffer tmp;
        MofWriter::appendClassElement(tmp, class1);

    // compare with the following result.
        char classCompare[] =
            "\n//    Class MyClass\n\n"
            "[association : DisableOverride, Restricted, \n"
            "q1 (55) : DisableOverride, Restricted, \n"
            "q2 (\"Hello\") : DisableOverride, Restricted]\n"
            "class MyClass : YourClass\n"
            "{\n"
            "string message;\n"
            "uint32 count;\n"
            "boolean booltest;\n"
            "string arraytest[] = {\"One\", \"Two\", \"Three\"};\n"
            "boolean isActive(string hostname, uint32 port);\n"
            "};\n";
        PEGASUS_TEST_ASSERT(resultTest(tmp, classCompare));

        CIMInstance instance1 = class1.buildInstance(false, false,
                                                     CIMPropertyList());
        Buffer tmpInstance;
        MofWriter::appendInstanceElement(tmpInstance, instance1);

        char instanceCompare[] =
            "\n//Instance of MyClass\n"
            "instance of MyClass\n"
            "{\n"
            "message = NULL;\n"
            "count = NULL;\n"
            "booltest = NULL;\n"
            "arraytest = {\"One\", \"Two\", \"Three\"};\n"
            "};\n";
        PEGASUS_TEST_ASSERT(resultTest(tmpInstance, instanceCompare));
    }
    // Test CimClass This is not really an association class.
    /*
    [abstract]
        class class1
        {
        [read]
        string DriveLetter;
        [read, Units("KiloBytes")]
        sint32 RawCapacity = 0;
        [write]
        string VolumeLabel;
        boolean NoParmsMethod();
        [Dangerous]
        boolean OneParmMethod([in] boolean FastFormat);
        }; */
    {
        CIMClass class1(CIMName ("SubClass"), CIMName ("SuperClass"));

        class1
            .addQualifier(CIMQualifier(CIMName ("abstract"), true))
                    .addQualifier(CIMQualifier(CIMName ("description"),
               String("This is a Description of my class. "
               "This is part 2 of the string to make it longer. "
               "This is part 3 of the same string for nothing.")))

            .addProperty(CIMProperty(CIMName ("DriveLetter"), String("A"))
                .addQualifier(CIMQualifier(CIMName ("read"), true)))

            .addProperty(CIMProperty(CIMName ("RawCapacity"),Sint32(99))
                .addQualifier(CIMQualifier(CIMName ("read"), true))
                .addQualifier(CIMQualifier(CIMName ("Units"),
                        String("KiloBytes"))))

            .addProperty(CIMProperty(CIMName ("VolumeLabel"), String(" ")))

                .addMethod(CIMMethod(CIMName ("NoParmsMethod"),
                                                CIMTYPE_BOOLEAN))

            .addMethod(CIMMethod(CIMName ("OneParmmethod"), CIMTYPE_BOOLEAN)
                .addParameter(CIMParameter(CIMName ("FastFormat"),
                        CIMTYPE_BOOLEAN)
                           .addQualifier(CIMQualifier(CIMName ("Dangerous"),
                                   true))
                             )
                      )

            .addMethod(CIMMethod(CIMName ("TwoParmMethod"),
                                        CIMTYPE_BOOLEAN)
                .addParameter(CIMParameter(CIMName ("FirstParam"),
                        CIMTYPE_BOOLEAN)
                           .addQualifier(CIMQualifier(
                               CIMName ("Dangerous"), true))
                           .addQualifier(CIMQualifier(
                               CIMName ("in"),true))
                            )
                .addParameter(CIMParameter(CIMName ("SecondParam"),
                        CIMTYPE_BOOLEAN)
                            .addQualifier(CIMQualifier(
                                CIMName ("Dangerous"),
                                    true))
                            .addQualifier(CIMQualifier(
                                CIMName ("in"),true))
                            )
                     )
                     ;
        if (verbose)
            MofWriter::printClassElement(class1);
        Buffer tmp;
        MofWriter::appendClassElement(tmp, class1);

        char classCompare[] =
            "\n//    Class SubClass\n\n"
            "[abstract : DisableOverride, Restricted, \n"
            "description (\"This is a Description of my class. This is part 2 "
            "of the string to make it longer. "
            "This is part 3 of the same string for nothing.\") "
            ": DisableOverride, Restricted]\n"
            "class SubClass : SuperClass\n"
            "{\n"
            "[read : DisableOverride, Restricted]\n"
            "string DriveLetter = \"A\";\n"
            "[read : DisableOverride, Restricted, \n"
            "Units (\"KiloBytes\") : DisableOverride, Restricted]\n"
            "sint32 RawCapacity = 99;\n"
            "string VolumeLabel = \" \";\n"
            "boolean NoParmsMethod();\n"
            "boolean OneParmmethod([Dangerous : DisableOverride, Restricted] "
            "boolean FastFormat);\n"
            "boolean TwoParmMethod([Dangerous : DisableOverride, Restricted, \n"
            "in : DisableOverride, Restricted] boolean FirstParam, "
            "[Dangerous : DisableOverride, Restricted, \n"
            "in : DisableOverride, Restricted] boolean SecondParam);\n"
            "};\n";
        PEGASUS_TEST_ASSERT(resultTest(tmp, classCompare));
    }

}

int main(int argc, char** argv)
{
    verbose = getenv("PEGASUS_TEST_VERBOSE") ? true : false;

    // Test values MOF generation

    //Array<Uint32> aa;
    //CIMValue vv(aa);

    test01(Boolean(true), "TRUE");
    test01(Boolean(false), "FALSE");
    test01(Char16('Z'), "Z");
    test01(Uint8(77), "77");
    test01(Sint8(-77), "-77");
    test01(Sint16(77), "77");
    test01(Sint16(-77), "-77");
    test01(Sint32(77), "77");
    test01(Sint32(-77), "-77");
    test01(Sint64(77), "77");
    test01(Sint64(-77), "-77");
    /* The following Tests are not Portable so are excluded
    test01(Real32(1.5), "1.5000000000000000e+00");
    test01(Real64(55.5), "5.5500000000000000e+01");
    */
    test01(Uint64(123456789), "123456789");
    test01(Sint64(-123456789), "-123456789");
    test01(CIMObjectPath("MyClass.key1=20,key2=\"my name\""),
            "\"MyClass.key1=20,key2=\\\"my name\\\"\"");
    test01(CIMDateTime("19991224120000.000000-300"),
            "\"19991224120000.000000-300\"");

    // Test for array values generation of MOF.
    // Note that we do not test all types.
    Array<String> arr;
    arr.append("One");
    arr.append("Two");
    arr.append("Three");
    CIMValue v(arr);

    test01(arr ,"{\"One\", \"Two\", \"Three\"}" );

    Array<Uint32> arr2;
    arr2.append(1);
    arr2.append(2);
    arr2.append(3);

    test01(arr2, "{1, 2, 3}");

    // test property MOF generation
    test03();

    // Qualifier Declaration
    test04();

    // Test Class and Instance mof generation
    test05();

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}
