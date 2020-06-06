
#if !defined(AFX_LOGINDIALOG_H__B7BDBDC1_892F_11D2_BDF1_004F4905CBCF__INCLUDED_)
#define AFX_LOGINDIALOG_H__B7BDBDC1_892F_11D2_BDF1_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog

class CLoginDialog : public CExtNCW < CBASEDLG >
{
   // Construction
public:
   DECLARE_DYNAMIC(CLoginDialog);
   CLoginDialog(CWnd* pParent = NULL);   // standard constructor
   virtual ~CLoginDialog();
   
   // Dialog Data
   //{{AFX_DATA(CLoginDialog)
   enum { IDD = IDD_LOGIN };
   CDLGBUTTON m_ctrlcambio;
   CDLGBUTTON m_ctrlok;
   CDLGBUTTON m_ctrlcancel;
   CDLGLABEL m_labelusu;
   CDLGLABEL m_labelpas;
   CDLGEDIT m_ctrlusu;
   CDLGEDIT m_ctrlpas;
   CString	m_password;
   CString	m_usuario;
   //}}AFX_DATA

   int m_pregusu;

   CString m_cambio;
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CLoginDialog)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
   
   // Implementation
protected:
   
   // Generated message map functions
   //{{AFX_MSG(CLoginDialog)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	virtual BOOL OnInitDialog();
	afx_msg void OnCambio();
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__B7BDBDC1_892F_11D2_BDF1_004F4905CBCF__INCLUDED_)
