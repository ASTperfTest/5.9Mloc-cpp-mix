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
// Author: Chip Vincent (cvincent@us.ibm.com)
//
// Modified By:    Barbara Packard (barbara_packard@hp.com)
//                Paulo Sehn        (paulo_sehn@hp.com)
//                Adriano Zanuz    (adriano.zanuz@hp.com)
//%////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_WMIValue_h
#define Pegasus_WMIValue_h

#include <Pegasus/Common/CIMValue.h>

PEGASUS_NAMESPACE_BEGIN

class PEGASUS_WMIPROVIDER_LINKAGE WMIValue : public CIMValue
{
public:
   WMIValue(const CIMValue & value);
   WMIValue(const VARIANT & value, const CIMTYPE type);
   WMIValue(const VARTYPE vt, void *pVal);
   WMIValue(const VARTYPE vt, void *pVal, const CIMTYPE Type);    // for arrays
   WMIValue(const CComVariant & vValue);

   VARIANT toVariant();
   operator VARIANT(void) const;

   // returns a variant type from the WMIVAlue
   void getAsVariant (CComVariant *var);

protected:
   WMIValue(void) { };

private:
    CIMValue getCIMValueFromVariant(
        VARTYPE vt, void *pVal, const CIMTYPE Type = CIM_ILLEGAL);
    CIMValue getArrayValue(
        const CComVariant & vValue, const CIMTYPE Type = CIM_ILLEGAL);
    CIMValue getValue(
        const CComVariant & vValue, const CIMTYPE Type = CIM_ILLEGAL);

    bool isArrayType(VARTYPE vt)
        {return (vt & VT_ARRAY) ? true : false;}

    bool isReferenceType(VARTYPE vt)
        {return (vt & VT_BYREF) ? true : false;}


};

PEGASUS_NAMESPACE_END

#endif
