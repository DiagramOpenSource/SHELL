//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxDoc.h"
#include "DsxCntrItem.h"
#include "DsxSrvrItem.h"

#include "DsxMainFrm.h"
#include "DsxLeftView.h"
#include "DsStdDlg.h"
#include "Ds9Window.h"
#include "DsTab9.h"
#include "DsxView.h"
#include "DsTreeMenu.h"
#include "TablaView.h"
#include "DsEdit.h"
#include "DsCombo.h"
#include "StaticLabel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc

IMPLEMENT_DYNCREATE(CDsxDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CDsxDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CDsxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
    ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
    ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleServerDoc::OnUpdatePasteMenu)
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleServerDoc::OnUpdatePasteLinkMenu)
ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleServerDoc::OnUpdateObjectVerbMenu)
ON_COMMAND(ID_OLE_EDIT_CONVERT, COleServerDoc::OnEditConvert)
ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleServerDoc::OnUpdateEditLinksMenu)
ON_COMMAND(ID_OLE_EDIT_LINKS, COleServerDoc::OnEditLinks)
ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleServerDoc::OnUpdateObjectVerbMenu)
ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDsxDoc, COleServerDoc)
	//{{AFX_DISPATCH_MAP(CDsxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDiagramx to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1A2764C8-3127-430E-95F7-ED2A545C62EE}
static const IID IID_IDiagramx =
{ 0x1a2764c8, 0x3127, 0x430e, { 0x95, 0xf7, 0xed, 0x2a, 0x54, 0x5c, 0x62, 0xee } };

BEGIN_INTERFACE_MAP(CDsxDoc, COleServerDoc)
	INTERFACE_PART(CDsxDoc, IID_IDiagramx, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc construction/destruction

CDsxDoc::CDsxDoc()
{

	es_directa = 0;

	m_curView = NULL;
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();



   m_EnWinsysMenu = 0;
   m_DefItem = NULL;

   m_menustd = NULL;
   m_menuser = NULL;   

   m_cfEdit    = NULL;
   m_bEditBk   = NULL;
   m_bEditINBk = NULL;
   m_cfLabel   = NULL;
   m_bLabelBk  = NULL;
   m_cfLabelI  = NULL;
   m_bLabelIBk = NULL;
   m_cfLabelR  = NULL;
   m_bLabelRBk = NULL;
   m_cfGrid    = NULL;
   m_bGridBk   = NULL;
   m_bGridSelBk= NULL;
   m_bGridCamBk= NULL;
   m_cfTitulos = NULL;
   m_cf9Campo  = NULL;
   
   DefaultFonts(FALSE);
   DefaultFonts9(FALSE);


   if (theApp.GetProfileInt("defaults","Skinable",(int)0) >= 2)
   {
		LeeProfileFont(&(m_lfEdit),"lfEdit");
		LeeProfileFont(&(m_lfLabel),"lfLabel");
		LeeProfileFont(&(m_lfLabelR),"lfLabelR");
		LeeProfileFont(&(m_lfLabelI),"lfLabelI");
		LeeProfileFont(&(m_lfTitulos),"lfTitulos");   
		LeeProfileFont(&(m_lfGrid) ,"lfGrid");   

		/* 9 */
		LeeProfileFont(&(m_font9campo) ,"font9campo");
		LeeProfileFont(&(m_font9texto) ,"font9texto");
		LeeProfileFont(&(m_font9rever) ,"font9rever");
		LeeProfileFont(&(m_font9inten) ,"font9inten");
		LeeProfileFont(&(m_font9mensa) ,"font9mensa");
		LeeProfileFont(&(m_font9boton) ,"font9boton");
		LeeProfileFont(&(m_font9edita) ,"font9edita");   
		/*****/
   }


#ifdef DSPROFUIS

   m_cEditBk = (COLORREF)theApp.GetProfileInt("Colores","cEditBk",(int)m_cEditBk);
   m_cEditFr = (COLORREF)theApp.GetProfileInt("Colores","cEditFr",(int)m_cEditFr);

   m_cEditINBk = (COLORREF)theApp.GetProfileInt("Colores","cEditINBk",(int)m_cEditINBk);
   m_cEditINFr = (COLORREF)theApp.GetProfileInt("Colores","cEditINFr",(int)m_cEditINFr);

   m_cLabelRBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelRBk",(int)m_cLabelRBk);
   m_cLabelRFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelRFr",(int)m_cLabelRFr);

   m_cLabelIBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelIBk",(int)m_cLabelIBk);
   m_cLabelIFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelIFr",(int)m_cLabelIFr);

   m_cGridBk = (COLORREF)theApp.GetProfileInt("Colores","cGridBk",(int)m_cGridBk);
   m_cGridFr = (COLORREF)theApp.GetProfileInt("Colores","cGridFr",(int)m_cGridFr);

   m_cGridSelBk = (COLORREF)theApp.GetProfileInt("Colores","cGridSelBk",(int)m_cGridSelBk);
   m_cGridSelFr = (COLORREF)theApp.GetProfileInt("Colores","cGridSelFr",(int)m_cGridSelFr);

   m_cGridCamBk = (COLORREF)theApp.GetProfileInt("Colores","cGridCamBk",(int)m_cGridCamBk);
   m_cGridCamFr = (COLORREF)theApp.GetProfileInt("Colores","cGridCamFr",(int)m_cGridCamFr);   


#else

   m_cEditBk = (COLORREF)theApp.GetProfileInt("Colores","cEditBk",(int)m_cEditBk);
   m_cEditFr = (COLORREF)theApp.GetProfileInt("Colores","cEditFr",(int)m_cEditFr);

   m_cEditINBk = (COLORREF)theApp.GetProfileInt("Colores","cEditINBk",(int)m_cEditINBk);
   m_cEditINFr = (COLORREF)theApp.GetProfileInt("Colores","cEditINFr",(int)m_cEditINFr);

   m_cLabelBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelBk",(int)m_cLabelBk);
   m_cLabelFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelFr",(int)m_cLabelFr);

   m_cLabelRBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelRBk",(int)m_cLabelRBk);
   m_cLabelRFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelRFr",(int)m_cLabelRFr);

   m_cLabelIBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelIBk",(int)m_cLabelIBk);
   m_cLabelIFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelIFr",(int)m_cLabelIFr);

   m_cGridBk = (COLORREF)theApp.GetProfileInt("Colores","cGridBk",(int)m_cGridBk);
   m_cGridFr = (COLORREF)theApp.GetProfileInt("Colores","cGridFr",(int)m_cGridFr);

   m_cGridSelBk = (COLORREF)theApp.GetProfileInt("Colores","cGridSelBk",(int)m_cGridSelBk);
   m_cGridSelFr = (COLORREF)theApp.GetProfileInt("Colores","cGridSelFr",(int)m_cGridSelFr);

   m_cGridCamBk = (COLORREF)theApp.GetProfileInt("Colores","cGridCamBk",(int)m_cGridCamBk);
   m_cGridCamFr = (COLORREF)theApp.GetProfileInt("Colores","cGridCamFr",(int)m_cGridCamFr);

#endif
   
   UpdateFonts(FALSE);   

}

CDsxDoc::~CDsxDoc()
{
   RegistraFonts();

   EraseFonts();
   
   if (pActiveDoc == this)
   {
       if (pMFrame)
           pActiveDoc = (CDsxDoc *)pMFrame->GetActiveDocument();
       else
           pActiveDoc = NULL;
   }

   AfxOleUnlockApp();
}

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc server implementation

COleServerItem* CDsxDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CDsxSrvrItem* pItem = new CDsxSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc Active Document server implementation

CDocObjectServer *CDsxDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}

void CDsxDoc::DefaultFonts9(BOOL update)
{
   /****** 9 ************/
   // Fuente para el texto normal
   HDC hDC;
   
   hDC = ::GetDC(NULL);

#ifdef DSPROFUIS9
   m_font9texto.lfWidth = 0;
   m_font9texto.lfHeight = -MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9texto.lfEscapement = 0;
   m_font9texto.lfOrientation = 0;
   m_font9texto.lfWeight = FW_NORMAL;
   m_font9texto.lfItalic = 0;
   m_font9texto.lfUnderline = 0;
   m_font9texto.lfStrikeOut = 0;
   m_font9texto.lfCharSet = ANSI_CHARSET;
   m_font9texto.lfOutPrecision = OUT_DEFAULT_PRECIS;
   m_font9texto.lfClipPrecision = CLIP_MASK;
   m_font9texto.lfQuality = DRAFT_QUALITY;
   m_font9texto.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;   
   strcpy(m_font9texto.lfFaceName,"Orator10 BT");

   m_font9inten = m_font9texto;
   m_font9campo = m_font9inten;
   m_font9rever = m_font9campo;
   m_font9mensa = m_font9rever;
   m_font9boton = m_font9mensa;	
   m_font9edita = m_font9campo;

   m_font9grid.lfWidth = 0;
   m_font9grid.lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9grid.lfEscapement = 0;
   m_font9grid.lfOrientation = 0;
   m_font9grid.lfWeight = FW_NORMAL;
   m_font9grid.lfItalic = 0;
   m_font9grid.lfUnderline = 0;
   m_font9grid.lfStrikeOut = 0;
   m_font9grid.lfCharSet = ANSI_CHARSET;
   m_font9grid.lfOutPrecision = OUT_DEFAULT_PRECIS;
   m_font9grid.lfClipPrecision = CLIP_MASK;
   m_font9grid.lfQuality = DRAFT_QUALITY;
   m_font9grid.lfPitchAndFamily = FF_DONTCARE;
   strcpy(m_font9grid.lfFaceName,"Arial");

#else

   m_font9texto.lfWidth = 0;
   m_font9texto.lfHeight = -MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9texto.lfEscapement = 0;
   m_font9texto.lfOrientation = 0;
   m_font9texto.lfWeight = FW_NORMAL;
   m_font9texto.lfItalic = 0;
   m_font9texto.lfUnderline = 0;
   m_font9texto.lfStrikeOut = 0;
   m_font9texto.lfCharSet = ANSI_CHARSET;
   m_font9texto.lfOutPrecision = OUT_DEFAULT_PRECIS;
   m_font9texto.lfClipPrecision = CLIP_MASK;
   m_font9texto.lfQuality = DRAFT_QUALITY;
   m_font9texto.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
   strcpy(m_font9texto.lfFaceName,"Courier");

   
   // Fuente para el texto en intensidad
   m_font9inten = m_font9texto;
   m_font9inten.lfWeight = FW_MEDIUM;    
   
   // Fuente para los campos
   
   m_font9campo = m_font9inten;
   strcpy(m_font9campo.lfFaceName,"Courier");
   m_font9campo.lfHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);

   m_font9edita = m_font9campo;
   
   
   // Fuente para el reverse
   m_font9rever = m_font9campo;  
   m_font9rever.lfWeight = FW_BOLD;
   
   // Fuente para los mensajes
   m_font9mensa = m_font9rever;
   m_font9mensa.lfWeight = FW_BOLD;
   m_font9mensa.lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9mensa.lfQuality = PROOF_QUALITY;
   m_font9mensa.lfPitchAndFamily = FF_ROMAN;
   strcpy(m_font9mensa.lfFaceName,"MS Sans Serif");
   
   // Fuente para los botones internos    
   /*
   m_font9boton = m_font9mensa;	
   m_font9boton.lfHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9boton.lfWeight = FW_BOLD;
   strcpy(m_font9boton.lfFaceName,"Times New Roman");
   */


   m_font9grid.lfWidth = 0;
   m_font9grid.lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   m_font9grid.lfEscapement = 0;
   m_font9grid.lfOrientation = 0;
   m_font9grid.lfWeight = FW_NORMAL;
   m_font9grid.lfItalic = 0;
   m_font9grid.lfUnderline = 0;
   m_font9grid.lfStrikeOut = 0;
   m_font9grid.lfCharSet = ANSI_CHARSET;
   m_font9grid.lfOutPrecision = OUT_DEFAULT_PRECIS;
   m_font9grid.lfClipPrecision = CLIP_MASK;
   m_font9grid.lfQuality = DRAFT_QUALITY;
   m_font9grid.lfPitchAndFamily = FF_DONTCARE;
   strcpy(m_font9grid.lfFaceName,"Arial");

   m_font9boton = m_font9grid;	
   m_font9boton.lfHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
