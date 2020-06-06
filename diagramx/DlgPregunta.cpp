// DlgPregunta.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgPregunta.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPregunta dialog


CDlgPregunta::CDlgPregunta(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgPregunta::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPregunta)
	m_clave = _T("");
	//}}AFX_DATA_INIT
	m_conok = FALSE;	 
	m_cualquiera = FALSE;

	char tmp[100];

	sprintf(tmp,"%02d",CTime::GetCurrentTime().GetDay());

	m_otraclave.Format("%ctomate%c",tmp[0],tmp[1]);

	DLGCONS;
}


void CDlgPregunta::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPregunta)
	DDX_Text(pDX, IDC_LACLAVE, m_clave);
	DDV_MaxChars(pDX, m_clave, 10);
	DDX_Control(pDX, IDOK, m_boton1);
	DDX_Control(pDX, IDCANCEL, m_boton2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgPregunta, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgPregunta)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPregunta message handlers

void CDlgPregunta::OnOK() 
{
	m_conok = TRUE;
    UpdateData(TRUE);
	if (m_cualquiera)
	{
		CBASEDLG::OnOK();
		return;
	}

	if (m_clave.Compare(m_laclave) && m_clave.Compare(m_otraclave))
        CBASEDLG::OnCancel();
    else
	    CBASEDLG::OnOK();
}
