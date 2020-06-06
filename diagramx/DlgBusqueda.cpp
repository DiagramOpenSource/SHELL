// DlgBusqueda.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "TablaView.h"
#include "DlgBusqueda.h"


// CDlgBusqueda dialog

IMPLEMENT_DYNAMIC(CDlgBusqueda, CBASEDLG)
CDlgBusqueda::CDlgBusqueda(CWnd* pParent /*=NULL*/)
	: CBASEDLG (CDlgBusqueda::IDD, pParent)
{
	m_pTabla = (CTablaView *)pParent;
	m_col = 0;
	m_sel = -1;

    m_limitei = "";
	m_limites = "";
	m_valor = "";
    m_mayus   = 0;
    m_inversa = 0;
	m_tipovalor = 0;

	DLGCONS;
}

CDlgBusqueda::~CDlgBusqueda()
{
}

void CDlgBusqueda::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUSCAREN, m_buscarpor);
	DDX_Control(pDX, IDC_EDIT2, m_edit1);
	DDX_Control(pDX, IDC_EDIT3, m_edit2);
	DDX_Control(pDX, IDC_EDIT4, m_edit3);
	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	DDX_Control(pDX, IDC_STATIC5, m_label5);

	DDX_Control(pDX, IDC_RADIOEXACTO, m_boton1);
	DDX_Control(pDX, IDC_RADIOPARCIAL, m_boton2);
	DDX_Control(pDX, IDC_RADIOCONTENIDO, m_boton3);
	DDX_Control(pDX, IDC_CHECK3, m_boton4);
	DDX_Control(pDX, IDC_CHECK4, m_boton5);

	DDX_Control(pDX, IDOK, m_boton6);
	DDX_Control(pDX, IDCANCEL, m_boton7);


	
}


BEGIN_MESSAGE_MAP(CDlgBusqueda, CBASEDLG)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeDesde)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeHasta)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeValor)
	ON_BN_CLICKED(IDC_RADIOEXACTO, OnRadioexacto)
	ON_BN_CLICKED(IDC_RADIOPARCIAL, OnRadioparcial)
	ON_BN_CLICKED(IDC_RADIOCONTENIDO, OnRadiocontenido)
END_MESSAGE_MAP()


// CDlgBusqueda message handlers

BOOL CDlgBusqueda::OnInitDialog()
{
	CBASEDLG::OnInitDialog();

	m_buscarpor.ResetContent();
	
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



void CDlgBusqueda::OnChangeDesde() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT4,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT4,"");
   }
}

void CDlgBusqueda::OnChangeHasta() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT4,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT4,"");
   }
}

void CDlgBusqueda::OnChangeValor() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT2,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT2,"");
   }
   GetDlgItemText(IDC_EDIT3,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT3,"");
   }
   CButton *pButton;
   GetDlgItemText(IDC_EDIT4,s);
   if (s.IsEmpty())
   {	
		pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
		pButton->SetCheck(0);
		pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
		pButton->SetCheck(0);
		pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
		pButton->SetCheck(0);

		pButton = (CButton *)GetDlgItem(IDC_CHECK3);
		pButton->EnableWindow(FALSE);
		pButton->SetCheck(0);
   }
   else
   {
	   BOOL hay = FALSE;
	   pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
	   if (pButton->GetCheck())
		   hay = TRUE;
	   else
	   {	
		   pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
		   if (pButton->GetCheck())
			   hay = TRUE;
		   else
		   {
			   pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
			   if (pButton->GetCheck())
				   hay = TRUE;
		   }
	   }
	   if (!hay)
	   {
		   pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
		   pButton->SetCheck(0);

		   pButton = (CButton *)GetDlgItem(IDC_CHECK3);
		   pButton->EnableWindow(FALSE);
		   pButton->SetCheck(0);
	   }
   }
}

void CDlgBusqueda::OnRadioexacto()
{
	CButton *pButton;	
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(1);
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(0);	

	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(FALSE);
	pButton->SetCheck(0);
}

void CDlgBusqueda::OnRadioparcial()
{
	CButton *pButton;
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(1);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(0);		

	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(TRUE);
	pButton->SetCheck(0);
}

void CDlgBusqueda::OnRadiocontenido()
{
	CButton *pButton;
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(1);


	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(TRUE);
}

void CDlgBusqueda::OnOK() 
{
	int i;
	i = m_buscarpor.GetCurSel();
	if (i >= 0)
	{
		m_sel = m_pTabla->m_def->m_colcampos[i];

		GetDlgItemText(IDC_EDIT2,m_limitei);
		GetDlgItemText(IDC_EDIT3,m_limites);
		GetDlgItemText(IDC_EDIT4,m_valor);
        m_mayus   = IsDlgButtonChecked(IDC_CHECK3);
        m_inversa = IsDlgButtonChecked(IDC_CHECK4);

        int ss = 0;
        if (m_limitei.IsEmpty() && m_limites.IsEmpty())
        {
            CButton *pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            if (pButton->GetCheck())
            {
                ss = 1;
            }
            else 
            {
                pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
                if (pButton->GetCheck())
                {
                    ss = 2;
                }
                else
                {
                    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
                    if (pButton->GetCheck())
                    {
                        ss = 3;
                    }
					else
					{
						ss = 4;
					}
                }
            }
        }
        else
        {
            m_valor = "";
        }
        m_tipovalor = ss;
	}


	CBASEDLG::OnOK();
}