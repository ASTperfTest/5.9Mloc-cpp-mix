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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define CMPI_VERSION 100

#include <Pegasus/Provider/CMPI/cmpidt.h>
#include <Pegasus/Provider/CMPI/cmpift.h>
#include <Pegasus/Provider/CMPI/cmpimacs.h>
#include <Providers/TestProviders/CMPI/TestUtilLib/cmpiUtilLib.h>

#define _ClassName "CMPI_TEST_Racing"
#define _ClassName_size strlen(_ClassName)

#define _Namespace "test/TestProvider"
#define _ProviderLocation \
    "/src/Providers/TestProviders/CMPI/TestAssociation/tests/"

static const CMPIBroker *_broker;

/* ---------------------------------------------------------------------------*/
/*                       CMPI Helper function                                 */
/* ---------------------------------------------------------------------------*/

const char * get_assoc_targetClass_Name(
    const CMPIBroker * _broker,
    const CMPIObjectPath * ref,
    const char * _RefLeftClass,
    const char * _RefRightClass,
    CMPIStatus * rc) 
{
    CMPIString * sourceClass = NULL;
    CMPIObjectPath * op = NULL;

    /* get name of source class */
    sourceClass = CMGetClassName(ref, rc);
    
    op = CMNewObjectPath(
        _broker,
        CMGetCharPtr(CMGetNameSpace(ref,rc)),
        _RefLeftClass,
        rc );

    if (strcmp(CMGetCharPtr(sourceClass),"CMPI_TEST_Person") == 0 )
    {
        return "CMPI_TEST_Vehicle"; 
    }
    else if ( strcmp(CMGetCharPtr(sourceClass),"CMPI_TEST_Vehicle") == 0 )
    {
        return "CMPI_TEST_Person";
    }
    else
    {
        return NULL;
    }
}


CMPIObjectPath* get_assoc_targetClass_ObjectPath(
    const CMPIBroker* _broker,
    const CMPIObjectPath* ref,
    const char* _RefLeftClass,
    const char* _RefRightClass,
    CMPIStatus* rc ) 
{
    CMPIObjectPath* op = NULL;
    const char* targetName = NULL;
    
    /* get name of the target class */
    targetName = get_assoc_targetClass_Name(
        _broker,
        ref,
        _RefLeftClass,
        _RefRightClass,
        rc);
    
    if ( targetName != NULL ) 
    {
        /* create new object path of the target class */
        op = CMNewObjectPath(
            _broker,
            CMGetCharPtr(CMGetNameSpace(ref,rc)),
            targetName,
            rc);
    }
    return op;
}

/* ---------------------------------------------------------------------------*/
/*                      Association Provider Interface                        */
/* ---------------------------------------------------------------------------*/
//CMPIPROVIDER

CMPIStatus TestCMPIAssociationProviderAssociationCleanup(
    CMPIAssociationMI* mi, 
    const CMPIContext* ctx,
    CMPIBoolean  term)
{

    CMReturn (CMPI_RC_OK);
}

CMPIStatus TestCMPIAssociationProviderAssociators(
    CMPIAssociationMI* mi,
    const CMPIContext* ctx,
    const CMPIResult* rslt,
    const CMPIObjectPath* ref,
    const char* _RefLeftClass,
    const char* _RefRightClass,
    const char* role, 
    const char* resultRole, 
    const char** properties)
{
    CMPIObjectPath * op = NULL;
    CMPIEnumeration * en = NULL;          
    CMPIData data ;
    CMPIStatus rc = { CMPI_RC_OK, NULL }; 

    CMPIString * sourceClass = NULL;

    PROV_LOG_OPEN (_ClassName, _ProviderLocation);                
    PROV_LOG ("\n\n********************* %s CMPI Associators() called", 
        _ClassName);
        
    sourceClass = CMGetClassName(ref,&rc);

    /* get object path of the target class */
    op = get_assoc_targetClass_ObjectPath(
        _broker,
        ref,
        _RefLeftClass,
        _RefRightClass,
        &rc);

    sourceClass = CMGetClassName(op,&rc);
    PROV_LOG(" target class: %s ",CMGetCharPtr(sourceClass));
    
    PROV_LOG (" New Object Path [%s]",
                CMGetCharPtr (CMGetNameSpace (ref, &rc)));

    /* Call to Associators */
    /* upcall to CIMOM; call enumInstances() of the target class */
    en = CBEnumInstances( _broker, ctx, op, NULL, &rc);

    /* as long as instance entries are found in the enumeration */
    while( CMHasNext( en, &rc) ) 
    {
        /* get the instance */
        data = CMGetNext( en, &rc);
                        
        /* and return the target class instance as result of the
         * associators() call
        */
        CMReturnInstance( rslt, data.value.inst );
    }
    PROV_LOG ("\n\n********************* %s CMPI Associators exited",
        _ClassName);
    PROV_LOG_CLOSE ();
    return rc;
}

