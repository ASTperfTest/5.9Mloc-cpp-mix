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

#ifndef Pegasus_ContentLanguageList_h
#define Pegasus_ContentLanguageList_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/LanguageTag.h>

#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES

PEGASUS_NAMESPACE_BEGIN

class ContentLanguageListRep;

//////////////////////////////////////////////////////////////
//
// ContentLanguageList
//
//////////////////////////////////////////////////////////////

/** <I><B>Experimental Interface</B></I><BR>
    This class represents a list of content languages (such as may appear
    in an HTTP Content-Language header value).  It is managed as a list of
    LanguageTag objects.
 */
class PEGASUS_COMMON_LINKAGE ContentLanguageList
{
public:

    /**
        Constructs an empty ContentLanguageList object.
     */
    ContentLanguageList();

    /**
        Copy constructor.
        @param contentLanguages The ContentLanguageList object to copy.
     */
    ContentLanguageList(const ContentLanguageList& contentLanguages);

    /**
        Destructor.
     */
    ~ContentLanguageList();

    /**
        Assignment operator.
        @param contentLanguages The ContentLanguageList object to copy.
     */
    ContentLanguageList& operator=(const ContentLanguageList& contentLanguages);

    /**
        Returns the number of LanguageTags in the ContentLanguageList object.
        @return Integer size of the ContentLanguageList list.
     */
    Uint32 size() const;

    /**
        Accesses a LanguageTag at a specified index.
        @param index Integer index of the LanguageTag to access.
        Valid indices range from 0 to size()-1.
        @return The LanguageTag corresponding to the specified index.
        @exception IndexOutOfBoundsException If the specified index is out of
        range.
     */
    LanguageTag getLanguageTag(Uint32 index) const;

    /**
        Appends a LanguageTag to the ContentLanguageList object.
        @param languageTag The LanguageTag to append.
     */
    void append(const LanguageTag& languageTag);

    /**
        Removes the specified LanguageTag from the ContentLanguageList object.
        @param index Integer index of the LanguageTag to remove.
        @exception IndexOutOfBoundsException If the specified index is out of
        range.
     */
    void remove(Uint32 index);

    /**
        Finds the specified LanguageTag in the ContentLanguageList object and
        returns its index.
        @param languageTag The LanguageTag to find.
        @return Integer index of the LanguageTag, if found; otherwise
        PEG_NOT_FOUND.
     */
    Uint32 find(const LanguageTag& languageTag) const;

    /**
        Removes all the LanguageTags from the ContentLanguageList object.
     */
    void clear();

    /**
        Tests ContentLanguageList objects for equality.
        @param contentLanguages A ContentLanguageList object to be compared.
        @return True if the ContentLanguageList objects contain the same
        LanguageTags in the same order, false otherwise.
     */
    Boolean operator==(const ContentLanguageList& contentLanguages) const;

    /**
        Tests ContentLanguageList objects for inequality.
        @param contentLanguages A ContentLanguageList object to be compared.
        @return False if the ContentLanguageList objects contain the same
        LanguageTags in the same order, true otherwise.
     */
    Boolean operator!=(const ContentLanguageList& contentLanguages) const;

private:
    ContentLanguageListRep *_rep;
};

PEGASUS_NAMESPACE_END

#endif  // PEGASUS_USE_EXPERIMENTAL_INTERFACES

#endif
