/*
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
*/

#include <Executor/Messages.h>
#include <Executor/tests/TestAssert.h>
#include <stdio.h>
#include <string.h>

#define CHECK(S1,S2) \
    do \
    { \
        PEGASUS_TEST_ASSERT(strcmp(MessageCodeToString(S1), S2) == 0); \
    } \
    while (0)

int main()
{
    CHECK((enum ExecutorMessageCode)0, "<unknown>");
    CHECK(EXECUTOR_PING_MESSAGE, "Ping");
    CHECK(EXECUTOR_OPEN_FILE_MESSAGE, "OpenFile");
    CHECK(EXECUTOR_START_PROVIDER_AGENT_MESSAGE, "StartProviderAgent");
    CHECK(EXECUTOR_DAEMONIZE_EXECUTOR_MESSAGE, "DaemonizeExecutor");
    CHECK(EXECUTOR_REMOVE_FILE_MESSAGE, "RemoveFile");
    CHECK(EXECUTOR_RENAME_FILE_MESSAGE, "RenameFile");
    CHECK(EXECUTOR_AUTHENTICATE_PASSWORD_MESSAGE, "AuthenticatePassword");
    CHECK(EXECUTOR_VALIDATE_USER_MESSAGE, "ValidateUser");
    CHECK(EXECUTOR_CHALLENGE_LOCAL_MESSAGE, "ChallengeLocal");
    CHECK(EXECUTOR_AUTHENTICATE_LOCAL_MESSAGE, "AuthenticateLocal");
    CHECK(EXECUTOR_UPDATE_LOG_LEVEL_MESSAGE, "UpdateLogLevel");
    CHECK((enum ExecutorMessageCode)100, "<unknown>");

    printf("+++++ passed all tests\n");

    return 0;
}
