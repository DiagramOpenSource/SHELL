#if !defined(AFX_DSCTRLSCROLL_H__F50E37F4_BC89_11D3_B2E7_004F4905CBCF__INCLUDED_)
#define AFX_DSCTRLSCROLL_H__F50E37F4_BC89_11D3_B2E7_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsCtrlScroll.h : header file
//

class CDsStdCtrl;
/////////////////////////////////////////////////////////////////////////////
// CDsCtrlScroll window

class CDsCtrlScroll : public CExtScrollBar
{
// Construction
public:
	CDsCtrlScroll();

// Attributes
public:
    CDsStdCtrl *m_dsctrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsCtrlScroll)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsCtrlScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsCtrlScroll)
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCTRLSCROLL_H__F50E37F4_BC89_11D3_B2E7_004F4905CBCF__INCLUDED_)
