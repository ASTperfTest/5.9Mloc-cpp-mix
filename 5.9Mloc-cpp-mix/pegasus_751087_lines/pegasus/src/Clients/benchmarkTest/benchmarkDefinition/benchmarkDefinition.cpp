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
// Author: Denise Eckstein, Hewlett-Packard Company 
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include "benchmarkDefinition.h"

PEGASUS_USING_STD;

#define MAX_NUMBER_OF_PROPERTIES   99999 
#define MAX_SIZE_OF_PROPERTY_VALUE 99999 
#define MAX_NUMBER_OF_INSTANCES    99999 

benchmarkDefinition::benchmarkDefinition ()
{
}

Uint32 benchmarkDefinition::getConfiguration(
                        const CIMName& className,
                        Uint32& numberOfProperties,
                        Uint32& sizeOfPropertyValue,
                        Uint32& numberOfInstances)
{

    String classNameString = className.getString();

    if (!String::equal(classNameString.subString(0, 14), "benchmarkClass"))
    {
       return(CIM_ERR_NOT_SUPPORTED);
    }

    String numberPropertiesString = classNameString.subString(15, 5);
    numberOfProperties = atoi(numberPropertiesString.getCString());
    if (numberOfProperties > MAX_NUMBER_OF_PROPERTIES)
    {
       return(CIM_ERR_NOT_SUPPORTED);
    }

    String sizePropertiesString = classNameString.subString(21, 5);
    sizeOfPropertyValue = atoi(sizePropertiesString.getCString());
    if (sizeOfPropertyValue > MAX_SIZE_OF_PROPERTY_VALUE)
    {
       return(CIM_ERR_NOT_SUPPORTED);
    }

    String numberInstancesString = classNameString.subString(27, 5);
    numberOfInstances = atoi(numberInstancesString.getCString());
    if (numberOfInstances > MAX_NUMBER_OF_INSTANCES)
    {
       return(CIM_ERR_NOT_SUPPORTED);
    }

    return(CIM_ERR_SUCCESS);
}

