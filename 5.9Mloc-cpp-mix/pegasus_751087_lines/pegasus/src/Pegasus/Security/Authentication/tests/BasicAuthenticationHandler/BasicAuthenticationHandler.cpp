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

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/PegasusAssert.h>
#include <iostream>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Base64.h>
#include <Pegasus/Common/AuthenticationInfo.h>
#include <Pegasus/Config/ConfigManager.h>
#include <Pegasus/Repository/CIMRepository.h>
#include <Pegasus/Security/UserManager/UserManager.h>
#include <Pegasus/Security/Authentication/BasicAuthenticationHandler.h>

//
// Enable debug messages
//
//#define DEBUG 1


PEGASUS_USING_PEGASUS;

PEGASUS_USING_STD;

static Boolean verbose;

String authType = "Basic";

String testUser = System::getEffectiveUserName();

String guestUser = "guest";

String guestPassword = "guest";

String invalidUser = "xyz123ww";

String invalidPassword = "xyz123ww";

String encodeUserPass(const String& userPass)
{
    //
    // copy userPass string content to Uint8 array for encoding
    //
    Buffer userPassArray;

    Uint32 userPassLength = userPass.size();

    userPassArray.reserveCapacity( userPassLength );
    userPassArray.clear();

    for( Uint32 i = 0; i < userPassLength; i++ )
    {
        userPassArray.append( (Uint8)userPass[i] );
    }

    //
    // base64 encode the user name and password
    //
    Buffer encodedArray;

    encodedArray = Base64::encode( userPassArray );

    String encodedStr =
        String( encodedArray.getData(), encodedArray.size() );

    if (verbose) cout << "userPass: " << userPass << endl;
    if (verbose) cout << "Encoded userPass: " << encodedStr << endl;

    return (encodedStr);
}

void testAuthHeader()
{
    BasicAuthenticationHandler  basicAuthHandler;

    String respHeader = basicAuthHandler.getAuthResponseHeader();

    if (verbose) cout << "realm = " << respHeader << endl;

    PEGASUS_TEST_ASSERT(respHeader.size() != 0);
}

//
// Test with invalid userPass (with no ':' separator)
//
void testAuthenticationFailure_1()
{
    String authHeader;
    Boolean authenticated;

    BasicAuthenticationHandler  basicAuthHandler;

    AuthenticationInfo* authInfo = new AuthenticationInfo(true);

    //
    // Test with invalid user password string
    //
    String userPass = testUser;
    userPass.append(guestPassword);

    authHeader.append(encodeUserPass(userPass));

    authenticated = basicAuthHandler.authenticate(authHeader, authInfo);

    if (authenticated)
        if (verbose)
            cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose)
            cout << "User " + testUser + " authentication failed." << endl;

    delete authInfo;

    PEGASUS_TEST_ASSERT(!authenticated);
}

//
// Test with invalid system user
//
void testAuthenticationFailure_2()
{
    String authHeader;
    Boolean authenticated;

    BasicAuthenticationHandler  basicAuthHandler;

    AuthenticationInfo* authInfo = new AuthenticationInfo(true);

    //
    // Test with invalid password
    //
    String userPass = invalidUser;
    userPass.append(":");
    userPass.append(guestPassword);

    authHeader.append(encodeUserPass(userPass));

    authenticated = basicAuthHandler.authenticate(authHeader, authInfo);

    if (authenticated)
        if (verbose)
            cout << "User " + invalidUser + " authenticated successfully."
                 << endl;
    else
        if (verbose)
            cout << "User " + invalidUser + " authentication failed." << endl;

    delete authInfo;

    PEGASUS_TEST_ASSERT(!authenticated);
}

//
// Test with invalid password
//
void testAuthenticationFailure_3()
{
    String authHeader;
    Boolean authenticated;

    BasicAuthenticationHandler  basicAuthHandler;

    AuthenticationInfo* authInfo = new AuthenticationInfo(true);

    String userPass = guestUser;
    userPass.append(":");
    userPass.append(invalidPassword);

    authHeader.append(encodeUserPass(userPass));

    authenticated = basicAuthHandler.authenticate(authHeader, authInfo);

    if (authenticated)
        if (verbose)
            cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose)
            cout << "User " + testUser + " authentication failed." << endl;

    delete authInfo;

    PEGASUS_TEST_ASSERT(!authenticated);
}

