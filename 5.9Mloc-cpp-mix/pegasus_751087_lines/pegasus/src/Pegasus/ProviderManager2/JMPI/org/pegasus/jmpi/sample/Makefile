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
ROOT = ../../../../../../../..

include $(ROOT)/mak/config.mak

.SUFFIXES:
.SUFFIXES: .java .class

.java.class:
	$(PEGASUS_JAVA_COMPILER) -classpath $(PEGASUS_HOME)/lib/JMPIImpl.jar $*.java

PACKAGEDIR = org/pegasus/jmpi/sample/

JAR1        = TestJavaProvider.jar
JAR1CLASSES = \
   TestJavaProvider.class
JAR2        = TestJavaProviderA.jar
JAR2CLASSES = \
   TestJavaProviderA.class
JAR3        = TestJavaProviderB.jar
JAR3CLASSES = \
   TestJavaProviderB.class
JARS        = $(JAR1) $(JAR2) $(JAR3)

all build: $(addprefix $(PEGASUS_HOME)/lib/,$(JARS))

depend:

clean:
	$(RM) *.class
	$(RM) $(JARS)

repository:
	cimmofl -I. -nroot/cimv2 $(subst .jar,,$(JAR1)).mof
	cimmofl -I. -nroot/PG_InterOp $(subst .jar,,$(JAR1))R.mof
	cimmofl -I. -nroot/cimv2 $(subst .jar,,$(JAR2)).mof
	cimmofl -I. -nroot/PG_InterOp $(subst .jar,,$(JAR2))R.mof
	cimmofl -I. -nroot/cimv2 $(subst .jar,,$(JAR3)).mof
	cimmofl -I. -nroot/PG_InterOp $(subst .jar,,$(JAR3))R.mof

$(PEGASUS_HOME)/lib/$(JAR1): $(JAR1CLASSES)
	$(PEGASUS_JAVA_JAR) -cf $@ $(addprefix -C ../../../.. $(PACKAGEDIR),$^)

$(PEGASUS_HOME)/lib/$(JAR2): $(JAR2CLASSES)
	$(PEGASUS_JAVA_JAR) -cf $@ $(addprefix -C ../../../.. $(PACKAGEDIR),$^)

$(PEGASUS_HOME)/lib/$(JAR3): $(JAR3CLASSES)
	$(PEGASUS_JAVA_JAR) -cf $@ $(addprefix -C ../../../.. $(PACKAGEDIR),$^)
