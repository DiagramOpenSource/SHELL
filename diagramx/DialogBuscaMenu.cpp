// DialogBuscaMenu.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DialogBuscaMenu.h"
#include "DsxLeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBuscaMenu dialog


CDialogBuscaMenu::CDialogBuscaMenu(CWnd* pParent /*=NULL*/)
	: CBASEDLG (CDialogBuscaMenu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBuscaMenu)
	m_texto = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDialogBuscaMenu::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBuscaMenu)
	DDX_Text(pDX, IDC_TBUSCAMENU, m_texto);	 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBuscaMenu, CBASEDLG)
	//{{AFX_MSG_MAP(CDialogBuscaMenu)	 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBuscaMenu message handlers

void CDialogBuscaMenu::OnOK() 
{	
	//CBASEDLG::OnOK();
	UpdateData(TRUE);
    if (theApp.m_LeftView)
	{
		theApp.m_LeftView->BuscaYRellenaMenuUser(m_texto);
	}
}


void CDialogBuscaMenu::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CBASEDLG::OnCancel();
}
