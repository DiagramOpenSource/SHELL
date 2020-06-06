// DlgOrdenar.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "TablaView.h"
#include "DlgOrdenar.h"


// CDlgOrdenar dialog

IMPLEMENT_DYNAMIC(CDlgOrdenar, CBASEDLG)
CDlgOrdenar::CDlgOrdenar(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgOrdenar::IDD, pParent)
{
	m_pTabla = (CTablaView *)pParent;
	m_col = 0;
	m_sel1 = -1;
	m_sel2 = -1;
	m_sel3 = -1;
	m_inver1 = FALSE;
	m_inver2 = FALSE;
	m_inver3 = FALSE;
	DLGCONS;
}

CDlgOrdenar::~CDlgOrdenar()
{
}

void CDlgOrdenar::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORDENARPOR, m_ordenar);
	DDX_Control(pDX, IDC_LUEGOPOR, m_luegopor);
	DDX_Control(pDX, IDC_LUEGOPOR2, m_luegopor2);

	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	DDX_Control(pDX, IDC_STATIC5, m_label5);
	DDX_Control(pDX, IDC_STATIC6, m_label6);

	DDX_Control(pDX, IDC_ASC1, m_boton1);
	DDX_Control(pDX, IDC_DES1, m_boton2);
	DDX_Control(pDX, IDC_ASC2, m_boton3);
	DDX_Control(pDX, IDC_DES2, m_boton4);
	DDX_Control(pDX, IDC_ASC3, m_boton5);
	DDX_Control(pDX, IDC_DES3, m_boton6);

	DDX_Control(pDX, IDOK, m_boton7);
	DDX_Control(pDX, IDCANCEL, m_boton8);

}


BEGIN_MESSAGE_MAP(CDlgOrdenar, CBASEDLG)
END_MESSAGE_MAP()


//// CDlgOrdenar message handlers


BOOL CDlgOrdenar::OnInitDialog()
{
	CBASEDLG::OnInitDialog();

	m_ordenar.ResetContent();
    m_luegopor.ResetContent();
    m_luegopor2.ResetContent();

	((CButton *)GetDlgItem(IDC_ASC1))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_ASC2))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_ASC3))->SetCheck(1);
	
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
		m_ordenar.AddString(s);
		m_luegopor.AddString(s);
		m_luegopor2.AddString(s);		
	}

	/*
    for (i = 0;i < nitem;i++)
    {
        for (j = 0;j < m_pTabla->m_def->m_n_campos_col;j++)
        {
            if (i == m_pTabla->m_def->m_colcampos[j])
            {
                break;
            }
        }
		if (j < m_pTabla->m_def->m_n_campos_col)
			continue;		
		s = m_pTabla->m_def->c_nombre(i,TRUE);
        px = (char *)(LPCSTR)s;
        while (*px)
        {
            oem_a_ansi((unsigned char *)px);
            px++;
        }
		idx = m_ordenar.AddString(s);		
		m_luegopor.AddString(s);
		m_luegopor2.AddString(s);
    } 
	*/

	if (m_col)
		m_ordenar.SetCurSel(m_col-1);


	return TRUE;
}

void CDlgOrdenar::OnOK() 
{
	int i;
	i = m_ordenar.GetCurSel();
	if (i >= 0)
	{
		m_sel1 = m_pTabla->m_def->m_colcampos[i];
		if  (((CButton *)GetDlgItem(IDC_DES1))->GetCheck())
			m_inver1 = TRUE;
	}
	i = m_luegopor.GetCurSel();
	if (i >= 0)
	{
		m_sel2 = m_pTabla->m_def->m_colcampos[i];
		if  (((CButton *)GetDlgItem(IDC_DES2))->GetCheck())
			m_inver2 = TRUE;
	}
	i = m_luegopor2.GetCurSel();
	if (i >= 0)
	{
		m_sel3 = m_pTabla->m_def->m_colcampos[i];
		if  (((CButton *)GetDlgItem(IDC_DES3))->GetCheck())
			m_inver3 = TRUE;
	}
	CBASEDLG::OnOK();
}