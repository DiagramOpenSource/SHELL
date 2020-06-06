#ifndef colorpag_h
#define colorpag_h

/////////////////////////////////////////////////////////////////////////////
// WCColorPage dialog

// WCColorPage is a property sheet that allows the selection of a color.
// WCMFCRES.RC must be included into the project resource file for any
// project that uses this class.

#include "colorbut.h"

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

class WCMFCLIB_CLASS WCColorPage : public CPropertyPage
{
// Construction
public:
	WCMFC_DEF WCColorPage();	// standard constructor
	
	COLORREF m_Color; // the whole reason for this!

	static COLORREF m_ColorArray[];

// Dialog Data
	//{{AFX_DATA(WCColorPage)
	CScrollBar	m_BlueBar;
	CScrollBar	m_GreenBar;
	CScrollBar	m_RedBar;
	CColorButton	m_SampleBtn;
	CColorButton	m_Btn9;
	CColorButton	m_Btn8;
	CColorButton	m_Btn7;
	CColorButton	m_Btn6;
	CColorButton	m_Btn5;
	CColorButton	m_Btn4;
	CColorButton	m_Btn3;
	CColorButton	m_Btn2;
	CColorButton	m_Btn16;
	CColorButton	m_Btn15;
	CColorButton	m_Btn14;
	CColorButton	m_Btn13;
	CColorButton	m_Btn12;
	CColorButton	m_Btn11;
	CColorButton	m_Btn10;
	CColorButton	m_Btn1;
	//}}AFX_DATA

// Implementation
protected:
	virtual void WCMFC_DEF DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(WCColorPage)
	virtual BOOL WCMFC_DEF OnInitDialog();
	afx_msg void WCMFC_DEF OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg void WCMFC_DEF SomeButtonClicked();
	DECLARE_MESSAGE_MAP()
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
