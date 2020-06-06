// DsTreeMenu.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsTreeMenu.h"
#include "DsxLeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsTreeMenu
/*
		m_cxSplitter = m_cySplitter = 3 + 2 + 2;
		m_cxBorderShare = m_cyBorderShare = 0;
		m_cxSplitterGap = m_cySplitterGap = 3 + 2 + 2;
		m_cxBorder = m_cyBorder = 2;
*/


#define CX_BORDER   1
#define CY_BORDER   1

CMiSplit::CMiSplit()
{	
  CSplitterWnd::CSplitterWnd();  
  m_buscadlg = NULL;
  m_cxBorder = m_cyBorder = 0;
  m_cxSplitter = m_cySplitter = 5;
  m_cxSplitterGap = m_cySplitterGap = 5;

  m_clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
  m_clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
  m_clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
  m_clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
  m_clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);

}

CMiSplit::~CMiSplit()
{
	if (m_buscadlg)
	{
		if (::IsWindow(m_buscadlg->m_hWnd))
		{
			m_buscadlg->DestroyWindow();
		}
		delete m_buscadlg;
	}
}

void CMiSplit::OnDrawSplitter(CDC* pDC, ESplitType nType,
	const CRect& rectArg)
{
    //enum ESplitType { splitBox, splitBar, splitIntersection, splitBorder };

	if (pDC == NULL)
        CSplitterWnd::OnDrawSplitter(pDC,nType,rectArg);
    else
    if (nType == splitBar)
    {
        CRect rect = rectArg;
        rect.right++;
        //rect.bottom += 4;
        pDC->Draw3dRect(rect, m_clrBtnFace, m_clrWindowFrame);		
		rect.InflateRect(0, -CY_BORDER);
        pDC->Draw3dRect(rect, m_clrBtnHilite, m_clrBtnShadow);
        rect.InflateRect(0, -CY_BORDER);
	    pDC->FillSolidRect(rect, m_clrBtnFace);
	}
    else
    if (nType != splitBorder)
        CSplitterWnd::OnDrawSplitter(pDC,nType,rectArg);

    return;
}


IMPLEMENT_DYNCREATE(CDsTreeMenu, CView)

CDsTreeMenu::CDsTreeMenu()
{
}

CDsTreeMenu::~CDsTreeMenu()
{
    theApp.m_splitmenu = NULL;
}


BEGIN_MESSAGE_MAP(CDsTreeMenu, CView)
	//{{AFX_MSG_MAP(CDsTreeMenu)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDsTreeMenu::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
    //CBrush *hh = CBrush().FromHandle((HBRUSH)(::GetStockObject(LTGRAY_BRUSH)));
   

    //pDC->FillRect(&rect,hh);

    //theApp.m_LeftView->GetClientRect(rect);


}

/////////////////////////////////////////////////////////////////////////////
// CViewPrueba diagnostics

#ifdef _DEBUG
void CDsTreeMenu::AssertValid() const
{
	CView::AssertValid();
}

void CDsTreeMenu::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CDsTreeMenu message handlers

int CDsTreeMenu::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    CCreateContext* pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

    
    lpCreateStruct->style &= ~WS_BORDER;
    lpCreateStruct->style |= WS_CLIPCHILDREN;


	if (CView::OnCreate(lpCreateStruct) == -1)	
        return -1;    

    if (!m_wndSplitter.Create( this, 2, 1,CSize(40,40),pContext,WS_CHILD | WS_VISIBLE | SPLS_DYNAMIC_SPLIT))
        return -1;

	//	
    CRect rect;
    GetClientRect(rect);
	/*
	if (m_buscadlg)
	{
		CRect rr;
		m_buscadlg->GetWindowRect(rr);
		rect.top += rr.Size().cy;
	}
	*/
    m_wndSplitter.MoveWindow(rect);
    
    theApp.m_splitmenu = &m_wndSplitter;

    return 0;
}

void CDsTreeMenu::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
    CRect rect;
    GetClientRect(rect);	
	if (m_wndSplitter.m_buscadlg)
	{
		CRect rr;
		m_wndSplitter.m_buscadlg->GetWindowRect(rr);
		rect.top += rr.Size().cy;
	}	
    m_wndSplitter.MoveWindow(rect);        
}


