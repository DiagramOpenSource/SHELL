#if !defined(AFX_DLGKACTIVA_H__23E8AA61_B88E_11D3_B2E2_004F4905CBCF__INCLUDED_)
#define AFX_DLGKACTIVA_H__23E8AA61_B88E_11D3_B2E2_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKActiva.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgKActiva dialog

class CDlgKActiva : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgKActiva(char *activacion,char *identifica,char *numero,char *telefonos,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgKActiva)
	enum { IDD = IDD_KACTIVACION };
	CString	m_activacion;
	CString	m_identifica;
	CString	m_numero;
	CString	m_telefonos;
	//}}AFX_DATA

    char *m_pnumero;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKActiva)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgKActiva)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKACTIVA_H__23E8AA61_B88E_11D3_B2E2_004F4905CBCF__INCLUDED_)
