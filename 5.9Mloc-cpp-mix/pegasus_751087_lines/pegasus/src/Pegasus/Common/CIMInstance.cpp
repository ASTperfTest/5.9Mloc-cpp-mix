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

#include "CIMInstanceRep.h"
#include "CIMInstance.h"
#include "DeclContext.h"
#include "Indentor.h"
#include "CIMName.h"
#include "XmlWriter.h"

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

#define PEGASUS_ARRAY_T CIMInstance
# include "ArrayImpl.h"
#undef PEGASUS_ARRAY_T

////////////////////////////////////////////////////////////////////////////////
//
// CIMInstance
//
////////////////////////////////////////////////////////////////////////////////

CIMInstance::CIMInstance()
    : _rep(0)
{
}

CIMInstance::CIMInstance(const CIMInstance& x)
{
    Inc(_rep = x._rep);
}

CIMInstance::CIMInstance(const CIMObject& x)
{
    if (!(_rep = dynamic_cast<CIMInstanceRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMInstance::CIMInstance(const CIMName& className)
{
    _rep = new CIMInstanceRep(
        CIMObjectPath(String::EMPTY, CIMNamespaceName(), className));
}

CIMInstance::CIMInstance(CIMInstanceRep* rep)
    : _rep(rep)
{
}

CIMInstance& CIMInstance::operator=(const CIMInstance& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMInstance::~CIMInstance()
{
    Dec(_rep);
}

const CIMName& CIMInstance::getClassName() const
{
    CheckRep(_rep);
    return _rep->getClassName();
}

const CIMObjectPath& CIMInstance::getPath() const
{
    CheckRep(_rep);
    return _rep->getPath();
}

void CIMInstance::setPath (const CIMObjectPath & path)
{
    CheckRep(_rep);
    _rep->setPath (path);
}

CIMInstance& CIMInstance::addQualifier(const CIMQualifier& qualifier)
{
    CheckRep(_rep);
    _rep->addQualifier(qualifier);
    return *this;
}

Uint32 CIMInstance::findQualifier(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findQualifier(name);
}

CIMQualifier CIMInstance::getQualifier(Uint32 index)
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

CIMConstQualifier CIMInstance::getQualifier(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

void CIMInstance::removeQualifier(Uint32 index)
{
    CheckRep(_rep);
    _rep->removeQualifier(index);
}

Uint32 CIMInstance::getQualifierCount() const
{
    CheckRep(_rep);
    return _rep->getQualifierCount();
}

CIMInstance& CIMInstance::addProperty(const CIMProperty& x)
{
    CheckRep(_rep);
    _rep->addProperty(x);
    return *this;
}

Uint32 CIMInstance::findProperty(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findProperty(name);
}

CIMProperty CIMInstance::getProperty(Uint32 index)
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

CIMConstProperty CIMInstance::getProperty(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

void CIMInstance::removeProperty(Uint32 index)
{
    CheckRep(_rep);
    _rep->removeProperty(index);
}

Uint32 CIMInstance::getPropertyCount() const
{
    CheckRep(_rep);
    return _rep->getPropertyCount();
}

Boolean CIMInstance::isUninitialized() const
{
    return _rep == 0;
}

Boolean CIMInstance::identical(const CIMConstInstance& x) const
{
    CheckRep(x._rep);
    CheckRep(_rep);
    return _rep->identical(x._rep);
}

CIMInstance CIMInstance::clone() const
{
    return CIMInstance((CIMInstanceRep*)(_rep->clone()));
}

CIMObjectPath CIMInstance::buildPath(const CIMConstClass& cimClass) const
{
    CheckRep(_rep);
    return _rep->buildPath(cimClass);
}

void CIMInstance::_checkRep() const
{
    if (!_rep)
        throw UninitializedObjectException();
}


void CIMInstance::filter(Boolean includeQualifiers, Boolean includeClassOrigin,
                        const CIMPropertyList& propertyList)
{
    CheckRep(_rep);
    _rep->filter(includeQualifiers, includeClassOrigin, propertyList);
}

////////////////////////////////////////////////////////////////////////////////
//
// CIMConstInstance
//
////////////////////////////////////////////////////////////////////////////////

CIMConstInstance::CIMConstInstance()
    : _rep(0)
{
}

CIMConstInstance::CIMConstInstance(const CIMConstInstance& x)
{
    Inc(_rep = x._rep);
}

CIMConstInstance::CIMConstInstance(const CIMInstance& x)
{
    Inc(_rep = x._rep);
}

CIMConstInstance::CIMConstInstance(const CIMObject& x)
{
    if (!(_rep = dynamic_cast<CIMInstanceRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMConstInstance::CIMConstInstance(const CIMConstObject& x)
{
    if (!(_rep = dynamic_cast<CIMInstanceRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMConstInstance::CIMConstInstance(const CIMName& className)
{
    _rep = new CIMInstanceRep(
        CIMObjectPath(String::EMPTY, CIMNamespaceName(), className));
}

CIMConstInstance& CIMConstInstance::operator=(const CIMConstInstance& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMConstInstance& CIMConstInstance::operator=(const CIMInstance& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMConstInstance::~CIMConstInstance()
{
    Dec(_rep);
}

const CIMName& CIMConstInstance::getClassName() const
{
    CheckRep(_rep);
    return _rep->getClassName();
}

const CIMObjectPath& CIMConstInstance::getPath() const
{
    CheckRep(_rep);
    return _rep->getPath();
}

Uint32 CIMConstInstance::findQualifier(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findQualifier(name);
}

CIMConstQualifier CIMConstInstance::getQualifier(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

Uint32 CIMConstInstance::getQualifierCount() const
{
    CheckRep(_rep);
    return _rep->getQualifierCount();
}

Uint32 CIMConstInstance::findProperty(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findProperty(name);
}

CIMConstProperty CIMConstInstance::getProperty(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

Uint32 CIMConstInstance::getPropertyCount() const
{
    CheckRep(_rep);
    return _rep->getPropertyCount();
}

Boolean CIMConstInstance::isUninitialized() const
{
    return _rep == 0;
}

Boolean CIMConstInstance::identical(const CIMConstInstance& x) const
{
    CheckRep(x._rep);
    CheckRep(_rep);
    return _rep->identical(x._rep);
}

CIMInstance CIMConstInstance::clone() const
{
    return CIMInstance((CIMInstanceRep*)(_rep->clone()));
}

CIMObjectPath CIMConstInstance::buildPath(const CIMConstClass& cimClass) const
{
    CheckRep(_rep);
    return _rep->buildPath(cimClass);
}

void CIMConstInstance::_checkRep() const
{
    if (!_rep)
        throw UninitializedObjectException();
}

PEGASUS_NAMESPACE_END
