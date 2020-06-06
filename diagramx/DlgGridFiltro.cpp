

// DlgGridFiltro.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgGridFiltro.h"
#include "DlgNewRela.h"

#include "TablaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char *quita_path(const char *ff);


/* informacion redundante ... por ampliaciones ... en fin ... */
/*
class CFiltroItem
{
public:
    CFiltroData();
    virtual ~CFiltroData();
    int  m_idx;
    BOOL m_checked;
    CString m_name;
    CString m_LimiteI;
    CString m_LimiteS;
    CString m_Valor;
    int     m_TipoValor;
}
*/

CFiltroItem::CFiltroItem(CFiltroDef *padre)
{
	m_padre = padre;
	m_name = "";
    m_idx = -1;
	m_lidx = -1;
	m_col = -1;
	m_campo = -1;
    m_TipoValor = 4;
    m_checked = FALSE;
    m_Mayus = FALSE;
    m_Inver = FALSE;
    m_LimiteI = "";
    m_LimiteS = "";
    m_Valor = "";
	m_Orden = FALSE;
	m_OrdenInverso = FALSE;
	m_Agrupado = FALSE;
	m_CondAgrupada = FALSE;
	m_Preguntar = FALSE;
	m_salida = TRUE;
	m_opera = 0;
}

CFiltroItem::CFiltroItem(CFiltroItem &otro)
{
	m_padre = otro.m_padre;
    m_col = otro.m_col;
	m_campo = otro.m_campo;	
	m_name = otro.m_name;
    m_TipoValor = otro.m_TipoValor;    
    m_Mayus = otro.m_Mayus;
    m_Inver = otro.m_Inver;
    m_LimiteI = otro.m_LimiteI;
    m_LimiteS = otro.m_LimiteS;
    m_Valor = otro.m_Valor;
	m_Orden = otro.m_Orden;
	m_OrdenInverso = otro.m_OrdenInverso;
	m_Agrupado = otro.m_Agrupado;
	m_CondAgrupada = otro.m_CondAgrupada;
	m_Preguntar = otro.m_Preguntar;
	m_salida = otro.m_salida;
	m_opera = otro.m_opera;	

	/* ESTOS NO SE COPIAN  */	
	m_checked = FALSE;
	m_lidx = -1;
    m_idx = -1;	
}

void CFiltroItem::operator=(CFiltroItem &otro)
{
	m_padre = otro.m_padre;
    m_col = otro.m_col;
	m_campo = otro.m_campo;
	m_name = otro.m_name;
    m_TipoValor = otro.m_TipoValor;    
    m_Mayus = otro.m_Mayus;
    m_Inver = otro.m_Inver;
    m_LimiteI = otro.m_LimiteI;
    m_LimiteS = otro.m_LimiteS;
    m_Valor = otro.m_Valor;
	m_Orden = otro.m_Orden;
	m_OrdenInverso = otro.m_OrdenInverso;
	m_Agrupado = otro.m_Agrupado;
	m_CondAgrupada = otro.m_CondAgrupada;
	m_Preguntar = otro.m_Preguntar;
	m_salida = otro.m_salida;
	m_opera = otro.m_opera;	

	/* ESTOS NO SE COPIAN  */
	/*
	m_checked = FALSE;
	m_lidx = -1;
    m_idx = -1;
	*/
}

CFiltroItem::~CFiltroItem()
{
}

BOOL CFiltroItem::TieneFiltro()
{
	if (!m_Valor.IsEmpty() || 
	    !m_LimiteI.IsEmpty() || 
	    !m_LimiteS.IsEmpty() ||
		m_opera > 0)
	{		
		return TRUE;
	}
	return FALSE;
}

BOOL CFiltroItem::EstaIncluido()
{
	/*
	if (!confiltro)
		return m_checked;
	
	if (!m_Valor.IsEmpty() || 
	    !m_LimiteI.IsEmpty() || 
	    !m_LimiteS.IsEmpty() ||
		m_opera > 0)
	{		
		return TRUE;
	}

	if (!noorden && m_Orden)
		return TRUE;

	*/
	return FALSE;
}

CFiltroDef::CFiltroDef(const char *nombredef,int tammem,CInfoDef *infodef,CFiltroDef *padre,int camposrel,int *campospadre,int *camposhijo,HTREEITEM hitem,BOOL manual)
{
	m_manual = manual;

	m_nomdef = nombredef;
	m_tammen = tammem;
	m_hitem = hitem;
	m_def = infodef;	
    m_item = NULL;
    m_nitem = 0;
    m_idx = NULL;
	m_sel = -1;
	m_borradef = 0;
	m_nparientes = 0;
	m_parientes = NULL;

	Relacion(padre,camposrel,campospadre,camposhijo);

	if (infodef)
	{
		m_nombre = infodef->m_info->NomFic;		
		MemoriaItems();
		PorDefecto();
	}
}

BOOL CFiltroDef::RellenaDef(CTablaView *pTabla)
{
	if (m_def)
		return TRUE;

	if (m_padre)
		m_padre->RellenaDef(pTabla);

	int len;
	char *p0;
	if (!m_tammen)
	{
		char tmp[512];
		len = strlen(m_nomdef)+1;
		p0 = tmp + (sizeof(long) * 3) + (sizeof(int)*2);
		strcpy(p0,m_nomdef);
		pTabla->EnviaRetro(22,len,sizeof(int)+1,tmp);
		if (*p0 != 1)
			return FALSE;
		p0++;
		m_tammen = *((int *)p0);
		if (m_tammen <= 0)
			return FALSE;
	}
	len = strlen(m_nomdef)+1;
	char *buffer = (char *)malloc(m_tammen+128);
	p0 = buffer + (sizeof(long) * 3) + (sizeof(int)*2);
	strcpy(p0,m_nomdef);
	pTabla->EnviaRetro(21,len,m_tammen+1,buffer);
	if (*p0)
	{
		p0++;
		m_def = new CInfoDef((struct Info_Entra_Campos *)p0,NULL);
		m_borradef = 1;
		m_nombre = m_def->m_info->NomFic;
		MemoriaItems();
		PorDefecto();
	}
	free(buffer);
	if (m_borradef)
		return TRUE;
	return FALSE;
}

CFiltroDef::~CFiltroDef()
{
	BorraParientes();
	BorraItems();
	if (m_borradef)
		delete m_def;
}

BOOL CFiltroDef::PorDefecto(BOOL incluir)
{    
    int j;
    for (int i = 0;i < m_nitem;i++)
    {
        for (j = 0;j < m_def->m_n_campos_col;j++)
        {
            if (i == m_def->m_colcampos[j])
            {
                break;
            }
        }
        if (incluir && j < m_def->m_n_campos_col)
        {
            m_item[i]->m_checked = TRUE;
        }
        else
        {
            m_item[i]->m_checked = FALSE;        
		}
        m_item[i]->m_idx = -1;
		m_item[i]->m_lidx = -1;
        m_item[i]->m_TipoValor = 4;
        m_item[i]->m_Mayus = FALSE;
        m_item[i]->m_Inver = FALSE;
        m_item[i]->m_LimiteI = "";
        m_item[i]->m_LimiteS = "";
        m_item[i]->m_Valor = "";
		m_item[i]->m_Orden = FALSE;
		m_item[i]->m_OrdenInverso = FALSE;
		m_item[i]->m_Agrupado = FALSE;
		m_item[i]->m_CondAgrupada = FALSE;
		m_item[i]->m_Preguntar = FALSE;
    }
    return TRUE;
}

void CFiltroDef::MemoriaItems()
{
    m_nitem = m_def->m_info->n_campos;
    m_item  = (CFiltroItem **)malloc(sizeof(void *)*m_nitem);
    m_idx =   (int *)malloc(sizeof(int) * m_nitem);
    char *px;
	int i;
    for (i = 0;i < m_nitem;i++)
    {
        m_item[i] = new CFiltroItem(this);
        m_item[i]->m_name = m_def->c_nombre(i,TRUE);
		m_item[i]->m_col = i;
		m_item[i]->m_campo = m_def->Campo(i,TRUE)->Id;
        m_idx[i] = -1;
        px = (char *)(LPCSTR)m_item[i]->m_name;
        while (*px)
        {
            oem_a_ansi((unsigned char *)px);
            px++;
        }
    }
	if (m_camposrel > 0)
	{
		for (i = 0;i < m_camposrel;i++)
		{
			m_campospadre[i] = m_padre->m_def->m_idcampos[m_campospadre_o[i]];
			m_camposhijo[i] = m_def->m_idcampos[m_camposhijo_o[i]];
		}
	}
}

void CFiltroDef::BorraItems()
{
    if (m_item)
    {
		int i;
        for(i = 0;i < m_nitem;i++)
        {
            delete m_item[i];
        }
        free((void *)m_item);
    }
	m_item = NULL;
    m_nitem = 0;
    if (m_idx)
    {
        free((void *)m_idx);
    }
	m_idx = NULL;
}

void CFiltroDef::BorraParientes()
{
	if (m_parientes)
	{
		int i;
		for (i = 0;i < m_nparientes;i++)
		{
			if (m_parientes[i])
			{
				delete m_parientes[i];
			}
		}
		free(m_parientes);
		m_parientes = 0;
		m_nparientes = 0;
	}
}

