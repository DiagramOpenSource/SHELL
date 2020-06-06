#if !defined(AFX_DLGINDEXADO_H__E6E82CA3_B910_11D3_B2E4_004F4905CBCF__INCLUDED_)
#define AFX_DLGINDEXADO_H__E6E82CA3_B910_11D3_B2E4_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIndexado.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIndexado dialog

class CDlgIndexado : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgIndexado(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIndexado)
	enum { IDD = IDD_INDEXADO };
	CString	m_panel1;
	CString	m_panel2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIndexado)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIndexado)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINDEXADO_H__E6E82CA3_B910_11D3_B2E4_004F4905CBCF__INCLUDED_)
