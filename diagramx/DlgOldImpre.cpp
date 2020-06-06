// DlgOldImpre.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgOldImpre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOldImpre dialog


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

CDlgOldImpre::CDlgOldImpre(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgOldImpre::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOldImpre)
	m_spool = theApp.GetProfileString("Defaults","m_spool",_T(""));
	m_impre = _T("");
    m_MargenI = theApp.GetProfileInt("Defaults","m_MargenI",(int)0);
    m_MargenS = theApp.GetProfileInt("Defaults","m_MargenS",(int)0);
    m_Lineas = theApp.GetProfileInt("Defaults","m_Lineas",(int)66);
    m_Ajusta = theApp.GetProfileInt("Defaults","m_Ajusta",(int)TRUE);
    m_Cols = theApp.GetProfileInt("Defaults","m_Cols",(int)80);
    m_cabeceraexcel = theApp.GetProfileInt("Defaults","m_cabeceraexcel",(int)FALSE);
    m_posexcel = theApp.GetProfileString("Defaults","m_posexcel",_T(""));
    m_MargenI2 = theApp.GetProfileInt("Defaults","m_MargenI2",(int)0);
	m_faxauto = theApp.GetProfileInt("Defaults","m_faxauto",(int)FALSE);
	m_NumeroFax= theApp.GetProfileString("Defaults","m_NumeroFax",_T(""));
	m_CorreoFax= theApp.GetProfileString("Defaults","m_CorreoFax",_T(""));
	m_CorreoAsunto=theApp.GetProfileString("Defaults","m_CorreoAsunto",_T(""));

	m_imprewindows = _T("");
	m_precrystal = theApp.GetProfileInt("Defaults","m_precrystal",(int)TRUE);
	m_pdfcrystal = theApp.GetProfileInt("Defaults","m_pdfcrystal",(int)FALSE);
	
	//}}AFX_DATA_INIT

    m_crcopias = theApp.GetProfileInt("Defaults","m_crcopias",(int)1);
    m_crdoblehoja = (BOOL)theApp.GetProfileInt("Defaults","m_crdoblehoja",(int)0);
    m_crdesdepg = theApp.GetProfileInt("Defaults","m_crdesdepg",(int)0);
    m_crhastapg = theApp.GetProfileInt("Defaults","m_crhastapg",(int)0);

	m_crselimprecristal = (BOOL)theApp.GetProfileInt("Defaults","m_crselimprecristal",(int)0);	

	m_SFont = "";
    m_nrecs = 0;

	DLGCONS;
}