void CFiltroDef::Relacion(CFiltroDef *padre,int camposrel,int *campospadre,int *camposhijo)
{
	m_padre = padre;
	m_camposrel = camposrel;

	if (!m_padre)
		m_camposrel = 0;
	if (m_camposrel > (sizeof(m_campospadre)/sizeof(int)))
	{
		m_camposrel = sizeof(m_campospadre)/sizeof(int);
	}	
	int i;
	for (i = 0; i < m_camposrel;i++)
	{		
		m_campospadre[i] = -1;
		m_camposhijo[i] = -1;
		m_campospadre_o[i] = campospadre[i];
		m_camposhijo_o[i] = camposhijo[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgGridFiltro dialog


CDlgGridFiltro::CDlgGridFiltro(/*CWnd* pParent /*=NULL*/ CTablaView *pTabla)
	: CExtNCW < CBASEDLG >(CDlgGridFiltro::IDD, /*pParent*/NULL)
{   

	m_sel = -1;
	m_nitems = 0;	
	m_filtrousuario = FALSE;
    CString clave = "GridFiltrosBorrador ";
    clave += pTabla->m_def->m_info->PathDef;

	//{{AFX_DATA_INIT(CDlgGridFiltro)
	m_soloseleccionados = FALSE;
	m_solofiltro = FALSE;
	m_alfabetico = FALSE;
	m_limitei = _T("");
	m_limites = _T("");
	m_valor = _T("");
	m_mayus = FALSE;
	m_inversa = FALSE;
	m_predeterminado = FALSE;
	m_nomcampo = _T("");
	m_registrolocal = (BOOL)theApp.GetProfileInt(clave,"_USER_O_LOCAL_",(int)FALSE);
	m_agrupado = FALSE;
	m_condagrupada = FALSE;
	m_preguntar = FALSE;
	m_presenta = TRUE;
	m_opera = -1;
	//}}AFX_DATA_INIT

    m_pTabla = pTabla;       
	m_principal = new CFiltroDef(m_pTabla->m_def->m_info->PathDef,0,m_pTabla->m_def);
	m_seleccion = m_principal;
    m_modificado = FALSE;

	CString s;
	int j;
	for (int i = 0;i < m_principal->m_def->m_n_campos_col;i++)
	{
		j = m_principal->m_def->m_colcampos[i];
		s.Format("%s[%d]",m_principal->m_item[j]->m_padre->m_nombre,j);
		m_ordenid[i] = s;
	}
	for (;i < MAXFLTITEMS;i++)
	{
		m_ordenid[i] = "";
	}

	m_npredeterminado = theApp.GetProfileString(clave,"_Predeterminado_","");
	m_bpredeterminado = (BOOL)theApp.GetProfileInt(clave,"_Predeterminado_Local_",(int)m_registrolocal);


	DLGCONS;
}


void CDlgGridFiltro::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON1, m_boton1);
	DDX_Control(pDX, IDC_BUTTON2, m_boton2);
	DDX_Control(pDX, IDC_BUTTON3, m_boton3);
    DDX_Control(pDX, IDC_NUEVARELACION, m_boton4);
	DDX_Control(pDX, IDC_BUTTONGUARDAR, m_boton5);
	DDX_Control(pDX, IDC_BUTTONBORRAR, m_boton6);
	DDX_Control(pDX, IDC_FORMATO, m_boton7);
	DDX_Control(pDX, IDC_SUBIR, m_boton8);
	DDX_Control(pDX, IDC_BAJAR, m_boton9);
	DDX_Control(pDX, IDOK, m_boton10);
	DDX_Control(pDX, IDCANCEL, m_boton11);

	DDX_Control(pDX, IDC_ORDENCLAVE, m_combo1);
	DDX_Control(pDX, IDC_COMBOCABECERA, m_combo2);
	DDX_Control(pDX, IDC_COMBOOPERA, m_combo3);


	DDX_Control(pDX, IDC_FNOMCAMPO, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);

	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	DDX_Control(pDX, IDC_STATIC5, m_label5);
	DDX_Control(pDX, IDC_STATIC6, m_label6);
	DDX_Control(pDX, IDC_STATIC7, m_label7);
	DDX_Control(pDX, IDC_STATIC8, m_label8);
	DDX_Control(pDX, IDC_STATIC9, m_label9);
	DDX_Control(pDX, IDC_STATIC10a, m_label10);
	DDX_Control(pDX, IDC_STATIC10b, m_label11);

	DDX_Control(pDX, IDC_CHECK8, m_check1);
	DDX_Control(pDX, IDC_CHECK7, m_check2);
	DDX_Control(pDX, IDC_CHECK5, m_check3);
	DDX_Control(pDX, IDC_CHECK2, m_check4);	
	DDX_Control(pDX, IDC_CHECK6, m_check5);
	DDX_Control(pDX, IDC_CHECKINCLUIDO, m_check6);
	DDX_Control(pDX, IDC_AGRU, m_check7);
	DDX_Control(pDX, IDC_CONDAGRU, m_check8);
	DDX_Control(pDX, IDC_PREGUNTAR, m_check9);
	DDX_Control(pDX, IDC_CHECK3, m_check10);
	DDX_Control(pDX, IDC_CHECK4, m_check11);	

	DDX_Control(pDX, IDC_SINORDEN, m_radio1);
	DDX_Control(pDX, IDC_ASCENDENTE, m_radio2);
	DDX_Control(pDX, IDC_DESCENDENTE, m_radio3);	
	
	DDX_Control(pDX, IDC_RADIOEXACTO, m_radio4);
	DDX_Control(pDX, IDC_RADIOPARCIAL, m_radio5);
	DDX_Control(pDX, IDC_RADIOCONTENIDO, m_radio6);
	

	//{{AFX_DATA_MAP(CDlgGridFiltro)
	DDX_Control(pDX, IDC_LIST3, m_relaciones);
	DDX_Control(pDX, IDC_LIST4, m_listado);
	DDX_Control(pDX, IDC_TREE1, m_ficheros);
	DDX_Control(pDX, IDC_COMBO2, m_combo);
	DDX_Control(pDX, IDC_LIST1, m_Lista);
	DDX_Check(pDX, IDC_CHECK2, m_soloseleccionados);
	DDX_Check(pDX, IDC_CHECK5, m_solofiltro);
	DDX_Check(pDX, IDC_CHECK6, m_alfabetico);
	DDX_Text(pDX, IDC_EDIT2, m_limitei);
	DDX_Text(pDX, IDC_EDIT3, m_limites);
	DDX_Text(pDX, IDC_EDIT4, m_valor);
	DDX_Check(pDX, IDC_CHECK3, m_mayus);
	DDX_Check(pDX, IDC_CHECK4, m_inversa);
	DDX_Check(pDX, IDC_CHECK7, m_predeterminado);
	DDX_Text(pDX, IDC_FNOMCAMPO, m_nomcampo);
	DDX_Check(pDX, IDC_CHECK8, m_registrolocal);
	DDX_Check(pDX, IDC_AGRU, m_agrupado);
	DDX_Check(pDX, IDC_CONDAGRU, m_condagrupada);
	DDX_Check(pDX, IDC_PREGUNTAR, m_preguntar);
	DDX_Check(pDX, IDC_CHECKINCLUIDO, m_presenta);
	DDX_CBIndex(pDX, IDC_COMBOOPERA, m_opera);
	//}}AFX_DATA_MAP
	
	/*
	if (pDX->m_bSaveAndValidate)
	{
		m_predeterminado = m_check2.GetCheck();
	    m_registrolocal  = m_check1.GetCheck();
	}
	else
	{
		if (m_predeterminado)	
			m_check2.SetCheck(1);
		else
			m_check2.SetCheck(0);	
		if (m_registrolocal)	
			m_check1.SetCheck(1);
		else
			m_check1.SetCheck(0);	
	}
	*/

}


BOOL CDlgGridFiltro::EsSimple()
{
	int i;

	return FALSE;


	if (!m_nitems)
		return TRUE;

	CString s;

	s = m_principal->m_nomdef;
	for (i = 0;i < m_nitems;i++)
	{
		if (s.Compare(m_items[i]->m_padre->m_nomdef))
		{
			return FALSE;
		}
		if (m_items[i]->m_opera > 0)
			return FALSE;
		if (m_items[i]->m_Agrupado)
			return FALSE;
	}
	return TRUE;
}

CDlgGridFiltro::~CDlgGridFiltro()
{
	/*
   if (!m_filtrousuario)
   {
		CString clave = "GridFiltrosBorrador ";
		//clave += m_pTabla->m_def->m_info->PathDef;
		theApp.WriteProfileInt(clave,"_USER_O_LOCAL_",(int)m_registrolocal);
   }
   */
   int i;
   for (i = 0;i < m_nitems;i++)
   {
	   if (m_items[i])
	   {
		  delete m_items[i];
	   }
   }
   m_nitems = 0;
   if (m_principal)
	   delete m_principal;
}

BEGIN_MESSAGE_MAP(CDlgGridFiltro, CBASEDLG)
    ON_CLBN_CHKCHANGE(IDC_LIST1, OnCheckLista)	
	//{{AFX_MSG_MAP(CDlgGridFiltro)
	ON_BN_CLICKED(IDC_BUTTONGUARDAR, OnButtonguardar)
	ON_BN_CLICKED(IDC_CHECK2, OnSeleccionados)
	ON_BN_CLICKED(IDC_CHECK5, OnSoloFiltro)
	ON_BN_CLICKED(IDC_CHECK6, OnOrdenAlfabetico)
	ON_BN_CLICKED(IDC_CHECK8, OnRegistroLocal)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeDesde)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeHasta)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeValor)
	ON_CBN_SELENDOK(IDC_COMBO2, OnSelendokCombo)
	ON_BN_CLICKED(IDC_BUTTON2, OnNinguno)
	ON_BN_CLICKED(IDC_BUTTON1, OnTodos)
	ON_BN_CLICKED(IDC_ASCENDENTE, OnAscendente)
	ON_BN_CLICKED(IDC_SINORDEN, OnSinorden)
	ON_BN_CLICKED(IDC_DESCENDENTE, OnDescendente)
	ON_BN_CLICKED(IDC_RADIOEXACTO, OnRadioexacto)
	ON_BN_CLICKED(IDC_RADIOPARCIAL, OnRadioparcial)
	ON_BN_CLICKED(IDC_RADIOCONTENIDO, OnRadiocontenido)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_BUTTONBORRAR, OnButtonborrar)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)	
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)	
	ON_BN_CLICKED(IDC_NUEVARELACION, OnNuevarelacion)
	ON_BN_CLICKED(IDC_FORMATO, OnFormato)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST4, OnBeginlabeleditList4)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST4, OnEndlabeleditList4)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, OnClickList4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, OnItemchangedList4)
	ON_CBN_EDITCHANGE(IDC_COMBOOPERA, OnEditchangeComboopera)
	ON_BN_CLICKED(IDC_CHECKINCLUIDO, OnCheckincluido)
	ON_CBN_SELCHANGE(IDC_COMBOOPERA, OnSelchangeComboopera)
	ON_BN_CLICKED(IDC_AGRU, OnAgrupador)
	ON_BN_CLICKED(IDC_SUBIR, OnArriba)
	ON_BN_CLICKED(IDC_BAJAR, OnAbajo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGridFiltro message handlers
/////////////////////////////////////////////////////////////////////////////
// CFiltroCListBox

CFiltroCListBox::CFiltroCListBox()
{
}

CFiltroCListBox::~CFiltroCListBox()
{
}


BEGIN_MESSAGE_MAP(CFiltroCListBox, CCheckListBox)
	//{{AFX_MSG_MAP(CFiltroCListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiltroCListBox message handlers


void CFiltroCListBox::PreSubclassWindow() 
{
    //& (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)
	CCheckListBox::PreSubclassWindow();
}

BOOL CDlgGridFiltro::OnInitDialog()
{
	CBASEDLG::OnInitDialog();

	m_check1.SetCheck(0);
	m_check2.SetCheck(0);
	::SendMessage(::GetDlgItem(m_hWnd,IDC_CHECK7), BM_SETCHECK, (WPARAM)0, 0L);
	::SendMessage(::GetDlgItem(m_hWnd,IDC_CHECK8), BM_SETCHECK, (WPARAM)0, 0L);

	m_check3.SetCheck(0);
	m_check4.SetCheck(0);
	m_check5.SetCheck(0);
	m_check6.SetCheck(0);
	m_check7.SetCheck(0);
	m_check8.SetCheck(0);
	m_check9.SetCheck(0);
	m_check10.SetCheck(0);
	m_check11.SetCheck(0);


	m_relaciones.SetHorizontalExtent(300);

	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	
	ListView_SetExtendedListViewStyleEx(m_listado.m_hWnd, styles, styles );

	m_listado.InsertColumn( 0,"Campo",LVCFMT_LEFT,50,0);
	m_listado.InsertColumn( 1,"L",LVCFMT_LEFT,-1,1);		
	m_listado.InsertColumn( 2,"Opera",LVCFMT_LEFT,-1,2);
	m_listado.InsertColumn( 3,"Ord.",LVCFMT_LEFT,-1,3); 
	m_listado.InsertColumn( 4,"SubCab.",LVCFMT_LEFT,-1,4); 
	
    if (!m_pTabla) return TRUE;

	{
		TV_INSERTSTRUCT Nuevo;
		char tmp[512];

		strcpy(tmp,m_principal->m_nombre);
		Nuevo.hParent = NULL;
		Nuevo.hInsertAfter = TVI_FIRST;					
		Nuevo.item.pszText = tmp;
		Nuevo.item.cchTextMax = strlen(tmp);
		Nuevo.item.iImage = 0;
		Nuevo.item.iSelectedImage = 0;			
		Nuevo.item.lParam = (LPARAM)m_principal;
		Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

		HTREEITEM root = m_ficheros.InsertItem(&Nuevo);
		m_principal->m_hitem = root;
		m_ficheros.InsertItem(TVIF_IMAGE | TVIF_TEXT,
		  "",
		  0,
		  0,
		  0,
		  0,
		  0,
		  root,
		  TVI_FIRST);
	    m_ficheros.SelectItem(root);
	}

	
	if (m_filtrousuario)
	{
		
		GetDlgItem(IDC_CHECK8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);		
		
	}

	/*
	m_radio4.ShowWindow(SW_SHOW);
	m_radio5.ShowWindow(SW_SHOW);
	m_radio6.ShowWindow(SW_SHOW);
	*/
    
    CString s,s1;

    GetWindowText(s1);
    m_pTabla->m_tw->GetWindowText(s);
    
    s1 += " ";
    s1 += s;

    SetWindowText(s1);		

	InicializaItems();

	return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGridFiltro::OnButtonguardar()
{
	Guarda();
}

void CDlgGridFiltro::OrdenId(int orden,CString id)
{
	int i;

	if (orden < 0 || orden >= MAXFLTITEMS)
		return;

	for (i = 0;i < MAXFLTITEMS;i++)
	{
		if (m_ordenid[i].IsEmpty())
			break;
		if (!id.Compare(m_ordenid[i]))
			break;
	}
	if (i >= MAXFLTITEMS)
		i = orden;

	if (i != orden)
	{
		m_ordenid[i] = m_ordenid[orden];
	}
	m_ordenid[orden] = id;
}

void CDlgGridFiltro::Guarda(BOOL Borra)
{
   m_modificado = FALSE;

   if (!Borra)
   {	   
	   OnSelchangeLista();
	   RecogeLista();
	   if (!RellenaListado())
	   {
			AfxMessageBox("Debe seleccionar al menos una columna",MB_OK|MB_ICONHAND);
			return;
	   }
	   OnSelchangeLista();
	   UpdateData(FALSE);

	   m_combo.GetWindowText(m_nombre);
	   if (m_nombre.IsEmpty())
	   {
		   m_nombre = "Nuevo";
		   m_combo.SelectString(-1,m_nombre);
	   }
	   if (m_combo.FindStringExact(-1,m_nombre) < 0)
	   {
		   m_combo.AddString(m_nombre);
	   }
   }

   CString clave = "GridFiltrosBorrador ";
   clave += m_pTabla->m_def->m_info->PathDef;
   CString dd,s = "",ss; 

   int i;
   if (!Borra)
   {
	   if (EsSimple())
	   {
		   for (i = 0;i < m_principal->m_nitem;i++)
		   {            
				if (m_principal->m_item[i]->m_checked)
				{                        
					/*m_principal->m_item[i]->m_checked*/
					dd.Format("%03d%c%1d%s%c%s%c%s%c",i,(m_principal->m_item[i]->m_salida ? 'S' : 'N'),m_principal->m_item[i]->m_TipoValor,
							   (LPCSTR)m_principal->m_item[i]->m_LimiteI,2,(LPCSTR)m_principal->m_item[i]->m_LimiteS,2,(LPCSTR)m_principal->m_item[i]->m_Valor,1);
					if (m_principal->m_item[i]->m_Inver)
					{
						s += (char)3;
					}
					if (m_principal->m_item[i]->m_Mayus)
					{
						s += (char)4;
					}
					if (m_principal->m_item[i]->m_Orden)
					{
						s += (char)5;
					}
					if (m_principal->m_item[i]->m_OrdenInverso)
					{
						s += (char)6;
					}
					if (m_principal->m_item[i]->m_Agrupado)
					{
						s += (char)7;
					}
					if (m_principal->m_item[i]->m_CondAgrupada)
					{
						s += (char)8;
					}
					if (m_principal->m_item[i]->m_Preguntar)
					{
						s += (char)9;
					}
					if (m_principal->m_item[i]->m_opera > 0)
					{
						s += (char)(13+m_principal->m_item[i]->m_opera);
					}
					s += dd;
				}
		   }
	   }
	   else
	   {
			int i,nfiltros,j,k;
			int nfiles;			
			int *idxfiles;
			CFiltroDef *defs[100],*def;
			int pariente[100];			
			idxfiles = new int[m_nitems];
			nfiles = 1;			
			defs[0] = m_principal;
			pariente[0] = 0;
			for (nfiltros = 0;nfiltros < m_nitems;nfiltros++)
			{
				for (i = 0;i < nfiles;i++)
				{
					if (!strcmp(defs[i]->m_nomdef,m_items[nfiltros]->m_padre->m_nomdef))
					{						
						break;
					}
				}
				if (i >= nfiles)
				{
					defs[nfiles] = m_items[nfiltros]->m_padre;					
					nfiles++;					
				}
			}
			for (i = 1;i < nfiles;i++)
			{
				pariente[i] = -1;
				if (defs[i]->m_padre)
				{
					for (j = 0;j < nfiles;j++)
					{
						if (defs[i]->m_padre == defs[j])
						{
							pariente[i] = j;
							break;
						}
					}
				}
				if (pariente[i] == -1)
				{
					ASSERT(FALSE);
					pariente[i] = 0;
				}
			}

			/* ordenamos por parentesco */			
			for (i = 1;i < nfiles;i++)
			{
				for (j = i+1;j < nfiles;j++)
				{
					if (pariente[i] > pariente[j])
					{
						k = pariente[i];
						pariente[i] = pariente[j];
						pariente[j] = k;
						def = defs[i];
						defs[i] = defs[j];
						defs[j] = def;
					}
				}
			}
			for (i = 0;i < nfiltros;i++)
			{
				for (j = 0;j < nfiles;j++)
				{
					if (!strcmp(defs[j]->m_nomdef,m_items[i]->m_padre->m_nomdef))
					{						
						idxfiles[i] = j;
						break;
					}
				}
				if (j >= nfiles)
				{
					ASSERT(FALSE);
					idxfiles[i] = 0;
				}
			}

		    s = (char)26;
		    dd.Format("%03d",nfiles);
			s += dd;
			for (i = 0;i < nfiles;i++)
			{
				s += defs[i]->m_nomdef;
				s += (char)2;
				s += defs[i]->m_nombre;
				s += (char)2;
				dd.Format("%03d",pariente[i]);
				s += dd;
				dd.Format("%03d",defs[i]->m_camposrel);
				s += dd;
				for (j = 0;j < defs[i]->m_camposrel;j++)
				{
					dd.Format("%03d",defs[i]->m_campospadre_o[j]);
					s += dd;
					dd.Format("%03d",defs[i]->m_camposhijo_o[j]);
					s += dd;
				}
			}
			s += (char)24;

			for (i = 0;i < m_nitems;i++)
			{
				if (m_ordenid[i].IsEmpty())
					break;
			}
			dd.Format("%03d",i);
			s += dd;

			for (j  = 0;j < m_nitems;j++)
			{			
				CString s1;				
				if (m_ordenid[j].IsEmpty())
				{
					break;
				}
				else for (i = 0;i < m_nitems;i++)
				{
					s1.Format("%s[%d]",m_items[i]->m_padre->m_nombre,m_items[i]->m_lidx);
					if (!s1.Compare(m_ordenid[j]))
						break;
				}
				if (i < m_nitems)
				{
					s += s1;
					s += (char)2;
				}
			}

			s += (char)25;
			for (i = 0;i < nfiltros;i++)
			{
				dd.Format("%03d",idxfiles[i]);
				s += dd;

				/*m_items[i]->m_checked*/
				dd.Format("%03d%c%1d%s%c%s%c%s%c",m_items[i]->m_campo,(m_items[i]->m_salida ? 'S' : 'N'),m_items[i]->m_TipoValor,
						   (LPCSTR)m_items[i]->m_LimiteI,2,(LPCSTR)m_items[i]->m_LimiteS,2,(LPCSTR)m_items[i]->m_Valor,1);
				if (m_items[i]->m_Inver)
				{
					s += (char)3;
				}
				if (m_items[i]->m_Mayus)
				{
					s += (char)4;
				}
				if (m_items[i]->m_Orden)
				{
					s += (char)5;
				}
				if (m_items[i]->m_OrdenInverso)
				{
					s += (char)6;
				}
				if (m_items[i]->m_Agrupado)
				{
					s += (char)7;
				}
				if (m_items[i]->m_CondAgrupada)
				{
					s += (char)8;
				}
				if (m_items[i]->m_Preguntar)
				{
					s += (char)9;
				}
				if (m_items[i]->m_opera > 0)
				{
					s += (char)(13+m_items[i]->m_opera);
				}
				s += dd;
			}

		   delete idxfiles;
	   }
   }
   
   if (!m_filtrousuario && m_registrolocal)
   {
	   theApp.WriteProfileBinary(clave,m_nombre,(LPBYTE)(LPCSTR)s,s.GetLength());	   
   }
   else
   {
	   GrabaFiltros(m_nombre,s);
   }

   ss = "";
   for (i = 0;i < m_combo.GetCount();i++)
   {
       if (!ss.IsEmpty())
           ss += (char)1;
       m_combo.GetLBText(i,dd);
       ss += dd;
   }
   ss += (char)0;
   if (!m_filtrousuario)
   {
	   if (m_registrolocal)
	   {
		   theApp.WriteProfileBinary(clave,"_INDICE_",(LPBYTE)(LPCSTR)ss,ss.GetLength());
	   }
	   else
	   {
		   GrabaFiltros("_INDICE_",ss);
	   }
   }
}

void CDlgGridFiltro::OnSeleccionados()
{    
    m_soloseleccionados = !m_soloseleccionados;
    UpdateData(TRUE);
    RecogeLista();
    /*OnSelchangeLista();*/
    UpdateData(FALSE);
    RellenaLista();
}

void CDlgGridFiltro::OnSoloFiltro()
{
    m_solofiltro = !m_solofiltro;
    UpdateData(TRUE);
    RecogeLista();
    /*OnSelchangeLista();*/
    UpdateData(FALSE);
    RellenaLista();
}

void CDlgGridFiltro::OnOrdenAlfabetico()
{
    m_alfabetico = !m_alfabetico;
    UpdateData(TRUE);
    RecogeLista();
    /*OnSelchangeLista();*/
    UpdateData(FALSE);
    RellenaLista();
}


void CDlgGridFiltro::OnCheckLista()
{
	int n = m_Lista.GetSelCount();
	int *idxs;
	int idx;

	OnSelchangeLista();

	idxs = new int[n];

	n = m_Lista.GetSelItems( n, idxs );
	if (n > 0)
	{
		int i;
		for (i = 0;i < n;i++)
		{			
			idx = m_seleccion->m_idx[idxs[i]];
			if (idx >= 0)
			{
				m_seleccion->m_item[idx]->m_checked = (BOOL)m_Lista.GetCheck(idxs[i]);
			}
		}
		RellenaListado();
	}    

	delete idxs;
}


void CDlgGridFiltro::OnSelchangeLista()
{
    /*
    int  m_idx;
    BOOL m_checked;
    CString m_name;
    CString m_LimiteI;
    CString m_LimiteS;
    CString m_Valor;
    int     m_TipoValor;
    */
	int sel = -1;
	POSITION pos = m_listado.GetFirstSelectedItemPosition();
    if (pos)
    {
       sel = m_listado.GetNextSelectedItem(pos);
    }	    

	if (sel < 0)
		return;
    
    int i;
    int ss;
    CButton *pButton;

    UpdateData(TRUE);

    if (m_sel > -1 && m_idxs[m_sel] > -1)
    {
        i = m_idxs[m_sel];
        m_limitei.TrimRight();
        m_limites.TrimRight();
        m_valor.TrimRight();
        m_items[i]->m_LimiteI = m_limitei;
        m_items[i]->m_LimiteS = m_limites;
        m_items[i]->m_Valor   = m_valor;
        m_items[i]->m_Mayus   = m_mayus;
        m_items[i]->m_Inver   = m_inversa;
		m_items[i]->m_Agrupado= m_agrupado;
		m_items[i]->m_CondAgrupada = m_condagrupada;
		m_items[i]->m_Preguntar = m_preguntar;

		m_items[i]->m_OrdenInverso = FALSE;

		m_items[i]->m_salida = m_presenta;		
		m_items[i]->m_opera = m_opera;


		pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
		if (pButton->GetCheck())
		{
			m_items[i]->m_Orden = TRUE;
			m_items[i]->m_OrdenInverso = FALSE;
		}  
		pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
		if (pButton->GetCheck())
		{
			m_items[i]->m_Orden = TRUE;
			m_items[i]->m_OrdenInverso = TRUE;
		}
		pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
		if (pButton->GetCheck())
		{
			m_items[i]->m_Orden = FALSE;
			m_items[i]->m_OrdenInverso = FALSE;
		}		

        ss = 0;
        if (m_limitei.IsEmpty() && m_limites.IsEmpty())
        {
            pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            if (pButton->GetCheck())
            {
                ss = 1;
            }
            else 
            {
                pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
                if (pButton->GetCheck())
                {
                    ss = 2;
                }
                else
                {
                    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
                    if (pButton->GetCheck())
                    {
                        ss = 3;
                    }
					else
					{
						ss = 4;
					}
                }
            }
        }
        else
        {
            m_items[i]->m_Valor = "";
        }
        m_items[i]->m_TipoValor = ss;

		int otra = m_items[i]->m_lidx;
		
		if (otra > -1)
		{
			*(m_items[i]->m_padre->m_item[otra]) = *(m_items[i]);
		}

		otra = m_items[i]->m_idx;

		if (otra > -1)
		{
			if (m_items[i]->m_salida)
			{	
				m_listado.SetItemText( otra, 1, "*" );
			}
			else
			{
				m_listado.SetItemText( otra, 1, "" );
			}


			switch(m_items[i]->m_opera)
			{
			case 1:
				m_listado.SetItemText( otra, 2, "Suma" );
				break;
			case 2:
				m_listado.SetItemText( otra, 2, "Máximo" );
				break;
			case 3:
				m_listado.SetItemText( otra, 2, "Mínimo" );
				break;
			case 4:
				m_listado.SetItemText( otra, 2, "Media" );
				break;
			case 5:
				m_listado.SetItemText( otra, 2, "Porcentaje Total" );
				break;
			case 6:
				m_listado.SetItemText( otra, 2, "Porcentaje Subtotal" );
				break;
			default:
				m_listado.SetItemText( otra, 2, "" );
				break;
			}

			if (m_items[i]->m_Orden)
			{	
				m_listado.SetItemText( otra, 3, "*" );
			}
			else
			{
				m_listado.SetItemText( otra, 3, "" );
			}

		}
	}

    m_sel = sel;
    
    if (m_sel > -1 && m_idxs[m_sel] > -1)
    {
        i = m_idxs[m_sel];

        m_limitei = m_items[i]->m_LimiteI;
        m_limites = m_items[i]->m_LimiteS;
        m_valor   = m_items[i]->m_Valor;
        m_mayus   = m_items[i]->m_Mayus;
        m_inversa = m_items[i]->m_Inver;
		m_agrupado = m_items[i]->m_Agrupado;
		m_condagrupada = m_items[i]->m_CondAgrupada;
		m_preguntar = m_items[i]->m_Preguntar;

        m_limitei.TrimRight();
        m_limites.TrimRight();
        m_valor.TrimRight();

		if (m_items[i]->m_Orden)
		{
			if (m_items[i]->m_OrdenInverso)
			{
				pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
				pButton->SetCheck(0);
				pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
				pButton->SetCheck(0);
				pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
				pButton->SetCheck(1);
			}
			else
			{
				pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
				pButton->SetCheck(0);
				pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
				pButton->SetCheck(1);
				pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
				pButton->SetCheck(0);
			}
			pButton = (CButton *)GetDlgItem(IDC_AGRU);
			pButton->EnableWindow(TRUE);
		}
		else
		{
			pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
			pButton->SetCheck(1);
			pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
			pButton->SetCheck(0);
			pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
			pButton->SetCheck(0);
			pButton = (CButton *)GetDlgItem(IDC_AGRU);
			pButton->EnableWindow(FALSE);
		}

        ss = m_items[i]->m_TipoValor;
		
        switch(ss)
        {
        case 1:
            pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            pButton->SetCheck(1);
            pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
            pButton->SetCheck(0);
			pButton = (CButton *)GetDlgItem(IDC_CHECK3);
			pButton->EnableWindow(FALSE);
			pButton->SetCheck(0);
            break;
        case 2:
            pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
            pButton->SetCheck(1);
            pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
            pButton->SetCheck(0);
			pButton = (CButton *)GetDlgItem(IDC_CHECK3);
			pButton->EnableWindow(FALSE);
			pButton->SetCheck(0);
            break;
        case 3:
            pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
            pButton->SetCheck(1);
			pButton = (CButton *)GetDlgItem(IDC_CHECK3);
			pButton->EnableWindow(TRUE);
            break;
        default:
            pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
            pButton->SetCheck(0);
            pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
            pButton->SetCheck(0);
			pButton = (CButton *)GetDlgItem(IDC_CHECK3);
			pButton->EnableWindow(FALSE);
			pButton->SetCheck(0);
            break;
        }
		

        m_nomcampo = m_items[i]->m_name;

		m_presenta = m_items[i]->m_salida;		
		m_opera = m_items[i]->m_opera;

		int otra = m_items[i]->m_lidx;		
		if (otra > -1)
		{
            int tipo = m_items[i]->m_padre->m_def->Campo(otra,TRUE)->c_tipo;
			if (tipo == 0 || tipo == 5)
				GetDlgItem(IDC_COMBOOPERA)->EnableWindow(FALSE);	
			else
				GetDlgItem(IDC_COMBOOPERA)->EnableWindow(TRUE);							
		}
		else
			GetDlgItem(IDC_COMBOOPERA)->EnableWindow(TRUE);	

        UpdateData(FALSE);
    }    


	/*
	BOOL orden = FALSE;
	for (i = 0;i < m_nitems;i++)
	{
		if (m_items[i]->m_Orden)
		{
			orden = TRUE;
			break;
		}
	}	
	if (orden)
		GetDlgItem(IDC_COMBOOPERA)->EnableWindow(TRUE);	
	else
		GetDlgItem(IDC_COMBOOPERA)->EnableWindow(FALSE);     
	*/
}

void CDlgGridFiltro::RellenaLista()
{
    int idx;        
    m_Lista.ResetContent( );

	if (!m_seleccion->RellenaDef(m_pTabla))
		return;

    int *lalista = new int[m_seleccion->m_nitem];
    int i;

    if (m_alfabetico)
    {
        int nj = 0,j,k;
        for (i = 0;i < m_seleccion->m_nitem;i++)
        {
            for (j = 0;j < nj;j++)
            {                
                if (m_seleccion->m_item[ lalista[j] ]->m_name.CompareNoCase(m_seleccion->m_item[i]->m_name) > 0)
                {                    
                    for (k = nj;k > j;k--)
                    {
                        lalista[k] = lalista[k-1];
                    }
                    lalista[j] = i;
                    nj++;
                    break;
                }
            }    
            if (j >= nj)
            {
                lalista[nj] = i;
                nj++;
            }
        }
    }
    else
    {
        for (i = 0;i < m_seleccion->m_nitem;i++)
        {
            lalista[i] = i;
        }
    }
    
    for (int _i = 0;_i < m_seleccion->m_nitem;_i++)
    {
        i = lalista[_i];
        if (m_solofiltro)
        {
            if (!m_seleccion->m_item[i]->TieneFiltro() && !m_seleccion->m_item[i]->m_Orden)
            {
                m_seleccion->m_item[i]->m_idx = -1;
                continue;
            }
        }
        if (m_soloseleccionados)
        {
            if (!m_seleccion->m_item[i]->m_checked)
            {
                m_seleccion->m_item[i]->m_idx = -1;
                continue;
            }
        }
        idx = m_Lista.AddString(m_seleccion->m_item[i]->m_name);
        m_seleccion->m_item[i]->m_idx = idx;
        m_seleccion->m_idx[idx] = i;
        if (m_seleccion->m_item[i]->m_checked)
            m_Lista.SetCheck(idx,1);
        else
            m_Lista.SetCheck(idx,0);
    }
    delete [] lalista;
    m_seleccion->m_sel = -1;
    /*OnSelchangeLista();*/
	PonPredeterminado();
}


int CDlgGridFiltro::RellenaItemsListado(CFiltroDef *pdef)
{
   int checks = 0;
   int idx,i;


   idx = m_nitems - 1;
   for (i = 0;i < pdef->m_nitem;i++)
   {       
	    if (pdef->m_item[i]->m_checked)
		{
			if (m_nitems >= MAXFLTITEMS)
				return checks;

			idx++;
			
			m_items[m_nitems] = new CFiltroItem(*pdef->m_item[i]);
			m_items[m_nitems]->m_idx = idx;
			m_items[m_nitems]->m_checked = TRUE;

			m_idxs[idx] = m_nitems;

			ASSERT(m_items[m_nitems]->m_idx == idx);

			m_items[m_nitems]->m_lidx = i;
			pdef->m_item[i]->m_lidx = m_nitems;
			m_nitems++;
			if (pdef->m_item[i]->m_salida)
			{	
				checks++;
			}
		}
		else
		{
			pdef->m_item[i]->m_lidx = -1;
		}
   }
   for (i = 0; i < pdef->m_nparientes;i++)
   {
	   if (pdef->m_parientes[i])
	   {
		   checks += RellenaItemsListado(pdef->m_parientes[i]);
	   }
   }   

   return checks;
}

BOOL CDlgGridFiltro::RellenaListado(BOOL offline)
{
   int checks = 0;          
   int sel = m_sel;   
   CString s;

   if (sel < 0)
	   sel = 0;
   m_sel = -1;


   if (!offline)
	m_listado.DeleteAllItems( );

   int i;
   for (i = 0;i < m_nitems;i++)
   {
	   if (m_items[i])
	   {
		  delete m_items[i];
	   }
   }
   m_nitems = 0;

   checks += RellenaItemsListado(m_principal);   

   /* rellenar lista  */   

	int idx = -1,xi;

	if (!checks)
	{
		for (i = 0;i< MAXFLTITEMS;i++)
			m_ordenid[i] = "";
		return FALSE;
	}
	else
	{
		for (xi = 0;xi < MAXFLTITEMS;xi++)
		{
			if (m_ordenid[xi].IsEmpty())
				break;
		}
		if (xi > m_nitems)
		{
			for (i = 0;i < xi;i++)
			{			
				for (int j = 0;j < m_nitems;j++)
				{
					s.Format("%s[%d]",m_items[j]->m_padre->m_nombre,m_items[j]->m_lidx);
					if (!s.Compare(m_ordenid[i]))
						break;
				}
				if (j >= m_nitems)
				{
					xi--;
					while(i < xi)
					{
						m_ordenid[i] = m_ordenid[i+1];
						i++;
					}
					m_ordenid[i] = "";
				}
			}
		}
	}

	for (xi = 0;xi < m_nitems;xi++)
	{			
		if (m_ordenid[xi].IsEmpty())
		{
			for (i = 0;i < m_nitems;i++)
			{
				s.Format("%s[%d]",m_items[i]->m_padre->m_nombre,m_items[i]->m_lidx);
				for (int j = 0;j < m_nitems;j++)
				{
					if (m_ordenid[j].IsEmpty())
						break;
					if (!s.Compare(m_ordenid[j]))
						break;
				}
				if (j >= m_nitems || m_ordenid[j].IsEmpty())
					break;
			}
		}
		else 
		{
			for (i = 0;i < m_nitems;i++)
			{
				s.Format("%s[%d]",m_items[i]->m_padre->m_nombre,m_items[i]->m_lidx);
				if (!s.Compare(m_ordenid[xi]))
					break;
			}
			if (i >= m_nitems)
			{
				for (i = 0;i < m_nitems;i++)
				{
					s.Format("%s[%d]",m_items[i]->m_padre->m_nombre,m_items[i]->m_lidx);
					for (int j = 0;j < m_nitems;j++)
					{
						if (m_ordenid[j].IsEmpty())
							break;
						if (!s.Compare(m_ordenid[j]))
							break;
					}
					if (j >= m_nitems || m_ordenid[j].IsEmpty())
						break;
				}
			}
		}

		ASSERT(i < m_nitems);

		s.Format("%s[%d]",m_items[i]->m_padre->m_nombre,m_items[i]->m_lidx);
		m_ordenid[xi] = s;

		s = m_items[i]->m_name;
		if (m_items[i]->m_padre != m_principal)
		{
			s += " (";
			s += m_items[i]->m_padre->m_nombre;
			s += ")";
		}
	    if (!offline)
		   idx = m_listado.InsertItem( idx+1, s );
		else
		   idx++;

		m_items[i]->m_idx = idx;
		m_idxs[idx] = i;

		if (offline)
			continue;

		if (m_items[i]->m_salida)
			m_listado.SetItemText( idx, 1, "*" );
		switch(m_items[i]->m_opera)
		{
		case 1:
			m_listado.SetItemText( idx, 2, "Suma" );
			break;
		case 2:
			m_listado.SetItemText( idx, 2, "Máximo" );
			break;
		case 3:
			m_listado.SetItemText( idx, 2, "Mínimo" );
			break;
		case 4:
			m_listado.SetItemText( idx, 2, "Media" );
			break;
		case 5:
			m_listado.SetItemText( idx, 2, "Porcentaje Total" );
			break;
		case 6:
			m_listado.SetItemText( idx, 2, "Porcentaje Subtotal" );
			break;
		}
		if (m_items[i]->m_Orden)
		{	
			m_listado.SetItemText( idx, 3, "*" );
		}
		else
		{
			m_listado.SetItemText( idx, 3, "" );
		}
	}

   if (!offline)
   {
	   for (i = 0;i < 5;i++)
	   {
		   m_listado.SetColumnWidth( i, LVSCW_AUTOSIZE_USEHEADER);
	   }  

	   if (sel > -1)
	   {
		   m_listado.SetItemState( sel, LVIS_SELECTED, LVIS_SELECTED );	   
		   /*OnSelchangeLista();*/
	   }
   }

   
   return TRUE;
}

BOOL CDlgGridFiltro::RecogeLista()
{
   int checks = 0;
   int i;
   for (i = 0;i < m_seleccion->m_nitem;i++)
   {
       if (m_seleccion->m_item[i]->m_idx != -1)
       {
           m_seleccion->m_item[i]->m_checked = (BOOL)m_Lista.GetCheck( m_seleccion->m_item[i]->m_idx );
		   if (m_seleccion->m_item[i]->m_checked)
			 checks++;
       }
   }
   if (!checks)
	   return FALSE;
   return TRUE;
}

void CDlgGridFiltro::OnRegistroLocal() 
{
    if (m_modificado)
    {
        QuizaGraba();
    }
	UpdateData(TRUE);
	m_registrolocal = !m_registrolocal; /* OJO ESTA A AUTO AHORA POR PROBLEMAS CON EL PROF-UIS, por eso la condicion inversa */
	m_principal->BorraItems();
    m_registrolocal = !m_registrolocal; 
	UpdateData(FALSE);
	m_principal->MemoriaItems();
	InicializaItems(); 

	CString clave = "GridFiltrosBorrador ";
	//clave += m_pTabla->m_def->m_info->PathDef;
	theApp.WriteProfileInt(clave,"_USER_O_LOCAL_",(int)m_registrolocal);
}


void CDlgGridFiltro::OnChangeDesde() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT4,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT4,"");
   }
}

