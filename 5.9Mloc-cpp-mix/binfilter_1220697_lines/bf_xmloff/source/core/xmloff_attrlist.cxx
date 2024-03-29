/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: xmloff_attrlist.cxx,v $
 *
 *  $Revision: 1.6.114.1 $
 *
 *  last change: $Author: hr $ $Date: 2008/01/02 16:48:56 $
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

#include <vector>

#ifndef _XMLOFF_XMLTOKEN_HXX
#include "xmltoken.hxx"
#endif
#include <rtl/uuid.h>
#include <rtl/memory.h>

#if OSL_DEBUG_LEVEL == 0
#  ifndef NDEBUG
#    define NDEBUG
#  endif
#endif
#include <assert.h>

#include "attrlist.hxx"
namespace binfilter {

using namespace ::rtl;
using namespace ::osl;
using namespace ::com::sun::star;
using namespace ::binfilter::xmloff::token;

struct SvXMLTagAttribute_Impl
{
	SvXMLTagAttribute_Impl(){}
	SvXMLTagAttribute_Impl( const OUString &rName,
						 const OUString &rValue )
		: sName(rName),
		sValue(rValue)
	{
	}

	OUString sName;
	OUString sValue;
};

struct SvXMLAttributeList_Impl
{
	SvXMLAttributeList_Impl()
	{
		// performance improvement during adding
		vecAttribute.reserve(20);
	}
	::std::vector<struct SvXMLTagAttribute_Impl> vecAttribute;
};



sal_Int16 SAL_CALL SvXMLAttributeList::getLength(void) throw( ::com::sun::star::uno::RuntimeException )
{
	return m_pImpl->vecAttribute.size();
}


SvXMLAttributeList::SvXMLAttributeList( const SvXMLAttributeList &r )
{
	m_pImpl = new SvXMLAttributeList_Impl;
	*m_pImpl = *(r.m_pImpl);
}

SvXMLAttributeList::SvXMLAttributeList( const uno::Reference< 
		xml::sax::XAttributeList> & rAttrList )
    : sType( GetXMLToken(XML_CDATA) )
{
	m_pImpl = new SvXMLAttributeList_Impl;

	SvXMLAttributeList* pImpl =
		SvXMLAttributeList::getImplementation( rAttrList );

	if( pImpl )
		*m_pImpl = *(pImpl->m_pImpl);
	else
		AppendAttributeList( rAttrList );
}

OUString SAL_CALL SvXMLAttributeList::getNameByIndex(sal_Int16 i) throw( ::com::sun::star::uno::RuntimeException )
{
	if( i < m_pImpl->vecAttribute.size() ) {
		return m_pImpl->vecAttribute[i].sName;
	}
	return OUString();
}


OUString SAL_CALL SvXMLAttributeList::getTypeByIndex(sal_Int16 i) throw( ::com::sun::star::uno::RuntimeException )
{
    return sType;
}

OUString SAL_CALL  SvXMLAttributeList::getValueByIndex(sal_Int16 i) throw( ::com::sun::star::uno::RuntimeException )
{
	if( i < m_pImpl->vecAttribute.size() ) {
		return m_pImpl->vecAttribute[i].sValue;
	}
	return OUString();

}

OUString SAL_CALL SvXMLAttributeList::getTypeByName( const OUString& sName ) throw( ::com::sun::star::uno::RuntimeException )
{
    return sType;
}

OUString SAL_CALL SvXMLAttributeList::getValueByName(const OUString& sName) throw( ::com::sun::star::uno::RuntimeException )
{
	::std::vector<struct SvXMLTagAttribute_Impl>::iterator ii = m_pImpl->vecAttribute.begin();

	for( ; ii != m_pImpl->vecAttribute.end() ; ii ++ ) {
		if( (*ii).sName == sName ) {
			return (*ii).sValue;
		}
	}
	return OUString();
}


uno::Reference< ::com::sun::star::util::XCloneable >  SvXMLAttributeList::createClone() throw( ::com::sun::star::uno::RuntimeException )
{
	uno::Reference< ::com::sun::star::util::XCloneable >  r = new SvXMLAttributeList( *this );
	return r;
}


SvXMLAttributeList::SvXMLAttributeList()
    : sType( GetXMLToken(XML_CDATA) )
{
	m_pImpl = new SvXMLAttributeList_Impl;
}



SvXMLAttributeList::~SvXMLAttributeList()
{
	delete m_pImpl;
}


void SvXMLAttributeList::AddAttribute( 	const OUString &sName ,
										const OUString &sValue )
{
	m_pImpl->vecAttribute.push_back( SvXMLTagAttribute_Impl( sName , sValue ) );
}

void SvXMLAttributeList::Clear()
{
	m_pImpl->vecAttribute.clear();

	assert( ! getLength() );
}

void SvXMLAttributeList::RemoveAttribute( const OUString sName )
{
	::std::vector<struct SvXMLTagAttribute_Impl>::iterator ii = m_pImpl->vecAttribute.begin();

	for( ; ii != m_pImpl->vecAttribute.end() ; ii ++ ) {
		if( (*ii).sName == sName ) {
			m_pImpl->vecAttribute.erase( ii );
			break;
		}
	}
}

void SvXMLAttributeList::AppendAttributeList( const uno::Reference< ::com::sun::star::xml::sax::XAttributeList >  &r )
{
	assert( r.is() );

	sal_Int32 nMax = r->getLength();
	sal_Int32 nTotalSize = m_pImpl->vecAttribute.size() + nMax;
	m_pImpl->vecAttribute.reserve( nTotalSize );

	for( sal_Int32 i = 0 ; i < nMax ; i ++ ) {
		m_pImpl->vecAttribute.push_back( SvXMLTagAttribute_Impl(
			r->getNameByIndex( i ) ,
			r->getValueByIndex( i )));
	}

	assert( nTotalSize == getLength());
}

// XUnoTunnel & co
const uno::Sequence< sal_Int8 > & SvXMLAttributeList::getUnoTunnelId() throw()
{
	static uno::Sequence< sal_Int8 > * pSeq = 0;
	if( !pSeq )
	{
		Guard< Mutex > aGuard( Mutex::getGlobalMutex() );
		if( !pSeq )
		{
			static uno::Sequence< sal_Int8 > aSeq( 16 );
			rtl_createUuid( (sal_uInt8*)aSeq.getArray(), 0, sal_True );
			pSeq = &aSeq;
		}
	}
	return *pSeq;
}

SvXMLAttributeList* SvXMLAttributeList::getImplementation( uno::Reference< uno::XInterface > xInt ) throw()
{
	uno::Reference< lang::XUnoTunnel > xUT( xInt, uno::UNO_QUERY );
	if( xUT.is() )
		return (SvXMLAttributeList*)xUT->getSomething( SvXMLAttributeList::getUnoTunnelId() );
	else
		return NULL;
}

// XUnoTunnel
sal_Int64 SAL_CALL SvXMLAttributeList::getSomething( const uno::Sequence< sal_Int8 >& rId )
	throw( uno::RuntimeException )
{
	if( rId.getLength() == 16 && 0 == rtl_compareMemory( getUnoTunnelId().getConstArray(),
														 rId.getConstArray(), 16 ) )
	{
		return (sal_Int64)this;
	}
	return 0;
}


}//end of namespace binfilter
