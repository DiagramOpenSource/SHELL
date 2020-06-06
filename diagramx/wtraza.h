#if !defined(AFX_WTRAZA_H__286ADD11_03C8_11D3_B168_004F4905CBCF__INCLUDED_)
#define AFX_WTRAZA_H__286ADD11_03C8_11D3_B168_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WTraza.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWTraza window
/////////////////////////////////////////////////////////////////////////////
// CVarTraza window

#include "SplitterBar.h"


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTrazaBuscar dialog

class CTrazaBuscar : public CDialog
{
// Construction
public:	
	CTrazaBuscar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrazaBuscar)
	enum { IDD = IDD_TRAZA_BUSCAR };
	CString	m_abuscar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrazaBuscar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrazaBuscar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CFonTraza window

typedef CMap< int ,int&,BOOL,BOOL> CBreaks;


class CInPlaceEdit;
//typedef CMap< int ,int&, CNodo*, CNodo* > CNodoArray;

class CFonTraza : public CListCtrl
{
// Construction
public:
	CFonTraza();
    int m_buslinea;
// Attributes
public:
   
    int  ind_fichero;
    char ficheros[50][10];
    CBreaks m_breaks[50];
	char m_fichero[10];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFonTraza)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Busca(LPCSTR s);
	int m_linea;
	BOOL Situate(int linea);
	BOOL PonTexto(LPCSTR texto,BOOL append = FALSE);
	BOOL PrepareControl(CWnd *pariente,CRect &area,CFont *font);
	virtual ~CFonTraza();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFonTraza)
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class CVarTraza : public CListCtrl
{
// Construction
public:
	CVarTraza();

// Attributes
public:

   CInPlaceEdit* m_pListEdit;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarTraza)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertaUno(int item,LPCSTR texto1,LPCSTR texto2);
	BOOL PositionControl( CWnd * pWnd, int iItem, int iSubItem );
	void Activa();
	void Desactiva();
	BOOL PrepareControl(CWnd *pariente,CRect &area,CFont *font);
	BOOL m_activa;
	virtual ~CVarTraza();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVarTraza)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

class CWTraza : public CWnd
{
// Construction
public:
	CWTraza();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWTraza)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_haybusqueda;
	void ActivaBreak();
	void PideVal(int var);
	void PideVar(int var);
	BOOL SetBreaks(LPCSTR p);
	BOOL m_continuar;
	BOOL m_salto;
	BOOL m_corre;
	BOOL m_refresca_breaks;
	int m_modoavance;
	BOOL m_activa;
	void DesactivaTraza(int entero,void *puntero);
	void PuntoTraza(int entero,void *puntero);
	BOOL m_visible;
	BOOL SetTexto(LPCSTR texto,BOOL append = FALSE);
	CRect m_rect;
	CFonTraza m_edit;
	CFont* m_pFont;
	CMenu m_menu;
	CSplitterBar m_split;
	CVarTraza m_vars;
    CTrazaBuscar m_buscar;

	virtual ~CWTraza();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWTraza)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrazaAvanceuno();
	afx_msg void OnTrazaCorrer();
	afx_msg void OnTrazaAvancecontinuo();
	afx_msg void OnTrazaAvancesalto();
	afx_msg void OnTrazaTodosbreak();
	afx_msg void OnTrazaAbrir();
	afx_msg void OnClose();
	afx_msg void OnTrazaBuscar();
	afx_msg void OnTrazaSiguiente();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window

class CInPlaceEdit : public CEdit
{
// Construction
public:
	CInPlaceEdit(int iItem, int iSubItem, CString sInitText);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void CalculateSize();
	virtual ~CInPlaceEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
        int m_iItem;
        int m_iSubItem;
        CString m_sInitText;
        BOOL    m_bESC;         // To indicate whether ESC key was pressed
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WTRAZA_H__286ADD11_03C8_11D3_B168_004F4905CBCF__INCLUDED_)
