#if !defined(AFX_DSTAB9_H__3BA23284_0B2F_490B_90C3_06798FC49524__INCLUDED_)
#define AFX_DSTAB9_H__3BA23284_0B2F_490B_90C3_06798FC49524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsTab9.h : header file
//
#include "DsTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDsTab9 window
//#ifdef DSPROFUIS
//#define BASEDsTab9 CDsTabCtrl
//#else
//#endif
class CDsTab9 : public BASEDsTab9
{
// Construction
public:
	CDsTab9();

// Attributes
public:
	int m_cursel;

#ifdef DSPROFUIS9
	CWnd *m_truco;
	int m_nosel;
#endif

// Operations
public:
	void GetTabWndRect(CRect &rect);

    void CenterWindow( CWnd* pAlternateOwner = NULL );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsTab9)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:	
	void PintaOpcion(int old_op,int op);
	virtual ~CDsTab9();	

#ifdef DSPROFUIS9
	int SetCurSel(int nItem );
	void DeleteAllItems( );
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsTab9)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTAB9_H__3BA23284_0B2F_490B_90C3_06798FC49524__INCLUDED_)
