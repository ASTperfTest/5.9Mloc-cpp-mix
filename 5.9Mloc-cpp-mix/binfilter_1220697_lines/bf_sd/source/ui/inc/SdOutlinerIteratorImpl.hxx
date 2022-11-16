/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: SdOutlinerIteratorImpl.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:24:00 $
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/

#ifndef _SD_OUTLINER_ITERATOR_IMPL_HXX
#define _SD_OUTLINER_ITERATOR_IMPL_HXX

#include "SdOutlinerIterator.hxx"
#include <tools/debug.hxx> //STRIP001 
class SdrObjListIter;
namespace binfilter {
class SdDrawDocument;
class SdDrawViewShell;
class SdPage;

namespace sd { namespace outliner {

class IteratorImplBase;

/** Base class for the polymorphic implementation class of the
    <type>Iterator</type> class.  The iterators based on this class are
    basically uni directional iterators.  Their direction can, however, be
    reversed at any point of their life time.
*/
class IteratorImplBase
{
public: 
    IteratorImplBase (SdDrawDocument* pDocument, SdDrawViewShell* pViewShell, 
        bool bDirectionIsForward){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 bool bDirectionIsForward);
    IteratorImplBase (SdDrawDocument* pDocument, SdDrawViewShell* pViewShell, 
        bool bDirectionIsForward, PageKind ePageKind, EditMode eEditMode){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 bool bDirectionIsForward, PageKind ePageKind, EditMode eEditMode);
    virtual ~IteratorImplBase (void){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 virtual ~IteratorImplBase (void);
};




/** Iterator all objects that belong to the current mark list
    a.k.a. selection.  It is assumed that all marked objects belong to the
    same page.  It is further assumed that the mark list does not change
    while an iterator is alive.  It is therefore the responsibility of an
    iterator's owner to handle the case of a changed mark list.

    <p>For documentation of the methods please refere to the base class
    <type>IteratorImplBase</type>.</p>
*/


/** Iterator for iteration over all objects in a single view.  On reaching
    the last object on the last page (or the first object on the first page)
    the view is *not* switched.  Further calls to the
    <member>GotoNextObject()</member> method will be ignored.

    <p>For documentation of the methods please refere to the base class
    <type>IteratorImplBase</type>.</p>
*/


/** Iterator for iteration over all objects in all views.  It automatically
    switches views when reaching the end/beginning of a view.

    <p>For documentation of the methods please refere to the base class
    <type>IteratorImplBase</type>.</p>
*/


} } // end of namespace ::sd::outliner

} //namespace binfilter
#endif
