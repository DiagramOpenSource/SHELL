// DsxMainFrm.h : interface of the CDsxFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSXMAINFRM_H__8C64B70B_7E9D_47B7_948C_30D9CED5D8B4__INCLUDED_)
#define AFX_DSXMAINFRM_H__8C64B70B_7E9D_47B7_948C_30D9CED5D8B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MenuBar.h"

class CDsxView;
class CDsxApp;
class CDsxLeftView;
class CTablaW;
class CDsHtml;
class CDsxInPlaceFrame;

#include "ComboNavega.h"

#ifdef DSPROFUIS

class CDsxFrame;
class CDsProfuisCustomOffice2003;
class CDsProfuisCustomOffice2007_R1;

/*
class DSPROFUIS_I
{
public:
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		const CRect &rcDst,
		const CRect &rcWnd,
		COLORREF micolor
		);
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		CWnd * pWnd,
		COLORREF micolor
		);
	virtual void SetThemeColors(
		COLORREF clrFillHint,
		COLORREF clrAccentHint
		);
};
*/

/*
class CSimpleControlBar : public CExtDynamicControlBar
{
public:
	DECLARE_SERIAL( CSimpleControlBar );
	COLORREF m_clrEditorBackground;
	CSimpleControlBar()
	{
		m_clrEditorBackground =
			CExtPaintManager::stat_HLStoRGB(
				double( ::rand() % 1000 ) / 1000.0,
				0.8,
				0.7
				);
	}
	virtual void OnSerializeDynamicProps( CArchive & ar )
	{
		CExtDynamicControlBar::OnSerializeDynamicProps( ar );
		if( ar.IsStoring() )
			ar << m_clrEditorBackground;
		else
			ar >> m_clrEditorBackground;
	}
}; // class CSimpleControlBar
*/

class CCDsMenuItems
{
public:
	CCDsMenuItems(MenuStruct *menu);
    ~CCDsMenuItems();
	CString m_nommenu;
	int m_nitems;
    CArray< CCDsMenuItems *, CCDsMenuItems * > m_sub;
	CArray< CString, CString > m_items;	
	CArray< long, long > m_op;		
};

//CExtComboBox
class CCDsMenuCombo: public CExtComboBox
{
	DECLARE_DYNCREATE(CCDsMenuCombo)
// Construction
public:
	CCDsMenuCombo();
	~CCDsMenuCombo();

// Attributes
public:	
	CCDsMenuItems *m_menu;
	CCDsMenuCombo *m_hijo;
// Operations
public:
	void Rellena(CCDsMenuItems *menu);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboNavega)
	public:	
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CComboNavega)
	afx_msg void OnSelendok();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class CDsMenuToolbar : public CExtToolControlBar 
{
public:
	CDsMenuToolbar();
	~CDsMenuToolbar();		
	void Rellena(MenuStruct *mi_menu, int defid);
public:
	CCDsMenuItems *m_menu;
	CCDsMenuCombo m_menus[3];   
/*
	virtual void OnNcAreaButtonsReinitialize()
	{        
		INT nCountOfNcButtons = NcButtons_GetCount();        
		if( nCountOfNcButtons > 0 )            
			return;    
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)        
		NcButtons_Add( new CExtBarNcAreaButtonAutoHide(this) );    
#endif        
		NcButtons_Add( new CExtBarNcAreaButtonMenu(this) );    
	}
	*/
};



class CMainToolBar : public CExtToolControlBar 
#else
class CMainToolBar : public CToolBar 
#endif
{
public:
   CComboNavega m_navega;
};


class CToolDropTarget : public COleDropTarget
{
	public:
			virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
			DWORD dwKeyState, CPoint point );
			virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
			DWORD dwKeyState, CPoint point );
			virtual void OnDragLeave( CWnd* pWnd );
			virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
			DROPEFFECT dropEffect, CPoint point);
	public:				
	private:		
};


class CMenuDropTarget : public COleDropTarget
{
	public:
			virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
			DWORD dwKeyState, CPoint point );
			virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
			DWORD dwKeyState, CPoint point );
			virtual void OnDragLeave( CWnd* pWnd );
			virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
			DROPEFFECT dropEffect, CPoint point);
	public:			
	private:		
};

