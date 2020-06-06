#if !defined(AFX_COMBONAVEGA_H__8FC875D6_C292_11D3_B2F4_004F4905CBCF__INCLUDED_)
#define AFX_COMBONAVEGA_H__8FC875D6_C292_11D3_B2F4_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboNavega.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboNavega window

#ifdef DSPROFUIS
#define BASECComboNavega CExtComboBox
#else
#define BASECComboNavega CComboBox
#endif
class CComboNavega : public BASECComboNavega
{
// Construction
public:
	CComboNavega();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboNavega)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	int Inserta(CString &s);
	virtual ~CComboNavega();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboNavega)
	afx_msg void OnSelendok();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBONAVEGA_H__8FC875D6_C292_11D3_B2F4_004F4905CBCF__INCLUDED_)
