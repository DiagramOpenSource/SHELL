// Ds9Window.h: interface for the CDs9Window class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DS9WINDOW_H__733D9378_758A_4063_AD3D_81BCCDEFEB4E__INCLUDED_)
#define AFX_DS9WINDOW_H__733D9378_758A_4063_AD3D_81BCCDEFEB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DsStdDlg.h"

class CDsTab9;

class CDs9Window : public CDsStdDlg
{
public:
	CDs9Window();
    DECLARE_DYNCREATE(CDs9Window)
	virtual ~CDs9Window();
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDs9Window)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:		
	CDsTab9 *m_tab9;
	HWND m_focoprevio;
	CDsStdCtrl m_yo;
	virtual BOOL Create(void *datos,CDsxView *pView, int& stdid);
protected:
	afx_msg BOOL Ds9DlgToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	//{{AFX_MSG(CDs9Window)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DS9WINDOW_H__733D9378_758A_4063_AD3D_81BCCDEFEB4E__INCLUDED_)