void CDlgGridFiltro::OnChangeHasta() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT4,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT4,"");
   }
}

void CDlgGridFiltro::OnChangeValor() 
{
   CString s;   
   GetDlgItemText(IDC_EDIT2,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT2,"");
   }
   GetDlgItemText(IDC_EDIT3,s);
   if (!s.IsEmpty())
   {
       SetDlgItemText(IDC_EDIT3,"");
   }
   CButton *pButton;
   GetDlgItemText(IDC_EDIT4,s);
   if (s.IsEmpty())
   {	
		pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
		pButton->SetCheck(0);
		pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
		pButton->SetCheck(0);
		pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
		pButton->SetCheck(0);

		pButton = (CButton *)GetDlgItem(IDC_CHECK3);
		pButton->EnableWindow(FALSE);
		pButton->SetCheck(0);
   }
   else
   {
	   BOOL hay = FALSE;
	   pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
	   if (pButton->GetCheck())
		   hay = TRUE;
	   else
	   {	
		   pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
		   if (pButton->GetCheck())
			   hay = TRUE;
		   else
		   {
			   pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
			   if (pButton->GetCheck())
				   hay = TRUE;
		   }
	   }
	   if (!hay)
	   {
		   pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
		   pButton->SetCheck(0);

		   pButton = (CButton *)GetDlgItem(IDC_CHECK3);
		   pButton->EnableWindow(FALSE);
		   pButton->SetCheck(0);
	   }
   }
}

