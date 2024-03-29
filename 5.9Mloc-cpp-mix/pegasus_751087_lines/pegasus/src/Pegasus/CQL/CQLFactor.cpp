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

#include "CQLFactor.h"
#include <Pegasus/CQL/CQLExpression.h>
#include <Pegasus/CQL/CQLFunction.h>
#include <Pegasus/CQL/CQLFactory.h>
#include <Pegasus/CQL/CQLFactorRep.h>
#include <Pegasus/Query/QueryCommon/QueryContext.h>

PEGASUS_NAMESPACE_BEGIN

/*
#define PEGASUS_ARRAY_T CQLFactor
#include <Pegasus/Common/ArrayImpl.h>
#undef PEGASUS_ARRAY_T
*/

CQLFactor::CQLFactor()
{
    _rep = new CQLFactorRep();
}

CQLFactor::CQLFactor(const CQLFactor& inCQLFact)
{
    _rep = new CQLFactorRep(inCQLFact._rep);
}

CQLFactor::CQLFactor(const CQLValue& inCQLVal)
{
    _rep = new CQLFactorRep(inCQLVal);
}

CQLFactor::CQLFactor(const CQLExpression& inCQLExp)
{
    _rep = new CQLFactorRep(inCQLExp);
}

CQLFactor::CQLFactor(const CQLFunction& inCQLFunc)
{
    _rep = new CQLFactorRep(inCQLFunc);
}

CQLFactor::~CQLFactor()
{
    if(_rep)
    {
        delete _rep;
    }
}

CQLValue CQLFactor::getValue()const
{
    return _rep->getValue();
}

CQLValue CQLFactor::resolveValue(const CIMInstance& CI,
                                 const QueryContext& QueryCtx)
{
    return _rep->resolveValue(CI, QueryCtx);
}
Boolean CQLFactor::isSimple()const
{
    return _rep->isSimple();
}
Boolean CQLFactor::isSimpleValue()const
{
    return _rep->isSimpleValue();
}

CQLFunction CQLFactor::getCQLFunction()const
{
    return _rep->getCQLFunction();
}

CQLExpression CQLFactor::getCQLExpression()const
{
    return _rep->getCQLExpression();
}

String CQLFactor::toString()const
{
    return _rep->toString();
}

void CQLFactor::applyContext(const QueryContext& inContext,
                 const CQLChainedIdentifier& inCid)
{
    _rep->applyContext(inContext,inCid);   
}

CQLFactor& CQLFactor::operator=(const CQLFactor& rhs)
{
    if(&rhs != this)
    {
        if(_rep)
    {
        delete _rep;
    }
        _rep = new CQLFactorRep(rhs._rep);
    }
    
    return *this;
}
/*
Boolean CQLFactor::operator==(const CQLFactor& factor)const
{
    return (*_rep == *(factor._rep));
}
Boolean CQLFactor::operator!=(const CQLFactor& factor)const
{
    return (!operator==(factor));

}
*/
PEGASUS_NAMESPACE_END