#endif

   ::ReleaseDC(NULL,hDC);

   m_9cfondillo = RGB(225,255,255);
   m_9ctextillo = RGB(0,0,0);
   m_9cfondo_seleccion = RGB(0,0,128);
   m_9ctexto_seleccion = RGB(255,255,255);
   m_9cfondo_campo_seleccion = RGB(64,64,64);
   m_9c_custom_subrayado = RGB(0,0,248);
   if (update)
   {
       UpdateFonts(TRUE);
   }
}


#ifdef OLDCOLORES
void CDsxDoc::DefaultFonts(BOOL update)
{   

   HFONT hfont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
   //ANSI_VAR_FONT
   //SYSTEM_FONT

   CFont().FromHandle( hfont )->GetLogFont( &m_lfEdit );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabel );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabelR );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabelI );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfTitulos );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfGrid );

   m_cEditBk = ::GetSysColor(COLOR_WINDOW);
   m_cEditFr = ::GetSysColor(COLOR_WINDOWTEXT);

   m_cEditINBk = ::GetSysColor(COLOR_INFOBK);
   m_cEditINFr = ::GetSysColor(COLOR_INFOTEXT);
   
   m_cLabelBk = ::GetSysColor(COLOR_BTNFACE);
   m_cLabelFr = ::GetSysColor(COLOR_BTNTEXT);

   /*
   m_cLabelRBk = ::GetSysColor(COLOR_INACTIVECAPTION);
   m_cLabelRFr = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
   */
   m_cLabelRBk = RGB(64,128,128);
   m_cLabelRFr = RGB(255,255,255);

   /*
   m_cLabelIBk = ::GetSysColor(COLOR_ACTIVECAPTION);
   m_cLabelIFr = ::GetSysColor(COLOR_CAPTIONTEXT);
   */
   m_cLabelIBk = ::GetSysColor(COLOR_INFOBK);
   m_cLabelIFr = ::GetSysColor(COLOR_INFOTEXT);
   
   m_cGridBk = ::GetSysColor(COLOR_WINDOW);
   m_cGridFr = ::GetSysColor(COLOR_WINDOWTEXT);

   m_cGridSelBk = ::GetSysColor(COLOR_HIGHLIGHT);
   m_cGridSelFr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

   m_cGridCamBk = ::GetSysColor(COLOR_MENU);
   m_cGridCamFr = ::GetSysColor(COLOR_MENUTEXT);

   if (update)
   {
       UpdateFonts(TRUE);
   }
}
#endif


void CDsxDoc::DefaultFonts(BOOL update)
{   

   HFONT hfont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
   //ANSI_VAR_FONT
   //SYSTEM_FONT

   CFont().FromHandle( hfont )->GetLogFont( &m_lfEdit );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabel );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabelR );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfLabelI );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfTitulos );
   CFont().FromHandle( hfont )->GetLogFont( &m_lfGrid );

   m_cEditBk = ::GetSysColor(COLOR_WINDOW);
   m_cEditFr = ::GetSysColor(COLOR_WINDOWTEXT);

   m_cEditINBk = ::GetSysColor(COLOR_INFOBK);
   m_cEditINFr = ::GetSysColor(COLOR_INFOTEXT);
   
   m_cLabelBk = ::GetSysColor(COLOR_BTNFACE);
   m_cLabelFr = ::GetSysColor(COLOR_BTNTEXT);

   /*
   m_cLabelRBk = ::GetSysColor(COLOR_INACTIVECAPTION);
   m_cLabelRFr = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
   */
   m_cLabelRBk = RGB(64,128,128);
   m_cLabelRFr = RGB(255,255,255);

   /*
   m_cLabelIBk = ::GetSysColor(COLOR_ACTIVECAPTION);
   m_cLabelIFr = ::GetSysColor(COLOR_CAPTIONTEXT);
   */
   m_cLabelIBk = ::GetSysColor(COLOR_INFOBK);
   m_cLabelIFr = ::GetSysColor(COLOR_INFOTEXT);
   
   m_cGridBk = ::GetSysColor(COLOR_WINDOW);
   m_cGridFr = ::GetSysColor(COLOR_WINDOWTEXT);

   m_cGridSelBk = ::GetSysColor(COLOR_HIGHLIGHT);
   m_cGridSelFr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

   m_cGridCamBk = ::GetSysColor(COLOR_MENU);
   m_cGridCamFr = ::GetSysColor(COLOR_MENUTEXT);

   if (update)
   {
       UpdateFonts(TRUE);
   }
}


void CDsxDoc::UpdateFonts(BOOL registra)
{
#ifdef DSPROFUIS
   //m_cLabelBk = (COLORREF)theApp.GetProfileInt("Colores","cLabelBk",(int)m_cLabelBk);
   if (theApp.m_colorset6)
		m_cLabelBk = theApp.m_color6;
   else
		m_cLabelBk = g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT );
	   //(COLORREF)g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT );
   //m_cLabelFr = (COLORREF)theApp.GetProfileInt("Colores","cLabelFr",(int)m_cLabelFr);
   if (theApp.m_colorset7)
	    m_cLabelFr = theApp.m_color7;
   else
		m_cLabelFr = (COLORREF)g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_IN );
#endif

   EraseFonts();

   m_cfEdit = new CFont();
   if (theApp.m_ifontcampo > -1) {
		m_cfEdit->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontcampo ]));
   }
   else  
	   m_cfEdit->CreateFontIndirect(&m_lfEdit);

   m_cfLabel = new CFont();
   m_cfLabel->CreateFontIndirect(&m_lfLabel);

   m_cfLabelI = new CFont();
   m_cfLabelI->CreateFontIndirect(&m_lfLabelI);

   m_cfLabelR = new CFont();
   m_cfLabelR->CreateFontIndirect(&m_lfLabelR);

   m_cfTitulos = new CFont();
   m_cfTitulos->CreateFontIndirect(&m_lfTitulos);

   m_cfGrid = new CFont();
   m_cfGrid->CreateFontIndirect(&m_lfGrid);

   m_cf9Campo = new CFont();
   m_cf9Campo->CreateFontIndirect(&m_font9campo);
   
   m_bEditBk = new CBrush();
   m_bEditBk->CreateSolidBrush(m_cEditBk);

   m_bEditINBk = new CBrush();
   m_bEditINBk->CreateSolidBrush(m_cEditINBk);

   m_bLabelBk = new CBrush();
   m_bLabelBk->CreateSolidBrush(m_cLabelBk);

   m_bLabelIBk = new CBrush();
   m_bLabelIBk->CreateSolidBrush(m_cLabelIBk);

   m_bLabelRBk = new CBrush();
   m_bLabelRBk->CreateSolidBrush(m_cLabelRBk);

   m_bGridBk = new CBrush();
   m_bGridBk->CreateSolidBrush(m_cGridBk);

   m_bGridSelBk = new CBrush();
   m_bGridSelBk->CreateSolidBrush(m_cGridSelBk);

   m_bGridCamBk = new CBrush();
   m_bGridCamBk->CreateSolidBrush(m_cGridCamBk);

   //m_cfLabel CFont   
   SIZE ss;
   HFONT oldfont;
   HDC hdc = ::GetDC(NULL);
   oldfont = (HFONT)::SelectObject(hdc,m_cfLabel->m_hObject);
   /*
   TEXTMETRIC tm;
   if (::GetTextMetrics(hdc,&tm))
   {
       tm.
   }
   */
   ::GetTextExtentPoint32(hdc,"XXXXXXXXXX",10,&ss);
   ::LPtoDP(hdc,(POINT *)&ss,1);
   ::SelectObject(hdc,oldfont);   
   ::ReleaseDC(NULL,hdc);

   CDsStdDlg::DsStdX = ss.cx + 1;  

   if (CDsStdDlg::DsStdX < (CDsStdDlg::DsMnStdX+1))
       CDsStdDlg::DsStdX = (CDsStdDlg::DsMnStdX+1);

   CDsStdDlg::DsStdY = ((ss.cy+::GetSystemMetrics(SM_CYBORDER)*2) * 10) + 20;   

   if (CDsStdDlg::DsStdY < (CDsStdDlg::DsMnStdY+20))
       CDsStdDlg::DsStdY = (CDsStdDlg::DsMnStdY+20);

   if ( (CDsStdDlg::DsStdX - CDsStdDlg::DsMnStdX) > 10)
       CDsStdDlg::DsStdOffX = 3;
   else
       CDsStdDlg::DsStdOffX = 0;
   if ( (CDsStdDlg::DsStdY - CDsStdDlg::DsMnStdY) > 30)
       CDsStdDlg::DsStdOffY = 3;
   else
       CDsStdDlg::DsStdOffY = 0;


   if (registra)
   {
        RegistraFonts();
   }


    POSITION pos = GetFirstViewPosition(),pos1,pos2;    
    CDsStdDlg *pdlg;
    CDsStdCtrl*pctrl;
    CWnd *pw;
    int stdid,stctid;
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas                  
       

       pos1 = pView->m_dlglist.GetStartPosition( );
       while( pos1 != NULL)
       {
           pView->m_dlglist.GetNextAssoc( pos1, stdid, pdlg );
           if (pdlg)
           {
               pos2 = pdlg->m_ctrlst.GetStartPosition( );
               while(pos2 != NULL)
               {
                   pdlg->m_ctrlst.GetNextAssoc( pos2, stctid, pctrl);
                   if (pctrl)
                   {
                       pw = (CWnd *)pctrl->m_ctrl;
                       switch(pctrl->m_tipo)
                       {
                       case 2: // edicion
                       case 11:
                           pw->SetFont(m_cfEdit);
                           break;
                       case 1: // label
                       case 9:
                           if (!((CStaticLabel *)pw)->m_group)
                           {
                              if (((CStaticLabel *)pw)->m_BkColor == m_cLabelRBk && ((CStaticLabel *)pw)->m_FrColor == m_cLabelRFr)
                              {
                                  pw->SetFont(m_cfLabelR);
                              }
                              else
                              if (((CStaticLabel *)pw)->m_BkColor == m_cLabelIBk && ((CStaticLabel *)pw)->m_FrColor == m_cLabelIFr)
                              {
                                  pw->SetFont(m_cfLabelI);
                              }
                              else
                              {
                                  pw->SetFont(m_cfLabel);
                              }                                           
                           }
                           break;
                       case 3: // boton
                           pw->SetFont(m_cfLabel);
                           break;
                       case 4: // grid
                           pw->SetFont(m_cfGrid,FALSE);
                           (((CTablaW *)pw)->m_tabla).SetFont(m_cfGrid,FALSE);

                           (((CTablaW *)pw)->m_tabla).SetBkColor( m_cGridBk );
                           (((CTablaW *)pw)->m_tabla).SetTextColor( m_cGridFr );

                           pw->Invalidate();
                           (((CTablaW *)pw)->m_tabla).Invalidate();
                           break;
                       case 6: // list box
                           pw->SetFont(m_cfEdit);
                           break;
                       case 7: // edicion multilinea
                           pw->SetFont(m_cfEdit);
                           break;
                       case 8: // group box (stadard windows)
                           pw->SetFont(m_cfLabel);
                           break;
                       case 999: // tab ctrl
                           pw->SetFont(m_cfTitulos);
                           break;
                       default:
                           break;
                       }
                   }
               }
           }
       }
    }

   if (theApp.m_LeftView)
   {
       CTreeCtrl *m_tree = m_menustd;
       if (m_tree)
       {
#ifdef DSPROFUIS
		   COLORREF colorfondo;
			if (theApp.m_colorset5)
			{
				colorfondo = theApp.m_color5;
			}
			else
			{
				colorfondo = g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT, this
						);
			}
			m_tree->SetBkColor( colorfondo );  
#else
           m_tree->SetBkColor( m_cGridBk );  
           m_tree->SetTextColor( m_cGridFr );
           m_tree->Invalidate();
#endif
       }
   }

   if (theApp.m_UserMenu)
   {
       CTreeCtrl *m_tree = &(((CTreeView *)theApp.m_UserMenu)->GetTreeCtrl());
       if (m_tree)
       {
#ifdef DSPROFUIS
		   COLORREF colorfondo;
			if (theApp.m_colorset5)
			{
				colorfondo = theApp.m_color5;
			}
			else
			{
				colorfondo = g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT, this
						);
			}
			m_tree->SetBkColor( colorfondo );  
#else
           m_tree->SetBkColor( m_cGridBk );  
           m_tree->SetTextColor( m_cGridFr );
           m_tree->Invalidate();
#endif
       }
   }

   
 
}

