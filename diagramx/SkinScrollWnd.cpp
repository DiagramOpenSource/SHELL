// SkinScrollWnd.cpp : implementation file
// ģ�飺�ؼ�����������
//		ͨ����һ���ؼ������໯ʵ�ֹ�������������CSkinScrollBar���ʹ�á�
//	�ƽ���	
//	2006.7.7
//////////////////////////////////////////////////////////////////////	

#include "stdafx.h"
#include "SkinScrollWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_UPDATE 100
#define UM_DESTMOVE	(WM_USER+100)

static LRESULT CALLBACK
HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CSkinScrollWnd *pSkin=(CSkinScrollWnd*)GetWindowLong(hwnd,GWL_USERDATA);
	LRESULT lr=::CallWindowProc(pSkin->m_funOldProc,hwnd,msg,wp,lp);
	if(pSkin->m_bOp) return lr;
	if(msg==WM_ERASEBKGND)
	{
		SCROLLINFO si;
		DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
		if(dwStyle&WS_VSCROLL)
		{
			memset(&si,0,sizeof(si));
			si.cbSize=sizeof(si);
			si.fMask=SIF_ALL;
			::GetScrollInfo(hwnd,SB_VERT,&si);
			pSkin->m_sbVert.SetScrollInfo(&si);
			pSkin->m_sbVert.EnableWindow( INT(si.nMax) >= INT(si.nPage) ? TRUE : FALSE );
		}
		if(dwStyle&WS_HSCROLL)
		{
			memset(&si,0,sizeof(si));
			si.cbSize=sizeof(si);
			si.fMask=SIF_ALL;
			::GetScrollInfo(hwnd,SB_HORZ,&si);
			pSkin->m_sbHorz.SetScrollInfo(&si);
			pSkin->m_sbHorz.EnableWindow( INT(si.nMax) >= INT(si.nPage) ? TRUE : FALSE );
		}
	}else if(msg==WM_NCCALCSIZE && wp)
	{
		LPNCCALCSIZE_PARAMS pNcCalcSizeParam=(LPNCCALCSIZE_PARAMS)lp;
		DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
		DWORD dwExStyle=::GetWindowLong(hwnd,GWL_EXSTYLE);
		BOOL  bLeftScroll=dwExStyle&WS_EX_LEFTSCROLLBAR;
		int nWid=::GetSystemMetrics(SM_CXVSCROLL);
		if(dwStyle&WS_VSCROLL) 
		{
			if(bLeftScroll)
				pNcCalcSizeParam->rgrc[0].left-=nWid-pSkin->m_nScrollWid;
			else
				pNcCalcSizeParam->rgrc[0].right+=nWid-pSkin->m_nScrollWid;
		}
		if(dwStyle&WS_HSCROLL) pNcCalcSizeParam->rgrc[0].bottom+=nWid-pSkin->m_nScrollWid;

		RECT rc,rcVert,rcHorz;
		::GetWindowRect(hwnd,&rc);
		::OffsetRect(&rc,-rc.left,-rc.top);

		nWid=pSkin->m_nScrollWid;
		if(bLeftScroll)
		{
			int nLeft=pNcCalcSizeParam->rgrc[0].left;
			int nBottom=pNcCalcSizeParam->rgrc[0].bottom;
			rcVert.right=nLeft;
			rcVert.left=nLeft-nWid;
			rcVert.top=0;
			rcVert.bottom=nBottom;
			rcHorz.left=nLeft;
			rcHorz.right=pNcCalcSizeParam->rgrc[0].right;
			rcHorz.top=nBottom;
			rcHorz.bottom=nBottom+nWid;
		}else
		{
			int nRight=pNcCalcSizeParam->rgrc[0].right;
			int nBottom=pNcCalcSizeParam->rgrc[0].bottom;
			rcVert.left=nRight;
			rcVert.right=nRight+nWid;
			rcVert.top=0;
			rcVert.bottom=nBottom;
			rcHorz.left=0;
			rcHorz.right=nRight;
			rcHorz.top=nBottom;
			rcHorz.bottom=nBottom+nWid;
		}
		if(dwStyle&WS_VSCROLL && dwStyle&WS_HSCROLL)
		{
			pSkin->m_nAngleType=bLeftScroll?1:2;
		}else
		{
			pSkin->m_nAngleType=0;
		}
		if(dwStyle&WS_VSCROLL)
		{
			pSkin->m_sbVert.MoveWindow(&rcVert);
			pSkin->m_sbVert.ShowWindow(SW_SHOW);
		}else
		{
			pSkin->m_sbVert.ShowWindow(SW_HIDE);
		}
		if(dwStyle&WS_HSCROLL)
		{
			pSkin->m_sbHorz.MoveWindow(&rcHorz);
			pSkin->m_sbHorz.ShowWindow(SW_SHOW);
		}else
		{
			pSkin->m_sbHorz.ShowWindow(SW_HIDE);
		}
		pSkin->PostMessage(UM_DESTMOVE,dwStyle&WS_VSCROLL,bLeftScroll);
	}
	return lr;
}

