#if !defined(AFX_DSTEXTEDIT_H__9EEF63B3_C4E3_11D3_B2F6_004F4905CBCF__INCLUDED_)
#define AFX_DSTEXTEDIT_H__9EEF63B3_C4E3_11D3_B2F6_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsTextEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsTextEdit window
#ifdef DSPROFUIS
#define BASECDsTEdit CEdit
#else
#define BASECDsTEdit CEdit
#endif

class CDsStdCtrl;

class CDsTextEdit : public BASECDsTEdit
{
// Construction
public:
	CDsTextEdit();

// Attributes
public:

    CDsStdCtrl *m_dsctrl;
	CFont *m_Font;
	int m_maximolinea;
	int m_lineas;
	int m_topelineas;
	CString m_guarda;
	DWORD m_gsel;

// Operations
public:

	void SetWindowText (LPCTSTR Text,BOOL tipohint = FALSE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsTextEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL TeclaStd(int tecla);
	virtual ~CDsTextEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsTextEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnUpdate();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	//}}AFX_MSG
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,CPoint point);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTEXTEDIT_H__9EEF63B3_C4E3_11D3_B2F6_004F4905CBCF__INCLUDED_)
