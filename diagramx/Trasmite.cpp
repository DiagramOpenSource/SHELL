// Trasmite.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "Trasmite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrasmite dialog
unsigned long crt1 = 0;
unsigned long crt2 = 0xffffffff;
unsigned long crt3 = 0xffffffff;

CTrasmite::CTrasmite(CWnd* pParent /*=NULL*/)
: CExtNCW < CBASEDLG >()
{
   Create(CTrasmite::IDD, pParent);
   //{{AFX_DATA_INIT(CTrasmite)
   m_Paso = _T("");
   //}}AFX_DATA_INIT
   m_Refrescar = FALSE;
   m_Salir = FALSE;
   DLGCONS;
}


void CTrasmite::DoDataExchange(CDataExchange* pDX)
{
   CBASEDLG::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTrasmite)
   DDX_Control(pDX, IDC_TPROGRESS, m_tprogress);
   DDX_Control(pDX, IDC_TAQUI, m_taqui);
   DDX_Control(pDX, IDC_TALLI, m_talli);
   DDX_Text(pDX, IDC_TPASO, m_Paso);
   DDV_MaxChars(pDX, m_Paso, 10);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrasmite, CBASEDLG)
//{{AFX_MSG_MAP(CTrasmite)
ON_NOTIFY(NM_DBLCLK, IDC_TALLI, OnDblclkTalli)
ON_NOTIFY(NM_DBLCLK, IDC_TAQUI, OnDblclkTaqui)
ON_WM_CLOSE()
ON_WM_DESTROY()
ON_WM_LBUTTONDBLCLK()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrasmite message handlers

BOOL CTrasmite::OnInitDialog()
{
   CBASEDLG::OnInitDialog();
   
   // TODO: Add extra initialization here
   
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrasmite::OnDblclkTalli(NMHDR* pNMHDR, LRESULT* pResult) 
{
   UpdateData(TRUE);
   if (m_Paso.Compare("HAMPERDIN"))
   {
      AfxMessageBox("Password Incorrecto");
      *pResult = 1;
      return;
   }
   
   
   
   TV_ITEM tv;
   tv.hItem = m_talli.GetSelectedItem( );
   if (tv.hItem)
   {	
      char tmp[512];
      
      m_alla = TRUE;
      m_trasmite = "";
      m_remoto.Replace('\\','/');
      
      tv.cchTextMax = 500;
      tv.pszText = tmp;
      tv.mask = TVIF_HANDLE | TVIF_TEXT;	
      if (m_talli.GetItem(&tv))
      {			
         if (tmp[0] == '*')
         {
            m_remoto += (tmp+2);
            m_remoto += "/";			    
         }
         else
            if (!strcmp(tmp,".."))
            {	
               m_remoto.Delete(m_remoto.GetLength()-1,1);
               int punto = m_remoto.ReverseFind( '/' );
               if (punto < 0)
               {
                  m_remoto = "/";
               }
               else
               {
                  m_remoto = m_remoto.Left(punto+1);
               }
            }
            else
               if (tmp[0] == ' ')			
               {
                  m_trasmite = tmp+2;
               }
               m_Refrescar = TRUE;
      }
   }
   *pResult = 0;
}

void CTrasmite::OnDblclkTaqui(NMHDR* pNMHDR, LRESULT* pResult)
{
   
   UpdateData(TRUE);
   if (m_Paso.Compare("HAMPERDIN"))
   {
      AfxMessageBox("Password Incorrecto");
      *pResult = 1;
      return;
   }
   
   
   TV_ITEM tv;
   tv.hItem = m_taqui.GetSelectedItem( );
   if (tv.hItem)
   {	
      char tmp[512];
      
      m_alla = FALSE;
      m_trasmite = "";
      m_local.Replace('\\','/');
      
      tv.cchTextMax = 500;
      tv.pszText = tmp;
      tv.mask = TVIF_HANDLE | TVIF_TEXT;	
      if (m_taqui.GetItem(&tv))
      {			
         if (tmp[0] == '*')
         {
            m_local += (tmp+2);
            m_local += "/";			    
         }
         else
            if (!strcmp(tmp,".."))
            {	
               m_local.Delete(m_local.GetLength()-1,1);
               int punto = m_local.ReverseFind( '/' );
               if (punto < 0)
               {
                  m_local = "/";
               }
               else
               {
                  m_local = m_local.Left(punto+1);
               }
            }
            else
               if (tmp[0] == ' ')			
               {
                  m_trasmite = tmp+2;
               }
               m_Refrescar = TRUE;
      }
   }
   *pResult = 0;
}

void CTrasmite::OnOK() 
{
   m_Salir = TRUE;
   CBASEDLG::OnOK();
}

void CTrasmite::OnClose()
{	
   m_Salir = TRUE;
   CBASEDLG::OnClose();
}

void CTrasmite::OnDestroy() 
{
   m_Salir = TRUE;
   CBASEDLG::OnDestroy();			
}

void CTrasmite::OnCancel() 
{
   m_Salir = TRUE;	
   CBASEDLG::OnCancel();
}

void CTrasmite::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   
   UpdateData(TRUE);
   if (m_Paso.Compare("HAMPERDIN"))
   {
      AfxMessageBox("Password Incorrecto");
      return;
   }
   
   CBASEDLG::OnLButtonDblClk(nFlags, point);
}
