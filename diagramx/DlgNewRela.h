#if !defined(AFX_DLGNEWRELA_H__4603D8D9_31C2_484D_95E9_402E1B200971__INCLUDED_)
#define AFX_DLGNEWRELA_H__4603D8D9_31C2_484D_95E9_402E1B200971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewRela.h : header file
//
#include "DlgGridFiltro.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRela dialog

class CDlgNewRela : public CExtNCW < CBASEDLG >
{
// Construction
public:
	CDlgNewRela(CDlgGridFiltro* pParent = NULL);   // standard constructor
	~CDlgNewRela();

	CDlgGridFiltro *m_filtro;
    CFiltroDef *m_fichero;

	CFiltroDef *m_relacionado;

	int m_checko;
	int m_checkd;

	void InformaRela();


// Dialog Data
	//{{AFX_DATA(CDlgNewRela)
	enum { IDD = IDD_NUEVARELA };
	CComboBox	m_ficheros;
	CFiltroCListBox	m_camposo;
	CFiltroCListBox	m_camposd;
	CString	m_rela;
	//}}AFX_DATA	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewRela)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDLGBUTTON m_button1;
	CDLGBUTTON m_button2;


	afx_msg void OnCheckOrigen();
	afx_msg void OnCheckDestino();

	// Generated message map functions
	//{{AFX_MSG(CDlgNewRela)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCamposo();
	afx_msg void OnSelchangeCamposd();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeFicheros();
	afx_msg void OnSelchangeFicheros();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWRELA_H__4603D8D9_31C2_484D_95E9_402E1B200971__INCLUDED_)
