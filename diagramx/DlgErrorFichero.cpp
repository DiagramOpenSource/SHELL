// DlgErrorFichero.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgErrorFichero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgErrorFichero dialog


CDlgErrorFichero::CDlgErrorFichero(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgErrorFichero::IDD, pParent)
{
    m_muerto = FALSE;
	//{{AFX_DATA_INIT(CDlgErrorFichero)
	m_edit1 = _T("");
	m_edit2 = _T("");
	m_edit3 = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgErrorFichero::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
    m_edit1.TrimLeft();
    m_edit1.TrimRight();
    m_edit2.TrimLeft();
    m_edit2.TrimRight();
    m_edit3.TrimLeft();
    m_edit3.TrimRight();
	//{{AFX_DATA_MAP(CDlgErrorFichero)
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgErrorFichero, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgErrorFichero)
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgErrorFichero message handlers



void CDlgErrorFichero::OnCancel() 
{
	//CBASEDLG::OnCancel();
}

void CDlgErrorFichero::OnOK() 
{		
	CWnd *pwnd = GetDlgItem(IDOK);
	if (pwnd && !pwnd->IsWindowEnabled())
	{
		return;
	}
    m_muerto = TRUE;
	CBASEDLG::OnOK();
}


BOOL CDlgErrorFichero::OnNcActivate(BOOL bActive) 
{	
	if (bActive || m_muerto)
	    return CBASEDLG::OnNcActivate(bActive);
    else
        return FALSE;
}
