#if !defined(AFX_DSGRIDBUTTON_H__69911D78_B15C_11D3_B2D3_004F4905CBCF__INCLUDED_)
#define AFX_DSGRIDBUTTON_H__69911D78_B15C_11D3_B2D3_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsGridButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsGridButton window

class CTablaView;

class CDsGridButton : public CButton
{
// Construction
public:
	CDsGridButton(CTablaView *tabla);

// Attributes
public:
    CBitmap m_bmp;
    CTablaView *m_tabla;
    int m_tipo;
    int m_nume;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsGridButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsGridButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsGridButton)
	afx_msg void OnClicked();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSGRIDBUTTON_H__69911D78_B15C_11D3_B2D3_004F4905CBCF__INCLUDED_)
