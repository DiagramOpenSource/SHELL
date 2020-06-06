// DsHtml.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsHtml.h"
#include "DsxMainFrm.h"
#include "DsxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsHtml

IMPLEMENT_DYNCREATE(CDsHtml, CHtmlView)

CDsHtml::CDsHtml()
{
	//{{AFX_DATA_INIT(CDsHtml)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_iniciado = FALSE;
    m_pView = NULL;    
}

CDsHtml::~CDsHtml()
{
    if (m_pView && m_pView->m_html == this)
    {
        m_pView->m_html = NULL;
    }
}

void CDsHtml::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDsHtml)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


//static UINT style = 0;

void CDsHtml::OnInitialUpdate()
{
    if (m_iniciado) return;
	CHtmlView::OnInitialUpdate();
    m_iniciado = TRUE;

    theApp.m_phtml = this;

    ((CDsxFrame *)theApp.m_pMainWnd)->m_toolweb.m_navega.EnableWindow(TRUE);
    
    Navega();
}


CDsxDoc* CDsHtml::GetDocument()
{
    if (m_pView)
        return m_pView->GetDocument();
    return NULL;    
}

BEGIN_MESSAGE_MAP(CDsHtml, CHtmlView)
	//{{AFX_MSG_MAP(CDsHtml)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsHtml diagnostics

#ifdef _DEBUG
void CDsHtml::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDsHtml::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDsHtml message handlers

int CDsHtml::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{    
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

void CDsHtml::Navigate2( LPCTSTR lpszURL, DWORD dwFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszHeaders, LPVOID lpvPostData, DWORD dwPostDataLen )
{
    theApp.m_url = lpszURL;

    CHtmlView::Navigate2( lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders, lpvPostData, dwPostDataLen );
}

void CDsHtml::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	
	
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CDsHtml::OnDocumentComplete(LPCTSTR lpszURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnDocumentComplete(lpszURL);
}

void CDsHtml::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::CalcWindowRect(lpClientRect, 1/*nAdjustType*/);
}

BOOL CDsHtml::PreCreateWindow(CREATESTRUCT& cs) 
{
    
	return CHtmlView::PreCreateWindow(cs);
}

void CDsHtml::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::PreSubclassWindow();
}



void CDsHtml::OnPaint() 
{

    //cs.style &= ~WS_BORDER;
    //cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    /*
    if (!style)
    {
        style = GetStyle() &~WS_VISIBLE;
    }
    else
    if ((style&~WS_VISIBLE) != (GetStyle()&~WS_VISIBLE))
    {
        ::SetWindowLong(m_hWnd,GWL_STYLE,style|(GetStyle()&WS_VISIBLE));
    }
    */

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CHtmlView::OnPaint() for painting messages
}

void CDsHtml::OnDestroy() 
{   
    theApp.m_phtml = NULL;

    if (::IsWindow(((CDsxFrame *)theApp.m_pMainWnd)->m_toolweb.m_navega.m_hWnd))
        ((CDsxFrame *)theApp.m_pMainWnd)->m_toolweb.m_navega.EnableWindow(FALSE);    

	CHtmlView::OnDestroy();		
}

BOOL CDsHtml::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
    if (pParentWnd && pParentWnd->IsKindOf(RUNTIME_CLASS(CDsxView)))
    {
        m_pView = (CDsxView *)pParentWnd;        
    }
	return CHtmlView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CDsHtml::Navega()
{
    CString s;
    ((CDsxFrame *)theApp.m_pMainWnd)->m_toolweb.m_navega.GetWindowText(s);

    if (!s.IsEmpty())
    {
        theApp.m_url = s;
	    Navigate2(s,NULL,NULL);
        return TRUE;
    }
    return FALSE;
}
