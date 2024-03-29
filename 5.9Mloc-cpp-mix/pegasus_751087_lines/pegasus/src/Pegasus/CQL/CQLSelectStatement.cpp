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

#include "CQLSelectStatement.h"
#include "CQLSelectStatementRep.h"
#include <Pegasus/Common/InternalException.h>
#include <Pegasus/CQL/CQLPredicate.h>

PEGASUS_NAMESPACE_BEGIN

CQLSelectStatement::CQLSelectStatement()
    :SelectStatement()
{
    _rep = new CQLSelectStatementRep();

    // Set the _rep into the base class also
    SelectStatement::_rep = _rep;
}

CQLSelectStatement::CQLSelectStatement(String& inQlang, String& inQuery,
    QueryContext& inCtx)
    :SelectStatement()
{
    _rep = new CQLSelectStatementRep(inQlang,inQuery,inCtx);
    
    // Set the _rep into the base class also
    SelectStatement::_rep = _rep;
}

CQLSelectStatement::CQLSelectStatement(String& inQlang, String& inQuery)
    :SelectStatement()
{
    _rep = new CQLSelectStatementRep(inQlang,inQuery);
    
    // Set the _rep into the base class also
    SelectStatement::_rep = _rep;
}

CQLSelectStatement::CQLSelectStatement(const CQLSelectStatement& statement)
    :SelectStatement()
{
    _rep = new CQLSelectStatementRep(*statement._rep);
    
    // Set the _rep into the base class also
    SelectStatement::_rep = _rep;
}

CQLSelectStatement::~CQLSelectStatement()
{
    if(_rep)
        delete _rep;

    // Note - no need to delete the rep in the base class
}

CQLSelectStatement& CQLSelectStatement::operator=(const CQLSelectStatement& rhs)
{
    if(&rhs != this)
    {
        if(_rep)
            delete _rep;
        _rep = new CQLSelectStatementRep(*rhs._rep);
    
        // Set the _rep into the base class also
        SelectStatement::_rep = _rep;
    }
    return *this;
}

Boolean CQLSelectStatement::evaluate(const CIMInstance& inCI)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->evaluate(inCI);
}

void CQLSelectStatement::applyProjection(CIMInstance& inCI,
    Boolean allowMissing)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->applyProjection(inCI, allowMissing);
}

void CQLSelectStatement::validate()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->validate();
}

Array<CIMObjectPath> CQLSelectStatement::getClassPathList()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getClassPathList();
}

CIMPropertyList CQLSelectStatement::getPropertyList(
    const CIMObjectPath& inClassName)
{
    // Should be set by the concrete sub-classes
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getPropertyList(inClassName);
}

CIMPropertyList CQLSelectStatement::getSelectPropertyList(
    const CIMObjectPath& inClassName)
{
    // Should be set by the concrete sub-classes
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getSelectPropertyList(inClassName);
}

CIMPropertyList CQLSelectStatement::getWherePropertyList(
    const CIMObjectPath& inClassName)
{
    // Should be set by the concrete sub-classes
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getWherePropertyList(inClassName);
}

Array<CQLChainedIdentifier> CQLSelectStatement::getSelectChainedIdentifiers()
{
    // Should be set by the concrete sub-classes
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getSelectChainedIdentifiers();
}

Array<CQLChainedIdentifier> CQLSelectStatement::getWhereChainedIdentifiers()
{
    // Should be set by the concrete sub-classes
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getWhereChainedIdentifiers();
}

void CQLSelectStatement::appendClassPath(const CQLIdentifier& inIdentifier)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->appendClassPath(inIdentifier);
}

void CQLSelectStatement::setPredicate(const CQLPredicate& inPredicate)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->setPredicate(inPredicate);
}

CQLPredicate CQLSelectStatement::getPredicate() const
{
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->getPredicate();
}

void CQLSelectStatement::insertClassPathAlias(
    const CQLIdentifier& inIdentifier, String inAlias)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->insertClassPathAlias(inIdentifier,inAlias);
}

void CQLSelectStatement::appendSelectIdentifier(const CQLChainedIdentifier& x)
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->appendSelectIdentifier(x);
}

void CQLSelectStatement::applyContext()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->applyContext();
}

void CQLSelectStatement::normalizeToDOC()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->normalizeToDOC();
}

void CQLSelectStatement::setHasWhereClause()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->setHasWhereClause();
}

Boolean CQLSelectStatement::hasWhereClause()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->hasWhereClause();
}

String CQLSelectStatement::toString()
{
    PEGASUS_ASSERT(_rep != NULL);
    
    return _rep->toString();
}

void CQLSelectStatement::clear(){
    PEGASUS_ASSERT(_rep != NULL);
    
    _rep->clear();
}

PEGASUS_NAMESPACE_END