void CDsxDoc::EraseFonts()
{
   if (m_cfEdit)
      delete m_cfEdit;
   if (m_bEditBk)
      delete m_bEditBk;
   if (m_bEditINBk)
      delete m_bEditINBk;
   if (m_cfLabel)
      delete m_cfLabel;
   if (m_bLabelBk)
      delete m_bLabelBk;
   if (m_cfLabelI)
      delete m_cfLabelI;
   if (m_bLabelIBk)
      delete m_bLabelIBk;
   if (m_cfLabelR)
      delete m_cfLabelR;
   if (m_bLabelRBk)
      delete m_bLabelRBk;
   if (m_cfGrid)
      delete m_cfGrid;
   if (m_bGridBk)
      delete m_bGridBk;
   if (m_bGridSelBk)
      delete m_bGridSelBk;
   if (m_bGridCamBk)
      delete m_bGridCamBk;
   if (m_cfTitulos)
      delete m_cfTitulos;
   if (m_cf9Campo)
      delete m_cf9Campo;
}

void CDsxDoc::RegistraFonts()
{
   GrabaProfileFont(&(m_lfEdit),"lfEdit");
   GrabaProfileFont(&(m_lfLabel),"lfLabel");
   GrabaProfileFont(&(m_lfLabelR),"lfLabelR");
   GrabaProfileFont(&(m_lfLabelI),"lfLabelI");
   GrabaProfileFont(&(m_lfTitulos),"lfTitulos");   
   GrabaProfileFont(&(m_lfGrid) ,"lfGrid");   

   /* 9 */
   GrabaProfileFont(&(m_font9campo) ,"font9campo");
   GrabaProfileFont(&(m_font9texto) ,"font9texto");
   GrabaProfileFont(&(m_font9rever) ,"font9rever");
   GrabaProfileFont(&(m_font9inten) ,"font9inten");
   GrabaProfileFont(&(m_font9mensa) ,"font9mensa");
   GrabaProfileFont(&(m_font9boton) ,"font9boton");
   GrabaProfileFont(&(m_font9edita) ,"font9edita");
   
   /*****/

   theApp.WriteProfileInt("Colores","cEditBk",(int)m_cEditBk);
   theApp.WriteProfileInt("Colores","cEditFr",(int)m_cEditFr);

   theApp.WriteProfileInt("Colores","cEditINBk",(int)m_cEditINBk);
   theApp.WriteProfileInt("Colores","cEditINFr",(int)m_cEditINFr);

   theApp.WriteProfileInt("Colores","cLabelBk",(int)m_cLabelBk);
   theApp.WriteProfileInt("Colores","cLabelFr",(int)m_cLabelFr);

   theApp.WriteProfileInt("Colores","cLabelRBk",(int)m_cLabelRBk);
   theApp.WriteProfileInt("Colores","cLabelRFr",(int)m_cLabelRFr);

   theApp.WriteProfileInt("Colores","cLabelIBk",(int)m_cLabelIBk);
   theApp.WriteProfileInt("Colores","cLabelIFr",(int)m_cLabelIFr);

   theApp.WriteProfileInt("Colores","cGridBk",(int)m_cGridBk);
   theApp.WriteProfileInt("Colores","cGridFr",(int)m_cGridFr);

   theApp.WriteProfileInt("Colores","cGridSelBk",(int)m_cGridSelBk);
   theApp.WriteProfileInt("Colores","cGridSelFr",(int)m_cGridSelFr);

   theApp.WriteProfileInt("Colores","cGridCamBk",(int)m_cGridCamBk);
   theApp.WriteProfileInt("Colores","cGridCamFr",(int)m_cGridCamFr);

   theApp.WriteProfileInt("defaults","Skinable",(int)2);
}


void CDsxDoc::InicializaDoc()
{
  // limpiar variables y poner defectos
  if (theApp.m_LeftView)
  {
      m_menustd = &(((CTreeView *)theApp.m_LeftView)->GetTreeCtrl());
  }
  pActiveDoc = this;
}

void CDsxDoc::NuevoDoc()
{
  // indicar al control de entorno el nuevo documento si es necesario
  // y en su caso, activar el entorno
  if (!pMFrame && theApp.m_pMainWnd)
  {
      pMFrame = (CDsxFrame *)theApp.m_pMainWnd;
  }
  SetTitle("INICIO SESION");
}

BOOL CDsxDoc::OnNewDocument()
{
	
	/*
   if (theApp.m_Embebido)
   {
	   return FALSE;
   }
   */

   if (!COleServerDoc::OnNewDocument())
      return FALSE;  
   
   InicializaDoc();   
   NuevoDoc();
   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDsxDoc commands


void CDsxDoc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
   // TODO: Add your command update UI handler code here
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );
   
}

void CDsxDoc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
   // TODO: Add your command update UI handler code here
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );
   
}

void CDsxDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
   // TODO: Add your command update UI handler code here
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );
   
}

void CDsxDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
   // TODO: Add your command update UI handler code here
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );
   
}

void CDsxDoc::OnUpdateFileSendMail(CCmdUI* pCmdUI) 
{
   // TODO: Add your command update UI handler code here
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );
   
}

BOOL CDsxDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{	

   theApp.m_Abierto = TRUE;   
   
   //if (!theApp.GetFeatureVersion())
   //{
   //   theApp.m_pMainWnd->MessageBox("La versión BASICA no incluye tratamiento de documento");
   //   return FALSE;
   //}  

   InicializaDoc();   
   NuevoDoc();
   if (!COleServerDoc::OnOpenDocument(lpszPathName))
      return FALSE;  
   
   SetModifiedFlag( FALSE );	   

   return TRUE;
}

BOOL CDsxDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	
   if (theApp.m_Embebido || theApp.m_Abierto)
   {	   
	   return COleServerDoc::CanCloseFrame(pFrame);
   }
   
   if (StatusEnWinsysMenu())
   {
       return COleServerDoc::CanCloseFrame(pFrame);
   }

   if (theApp.m_enrun)
   {
	    if (theApp.m_9wnd)
		{			
			pDsSys->DespachadorInterno9(99,NULL);
			if (pDsSys->m_fDespachadorInterno)
			{
				pDsSys->DespachadorInterno(99,NULL);
			}
		}
		else
		{
			pDsSys->DespachadorInterno(99,NULL);
			if (pDsSys->m_f9DespachadorInterno)
			{
				pDsSys->DespachadorInterno9(99,NULL);
			}
		}
        theApp.m_ulttecla = 807;
   }
   else
   {
        AfxMessageBox("La opción en ejecución debe terminar para poder salir");
   }
   return FALSE;   
}

void CDsxDoc::OnCloseDocument() 
{      
	/*
   if (m_metacombo)
   {
        m_metacombo->Muere();
        m_metacombo = NULL;
   }
   */
   COleServerDoc::OnCloseDocument();
}


void CDsxDoc::GrabaProfileFont(LOGFONT *logfont,LPCSTR nombre)
{
   CString id;
   id.Format("%s.Height",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfHeight);
   id.Format("%s.Width",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfWidth);
   id.Format("%s.Escapement",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfEscapement);
   id.Format("%s.Orientation",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfOrientation);
   id.Format("%s.Weight",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfWeight);
   id.Format("%s.Italic",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfItalic);
   id.Format("%s.Underline",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfUnderline);
   id.Format("%s.StrikeOut",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfStrikeOut);
   id.Format("%s.CharSet",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfCharSet);
   id.Format("%s.OutPrecision",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfOutPrecision);
   id.Format("%s.ClipPrecision",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfClipPrecision);
   id.Format("%s.Quality",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfQuality);
   id.Format("%s.PitchAndFamily",nombre);
   theApp.WriteProfileInt("Fonts",id,(int)logfont->lfPitchAndFamily);
   id.Format("%s.FaceName",nombre);
   theApp.WriteProfileString("Fonts",id,logfont->lfFaceName);


   /*

   static int n = 0;

   FILE *file = fopen("c:\\font.txt","a");

   fprintf(file,":%d\r\n",n++);
   fprintf(file,"FaceName %s\r\n",logfont->lfFaceName);
   fprintf(file,"Height %ld\r\n",(int)logfont->lfHeight);   
   fprintf(file,"Width %ld\r\n",(int)logfont->lfWidth);   
   fprintf(file,"Escapement %ld\r\n",(int)logfont->lfEscapement);   
   fprintf(file,"Orientation %ld\r\n",(int)logfont->lfOrientation);   
   fprintf(file,"Weight %ld\r\n",(int)logfont->lfWeight);   
   fprintf(file,"Italic %ld\r\n",(int)logfont->lfItalic);   
   fprintf(file,"Underline %ld\r\n",(int)logfont->lfUnderline);   
   fprintf(file,"StrikeOut %ld\r\n",(int)logfont->lfStrikeOut);   
   fprintf(file,"CharSet %ld\r\n",(int)logfont->lfCharSet);   
   fprintf(file,"OutPrecision %ld\r\n",(int)logfont->lfOutPrecision);   
   fprintf(file,"ClipPrecision %ld\r\n",(int)logfont->lfClipPrecision);   
   fprintf(file,"Quality %ld\r\n",(int)logfont->lfQuality);   
   fprintf(file,"PitchAndFamily %ld\r\n",(int)logfont->lfPitchAndFamily);
   
   fclose(file);

  */
   
}

void CDsxDoc::LeeProfileFont(LOGFONT *logfont, LPCSTR nombre)
{
   CString id;
   id.Format("%s.Height",nombre);
   logfont->lfHeight = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfHeight);
   id.Format("%s.Width",nombre);
   logfont->lfWidth = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfWidth);
   id.Format("%s.Escapement",nombre);
   logfont->lfEscapement = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfEscapement);
   id.Format("%s.Orientation",nombre);
   logfont->lfOrientation = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfOrientation);
   id.Format("%s.Weight",nombre);
   logfont->lfWeight = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfWeight);
   id.Format("%s.Italic",nombre);
   logfont->lfItalic = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfItalic);
   id.Format("%s.Underline",nombre);
   logfont->lfUnderline = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfUnderline);
   id.Format("%s.StrikeOut",nombre);
   logfont->lfStrikeOut = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfStrikeOut);
   id.Format("%s.CharSet",nombre);
   logfont->lfCharSet = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfCharSet);
   id.Format("%s.OutPrecision",nombre);
   logfont->lfOutPrecision = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfOutPrecision);
   id.Format("%s.ClipPrecision",nombre);
   logfont->lfClipPrecision = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfClipPrecision);
   id.Format("%s.Quality",nombre);
   logfont->lfQuality = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfQuality);
   id.Format("%s.PitchAndFamily",nombre);
   logfont->lfPitchAndFamily = (int)theApp.GetProfileInt("Fonts",id,(int)logfont->lfPitchAndFamily);
   id.Format("%s.FaceName",nombre);
   strcpy(logfont->lfFaceName,theApp.GetProfileString("Fonts",id,logfont->lfFaceName));
   
}

/*****************************************************************************************************/

BOOL BuscaComando_Menu(CMenu *menu,UINT comando,CString &nombre)
{
   if (menu->GetMenuString( comando , nombre, MF_BYCOMMAND ) > 0)
   {
	   return TRUE;
   }
   UINT items = menu->GetMenuItemCount();
   for (UINT i = 0;i < items;i++)
   {
      CMenu *otro = menu->GetSubMenu( i );
      if (otro)
      {
         if (BuscaComando_Menu(otro,comando,nombre))
		 {
			 return TRUE;
		 }
      }	  
   }
   return FALSE;
}


static void Destruye_Menu(CMenu *menu)
{
   UINT items = menu->GetMenuItemCount();
   for (UINT i = 0;i < items;i++)
   {
      CMenu *otro = menu->GetSubMenu( i );
      if (otro)
      {
         //otro->RemoveMenu( i , MF_BYPOSITION );
         menu->RemoveMenu( i , MF_BYPOSITION );
         i--;
         items--;
         Destruye_Menu(otro);
      }
   }
   menu->DestroyMenu( );
}

