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
OS_TYPE = windows
OS = win64
ARCHITECTURE = X86X64
COMPILER = msvc

SYS_INCLUDES =

DEPEND_INCLUDES =

DEFINES = -DPEGASUS_PLATFORM_$(PEGASUS_PLATFORM) -D_WIN32_WINNT=0x0400
#-D_WIN32_WINNT=0x0400 -DWINVER=0x0400


#
# Determine the version of the compiler being used.
#
CL_VERSION := $(word 8, $(shell cl.exe 2>&1))
CL_MAJOR_VERSION := $(word 1, $(subst .,  , $(CL_VERSION)))


#
# Determine the version of Windows being used.
# IPv6 is not supported on Windows 2000 (earliest Windows version supported
# by Pegasus), but we don't enforce that here.
# IPv6 is not supported by the VC 6 compiler (CL_MAJOR_VERSION is 12 for VC 6).
#
ifeq ($(CL_MAJOR_VERSION), 12)
    PEGASUS_ENABLE_IPV6 = false
endif


#
# The flags set here should be valid for VC 6.
#
# The -Zm105 flag was added as part of bug 4418 to resolve this compile error:
#   C:\Program Files\Microsoft Visual Studio\VC98\include\xlocale(467) : 
#   fatal error C1076: compiler limit : internal heap limit reached; use /Zm to 
#   specify a higher limit
# 
CXX_VERSION_FLAGS := -Zm105
CXX_VERSION_DEBUG_FLAGS :=
CXX_VERSION_RELEASE_FLAGS :=
LINK_VERSION_RELEASE_FLAGS :=


#
# CL_MAJOR_VERSION 13 is VC 7
#
ifeq ($(CL_MAJOR_VERSION), 13)
    CXX_VERSION_FLAGS := -EHsc
    CXX_VERSION_DEBUG_FLAGS := -Gs
    CXX_VERSION_RELEASE_FLAGS := -Gs -GF -Gy
    LINK_VERSION_RELEASE_FLAGS := /LTCG /OPT:REF /OPT:ICF=5 /OPT:NOWIN98
endif


#
# CL_MAJOR_VERSION 14 is VC 8
#
ifeq ($(CL_MAJOR_VERSION), 14)
    CXX_VERSION_FLAGS := -EHsc
    CXX_VERSION_DEBUG_FLAGS := -RTCc -RTCsu
    CXX_VERSION_RELEASE_FLAGS := -GF -GL -Gy
    LINK_VERSION_RELEASE_FLAGS := /LTCG /OPT:REF /OPT:ICF=5 /OPT:NOWIN98
    DEFINES += -D_CRT_SECURE_NO_DEPRECATE
endif

    CXX_VERSION_FLAGS := -Wp64 -EHsc
    CXX_VERSION_DEBUG_FLAGS := -RTCc -RTCsu

ifdef PEGASUS_USE_DEBUG_BUILD_OPTIONS 
    FLAGS = $(CXX_VERSION_FLAGS) $(CXX_VERSION_DEBUG_FLAGS) -GR -W3 -Od -Zi -MDd -DDEBUG -Fd$(OBJ_DIR)/
    LINK_FLAGS += -debug
else
    FLAGS = $(CXX_VERSION_FLAGS) $(CXX_VERSION_RELEASE_FLAGS) -GR -W3 -O2 -MD
    LINK_FLAGS += $(LINK_VERSION_RELEASE_FLAGS)
endif


ifdef PEGASUS_DEBUG_CIMEXCEPTION
    DEFINES += -DPEGASUS_DEBUG_CIMEXCEPTION
endif

# if PEGASUS_ENABLE_SLP is already set then honor the users preference else
# Enable the compilation of the SLP functions.
#
ifndef PEGASUS_ENABLE_SLP
    PEGASUS_ENABLE_SLP = true
endif

# ATTN KS 20020927 - Add flag to allow conditional testing of interoperability
# changes during interoperability tests.
ifdef PEGASUS_SNIA_INTEROP_TEST
    DEFINES+= -DPEGASUS_SNIA_INTEROP_TEST
