#if !defined(AFX_DLGCPASSWD_H__4F512BEE_1574_4C69_8F37_859AA67AC0A7__INCLUDED_)
#define AFX_DLGCPASSWD_H__4F512BEE_1574_4C69_8F37_859AA67AC0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPasswd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPasswd dialog

class CDlgCPasswd : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgCPasswd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCPasswd)
	enum { IDD = IDD_NEWPASSWD };
	CString	m_password;
	CString	m_nuevo;
	CString	m_confirma;
	CString	m_usuario;
	//}}AFX_DATA
    CDLGEDIT m_edit1;
    CDLGEDIT m_edit2;
    CDLGEDIT m_edit3;
    CDLGEDIT m_edit4;
    CDLGBUTTON m_ctrlok;
    CDLGBUTTON m_ctrlcancel;
	CDLGLABEL m_label1;
	CDLGLABEL m_label2;
	CDLGLABEL m_label3;
	CDLGLABEL m_label4;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPasswd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPasswd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPASSWD_H__4F512BEE_1574_4C69_8F37_859AA67AC0A7__INCLUDED_)
