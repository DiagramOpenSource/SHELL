#if !defined(AFX_LISTEDIT_H__66120658_743C_44CA_959D_B7C664D40BD0__INCLUDED_)
#define AFX_LISTEDIT_H__66120658_743C_44CA_959D_B7C664D40BD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListEdit window

class CListEdit : public CRichEditCtrl
{
// Construction
public:
	CListEdit();

// Attributes
public:

    CFont *m_Fuente_s;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMainLstW window

class CMainLstW : public CWnd
{
// Construction
public:
	CMainLstW();

// Attributes
public:
    CListEdit *m_pMEdt;
    long m_inicio;
    long m_seguido;
    BOOL m_comenzado;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainLstW)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainLstW();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainLstW)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTEDIT_H__66120658_743C_44CA_959D_B7C664D40BD0__INCLUDED_)