static HMENU Inserta_SubMenus(MenuStruct *menu,int idcounter)
{
   CMenu el_menu;
   el_menu.CreatePopupMenu();
   for (int i = 0;menu->Items[i].NomItem;i++)
   {
      if (menu->Items[i].OpItem[0] == 1 && menu->Items[i].OpItem[1] == 2 && menu->Items[i].OpItem[2] == 3 && menu->Items[i].OpItem[3] == 0)
	     {
         MenuStruct *tmp_m = (MenuStruct *)menu->Items[i].OpItem;
         el_menu.InsertMenu( i, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)Inserta_SubMenus((MenuStruct *)menu->Items[i].OpItem,idcounter) ,(char *)tmp_m->NomMenu );
      }
      else
	     {
         long mi_par;
         memcpy((void *)&(mi_par),menu->Items[i].OpItem+1,sizeof(long));
		 mi_par |= idcounter;
		 /*el_menu.DeleteMenu(MID_XPRIMERA+(UINT)mi_par, MF_BYCOMMAND);*/
         el_menu.InsertMenu( i, MF_BYPOSITION  | MF_STRING,MID_XPRIMERA+(UINT)mi_par/*+(rand()%10) test */,(char *)menu->Items[i].NomItem);
      }
   }
	  
   HMENU este_menu = el_menu.Detach();
   return(este_menu);  
}

/*****************************************************************************************************/

#ifdef OLD_IRA
void CDsxDoc::RefrescaIra()
{
   if (!pMFrame) return;   

   CMenu *menu = pMFrame->GetMenu();
   if (menu) 
   {  
      CMenu *ant_a = pMFrame->GetMenu()->GetSubMenu(3);
      
      CMenu ir_a;
      ir_a.CreatePopupMenu();
      {
         MENUITEMINFO item;
         item.cbSize = sizeof(item);
         item.fMask = MIIM_SUBMENU;
         item.hSubMenu = ir_a.m_hMenu;
         ::SetMenuItemInfo(pMFrame->GetMenu()->m_hMenu,3,TRUE,&item);
      }
      if (ant_a)
         ant_a->DestroyMenu();
      
      char **desc_funs;
      int numero_funs;

      if (theApp.m_9wnd)
		  numero_funs = pDsSys->DespachadorInterno9(1,(void *)&desc_funs);
	  else
		  numero_funs = pDsSys->DespachadorInterno(1,(void *)&desc_funs);
      
      int n = 0;
      for (int i = 0;i < numero_funs;i++)
      {
         if (desc_funs[i])
	     {
            ir_a.InsertMenu(i,MF_STRING | MF_BYPOSITION,823+i,desc_funs[i]);
            n++;
         }
      }
      if (!n)
      {
         ir_a.InsertMenu(0,MF_STRING | MF_BYPOSITION,-1,"No Hay");
      }        
      ir_a.Detach();
      pMFrame->DrawMenuBar();
   }	 
}
#else
#ifdef OLD_IRAMENUPROPIO
void CDsxDoc::RefrescaIra()
{
   if (!pMFrame) return;   

   CMenu *menu = pMFrame->GetMenu();
   if (menu) 
   {  
      CMenu *ant_a = pMFrame->GetMenu()->GetSubMenu(3);
      
      CMenu ir_a;
      ir_a.CreatePopupMenu();
      {
         MENUITEMINFO item;
         item.cbSize = sizeof(item);
         item.fMask = MIIM_SUBMENU;
         item.hSubMenu = ir_a.m_hMenu;
         ::SetMenuItemInfo(pMFrame->GetMenu()->m_hMenu,3,TRUE,&item);
      }
      if (ant_a) 
		  ant_a->DestroyMenu();

	  int n = 0;

      
       CString funcion,s,mif;
	   s = theApp.GetProfileString("MenuPropio","Indice","");
	   int pp,i,aa;	   
	   while(!s.IsEmpty())
	   {
		   funcion = s.Mid(0,5);
		   if (funcion.GetLength() < 5 || funcion[0] != 'M')
			   break;
		   s.Replace(funcion,"");
		   mif = theApp.GetProfileString("MenuPropio",funcion,"");
		   if (!mif.IsEmpty())
		   {
			   pp = mif.Find(10);
			   if (pp > -1)
			   {
				    int j;
					CString s1;
					s1 = mif.Mid(pp+1);
					j = s1.Find(10);

				    i = atoi(((const char *)funcion) + 1);

//					MID_XOPCION1 && nID < MID_ACTUALIZAMENU
					ir_a.InsertMenu(i,MF_STRING | MF_BYPOSITION, MID_XOPCION1+i ,(char *)((const char *)mif) + 1 + pp);
					n++;

					if (i >= theApp.m_nlistadirecta)
					{	
						aa = theApp.m_nlistadirecta;
						theApp.m_nlistadirecta = i+1;
						if (!theApp.m_listadirecta)
						{							
							theApp.m_listadirecta = (char **)malloc(sizeof(char *)*theApp.m_nlistadirecta);
						}
						else
						{							
							theApp.m_listadirecta = (char **)realloc((void *)theApp.m_listadirecta,sizeof(char *)*theApp.m_nlistadirecta);
						}
						for (;aa < theApp.m_nlistadirecta;aa++)
						{
							theApp.m_listadirecta[aa] = NULL;
						}
					}
					mif.SetAt(pp,0);
					if (theApp.m_listadirecta[i])
						free((void *)theApp.m_listadirecta[i]);
					theApp.m_listadirecta[i] = (char *)malloc(strlen(mif)+1);
					strcpy(theApp.m_listadirecta[i],mif);
			   }
		   }
	   }
      
      ir_a.Detach();
      pMFrame->DrawMenuBar();
   }	 
}
#else
void CDsxDoc::RefrescaIra()
{
   if (!pMFrame) return;   

   CMenu *menu = pMFrame->GetMenu();
   if (menu) 
   {  
      CMenu *ant_a = pMFrame->GetMenu()->GetSubMenu(3);
      
      CMenu ir_a;
      ir_a.CreatePopupMenu();
      {
         MENUITEMINFO item;
         item.cbSize = sizeof(item);
         item.fMask = MIIM_SUBMENU;
         item.hSubMenu = ir_a.m_hMenu;
         ::SetMenuItemInfo(pMFrame->GetMenu()->m_hMenu,3,TRUE,&item);
      }
      if (ant_a) 
		  ant_a->DestroyMenu();

	  int n = 0;
	  CString s,ss;
      for (int i = 0;i < 10;i++)
      {

        if (!theApp.m_pFunciones[i].IsEmpty())
        {		
            s = theApp.m_dFunciones[i];
            s.TrimLeft();
            s.TrimRight();
        }
		else
		{
			continue;
		}		
		if (!i)
		{
			ss.Format("(Ctrl-F12)");
		}
		else
		{
			ss.Format("(Ctrl-F%d)",i+1);
		}
		s += ss;
		
        ir_a.InsertMenu(n,MF_STRING | MF_BYPOSITION,833+i,s);
        n++;
      }        
      if (!n)
      {
         ir_a.InsertMenu(0,MF_STRING | MF_BYPOSITION,0,"No Hay");
      }      
      ir_a.Detach();
      pMFrame->DrawMenuBar();
   }	 
}
#endif
#endif

void CDsxDoc::IluminaTreeOpcion(CTreeCtrl *tree)
{
   if (!tree) tree = m_menustd;
   if (!tree) return;

   if (!theApp.m_LeftView) return;

   TV_ITEM tv;
   tv.hItem = tree->GetSelectedItem( );
   tv.iImage = 3;
   tv.iSelectedImage = 3;            
   tv.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;
   tree->SetItem(&tv);
}


void CDsxDoc::Pon_Tree_Items(CTreeCtrl *tree,HTREEITEM Parent,MenuStruct *menu,int defid)
{
   TV_INSERTSTRUCT tvstruct;
   
   tvstruct.hParent = Parent;
   tvstruct.hInsertAfter = TVI_LAST;
   tvstruct.item.iImage = 0;
   tvstruct.item.iSelectedImage = 0;
   tvstruct.item.pszText = (char *)menu->NomMenu;
   tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
   HTREEITEM hThisItem = tree->InsertItem(&tvstruct);
   
   
   TV_ITEM tv;
   tv.hItem = hThisItem;
   tv.mask = TVIF_HANDLE | TVIF_PARAM;
   tv.lParam = 0;   // Los nodos no tienen parametro, no 'ejecutan'
   tree->SetItem(&tv);
   
   
   for (int i = 0;menu->Items[i].NomItem;i++)
   {
      if (menu->Items[i].OpItem[0] == 1 && menu->Items[i].OpItem[1] == 2 && menu->Items[i].OpItem[2] == 3 && menu->Items[i].OpItem[3] == 0)
	     {
         Pon_Tree_Items(tree,hThisItem,(MenuStruct *)menu->Items[i].OpItem,defid);
      }
      else
	     {
         tvstruct.hParent = hThisItem;
         tvstruct.hInsertAfter = TVI_LAST;
         tvstruct.item.iImage = 2;
         tvstruct.item.iSelectedImage = 2;
         tvstruct.item.pszText = (char *)menu->Items[i].NomItem;
         tvstruct.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE ;		 
         
         tv.hItem = tree->InsertItem(&tvstruct);
         tv.mask = TVIF_HANDLE | TVIF_PARAM;
         memcpy((void *)&(tv.lParam),menu->Items[i].OpItem+1,sizeof(long));
         tv.lParam++;
         if ((int)tv.lParam == defid)
         {
            m_DefItem = tv.hItem;
         }
         tree->SetItem(&tv);
      }
	  }
   
}

void CDsxDoc::RellenaMenuStd(MenuStruct *mi_menu, int defid)
{	  
	  static int idcounter = 0; /* ya que el tema de la persistencia de algunos submenus por cmdid al derivarlos por profuis 
							     no se puede resolver rapido, como workaround uso ids diferentes que luego se masquean */
      if (!pMFrame) return;

	  theApp.m_nomstdmenu = mi_menu->NomMenu;	  

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-0]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

#ifdef DSPROFUIS
	  pMFrame->m_dsmenubar.Rellena(mi_menu,defid);	  
#endif
	  
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-1]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

	  CMenu::DeleteTempMap();
      CMenu *menu = pMFrame->GetMenu();
      if (menu) 
      {  
         CMenu *ir_a = pMFrame->GetMenu()->GetSubMenu(5);
         if (ir_a)
         {			 
		    Destruye_Menu(pMFrame->GetMenu()->GetSubMenu(5));
			pMFrame->GetMenu()->RemoveMenu( 5 , MF_BYPOSITION );					
         }
		 CMenu::DeleteTempMap();
         CString s;
         s = "-";
         s += (char *)mi_menu->NomMenu;
         s += "-";
		 int id;

		 id = idcounter++;
		 id &= 0xffff;
		 id <<= 16;

         pMFrame->GetMenu()->InsertMenu( 5, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)Inserta_SubMenus(mi_menu,id) , s );
		 /*
		  test
		 pMFrame->GetMenu()->InsertMenu( 5, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)Inserta_SubMenus(mi_menu) , s );
		 pMFrame->GetMenu()->InsertMenu( 5, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)Inserta_SubMenus(mi_menu) , s );
		 pMFrame->GetMenu()->InsertMenu( 5, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)Inserta_SubMenus(mi_menu) , s );
		 */
      }
	  
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-2]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

      RefrescaIra();
   
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-3]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }


   if (!theApp.m_LeftView) return;
   CTreeCtrl *m_tree = m_menustd;

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-4]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

   if (!m_tree) return;
   
   m_tree->SelectItem(m_tree->GetRootItem());

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-5]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

   m_tree->DeleteAllItems();

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-6]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

   if (mi_menu)
   {
      int idd = defid;
      if (idd < 0) idd = -idd;
      m_DefItem = NULL;
      CMenu *main = pMFrame->GetMenu();
      int single = 0;
      if (main)
      {
         CMenu *popup = main->GetSubMenu( 2 );
         if (popup)
         {
            if ((popup->GetMenuState(ID_SINGLEEXPAND,  MF_BYCOMMAND    ) & MF_CHECKED))
               single = 1;
         }
      }
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-7]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

      m_tree->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT); 
      Pon_Tree_Items(m_tree,NULL,mi_menu,idd);   
      m_tree->Expand(m_tree->GetRootItem(), TVE_COLLAPSE );
      m_tree->SelectItem(NULL);	
      if (single)
      {
         m_tree->ModifyStyle(0,TVS_SINGLEEXPAND);
      }
      else
      {
         m_tree->ModifyStyle(TVS_SINGLEEXPAND,0);	 
      }
      m_tree->Expand(m_tree->GetRootItem(), TVE_EXPAND);
      if (!m_DefItem)
      {
         m_tree->SelectItem(m_tree->GetRootItem());
      }
      else
      {
         m_tree->SelectItem(m_DefItem);
         if (defid < 0)
         {
            IluminaTreeOpcion(m_tree);
         }
      }
   }
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-8]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

