/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svx_clonelist.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: hr $ $Date: 2007/01/02 17:29:52 $
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

// #i13033#
// New mechanism to hold a ist of all original and cloned objects for later
// re-creating the connections for contained connectors

#ifndef _CLONELIST_HXX_
#include <clonelist.hxx>
#endif

#ifndef _SVDOEDGE_HXX
#include <svdoedge.hxx>
#endif

#ifndef _E3D_SCENE3D_HXX
#include "scene3d.hxx"
#endif
namespace binfilter {

CloneList::CloneList()
{
}

CloneList::~CloneList()
{
}

void CloneList::AddPair(const SdrObject* pOriginal, SdrObject* pClone)
{
	maOriginalList.Insert((SdrObject*)pOriginal, LIST_APPEND);
	maCloneList.Insert(pClone, LIST_APPEND);

	// look for subobjects, too.
	sal_Bool bOriginalIsGroup(pOriginal->IsGroupObject());
	sal_Bool bCloneIsGroup(pClone->IsGroupObject());

	if(bOriginalIsGroup && pOriginal->ISA(E3dObject) && !pOriginal->ISA(E3dScene))
		bOriginalIsGroup = sal_False;

	if(bCloneIsGroup && pClone->ISA(E3dObject) && !pClone->ISA(E3dScene))
		bCloneIsGroup = sal_False;

	if(bOriginalIsGroup && bCloneIsGroup)
	{
		const SdrObjList* pOriginalList = pOriginal->GetSubList();
		SdrObjList* pCloneList = pClone->GetSubList();

		if(pOriginalList && pCloneList 
			&& pOriginalList->GetObjCount() == pCloneList->GetObjCount())
		{
			for(sal_uInt32 a(0); a < pOriginalList->GetObjCount(); a++)
			{
				// recursive call
				AddPair(pOriginalList->GetObj(a), pCloneList->GetObj(a));
			}
		}
	}
}

const SdrObject* CloneList::GetOriginal(sal_uInt32 nIndex) const
{
	return (SdrObject*)maOriginalList.GetObject(nIndex);
}

SdrObject* CloneList::GetClone(sal_uInt32 nIndex) const
{
	return (SdrObject*)maCloneList.GetObject(nIndex);
}

}