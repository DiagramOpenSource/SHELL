#if !defined(AFX_DLGCAMBIACLAVE_H__F7459AD4_E3A2_48FD_A89A_38C949A739FF__INCLUDED_)
#define AFX_DLGCAMBIACLAVE_H__F7459AD4_E3A2_48FD_A89A_38C949A739FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCambiaClave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaClave dialog

class CDlgCambiaClave : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgCambiaClave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCambiaClave)
	enum { IDD = IDD_CAMBIACLAVE };
	CString	m_clave;
	CString	m_cclave;
	//}}AFX_DATA

	BOOL m_conok;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCambiaClave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCambiaClave)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMBIACLAVE_H__F7459AD4_E3A2_48FD_A89A_38C949A739FF__INCLUDED_)
