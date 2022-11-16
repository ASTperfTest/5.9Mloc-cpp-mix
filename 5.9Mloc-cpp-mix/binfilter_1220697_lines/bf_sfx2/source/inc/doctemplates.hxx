/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: doctemplates.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005/09/08 03:25:46 $
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

#ifndef _SFX_DOCTEMPLATES_HXX_
#define _SFX_DOCTEMPLATES_HXX_

#ifndef  _CPPUHELPER_WEAK_HXX_
#include <cppuhelper/weak.hxx>
#endif
#ifndef  _CPPUHELPER_IMPLBASE3_HXX_
#include <cppuhelper/implbase3.hxx>
#endif

#ifndef _COM_SUN_STAR_CONTAINER_XNAMEACCESS_HPP_
#include <com/sun/star/container/XNameAccess.hpp>
#endif
 
#ifndef  _COM_SUN_STAR_FRAME_XDOCUMENTTEMPLATES_HPP_
#include <com/sun/star/frame/XDocumentTemplates.hpp>
#endif
#ifndef  _COM_SUN_STAR_FRAME_XSTORABLE_HPP_
#include <com/sun/star/frame/XStorable.hpp>
#endif
#ifndef _COM_SUN_STAR_FRAME_XMODEL_HPP_
#include <com/sun/star/frame/XModel.hpp>
#endif

#ifndef  _COM_SUN_STAR_LANG_LOCALE_HPP_
#include <com/sun/star/lang/Locale.hpp>
#endif
#ifndef  _COM_SUN_STAR_LANG_XLOCALIZABLE_HPP_
#include <com/sun/star/lang/XLocalizable.hpp>
#endif
#ifndef  _COM_SUN_STAR_LANG_XSERVICEINFO_HPP_
#include <com/sun/star/lang/XServiceInfo.hpp>
#endif
#ifndef _COM_SUN_STAR_LANG_XMULTISERVICEFACTORY_HPP_
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#endif

#ifndef  _COM_SUN_STAR_UCB_XCONTENT_HPP_
#include <com/sun/star/ucb/XContent.hpp>
#endif

#ifndef  _COM_SUN_STAR_BEANS_PROPERTYVALUE_HPP_
#include <com/sun/star/beans/PropertyValue.hpp>
#endif
 
#ifndef  _COM_SUN_STAR_UNO_RUNTIMEEXCEPTION_HPP_
#include <com/sun/star/uno/RuntimeException.hpp>
#endif
#ifndef  _COM_SUN_STAR_UNO_XINTERFACE_HPP_
#include <com/sun/star/uno/XInterface.hpp>
#endif

#ifndef _UCBHELPER_CONTENT_HXX
#include <ucbhelper/content.hxx>
#endif

#ifndef _SFX_SFXUNO_HXX
#include <sfxuno.hxx>
#endif
namespace binfilter {

//--------------------------------------------------------------------------------------------------------

#define LOCALE						::com::sun::star::lang::Locale
#define REFERENCE					::com::sun::star::uno::Reference
#define RUNTIMEEXCEPTION			::com::sun::star::uno::RuntimeException
#define PROPERTYVALUE				::com::sun::star::beans::PropertyValue
#define XCONTENT					::com::sun::star::ucb::XContent
#define XDOCUMENTTEMPLATES			::com::sun::star::frame::XDocumentTemplates
#define XINTERFACE					::com::sun::star::uno::XInterface
#define XLOCALIZABLE				::com::sun::star::lang::XLocalizable
#define XMODEL						::com::sun::star::frame::XModel
#define XMULTISERVICEFACTORY		::com::sun::star::lang::XMultiServiceFactory
#define XNAMEACCESS					::com::sun::star::container::XNameAccess
#define XSERVICEINFO				::com::sun::star::lang::XServiceInfo
#define XSTORABLE					::com::sun::star::frame::XStorable

//--------------------------------------------------------------------------------------------------------

class SfxDocTplService_Impl;

class SfxDocTplService: public ::cppu::WeakImplHelper3< XLOCALIZABLE, XDOCUMENTTEMPLATES, XSERVICEINFO >
{
	SfxDocTplService_Impl		*pImp;

public:
									SFX_DECL_XSERVICEINFO

									SfxDocTplService( const REFERENCE < ::com::sun::star::lang::XMultiServiceFactory >& xFactory );
								   ~SfxDocTplService();

	// --- XLocalizable ---
    void SAL_CALL					setLocale( const LOCALE & eLocale ) throw( RUNTIMEEXCEPTION );
    LOCALE SAL_CALL					getLocale() throw( RUNTIMEEXCEPTION );

	// --- XDocumentTemplates ---
    REFERENCE< XCONTENT > SAL_CALL	getContent() throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				storeTemplate( const ::rtl::OUString& GroupName,
												   const ::rtl::OUString& TemplateName,
												   const REFERENCE< XSTORABLE >& Storable ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				addTemplate( const ::rtl::OUString& GroupName,
												 const ::rtl::OUString& TemplateName,
												 const ::rtl::OUString& SourceURL ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				removeTemplate( const ::rtl::OUString& GroupName,
													const ::rtl::OUString& TemplateName ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				renameTemplate( const ::rtl::OUString& GroupName,
													const ::rtl::OUString& OldTemplateName,
													const ::rtl::OUString& NewTemplateName ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				addGroup( const ::rtl::OUString& GroupName ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				removeGroup( const ::rtl::OUString& GroupName ) throw( RUNTIMEEXCEPTION );
    sal_Bool SAL_CALL				renameGroup( const ::rtl::OUString& OldGroupName,
												 const ::rtl::OUString& NewGroupName ) throw( RUNTIMEEXCEPTION );
    void SAL_CALL					update() throw( RUNTIMEEXCEPTION );
};

}//end of namespace binfilter
#endif