// DsPopup.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsStdDlg.h"
#include "DsPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsPopup

CDsPopup::CDsPopup()
{
	m_show = FALSE;
	m_desatendido = 0;
	m_w2 = NULL;
}

CDsPopup::~CDsPopup()
{
}


BEGIN_MESSAGE_MAP(CDsPopup, CWnd)
	//{{AFX_MSG_MAP(CDsPopup)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDsPopup message handlers

void CDsPopup::OnClose() 
{
	theApp.TeclaAlActivo(807);
	//CWnd::OnClose();
}
