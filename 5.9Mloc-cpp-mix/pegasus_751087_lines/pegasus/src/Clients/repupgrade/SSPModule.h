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
// Author: Sushma Fernandes, Hewlett-Packard Company
//         (sushma_fernandes@hp.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_SSPModule_h
#define Pegasus_SSPModule_h

#include <Pegasus/Common/CIMClass.h>
#include <Pegasus/Common/CIMInstance.h>
#include <Pegasus/Common/CIMQualifierDecl.h>

PEGASUS_NAMESPACE_BEGIN

/**
     When moving to a newer version of CIM schema, certain DMTF schema changes 
     could make some elements in an old repository incompatible with 
     the new schema.

     In order to import the impacted elements to the new repository, 
     special processing may have to be performed to make them compatible 
     with the newer version of the schema.  

     The special processing is performed in a Schema Special 
     Processsing Module.  A Schema Special Processsing Module will inherit 
     from a SchemaSpecialProcessModule class defined below: 

 */
class SchemaSpecialProcessModule 
{

public:

    /**

        Constructor

    */
    SchemaSpecialProcessModule(){};

    /**

        Destructor

    */
    virtual ~SchemaSpecialProcessModule(){};

    /**

        Perform special processing on a CIMQualifier. This includes verifying 
        whether the qualifier is impacted. If yes, it must be appropriately 
        updated.

         @param       inputQual       the qualifier to be processed. This will 
                                      be provided by the framework.

         @param       outputQual      updated qualifier. 

         @return      true            if the qualifier should be created.

                      false           if the framework should  
                                      ignore the qualifier. 


    */
    virtual Boolean processQualifier (CIMQualifierDecl& inputQual, 
                                      CIMQualifierDecl& outQual) = 0;

    /**

        Perform special processing on a CIMClass. This includes verifying 
        whether the class is impacted. If yes, it must be appropriately updated.

        @param       inputClass       the class to be processed. This will 
                                      be provided by the framework.

        @param       outputClass      updated class. 

        @return      true             if the class should be created.

                     false            if the framework should
                                      ignore the class. 


    */
    virtual Boolean processClass (CIMClass& inputClass,
                                  CIMClass& outputClass) = 0;

    /**

        Perform special processing on a CIMInstance. This includes verifying 
        whether the instance is impacted. If yes, it must be appropriately 
        updated.

        @param       inputInstance   the qualifier to be processed. This will 
                                     be provided by the framework.

        @param       outputInstance  updated instance. 

        @return      true            if the instance should be created.

                     false           if the framework should ignore
                                     instance.

    */
    virtual Boolean processInstance (CIMInstance& inputInstance,
                                     CIMInstance& outputInstance) = 0;

    /**

        Defines the Special Processing Module entry point.

    */
    static const String CREATE_SSPMODULE_ENTRY_POINT;

};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_SSPModule_h */
