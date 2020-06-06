#if !defined(AFX_DLGCAMBIAPASSWD_H__BB4AB89A_369C_4BDE_9588_44563B9F9E3B__INCLUDED_)
#define AFX_DLGCAMBIAPASSWD_H__BB4AB89A_369C_4BDE_9588_44563B9F9E3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCambiaPasswd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaPasswd dialog

class CDlgCambiaPasswd : public CExtNCW < CBASEDLG >
{
// Construction
public:
	DECLARE_DYNAMIC(CDlgCambiaPasswd);
	CDlgCambiaPasswd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCambiaPasswd)
	enum { IDD = IDD_NEWPASSWD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCambiaPasswd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCambiaPasswd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMBIAPASSWD_H__BB4AB89A_369C_4BDE_9588_44563B9F9E3B__INCLUDED_)
