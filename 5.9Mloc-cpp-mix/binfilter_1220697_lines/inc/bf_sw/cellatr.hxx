/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: cellatr.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: kz $ $Date: 2006/11/08 13:09:09 $
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

#ifndef _CELLATR_HXX
#define _CELLATR_HXX

#ifndef _SFXINTITEM_HXX //autogen
#include <svtools/intitem.hxx>
#endif
#ifndef _ZFORLIST_HXX //autogen
#include <svtools/zforlist.hxx>
#endif

#include "format.hxx"
#include "cellfml.hxx"
namespace binfilter {


class SwTblBoxNumFormat : public SfxUInt32Item
{
	BOOL bAuto;		// automatisch vergebenes Flag
public:
	SwTblBoxNumFormat( UINT32 nFormat = NUMBERFORMAT_TEXT,
						BOOL bAuto = FALSE );

	// "pure virtual Methoden" vom SfxPoolItem
	virtual int             operator==( const SfxPoolItem& ) const;
	virtual SfxPoolItem*	Clone( SfxItemPool* pPool = 0 ) const;
	virtual SfxPoolItem*	Create(SvStream &, USHORT nVer) const;
	virtual SvStream&		Store(SvStream &, USHORT nIVer) const;
	virtual USHORT			GetVersion( USHORT nFileVersion) const;

	inline SwTblBoxNumFormat& operator=( const SwTblBoxNumFormat& rAttr )
	{
		SetValue( rAttr.GetValue() );
		SetAutoFlag( rAttr.GetAutoFlag() );
		return *this;
	}

	BOOL GetAutoFlag() const 					{ return bAuto; }
	void SetAutoFlag( BOOL bFlag = TRUE )		{ bAuto = bFlag; }
};

class SwTblBoxFormula : public SfxPoolItem, public SwTableFormula
{
	SwModify* pDefinedIn;	// Modify-Object, in dem die Formel steht
							// kann nur TablenBoxFormat sein

public:
	SwTblBoxFormula( const String& rFormula );

	// "pure virtual Methoden" vom SfxPoolItem
	virtual int             operator==( const SfxPoolItem& ) const;
	virtual SfxPoolItem*	Clone( SfxItemPool* pPool = 0 ) const;
    virtual SfxPoolItem*    Create(SvStream &, USHORT nVer) const;
    virtual SvStream&       Store(SvStream &, USHORT nIVer) const;
	virtual USHORT			GetVersion( USHORT nFileVersion) const;

	// erfrage und setze den Modify-Pointer
	inline const SwModify* GetDefinedIn() const { return pDefinedIn; }
	inline void ChgDefinedIn( const SwModify* pNew )
											{ pDefinedIn = (SwModify*)pNew; }
	// suche den Node, in dem die Formel steht:
	//	BoxAttribut	-> BoxStartNode
	virtual const SwNode* GetNodeOfFormula() const;

          SwTableBox* GetTableBox();
    const SwTableBox* GetTableBox() const
        { return ((SwTblBoxFormula*)this)->GetTableBox(); }

	// Status aendern
    void ChangeState( const SfxPoolItem* pItem );
	// berechne die Formel
};

class SwTblBoxValue : public SfxPoolItem
{
	double nValue;
public:
	SwTblBoxValue();
	SwTblBoxValue( const double aVal );

	// "pure virtual Methoden" vom SfxPoolItem
	virtual int             operator==( const SfxPoolItem& ) const;
	virtual SfxPoolItem*	Clone( SfxItemPool* pPool = 0 ) const;
	virtual SfxPoolItem*	Create(SvStream &, USHORT nVer) const;
	virtual SvStream&		Store(SvStream &, USHORT nIVer) const;
	virtual USHORT			GetVersion( USHORT nFileVersion) const;

	inline SwTblBoxValue& operator=( const SwTblBoxValue& rCmp )
	{
		nValue = rCmp.nValue;
		return *this;
	}

	double GetValue() const 					{ return nValue; }
	void SetValue( const double nVal )			{ nValue = nVal; }
};



//***************************************************************************

#if !(defined(MACOSX) && ( __GNUC__ < 3 ))
// GrP moved to gcc_outl.cxx; revisit with gcc3
inline const SwTblBoxNumFormat      &SwAttrSet::GetTblBoxNumFmt(BOOL bInP) const
	{   return (const SwTblBoxNumFormat&)Get( RES_BOXATR_FORMAT,bInP); }
inline const SwTblBoxFormula		&SwAttrSet::GetTblBoxFormula(BOOL bInP) const
	{   return (const SwTblBoxFormula&)Get( RES_BOXATR_FORMULA,bInP); }
inline const SwTblBoxValue			&SwAttrSet::GetTblBoxValue(BOOL bInP) const
	{   return (const SwTblBoxValue&)Get( RES_BOXATR_VALUE, bInP); }
#endif

//***************************************************************************

#if !(defined(MACOSX) && ( __GNUC__ < 3 ))
// GrP moved to gcc_outl.cxx; revisit with gcc3
inline const SwTblBoxNumFormat		&SwFmt::GetTblBoxNumFmt(BOOL bInP) const
	{   return aSet.GetTblBoxNumFmt(bInP); }
inline const SwTblBoxFormula		&SwFmt::GetTblBoxFormula(BOOL bInP) const
	{   return aSet.GetTblBoxFormula(bInP); }
inline const SwTblBoxValue			&SwFmt::GetTblBoxValue(BOOL bInP) const
	{   return aSet.GetTblBoxValue(bInP); }
#endif

} //namespace binfilter
#endif