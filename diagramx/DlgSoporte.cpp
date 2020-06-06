// DlgSoporte.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgSoporte.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSoporte dialog


CDlgSoporte::CDlgSoporte(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgSoporte::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSoporte)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgSoporte::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSoporte)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSoporte, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgSoporte)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSoporte message handlers

void CDlgSoporte::OnOK() 
{
	// TODO: Add extra validation here
	
	//CBASEDLG::OnOK();
}

void CDlgSoporte::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBASEDLG::OnCancel();
}
