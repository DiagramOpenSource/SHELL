#if !defined(AFX_TABLAVIEW_H__C1BDED83_090F_11D3_B178_004F4905CBCF__INCLUDED_)
#define AFX_TABLAVIEW_H__C1BDED83_090F_11D3_B178_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TablaView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTablaView window

#define COLUMNA_INICIAL 1
#define COLUMNA_FINAL   1

class CDsCombo;
class CDsxDoc;
class CDsxView;
class CDsStdCtrl;
class CDlgGridFiltro;
class CDlgCol;
class CTablaView;
class DlgRegCambio;

typedef CMap< int ,int&,char *,char *> CDataCache;

class CInfoDef
{
public:
	void PintaCamposStaticos();
	int EsDeClave(int campo,BOOL directo = FALSE);
	int EsDeLaClave(int campo,int clave,BOOL directo = FALSE);
	int k_orden(int clave,int comp);
	int k_campo(int clave,int comp,BOOL directo = FALSE);
	struct Info_Entra_Clave* Clave(int clave);
	int EsInicialClave(int campo,BOOL directo = FALSE,int laclave = -1);
    BOOL CreaCamcolu(BOOL iniciar = FALSE,BOOL todas = FALSE);
    CInfoDef(struct Info_Entra_Campos *info,CTablaView *pTabla);
    ~CInfoDef();
    
    struct Info_Entra_Campo *Campo(int campo,BOOL directo = FALSE);
    char *c_nombre(int campo,BOOL directo = FALSE);
    char *c_minimo(int campo,BOOL directo = FALSE);
    char *c_maximo(int campo,BOOL directo = FALSE);
    char *c_usi(int campo,BOOL directo = FALSE);

    int *m_idcampos;

    int *m_colcampos;

    int m_n_campos_col;

    BOOL m_usermode;
	

    CTablaView *m_pTabla;
	CString m_RegPath;

    struct Info_Entra_Campos *m_info;
};

class CTablaEdit;
class CTablaW;
int compare_orden_string( const void *arg1, const void *arg2 );
int compare_orden_fecha( const void *arg1, const void *arg2 );

