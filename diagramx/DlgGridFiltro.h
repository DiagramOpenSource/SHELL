#if !defined(AFX_DLGGRIDFILTRO_H__58943F57_C018_49D8_984E_0A919197D7DD__INCLUDED_)
#define AFX_DLGGRIDFILTRO_H__58943F57_C018_49D8_984E_0A919197D7DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGridFiltro.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFiltroData
#define MAXFLTITEMS 1024

class CFiltroDef;

class CFiltroItem
{
public:
    CFiltroItem(CFiltroDef *padre);
	CFiltroItem(CFiltroItem &otro);
    virtual ~CFiltroItem();
	BOOL EstaIncluido();
	BOOL TieneFiltro();
	void operator=(CFiltroItem &otro);

	CFiltroDef *m_padre;
    int  m_idx;
	int  m_col;
	int  m_campo;
	int  m_lidx;
    BOOL m_checked;
    CString m_name;
    CString m_LimiteI;
    CString m_LimiteS;
    CString m_Valor;
    BOOL    m_Mayus;
    BOOL    m_Inver;
    int     m_TipoValor;
	BOOL    m_Orden;
	BOOL    m_OrdenInverso;
	BOOL    m_Agrupado;
	BOOL    m_CondAgrupada;
	BOOL    m_Preguntar;
	BOOL    m_salida;
	int     m_opera;
};

class CInfoDef;
class CFiltroDef;

class CTablaView;

class CFiltroDef
{
public:
	CFiltroDef(const char *nombredef,int tammem,CInfoDef *infodef = NULL,CFiltroDef *padre = NULL,int camposrel = 0,int *campospadre = NULL,int *camposhijo = NULL,HTREEITEM hitem = NULL,BOOL manual = FALSE);
	virtual ~CFiltroDef();
	BOOL PorDefecto(BOOL incluir = FALSE);
	void MemoriaItems();
	void BorraItems();
	BOOL RellenaDef(CTablaView *pTabla);
	void BorraParientes();
	void Relacion(CFiltroDef *padre,int camposrel,int *campospadre,int *camposhijo);

	CString m_nomdef;
	CString m_id;
	CString m_nombre;

	CFiltroDef *m_padre;

	HTREEITEM  m_hitem;

	BOOL       m_manual;
	int        m_camposrel;
	int        m_campospadre_o[32]; // 16 = MAXCAMPOSPORCLAVE
	int        m_camposhijo_o[32]; // 16 = MAXCAMPOSPORCLAVE
	int        m_campospadre[32]; // 16 = MAXCAMPOSPORCLAVE
	int        m_camposhijo[32]; // 16 = MAXCAMPOSPORCLAVE

	int         m_nparientes;
	CFiltroDef **m_parientes;

	int          m_nitem;
	CFiltroItem **m_item;    
	int          *m_idx;
	int          m_sel;

	int m_tammen;
	CInfoDef *m_def;
private:
	int m_borradef;
};

/////////////////////////////////////////////////////////////////////////////
// CFiltroCListBox window

class CFiltroCListBox : public CCheckListBox
{
// Construction
public:
	CFiltroCListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiltroCListBox)
	public:
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFiltroCListBox();    

	// Generated message map functions
protected:
	//{{AFX_MSG(CFiltroCListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CDlgGridFiltro dialog


class CDlgGridFiltro : public CExtNCW < CBASEDLG >
{
// Construction
public:
	void MemoriaItems();
	BOOL CargaPredeterminado();
	void PonPredeterminado();
	void BorraItems();
	void RellenaMenu(CMenu *filtros,int desde,int id);
	void InicializaItems();
	BOOL LeerFiltros(LPCSTR clave,CString &filtros);
	BOOL GrabaFiltros(LPCSTR clave,LPCSTR filtros);
	BOOL QuizaGraba();
	BOOL LeeDelRegistro();
	BOOL PorDefecto();
	BOOL RecogeLista();
	void RellenaLista();
	BOOL RellenaListado(BOOL offline = FALSE);
	BOOL EsSimple();
	void Guarda(BOOL Borra = FALSE);
	int RellenaItemsListado(CFiltroDef *pdef);
	void OrdenId(int orden,CString id);
	CDlgGridFiltro(/*CWnd* pParent = NULL*/CTablaView *pTabla = NULL);   // standard constructor
    virtual ~CDlgGridFiltro();

    CTablaView *m_pTabla;
	CFiltroDef  *m_principal;
	CFiltroDef  *m_seleccion;
	
	int			 m_nitems;
	CFiltroItem *m_items[MAXFLTITEMS];
	int m_idxs[MAXFLTITEMS]; // indice al listado, se crea consecutivo luedo se ordena
	CString m_ordenid[MAXFLTITEMS]; // que tenemos metido

