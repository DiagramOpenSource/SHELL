// DlgCambiaPasswd.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgCambiaPasswd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaPasswd dialog


IMPLEMENT_DYNAMIC(CDlgCambiaPasswd,CBASEDLG);

CDlgCambiaPasswd::CDlgCambiaPasswd(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgCambiaPasswd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCambiaPasswd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCambiaPasswd::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCambiaPasswd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCambiaPasswd, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCambiaPasswd)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaPasswd message handlers
