#if !defined(AFX_DLGUSUARIOS_H__2B1810F3_02D1_11D3_B165_004F4905CBCF__INCLUDED_)
#define AFX_DLGUSUARIOS_H__2B1810F3_02D1_11D3_B165_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUsuarios.h : header file
//

#include "GridListCtrl1.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUsuarios dialog

class CDlgUsuarios : public CExtNCW < CBASEDLG >
{
// Construction
public:    
    int m_matapid;
	int m_modosale;
	CString m_users;
	CString m_destinos;
	CDlgUsuarios(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUsuarios)
	enum { IDD = IDD_USUARIOS };
	CGridListCtrl	m_GridCtrl;
	CString	m_mensaje;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUsuarios)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUsuarios)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnviar();
	afx_msg void OnBorrar();
	afx_msg void OnUsurefresca();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSUARIOS_H__2B1810F3_02D1_11D3_B165_004F4905CBCF__INCLUDED_)
