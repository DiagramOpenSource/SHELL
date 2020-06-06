#ifndef cbitmapcell_h
#define cbitmapcell_h

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

// CBitmapCell is a class which allows you to put a bitmap into a CCellGrid cell.

class WCMFCLIB_CLASS CBitmapCell : public CCell
{
  	public:
  	WCMFC_DEF CBitmapCell(UINT nIdResource);
  	WCMFC_DEF ~CBitmapCell();
   virtual void WCMFC_DEF DrawUnconstrained(CDC* pDC, int x, int y) const;
   virtual void WCMFC_DEF DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const;
   virtual CSize WCMFC_DEF GetExtent(CDC* pDC) const;
   
   protected:
   
   CBitmap m_Bitmap;
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
