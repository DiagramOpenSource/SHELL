
#ifdef YANOEXISTE

#if !defined(AFX_WINDOWDS_H__5E4B6417_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_)
#define AFX_WINDOWDS_H__5E4B6417_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindowDs.h : header file
//


class CEntornoDs;
class CThreadDs;
class CTabMenu;
class CMainFrame;
class CDsEdit;
class CTablaW;
/////////////////////////////////////////////////////////////////////////////
// CWindowDs window

extern "C"
{
void EntradaBinaria(int entero,void *puntero);
}


#define PARENTCWindowDs CTabCtrl
class CWindowDs : public CTabCtrl
{
   friend CThreadDs;
   friend CEntornoDs;
   friend CMainFrame;
   // Construction
public:
   CWindowDs(CEntornoDs *pEDs);
   
   friend void EntradaBinaria(int entero,void *puntero);
   // Attributes
public:
   
   // Operations
public:
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CWindowDs)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
   
   // Implementation
public:
	BOOL PintaCampo(int tipo,int id_control,int id_campo,char *dato);
	void PonFoco();
	BOOL ActivaControles(BOOL recursion = FALSE);
	BOOL DesactivaControles(BOOL recursion = FALSE);
	BOOL EliminaGrid(int ind_grid);
	BOOL CreaGrid(void *puntero);
   int  ActivaEdita(char *txt,int area);
   void HazVisible();
   void SubeVentanaBuena();
   void CalculaRect();
   void *m_salvastdpan;
   void DesactivaTabMenu(int codigo);
   void ActivaTabMenu(char *opcion,CRect *rect,int codigo,int precodigo,int defecto);
   void DesactivaScrollBar(void);
   void ActivaScrollBar(CRect *rect,BOOL vertical = TRUE);
   virtual ~CWindowDs();
   CToolTipCtrl m_tooltip;

   int m_nivel_recursion;
   
   // Generated message map functions
protected:
   //{{AFX_MSG(CWindowDs)
   afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnPaint();
   afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   BOOL m_visible;
   CScrollBar *m_ScrollBar[100];	
   int m_IndScrollBar;
   CTabMenu   *m_TabMenu[100];
   int m_IndTabMenu;
   CDsEdit    *m_Edit[100];
   int m_IndEdit;
   CTablaW    *m_Grid[100];   
   int m_IndGrid;
   CRect m_rect;	
   CEntornoDs * m_pEDs;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWDS_H__5E4B6417_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_)

#endif