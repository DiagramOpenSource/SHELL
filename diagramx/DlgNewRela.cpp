// DlgNewRela.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgNewRela.h"
#include "TablaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRela dialog

const char *quita_path(const char *ff);

CDlgNewRela::CDlgNewRela(CDlgGridFiltro* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgNewRela::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewRela)
	m_rela = _T("");
	//}}AFX_DATA_INIT
	m_filtro = pParent;
	m_relacionado = NULL;
	m_checko = -1;
	m_checkd = -1;

	DLGCONS;
}

CDlgNewRela::~CDlgNewRela()	
{
	if (m_relacionado)
	{
		delete m_relacionado;
	}
}

void CDlgNewRela::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_button1);
	DDX_Control(pDX, IDCANCEL, m_button2);
	//{{AFX_DATA_MAP(CDlgNewRela)
	DDX_Control(pDX, IDC_FICHEROS, m_ficheros);
	DDX_Control(pDX, IDC_CAMPOSO, m_camposo);
	DDX_Control(pDX, IDC_CAMPOSD, m_camposd);
	DDX_Text(pDX, IDC_RELA, m_rela);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewRela, CBASEDLG)
    ON_CLBN_CHKCHANGE(IDC_CAMPOSO, OnCheckOrigen)	
	ON_CLBN_CHKCHANGE(IDC_CAMPOSD, OnCheckDestino)	
	//{{AFX_MSG_MAP(CDlgNewRela)
	ON_LBN_SELCHANGE(IDC_CAMPOSO, OnSelchangeCamposo)
	ON_LBN_SELCHANGE(IDC_CAMPOSD, OnSelchangeCamposd)
	ON_CBN_EDITCHANGE(IDC_FICHEROS, OnEditchangeFicheros)
	ON_CBN_SELCHANGE(IDC_FICHEROS, OnSelchangeFicheros)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRela message handlers

void CDlgNewRela::OnOK() 
{
	int n = m_camposo.GetCount();
	int *campospadre;
	int *camposhijo;
	int camposrel = 0;
	int j;
	int i;

	for (i = 0;i < n;i++)
	{
		if ((BOOL)m_camposo.GetCheck(i))
			camposrel++;
	}
	if (!camposrel)
	{
		AfxMessageBox("Debe indicar los campos de la relación");
		return;
	}
	campospadre = new int[camposrel];
	camposhijo  = new int[camposrel];

	j = 0;
	for (i = 0;i < n;i++)
	{
		if ((BOOL)m_camposo.GetCheck(i))
		{
			campospadre[j] = m_fichero->m_item[i]->m_campo;
			j++;
		}
	}

	n = m_camposd.GetCount();
	j = 0;
	for (i = 0;i < n && j < camposrel;i++)
	{	
		if ((BOOL)m_camposd.GetCheck(i))
		{
			camposhijo[j] = m_relacionado->m_item[i]->m_campo;
			j++;
		}
	}

	m_relacionado->Relacion(m_fichero,camposrel,campospadre,camposhijo);
	m_relacionado->BorraItems();
	m_relacionado->MemoriaItems();
	m_relacionado->PorDefecto();

	CBASEDLG::OnOK();
}

void CDlgNewRela::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBASEDLG::OnCancel();
}

void CDlgNewRela::OnSelchangeCamposo() 
{	
	
}

void CDlgNewRela::OnSelchangeCamposd() 
{	
	
}

BOOL CDlgNewRela::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	int i,idx;
	

	m_camposo.ModifyStyle(m_camposo.m_hWnd,LBS_SORT,0,SWP_FRAMECHANGED);
	m_camposd.ModifyStyle(m_camposo.m_hWnd,LBS_SORT,0,SWP_FRAMECHANGED);

    for (int _i = 0;_i < m_fichero->m_nitem;_i++)
    {
        i = m_fichero->m_idx[_i];
        idx = m_camposo.AddString(m_fichero->m_item[i]->m_name);
        ASSERT(m_fichero->m_item[i]->m_idx == idx);        
		/*
        if (m_fichero->m_item[i]->m_checked)
            m_camposo.SetCheck(idx,1);
        else
            m_camposo.SetCheck(idx,0);
		*/
    }

	char tmp[10100];
	int n;
	int inicio = 0;
	char *p = tmp;

	do
	{
		p = tmp + (sizeof(long) * 3) + (sizeof(int)*2);

		n = 10000;
		memcpy(p,(char *)&n,sizeof(int));		
		memcpy(p+sizeof(int),&inicio,sizeof(int));
		m_filtro->m_pTabla->EnviaRetro(23,sizeof(int)*2,10004,tmp);

		
		n = *((int *)p);
		p += sizeof(int);

		CString s;

		for (i = 0;i < n;i++)
		{
			s = (LPCSTR)p;
			s = " - " + s;
			p += (strlen(p)+1);
			s = (LPCSTR)p + s;
			p += (strlen(p)+1);
			m_ficheros.AddString( s );
		}	
		inicio += i;
	} while(*p == 1);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNewRela::OnEditchangeFicheros() 
{
}

void CDlgNewRela::OnSelchangeFicheros() 
{
	char tmp[512],tmp2[512];
	CString s;
	int i;
	strcpy(tmp,m_filtro->m_pTabla->m_def->m_info->PathDef);
	*((char *)quita_path(tmp)) = 0;

	m_ficheros.GetWindowText((LPTSTR)tmp2,500);
	for (i = strlen(tmp2);i > 0;i--)
	{
		if (tmp2[i] == '-')
		{
			i += 2;
			break;
		}
	}
	strcat(tmp,tmp2+i);
	
	CFiltroDef *def = new CFiltroDef(tmp,0,NULL);
	def->RellenaDef(m_filtro->m_pTabla);
	m_camposd.ResetContent();
	for (i = 0;i < def->m_nitem;i++)
	{
		m_camposd.AddString(def->m_item[i]->m_name);
	}	


	if (m_relacionado)
	{
		delete m_relacionado;
	}
	m_relacionado = def;	
}


void CDlgNewRela::OnCheckOrigen()
{
	int i;
	if (m_checko != -1)
	{
		m_camposo.SetCheck(m_checko,0);
		m_checko = -1;
	}
	int n = m_camposo.GetCount();
	for (i = 0;i < n;i++)
	{
		if ((BOOL)m_camposo.GetCheck(i))
		{
			m_checko = i;
			break;
		}
	}
	InformaRela();
}

void CDlgNewRela::OnCheckDestino()
{
	int i;
	if (m_checkd != -1)
	{
		m_camposd.SetCheck(m_checkd,0);
		m_checkd = -1;
	}
	int n = m_camposd.GetCount();
	for (i = 0;i < n;i++)
	{
		if ((BOOL)m_camposd.GetCheck(i))
		{
			m_checkd = i;
			break;
		}
	}
	InformaRela();
}

void CDlgNewRela::InformaRela()
{
	m_rela = "";
	if (m_checko != -1)
	{
		m_rela += m_fichero->m_item[m_checko]->m_name;
	}
	m_rela += " <-> ";
	if (m_checkd != -1 && m_relacionado)
	{
		m_rela += m_relacionado->m_item[m_checkd]->m_name;
	}	
	UpdateData(FALSE);
}
