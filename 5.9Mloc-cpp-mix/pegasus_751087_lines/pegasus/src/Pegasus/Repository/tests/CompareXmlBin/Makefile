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

#
# CompareXmlBin Makefile
#
# Commands available are:
#
# make tests			runs the whole test - build, test clean
#
# make repo_build		build the default (Interanl) repository
# make repo_build_whole		builds the repository with all namespaces
# make repo_build_Internal  	build the repository with Interanal namespace 
# make repo_build_Interop	build the repository with InterOp namespace
# make repo_build_ManagedSystem	build the repository with ManagedSystem namespace
#
# make run_test			runs the test
# make clean			cleans all
#

ROOT = ../../../../..
DIR = Pegasus/Repository/tests/CompareXmlBin

include $(ROOT)/mak/config.mak
include $(ROOT)/mak/test.mak

##
## REPO_DIR : Directory to build the test repositories in.
##
## NOTE: The shorthand notation of '.' (DOT) for the current working directory 
##       cannot be used in REPO_DIR. This path specification gets passed to
##       the make repositories makefiles that are executing within the context
##       of the schema directory. This would cause a repository to be created 
##       in each direcory that is being processed as part of the repository
##       build. 
##

## REPO_DIR = $(REPOSITORY_DIR) 
ifeq ($(TMP_DIR),.)
    REPO_DIR = $(PEGASUS_ROOT)/src/$(DIR)
else
    REPO_DIR = $(TMP_DIR)
endif


## REPO_XXX : The file name to be used for each type of test repository.
## 
REPO_XML = repository_xml
REPO_BIN = repository_bin

.PHONY: repo_build run_test tests poststarttests

tests:  repo_build run_test clean
	@ $(ECHO) +++++ CompareXmlBin repository test: FINISHED with test

#
# For the following poststarttest rule to work the comparerepositories 
# test will have to understand how to open a connection to the server
# or a file. Build one (XML) repository in the server and the BIN 
# in a local file then compare them. It might be an interesting test
#
# poststarttests: repo_Server_build run_test clean
# 	@ $(ECHO) +++++ CompareXmlBin repository test: FINISHED with test
#

poststarttests:

clean:
	@ $(RMREPOSITORY) $(REPO_DIR)/$(REPO_XML)
	@ $(RMREPOSITORY) $(REPO_DIR)/$(REPO_BIN)

messages:

depend:

run_test:
	TestCompareRepositories $(REPO_DIR)/$(REPO_XML) $(REPO_DIR)/$(REPO_BIN) 

#
# build both XML and BIN in local files 
#
# The following would build the entire repository with all name spaces
#
#	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
#
# To save time the Internal namespace is built rather than the whole repository
# To build the whole repository or other name namespaces, then use the 
# appropiate repo_build rule dependency
# EG:
#
# repo_build: repo_build_all
# repo_build: repo_build_Internal
# repo_build: repo_build_InterOp
# repo_build: repo_build_ManagedSystems


repo_build: repo_build_Internal

repo_build_all:
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_BIN) REPOSITORY_MODE=BIN


repo_build_Internal:
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/Internal repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/Internal repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_BIN) REPOSITORY_MODE=BIN

repo_build_InterOp:
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/InterOp repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/InterOp repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_BIN) REPOSITORY_MODE=BIN

repo_build_ManagedSystems:
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/ManagedSystems repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/ManagedSystems repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_BIN) REPOSITORY_MODE=BIN

#
# build the XML with the server
# build the BIN in a local file
#
repo_Server_build:
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/Internal repositoryServer REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_XML) REPOSITORY_MODE=XML
	@ $(MAKE) "-SC" $(ROOT)/Schemas/Pegasus/Internal repository REPOSITORY_DIR=$(REPO_DIR) REPOSITORY_NAME=$(REPO_BIN) REPOSITORY_MODE=BIN