CMPIStatus TestCMPIAssociationProviderAssociatorNames(
    CMPIAssociationMI* mi,
    const CMPIContext* ctx,
    const CMPIResult* rslt,
    const CMPIObjectPath* ref,
    const char* _RefLeftClass,
    const char* _RefRightClass,
    const char* role,
    const char* resultRole)
{
    CMPIObjectPath* op = NULL;
    CMPIObjectPath* rop = NULL;
    CMPIEnumeration* en = NULL;
    CMPIData data ;

    CMPIStatus rc = { CMPI_RC_OK, NULL }; 

    PROV_LOG_OPEN (_ClassName, _ProviderLocation);                           
    PROV_LOG ("\n\n********************* %s CMPI AssociatorNames() called",
        _ClassName);

    /* get object path of the target class */
    op = get_assoc_targetClass_ObjectPath(
        _broker,
        ref,
        _RefLeftClass,
        _RefRightClass,
        &rc);

    PROV_LOG (" New Object Path [%s]",
        CMGetCharPtr (CMGetNameSpace (ref, &rc)));                

    /* create new object path of association */
    rop = CMNewObjectPath(
        _broker,
        CMGetCharPtr(CMGetNameSpace(ref,&rc)),
        _ClassName,
        &rc );

    /* upcall to CIMOM; call enumInstanceNames() of the target class */
    en = CBEnumInstanceNames( _broker, ctx, op, &rc);

    /* as long as object path entries are found in the enumeration */
    while( CMHasNext( en, &rc) ) 
    {
        /* get the object path */
        data = CMGetNext(en, &rc);
        
        /* and return the target class object path as result of the
         * associatorNames() call
        */
        CMReturnObjectPath( rslt, data.value.ref ); 
    }
    PROV_LOG ("\n\n********************* %s CMPI AssociatorNames() exited",
        _ClassName);
    PROV_LOG_CLOSE ();
    return rc;
}

CMPIStatus TestCMPIAssociationProviderReferences(
    CMPIAssociationMI * mi,
    const CMPIContext * ctx,
    const CMPIResult * rslt,
    const CMPIObjectPath * ref,
    const char *resultClass,
    const char *role ,
    const char** properties)
{
    CMPIInstance * ci = NULL;
    CMPIObjectPath * op = NULL;
    CMPIObjectPath * rop = NULL;
    CMPIObjectPath * cop = NULL;
    CMPIEnumeration * en = NULL;
    CMPIData data ;

    const char * targetName = NULL;
    char * _thisClassName;
    char * _RefLeftClass = NULL;    
    char * _RefRightClass = NULL; 
    char * _RefLeft = NULL;       
    char * _RefRight = NULL;      
    
    CMPIStatus rc = {CMPI_RC_OK, NULL};
    CMPIString *sourceClass = NULL;

    _thisClassName=_ClassName;

    PROV_LOG_OPEN (_thisClassName, _ProviderLocation); 
    PROV_LOG ("\n\n********************* %s CMPI References() called",
        _thisClassName);

    /* get object path of the target class */
    op = get_assoc_targetClass_ObjectPath(
        _broker,
        ref,
        _RefLeftClass,
        _RefRightClass,
        &rc);

    PROV_LOG (" New Object Path [%s]",
        CMGetCharPtr (CMGetNameSpace (ref, &rc)));

    /* create new object path of association */
    rop = CMNewObjectPath(
        _broker,
        CMGetCharPtr(CMGetNameSpace(ref,&rc)),
        _thisClassName,
        &rc );

    /* upcall to CIMOM; call enumInstanceNames() of the target class */
    en = CBEnumInstanceNames( _broker, ctx, op, &rc);

    /* as long as object path entries are found in the enumeration */
    while( CMHasNext( en, &rc) ) 
    {
        /* get the object path */
        data = CMGetNext( en, &rc);
        {
            /* create new instance of the association */
            ci = CMNewInstance( _broker, rop, &rc);

            /* get name of the target class */
            targetName = get_assoc_targetClass_Name(
                _broker,
                ref,
                _RefLeftClass,
                _RefRightClass,
                &rc);

            /* set the properties of the association instance depending on the
             * constellation of the source class (parameter ref) and the target
             * class (see targetName)
            */
            if(strcmp (targetName, "CMPI_TEST_Person")== 0)
            {
                CMSetProperty(
                    ci,
                    "model",
                    (CMPIValue*)&(data.value.ref),
                    CMPI_ref );
                CMSetProperty( ci, "driver", (CMPIValue*)&(ref), CMPI_ref );
            }
            else if( strcmp( targetName,"CMPI_TEST_Vehicle") == 0 ) 
            {
                CMSetProperty(
                    ci,
                    "model",
                    (CMPIValue*)&(data.value.ref),
                    CMPI_ref );
                CMSetProperty( ci, "driver", (CMPIValue*)&(ref), CMPI_ref );
            }
            
            CMReturnInstance( rslt, ci );
        }
    }
    PROV_LOG ("\n\n********************* %s CMPI References() exited",
        _thisClassName);
    PROV_LOG_CLOSE ();

    return rc;
}

