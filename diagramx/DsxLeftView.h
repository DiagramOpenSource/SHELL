// DsxLeftView.h : interface of the CDsxLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSXLEFTVIEW_H__3591B32A_DE2C_4214_A09E_36C37E84B26F__INCLUDED_)
#define AFX_DSXLEFTVIEW_H__3591B32A_DE2C_4214_A09E_36C37E84B26F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDsxDoc;


class CDsxLeftView : public CTreeView
{
protected: // create from serialization only
	CDsxLeftView();
	DECLARE_DYNCREATE(CDsxLeftView)

// Attributes
public:
	CDsxDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxLeftView)
	public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AsignaAtecla(int item);
	int GetCorriente();
	HTREEITEM BuscaItemData(HTREEITEM primero,int data);
	BOOL BorraSubOpciones(HTREEITEM item);
	void RellenaMenuPropio();
	BOOL BorraMenuPropio(BOOL todo = FALSE);
	BOOL BorraBusqueda();	
	HTREEITEM GetNextItem(HTREEITEM item);
	void BuscaYRellenaMenuUser(LPCSTR patern);
   void PintaColapso(HTREEITEM item);
   BOOL EjecutaSeleccion();
	virtual ~CDsxLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CImageList m_ImageList;

// Generated message map functions
protected:
   void ShowPopupMenu( CPoint& point );
   //afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CDsxLeftView)
   afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);	
   afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DsxLeftView.cpp
inline CDsxDoc* CDsxLeftView::GetDocument()
   { return (CDsxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXLEFTVIEW_H__3591B32A_DE2C_4214_A09E_36C37E84B26F__INCLUDED_)
