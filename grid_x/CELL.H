#ifndef ccell_h
#define ccell_h

// CCell is a base class for all other cells which allows you to put a 
// cell into a CCellGrid cell.

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

class WCMFCLIB_CLASS CCell
{
   public:
   WCMFC_DEF CCell();
   virtual WCMFC_DEF ~CCell();
   virtual void WCMFC_DEF DrawUnconstrained(CDC* pDC, int x, int y) const;
   virtual void WCMFC_DEF DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const;
   virtual CSize WCMFC_DEF GetExtent(CDC* pDC) const;
   

   protected:
   CCell(const CCell& rhs); // copy constructor
   const CCell& operator=(const CCell& rhs); // assignment operator
   
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
