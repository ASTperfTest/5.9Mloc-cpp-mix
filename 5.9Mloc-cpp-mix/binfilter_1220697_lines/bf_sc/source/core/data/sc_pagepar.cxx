/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sc_pagepar.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/27 14:22:51 $
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

// INCLUDE ---------------------------------------------------------------

// System - Includes -----------------------------------------------------

#ifdef PCH
#endif


#include <string.h>

#include "pagepar.hxx"
namespace binfilter {


//========================================================================
// struct ScPageTableParam:

/*N*/ ScPageTableParam::ScPageTableParam()
/*N*/ {
/*N*/ 	Reset();
/*N*/ }

//------------------------------------------------------------------------

/*N*/ ScPageTableParam::ScPageTableParam( const ScPageTableParam& r )
/*N*/ {
/*N*/ 	*this = r;
/*N*/ }

//------------------------------------------------------------------------

/*N*/ __EXPORT ScPageTableParam::~ScPageTableParam()
/*N*/ {
/*N*/ }

//------------------------------------------------------------------------

/*N*/ void __EXPORT ScPageTableParam::Reset()
/*N*/ {
/*N*/ 	bNotes=bGrid=bHeaders=bDrawings=
/*N*/ 	bLeftRight=bScaleAll=bScalePageNum=
/*N*/ 	bFormulas=bNullVals=bSkipEmpty			= FALSE;
/*N*/ 	bTopDown=bScaleNone=bCharts=bObjects	= TRUE;
/*N*/ 	nScaleAll		= 100;
/*N*/ 	nScalePageNum	= 0;
/*N*/ 	nFirstPageNo	= 1;
/*N*/ }

//------------------------------------------------------------------------


//------------------------------------------------------------------------


//========================================================================
// struct ScPageAreaParam:

/*N*/ ScPageAreaParam::ScPageAreaParam()
/*N*/ {
/*N*/ 	Reset();
/*N*/ }

//------------------------------------------------------------------------

/*N*/ ScPageAreaParam::ScPageAreaParam( const ScPageAreaParam& r )
/*N*/ {
/*N*/ 	*this = r;
/*N*/ }

//------------------------------------------------------------------------

/*N*/ __EXPORT ScPageAreaParam::~ScPageAreaParam()
/*N*/ {
/*N*/ }

//------------------------------------------------------------------------

/*N*/ void __EXPORT ScPageAreaParam::Reset()
/*N*/ {
/*N*/ 	bPrintArea = bRepeatRow = bRepeatCol = FALSE;
/*N*/ 
/*N*/ 	memset( &aPrintArea, 0, sizeof(ScRange) );
/*N*/ 	memset( &aRepeatRow, 0, sizeof(ScRange) );
/*N*/ 	memset( &aRepeatCol, 0, sizeof(ScRange) );
/*N*/ }

//------------------------------------------------------------------------


//------------------------------------------------------------------------

}