void CDlgGridFiltro::OnSelendokCombo() 
{   
	OnSelchangeLista();

    if (m_modificado)
    {
        QuizaGraba();
    }
    int sel = m_combo.GetCurSel( );
    if (sel > -1)
    {
        m_combo.GetLBText(sel,m_nombre);
        m_combo.SetWindowText(m_nombre);
    }
    else
        m_combo.GetWindowText(m_nombre);
	
    if (!LeeDelRegistro())
    {
        m_principal->PorDefecto(TRUE);
    }    
	
    m_modificado = FALSE;
    RellenaLista();
	RellenaListado();
}

void CDlgGridFiltro::OnNinguno() 
{
    m_Lista.SelItemRange(FALSE, 0, m_Lista.GetCount());
}

void CDlgGridFiltro::OnTodos() 
{	
    m_Lista.SelItemRange(TRUE, 0, m_Lista.GetCount());	
}

BOOL CDlgGridFiltro::LeeDelRegistro()
{
       CString clave = "GridFiltrosBorrador ";
       clave += m_principal->m_def->m_info->PathDef;
       CString dd,dd2,s = "";    


	   if (m_filtrousuario)
	   {
		   if (!LeerFiltros(m_nombre,s))
		   {
			   return FALSE;
		   }
	   }
       else if (m_registrolocal)
	   {
		   unsigned char *px;
		   unsigned int ll;
		   if (!theApp.GetProfileBinary(clave,m_nombre,&px,&ll))
		   {           
			   return FALSE;
		   }
		   if (!ll)
		   {
			   return FALSE;
		   }
		   px[ll-1] = 0;
		   s = px;
		   delete [] px;
	   }
	   else
	   {
		   if (!LeerFiltros(m_nombre,s))
		   {
			   return FALSE;
		   }
	   }       


	   m_principal->PorDefecto();       
       int i,ni,j,ji;
	   
	   int opera = 0;
       BOOL Inver = FALSE,Mayus = FALSE;
	   BOOL Orden = FALSE,OrdenInverso = FALSE;
	   BOOL Agrupado     = FALSE;
	   BOOL CondAgrupada = FALSE;
	   BOOL Preguntar    = FALSE;


	   if (s[0] == (char)26)
	   { /* complex */
		   CFiltroDef *defs[100],*padre,*hijo,*def;
		   int nf;
		   int relpadre[100];
		   int relhijo[100];

		   s = s.Mid(1);
		   nf = atoi(s.Mid(0,3));
		   s = s.Mid(3);
		   s += (char)0;
		   ni = 0;
		   while(ni < nf)
		   {
			  for (i = 0;s[i] && s[i] != 2 && s[i] != 25 && s[i] != 24;i++);
			  if (!i || (s[i] != 2 && s[i] != 25 && s[i] != 24))
				  return FALSE;			  
			  dd = s.Mid(0,i);
			  s = s.Mid(i+1);
			  for (i = 0;s[i] && s[i] != 2 && s[i] != 25 && s[i] != 24;i++);
			  if (!i || (s[i] != 2 && s[i] != 25 && s[i] != 24))
				  return FALSE;			  
			  dd2 = s.Mid(0,i);			  
			  s = s.Mid(i+1);
			  j = atoi(s.Mid(0,3)); /* pariente */
			  s = s.Mid(3);
			  ji = atoi(s.Mid(0,3)); /* relaciones pariente */
			  s = s.Mid(3);
			  if (ji < 0 || (ji == 0 && ni > 0))
			  {		
				  return FALSE;
			  }
			  for (i = 0;i < ji;i++)
			  {
				  relpadre[i] = atoi(s.Mid(0,3));
				  s = s.Mid(3); /* campo padre */
				  relhijo[i] = atoi(s.Mid(0,3));
				  s = s.Mid(3); /* campo hijo */			
			  }
			  if (!ni)
			  {
				  if (dd.Compare(m_principal->m_nomdef))
				  {
					  return FALSE;
				  }
				  if (j != ni)
					  return FALSE;
				  defs[0] = m_principal;
			  }
			  else
			  {
				  if (j >= ni)
					  return FALSE;
				  padre = defs[j];
				  if (!padre->m_nparientes)
				  {					  
					  if (::IsWindow(m_ficheros.m_hWnd))
					  {
						  LRESULT Result = 0;
						  NM_TREEVIEW NMTreeView;
						  memset((void *)&NMTreeView,0,sizeof(NMTreeView));

						  NMTreeView.itemNew.lParam = (LPARAM)padre;
						  NMTreeView.itemNew.hItem  = padre->m_hitem;
						  OnItemexpandingTree1((NMHDR*)&NMTreeView, &Result);
					  }
				  }
				  hijo = NULL;
				  for (i = 0;i < padre->m_nparientes;i++)
				  {
					  if (padre->m_parientes[i] && !dd.Compare(padre->m_parientes[i]->m_nomdef))
					  {
						  if (padre->m_parientes[i]->m_camposrel == ji)
						  {
							  for (j = 0;j < ji;j++)
							  {
								  if (padre->m_parientes[i]->m_campospadre_o[j] != relpadre[j])
									  break;
								  if (padre->m_parientes[i]->m_camposhijo_o[j] != relhijo[j])
									  break;
							  }
							  if (j >= ji)
							  {
								  hijo = padre->m_parientes[i];
								  break;
							  }
						  }
					  }
				  }
				  if (!hijo)
				  {
					  hijo = new CFiltroDef(dd,0/* la memoria que se reasigne mas seguro ... */,NULL,padre,ji,relpadre,relhijo);
					  if (padre->m_parientes)
					  {
						  padre->m_parientes = (CFiltroDef **)realloc((void *)padre->m_parientes,sizeof(CFiltroDef *)*(padre->m_nparientes+1));
					  }
					  else
					  {
						  padre->m_nparientes = 0;
						  padre->m_parientes = (CFiltroDef **)malloc(sizeof(CFiltroDef *));
					  }
					  padre->m_parientes[padre->m_nparientes] = hijo;
					  padre->m_nparientes++;

					  if (::IsWindow(m_ficheros.m_hWnd))
					  {
						  TV_INSERTSTRUCT Nuevo;				
						  Nuevo.hParent = padre->m_hitem;
						  Nuevo.hInsertAfter = TVI_LAST;					

						  Nuevo.item.pszText = (char *)(const char *)dd2;
						  Nuevo.item.cchTextMax = strlen(dd2);
						  Nuevo.item.iImage = 0;
				 		  Nuevo.item.iSelectedImage = 0;			
						  Nuevo.item.lParam = (LPARAM)hijo;
						  Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

						  m_ficheros.InsertItem(TVIF_IMAGE | TVIF_TEXT,
							"",
							0,
							0,
							0,
							0,
							0,
							(hijo->m_hitem = m_ficheros.InsertItem(&Nuevo)),
							TVI_FIRST);
					  }
				  }
				  defs[ni] = hijo;
			  }
			  ni++;
		   }
		   if (ni != nf)
			   return FALSE;

		   if (s[0] == 24)
		   {
			   int nj;
			   s = s.Mid(1);
			   nj = atoi(s.Mid(0,3));
			   s = s.Mid(3);
			   for (j = 0;j < MAXFLTITEMS;j++)
			   {
				   m_ordenid[j] = "";
			   }
			   for (j = 0;j < nj;j++)
			   {
				   for (i = 0;s[i] && s[i] != 2 && s[i] != 25;i++);
				   if (!i || (s[i] != 2 && s[i] != 25))
				   	  return FALSE;
			       dd = s.Mid(0,i);
			       s = s.Mid(i+1);
				   m_ordenid[j] = dd;
			   }
		   }
		   else
		   {
			   for (j = 0;j < MAXFLTITEMS;j++)
			   {
				   m_ordenid[j] = "";
			   }
		   }

		   if (s[0] != 25)
			   return FALSE;
		   s = s.Mid(1);
		   while(!s.IsEmpty())
		   {
			   for (i = 0;s[i] && s[i] != 1;i++);
			   if (i)
			   {
				   dd = s.Mid(0,i);
				   dd += (char)0;

				   ji = atoi(dd.Mid(0,3));
				   if (ji < 0 || ji >= nf)
					   return FALSE;

				   def = defs[ji];
				   
				   def->RellenaDef(m_pTabla);

				   dd= dd.Mid(3);

				   Inver        = FALSE;
				   Mayus        = FALSE;
				   Orden        = FALSE;
				   OrdenInverso = FALSE;
				   Agrupado     = FALSE;
				   CondAgrupada = FALSE;
				   Preguntar    = FALSE;

				   ji = dd.Find((char)3);
				   if (ji > -1)
				   {
					  dd.Remove((char)3);
					  Inver = TRUE;
				   }
				   ji = dd.Find((char)4);
				   if (ji > -1)
				   {
					  dd.Remove((char)4);
					  Mayus = TRUE;
				   }               
				   ji = dd.Find((char)5);
				   if (ji > -1)
				   {
					  dd.Remove((char)5);
					  Orden = TRUE;
				   }
				   ji = dd.Find((char)6);
				   if (ji > -1)
				   {
					  dd.Remove((char)6);
					  OrdenInverso = TRUE;
				   }
				   ji = dd.Find((char)7);
				   if (ji > -1)
				   {
					  dd.Remove((char)7);
					  Agrupado = TRUE;
				   }
				   ji = dd.Find((char)8);
				   if (ji > -1)
				   {
					  dd.Remove((char)8);
					  CondAgrupada = TRUE;
				   }
				   ji = dd.Find((char)9);
				   if (ji > -1)
				   {
					  dd.Remove((char)9);
					  Preguntar = TRUE;
				   }
				   opera = 0;
				   ji = dd.Find((char)14);
				   if (ji > -1)
				   {
					  dd.Remove((char)14);
					  opera = 1;
				   }
				   ji = dd.Find((char)15);
				   if (ji > -1)
				   {
					  dd.Remove((char)15);
					  opera = 2;
				   }
				   ji = dd.Find((char)16);
				   if (ji > -1)
				   {
					  dd.Remove((char)16);
					  opera = 3;
				   }
				   ji = dd.Find((char)17);
				   if (ji > -1)
				   {
					  dd.Remove((char)17);
					  opera = 4;
				   }
				   ji = dd.Find((char)18);
				   if (ji > -1)
				   {
					  dd.Remove((char)18);
					  opera = 5;
				   }
				   ji = dd.Find((char)19);
				   if (ji > -1)
				   {
					  dd.Remove((char)19);
					  opera = 6;
				   }


				   ni = atoi(dd.Mid(0,3));
				   if (ni < 0 || ni >= def->m_def->m_info->n_campos)
				   {
					   return FALSE;
				   }
				   ni = def->m_def->m_idcampos[ni];
				   if (ni < 0 || ni >= def->m_nitem)
				   {
					   return FALSE;
				   }

				   def->m_item[ni]->m_opera = opera;
				   def->m_item[ni]->m_Inver = Inver;
				   def->m_item[ni]->m_Mayus = Mayus;
				   def->m_item[ni]->m_Orden = Orden;
				   def->m_item[ni]->m_OrdenInverso = OrdenInverso;
				   def->m_item[ni]->m_Agrupado = Agrupado;
				   def->m_item[ni]->m_CondAgrupada = CondAgrupada;
				   def->m_item[ni]->m_Preguntar = Preguntar;

				   def->m_item[ni]->m_checked = TRUE;
				   if (dd[3] == 'S')
				   {
					   def->m_item[ni]->m_salida = TRUE;
				   }
				   else
				   {
					   def->m_item[ni]->m_salida = FALSE;
				   }
				   def->m_item[ni]->m_TipoValor = dd[4]-48;

				   for(j=5;dd[j] && dd[j] != 2;j++);
				   def->m_item[ni]->m_LimiteI = dd.Mid(5,j-5);
				   def->m_item[ni]->m_LimiteI.Remove((char)0);
				   def->m_item[ni]->m_LimiteI.TrimRight();
				   j++;
				   for(ji = j;dd[j] && dd[j] != 2;j++);
				   def->m_item[ni]->m_LimiteS = dd.Mid(ji,j-ji);
				   def->m_item[ni]->m_LimiteS.Remove((char)0);
				   def->m_item[ni]->m_LimiteS.TrimRight();
				   j++;
				   def->m_item[ni]->m_Valor = dd.Mid(j);
				   def->m_item[ni]->m_Valor.Remove((char)0);
				   def->m_item[ni]->m_Valor.TrimRight();
			   }
			   else
				   break;
			  
			   if (s.GetLength() <= (i+1))
				   s = "";
			   else
				   s = s.Mid(i+1);
		   }

	   }
	   else
	   { /* simple */
		   while(!s.IsEmpty())
		   {
			   s += (char)0;
			   for (i = 0;s[i] && s[i] != 1;i++);
			   if (i)
			   {
				   dd = s.Mid(0,i);
				   dd += (char)0;

				   Inver = FALSE;
				   Mayus = FALSE;
				   Orden = FALSE;
				   OrdenInverso = FALSE;
				   Agrupado     = FALSE;
				   CondAgrupada = FALSE;
				   Preguntar    = FALSE;

				   ji = dd.Find((char)3);
				   if (ji > -1)
				   {
					  dd.Remove((char)3);
					  Inver = TRUE;
				   }
				   ji = dd.Find((char)4);
				   if (ji > -1)
				   {
					  dd.Remove((char)4);
					  Mayus = TRUE;
				   }               
				   ji = dd.Find((char)5);
				   if (ji > -1)
				   {
					  dd.Remove((char)5);
					  Orden = TRUE;
				   }
				   ji = dd.Find((char)6);
				   if (ji > -1)
				   {
					  dd.Remove((char)6);
					  OrdenInverso = TRUE;
				   }
				   ji = dd.Find((char)7);
				   if (ji > -1)
				   {
					  dd.Remove((char)7);
					  Agrupado = TRUE;
				   }
				   ji = dd.Find((char)8);
				   if (ji > -1)
				   {
					  dd.Remove((char)8);
					  CondAgrupada = TRUE;
				   }
				   ji = dd.Find((char)9);
				   if (ji > -1)
				   {
					  dd.Remove((char)9);
					  Preguntar = TRUE;
				   }
				   opera = 0;
				   ji = dd.Find((char)14);
				   if (ji > -1)
				   {
					  dd.Remove((char)14);
					  opera = 1;
				   }
				   ji = dd.Find((char)15);
				   if (ji > -1)
				   {
					  dd.Remove((char)15);
					  opera = 2;
				   }
				   ji = dd.Find((char)16);
				   if (ji > -1)
				   {
					  dd.Remove((char)16);
					  opera = 3;
				   }
				   ji = dd.Find((char)17);
				   if (ji > -1)
				   {
					  dd.Remove((char)17);
					  opera = 4;
				   }
				   ji = dd.Find((char)18);
				   if (ji > -1)
				   {
					  dd.Remove((char)18);
					  opera = 5;
				   }
				   ji = dd.Find((char)19);
				   if (ji > -1)
				   {
					  dd.Remove((char)19);
					  opera = 6;
				   }

				   ni = atoi(dd.Mid(0,3));
				   if (ni < 0 || ni >= m_principal->m_nitem)
				   {
					   return FALSE;
				   }
				   m_principal->m_item[ni]->m_opera = opera;
				   m_principal->m_item[ni]->m_Inver = Inver;
				   m_principal->m_item[ni]->m_Mayus = Mayus;
				   m_principal->m_item[ni]->m_Orden = Orden;
				   m_principal->m_item[ni]->m_OrdenInverso = OrdenInverso;
				   m_principal->m_item[ni]->m_Agrupado = Agrupado;
				   m_principal->m_item[ni]->m_CondAgrupada = CondAgrupada;
				   m_principal->m_item[ni]->m_Preguntar = Preguntar;

				   m_principal->m_item[ni]->m_checked = TRUE;
				   if (dd[3] == 'S')
				   {
					   m_principal->m_item[ni]->m_salida = TRUE;
				   }
				   else
				   {
					   m_principal->m_item[ni]->m_salida = FALSE;
				   }

				   m_principal->m_item[ni]->m_TipoValor = dd[4]-48;

				   for(j=5;dd[j] && dd[j] != 2;j++);
				   m_principal->m_item[ni]->m_LimiteI = dd.Mid(5,j-5);
				   m_principal->m_item[ni]->m_LimiteI.Remove((char)0);
				   m_principal->m_item[ni]->m_LimiteI.TrimRight();
				   j++;
				   for(ji = j;dd[j] && dd[j] != 2;j++);
				   m_principal->m_item[ni]->m_LimiteS = dd.Mid(ji,j-ji);
				   m_principal->m_item[ni]->m_LimiteS.Remove((char)0);
				   m_principal->m_item[ni]->m_LimiteS.TrimRight();
				   j++;
				   m_principal->m_item[ni]->m_Valor = dd.Mid(j);
				   m_principal->m_item[ni]->m_Valor.Remove((char)0);
				   m_principal->m_item[ni]->m_Valor.TrimRight();
			   }
			   else
				   break;
			   s = s.Mid(i+1);
		   }
	   }

    return TRUE;
}

