#if !defined(AFX_ENVIACORREO_H__6361BC9A_BBD5_4786_811B_4AB39E9A8D6F__INCLUDED_)
#define AFX_ENVIACORREO_H__6361BC9A_BBD5_4786_811B_4AB39E9A8D6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnviaCorreo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnviaCorreo dialog

class CEnviaCorreo : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CEnviaCorreo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnviaCorreo)
	enum { IDD = IDD_ENVIACORREO };
	CString	m_documento;
	CString	m_destino;
	CString	m_origen;
	CString	m_asunto;
	CString	m_nota;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnviaCorreo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnviaCorreo)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVIACORREO_H__6361BC9A_BBD5_4786_811B_4AB39E9A8D6F__INCLUDED_)
