// DlgIndexado.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgIndexado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIndexado dialog


CDlgIndexado::CDlgIndexado(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgIndexado::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIndexado)
	m_panel1 = _T("");
	m_panel2 = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgIndexado::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIndexado)
	DDX_Text(pDX, IDC_EDIT1, m_panel1);
	DDX_Text(pDX, IDC_EDIT2, m_panel2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIndexado, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgIndexado)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIndexado message handlers

void CDlgIndexado::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CBASEDLG::OnCancel();
}

void CDlgIndexado::OnOK() 
{
		
	CBASEDLG::OnOK();
}