void CDlgGridFiltro::OnCancel() 
{
    if (m_modificado)
    {
        QuizaGraba();
    }		
	CBASEDLG::OnCancel();
}

void CDlgGridFiltro::OnOK() 
{
    OnSelchangeLista();
    RecogeLista();
	if (!RellenaListado())
	{
		AfxMessageBox("Debe seleccionar al menos una columna",MB_OK|MB_ICONHAND);
		return;
	}
    m_combo.GetWindowText(m_nombre);
	if (m_filtrousuario)
	{
		m_pTabla->EjecutaFiltro(this);
		return;
	}
    if (m_modificado)
    {
        QuizaGraba();
    }
	CBASEDLG::OnOK();
}

BOOL CDlgGridFiltro::QuizaGraba()
{
    m_combo.GetWindowText(m_nombre);
    if (m_nombre.Compare("Nuevo"))
    {
        if (AfxMessageBox("¿Desea guardar los cambios?",MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
            OnButtonguardar();
            return TRUE;
        }
    }
    return FALSE;
}

void CDlgGridFiltro::OnAscendente() 
{
	m_modificado = TRUE;
	CButton *pButton;
	pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
	pButton->SetCheck(1);
	pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
	pButton->SetCheck(0);	
	pButton = (CButton *)GetDlgItem(IDC_AGRU);
	pButton->EnableWindow(TRUE);
	for (int i = 0;i < m_nitems;i++)
	{
		if (m_items[i]->m_Agrupado)
		{
			pButton->SetCheck(1);
			break;
		}
	}
	OnSelchangeLista();	
}

void CDlgGridFiltro::OnSinorden() 
{
	m_modificado = TRUE;
	CButton *pButton;
	pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
	pButton->SetCheck(1);
	pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
	pButton->SetCheck(0);	
	pButton = (CButton *)GetDlgItem(IDC_AGRU);
	pButton->SetCheck(0);
	pButton->EnableWindow(FALSE);
	OnSelchangeLista();	
}

void CDlgGridFiltro::OnDescendente() 
{
	m_modificado = TRUE;
	CButton *pButton;
	pButton = (CButton *)GetDlgItem(IDC_SINORDEN);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_ASCENDENTE);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_DESCENDENTE);
	pButton->SetCheck(1);		
	pButton = (CButton *)GetDlgItem(IDC_AGRU);
	pButton->EnableWindow(TRUE);
	for (int i = 0;i < m_nitems;i++)
	{
		if (m_items[i]->m_Agrupado)
		{
			pButton->SetCheck(1);
			break;
		}
	}
	OnSelchangeLista();	
}

