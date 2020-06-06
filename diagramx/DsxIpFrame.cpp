// DsxIpFrame.cpp : implementation of the CDsxInPlaceFrame class
//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxIpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsxInPlaceFrame

IMPLEMENT_DYNCREATE(CDsxInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CDsxInPlaceFrame, COleDocIPFrameWnd)
	//{{AFX_MSG_MAP(CDsxInPlaceFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsxInPlaceFrame construction/destruction

CDsxInPlaceFrame::CDsxInPlaceFrame()
{
}

CDsxInPlaceFrame::~CDsxInPlaceFrame()
{
}

int CDsxInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->hMenu = NULL;
	if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar implements in-place resizing.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // fail to create
	}

	// By default, it is a good idea to register a drop-target that does
	//  nothing with your frame window.  This prevents drops from
	//  "falling through" to a container that supports drag-drop.
	m_dropTarget.Register(this);

	return 0;
}


static UINT indicators[] =
{
      ID_SEPARATOR,   // status line indicator	      
      IDS_STRINGCABECERA,
      IDS_RELACION,
	  IDS_USUARIO,
      //IDS_CLAVES,
      //ID_INDICATOR_CAPS,
      //ID_INDICATOR_NUM,
      //ID_INDICATOR_SCRL
};

// OnCreateControlBars is called by the framework to create control bars on the
//  container application's windows.  pWndFrame is the top level frame window of
//  the container and is always non-NULL.  pWndDoc is the doc level frame window
//  and will be NULL when the container is an SDI application.  A server
//  application can place MFC control bars on either window.
BOOL CDsxInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// Remove this if you use pWndDoc
	//UNREFERENCED_PARAMETER(pWndDoc);
