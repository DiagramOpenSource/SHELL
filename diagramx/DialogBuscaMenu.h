#if !defined(AFX_DIALOGBUSCAMENU_H__781D4477_A6AF_41B4_A0C5_B3BD933060DE__INCLUDED_)
#define AFX_DIALOGBUSCAMENU_H__781D4477_A6AF_41B4_A0C5_B3BD933060DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBuscaMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBuscaMenu dialog

class CDialogBuscaMenu : public CBASEDLG
{
// Construction
public:
	CDialogBuscaMenu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBuscaMenu)
	enum { IDD = IDD_BUSMENU };
	CString	m_texto;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBuscaMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBuscaMenu)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBUSCAMENU_H__781D4477_A6AF_41B4_A0C5_B3BD933060DE__INCLUDED_)
