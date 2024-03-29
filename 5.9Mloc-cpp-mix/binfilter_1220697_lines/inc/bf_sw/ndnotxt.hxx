/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: ndnotxt.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2006/10/28 04:49:25 $
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
#ifndef _NDNOTXT_HXX
#define _NDNOTXT_HXX

#include "node.hxx"
class PolyPolygon; 
namespace binfilter {



// --------------------
// SwNoTxtNode
// --------------------

class SwNoTxtNode : public SwCntntNode
{
	friend class SwNodes;
	friend class SwNoTxtFrm;

	String aAlternateText;		// alternativer Text  (HTML)

	PolyPolygon *pContour;		// Polygon fuer Konturumlauf
    BOOL bAutomaticContour : 1; // automatic contour polygon, not manipulated
	BOOL bContourMapModeValid : 1; // contour map mode is not the graphics's
								   // prefered map mode, but either
   								   // MM100 or or pixel
	BOOL bPixelContour : 1;		// contour map mode is invalid and pixel.

	// erzeugt fuer alle Ableitungen einen AttrSet mit Bereichen
	// fuer Frame- und Grafik-Attributen (wird nur vom SwCntntNode gerufen)
	virtual void NewAttrSet( SwAttrPool& );

	SwNoTxtNode( const SwNoTxtNode& );				//nicht erlaubt
	SwNoTxtNode &operator=( const SwNoTxtNode& );	//nicht erlaubt

protected:
	SwNoTxtNode( const SwNodeIndex &rWhere, const BYTE nNdType,
				SwGrfFmtColl *pGrColl, SwAttrSet* pAutoAttr = 0 );

public:
	~SwNoTxtNode();

	virtual SwCntntFrm *MakeFrm();

	inline SwGrfFmtColl *GetGrfColl() const;

	virtual Size GetTwipSize() const = 0;


	// alternativen Text abfragen/setzen
	const String& GetAlternateText() const		{ return aAlternateText; }
	void SetAlternateText( const String& rTxt, sal_Bool bBroadcast=sal_False );

    void               SetContour( const PolyPolygon *pPoly,
                                   BOOL bAutomatic = FALSE );
	const PolyPolygon *HasContour() const;
	const BOOL		   _HasContour() const { return pContour!=0; };

	void			   SetAutomaticContour( BOOL bSet ) { bAutomaticContour = bSet; }
    const BOOL         HasAutomaticContour() const { return bAutomaticContour; }

	// set either a MM100 or pixel contour
    void               SetContourAPI( const PolyPolygon *pPoly ){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 void               SetContourAPI( const PolyPolygon *pPoly );

	// get either a MM100 or pixel contour, return FALSE if no contour is set.
	BOOL  			   GetContourAPI( PolyPolygon &rPoly ) const;

	void			   SetPixelContour( BOOL bSet ) { bPixelContour = bSet; }
    const BOOL         IsPixelContour() const;

    const BOOL         IsContourMapModeValid() const { return bContourMapModeValid; }

	//Besorgt die Graphic, mit SwapIn fuer GrfNode, per GetData fuer OLE.
    Graphic GetGraphic() const;
};

inline SwGrfFmtColl* SwNoTxtNode::GetGrfColl() const
{
	return (SwGrfFmtColl*)GetRegisteredIn();
}

// Inline Metoden aus Node.hxx - erst hier ist der TxtNode bekannt !!
#if !(defined(MACOSX) && ( __GNUC__ < 3 ))
// GrP moved to gcc_outl.cxx; revisit with gcc3
inline SwNoTxtNode *SwNode::GetNoTxtNode()
{
	return ND_NOTXTNODE & nNodeType ? (SwNoTxtNode*)this : 0;
}
inline const SwNoTxtNode *SwNode::GetNoTxtNode() const
{
	return ND_NOTXTNODE & nNodeType ? (const SwNoTxtNode*)this : 0;
}
#endif

} //namespace binfilter
#endif	// _NDNOTXT_HXX