#ifdef DSPROFUIS
	  theApp.m_MenuBar->UpdateMenuBar();
#else
	  theApp.m_MenuBar->InitItems();
	  theApp.m_MenuBar->RefreshBar();
#endif
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0-9]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

}

void CDsxDoc::PonEnWinsysMenu(int status)
{

	if (theApp.m_9wnd)
	{
		/*
		if (theApp.m_9wnd->m_tab9)
		{
			if (status)
			{
				theApp.m_9wnd->m_tab9->ShowWindow(SW_HIDE);
			}
			else
			{
				theApp.m_9wnd->m_tab9->ShowWindow(SW_SHOW);
				::SetWindowPos(theApp.m_9wnd->m_tab9->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		else
		{
			if (status)
			{
				theApp.m_9wnd->ShowWindow(SW_HIDE);
			}
			else
			{
				theApp.m_9wnd->ShowWindow(SW_SHOW);
				::SetWindowPos(theApp.m_9wnd->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		*/
	}
	
    if (m_EnWinsysMenu == status)
	{
		return;
	}

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [1-0]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }

    m_EnWinsysMenu = status;


    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas
       pView->Invalidate();
    }

	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [1-1]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }


   if (theApp.m_LeftView)
   {
      if (status)
      {          
          ::SendMessage(theApp.m_LeftView->m_hWnd,WM_COMMAND,ID_FOCUS_TREE,NULL);
      }
      else
      {          
          ::SendMessage(theApp.m_LeftView->m_hWnd,WM_COMMAND,ID_UNFOCUS_TREE,NULL);
      }
   }
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [1-2]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }


}

int CDsxDoc::StatusEnWinsysMenu()
{
   return(m_EnWinsysMenu);
}

void CDsxDoc::OpcionDeMenu(int punto)
{   
   BOOL ejecuta = TRUE; 
   BOOL ocultar = FALSE;

   if (!pMFrame) return;

   theApp.m_opcstdmenu = "";
   if (theApp.m_Embebido)
   {
	   ejecuta = FALSE;
	   ocultar = FALSE;
   }
   else
   {
	   CMenu *main = pMFrame->GetMenu();
	   if (main)
	   {
		  
		  CMenu *popup = main->GetSubMenu( 2 );
		  if (popup)
		  {
			 if (/*!*/(popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
			 {
				// ejecuta = FALSE;
				/*ocultar = FALSE;*/
				  ocultar = TRUE;
			 }
		  }
		  
   	      if (punto > 0)
		  {
			  popup = main->GetSubMenu( 5 );
			  if (popup)
			  {
				 UINT comando;
				 comando = punto+MID_XPRIMERA-1;
				 BuscaComando_Menu(popup,comando,theApp.m_opcstdmenu);
			  }
		  }
	   }   
	   
	   if (StatusEnWinsysMenu())
	   {
		  ejecuta = FALSE;
		  ocultar = FALSE;
	   }
   }
   
   if (punto)
   {
	  int directa = (int)punto;
	  if (directa  < 0)
	  {
		     punto = 0;
			 directa = -(directa+1);
			 if (directa >= theApp.m_nlistadirecta || !pDsSys || !theApp.m_listadirecta[directa] || !theApp.m_listadirecta[directa][0])
			 {				 
				 return;
			 }			 
	  }
	  else
		 directa = -1;

      if (!ejecuta)
      {
	    if (/*!theApp.m_simul9*/ StatusEnWinsysMenu())
		{
			 IluminaTreeOpcion();

			 if (directa >= 0)
			 {
				 es_directa++;
				 HWND hwnd = ::GetFocus();
				 if (theApp.m_9wnd)
				 {
					 theApp.m_9wnd->SetFocus();
				 }
				 if (pDsSys->m_f9DespachadorInterno)			 
				 {
					pDsSys->DespachadorInterno9(16,theApp.m_listadirecta[directa]);
					if (theApp.m_9wnd)
					{
						theApp.m_9wnd->PostMessage(WM_COMMAND,(WPARAM)BUSCA_PTECS,(LPARAM)0);
					}
				 }
				 else if (pDsSys->m_fDespachadorInterno)
					pDsSys->DespachadorInterno(16,theApp.m_listadirecta[directa]);

				 ::SetFocus(hwnd);
				 es_directa--;
				 //theApp.m_UserMenu->SetFocus();
			 }
			 else
			 {
				TeclaStd(MID_XPRIMERA+(unsigned short)(punto-1));
			 }
		}
      }
      else
      {
         // comprobar si no esta embebido etc ...
         char exe[1024];
         char par[1024];
		 CString titulo;

		 memset(exe,0,sizeof(exe));
		 if (theApp.m_9wnd || theApp.m_simul9)
		 {
			 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)punto);
		 }
		 else
		 {
			 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)punto);
		 }

		 char *opdirecta = NULL;		 
		 if (directa >= 0) {
			opdirecta = theApp.m_listadirecta[directa]+1;
		 }
		 else if (es_directa > 0)  {
			char *pp = exe;
			pp += (strlen(exe)+1);
			if (*pp == 7 && *(pp+1) == 7) {
				pp+=2;
				opdirecta = pp;
			}
			else
				return;
		 }

         memcpy(par,"/DS:\"",5);
		 for (int i = 0;par[i];i++)
		 {
			 if (par[i] == 3)
			 {
				 par[i] = 0;
				 titulo = par+i+1;
				 break;
			 }
		 }

		 if (opdirecta)
		 {			 
			int i;
			for (i = 0;par[i];i++)
			{
				if (par[i] == '$' && par[i+1] == '$' && par[i+2] == '$')
				{
					i += 3;
					for(;par[i];i++)
					{
						if (par[i] == '#')
						{
							i++;						
							strcpy(par+i," //");
							strcat(par+i,opdirecta);
							for (i++;par[i];i++)
							{
								if (par[i] == ' ')
									par[i] = '&';
							}
							strcat(par,"\"");
							break;
						}
					}
					break;
				}
			}
		 }		 
		 

		 HWND mihwnd = ::AfxGetMainWnd()->m_hWnd;		 
         strcat(par,"\"");

		 sprintf(par+strlen(par)," /$%lu",(unsigned long)mihwnd);

		 ejecuta = TRUE;
		 CString titulo2;
		 ::AfxGetMainWnd()->GetWindowText(titulo2);

		 HWND hwnd = NULL;

		 if (ocultar && !titulo.IsEmpty())
		 {   
			 titulo = theApp.m_nomstdmenu + " " + theApp.m_opcstdmenu + " " + titulo;
			 if (titulo.Compare(titulo2))
			 {				 
				 hwnd = ::FindWindow(NULL,titulo);			 
				 if (hwnd && hwnd != mihwnd)
				 {
	  				  if (theApp.m_minimizado)
				      {
					 
							::ShowWindow(hwnd,SW_HIDE);
							::ShowWindow(hwnd,SW_RESTORE);
							::ShowWindow(hwnd,SW_SHOW);
							::BringWindowToTop(hwnd);
							::SetForegroundWindow(hwnd);

							::ShowWindow(mihwnd,SW_HIDE);
							::ShowWindow(mihwnd,SW_MINIMIZE);
							::ShowWindow(mihwnd,SW_SHOW);

					  }
					  else
					  {
						 ::BringWindowToTop(hwnd);
						 ::SetForegroundWindow(hwnd);
					  }
					 
					 ejecuta = FALSE;
				 }
			 }
			 else
			 {
				 ejecuta = FALSE;
			 }
		 }
		 if (ejecuta)
		 {
			if ((int)::ShellExecute(NULL, "open",exe,par,NULL, SW_SHOWNORMAL) > 32)
			{
				if (theApp.m_minimizado)
				{
					if (ocultar)
					{					
						::ShowWindow(mihwnd,SW_HIDE);
						::ShowWindow(mihwnd,SW_MINIMIZE);
						::ShowWindow(mihwnd,SW_SHOW);
					}
				}
			}
		 }
      }
   }
}

BOOL CDsxDoc::TeclaStd(int tecla,int wid,int id,int sid,int wcid,int ctipo,Cid cid)
{		

    if (tecla == ID_ANTERIORPANE || tecla == ID_SIGUIENTEPANE)
    {
        return FALSE;
    } 

	{		
		if (tecla >= 833 && tecla < 843)
		{
			/* Ejecucion por tecla de funciuon */
			
			if (!CompruebaTareaTeclaFuncion(tecla))
			{
				int otropid = 0;
				tecla -= 833;
				if (!theApp.m_pFunciones[tecla].IsEmpty())
				{
					if (StatusEnWinsysMenu())
					{					
						if (theApp.m_9wnd)
							pDsSys->DespachadorInterno9(16,(void *)(const char *)theApp.m_pFunciones[tecla]);
						else
							pDsSys->DespachadorInterno(16,(void *)(const char *)theApp.m_pFunciones[tecla]);
					}
					else
					{
						  char exe[1024];
						  char par[1024];
						  int i;
						  if (theApp.m_9wnd || theApp.m_simul9)
						  {
							 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,0);
						  }
						  else
						  {
							 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,0);
						  }
						  memcpy(par,"/DS:\"",5);
						  for (i = 0;par[i];i++)
						  {
								if (par[i] == '$' && par[i+1] == '$' && par[i+2] == '$')
								{
									i += 3;
									for(;par[i];i++)
									{
										if (par[i] == '#')
										{
											i++;						
											strcpy(par+i," //");
											strcat(par+i,((const char *)theApp.m_pFunciones[tecla])+1);
											for (i++;par[i];i++)
											{
												if (par[i] == ' ')
													par[i] = '&';
											}
											strcat(par,"\"");
											break;
										}
									}
									break;
								}
						  }
						  BOOL ocultar = FALSE;
						  {							  
							   CMenu *main = pMFrame->GetMenu();
							   if (main)
							   {		  
								  CMenu *popup = main->GetSubMenu( 2 );
								  if (popup)
								  {
									 if ((popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
									 {
										ocultar = TRUE;
									 }
								  }
							   }
						  }
						  {				
							STARTUPINFO si;
							PROCESS_INFORMATION ProcessInformation; 
							char cmd[1024];

							memset((void *)&ProcessInformation,0,sizeof(ProcessInformation));
							memset((void *)&si,0,sizeof(si));
							si.cb = sizeof(STARTUPINFO); 
							si.lpReserved = NULL;
							si.lpTitle = NULL; 
							si.lpDesktop = NULL;
							si.dwX = si.dwY = si.dwXSize = si.dwYSize = 0L; 
							si.dwFlags = STARTF_USESHOWWINDOW; 
							si.wShowWindow = SW_SHOW; 
							si.lpReserved2 = NULL; 
							si.cbReserved2 = 0; 

							strcpy(cmd,exe);							
							strcat(cmd," ");
							strcat(cmd,par);
 
							if (CreateProcess(NULL,cmd, NULL, NULL, FALSE, 
											   0, NULL, NULL, &si , &ProcessInformation)) 
							{ 
								otropid = (int)ProcessInformation.dwProcessId;
								CloseHandle(ProcessInformation.hProcess); 
								CloseHandle(ProcessInformation.hThread); 
								if (theApp.m_minimizado)
								{
									if (ocultar)
									{
										HWND lamia = theApp.m_pMainWnd->m_hWnd;
										::ShowWindow(lamia,SW_HIDE);
										::ShowWindow(lamia,SW_MINIMIZE);
										::ShowWindow(lamia,SW_SHOW);
									}
								}
							} 						  

						  }
					}
					/*
					{
					  CString funcion;
					  funcion.Format("Task_%03d",tecla+833);
					  theApp.WriteProfileInt("Funciones",funcion,otropid);
					}
					*/
					return TRUE;
				}
				tecla += 833;
			}
			else
			{
				if (!theApp.m_pFunciones[tecla-833].IsEmpty())
					return TRUE;
			}
		}
	}

	/* habra que distinguir si el mensaje es para el 9 o no */
	if (/*id == 0 &&*/ theApp.m_9wnd) 
	{
		BOOL paranueve = FALSE;
		if ((::IsWindowVisible(theApp.m_9wnd->m_hWnd) || StatusEnWinsysMenu())/* == ::GetFocus()*/ || tecla == 802 || tecla == 807 || tecla == 806 ||  tecla > 999 || tecla == 822)
		{
			paranueve = TRUE;
		} else if (wcid != -1)
		{
            CDsStdCtrl *pw = NULL;              
            if (CDsStdDlg::m_todosctrl.Lookup(wcid,pw) && pw && pw->m_pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window)))
            {
				paranueve = TRUE;
			}
		}
		else if (ctipo == 9990999)
		{
			paranueve = TRUE;
		}

		if (paranueve)
		{
			/***** 9 ******/
			if (tecla == 822)
				tecla = 823;
			else if (tecla == 823)
				tecla = 822;
			if (tecla > 0 && tecla < 800)
			{				
				::PostMessage(theApp.m_9wnd->m_hWnd,WM_CHAR,tecla,(LPARAM)NULL);
			}
			else
			{
				::PostMessage(theApp.m_9wnd->m_hWnd,WM_COMMAND,tecla,(LPARAM)NULL);
			}
			return TRUE;
		}
	}

	if (tecla == TECLA_PING && theApp.m_ulttecla != -1)
		return FALSE;
	
	if (tecla != 999 && theApp.m_ulttecla == 999)
		return FALSE;
	/***** 10 *****/
	theApp.m_ulttecla = tecla;

	theApp.m_ult_wid  = wid;
	theApp.m_ult_id   = id;
	theApp.m_ult_sid  = sid;
	theApp.m_ult_wcid = wcid;
	theApp.m_ult_ctipo = ctipo;
	theApp.m_ult_cid  = cid;
	return TRUE;
}

