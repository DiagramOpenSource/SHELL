// DlgCol.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgCol.h"
#include "TablaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCol dialog


CDlgCol::CDlgCol(CTablaView *pTabla/*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgCol::IDD, pTabla)
{
	//{{AFX_DATA_INIT(CDlgCol)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTabla = pTabla;
	m_indices = new int[m_pTabla->m_def->m_info->n_campos];
	DLGCONS;
}

CDlgCol::~CDlgCol()
{
	delete m_indices;
}

void CDlgCol::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCol)
	DDX_Control(pDX, IDC_LIST1, m_Lista);

	DDX_Control(pDX, IDC_ARRIBA, m_boton1);
	DDX_Control(pDX, IDC_ABAJO, m_boton2);
	DDX_Control(pDX, IDC_BUTTON1, m_boton3);
	DDX_Control(pDX, IDC_BUTTON2, m_boton4);
	DDX_Control(pDX, IDOK, m_boton5);
	DDX_Control(pDX, IDCANCEL, m_boton6);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCol, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCol)
	ON_BN_CLICKED(IDC_ARRIBA, OnArriba)
	ON_BN_CLICKED(IDC_ABAJO, OnAbajo)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCol message handlers

BOOL CDlgCol::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
    int idx,j;
    m_Lista.ResetContent( );

	int nitem = m_pTabla->m_def->m_info->n_campos;
	CString s;
	char *px;
	int i;

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
		idx = m_Lista.AddString(s);
		ASSERT(idx == j);
		m_Lista.SetCheck(idx,1);
		m_indices[idx] = i;
	}

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
		idx = m_Lista.AddString(s);		
		m_Lista.SetCheck(idx,0);
		m_indices[idx] = i;
    }    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCol::OnButton1() 
{	
	m_Lista.SelItemRange(TRUE, 0, m_Lista.GetCount());	
}

void CDlgCol::OnButton2() 
{
	m_Lista.SelItemRange(FALSE, 0, m_Lista.GetCount());	
}


void CDlgCol::OnArriba()
{
	CString s;
	int check;


	for (int i = 1;i < m_Lista.GetCount();i++)
	{
		if (m_Lista.GetSel(i) && !m_Lista.GetSel(i-1))
		{
			check = m_Lista.GetCheck(i);
			m_Lista.GetText(i,s);
			m_Lista.DeleteString(i);
			m_Lista.InsertString(i-1,s);
			m_Lista.SetCheck(i-1,check);			
			m_Lista.SetSel(i-1,TRUE);
			check = m_indices[i-1];
			m_indices[i-1] = m_indices[i];
			m_indices[i] = check;
		}
	}
}

void CDlgCol::OnAbajo()
{
	CString s;
	int check;

	for (int i = m_Lista.GetCount()-2;i >= 0;i--)
	{
		if (m_Lista.GetSel(i) && !m_Lista.GetSel(i+1))
		{
			check = m_Lista.GetCheck(i);
			m_Lista.GetText(i,s);
			m_Lista.DeleteString(i);
			m_Lista.InsertString(i+1,s);
			m_Lista.SetCheck(i+1,check);
			m_Lista.SetSel(i+1,TRUE);
			check = m_indices[i+1];
			m_indices[i+1] = m_indices[i];
			m_indices[i] = check;
		}
	}
}

void CDlgCol::OnCheck2() 
{
	/*
    m_soloseleccionados = !m_soloseleccionados;
    UpdateData(TRUE);
    RecogeLista();
    OnSelchangeLista();
    UpdateData(FALSE);
    RellenaLista();	
	*/
}

void CDlgCol::OnCheck6() 
{
	/*
    m_alfabetico = !m_alfabetico;
    UpdateData(TRUE);
    RecogeLista();
    OnSelchangeLista();
    UpdateData(FALSE);
    RellenaLista();	
	*/
}

void CDlgCol::OnOK() 
{
	// TODO: Add extra validation here
	int ncols = 0;
	int nitem = m_pTabla->m_def->m_info->n_campos;	
	int i;
	for (i = 0;i < nitem;i++)
	{
		if (m_Lista.GetCheck(i))
		{
			ncols++;
		}
	}	
	if (!ncols)
	{
		AfxMessageBox("Debe seleccionar al menos una columna",MB_OK|MB_ICONHAND);
		return;
	}
	m_pTabla->m_curcol = 0;
	m_pTabla->BorraCache();
	ncols = 0;
	for (i = 0;i < nitem;i++)
	{
		if (m_Lista.GetCheck(i))
		{
			m_pTabla->m_def->m_colcampos[ncols] = m_indices[i];
			ncols++;
		}
	}	
	m_pTabla->m_def->m_n_campos_col = ncols;
    for (i = ncols;i < m_pTabla->m_def->m_info->t_total_campos;i++)
    {
        m_pTabla->m_def->m_colcampos[ i ] = -1;
    }
	CBASEDLG::OnOK();
}
