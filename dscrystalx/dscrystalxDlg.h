// dscrystalxDlg.h : header file
//

#if !defined(AFX_DSCRYSTALXDLG_H__C20DDEE5_D777_41FE_B995_592BDC8D86A0__INCLUDED_)
#define AFX_DSCRYSTALXDLG_H__C20DDEE5_D777_41FE_B995_592BDC8D86A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDscrystalxDlg dialog

class CDscrystalxDlg : public CDialog
{
// Construction
public:
	CDscrystalxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDscrystalxDlg)
	enum { IDD = IDD_DSCRYSTALX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDscrystalxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDscrystalxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCRYSTALXDLG_H__C20DDEE5_D777_41FE_B995_592BDC8D86A0__INCLUDED_)
