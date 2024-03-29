/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sw_flddropdown.cxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:50:26 $
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

#include <algorithm>
#include <svtools/poolitem.hxx>
#ifndef _UNOFLDMID_H
#include <unofldmid.h>
#endif
#include <flddropdown.hxx>
namespace binfilter {
extern String& GetString( const ::com::sun::star::uno::Any& rAny, String& rStr ); //STRIP008
static String aEmptyString;

SwDropDownFieldType::SwDropDownFieldType()
    : SwFieldType(RES_DROPDOWN)
{
}

SwDropDownFieldType::~SwDropDownFieldType()
{
}

SwFieldType * SwDropDownFieldType::Copy() const
{
    return new SwDropDownFieldType;
}

SwDropDownField::SwDropDownField(SwFieldType * pTyp)
    : SwField(pTyp, 0, LANGUAGE_SYSTEM)
{
}

SwDropDownField::SwDropDownField(const SwDropDownField & rSrc)
    : SwField(rSrc.GetTyp(), rSrc.GetFormat(), rSrc.GetLanguage()), 
      aValues(rSrc.aValues), aSelectedItem(rSrc.aSelectedItem), 
      aName(rSrc.aName)
{
}

SwDropDownField::~SwDropDownField()
{
}

String SwDropDownField::Expand() const
{
    String sSelect = GetSelectedItem();
    if(!sSelect.Len())
    {        
        vector<String>::const_iterator aIt = aValues.begin();
        if ( aIt != aValues.end())
            sSelect = *aIt;
    }
    //if still no list value is available a default text of 10 spaces is to be set
    if(!sSelect.Len())
        sSelect.AppendAscii ( RTL_CONSTASCII_STRINGPARAM ("          "));
    return sSelect;
}

SwField * SwDropDownField::Copy() const
{
    return new SwDropDownField(*this);
}

const String & SwDropDownField::GetPar1() const
{
    return GetSelectedItem();
}

String SwDropDownField::GetPar2() const
{
    return GetName();
}

void SwDropDownField::SetPar1(const String & rStr)
{
    SetSelectedItem(rStr);
}

void SwDropDownField::SetPar2(const String & rName)
{
    SetName(rName);
}

void SwDropDownField::SetItems(const Sequence<OUString> & rItems)
{
    aValues.clear();

    sal_Int32 aCount = rItems.getLength();
    for (int i = 0; i < aCount; i++)
        aValues.push_back(rItems[i]);

    aSelectedItem = aEmptyString;
}

Sequence<OUString> SwDropDownField::GetItemSequence() const
{
    Sequence<OUString> aSeq( aValues.size() );
    OUString* pSeq = aSeq.getArray();
    int i = 0;
    vector<String>::const_iterator aIt;
    
    for (aIt = aValues.begin(); aIt != aValues.end(); aIt++)
    {
        pSeq[i] = ::rtl::OUString(*aIt);
        
        i++;
    }

    return aSeq;
}

const String & SwDropDownField::GetSelectedItem() const
{
    return aSelectedItem;
}

const String & SwDropDownField::GetName() const
{
    return aName;
}

BOOL SwDropDownField::SetSelectedItem(const String & rItem)
{
    vector<String>::const_iterator aIt = 
        find(aValues.begin(), aValues.end(), rItem);

    if (aIt != aValues.end())
        aSelectedItem = *aIt;
    else
        aSelectedItem = String();

    return (aIt != aValues.end());
}

void SwDropDownField::SetName(const String & rName)
{
    aName = rName;
}

BOOL SwDropDownField::QueryValue(Any &rVal, BYTE nMId) 
    const
{
    nMId &= ~CONVERT_TWIPS;
    switch( nMId )
    {
    case FIELD_PROP_PAR1:
        rVal <<= ::rtl::OUString(GetSelectedItem());
        break;
    case FIELD_PROP_PAR2:
        rVal <<= ::rtl::OUString(GetName());
        break;
    case FIELD_PROP_STRINGS:
        rVal <<= GetItemSequence();

        break;

    default:
        DBG_ERROR("illegal property");
    }
    return sal_True;
}

BOOL SwDropDownField::PutValue(const Any &rVal, 
                               BYTE nMId)
{
    nMId &= ~CONVERT_TWIPS;
    switch( nMId )
    {
    case FIELD_PROP_PAR1:
        {
            String aTmpStr;
			::binfilter::GetString( rVal, aTmpStr );
            
            SetSelectedItem(aTmpStr);
        }
        break;

    case FIELD_PROP_PAR2:
        {
            String aTmpStr;
            ::binfilter::GetString( rVal, aTmpStr );
            
            SetName(aTmpStr);
        }
        break;

    case FIELD_PROP_STRINGS:
        {
            Sequence<OUString> aSeq;
            rVal >>= aSeq;
            SetItems(aSeq);
        }
        break;
        
    default:
        DBG_ERROR("illegal property");
    }
    return sal_True;
}
}
