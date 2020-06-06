#if !defined(AFX_DLGBROWSE_H__E8E1FCB7_6F05_4329_B591_60D20DA16972__INCLUDED_)
#define AFX_DLGBROWSE_H__E8E1FCB7_6F05_4329_B591_60D20DA16972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBrowse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowse dialog

class CDlgBrowse : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgBrowse(CWnd* pParent = NULL);   // standard constructor

	BOOL BorraSubOpciones(HTREEITEM item);
	BOOL m_solofiles;
	BOOL m_solodirs;
	CString m_base;
	CString m_seleccion;
	CImageList m_ImageList;

// Dialog Data
	//{{AFX_DATA(CDlgBrowse)
	enum { IDD = IDD_BROWSE };
	CTreeCtrl	m_sele;
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBrowse)
	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBrowse)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandedSele(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingSele(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSele(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBROWSE_H__E8E1FCB7_6F05_4329_B591_60D20DA16972__INCLUDED_)
