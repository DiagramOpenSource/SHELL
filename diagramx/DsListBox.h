#if !defined(AFX_DSLISTBOX_H__B4F6DB76_C1E0_11D3_B2F2_004F4905CBCF__INCLUDED_)
#define AFX_DSLISTBOX_H__B4F6DB76_C1E0_11D3_B2F2_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsListBox window
class CDsStdCtrl;

class CDsListBox : public CListBox
{
// Construction
public:
	CDsListBox();

// Attributes
public:
    int m_idmensa;
    CDsStdCtrl *m_dsctrl;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsListBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsListBox)
	afx_msg void OnSetfocus();
	afx_msg void OnKillfocus();
	afx_msg void OnDblclk();
	afx_msg int VKeyToItem(UINT nKey, UINT nIndex);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSLISTBOX_H__B4F6DB76_C1E0_11D3_B2F2_004F4905CBCF__INCLUDED_)
