// DlgExporta.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgExporta.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExporta dialog


CDlgExporta::CDlgExporta(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgExporta::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExporta)
	m_borrar = FALSE;
	m_dbf = 0;
	m_especificar = -1;
	m_excel = -1;	
	m_noabrir = 0;
	m_pdb = -1;
	m_predeterminado = -1;
	m_destino = _T("");
	m_programa = _T("");
	m_crystal = -1;
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgExporta::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_group1);
	DDX_Control(pDX, IDC_STATIC2, m_group2);

	DDX_Control(pDX, IDOK, m_button1);
	DDX_Control(pDX, IDCANCEL, m_button2);
	DDX_Control(pDX, IDC_BUTTONDESTINO, m_button3);
	DDX_Control(pDX, IDC_BUTTONESPE, m_button4);
	
	DDX_Control(pDX, IDC_CHECKBORRAR, m_cc1);
	DDX_Control(pDX, IDC_RADIODBF, m_cc2);
	DDX_Control(pDX, IDC_RADIOESPE, m_cc3);
	DDX_Control(pDX, IDC_RADIOEXCEL, m_cc4);
	DDX_Control(pDX, IDC_RADIONOABRIR, m_cc5);
	DDX_Control(pDX, IDC_RADIOPDB, m_cc6);
	DDX_Control(pDX, IDC_RADIOPREDER, m_cc9);	
	DDX_Control(pDX, IDC_RADIOCRYSTAL2, m_cc11);
	DDX_Control(pDX, IDC_DESTINO, m_cc7);
	DDX_Control(pDX, IDC_PROGRAMA, m_cc8);
	

	//{{AFX_DATA_MAP(CDlgExporta)
	DDX_Check(pDX, IDC_CHECKBORRAR, m_borrar);	
	DDX_Radio(pDX, IDC_RADIODBF, m_dbf);
	DDX_Radio(pDX, IDC_RADIOESPE, m_especificar);
	DDX_Radio(pDX, IDC_RADIOEXCEL, m_excel);
	DDX_Radio(pDX, IDC_RADIONOABRIR, m_noabrir);
	DDX_Radio(pDX, IDC_RADIOPDB, m_pdb);
	DDX_Radio(pDX, IDC_RADIOPREDER, m_predeterminado);
	DDX_Radio(pDX, IDC_RADIOCRYSTAL2, m_crystal);
	DDX_Text(pDX, IDC_DESTINO, m_destino);
	DDX_Text(pDX, IDC_PROGRAMA, m_programa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExporta, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgExporta)
	ON_BN_CLICKED(IDC_BUTTONDESTINO, OnButtondestino)
	ON_BN_CLICKED(IDC_RADIOPDB, OnRadiopdb)
	ON_BN_CLICKED(IDC_RADIODBF, OnRadiodbf)
	ON_BN_CLICKED(IDC_RADIONOABRIR, OnRadionoabrir)
	ON_BN_CLICKED(IDC_RADIOEXCEL, OnRadioexcel)
	ON_BN_CLICKED(IDC_RADIOPREDER, OnRadiopreder)
	ON_BN_CLICKED(IDC_RADIOESPE, OnRadioespe)
	ON_BN_CLICKED(IDC_BUTTONESPE, OnButtonespe)
	ON_BN_CLICKED(IDC_RADIOCRYSTAL2, OnRadiocrystal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExporta message handlers
/*
*/
void CDlgExporta::OnButtondestino() 
{
	UpdateData(TRUE);
	CString ext,ext1;
	char tmp2[512];
	if (!m_dbf)
	{
		ext = ".dbf";
		ext1 = "*.dbf";
	}
	else
	{
		ext = ".pdb";
		ext1 = "*.pdb";
	}
	strcpy(tmp2,m_destino);
	strcat(tmp2,ext);
	
	CFileDialog dlgFile(FALSE,ext,tmp2,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,ext1,theApp.m_pMainWnd);
	dlgFile.m_ofn.lpstrFile = tmp2;
	if (dlgFile.DoModal() == IDOK)
	{
		  m_destino = tmp2;
		  m_destino.Replace(ext,"");
	}
	UpdateData(FALSE);
}

void CDlgExporta::OnRadiopdb() 
{
	UpdateData(TRUE);
	m_dbf = -1;
	m_pdb = 0;
	UpdateData(FALSE);
}

void CDlgExporta::OnRadiodbf() 
{
	UpdateData(TRUE);
	m_dbf = 0;
	m_pdb = -1;
	UpdateData(FALSE);	
}

void CDlgExporta::OnRadionoabrir() 
{	
	UpdateData(TRUE);
	m_noabrir = 0;	
	m_especificar = -1;
	m_excel = -1;	
	m_crystal = -1;
	m_predeterminado = -1;	
	GetDlgItem(IDC_PROGRAMA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONESPE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_CHECKBORRAR)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

void CDlgExporta::OnRadiocrystal() 
{
	UpdateData(TRUE);
	m_noabrir = -1;	
	m_especificar = -1;
	m_excel = -1;	
	m_crystal = 0;
	m_predeterminado = -1;	
	GetDlgItem(IDC_PROGRAMA)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONESPE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_CHECKBORRAR)->EnableWindow(TRUE);	
	UpdateData(FALSE);
}

void CDlgExporta::OnRadioexcel() 
{
	UpdateData(TRUE);
	m_noabrir = -1;	
	m_especificar = -1;
	m_excel = 0;	
	m_crystal = -1;
	m_predeterminado = -1;	
	GetDlgItem(IDC_PROGRAMA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONESPE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_CHECKBORRAR)->EnableWindow(TRUE);	
	UpdateData(FALSE);
}

void CDlgExporta::OnRadiopreder() 
{
	UpdateData(TRUE);
	m_noabrir = -1;	
	m_especificar = -1;
	m_excel = -1;	
	m_crystal = -1;
	m_predeterminado = 0;	
	GetDlgItem(IDC_PROGRAMA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONESPE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_CHECKBORRAR)->EnableWindow(TRUE);	
	UpdateData(FALSE);
}

void CDlgExporta::OnRadioespe() 
{
	UpdateData(TRUE);
	m_noabrir = -1;	
	m_especificar = 0;
	m_excel = -1;	
	m_crystal = -1;
	m_predeterminado = -1;	
	GetDlgItem(IDC_PROGRAMA)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONESPE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_CHECKBORRAR)->EnableWindow(TRUE);	
	UpdateData(FALSE);
}

void CDlgExporta::OnButtonespe() 
{
	CString extension;	
	UpdateData(TRUE);
	
	if (!m_crystal)
	{
		extension = "rpt";
	}
	else
	{
		extension = "exe";
	}
	char tmp2[512];	
	char tt[51];

	strcpy(tt,"*.");
	strcat(tt,extension);

	if (m_programa.IsEmpty())
	{
		strcpy(tmp2,tt);
	}
	else
	{
		strcpy(tmp2,m_programa);
	}
	CFileDialog dlgFile(TRUE,extension,tmp2,OFN_HIDEREADONLY|OFN_EXPLORER,tt,theApp.m_pMainWnd);
	dlgFile.m_ofn.lpstrFile = tmp2;
	if (dlgFile.DoModal() == IDOK)
	{
		m_programa = tmp2;		  
	}
	UpdateData(FALSE);	
}