//
// Test with invalid CIM user or invalid password
//
void testAuthenticationFailure_4()
{
    String authHeader;
    Boolean authenticated;

    BasicAuthenticationHandler  basicAuthHandler;

    AuthenticationInfo* authInfo = new AuthenticationInfo(true);

    String userPass = invalidUser;
    userPass.append(":");
    userPass.append(invalidPassword);

    authHeader.append(encodeUserPass(userPass));

    authenticated = basicAuthHandler.authenticate(authHeader, authInfo);

    if (authenticated)
        if (verbose)
            cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose)
            cout << "User " + testUser + " authentication failed." << endl;

    delete authInfo;

    PEGASUS_TEST_ASSERT(!authenticated);
}

//
// Test with valid user name and password
// (Assuming there is a valid CIM user 'guest' with password 'guest')
//
void testAuthenticationSuccess()
{
    String authHeader;

    BasicAuthenticationHandler  basicAuthHandler;

    AuthenticationInfo* authInfo = new AuthenticationInfo(true);

    String userPass = guestUser;
    userPass.append(":");
    userPass.append(guestPassword);

    authHeader.append(encodeUserPass(userPass));

    Boolean authenticated;

    authenticated = basicAuthHandler.authenticate(authHeader, authInfo);

    if (authenticated)
        if (verbose)
            cout << "User " + guestUser + " authenticated successfully."
                 << endl;
    else
        if (verbose)
            cout << "User " + guestUser + " authentication failed." << endl;

    delete authInfo;

    //PEGASUS_TEST_ASSERT(authenticated);
}

////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    verbose = (getenv ("PEGASUS_TEST_VERBOSE")) ? true : false;
    if (verbose) cout << argv[0] << ": started" << endl;

#if defined(PEGASUS_OS_TYPE_UNIX)

    try
    {
#ifdef DEBUG
        Tracer::setTraceFile("./Authentication.trc");
        Tracer::setTraceComponents("all");
        Tracer::setTraceLevel(Tracer::LEVEL4);
        verbose = true;
#endif

        ConfigManager* configManager = ConfigManager::getInstance();

        const char* path = getenv("PEGASUS_HOME");
        String pegHome = path;

        if(pegHome.size())
            ConfigManager::setPegasusHome(pegHome);

        if (verbose)
            cout << "Peg Home : " << ConfigManager::getPegasusHome() << endl;

        if (verbose)
            cout << "Doing testAuthHeader()...." << endl;

        // -- Create a test repository:

        const char* tmpDir = getenv ("PEGASUS_TMP");
        String repositoryPath;
        if (tmpDir == NULL)
        {
            repositoryPath = ".";
        }
        else
        {
            repositoryPath = tmpDir;
        }
        repositoryPath.append("/repository");

        PEGASUS_TEST_ASSERT(FileSystem::isDirectory(repositoryPath));

        CIMRepository* repository = new CIMRepository(repositoryPath);

        // -- Create a UserManager object:

        UserManager* userManager = UserManager::getInstance(repository);

        testAuthHeader();

        if (verbose) cout << "Doing testAuthenticationFailure_1()...." << endl;
        testAuthenticationFailure_1();

        if (verbose) cout << "Doing testAuthenticationFailure_2()...." << endl;
        testAuthenticationFailure_2();

        if (verbose) cout << "Doing testAuthenticationFailure_3()...." << endl;
        testAuthenticationFailure_3();

        if (verbose) cout << "Doing testAuthenticationFailure_4()...." << endl;
        testAuthenticationFailure_4();

        if (verbose) cout << "Doing testAuthenticationSuccess()...." << endl;
        testAuthenticationSuccess();
    }
    catch(Exception& e)
    {
        cout << argv[0] << " Exception: " << e.getMessage() << endl;
        PEGASUS_TEST_ASSERT(0);
    }

#endif

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}