void CDsxDoc::PonFoco()
{
    // habra que tener cuenta exacta del elemento poseedor del foco para el documento
}

void CDsxDoc::ControlDePantallas(int operacion, void *datos)
{
	BOOL Usar9 = FALSE;
	int def_id = -1;
	if (operacion == 999) {
		int wid = *((int *)datos);
		CDsStdDlg *pdlg = NULL;
		if (m_curView) {
			m_curView->m_dlglist.Lookup(wid,pdlg);
			if (pdlg != NULL) {
				CRect r(0,0,0,0);  
				pdlg->EnsureVisible(&r);
			}
		}
		return;
	}
	if (operacion == -10)
	{
		operacion = 2;
		Usar9 = TRUE;
	}
	else
	{
		if (operacion == 2 || operacion == 12)
		{
			def_id = *((int *)datos) & 0xffffff00;
			if ( (def_id & 0x80000000) || !(def_id & 0x40000000) )
				def_id = -1;		
			else
			{
				*((int *)datos) &= 0xff;
				if (*((int *)datos) == 255)
				{
					*((int *)datos) = -1;
				}
			}
		}
	}
    if (!operacion || operacion == 2 || operacion == 10 || operacion == 12) // crear ventana
    {        
        CRect rect;
        CDsStdDlg *pdlg = NULL;
        int wid = -2;
        int conreplica = 1;
        if (operacion >= 10)
        {
            operacion -= 10;
            conreplica = 0;
        }

        POSITION pos = GetFirstViewPosition();
		CDsxView* pView;
        while (pos != NULL)
        {  
		   if (m_curView)
		   {
			   pView = m_curView;
		   }
		   else
		   {
			   pView = (CDsxView *)GetNextView(pos);
			   if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
			   m_curView = pView;
		   }	   

           int *p1 = (int *)datos;		   

           if (conreplica && p1[0] > 1 && !p1[1])
           {
               pdlg = pView->BuscaStdDlg(p1[0]-1,0,wid);
               if (pdlg)
               {
                   pdlg->m_replicante++;
               }
               break;
           }
           pView->GetClientRect(rect);
		   if (Usar9)
		   {
			   pdlg = (CDsStdDlg *)new CDs9Window();			   
		   }
		   else
		   {
			   pdlg = new CDsStdDlg();
		   }
            int stdid;
		    stdid = def_id;
           if (!pdlg->Create(datos,pView,stdid))
           {
               delete pdlg;
           }
           else
           {
               if (wid == -2)
                  wid = stdid;
               else
                  wid = -1;
               if (!operacion)
               {
                   pdlg->ShowWindow(SW_SHOW);
                   if (pdlg->m_sid_padre)
                   {
                        ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
                   }
                   ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
               }
			   if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_ctrl && ::IsWindow(theApp.m_ctrlactivo->m_ctrl->m_hWnd))
			   {
				   pdlg->m_IdAntFoco = theApp.m_ctrlactivo->m_ctrlid;
			   }			    
           }
		   break;
        }
        if (wid == -2) wid = -1;
        *((int *)datos) = wid;
	    //if (theApp.m_ult_wid == -1)
		{			
			theApp.m_ult_wid  = wid;
			if (pdlg && pdlg->m_wid == wid)
			{
				theApp.m_ult_id   = pdlg->m_id;
				theApp.m_ult_sid  = pdlg->m_sid;			
			}
		}
    }
    else if (operacion == 1)
    {
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas
           int *p = (int *)datos;
           int stdid;
           CDsStdDlg *pdlg;
           if ((pdlg = pView->BuscaStdDlg(*p,*(p+1),stdid)))
           {
			   int oldid = pdlg->m_IdAntFoco;
			   p[0] = pdlg->m_sid;
			   p[1] = -1;
			   pdlg->BorraDescendencia(p+1);
               /*
               if (pdlg->m_id != *p)
               {
                   if (pdlg->m_replicante)
                       pdlg->m_replicante--;
               }
               else
               {
               */
                   pView->EliminaStdDlg(stdid);
               /*}*/
			   if (oldid > -1)
			   {
				   CDsStdCtrl *pctrl;
				   if (CDsStdDlg::m_todosctrl.Lookup(oldid,pctrl))
				   {
					   pctrl->SetFocus();					   
				   }
			   }
           }
        }
    }
    else
    if (operacion == 4 || operacion == 5)
    {        
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) 
			   continue; // aqui solo las Diagram9View de pantallas           
           int *p = (int *)datos;           
           CDsStdDlg *pdlg = pView->ActivaDlg(p[0],p[1],p[2],(operacion == 4) ? TRUE : FALSE );
           int  wid = -1;
           if (pdlg)
		   {
               wid = pdlg->m_wid;
			   
			   if (!p[2] && p[3] > 2480 && p[3] < 9999)
			   {				   
				   pdlg->m_lpf = p[3];/* - pdlg->m_lpi) + 101;*/
				   pdlg->Crece(TRUE);
			   }	
			   //if (theApp.m_ult_wid == -1)
			   {
					theApp.m_ult_wid  = p[0];
					theApp.m_ult_id   = p[1];
					theApp.m_ult_sid  = p[2];
			   }

		   }
           *((int *)datos) = wid;
        }
    }
    else
    if (operacion == 6)
    {
        // PUSHV!!!
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
           int *p = (int *)datos;           
           int stdid;
           CDsStdDlg *pdlg = pView->BuscaStdDlg(p[0],0,stdid);
           if (pdlg)
           {
               pdlg->PushV(p[1],p[2]);
           }
        }
    }
    else
    if (operacion == 7)
    {
        // POPV
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
           int *p = (int *)datos;
           int stdid;
           CDsStdDlg *pdlg = pView->BuscaStdDlg(p[1],0,stdid);
           if (pdlg)
           {
               pdlg->PopV(p[2],p);
           }
        }
    }
    else
    if (operacion == 8)
    {
        // borrar descendencia ...
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
           int *p = (int *)datos;
           CDsStdDlg *pdlg;

           /*
           if (p[4] != -1 && p[5] != -1)
           {
               pdlg = pView->LocalizaDlg(-1,p[2],0,-1,-1);
           }
           else
           */
           {
                pdlg = pView->LocalizaDlg(p[1],p[2],p[3],-1,-1);
           }
           if (pdlg)
           {
               pdlg->BorraDescendencia(p,p[4],p[5]);
               break;
           }
        }        
    }
    else
    if (operacion == 9) /* scroll! */
    {
        int *p = (int *)datos;
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
           int *p = (int *)datos;           
           int stdid;
           CDsStdDlg *pdlg = pView->BuscaStdDlg(p[1],p[2],stdid);
           if (pdlg)
           {
               pdlg->ScrollCtrls(p[3],p[4],p[5]);
           }
        }
    }
}

int CDsxDoc::InsertaControl(int wid,int id,int sid,int tipo, int lpos,int lpf,void *datos,int Atrib, int Cc, int Cf,int prev_id)
{
        int r = -1;
        int otro_r = -1;
		if (m_curView)
		{
           r = m_curView->InsertaControl(wid,id,sid,tipo,lpos,lpf,datos,Atrib,Cc,Cf,prev_id);
           if (r != -1 && otro_r == -1)
           {
               otro_r = r;
           }
		   if (otro_r == r)
				return r;
		   else
				return -1;
		}
        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           

		   m_curView = pView;

           r = pView->InsertaControl(wid,id,sid,tipo,lpos,lpf,datos,Atrib,Cc,Cf,prev_id);
           if (r != -1 && otro_r == -1)
           {
               otro_r = r;
           }
        }

        if (otro_r == r)
            return r;
        else
            return -1;
}

int CDsxDoc::ControlEdicion(int wid, int id, int sid, int cwid, Cid cid, int flag,void *datos,int pre_id)
{
        int r = -1;
        int otro_r = -1;

        if (cwid != -1)
        {
              CDsStdCtrl *pw = NULL;              
              if (CDsStdDlg::m_todosctrl.Lookup(cwid,pw) && pw)
              {                  
                  int *p1 = (int *)datos;
                  if (pw->m_pdlg->m_id == id && pw->m_pdlg->m_sid == sid  && (p1[0] == 11 || CDsStdDlg::ComparaCid(pw->m_cid,cid,FALSE)))
                  {
                     return pw->ControlEdicion(flag,datos,pre_id);
                  } 
				  /*
                  else
                  {
                     
                     ASSERT(FALSE);
                  } 
				  */                  
              }
        }

        POSITION pos = GetFirstViewPosition();
        while (pos != NULL)
        {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas 

           r = pView->ControlEdicion(wid,id,sid,cwid,cid,flag,datos,pre_id);
           if (r != -1 && otro_r == -1)
           {
               otro_r = r;
           }
        }
        if (otro_r == r)
            return r;
        else
            return -1;
}

int CDsxDoc::CreaGrid(void *datacontrol)
{
      char *p = (char *)datacontrol;        
      /*int g_modo;*/
      int p1,p2,wid,id,sid;

      p += sizeof(long)*3;
      
      p += sizeof(int);
      
      p += sizeof(long);
      
      p += sizeof(long);
      
      p += (strlen(p) + 1);

      /*g_modo = *((int *)p);*/

      p += sizeof(int);

      p1 = *((int *)p);
      p += sizeof(int);
      p2 = *((int *)p);
      p += sizeof(int);

      wid = *((int *)p);
      p += sizeof(int);
      id = *((int *)p);
      p += sizeof(int);
      sid = *((int *)p);
      p += sizeof(int);


      int r = -1;
      POSITION pos = GetFirstViewPosition();
      while (pos != NULL)
      {           
           CDsxView* pView = (CDsxView *)GetNextView(pos);
           if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           

           r = pView->InsertaControl(wid,id,sid,4,p1,p2,datacontrol);
           if (r != -1)
           {
               break;
           }
      }

      return r;
}