#ifdef DSPROFUIS	
class CDSSplitterWnd;
#endif

#ifdef DSXTLIB
class CDsxFrame : public CXTPFrameWnd
#else
#ifdef DSPROFUIS	
#ifndef PROFUIS240
class CDsxFrame 
	: public CExtNCW < CFrameWnd > , public CExtDynamicBarSite
#else
class CDsxFrame 
	: public CFrameWnd 
#endif
	//, public CExtDynamicBarSite
#else
class CDsxFrame : public CFrameWnd
#endif
#endif
{
   friend CDsxApp;
   friend CDsxLeftView;
   friend CTablaW;
   friend CDsHtml;
   friend CDsxInPlaceFrame;
	
protected: // create from serialization only
	CDsxFrame();
	DECLARE_DYNCREATE(CDsxFrame)

public:
// Attributes
    UINT m_debugtimer;
	UINT m_signaltimer;
#ifdef DSPROFUIS    

	CDSSplitterWnd *m_wndSplitter;	
//	CExtDynamicControlBar *    m_pBarPersistent;
	WINDOWPLACEMENT            m_dataFrameWP;
#else
	CSplitterWnd *m_wndSplitter;
#endif	
public:
	int m_sinbarras;

	BOOL m_alsoltarcentrar;
	BOOL m_hayquecentrar;
		
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxFrame)
	public:
   virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   virtual void ActivateFrame(int nCmdShow = -1);
	protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);   
	//}}AFX_VIRTUAL

// Implementation
public:
   BOOL SetStatusBarText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
   CMenu* GetMenu( );
   void DrawMenuBar();
   void InstalaLook();
   void RecargaSkinBin();

	virtual ~CDsxFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	NOTIFYICONDATA m_nid;
	BOOL m_notifyicon;

#ifdef DSPROFUIS	

	CExtPaintManager *m_paintmanager; // base mas "alta" de todas

/*
#ifndef PROFUIS240
	CDsProfuisCustomOffice2007_R1 *m_paintmanager;
#else
	CDsProfuisCustomOffice2003 *m_paintmanager;
#endif
*/

	CToolDropTarget m_CToolDropTarget;
	CMenuDropTarget m_CMenuDropTarget;

	CExtToolControlBar m_wndToolBar;
	CExtToolControlBar  m_wndToolMensa;
	CExtToolControlBar m_gridtool;	
	CDsMenuToolbar m_dsmenubar;
	CExtStatusControlBar  m_wndStatusBar;	
	//CStatusBar   m_wndStatusBar;	
	//CReBar       m_wndReBar;		
	//CExtCTA < CExtMenuControlBar >  m_wndMenuBar;
	CExtMenuControlBar   m_wndMenuBar;	
	CMainToolBar m_toolweb;	
protected:  // control bar embedded members
#else
	CToolDropTarget m_CToolDropTarget;
	CMenuDropTarget m_CMenuDropTarget;
	CToolBar	 m_wndToolBar;
	CToolBar	 m_wndToolMensa;
	CToolBar	 m_gridtool;	
	CStatusBar   m_wndStatusBar;	
	//CReBar       m_wndReBar;		
	CMenuBar     m_wndMenuBar;
	CMainToolBar m_toolweb;
protected:  // control bar embedded members
#endif

	

