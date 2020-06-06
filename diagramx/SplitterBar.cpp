// SplitterBar.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "SplitterBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSplitterBar, CWnd)

BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
	//{{AFX_MSG_MAP(CSplitterBar)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSplitterBar::CSplitterBar()
{
	m_bDragging=FALSE;
	m_pwndLeftPane=m_pwndRightPane=NULL;
	m_BarWidth = 5;
}

CSplitterBar::~CSplitterBar()
{
}

BOOL CSplitterBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bHorizontal)
{
	CWnd* pWnd = this;
	m_bHorizontal=bHorizontal;
	if (m_bHorizontal)
	   m_BarWidth = GetSystemMetrics(SM_CXFRAME);
	else
	   m_BarWidth = GetSystemMetrics(SM_CYFRAME);	
	return pWnd->Create(NULL, "", dwStyle, rect, pParentWnd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar message handlers

void CSplitterBar::OnPaint() 
{
	RECT rc;
	if (!GetUpdateRect(&rc)) return;

	PAINTSTRUCT paint;
	CDC *pDC = BeginPaint(&paint);

	CRect rect;
	GetClientRect(rect);
    pDC->FillSolidRect( rect, ::GetSysColor(COLOR_3DFACE) );
	if (m_bHorizontal)
	   {
	   rect.left -= 2;
	   rect.right += 2;
	   }
	else
	   {
	   rect.top -= 2;
	   rect.bottom += 2;
	   }
	pDC->Draw3dRect(&rect,
				  ::GetSysColor(COLOR_3DHILIGHT),
				  ::GetSysColor(COLOR_3DDKSHADOW));
	EndPaint(&paint);
}


UINT CSplitterBar::OnNcHitTest(CPoint point) 
{	
	return HTCLIENT;
}
/*
LRESULT CSplitterBar::OnNcHitTest(CPoint point) 
{	
	return HTCLIENT;
}
*/
int CSplitterBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetWindowRect(&m_rectSplitter);
	SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	//Initialize left most and right most coordinator
	CRect rectParent;
	GetParent()->GetClientRect(rectParent);
	if(m_bHorizontal)
	{
		m_cxLeftMost=rectParent.top;
		m_cxRightMost=rectParent.bottom;
	}
	else
	{
		m_cxLeftMost=rectParent.left;
		m_cxRightMost=rectParent.right;
	}

	return 0;
}

BOOL CSplitterBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint ptCursor=GetMessagePos();		
	if(IsCursorOverSplitter(ptCursor))
	{
		::SetCursor( AfxGetApp()->LoadCursor(m_bHorizontal?AFX_IDC_VSPLITBAR:AFX_IDC_HSPLITBAR));	
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSplitterBar::IsCursorOverSplitter( const CPoint& ptCursor )
{
	CRect rectSplitter;
	GetWindowRect(rectSplitter);
	return rectSplitter.PtInRect( ptCursor );
}

void CSplitterBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(nFlags & MK_LBUTTON && m_bDragging)
	{
		DrawDraggingBar(point);
		return;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CSplitterBar::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	ClientToScreen(&point);
	if(IsCursorOverSplitter(point))
	{
		SetCapture();
		m_bDragging=TRUE;
		GetWindowRect(&m_rectSplitter);		
		ScreenToClient(&point);
		DrawDraggingBar(point,DRAG_ENTER);
		return;
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CSplitterBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		DrawDraggingBar(point,DRAG_EXIT);
		//Move the splitter here
		ClientToScreen(&point);

		//with in client area?
		if(m_bHorizontal)
		{
			CPoint pointLeftMost;
			pointLeftMost.y=m_cxLeftMost;
			GetParent()->ClientToScreen(&pointLeftMost);
			CPoint pointRightMost;
			pointRightMost.y=m_cxRightMost;
			GetParent()->ClientToScreen(&pointRightMost);

			if(point.y < pointLeftMost.y)
				point.y=pointLeftMost.y;
			if(point.y > pointRightMost.y)
				point.y=pointRightMost.y;

			m_rectDragCurt=m_rectSplitter;
			m_rectDragCurt.top=point.y;
			m_rectDragCurt.bottom=point.y+m_rectSplitter.Height();
		}
		else
		{
			CPoint pointLeftMost;
			pointLeftMost.x=m_cxLeftMost;
			GetParent()->ClientToScreen(&pointLeftMost);
			CPoint pointRightMost;
			pointRightMost.x=m_cxRightMost;
			GetParent()->ClientToScreen(&pointRightMost);

			if(point.x < pointLeftMost.x)
				point.x=pointLeftMost.x;
			if(point.x > pointRightMost.x)
				point.x=pointRightMost.x;

			m_rectDragCurt=m_rectSplitter;
			m_rectDragCurt.left=point.x;
			m_rectDragCurt.right=point.x+m_rectSplitter.Width();
		}
		GetParent()->ScreenToClient(m_rectDragCurt);
		MoveWindow(m_rectDragCurt,TRUE);
		//OnPaint();

		ReleaseCapture();
		m_bDragging=FALSE;
		MovePanes();
		GetParent()->SendMessage(WM_SPLITTER_MOVED,0,0L);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CSplitterBar::DrawDraggingBar(CPoint point,DRAGFLAG df)
{
	ClientToScreen(&point);
	m_rectDragCurt=m_rectSplitter;
	if(m_bHorizontal)
	{
		m_rectDragCurt.top=point.y;
		m_rectDragCurt.bottom=point.y+m_rectSplitter.Height();
	}
	else
	{
		m_rectDragCurt.left=point.x;
		m_rectDragCurt.right=point.x+m_rectSplitter.Width();
	}

	CSize size(m_rectDragCurt.Width(),m_rectDragCurt.Height());

	CWnd *pParentWnd=GetParent();
	ASSERT(pParentWnd);
	CDC *pDC=pParentWnd->GetDC();	
	pParentWnd->ScreenToClient(m_rectDragCurt);
	switch(df)
	{
	case DRAG_ENTER:
		 pDC->DrawDragRect(m_rectDragCurt,size,NULL,size);
		 break;
	case DRAG_EXIT:	//fall through
	default:
		 pDC->DrawDragRect(m_rectDragCurt,size,m_rectDragPrev,size);
		 break;
	}

	pParentWnd->ReleaseDC(pDC);
	m_rectDragPrev=m_rectDragCurt;
}

void CSplitterBar::SetPanes(CWnd *pwndLeftPane,CWnd *pwndRightPane)
{
	ASSERT(pwndLeftPane);
	ASSERT(pwndRightPane);

	m_pwndLeftPane=pwndLeftPane;
	m_pwndRightPane=pwndRightPane;

	if(m_bHorizontal)
	{
		//Initialize splitter bar position & size
		CRect rectBar;
		pwndLeftPane->GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);
		rectBar.top=rectBar.bottom;
		int nBarWidth=m_BarWidth;
		rectBar.top-=nBarWidth/2;
		rectBar.bottom+=nBarWidth/2;
		MoveWindow(rectBar);
		
		//repostion top & bottom panes
		MovePanes();

		//calculate top most and bottom most coordinator
		CRect rectLeft;
		m_pwndLeftPane->GetWindowRect(rectLeft);
		GetParent()->ScreenToClient(rectLeft);
		m_cxLeftMost=rectLeft.top;

		CRect rectRight;
		m_pwndRightPane->GetWindowRect(rectRight);
		GetParent()->ScreenToClient(rectRight);
		m_cxRightMost=rectRight.bottom;
	}
	else
	{
		//Initialize splitter bar position & size
		CRect rectBar;
		pwndLeftPane->GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);
		rectBar.left=rectBar.right;
		int nBarWidth=m_BarWidth;
		rectBar.left-=nBarWidth/2;
		rectBar.right+=nBarWidth/2;
		MoveWindow(rectBar);
		
		//repostion left & rigth panes
		MovePanes();

		//calculate left most and right most coordinator
		CRect rectLeft;
		m_pwndLeftPane->GetWindowRect(rectLeft);
		GetParent()->ScreenToClient(rectLeft);
		m_cxLeftMost=rectLeft.left;

		CRect rectRight;
		m_pwndRightPane->GetWindowRect(rectRight);
		GetParent()->ScreenToClient(rectRight);
		m_cxRightMost=rectRight.right;
	}
}

void CSplitterBar::MovePanes()
{
	ASSERT(m_pwndLeftPane);
	ASSERT(m_pwndRightPane);

	if(m_bHorizontal)
	{
		//Get position of the splitter bar
		CRect rectBar;
		GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);

		//reposition top pane
		CRect rectLeft;
		//m_pwndLeftPane->GetWindowRect(rectLeft);
		//GetParent()->ScreenToClient(rectLeft);		
		GetParent()->GetClientRect(rectLeft);

		rectLeft.bottom=rectBar.top/*+m_BarWidth*/;
		m_pwndLeftPane->MoveWindow(rectLeft);

		//reposition bottom pane
		CRect rectRight;
		//m_pwndRightPane->GetWindowRect(rectRight);
		//GetParent()->ScreenToClient(rectRight);
        GetParent()->GetClientRect(rectRight);

		rectRight.top=rectBar.bottom/*-m_BarWidth*/;
		m_pwndRightPane->MoveWindow(rectRight);
	}
	else
	{
		//Get position of the splitter bar
		CRect rectBar;
		GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);

		//reposition left pane
		CRect rectLeft;
		//m_pwndLeftPane->GetWindowRect(rectLeft);
		//GetParent()->ScreenToClient(rectLeft);
		GetParent()->GetClientRect(rectLeft);

		rectLeft.right=rectBar.left/*+m_BarWidth*/;
		m_pwndLeftPane->MoveWindow(rectLeft);

		//reposition right pane
		CRect rectRight;
		//m_pwndRightPane->GetWindowRect(rectRight);
		//GetParent()->ScreenToClient(rectRight);
		GetParent()->GetClientRect(rectRight);

		rectRight.left=rectBar.right/*-m_BarWidth*/;
		m_pwndRightPane->MoveWindow(rectRight);
	}

	//repaint client area
	GetParent()->Invalidate();
}


void CSplitterBar::Reajusta()
{    
	CRect rectParent;	
	CRect rectBar;
	GetWindowRect(rectBar);
	GetParent()->ScreenToClient(rectBar);
	GetParent()->GetClientRect(rectParent);
	if(m_bHorizontal)
	{
	    m_rectSplitter.left = rectParent.left;
		m_rectSplitter.right = rectParent.right;
		rectBar.left = rectParent.left;
		rectBar.right = rectParent.right;
		m_cxLeftMost=rectParent.top;
		m_cxRightMost=rectParent.bottom;		
		
	}
	else
	{
	    m_rectSplitter.top = rectParent.top;
		m_rectSplitter.bottom = rectParent.bottom;
	    rectBar.top = rectParent.top;
		rectBar.bottom = rectParent.bottom;
		m_cxLeftMost=rectParent.left;
		m_cxRightMost=rectParent.right;
	}
		
    MoveWindow(rectBar);
	MovePanes();
}