CSkinScrollWnd* SkinWndScroll(CWnd *pWnd)
{
	CSkinScrollWnd *pFrame=new CSkinScrollWnd;
	pFrame->SkinWindow(pWnd);
	return pFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinScrollWnd

CSkinScrollWnd::CSkinScrollWnd()
{
	m_funOldProc=NULL;
	m_bOp=FALSE;
	m_nScrollWid=16;
	m_nAngleType=0;
	m_primera = TRUE;
}

CSkinScrollWnd::~CSkinScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CSkinScrollWnd, CWnd)
	//{{AFX_MSG_MAP(CSkinScrollWnd)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_DESTMOVE,OnDestMove)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSkinScrollWnd message handlers


BOOL CSkinScrollWnd::SkinWindow(CWnd *pWnd)
{
	ASSERT(m_hWnd==NULL);
	m_nScrollWid = ::GetSystemMetrics( SM_CXVSCROLL );

	CWnd *pParent=pWnd->GetParent();
	ASSERT(pParent);
	RECT rcFrm,rcWnd;
	pWnd->GetWindowRect(&rcFrm);
	pParent->ScreenToClient(&rcFrm);
	rcWnd=rcFrm;
	OffsetRect(&rcWnd,-rcWnd.left,-rcWnd.top);
	UINT uID=pWnd->GetDlgCtrlID();
	DWORD dwStyle=pWnd->GetStyle();
	DWORD dwFrmStyle=WS_CHILD|SS_NOTIFY;
	DWORD dwFrmStyleEx=0;
	if(dwStyle&WS_VISIBLE) dwFrmStyle|=WS_VISIBLE;
	if(dwStyle&WS_BORDER)
	{
		dwFrmStyle|=WS_BORDER;
		pWnd->ModifyStyle(WS_BORDER,0);
		int nBorder=::GetSystemMetrics(SM_CXBORDER);
		rcWnd.right-=nBorder*2;
		rcWnd.bottom-=nBorder*2;
	}
	DWORD dwExStyle=pWnd->GetExStyle();
	if(dwExStyle&WS_EX_CLIENTEDGE)
	{
		pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0);
		int nBorder=::GetSystemMetrics(SM_CXEDGE);
		rcWnd.right-=nBorder*2;
		rcWnd.bottom-=nBorder*2;
		dwFrmStyleEx|=WS_EX_CLIENTEDGE;
	}
	this->CreateEx(dwFrmStyleEx,AfxRegisterWndClass(NULL),"SkinScrollBarFrame",dwFrmStyle,rcFrm,pParent,uID);

	m_wndLimit.Create(NULL,"LIMIT",WS_CHILD|WS_VISIBLE,/*rcWnd */CRect(0,0,0,0),this,200);
	m_sbHorz.Create(WS_CHILD,CRect(0,0,0,0),this,100);
	m_sbVert.Create(WS_CHILD|SBS_VERT,CRect(0,0,0,0),this,101);

	pWnd->SetParent(&m_wndLimit);
	SetWindowLong(pWnd->m_hWnd,GWL_USERDATA,(LONG)this);
	m_funOldProc=(WNDPROC)SetWindowLong(pWnd->m_hWnd,GWL_WNDPROC,(LONG)HookWndProc);

	pWnd->MoveWindow(&rcWnd);
	SetTimer(TIMER_UPDATE,500,NULL);

	BOOL  bLeftScroll=dwExStyle&WS_EX_LEFTSCROLLBAR;
	
	pWnd->SetWindowPos(NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOSIZE|SWP_NOZORDER|SWP_NOMOVE);	
	return TRUE;
}

void CSkinScrollWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if(cx==0 || cy==0) return;
	if(m_wndLimit.m_hWnd)
	{
		CWnd *pWnd=m_wndLimit.GetWindow(GW_CHILD);
		pWnd->MoveWindow(0,0,cx,cy);
	}
}

void CSkinScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd *pWnd=m_wndLimit.GetWindow(GW_CHILD);
	m_bOp=TRUE;
	pWnd->SendMessage(WM_VSCROLL,MAKELONG(nSBCode,nPos),0);
	m_bOp=FALSE;
	if(nSBCode==SB_THUMBTRACK) return;
	SCROLLINFO si={0};
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	pWnd->GetScrollInfo(SB_VERT,&si);
	pScrollBar->SetScrollInfo(&si);
}

void CSkinScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd *pWnd=m_wndLimit.GetWindow(GW_CHILD);
	m_bOp=TRUE;
	pWnd->SendMessage(WM_HSCROLL,MAKELONG(nSBCode,nPos),0);
	m_bOp=FALSE;
	if(nSBCode==SB_THUMBTRACK) return;
	SCROLLINFO si={0};
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	pWnd->GetScrollInfo(SB_HORZ,&si);
	pScrollBar->SetScrollInfo(&si);
}

void CSkinScrollWnd::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

void CSkinScrollWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CSkinScrollWnd::OnTimer(UINT nIDEvent) 
{
	nIDEvent;
	CWnd *pWnd=m_wndLimit.GetWindow(GW_CHILD);
	SCROLLINFO si1={0};
	si1.cbSize=sizeof(SCROLLINFO);
	si1.fMask=SIF_ALL;
	SCROLLINFO si2={0};
	if(m_sbVert.IsWindowVisible())
	{
		pWnd->GetScrollInfo(SB_VERT,&si1);
		m_sbVert.GetScrollInfo(&si2,SIF_ALL);
		if(si1.nMax!=si2.nMax||si1.nMin!=si2.nMin||si1.nPos!=si2.nPos||si1.nPage!=si2.nPage)
			m_sbVert.SetScrollInfo(&si1);
	}
	if(m_sbHorz.IsWindowVisible())
	{
		pWnd->GetScrollInfo(SB_HORZ,&si1);
		m_sbHorz.GetScrollInfo(&si2,SIF_ALL);
		if(si1.nMax!=si2.nMax||si1.nMin!=si2.nMin||si1.nPos!=si2.nPos||si1.nPage!=si2.nPage)
			m_sbHorz.SetScrollInfo(&si1);
	}
	if (m_primera)
	{
		m_primera = FALSE;
	}
}

LRESULT CSkinScrollWnd::OnDestMove(WPARAM wParam, LPARAM lParam)
{
	m_bOp=TRUE;
	BOOL bVScroll=wParam;
	BOOL bLeftScroll=lParam;
	RECT rcClient;
	GetClientRect(&rcClient);
	RECT rcLimit,rcWnd;
	rcWnd=rcClient;
	rcLimit=rcClient;
	if(m_sbHorz.IsWindowVisible()) rcLimit.bottom-=m_nScrollWid;
	if(bLeftScroll)
	{
		if(bVScroll)
		{
			rcLimit.left+=m_nScrollWid;
			OffsetRect(&rcWnd,-m_nScrollWid,0);
		}
	}else
	{
		if(bVScroll) rcLimit.right-=m_nScrollWid;
	}
	m_wndLimit.MoveWindow(&rcLimit);
	m_wndLimit.GetWindow(GW_CHILD)->SetWindowPos(NULL,rcWnd.left,0,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_bOp=FALSE;
	return 0;
}
