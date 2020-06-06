// DlgOperaciones.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "TablaView.h"
#include "DlgOperaciones.h"


// CDlgOperaciones dialog

IMPLEMENT_DYNAMIC(CDlgOperaciones, CBASEDLG)
CDlgOperaciones::CDlgOperaciones(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgOperaciones::IDD, pParent)
{
	m_opera = 0;
	m_pTabla = (CTablaView *)pParent;
	m_col = 0;
	m_sel = -1;

    DLGCONS;
}

CDlgOperaciones::~CDlgOperaciones()
{
}

void CDlgOperaciones::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_buscarpor);

	DDX_Control(pDX, IDC_MAXIMO, m_boton1);
	DDX_Control(pDX, IDC_MINIMO, m_boton2);
	DDX_Control(pDX, IDC_SUMA, m_boton3);

	DDX_Control(pDX, IDOK, m_boton4);
	DDX_Control(pDX, IDCANCEL, m_boton5);

	DDX_Control(pDX, IDC_STATIC1, m_label1);


}


BEGIN_MESSAGE_MAP(CDlgOperaciones, CBASEDLG)
END_MESSAGE_MAP()


// CDlgOperaciones message handlers
BOOL CDlgOperaciones::OnInitDialog()
{
	CBASEDLG::OnInitDialog();

	m_buscarpor.ResetContent();

	((CButton *)GetDlgItem(IDC_SUMA))->SetCheck(1);
	
	int nitem = m_pTabla->m_def->m_info->n_campos;
	CString s;
	char *px;
	int i,j;

	for (j = 0;j < m_pTabla->m_def->m_n_campos_col;j++)
	{
		i = m_pTabla->m_def->m_colcampos[j];
		s = m_pTabla->m_def->c_nombre(i,TRUE);
        px = (char *)(LPCSTR)s;
        while (*px)
        {
            oem_a_ansi((unsigned char *)px);
            px++;
        }			
		m_buscarpor.AddString(s);		
	}


	if (m_col)
		m_buscarpor.SetCurSel(m_col-1);


	return TRUE;
}

void CDlgOperaciones::OnOK() 
{
	int i;
	i = m_buscarpor.GetCurSel();
	if (i >= 0)
	{
		m_sel = m_pTabla->m_def->m_colcampos[i];

		if (IsDlgButtonChecked(IDC_SUMA))
			m_opera = 1;
		else if (IsDlgButtonChecked(IDC_MAXIMO))
			m_opera = 2;
		else if (IsDlgButtonChecked(IDC_MINIMO))
			m_opera = 3;
	}


	CBASEDLG::OnOK();
}