    /*CFiltroItem **m_item;*/
    /*int         *m_idx;*/
    /*int          m_nitem;*/
    /*int          m_sel;*/

    CString      m_nombre;

	CString      m_npredeterminado;

	BOOL         m_bpredeterminado;

    BOOL         m_modificado;

	BOOL		 m_filtrousuario;

	void OnSelchangeLista();	

	int m_sel;

// Dialog Data
	//{{AFX_DATA(CDlgGridFiltro)
	enum { IDD = IDD_FILTRO };
	CListBox	m_relaciones;
	CListCtrl	m_listado;
	CTreeCtrl	m_ficheros;
	CDLGCOMBO	m_combo;
	CFiltroCListBox	m_Lista;
	BOOL	m_soloseleccionados;
	BOOL	m_solofiltro;
	BOOL	m_alfabetico;
	CString	m_limitei;
	CString	m_limites;
	CString	m_valor;
	BOOL	m_mayus;
	BOOL	m_inversa;
	BOOL	m_predeterminado;
	CString	m_nomcampo;
	BOOL	m_registrolocal;
	BOOL	m_agrupado;
	BOOL	m_condagrupada;
	BOOL	m_preguntar;
	BOOL	m_presenta;
	int		m_opera;
	//}}AFX_DATA

	CDLGBUTTON m_boton1;
	CDLGBUTTON m_boton2;
	CDLGBUTTON m_boton3;
	CDLGBUTTON m_boton4;
	CDLGBUTTON m_boton5;
	CDLGBUTTON m_boton6;
	CDLGBUTTON m_boton7;
	CDLGBUTTON m_boton8;
	CDLGBUTTON m_boton9;
	CDLGBUTTON m_boton10;
	CDLGBUTTON m_boton11;

	CDLGCOMBO	m_combo1;
	CDLGCOMBO	m_combo2;
	CDLGCOMBO	m_combo3;

	CDLGEDIT m_edit1;
	CDLGEDIT m_edit2;
	CDLGEDIT m_edit3;
	CDLGEDIT m_edit4;    

	CDLGGROUP m_label1;
	CDLGGROUP m_label2;
	CDLGGROUP m_label3;
	CDLGGROUP m_label4;
	CDLGGROUP m_label5;
	CDLGLABEL m_label6;
	CDLGLABEL m_label7;
	CDLGLABEL m_label8;
	CDLGLABEL m_label9;
	CDLGLABEL m_label10;
	CDLGLABEL m_label11;

	CDLGCHECK m_check1;
	CDLGCHECK m_check2;
	CDLGCHECK m_check3;
	CDLGCHECK m_check4;
	CDLGCHECK m_check5;
	CDLGCHECK m_check6;
	CDLGCHECK m_check7;
	CDLGCHECK m_check8;
	CDLGCHECK m_check9;
	CDLGCHECK m_check10;
	CDLGCHECK m_check11;

	CDLGRADIO m_radio1;
	CDLGRADIO m_radio2;
	CDLGRADIO m_radio3;
	
	CDLGRADIO m_radio4;
	CDLGRADIO m_radio5;
	CDLGRADIO m_radio6;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGridFiltro)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	afx_msg void OnCheckLista();

	// Generated message map functions
	//{{AFX_MSG(CDlgGridFiltro)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonguardar();
	afx_msg void OnSeleccionados();
	afx_msg void OnSoloFiltro();
	afx_msg void OnOrdenAlfabetico();	
	virtual void OnOK();
	afx_msg void OnRegistroLocal();
	afx_msg void OnChangeDesde();
	afx_msg void OnChangeHasta();
	afx_msg void OnChangeValor();
	afx_msg void OnSelendokCombo();
	afx_msg void OnNinguno();
	afx_msg void OnTodos();
	virtual void OnCancel();
	afx_msg void OnAscendente();
	afx_msg void OnSinorden();
	afx_msg void OnDescendente();
	afx_msg void OnRadioexacto();
	afx_msg void OnRadioparcial();
	afx_msg void OnRadiocontenido();
	afx_msg void OnCheck7();
	afx_msg void OnButtonborrar();
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNuevarelacion();
	afx_msg void OnFormato();
	afx_msg void OnSelchangeList1();
	afx_msg void OnBeginlabeleditList4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditchangeComboopera();
	afx_msg void OnCheckincluido();
	afx_msg void OnSelchangeComboopera();
	afx_msg void OnAgrupador();
	afx_msg void OnArriba();
	afx_msg void OnAbajo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRIDFILTRO_H__58943F57_C018_49D8_984E_0A919197D7DD__INCLUDED_)