#ifndef DSPROFUIS
	if (pMFrame)
	{
		m_wndMenuBar.SetOwner(pMFrame);
		m_wndToolBar.SetOwner(pMFrame);
		//m_wndStatusBar.SetOwner(pMFrame);		
		
		if (!m_wndMenuBar.CreateEx(pWndFrame, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER) ||
			!m_wndMenuBar.LoadMenuBar(IDR_MAINMENU))
		{
			TRACE0("Failed to create menubar\n");
			ASSERT(FALSE);
			return FALSE;
		}
		theApp.m_MenuBar = &m_wndMenuBar;
		

		if (!m_wndToolBar.CreateEx(pWndFrame,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		  | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("Failed to create toolbar\n");
			ASSERT(FALSE);
			return FALSE;
		}
		//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	   CRect rect;
	   int index = 0;
	   /*
	   while(m_wndToolBar.GetItemID(index)!=ID_FONDOWEB) index++;
    
	   m_wndToolBar.SetButtonInfo(index, ID_FONDOWEB, TBBS_SEPARATOR, 250 );
	   m_wndToolBar.GetItemRect(index, &rect);

	   rect.top +=1;
	   rect.bottom += 200;
	   */

	   /*
	   m_wndMenuBar.EnableDockingEx(CBRS_ALIGN_ANY);
	   pWndFrame->EnableDocking(CBRS_ALIGN_ANY);
	   pWndFrame->DockControlBar(&m_wndMenuBar);
	   */

	   /*
	   if (!m_wndToolBar.m_navega.Create(WS_CHILD| CBS_AUTOHSCROLL | 
										   CBS_DROPDOWN | CBS_HASSTRINGS ,
										   rect,&(m_wndToolBar), IDD_MIUSERWEB))
	   {
		   TRACE0("Failed to create combo-box\n");
		   ASSERT(FALSE);
		   return FALSE;
	   }
	   HFONT hfont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

	   m_wndToolBar.m_navega.EnableWindow(FALSE);
	   m_wndToolBar.m_navega.SendMessage(WM_SETFONT,(WPARAM)hfont,MAKELPARAM(TRUE, 0));   

	   m_wndToolBar.m_navega.ShowWindow(SW_SHOW);
    
	   m_wndToolBar.m_navega.AddString(theApp.m_url);
	   m_wndToolBar.m_navega.SetCurSel(0);
	   */

	   /*
	   if (!m_wndStatusBar.Create(this) ||
		  !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	   {
		  TRACE0("Failed to create status bar\n");
		  ASSERT(FALSE);
		  return FALSE;      // fail to create
	   }
	   */


		m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY);

		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		pWndFrame->EnableDocking(CBRS_ALIGN_ANY);
		pWndFrame->DockControlBar(&(m_wndToolBar));

	   CMenu *main = pMFrame->GetMenu();
	   if (main)
	   {
		  CMenu *popup = main->GetSubMenu( 0 );
		  if (popup)
		  {
			 if (!theApp.GetProfileInt("Defaults","Impresora Standard",1))
				popup->CheckMenuItem( ID_IMPRESORAINTERNA,  MF_BYCOMMAND | MF_UNCHECKED);
			 else
				popup->CheckMenuItem( ID_IMPRESORAINTERNA,  MF_BYCOMMAND | MF_CHECKED);
			 if (!theApp.GetProfileInt("Defaults","Impresora Excel",0))
				popup->CheckMenuItem( ID_IMPREXCEL,  MF_BYCOMMAND | MF_UNCHECKED);
			 else
				popup->CheckMenuItem( ID_IMPREXCEL,  MF_BYCOMMAND | MF_CHECKED);
			 if (!theApp.GetProfileInt("Defaults","Impresora Pantalla",0))
				popup->CheckMenuItem( ID_IMPREPANTA,  MF_BYCOMMAND | MF_UNCHECKED);
			 else
				popup->CheckMenuItem( ID_IMPREPANTA,  MF_BYCOMMAND | MF_CHECKED);
		  }
		  popup = main->GetSubMenu( 2 );
		  if (popup)
		  {
			 if (!theApp.GetProfileInt("Defaults","Expandir solo una rama",0))
			 {
				popup->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_UNCHECKED);			  
			 }
			 else
			 {
				popup->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_CHECKED);			  
			 }
			 if (!theApp.GetProfileInt("Defaults","Multi Tarea",1))
			 {
				popup->CheckMenuItem( ID_MULTITAREA,  MF_BYCOMMAND | MF_UNCHECKED);			  
			 }
			 else
			 {
				popup->CheckMenuItem( ID_MULTITAREA,  MF_BYCOMMAND | MF_CHECKED);			  
			 }

			 if (theApp.m_hidehtml)
			 {
				popup->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_UNCHECKED);            
			 }
			 else
			 {
				popup->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_CHECKED);            
			 }
			 if (theApp.m_usarweb)
			 {
				popup->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_CHECKED);
			 }
			 else
			 {
				popup->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_UNCHECKED);
			 }         
		  }
	   }

		return TRUE;
	}
	
	// Set owner to this window, so messages are delivered to correct app
	m_wndToolBar.SetOwner(this);

	// Create toolbar on client's frame window
	if (!m_wndToolBar.CreateEx(pWndFrame,WS_CHILD | WS_VISIBLE | CBRS_TOP) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// Set owner to this window, so messages are delivered to correct app
	/*
	m_wndDlgBar.SetOwner(this);

	// Create dialog bar on client's frame window
	if (!m_wndDlgBar.Create(pWndFrame, IDR_MAINFRAME,
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;
	}
	*/

	// Set the owner of this window, so messages are delivered to correct app
	//m_wndReBar.SetOwner(this);

	// Create a rebar on client's frame window
	//if (!m_wndReBar.Create(pWndFrame,RBS_BANDBORDERS,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | /*CBRS_TOP*/ CCS_RIGHT ) /*||
	//	!m_wndReBar.AddBar(&m_wndToolBar)*/ /*||
	//	!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	//{
	//	TRACE0("Failed to create rebar\n");
	//	return FALSE;
	//}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	/*
	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		*/

	if (theApp.m_tooltip) {
		theApp.m_tooltip.Create(this,TTS_NOPREFIX | 0x40);
		theApp.m_tooltip.Activate( TRUE );			   
		::SendMessage(theApp.m_tooltip.m_hWnd, TTM_SETMAXTIPWIDTH, 0, 200);
		m_wndToolBar.GetToolBarCtrl().SetToolTips(&theApp.m_tooltip);	
	}
#endif
	return TRUE;
}

BOOL CDsxInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxInPlaceFrame diagnostics

#ifdef _DEBUG
void CDsxInPlaceFrame::AssertValid() const
{
	COleDocIPFrameWnd::AssertValid();
}

void CDsxInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDsxInPlaceFrame commands

BOOL CDsxInPlaceFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleDocIPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDsxInPlaceFrame::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}
