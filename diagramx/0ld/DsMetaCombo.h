#if !defined(AFX_DSMETACOMBO_H__F0DD2B36_A3D8_11D3_B2BA_004F4905CBCF__INCLUDED_)
#define AFX_DSMETACOMBO_H__F0DD2B36_A3D8_11D3_B2BA_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsMetaCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsMetaCombo window

#include "DsCombo.h"

class CDsEdit;

class CDsMetaCombo : public CDsCombo
{
// Construction
public:
	CDsMetaCombo();

// Attributes
public:

// Operations
public:

    CDsEdit *m_dsedit;

	BOOL m_comborel;
	BOOL m_combactiva;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsMetaCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void EsRelacion(BOOL relacion = TRUE);
	void Muere();
	virtual ~CDsMetaCombo();

	virtual BOOL TeclaStd(int tecla);

protected:
	virtual BOOL CreateEdit ();
	// Generated message map functions
protected:
	afx_msg BOOL CDsComboDropWndToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	//{{AFX_MSG(CDsMetaCombo)
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchange();
	afx_msg void OnEditchange();
	afx_msg void OnDropdown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSMETACOMBO_H__F0DD2B36_A3D8_11D3_B2BA_004F4905CBCF__INCLUDED_)
