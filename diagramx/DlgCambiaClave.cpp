// DlgCambiaClave.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgCambiaClave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaClave dialog


CDlgCambiaClave::CDlgCambiaClave(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgCambiaClave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCambiaClave)
	m_clave = _T("");
	m_cclave = _T("");
	//}}AFX_DATA_INIT
	m_conok = FALSE;
}


void CDlgCambiaClave::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCambiaClave)
	DDX_Text(pDX, IDC_LACLAVE3, m_clave);
	DDV_MaxChars(pDX, m_clave, 19);
	DDX_Text(pDX, IDC_LACLAVE2, m_cclave);
	DDV_MaxChars(pDX, m_cclave, 19);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCambiaClave, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCambiaClave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCambiaClave message handlers

void CDlgCambiaClave::OnOK() 
{
	m_conok = TRUE;
    UpdateData(TRUE);
	if (!m_clave.GetLength() || m_clave.Compare(m_cclave))
	{
		if (m_clave.GetLength())
			AfxMessageBox("La clave no puede estar vacia!");			
		else
			AfxMessageBox("La clave no coincide!");
			
        CBASEDLG::OnCancel();
	}
    else
	    CBASEDLG::OnOK();
}