void CDlgOldImpre::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOldImpre)
	DDX_Control(pDX, IDC_SELIMPRE, m_boton1);
    DDX_Control(pDX, IDC_IMPREWINDOWS, m_edit1);
	DDX_Control(pDX, IDC_MARGENI, m_edit2);
	DDX_Control(pDX, IDC_MARGENI2, m_edit3);
	DDX_Control(pDX, IDC_MARGENS, m_edit4);
	DDX_Control(pDX, IDC_IMPRELINEAS, m_edit5);
	DDX_Control(pDX, IDC_IMPRECOLS, m_edit6);
	DDX_Control(pDX, IDC_POSEXCEL, m_edit7);
	DDX_Control(pDX, IDC_LAIMPRE, m_edit8);
	DDX_Control(pDX, IDC_SPOOLOLDIMPRE, m_edit9);
	DDX_Control(pDX, IDC_NUMEROFAX, m_edit10);
	DDX_Control(pDX, IDC_CORREOFAX, m_edit11);
	DDX_Control(pDX, IDC_CORREOASUNTO, m_edit12);	

	DDX_Control(pDX, IDC_COPIAS, m_edit13);	
	DDX_Control(pDX, IDC_DOBLEHOJA, m_check7);	
	DDX_Control(pDX, IDC_DESDEPG, m_edit14);	
	DDX_Control(pDX, IDC_HASTAPG, m_edit15);	

	DDX_Control(pDX, IDC_SELIMPRECRISTAL, m_check8);

	DDX_Control(pDX, IDOK, m_boton2);
	DDX_Control(pDX, IDCANCEL, m_boton3);

	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	DDX_Control(pDX, IDC_STATIC5, m_label5);
	DDX_Control(pDX, IDC_STATIC6, m_label6);
	DDX_Control(pDX, IDC_STATIC7, m_label7);
	DDX_Control(pDX, IDC_STATIC8, m_label8);
	DDX_Control(pDX, IDC_STATIC9, m_label9);
	DDX_Control(pDX, IDC_STATIC10, m_label10);
	DDX_Control(pDX, IDC_STATIC11, m_label11);
	DDX_Control(pDX, IDC_STATIC12, m_label12);

	DDX_Control(pDX, IDC_STATIC50, m_label50);
	DDX_Control(pDX, IDC_STATIC51, m_label51);
	DDX_Control(pDX, IDC_STATIC52, m_label52);
	DDX_Control(pDX, IDC_STATIC53, m_label53);
	DDX_Control(pDX, IDC_STATIC54, m_label54);
	DDX_Control(pDX, IDC_STATIC55, m_label55);

	DDX_Control(pDX, IDC_STATIC56, m_label56);
	DDX_Control(pDX, IDC_STATIC57, m_label57);
	DDX_Control(pDX, IDC_STATIC58, m_label58);
	DDX_Control(pDX, IDC_STATIC59, m_label59);


	DDX_Control(pDX, IDC_PRECRYSTAL, m_check1);
	DDX_Control(pDX, IDC_IMPREAJUSTA, m_check2);
	DDX_Control(pDX, IDC_EXCELCABECERA, m_check3);
	//DDX_Control(pDX, IDC_IMPREOEM, m_check4);
	DDX_Control(pDX, IDC_FAXAUTOMATICO, m_check5);
	DDX_Control(pDX, IDC_PRECRYSTAL2, m_check6);
	
	DDX_Control(pDX, IDC_LISTAOLDIMPRE, m_lista);
	DDX_Text(pDX, IDC_SPOOLOLDIMPRE, m_spool);
	DDX_Text(pDX, IDC_LAIMPRE, m_impre);
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
	DDX_Text(pDX, IDC_IMPREWINDOWS, m_imprewindows);
	DDX_Check(pDX, IDC_PRECRYSTAL, m_precrystal);
	DDX_Check(pDX, IDC_PRECRYSTAL2, m_pdfcrystal);	
	DDX_Check(pDX, IDC_FAXAUTOMATICO, m_faxauto);

	DDX_Text(pDX, IDC_NUMEROFAX, m_NumeroFax);
	DDX_Text(pDX, IDC_CORREOFAX, m_CorreoFax);
	DDX_Text(pDX, IDC_CORREOASUNTO, m_CorreoAsunto);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COPIAS, m_crcopias);
	DDX_Check(pDX, IDC_DOBLEHOJA, m_crdoblehoja);   
	DDX_Text(pDX, IDC_DESDEPG, m_crdesdepg);
	DDX_Text(pDX, IDC_HASTAPG, m_crhastapg);

	DDX_Check(pDX, IDC_SELIMPRECRISTAL, m_crselimprecristal);   
	
	
    if (pDX->m_bSaveAndValidate)
    {	    
      GetDlgItemText(IDC_IMPREFONT,m_SFont);
    }
}


