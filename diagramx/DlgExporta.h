#if !defined(AFX_DLGEXPORTA_H__9750B39E_8DCD_4AE0_A3C5_A99C0A38D1EA__INCLUDED_)
#define AFX_DLGEXPORTA_H__9750B39E_8DCD_4AE0_A3C5_A99C0A38D1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExporta.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExporta dialog

class CDlgExporta : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgExporta(CWnd* pParent = NULL);   // standard constructor

	CDLGGROUP m_group1;
	CDLGGROUP m_group2;

	CDLGRADIO m_cc9;

	CDLGCHECK m_cc1;
	CDLGRADIO m_cc2;
	CDLGRADIO m_cc3;
	CDLGRADIO m_cc4;
	CDLGRADIO m_cc5;
	CDLGRADIO m_cc6;	
	CDLGEDIT m_cc7;
	CDLGEDIT m_cc8;	
	CDLGRADIO m_cc10;
	CDLGRADIO m_cc11;

	CDLGBUTTON m_button1;
	CDLGBUTTON m_button2;
	CDLGBUTTON m_button3;
	CDLGBUTTON m_button4;

// Dialog Data
	//{{AFX_DATA(CDlgExporta)
	enum { IDD = IDD_EXPORTAGRID };
	BOOL	m_borrar;
	int		m_dbf;
	int		m_especificar;
	int		m_excel;
	int		m_noabrir;
	int		m_pdb;
	int		m_predeterminado;
	CString	m_destino;
	CString	m_programa;
	int		m_crystal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExporta)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExporta)
	afx_msg void OnButtondestino();
	afx_msg void OnRadiopdb();
	afx_msg void OnRadiodbf();
	afx_msg void OnRadionoabrir();
	afx_msg void OnRadioexcel();
	afx_msg void OnRadiopreder();
	afx_msg void OnRadioespe();
	afx_msg void OnButtonespe();
	afx_msg void OnRadiocrystal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTA_H__9750B39E_8DCD_4AE0_A3C5_A99C0A38D1EA__INCLUDED_)