int CDsxDoc::ControlBinario(int operacion, void *datacontrol)
{
    switch(operacion)
       {
       case 0: // CREA GRID!!!
		   {
              return CreaGrid(datacontrol);
		   }
       case 1: // Destruye GRID!!    
           {
               int id_grid;
               memcpy((void *)&id_grid,(void *)(((char *)datacontrol)+sizeof(long)*3),sizeof(int));
               CDsStdCtrl *pctrl;
               if (CDsStdDlg::m_todosctrl.Lookup(id_grid,pctrl))
               {
				   CTablaW *ptabla = (CTablaW *)pctrl->m_ctrl;
				   if ((ptabla->m_gmodo & 0x1000000) && theApp.m_9wnd)
				   {
					   theApp.m_9wnd->SetFocus();
				   }
                   CDsStdDlg *pdlg = pctrl->m_pdlg;
                   if (pdlg->DestruyeControl(id_grid,TRUE))
                   {					    
                        return id_grid;
                   }
               }               
           }
         break;
       case 2: // Pintar campo en GRID
           {                              
               int id_grid,id_campo;
               memcpy((void *)&id_grid,(void *)(((char *)datacontrol)+sizeof(long)*3),sizeof(int));
               memcpy((void *)&id_campo,(void *)(((char *)datacontrol)+sizeof(long)*3+sizeof(int)),sizeof(int));
               CDsStdCtrl *pctrl;
               if (CDsStdDlg::m_todosctrl.Lookup(id_grid,pctrl))
               {
                   CTablaW *ptabla = (CTablaW *)pctrl->m_ctrl;
                   if (ptabla)
                   {
                       ptabla->m_tabla.PintaIdCampo(id_campo,(char *)(((char *)datacontrol)+sizeof(long)*3+sizeof(int)*2));
                   }
               }

               /*
               if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed)
	           {
                   theApp.m_EntornoActivo->m_wallowed->PintaCampo(IDW_WGRID,id_grid,id_campo,(char *)(((char *)puntero)+sizeof(long)*3+sizeof(int)*2));
               }
               */
           }
         break;
       case 3:
       case 4:
       case 5:
           {   // refrescar grid
               int id_grid;
               long regs;
               memcpy((void *)&id_grid,(void *)(((char *)datacontrol)+sizeof(long)*3),sizeof(int));           
               memcpy((void *)&regs,(void *)(((char *)datacontrol)+sizeof(long)*3+sizeof(int)),sizeof(long));
               CDsStdCtrl *pctrl;
               if (CDsStdDlg::m_todosctrl.Lookup(id_grid,pctrl))
               {
                   CTablaW *ptabla = (CTablaW *)pctrl->m_ctrl;
                   if (ptabla)
                   {
                       int nuevo_cur = -1;

                       if (!ptabla->m_tabla.m_registros && regs)
                           nuevo_cur = 0;

					   ptabla->m_tabla.SetRedraw(FALSE);
					   //ptabla->m_tabla.LockWindowUpdate();
                       /*if (operacion > 3)*/
                         ptabla->m_tabla.BorraCache();
                       ptabla->m_tabla.PonRegistros(regs);                      

                       if (operacion == 5)
                       {
                           nuevo_cur = regs-1;                           
                           ptabla->m_tabla.m_curcol = 1;
                       }

                       if (nuevo_cur != -1)
                       {
                           ptabla->m_tabla.NuevoCorriente(nuevo_cur,TRUE);
                           //ptabla->m_tabla.m_cursel = 
                       }

					   ptabla->m_tabla.SetRedraw(TRUE);
					   //ptabla->m_tabla.UnlockWindowUpdate();
					   ptabla->m_tabla.ActualizaVentana();
					   //ptabla->m_tabla.InvalidateRect(NULL);
                       if (operacion > 3 || nuevo_cur != -1)
                       {
                           ptabla->m_tabla.SetFocus();
                       }					   
                       return id_grid;
                   }
               }
           }
           break;
       case 6: /* recogida de datos */
           {
               int id_grid;
               char *data = (char *)datacontrol;
               data += (sizeof(long)*3);
               memcpy((void *)&id_grid,data,sizeof(int));
               data += sizeof(int);
               CDsStdCtrl *pctrl;
               if (CDsStdDlg::m_todosctrl.Lookup(id_grid,pctrl))
               {
                   CTablaW *pw = (CTablaW *)pctrl->m_ctrl;
                   if (pw->m_tabla.m_dsedit)
                   {
                       int *p = (int *)data;
                       *p = pw->m_tabla.m_dsedit->GetWindowTextLength( );
                       pw->m_tabla.m_dsedit->GetWindowText((char *)(p+1),250);					   
					   for (char *kp = (char *)(p+1);*kp;kp++)
					   {          
						  ansi_a_oem((unsigned char *)kp);
					   }
                       /*
                       pw->m_tabla.m_dsedit->DestroyWindow();
                       delete pw->m_tabla.m_dsedit;
                       */
                       pw->m_tabla.m_dsedit = NULL;
                       pw->m_tabla.m_editando = FALSE;                       
                   }
               }
           }
         break;
	   case 7:
		   /* control de ventana, a mejorar, implementado asi por prisas */
		   {
               CDsStdDlg *pdlg = NULL;
               CDsxView* pView = NULL;
               POSITION pos = GetFirstViewPosition();
               while (pos != NULL)
               {           
                   pView = (CDsxView *)GetNextView(pos);                   
				   if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; 
                   break;
               }
               if (!pView) break;

               char *data = (char *)datacontrol;
               data += (sizeof(long)*3);
               int *pid = (int *)data;
			   data += (sizeof(int)*3);
			   pdlg = pView->LocalizaDlg(pid[0],pid[1],pid[2],-1,-1);
			   if (pdlg)
			   {
				   pid[0] = pdlg->OpcionesVentana(data);
			   }
			   else
				   pid[0] = -1;
		   }
	     break;
       case 10: /* ocx */
           {
               CDsStdDlg *pdlg = NULL;
               CDsxView* pView = NULL;
               POSITION pos = GetFirstViewPosition();
               while (pos != NULL)
               {           
                   pView = (CDsxView *)GetNextView(pos);                   
				   if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
                   break;
               }
               if (!pView) break;

               long id_ocx;
               long operacion;
			   int pi,pf;
               char *data = (char *)datacontrol;
               data += (sizeof(long)*3);
               long *pidocx = (long *)data;
               id_ocx = pidocx[0];
               operacion = pidocx[1];
			   pi = pidocx[4];
			   pf = pidocx[5];

               pdlg = pView->LocalizaDlg(-1,(int)pidocx[2],(int)pidocx[3],(int)pidocx[4],(int)pidocx[5]);
               if (!pdlg)
               {
                   break;
               }
               //
               data += (sizeof(long)*6);
               switch(operacion)
               {
               case 1:
                   id_ocx = pdlg->InsertaControl(12,pdlg->m_lpi,pdlg->m_lpi, (void *)data);
                   pidocx[0] = id_ocx;
                   break;
               case 2:
                   pdlg->DestruyeControl((int)id_ocx,TRUE);
                   break;
               case 3:
                   {
                       CDsStdCtrl *pctrl;
                       int idd = (int)id_ocx;
					   pidocx[0] = -1;
					   HDC hdc = NULL;
					   HWND hwnd = NULL;
					   long x = 0,y = 0,dx = 0,dy = 0;
                       if (CDsStdDlg::m_todosctrl.Lookup(idd,pctrl))
                       {
                           CDsOCX *w1;
                           void *parametros = (void *)(data+1+*data);
						   RECT rect;
						   if (pi != -1 && pf != -1)
						   {							   
							   char tmp[512];

							   hwnd = pdlg->m_hWnd;

							   if (pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window)))
							   {
									rect.left = pi;
									rect.right = 0;
									rect.top  = pf;
									rect.bottom = 0;
									memcpy(tmp,(void *)&rect,sizeof(RECT));
									pDsSys->DespachadorInterno9(15,(void *)tmp);							   
									memcpy((void *)&rect,tmp,sizeof(RECT));
									memcpy((void *)&hdc,tmp+sizeof(RECT),sizeof(HDC));
									x = rect.left;
									y = rect.top;
									dx = rect.right-rect.left;
									dy = rect.bottom-rect.top;							   
							   }
							   else
							   {
									x = pdlg->m_offx + ((pi%100) *  pdlg->m_stdx)/10;
									y = pdlg->m_offy + ((pi/100) *  pdlg->m_stdy)/10;
									dx = (((pf%100)-(pi%100)+1) * pdlg->m_stdx)/10;
									dy = (((pf/100)-(pi/100)+1) * pdlg->m_stdy)/10;
							   }
						   }						   						   
                           w1 = (CDsOCX *)pctrl->m_ctrl;                           
						   if (w1->Invoca(data+1,hwnd,x,y,dx,dy,parametros))
						   {
							   pidocx[0] = id_ocx;							   
						   }
						   if (hdc && hwnd)
						   {
							   HDC whdc = ::GetDC(hwnd);
							   BitBlt(hdc, x, y, dx, dy, whdc, x, y, SRCCOPY);
							   ::ReleaseDC(hwnd,whdc);
						   }
                       }
                   }
                   break;
               default:
                   break;
               }
           }
         break;
	   case 100: // Interface externo;
		 CargaInterface((char *)datacontrol);
		 break;
       default:
         break;
       }


  return -1;
}


int CDsxDoc::ControlDeUsuario(int id, int sid, char *nom, int pi, int pf, void *dato,int prev_id)
{
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           

       CDsStdDlg *pdlg = pView->LocalizaDlg(-1,id,sid,pi,pf);
       if (pdlg)
       {
           return pdlg->ControlUsuario(nom,pi,pf,dato,prev_id);
       }       
    }
    return -1;    
}

int CDsxDoc::ErrorFichero(char *data1, char *data2, char *data3, int ok)
{
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
       return pView->ErrorFichero(data1,data2,data3,ok);
    }
    return -1;
}

BOOL CDsxDoc::FinErrorFichero()
{
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
       return pView->FinErrorFichero();
    }
    return FALSE;
}


int CDsxDoc::ControlDialogo(int modo,char *datos)
{    
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {           
       CDsxView* pView = (CDsxView *)GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) continue; // aqui solo las Diagram9View de pantallas           
       int *p = (int *)datos;           
       int stdid;
       CDsStdDlg *pdlg = pView->BuscaStdDlg(p[1],p[2],stdid);
       if (pdlg)
       {
           return pdlg->ControlDialogo(modo,datos);
       }
    }
    return 0;
}

static int ajustafuente(LOGFONT *pfont,int maxx,int maxy,int topey = -1)
{
	int size;
	int factor = -1;
	int umaxx = maxx,umaxy = maxy;
    HFONT Font;
    HFONT ant_Font;

	HDC hDC = ::GetDC(theApp.m_9wnd->m_hWnd);
	::SetMapMode(hDC,MM_TEXT);
	::SetBkMode(hDC,TRANSPARENT);
	::SetTextAlign(hDC,TA_NOUPDATECP | TA_LEFT );

	size = -1;
	if (size > 0)
		factor = 1;
	//m_font9texto.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	pfont->lfHeight = size;
	do
	{			    
		Font = CreateFontIndirect(pfont);		   
		ant_Font = (HFONT)::SelectObject( hDC, Font );
		TEXTMETRIC tm;
		CSize scaracter;
		if (::GetTextMetrics(hDC,&tm))
			{
				scaracter.cy = tm.tmHeight;
				scaracter.cx = tm.tmAveCharWidth; 
			}
			else
				::GetTextExtentPoint(hDC,"M",1,&scaracter);
		::SelectObject( hDC, ant_Font );
		::DeleteObject(Font);

		if (scaracter.cy > 32 || scaracter.cx > 14)
			break;

		if (topey != -1 && scaracter.cy > topey)
			break;

		if (((scaracter.cx * 80)+16) > maxx)
			break;
		if (((scaracter.cy * 22)) > maxy)
			break;

		size = pfont->lfHeight;
		umaxx = (scaracter.cx * 80)+16;
		umaxy = scaracter.cy * 22;

		pfont->lfHeight += factor;
	} while(-1);

	::ReleaseDC(theApp.m_9wnd->m_hWnd,hDC);

	pfont->lfHeight = size;

	size = (int)MAKELONG((WORD)umaxx,(WORD)umaxy);

	return size;
}

