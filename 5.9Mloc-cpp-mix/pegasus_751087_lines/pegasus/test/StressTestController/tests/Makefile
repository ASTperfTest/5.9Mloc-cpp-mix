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
ROOT = $(PEGASUS_ROOT)

DIR = ../test/StressTestController/tests

include $(ROOT)/mak/config.mak
include $(ROOT)/mak/test.mak

RESULTFILE =  $(TMP_DIR)/result
MASTERRESULTFILE = $(ROOT)/src/$(DIR)/result.master


clean:
	@$(RM) $(RESULTFILE)

tests:
	@$(RM) $(RESULTFILE)
	@$(ECHO) Running Usage tests for: TestStressTestController  
	@$(MAKE) -i -s stresstests_ignoreerror
	@$(STRIPCRS)
	$(COMPARERESULTS)
	@$(ECHO) +++ Usage Tests passed +++
	@$(RM) $(RESULTFILE)

stresstests_ignoreerror:
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -a >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -b >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -c >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -d >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -e >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -f >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -g >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -i >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -j >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -k >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -l >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -m >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -n >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -o >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -q >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -t >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -v >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -x >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -y >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@TestStressTestController -z >> $(RESULTFILE) $(REDIRECTERROR)

	@$(ECHO) "Missing required value for hostname - error" >> $(RESULTFILE)
	@TestStressTestController -h >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Missing required value for port - error" >> $(RESULTFILE)
	@TestStressTestController -p >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Missing required value for username - error" >> $(RESULTFILE)
	@TestStressTestController -u >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Missing required value for password - error" >> $(RESULTFILE)
	@TestStressTestController -w >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Display usage" >> $(RESULTFILE)
	@TestStressTestController --help >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Missing configuration file - error" >> $(RESULTFILE)
	@TestStressTestController nofile.conf >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Unexpected Argument - error" >> $(RESULTFILE)
	@TestStressTestController nofile.conf anotherfile.conf>> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "Invalid Tolerance, Port number, & Host name  - error" >> $(RESULTFILE)
	@TestStressTestController invalid_configuration.conf >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "No clients specified in Configuration - error" >> $(RESULTFILE)
	@TestStressTestController invalid_noclients.conf>> $(RESULTFILE) $(REDIRECTERROR)

poststarttests:

depend:
