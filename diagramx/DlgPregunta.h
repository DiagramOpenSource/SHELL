#if !defined(AFX_DLGPREGUNTA_H__8DAEC643_AB09_11D3_B2C6_004F4905CBCF__INCLUDED_)
#define AFX_DLGPREGUNTA_H__8DAEC643_AB09_11D3_B2C6_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPregunta.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPregunta dialog

class CDlgPregunta : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgPregunta(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPregunta)
	enum { IDD = IDD_ENTRACLAVE };
	CString	m_clave;
	//}}AFX_DATA

    CString	m_laclave;

	BOOL m_conok;
	BOOL m_cualquiera;

	CDLGBUTTON m_boton1;
	CDLGBUTTON m_boton2;

	CString	m_otraclave;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPregunta)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPregunta)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPREGUNTA_H__8DAEC643_AB09_11D3_B2C6_004F4905CBCF__INCLUDED_)