//public CExtWFF < CExtWS < CBASELISTCTRL > >
class CTablaView : public CExtWFF < CExtWS < CBASELISTCTRL > >
{
    friend class CTablaW;
    friend class CDlgGridFiltro;
	friend class CDlgCol;
	friend int compare_orden_string( const void *arg1, const void *arg2 );
	friend int compare_orden_fecha( const void *arg1, const void *arg2 ); 
// Construction
public:
	CTablaView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTablaView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void InvalidateRect( LPCRECT lpRect, BOOL bErase = TRUE );

// Implementation
public:
	void AjustaMarco();
	BOOL ExportaDbf(CString &destino);
	BOOL GuardaPosiciones();
	BOOL LocalizaRegistroOrdenado(CString &str,long ini = 0,long fin = -1);
	void PonTotalRegistros(long registros);
	void ActualizaVentana();
	void MarcasCabecera();
	void QuitaWait();
	void PonWait();
	void FinReorden();
	int MAX_CACHE;
	int MAX_ORDENACION_LOCAL;
	int MAXCOMPLETO;
	BOOL DobleClick();
	int DespachadorInterno(int parametro,void *buffer);
	BOOL EnviaCamcolu();
	BOOL CreaColumnas();
	void EjecutaRegCambio(DlgRegCambio *pdlg);
	BOOL EjecutaFiltro(CDlgGridFiltro *dlg);
	BOOL Filtros(BOOL Predeterminado = FALSE,BOOL conmenu = FALSE);
	BOOL BorraLinea();
	BOOL Refresca();
	BOOL MantenimientoLinea();
	BOOL TeclaStd(int tecla);
	void PintaIdCampo(int idcampo,char *dato);
	long m_ultpospadre;
	BOOL EdicionLocal(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OpcionesConsulta(POINT pp);
	long RegistroDelPadre(long reghijo);
	BOOL PonRegistros(int registros);
	BOOL NuevoCorriente(int nuevo,BOOL posicionalo = TRUE);
	BOOL CambiarOrden(int clave);
	BOOL NotificaPosicion();
	BOOL Scroll( CSize size );
	void EnviaDato(CString &str);
	void CambiaCorriente(int nuevo,int salto = 0,BOOL nonotifica = FALSE);
	void BorraCache(int from = -1);
    int * m_tamcampos;
    int * m_flagcampos;
    int * m_poscampos;
	int * m_iniciales;
	int * m_orderarray;
	int * m_clavesvalidas;
	int * m_totalizadores;
	BOOL m_repintar;
	BOOL Cacheamelo(int from,int to);
	BOOL m_cacheado;
	int m_mayor_cache;
	int m_menor_cache;
	int m_registros;
	BOOL PrepareControl(CWnd *pariente,CRect &area,struct Info_Entra_Campos *Columnas,long registros,CFont *font,int localizador,long inicial,int g_modo,int id,int padre,CDsxView *principal,char *pextra = NULL);
    CDsxView *m_principal;
	virtual ~CTablaView();

	int m_sincachear;
	int m_yomismo;
	int m_enclic;
	int m_clicposiciona;
	int m_enteclado;
	int m_precursel;
	int m_postposi;

    BOOL EnsureVisible( int nItem, BOOL bPartialOK );

	unsigned long DameIsrecCorriente();

	int EnviaRetro(int operacion,int ida,int vuelta,char *buffer);


	CWnd m_marcototal;
#ifdef DSPROFUIS
	CExtEdit m_totaledit[20];
#else
	CEdit m_totaledit[20];
#endif
    CTablaW *m_tw;

    int m_padre;

    int m_id;

    BOOL PosicionElemento(RECT *prect, int iItem, int iSubItem,BOOL Absoluto = FALSE);

    void AjustaColumnaFinal();

    int m_curcol;
    int m_cursel;    

    int m_gmodo;
	int m_ini_gmodo;
    
    class CInfoDef *m_def;

    CTablaEdit *m_edit; // edicion interna independiente
    CDsCombo *m_dsedit;  // edicion de campo dependiente
    BOOL m_editando;

	int m_solofoco;

	CRect m_recttotal;

    RECT* m_pintarects;

    BOOL m_confocus;

    int m_localizador;

    int m_NotificaOnDraw;

	int m_reordenado;
	int *m_reorden;

    CDsxDoc *m_pDoc;

	BOOL m_escampo;
	BOOL m_noesconsulta;

	BOOL m_okpredeterminado;

	int m_focohijo;

	HCURSOR m_prevcursor;	
	int m_cursorwaits;

	BOOL m_primerapintada;

	unsigned long m_cuentaupdate;

	CString m_defaultama;

	int m_mtecla;

	BOOL m_totbpedirdatos;			

	BOOL m_ajustadomarco;


	BOOL m_refrespending;


	void OnExportaExcel(LPCSTR fichero);



	// Generated message map functions
protected:
	//{{AFX_MSG(CTablaView)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdfinditem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegintrack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnddrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnExportaExcel();
	afx_msg void OnExportaWord();
	afx_msg void OnExportaPlb();
	afx_msg void OnExportaRpt();
	afx_msg void OnExportaXml();
	afx_msg void OnGridimprime();	
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	/*afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );*/

	DECLARE_MESSAGE_MAP()
private:	
	int* m_apintar;
    char * m_buffer;
    CDataCache m_cache;
	int m_lenreg;
	BOOL m_pintando;	
	BOOL m_flagpintar;
	BOOL m_guardaposiciones;
	BOOL m_guarda_posicion;
	int m_columnas;	
    CImageList* pHdrImageList;    
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTablaW window

#include "DsGridButton.h"

class CMainFrame;

class CTablaW : public CExtScrollWnd // CExtWFF < CExtWS < CWnd > >
{
    friend class CDsxFrame;	
// Construction
public:
	CTablaW();

// Attributes
public:

	CWnd *m_skinw;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTablaW)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int TotalBoton();
	int BotonPintado(int itm,int modo);
	int AseguraBoton(int itm);
	CFont * m_font;
	CRect m_rect;
	BOOL Crea(CWnd *Pariente,LPCSTR Titulo,struct Info_Entra_Campos *Columnas,long registros,long inicial,int id,CFont *font,int localizador,int g_modo,RECT *rect,int padre,CDsxView *principal,char *pextra = NULL);
	CTablaView m_tabla;
    CDsGridButton *m_consulta;
    CDsGridButton **m_seleccion;
    int m_botsel;
    int m_topsel;
	virtual ~CTablaW();
    int m_gmodo;
    int m_padre;
    CRect m_padrepos;
	CWnd* SetFocus( );

    CDsStdCtrl *m_dsctrl;

	BOOL m_escampo;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTablaW)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExportaWord();
	afx_msg void OnExportaExcel();
	afx_msg void OnExportaPlb();
	afx_msg void OnExportaRpt();
	afx_msg void OnExportaXml();
	afx_msg void OnGridimprime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTablaEdit window

class CTablaEdit : public CEdit
{
// Construction
public:

// Attributes
public:
	HWND m_parienta;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTablaEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CTablaEdit(int iItem, int iSubItem, CString sInitText);
	void CalulateSize();
	virtual ~CTablaEdit();

    BOOL    m_bESC;         // To indicate whether ESC key was pressed        

private:
        int m_iItem;
        int m_iSubItem;
        CString m_sInitText;        

	// Generated message map functions
protected:
	//{{AFX_MSG(CTablaEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLAVIEW_H__C1BDED83_090F_11D3_B178_004F4905CBCF__INCLUDED_)
