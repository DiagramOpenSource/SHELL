#if !defined(AFX_DLGCOL_H__C5A2E424_AED2_4089_9027_456A42142246__INCLUDED_)
#define AFX_DLGCOL_H__C5A2E424_AED2_4089_9027_456A42142246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCol.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCol dialog

class CTablaView;

class CDlgCol : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgCol(/*CWnd* pParent = NULL*/CTablaView *pTabla = NULL);   // standard constructor
	~CDlgCol();

	int *m_indices;
	CTablaView *m_pTabla;
// Dialog Data
	//{{AFX_DATA(CDlgCol)
	enum { IDD = IDD_COLUMNAS };
	CCheckListBox m_Lista;
	//}}AFX_DATA
	CDLGBUTTON m_boton1;
	CDLGBUTTON m_boton2;
	CDLGBUTTON m_boton3;
	CDLGBUTTON m_boton4;
	CDLGBUTTON m_boton5;
	CDLGBUTTON m_boton6;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCol)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCol)
	virtual BOOL OnInitDialog();
	afx_msg void OnArriba();
	afx_msg void OnAbajo();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCheck2();
	afx_msg void OnCheck6();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOL_H__C5A2E424_AED2_4089_9027_456A42142246__INCLUDED_)
