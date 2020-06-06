// DlgProgreso.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgProgreso.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgreso dialog


CDlgProgreso::CDlgProgreso(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgProgreso::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgreso)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgProgreso::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgreso)
	DDX_Control(pDX, IDC_PROGRESO, m_progreso);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgreso, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgProgreso)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgreso message handlers
