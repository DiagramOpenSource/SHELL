#if !defined(AFX_DLGERRORFICHERO_H__969EEB76_B7BC_11D3_B2E0_004F4905CBCF__INCLUDED_)
#define AFX_DLGERRORFICHERO_H__969EEB76_B7BC_11D3_B2E0_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgErrorFichero.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgErrorFichero dialog

class CDlgErrorFichero : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgErrorFichero(CWnd* pParent = NULL);   // standard constructor

    BOOL m_muerto;

// Dialog Data
	//{{AFX_DATA(CDlgErrorFichero)
	enum { IDD = IDD_ERRORFICHERO };
	CString	m_edit1;
	CString	m_edit2;
	CString	m_edit3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgErrorFichero)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgErrorFichero)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGERRORFICHERO_H__969EEB76_B7BC_11D3_B2E0_004F4905CBCF__INCLUDED_)
