#//%2006////////////////////////////////////////////////////////////////////////
#//
#// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
#// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
#// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
#// IBM Corp.; EMC Corporation, The Open Group.
#// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
#// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
#// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
#// EMC Corporation; VERITAS Software Corporation; The Open Group.
#// Copyright (c) 2006 Hewlett-Packard Development Company, L.P.; IBM Corp.;
#// EMC Corporation; Symantec Corporation; The Open Group.
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy
#// of this software and associated documentation files (the "Software"), to
#// deal in the Software without restriction, including without limitation the
#// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#// sell copies of the Software, and to permit persons to whom the Software is
#// furnished to do so, subject to the following conditions:
#// 
#// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
#// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
#// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
#// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#//
#//==============================================================================
ROOT = ../../../../..
DIR = Pegasus/Common/tests/Buffer
include $(ROOT)/mak/config.mak
include ../libraries.mak

LOCAL_DEFINES = -DPEGASUS_INTERNALONLY

##
## Add -DPEGASUS_STRESS_BUFFER_MEMORY if PEGASUS_STRESS_BUFFER_MEMORY 
## environment variable defined.
##
ifdef PEGASUS_STRESS_BUFFER_MEMORY
    ifeq ($(PEGASUS_STRESS_BUFFER_MEMORY),true)
        LOCAL_DEFINES += -DPEGASUS_STRESS_BUFFER_MEMORY
    else
        ifneq ($(PEGASUS_STRESS_BUFFER_MEMORY),false)
            $(error "PEGASUS_STRESS_BUFFER_MEMORY must be true or false")
        endif
    endif
endif

PROGRAM = TestBuffer
SOURCES = Buffer.cpp

include $(ROOT)/mak/program.mak

tests:
	$(PROGRAM)

poststarttests:
