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

#ifndef Pegasus_QualifierList_h
#define Pegasus_QualifierList_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Linkage.h>
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/CIMQualifier.h>
#include <Pegasus/Common/CIMScope.h>
#include <Pegasus/Common/Array.h>
#include <Pegasus/Common/Pair.h>
#include <Pegasus/Common/Buffer.h>


/*  ATTN: P2 KS 25 Mar 2002 - The function names are a mismash of local and
    taken from the class and instance functions.  Thus, we have getCount but
    getQualifier.  This causes confusion with the functions in class and
    instance which are specifically getQualifier.  We should clean up so the
    names here remove the Qualifier portion.
*/
PEGASUS_NAMESPACE_BEGIN

class DeclContext;


/* ATTN: KS P3 DEFER 1 May 2002.
    We have list processors (really array processors) for
    qualifiers, properties, methods(???) but they are all slightly different.
    Should we create a common base??
*/
/** This class is for representing Qualifier lists in the CIM interface.

    Members are provided for accessing elements of the the internal property
    list. There are none for modifying elements (the entire array must be
    formed and passed to the constructor or replaced by calling set()).
*/

class PEGASUS_COMMON_LINKAGE CIMQualifierList
{
public:

    /// constructor - Constructs a NULL qualifier list.
    CIMQualifierList();

    ///
    ~CIMQualifierList();

    /** add adds a single CIMQualifier to a CIMQualifierList
    */
    CIMQualifierList& add(const CIMQualifier& qualifier);

    /** getCount - Returns the count of qualifiers in the list
        @return Zero origin count of qualifiers in the qualifier list.
    */
    Uint32 getCount() const
    {
        return _qualifiers.size();
    }

    /** getQualifer - Gets the qaulifier defined at the index provided
        in the Qualifier list.
        @param index - The position in the qualifierlist containing the
        qualifier.
        @return CIMQualifier object.
        @exception - Throws OutofBounds exception of pso not within the
        qualifier list.
        ATTN: P0 KS Mar 2002 Add the outofbounds exception.
    */
    CIMQualifier& getQualifier(Uint32 index);

    const CIMQualifier& getQualifier(Uint32 index) const
    {
        return _qualifiers[index];
    }

    /** removeQualifier - Removes the Qualifier defined by
        the index parameter
        @exception IndexOutOfBoundsException if index not within
        range of current qualifiers.
    */
    void removeQualifier(Uint32 index);

    /** find - Searches for a qualifier with the specified `
        input name if it exists in the class
        @param name CIMName of the qualifier
        to be found @return Position of the qualifier in the Class.
        @return Returns index of the qualifier found or PEG_NOT_FOUND
        if not found.
    */
    Uint32 find(const CIMName& name) const;

    /** exists - Returns true if the qualifier with the
        specified name exists in the class
        @param name CIMName name of the qualifier object being tested.
        @return True if the qualifier exits in the list.  Otherwise
        false is returned.
    */

    Boolean exists(const CIMName& name) const
    {
        return (find(name) != PEG_NOT_FOUND);
    }

    /** isTrue - Determines if the qualifier defined by
        the input parameter exists for the class, is Boolean, and
        has a value of true.
        This function is useful to quickly determine the state of a
        qualifier.
        @param CIMName containing the qualifier  name.
        @return Boolean True if the qualifier exists,
    */
    Boolean isTrue(const CIMName& name) const;

    /// findReverse - See find
    Uint32 findReverse(const CIMName& name) const;

    /** resolve - Resolves the qualifierList based on the information provided.
        The resolved qualifiers are the result of validating and testing the
        qualifiers against the inherited qualifiers and qualifier declarations.
        The qualifier list contains the set of resolved qualifiers when the
        function is complete.

        Resolution includes:
        1. Determining if the qualifier is declared (obtainable form
           declContext).
        2. Qualifier is same type as declaration
        3. Valid for the scope provided (Qualifier scope contains the provided
           scope).
        4. Whether qualifier can be overridden.
        5. Whether it should be propagated to subclass
        If a qualifier can be overridden it is put into the qualifiers array.

        @param declContext  Declaration context for this resolution (ex
        repository, simple)
        @param nameSpace Namespace in which to find the declaration.
        @param scope - Scope of the entity doing the resolution (ex. Class,
        association, etc.)
        @param isInstancePart - TBD
        @param inheritedQualifiers - CimQualifierList defining List of
        inherited qualifiers
        @param propagateQualifiers Boolean indicator whether to propagate
        qualifiers.
        @exception - There are a number of different
    */
    void resolve(
        DeclContext* declContext,
        const CIMNamespaceName & nameSpace,
        CIMScope scope,
        Boolean isInstancePart,
        CIMQualifierList& inheritedQualifiers,
        Boolean propagateQualifiers);

    ///
    void toXml(Buffer& out) const;

    ///
    void toMof(Buffer& out) const;

    ///
    void print(PEGASUS_STD(ostream) &o=PEGASUS_STD(cout)) const;

    ///
    Boolean identical(const CIMQualifierList& x) const;

    ///
    void cloneTo(CIMQualifierList& x) const;

private:

    Array<CIMQualifier> _qualifiers;
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_QualifierList_h */
