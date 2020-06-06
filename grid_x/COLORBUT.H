#ifndef ccolorbutton_h
#define ccolorbutton_h


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

/////////////////////////////////////////////////////////////////////////////
// CColorButton window
//
// CColorButton is an owner drawn button class that is used by the 
// WCColorPage class.  It simply fills its client area with a color.

class WCMFCLIB_CLASS CColorButton : public CButton
{
// Construction
public:                            
	DECLARE_DYNAMIC(CColorButton)
	WCMFC_DEF CColorButton(COLORREF color = 0);

// Attributes
public:

// Operations
public:
	COLORREF WCMFC_DEF GetColor();
	void WCMFC_DEF SetColor(COLORREF color);

// Implementation
public:
	virtual WCMFC_DEF ~CColorButton();

protected:

	virtual void WCMFC_DEF DrawItem(LPDRAWITEMSTRUCT lpDI);
	COLORREF m_Color;
};

/////////////////////////////////////////////////////////////////////////////

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