// Generated message map functions
protected:
	//{{AFX_MSG(CDsxFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);   
   afx_msg void OnVertreemenu();      
   afx_msg void OnCentrar();
   afx_msg void OnSingleexpand();
   afx_msg void OnMultitarea();
	afx_msg void OnDestroy();
	afx_msg void OnMenuser();
	afx_msg void OnSiemprehtml();
	afx_msg void OnFondohtml();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateTestOtro(CCmdUI* pCmdUI);
	afx_msg void OnNextPane();
	afx_msg void OnPrevPane();
	afx_msg void OnAltDown();
	afx_msg void OnAltUp();		
	afx_msg void OnRecordLast();
	afx_msg void OnUpdateRecordLast(CCmdUI* pCmdUI);
	afx_msg void OnRecordFirst();
	afx_msg void OnUpdateRecordFirst(CCmdUI* pCmdUI);
	afx_msg void OnRecordDelete();
	afx_msg void OnUpdateRecordDelete(CCmdUI* pCmdUI);
	afx_msg void OnRecordNew();
	afx_msg void OnUpdateRecordNew(CCmdUI* pCmdUI);
	afx_msg void OnRecordNext();
	afx_msg void OnUpdateRecordNext(CCmdUI* pCmdUI);
	afx_msg void OnRecordPrev();
	afx_msg void OnUpdateRecordPrev(CCmdUI* pCmdUI);
	afx_msg void OnRecordUpdate();
	afx_msg void OnUpdateRecordUpdate(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnVercaption();	
	afx_msg void OnExportaExcel();
	afx_msg void OnUpdateExportaExcel(CCmdUI* pCmdUI);
	afx_msg void OnExportaWord();
	afx_msg void OnUpdateExportaWord(CCmdUI* pCmdUI);
	afx_msg void OnConsultaFiltro();
	afx_msg void OnUpdateConsultaFiltro(CCmdUI* pCmdUI);
	afx_msg void OnBuscamenu();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTooltips();
	afx_msg void OnActivarCristal();
	afx_msg void OnListatareas();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnImpredatos();
	afx_msg void OnControlremto();
	afx_msg void OnUpdateActivarErp(CCmdUI* pCmdUI);
	afx_msg void OnActivarErp();
	afx_msg void OnExportaPlb();
	afx_msg void OnUpdateExportaPlb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControlremto(CCmdUI* pCmdUI);
	afx_msg void OnExportaRpt();
	afx_msg void OnUpdateExportaRpt(CCmdUI* pCmdUI);
	afx_msg void OnActivarEvt();
	afx_msg void OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExportaXml();
	afx_msg void OnUpdateExportaXml(CCmdUI* pCmdUI);
	afx_msg void OnGridimprime();
	afx_msg void OnUpdateGridimprime(CCmdUI* pCmdUI);
	afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
	//}}AFX_MSG
	afx_msg BOOL DsToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivarEvtpbx();
	afx_msg void OnActivarEvtcrm();
#ifdef DSPROFUIS	
    afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
    afx_msg BOOL OnBarCheck(UINT nID);
    afx_msg LRESULT OnDrawPopupMenuItem( WPARAM wParam, LPARAM lParam ); 
#endif
	afx_msg LRESULT OnTray(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#ifdef DSPROFUIS



class CDsProfuisCustomOffice2007_R1 : public CExtPaintManagerOffice2007_R1
{	
public:
	DECLARE_DYNCREATE( CDsProfuisCustomOffice2007_R1 )
public:
	CDsProfuisCustomOffice2007_R1();
	

    virtual void InitTranslatedColors();
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		const CRect &rcDst,
		const CRect &rcWnd,
		COLORREF micolor
		);
	virtual bool DSPaintDockerBkgnd2003(
		bool bClientMapping,
		CDC & dc,
		const CRect &rcDst,
		const CRect &rcWnd,
		COLORREF micolor
		);
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		CWnd * pWnd,
		COLORREF micolor
		);
	virtual void PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);

	virtual void SetThemeColors(
		COLORREF clrFillHint,
		COLORREF clrAccentHint
		)
	{
		m_colorset = TRUE;
		m_clrFillHint = clrFillHint;
		m_clrAccentHint = clrAccentHint;
		InitTranslatedColors();
		InitHelperBrushes();

		g_CmdManager.OnGlobalPaintManagerChanged();

		CDsxFrame* pMainFrame = (STATIC_DOWNCAST(CDsxFrame,AfxGetMainWnd()));
		if (pMainFrame)
		{
		
			ASSERT_VALID( pMainFrame );
			pMainFrame->RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
				| RDW_FRAME | RDW_ALLCHILDREN
				);
			CExtControlBar::stat_RedrawFloatingFrames( pMainFrame );
			CExtControlBar::stat_RecalcBarMetrics( pMainFrame );
		}
	}