CMPIStatus TestCMPIAssociationProviderReferenceNames(
    CMPIAssociationMI * mi,
    const CMPIContext * ctx,
    const CMPIResult * rslt,
    const CMPIObjectPath * ref,
    const char* resultClass,
    const char* role) 
{
    CMPIInstance * ci = NULL;
    CMPIObjectPath * op = NULL;
    CMPIObjectPath * rop = NULL;
    CMPIObjectPath * cop = NULL;
    CMPIEnumeration * en = NULL;
    CMPIData data ;

    const char * targetName = NULL;
    const char * _thisClassName;
    const char * _RefLeftClass = NULL;    
    const char * _RefRightClass = NULL;   
    const char * _RefLeft = NULL;   
    const char * _RefRight = NULL;   

    CMPIStatus rc = {CMPI_RC_OK, NULL};
    _thisClassName=_ClassName;

    PROV_LOG_OPEN (_thisClassName, _ProviderLocation);
    PROV_LOG ("\n\n********************* %s CMPI ReferenceNames() called",
        _thisClassName);

    /* get object path of the target class */
    op = get_assoc_targetClass_ObjectPath(
        _broker,
        ref,
        _RefLeftClass,
        _RefRightClass,
        &rc);

    PROV_LOG (" New Object Path [%s]",
    CMGetCharPtr (CMGetNameSpace (ref, &rc)));                

    /* create new object path of association */
    rop = CMNewObjectPath(
        _broker,
        CMGetCharPtr(CMGetNameSpace(ref,&rc)),
        _thisClassName,
        &rc );

    /* upcall to CIMOM; call enumInstanceNames() of the target class */
    en = CBEnumInstanceNames( _broker, ctx, op, &rc);

    /* as long as object path entries are found in the enumeration */
    while( CMHasNext( en, &rc) ) 
    {
        /* get the object path */
        data = CMGetNext( en, &rc);

        /* create new instance of the association */
        ci = CMNewInstance( _broker, rop, &rc);

        /* get name of the target class */
        targetName = get_assoc_targetClass_Name(
            _broker,
            ref,
            _RefLeftClass,
            _RefRightClass,
            &rc);

        /* set the properties of the association instance depending on the
         * constellation of the source class (parameter ref) and the target
         * class (see targetName)
        */

        if(strcmp (targetName, "CMPI_TEST_Person")== 0)
        {
            CMSetProperty(
                ci,
                "model",
                (CMPIValue*)&(data.value.ref),
                CMPI_ref );
            CMSetProperty( ci, "driver", (CMPIValue*)&(ref), CMPI_ref );
        }
        else if( strcmp( targetName,"CMPI_TEST_Vehicle") == 0 ) 
        {
            CMSetProperty(
                ci,
                "model",
                (CMPIValue*)&(data.value.ref),
                CMPI_ref );
            CMSetProperty( ci, "driver", (CMPIValue*)&(ref), CMPI_ref );
        }
        
        /* get object path of association instance */
        cop = CMGetObjectPath(ci,&rc);

        /* set namespace in object path of association */
        CMSetNameSpace(cop,CMGetCharPtr(CMGetNameSpace(ref,&rc)));

        /* and return the association object path as result of the
         * referenceNames() call
        */
        CMReturnObjectPath( rslt, cop );
    }

    PROV_LOG ("\n\n********************* %s CMPI ReferenceNames() exited",
        _thisClassName);
    PROV_LOG_CLOSE ();

    return rc;
}


//CMPIPROVIDER
/* ---------------------------------------------------------------------------*/
/*                              Provider Factory                              */
/* ---------------------------------------------------------------------------*/

CMAssociationMIStub(
    TestCMPIAssociationProvider,
    TestCMPIAssociationProvider,
    _broker,
    CMNoHook)

/* ---------------------------------------------------------------------------*/
/*             end of TestCMPIProvider                      */
/* ---------------------------------------------------------------------------*/

