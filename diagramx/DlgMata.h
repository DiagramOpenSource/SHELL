#if !defined(AFX_DLGMATA_H__790F2E59_02FB_11D3_B165_004F4905CBCF__INCLUDED_)
#define AFX_DLGMATA_H__790F2E59_02FB_11D3_B165_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMata.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMata dialog

class CDlgMata : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgMata(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMata)
	enum { IDD = IDD_MATA };
	CString	m_clavemata;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMata)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMata)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATA_H__790F2E59_02FB_11D3_B165_004F4905CBCF__INCLUDED_)
