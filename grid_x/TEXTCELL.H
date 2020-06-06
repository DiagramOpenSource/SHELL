#ifndef textcell_h
#define textcell_h

#include "cell.h"

#ifdef WIN32
	#define WCMFC_DEF
	#ifdef WCMFCLIB
	#define WCMFCLIB_CLASS AFX_CLASS_EXPORT
	#else
	#define WCMFCLIB_CLASS AFX_CLASS_IMPORT
	#endif
#else
	#define WCMFCLIB_CLASS
	#ifdef _WCMFC_DEF
	#define WCMFC_DEF EXPORT
	#else
	#define WCMFC_DEF
	#endif
#endif

// CText cell is a class used for CCellGrid.  Use it to put
// left justified text in a cell. It is the base class for other text cells.

class WCMFCLIB_CLASS CTextCell : public CCell
{
 	public:
 	WCMFC_DEF CTextCell(LPCTSTR someText = 0);
 	WCMFC_DEF ~CTextCell();
 	
 	WCMFC_DEF CTextCell(const CTextCell &rhs);
 	const CTextCell& WCMFC_DEF operator=(const CTextCell& rhs);
 	
 	virtual void WCMFC_DEF DrawUnconstrained(CDC* pDC, int x, int y) const;
 	virtual void WCMFC_DEF DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const;
 	
 	LPCTSTR WCMFC_DEF GetText() const;
 	void WCMFC_DEF SetText(LPCTSTR someText);
 	
 	CSize WCMFC_DEF GetExtent(CDC* pDC) const;
 	
 	protected:
 	LPTSTR m_pszText; // 
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
