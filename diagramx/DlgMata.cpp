// DlgMata.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "DlgMata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMata dialog


CDlgMata::CDlgMata(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgMata::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMata)
	m_clavemata = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgMata::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMata)
	DDX_Text(pDX, IDC_CLAVEMATA, m_clavemata);
	DDV_MaxChars(pDX, m_clavemata, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMata, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgMata)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMata message handlers
