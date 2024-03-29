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

#include "CIMClassRep.h"
#include "CIMClass.h"

PEGASUS_NAMESPACE_BEGIN

#define PEGASUS_ARRAY_T CIMClass
# include "ArrayImpl.h"
#undef PEGASUS_ARRAY_T

////////////////////////////////////////////////////////////////////////////////
//
// CIMClass
//
////////////////////////////////////////////////////////////////////////////////

CIMClass::CIMClass()
    : _rep(0)
{
}

CIMClass::CIMClass(const CIMClass& x)
{
    Inc(_rep = x._rep);
}

CIMClass::CIMClass(const CIMObject& x)
{
    if (!(_rep = dynamic_cast<CIMClassRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMClass::CIMClass(
    const CIMName& className,
    const CIMName& superClassName)
{
    _rep = new CIMClassRep(className, superClassName);
}

CIMClass::CIMClass(CIMClassRep* rep)
    : _rep(rep)
{
}

CIMClass& CIMClass::operator=(const CIMClass& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMClass::~CIMClass()
{
    Dec(_rep);
}

Boolean CIMClass::isAssociation() const
{
    CheckRep(_rep);
    return _rep->isAssociation();
}

Boolean CIMClass::isAbstract() const
{
    CheckRep(_rep);
    return _rep->isAbstract();
}

const CIMName& CIMClass::getClassName() const
{
    CheckRep(_rep);
    return _rep->getClassName();
}

const CIMObjectPath& CIMClass::getPath() const
{
    CheckRep(_rep);
    return _rep->getPath();
}

void CIMClass::setPath (const CIMObjectPath & path)
{
    CheckRep(_rep);
    _rep->setPath (path);
}

const CIMName& CIMClass::getSuperClassName() const
{
    CheckRep(_rep);
    return _rep->getSuperClassName();
}

void CIMClass::setSuperClassName(const CIMName& superClassName)
{
    CheckRep(_rep);
    _rep->setSuperClassName(superClassName);
}

CIMClass& CIMClass::addQualifier(const CIMQualifier& qualifier)
{
    CheckRep(_rep);
    _rep->addQualifier(qualifier);
    return *this;
}

Uint32 CIMClass::findQualifier(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findQualifier(name);
}

CIMQualifier CIMClass::getQualifier(Uint32 index)
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

CIMConstQualifier CIMClass::getQualifier(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

void CIMClass::removeQualifier(Uint32 index)
{
    CheckRep(_rep);
    _rep->removeQualifier(index);
}

Uint32 CIMClass::getQualifierCount() const
{
    CheckRep(_rep);
    return _rep->getQualifierCount();
}

CIMClass& CIMClass::addProperty(const CIMProperty& x)
{
    CheckRep(_rep);
    _rep->addProperty(x);
    return *this;
}

Uint32 CIMClass::findProperty(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findProperty(name);
}

CIMProperty CIMClass::getProperty(Uint32 index)
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

CIMConstProperty CIMClass::getProperty(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

void CIMClass::removeProperty(Uint32 index)
{
    CheckRep(_rep);
    _rep->removeProperty(index);
}

Uint32 CIMClass::getPropertyCount() const
{
    CheckRep(_rep);
    return _rep->getPropertyCount();
}

CIMClass& CIMClass::addMethod(const CIMMethod& x)
{
    CheckRep(_rep);
    _rep->addMethod(x);
    return *this;
}

Uint32 CIMClass::findMethod(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findMethod(name);
}

CIMMethod CIMClass::getMethod(Uint32 index)
{
    CheckRep(_rep);
    return _rep->getMethod(index);
}

CIMConstMethod CIMClass::getMethod(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getMethod(index);
}

void CIMClass::removeMethod(Uint32 index)
{
    CheckRep(_rep);
    _rep->removeMethod(index);
}

Uint32 CIMClass::getMethodCount() const
{
    CheckRep(_rep);
    return _rep->getMethodCount();
}

Boolean CIMClass::isUninitialized() const
{
    return (_rep == 0)? true : false;
}

Boolean CIMClass::identical(const CIMConstClass& x) const
{
    CheckRep(x._rep);
    CheckRep(_rep);
    return _rep->identical(x._rep);
}

CIMClass CIMClass::clone() const
{
    return CIMClass((CIMClassRep*)(_rep->clone()));
}

void CIMClass::getKeyNames(Array<CIMName>& keyNames) const
{
    CheckRep(_rep);
    _rep->getKeyNames(keyNames);
}

Boolean CIMClass::hasKeys() const
{
    CheckRep(_rep);
    return _rep->hasKeys();
}

void CIMClass::_checkRep() const
{
    if (!_rep)
        throw UninitializedObjectException();
}

CIMInstance CIMClass::buildInstance(Boolean includeQualifiers,
    Boolean includeClassOrigin,
    const CIMPropertyList& propertyList) const
{
    CheckRep(_rep);
    return _rep->buildInstance(includeQualifiers,
        includeClassOrigin,
        propertyList);
}

////////////////////////////////////////////////////////////////////////////////
//
// CIMConstClass
//
////////////////////////////////////////////////////////////////////////////////

CIMConstClass::CIMConstClass()
    : _rep(0)
{
}

CIMConstClass::CIMConstClass(const CIMConstClass& x)
{
    Inc(_rep = x._rep);
}

CIMConstClass::CIMConstClass(const CIMClass& x)
{
    Inc(_rep = x._rep);
}

CIMConstClass::CIMConstClass(const CIMObject& x)
{
    if (!(_rep = dynamic_cast<CIMClassRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMConstClass::CIMConstClass(const CIMConstObject& x)
{
    if (!(_rep = dynamic_cast<CIMClassRep*>(x._rep)))
        throw DynamicCastFailedException();
    Inc(_rep);
}

CIMConstClass::CIMConstClass(
    const CIMName& className,
    const CIMName& superClassName)
{
    _rep = new CIMClassRep(className, superClassName);
}

CIMConstClass& CIMConstClass::operator=(const CIMConstClass& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMConstClass& CIMConstClass::operator=(const CIMClass& x)
{
    if (x._rep != _rep)
    {
        Dec(_rep);
        Inc(_rep = x._rep);
    }
    return *this;
}

CIMConstClass::~CIMConstClass()
{
    Dec(_rep);
}

Boolean CIMConstClass::isAssociation() const
{
    CheckRep(_rep);
    return _rep->isAssociation();
}

Boolean CIMConstClass::isAbstract() const
{
    CheckRep(_rep);
    return _rep->isAbstract();
}

const CIMName& CIMConstClass::getClassName() const
{
    CheckRep(_rep);
    return _rep->getClassName();
}

const CIMObjectPath& CIMConstClass::getPath() const
{
    CheckRep(_rep);
    return _rep->getPath();
}

const CIMName& CIMConstClass::getSuperClassName() const
{
    CheckRep(_rep);
    return _rep->getSuperClassName();
}

Uint32 CIMConstClass::findQualifier(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findQualifier(name);
}

CIMConstQualifier CIMConstClass::getQualifier(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getQualifier(index);
}

Uint32 CIMConstClass::getQualifierCount() const
{
    CheckRep(_rep);
    return _rep->getQualifierCount();
}

Uint32 CIMConstClass::findProperty(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findProperty(name);
}

CIMConstProperty CIMConstClass::getProperty(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getProperty(index);
}

Uint32 CIMConstClass::getPropertyCount() const
{
    CheckRep(_rep);
    return _rep->getPropertyCount();
}

Uint32 CIMConstClass::findMethod(const CIMName& name) const
{
    CheckRep(_rep);
    return _rep->findMethod(name);
}

CIMConstMethod CIMConstClass::getMethod(Uint32 index) const
{
    CheckRep(_rep);
    return _rep->getMethod(index);
}

Uint32 CIMConstClass::getMethodCount() const
{
    CheckRep(_rep);
    return _rep->getMethodCount();
}

Boolean CIMConstClass::isUninitialized() const
{
    return (_rep == 0)? true : false;
}

Boolean CIMConstClass::identical(const CIMConstClass& x) const
{
    CheckRep(x._rep);
    CheckRep(_rep);
    return _rep->identical(x._rep);
}

CIMClass CIMConstClass::clone() const
{
    return CIMClass((CIMClassRep*)(_rep->clone()));
}

void CIMConstClass::getKeyNames(Array<CIMName>& keyNames) const
{
    CheckRep(_rep);
    _rep->getKeyNames(keyNames);
}

Boolean CIMConstClass::hasKeys() const
{
    CheckRep(_rep);
    return _rep->hasKeys();
}

void CIMConstClass::_checkRep() const
{
    if (!_rep)
        throw UninitializedObjectException();
}

PEGASUS_NAMESPACE_END
