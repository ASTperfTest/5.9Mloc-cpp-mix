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
# Makefile builds a Sample Provider Namespace
# ATTN: Today this makefile is implementation dependent.
#       It assumes that the repository is a set of directories
#       that can be removed by removing the directories.
#       This must be updated to an implementation independent repository
#       remove function.

ROOT = $(PEGASUS_ROOT)
include $(ROOT)/mak/config.mak
include $(ROOT)/mak/configschema.mak

# Loads the sample MOF into a separate namespace.
# The following variable determine the version of the Schema to
# be loaded into the Sample Provider namespace.
# Update the following variable to change the version.

SAMPLEPROVIDERNS=root/SampleProvider
SAMPLEPROVIDERNSDIRECTORY=$(REPOSITORY_ROOT)/root\#SampleProvider

# Load the sample MOF with the local compiler. Cleans the existing repository
# before reloading.
# NOTE: I believe that the remove is duplicated for some reason.
repository:
	@ $(ECHO) +++++ Removing existing repository named $(SAMPLEPROVIDERNS)
	@$(RMREPOSITORY) "$(SAMPLEPROVIDERNSDIRECTORY)"
	@ $(ECHO) +++++ Loading CIM_Core$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Core$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_Event$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Event$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_Physical$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Physical$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_System$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_System$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading PG_Events$(PG_INTEROP_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-I$(PG_INTEROP_MOF_PATH)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(PG_INTEROP_MOF_PATH)/PG_Events$(PG_INTEROP_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading SampleProvider class definitions into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" SampleProviderSchema.mof
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" AssociationProvider.mof
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" ClientTest.mof
	@ $(ECHO) +++++ Registering Sample Providers  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" SampleProviderSchemaR.mof
ifdef PEGASUS_ENABLE_EXECQUERY
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" InstanceQueryProviderR.mof
endif
ifeq ($(PEGASUS_ENABLE_CMPI_PROVIDER_MANAGER), true)
	@ $(ECHO) +++++ Installing the MOF file for CMPI Providers
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" CWS_FilesAndDir.mof
	@ $(ECHO) +++++ Registering FilesAndDir schema Providers  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" CWS_FilesAndDirR.mof

	@ $(ECHO) +++++ Installing the MOF file for CMPI ProcessIndication Provider
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" RT_Sample.mof
	@ $(ECHO) +++++ Registering CMPI ProcessIndication Provider  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" RT_SampleR.mof

	@ $(ECHO) +++++ Registering FilesAndDir CXX schema Providers  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" CWS_FilesAndDir_CXX_R.mof

	@ $(ECHO) +++++ Registering TestCMPI_CXX schema Providers  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" TestCMPI_CXX.mof
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" TestCMPI_CXX_R.mof

	@ $(ECHO) +++++ Installing the MOF file for SampleClass CMPI Provider
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" CMPISampleClass.mof
	@ $(ECHO) +++++ Registering CMPI SampleClass Provider  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" CMPISampleClassR.mof

	@ $(ECHO) +++++ Loading CMPI Sample Provider class definitions into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(SAMPLEPROVIDERNS)" CMPISampleProviderSchema.mof
	
	@ $(ECHO) +++++ Registering CMPI Sample Providers  ...
	@ cimmofl "-R$(REPOSITORY_DIR)" "-N$(REPOSITORY_NAME)" "-M$(REPOSITORY_MODE)" "-n$(INTEROPNS)" CMPISampleProviderSchemaR.mof
endif

# Load the sample mof through the Client interface compiler but do not
# clean out the existing version.
repositoryServer:
	@ $(ECHO) +++++ Loading CIM_Core$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Core$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_Event$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Event$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading PG_Events$(PG_INTEROP_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-I$(PG_INTEROP_MOF_PATH)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(PG_INTEROP_MOF_PATH)/PG_Events$(PG_INTEROP_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_Physical$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_Physical$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading CIM_System$(CIM_SCHEMA_VER) into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-I$(CIM_SCHEMA_DIR)" "-n$(SAMPLEPROVIDERNS)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/CIM_System$(CIM_SCHEMA_VER).mof
	@ $(ECHO) +++++ Loading SampleProvider class definitions into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmof "-n$(SAMPLEPROVIDERNS)" SampleProviderSchema.mof
	@ cimmof "-n$(SAMPLEPROVIDERNS)" AssociationProvider.mof
	@ cimmof "-n$(SAMPLEPROVIDERNS)" ClientTest.mof
	@ $(ECHO) +++++ Registering SampleProvider schema Providers  ...
	@ cimmof "-n$(INTEROPNS)" SampleProviderSchemaR.mof
ifdef PEGASUS_ENABLE_EXECQUERY
	@ cimmof "-n$(INTEROPNS)" InstanceQueryProviderR.mof
endif
ifeq ($(PEGASUS_ENABLE_CMPI_PROVIDER_MANAGER), true)
	@ $(ECHO) +++++ Installing the MOF file for CMPI Providers
	@ cimmof "-n$(SAMPLEPROVIDERNS)" CWS_FilesAndDir.mof
	@ $(ECHO) +++++ Registering FilesAndDir schema Providers  ...
	@ cimmof "-n$(INTEROPNS)" CWS_FilesAndDirR.mof

	@ $(ECHO) +++++ Installing the MOF file for CMPI ProcessIndication Provider
	@ cimmof "-n$(SAMPLEPROVIDERNS)" RT_Sample.mof
	@ $(ECHO) +++++ Registering CMPI ProcessIndication Provider  ...
	@ cimmof "-n$(INTEROPNS)" RT_SampleR.mof

	@ $(ECHO) +++++ Registering FilesAndDir_CXX_R schema Providers  ...
	@ cimmof "-n$(INTEROPNS)" CWS_FilesAndDir_CXX_R.mof
	
	@ $(ECHO) +++++ Registering TestCMPI_CXX_R schema Providers  ...
	@ cimmof "-n$(SAMPLEPROVIDERNS)" TestCMPI_CXX.mof
	@ cimmof "-n$(INTEROPNS)" TestCMPI_CXX_R.mof
	
	@ $(ECHO) +++++ Installing the MOF file for SampleClass CMPI Provider
	@ cimmof "-n$(SAMPLEPROVIDERNS)" CMPISampleClass.mof

	@ $(ECHO) +++++ Registering SampleClass CMPI Provider  ...
	@ cimmof "-n$(INTEROPNS)" CMPISampleClassR.mof

	@ $(ECHO) +++++ Loading CMPI Sample Provider class definitions into $(SAMPLEPROVIDERNS) namespace ...
	@ cimmofl "-n$(SAMPLEPROVIDERNS)" CMPISampleProviderSchema.mof
	
	@ $(ECHO) +++++ Registering CMPI Sample Providers  ...
	@ cimmofl "-n$(INTEROPNS)" CMPISampleProviderSchemaR.mof
endif

removerepository:
	@ $(ECHO) +++++ Removing existing repository named $(SAMPLEPROVIDERNS)
	@ $(RMREPOSITORY) "$(SAMPLEPROVIDERNSDIRECTORY)"

unregister:
	$(MAKE) -i unregisterproviders

unregisterproviders:
	cimprovider -r -m SampleIndicationProviderModule
	cimprovider -r -m SampleInstanceProviderModule
ifdef PEGASUS_ENABLE_EXECQUERY
	cimprovider -r -m SampleInstanceQueryProviderModule
endif
	cimprovider -r -m SampleMethodProviderModule
	cimprovider -r -m SampleAssociationProviderModule
	cimprovider -r -m SimpleDisplayConsumerModule
ifeq ($(PEGASUS_ENABLE_CMPI_PROVIDER_MANAGER), true)
	cimprovider -r -m FilesAndDirectoriesModule
	cimprovider -r -m CMPI_RT_SampleProviderModule
	cimprovider -r -m FilesAndDirectoriesModule_CXX
	cimprovider -r -m CMPISampleClassModule
	cimprovider -r -m SampleCMPIMethodProviderModule
endif

depend:

sub:

misc:

tests:

poststarttests:

general:

clean:
