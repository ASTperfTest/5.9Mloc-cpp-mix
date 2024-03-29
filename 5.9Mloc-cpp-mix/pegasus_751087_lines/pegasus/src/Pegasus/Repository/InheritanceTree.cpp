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

#include <Pegasus/Common/String.h>
#include <Pegasus/Common/HashTable.h>
#include <Pegasus/Common/Dir.h>
#include <Pegasus/Common/XmlWriter.h>
#include <Pegasus/Common/CommonUTF.h>
#include <Pegasus/Common/CIMNameUnchecked.h>
#include "InheritanceTree.h"

#if 0
#undef PEG_METHOD_ENTER
#undef PEG_METHOD_EXIT
#define PEG_METHOD_ENTER(x,y)  cout<<"--- Enter: "<<y<<endl;
#define PEG_METHOD_EXIT()
#endif

PEGASUS_NAMESPACE_BEGIN

PEGASUS_USING_STD;

////////////////////////////////////////////////////////////////////////////////
//
// NoCaseEqualFunc
//
////////////////////////////////////////////////////////////////////////////////

struct NoCaseEqualFunc
{
    static Boolean equal(const String& x, const String& y)
    {
        return String::equalNoCase(x, y);
    }
};

////////////////////////////////////////////////////////////////////////////////
//
// InheritanceTreeRep
//
////////////////////////////////////////////////////////////////////////////////

struct InheritanceTreeNode;

struct InheritanceTreeRep
{
    typedef HashTable<
        String, InheritanceTreeNode*, NoCaseEqualFunc, HashLowerCaseFunc> Table;
    Table table;

    // Tradeoff: chosing a larger value decreases hash lookup time but
    // increases iteration (which seems to be the dominant operations).
    // This power of two (256) seems to produce the best results.

    InheritanceTreeRep() : table(256)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////
//
// InheritanceTreeNode
//
////////////////////////////////////////////////////////////////////////////////

class NameSpace;

struct InheritanceTreeExt
{
    InheritanceTreeExt(NameSpace* t, InheritanceTreeNode* itn)
       : tag(t), node(itn) {}
    NameSpace* tag;
    InheritanceTreeNode* node;
};


struct InheritanceTreeNode
{
    InheritanceTreeNode(const CIMName& className);

    void addSubClass(InheritanceTreeNode* subClass);

    Boolean removeSubClass(InheritanceTreeNode* subClass);

    void getSubClassNames(
        Array<CIMName>& subClassNames,
        Boolean deepInheritance,
        NameSpace* tag = 0);

    void getSuperClassNames(Array<CIMName>& superClassNames);

