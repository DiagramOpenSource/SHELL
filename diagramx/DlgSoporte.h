#if !defined(AFX_DLGSOPORTE_H__B677B7E5_39A0_4D59_8FFD_4D8C4BE09D4D__INCLUDED_)
#define AFX_DLGSOPORTE_H__B677B7E5_39A0_4D59_8FFD_4D8C4BE09D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSoporte.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSoporte dialog

class CDlgSoporte : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgSoporte(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSoporte)
	enum { IDD = IDD_SOPORTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSoporte)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSoporte)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOPORTE_H__B677B7E5_39A0_4D59_8FFD_4D8C4BE09D4D__INCLUDED_)
