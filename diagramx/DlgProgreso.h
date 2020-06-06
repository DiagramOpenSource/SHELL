#if !defined(AFX_DLGPROGRESO_H__6EF577C2_15C9_4698_9E05_F1616AC7CCBC__INCLUDED_)
#define AFX_DLGPROGRESO_H__6EF577C2_15C9_4698_9E05_F1616AC7CCBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgreso.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgreso dialog

class CDlgProgreso : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgProgreso(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgreso)
	enum { IDD = IDD_PROGRESO };
	CProgressCtrl	m_progreso;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgreso)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgreso)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESO_H__6EF577C2_15C9_4698_9E05_F1616AC7CCBC__INCLUDED_)
