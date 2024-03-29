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

/*!
    \file serialization.h
    \brief Header file for the binary serializer component for Remote CMPI.

    This file defines a function table of methods to be used for serializing
    and deserializing CMPI data respectively. It covers most of the encapsulated
    data types, however, those which cannot be fully serialized or
    deserialized due to API restrictions are not included, e.g. CMPIResult,
    CMPIContext or CMPIEnumeration.
*/

#ifndef _SOCKETCOMM_H
#define _SOCKETCOMM_H

#include "serialization.h"
#include "debug.h"

/****************************************************************************/
//! MI service request identifier.
/*!
    This struct holds the function name and a function pointer for
    remote MI calls. Once the function name has been deserialized, the
    appropriate method may then resolve the handler function to be called.
*/
struct  socket_mi_function
{
    char * name;        /*!< function name */
    void (* function) ( 
        int,
        CONST CMPIBroker *,
        CONST CMPIContext *,
        CONST CMPIObjectPath * ); /*!< function pointer */
};


//! Broker service request identifier.
/*!
    This struct holds the function name and a function pointer for
    up-calls. Once the function name has been deserialized, the
    appropriate method may then resolve the handler function to be called.

*/
struct  socket_mb_function
{
    char * name;        /*!< function name  */
    void (* function) ( 
        int, 
        CONST CMPIBroker *, 
        CONST CMPIContext * ); /*!< function pointer  */
};



/****************************************************************************/

extern char * RCMPI_CTX_ID;

/****************************************************************************/

void socketcomm_copy_args ( CMPIArgs * src, CMPIArgs * dst );

void socketcomm_array2result ( CMPIArray * array, CONST CMPIResult * result );

void socketcomm_serialize_props ( 
    int socket,
    const struct BinarySerializerFT * sft,
    char ** props );
char ** socketcomm_deserialize_props ( 
    int socket,
    const struct BinarySerializerFT * sft,
    CONST CMPIBroker * broker );

void socketcomm_serialize_context ( 
    int socket,
    const struct BinarySerializerFT * sft,
    CONST CMPIContext * ctx );
void socketcomm_deserialize_context ( 
    int socket,
    const struct BinarySerializerFT * sft,
    CONST CMPIBroker * broker,
    CONST CMPIContext * ctx );

#endif

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