BOOL CDsxDoc::PideFuente9(UINT nID)
{
   LOGFONT *pfont = NULL;
   BOOL Fixed = TRUE;
   HDC hDC;

   switch(nID)
   {
   case ID_AJUSTARPANTALLA9_125:
	       hDC = ::GetDC(NULL);
		   m_font9texto.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9campo.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9rever.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9inten.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9edita.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		   
		   ::ReleaseDC(NULL,hDC);

	       UpdateFonts();
		   pDsSys->NuevaVentana9(this,true);
		   return TRUE;
   case ID_AJUSTARPANTALLA9_150:
		   hDC = ::GetDC(NULL);
		   m_font9texto.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9campo.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9rever.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9inten.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9edita.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   ::ReleaseDC(NULL,hDC);

	       UpdateFonts();
		   pDsSys->NuevaVentana9(this,true);
		   return TRUE;
   case ID_PANTALLA9_175:
	       hDC = ::GetDC(NULL);
		   m_font9texto.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9campo.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9rever.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9inten.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9edita.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		   ::ReleaseDC(NULL,hDC);

	       UpdateFonts();
		   pDsSys->NuevaVentana9(this,true);
		   return TRUE;
   case ID_PANTALLA9_200:
	       hDC = ::GetDC(NULL);
		   m_font9texto.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9campo.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9rever.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9inten.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   m_font9edita.lfHeight = -MulDiv(15, GetDeviceCaps(hDC, LOGPIXELSY), 72);

		   ::ReleaseDC(NULL,hDC);

	       UpdateFonts();
		   pDsSys->NuevaVentana9(this,true);
		   return TRUE;
   case ID_AJUSTARPANTALLA9_100:
   case ID_9FONTDEFAULT:
	   DefaultFonts9(TRUE);	   
	   pDsSys->NuevaVentana9(this,true);
	   return TRUE;
   case ID_9FONTAUTO:
	   {		   
		   int factor;
		   int maxx,maxy;
		   CRect rect;

		   theApp.m_iStdView->GetClientRect(&rect);
		   if (rect.Size().cx < 2)
		   {
			   theApp.m_LeftView->GetClientRect(&rect);
			   if (theApp.m_UserMenu)
			   {
				   CRect rect2;
				   theApp.m_UserMenu->GetClientRect(&rect2);
				   rect.bottom += (rect2.Size().cy + 4);
			   }
		   }
		   maxx = rect.Size().cx;
		   maxy = rect.Size().cy;

		   factor = ajustafuente(&m_font9campo,maxx,maxy);
		   
		   maxx = LOWORD((DWORD)factor);
		   maxy = HIWORD((DWORD)factor);

		   ajustafuente(&m_font9texto,maxx,maxy);
		   ajustafuente(&m_font9rever,maxx,maxy);
		   ajustafuente(&m_font9inten,maxx,maxy);
		   ajustafuente(&m_font9mensa,maxx,maxy);
		   ajustafuente(&m_font9edita,maxx,maxy);		   

		   theApp.m_9wnd->m_tab9->GetTabWndRect(rect);
		   ajustafuente(&m_font9boton,maxx,maxy,rect.Size().cy-4);
		   

	       UpdateFonts();
		   pDsSys->NuevaVentana9(this,true);
		   return TRUE;
	   }
	   return FALSE;
   case ID_9AJUSTASIZE:
	   pfont = &m_font9campo;
	   break;
   case ID_9FONTTEXTO:
       pfont = &m_font9texto;
	   break;
   case ID_9OTROSFONTS:
	   pfont = &m_font9rever;
	   break;
   case ID_9FONTINTEN:
	   pfont = &m_font9inten;
	   break;
   case ID_9FONTMENSA:
	   pfont = &m_font9mensa;
	   Fixed = FALSE;
	   break;
   case ID_9FONTBOTOM:
	   pfont = &m_font9boton;  
 	  Fixed = FALSE;
	   break;
   case ID_9FONTEDITA:
	   pfont = &m_font9edita;
	   break;
   }
   if (pfont)
   {
	   if (PideFuente(pfont,Fixed,FALSE))
	   {
		   if (pfont == &m_font9campo)
		   {		   
			   m_font9rever = m_font9campo;
			   m_font9inten = m_font9campo;
			   m_font9texto = m_font9campo;
			   m_font9mensa = m_font9campo;
			   m_font9boton = m_font9campo;
			   m_font9edita = m_font9campo;
		   }
	       UpdateFonts();
		   pDsSys->NuevaVentana9(this);
	   }
	   return TRUE;
   }
   return FALSE;
}

BOOL CDsxDoc::PideFuente(LOGFONT *pfont,BOOL Fixed,BOOL update)
{
  LOGFONT MainFontRec = *pfont;
  DWORD flags = CF_FORCEFONTEXIST | CF_SCREENFONTS;
  if (Fixed)
  {
	 flags |= CF_FIXEDPITCHONLY;
  }
  CFontDialog fuente(&MainFontRec,flags);
  if (fuente.DoModal() == IDOK)
  {
      *pfont = MainFontRec;
	  if (update)
	  {
		  UpdateFonts();
	  }
	  theApp.ActivacionPorDefecto(NULL);	  
      return TRUE;
  }

  theApp.ActivacionPorDefecto(NULL);
  return FALSE;
}



void CDsxDoc::OnFilePrint() 
{
	// TODO: Add your command handler code here
	
}

void CDsxDoc::OnFilePrintSetup() 
{
	// TODO: Add your command handler code here
	
}

void CDsxDoc::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	
}


/////////////////////////////////////////////////////////////////////////////
// CDsxDoc serialization

void CDsxDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class COleServerDoc enables serialization
	//  of the container document's COleClientItem objects.
	COleServerDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc diagnostics

#ifdef _DEBUG
void CDsxDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CDsxDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDsxDoc commands

COleIPFrameWnd* CDsxDoc::CreateInPlaceFrame(CWnd* pParentWnd) 
{

         ASSERT_VALID(this);
         ASSERT_VALID(pParentWnd);
   
         // Get run-time class from the doc template.
         CDocTemplate* pTemplate = GetDocTemplate();
         ASSERT_VALID(pTemplate);
   
         // Use existing view if possible.
         CView* pView = NULL;
         CFrameWnd* pFrame = GetFirstFrame();
         if (pFrame != NULL)
         {
            pView =
               (CView*)pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST,
   TRUE);
            if (pView != NULL)
            {
               ASSERT_KINDOF(CView, pView);
               m_dwOrigStyle = pView->GetStyle();
               m_dwOrigStyleEx = pView->GetExStyle();
            }
         }
   
         // Create the frame from the template, ALWAYS use the CView of the
         // CSplitterWnd.
         COleIPFrameWnd* pFrameWnd = (COleIPFrameWnd*)
            pTemplate->CreateOleFrame(pParentWnd, this, FALSE);
         if (pFrameWnd == NULL)
            return NULL;
   
         // Connect the splitter window (CSplitterWnd class) to the
         // COleDocIPFrameWnd.
         CWnd* split = pFrame->GetWindow(GW_CHILD);
         VERIFY(pFrame == split->SetParent(pFrameWnd));
   
         // Remember the original parent window for deactivate--uses the
         // CFrameWnd-derived class (that is, CMDIChildWnd for MDI).
         m_pOrigParent = pFrame;
   
         // Set the active view of COleDocIPFrameWnd.
         pFrameWnd->SetActiveView(pView, FALSE);
         pFrameWnd->RecalcLayout();
         pView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
   
         // Verify the type.
         ASSERT_VALID(pFrameWnd);
         ASSERT_KINDOF(COleIPFrameWnd, pFrameWnd);
         return pFrameWnd;
      	//return COleServerDoc::CreateInPlaceFrame(pParentWnd);
}

void CDsxDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd) 
{

         ASSERT_VALID(this);
         ASSERT_VALID(pFrameWnd);
   
         // connect view to original, if existing view was used
         if (m_pOrigParent != NULL)
         {
            CView* pView = (CView*)pFrameWnd->GetDescendantWindow(
   AFX_IDW_PANE_FIRST, TRUE);
            ASSERT_VALID(pView);
   
            // Leaving the focus on an MDI child or one of its child
            // windows causes Windows to get confused when the child window
            // is destroyed, not to mention the fact that the focus will be
            // out of sync with activation.
            if (::GetFocus() == pView->m_hWnd)
            {
               // Move focus to somewhere safe.
               HWND hWnd = ::GetParent(pFrameWnd->m_hWnd);
               if (hWnd != NULL)
                  ::SetFocus(hWnd);
   
               // Check again.
               if (::GetFocus() == pView->m_hWnd)
                  SetFocus(NULL); // last ditch effort
			}
   
            // Set parent of the splitter window (CSplitterWnd class) to be
            // the CFrameWnd-derived class (that is, CMDIChildWnd for MDI
            // application).
            ASSERT_KINDOF(CFrameWnd, m_pOrigParent);
            CFrameWnd* frame = (CFrameWnd*) m_pOrigParent;
            CWnd* split = pFrameWnd->GetWindow(GW_CHILD);
            VERIFY(pFrameWnd == split->SetParent(frame));
   
            // Set the active view of CFrameWnd-derived class (that is,
            // CMDIChildWnd for MDI application).
            frame->SetActiveView(pView, FALSE);
            frame->RecalcLayout();
   
            m_pOrigParent = NULL;
   
            // Remove any scrollbars added because of in-place activation.
            if ((m_dwOrigStyle & (WS_HSCROLL|WS_VSCROLL)) == 0 &&
               (pView->GetStyle() & (WS_HSCROLL|WS_VSCROLL)) != 0)
            {
               ::SetScrollRange(pView->m_hWnd, SB_HORZ, 0, 0, TRUE);
               ::SetScrollRange(pView->m_hWnd, SB_VERT, 0, 0, TRUE);
            }
   
            // Restore old 3D style.
            pView->ModifyStyleEx(0, m_dwOrigStyleEx & WS_EX_CLIENTEDGE,
               SWP_DRAWFRAME);
   
            // Force recalc layout on splitter window.
            CSplitterWnd* pSplitter = CView::GetParentSplitter(pView,
               TRUE);
            if (pSplitter != NULL)
               pSplitter->RecalcLayout();
         }
   
         // No active view or document during destroy.
         pFrameWnd->SetActiveView(NULL);
   
         // Destroy in-place frame window.
         pFrameWnd->DestroyWindow();
      	//COleServerDoc::DestroyInPlaceFrame(pFrameWnd);
}

BOOL CDsxDoc::CompruebaTareaTeclaFuncion(int tecla)
{
	return FALSE;

	CString funcion;
	int pid,i;
	int mipid = getpid();
	funcion.Format("Task_%03d",tecla);
	pid = theApp.GetProfileInt("Funciones",funcion,0);

	if (pid == mipid)
		return TRUE;


  BOOL ocultar = FALSE;
   CMenu *main = pMFrame->GetMenu();
   if (main)
   {		  
	  CMenu *popup = main->GetSubMenu( 2 );
	  if (popup)
	  {
		 if ((popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
		 {
			ocultar = TRUE;
		 }
	  }
   }

	CString s;
	TASK_LIST *ptask = NULL;
	int ntasks = 0;
	HWND otra;
	HWND lamia = theApp.m_pMainWnd->m_hWnd;
	if (theApp.m_9wnd)
	{
		if (pDsSys)
		{
			ntasks = pDsSys->DespachadorInterno9(500,(void *)&ptask);
		}
	}
	else
	{
		 if (pDsSys)
		 { 
			 ntasks = pDsSys->DespachadorInterno(500,(void *)&ptask);
		 }
	}
	
	if (ntasks && ptask)
	{
		  otra = NULL;
		  for (i = 0;i < ntasks;i++)
		  {
			  s = ptask[i].ProcessName;
			  s.MakeLower();
			  if (s.Find("diagramx") > -1 && ptask[i].hwnd && mipid != (int)ptask[i].dwProcessId && ::IsWindowVisible((HWND)ptask[i].hwnd) && pid == (int)ptask[i].dwProcessId)
			  {
				  otra = (HWND)ptask[i].hwnd;
				  break;
			  }
		  }
		  if (otra)
		  {		
				if (theApp.m_minimizado)
				{
					::ShowWindow(otra,SW_HIDE);
					if (!::IsWindowVisible(otra))
					{		 
						::ShowWindow(otra,SW_RESTORE);
						::ShowWindow(otra,SW_SHOW);
						::BringWindowToTop(otra);
						::SetForegroundWindow(otra);
						if (ocultar)
						{
							::ShowWindow(lamia,SW_HIDE);
							::ShowWindow(lamia,SW_MINIMIZE);
							::ShowWindow(lamia,SW_SHOW);
						}
						return TRUE;
					}
					else
					{
						::ShowWindow(otra,SW_SHOW);
					}
				}
				else
				{
					::BringWindowToTop(otra);
					::SetForegroundWindow(otra);
				}
		  }
	}	
	return FALSE;
}
