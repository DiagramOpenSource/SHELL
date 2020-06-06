// dsxView.h : interface of the CDsxView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSXVIEW_H__0B212BFD_F402_41B5_B58D_6B594AA02B2F__INCLUDED_)
#define AFX_DSXVIEW_H__0B212BFD_F402_41B5_B58D_6B594AA02B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DsHtml.h"

class CDsxCntrItem;

class CDsxLeftView;

class CDsStdDlg;

class CDlgErrorFichero;

class CDsxView : public CScrollView 
{
	friend CDsxLeftView;
	
protected: // create from serialization only
	CDsxView();
	DECLARE_DYNCREATE(CDsxView)

// Attributes
public:   

   CDsHtml *m_html;
   CBitmap *m_bmp;

   CStdDlgLst m_dlglist;

	CDsxDoc* GetDocument();
	// m_pSelection holds the selection to the current CDsxCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CDsxCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	CDsxCntrItem* m_pSelection;

	CDsStdDlg **m_listaoculta;
	int m_prioridad[500];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxView)
	public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
   virtual void OnInitialUpdate(); // called first time after construct
   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
   virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ActivaHtml(void);
	BOOL FinErrorFichero();
	int ErrorFichero(char *data1,char *data2,char *data3,int ok);
	BOOL EnsureVisible(CRect *pwrect); // la ventana es en screen coordinates
    BOOL TeclaStd(int tecla,int wid = -1,int id = -1,int sid = -1,int wcid = -1,int ctipo = -1,Cid cid = NULO_CID);
	int ControlEdicion(int wid, int id, int sid, int cwid, Cid cid, int flag,void *datos,int pre_id = -1);
	CDsStdDlg* LocalizaDlg(int wid,int id,int sid,int lpi,int lpf,BOOL best = FALSE);
	CDsStdDlg* ActivaDlg(int wid,int id,int sid,BOOL activar = TRUE);
	int InsertaControl(int wid,int id,int sid,int tipo,int lpos,int lpf,void *dato,int Atrib = (int)' ', int Cc = -1, int Cf = -1,int prev_id = -1);
	CDsStdDlg * BuscaStdDlg(int id,int sid,int& stdid);
	BOOL EliminaStdDlg(int stdid);
   void SetScrollDs(void);
    void ocultaVentanas();
	void muestraVentanas();
	virtual ~CDsxView();

   CDlgErrorFichero *m_errorf;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDsxView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
   afx_msg void OnDestroy();
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnInsertObject();
   afx_msg void OnCancelEditCntr();
   afx_msg void OnCancelEditSrvr();
   afx_msg void OnUpdateOleInsertNew(CCmdUI* pCmdUI);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG	
    
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dsxView.cpp
inline CDsxDoc* CDsxView::GetDocument()
   { return (CDsxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXVIEW_H__0B212BFD_F402_41B5_B58D_6B594AA02B2F__INCLUDED_)
