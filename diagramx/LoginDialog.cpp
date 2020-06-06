// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "LoginDialog.h"
#include "DlgCPasswd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog

IMPLEMENT_DYNAMIC(CLoginDialog,CBASEDLG);

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
: CExtNCW < CBASEDLG >(CLoginDialog::IDD, pParent)
{
   //{{AFX_DATA_INIT(CLoginDialog)
   m_password = _T("");
   m_usuario = _T("");
   //}}AFX_DATA_INIT
   m_cambio = "";
   m_pregusu = 0;
   DLGCONS;
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
   CBASEDLG::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CLoginDialog)
   DDX_Control(pDX, IDC_CAMBIO, m_ctrlcambio);
   DDX_Control(pDX, IDOK, m_ctrlok);
   DDX_Control(pDX, IDCANCEL, m_ctrlcancel);
   DDX_Control(pDX, IDC_LABEL_USUARIO, m_labelusu);
   DDX_Control(pDX, IDC_LABEL_PASSWORD, m_labelpas);
   DDX_Control(pDX, IDC_USUARIO, m_ctrlusu);
   DDX_Control(pDX, IDC_PASSWORD, m_ctrlpas);
   DDX_Text(pDX, IDC_PASSWORD, m_password);
   DDV_MaxChars(pDX, m_password, 10);
   DDX_Text(pDX, IDC_USUARIO, m_usuario);
   DDV_MaxChars(pDX, m_usuario, 10);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginDialog, CBASEDLG)
//{{AFX_MSG_MAP(CLoginDialog)
	ON_WM_KILLFOCUS()
	ON_WM_NCACTIVATE()
	ON_BN_CLICKED(IDC_CAMBIO, OnCambio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog message handlers

void CLoginDialog::OnKillFocus(CWnd* pNewWnd) 
{
	CBASEDLG::OnKillFocus(pNewWnd);
}

BOOL CLoginDialog::OnNcActivate(BOOL bActive) 
{
	BOOL r = CBASEDLG::OnNcActivate(bActive);
	return r;
}

BOOL CLoginDialog::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();		

    theApp.m_dlghwnd = m_hWnd;
	if (theApp.m_cambio_autorizado)
	{
		GetDlgItem(IDC_CAMBIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CAMBIO)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CAMBIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CAMBIO)->ShowWindow(SW_HIDE);
	}

	if (m_pregusu)
	{
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		if (m_pregusu == 2)
		{
			GetDlgItem(IDC_USUARIO)->EnableWindow(FALSE);
		}
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CLoginDialog::~CLoginDialog()
{
	theApp.m_dlghwnd = NULL;
}

void CLoginDialog::OnCambio() 
{
	CDlgCPasswd dlg;
	UpdateData(TRUE);
	dlg.m_usuario = m_usuario;
	dlg.m_password = m_password;
	if (dlg.DoModal() == IDOK)
	{
		dlg.m_nuevo.TrimRight();
		dlg.m_password.TrimRight();
		if (dlg.m_nuevo.IsEmpty())
		{
			MessageBox("No se admite una nueva clave vacía","Atencion",MB_OK|MB_ICONHAND);							
			return;
		}
		if (!dlg.m_nuevo.Compare(dlg.m_password))
		{
			MessageBox("La nueva clave ha de ser diferente de la anterior","Atencion",MB_OK|MB_ICONHAND);
			return;
		}

		if (dlg.m_nuevo.Compare(dlg.m_confirma))
		{
			MessageBox("Las claves no coinciden","Atencion",MB_OK|MB_ICONHAND);
		}
		else
		{
			m_usuario = dlg.m_usuario;
			m_password = dlg.m_password;
			m_cambio = dlg.m_nuevo;
			EndDialog(IDOK);
		}
	}
}