protected:
	BOOL m_colorset;
	
	virtual bool OnQueryThemeColors(
		COLORREF *pclrFillHint,
		COLORREF *pclrAccentHint = NULL
		)
	{

		if (theApp.m_colorset1 && theApp.m_colorset2)
		{
			m_clrFillHint = theApp.m_color1;
			m_clrAccentHint = theApp.m_color2;
		}
		return CExtPaintManagerOffice2007_R1::OnQueryThemeColors(pclrFillHint,pclrAccentHint);
		/*
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2007_R1::OnQueryThemeColors(pclrFillHint,pclrAccentHint);
		}		
		if( pclrFillHint != NULL )
			*pclrFillHint = m_clrFillHint;
		if( pclrAccentHint != NULL )
			*pclrAccentHint = m_clrAccentHint;
		return true;
	    */
	}
	virtual bool OnQueryUseThemeColors() const
	{
		return CExtPaintManagerOffice2007_R1::OnQueryUseThemeColors();
		/*
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2007_R1::OnQueryUseThemeColors();
		}
		return true;
		*/
	}

	virtual e_system_theme_t OnQuerySystemTheme() const
	{		
		return ThemeUnknown;	 
		//return CExtPaintManagerOffice2007_R1::OnQuerySystemTheme();
		/*
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2007_R1::OnQuerySystemTheme();
		}
		return ThemeUnknown;	 
		*/
	}
	

};

class CDsProfuisCustomOffice2003 : public CExtPaintManagerOffice2003
{	
public:
	DECLARE_DYNCREATE( CDsProfuisCustomOffice2003 )
public:
	CDsProfuisCustomOffice2003()
	{
		m_colorset = FALSE;
	};
	void PintaBotonRedondo(CDC & dc,LPCRECT rectClient, COLORREF clrTopLeft, COLORREF clrBottomRight  );


    virtual void InitTranslatedColors();
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		const CRect &rcDst,
		const CRect &rcWnd,
		COLORREF micolor
		);
	virtual bool DSPaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		CWnd * pWnd,
		COLORREF micolor
		);
	virtual bool PaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);
	virtual void PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);

	virtual void SetThemeColors(
		COLORREF clrFillHint,
		COLORREF clrAccentHint
		)
	{
		m_colorset = TRUE;
		m_clrFillHint = clrFillHint;
		m_clrAccentHint = clrAccentHint;
		InitTranslatedColors();
		InitHelperBrushes();

		CDsxFrame* pMainFrame = (STATIC_DOWNCAST(CDsxFrame,AfxGetMainWnd()));
		if (pMainFrame)
		{
		
			ASSERT_VALID( pMainFrame );
			pMainFrame->RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
				| RDW_FRAME | RDW_ALLCHILDREN
				);
			CExtControlBar::stat_RedrawFloatingFrames( pMainFrame );
			CExtControlBar::stat_RecalcBarMetrics( pMainFrame );
		}
	}

	BOOL m_colorset;
	COLORREF m_clrFillHint;
	COLORREF m_clrAccentHint;

protected:
	
	virtual bool OnQueryThemeColors(
		COLORREF *pclrFillHint,
		COLORREF *pclrAccentHint = NULL
		)
	{
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2003::OnQueryThemeColors(pclrFillHint,pclrAccentHint);
		}
		if( pclrFillHint != NULL )
			*pclrFillHint = m_clrFillHint;
		if( pclrAccentHint != NULL )
			*pclrAccentHint = m_clrAccentHint;
		return true;
	}
	virtual bool OnQueryUseThemeColors() const
	{
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2003::OnQueryUseThemeColors();
		}
		return true;
	}

	virtual e_system_theme_t OnQuerySystemTheme() const
	{
		if (!m_colorset)
		{
			return CExtPaintManagerOffice2003::OnQuerySystemTheme();
		}
		return ThemeUnknown;
	}
	

};
#endif
extern CDsxFrame * pMFrame;

#ifdef DSPROFUIS
class CDSSplitterWnd : public CExtSplitterWnd
{
public:
	virtual void OnDrawSplitter(
		CDC* pDC,
		ESplitType nType,
		const CRect & rectArg
		)
	{
		if (pMFrame->m_sinbarras)
			return;
		CExtSplitterWnd::OnDrawSplitter(pDC,nType,rectArg);
	}
	BOOL Activado()
	{
		if (m_pColInfo)
			return TRUE;
		return FALSE;
	}
};
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXMAINFRM_H__8C64B70B_7E9D_47B7_948C_30D9CED5D8B4__INCLUDED_)
