#if !defined(AFX_SKINSCROLLWND_H__891A5F44_7A55_46DF_8BD7_9C454283CE40__INCLUDED_)
#define AFX_SKINSCROLLWND_H__891A5F44_7A55_46DF_8BD7_9C454283CE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinScrollWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSkinScrollWnd window

class CSkinScrollWnd : public CWnd
{
// Construction
public:
	CSkinScrollWnd();

// Attributes
public:
	int			m_nScrollWid;
	CExtScrollBar	m_sbHorz,m_sbVert;
	CWnd		m_wndLimit;
	BOOL		m_bOp;
	BOOL		m_primera;
	int			m_nAngleType;
// Operations
public:
	WNDPROC		m_funOldProc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinScrollWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SkinWindow( CWnd *pWnd );
	virtual ~CSkinScrollWnd();

	// Generated message map functions
protected:
	afx_msg LRESULT OnDestMove(WPARAM wParam,LPARAM lParam);
	//{{AFX_MSG(CSkinScrollWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CSkinScrollWnd* SkinWndScroll(CWnd *pWnd);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINSCROLLWND_H__891A5F44_7A55_46DF_8BD7_9C454283CE40__INCLUDED_)
