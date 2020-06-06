// DsDlgBusqueda.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsDlgBusqueda.h"
#include ".\dsdlgbusqueda.h"


// CDsDlgBusqueda dialog

IMPLEMENT_DYNAMIC(CDsDlgBusqueda, CDialog)
CDsDlgBusqueda::CDsDlgBusqueda(CWnd* pParent /*=NULL*/)
	: CDialog(CDsDlgBusqueda::IDD, pParent)
	, m_desde(_T(""))
	, m_hasta(_T(""))
	, m_valor(_T(""))
{
}

CDsDlgBusqueda::~CDsDlgBusqueda()
{
}

void CDsDlgBusqueda::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUSCAREN, m_buscaren);
	DDX_Text(pDX, IDC_EDIT2, m_desde);
	DDX_Text(pDX, IDC_EDIT3, m_hasta);
	DDX_Text(pDX, IDC_EDIT4, m_valor);
}


BEGIN_MESSAGE_MAP(CDsDlgBusqueda, CDialog)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
END_MESSAGE_MAP()


// CDsDlgBusqueda message handlers

void CDsDlgBusqueda::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
}