void CDlgGridFiltro::OnRadioexacto() 
{
	m_modificado = TRUE;
	CButton *pButton;	
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(1);	
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(0);	

	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(FALSE);
	pButton->SetCheck(0);	
}

void CDlgGridFiltro::OnRadioparcial() 
{
	m_modificado = TRUE;
	CButton *pButton;
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(1);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(0);		

	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(TRUE);
	pButton->SetCheck(0);
}

void CDlgGridFiltro::OnRadiocontenido() 
{		
	m_modificado = TRUE;
	CButton *pButton;
    pButton = (CButton *)GetDlgItem(IDC_RADIOEXACTO);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOPARCIAL);
    pButton->SetCheck(0);
    pButton = (CButton *)GetDlgItem(IDC_RADIOCONTENIDO);
    pButton->SetCheck(1);


	pButton = (CButton *)GetDlgItem(IDC_CHECK3);
	pButton->EnableWindow(TRUE);
}

BOOL CDlgGridFiltro::GrabaFiltros(LPCSTR clave, LPCSTR filtros)
{
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 14;
    char *p;
    char *tmp;
	int memoria;

    memoria = strlen(clave)+ strlen(filtros) + 2;

    tam = (sizeof(long) * 7) + (sizeof(int)*2) + 100 + memoria;
    respuesta = tam;

	tmp = (char *)malloc(tam + 1);

    p = tmp;

    memcpy((void *)p,(void *)&m_pTabla->m_localizador,sizeof(int));
    p += sizeof(int);


    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);  
	
	if (m_filtrousuario)
		*p = 0;
	else
		strcpy(p,clave);
	p += (strlen(p) + 1);
	strcpy(p,filtros);	

    m_pTabla->DespachadorInterno(13,(void *)tmp);

	free((void *)tmp);

	return TRUE;
}