BEGIN_MESSAGE_MAP(CDlgOldImpre, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgOldImpre)
	ON_LBN_SELCHANGE(IDC_LISTAOLDIMPRE, OnSelchangeListaoldimpre)
	ON_EN_CHANGE(IDC_LAIMPRE, OnChangeLaimpre)
	ON_EN_KILLFOCUS(IDC_LAIMPRE, OnKillfocusLaimpre)
    ON_BN_CLICKED(IDC_IMPREAJUSTA, OnImpreajusta)
    ON_EN_CHANGE(IDC_MARGENI, OnChangeMargeni)
	ON_BN_CLICKED(IDC_SELIMPRE, OnSelimpre)
	ON_BN_CLICKED(IDC_PRECRYSTAL, Onprecrystal)
	ON_BN_CLICKED(IDC_PRECRYSTAL2, Onpdfcrystal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOldImpre message handlers

BOOL CDlgOldImpre::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();    
	

   {
      LOGFONT tmplf;
      tmplf.lfCharSet = DEFAULT_CHARSET;
      tmplf.lfFaceName[0] = 0;
      tmplf.lfPitchAndFamily = 0;
      //CDC hDC;
      //hDC.CreateDC( LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData );
      CPrintDialog pdlg(FALSE);	  
      theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );

	  m_imprewindows = pdlg.GetDeviceName();
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

   
   UpdateData(FALSE);
   /*
   OnImpreajusta();   
   */

   
    int cursel = 0;    

	m_nrecs = 0;

	m_recs[m_nrecs++] = "Estandard";
	m_lista.AddString("Estandard");
	m_recs[m_nrecs++] = "Pantalla Estandard";
	m_lista.AddString("Pantalla Estandard");
	m_recs[m_nrecs++] = "Excel Estandard";
	m_lista.AddString("Excel Estandard");
	m_recs[m_nrecs++] = "Crystal Reports";
	m_lista.AddString("Crystal Reports");
	m_recs[m_nrecs++] = "Fax";
	m_lista.AddString("Fax (CEFAX)");
	m_recs[m_nrecs++] = "PDF";
	m_lista.AddString("Fichero PDF");
	m_recs[m_nrecs++] = "Correo";
	m_lista.AddString("Correo (pdf)");

    CString dir = theApp.m_dirbase;
    CString s;
    dir += "dev/imp/imp.d";

    FILE *file = fopen(dir,"r");
    char opc[256];
    char tmp[256];

	if (file)
    {
        while(::fgets(opc,80,file))
		 {             
             if (!fgets(tmp,80,file)) break;
             s = tmp;
             s.Remove((char)10);
             s.TrimRight();

             m_recs[m_nrecs] = opc;
             m_recs[m_nrecs].Remove((char)10);
             m_recs[m_nrecs].TrimRight();
             m_nrecs++;             
             m_lista.AddString(s);
		 }
         ::fclose(file);
    }    

	if (m_impre.IsEmpty() || !m_impre.Compare("ibm80"))
	{
		m_impre = theApp.GetProfileString("Defaults","m_impre",m_impre);
	}

	for (int i = 0;i < m_nrecs;i++)
	{
         if (!m_recs[i].Compare(m_impre))
         {
             cursel = i;
         }
	}

    if (cursel < 0 && m_impre.IsEmpty())
    {
        cursel = 0;
    }
	
    if (cursel >= 0)
    {
        m_lista.SetCurSel(cursel);
        OnSelchangeListaoldimpre();
    }

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOldImpre::OnSelchangeListaoldimpre() 
{
    TestImpre();
}

void CDlgOldImpre::OnChangeLaimpre() 
{
    UpdateData(TRUE);
}

void CDlgOldImpre::OnKillfocusLaimpre() 
{
    if (TestImpre())
    {
        CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
        if (wnd)
        {
            wnd->SetFocus();
        }
    }    
}

	
int CDlgOldImpre::TestImpre()
{	
	// Falta
	//IDC_IMPREOEM
	UpdateData(TRUE);
    int item = m_lista.GetCurSel();
	if (item < 0)
	{
		item = 0;
		m_lista.SetCurSel(item);		
	}
    m_impre = m_recs[item];
	UpdateData(FALSE);
	EnableItem(IDC_SPOOLOLDIMPRE,FALSE);
	EnableItem(IDC_IMPREFONT,FALSE);
	EnableItem(IDC_MARGENI,FALSE);
	EnableItem(IDC_MARGENS,FALSE);
	EnableItem(IDC_IMPRELINEAS,FALSE);
	EnableItem(IDC_IMPREAJUSTA,FALSE);
	EnableItem(IDC_IMPRECOLS,FALSE);
	EnableItem(IDC_EXCELCABECERA,FALSE);
	EnableItem(IDC_POSEXCEL,FALSE);
	EnableItem(IDC_MARGENI2,FALSE);
	EnableItem(IDC_IMPREWINDOWS,FALSE);
	EnableItem(IDC_SELIMPRE,FALSE);
	EnableItem(IDC_PRECRYSTAL,FALSE);
	EnableItem(IDC_PRECRYSTAL2,FALSE);

    EnableItem(IDC_FAXAUTOMATICO,FALSE);
	EnableItem(IDC_NUMEROFAX,FALSE);
	EnableItem(IDC_CORREOFAX,FALSE);

	EnableItem(IDC_CORREOASUNTO,FALSE);
	
	EnableItem(IDC_COPIAS,FALSE);
    EnableItem(IDC_DOBLEHOJA,FALSE);
	EnableItem(IDC_DESDEPG,FALSE);
	EnableItem(IDC_HASTAPG,FALSE);


	EnableItem(IDC_SELIMPRECRISTAL,FALSE);

	


	if (item < 7)
	{		
		switch(item)
		{
		case 0:
			EnableItem(IDC_SELIMPRE,TRUE);
			EnableItem(IDC_IMPREFONT,TRUE);
			EnableItem(IDC_MARGENI,TRUE);
			EnableItem(IDC_MARGENI2,TRUE);
			EnableItem(IDC_MARGENS,TRUE);		    
		    EnableItem(IDC_IMPREAJUSTA,TRUE);		    
			EnableItem(IDC_IMPREWINDOWS,TRUE);
			OnImpreajusta();
			break;
		case 2:
			EnableItem(IDC_EXCELCABECERA,TRUE);
			EnableItem(IDC_POSEXCEL,TRUE);		
			break;
		case 3:
			/*
			EnableItem(IDC_SELIMPRE,TRUE);
			EnableItem(IDC_IMPREWINDOWS,TRUE);
			*/
			EnableItem(IDC_PRECRYSTAL,TRUE);
            EnableItem(IDC_PRECRYSTAL2,TRUE);
			if (m_pdfcrystal)
			{
				EnableItem(IDC_SPOOLOLDIMPRE,TRUE);
				CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
				if (wnd)
				{
					wnd->SetReadOnly( FALSE );
				}
			}
			EnableItem(IDC_COPIAS,TRUE);
            EnableItem(IDC_DOBLEHOJA,TRUE);
			EnableItem(IDC_DESDEPG,TRUE);
			EnableItem(IDC_HASTAPG,TRUE);

			if (!m_precrystal)
				EnableItem(IDC_SELIMPRECRISTAL,TRUE);
			else
				EnableItem(IDC_SELIMPRECRISTAL,FALSE);
			break;
		case 4:
			m_check5.SetWindowText("Nº de fax precargado desde el informe:");
			m_label11.SetWindowText("Enviar reporte a:");
			EnableItem(IDC_FAXAUTOMATICO,TRUE);
			EnableItem(IDC_NUMEROFAX,TRUE);
			EnableItem(IDC_CORREOFAX,TRUE);
			EnableItem(IDC_CORREOASUNTO,TRUE);
			break;
		case 5:
			EnableItem(IDC_PRECRYSTAL,TRUE);
			EnableItem(IDC_SPOOLOLDIMPRE,TRUE);
			{
			CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
			if (wnd)
				wnd->SetReadOnly( FALSE );
			}
			break;
		case 6:			
			m_check5.SetWindowText("Correo destino precargado desde el informe:");
			m_label11.SetWindowText("Correo destino:");
			//EnableItem(IDC_PRECRYSTAL,TRUE);
			EnableItem(IDC_FAXAUTOMATICO,TRUE);
			EnableItem(IDC_CORREOFAX,TRUE);
			EnableItem(IDC_CORREOASUNTO,TRUE);
			break;
		default:
			break;
		}		
		UpdateData(FALSE);
		return 0;
	}
	EnableItem(IDC_SPOOLOLDIMPRE,TRUE);

    int spool = 0;
    char tmp[512];
    int *p1 = (int *)tmp;
    *p1 = -1;
    strcpy((char *)(p1+1),m_impre);
    if (theApp.m_9wnd)
	{
		pDsSys->DespachadorInterno9(515,(void *)tmp);
	}
	else
	{
		pDsSys->DespachadorInterno(515,(void *)tmp);
	}
    if (*p1 < 0)
    {
        m_impre = "";
    }
    else
    {
        CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
        if (wnd)
        {
            char *p = (char *)(p1+1);
			if (*p == '@')
				p++;
            if (*p == '*')
            {
                wnd->SetReadOnly( FALSE );
                if (m_spool.IsEmpty())
                {
                    m_spool = m_inispool;
                }
                spool = 1;                
            }
            else
            {
                wnd->SetReadOnly( TRUE );
                //m_spool = "";
            }
        }
    }
    UpdateData(FALSE);

    return spool;
}

void CDlgOldImpre::OnCancel() 
{
	m_impre = "";
    m_spool = "";
    UpdateData(FALSE);
	CBASEDLG::OnCancel();
}


void CDlgOldImpre::Onprecrystal() 
{
	UpdateData();	
	if (!m_precrystal)
		EnableItem(IDC_SELIMPRECRISTAL,TRUE);
	else
		EnableItem(IDC_SELIMPRECRISTAL,FALSE);
}

void CDlgOldImpre::Onpdfcrystal() 
{
	UpdateData();	
	if (m_pdfcrystal)
	{
		EnableItem(IDC_SPOOLOLDIMPRE,TRUE);
		CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
		if (wnd)
		{
			wnd->SetReadOnly( FALSE );
		}
	}
	else
	{
		EnableItem(IDC_SPOOLOLDIMPRE,TRUE);
		CEdit *wnd = (CEdit *)GetDlgItem(IDC_SPOOLOLDIMPRE);
		if (wnd)
		{
			wnd->SetReadOnly( TRUE );
		}
	}
}

void CDlgOldImpre::OnImpreajusta() 
{
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

void CDlgOldImpre::OnChangeMargeni() 
{
   UpdateData(TRUE);
   m_MargenI2 = m_MargenI;
   UpdateData(FALSE);
}

void CDlgOldImpre::OnSelimpre() 
{
   UpdateData(TRUE);
   CPrintDialog pdlg(TRUE);	 
   
   theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );   
   if (pdlg.DoModal() == IDOK)
   {	   
      theApp.SelectPrinter(pdlg.m_pd.hDevNames , pdlg.m_pd.hDevMode, FALSE );
	  m_imprewindows = pdlg.GetDeviceName();
	  UpdateData(FALSE);
   }	
}

BOOL CDlgOldImpre::EnableItem(int item, BOOL enable)
{
   CWnd *pwnd = GetDlgItem(item);
   if (pwnd)
	   pwnd->EnableWindow(enable);
   return enable;
}

void CDlgOldImpre::OnOK() 
{
	UpdateData(TRUE);

	if (!strcmp(m_impre,"Fax"))
	{
		char tmp[512];
		FILE *f;
		sprintf(tmp,"\\\\cefax\\\\envios\\tmp_%d.test",rand());
		f = fopen(tmp,"w");
		if (!f)
		{
			AfxMessageBox("Recurso de red \\\\cefax\\envios inaccesible. Consulte con su administrador de red.",MB_ICONHAND|MB_OK);
			return;
		}
		fclose(f);
		unlink(tmp);
		if (!m_faxauto)
		{
			m_NumeroFax.TrimRight();
			if (m_NumeroFax.IsEmpty())
			{
				AfxMessageBox("Deberá informar el nº de fax por favor.",MB_ICONHAND|MB_OK);
				CEdit *wnd = (CEdit *)GetDlgItem(IDC_NUMEROFAX);
				if (wnd)
				{
					wnd->SetFocus();
				}				
				return;
			}
		}
	}

	if (!strcmp(m_impre,"Correo"))
	{
		if (!m_faxauto)
		{
			m_CorreoFax.TrimRight();
			if (m_CorreoFax.IsEmpty())
			{
				AfxMessageBox("Deberá informar de la direccion de destino, por favor.",MB_ICONHAND|MB_OK);
				CEdit *wnd = (CEdit *)GetDlgItem(IDC_CORREOFAX);
				if (wnd)
				{
					wnd->SetFocus();
				}				
				return;
			}			
		}
	}	

	theApp.WriteProfileInt("Defaults","m_precrystal",(int)m_precrystal);
	theApp.WriteProfileInt("Defaults","m_pdfcrystal",(int)m_pdfcrystal);
	theApp.WriteProfileString("Defaults","m_impre",m_impre);

	theApp.WriteProfileString("Defaults","m_spool",m_spool);
    theApp.WriteProfileInt("Defaults","m_MargenI",(int)m_MargenI);
    theApp.WriteProfileInt("Defaults","m_MargenS",(int)m_MargenS);
    theApp.WriteProfileInt("Defaults","m_Lineas",(int)m_Lineas);
    theApp.WriteProfileInt("Defaults","m_Ajusta",(int)m_Ajusta);
    theApp.WriteProfileInt("Defaults","m_Cols",(int)m_Cols);
    theApp.WriteProfileInt("Defaults","m_cabeceraexcel",(int)m_cabeceraexcel);
    theApp.WriteProfileString("Defaults","m_posexcel",m_posexcel);
    theApp.WriteProfileInt("Defaults","m_MargenI2",(int)m_MargenI2);
	theApp.WriteProfileInt("Defaults","m_faxauto",(int)m_faxauto);
	theApp.WriteProfileString("Defaults","m_NumeroFax",m_NumeroFax);
	theApp.WriteProfileString("Defaults","m_CorreoFax",m_CorreoFax);
	theApp.WriteProfileString("Defaults","m_CorreoAsunto",m_CorreoAsunto);

    theApp.WriteProfileInt("Defaults","m_crcopias",(int)m_crcopias);
    theApp.WriteProfileInt("Defaults","m_crdoblehoja",(int)m_crdoblehoja);
    theApp.WriteProfileInt("Defaults","m_crdesdepg",(int)m_crdesdepg);
    theApp.WriteProfileInt("Defaults","m_crhastapg",(int)m_crhastapg);

	theApp.WriteProfileInt("Defaults","m_crselimprecristal",(int)m_crselimprecristal);
	
	theApp.m_pedidaimpresora = TRUE;

	//EnableItem(IDC_COPIAS,FALSE);
    //EnableItem(IDC_DOBLEHOJA,FALSE);
	//EnableItem(IDC_DESDEPG,FALSE);
	//EnableItem(IDC_HASTAPG,FALSE);

	CBASEDLG::OnOK();
}
