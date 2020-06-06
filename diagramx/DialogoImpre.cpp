
// DialogoImpre.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "DialogoImpre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogoImpre dialog


CDialogoImpre::CDialogoImpre(CWnd* pParent /*=NULL*/)
: CDialog(CDialogoImpre::IDD, pParent)
{
   //{{AFX_DATA_INIT(CDialogoImpre)
   m_MargenI = 0;
   m_MargenS = 0;
   m_Lineas = 66;
   m_Ajusta = FALSE;
   m_Cols = 140;
   m_cabeceraexcel = FALSE;
   m_posexcel = _T("");
   m_MargenI2 = 0;
   //}}AFX_DATA_INIT
   
   m_SFont = "";
}


void CDialogoImpre::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDialogoImpre)
   DDX_Control(pDX, IDC_IMPREFONT, m_Font);
   DDX_Text(pDX, IDC_MARGENI, m_MargenI);
   DDX_Text(pDX, IDC_MARGENS, m_MargenS);
   DDX_Text(pDX, IDC_IMPRELINEAS, m_Lineas);
   DDV_MinMaxInt(pDX, m_Lineas, 1, 9999);
   DDX_Check(pDX, IDC_IMPREAJUSTA, m_Ajusta);
   DDX_Text(pDX, IDC_IMPRECOLS, m_Cols);
   DDV_MinMaxInt(pDX, m_Cols, 0, 9999);
   DDX_Check(pDX, IDC_EXCELCABECERA, m_cabeceraexcel);
   DDX_Text(pDX, IDC_POSEXCEL, m_posexcel);
   DDV_MaxChars(pDX, m_posexcel, 7);
   DDX_Text(pDX, IDC_MARGENI2, m_MargenI2);
   //}}AFX_DATA_MAP
   if (pDX->m_bSaveAndValidate)
   {	    
      GetDlgItemText(IDC_IMPREFONT,m_SFont);
   }
}


BEGIN_MESSAGE_MAP(CDialogoImpre, CDialog)
//{{AFX_MSG_MAP(CDialogoImpre)
ON_BN_CLICKED(IDC_IMPREAJUSTA, OnImpreajusta)
ON_EN_CHANGE(IDC_MARGENI, OnChangeMargeni)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static int CALLBACK EnumFontFamExProc(
                                      ENUMLOGFONTEX *lpelfe,    // pointer to logical-font data
                                      NEWTEXTMETRICEX *lpntme,  // pointer to physical-font data
                                      int FontType,             // type of font
                                      LPARAM lParam             // application-defined data
                                      )
{
   if ( (lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH) )
   {  
      CComboBox *f = (CComboBox *)lParam;
      CString s = lpelfe->elfLogFont.lfFaceName;	 	 
      if ( lpelfe->elfLogFont.lfOutPrecision == OUT_STROKE_PRECIS )
      {
         if (f->FindString(-1, s ) < 0)
            f->AddString(s);
      }
   }
   return(1);
}

BOOL CDialogoImpre::OnInitDialog() 
{
   CDialog::OnInitDialog();
   
   //
   
   {
      LOGFONT tmplf;
      tmplf.lfCharSet = DEFAULT_CHARSET;
      tmplf.lfFaceName[0] = 0;
      tmplf.lfPitchAndFamily = 0;
      //CDC hDC;
      //hDC.CreateDC( LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData );
      CPrintDialog pdlg(FALSE);
      theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );
      HDC hDC = pdlg.CreatePrinterDC( );
      ::EnumFontFamiliesEx(pdlg.m_pd.hDC, &tmplf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)&m_Font,0);
      ::DeleteDC(hDC);
   }
   int sel;
   if ((sel = m_Font.FindString(-1, m_SFont )) < 0)
   {
      sel = m_Font.AddString(m_SFont);
   } 
   m_Font.SetCurSel(sel);
   
   OnImpreajusta();
   
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogoImpre::OnImpreajusta() 
{
   // TODO: Add your control notification handler code here
   CWnd *w = GetDlgItem(IDC_IMPRELINEAS);
   if (!w) return;
   CWnd *w2 = GetDlgItem(IDC_IMPRECOLS);
   if (!w2) return;
   UpdateData();
   if (m_Ajusta)
   {
      w->EnableWindow( TRUE );
      w2->EnableWindow( TRUE );
   }
   else
   {
      w->EnableWindow( FALSE );
      w2->EnableWindow( FALSE );
   }
}

void CDialogoImpre::OnChangeMargeni() 
{
   UpdateData(TRUE);
   m_MargenI2 = m_MargenI;
   UpdateData(FALSE);
}
