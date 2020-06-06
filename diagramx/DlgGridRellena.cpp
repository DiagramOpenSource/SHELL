// DlgGridRellena.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "DlgGridRellena.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGridRellena dialog


CDlgGridRellena::CDlgGridRellena(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgGridRellena::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGridRellena)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    Create( CDlgGridRellena::IDD, pParent);
	DLGCONS;
}


void CDlgGridRellena::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGridRellena)
	DDX_Control(pDX, IDC_ANIMATE1, m_animacion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGridRellena, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgGridRellena)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGridRellena message handlers

BOOL CDlgGridRellena::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    CString miavi;
    miavi = theApp.m_dirbase;
    miavi += "dev\\ayuda.avi";
    
    m_animacion.Open( miavi );	
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGridRellena::OnSetFocus(CWnd* pOldWnd) 
{
	if (pOldWnd && ::IsWindow(pOldWnd->m_hWnd)) 
	{
		::SetFocus(pOldWnd->m_hWnd); // no queremos el foco para nada
	}
	//CBASEDLG::OnSetFocus(pOldWnd);	
}
