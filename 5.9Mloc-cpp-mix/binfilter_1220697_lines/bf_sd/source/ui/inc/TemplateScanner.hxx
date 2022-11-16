/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: TemplateScanner.hxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 18:24:12 $
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

#ifndef _TEMPLATE_SCANNER_HXX
#define _TEMPLATE_SCANNER_HXX

#ifndef _UCBHELPER_CONTENT_HXX 
#include <ucbhelper/content.hxx>
#endif





#ifndef _STRING_HXX
#include <tools/string.hxx>
#endif

#include <vector>
namespace binfilter {

namespace sd
{

/** Representation of a template or layout file.
*/
class TemplateEntry
{
public:
    TemplateEntry   (const String& rTitle, const String& rPath)
        :   m_aTitle (rTitle), m_aPath  (rPath) {}

    String m_aTitle;
    String m_aPath;
};




/** Representation of a template or layout folder.
*/
class TemplateDir
{
public:
    TemplateDir (const String & rRegion, const String & rUrl )
        :   m_aRegion(rRegion), m_aUrl(rUrl) {}

    String                      m_aRegion;
    String                      m_aUrl;
    std::vector<TemplateEntry*> m_aEntries;
};




/** This class scans the template folders for impress templates and puts
    them into listboxes.  While scanning the found templates are collected
    in a local list.  After completing the scan an object of this class may
    be called to offer this list to transfer its contents to the caller.
    Every remaining entries are deleted from the local list in the
    destructor.  An easy way to transfer the whole list is to call the swap
    method on an STL vector with the list as argument.
*/
class   TemplateScanner
{
public:
    /** Create a new template scanner and prepare but do not execute the scanning.
    */
    TemplateScanner (void);

    /** The destructor deletes any remaining entries of the local list of
        templates.
    */
    virtual ~TemplateScanner (void);

    /** Execute the actual scanning of templates.  When this method
        terminates the result can be obtained by calling the
        <member>GetTemplateList</member> method.
    */
    void Scan (void);

    /** Return the list of template folders.  It lies in the responsibility
        of the caller to take ownership of some or all entries and remove
        them from the returned list.  All entries that remain until the
        destructor is called will be destroyed.
    */
    std::vector<TemplateDir*>& GetFolderList (void);

protected:
    /** Set the member mxTemplateRoot to the XContent that represents the
        root of the template tree.
    */
    void ScanFolders (void);

    /** Scan the given template folder for impress templates and insert them
        into the given list.
        @param  rRoot   
            specifies the folder to search.
        @param  pDir    
            The list into which the entries for the impress templates are to
            be inserted.
    */
    void ScanEntries (::ucb::Content& rRoot, TemplateDir* pDir);
    
    /** Obtain the root folder of the template folder hierarchy.  The result
        is stored in mxTemplateRoot for later use.
    */
    void GetTemplateRoot (void);
    
private:

    /** The data structure that is to be filled with information about the
        template files.
    */
     std::vector<TemplateDir*> maFolderList;
    
    /** The root folder of the template folders.
    */
    ::com::sun::star::uno::Reference< ::com::sun::star::ucb::XContent>   
        mxTemplateRoot;
};

}

} //namespace binfilter
#endif