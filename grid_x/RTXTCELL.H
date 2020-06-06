#ifndef crighttextcell_h
#define crighttextcell_h

#include "textcell.h"

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

// CRightText cell is a class used for CCellGrid.  Use it to put
// right justified text in a cell.

class WCMFCLIB_CLASS CRightTextCell : public CTextCell
{
	public:
	WCMFC_DEF CRightTextCell(LPCTSTR text = 0);
	
	// no destructor, copy constructor, or assignment operator defined
	// let compiler provide inlined versions
	
 	virtual void WCMFC_DEF DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const;
};

inline CRightTextCell::CRightTextCell(LPCTSTR text) : CTextCell(text){}

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif // end of sentinel code
