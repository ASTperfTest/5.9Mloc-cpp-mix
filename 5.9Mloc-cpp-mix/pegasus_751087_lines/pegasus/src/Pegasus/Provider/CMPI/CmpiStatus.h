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
//=============================================================================
//
//%////////////////////////////////////////////////////////////////////////////

#ifndef _CmpiStatus_h_
#define _CmpiStatus_h_

#include "cmpidt.h"
#include "cmpift.h"

#include "CmpiBaseMI.h"
#include "Linkage.h"

/** 
    This class represents the status of a provider function invocation.
*/

class PEGASUS_CMPI_PROVIDER_LINKAGE CmpiStatus
{
    friend class CmpiInstanceMI;
    friend class CmpiMethodMI;
    friend class CmpiBaseMI;
    friend class CmpiAssociationMI;
    friend class CmpiPropertyMI;
    friend class CmpiIndicationMI;
protected:

    /** 
        CmpiStatus actually is a CMPIStatus struct.
    */
    CMPIStatus st;

    /** 
        status - Returns CMPIStatus struct, to be used by MI drivers only.
    */
    CMPIStatus status() const;

private:

    /** 
        Constructor - not to be used.
    */
    CmpiStatus();

public:

    /** 
        Aux Constructor - set from CMPIStatus.
        @param stat the CMPIStatus
    */
    CmpiStatus(const CMPIStatus stat);

    /** 
        Constructor - set rc only.
        @param rc the return code.
    */
    CmpiStatus(const CMPIrc rc);

    /** 
        Constructor - set rc and message.
        @param rc The return code.
        @param msg Descriptive message.
    */
    CmpiStatus(const CMPIrc rcp, const char *msg);

    /** 
        rc - get the rc value.
    */
    CMPIrc rc() const;

    /** 
        msg - get the msg component.
    */
    const char*  msg() const;
};

#endif