BOOL CDlgGridFiltro::LeerFiltros(LPCSTR clave, CString &filtros)
{
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 15;
    char *p;
    char *tmp;
	char tmpx[756];
	int memoria;


    tam = (sizeof(long) * 7) + (sizeof(int)*2) + 512;
    respuesta = tam;

    p = tmpx;

    memcpy((void *)p,(void *)&m_pTabla->m_localizador,sizeof(int));
    p += sizeof(int);

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);

	if (m_filtrousuario)
		*p = 0;
	else
		strcpy(p,clave);
	p += (strlen(p)+1);

	memset(p,0,sizeof(int));

	m_pTabla->DespachadorInterno(13,(void *)tmpx);

	memcpy((void *)&memoria,(void *)p,sizeof(int));

	if (!memoria)
	{
		return FALSE;
	}

	operacion = 16;

    tam = (sizeof(long) * 7) + (sizeof(int)*2) + 100 + memoria;
    respuesta = tam;

	tmp = (char *)malloc(tam + 1);

    p = tmp;

    memcpy((void *)p,(void *)&m_pTabla->m_localizador,sizeof(int));
    p += sizeof(int);

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
	p += sizeof(int);  
	if (m_filtrousuario)
		*p = 0;
	else
		strcpy(p,clave);
	p += (strlen(p)+1);    
	memcpy((void *)p,(void *)&memoria,sizeof(int));

    m_pTabla->DespachadorInterno(13,(void *)tmp);
	
	p[memoria] = 0;
	filtros = p;
	
	free((void *)tmp);

	return TRUE;
}

void CDlgGridFiltro::RellenaMenu(CMenu *filtros,int desde,int id)
{
	CString s;
	int cuenta = 0;
	s = "";

	CString clave = "GridFiltrosBorrador ";
	clave += m_principal->m_def->m_info->PathDef;

	unsigned char *in;
	unsigned int ll;
	if (theApp.GetProfileBinary(clave,"_INDICE_",&in,&ll))
	{
		in[ll-1] = 0;
		s = in;
		delete [] in;
	}	


	if (!s.IsEmpty())
	{
		LPTSTR px,ss;

		filtros->InsertMenu(desde,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
		desde++;

		s += '\0';		
		ss = s.LockBuffer();
		px = ss;
		for (int i = 0;ss[i];i++)
		{
		   if (ss[i] == 1)
		   {
			   ss[i] = 0;
			   if (strlen(px))
			   {
				   if (strcmp(px,"Nuevo"))
				   {
						filtros->InsertMenu(desde,MF_STRING | MF_BYPOSITION,id+cuenta,(const char *)px);					
						cuenta++;
						desde++;				   
				   }
			   }
			   i++;
			   px = ss + i;
		   }
		}
		if (strlen((char *)px))
		{
		    if (strcmp(px,"Nuevo"))
			{
				filtros->InsertMenu(desde,MF_STRING | MF_BYPOSITION,id+cuenta,(const char *)px);
				cuenta++;
				desde++;			
			}
		}
		s.UnlockBuffer();
	}

	cuenta += 1000;
	s = "";
	LeerFiltros("_INDICE_",s);	

	if (!s.IsEmpty())
	{
		LPTSTR px,ss;

		filtros->InsertMenu(desde,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
		desde++;

		s += '\0';		
		ss = s.LockBuffer();
		px = ss;
		for (int i = 0;ss[i];i++)
		{
		   if (ss[i] == 1)
		   {
			   ss[i] = 0;
			   if (strlen(px))
			   {
				   if (strcmp(px,"Nuevo"))
				   {
					   filtros->InsertMenu(desde,MF_STRING | MF_BYPOSITION,id+cuenta,(const char *)px);					
					   cuenta++;
					   desde++;				   
				   }
			   }
			   i++;
			   px = ss + i;
		   }
		}
		if (strlen((char *)px))
		{
		    if (strcmp(px,"Nuevo"))
			{
				filtros->InsertMenu(desde,MF_STRING | MF_BYPOSITION,id+cuenta,(const char *)px);
				cuenta++;
				desde++;			
			}
		}
		s.UnlockBuffer();
	}

}

void CDlgGridFiltro::InicializaItems()
{
	CString s;
	s = "";
	if (!m_filtrousuario)
	{
		if (m_registrolocal)
		{
			CString clave = "GridFiltrosBorrador ";
			clave += m_principal->m_def->m_info->PathDef;

			unsigned char *in;
			unsigned int ll;
			if (theApp.GetProfileBinary(clave,"_INDICE_",&in,&ll))
			{
				in[ll-1] = 0;
				s = in;
				delete [] in;
			}	
		}
		else
		{
			LeerFiltros("_INDICE_",s);
		}
	}

	m_combo.ResetContent( );	

	{
		LPTSTR px,ss;
		s += '\0';		
		ss = s.LockBuffer();
		px = ss;
		for (int i = 0;ss[i];i++)
		{
		   if (ss[i] == 1)
		   {
			   ss[i] = 0;
			   if (strlen(px))
					m_combo.AddString(px);
			   i++;
			   px = ss + i;
		   }
		}
		if (strlen((char *)px))
		   m_combo.AddString((char *)px);
		s.UnlockBuffer();
	}

	m_nombre = "Nuevo";
    if (m_combo.FindStringExact(-1,m_nombre) < 0)
    {
        m_combo.AddString(m_nombre);
    }
    m_combo.SelectString(-1,m_nombre);	

    if (!LeeDelRegistro())
    {
        m_principal->PorDefecto(TRUE);
    }
    
    OnSelchangeLista();
    UpdateData(FALSE);
    RellenaLista();
	RellenaListado();

    m_modificado = FALSE;
}


void CDlgGridFiltro::OnCheck7()  // PREDETERMINADA
{
    if (m_filtrousuario)
	   return;

    CString clave = "GridFiltrosBorrador ";
    clave += m_principal->m_def->m_info->PathDef;

	UpdateData(TRUE);
	
    int sel = m_combo.GetCurSel( );
    if (sel > -1)
    {
        m_combo.GetLBText(sel,m_nombre);
        m_combo.SetWindowText(m_nombre);
    }
    else
        m_combo.GetWindowText(m_nombre);

    if (!m_predeterminado)/* OJO ESTA A AUTO AHORA POR PROBLEMAS CON EL PROF-UIS, por eso la condicion inversa */
	{
	    m_npredeterminado = "";
		m_bpredeterminado = m_registrolocal;
		m_predeterminado = FALSE;
	}
	else
	{
	    m_npredeterminado = m_nombre;
		m_bpredeterminado = m_registrolocal;
		m_predeterminado = TRUE;
	}
	theApp.WriteProfileString(clave,"_Predeterminado_",m_npredeterminado);
	theApp.WriteProfileInt(clave,"_Predeterminado_Local_",(int)m_bpredeterminado);

	
	UpdateData(FALSE);	
}

void CDlgGridFiltro::PonPredeterminado()
{
   if (m_filtrousuario)
	   return;
   UpdateData(TRUE);   
   if (!m_npredeterminado.Compare(m_nombre))
   {	   
	   if (m_registrolocal == m_bpredeterminado)
	   {
		   m_predeterminado = TRUE;
	   }
	   else
	   {
		   m_predeterminado = FALSE;
	   }
   }
   else
   {
	   m_predeterminado = FALSE;
   }
   UpdateData(FALSE);
}

/**************************************************/
/* A llamar sin inicializar realmente el dialogo */
BOOL CDlgGridFiltro::CargaPredeterminado()
{	
	m_registrolocal = m_bpredeterminado;
	m_nombre = m_npredeterminado;
	if (m_nombre.IsEmpty())
	{
		return FALSE;
	}
	return LeeDelRegistro();
}

void CDlgGridFiltro::OnButtonborrar() 
{
	if (AfxMessageBox("¿Seguro?",MB_YESNO) != IDYES)
	{
		return;
	}
	UpdateData(TRUE);
	OnSelchangeLista();
    int sel = m_combo.GetCurSel( );
    if (sel > -1)
    {
        m_combo.GetLBText(sel,m_nombre);
        m_combo.SetWindowText(m_nombre);
		m_combo.DeleteString(sel);
		Guarda(TRUE);
    }
    m_nombre = "Nuevo";
    m_combo.SelectString(-1,m_nombre);
    if (m_combo.FindStringExact(-1,m_nombre) < 0)
    {
       m_combo.AddString(m_nombre);
	   m_combo.SelectString(-1,m_nombre);
    }
    if (!LeeDelRegistro())
    {
        m_principal->PorDefecto(TRUE);
    }
    m_modificado = FALSE;
    RellenaLista();
	RellenaListado();
}

void CDlgGridFiltro::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgGridFiltro::OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;	
	if (pNMTreeView->itemNew.lParam)
	{
		CFiltroDef *padre = (CFiltroDef *)pNMTreeView->itemNew.lParam;
		if (!(pNMTreeView->itemNew.state & TVIS_EXPANDED))
		{
			HTREEITEM child;
			child = m_ficheros.GetChildItem( pNMTreeView->itemNew.hItem);
			if (child && !m_ficheros.GetItemData(child))
			{
				char tmp[10512];				
				int *p1;
				int len;
				TV_INSERTSTRUCT Nuevo;				
				CFiltroDef *hijo;
				
				m_ficheros.DeleteItem(child);				

				p1 = (int *)(tmp+(sizeof(long) * 3) + (sizeof(int)*2));
				*p1 = 9900;
				len = sizeof(int);
				if (padre == m_principal)
				{
					*(p1+1) = 0;
					len += sizeof(int);					
				}
				else
				{
					strcpy((char *)(p1+1),padre->m_nomdef);
					len += (strlen(padre->m_nomdef)+1);
				}
				m_pTabla->EnviaRetro(20,len,10000,tmp);
				/* 
				CFiltroDef(m_pTabla->m_def->m_info->PathDef,0);
				*/				
				if (!*p1)
				{
					m_ficheros.InsertItem(TVIF_IMAGE | TVIF_TEXT,
					  "Sin Relaciones",
					  0,
					  0,
					  0,
					  0,
					  0,
					  pNMTreeView->itemNew.hItem,
					  TVI_LAST);
				}
				else
				{
					int maxrel = *p1;
					int memdef;
					int nunion;
					char *p0;
					int *campospadre;
					int *camposhijo;
					p1++;
					if (padre->m_parientes)
					{
						padre->BorraParientes();
					}
					padre->m_nparientes = maxrel;
					padre->m_parientes = (CFiltroDef **)malloc(sizeof(CFiltroDef *)*maxrel);
					memset((void *)padre->m_parientes,0,sizeof(CFiltroDef *)*maxrel);

					for (int i = 0;i < maxrel;i++)
					{
						memdef = *p1;
						p1++;
						nunion = *p1;
						p1++;
						campospadre = p1;
						p1 += nunion;
						camposhijo = p1;
						p1 += nunion;
						p0 = (char *)p1;
						hijo = new CFiltroDef(p0,memdef,NULL,padre,nunion,campospadre,camposhijo,NULL);
						padre->m_parientes[i] = hijo;
						// path del def
						p0 += (strlen(p0)+1);
						// nombre del def						

						Nuevo.hParent = pNMTreeView->itemNew.hItem;
						Nuevo.hInsertAfter = TVI_LAST;					
						Nuevo.item.pszText = p0;
						Nuevo.item.cchTextMax = strlen(p0);
						Nuevo.item.iImage = 0;
						Nuevo.item.iSelectedImage = 0;			
						Nuevo.item.lParam = (LPARAM)hijo;
						Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

						m_ficheros.InsertItem(TVIF_IMAGE | TVIF_TEXT,
						  "",
						  0,
						  0,
						  0,
						  0,
						  0,
						  (hijo->m_hitem = m_ficheros.InsertItem(&Nuevo)),
						  TVI_FIRST);									

						p0 += (strlen(p0)+1);
						if (*p0)
							break;
						p0++;

						p1 = (int *)p0;
					}
				}
			}
		}
	}
	*pResult = 0;
}

void CDlgGridFiltro::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_relaciones.ResetContent( );

	if (pNMTreeView->itemNew.lParam && m_seleccion != (CFiltroDef *)pNMTreeView->itemNew.lParam)
	{
		m_seleccion = (CFiltroDef *)pNMTreeView->itemNew.lParam;
		RellenaLista();
		if (m_seleccion->m_camposrel > 0 && m_seleccion->m_padre)
		{
			int j;
			CString s;
			for (int i = 0;i < m_seleccion->m_camposrel;i++)
			{
				j = m_seleccion->m_campospadre[i];
				if (j > -1)
				{
					s = m_seleccion->m_padre->m_item[j]->m_name;
					j = m_seleccion->m_camposhijo[i];
					if (j > -1)
					{
						s += " <-> ";
						s += m_seleccion->m_item[j]->m_name;
					}
					m_relaciones.AddString(s);
				}
			}
		}
	}	
	
	*pResult = 0;
}

