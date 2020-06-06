// DlgCPasswd.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgCPasswd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPasswd dialog


CDlgCPasswd::CDlgCPasswd(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgCPasswd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPasswd)
	m_password = _T("");
	m_nuevo = _T("");
	m_confirma = _T("");
	m_usuario = _T("");
	//}}AFX_DATA_INIT
	DLGCONS;
}


void CDlgCPasswd::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPasswd)
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 10);
	DDX_Text(pDX, IDC_PASSWORD2, m_nuevo);
	DDV_MaxChars(pDX, m_nuevo, 10);
	DDX_Text(pDX, IDC_PASSWORD3, m_confirma);
	DDV_MaxChars(pDX, m_confirma, 10);
	DDX_Text(pDX, IDC_USUARIO, m_usuario);
	DDV_MaxChars(pDX, m_usuario, 10);

    DDX_Control(pDX, IDOK, m_ctrlok);
    DDX_Control(pDX, IDCANCEL, m_ctrlcancel);
	DDX_Control(pDX, IDC_USUARIO, m_edit1);
	DDX_Control(pDX, IDC_PASSWORD, m_edit2);
	DDX_Control(pDX, IDC_PASSWORD2, m_edit3);
	DDX_Control(pDX, IDC_PASSWORD3, m_edit4);
	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPasswd, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCPasswd)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPasswd message handlers