endif

RM = mu rm

RMDIRHIER = mu rmdirhier

MKDIRHIER = mu mkdirhier

DIFF = mu compare

SORT = mu sort

COPY = mu copy

MOVE = mu move

CXX = cl -nologo

EXE_OUT = -Fe

LIB_OUT = -out:

OBJ = .obj

OBJ_OUT = -Fo

EXE = .exe

DLL = .dll

ILK = .ilk

PDB = .pdb

EXP = .exp

AR = LINK -nologo -dll

LIB_PREFIX =

LIB_SUFFIX = .lib

TOUCH = mu touch

ECHO = mu echo

LEX = flex

YACC = bison

SH = bash

# Windows DLLs are installed in the $(PEGASUS_HOME)/bin directory
PEGASUS_DEST_LIB_DIR = bin
 
# The Provider User Context feature (PEP 197) is not supported on Windows
PEGASUS_DISABLE_PROV_USERCTXT=1

# Windows does not support local domain sockets or the equivalent Bug 2147
PEGASUS_DISABLE_LOCAL_DOMAIN_SOCKET=1

# l10n
ifdef PEGASUS_HAS_MESSAGES
    DEFINES += -DPEGASUS_HAS_MESSAGES
    ifdef ICU_ROOT
        MSG_COMPILE = genrb
        MSG_FLAGS =
        MSG_SOURCE_EXT = .txt
        MSG_COMPILE_EXT = .res
        CNV_ROOT_CMD = cnv2rootbundle

##################################
##
## ICU_NO_UPPERCASE_ROOT if set, specifies NOT to uppercase the root resource bundle,
## default is to uppercase the root resource bundle##
##################################

ifdef ICU_NO_UPPERCASE_ROOT
    CNV_ROOT_FLAGS =
else
    CNV_ROOT_FLAGS = -u
endif

####################################
##
##   ICU_ROOT_BUNDLE_LANG if set, specifies the language that the root resource bundle will be generated from
##   defaults to _en if not set.  if set, for any directory containing resource bundles,
##   there must exist a file name: package(the value of ICU_ROOT_BUNDLE_LANG).txt or the make messages target will fail
##
##   We have to use the full path library because the cl /Fe command in program-windows.mak does
##   not take libpath as an argument (as far as I can tell)
####################################

ifdef ICU_ROOT_BUNDLE_LANG
    MSG_ROOT_SOURCE = $(ICU_ROOT_BUNDLE_LANG)
else
    MSG_ROOT_SOURCE = _en
endif
        DEFINES += -DPEGASUS_HAS_ICU
        EXTRA_LIBRARIES += $(ICU_INSTALL)/lib/icuuc.lib $(ICU_INSTALL)/lib/icuin.lib $(ICU_INSTALL)/lib/icudt.lib
        SYS_INCLUDES += -I$(ICU_ROOT)/source/common -I$(ICU_ROOT)/source/i18n
    endif
endif

PEGASUS_JAVA_CLASSPATH_DELIMITER = ;

##################################
##
## Pegasus WMIMapper
## 
##################################

ifeq ($(PEGASUS_BUILD_WMIMAPPER),true)
  FLAGS += -DPEGASUS_WMIMAPPER
else 
  ifdef PEGASUS_WMIMAPPER
    FLAGS += -DPEGASUS_WMIMAPPER
  endif   
endif

##################################
## 
## The newer compiler versions need neither MS Platform SDK installed nor MSSdk variable defined.
##
##################################
ifeq ($(PEGASUS_BUILD_WMIMAPPER),true)
  ifeq ($(CL_MAJOR_VERSION), 12)
    PEGASUS_WMIMAPPER_NEED_MSSDK=true
  endif
  ifeq ($(CL_MAJOR_VERSION), 13) 
    PEGASUS_WMIMAPPER_NEED_MSSDK=true
  endif
  ifeq ($(PEGASUS_WMIMAPPER_NEED_MSSDK),true)
    ifndef MSSdk
      $(error MSSdk environment variable undefined)
    endif
  endif
endif