void CDlgGridFiltro::OnNuevarelacion() 
{
	CDlgNewRela dlg(this);

	{
	  LRESULT Result = 0;
	  NM_TREEVIEW NMTreeView;
	  memset((void *)&NMTreeView,0,sizeof(NMTreeView));

	  NMTreeView.itemNew.lParam = (LPARAM)m_seleccion;
	  NMTreeView.itemNew.hItem  = m_seleccion->m_hitem;
	  OnItemexpandingTree1((NMHDR*)&NMTreeView, &Result);
	}	

	dlg.m_fichero = m_seleccion;

	if (dlg.DoModal() == IDOK)
	{
		int n;		
		TV_INSERTSTRUCT Nuevo;
		HTREEITEM child;
			
		n = m_seleccion->m_nparientes++;
		m_seleccion->m_parientes = (CFiltroDef **)realloc((void *)m_seleccion->m_parientes,sizeof(CFiltroDef *)*m_seleccion->m_nparientes);
		m_seleccion->m_parientes[n] = dlg.m_relacionado;
		dlg.m_relacionado = NULL;	

        child = m_ficheros.GetChildItem( m_seleccion->m_hitem );
		if (child && !m_ficheros.GetItemData(child))
		{
			m_ficheros.DeleteItem(child);
		}
		Nuevo.hParent = m_seleccion->m_hitem;
		Nuevo.hInsertAfter = TVI_LAST;					
		Nuevo.item.pszText = m_seleccion->m_parientes[n]->m_def->m_info->NomFic;
		Nuevo.item.cchTextMax = strlen(m_seleccion->m_parientes[n]->m_def->m_info->NomFic);
		Nuevo.item.iImage = 0;
		Nuevo.item.iSelectedImage = 0;			
		Nuevo.item.lParam = (LPARAM)m_seleccion->m_parientes[n];
		Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

		m_ficheros.InsertItem(TVIF_IMAGE | TVIF_TEXT,
		  "",
		  0,
		  0,
		  0,
		  0,
		  0,
		  (m_seleccion->m_parientes[n]->m_hitem = m_ficheros.InsertItem(&Nuevo)),
		  TVI_FIRST);
	}
}


void CDlgGridFiltro::OnFormato() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgGridFiltro::OnSelchangeList1() 
{
	/* Ya no ... sustituida por resultado OnClickList4 */
	/*OnSelchangeLista();*/

	if (m_seleccion)
	{
		m_seleccion->m_sel = m_Lista.GetCaretIndex();
	}
}

void CDlgGridFiltro::OnBeginlabeleditList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgGridFiltro::OnEndlabeleditList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgGridFiltro::OnClickList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*OnSelchangeLista();*/
	*pResult = 0;
}

void CDlgGridFiltro::OnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if ( (pNMListView->uNewState & LVIS_SELECTED) )
    {
		OnSelchangeLista();
	}	
	*pResult = 0;
}

void CDlgGridFiltro::OnEditchangeComboopera() 
{	
	OnSelchangeLista();	
}

void CDlgGridFiltro::OnCheckincluido() 
{
	OnSelchangeLista();		
}

void CDlgGridFiltro::OnSelchangeComboopera() 
{
	OnSelchangeLista();	
}

void CDlgGridFiltro::OnAgrupador() 
{
	UpdateData(TRUE);
	for (int i = 0;i < m_nitems;i++)
	{
		if (m_items[i]->m_Orden)
		{
			m_items[i]->m_Agrupado = m_agrupado;
		}
		else
		{
			m_items[i]->m_Agrupado = FALSE;
		}
	}
}

void CDlgGridFiltro::OnArriba()
{
	CString *s;
	CString s1;
	int n = m_listado.GetHeaderCtrl()->GetItemCount();
	int j,i;

	s = new CString[n];

	for (i = 1;i < m_listado.GetItemCount();i++)
	{
		if (m_listado.GetItemState(i,LVIS_SELECTED) && !m_listado.GetItemState(i-1,LVIS_SELECTED))
		{
			for (j = 0;j < n;j++)
			{
				s[j] = m_listado.GetItemText( i, j );
			}

			j = m_idxs[i];
			m_idxs[i-1] = j;
			m_idxs[i] = m_idxs[i-1];

			j = m_idxs[i];
			m_items[j]->m_idx = i;
			j = m_idxs[i-1];
			m_items[j]->m_idx = i-1;
		    
			s1 = m_ordenid[i];
			m_ordenid[i] = m_ordenid[i-1];
			m_ordenid[i-1] = s1;


			m_listado.DeleteItem(i);			
			m_listado.InsertItem(i-1,s[0]);
			for (j = 1;j < n;j++)
			{
				m_listado.SetItemText(i-1,j,s[j]);
			}
			m_listado.SetItemState( i-1, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	delete [] s;
}

void CDlgGridFiltro::OnAbajo()
{
	CString *s,s1;
	int n = m_listado.GetHeaderCtrl()->GetItemCount();
	int j,i;

	s = new CString[n];

	for (i = m_listado.GetItemCount()-2;i >= 0;i--)
	{
		if (m_listado.GetItemState(i,LVIS_SELECTED) && !m_listado.GetItemState(i+1,LVIS_SELECTED))
		{
			for (j = 0;j < n;j++)
			{
				s[j] = m_listado.GetItemText( i, j );
			}

			j = m_idxs[i];
			m_idxs[i+1] = j;
			m_idxs[i] = m_idxs[i+1];

			j = m_idxs[i];
			m_items[j]->m_idx = i;
			j = m_idxs[i+1];
			m_items[j]->m_idx = i+1;

			s1 = m_ordenid[i];
			m_ordenid[i] = m_ordenid[i+1];
			m_ordenid[i+1] = s1;

			m_listado.DeleteItem(i);
			m_listado.InsertItem(i+1,s[0]);
			for (j = 1;j < n;j++)
			{
				m_listado.SetItemText(i+1,j,s[j]);
			}
			m_listado.SetItemState( i+1, LVIS_SELECTED, LVIS_SELECTED);
			//m_items[i]
		}
	}
	delete [] s;	
}
