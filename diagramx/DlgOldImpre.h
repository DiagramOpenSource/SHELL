#if !defined(AFX_DLGOLDIMPRE_H__45AAAE57_E2E7_4481_AB0B_010D0404514E__INCLUDED_)
#define AFX_DLGOLDIMPRE_H__45AAAE57_E2E7_4481_AB0B_010D0404514E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOldImpre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOldImpre dialog

class CDlgOldImpre : public CExtNCW < CBASEDLG >
{
// Construction
public:
	BOOL EnableItem(int item,BOOL enable);
	int TestImpre();
	CDlgOldImpre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOldImpre)
	enum { IDD = IDD_DIALOGOOLDIMPRE };
	CListBox	m_lista;
	CString	m_spool;
	CString	m_impre;
    CDLGCOMBO	m_Font;
    int		m_MargenI;
    int		m_MargenS;
    int		m_Lineas;
    BOOL	m_Ajusta;
    int		m_Cols;
    BOOL	m_cabeceraexcel;
    CString	m_posexcel;
    int		m_MargenI2;
	CString	m_imprewindows;
	BOOL	m_precrystal;
	BOOL    m_pdfcrystal;
	BOOL	m_faxauto;
	//}}AFX_DATA


    int	m_crcopias;
	BOOL	m_crdoblehoja;
	BOOL    m_crselimprecristal;
	int	m_crdesdepg;
	int	m_crhastapg;

    int m_nrecs;
    CString m_recs[500];
    CString m_inispool;

    CString m_SFont;  


	CString m_NumeroFax;
	CString m_CorreoFax;
	CString m_CorreoAsunto;

	CDLGBUTTON m_boton1;
	CDLGBUTTON m_boton2;
	CDLGBUTTON m_boton3;

	CDLGEDIT m_edit1;
	CDLGEDIT m_edit2;
	CDLGEDIT m_edit3;
	CDLGEDIT m_edit4;    
	CDLGEDIT m_edit5;
	CDLGEDIT m_edit6;
	CDLGEDIT m_edit7;
	CDLGEDIT m_edit8;
	CDLGEDIT m_edit9;
	CDLGEDIT m_edit10;
	CDLGEDIT m_edit11;
	CDLGEDIT m_edit12;


	CDLGEDIT m_edit13;
	CDLGEDIT m_edit14;
	CDLGEDIT m_edit15;



	CDLGGROUP m_label1;
	CDLGLABEL m_label2;
	CDLGLABEL m_label3;
	CDLGLABEL m_label4;
	CDLGLABEL m_label5;
	CDLGLABEL m_label6;
	CDLGLABEL m_label7;
	CDLGLABEL m_label8;
	CDLGLABEL m_label9;
	CDLGLABEL m_label10;
	CDLGLABEL m_label11;
	CDLGLABEL m_label12;

	CDLGLABEL m_label50;
	CDLGLABEL m_label51;
	CDLGLABEL m_label52;
	CDLGLABEL m_label53;
	CDLGLABEL m_label54;
	CDLGLABEL m_label55;

	CDLGGROUP  m_label56;
	CDLGGROUP  m_label57;
	CDLGGROUP  m_label58;
	CDLGGROUP  m_label59;


	CDLGCHECK m_check1;
	CDLGCHECK m_check2;
	CDLGCHECK m_check3;
	CDLGCHECK m_check4;
	CDLGCHECK m_check5;
	CDLGCHECK m_check6;
	CDLGCHECK m_check7;

	CDLGCHECK m_check8;
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOldImpre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOldImpre)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListaoldimpre();
	afx_msg void OnChangeLaimpre();
	afx_msg void OnKillfocusLaimpre();
	virtual void OnCancel();
    afx_msg void OnImpreajusta();
	afx_msg void Onprecrystal();	
	afx_msg void Onpdfcrystal();	
    afx_msg void OnChangeMargeni();
	afx_msg void OnSelimpre();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOLDIMPRE_H__45AAAE57_E2E7_4481_AB0B_010D0404514E__INCLUDED_)
