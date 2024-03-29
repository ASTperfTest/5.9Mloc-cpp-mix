/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: ofapch.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006/07/25 09:17:02 $
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

#ifndef _OFAPCH_HXX
#define _OFAPCH_HXX

#define _SVSTDARR_STRINGSISORTDTOR
#define _STD_VAR_ARRAYS
#define _INCL_EXPTRAY
#define _INCL_SFXNEWS

#ifndef OS2
// initialized data in header!
#include <sysdep.hxx>
#endif
#include <svgen.hxx>
#include <tlgen.hxx>
#include <sfx.hxx>
#include <sfxsh.hxx>
#include <sfxdoc.hxx>
#include <sfxdlg.hxx>
#include <sfxview.hxx>
#include <sfxiiter.hxx>
#include <svstdarr.hxx>
#include <svmem.hxx>
#define ITEMID_SPELLCHECK SID_ATTR_SPELL
#define ITEMID_HYPHENREGION 	SID_ATTR_HYPHENREGION
#ifndef OS2
#include <options.hxx>
#endif
namespace binfilter {

} //namespace binfilter
#endif
