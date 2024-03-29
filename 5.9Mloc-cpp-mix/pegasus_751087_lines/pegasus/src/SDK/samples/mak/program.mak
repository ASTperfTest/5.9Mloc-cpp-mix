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
include $(ROOT)/mak/common.mak

PATHED_PROGRAM = $(PEGASUS_SAMPLE_BIN_DIR)/$(PROGRAM)
TMP_LIST_STEP1 = $(foreach i,$(SOURCES),$(PEGASUS_SAMPLE_OBJ_DIR)/$i)
TMP_LIST_STEP2 = $(TMP_LIST_STEP1:.cpp=$(OBJ_SUFFIX))
PATHED_OBJECTS = $(TMP_LIST_STEP2:.c=$(OBJ_SUFFIX))

$(PEGASUS_SAMPLE_OBJ_DIR)/%.o: %.cpp
	$(COMPILE_CXX_COMMAND) $(PROGRAM_COMPILE_OPTIONS) -c -o $@ -I $(PEGASUS_INCLUDE_DIR) $(DEFINES) $*.cpp

$(PATHED_PROGRAM): $(PEGASUS_SAMPLE_OBJ_DIR)/target $(PEGASUS_SAMPLE_BIN_DIR)/target $(PATHED_OBJECTS)
	$(PROGRAM_LINK_COMMAND) $(PROGRAM_LINK_OPTIONS) $(LINK_OUT)$@ $(PATHED_OBJECTS) $(DYNAMIC_LIBRARIES) $(SYS_LIBS)

clean:
	@$(foreach i, $(PATHED_OBJECTS), $(RM) $(i);)
	@$(foreach i, $(PATHED_PROGRAM), $(RM) $(i);)
	@$(foreach i, $(ADDITIONAL_CLEAN_FILES), $(RM) $(i);)

depend:

tests:

poststarttests:

include $(ROOT)/mak/build.mak

