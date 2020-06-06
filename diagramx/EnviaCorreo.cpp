// EnviaCorreo.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "EnviaCorreo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnviaCorreo dialog


CEnviaCorreo::CEnviaCorreo(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CEnviaCorreo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnviaCorreo)
	m_documento = _T("");
	m_destino = _T("");
	m_origen = _T("");
	m_asunto = _T("");
	m_nota = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CEnviaCorreo::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnviaCorreo)
	DDX_CBString(pDX, IDC_COMBO1, m_documento);
	DDX_Text(pDX, IDC_EDIT1, m_destino);
	DDX_Text(pDX, IDC_EDIT2, m_origen);
	DDX_Text(pDX, IDC_EDIT3, m_asunto);
	DDX_Text(pDX, IDC_EDIT4, m_nota);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnviaCorreo, CBASEDLG)
	//{{AFX_MSG_MAP(CEnviaCorreo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnviaCorreo message handlers

void CEnviaCorreo::OnOK() 
{
	// TODO: Add extra validation here
	
	CBASEDLG::OnOK();
}
