// DlgKActiva.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgKActiva.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKActiva dialog


CDlgKActiva::CDlgKActiva(char *activacion,char *identifica,char *numero,char *telefonos,CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgKActiva::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgKActiva)
	m_activacion = activacion;
	m_identifica = identifica;
	m_numero     = numero;
	m_telefonos  = telefonos;
	//}}AFX_DATA_INIT
    m_pnumero = numero;
	DLGCONS;
}


void CDlgKActiva::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKActiva)
	DDX_Text(pDX, IDC_ACTIVACION, m_activacion);
	DDX_Text(pDX, IDC_IDENTIFICA, m_identifica);
	DDX_Text(pDX, IDC_NUMERO, m_numero);
	DDX_Text(pDX, IDC_TELEFONOS, m_telefonos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKActiva, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgKActiva)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKActiva message handlers

void CDlgKActiva::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
    strcpy(m_pnumero,m_numero);
	CBASEDLG::OnOK();
}
