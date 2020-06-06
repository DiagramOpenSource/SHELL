#if !defined(AFX_DSPOPUP_H__6E0389C7_D807_41B5_A528_E31710705CDB__INCLUDED_)
#define AFX_DSPOPUP_H__6E0389C7_D807_41B5_A528_E31710705CDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsPopup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsPopup window
class CDs9Window;

class CDsPopup : public CDSWRB < CWnd >
{
// Construction
public:
	CDsPopup();

// Attributes
public:
	CDs9Window *m_w2;	
	BOOL m_show;	
	int m_desatendido;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsPopup)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPOPUP_H__6E0389C7_D807_41B5_A528_E31710705CDB__INCLUDED_)
