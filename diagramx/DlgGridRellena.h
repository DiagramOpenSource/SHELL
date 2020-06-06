#if !defined(AFX_DLGGRIDRELLENA_H__C50ABCC6_82CD_11D3_B272_004F4905CBCF__INCLUDED_)
#define AFX_DLGGRIDRELLENA_H__C50ABCC6_82CD_11D3_B272_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGridRellena.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGridRellena dialog

class CDlgGridRellena : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgGridRellena(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGridRellena)
	enum { IDD = IDD_GRIDRELLENA };
	CAnimateCtrl	m_animacion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGridRellena)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGridRellena)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRIDRELLENA_H__C50ABCC6_82CD_11D3_B272_004F4905CBCF__INCLUDED_)