    void print(PEGASUS_STD(ostream)& os) const;
#if 0
    Boolean isSubClass(const CIMName& className) const;
#endif
    CIMName className;
    InheritanceTreeNode* superClass;
    InheritanceTreeNode* sibling;
    union {
    InheritanceTreeNode* subClasses;
       Array<InheritanceTreeExt*>* extNodes;
    };
    Boolean provisional;
    Boolean extension;
};

InheritanceTreeNode::InheritanceTreeNode(const CIMName& className)
    : className(className), superClass(0),
    sibling(0), subClasses(0), provisional(true), extension(false)
{
}

void InheritanceTreeNode::addSubClass(InheritanceTreeNode* subClass)
{
    subClass->superClass = this;
    subClass->sibling = subClasses;
    subClasses = subClass;
}

Boolean InheritanceTreeNode::removeSubClass(InheritanceTreeNode* subClass)
{
    InheritanceTreeNode* prev = 0;

    for (InheritanceTreeNode* p = subClasses; p; p = p->sibling)
    {
        if (p == subClass)
        {
            if (prev)
                prev->sibling = subClass->sibling;
            else
                subClasses = subClass->sibling;

            return true;
        }
        prev = p;
    }

    return false;
}

void InheritanceTreeNode::getSubClassNames(
    Array<CIMName>& subClassNames,
    Boolean deepInheritance,
    NameSpace* ns)
{
    // For each subClass:

    for (InheritanceTreeNode* p = subClasses; p; p = p->sibling)
    {
        if (p->extension)
        {
            for (int j = 0, m = p->extNodes->size(); j < m; j++)
            {
                InheritanceTreeExt* itx = (*(p->extNodes))[j];
                subClassNames.append(p->className);
                if (!ns)
                {
                    InheritanceTreeNode* itn=itx->node;
                    itn->getSubClassNames(subClassNames, deepInheritance, ns);
                }
                else if (itx->tag == ns)
                {
                    InheritanceTreeNode* itn=itx->node;
                    itn->getSubClassNames(subClassNames, deepInheritance, ns);
                    break;
                }
            }
        }
        else
        {
            subClassNames.append(p->className);
            if (deepInheritance)
            {
                p->getSubClassNames(subClassNames, true, ns);
            }
        }
    }
}

#if 0
Boolean InheritanceTreeNode::isSubClass(const CIMName& className_) const
{
    if (className.equal (className_))
        return true;

    for (InheritanceTreeNode* p = subClasses; p; p = p->sibling)
    {
        if (p->className.equal (className_))
            return true;
    }

    return false;
}
#endif

void InheritanceTreeNode::getSuperClassNames(Array<CIMName>& superClassNames)
{
    // For each superClass:

    for (InheritanceTreeNode* p = superClass; p; p = p->superClass)
    {
        superClassNames.append(p->className);
        // p->getSuperClassNames(superClassNames);
    }
}

void InheritanceTreeNode::print(PEGASUS_STD(ostream)& os) const
{
    os << className << " : " ;
    os << (superClass ? superClass->className : CIMName ());

    os << " { ";

    for (InheritanceTreeNode* p = subClasses; p; p = p->sibling)
        os << p->className << ' ';

    os << "}" << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
// InheritanceTree
//
////////////////////////////////////////////////////////////////////////////////

InheritanceTree::InheritanceTree()
{
    _rep = new InheritanceTreeRep;
}

InheritanceTree::~InheritanceTree()
{
    for (InheritanceTreeRep::Table::Iterator i = _rep->table.start(); i; i++)
        delete i.value();

    delete _rep;
}
void InheritanceTree::insert(
    const String& className,
    const String& superClassName,
    InheritanceTree& parentTree,
    NameSpace* tag)
{
    InheritanceTreeNode* superClassNode = 0;

    if (superClassName.size() &&
        !parentTree._rep->table.lookup(superClassName, superClassNode))
    {
        superClassNode =
            new InheritanceTreeNode(CIMNameUnchecked(superClassName));
        parentTree._rep->table.insert(superClassName, superClassNode);
    }

    InheritanceTreeNode* extNode = 0;

    if (!parentTree._rep->table.lookup(className, extNode))
    {
        extNode = new InheritanceTreeNode(CIMNameUnchecked(className));
        parentTree._rep->table.insert(className, extNode);
        extNode->extension=true;
        extNode->extNodes=new Array<InheritanceTreeExt*>;
    }

    extNode->provisional = false;

    if (superClassNode)
        superClassNode->addSubClass(extNode);

    InheritanceTreeNode* classNode = 0;

    if (!_rep->table.lookup(className, classNode))
    {
        classNode = new InheritanceTreeNode(className);
        _rep->table.insert(className, classNode);
    }

    extNode->extNodes->append(new InheritanceTreeExt(tag,classNode));

    classNode->superClass = superClassNode;
    /* temp comment out this code from bug 3352.  See bug 3498 for reason
    if (extNode)
    {
       for (int i=0, m=extNode->extNodes->size(); i < m; i++)
          if ((*extNode->extNodes)[i])
            delete (*(extNode->extNodes))[i];
       delete extNode;
    }
    extNode = NULL;*/

}

void InheritanceTree::insert(
    const String& className,
    const String& superClassName)
{
    // ATTN: need found flag!

    // -- Insert superclass:

    InheritanceTreeNode* superClassNode = 0;

    if (superClassName.size() &&
        !_rep->table.lookup(superClassName, superClassNode))
    {
        superClassNode =
            new InheritanceTreeNode(CIMNameUnchecked(superClassName));
        _rep->table.insert(superClassName, superClassNode);
    }

    // -- Insert class:

    InheritanceTreeNode* classNode = 0;

    if (!_rep->table.lookup(className, classNode))
    {
        classNode = new InheritanceTreeNode(CIMNameUnchecked(className));
        _rep->table.insert(className, classNode);
    }

    classNode->provisional = false;

    // -- Link the class and superclass nodes:

    if (superClassNode)
        superClassNode->addSubClass(classNode);
}

void InheritanceTree::insertFromPath(const String& path,
      InheritanceTree* parentTree,
      NameSpace* ns)
{
    for (Dir dir(path); dir.more(); dir.next())
    {
        String fileName = dir.getName();

        // Ignore the current and parent directories.

        if (fileName == "." || fileName == "..")
            continue;

        Uint32 dot = fileName.find('.');

        // Ignore files without dots in them:

        if (dot == PEG_NOT_FOUND)
            continue;

        String className = fileName.subString(0, dot);
        String superClassName = fileName.subString(dot + 1);

        if (superClassName == "#")
            superClassName.clear();


#ifdef PEGASUS_REPOSITORY_ESCAPE_UTF8
        if (ns)
            insert(
                escapeStringDecoder(className),
                escapeStringDecoder(superClassName),
                *parentTree, ns);
        else
            insert(
                escapeStringDecoder(className),
                escapeStringDecoder(superClassName));
#else
        if (ns)
            insert(className, superClassName, *parentTree, ns);
        else
            insert(className, superClassName);
#endif
    }
}

void InheritanceTree::check() const
{
    for (InheritanceTreeRep::Table::Iterator i = _rep->table.start(); i; i++)
    {
        if (i.value()->provisional)
            throw InvalidInheritanceTree(i.value()->className.getString());
    }
}

Boolean InheritanceTree::getSubClassNames(
    const CIMName& className,
    Boolean deepInheritance,
    Array<CIMName>& subClassNames,
    NameSpace* ns) const
{
    // -- Case 1: className is empty: get all class names (if deepInheritance)
    // -- or just root class names (if not deepInheritance).

    if (className.isNull())
    {
        for (InheritanceTreeRep::Table::Iterator i = _rep->table.start();i;i++)
        {
            InheritanceTreeNode* itn=i.value();
            if (itn->extension)
            {
                if (!ns)
                    continue;
                for (int j=0,m=itn->extNodes->size(); j<m; j++)
                {
                    InheritanceTreeExt* itx=(*(itn->extNodes))[j];
                    if (itx->tag==ns)
                    {
                        InheritanceTreeNode* itn=itx->node;
                        if (deepInheritance)
                        {
                            subClassNames.append(CIMNameUnchecked(i.key()));
                            itn->getSubClassNames(
                                subClassNames, deepInheritance, ns);
                        }
                        else if (!i.value()->superClass)
                            subClassNames.append(CIMNameUnchecked(i.key()));
                        break;
                    }
                }
            }
            else if (deepInheritance)
            {
                // Append all classes:
                subClassNames.append(CIMNameUnchecked(i.key()));
            }
            else if (!i.value()->superClass)
            {
                // Just append root classes:
                subClassNames.append(CIMNameUnchecked(i.key()));
            }
        }
        return true;
    }

    // -- Case 2: className non-empty: get names of classes descendent from
    // -- the given class.

    for (InheritanceTreeRep::Table::Iterator i = _rep->table.start(); i; i++)
    {
        if (className.equal (CIMNameUnchecked(i.key())))
        {
            i.value()->getSubClassNames(subClassNames, deepInheritance, ns);
            return true;
        }
    }

    // Not found!
    return false;
}

#if 0
Boolean InheritanceTree::isSubClass(
    const CIMName& class1,
    const CIMName& class2) const
{
    InheritanceTreeNode* node = 0;

    if (!_rep->table.lookup(class1.getString(), node))
        return false;

    return node->isSubClass(class2.getString());
}
#endif

Boolean InheritanceTree::getSuperClassNames(
    const CIMName& className,
    Array<CIMName>& superClassNames) const
{
    InheritanceTreeNode* classNode;

    if (_rep->table.lookup(className.getString(), classNode))
    {
        classNode->getSuperClassNames(superClassNames);
        return true;
    }

    return false;
}

Boolean InheritanceTree::getSuperClass(
    const CIMName& className,
    CIMName& superClassName) const
{
    InheritanceTreeNode* classNode;

    if (_rep->table.lookup(className.getString(), classNode))
    {
        if (classNode->superClass)
        {
            superClassName = classNode->superClass->className;
        }
        else
        {
            superClassName.clear();
        }

        return true;
    }

    return false;
}

Boolean InheritanceTree::hasSubClasses(
    const CIMName& className,
    Boolean& hasSubClasses) const
{
    InheritanceTreeNode* node = 0;

    if (!_rep->table.lookup(className.getString(), node))
        return false;

    hasSubClasses = node->subClasses != 0;
    return true;
}

Boolean InheritanceTree::containsClass(const CIMName& className) const
{
    return _rep->table.contains(className.getString());
}

void InheritanceTree::remove(
    const CIMName& className,
    InheritanceTree& parentTree,
    NameSpace* tag)
{
    // -- Lookup the node:

    InheritanceTreeNode* node = 0;

    if (!_rep->table.lookup(className.getString(), node))
        throw PEGASUS_CIM_EXCEPTION(
            CIM_ERR_INVALID_CLASS, className.getString());

    // -- Disallow if is has any subclasses:

    if (node->subClasses)
        throw PEGASUS_CIM_EXCEPTION(
            CIM_ERR_CLASS_HAS_CHILDREN, className.getString());

    // -- Remove as child of superclass:

    InheritanceTreeNode* superClass = node->superClass;

    if (tag)
    {
        InheritanceTreeNode* itn = 0;
        if (parentTree._rep->table.lookup(className.getString(), itn))
        {
            if (itn->extension)
            {
                for (int j = 0, m = itn->extNodes->size(); j < m; j++)
                {
                    if ((*(itn->extNodes))[j]->tag == tag)
                    {
                        itn->extNodes->remove(j);
                        break;
                    }
                }
                if (itn->extNodes->size() == 0)
                {
                    delete itn->extNodes;
                    parentTree._rep->table.remove(className.getString());
                }
            }
        }
        else
        {
            Boolean result = superClass->removeSubClass(node);
            PEGASUS_ASSERT(result);
        }
    }
    else if (superClass)
    {
        Boolean result = superClass->removeSubClass(node);
        PEGASUS_ASSERT(result);
    }


    // -- Remove from the hash table and delete:

    Boolean result = _rep->table.remove(className.getString());
    PEGASUS_ASSERT(result);
    delete node;
}

void InheritanceTree::print(PEGASUS_STD(ostream)& os) const
{
    for (InheritanceTreeRep::Table::Iterator i = _rep->table.start(); i; i++)
        i.value()->print(os);
}

PEGASUS_NAMESPACE_END
