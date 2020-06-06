
// TablaView.cpp : implementation file
//

#include "stdafx.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <direct.h>

#include "Diagramx.h"
#include "TablaView.h"
#include "DlgGridRellena.h"
#include "DsStdDlg.h"
#include "Trasmite.h"
#include "DsxView.h"
#include "DsxDoc.h"
#include "DsEdit.h"
#include "DsCombo.h"

#include "DlgGridFiltro.h"
#include "DlgCol.h"
#include "DlgOrdenar.h"
#include "DlgBusqueda.h"
#include "DlgOperaciones.h"

#include "DsWord.h"
#include "DsExcel.h"

#include "DlgProgreso.h"

#include "DlgExporta.h"

#include "SkinScrollWnd.h"

#include "DlgRegCambio.h"

//#include "Diagram9Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* tendra que ser configurable */
#define MAX_MEMORIA 5000000




CInfoDef::CInfoDef(struct Info_Entra_Campos *info,CTablaView *pTabla)
{
    ASSERT(info);    

    int i;

    m_usermode = FALSE;

    m_pTabla = pTabla;

    m_info = (struct Info_Entra_Campos *)::malloc(info->Len);
    ::memcpy((void *)m_info,(void *)info,info->Len);

    //ASSERT(m_info->t_total_campos == m_info->n_campos);

	m_RegPath = m_info->PathDef;

	m_RegPath.Remove((char)7);
	m_RegPath.TrimRight();

	if (m_RegPath.IsEmpty())
	{
		m_pTabla->m_tw->GetWindowText(m_RegPath);
	}

	m_RegPath.Replace('/','_');
	m_RegPath.Replace('\\','_');
	m_RegPath.Replace(':','_');

	/*
    for (i = 0;m_info->PathDef[i];i++)
    {
        if (m_info->PathDef[i] == '/' || m_info->PathDef[i] == '\\' || m_info->PathDef[i] == ':')
        {
            m_info->PathDef[i] = '_';
        }
    }
	*/
    
    m_idcampos = new int[m_info->t_total_campos];

    m_colcampos = new int[m_info->t_total_campos];	
    for (i = 0;i < m_info->n_campos;i++)
    {        
        m_idcampos[ i ] = -1;
    }
    
    CreaCamcolu();
}


BOOL CInfoDef::CreaCamcolu(BOOL iniciar,BOOL todas)
{
    struct Info_Entra_Campo *pC;
    int i;    

    for (i = 0;i < m_info->n_campos;i++)
    {
        pC = (struct Info_Entra_Campo *)((char *) ((char *)m_info) + *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (i*sizeof(int)) )) );
        m_idcampos[ pC->Id ] = i;
    }

	if (!m_pTabla)
	{        
		return FALSE;
	}

	m_pTabla->m_defaultama = "";
    m_n_campos_col = 0;



    if (!iniciar)
    {
        CString cc;
        CTablaW *pwpadre = NULL;
        if (m_pTabla->m_padre > -1)
        {
            CDsStdCtrl *pw;
            CDsStdDlg::m_todosctrl.Lookup(m_pTabla->m_padre,pw);
            if (pw)
            {
                pwpadre = (CTablaW *)pw->m_ctrl;
            }
        }

		
		{
			m_pTabla->m_tw->GetWindowText(cc);

			cc += " ";
			cc += m_RegPath;		
	        
			CString dd;			

			if (!iniciar && m_pTabla)
			{
				dd = theApp.GetProfileString("GridDefaultOrden",cc,"-1");
				m_pTabla->m_reordenado = atoi(dd);
			}
			
			if (!m_pTabla || !(m_pTabla->m_ini_gmodo&1))
			{
				dd = theApp.GetProfileString("GridDefaultOrder",cc,"");
			}		

			if (!dd.IsEmpty())
			{
				int campo;            
				while(!iniciar && !dd.IsEmpty())
				{
					campo = atoi(dd.Mid(0,3));
					if (campo < 0 || campo >= m_info->n_campos)
					{
						iniciar = TRUE;
						break;
					}
					for (i = 0;i < m_n_campos_col;i++)
					{
						if (m_colcampos[i] == campo)
						{
							iniciar = TRUE;
							break;
						}
					}

					m_colcampos[m_n_campos_col] = campo;
					//pC = (struct Info_Entra_Campo *)((char *) ((char *)m_info) + *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (campo*sizeof(int)) )) );
					//m_idcampos[ pC->Id ] = campo;
					m_n_campos_col++;

					dd = dd.Mid(3);
				}
				if (!iniciar)
				{
					m_pTabla->m_defaultama = theApp.GetProfileString("GridDefaultTaman",cc,"");
				}
			}
			else 
				iniciar = TRUE;

			if (iniciar && pwpadre && pwpadre->m_tabla.m_def->m_info->n_campos == m_info->n_campos)
			{		
				iniciar = FALSE;
				m_pTabla->m_reordenado = pwpadre->m_tabla.m_reordenado;
				m_n_campos_col = pwpadre->m_tabla.m_def->m_n_campos_col;
				for (i = 0;i < m_n_campos_col;i++)
				{
					m_colcampos[i] = pwpadre->m_tabla.m_def->m_colcampos[i];
				}
				m_pTabla->m_defaultama = pwpadre->m_tabla.m_defaultama;
			}
		}
    }

    if (iniciar)
    {
        m_n_campos_col = 0;
        for (i = 0;i < m_info->n_campos;i++)
        {
            pC = (struct Info_Entra_Campo *)((char *) ((char *)m_info) + *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (i*sizeof(int)) )) );
            //m_idcampos[ pC->Id ] = i;

            if ( (todas && ((char *)((char *)pC) + pC->d_nombre)[0] != '~') || !(pC->c_flags & 1))
            {
                m_colcampos[m_n_campos_col] = i;
                m_n_campos_col++;
            }
        }
    }
    else
    {
        m_usermode = TRUE;
    }
	if (!m_n_campos_col) {
        for (i = 0;i < m_info->n_campos;i++)
        {
            pC = (struct Info_Entra_Campo *)((char *) ((char *)m_info) + *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (i*sizeof(int)) )) );
            //m_idcampos[ pC->Id ] = i;

            if (((char *)((char *)pC) + pC->d_nombre)[0] != '~')
            {
                m_colcampos[m_n_campos_col] = i;
                m_n_campos_col++;
            }
        }
	}

    for (i = m_n_campos_col;i < m_info->t_total_campos;i++)
    {
        m_colcampos[ i ] = -1;
    }
    return TRUE;
}

CInfoDef::~CInfoDef()
{
    if (m_info)
        ::free((void *)m_info);
    if (m_idcampos)
        delete [] m_idcampos;
    if (m_colcampos)
        delete [] m_colcampos;
}

struct Info_Entra_Campo *CInfoDef::Campo(int campo,BOOL directo)
{	
    ASSERT(campo >= 0 && campo < m_info->n_campos);

    if (!directo)
    {
        campo = m_colcampos[ campo ];

        ASSERT(campo > -1);
    }

    register int d = *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (campo*sizeof(int)) ));

    return( (struct Info_Entra_Campo *)((char *) ((char *)m_info) + d ) );
}
char *CInfoDef::c_nombre(int campo,BOOL directo)
{
    struct Info_Entra_Campo *pcampo = Campo(campo,directo);
    return( (char *)((char *)pcampo) + pcampo->d_nombre );
}
char *CInfoDef::c_minimo(int campo,BOOL directo)
{
    struct Info_Entra_Campo *pcampo = Campo(campo,directo);
    return( (char *)((char *)pcampo) + pcampo->d_minimo );
}
char *CInfoDef::c_maximo(int campo,BOOL directo)
{
    struct Info_Entra_Campo *pcampo = Campo(campo,directo);
    return( (char *)((char *)pcampo) + pcampo->d_maximo );
}
char *CInfoDef::c_usi(int campo,BOOL directo)
{
    struct Info_Entra_Campo *pcampo = Campo(campo,directo);
    return( (char *)((char *)pcampo) + pcampo->d_usi);
}

struct Info_Entra_Clave* CInfoDef::Clave(int clave)
{
    ASSERT(clave >= 0 && clave < m_info->n_claves);

    register int d = *((int *)((char *) ((char *)m_info) + m_info->d_Claves + (clave*sizeof(int)) ));

    return( (struct Info_Entra_Clave *)((char *) ((char *)m_info) + d ) );
}

int CInfoDef::k_campo(int clave, int comp,BOOL directo)
{
    struct Info_Entra_Clave *pclave = Clave(clave);

    if (!pclave->n_componentes) return -1;

    ASSERT(comp >= 0 && comp < pclave->n_componentes);
    int campo = (int)*((short *)((char *)(((char *)pclave) + pclave->d_componentes + sizeof(short)*comp)));
    if (!directo)
    {
		int i;
        for (i = 0;i < m_n_campos_col;i++)
        {
            if (m_colcampos[i] == campo)
            {
                campo = i;
                break;
            }
        }
        if (i >= m_n_campos_col)
        {
            campo = -1;
        }
    }
    return( campo );
}

int CInfoDef::k_orden(int clave, int comp)
{
    struct Info_Entra_Clave *pclave = Clave(clave);

    ASSERT(comp >= 0 && comp < pclave->n_componentes);

    return( (int)*((short *)((char *)(((char *)pclave) + pclave->d_orden + sizeof(short)*comp)))  );
}

int CInfoDef::EsInicialClave(int campo,BOOL directo,int laclave)
{
    
	if (!m_pTabla)
		return 0;

    ASSERT(campo >= 0 /*&& campo < m_info->n_campos*/);
 

    if (!directo)
    {
        campo = m_colcampos[ campo ];
    }

	int j,n;
	int ci,cf;
	if (laclave == -1)
	{
		ci = 0;
		cf = m_info->n_claves;
	}
	else
	{
		ci = laclave;
		cf = laclave+1;
	}
	int i;
    for (i = ci;i < cf;i++)
    {
		n = Clave(i)->n_componentes;
        if (!n) continue;
		if ((m_pTabla->m_gmodo & 0x80000))
		{
			ASSERT(m_pTabla->m_clavesvalidas != NULL);

			for (j = 0;j < m_pTabla->m_clavesvalidas[0];j++)
			{
				if (m_pTabla->m_clavesvalidas[j+1] == i)
					break;
			}
			if (j >= m_pTabla->m_clavesvalidas[0])
			{
				continue;
			}
		}
		for (j = 0;j < n;j++)
		{
			if (!directo && (m_pTabla->m_gmodo & 0x80000) && k_campo(i,j,FALSE) < 0)
			{
				continue;
			}
			if (campo == k_campo(i,j,TRUE))
			{
				if (!(m_pTabla->m_gmodo & 256) || m_info->clave_actual == i)
				{
					return i;
				}
			}
			break;
		}
    }
    return -1;
}

int CInfoDef::EsDeClave(int campo, BOOL directo)
{
	if (!m_pTabla)
		return 0;

    ASSERT(campo >= 0 && campo < m_info->n_campos);
 
    if (!directo)
    {
        campo = m_colcampos[ campo ];
    }

	int j,n,i;
    for (i = 0;i < m_info->n_claves;i++)
    {
		n = Clave(i)->n_componentes;
        if (!n) continue;
		for (j = 0;j < n;j++)
		{
			if (campo == k_campo(i,j,TRUE))
			{
				if (!(m_pTabla->m_gmodo & 256) || m_info->clave_actual == i)
				{
					return i;
				}
			}
		}
    }
    return -1;
}

int CInfoDef::EsDeLaClave(int campo, int clave,BOOL directo)
{

	if (!m_pTabla)
		return 0;

    ASSERT(campo >= 0 && campo < m_info->n_campos);
    
    if (!directo)
    {
        campo = m_colcampos[ campo ];
    }

    struct Info_Entra_Clave *pclave = Clave(clave);
	int i;
    for (i = 0;i < pclave->n_componentes;i++)
    {
        if (campo == (int)*((short *)((char *)(((char *)pclave) + pclave->d_componentes + sizeof(short)*i))))
        {
			if (!(m_pTabla->m_gmodo & 256) || m_info->clave_actual == clave)
			{
				return i;
			}
        }
    }
    return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CTablaView

CTablaView::CTablaView()
{
m_ajustadomarco = FALSE;
m_totbpedirdatos = FALSE;
m_mtecla = -1;
m_cuentaupdate = 0;
m_localizador= -1;
m_columnas   = 0;
m_apintar    = NULL;
m_pintarects = NULL;
m_flagpintar = TRUE;
m_pintando   = FALSE;
m_guardaposiciones = FALSE;
m_guarda_posicion = TRUE;
m_buffer     = NULL;
m_lenreg     = 0;
m_registros  = 0;
m_mayor_cache= 0;
m_menor_cache= 0;
m_repintar   = FALSE;
m_orderarray = NULL;
m_iniciales  = NULL;
m_edit       = NULL;
m_dsedit     = NULL;
pHdrImageList= NULL;
m_curcol     = 0;
m_tamcampos  = NULL;
m_poscampos  = NULL;
m_flagcampos = NULL;
m_gmodo      = 0;
m_ini_gmodo  = 0;
m_editando   = FALSE;
m_solofoco   = 0;
m_def = NULL;
m_cache.RemoveAll( );
m_confocus = FALSE;
m_NotificaOnDraw = 0;
m_ultpospadre = -1;
m_principal = NULL;
m_id = -1;
m_tw = NULL;
m_escampo = NULL;
m_noesconsulta = NULL;
m_okpredeterminado = FALSE;
m_clavesvalidas = NULL;
m_totalizadores = NULL;
m_focohijo = -1;
MAX_CACHE = 500;
MAX_ORDENACION_LOCAL=500;
MAXCOMPLETO = 5;
m_reordenado = -1;
m_reorden = NULL;
m_prevcursor = NULL;
m_cursorwaits = 0;
m_cacheado = FALSE;
m_primerapintada = TRUE;
m_sincachear = 0;
m_refrespending = FALSE;
m_yomismo = 0;
m_enclic = 0;
m_clicposiciona = 0;
m_enteclado = 0;
m_precursel = -1;
m_postposi = 0;
}

CTablaView::~CTablaView()
{

    BorraCache();

	if (::IsWindow(m_marcototal.m_hWnd))
	{
		m_marcototal.DestroyWindow();
	}

    if (m_apintar)
      delete[] m_apintar;
    if (m_pintarects)
        delete [] m_pintarects;
    if (m_def)
        delete m_def;

    if (m_tamcampos)
        delete [] m_tamcampos;
    if (m_poscampos)
        delete [] m_poscampos;
    
    if (m_orderarray)
        delete [] m_orderarray;
    if (m_iniciales)
        delete [] m_iniciales;

    if (m_flagcampos)
        delete [] m_flagcampos;

	if (m_clavesvalidas)
		delete [] m_clavesvalidas;		

	if (m_totalizadores)
		delete [] m_totalizadores;		

	int i;
	for (i = 0;i < 20;i++)
	{
		if (::IsWindow(m_totaledit[i].m_hWnd))
		{
			m_totaledit[i].DestroyWindow();
		}
	}
    if (m_buffer)
    {
        delete [] m_buffer;
        m_buffer = NULL;
    }

    if (m_edit)
        delete m_edit;

    if (m_dsedit)
        delete m_dsedit;

    if (pHdrImageList)
        delete pHdrImageList;    
}


BEGIN_MESSAGE_MAP(CTablaView, BASELISTCTRL)
	//{{AFX_MSG_MAP(CTablaView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdcachehint)
	ON_NOTIFY_REFLECT(LVN_ODFINDITEM, OnOdfinditem)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(HDN_BEGINDRAG,0, OnBegindrag)    
	ON_NOTIFY(HDN_BEGINTRACKW,0, OnBegintrack)
	ON_NOTIFY(HDN_ENDDRAG,0, OnEnddrag)
	ON_NOTIFY(HDN_ENDTRACKW,0, OnEndtrack)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY(HDN_ITEMCLICKW,0, OnItemclick)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_COMMAND(ID_EXPORTA_EXCEL, OnExportaExcel)
	ON_COMMAND(ID_EXPORTA_WORD, OnExportaWord)
	ON_COMMAND(ID_EXPORTA_PLB, OnExportaPlb)
	ON_COMMAND(ID_EXPORTA_RPT, OnExportaRpt)
	ON_COMMAND(ID_EXPORTA_XML, OnExportaXml)
    ON_NOTIFY(HDN_BEGINTRACK,0, OnBegintrack)
    ON_NOTIFY(HDN_ENDTRACK,0, OnEndtrack)
    ON_NOTIFY(HDN_ITEMCLICK,0, OnItemclick)
	ON_COMMAND(ID_GRIDIMPRIME, OnGridimprime)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	/*ON_WM_MEASUREITEM_REFLECT()*/
END_MESSAGE_MAP()

/*
	ON_NOTIFY_REFLECT(HDN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(HDN_BEGINTRACK, OnBegintrack)
	ON_NOTIFY_REFLECT(HDN_ENDDRAG, OnEnddrag)
	ON_NOTIFY_REFLECT(HDN_ENDTRACK, OnEndtrack)	
*/


/////////////////////////////////////////////////////////////////////////////
// CTablaView message handlers


void CTablaView::AjustaMarco()
{

	return;

	if (GetItemCount() < 1)
		return;

	int nitems = GetCountPerPage();
	int sizey;
	CRect rectview,rectitem,rectcontrol;
	GetWindowRect(rectcontrol);
	CHeaderCtrl *phdr = GetHeaderCtrl();
	if (!phdr)
		return;	

	m_ajustadomarco = TRUE;

	phdr->GetWindowRect(rectview);
	GetItemRect(0,rectitem,LVIR_BOUNDS);

	sizey = (rectcontrol.Size().cy /*- rectview.Size().cy*/) - (rectitem.Size().cy * nitems) - 3;

    if (GetScrollBarCtrl(SB_HORZ))
	{
		sizey += ::GetSystemMetrics(SM_CYHSCROLL);
	}	  

	rectcontrol.bottom -= sizey;

	GetParent()->ScreenToClient(rectcontrol);
	MoveWindow(rectcontrol);

	GetWindowRect(rectcontrol);
    GetParent()->GetParent()->ScreenToClient(rectcontrol);
	GetParent()->MoveWindow(rectcontrol);
}


void CTablaView::AjustaColumnaFinal()
{
	GetColumnOrderArray((LPINT)m_orderarray,m_columnas);
    if (COLUMNA_FINAL)
    {
        CRect rc;
        int i,t,top;

        GetClientRect(rc);        

        top = 0;
        t = 0;
        for (i = (m_columnas-COLUMNA_FINAL-1);i >= 0 && top < rc.Size().cx;i--)
        {
            t = GetColumnWidth(m_orderarray[i]);
            top += t;
        }
        if (top >= rc.Size().cx) top -= t;

        SetColumnWidth(m_columnas -1 ,(rc.Size().cx - top));
    }
}

void CTablaView::PonTotalRegistros(long registros)
{
	m_cacheado = FALSE;
	m_registros = registros;
	if (m_totalizadores)
		m_totbpedirdatos = TRUE;
    if ((m_gmodo & 1)/* || (m_gmodo & 0x100000)*/)
    {
        SetItemCountEx( m_registros + 1,LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
		if (m_cursel >= (m_registros+1))
		{
			/*m_cursel = m_registros;*/
			NuevoCorriente(m_registros,TRUE);
		}
    }
    else
    {
        SetItemCountEx( m_registros, LVSICF_NOINVALIDATEALL  | LVSICF_NOSCROLL);
		if (m_cursel >= m_registros)
		{
			/*m_cursel = m_registros-1;*/
			NuevoCorriente(m_registros-1,TRUE);
		}
    }	
	//AjustaMarco();
}

BOOL CTablaView::PrepareControl(CWnd *pariente, CRect &area,struct Info_Entra_Campos *Columnas,long registros,CFont *font,int localizador,long inicial,int g_modo,int id,int padre,CDsxView *principal,char *pextra)
{

    if (m_hWnd) return FALSE;	

    m_principal = principal;

	if (m_principal)
	{
		m_pDoc = m_principal->GetDocument();
	}

    m_id = id;

    m_padre = padre;
    m_gmodo = g_modo;
	m_ini_gmodo = m_gmodo;
    DWORD style =/*WS_CLIPSIBLINGS |*/ WS_CHILD | LVS_REPORT | /*LVS_SHOWSELALWAYS |*/ LVS_SINGLESEL | LVS_OWNERDATA;

    if (!Create(style , area , pariente , IDW_GRID + id))
	   return FALSE;

	ASSERT( m_hWnd );	

	PonWait();
	SetRedraw(FALSE);
	//LockWindowUpdate();


	if ((m_gmodo & 0x100000))
	{
		int n =	GetCountPerPage( );
		m_recttotal = area;
		area.bottom -= (area.Size().cy / n);
		MoveWindow(area);		
		m_recttotal.top = area.bottom;
		m_marcototal.Create(AfxRegisterWndClass(NULL),"",WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,m_recttotal,pariente,43124);
	}

    // BASELISTCTRL

    m_def = new CInfoDef(Columnas,this);    
	int aordenar = m_reordenado;

	if ((m_gmodo & 0x80000))
	{
		if (pextra)
		{
			int *p1 = (int *)pextra;
			int i;
			m_clavesvalidas = new int[p1[0]+1];
			for (i = 0;i <= p1[0];i++)
			{
				m_clavesvalidas[i] = p1[i];
			}
			pextra += ((p1[0]+1)*sizeof(int));
		}
		else
		{
			m_clavesvalidas = new int[2];
			m_clavesvalidas[0] = 1;
			m_clavesvalidas[1] = m_def->m_info->clave_defecto;
		}
	}

	if ((m_gmodo & 0x100000))
	{
		if (pextra)
		{
			int *p1 = (int *)pextra;
			int j;
			int i;
			m_totalizadores = new int[p1[0]+1];
			for (i = 0;i <= p1[0];i++)
			{
				if (i)
				{
					m_totalizadores[i] = -1;
					for (j = 0;j < m_def->m_n_campos_col;j++)
					{
						if (m_def->m_idcampos[p1[i]&0xffff] == m_def->m_colcampos[j])
						{
							m_totalizadores[i] = COLUMNA_INICIAL+j;
						}
					}
				}
				else
				{
					m_totalizadores[i] = p1[i];
				}
			}			
			pextra += ((p1[0]+1)*sizeof(int));
		}
	}


    m_localizador = localizador;

    if (m_def->m_usermode)
    {
       EnviaCamcolu();
    }
    
    if (font)
    {
	   SetFont(font);
    }

    /*| LVS_EX_CHECKBOXES*/
	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_INFOTIP | LVS_EX_FLATSB | LVS_EX_CHECKBOXES;
	
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles );
    
	PonTotalRegistros(registros);    

    SetBkColor( m_pDoc->m_cGridBk );    
	
    //m_cache.InitHashTable( 1200 );

    pHdrImageList = new CImageList();

    pHdrImageList->Create( IDB_CLAVE, 8, 0, RGB(192,192,192) );

    GetHeaderCtrl()->SetImageList( pHdrImageList );

    m_mayor_cache =  -1;
    m_menor_cache =  registros+1;	
    CreaColumnas();		
	if (!(m_gmodo & 1) && (m_registros * 2) < (MAX_CACHE + (MAX_CACHE * 100/20)))
	{
		if (!m_registros)
		{
			m_cache.InitHashTable( 257 );
		}
		else
		{
			m_cache.InitHashTable( (m_registros * 2) );
		}
	}
	else
	{
		m_cache.InitHashTable( MAX_CACHE + (MAX_CACHE * 100/20));
	}

	SetRedraw(TRUE);
	//UnlockWindowUpdate();

    if (inicial)
    {
       m_cursel = inicial - 1;
	   m_yomismo++;
       SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED );	   
	   m_yomismo--;
	   /*
       if (m_cursel > (GetCountPerPage( ) / 4))
       {
          CSize s;
          CRect rect;
          GetItemRect( m_cursel, &rect, LVIR_BOUNDS );
          s.cx = 0;
          s.cy = (m_cursel - (GetCountPerPage( ) / 4)) * rect.Size().cy;
          Scroll(s);
       }
	   */
    }	


    /*
	// Esto realmente es correcto pero para evitar problemas inicialmente 
	// siempre sera la columan inicial
    CRect r;        
    GetClientRect(r);
    int off = 0;
	int i;
    for (i = 0;i < m_curcol;i++)
    {            
        off += GetColumnWidth( i );
    }
    if ((off+GetColumnWidth( m_curcol )) > r.Size().cx)
    {
        CSize s;
        s.cx = off + 1;
        s.cy = 0;
        Scroll(s);
    }
	*/

	if (!(m_gmodo & 0x2000000))
		ShowWindow(SW_SHOW);

    NotificaPosicion();
    m_pDoc += (crt1-crt3)+crt2;
	if (!(m_gmodo & 0x2000000))
		EnsureVisible(m_cursel,TRUE);
	if ((m_gmodo & 0x2000000))
	{
		m_okpredeterminado = TRUE;
		PostMessage(WM_COMMAND,ID_AVISO_EDITAFILTRO);
	}
	else if (/*m_padre > -1 && */(m_gmodo & 0x40000))
	{
		m_okpredeterminado = TRUE;
		PostMessage(WM_COMMAND,ID_AVISO_GRIDRELLENA);
	}    
	else
	{
		if (!m_okpredeterminado && m_padre < 0)
		{			
			PostMessage(WM_COMMAND,ID_AVISO_PREDETERMINADO);			
		}
		m_okpredeterminado = TRUE;
	}	

	QuitaWait();

	if (aordenar > -1)
	{
		CambiarOrden((-aordenar) - 2);
	}		

	PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);

    return TRUE;
}

BOOL CTablaView::PonRegistros(int registros)
{
    int ant = m_registros;
	PonTotalRegistros(registros);
    if (!ant)
    {
        NuevoCorriente(0,FALSE);
        if ((m_gmodo & 0x10000) && m_padre > -1)
        {
            m_NotificaOnDraw |= 1;
        }
    }

    return TRUE;
}


void CTablaView::BorraCache(int from)
{
    POSITION pos = m_cache.GetStartPosition( );

	m_cacheado = FALSE;
	FinReorden();

    int item;
    char *data;

	m_mayor_cache =  -1;
	m_menor_cache =  m_registros+1;	

    while(pos)
    {
        data = NULL;
        m_cache.GetNextAssoc( pos , item, data );	
        if (data)
        {
			if (item > from)
			{
				m_cache.RemoveKey( item );
				delete [] data;
			}
			else
			{
				if (m_mayor_cache < item)
					m_mayor_cache = item;
				if (m_menor_cache > item)
					m_menor_cache = item;
			}
        }
    }
	if (from == -1)
	{
		m_cache.RemoveAll( );

		m_mayor_cache =  -1;
		m_menor_cache =  m_registros+1;	
	}

}


void CTablaView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;		
    
    /*
    if (!theApp.m_EntornoActivo) 
	   {
        *pResult = 1;
        return;
	   }        
    */
    
   
    if ((pDispInfo->item.mask & LVIF_TEXT))
    {                
        char *p;
        int item;
        int campo;
        
        if (pDispInfo->item.iSubItem < COLUMNA_INICIAL || pDispInfo->item.iSubItem >= (m_columnas-COLUMNA_FINAL))
        {
            strcpy(pDispInfo->item.pszText," ");
            *pResult = 0;
            return;
        }
        
        
        item = pDispInfo->item.iItem;        
		if (m_reordenado > -1 && m_reorden)
		{
			item = m_reorden[item];
		}
		campo = pDispInfo->item.iSubItem - COLUMNA_INICIAL;
        
        p = NULL;
        if (!m_cache.Lookup( item , p ))
        {
			Cacheamelo(item,item);
			if (!m_cache.Lookup( item , p ))
			{
				*pResult = 1;
				return;
			}            
        }
        //if (m_cache.Lookup( item , p ))
        {            
            pDispInfo->item.pszText[0] = ' ';
            if (m_tamcampos[campo] < 0 || m_tamcampos[campo] > 512)
            {
#ifdef _DEBUG
                DebugBreak();
#endif
            }
            memcpy(pDispInfo->item.pszText+1,p+m_poscampos[campo],m_tamcampos[campo]);

            pDispInfo->item.pszText[ m_tamcampos[campo] + 1 ] = 0;

			char *texto = pDispInfo->item.pszText;
            for (int o = 0;texto[o];o++)
            {
                oem_a_ansi((unsigned char *)texto+o);
            }

            *pResult = 0;
            return;
        }
        /*
        else
            DebugBreak();
        */
    }    
    
    if (pDispInfo->item.mask & LVIF_STATE)
    {
        if( m_cursel == pDispInfo->item.iItem )
        {
            pDispInfo->item.state = LVIS_SELECTED; ///* | LVIS_FOCUSED;
        }
        else
        {
            pDispInfo->item.state = 0;
        }        

    }
    
    *pResult = 0;
}


BOOL CTablaView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	   if ((wParam & 0xffff) == ID_GRIDENDLABELEDIT)
	   {
		    char tmp[512];
			//if( m_edit )
			{		
				CString str;
				if (lParam)
				{
					str = (char *)lParam;
					free((char *)lParam);
				}

				/*
				if( pDispInfo->item.pszText)		
					str = pDispInfo->item.pszText;        
				*/
				//BOOL vale = !m_edit->m_bESC;	

				{
					unsigned char *pp = (unsigned char *)str.GetBuffer(str.GetLength()+1);
					while(*pp)
					{
						ansi_a_oem(pp);
						pp++;
					}
					str.ReleaseBuffer(-1);
				}


				m_solofoco++;
				SetFocus();
				m_solofoco--;

				//m_edit = NULL;
				//m_editando = FALSE;


				if (m_def->Campo(m_curcol-COLUMNA_INICIAL)->c_tipo == 0)
				{
					CString qs;
					qs = m_def->c_maximo(m_curcol-COLUMNA_INICIAL);

					unsigned char relleno = ' ';      
					int i,c,maxqc;					

					maxqc = m_def->Campo(m_curcol-COLUMNA_INICIAL)->c_maxqc;

					i = qs.Find("\\");
					if (i >= 0)
					{
						i = (int)((unsigned char)qs[i+1]);
						if (i)
						{
							relleno = (unsigned char)i;
						}
					}
		      
					if (qs.Find("+") >= 0)
					{
						int dd;          			  
		          
						str.TrimLeft();
						str.TrimRight();
						strcpy(tmp,str);
						i = strlen(tmp);

						dd = maxqc-i;
						for (c = i;c >= 0;c--)
						{

							tmp[dd + c] = tmp[c];
						}
						for (c = 0;c < dd;c++)
							tmp[c] = relleno;

						str = tmp;
					}
					else
					{
						if (relleno != ' ')
						{
							str.TrimRight();
							strcpy(tmp,str);
							i = strlen(tmp);
							for (;i < maxqc;i++)
								tmp[i] = relleno;
							tmp[i] = 0;
							str = tmp;
						}
					}

					if (qs.Find(">") >= 0)
					{
						str.MakeUpper( );				  
					}
					else
					if (qs.Find("<") >= 0)
					{
						str.MakeLower( );				  
					}
				}
				else if (m_def->Campo(m_curcol-COLUMNA_INICIAL)->c_tipo != 5) {
					CString qi;
					qi = m_def->c_minimo(m_curcol-COLUMNA_INICIAL);
					CString qs;
					qs = m_def->c_maximo(m_curcol-COLUMNA_INICIAL);
					str.TrimLeft();
					str.TrimRight();
					int n = max(qi.GetLength(),qs.GetLength());
					int l = str.GetLength();
					char relleno = ' ';
					if (qi[0] == '0') {
						relleno = '0';
					}
					int i;
					for (i = 0;i < (n-l);i++) {
						tmp[i] = relleno;
					}
					tmp[i] = 0;
					str = tmp + str;
				}

				//if (vale)
				{
					if ((m_NotificaOnDraw & 4))
					{
						str = "\\\\"+str;
						m_NotificaOnDraw &= ~4;
					}

					int clave = -1; // falta invento para claves cruzadas!!!
					if ( (clave = m_def->EsInicialClave(m_curcol-COLUMNA_INICIAL)) > -1)
					{
						if (clave != m_def->m_info->clave_actual)
						{
							CambiarOrden(clave);
						}
					}
					else
					{
						if (str.Find("\\\\",0) < 0)
						{
							if (m_reordenado != m_curcol)
							{
								clave = -(m_curcol+2);
								CambiarOrden(clave);
							}
						}
					}

					if (m_reordenado > -1 && str.Find("\\\\",0) < 0)
					{
						//str = " " + str;
						str.TrimLeft();
						LocalizaRegistroOrdenado(str);				
					}
					else
					{
						EnviaDato(str);				
					}
				}
			}
			if ((m_NotificaOnDraw & 4))
			{
				m_NotificaOnDraw &= ~4;
			}    
			PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)2);
			/*NotificaPosicion();*/
			return TRUE;	
		} else if ((wParam & 0xffff) == ID_GRID_POSICION)
		{
		    NuevoCorriente(lParam);
			return TRUE;
		}
	    else if ((wParam & 0xffff) == ID_GRID_DELETE_OBJECT)
		{
			if (lParam)
			{
				CObject *pobj = (CObject *)lParam;
				delete pobj;
				m_solofoco++;
				SetFocus();
				m_solofoco--;
			}
			return TRUE;
		}
	    else if ((wParam & 0xffff) == ID_AVISO_INVALIDATE_GRID)
		{
            //AjustaMarco();
			ActualizaVentana();
			//InvalidateRect(NULL);
			return TRUE;
		}
		else if ((wParam & 0xffff) == ID_AVISO_POSTGRIDENSUREVISIBLE)
		{			
			if (lParam == 3)
			{
				TeclaStd(TECLA_PING);
			}
			else if (lParam == 2)
			{				
				HWND hwnd = ::GetFocus();
				if (hwnd)
					::BringWindowToTop(hwnd);
			}
			else if (lParam == 1)
			{
				if (m_tw)
					m_tw->BringWindowToTop();
			}
			else if (lParam == 0)
				EnsureVisible(m_cursel,TRUE);			
			return TRUE;
		}
		else if ((wParam & 0xffff) == ID_AVISO_POSTGRIDPOSICIONA)
        {
            CTablaW *pwpadre = NULL;            
            if (m_padre > -1)
            {
              CDsStdCtrl *pw;
              CDsStdDlg::m_todosctrl.Lookup(m_padre,pw);
              if (pw)
              {
                  pwpadre = (CTablaW *)pw->m_ctrl;
              }
            }

            if (pwpadre)
            {
                if (m_ultpospadre > -1)
                {
                    long reg = RegistroDelPadre(m_ultpospadre);
                    pwpadre->m_tabla.SendMessage(WM_COMMAND,ID_AVISO_GRIDPOSICIONA,(LPARAM)reg);
                }
                else
                {
                    pwpadre->m_tabla.SendMessage(WM_COMMAND,ID_AVISO_POSTGRIDPOSICIONA);
                }				
            }
			else
				NotificaPosicion();
            return TRUE;
        }
        else
        if ( (wParam & 0xffff) == ID_AVISO_GRIDPOSICIONA)
        {
            long tam;
            long respuesta;
            long modo = 1;
            long registro;
            int operacion = 4;
            char *p;
            char tmp[512];			

            registro = (long)lParam;

            p = tmp;

            memcpy((void *)p,(void *)&m_localizador,sizeof(int));
            p += sizeof(int);

            tam = (sizeof(long) * 6) + (sizeof(int)*2) + 100;
            respuesta = tam;

            memcpy((void *)p,(void *)&tam,sizeof(long));
            p += sizeof(long);	
            memcpy((void *)p,(void *)&respuesta,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&modo,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&operacion,sizeof(int));
            p += sizeof(int);
            memcpy((void *)p,(void *)&registro,sizeof(long));
            p += sizeof(long);
            
            *p = 0;                     
            DespachadorInterno(13,(void *)tmp);

            memcpy((void *)&operacion,p,sizeof(int));
            p += sizeof(int);
            if (operacion == 1)
            {
                BorraCache();
            }
            memcpy((void *)&registro,(void *)p,sizeof(long));
            p += sizeof(long);

			if (registro)
			{
				registro--;
				if (m_reordenado > -1 && m_reorden)
				{
					for (long i = 0;i < m_registros;i++)
					{
						if (registro == m_reorden[i])
						{
							registro = i;
							break;
						}
					}
				}
			}
			
            NuevoCorriente(registro,TRUE);
            EnsureVisible( registro, TRUE );			

            return TRUE;
        }
        else    
        if ( (wParam & 0xffff) == ID_AVISO_GRIDDEFAULTS )
        {
            EnviaCamcolu();
            return TRUE;
        }
        else
        if ( (wParam & 0xffff) == ID_AVISO_GRIDRELLENA )
        {
            CTablaW *pwpadre = NULL;
            if (m_padre > -1)
            {
              CDsStdCtrl *pw = NULL;              
              if (CDsStdDlg::m_todosctrl.Lookup(m_padre,pw) && pw)
              {
                  pwpadre = (CTablaW *)pw->m_ctrl;
              }
            }
			else
			   pwpadre = m_tw;
            if (pwpadre && (m_gmodo & 0x40000))
            {				
                CDlgGridRellena *prellena = new CDlgGridRellena(pwpadre);
                prellena->CenterWindow(pwpadre);
                prellena->ShowWindow(SW_SHOW);
				

                SetFocus();

                long tam;
                long respuesta;
                long modo = 1;
                //long registro;
                int operacion = 5;
                long registro;
                char *p;
                char tmp[512];

				//pwpadre->m_tabla.m_focohijo = m_id;

                //registro = (long)lParam;

                p = tmp;

                memcpy((void *)p,(void *)&m_localizador,sizeof(int));
                p += sizeof(int);

                tam = (sizeof(long) * 10) + (sizeof(int)*2) + 100;
                respuesta = tam;

                memcpy((void *)p,(void *)&tam,sizeof(long));
                p += sizeof(long);	
                memcpy((void *)p,(void *)&respuesta,sizeof(long));
                p += sizeof(long);
                memcpy((void *)p,(void *)&modo,sizeof(long));
                p += sizeof(long);
                memcpy((void *)p,(void *)&operacion,sizeof(int));
                p += sizeof(int);
                *p = 0;                     
                DespachadorInterno(13,(void *)tmp);
                
                memcpy((void *)&registro,(void *)p,sizeof(long));
                p += sizeof(long);
				
                if (registro)
                {
					SetRedraw(FALSE);
					//LockWindowUpdate();
					BorraCache();
                    PonRegistros(registro);
					SetRedraw(TRUE);
					//UnlockWindowUpdate();
					ActualizaVentana();
                    //InvalidateRect(NULL);
                }
				
				
				// SetFocus();

				//pwpadre->m_tabla.m_focohijo = -1;

				prellena->EndDialog(IDOK);
                delete prellena;

				if (!m_registros)
				{
					AfxMessageBox("No hay registros",MB_OK | MB_ICONHAND);
					if ((m_gmodo & 0x10000))
					{
						m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
					}
					else 
					{
						TeclaStd(806);
					}
				}
				
				PostMessage(WM_COMMAND,ID_AVISO_POSTGRIDENSUREVISIBLE,(LPARAM)1);
            }
            return TRUE;
        }
        else
        if ( (wParam & 0xffff) == ID_AVISO_GRIDLOCALEDITA )
        {
            EdicionLocal(0,0,0);
            return TRUE;
        }
        else
        if ( (wParam & 0xffff) == ID_AVISO_PREDETERMINADO )
        {
            Filtros(TRUE);
            return TRUE;
        }
        else
        if ( (wParam & 0xffff) == ID_AVISO_EDITAFILTRO )
        {
            Filtros(FALSE);
			GetParent()->PostMessage(WM_CLOSE,0,0);
            return TRUE;
        }
		else if ( (wParam & 0xffff) == ID_GRIDNUEVACOLUMNA)
		{
			
			CambiaCorriente(((int)lParam)-1,0,TRUE);
			if (!NotificaPosicion())
			{
				if ((m_gmodo & 0x10000) && m_padre > -1)
					m_NotificaOnDraw |= 1;				
				else /*if (!(m_gmodo & 0x1000000))*/
					m_NotificaOnDraw |= 8;
			}
			return TRUE;
		}
        else if ( (wParam & 0xffff) == ID_NUEVAPOSICIONGRID )
        {			
			if ((int)lParam == 2)
			{
				if (!NotificaPosicion())
				{
					if ((m_gmodo & 0x10000) && m_padre > -1)
						m_NotificaOnDraw |= 1;				
					else /*if (!(m_gmodo & 0x1000000))*/
						m_NotificaOnDraw |= 8;
				}
			}
			else
			{
				m_postposi--;
				NuevoCorriente(-1,(BOOL)lParam);
			}			
            return TRUE;
        }		
		else if ( (wParam & 0xffff) == ID_AVISO_DOUBLECLICK )
		{
			if (m_editando)
			{
				theApp.ActivacionPorDefecto();
				return TRUE;
			}
		 
			{				
				if (DobleClick())
				{					
					return TRUE;
				}				
			}

			if (!(m_gmodo & 0x20000))
			{        
				if (!(m_gmodo & 1))
				{
					if (!(m_gmodo & 32))
					{
						TeclaStd(802);        
					}
				}
				else
				{
					MantenimientoLinea();
				}
			}
			else
			{       
				SendMessage(WM_COMMAND,ID_AVISO_POSTGRIDPOSICIONA);
				if (m_principal)
				{
				m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
				}        
			}
			return TRUE;
		}
		else if ((wParam & 0xffff) == ID_COMMANDCONTROL)
		{
			char *p = (char *)lParam;
			char *p1;
			int col;
			LVCOLUMN colum;
			if (!memcmp(p,"COLN:",5))
			{
				p += 5;
				p1 = p;
				while(*p1 && *p1 != ',') p1++;
				if (*p1 == ',')
				{
					*p1 = 0;
					col = atoi(p);
					*p1 = ',';
					p = p1 + 1;

					memset((void *)&colum,0,sizeof(colum));
					colum.mask = LVCF_SUBITEM | LVCF_TEXT;
					colum.iSubItem = col;
                    for (int o = 0;p[o];o++)
                    {
                        oem_a_ansi((unsigned char *)p+o);
                    }
					colum.pszText = p;
					colum.cchTextMax = strlen(p)+1;
					SetColumn( col , &colum );
				}				
			}
			return TRUE;
		}

        return BASELISTCTRL::OnCommand(wParam, lParam);
}


void CTablaView::OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult) 
{
    static int c_recursion = 0;

/*
    if (c_recursion) 
    {
        *pResult = 0;
        return;
    }
*/

	{
		long nr = GetItemCount();
		if (nr != m_registros) {
           nr = m_registros;
		}
	}
    c_recursion++;

	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;


    if (!::IsWindow(pCacheHint->hdr.hwndFrom))
    {
        return;
    }


    //if (c_recursion < 2)
	   
	   //**** PonWait();
	   if (pCacheHint->iTo > m_registros)
	   {
		   *pResult = 1;
	   }
	   else
	   {
		   if (!Cacheamelo(pCacheHint->iFrom,pCacheHint->iTo))
		   {
			   *pResult = 1;
		   }
		   else
		   {
			   *pResult = 0;
		   }
	   }
	   //**** QuitaWait();
	   /*
    else
    {
        ASSERT(FALSE);
    }
	*/


	*pResult = 0;
    c_recursion--;
}


BOOL CTablaView::Cacheamelo(int from, int to)
{
    int desde_campo = 0;
    int hasta_campo = m_columnas - 1 - COLUMNA_FINAL - COLUMNA_INICIAL;
    int operacion = 0,hasta;
    long tam;
    long respuesta;
    long modo = 1;
    long registro,f_,t_,c_;
	long totenvio;
    char *p;
	char *p1;
    char *data;
	char *p2;
	char *p3;
    int prime;
    int pagi;	
	long totalc;
	long tregistros = GetItemCount( );
	BOOL a_pintando = m_pintando;
	BOOL mododetallado = FALSE;
	long nuevasel = -1;
	BOOL bajatodo = FALSE;
	BOOL ignoracache = FALSE;
	BOOL caso98 = FALSE;
	static int recursion = 0;


	if (m_sincachear)
	{
		m_refrespending = TRUE;
		return TRUE;
	}

	if (!m_buffer)
		return TRUE;

	if (from == -1 && to == -1)
	{
		from = 0;
		to = m_registros;
		bajatodo = TRUE;
	}
	if (m_cacheado && from != to)
		return TRUE;

	if (recursion)
		return FALSE;
	recursion++;

	
	if (from == to && !bajatodo)
		mododetallado = TRUE;
	

    m_pintando = TRUE;
	//LockWindowUpdate();
	pagi  = GetCountPerPage();

	if (m_mayor_cache > -1 && (m_gmodo & 128) && !(m_gmodo & 0x100000) && from > m_mayor_cache)
	{
		if (from <= (m_mayor_cache+pagi))
		{
			from = m_mayor_cache;
			if (from < 0)
				from = 0;		
		}
	}
	

    int total = m_cache.GetCount( );
    int i,d1,d2;
    if (total > MAX_CACHE)
    {        
       d1 = from;
       d2 = m_registros - to;
       if (d2 > d1)
       {
           while(total > MAX_CACHE)
           {
               if (m_cache.Lookup( m_mayor_cache , p ))
               {
                   delete [] p;
                   m_cache.RemoveKey(m_mayor_cache);
               }
               m_mayor_cache--;
               total--;
           }
       }
       else
       {
           while(total > MAX_CACHE)
           {
               if (m_cache.Lookup( m_menor_cache , p ))
               {
                   delete [] p;
                   m_cache.RemoveKey(m_menor_cache);
               }
               m_menor_cache++;
               total--;
           }
       }
    }	
    
	if (mododetallado)	
		prime = from;	
	else
	{
        prime = GetTopIndex();
		if (prime < (from-pagi))
		{
			prime = from;
		}
	}

    if (prime < from)
        f_ = (long)prime;
    else
        f_ = (long)from;    

	
    if (!mododetallado && (prime+pagi) > to)
        t_ = (long)(prime+pagi);
    else	
        t_ = (long)to;

	if (t_ >= m_registros)
	{
		if ((m_gmodo&1))
		    t_ = m_registros;
		else
			t_ = m_registros-1;
	}

	/* Intentar Modo Completo */

	totenvio = (t_ - f_) + 1;

	if (totenvio == 1&& !bajatodo)
		mododetallado = TRUE;

	if (!mododetallado)
	{
		BOOL actualizar = FALSE;
		i = f_;
		while(i <= t_ && !m_cacheado /*&& i <= m_registros*/)
		{
			if (ignoracache)
			{				
				hasta = i + MAXCOMPLETO;
				if (hasta > t_)
					hasta = t_;
			}
			else
			{
				while(i <= t_)
				{
					if (!m_cache.Lookup( i , p ))
						break;
					i++;
				}
				if (i > t_) {
					hasta = i;
					break;
				}
				hasta = i + 1;
				while(hasta < (i + MAXCOMPLETO) && hasta <= t_)
				{
					if (m_cache.Lookup( hasta , p ))
					{
						hasta--;
						break;
					}
					hasta++;
				}
				if (hasta < i)
				{
					break;
				}
                if (hasta > t_)
					hasta = t_;
			}

			p = m_buffer;

			registro = (unsigned long)0xFFFFFFFE;

			tam = (sizeof(long) * 15) + (sizeof(int)*3) + 2;
			respuesta = tam + ((m_lenreg+6+sizeof(long)*2) * ((hasta-i)+1));

			memcpy((void *)p,(void *)&m_localizador,sizeof(int));
			p += sizeof(int);        

			memcpy((void *)p,(void *)&tam,sizeof(long));
			p += sizeof(long);	
			memcpy((void *)p,(void *)&respuesta,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&modo,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&operacion,sizeof(int));
			p += sizeof(long);
			memcpy((void *)p,(void *)&registro,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&desde_campo,sizeof(int));
			p += sizeof(int);
			memcpy((void *)p,(void *)&hasta_campo,sizeof(int));
			p += sizeof(int);
			p1 = p;
			memcpy((void *)p,(void *)&i,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&hasta,sizeof(long));
			p += sizeof(long);

			c_ = (long)m_cursel + 1;
			memcpy((void *)p,(void *)&c_,sizeof(long));
			p += sizeof(long);

			*p = 0;
			DespachadorInterno(13,(void *)m_buffer);

			caso98 = FALSE;
			if (*p >= 100)
			{
				*p -= 100;
				if (!bajatodo)
				{
					BorraCache();
					actualizar = TRUE;				
					caso98 = TRUE;
				}
			}


			if (*p == 99)
			{
				mododetallado = TRUE;
				break;
			}

			if (*p == 1)
			{				
				BorraCache();				
				actualizar = TRUE;				
			}
			else if (*p == 2)
			{				
				BorraCache();
				actualizar = TRUE;				
			}
			else if (*p == 3)
			{			
				PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);				
			}
			else if (*p == 4)
			{	
				/*
				BorraCache(from);				
				actualizar = TRUE;
				*/
				PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);
			}

			p++; /* salto del indicador de estado */


			memcpy((void *)&i,(void *)p1,sizeof(long));
			p1 += sizeof(long);
			memcpy((void *)&hasta,(void *)p1,sizeof(long));		


			totalc = m_cache.GetCount();
			for(;i <= hasta;i++)
			{
				data = new char[ m_lenreg + 1 + sizeof(long)*2 + 6];
				memcpy(data,p,m_lenreg+sizeof(long)*2 + 6);
				p += (m_lenreg+(sizeof(long)*2)+6);

				if (ignoracache)
				{
					p3 = NULL;
					if (m_cache.Lookup( i , p3))
					{
						delete p3;
					}
				}
				
				/*
				if (i && !memcmp(data,"            ",10))
				{
					p3 = p3;
				}
				*/
				m_cache[i] = data;
				if (i < m_menor_cache)
				{
					m_menor_cache = i;
				}
				if (i > m_mayor_cache)
				{
					m_mayor_cache = i;
				}

				totalc = m_cache.GetCount();
				{
					if (totalc > MAX_CACHE)
					{
						int j1,j2;
						j1 = i - m_menor_cache;
						j2 = m_mayor_cache - i;
						if (j1 >= j2)
						{
							j1 = m_menor_cache;
							m_menor_cache++;					
						}
						else
						{
							j1 = m_mayor_cache;
							m_mayor_cache--;
						}
						if (m_cache.Lookup( j1 , p2 ))
						{
						   delete [] p2;
						   m_cache.RemoveKey(j1);
						}
					}
				}
			}

			nuevasel = *((long *) (p+sizeof(long)) );
			if (!nuevasel)
				nuevasel = 1;
			if ((nuevasel-1) != m_cursel)
			{
				BOOL posicionalo = FALSE;
				if (nuevasel > *((long *) (p) ))
				{
					nuevasel = *((long *) (p) );
				}

				if (bajatodo)
				{
					m_cursel = nuevasel - 1;
				}
				else
				{
					
					if (m_cursel >= from && m_cursel <= to)
   						posicionalo = TRUE;

					m_cursel = nuevasel-1;
					m_postposi++;
					PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)posicionalo);
					/*NuevoCorriente( nuevasel-1, posicionalo);*/
				}				
			}

			if (*((long *) (p) ) != m_registros)
			{
				if (*((long *) (p) ) < 0 )
				{
					m_pintando = a_pintando;
					BorraCache();					
					ActualizaVentana();

					recursion--;

					return FALSE;
				}
				else
				{					
					actualizar = TRUE;
					PonTotalRegistros(*((long *)(p) ));

					BorraCache(m_registros);
					
					tregistros = GetItemCount( );
				}
				if (f_ >= m_registros || t_ >= m_registros)
				{
					caso98 = TRUE;
					break;
				}
			}

			if (f_ >= m_registros || t_ >= m_registros)
			{
				break;
			}

			if (tregistros && totalc >= tregistros)
			{
				m_cacheado = TRUE;
				break;
			}
		}
		if (actualizar)
		{			
			ActualizaVentana();
			actualizar = FALSE;
		}
	}

	if (mododetallado)
	{
		tam = (sizeof(long) * 15) + (sizeof(int)*3) + 1 + /*m_lenreg + */1;
		respuesta = tam + m_lenreg + sizeof(long)*2 + 6;

		for (i = from;i <= to;i++)
		{

			if (m_cache.Lookup( i , p )) continue;

			p = m_buffer;
			registro = i+1;
        
			memcpy((void *)p,(void *)&m_localizador,sizeof(int));
			p += sizeof(int);        
        
			memcpy((void *)p,(void *)&tam,sizeof(long));
			p += sizeof(long);	
			memcpy((void *)p,(void *)&respuesta,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&modo,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&operacion,sizeof(int));
			p += sizeof(long);
			memcpy((void *)p,(void *)&registro,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&desde_campo,sizeof(int));
			p += sizeof(int);
			memcpy((void *)p,(void *)&hasta_campo,sizeof(int));
			p += sizeof(int);
			memcpy((void *)p,(void *)&f_,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&t_,sizeof(long));
			p += sizeof(long);

			c_ = (long)m_cursel + 1;
			memcpy((void *)p,(void *)&c_,sizeof(long));
			p += sizeof(long);


			*p = 0;
			DespachadorInterno(13,(void *)m_buffer);

			caso98 = FALSE;

			if (*p >= 100)
			{
				*p -= 100;
				if (!bajatodo)
				{
					BorraCache();
					caso98 = TRUE;				
				}
			}

			if (*p == 99) // Error
			{
				break; /* !!!!! */
			}


			{
				long nc = *((long *) (p+m_lenreg+sizeof(long)*3+1+6) );
				BOOL posicionalo = FALSE;           
				
				if ((nc-1) != m_cursel)
				{
					if (nc > *((long *) (p+m_lenreg+sizeof(long)*2+1+6) ))
						nc = *((long *) (p+m_lenreg+sizeof(long)*2+1+6) );

					if (m_cursel >= from && m_cursel <= to)
						posicionalo = TRUE;	
					
					m_cursel = nuevasel-1;
					m_postposi++;
					PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)posicionalo);					

					/*NuevoCorriente( nc -1, posicionalo);*/
				}
			}
			
			if (*((long *) (p+m_lenreg+sizeof(long)*2+1+6) ) != m_registros)
			{
				if (*((long *) (p+m_lenreg+sizeof(long)*2+1+6) ) < 0 )
				{
					*p = 2;
				}
				else
				{
					PonTotalRegistros(*((long *) (p+m_lenreg+sizeof(long)*2+1+6) ));
					tregistros = GetItemCount( );
				}
			}
        

			if (*p == 1)
			{
				long nc = *((long *) (p+m_lenreg+sizeof(long)*3+1+6) );
				long dif;			
				BOOL posicionalo = FALSE;
            

				/*
				if (m_cursel >= from && m_cursel <= to)
					posicionalo = TRUE;			
				*/
				if (!nc)
				{
					nc = 1;
				}
				if (nc > m_registros)
					nc = m_registros;
				dif = nc - m_cursel;			

				from += dif;
				if (from < 0) from = 0;
				to += dif;
				if (to >= m_registros) to = m_registros - 1;

				f_ += dif;
				t_ += dif;

				m_pintando = a_pintando;
				BorraCache();            

				m_cursel = nuevasel-1;
				m_postposi++;
				PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)posicionalo);					

				/*NuevoCorriente( nc -1, posicionalo);*/

				//PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,nc);

				ActualizaVentana();

				//InvalidateRect(NULL);//Puede provocar recursion!!!


				/*
				m_cursel -= registro;
				// en teoria no deberia cambiar el corriente
				SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED);            
				EnsureVisible( 0, TRUE );            
				*/
				i = from - 1;
				continue;            
			}
			else if (*p == 2)
			{
				m_pintando = a_pintando;
				BorraCache();

				/*
				m_cursel += (m_registros - registro);
				// en teoria no deberia cambiar el corriente
				SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED);
            
				EnsureVisible( m_registros-1, TRUE );            
				*/
				//PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);
				ActualizaVentana();
				//InvalidateRect(NULL);//Puede provocar recursion!!!
            
				//UnlockWindowUpdate();
				recursion--;
				return FALSE;
			}
			else if (*p == 3)
			{			
				PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);
				break;
			}
			else if (*p == 4)
			{
				/*
				m_pintando = a_pintando;
				BorraCache();
				*/
				PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);
				//UnlockWindowUpdate();
				/*
				recursion--;
				return FALSE;
				*/
			}
			p++;

			data = new char[ m_lenreg + 1 + sizeof(long)*2 + 6];
			memcpy(data,p,m_lenreg+(sizeof(long)*2)+6);
			/*
			if (i && !memcmp(data,"           ",10))
			{
				p3 = p3;
			}
			*/
			m_cache[i] = data;
			if (i < m_menor_cache)
			{
				m_menor_cache = i;
			}
			if (i > m_mayor_cache)
			{
				m_mayor_cache = i;
			}			

			totalc = m_cache.GetCount();
			if (totalc == tregistros)
			{
				m_cacheado = TRUE;
				break;
			}
			else
			{
				if (totalc > MAX_CACHE)
				{
					int j1,j2;
					j1 = i - m_menor_cache;
					j2 = m_mayor_cache - i;
					if (j1 >= j2)
					{
						j1 = m_menor_cache;
						m_menor_cache++;					
					}
					else
					{
						j1 = m_mayor_cache;
						m_mayor_cache--;
					}
					if (m_cache.Lookup( j1 , p2 ))
					{
					   delete [] p2;
					   m_cache.RemoveKey(j1);
					}
				}
			}		
		}
	}

	if (caso98)
	{
		PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);
		/*ActualizaVentana();*/
	}
    m_pintando = a_pintando;

	//UnlockWindowUpdate();

    recursion--;
    return TRUE;
}

void CTablaView::OnOdfinditem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;
	
	*pResult = -1;
}

void CTablaView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{    
    static int o_recursion = 0;

	NMLVCUSTOMDRAW *pCD = (NMLVCUSTOMDRAW*)pNMHDR;    
    
	
    if (o_recursion > 1)
    {
	
		/*
        *pResult = CDRF_SKIPDEFAULT;
        m_repintar = TRUE;		
        return;
		*/	
    }
	
	/*
	if (m_pintando)
	{
		m_pintando = m_pintando ;
	}
	*/
	

	//EnableWindow(FALSE);
    m_pintando = TRUE;
	
    o_recursion++;
	*pResult = 0; 
	switch( pCD->nmcd.dwDrawStage )
		{
		case  CDDS_PREERASE:
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case  CDDS_PREPAINT:
            if (o_recursion > 1)
            {
                *pResult = CDRF_NOTIFYITEMDRAW;
            }
            else
            if (m_apintar && m_flagpintar)
            { 			  
                int i,j,hh;
                CRect rect,rc;                
                int offset;                
                int iItem = GetTopIndex( );                               
				
                GetClientRect(rc);

                if (m_tw)
                {
                    CRect re2;
                    j = GetItemCount( ) - iItem;
                    for (i = 0;i < j;i++)
                    {
                        GetItemRect( iItem+i, &re2, LVIR_BOUNDS );

                        if (re2.top >= rc.bottom) break;

                        m_tw->AseguraBoton(i);
						if ((iItem+i) >= m_registros && (m_gmodo & 1))
						{
							m_tw->BotonPintado(i,1);
						}
                    }
                    if (m_cursel < iItem || m_cursel >= (iItem+i))
                    {
                        m_tw->BotonPintado(-1,0);
                    }
					m_tw->AseguraBoton(-(i+1));
                }
                
                GetColumnOrderArray((LPINT)m_orderarray,m_columnas);

                GetItemRect( iItem, &rect, LVIR_BOUNDS );                
                
                offset = rect.left;
                j = 0;
				int kp;
				CRect rectt;
                for (i = 0;i < m_columnas;i++)
                { 
                    hh = GetColumnWidth( m_orderarray[i] );
                    rect.left  = offset;
                    rect.right = rect.left + hh;
                    m_pintarects[i] = rect;

					if ((m_gmodo & 0x100000))
					{
						for (kp = 1;kp <= m_totalizadores[0];kp++)
						{
							if (m_totalizadores[kp] == m_orderarray[i])
							{
								rectt = rect;
								rect.top = 0;
								rect.bottom = m_recttotal.Size().cy;
								if (!::IsWindow(m_totaledit[kp-1].m_hWnd))
								{
									m_totaledit[kp-1].Create(ES_READONLY|ES_RIGHT|/*WS_BORDER|*/WS_CHILD|WS_VISIBLE, rect, &m_marcototal, ID_TRAZASINBOLFIN );
									m_totbpedirdatos = TRUE;
								}
								else
								{
									m_totaledit[kp-1].MoveWindow(rect);
								}
							}
						}
					}
                    
                    if ((rect.left >= rc.left && rect.left <= rc.right) || (rect.right >= rc.left && rect.right <= rc.right))
                    {
                        /*if (j < 3)*/ // todos!!!
                        {                           
                           if (hh > (rc.Size().cx-1))
                           {
                               SetColumnWidth( m_orderarray[ i ] , rc.Size().cx-1);
                               hh = GetColumnWidth( m_orderarray[i] );
                               rect.right = rect.left + hh;
                               m_pintarects[i] = rect;
                               AjustaColumnaFinal();
                           }
                        }
                        m_apintar[j] = i;
                        j++;
                    }
                    offset = offset + hh;
                }
                m_apintar[j] = -1;     
				if (m_totbpedirdatos) // Solo totales!!!
				{					
					char texto[512];
					LV_DISPINFO DispInfo;
					LRESULT result;
					Cacheamelo(m_registros,m_registros);
                    DispInfo.item.pszText = texto;
                    DispInfo.item.mask = LVIF_TEXT;    
                    DispInfo.item.iItem = m_registros;
					for (kp = 1;kp <= m_totalizadores[0];kp++)
					{						
						if (m_totalizadores[kp] > -1 && ::IsWindow(m_totaledit[kp-1].m_hWnd))
						{
							DispInfo.item.iSubItem = m_totalizadores[kp];
							OnGetdispinfo((NMHDR*)&DispInfo,&result);
							m_totaledit[kp-1].SetWindowText(DispInfo.item.pszText);
						}
					}
					m_totbpedirdatos = FALSE;
				}
                *pResult = CDRF_NOTIFYITEMDRAW;
            }
            else
            {
                *pResult = CDRF_SKIPDEFAULT;
            }			
			break;
		case  CDDS_ITEMPREPAINT:
            {
				long flags;

                int iItem=(int)pCD->nmcd.dwItemSpec;
				char *px = NULL;
				COLORREF ccf,cct;
				unsigned char rgbc[6];
				unsigned char rgbc0[6];
				memset(rgbc0,0,6);			    


                {                    
					int oitem = iItem;
					if (m_reordenado > -1 && m_reorden)
					{
						oitem = m_reorden[oitem];
					}
                    if (!m_cache.Lookup( oitem , px )) 
                    {
                        *pResult = CDRF_SKIPDEFAULT;
                        break;
                         //Cacheamelo(iItem,iItem);
                    }
					memcpy(rgbc,(void *)(px+m_lenreg+sizeof(long)*2),6);
                }
				cct = RGB(rgbc[0],rgbc[1],rgbc[2]);
				ccf = RGB(rgbc[3],rgbc[4],rgbc[5]);

                CDC* pDC = new CDC();
                pDC->Attach(pCD->nmcd.hdc);
                    //CDC::FromHandle(pCD->nmcd.hdc);
                
                LV_DISPINFO DispInfo;
                LRESULT result;
                char texto[512];
                CRect rect;
                CRect r2;
                CRect rc;

                GetClientRect(r2);
                
                DispInfo.item.pszText = texto;
                DispInfo.item.mask = LVIF_TEXT;
                
    
                DispInfo.item.iItem = iItem;                 
                
                GetItemRect( iItem, rc , LVIR_BOUNDS );
                rc.left = r2.left;
                rc.right = r2.right;

				memcpy((void *)&flags,(void *)(px+m_lenreg+sizeof(long)),sizeof(long));
                
                if( m_cursel == iItem)
                {
					if (!m_postposi)
					{
						m_yomismo++;
						SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED );
						m_yomismo--;
					}


					if (m_pDoc->m_cGridSelBk == m_pDoc->m_cGridBk && m_pDoc->m_cGridFr == m_pDoc->m_cGridFr)
					{						
						COLORREF  cbk = m_pDoc->m_cGridSelBk;
						if ((flags & 1)) /* color especial */
						{
							if ( ((cbk & 0x00FF0000) < 0x00800000) )
							{
								cbk += 0x00800000;
							}
							else
							{
								cbk -= 0x00800000;
							}
							if ( ((cbk & 0x0000FF00) < 0x00008000) )
							{
								cbk += 0x00008000;
							}
							else
							{
								cbk -= 0x00008000;
							}
							if ( ((cbk & 0x000000FF) < 0x00000080) )
							{
								cbk += 0x00000080;
							}
							else
							{
								cbk -= 0x00000080;
							}							
						}
						else if ((iItem%2))
						{
							if ( ((cbk & 0x00FF0000) < 0x00EE0000) )
							{
								cbk += 0x00120000;
							}
							else
							{
								cbk -= 0x00120000;
							}
							if ( ((cbk & 0x0000FF00) < 0x0000EE00) )
							{
								cbk += 0x00001200;
							}
							else
							{
								cbk -= 0x00001200;
							}
							if ( ((cbk & 0x000000FF) < 0x000000EE) )
							{
								cbk += 0x00000012;
							}
							else
							{
								cbk -= 0x00000012;
							}							
						}    
						pDC->SetTextColor( m_pDoc->m_cGridSelFr ); //
						pDC->SetBkColor(cbk);
						pDC->FillSolidRect( rc, cbk );
						//pDC->FillRect(rc, &CBrush(cbk));
					}                    
					else
					{
						pDC->SetTextColor( m_pDoc->m_cGridSelFr );
						pDC->SetBkColor(m_pDoc->m_cGridSelBk );
						pDC->FillSolidRect( rc, m_pDoc->m_cGridSelBk);
						//pDC->FillRect(rc, m_pDoc->m_bGridSelBk ); //
					}

                    if ((m_NotificaOnDraw & 1))
                    {
                        char *p2;
                        long reg;
                        p2 = NULL;
						int oitem = m_cursel;
					    if (m_reordenado > -1 && m_reorden)
						{
						    oitem = m_reorden[oitem];
						}
                        if (m_cache.Lookup( oitem, p2 ) && p2)
                        {
                            CTablaW *pwpadre = NULL;
                            if (m_padre > -1)
                            {
                              CDsStdCtrl *pw;
                              CDsStdDlg::m_todosctrl.Lookup(m_padre,pw);
                              if (pw)
                              {
                                  pwpadre = (CTablaW *)pw->m_ctrl;
                              }
                            }

                            if ( (m_gmodo & 0x10000) && pwpadre )
                            {
                               memcpy((void *)&reg,(void *)(p2+m_lenreg),sizeof(long));
                               if (  !(pwpadre->m_gmodo & 1))
                               {
                                   reg = RegistroDelPadre(reg);
                                   pwpadre->m_tabla.PostMessage(WM_COMMAND,ID_AVISO_GRIDPOSICIONA,(LPARAM)reg);
                               }
                               else
                               {
                                   m_ultpospadre = reg;
                               }
                            }
                        }
                        m_NotificaOnDraw &= ~1;
                    }
                    if ((m_NotificaOnDraw & 2))
                    {
                        PostMessage(WM_COMMAND,ID_AVISO_GRIDLOCALEDITA);
                        m_NotificaOnDraw &= ~2;
                    }
                    if ((m_NotificaOnDraw & 8))
                    {
						m_NotificaOnDraw &= ~8;
						PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)2);
						//NotificaPosicion();						
					}
                }
                else
                {                    
                    pDC->SetTextColor( m_pDoc->m_cGridFr );
                    
                    COLORREF  cbk = m_pDoc->m_cGridBk;
					if ((flags & 1)) /* color especial */
					{
						if ((flags & 1)) /* color especial */
						{
							if ( ((cbk & 0x00FF0000) < 0x00800000) )
							{
								cbk += 0x00800000;
							}
							else
							{
								cbk -= 0x00800000;
							}
							if ( ((cbk & 0x0000FF00) < 0x00008000) )
							{
								cbk += 0x00008000;
							}
							else
							{
								cbk -= 0x00008000;
							}
							if ( ((cbk & 0x000000FF) < 0x00000080) )
							{
								cbk += 0x00000080;
							}
							else
							{
								cbk -= 0x00000080;
							}							
						}
					}
					else if ((m_gmodo & 0x100000) && iItem >= m_registros)
					{						
                        if ( ((cbk & 0x00FF0000) < 0x00DD0000) )
                        {
                            cbk += 0x00220000;
                        }
						else
						{
							cbk -= 0x00220000;
						}						
                        if ( ((cbk & 0x0000FF00) < 0x0000DD00) )
                        {
                            cbk += 0x00002200;
                        }
						else
						{
							cbk -= 0x00002200;
						}
						
						/*
                        if ( ((cbk & 0x000000FF) < 0x000000DD) )
                        {
                            cbk += 0x00000022;
                        }
						else
						{
							cbk -= 0x00000022;
						}
						*/
						
					}
					else if ((iItem%2))
                    {
                        if ( ((cbk & 0x00FF0000) < 0x00EE0000) )
                        {
                            cbk += 0x00120000;
                        }
						else
						{
							cbk -= 0x00120000;
						}
                        if ( ((cbk & 0x0000FF00) < 0x0000EE00) )
                        {
                            cbk += 0x00001200;
                        }
						else
						{
							cbk -= 0x00001200;
						}
                        if ( ((cbk & 0x000000FF) < 0x000000EE) )
                        {
                            cbk += 0x00000012;
                        }
						else
						{
							cbk -= 0x00000012;
						}
                    }
					
					pDC->SetBkColor(cbk);
					pDC->FillSolidRect( rc, cbk );
                    //pDC->FillRect(rc, &CBrush(cbk));
                    

					/*
                    if (!(iItem%8))
                        pDC->FillRect(rc, &CBrush(RGB(250,181,182)) );
                    else
                    if ((iItem%8) == 1)
                        pDC->FillRect(rc, &CBrush(RGB(250,250,182)) );
                    else
                    if ((iItem%8) == 2)
                        pDC->FillRect(rc, &CBrush(RGB(182,250,182)) );
                    else
                    if ((iItem%8) == 3)
                        pDC->FillRect(rc, &CBrush(RGB(182,250,251)) );
                    else
                    if ((iItem%8) == 4)
                        pDC->FillRect(rc, &CBrush(RGB(182,181,251)) );
                    else
                    if ((iItem%8) == 5)
                        pDC->FillRect(rc, &CBrush(RGB(250,181,251)) );
                    else
                    if ((iItem%8) == 6)
                        pDC->FillRect(rc, &CBrush(RGB(250,250,251)) );
                    else
                    if ((iItem%8) == 7)
                        pDC->FillRect(rc, &CBrush(RGB(182,181,182)) );
                    */
                       
                }

				if (memcmp(rgbc,rgbc0,6)) {
					if( m_cursel == iItem) {
						pDC->SetTextColor( ccf );
						pDC->SetBkColor(cct );
						pDC->FillSolidRect( rc, cct);
					}
					else
					{
						pDC->SetTextColor( cct );
						pDC->SetBkColor(ccf );
						pDC->FillSolidRect( rc, ccf);
					}
				}

                if (m_tw)
                {
                    if (iItem >= m_registros/* && (m_gmodo & 0x1)*/)
                    {
						if ((m_gmodo & 1))
						{
							if (m_cursel == iItem)
							{
								m_tw->BotonPintado(iItem-GetTopIndex(),2);
							}
							else
								m_tw->BotonPintado(iItem-GetTopIndex(),1);
						}
						/*
						else
							m_tw->BotonPintado(iItem-GetTopIndex(),0); // En principio totales
						*/
                    }                    
                    if( m_cursel == iItem)
                    {
                        m_tw->BotonPintado(iItem-GetTopIndex(),0);
                    }                
                }                
                
                int sbi;
				int oldmode;
				BOOL colorcamp = FALSE;
				COLORREF precolor1;
				COLORREF precolor2;
				int i;
				oldmode = pDC->SetBkMode( TRANSPARENT ); //OPAQUE
                for (i = 0;/*m_apintar[i] != -1*/ m_apintar[i] >= 0 && m_apintar[i] < m_columnas;i++)
                {
                    sbi = m_orderarray[ m_apintar[i] ];
                    if (GetSubItemRect( iItem, sbi , LVIR_BOUNDS , rect ))
                    {                        
                        rect.left = m_pintarects[ m_apintar[i] ].left;
                        rect.right = m_pintarects[ m_apintar[i] ].right;                  
                        
                        if (sbi < COLUMNA_INICIAL)
                        {   
                            if (rect.Size().cx)
                            {
								pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
								pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
                                //pDC->FillRect(rect, &CBrush(::GetSysColor(COLOR_3DFACE)) );
                            
                                //rect.DeflateRect( 0, 1 );
                                rect.bottom--;
                                pDC->Draw3dRect( &rect, ::GetSysColor(COLOR_3DHILIGHT) , ::GetSysColor(COLOR_3DDKSHADOW));								
                                /*
                                rect.DeflateRect( 1, 1 );
                                pDC->Draw3dRect( &rect, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DSHADOW));
                                */
                                if (iItem >= m_registros && (m_gmodo & 0x1))
                                {
                                    HICON icono = theApp.LoadIcon(IDI_NUEVA);                                
                                    pDC->DrawIcon( rect.left+(rect.Size().cx-8)/2, rect.top+(rect.Size().cy-8)/2, icono );
                                }
                                else if( m_cursel == iItem)
                                {
                                    //rect.DeflateRect( 4, 5 );
                                    //CDC
                                    //BOOL FillRgn( CRgn* pRgn, CBrush* pBrush );

                                    HICON icono = theApp.LoadIcon(IDI_INDICATOR);
                                
                                    pDC->DrawIcon( rect.left+(rect.Size().cx-6)/2, rect.top+(rect.Size().cy-8)/2, icono );
                                    //pDC->FillRect(&rect,&CBrush( theApp.m_ctextillo ) );
                                }
                            }
                        }
                        else                            
                        {
                            if (sbi < (m_columnas-COLUMNA_FINAL))
                            {                                
                                DispInfo.item.iSubItem = sbi;
                                OnGetdispinfo((NMHDR*)&DispInfo,&result);
                                if (result == 1)
                                {
                                    *pResult = CDRF_SKIPDEFAULT;
                                    break;// sale del bucle 
                                }
								/*
                                for (int o = 0;texto[o];o++)
                                {
                                    oem_a_ansi((unsigned char *)texto+o);
                                }
								*/
                                if (m_cursel == iItem && sbi == m_curcol && !m_editando)
                                {
									precolor1 = pDC->GetTextColor();
									precolor2 = pDC->GetBkColor();
                                    pDC->SetTextColor( m_pDoc->m_cGridCamFr ); //
									pDC->SetBkColor(m_pDoc->m_cGridCamBk);
									pDC->FillSolidRect( rect, m_pDoc->m_cGridCamBk);
                                    //pDC->FillRect(rect, m_pDoc->m_bGridCamBk );
									colorcamp = TRUE;
                                }  
								else
								{
									if (colorcamp)
									{
										pDC->SetTextColor( precolor1 ); //
										pDC->SetBkColor(precolor2);
										colorcamp = FALSE;
									}
								}
                                
                                UINT format = DT_SINGLELINE|DT_NOPREFIX|DT_LEFT|DT_VCENTER/*|DT_NOCLIP*/;
                                
                                if ( (m_flagcampos[sbi-COLUMNA_INICIAL] & 16) )
                                {
                                    if (pDC->GetTextExtent(texto,strlen(texto)).cx < rect.Size().cx)
                                    {
                                        format &= ~DT_LEFT;
                                        format |= DT_RIGHT;
                                        strcat(texto," ");
                                    }
                                }
                                pCD->clrText = pDC->GetTextColor();
                                pCD->clrTextBk = pDC->GetBkColor();

								{
									char *pcut;
								    if ((pcut = strchr(texto,13)) || (pcut = strchr(texto,10)))
									{
										*pcut = 0;
									}
								}
								
                                pDC->DrawText( texto,strlen(texto),rect,format);								

                                if (m_cursel == iItem && sbi == m_curcol && !m_editando)
                                {
                                    pDC->SetTextColor( m_pDoc->m_cGridSelFr ); //
                                }
                            }
                        }
                  }               
                }
				pDC->SetBkMode( oldmode );

                if (pDC)
                {
                   pDC->Detach();
                   delete pDC;
                }
            }

			*pResult = CDRF_SKIPDEFAULT;
            //ValidateRect(NULL);
		    break;
		case  CDDS_ITEMPREPAINT | CDDS_SUBITEM:			
		    /* esto no sirve ya que de todos modos antes pide el valor del subitem */
			//if (pCD->iSubItem > 5)
			    *pResult = CDRF_SKIPDEFAULT;
            //else
			//    *pResult = CDRF_DODEFAULT;
			break;
		default:
		    *pResult = CDRF_SKIPDEFAULT;
		    //*pResult = CDRF_DODEFAULT;
			break;
		}    
    o_recursion--;
    m_pintando = FALSE;	
    if (!o_recursion)
    {
        if (m_repintar)
        {
            m_repintar = FALSE;
			ActualizaVentana();
            //InvalidateRect(NULL);
        }
    }
	//EnableWindow(TRUE);
}


/*
#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN         1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN         3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_LEFT             6
#define SB_BOTTOM           7
#define SB_RIGHT            7
#define SB_ENDSCROLL        8
*/
void CTablaView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{   
    static int h_recursion = 0;
    if (m_pintando) 
		return;

    if (h_recursion)
    {
        return;
    }    

    h_recursion++;     

    //SetRedraw( FALSE );

    BOOL escrolado = FALSE;

    if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
    {

        //int mi,ma;
        //GetScrollRange( SB_HORZ , &mi, &ma );       
        int apos = GetScrollPos( SB_HORZ );
        int difpos = (int)nPos - apos;
        int posi = 0,x = 0,i;

        for (i = 0;i < m_columnas;i++)
        {           
            if ( (posi + GetColumnWidth( m_orderarray[i])) > apos)
                break;            
            posi += GetColumnWidth( m_orderarray[i] );
        }
        if (difpos < 0)
        {
            if ((int)nPos < posi && i)
            {
                do
                {
                    if (i < 0 || difpos > (x-GetColumnWidth( m_orderarray[i-1])))
                        break;
                    x -= GetColumnWidth( m_orderarray[i-1]);
                    i--;
                } while(-1);
            }
        }
        else if (difpos > 0)        
        {
            if ((int)nPos > (posi+GetColumnWidth(m_orderarray[i])))
            {
                do
                {
                    if (i >= m_columnas || difpos < (x+GetColumnWidth(m_orderarray[i])))
                        break;
                    x += GetColumnWidth(m_orderarray[i]);
                    i++;
                } while(-1);
            }
        }
        if (x)
        {
            CSize s;
            s.cy = 0;
            s.cx = x;
            Scroll(s);
            escrolado = TRUE;
        }
    }
    
    if (nSBCode == SB_LINERIGHT && m_apintar[1] > -1)
    {
        CSize s;
        s.cy = 0;
        s.cx = m_pintarects[ m_apintar[1] ].right;
        Scroll(s);
        escrolado = TRUE;
    }


    if (nSBCode == SB_LINELEFT && m_apintar[0] > 0)
    {
        CSize s;
        s.cy = 0;
        s.cx = m_pintarects[ m_apintar[0]-1 ].left;
        Scroll(s);
        escrolado = TRUE;
    }


    if (nSBCode == SB_PAGERIGHT)
    {
        int i,x = 0;
        CRect r;

        GetClientRect(r);
        
        for (i = m_apintar[0];i < m_columnas;i++)
        {
            if (m_pintarects[i].left <= r.right) 
                x = m_pintarects[i].left;
            else
                break;
        }
        
        if (x)
        {
            CSize s;
            s.cy = 0;
            s.cx = x;
            Scroll(s);
            escrolado = TRUE;
        }
    }


    if (nSBCode == SB_PAGELEFT)
    {
        int i,x = 0;
        CRect r;

        GetClientRect(r);
        
        for (i = m_apintar[0];i >= 0;i--)
        {
            if (m_pintarects[i].left >= -r.Size().cx)
                x = m_pintarects[i].left;
            else
                break;
        }
        
        if (x)
        {
            CSize s;
            s.cy = 0;
            s.cx = x;
            Scroll(s);
            escrolado = TRUE;
        }
    }


    //SetRedraw( TRUE );
    //if (escrolado)
    //    InvalidateRect(NULL);

    h_recursion--;
}

static int v_recursion = 0;
void CTablaView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    
    if (m_editando) 
        return;
    if (m_pintando) 
        return;

    if (v_recursion)
    {
        return;
    }

    v_recursion++;

	switch (nSBCode)
	{
	  case SB_PAGEDOWN:
	  case SB_PAGEUP:
	  case SB_LINEDOWN:
	  case SB_LINEUP:
		// Don't update window if paging up or down. It looks
		// terrible!
		
		SetRedraw(FALSE);
		ShowWindow(SW_HIDE);
		//LockWindowUpdate();		
		BASELISTCTRL::OnVScroll(nSBCode, nPos, pScrollBar);		
		SetRedraw(TRUE);
		ShowWindow(SW_SHOW);		
		//UnlockWindowUpdate();
		break;
	  default:
		BASELISTCTRL::OnVScroll(nSBCode, nPos, pScrollBar);
		break;
	}
  
    //BASELISTCTRL::OnVScroll(nSBCode, nPos, pScrollBar);
	/*
    int iItem = GetTopIndex( );
    if (m_tw)
    {   
        int total = m_tw->TotalBoton();
        if (m_cursel < iItem || m_cursel >= (iItem+total))
        {
            m_tw->BotonPintado(-1,0);
        }
        else
        {
            if (m_cursel >= m_registros)
            {
                m_tw->BotonPintado(m_cursel-iItem,2);
            }
            else
            {
                m_tw->BotonPintado(m_cursel-iItem,0);
            }
        }
    }
	*/
    v_recursion--;
}

void CTablaView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;		
	
	//m_cuentaupdate++;

    if ( (pNMListView->uNewState & LVIS_SELECTED) )
    {
		if (m_enteclado && !m_yomismo && pNMListView->iItem)
		{			
			if (m_precursel != -1 && m_precursel != m_cursel)
			{
				int n;					
				n = pNMListView->iItem - m_precursel;				
				if (n == 1 || n == -1)
				{
					m_cursel += n;
					m_postposi++;
					PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)0);
				}
				*pResult = 1;
				return;
			}
			
		}
		if (pNMListView->iItem < (m_registros+(m_gmodo & 1)))
		{
			m_cursel = pNMListView->iItem;
			
			m_yomismo++;			
			SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED );
			m_yomismo--;			

			if (m_cursel >= m_registros)
			{
				m_curcol = COLUMNA_INICIAL;
			}
			
			if (m_enclic)
			{			
				m_clicposiciona = m_enclic;				
				/*
				if ((m_gmodo & 0x10000) && m_padre > -1)
					m_NotificaOnDraw |= 1;				
				else 
					m_NotificaOnDraw |= 8;								
				*/
			}
			else
			{			
				if (!NotificaPosicion())
				{
					if ((m_gmodo & 0x10000) && m_padre > -1)
						m_NotificaOnDraw |= 1;				
					else /*if (!(m_gmodo & 0x1000000))*/
						m_NotificaOnDraw |= 8;								
				}        
			}
        
			if (!(m_gmodo & 0x10000) && !(m_gmodo & 0x100000) && !(m_gmodo & 1) && m_cursel >= m_registros )
			{            
				TeclaStd(802);            
			} 
		}        
    }

	*pResult = 0;
}

void CTablaView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY * phdn = (HD_NOTIFY *) pNMHDR;
    
	if ((m_gmodo & 0x4000000))
	{
		*pResult = 1;
	}
	else
	{
		if (phdn->iItem >= (m_columnas - COLUMNA_FINAL) || phdn->iItem < COLUMNA_INICIAL || m_editando)
			*pResult = 1;
		else
		{
			*pResult = 0;
			m_flagpintar = TRUE;	
		}
	}
}

void CTablaView::OnEnddrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY * phdn = (HD_NOTIFY *) pNMHDR;


	/*
	m_flagpintar = FALSE;	
    AjustaColumnaFinal();
	ValidateRect(NULL);
	m_flagpintar = TRUE;	
	InvalidateRect(NULL);	
	*/
	
	m_guardaposiciones = TRUE;
	PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);	
    
	

	*pResult = 0;
}


void CTablaView::OnBegintrack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;	
    if (m_editando) 
    {
        *pResult = 1;
        return;
    }
	m_flagpintar = FALSE;	
	*pResult = 0;
}

void CTablaView::OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	/*
    AjustaColumnaFinal();
	*/
	m_flagpintar = TRUE;	
	m_guardaposiciones = TRUE;	
	PostMessage(WM_COMMAND,ID_AVISO_INVALIDATE_GRID,(LPARAM)0);	
    
	
	*pResult = 0;
}
const char *quita_path(const char *ff);
void CTablaView::EjecutaRegCambio(DlgRegCambio *pdlg) {
    char *p;
    long tam;
    long respuesta;
    long modo = 1;
    int operacion;
    int r = -1;
    char *tmp;
    int memoria;
	int nfiltros = 0;
	struct grid_filtro *pfiltros = NULL;
	int flag0;
	int ncols = 0;
	int *cols = NULL;
    
	{
		//int i;
		operacion = 24;
		
		pfiltros = (struct grid_filtro *)malloc(sizeof(struct grid_filtro)*100);
		memset((void *)pfiltros,0,sizeof(struct grid_filtro)*100);
		ncols = 0;
		cols = (int *)malloc(100*sizeof(int));

		/*
	DDX_Control(pDX, IDC_COMBO1, Usuario);
	DDX_Control(pDX, IDC_EDIT1, Desde);
	DDX_Control(pDX, IDC_EDIT2, Hasta);
	DDX_Control(pDX, IDC_COMBO2, Campo);
	DDX_Control(pDX, IDC_EDIT3, ValorAnterior);
	DDX_Control(pDX, IDC_EDIT4, ValorNuevo);

	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);		
	DDX_Control(pDX, IDC_RADIO4, m_radio4);
	DDX_Control(pDX, IDC_RADIO5, m_radio5);
	DDX_Control(pDX, IDC_RADIO6, m_radio6);

	inserta_campo(pfi,0,"Usuario","Usuario",10,"","","");
	inserta_campo(pfi,1,"Fecha","Fecha",-1,"","","");
	inserta_campo(pfi,2,"Hora","Hora",8,"","","");
	inserta_campo(pfi,3,"Tipo","Tipo",1,"","","");
	inserta_campo(pfi,4,"Accion","Accion",2,"","","");
	inserta_campo(pfi,5,"Tab-Def","Tab-Def",12,"","","");
	inserta_campo(pfi,6,"Campo","Campo",3,"","","");
	inserta_campo(pfi,7,"Clave","Clave",50,"","","");
	inserta_campo(pfi,8,"Mac","Mac",17,"","","");
	inserta_campo(pfi,9,"Empresa","Empresa",10,"","","");
	inserta_campo(pfi,10,"Titulo","Titulo",50,"","","");
	inserta_campo(pfi,11,"ValorAnt","Valor Anterior",30,"","","");
	inserta_campo(pfi,12,"ValorNue","Valor Nuevo",30,"","","");
    inserta_campo(pfi,13,"Registro","Registro",0,"0","999999999999","0");

	*/
		cols[ncols] = 0; /* usuario */
		ncols++;
		cols[ncols] = 1; /* fecha */
		ncols++;
		cols[ncols] = 2; /* hora */
		ncols++;
		cols[ncols] = 11; /* Valorant */
		ncols++;
		cols[ncols] = 12; /* Valornue */
		ncols++;
		cols[ncols] = 7; /* clave */
		ncols++;
		cols[ncols] = 6; /* campo */
		ncols++;
		cols[ncols] = 5; /* tab-def */
		ncols++;
		cols[ncols] = 9; /* Empresa */
		ncols++;
		cols[ncols] = 10; /* Titulo */
		ncols++;
		cols[ncols] = 3; /* tipo */
		ncols++;
		cols[ncols] = 4; /* Accion */
		ncols++;
		cols[ncols] = 8; /* MAC */
		ncols++;		


		pdlg->m_desdehora.Trim();
		pdlg->m_hastahora.Trim();

		if (!pdlg->m_usu.IsEmpty()) {			
		    pfiltros[nfiltros].tipo  = 1;			
			strncpy(pfiltros[nfiltros].valor,pdlg->m_usu,298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 0;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}
		if (!pdlg->m_notabla) {
			pfiltros[nfiltros].tipo  = 1;			
			strncpy(pfiltros[nfiltros].valor,quita_path(m_def->m_info->PathDef),298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 5;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}
		if (pdlg->m_campo > 0) {
		    pfiltros[nfiltros].tipo  = 1;
			sprintf(pfiltros[nfiltros].valor,"%03d",m_def->Campo(pdlg->m_campo-1,TRUE)->Id);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 6;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}
		if (pdlg->m_sel < 5) {
			/***/
		    pfiltros[nfiltros].tipo  = 0;
			strncpy(pfiltros[nfiltros].limi,pdlg->m_desde,298);
			strncpy(pfiltros[nfiltros].lims,pdlg->m_hasta,298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 1;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}
		if (!pdlg->m_valant.IsEmpty()) {
			pfiltros[nfiltros].tipo  = 3;
			strncpy(pfiltros[nfiltros].valor,pdlg->m_valant,298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 11;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}
		if (!pdlg->m_valnue.IsEmpty()) {
			pfiltros[nfiltros].tipo  = 3;			
			strncpy(pfiltros[nfiltros].valor,pdlg->m_valnue,298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 12;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}

		if (!pdlg->m_hastahora.IsEmpty() || !pdlg->m_desdehora.IsEmpty()) {
		    pfiltros[nfiltros].tipo  = 0;
			strncpy(pfiltros[nfiltros].limi,pdlg->m_desdehora,298);
			strncpy(pfiltros[nfiltros].lims,pdlg->m_hastahora,298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 2;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}

		if (!nfiltros) {
		    pfiltros[nfiltros].tipo  = 0;
			strncpy(pfiltros[nfiltros].limi,"",298);
			strncpy(pfiltros[nfiltros].lims,"zzzzzzzzzzzzzzzzzzzz",298);
			flag0 = 0;
			pfiltros[nfiltros].tipo |= (flag0 << 4);
			pfiltros[nfiltros].campo = 0;
			pfiltros[nfiltros].mayus = 0;
			pfiltros[nfiltros].inver = 0;
			nfiltros++;
		}

		memoria = (ncols*sizeof(int)) + (nfiltros*sizeof(struct grid_filtro));

		tam = (sizeof(long) * 4) + (sizeof(int)*6) + 100;

		tmp = (char *)malloc(tam+memoria+1);
		p = tmp;

		memcpy((void *)p,(void *)&m_localizador,sizeof(int));
		p += sizeof(int);
    
		respuesta = tam;
		tam += memoria;

		memcpy((void *)p,(void *)&tam,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&respuesta,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&modo,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&operacion,sizeof(int));
		p += sizeof(int);
    
		memcpy((void *)p,(void *)&r,sizeof(int));
		p += sizeof(int);

		strcpy(p,"Histórico de Cambios");
		p += (strlen(p)+1);

		memcpy((void *)p,(void *)&ncols,sizeof(int));
		p += sizeof(int);
		memcpy((void *)p,(void *)cols,ncols*sizeof(int));
		p += (ncols*sizeof(int));
		memcpy((void *)p,(void *)&nfiltros,sizeof(int));
		p += sizeof(int);
		memcpy((void *)p,(void *)pfiltros,nfiltros*sizeof(struct grid_filtro));

		free((void *)cols);
		free((void *)pfiltros);

		if (ncols)
		{
			DespachadorInterno(13,(void *)tmp);
		}
		free((void *)tmp);
	}
}

BOOL CTablaView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{	
    HD_NOTIFY	*pHDN = (HD_NOTIFY*)lParam;

	LPNMHDR pNH = (LPNMHDR) lParam; 
	// wParam is zero for Header ctrl
	if (!(m_gmodo & 0x4000000))
	{
		if( wParam == 0 )
		{
			if (pNH->code == NM_RCLICK  )
			{
				CPoint pt(GetMessagePos());
				CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
				pHeader->ScreenToClient(&pt);		    
			    
				int nCol = -1;
				CRect rcCol;
				int i;
				for(i=0; i < m_columnas && Header_GetItemRect(pHeader->m_hWnd, i, &rcCol); i++ )
				{
					if( rcCol.PtInRect( pt ) )
					{
						nCol = i;
						break;
					}
				}

				CMenu menu;
				CMenu filtros;
				int r = 1;

				if (menu.CreatePopupMenu( )) 
				{					

					filtros.CreatePopupMenu();

					menu.InsertMenu(0,MF_STRING | MF_BYPOSITION,1,"Ordenar");
					menu.InsertMenu(1,MF_STRING | MF_BYPOSITION,2,"Buscar");
					menu.InsertMenu(2,MF_STRING | MF_BYPOSITION,3,"Operaciones");
					menu.InsertMenu(3,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);    
					menu.InsertMenu(5,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)filtros.m_hMenu,"Filtro avanzado");					
					{
						CDlgGridFiltro dlg(this);
						filtros.InsertMenu(0,MF_STRING | MF_BYPOSITION,ID_CONSULTA_FILTRO,"Mantenimiento");		
						dlg.RellenaMenu(&filtros,1,100000);
					}
					menu.InsertMenu(5,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);    
					menu.InsertMenu(6,MF_STRING | MF_BYPOSITION,ID_GRID_RESTAURA,"Restaurar predeterminados");
					menu.InsertMenu(7,MF_STRING | MF_BYPOSITION,ID_CONSULTA_TODACOLUMNAS,"Ver todas las columnas");
					menu.InsertMenu(8,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);    
					menu.InsertMenu(9,MF_STRING | MF_BYPOSITION,7,"Borrar Columna");
					menu.InsertMenu(10,MF_STRING | MF_BYPOSITION,8,"Borrar Columnas Hasta el Final");
					menu.InsertMenu(11,MF_STRING | MF_BYPOSITION,9,"Cuadro de Seleccion");
					menu.InsertMenu(12,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
					menu.InsertMenu(13,MF_STRING | MF_BYPOSITION,10,"Consultar registro de cambios");

					POINT pp;
					::GetCursorPos(&pp);
					r = (int)menu.TrackPopupMenu(TPM_LEFTALIGN| TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y,this,NULL);
				}

				if (r == 10) {
					DlgRegCambio dlgcambio(this);
					if (dlgcambio.DoModal() == IDOK) {
						EjecutaRegCambio(&dlgcambio);
					}
				}
				else if (r == ID_CONSULTA_TODACOLUMNAS)
				{
					m_curcol = 0;
					BorraCache();
					m_def->CreaCamcolu(TRUE,TRUE);
					EnviaCamcolu();
					CreaColumnas();
					InvalidateRect(NULL);
					GuardaPosiciones();
				}
				else if (r == ID_GRID_RESTAURA)
				{
					if (AfxMessageBox("¿Seguro que desea Restaurar predeterminados?",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						m_curcol = 0;
						BorraCache();
						m_def->CreaCamcolu(TRUE);
						EnviaCamcolu();
						CreaColumnas();
						InvalidateRect(NULL);
						GuardaPosiciones();
					}
				}
				else if (r == 1)
				{
					CDlgOrdenar dlg(this);
					dlg.m_col = nCol;
					if (dlg.DoModal() == IDOK)
					{
						CDlgGridFiltro dlgf(this);
						CString s;
						int j;
						for (int i = 0;i < m_def->m_n_campos_col;i++)
						{
							j = m_def->m_colcampos[i];
							dlgf.m_principal->m_item[j]->m_checked = TRUE;
						}
						if (dlg.m_sel1 > -1)
						{
							dlgf.m_principal->m_item[dlg.m_sel1]->m_Orden = TRUE;
							dlgf.m_principal->m_item[dlg.m_sel1]->m_OrdenInverso = dlg.m_inver1;
							s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel1]->m_padre->m_nombre,dlg.m_sel1);
							dlgf.OrdenId(0,s);
						}
						if (dlg.m_sel2 > -1)
						{
							dlgf.m_principal->m_item[dlg.m_sel2]->m_Orden = TRUE;
							dlgf.m_principal->m_item[dlg.m_sel2]->m_OrdenInverso = dlg.m_inver2;
							s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel2]->m_padre->m_nombre,dlg.m_sel2);
							dlgf.OrdenId(1,s);
						}
						if (dlg.m_sel3 > -1)
						{
							dlgf.m_principal->m_item[dlg.m_sel3]->m_Orden = TRUE;
							dlgf.m_principal->m_item[dlg.m_sel3]->m_OrdenInverso = dlg.m_inver3;
							s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel3]->m_padre->m_nombre,dlg.m_sel3);
							dlgf.OrdenId(2,s);
						}
						
						dlgf.RellenaListado(TRUE);
						EjecutaFiltro(&dlgf);
					}
				}
				else if (r == 2)
				{
					CDlgBusqueda dlg(this);
					dlg.m_col = nCol;
					if (dlg.DoModal() == IDOK)
					{
						CDlgGridFiltro dlgf(this);
						CString s;
						int j;
						for (int i = 0;i < m_def->m_n_campos_col;i++)
						{
							j = m_def->m_colcampos[i];
							dlgf.m_principal->m_item[j]->m_checked = TRUE;
						}
						if (dlg.m_sel > -1)
						{
							dlgf.m_principal->m_item[dlg.m_sel]->m_Orden = TRUE;
							dlgf.m_principal->m_item[dlg.m_sel]->m_LimiteI = dlg.m_limitei;
							dlgf.m_principal->m_item[dlg.m_sel]->m_LimiteS = dlg.m_limites;
							dlgf.m_principal->m_item[dlg.m_sel]->m_Valor = dlg.m_valor;
							dlgf.m_principal->m_item[dlg.m_sel]->m_TipoValor= dlg.m_tipovalor;
							dlgf.m_principal->m_item[dlg.m_sel]->m_Mayus = dlg.m_mayus;
							dlgf.m_principal->m_item[dlg.m_sel]->m_Inver = dlg.m_inversa;

							s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel]->m_padre->m_nombre,dlg.m_sel);
							dlgf.OrdenId(0,s);							
						}						
						dlgf.RellenaListado(TRUE);
						EjecutaFiltro(&dlgf);
					}					
				}
				else if (r == 3)
				{
					CDlgOperaciones dlg(this);
					dlg.m_col = nCol;
					if (dlg.DoModal() == IDOK)
					{
						CDlgGridFiltro dlgf(this);
						CString s;
						int j;
						for (int i = 0;i < m_def->m_n_campos_col;i++)
						{
							j = m_def->m_colcampos[i];
							dlgf.m_principal->m_item[j]->m_checked = TRUE;
						}
						if (dlg.m_sel > -1)
						{
							dlgf.m_principal->m_item[dlg.m_sel]->m_Orden = TRUE;
							dlgf.m_principal->m_item[dlg.m_sel]->m_opera = dlg.m_opera;

							s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel]->m_padre->m_nombre,dlg.m_sel);
							dlgf.OrdenId(0,s);							
						}						
						dlgf.RellenaListado(TRUE);
						EjecutaFiltro(&dlgf);
					}					
				}
				else if (r == 9)
				{
						CDlgCol dlg(this);
						if (dlg.DoModal() == IDOK)
						{
							EnviaCamcolu();
							CreaColumnas();
							InvalidateRect(NULL);
							GuardaPosiciones();
						}
				}            
				else if (r == ID_CONSULTA_FILTRO)
				{
					Filtros();
				} 
				else if (r >= 100000)
				{		
					CDlgGridFiltro dlg(this);
					if (r >= 101000)		
						dlg.m_registrolocal = FALSE;		
					else
						dlg.m_registrolocal = TRUE;
					filtros.GetMenuString( r,dlg.m_nombre,MF_BYCOMMAND);
					if (dlg.LeeDelRegistro())
					{
						dlg.RellenaListado(TRUE);
						EjecutaFiltro(&dlg);
					}
					else
					{
						AfxMessageBox("Filtro erroneo, acceda a la opción de Mantenimiento");
					}
				}
				else if (nCol >= COLUMNA_INICIAL && nCol < (m_columnas-COLUMNA_FINAL))
				{
					if (r >= 7 && r <= 8)
					{
						if ( (m_columnas -COLUMNA_FINAL-COLUMNA_INICIAL) < 2)
							return TRUE;

						LVCOLUMN colum;
						BorraCache();
						int _nCol;
						int nLimi = nCol;

						if (r == 8)
						{
							_nCol = m_columnas - 2;
						}
						else
						{
							_nCol = nCol;
						}

						for (nCol = _nCol;nCol >= nLimi;nCol--)
						{
							int i;
							DeleteColumn( nCol );
							m_columnas--;                						
							for (i = 0;i < m_columnas;i++)
							{
								memset((void *)&colum,0,sizeof(colum));
								colum.mask = LVCF_ORDER | LVCF_SUBITEM;
								colum.iSubItem = i;
								colum.iOrder = i;
								SetColumn( i , &colum );
							}
							m_def->m_n_campos_col--;
							for (i = nCol-COLUMNA_INICIAL;i < m_def->m_n_campos_col;i++)
							{
								m_def->m_colcampos[i] = m_def->m_colcampos[i+1];
							}
							m_def->m_colcampos[m_def->m_n_campos_col] = -1;
						}
						i = 0;
						int offset = 0;
						for (int col = 0;col < m_def->m_n_campos_col;col++)
						{
							m_poscampos[i] = offset;
							m_tamcampos[i] = m_def->Campo(col)->c_maxqc;
							m_flagcampos[i] = m_def->Campo(col)->c_tipo;
							if (m_flagcampos[i] != 0 && m_flagcampos[i] != 5)
							{
								m_flagcampos[i] |= 16;
							}
							offset += m_tamcampos[i];        
							i++;
						}
						m_lenreg = offset;
						m_apintar[0] = -1;

						m_sincachear++;
						AjustaColumnaFinal();
						EnviaCamcolu();
						m_sincachear--;
						//m_iniciales[0] = 
						//m_iniciales[1] = -1;
						m_refrespending = FALSE;
						InvalidateRect(NULL);
						GuardaPosiciones();
					}
				}
				return TRUE;
			}
		}
	}
/*
    if (pNH->code == NM_RETURN)
    {
        return TRUE;
    }

    if (pNH->code == LVN_KEYDOWN)
    {
        LV_KEYDOWN *kd;
        kd = (LV_KEYDOWN *)lParam;
    }
*/
/*
typedef struct tagLV_KEYDOWN {
    NMHDR hdr;   
    WORD wVKey;  
    UINT flags;  
} LV_KEYDOWN
*/

	return BASELISTCTRL::OnNotify(wParam, lParam, pResult);
}


void CTablaView::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	BOOL r = FALSE;
	int clave = 0;
	int Item;	

    if (m_editando) 
    {
        *pResult = 1;
        return;
    }

    if (phdn->iItem < COLUMNA_INICIAL)
    {
        *pResult = 1;
    }
    else
    if (phdn->iItem >= (m_columnas - COLUMNA_FINAL))
    {
        *pResult = 1;
    }
    else
    {
		Item = phdn->iItem;
		/*
        if ( (m_gmodo & 1) && !(m_gmodo & 0x80000) )
        {
            // disparar evento ....?
        }
        else
		*/
        { 
			clave = -1;
			if ((m_gmodo & 256))
			{
				clave = m_def->EsDeLaClave(Item-COLUMNA_INICIAL,m_def->m_info->clave_actual);
			}
			else
			{
				if (m_reordenado != Item && clave == -1)
				{
					if (!(m_ini_gmodo & 1) && !(m_ini_gmodo & 128) && m_reordenado == -1 || !m_reorden)
					{
						clave = m_def->EsInicialClave(Item-COLUMNA_INICIAL);
					}
					if (clave > -1)
					{            
						CMenu escoge;
						int varias = 0;
						if (!(m_gmodo & 0x80000) && escoge.CreatePopupMenu())
						{
							CString s;
							int h;
							int i;
							for (i = 0;i < m_def->m_info->n_claves;i++)
							{
								if (m_def->k_campo(i,0) == (Item-COLUMNA_INICIAL))
								{
									h = m_def->k_campo(i,0);
									if (h < 0)
										s = "oculto";
									else
										s = m_def->c_nombre(h);
									for (int j = 1;j < m_def->Clave(i)->n_componentes;j++)
									{
										s += " + ";
										h = m_def->k_campo(i,j);
										if (h < 0)
										{
											s += "oculto";
										}
										else
										{
											s += m_def->c_nombre(h);
										}
									}
									escoge.InsertMenu(varias,MF_STRING | MF_BYPOSITION,10000+i,s);
									varias++;
								}
							}
						}

						if (varias > 1)
						{
							POINT pp;
							::GetCursorPos(&pp);                    
							int r = (int)escoge.TrackPopupMenu(TPM_CENTERALIGN | TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y-10,this,NULL);
							if (r >= 10000)
								clave = r - 10000;
						}
					}
					else
					{
						if (!(m_ini_gmodo & 1)) /* de momento no en mantenimientos ... */
						{
							clave = -(Item+2);
						}
					}
				}
			}
			if (clave != -1)
			{
				r = CambiarOrden(clave);
			}
            
	    *pResult = 0;
        }
    }	
	
    SetFocus();
	
}


void CTablaView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    /*
    if ( (m_gmodo & 0x10000) ) // modo 'hijo'
    {

    }
    else
    */  
	

	
	CPoint pt;
	UINT Flags = 0;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	int item = HitTest(pt,&Flags);
	if (item < 0)
		return;
   
	/*NotificaPosicion();*/
	PostMessage(WM_COMMAND,ID_AVISO_DOUBLECLICK,item/*m_cursel*/);

	*pResult = 0;
}

void CTablaView::OnRButtonDown(UINT nFlags, CPoint point) 
{
}


void CTablaView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	LVHITTESTINFO ht;
	BOOL notifica = FALSE;
	/*
	int state = 0;
	*/

	try
	{

	/*
	ht.pt = point;
	int rval = ListView_SubItemHitTest( m_hWnd, &ht );
    if (rval >= 0)
	{
	   state = GetItemState( ht.iItem, LVIS_FOCUSED );
	}
	*/

    /*
    if (m_pListEdit)
	   {
	   if( GetFocus() != this ) SetFocus();
	   }
    */
    if (m_editando && (m_gmodo & 1))
    {        
        TeclaStd(806);        
        return;
    }

	m_enclic++;
    BASELISTCTRL::OnLButtonDown(nFlags, point);	
	if (m_enclic == m_clicposiciona)
	{
		m_clicposiciona = 0;
		notifica = TRUE;		
	}
	m_enclic--;
	ht.pt = point;
	int rval = ListView_SubItemHitTest( m_hWnd, &ht );
    if (rval >= 0)
	{		
		if (ht.iSubItem != m_curcol)
		{
			notifica = TRUE;
		}
		if (notifica)
			PostMessage(WM_COMMAND,ID_GRIDNUEVACOLUMNA,(LPARAM)(ht.iSubItem+1));
	}
	else
		PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)2);

	//if (!state /*|| !m_activa || (ht.iItem == (GetItemCount()-1) &&  ht.iSubItem) */) 
    //{
	//  return;
	//}

#ifdef kakakaka
    CambiaCorriente(ht.iSubItem);
	
    if (m_curcol == ht.iSubItem && !(m_gmodo & 1))
    {
		/*
        int clave; // falta invento para claves cruzadas!!!
        if ( (clave = m_def->EsInicialClave(m_curcol-COLUMNA_INICIAL)) > -1 && clave != m_def->m_info->clave_actual)
        {
            CambiarOrden(clave);            
            return;
        }		
        else
        {
            if (m_def->EsDeLaClave(m_curcol-COLUMNA_INICIAL,m_def->m_info->clave_actual) > -1)
            {
                clave = m_def->m_info->clave_actual;
            }
        }

        if (clave > -1)
        {
            EdicionLocal(0,0,0);
        }
		*/
    }
#endif
	} 
	catch(...)
	{
		return;
	}
}


void CTablaView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CString str = pDispInfo->item.pszText;
	int item    = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;

	/*
	{
		unsigned char *pp = (unsigned char *)str.GetBuffer(str.GetLength()+1);
		while(*pp)
		{
			oem_a_ansi(pp);
			pp++;
		}
		str.ReleaseBuffer(-1);
	}
	*/

    CTablaEdit* pListEdit = new CTablaEdit( item, subitem, str );
	
	CRect  rect( 0,0,1,1 );
	DWORD dwStyle = ES_LEFT;
	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_WANTRETURN/*|ES_MULTILINE|ES_AUTOVSCROLL*/;
	if (pListEdit->Create( dwStyle, rect, this , ID_TRAZASINBOLFIN ))
	   {	
       RECT rect;
	   m_edit = pListEdit;
       m_editando = TRUE;
	   PosicionElemento( &rect, item, subitem ,TRUE);	
       m_edit->MoveWindow( &rect );
       ::SetWindowPos(m_edit->m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);           
       ::SetWindowPos(m_edit->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	   //m_edit->CalulateSize();	   
	   }
	else
	   {
	   delete pListEdit;	   
	   }	
	
	*pResult = 1;
}

void CTablaView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	char *str = NULL;

	
	if (m_edit && pDispInfo->item.pszText)
	{
		BOOL vale = !m_edit->m_bESC;	
		if (vale)
		{
			str = (char *)malloc(strlen(pDispInfo->item.pszText)+1);
			strcpy(str,pDispInfo->item.pszText);
			PostMessage(WM_COMMAND,ID_GRIDENDLABELEDIT,(LPARAM)str);	
		}
		PostMessage(WM_COMMAND,ID_GRID_DELETE_OBJECT,(LPARAM)m_edit);
		m_edit = NULL;		
	}
	m_editando = FALSE;
	m_solofoco++;
	SetFocus();
	m_solofoco--;
	*pResult = 0;
}


void CTablaView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
        if (!m_editando)
        {
            if ((m_gmodo & 1))
            {   
                //TeclaStd(nChar);
				m_mtecla = nChar;
                MantenimientoLinea();
                /*
                TeclaStd(802);                
                if (isprint(nChar))
                {
                    DebugBreak();
                    //theApp.m_EntornoActivo->m_wallowed->PostMessage(WM_COMMAND,(WPARAM)999,(LPARAM)nChar);
                } 
                */
            }
            else
            {
				/*
                int clave = -1; // falta invento para claves cruzadas!!!
                if ( (clave = m_def->EsInicialClave(m_curcol-COLUMNA_INICIAL)) > -1 && clave != m_def->m_info->clave_actual)
                {
                    //CambiarOrden(clave);// lo haremos luego
                    //return;
                }
                else
                {
                    if (m_def->EsDeLaClave(m_curcol-COLUMNA_INICIAL,m_def->m_info->clave_actual) > -1)
                    {
                        clave = m_def->m_info->clave_actual;
                    }
                }
				*/

                //if (clave > -1)
                {
                    EdicionLocal(nChar,nRepCnt,nFlags);
                }
            }
          }
}

void CTablaView::CambiaCorriente(int nuevo,int salto,BOOL nonotifica)
{
    if (nuevo < COLUMNA_INICIAL || nuevo > (m_columnas-COLUMNA_FINAL)) return;

    //LV_COLUMN Column;   

    //memset(&Column,0,sizeof(Column));


    int orden;
    for (orden = 0;orden < m_columnas;orden++)
    {
        if (nuevo == m_orderarray[orden]) break;
    }

    if (salto)
    {
        orden += salto;
        if (orden < COLUMNA_INICIAL || orden >= (m_columnas-COLUMNA_FINAL))
        {
            return;
        }
        nuevo = m_orderarray[orden];
    }


    //Column.mask = LVCF_IMAGE;
    //GetColumn( nuevo, &Column );

    int sc = 0;

    if (m_pintarects[ orden ].left < 0)
    {
        sc = m_pintarects[ orden ].left+1;
    }
    else    
    {
        CRect r;        
        GetClientRect(r);
        int dif = m_pintarects[ orden ].right - r.right;
        if (dif > 0 && m_apintar[0] != -1)
        {
			int i;
            for(i = m_apintar[0];i < (m_columnas-COLUMNA_FINAL);i++)
            {
                sc = m_pintarects[i].right + 1;
                if (sc >= dif) break;
            }
        }
    }    

    if (sc)
    {
        CSize s;
        s.cy = 0;
        s.cx = sc;
        Scroll(s);
    }
    
    m_curcol = nuevo;

	if (m_guarda_posicion)
	{
		m_guardaposiciones = TRUE;
	}

    RedrawItems( m_cursel, m_cursel );

	if (nonotifica)
		return;


    NotificaPosicion();	

	ActualizaVentana();

    //Column.mask = LVCF_IMAGE;
    //Column.iImage = 0;
    //SetColumn( m_curcol, &Column ) ;

}


void CTablaView::OnSetFocus(CWnd* pOldWnd) 
{
    if (m_tw && m_tw->m_dsctrl)
    {
        m_tw->m_dsctrl->EnsureVisible();
    }	

    BASELISTCTRL::OnSetFocus(pOldWnd);
	if (m_solofoco)
		return;

	if (m_focohijo > -1)
	{
       CDsStdCtrl *pctrl = NULL;
       if (CDsStdDlg::m_todosctrl.Lookup(m_focohijo,pctrl) && pctrl)
       {
		   CTablaW *ptabla = (CTablaW *)pctrl->m_ctrl;
		   ptabla->SetFocus();
		   return;
	   }
	   else
	   {
		   m_focohijo = -1;
	   }
	}

	if (m_editando)
    {
        theApp.ActivacionPorDefecto();
        return;
    }   

    if (m_tw && m_tw->m_dsctrl)
    {
		theApp.m_antctrlactivo = theApp.m_ctrlactivo;
        theApp.m_ctrlactivo = m_tw->m_dsctrl;
        //m_tw->m_dsctrl->EnsureVisible();
    }
	/* Sustituido por tipo 7
    {
        long tam;
        long respuesta;
        long modo = 1;        
        int operacion = 17;
        char *p;
        char tmp[512];

        p = tmp;

        memcpy((void *)p,(void *)&m_localizador,sizeof(int));
        p += sizeof(int);

        tam = (sizeof(long) * 6) + (sizeof(int)*2) + 100;
        respuesta = tam;

        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(int);        
        *p = 0;                     
        DespachadorInterno(13,(void *)tmp);
	}
	*/

	GetParent()->SendMessage(WM_NCACTIVATE,TRUE);	    

    m_confocus = TRUE;

	if (!m_postposi)
	{
		m_yomismo++;
		SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED| LVIS_FOCUSED );
		m_yomismo--;
	}

	if (m_escampo && m_noesconsulta)
	{
		TeclaStd(999);
	}
}


void CTablaView::OnKillFocus(CWnd* pNewWnd) 
{
	BASELISTCTRL::OnKillFocus(pNewWnd);

    /*
    if (m_tw && m_tw->m_dsctrl && theApp.m_ctrlactivo == m_tw->m_dsctrl)
    {
        theApp.m_antctrlactivo = theApp.m_ctrlactivo;
        theApp.m_ctrlactivo = NULL;
    }
	*/
    
    GetParent()->SendMessage(WM_NCACTIVATE,FALSE);

    m_confocus = FALSE;

	/* Sustituido por tipo 6
    {
        long tam;
        long respuesta;
        long modo = 1;        
        int operacion = 18;
        char *p;
        char tmp[512];

        p = tmp;

        memcpy((void *)p,(void *)&m_localizador,sizeof(int));
        p += sizeof(int);

        tam = (sizeof(long) * 6) + (sizeof(int)*2) + 100;
        respuesta = tam;

        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(int);        
        *p = 0;                     
        DespachadorInterno(13,(void *)tmp);
	}
	*/
}


void CTablaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    const UINT ENHANCEDKEY = 0x0100;
    const UINT PAGEUPKEY   = 0x21;
    const UINT PAGEDNKEY   = 0x22;
    BOOL procesar = TRUE;
	static BOOL estoyaqui = FALSE;

	if (estoyaqui)
		return;
    if (m_tw && m_tw->m_dsctrl)
    {
        m_tw->m_dsctrl->EnsureVisible();
    }
    
    switch(nChar)
    {
    case VK_TAB:
        if ((::GetKeyState( VK_SHIFT ) & 0x8000))
        {
            CambiaCorriente(m_curcol,-1);        
        }
        else        
        {
            CambiaCorriente(m_curcol,1);        
        }
        procesar = FALSE;
        break;
    case VK_LEFT:    
        if ((::GetKeyState( VK_CONTROL ) & 0x8000))
            OnHScroll(SB_PAGELEFT, 0,NULL);
        else
        {
            CambiaCorriente(m_curcol,-1);
        }
        procesar = FALSE;
        break;
    case VK_RIGHT:    
        if ((::GetKeyState( VK_CONTROL ) & 0x8000))
            OnHScroll(SB_PAGERIGHT, 0,NULL);
        else
        {
            CambiaCorriente(m_curcol,1);
        }
        procesar = FALSE;
        break;
    default:        
        break;
    }

    if (procesar)
	{
	  // Handle enhanced keys since OnChar doesn't seem to get
	  // them...
	  //if (nFlags & ENHANCEDKEY)
	
	   m_enteclado++;
	   m_precursel = m_cursel;
		
	  {
		// Don't update window if paging up or down. It looks
		// terrible!
		 
		switch (nChar)
		{
			/*
		case VK_HOME:
		   Cacheamelo(0,0);
           NuevoCorriente(0,TRUE);
           EnsureVisible(m_cursel,TRUE);
		   m_precursel = -1;
		   m_enteclado--;
		  return;
	  */
		case VK_END:
		   if (GetCountPerPage() < m_registros)
			   Cacheamelo(m_registros-GetCountPerPage(),m_registros);
		   /*
           NuevoCorriente(m_registros-1,FALSE);
           EnsureVisible(m_cursel,TRUE);
		   m_precursel = -1;
		   m_enteclado--;
		  return;		  
		  */
		   break;
		case PAGEUPKEY:
		  OnVScroll(SB_PAGEUP, 0, 0);
		   m_precursel = -1;
		   m_enteclado--;
		  return;

		case PAGEDNKEY:
		  OnVScroll(SB_PAGEDOWN, 0, 0);
		   m_precursel = -1;
		   m_enteclado--;
		  return;
		}
		
	  }	  
	  

	   estoyaqui = TRUE;
       BASELISTCTRL::OnKeyDown(nChar, nRepCnt, nFlags);
	   estoyaqui = FALSE;
	   m_precursel = -1;
	   m_enteclado--;
	}
}

void CTablaView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    switch(nChar)
    {
    case VK_RETURN:
    case VK_ESCAPE:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_TAB:
        break;
    default:
        BASELISTCTRL::OnKeyUp(nChar, nRepCnt, nFlags);
    }
}

BOOL CTablaView::EnsureVisible( int nItem, BOOL bPartialOK )
{
	BOOL ret;	
    /*
    if (m_tw && m_tw->m_dsctrl)
    {
        m_tw->m_dsctrl->EnsureVisible();
    }
    */
	ret = BASELISTCTRL::EnsureVisible( nItem, bPartialOK );
	//ASSERT(ret);
    return ret;
}

BOOL CTablaView::PosicionElemento(RECT *prect, int iItem, int iSubItem,BOOL Absoluto)
{
ASSERT( iItem >= 0 );

if (!Absoluto)
{
	if (iSubItem < 0 || iSubItem >= m_def->m_info->n_campos)
		return FALSE;
	int campo;
	for (campo = 0;campo < m_def->m_n_campos_col;campo++)
	{
		if (m_def->m_colcampos[campo] == iSubItem)
		{
			break;
		}
	}	
	if (campo >= m_def->m_n_campos_col)
		return FALSE;
	iSubItem = campo + COLUMNA_INICIAL;
}
if (iSubItem < 0 || iSubItem >= m_columnas) 
   return FALSE;

if( !EnsureVisible( iItem, TRUE ) ) 
   return FALSE;

int i;
int pi;
int offset = 0;
int offini = 0;

CRect rect;
GetItemRect( iItem, &rect, LVIR_BOUNDS );

CRect rcClient;
GetClientRect( &rcClient );

GetColumnOrderArray((LPINT)m_orderarray,m_columnas);

int hh;
for( pi = 0; pi < m_columnas ; pi++ )
{

     hh = GetColumnWidth( m_orderarray[pi] );                    
     if ( ((rect.left+offini) >= rcClient.left && (rect.left+offini) <= rcClient.right) || ((rect.left+offini+hh) >= rcClient.left && (rect.left+offini+hh) <= rcClient.right) )
     {
         break;
     }
	 offini += hh;
}

for( i = 0; m_orderarray[i] != iSubItem; i++ )
	offset += GetColumnWidth( m_orderarray[i] );
int colwidth = GetColumnWidth( iSubItem );


if ((offset + colwidth + rect.left) > rcClient.right)
{
	CSize size;
    int i2;
    int off2 = 0;

    for (i2 = pi;i2 < m_columnas && i2 != i;i2++)
    {
        if ((offset + colwidth + rect.left - off2) <= rcClient.right) break;
        off2 += GetColumnWidth( m_orderarray[i2] );
    }

	size.cx = off2;
	size.cy = 0;
	Scroll( size );
    rect.left -= size.cx;
    rect.right -= size.cx;	
}

if( (offset + rect.left) < 0)
 {
	CSize size;
	size.cx = offset + rect.left + 1;
	size.cy = 0;
	Scroll( size );		
    rect.left -= size.cx;
    rect.right -= size.cx;	
 }		

rect.left += (offset+1);
rect.right = rect.left + colwidth - 1;
if( rect.right > rcClient.right) 
	rect.right = rcClient.right;
*prect = rect;

return 1;	
}


void CTablaView::EnviaDato(CString &str)
{
	    char *p = m_buffer;
        int operacion = 2;
        long rr;
        
        memcpy((void *)p,(void *)&m_localizador,sizeof(int));
        p += sizeof(int);
        
        long tam = (sizeof(long) * 5) + (sizeof(int)*2) + 1 + strlen(str) + 1 + 500;
        long respuesta = tam;
        int modo = 1;
        
        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(long);
        int campo = m_curcol - COLUMNA_INICIAL;
        memcpy((void *)p,(void *)&campo,sizeof(int));
        p += sizeof(int);
        strcpy(p,str);

        DespachadorInterno(13,(void *)m_buffer);
       

       p -= sizeof(int);

       memcpy((void *)&rr,(void *)p,sizeof(long));

       p += sizeof(long);
       
       if (rr > 0)
       {
           BorraCache();
		   m_sincachear++;	
           if (*((long *)p) != m_registros)
           {
			    PonTotalRegistros(*((long *)p));
           }                  
		   m_sincachear--;	
	       m_refrespending = FALSE;
           InvalidateRect(NULL);
		   PostMessage(WM_COMMAND,ID_GRID_POSICION,(LPARAM)(rr-1));
		   //ActualizaVentana();
       }
}


BOOL CTablaView::Scroll(CSize size)
{
   if (m_editando)
       return FALSE;   

   BOOL r = BASELISTCTRL::Scroll(size);

   return r;
}

BOOL CTablaView::OnEraseBkgnd(CDC* pDC) 
{
	if (!m_pDoc || !m_registros || m_primerapintada)
	{
		m_primerapintada = FALSE;
		return BASELISTCTRL::OnEraseBkgnd(pDC);
	}
	CRect r0,ru1,ru2,ru3,ru4;
	BOOL exclusion = FALSE;
	GetClientRect(r0);
	
    if (m_registros)
    {		
        int ini = GetTopIndex();		
        CRect ru;        
        CRect r1;		
		GetItemRect(ini,r1,LVIR_BOUNDS);
		ru = r1;
		ru.right = r0.right;
		ini++;
        for (;ini < m_registros;ini++)
        {
            GetItemRect(ini,r1,LVIR_BOUNDS);
            if (r1.top >= r0.bottom) 
               break;
			ru.bottom = r1.bottom;
        }		
		/*
        if (ini < m_registros)
        {
            return TRUE;
        }
		*/
		exclusion = TRUE;
		ru1.top = r0.top;
		ru1.left = r0.left;
		ru1.bottom = ru.top;
		ru1.right = r0.right;
		ru2.top = ru.top;
		ru2.left = r0.left;
		ru2.bottom = ru.bottom;
		ru2.right = ru.left;
		ru3.top = ru.top;
		ru3.left = ru.right;
		ru3.bottom = ru.bottom;
		ru3.right = r0.right;
		ru4.top = ru.bottom;
		ru4.left = r0.left;
		ru4.bottom = r0.bottom;
		ru4.right = r0.right;
    }

	if (exclusion)
	{
		pDC->FillSolidRect( ru1,m_pDoc->m_cGridBk);	
		pDC->FillSolidRect( ru2,m_pDoc->m_cGridBk);	
		pDC->FillSolidRect( ru3,m_pDoc->m_cGridBk);	
		pDC->FillSolidRect( ru4,m_pDoc->m_cGridBk);	
	}
	else
	{
		pDC->FillSolidRect( r0,m_pDoc->m_cGridBk);	
	}
	return TRUE;
    //return BASELISTCTRL::OnEraseBkgnd(pDC);
}


BOOL CTablaView::PreTranslateMessage(MSG* pMsg) 
{
	if (!m_editando && pMsg->message == WM_KEYDOWN)
    {
        if (VK_RETURN == (int)pMsg->wParam)
        {
               if ((m_gmodo & 1))
               {
                   MantenimientoLinea();
               }
               else
               if (!(m_gmodo & 0x20000))
               {
                   TeclaStd(802);                   
               }
               else
               {                    
                    SendMessage(WM_COMMAND,ID_AVISO_POSTGRIDPOSICIONA);                    
                    
                    if (m_principal)
                    {
                        m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
                    }
                    else
                        TeclaStd(802);
               }
               return TRUE;
        }
        else if (VK_ESCAPE == (int)pMsg->wParam)
        {
			if ( !(m_gmodo & 0x8000000) ) 
			{
				if ( (m_gmodo & 0x10000) ) // modo 'hijo'
				{                
					if (m_principal)
					{
						m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
					}
					else
						TeclaStd(806);
				}
				else
					TeclaStd(806);
			}
            return TRUE;
        }
		else if (VK_CANCEL == (int)pMsg->wParam  || (pMsg->wParam == 'Q' && (::GetKeyState( VK_CONTROL ) & 0x8000)))
		{
			if ( !(m_gmodo & 0x8000000) ) 
			{
				if ( (m_gmodo & 0x10000) ) // modo 'hijo'
				{                
					if (m_principal)
					{
						m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
					}
					else
						TeclaStd(807);
				}
				else
					TeclaStd(807);
			}
            return TRUE;
		}
    }
	
	return BASELISTCTRL::PreTranslateMessage(pMsg);
}



BOOL CTablaView::NotificaPosicion()
{
long tam;
long respuesta;
long modo = 1;
long registro;
int operacion = 1;
char *p2,*p;

if (!m_buffer)
   return FALSE;

if ( (m_gmodo & 0x10000))
{
	modo = 1;
}

p2 = NULL;

int oitem = m_cursel;
if (m_reordenado > -1 && m_reorden)
{
	oitem = m_reorden[oitem];
}

if ((!m_cache.Lookup( oitem , p2 ) || !p2) && (m_gmodo & 0x10000) && m_padre > -1)
{	
	int ant = m_sincachear;
	m_sincachear = 0;
	Cacheamelo(oitem,oitem);
	m_sincachear = ant;
	if (!m_cache.Lookup( oitem , p2 ))
	{
		p2 = NULL;
	}	
}

if (p2)
{
    long reg;
    
    memcpy((void *)(&registro),(void *)(p2+m_lenreg),sizeof(long));

    p = m_buffer;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 6) + (sizeof(int)*2) + 2 + m_lenreg;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);
    memcpy((void *)p,(void *)&registro,sizeof(long));
    p += sizeof(long);        
    registro = (long)(oitem+1);
    memcpy((void *)p,(void *)&registro,sizeof(long));
    p += sizeof(long);
    int curcol = m_curcol - COLUMNA_INICIAL;
    memcpy((void *)p,(void *)&curcol,sizeof(int));
    p += sizeof(int);
    memcpy((void *)&reg,(void *)(p2+m_lenreg),sizeof(long));
    memcpy((void *)p,(void *)&reg,sizeof(long));
    p += sizeof(long);

    memcpy(p,p2,m_lenreg);
    p += m_lenreg;

    *p = 0;                     
    if (DespachadorInterno(13,(void *)m_buffer) < 0)
	{
		exit(0);
	}

	//PostMessage(WM_COMMAND,ID_AVISO_POSTGRIDENSUREVISIBLE,(LPARAM)3);
    //TeclaStd(TECLA_PING);

    CTablaW *pwpadre = NULL;            
    if (m_padre > -1)
    {
      CDsStdCtrl *pw;
      CDsStdDlg::m_todosctrl.Lookup(m_padre,pw);
      if (pw)
      {
          pwpadre = (CTablaW *)pw->m_ctrl;
      }
    }

	
    if ( (m_gmodo & 0x10000) && pwpadre)
    {
        if (!(pwpadre->m_gmodo & 1) )
        {
           reg = RegistroDelPadre(reg);
           pwpadre->m_tabla.PostMessage(WM_COMMAND,ID_AVISO_GRIDPOSICIONA,(LPARAM)reg);
        }
        else
        {
           m_ultpospadre = reg;
        }
    }	
	
    return TRUE;
    }

m_NotificaOnDraw |= 8;
return FALSE;
}


static CTablaView *pOrdenTabla = NULL;
static int cOrdenOffset = 0;
static int cOrdenLen    = 0;
static int cOrdenTipo   = 0;
int compare_orden_string( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   int i1 = * ( int* ) arg1;
   int i2 = * ( int* ) arg2;
   char *p1 = NULL;
   char *p2 = NULL;

   pOrdenTabla->m_cache.Lookup( i1 , p1);
   pOrdenTabla->m_cache.Lookup( i2 , p2);
   if (!p1 || !p2)
	   return 0;
   return memcmp(p1+cOrdenOffset,p2+cOrdenOffset,cOrdenLen);
}
int compare_orden_fecha( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   int i1 = * ( int* ) arg1;
   int i2 = * ( int* ) arg2;
   char *p1 = NULL;
   char *p2 = NULL;
   char f1[10];
   char f2[10];

   pOrdenTabla->m_cache.Lookup( i1 , p1);
   pOrdenTabla->m_cache.Lookup( i2 , p2);
   if (!p1 || !p2)
	   return 0;
   p1+=cOrdenOffset;
   p2+=cOrdenOffset;
   memcpy(f1  ,p1+6,4);
   memcpy(f1+4,p1+3,2);
   memcpy(f1+6,p1  ,2);

   memcpy(f2  ,p2+6,4);
   memcpy(f2+4,p2+3,2);
   memcpy(f2+6,p2  ,2);

   return memcmp(f1,f2,8);
}

int memcpys_limpio(char* buff,LPCSTR clave,int cOrdenLen) {
	int i,j = 0;
	for (i = 0;i < cOrdenLen;i++) {
		if (clave[i] != '.') {
			if (clave[i] == ',')
				buff[j++]='.';
			else
				buff[j++]=clave[i];
		}
	}
	buff[j] = 0;
	return j;
}

int CompareBuscaClave(LPCSTR clave,char **array, int ini, int fin)
{   
   int comp = fin-ini;
   int medio = comp / 2;  
   char f1[10];
   char f2[10];
   char buff[128];
   double dclave,dotro;
   
   medio += ini;

   if (!comp)
   {
	   return medio;
   }  

   if (cOrdenTipo == 5)
   {
	   memcpy(f1  ,clave+6,4);
	   memcpy(f1+4,clave+3,2);
	   memcpy(f1+6,clave  ,2);
   } else if (cOrdenTipo != 0) {
	   memcpys_limpio(buff,clave,cOrdenLen);	   
	   dclave = atof(buff);
   }

   if (comp == 1)
   {
	   if (cOrdenTipo == 5)
	   {		   
		   memcpy(f2  ,array[medio]+6,4);
		   memcpy(f2+4,array[medio]+3,2);
		   memcpy(f2+6,array[medio]  ,2);
		   if (memcmp(f2,f1,8) <= 0)
		   {
			   return medio+1;
		   }
		   return medio;
	   }
	   else
	   {
		   if (cOrdenTipo == 0) {
			   if (_strnicmp(array[medio],clave,cOrdenLen) <= 0)
			   {
			 	 return medio+1;
			   }
		   } else {
				memcpys_limpio(buff,array[medio],cOrdenLen);				
				dotro = atof(buff);
				if (dotro <= dclave)
					return medio+1;
		   }
		   return medio;
	   }
   }
   if (cOrdenTipo == 5)
   {		   
	    memcpy(f2  ,array[medio]+6,4);
	    memcpy(f2+4,array[medio]+3,2);
	    memcpy(f2+6,array[medio]  ,2);
		comp = memcmp(f2,f1,8);
   }
   else
   {
	    if (cOrdenTipo == 0)
			comp = _strnicmp(array[medio],clave,cOrdenLen);
		else {
			memcpys_limpio(buff,array[medio],cOrdenLen);			
			dotro = atof(buff);
			if (dotro < dclave)
				comp = -1;
			else if (dotro > dclave)
				comp = 1;
			else
				comp = 0;
		}
   }
   if (comp < 0)
   {
	   return CompareBuscaClave(clave,array,medio+1,fin);
   }
   if (comp > 0)
   {
	   return CompareBuscaClave(clave,array,ini,medio);
   }  
   return medio+1;
}


BOOL CTablaView::CambiarOrden(int clave)
{
    long tam;
    long respuesta;
    long modo = 1;            
    int operacion = 3;       
    int nuevo = -1;
    

    if (clave == -1) 
	{
		return FALSE;
	}

	if (m_reordenado == clave)
	{
		return FALSE;
	}

	if (clave == m_def->m_info->clave_actual)
	{
		if (m_reordenado > -1)
		{
			FinReorden();
			ActualizaVentana();
			//InvalidateRect(NULL);
		}
	}

	if ((m_gmodo & 256) && clave != m_def->m_info->clave_actual)
	{
		return FALSE;
	}

	FinReorden();

	if (clave < 0)
	{
		int lenta = 0;
		{ 
		   char valor[512];
		   strcpy(valor,"CONEXION_LENTA");
		   if (llama_fDespachadorInterno(pDsSys->m_f9DespachadorInterno,22,(void *)valor) >= 0)
		   {
			   lenta = atoi(valor);
		   }
		}	    
		clave = (-clave) - 2;
	    if (!lenta && m_registros < MAX_ORDENACION_LOCAL)
		{
		   /* lo haremos en memoria */
		   int *elorden;
		   char **auxiliar;
		   int campo;		   		   
		   
		   campo = clave - COLUMNA_INICIAL;
		   if (campo < 0)
			   return FALSE;

		   PonWait();
		   Cacheamelo(-1,-1);
		   m_reordenado = clave;		   
		   elorden = new int[m_registros+3];
		   auxiliar = new char *[m_registros+3];
		   pOrdenTabla = this;		   
		   cOrdenOffset = m_poscampos[campo];
		   cOrdenLen    = m_tamcampos[campo];
		   cOrdenTipo   = m_def->Campo(campo)->c_tipo;
		   char *p1;
		   int n;
		   register int j;
		   int i;
		   for (i = 0;i < m_registros;i++)
		   {
			   if (!m_cache.Lookup( i , p1))
			   {
				   /*
				   Cacheamelo(i,i);
				   if (!m_cache.Lookup( i , p1))
				   */
				   {				   
					   ASSERT(FALSE);
					   delete [] elorden;
					   delete [] auxiliar;
					   m_reordenado = -1;
					   QuitaWait();
					   return FALSE;
				   }				   
			   }
			   p1 += cOrdenOffset;
			   if (!i)
			   {
				   auxiliar[i] = p1;
				   elorden[i] = i;
			   }
			   else
			   {
				   n = CompareBuscaClave(p1,auxiliar,0,i);
				   for (j = i;j > n;j--)
				   {
					   auxiliar[j] = auxiliar[j-1];
					   elorden[j]  = elorden[j-1];
				   }
				   auxiliar[n] = p1;
				   elorden[n] = i;
			   }		   
		   }
	       auxiliar[i] = NULL;
		   elorden[i] = i;
		   i++;
	       auxiliar[i] = NULL;
		   elorden[i] = i;
		   /*
		   if (m_def->Campo(campo)->c_tipo == 5)
		   {
			   qsort( (void *)elorden,m_registros,sizeof(int),compare_orden_fecha);
		   }
		   else
		   {
			   qsort( (void *)elorden,m_registros,sizeof(int),compare_orden_string);
		   }
		   */
		   delete [] auxiliar;
		   m_reorden = elorden;
		   m_def->m_info->clave_actual = -1;
		   MarcasCabecera();
		   ActualizaVentana();
		   //InvalidateRect(NULL);
		   QuitaWait();
		   GuardaPosiciones();

		   NotificaPosicion();

		   return TRUE;
		}
	    else
		{
		   clave = -(m_def->Campo(clave-COLUMNA_INICIAL)->Id + 2);
		}
	}

	PonWait();	

	BorraCache();

    char *p = m_buffer;           

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 6) + sizeof(int) + 1 + 500; /* se reserva un par de long mas de vuelta ...*/
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);                
    memcpy((void *)p,(void *)&clave,sizeof(int));

    DespachadorInterno(13,(void *)m_buffer);
    
    memcpy((void *)&clave,(void *)p,sizeof(int));	

	if (clave == -9999)
	{
		m_reordenado = -1;
		m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_localizador);
		QuitaWait();
		return TRUE;
	}	
    else if (clave != -1)
    {
		//LockWindowUpdate();
		m_reordenado = -1;

		m_sincachear++;

		if (clave < -1)
		{
			clave = (-clave)-2;
			if (clave >= 0 && clave < m_def->m_info->t_total_campos)
			{	
				nuevo = m_def->m_idcampos[clave];
				for (clave = 0;clave < m_def->m_info->t_total_campos;clave++)
				{
					if (nuevo == m_def->m_colcampos[clave])
					{
						nuevo = clave;
						break;
					}
				}
				nuevo += COLUMNA_INICIAL;				
				m_reordenado = -(nuevo)-2;
			}			
		}
		else
		{
			int i;
			m_def->m_info->clave_actual = clave;
			for (i = 0;i < m_def->Clave(clave)->n_componentes;i++)
			{
				nuevo = m_def->k_campo(clave,i,FALSE);
				if (nuevo >= 0)
				{
					nuevo += COLUMNA_INICIAL;
					break;
				}
			}
		}
		long reg_nue;

		p += sizeof(int);

		memcpy((void *)&reg_nue,(void *)p,sizeof(long));
		p += sizeof(long);
		reg_nue--;
		
		if (*((long *)p) != m_registros)
		{
			PonTotalRegistros(*((long *)p));
		}		
		
		if (m_cursel != reg_nue)
		{
			NuevoCorriente(reg_nue,TRUE);
		}		

		if (nuevo > -1 && nuevo != m_curcol)
		{
			CambiaCorriente(nuevo);
		}

		MarcasCabecera();

		m_sincachear--;

		//UnlockWindowUpdate();
		m_refrespending = FALSE;

		InvalidateRect(NULL);

		EnsureVisible(m_cursel,TRUE);

        /*NotificaPosicion();*/

		QuitaWait();
		GuardaPosiciones();

        return TRUE;
    }	
	QuitaWait();
return FALSE;
}


BOOL CTablaView::Refresca()
{
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 11;
    char *p;
    char tmp[512];
	static int recursion = 0;

	if (recursion)
	{
		return FALSE;
	}
	recursion++;
    

	BorraCache();

	if (m_totalizadores)
		m_totbpedirdatos = TRUE;

	//m_sincachear++;

    p = tmp;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 7) + (sizeof(int)*2) + 100;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);    

    DespachadorInterno(13,(void *)tmp);
    
    long registros;
    long reg_nue;
    memcpy((void *)&registros,(void *)p,sizeof(long));
    p += sizeof(long);
    memcpy((void *)&reg_nue,(void *)p,sizeof(long));
    reg_nue--;    

    if (registros != m_registros)
    {
		PonTotalRegistros(registros);
    }

    // Cacheamelo(GetTopIndex(),GetTopIndex()+GetCountPerPage()); //Revisar corriente
	
    if (m_cursel != reg_nue)
    {		
		/*
		if (m_def && !strcmp(m_def->m_RegPath,"d__ds_dspreven_def_pycm0004"))
		{
			int i ;
			i = 0;
		}
		*/
        NuevoCorriente(reg_nue);
    } 
	

	//m_sincachear--;

    ActualizaVentana();

	if (!NotificaPosicion())
    {
        m_NotificaOnDraw |= 8;
    }

	recursion--;

    return TRUE;
}

void CTablaView::InvalidateRect( LPCRECT lpRect, BOOL bErase)
{
	BASELISTCTRL::InvalidateRect( lpRect, bErase);
	UpdateWindow();
}

void CTablaView::ActualizaVentana()
{	
	int n;

	if (m_guardaposiciones)
	{					
		m_guardaposiciones = FALSE;		
		AjustaColumnaFinal();
		GuardaPosiciones();
		InvalidateRect(NULL);
		return;
	}

	n = GetItemCount();
	if (n == (m_gmodo & 1))
	{
		if (m_tw)
			m_tw->InvalidateRect(NULL);
		InvalidateRect(NULL);
	}
	else
	{
		int ini_iItem = GetTopIndex( );
		int fin_iItem = ini_iItem + GetCountPerPage();
		RedrawItems(ini_iItem,fin_iItem);		
		/*UpdateWindow();*/
	}
}

BOOL CTablaView::NuevoCorriente(int nuevo,BOOL posicionalo)
{
   if (m_cursel == nuevo) 
	   return FALSE;

   int ant = m_cursel;
   int dif;   

   if (nuevo == -1)
   {
	   POSITION pos;
	   nuevo = m_cursel;
	   pos = GetFirstSelectedItemPosition( );
	   if (pos)
	   {
		   ant = GetNextSelectedItem(pos);
	   }
   }   
   
   dif = m_cursel - GetTopIndex( );
   m_cursel = nuevo;   
   m_sincachear++;
   m_yomismo++;
   
   //unsigned long cuentaupdate = m_cuentaupdate;
   
   SetItemState( m_cursel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED| LVIS_FOCUSED );   
   m_yomismo--;
   /*
   if (cuentaupdate == m_cuentaupdate)
   {
	   NM_LISTVIEW hdr;	   
	   LRESULT Result;
	   memset((void *)&hdr,0,sizeof(hdr));
	   hdr.uNewState = LVIS_SELECTED | LVIS_FOCUSED;
	   hdr.iItem = m_cursel;
	   OnItemchanged((NMHDR *)&hdr, &Result);
   }
   */
   if (posicionalo)
   {	   
	   if (ant == m_cursel)
	   {
		   EnsureVisible(m_cursel,TRUE);
	   }
	   else
	   {
		   CSize s;
		   CRect rect;
		   GetItemRect( m_cursel, &rect, LVIR_BOUNDS );
		   s.cx = 0;
		   s.cy = (m_cursel - ant) * rect.Size().cy;
		   //SetRedraw( FALSE );
		   Scroll(s);
		   //SetRedraw( TRUE );		   
	   }
	   m_sincachear--;
	   if (m_refrespending)
	   {
		   m_refrespending = FALSE;
		   Invalidate();
	   }
       return TRUE;
   }
   /*}*/
   m_sincachear--;
   if (m_refrespending)
   {
		m_refrespending = FALSE;
		Invalidate();
   }
   return FALSE;
}

long CTablaView::RegistroDelPadre(long reghijo)
{
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 6;
    char *p;
    char tmp[512];
    

    p = tmp;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 7) + (sizeof(int)*2) + 100;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);
    memcpy((void *)p,(void *)&reghijo,sizeof(long));    

    DespachadorInterno(13,(void *)tmp);
    
    memcpy((void *)&reghijo,(void *)p,sizeof(long));
    
    return reghijo;
}

BOOL CTablaView::GuardaPosiciones()
{
	int j;
	CString s,o;
	CString tam;

	GetColumnOrderArray((LPINT)m_orderarray,m_columnas);
	//CString dd = theApp.WriteProfileString("GridDefaultOrder",cc,"");
	s = "";
	for (j = COLUMNA_INICIAL;j < (m_columnas-COLUMNA_FINAL);j++)
	{            
		o.Format("%03d",m_def->m_colcampos[m_orderarray[j]-COLUMNA_INICIAL]);
		s += o;
		o.Format("%04d",GetColumnWidth(m_orderarray[j]));
		tam += o;
	}
	CString cc;

	m_tw->GetWindowText(cc);

	cc += " ";
	cc += m_def->m_RegPath;

	theApp.WriteProfileString("GridDefaultOrder",cc,s);			
	theApp.WriteProfileString("GridDefaultTaman",cc,tam);
	

	if (m_guarda_posicion)
	{
		/*
		s.Format("%3d",m_reordenado);
		theApp.WriteProfileString("GridDefaultOrden",cc,s);		
		*/

		s.Format("%3d",m_curcol);
		theApp.WriteProfileString("GridDefaultCol",cc,s);	
	}

	return TRUE;
}


BOOL CTablaView::OpcionesConsulta(POINT pp)
{
	if ((m_gmodo & 0x4000000))
	{
		return FALSE;
	}
    CMenu menu,claves,clavesc,Orden,filtros;   
    if (!menu.CreatePopupMenu( )) return FALSE;    
    if (!claves.CreatePopupMenu( )) return FALSE;    
    if (!clavesc.CreatePopupMenu( )) return FALSE; 
    if (!Orden.CreatePopupMenu( )) return FALSE;
	if (!filtros.CreatePopupMenu( )) return FALSE;

    if (!(m_gmodo & 1))
    {
		if (!(m_ini_gmodo & 1))
		{			
			
			menu.InsertMenu(0,MF_STRING | MF_BYPOSITION,ID_GRID_RESTAURA,"Restaurar Predeterminados");
			menu.InsertMenu(1,MF_STRING | MF_BYPOSITION,ID_CONSULTA_TODACOLUMNAS,"Todas las columnas");
			menu.InsertMenu(2,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);    			
			/*
			if ((m_gmodo & 128))
			{
				menu.InsertMenu(2,MF_STRING | MF_BYPOSITION | MF_POPUP | MF_GRAYED,(int)Orden.m_hMenu,"Consultar por Clave");
			}
			else
			{
				menu.InsertMenu(2,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)Orden.m_hMenu,"Consultar por Clave");
			}
			menu.InsertMenu(3,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
			*/
			menu.InsertMenu(3,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)filtros.m_hMenu,"Filtro avanzado");
			
			//Orden.InsertMenu(0,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)claves.m_hMenu,"Exacta");

			menu.InsertMenu(4,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
			/*
			menu.InsertMenu(6,MF_STRING | MF_BYPOSITION | MF_GRAYED,-1,"CONSULTA ASIMETRICA");
			*/
			menu.InsertMenu(5,MF_STRING | MF_BYPOSITION,1,"Ordenar");
			
			menu.InsertMenu(6,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
			menu.InsertMenu(7,MF_STRING | MF_BYPOSITION,2,"Buscar");
			menu.InsertMenu(8,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
			menu.InsertMenu(9,MF_STRING | MF_BYPOSITION,3,"Operaciones");
			menu.InsertMenu(10,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
			menu.InsertMenu(11,MF_STRING | MF_BYPOSITION,144,"Consultar registro de cambios");

			/*
			if (m_reordenado > -1)
			{
				menu.InsertMenu(8,MF_STRING | MF_BYPOSITION,ID_GUARDA_POSICIONES,"Guardar ordenacion sin clave");
				menu.InsertMenu(9,MF_STRING | MF_BYPOSITION,ID_GUARDA_POSICIONES2,"Anular ordenacion grabada sin clave");
			}
			Orden.InsertMenu(0,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)claves.m_hMenu,"Exacta");
			Orden.InsertMenu(1,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)clavesc.m_hMenu,"Parcial");
			*/
		}
		else
		{
			/*(int)claves.m_hMenu*/
			if ((m_gmodo & 128))
			{
				menu.InsertMenu(0,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)filtros.m_hMenu,"Consultar");
			}
			else
			{
				menu.InsertMenu(0,MF_STRING | MF_BYPOSITION /*| MF_POPUP*/,10000+m_def->m_info->clave_actual,"Consultar");
			}			
		}
    }
    else
    {	
		/*(int)Orden.m_hMenu*/
		if ((m_gmodo & 128))
		{
			menu.InsertMenu(0,MF_STRING | MF_BYPOSITION | MF_POPUP,(int)filtros.m_hMenu,"Consultar");			
		}
		else
		{
			menu.InsertMenu(0,MF_STRING | MF_BYPOSITION /*| MF_POPUP*/,10000+m_def->m_info->clave_actual,"Consultar");
		}
        menu.InsertMenu(1,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
        menu.InsertMenu(2,MF_STRING | MF_BYPOSITION,ID_RECORD_UPDATE,"Modificar");
        menu.InsertMenu(3,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
        menu.InsertMenu(4,MF_STRING | MF_BYPOSITION,ID_RECORD_NEW,"Insertar");    
        menu.InsertMenu(5,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
        menu.InsertMenu(6,MF_STRING | MF_BYPOSITION,ID_RECORD_DELETE,"Borrar");
    }
	
	{
		CDlgGridFiltro dlg(this);
		filtros.InsertMenu(0,MF_STRING | MF_BYPOSITION,ID_CONSULTA_FILTRO,"Mantenimiento");		
		dlg.RellenaMenu(&filtros,1,100000);		
	}

    //claves.m_hMenu
	/*
    CString s;
    int camp;
    int conts = 0;
    int npos = 0;
	int cc;
	int i;
    for (i = 0;i < m_def->m_info->n_claves;i++)
    {
        camp = m_def->k_campo(i,0);

        if (camp < 0) continue;

        s = m_def->c_nombre(camp);
        for (int j = 1;j < m_def->Clave(i)->n_componentes;j++)
        {
			cc = m_def->k_campo(i,j);
			if (cc > -1)
			{
				s += " + ";
				s += m_def->c_nombre(cc);
			}
        }
        
        if (!(m_gmodo & 1) && !m_def->Campo(camp)->c_tipo)
        {
            clavesc.InsertMenu(conts,MF_STRING | MF_BYPOSITION,20000+i,s);
            conts++;
        }

	    if (i == m_def->m_info->clave_actual)
			continue;
        
        if (!(m_gmodo & 1))
            claves.InsertMenu(npos,MF_STRING | MF_BYPOSITION,10000+i,s);
        else
            Orden.InsertMenu(npos,MF_STRING | MF_BYPOSITION,10000+i,s);
        npos++;
    }
	*/
    int r = (int)menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y,this,NULL);
	if (r == 144) {
		DlgRegCambio dlgcambio(this);
		if (dlgcambio.DoModal() == IDOK) {
			EjecutaRegCambio(&dlgcambio);
		}
	}
    else if (r == ID_GUARDA_POSICIONES)
    {
		{
			CString s;
			CString cc;

			m_tw->GetWindowText(cc);

			cc += " ";
			cc += m_def->m_RegPath;

			s.Format("%3d",m_reordenado);
			theApp.WriteProfileString("GridDefaultOrden",cc,s);			
		}
    }
    else if (r == ID_GUARDA_POSICIONES2)
    {		
		CambiarOrden(m_def->m_info->clave_defecto);
		{
			CString s;
			CString cc;

			m_tw->GetWindowText(cc);

			cc += " ";
			cc += m_def->m_RegPath;

			s.Format("%3d",m_reordenado);
			theApp.WriteProfileString("GridDefaultOrden",cc,s);			
		}
	}
	else if (r == ID_CONSULTA_TODACOLUMNAS)
	{
		m_curcol = 0;
		BorraCache();
		m_def->CreaCamcolu(TRUE,TRUE);
		EnviaCamcolu();
		CreaColumnas();
		InvalidateRect(NULL);
		GuardaPosiciones();
	}
    else if (r == ID_GRID_RESTAURA)
    {
		if (AfxMessageBox("¿Seguro que desea Restaurar Predeterminados?",MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_curcol = 0;
			BorraCache();
			m_def->CreaCamcolu(TRUE);
			EnviaCamcolu();
			CreaColumnas();
			InvalidateRect(NULL);
			GuardaPosiciones();
		}
    }
    else if (r == ID_CONSULTA_FILTRO)
    {
        Filtros();
    } 
    else if (r == ID_RECORD_UPDATE || r == ID_RECORD_NEW || r == ID_RECORD_DELETE)
    {
        TeclaStd(r);
    }
	else if (r >= 100000)
	{		
		CDlgGridFiltro dlg(this);
		if (r >= 101000)		
			dlg.m_registrolocal = FALSE;		
		else
			dlg.m_registrolocal = TRUE;
		filtros.GetMenuString( r,dlg.m_nombre,MF_BYCOMMAND);
		if (dlg.LeeDelRegistro())
		{
			dlg.RellenaListado(TRUE);
			EjecutaFiltro(&dlg);
		}
		else
		{
			AfxMessageBox("Filtro erroneo, acceda a la opción de Mantenimiento");
		}
	}
    else if (r >= 10000 && r < 100000)
    {
        int contenido = 0;
        if (r >= 20000)
        {
            contenido = 1;
            r -= 10000;
        }
        if (!(m_gmodo & 1) && !(m_gmodo & 8) && !(m_gmodo & 256))
        {
		   //if (m_curcol != 
		   //CambiaCorriente(int nuevo,int salto)
           if (!CambiarOrden(r-10000))
           {
			   //ActualizaVentana();
               //InvalidateRect(NULL);
           }
           m_NotificaOnDraw |= 2; // fijo que repinta
           if (contenido)
           {
               m_NotificaOnDraw |= 4;
           }
		   ActualizaVentana();
           return TRUE;            
        }
        else
        {
			r -= 10000;
			if (1/*r != m_def->m_info->clave_actual*/)
			{
				char *p;
				long tam;
				long respuesta;
				long modo = 1;
				int operacion = 7;
				char tmp[512];            
				p = tmp;

				memcpy((void *)p,(void *)&m_localizador,sizeof(int));
				p += sizeof(int);

				tam = (sizeof(long) * 4) + (sizeof(int)*4) + 100;
				respuesta = tam;

				memcpy((void *)p,(void *)&tam,sizeof(long));
				p += sizeof(long);	
				memcpy((void *)p,(void *)&respuesta,sizeof(long));
				p += sizeof(long);
				memcpy((void *)p,(void *)&modo,sizeof(long));
				p += sizeof(long);
				memcpy((void *)p,(void *)&operacion,sizeof(int));
				p += sizeof(int);
            
				memcpy((void *)p,(void *)&r,sizeof(int));
				p += sizeof(int);

				DespachadorInterno(13,(void *)tmp);
			}
        }
    }   
	else if (r == 1)
	{
		CDlgOrdenar dlg(this);
		dlg.m_col = -1;
		if (dlg.DoModal() == IDOK)
		{
			CDlgGridFiltro dlgf(this);
			CString s;
			int j;
			for (int i = 0;i < m_def->m_n_campos_col;i++)
			{
				j = m_def->m_colcampos[i];
				dlgf.m_principal->m_item[j]->m_checked = TRUE;
			}
			if (dlg.m_sel1 > -1)
			{
				dlgf.m_principal->m_item[dlg.m_sel1]->m_Orden = TRUE;
				dlgf.m_principal->m_item[dlg.m_sel1]->m_OrdenInverso = dlg.m_inver1;
				s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel1]->m_padre->m_nombre,dlg.m_sel1);
				dlgf.OrdenId(0,s);
			}
			if (dlg.m_sel2 > -1)
			{
				dlgf.m_principal->m_item[dlg.m_sel2]->m_Orden = TRUE;
				dlgf.m_principal->m_item[dlg.m_sel2]->m_OrdenInverso = dlg.m_inver2;
				s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel2]->m_padre->m_nombre,dlg.m_sel2);
				dlgf.OrdenId(1,s);
			}
			if (dlg.m_sel3 > -1)
			{
				dlgf.m_principal->m_item[dlg.m_sel3]->m_Orden = TRUE;
				dlgf.m_principal->m_item[dlg.m_sel3]->m_OrdenInverso = dlg.m_inver3;
				s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel3]->m_padre->m_nombre,dlg.m_sel3);
				dlgf.OrdenId(2,s);
			}
			
			dlgf.RellenaListado(TRUE);
			EjecutaFiltro(&dlgf);
		}
	}
	else if (r == 2)
	{
		CDlgBusqueda dlg(this);
		dlg.m_col = -1;
		if (dlg.DoModal() == IDOK)
		{
			CDlgGridFiltro dlgf(this);
			CString s;
			int j;
			for (int i = 0;i < m_def->m_n_campos_col;i++)
			{
				j = m_def->m_colcampos[i];
				dlgf.m_principal->m_item[j]->m_checked = TRUE;
			}
			if (dlg.m_sel > -1)
			{
				dlgf.m_principal->m_item[dlg.m_sel]->m_Orden = TRUE;
				dlgf.m_principal->m_item[dlg.m_sel]->m_LimiteI = dlg.m_limitei;
				dlgf.m_principal->m_item[dlg.m_sel]->m_LimiteS = dlg.m_limites;
				dlgf.m_principal->m_item[dlg.m_sel]->m_Valor = dlg.m_valor;
				dlgf.m_principal->m_item[dlg.m_sel]->m_TipoValor= dlg.m_tipovalor;
				dlgf.m_principal->m_item[dlg.m_sel]->m_Mayus = dlg.m_mayus;
				dlgf.m_principal->m_item[dlg.m_sel]->m_Inver = dlg.m_inversa;

				s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel]->m_padre->m_nombre,dlg.m_sel);
				dlgf.OrdenId(0,s);				
			}						
			dlgf.RellenaListado(TRUE);
			EjecutaFiltro(&dlgf);
		}					
	}
	else if (r == 3)
	{
		CDlgOperaciones dlg(this);
		dlg.m_col = -1;
		if (dlg.DoModal() == IDOK)
		{
			CDlgGridFiltro dlgf(this);
			CString s;
			int j;
			for (int i = 0;i < m_def->m_n_campos_col;i++)
			{
				j = m_def->m_colcampos[i];
				dlgf.m_principal->m_item[j]->m_checked = TRUE;
			}
			if (dlg.m_sel > -1)
			{
				dlgf.m_principal->m_item[dlg.m_sel]->m_Orden = TRUE;
				dlgf.m_principal->m_item[dlg.m_sel]->m_opera = dlg.m_opera;

				s.Format("%s[%d]",dlgf.m_principal->m_item[dlg.m_sel]->m_padre->m_nombre,dlg.m_sel);
				dlgf.OrdenId(0,s);
			}						
			dlgf.RellenaListado(TRUE);
			EjecutaFiltro(&dlgf);
		}					
	}

    return FALSE;
}

BOOL CTablaView::EdicionLocal(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    /*
   	CString str;
	str = GetItemText( ht.iItem, ht.iSubItem );
    str.Delete(0);
    LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_BEGINLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = ht.iItem;
	dispinfo.item.iSubItem = ht.iSubItem;
	dispinfo.item.pszText = (LPTSTR)((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
    */


	CString str;
	str = GetItemText( m_cursel, m_curcol );
    str.Delete(0);
    LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_BEGINLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_cursel;
	dispinfo.item.iSubItem = m_curcol;
	dispinfo.item.pszText = (LPTSTR)((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

    if (m_edit && nChar)
    {
        int shift = ::GetKeyState( VK_SHIFT ) & 128;
        int caps  = ::GetKeyState( VK_CAPITAL ) & 1;
        if (nChar >= 'A' && nChar <= 'Z' && !caps && !shift)
            nChar += 32;
        m_edit->SendMessage(WM_CHAR,nChar,MAKELPARAM((WORD)nRepCnt,(WORD)nFlags));
    }


    return TRUE;
}


void CTablaView::PintaIdCampo(int idcampo,char *dato)
{
    int c = m_def->m_idcampos[idcampo];
	int i;

    for (i = 0;i < m_def->m_n_campos_col;i++)
    {
        if (m_def->m_colcampos[i] == c)
        {            
            break;
        }
    }


    if (c > -1 && i < m_def->m_n_campos_col)
    {		
        char *p = NULL;
        int item = m_cursel;
		if (m_reordenado > -1 && m_reorden)
		{
			item = m_reorden[item];
		}
        if (m_cache.Lookup( item , p ))
        {
			int len,copiado;
			len = m_tamcampos[i];
			copiado = strlen(dato);			
			memcpy(p+m_poscampos[i],dato,strlen(dato));
			if (copiado < len)
			{
				memset(p+m_poscampos[i]+copiado,' ',len-copiado);
			}
            //strcpy(p+m_poscampos[i],dato);
            RedrawItems( item, item );
        }
    }
}


BOOL CTablaView::TeclaStd(int tecla)
{
   
    if (m_tw && m_tw->m_dsctrl)
    {
        m_tw->m_dsctrl->EnsureVisible();
    }

    switch(tecla)
    {
    case ID_RECORD_LAST:
        NuevoCorriente(m_registros-1,FALSE);
        EnsureVisible(m_cursel,TRUE);
        return TRUE;
        break;
    case ID_RECORD_FIRST:
        NuevoCorriente(0,TRUE);
        EnsureVisible(m_cursel,TRUE);
        return TRUE;
        break;
    case ID_RECORD_DELETE:
        BorraLinea();
        return TRUE;
        break;
    case ID_RECORD_NEW:
        NuevoCorriente(m_registros,FALSE);
        EnsureVisible(m_cursel,TRUE);
        MantenimientoLinea();
        return TRUE;
        break;
    case ID_RECORD_NEXT:
        NuevoCorriente(m_cursel+1,FALSE);
        EnsureVisible(m_cursel,TRUE);
        return TRUE;
        break;
    case ID_RECORD_PREV:
        NuevoCorriente(m_cursel-1,FALSE);
        EnsureVisible(m_cursel,TRUE);
        return TRUE;
        break;
    case ID_RECORD_UPDATE:
        MantenimientoLinea();
        return TRUE;
        break;
    default:
        break;
    }

  if (m_tw && m_tw->m_dsctrl)
  {
      return m_tw->m_dsctrl->TeclaStd(tecla);
  }
  return FALSE;
}

class CDSCierraMessageBox : public CWinThread
{
public:
	CString m_tituloabuscar;
	HWND m_hwnd;
	virtual int Run( );
};

void cierraboxemergencia(void *param)
{
	HWND hwnd;
	int i = 0;
	do
	{		
	    hwnd = ::FindWindow(NULL,"   "); // buscamos el messagebox
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0); // un simple close tendria que bastar
			break;
		}
		::Sleep(10);
		i++;
	} while(i < 100);
}

int CDSCierraMessageBox::Run()
{
	Sleep(100); // Tiempo para inicializar el message box

	m_hwnd = ::FindWindow(NULL,m_tituloabuscar); // buscamos el messagebox

	::PostMessage(m_hwnd,WM_CLOSE,0,0); // un simple close tendria que bastar

	m_hwnd = ::FindWindow(NULL,m_tituloabuscar);

	if (m_hwnd) // si no se ha cerrado probamos otra vez
	{
		::PostMessage(m_hwnd,WM_CLOSE,0,0);
	}

	return 0;
}

BOOL CTablaView::MantenimientoLinea()
{
    char *p;
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 10;
    char tmp[512];
	int ant_cur = m_curcol;
	static int recursion = 0;
	// /DS:"@d:/ds @-C:217.13.81.99-Md:\bp $$$ADMIN,BPM"

	if (recursion)
		return FALSE;

	recursion++;

	/*
	_beginthread(cierraboxemergencia,0,NULL);
	::MessageBox(NULL,"","   ",MB_OK);
	*/

	// Chapuza para salir del paso, por orden expresa de Aura

	/* 
	CDSCierraMessageBox *pthread;
	SECURITY_ATTRIBUTES security;

	security.nLength = sizeof(security);
	security.bInheritHandle = NULL;
	security.lpSecurityDescriptor = NULL;	
	
	pthread = (CDSCierraMessageBox *)AfxBeginThread(RUNTIME_CLASS(CDSCierraMessageBox),THREAD_BASE_PRIORITY_LOWRT,0,CREATE_SUSPENDED,&security);

	if (!pthread)
	{
		::MessageBox(NULL,"Va a entrar un registro, pulse el boton ok","Atencion",MB_OK);
	}
	else
	{
		pthread->m_bAutoDelete = TRUE;
		pthread->m_tituloabuscar = "x_x_x_x";

		pthread->ResumeThread();

		::MessageBox(NULL,"","x_x_x_x",MB_OK);
	}
	*/

	//BorraCache();

    p = tmp;	

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 4) + (sizeof(int)*4) + 100;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);

    if (DespachadorInterno(13,(void *)tmp) < 0)
	{
		exit(0);
	}

	if (m_registros != *((long *)p))
	{
		BorraCache();
		PonRegistros(*((long *)p));
	}

	
	//Invalidate();

	m_sincachear++;	
	/*
	if (theApp.m_avancelinea)
		CambiaCorriente(ant_cur);	
	*/
	if (GetItemCount() > (m_cursel+1))
	{
		if (0 /*theApp.m_avancelinea*/ )
		{
		    NuevoCorriente(m_cursel+1,TRUE);
			/* reponer 
			NuevoCorriente(m_registros,TRUE);
			*/
		    NotificaPosicion();
		}
		else
		{
			m_cursel++;/* = m_registros;*/
			m_postposi++;
			PostMessage(WM_COMMAND,ID_NUEVAPOSICIONGRID,(LPARAM)FALSE);
			PostMessage(WM_COMMAND,ID_AVISO_POSTGRIDENSUREVISIBLE,(LPARAM)0);
		}
	}
	m_sincachear--;	
	/*
	if (theApp.m_avancelinea)
		ActualizaVentana();	
	*/

	recursion--;

   if (m_refrespending)
   {
		m_refrespending = FALSE;
		Invalidate();
   }
    return FALSE;
}

BOOL CTablaView::BorraLinea()
{
    char *p;
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 12;
    char tmp[512];
    p = tmp;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 4) + (sizeof(int)*4) + 100;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);

    DespachadorInterno(13,(void *)tmp);

    BorraCache();
	ActualizaVentana();
    //InvalidateRect(NULL);

    //SetFocus();

    return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CTablaW

CTablaW::CTablaW()
{
m_rect.SetRect(0,0,300,300);
m_font = NULL;
m_gmodo = 0;
m_padre = -1;
m_padrepos.SetRectEmpty();
m_consulta = NULL;
m_seleccion = NULL;
m_botsel = -1;
m_topsel = -1;
m_dsctrl = NULL;
m_escampo = FALSE;
m_skinw = NULL;
}

CTablaW::~CTablaW()
{
    if (m_dsctrl)        
    {
        if (theApp.m_antctrlactivo == m_dsctrl)
        {
           theApp.m_antctrlactivo = NULL;
        }
        if (theApp.m_ctrlactivo == m_dsctrl)
        {
           theApp.m_ctrlactivo = NULL;
        }
    }


    if (m_font)
       delete m_font;

    CTablaW *pwpadre = NULL;            
    if (m_padre > -1)
    {
      CDsStdCtrl *pw = NULL;      
      if (CDsStdDlg::m_todosctrl.Lookup(m_padre,pw) && pw)
      {
          pwpadre = (CTablaW *)pw->m_ctrl;
      }
    }

	if (pwpadre && !m_padrepos.IsRectEmpty() && !(pwpadre->m_gmodo&4))
    {
        pwpadre->MoveWindow(m_padrepos);
    }

    if (m_consulta)
        delete m_consulta;

    if (m_seleccion)
    {
        CDsGridButton **sel = m_seleccion;
        while (*sel)
        {
            (*sel)->DestroyWindow();
            delete *sel;
            sel++;
        }
        free((void *)m_seleccion);
    }
}


BEGIN_MESSAGE_MAP(CTablaW, CWnd)
	//{{AFX_MSG_MAP(CTablaW)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EXPORTA_WORD, OnExportaWord)
	ON_COMMAND(ID_EXPORTA_EXCEL, OnExportaExcel)
	ON_COMMAND(ID_EXPORTA_PLB, OnExportaPlb)
	ON_COMMAND(ID_EXPORTA_RPT, OnExportaRpt)	
	ON_COMMAND(ID_EXPORTA_XML, OnExportaXml)
	ON_COMMAND(ID_GRIDIMPRIME, OnGridimprime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTablaW message handlers


BOOL CTablaW::Crea(CWnd *Pariente,LPCSTR Titulo,struct Info_Entra_Campos *Columnas,long registros,long inicial,int id,CFont *font,int localizador,int g_modo,RECT *rt,int padre,CDsxView *principal,char *pextra)
{
BOOL r;
CWnd *Owner = Pariente;
DWORD visible = 0;
//BOOL subela = FALSE;

//m_gmodo  bits: 0 = consulta simple  1 = borde simple 2 = ventana parcial 3 = movible 4 = ajustable

m_padre = padre;

if (!(g_modo & 0x2000000))
{
	visible = WS_VISIBLE;
}

CTablaW *pwpadre = NULL;
if (m_padre > -1)
{
  CDsStdCtrl *pw;
  CDsStdDlg::m_todosctrl.Lookup(m_padre,pw);
  if (pw)
  {
      pwpadre = (CTablaW *)pw->m_ctrl;
  }
}
if (!pwpadre)
  m_padre = padre = -1;


if (Pariente)
  {
  DWORD style = WS_CHILD | visible | WS_CLIPCHILDREN /*| WS_CLIPSIBLINGS*/ | WS_TABSTOP;
  DWORD styleex = 0;
  if (Pariente == (CWnd *)theApp.StdView() && Pariente != (CWnd *)theApp.StdView(TRUE))
  {	 
	  Pariente  = (CWnd *)theApp.StdView(TRUE);
  }
  if (! (g_modo & 4) )
  {
      
      if (m_padre > -1 && (g_modo & 0x10000) )
      {
          if (!(pwpadre->m_gmodo & 1) && !(pwpadre->m_gmodo  & 8))
          {
              CRect pr;
              Pariente->GetWindowRect(m_rect);
              pwpadre->GetWindowRect(pr);
              pr.top    -= m_rect.top;
              pr.bottom -= m_rect.top;
              pr.left   -= m_rect.left;
              pr.right  -= m_rect.left;

              m_padrepos = pr;

              m_rect = pr;
              if (pwpadre->m_padre == -1)
              {
                  m_rect.top += (pr.bottom - pr.top) / 4;
                  pr.bottom  -= ((pr.bottom - pr.top) / 4)*3;          
                  pwpadre->MoveWindow(pr);
              }
          }
          else
          {              
              Pariente->GetClientRect(m_rect);
          }
		  //subela = TRUE;
      }
      else
          Pariente->GetClientRect(m_rect);
  }
  else
  {
	 m_rect = *rt;	 
  }

  if ( !(g_modo & 8) )
  {
      styleex |= (WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
      style |= (WS_OVERLAPPEDWINDOW | WS_CAPTION);
  }

  if ( !(g_modo & 16) )
  {
      style |=  WS_THICKFRAME;
  }
  else
  {
      style |=  WS_BORDER;
  }
  if ( (g_modo & 2048 /*0x800*/) )
  { 
	  Pariente->ClientToScreen(m_rect);
	  theApp.StdView(TRUE)->ScreenToClient(m_rect);
	  styleex |= WS_EX_TOPMOST;
	  r = CreateEx(styleex, AfxRegisterWndClass(NULL),Titulo,style,m_rect,theApp.StdView(TRUE),IDW_STDDLG+id);
	  SetOwner(Owner);
  }
  else
  {
	  r = CreateEx(styleex, AfxRegisterWndClass(NULL),Titulo,style,m_rect,Pariente,IDW_STDDLG+id);
  }
  if (Owner != Pariente)
  {
	  SetOwner(Owner);
  }
}
else
{
    /* si no hay pariente ... veremos */
  r = CreateEx(0, AfxRegisterWndClass(NULL),
	Titulo, visible | WS_POPUPWINDOW  | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN,
	 m_rect,NULL,NULL);
}

    if (!r) return r;

   ::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)::GetSysColorBrush(COLOR_3DFACE));      
    
    m_gmodo = g_modo;

    if (font)
    {
       m_font = font;
       SetFont(m_font);
    }	

    m_seleccion = (CDsGridButton **)malloc(sizeof(CDsGridButton *));
    m_seleccion[0] = NULL;   
    m_tabla.m_tw = this;
    m_botsel = -1;

	m_tabla.m_escampo = m_escampo;

    CRect rect;
    GetClientRect(&rect);
    rect.left += ::GetSystemMetrics(SM_CXVSCROLL);

    if (!m_tabla.PrepareControl(this, rect,Columnas,registros,m_font,localizador,inicial,g_modo,id,padre,principal,pextra)) 
        {DestroyWindow();return FALSE;}



    CRect rect2;
    CHeaderCtrl *hdr = m_tabla.GetHeaderCtrl();
    hdr->GetItemRect(0,rect2);
    rect2.left = 0;
    rect2.right = ::GetSystemMetrics(SM_CXVSCROLL);
    m_consulta = new CDsGridButton(&m_tabla);
    m_consulta->Create("", WS_CHILD  | BS_PUSHBUTTON | BS_BITMAP, rect2, this ,IDW_STDDLG+id);
    m_consulta->m_bmp.LoadBitmap( IDB_CONSULGRID );
    m_consulta->SetBitmap( m_consulta->m_bmp );
	if (!(g_modo & 0x2000000))
		m_consulta->ShowWindow(SW_SHOW);
    m_consulta->m_tipo = 1;

	/*
	if (subela)
		::PostMessage(m_tabla.m_hWnd,WM_COMMAND,ID_AVISO_POSTGRIDENSUREVISIBLE,(LPARAM)1);		
	*/
	//m_skinw = SkinWndScroll(&m_tabla);

return TRUE;
}

void CTablaW::OnSize(UINT nType, int cx, int cy) 
{
	CExtScrollWnd::OnSize(nType, cx, cy);

	if (m_tabla.m_hWnd)
	{
      CRect rect;
      GetClientRect(&rect);
      rect.left += ::GetSystemMetrics(SM_CXVSCROLL);      

	  if (m_skinw)
		  m_skinw->MoveWindow( rect);
	  else
		  m_tabla.MoveWindow( rect);

	  //m_tabla.AjustaMarco();
	}

    /*
    if (GetParent())
      ::SendMessage(GetParent()->m_hWnd,WM_COMMAND,ID_REAJUSTASCROLL,0);
    */
}

void CTablaW::OnMove(int x, int y) 
{
	CExtScrollWnd::OnMove(x, y);

    /*
    if (GetParent())
	  ::SendMessage(GetParent()->m_hWnd,WM_COMMAND,ID_REAJUSTASCROLL,0);	
    */
}


void CTablaW::OnSetFocus(CWnd* pOldWnd) 
{  
    
	CExtScrollWnd::OnSetFocus(pOldWnd);

    if (!IsWindowEnabled( ))
    {
        //pOldWnd->SetFocus();        
        return;
    }

    if (m_tabla.m_hWnd)
    {
        m_tabla.SetFocus();
    }	    
}

void CTablaW::OnClose() 
{
    if ( (m_gmodo & 0x10000) ) // modo 'hijo'
    {       
        
        if (m_tabla.m_principal)
        {
            m_tabla.m_principal->PostMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,(LPARAM)m_tabla.m_localizador);
        }                
    }
    else
    {        
        m_tabla.TeclaStd(806);	    
    }
    //CExtScrollWnd::OnClose();
}

CWnd* CTablaW::SetFocus( )
{	
    if (m_tabla.m_hWnd)
    {
        return m_tabla.SetFocus();
    }
	return CExtScrollWnd::SetFocus();	
}

/////////////////////////////////////////////////////////////////////////////
// CTablaEdit


CTablaEdit::CTablaEdit(int iItem, int iSubItem, CString sInitText)
:m_sInitText( sInitText )
{
m_iItem = iItem;
m_iSubItem = iSubItem;
m_bESC = FALSE;
m_parienta = NULL;
}


CTablaEdit::~CTablaEdit()
{
}


BEGIN_MESSAGE_MAP(CTablaEdit, CEdit)
	//{{AFX_MSG_MAP(CTablaEdit)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTablaEdit message handlers

int CTablaEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_parienta = ::GetParent(m_hWnd);
    SetFocus();
	CFont* font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText( m_sInitText );	
	CalulateSize();
	SetSel( 0, -1 );

	return 0;
}

void CTablaEdit::CalulateSize()
{
	// Get text extent
	CString str;    

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size;

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );
	
	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );
	
	if( !(GetStyle() & ES_MULTILINE ) )
		{
		size = dc.GetTextExtent( str );
		dc.SelectObject( pFontDC );
		size.cx += 5;                           // add some extra buffer
		}
	else
		{
		CRect thinrect( rect );  // To measure the skinniest text box
		CRect widerect( rect );  // To measure the wides text box
		widerect.right = parentrect.right;
		// Use the shortest of the two box sizes.
		int thinheight = dc.DrawText( str, &thinrect, DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS|DT_WORDBREAK );
		int wideheight = dc.DrawText( str, &widerect, DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS|DT_WORDBREAK );
		if(wideheight && thinheight >= wideheight )
			{
			size.cy = wideheight + 5;
			size.cx = widerect.right - widerect.left + 5;
			}
		else
			{
			size.cy = thinheight + 5;
			size.cx = thinrect.right - thinrect.left + 5;
			}
		}
	
	// Check whether control needs to be resized
	// and whether there is space to grow
	int changed = 0;
	if( size.cx > rect.Width() )
		{
		if( size.cx + rect.left < parentrect.right-2 )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right-2;
		changed = 1;
		}
	if( size.cy > rect.Height() )
		{
		if( size.cy + rect.top < parentrect.bottom-2 )
			rect.bottom = rect.top + size.cy;
		else
			{
			rect.bottom = parentrect.bottom-2;
			ShowScrollBar( SB_VERT );
			}
		changed = 1;
		}
	// If the size became larger rposition the window.
	if( changed )
		MoveWindow( &rect );
}

void CTablaEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
    {
		if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		if (::IsWindow(m_parienta))
		{
			::SetFocus(m_parienta);
		}		
		return;
    }	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CTablaEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);
	
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = m_parienta;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? 0 : str.GetLength();
	
	::SendMessage(m_parienta, WM_NOTIFY, ::GetDlgCtrlID(m_parienta), 
		(LPARAM)&dispinfo );
}

BOOL CTablaEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
        if (VK_RETURN == (int)pMsg->wParam)
        {
			if (::IsWindow(m_parienta))
			{
				::SetFocus(m_parienta);
			}		
			return TRUE;
		}
	}	
	return CEdit::PreTranslateMessage(pMsg);
}

void CTablaView::OnPaint() 
{
    //if (v_recursion)         
    //{
     //  CPaintDC dc(this);
    //}
    //else
		BASELISTCTRL::OnPaint();
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call BASELISTCTRL::OnPaint() for painting messages
}


int CTablaW::AseguraBoton(int itm)
{
    if (m_seleccion)
    {
        
        int tot = 0;
        CDsGridButton **sel = m_seleccion;
        while (*sel)
        {
            tot++;
            sel++;
        }
        if (itm < 0)
        {
			int i;
            itm = (-itm)-1;
            for (i = itm;i < tot;i++)
            {
                if (i == m_botsel)
                    m_botsel = -1;
                if (i == m_topsel)
                    m_topsel = -1;
                m_seleccion[i]->DestroyWindow();
                delete m_seleccion[i];
            }
            if (itm < tot)
            {
                m_seleccion[itm] = NULL;
            }
        }
        else
        {
            int tope = itm+1;

            if (tot < tope)
            {
                CRect rect2;
                CHeaderCtrl *hdr = m_tabla.GetHeaderCtrl();
                hdr->GetItemRect(0,rect2);
                POINT point;
                point.x = 0;
                point.y = rect2.bottom+1;
                m_tabla.GetItemRect(0,rect2,LVIR_BOUNDS);
                int ss = rect2.Size().cy;
    
                rect2.left = point.x;
                rect2.right = ::GetSystemMetrics(SM_CXVSCROLL);
                rect2.top  = point.y;
                rect2.bottom = point.y + ss;

                rect2.top    += (ss*tot);
                rect2.bottom += (ss*tot);

                m_seleccion = (CDsGridButton **)realloc((void *)m_seleccion,sizeof(CDsGridButton *)*(tope+1));
                for (;tot < tope;tot++)
                {
                    m_seleccion[tot] = new CDsGridButton(&m_tabla);
                    m_seleccion[tot]->Create("", WS_CHILD  | BS_PUSHBUTTON | BS_BITMAP, rect2, this ,IDW_STDDLG+m_tabla.m_id);
                    m_seleccion[tot]->m_tipo = 0;
                    m_seleccion[tot]->ShowWindow(SW_SHOW);
                    m_seleccion[tot]->m_nume = tot;
                    rect2.top    += ss;
                    rect2.bottom += ss;
                }
                m_seleccion[tot] = NULL;
            }
        }
    }

    return itm;
}

int CTablaW::BotonPintado(int itm,int modo)
{
    int tot = 0;
    CDsGridButton **sel = m_seleccion;
    if (!sel) return -1;


    while (*sel)
    {
        tot++;
        sel++;
    }

    if (modo)
    {
        if (itm < 0)
        {
            if (m_topsel >= 0 && m_topsel < tot)
            {
                m_seleccion[m_topsel]->SetBitmap( NULL );
                m_seleccion[m_topsel]->m_bmp.DeleteObject();
            }
            m_topsel = -1;
            itm = -1;
            return 0;
        }
        if (m_botsel == itm || modo == 2)
        {
            if (m_botsel >= 0 && m_botsel < tot)
            {
                m_seleccion[m_botsel]->SetBitmap( NULL );
                m_seleccion[m_botsel]->m_bmp.DeleteObject();
            }
        }
        
        
        if (itm >= tot)
            return 0;
        

        if (m_seleccion[itm]->m_bmp.m_hObject)
        {
            m_seleccion[itm]->SetBitmap( NULL );
            m_seleccion[itm]->m_bmp.DeleteObject();        
        }
        if (m_topsel != itm)
        {
            if (m_topsel >= 0 && m_topsel < tot)
            {
                m_seleccion[m_topsel]->SetBitmap( NULL );
                m_seleccion[m_topsel]->m_bmp.DeleteObject();
            }
			else
			{
				//ASSERT(FALSE);
			}
        }
        m_topsel = itm;
        if (modo == 2)
           m_seleccion[m_topsel]->m_bmp.LoadBitmap( IDB_BOTGNUESEL );
        else
           m_seleccion[m_topsel]->m_bmp.LoadBitmap( IDB_BOTGRIDNUE );
        m_seleccion[m_topsel]->SetBitmap( m_seleccion[m_topsel]->m_bmp );
        if (modo == 2)
            m_botsel = m_topsel;
    }
    else
    {
        if (itm < 0)
        {
            if (m_botsel >= 0 && m_botsel < tot)
            {
                m_seleccion[m_botsel]->SetBitmap( NULL );
                m_seleccion[m_botsel]->m_bmp.DeleteObject();
            }
            m_botsel = -1;
            itm = -1;
            return 0;
        }

        if (m_botsel != itm || m_botsel == m_topsel)
        {
            if (m_botsel >= 0 && m_botsel < tot && m_botsel != m_topsel)
            {
                m_seleccion[m_botsel]->SetBitmap( NULL );
                m_seleccion[m_botsel]->m_bmp.DeleteObject();
            }			
            if (m_botsel == m_topsel)
            {
                m_topsel = -1;
            }
            m_botsel = itm;
			if (m_botsel >= 0 && m_botsel < tot)
			{				
				if (m_seleccion[m_botsel]->m_bmp.m_hObject)
				{
					m_seleccion[m_botsel]->SetBitmap( NULL );
					m_seleccion[m_botsel]->m_bmp.DeleteObject();
				}
				m_seleccion[m_botsel]->m_bmp.LoadBitmap( IDB_BOTGRIDSEL );
				m_seleccion[m_botsel]->SetBitmap( m_seleccion[m_botsel]->m_bmp );
			}
			/*
			else
				ASSERT(FALSE);
			*/
        }
    }
    return itm;
}

int CTablaW::TotalBoton()
{
    CDsGridButton **sel = m_seleccion;
    if (!sel) return 0;

    int tot = 0;
    while (*sel)
    {
        tot++;
        sel++;
    }
    return tot;
}

BOOL CTablaW::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class    
	return CExtScrollWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CTablaView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class    
	return BASELISTCTRL::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CTablaW::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{		
	CExtScrollWnd::OnNcLButtonDown(nHitTest, point);
    SetFocus();
}

void CTablaW::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CExtScrollWnd::OnLButtonDown(nFlags, point);
    SetFocus();
}

void modocolumna(long fila,long col,char *res)
{
   int i =0,j;
   char ss;

   
   do
   {
	   col--;
       res[i] = (char )(col%26) + 'A';	   
       col /= 26;
       i++;
   } while(col);
   res[i] = 0;

   for (j = 0;j < i/2;j++)
   {
       ss = res[j];
       res[j] = res[i-j-1];
       res[i-j-1] = ss;
   }
   sprintf(res+i,"%ld",fila);
}
void CTablaView::OnExportaExcel() 
{
	OnExportaExcel(NULL);
}
void CTablaView::OnExportaExcel(LPCSTR fichero) 
{
    CDsExcel excel;
    long rr,i,ij;
    int ii;
    char *px;    
    CString str;
    char tmp[256];
    char tmp2[256];
    LVCOLUMN ccc;
	CDlgProgreso dlgp;
	long progreso = 0;
        
    /*
    COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
               vtTrue((short)TRUE),
               vtFalse((short)FALSE);
    */
	if (!fichero)
	{

		if (m_registros > 100 && !theApp.alt_excel )
		{
			strcpy(tmp,"¿Exportar a Excel? (el proceso puede tardar considerablemente, para volumen de datos es recomendable usar la exportación dbf-Excel)");
		}
		else
		{
			strcpy(tmp,"¿Exportar a Excel?");
		}
		if (AfxMessageBox(tmp,MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}


	HCURSOR  ocur = ::SetCursor(::LoadCursor(AfxGetResourceHandle( ),MAKEINTRESOURCE(IDC_WAIT)));
    ::ShowCursor(TRUE);

	dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
	dlgp.SetWindowText("Exportando a Excel");
	dlgp.m_progreso.SetRange32( 0,m_registros/40+1);
	dlgp.ShowWindow(SW_SHOW);
	dlgp.BringWindowToTop();
	
	try
    {
		if (theApp.alt_excel) {
			char tmpx[2048];
			memset(tmpx,0,sizeof(tmp));
			strcpy(tmpx,"inicializacion");
			if (fichero && *fichero) {
               strcpy(tmpx+31,fichero);
			   strcat(tmpx+31,",A1,,");
			}
			else
               strcpy(tmpx+31,"nuevo,A1,,");
			FuncionesImpresion(998,(void *)tmpx);
			memset(tmpx,0,sizeof(tmp));
			strcpy(tmpx,"imprime");
			for (ij = COLUMNA_INICIAL;ij < m_columnas;ij++)
			{
				i = m_orderarray[ij];
				//i = ij;
				try
				{	 
					memset((void *)&ccc,0,sizeof(ccc));
					ccc.mask = LVCF_TEXT;
					ccc.pszText = tmp2;
					ccc.cchTextMax = sizeof(tmp2)-1;
					GetColumn( i, &ccc );
					//modocolumna(1,ij-COLUMNA_INICIAL+1,tmp);
					//excel.m_pSheet->Range[tmp]->Value = _bstr_t(tmp2);
					ii = strlen(tmp2);
					tmp2[ii] = 9;
					tmp2[ii+1] = 0;
					strcat(tmpx+31,tmp2);
				}
				catch (...)
				{
					//ASSERT(FALSE);
				}
			}
			FuncionesImpresion(998,(void *)tmpx);

			for (rr = 1;rr <= m_registros;rr++)
			{
				if (!((rr-1)%40))
				{
					progreso++;
					dlgp.m_progreso.SetPos(progreso);
				}

				memset(tmpx,0,sizeof(tmp));
				strcpy(tmpx,"imprime");
				try
				{	 

					ii = (int)rr-1;
					if (!m_cache.Lookup( ii , px )) 
					{
						Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
					}                
	                
					for (ij = COLUMNA_INICIAL;ij < m_columnas;ij++)
					{
						i = m_orderarray[ij];
						// i = ij;
						str = GetItemText( rr-1, i );
						if (str[0] == ' ')
						{
							str = str.Mid(1);
						}
						/*
						for (px = (char *)(LPCSTR)str;*px;px++)
						{
						oem_a_ansi((unsigned char *)px);
						}
						*/
						//modocolumna(rr+1,ij-COLUMNA_INICIAL+1,tmp);

						/*
						if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 0)
						{
							excel.m_pSheet->Range[tmp]->NumberFormat = _bstr_t((LPCSTR)"@");
						}
						else if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 5)
						{
							CString stmp;						
							stmp = str.Mid(3,2);
							stmp += '/';
							stmp += str.Mid(0,2);
							stmp += '/';
							stmp += str.Mid(6,4);
							str = stmp;
						}
						else if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))
						{
							str.Remove('.');
							str.Replace(',','.');
						}
						*/
						//excel.m_pSheet->Range[tmp]->Value = _bstr_t((LPCSTR)str);
						strcpy(tmp2,(LPCSTR)str);
						ii = strlen(tmp2);
						tmp2[ii] = 9;
						tmp2[ii+1] = 0;
						strcat(tmpx+31,tmp2);

					}
				}
				catch (...)
				{
					//ASSERT(FALSE);
				}
				FuncionesImpresion(998,(void *)tmpx);
			}        
			memset(tmpx,0,sizeof(tmp));
			strcpy(tmpx,"finalizacion");
			FuncionesImpresion(998,(void *)tmpx);
		}
		else {
			if (!excel.AbreExcel("")) 
			{
				::SetCursor(ocur);			
				return;
			}


			//enum Excel::XlCalculation oldcal;

			//oldcal = excel.m_pXL->Calculation;

			excel.m_pXL->Calculation = Excel::xlCalculationManual;

			excel.m_pXL->Visible = VARIANT_FALSE;

			for (ij = COLUMNA_INICIAL;ij < m_columnas;ij++)
			{
				i = m_orderarray[ij];
				//i = ij;
				try
				{	 
					memset((void *)&ccc,0,sizeof(ccc));
					ccc.mask = LVCF_TEXT;
					ccc.pszText = tmp2;
					ccc.cchTextMax = sizeof(tmp2)-1;
					GetColumn( i, &ccc );
					modocolumna(1,ij-COLUMNA_INICIAL+1,tmp);
					excel.m_pSheet->Range[tmp]->Value = _bstr_t(tmp2);
				}
				catch (...)
				{
					//ASSERT(FALSE);
				}
			}

			for (rr = 1;rr <= m_registros;rr++)
			{
				if (!((rr-1)%40))
				{
					progreso++;
					dlgp.m_progreso.SetPos(progreso);
				}

				try
				{	 

					ii = (int)rr-1;
					if (!m_cache.Lookup( ii , px )) 
					{
						Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
					}                
	                
					for (ij = COLUMNA_INICIAL;ij < m_columnas;ij++)
					{
						i = m_orderarray[ij];
						// i = ij;
						str = GetItemText( rr-1, i );
						if (str[0] == ' ')
						{
							str = str.Mid(1);
						}
						/*
						for (px = (char *)(LPCSTR)str;*px;px++)
						{
						oem_a_ansi((unsigned char *)px);
						}
						*/
						modocolumna(rr+1,ij-COLUMNA_INICIAL+1,tmp);

						if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 0)
						{
							excel.m_pSheet->Range[tmp]->NumberFormat = _bstr_t((LPCSTR)"@");
						}
						else if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 5)
						{
							CString stmp;						
							stmp = str.Mid(3,2);
							stmp += '/';
							stmp += str.Mid(0,2);
							stmp += '/';
							stmp += str.Mid(6,4);
							str = stmp;
						}
						else if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))
						{
							str.Remove('.');
							str.Replace(',','.');
						}
						excel.m_pSheet->Range[tmp]->Value = _bstr_t((LPCSTR)str);
					}
				}
				catch (...)
				{
					//ASSERT(FALSE);
				}
			}        
			excel.m_pXL->Calculation = Excel::xlCalculationAutomatic;

			excel.m_pXL->Visible = VARIANT_TRUE;
		}
	}
	catch( ... )
	{

	}

	dlgp.DestroyWindow();
    
    ::SetCursor(ocur);
}

void CTablaView::OnExportaWord() 
{
	CDsWord word;
    int ii,tam,tam1;
    long rr,i,ij;
    long cols;
    CString str,txt;
    char *px;
    LVCOLUMN ccc;
    char tmp2[256];
	CDlgProgreso dlgp;
	long progreso = 0;

	{
		if (m_registros > 100)
		{
			strcpy(tmp2,"¿Exportar a Word? (el proceso puede tardar considerablemente)");
		}
		else
		{
			strcpy(tmp2,"¿Exportar a Word?");
		}
		if (AfxMessageBox(tmp2,MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}


	
	PonWait();

	dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
	dlgp.SetWindowText("Exportando a Word");
	dlgp.m_progreso.SetRange32( 0,m_registros/40+1);
	dlgp.ShowWindow(SW_SHOW);
	dlgp.BringWindowToTop();

	try
    {    
        if (!word.AbreWord("")) 
		{
			QuitaWait();
			return;
		}
		

        word.m_pWord->Visible = VARIANT_FALSE;

        word.m_pWord->Options->CheckSpellingAsYouType = VARIANT_FALSE;
        
        word.m_pWord->Options->CheckGrammarAsYouType = VARIANT_FALSE;

        word.m_pRange = word.m_pDoc->Range(&_variant_t((long)0),&_variant_t((long)0));
        cols = m_columnas;
        if (cols > 10) cols = 5;       

        txt = "";        

        try
        {	 
            for (ij = COLUMNA_INICIAL;ij < cols;ij++)
            {
                /*i = m_orderarray[ij];*/
				i = ij;
                memset((void *)&ccc,0,sizeof(ccc));
                ccc.mask = LVCF_TEXT;
                ccc.pszText = tmp2;
                ccc.cchTextMax = sizeof(tmp2)-1;
                GetColumn( i, &ccc );
                /*
                if (i > COLUMNA_INICIAL)
                {
                    txt += "\t";
                }
                */

                tam = m_def->Campo(i-COLUMNA_INICIAL)->c_maxqc;
                tam1 = strlen(tmp2);
                txt += (LPCSTR)tmp2;
                for (;tam1 < tam;tam1++)
                {
                    txt += " ";
                }
                txt += " ";
            }
            txt += "\r\n";
            word.m_pRange->InsertAfter(_bstr_t((LPCSTR)txt));
            txt = "\r\n";
            word.m_pRange->InsertAfter(_bstr_t((LPCSTR)txt));
        }
	    catch (...)
	    {
            //ASSERT(FALSE);
	    }


        for (rr = 0;rr < m_registros;rr++)
        {
				if (!((rr-1)%40))
				{
					progreso++;
					dlgp.m_progreso.SetPos(progreso);
				}

            try
            {	 

                ii = (int)rr;
                if (!m_cache.Lookup( ii , px )) 
                {
                    Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
                }

                txt = "";
                
                for (ij = COLUMNA_INICIAL;ij < cols;ij++)
                {
                    i = m_orderarray[ij];
	                str = GetItemText( rr, i );

                    //pTabla->Cell(rr,i)->Range->Text = _bstr_t((LPCSTR)str);
                    /*
                    if (i > COLUMNA_INICIAL)
                    {
                        txt += "\t";
                    }
                    */
                    str = str.Mid(1);
                    txt += str;
                    tam = m_def->Campo(i-COLUMNA_INICIAL)->c_maxqc;
                    tam1 = str.GetLength();                    
                    for (;tam1 < tam;tam1++)
                    {
                        txt += " ";
                    }
                    txt += " ";
                }
                
				/*
                for (px = (char *)(LPCSTR)txt;*px;px++)
                {
                   oem_a_ansi((unsigned char *)px);
                }
				*/
                txt += "\r\n";

                word.m_pRange->InsertAfter( _bstr_t((LPCSTR)txt) );
            }
	        catch (...)
	        {
                ASSERT(FALSE);
	        }
        }

        word.m_pRange->Font->Name = "Courier New";
        word.m_pRange->Font->Size = 8;

        word.m_pWord->Visible = VARIANT_TRUE;
    }
	catch (...)
	{
		
	}
	dlgp.DestroyWindow();
	QuitaWait();
    word.m_WordEventSink.m_pDsWord = NULL;
}

void CTablaW::OnExportaWord() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnExportaWord();
    }
}

void CTablaW::OnExportaExcel() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnExportaExcel();
    }
}

BOOL CTablaView::EjecutaFiltro(CDlgGridFiltro *dlg)
{
    char *p;
    long tam;
    long respuesta;
    long modo = 1;
    int operacion;
    int r = -1;
    char *tmp;
    int memoria;
	int nfiltros = 0;
	struct grid_filtro *pfiltros = NULL;
	int flag0;
	int ncols = 0;
	int *cols = NULL;
    
	if (dlg->EsSimple())
	{
		int i;
		operacion = 7;
		
		pfiltros = (struct grid_filtro *)malloc(sizeof(struct grid_filtro)*m_def->m_info->n_campos);		
		ncols = 0;
		cols = (int *)malloc(m_def->m_info->n_campos*sizeof(int));

		for (int xi = 0;xi < dlg->m_nitems;xi++)
		{
			/* m_checked */
			i = dlg->m_idxs[xi];
			if (dlg->m_items[i]->m_salida)
			{
				cols[ncols] = i;
				ncols++;
			}
			if (dlg->m_items[i]->TieneFiltro() || dlg->m_items[i]->m_Orden)
			{
				pfiltros[nfiltros].tipo  = (dlg->m_items[i]->m_TipoValor & 0xf);
				flag0 = 0;
				if (dlg->m_items[i]->m_Orden)
				{
					flag0 |= 1;
					if (dlg->m_items[i]->m_OrdenInverso)
					{
						flag0 |= 2;
					}
					if (dlg->m_items[i]->m_Agrupado)
					{
						flag0 |= 4;
					}
				}
				if (dlg->m_items[i]->m_CondAgrupada)
					flag0 |= 8;
				if (dlg->m_items[i]->m_Preguntar)
					flag0 |= 0x10;
				if (dlg->m_items[i]->m_opera < 0)
					dlg->m_items[i]->m_opera = 0;				
				flag0 |= ((dlg->m_items[i]->m_opera&0xf) << 16);

				pfiltros[nfiltros].tipo |= (flag0 << 4);

				pfiltros[nfiltros].campo = dlg->m_items[i]->m_campo;//m_def->Campo(i,TRUE)->Id;
				pfiltros[nfiltros].mayus = (int)dlg->m_items[i]->m_Mayus;
				pfiltros[nfiltros].inver = (int)dlg->m_items[i]->m_Inver;
				strncpy(pfiltros[nfiltros].valor,dlg->m_items[i]->m_Valor,298);
				strncpy(pfiltros[nfiltros].limi,dlg->m_items[i]->m_LimiteI,298);
				strncpy(pfiltros[nfiltros].lims,dlg->m_items[i]->m_LimiteS,298);                    
				nfiltros++;
			}
		}

		memoria = (ncols*sizeof(int)) + (nfiltros*sizeof(struct grid_filtro));

		tam = (sizeof(long) * 4) + (sizeof(int)*6) + 100;

		tmp = (char *)malloc(tam+memoria+1);
		p = tmp;

		memcpy((void *)p,(void *)&m_localizador,sizeof(int));
		p += sizeof(int);
    
		respuesta = tam;
		tam += memoria;

		memcpy((void *)p,(void *)&tam,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&respuesta,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&modo,sizeof(long));
		p += sizeof(long);
		memcpy((void *)p,(void *)&operacion,sizeof(int));
		p += sizeof(int);
    
		memcpy((void *)p,(void *)&r,sizeof(int));
		p += sizeof(int);

		strcpy(p,dlg->m_nombre);            
		p += (strlen(p)+1);

		memcpy((void *)p,(void *)&ncols,sizeof(int));
		p += sizeof(int);
		memcpy((void *)p,(void *)cols,ncols*sizeof(int));
		p += (ncols*sizeof(int));
		memcpy((void *)p,(void *)&nfiltros,sizeof(int));
		p += sizeof(int);
		memcpy((void *)p,(void *)pfiltros,nfiltros*sizeof(struct grid_filtro));

		free((void *)cols);
		free((void *)pfiltros);

		if (ncols)
		{
			DespachadorInterno(13,(void *)tmp);
		}
		free((void *)tmp);
	}
	else
	{
		operacion = 8;

		if (dlg->m_nitems > 0)
		{
			int i,nfiltros,j,k;
			int nfiles;
			char files[100][128];
			int *idxfiles;			
			CFiltroDef *defs[100],*def;
			int pariente[100];			
			
			idxfiles = new int[dlg->m_nitems];
			pfiltros = (struct grid_filtro *)malloc(sizeof(struct grid_filtro)*dlg->m_nitems);
			memset(files,0,sizeof(files));			
			nfiles = 1;
			strcpy(files[0],dlg->m_principal->m_nomdef);
			defs[0] = dlg->m_principal;
			pariente[0] = 0;
			for (nfiltros = 0;nfiltros < dlg->m_nitems;nfiltros++)
			{
				for (i = 0;i < nfiles;i++)
				{
					if (!strcmp(defs[i]->m_nomdef,dlg->m_items[nfiltros]->m_padre->m_nomdef))
					{						
						break;
					}
				}
				if (i >= nfiles)
				{
					defs[nfiles] = dlg->m_items[nfiltros]->m_padre;					
					strcpy(files[nfiles],dlg->m_items[nfiltros]->m_padre->m_nomdef);
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
			for (int xi = 0;xi < nfiltros;xi++)
			{
				i = dlg->m_idxs[xi];
				for (j = 0;j < nfiles;j++)
				{
					if (!strcmp(defs[j]->m_nomdef,dlg->m_items[i]->m_padre->m_nomdef))
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
				pfiltros[i].tipo  = (dlg->m_items[i]->m_TipoValor & 0xf);
				flag0 = 0;
				if (dlg->m_items[i]->m_Orden)
				{
					flag0 |= 1;
					if (dlg->m_items[i]->m_OrdenInverso)
					{
						flag0 |= 2;
					}
					if (dlg->m_items[i]->m_Agrupado)
					{
						flag0 |= 4;
					}
				}
				if (dlg->m_items[i]->m_CondAgrupada)
					flag0 |= 8;
				if (dlg->m_items[i]->m_Preguntar)
					flag0 |= 0x10;
				if (dlg->m_items[i]->m_opera < 0)
					dlg->m_items[i]->m_opera = 0;				
				flag0 |= ((dlg->m_items[i]->m_opera&0xf) << 12);
				pfiltros[i].tipo |= (flag0 << 4);				

				pfiltros[i].campo = dlg->m_items[i]->m_campo;
				pfiltros[i].mayus = (int)dlg->m_items[i]->m_Mayus;
				pfiltros[i].inver = (int)dlg->m_items[i]->m_Inver;
				strncpy(pfiltros[i].valor,dlg->m_items[i]->m_Valor,298);
				strncpy(pfiltros[i].limi,dlg->m_items[i]->m_LimiteI,298);
				strncpy(pfiltros[i].lims,dlg->m_items[i]->m_LimiteS,298);
			}

			memoria = nfiltros*(sizeof(struct grid_filtro)+sizeof(int)*3)  + nfiles*(128+sizeof(int));

			tam = (sizeof(long) * 4) + (sizeof(int)*6) + 100;

			for (xi = 0;xi  < nfiltros;xi++)
			{
				i = dlg->m_idxs[xi];

				if (!dlg->m_items[i]->TieneFiltro() && !dlg->m_items[i]->m_Orden)
				{
					memoria -= sizeof(struct grid_filtro);
				}
			}

			tmp = (char *)malloc(tam+memoria+1);
			p = tmp;

			memcpy((void *)p,(void *)&m_localizador,sizeof(int));
			p += sizeof(int);
    
			respuesta = tam;
			tam += memoria;

			memcpy((void *)p,(void *)&tam,sizeof(long));
			p += sizeof(long);	
			memcpy((void *)p,(void *)&respuesta,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&modo,sizeof(long));
			p += sizeof(long);
			memcpy((void *)p,(void *)&operacion,sizeof(int));
			p += sizeof(int);
    
			memcpy((void *)p,(void *)&r,sizeof(int));
			p += sizeof(int);

			strcpy(p,dlg->m_nombre);            
			p += (strlen(p)+1);

			memcpy((void *)p,(void *)&nfiles,sizeof(int));
			p += sizeof(int);
			for (i = 0;i  < nfiles;i++)
			{
				memcpy(p,files[i],128);
				p += 128;
				if (i)
				{
					flag0 = 1; /* incluimos info relacion */				
				}
				else
				{
					flag0 = 0; 
				}
				memcpy((void *)p,(void *)&flag0,sizeof(int));
				p += sizeof(int);
				if ((flag0 & 1))
				{
					memcpy((void *)p,(void *)&(pariente[i]),sizeof(int));
					p += sizeof(int);
					memcpy((void *)p,(void *)&(defs[i]->m_camposrel),sizeof(int));
					p += sizeof(int);
					for (j = 0;j < defs[i]->m_camposrel;j++)
					{						
						memcpy((void *)p,(void *)&(defs[i]->m_campospadre_o[j]),sizeof(int));
						p += sizeof(int);						
						memcpy((void *)p,(void *)&(defs[i]->m_camposhijo_o[j]),sizeof(int));
						p += sizeof(int);
					}					
				}
			}
			memcpy((void *)p,(void *)&nfiltros,sizeof(int));
			p += sizeof(int);

			for (xi = 0;xi  < nfiltros;xi++)
			{
				i = dlg->m_idxs[xi];
				flag0 = 0;
				if (dlg->m_items[i]->m_salida)
				{
					flag0 = 1;
				}
				if (!dlg->m_items[i]->TieneFiltro() && !dlg->m_items[i]->m_Orden)
				{					
					flag0 |= 2;				
				}				
				else if (!dlg->m_items[i]->TieneFiltro())
				{
					flag0 |= 4;
				}
				
				memcpy((void *)p,(void *)&flag0,sizeof(int));
				p += sizeof(int);
				memcpy((void *)p,(void *)(idxfiles+i),sizeof(int));
				p += sizeof(int);				
				memcpy((void *)p,(void *)&(dlg->m_items[i]->m_campo),sizeof(int));
				p += sizeof(int);
				if (!(flag0 & 2))
				{
					memcpy((void *)p,(void *)&(pfiltros[i]),sizeof(struct grid_filtro));
					p += sizeof(struct grid_filtro);
				}
			}
			
			delete idxfiles;			
			free((void *)pfiltros);

			DespachadorInterno(13,(void *)tmp);
			
			free((void *)tmp);
		}
	}

	return TRUE;
}

BOOL CTablaView::Filtros(BOOL Predeterminado,BOOL conmenu)
{
	if ((m_gmodo & 0x4000000))
	{
		return FALSE;
	}
    CDlgGridFiltro dlg(this);


	if (conmenu)
	{
        CMenu filtros;   
		filtros.CreatePopupMenu();

		Predeterminado = FALSE;
		filtros.InsertMenu(0,MF_STRING | MF_BYPOSITION,ID_CONSULTA_FILTRO,"Mantenimiento");		
		dlg.RellenaMenu(&filtros,1,100000);		

		POINT pp;
		GetCursorPos(&pp);
		int r = (int)filtros.TrackPopupMenu(TPM_LEFTALIGN| TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y,this,NULL);
		if (r != ID_CONSULTA_FILTRO)
		{
			if (r >= 100000 && r < 199999)
			{
				if (r >= 101000)		
					dlg.m_registrolocal = FALSE;		
				else
					dlg.m_registrolocal = TRUE;
				filtros.GetMenuString( r,dlg.m_nombre,MF_BYCOMMAND);
				if (!dlg.LeeDelRegistro())
				{
					AfxMessageBox("Filtro erroneo, acceda a la opción de Mantenimiento");
					return FALSE;
				}
				Predeterminado = TRUE;
			}
			else
				return FALSE;
		}
	}
	else
	{
		if ((m_gmodo & 0x2000000))
		{
			dlg.m_filtrousuario = TRUE;
			dlg.DoModal();
			return TRUE;
		}

		if (Predeterminado)
		{
			if (!dlg.CargaPredeterminado())
				return FALSE;
			dlg.RellenaListado(TRUE);
		}
	}

    if (Predeterminado || dlg.DoModal() == IDOK)
    {
		return EjecutaFiltro(&dlg);
    }
	return FALSE;
}

BOOL CTablaView::CreaColumnas()
{

   BorraCache();

   int nColumnCount = GetHeaderCtrl()->GetItemCount();


   if (m_buffer)
   {
        delete [] m_buffer;
        m_buffer = NULL;
   }

   int i;
   for (i=nColumnCount-1;i >= 0 ;i--)
   {
       DeleteColumn(i);
   }

    if (m_apintar)
      delete[] m_apintar;
    if (m_pintarects)
        delete [] m_pintarects;

    if (m_tamcampos)
        delete [] m_tamcampos;
    if (m_poscampos)
        delete [] m_poscampos;
    
    if (m_orderarray)
        delete [] m_orderarray;
    if (m_iniciales)
        delete [] m_iniciales;

    if (m_flagcampos)
        delete [] m_flagcampos;


    CDC* hDc = GetDC();
    TEXTMETRIC tm;
    int tamax;

    if (hDc->GetTextMetrics(&tm))
    {       
       tamax = tm.tmAveCharWidth;  //tmMaxCharWidth; 
    }
    else
    {
       tamax = hDc->GetTextExtent( "M", 1 ).cx;
    }

    ReleaseDC(hDc);


	LV_COLUMN		lvcolumn;     

    for(m_columnas = 0;m_columnas < COLUMNA_INICIAL;m_columnas++)
    {
	    lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_IMAGE | LVCF_ORDER | LVCF_WIDTH ;
	    lvcolumn.fmt = LVCFMT_COL_HAS_IMAGES;
	    lvcolumn.iSubItem = m_columnas;
        lvcolumn.iOrder = m_columnas;	    
        lvcolumn.iImage =  0;
        lvcolumn.cx = 0;//20;
	    InsertColumn(m_columnas, &lvcolumn);	
    }
    
    int extrai = 0;
    int comp;

	m_curcol = COLUMNA_INICIAL;
    
	BOOL sinclaves = FALSE;

	if ((m_gmodo & 256) || ((m_gmodo & 1) && (!(m_gmodo & 0x80000) || ((m_gmodo & 0x80000) && m_clavesvalidas[0] < 2))))
	{
		sinclaves = TRUE;
	}

	CString stama = m_defaultama;

    /*LVCFMT_LEFT | LVCFMT_COL_HAS_IMAGES;*/
    char tmpxx[512],*px;
	int col;
    for (col = 0;col < m_def->m_n_campos_col;col++)
    {
	   lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;        
	   lvcolumn.fmt = 0;
       strcpy(tmpxx,m_def->c_nombre(col));
       for (px = tmpxx;*px;px++)
       {
          oem_a_ansi((unsigned char *)px);
       }
	   lvcolumn.pszText =tmpxx;           
	   lvcolumn.iSubItem = m_columnas;
       lvcolumn.iOrder = m_columnas;


	   if (!sinclaves && !(m_gmodo & 1) && !(m_gmodo & 256))
	   {	   
		   extrai = 20;
	   }
	   else
	   {
		   extrai = 0;
	   }

       lvcolumn.iImage = -1;

	   if (!sinclaves)
	   {
		   int otra;	   
		   if (m_def->EsInicialClave(col,FALSE,m_def->m_info->clave_actual) > -1)
		   {
			   comp = m_def->EsDeLaClave(col,m_def->m_info->clave_actual);

			   ASSERT(comp > -1);

			   m_curcol = COLUMNA_INICIAL + col;
			   if (m_def->k_orden(m_def->m_info->clave_actual,comp) < 0)
				  lvcolumn.iImage =  4;
			   else			   
				  lvcolumn.iImage =  3;           
			   extrai = 20;
		   }
		   else
		   {
			   if ((otra = m_def->EsInicialClave(col)) > -1)
			   {
				   /*
				   comp = m_def->EsDeLaClave(col,otra);

				   ASSERT(comp > -1);

				   if (m_def->k_orden(otra,comp) < 0)
					  lvcolumn.iImage =  4;
				   else
					  lvcolumn.iImage =  3;
				   */
				   lvcolumn.iImage =  2;
				   extrai = 20;
			   }
		   }

		   if (extrai)
		   {
			   lvcolumn.mask |= LVCF_IMAGE;
			   lvcolumn.fmt  |= LVCFMT_BITMAP_ON_RIGHT;
		   }	   
	   }

	   if (m_guarda_posicion)
	   {
			CString cc;

			m_tw->GetWindowText(cc);

			cc += " ";
			cc += m_def->m_RegPath;		
	        
			CString dd;			

			dd.Format("%d",m_curcol);
			dd = theApp.GetProfileString("GridDefaultCol",cc,dd);
			m_curcol = atoi(dd);
	   }

	   
	   if (stama.GetLength() > 3)
	   {
		   lvcolumn.cx = atoi(stama.Mid(0,4));
		   stama = stama.Mid(4);
	   }
	   else
	   {
		   lvcolumn.cx = m_def->Campo(col)->c_qc * tamax;
		   if (lvcolumn.cx < (GetStringWidth( lvcolumn.pszText )+tamax*2+extrai))
		   {
			   lvcolumn.cx = GetStringWidth( lvcolumn.pszText )+tamax*2+extrai;
		   }       
	   }
     
	   InsertColumn(m_columnas, &lvcolumn);	
	   m_columnas++;	   
	}		
    
    m_tamcampos  = new int[ m_columnas + 1 + COLUMNA_FINAL];
    m_poscampos  = new int[ m_columnas + 1+ COLUMNA_FINAL];
    m_flagcampos = new int[ m_columnas + 1+ COLUMNA_FINAL];

    m_orderarray = new int[ m_columnas + 1+ COLUMNA_FINAL];

    m_iniciales = new int[ m_columnas + 1+ COLUMNA_FINAL];    
    m_iniciales[0] = -1;

    /*
    #define CHARTIPO    0
    #define CHARTAMA  1
    #define INTTIPO     1
    #define INTTAMA   2
    #define LONGTIPO    2
    #define LONGTAMA  4
    #define DOUBLETIPO  3
    #define DOUBLETAMA (sizeof(double))
    #define FLOATTIPO   4
    #define FLOATTAMA (sizeof(float))
    #define FECHATIPO   5
    #define FECHATAMA 8
    */

    i = 0;
    int offset = 0;
    for (col = 0;col < m_def->m_n_campos_col;col++)
    {
        m_poscampos[i] = offset;
        m_tamcampos[i] = m_def->Campo(col)->c_maxqc;
        m_flagcampos[i] = m_def->Campo(col)->c_tipo;
        if (m_flagcampos[i] != 0 && m_flagcampos[i] != 5)
        {
            m_flagcampos[i] |= 16;
        }
        offset += m_tamcampos[i];        
        i++;
    }
    m_lenreg = offset;     	
	MAX_CACHE = MAX_MEMORIA / m_lenreg;

	MAXCOMPLETO = 50000 / (m_lenreg+sizeof(long));

	MAX_ORDENACION_LOCAL = MAXCOMPLETO*10;
	if (MAX_ORDENACION_LOCAL > MAX_CACHE)
		MAX_ORDENACION_LOCAL = MAX_CACHE;

	int nmbuffer = m_lenreg + 6 + 10 + (sizeof(long) * 15) + (sizeof(int)*3) + ((m_lenreg+6+sizeof(long)*2)*(MAXCOMPLETO+1));
    m_buffer = new char[nmbuffer];

       
    m_apintar = new int[ m_columnas + 1+ COLUMNA_FINAL];
    m_apintar[0] = -1;

    m_pintarects = new RECT[ m_columnas + 1+ COLUMNA_FINAL];		
    
    for (i = 0;i < COLUMNA_FINAL;i++)
    {
	   lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	   lvcolumn.fmt = LVCFMT_LEFT;
	   lvcolumn.pszText = " ";
	   lvcolumn.iSubItem = m_columnas;
       lvcolumn.iOrder = m_columnas;	    
	   lvcolumn.cx = tamax*2;
	   InsertColumn(m_columnas, &lvcolumn);	
       m_columnas++;
    }

    AjustaColumnaFinal();

    return TRUE;
}

BOOL CTablaView::EnviaCamcolu()
{
    long tam;
    long respuesta;
    long modo = 1;
    int operacion = 13;
    char *p;
    char tmp[5000];
    

    p = tmp;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 7) + (sizeof(int)*3) + 100 + (m_def->m_n_campos_col*sizeof(int));
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);    
    memcpy((void *)p,(void *)&(m_def->m_n_campos_col),sizeof(int));
    p += sizeof(int);
    memcpy((void *)p,(void *)m_def->m_colcampos,(m_def->m_n_campos_col*sizeof(int)));

    DespachadorInterno(13,(void *)tmp);    

    return TRUE;
}

BOOL CTablaW::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    if ((wParam & 0xffff) == ID_CUSTOM_ESTADO)
    {
		if (lParam == 1001)
		{
			m_tabla.m_gmodo &= ~1;
			m_gmodo = m_tabla.m_gmodo;
			m_tabla.m_noesconsulta = FALSE;
		} else if (lParam == 1100)
		{
			m_tabla.m_gmodo = m_tabla.m_ini_gmodo;			
			m_gmodo = m_tabla.m_gmodo;
			m_tabla.m_noesconsulta = TRUE;
		}		
		m_tabla.PonTotalRegistros(m_tabla.m_registros);
		m_tabla.ActualizaVentana();
		/*
		Invalidate();
		UpdateWindow();
		*/
		
		return TRUE;
	}
	return CExtScrollWnd::OnCommand(wParam, lParam);
}

int CTablaView::DespachadorInterno(int parametro, void *buffer)
{  
   if ((m_gmodo & 0x1000000))
   {
		return pDsSys->DespachadorInterno9(parametro,buffer);
   }
   else
   {
		int r = pDsSys->DespachadorInterno(parametro,buffer);
		return r;
   }
}

BOOL CTablaView::DobleClick()
{   
    long tam;
    long respuesta;
    long modo = 1;        
    int operacion = 19;
    char *p;
    char tmp[512];
	int col;

    p = tmp;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 6) + (sizeof(int)*3) + 300;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int); 
	*p = 0;		
	col = m_curcol;
	memcpy((void *)(p+1),(void *)&col,sizeof(int));
    DespachadorInterno(13,(void *)tmp);
    TeclaStd(TECLA_PING);
	if (!strcmp(p,"SKIPDEFAULT"))
	{		
		return TRUE;
	}
	
   return FALSE;
}


void CTablaView::FinReorden()
{
  if (m_reordenado > -1)
  {		
	  m_reordenado = -1;
  }
  if (m_reorden)
  {
	  delete [] m_reorden;
	  m_reorden = NULL;
  }
}

void CTablaView::PonWait()
{
	if (!m_prevcursor)
	{
		m_prevcursor = SetCursor(LoadCursor(NULL,IDC_WAIT));
	}
	else
	{
		m_cursorwaits++;
	}
}

void CTablaView::QuitaWait()
{
	if (m_prevcursor)
	{
		if (!m_cursorwaits)
		{
			SetCursor(m_prevcursor);
			m_prevcursor = NULL;
		}
		else
			m_cursorwaits--;
	}
}

void CTablaView::MarcasCabecera()
{
    LV_COLUMN Column;   
	int clave = m_def->m_info->clave_actual;
	int k,otra;

	int i;
	for (i = COLUMNA_INICIAL;i < m_columnas;i++)
	{
        memset(&Column,0,sizeof(Column));
        Column.mask = LVCF_IMAGE | LVCF_FMT /*| LVCF_SUBITEM*/;
		Column.iSubItem = i;
        GetColumn( i /*m_orderarray[i]*/, &Column );
        Column.mask = LVCF_IMAGE | LVCF_FMT /*| LVCF_SUBITEM*/;
		Column.iSubItem = i;
		Column.iImage = -1;
		if (m_reordenado != -1)
		{
			if (m_reordenado == i || (-(m_reordenado+2)) == i)
			{
					Column.iImage = 3;
					//Column.fmt   |= LVCFMT_BITMAP_ON_RIGHT;
			}
		}
		else
		{
			if (m_def->EsInicialClave(i-COLUMNA_INICIAL,FALSE,clave) > -1)
			{
				k = m_def->EsDeLaClave(i-COLUMNA_INICIAL,clave);
				if (k < 0 || m_def->k_orden(clave,k) >= 0)				
				   Column.iImage =  3;
				else
				   Column.iImage =  4; 
				//Column.fmt   |= LVCFMT_BITMAP_ON_RIGHT;
			}
			else
			{
				if ((otra = m_def->EsInicialClave(i-COLUMNA_INICIAL)) > -1)
				{
					/*
					k = m_def->EsDeLaClave(i-COLUMNA_INICIAL,otra);
					if (k < 0 || m_def->k_orden(otra,k) >= 0)
					   Column.iImage =  3;
					else
					   Column.iImage =  4; 
					*/
					Column.iImage =  2; 
					//Column.fmt   |= LVCFMT_BITMAP_ON_RIGHT;
				}
				else
				{
					Column.iImage = -1;
					//Column.fmt   &= ~LVCFMT_BITMAP_ON_RIGHT;
				}
			}
		}
		SetColumn( i /*m_orderarray[i]*/, &Column ) ;
	}    
}

int CTablaView::EnviaRetro(int operacion,int ida,int vuelta,char *buffer)
{
	char *p = buffer;            
	int tam,respuesta,modo = 1;

    memcpy((void *)p,(void *)&m_localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 3) + (sizeof(int)*2) + ida;
    respuesta = (sizeof(long) * 3) + (sizeof(int)*2) + vuelta;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);
    return DespachadorInterno(13,(void *)buffer);
}

void CInfoDef::PintaCamposStaticos()
{
	/*
	struct Info_Entra_Campo *pC;
	int i;
    for (i = 0;i < m_info->n_campos;i++)
    {
        pC = (struct Info_Entra_Campo *)((char *) ((char *)m_info) + *((int *)((char *) ((char *)m_info) + m_info->d_Campos + (i*sizeof(int)) )) );        		
        if ((pC->c_flags & 4))
        {
        }		
    }
	*/
}

BOOL CTablaView::LocalizaRegistroOrdenado(CString &str,long ini,long fin)
{
	if (fin == -1)
		fin = m_registros;

	int medio = ini + (fin-ini) / 2;
	int comp;
	CString str2;

	if (ini >= m_registros)
	{		
		medio = m_registros-1;
	}
	else if (medio != ini && medio != fin)
	{
		str2 = GetItemText( medio , m_curcol );
		str2.TrimLeft();
		comp = str2.CompareNoCase(str);
		
		if (comp < 0)
		{
			return LocalizaRegistroOrdenado(str,medio+1,fin);
		}
		else if (comp > 0)
		{
			return LocalizaRegistroOrdenado(str,ini,medio);
		}
	}
	else
	{
		medio = ini;
	}

	while(medio < (m_registros-1))
	{
		str2 = GetItemText( medio, m_curcol );
		str2.TrimLeft();
		comp = str2.CompareNoCase(str);
		if (comp >= 0)
			break;
		medio++;
	}
	while(medio > 0)
	{
		str2 = GetItemText( medio - 1, m_curcol );
		str2.TrimLeft();
		comp = str2.CompareNoCase(str);
		if (comp < 0)
			break;
		medio--;
	}

	NuevoCorriente(medio,TRUE);
	return TRUE;
}


struct pdb_header
{
	char name[32];
	unsigned short attributes;
	unsigned short version;
	unsigned long creationdate;
	unsigned long modificationdate;
	unsigned long lastbackupdate;
	unsigned long modificationnumber;
	unsigned long appinfoarea;
	unsigned long sortinfoarea;
	char databasetype[4];
	char creatorid[4];
	unsigned long uniqueidseed;
	unsigned long nextrecordlistid;
	unsigned short nrecords;
};

struct pdb_record
{
	unsigned long recorddataoffset;	
	unsigned char uniqueid[4];
};


struct pdb_appcampo
{
	unsigned short offsetdesc;
	unsigned short tipocampo;
	unsigned short nulo;
};

struct pdb_appinfo
{
	char relleno[276];
	char id[12];
	unsigned short size;
	unsigned short offsetdesc;
	unsigned short ncampos;
	unsigned short minlen;
	char unknow[4];
	/*struct pdb_appcampo campos[n];*/
	/*char *descripciones[n];*/
	char relleno2[128]; /* probar a poner 9 nulos por campo */
};

const char *quita_path(const char *ff)
{
   const char *f0 = ff;
   ff += strlen(ff);
   while (ff >= f0 && *ff != '\\' && *ff != '/') ff--;
   ff++;
   if (ff == f0 && *f0 && *(f0+1) == ':')
   {
      ff += 2;
   }
   return(ff);
}

#define Uchar unsigned char
static int stint( short int i,Uchar *p )                   /* create 2 byte MIF from an int */
{
    Uchar *q;
    
    q = ( Uchar *)&i;
    *p = q[1]; p[1] = *q;
    return( 0 );
}
int stlong( long l,Uchar *p )              /* make MIF from a long */
{
    register Uchar *q;
    
    q = ( Uchar *)&l;
    *p = q[3];
    p[1] = q[2];
    p[2] = q[1];
    p[3] = *q;
    return( 0 );
}

void CTablaView::OnExportaRpt() 
{
	AfxMessageBox("No existe el modulo de creación de Crystal Reports",MB_OK | MB_ICONHAND);

	return;
}


static void PathExcel(CString &destino)
{
		HKEY hKey = NULL;			
		LONG retv;			
		DWORD Type;	
		DWORD Size;		
		char tmpp[512];
		char *elpath;
		CLSID clsid;
		USES_CONVERSION;
		
		strcpy(tmpp+1,"Excel.Application");		
		tmpp[0] = 0;			
		elpath = tmpp;

		if (CLSIDFromProgID(T2OLE(tmpp+1),&clsid ) == S_OK)
		{
			LPOLESTR psz = NULL;
			StringFromCLSID(clsid,&psz);
			if (psz)
			{					
				sprintf(tmpp,"CLSID\\%s\\LocalServer32",OLE2T(psz));
   				if ((retv = RegOpenKeyEx(HKEY_CLASSES_ROOT,tmpp,0,KEY_READ,&hKey)) == ERROR_SUCCESS)
				{				
					
					Type = REG_DWORD;		
					Size = sizeof(tmpp);
					if (RegQueryValueEx(hKey,NULL,NULL,&Type,(unsigned char *)tmpp,&Size) == ERROR_SUCCESS)
					{
						if (Type != REG_SZ)
						{
							tmpp[0] = 0;
						}
					}
					RegCloseKey(hKey);
				}
				else
				{
					sprintf(tmpp,"CLSID\\%s\\InprocServer32",OLE2T(psz));
   					if ((retv = RegOpenKeyEx(HKEY_CLASSES_ROOT,tmpp,0,KEY_READ,&hKey)) == ERROR_SUCCESS)
					{				
						
						Type = REG_DWORD;		
						Size = sizeof(tmpp);
						if (RegQueryValueEx(hKey,NULL,NULL,&Type,(unsigned char *)tmpp,&Size) == ERROR_SUCCESS)
						{
							if (Type != REG_SZ)
							{
								tmpp[0] = 0;
							}
						}
						RegCloseKey(hKey);
					}
				}
				CoTaskMemFree(psz);
			}
		}

		if (*elpath == '"')
		{	
			elpath++;
			while(*elpath && *elpath != '"')
				elpath++;
			*elpath = 0;
			elpath = tmpp + 1;
		}
		else
		{				
			while(*elpath && *elpath != ',')
				elpath++;
			*elpath = 0;
			elpath = tmpp;
		}
		destino = elpath;
		destino.Replace("/automation","");
}


void CTablaView::OnExportaPlb() 
{	
	CDlgExporta dlg(this);	

	char tmp2[512];		

	strcpy(tmp2,theApp.m_dirbase);
	strcat(tmp2,"crystal");
	mkdir(tmp2);
	strcat(tmp2,"\\");
    strcat(tmp2,quita_path(m_def->m_RegPath));
	dlg.m_destino = tmp2;


	if (dlg.DoModal() != IDOK)
		return;

	if (!dlg.m_excel && theApp.alt_excel) {
		OnExportaExcel((LPCSTR)dlg.m_destino);
		return;
	}

	if (!dlg.m_dbf)
	{
		if (!ExportaDbf(dlg.m_destino))
		{
			return;
		}		
	}
	else
	{
		struct pdb_header hdr;
		struct pdb_record rec;
		struct pdb_appinfo app;
		struct pdb_appcampo cam;
		long offsetdata,datol;
		int hdrsize    = 78;
		int recordsize = 8;
		int i,ncampos,ij,rr,ii;
		unsigned short nrecords;
		int file;
		short offset,size,vacio = 0;	
		char tmp[512];	
		double datod;
		CString nombre = "",str;
		char *px;
		LVCOLUMN ccc;
		int columnas = m_columnas-COLUMNA_FINAL;
		CDlgProgreso dlgp;
		long progreso;
		unsigned char idouble[8],ic;	
		extern double atof(const char *s);


		nombre = dlg.m_destino;
		
		sprintf(tmp2,"%s.pdb",nombre);
		file = open(tmp2,O_RDWR|O_CREAT|O_TRUNC|O_BINARY,S_IREAD|S_IWRITE);
		if (file < 0)
			return;

		dlg.m_destino = tmp2;


		PonWait();
		dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
		str = "Grabando ";
		str += nombre;
		str += ".plb";
		dlgp.SetWindowText(str);
		dlgp.m_progreso.SetRange32( 0,m_registros/20+1);
		dlgp.ShowWindow(SW_SHOW);
		dlgp.BringWindowToTop();

		memset(&hdr,0,sizeof(hdr));
		strcpy(hdr.name,quita_path(nombre));
		stint((short)8,(Uchar *)&(hdr.attributes));
		stint((short)1,(Uchar *)&(hdr.version));
		stlong((long)time(NULL),(Uchar *)&(hdr.creationdate));
		stlong((long)time(NULL),(Uchar *)&(hdr.modificationdate));
		
		nrecords = (unsigned short)m_registros;

		stlong((long)hdrsize+nrecords*recordsize,(Uchar *)&(hdr.appinfoarea));

		memcpy(hdr.databasetype,"DATA",4);	
		memcpy(hdr.creatorid,"1111",4);

		stint((short)nrecords,(Uchar *)&(hdr.nrecords));

		write(file,&hdr,sizeof(hdr));

		memset(&app,0,sizeof(app));	

		/* es por la apriencia del progreso la primera vez */
		lseek(file,(long)hdrsize,SEEK_SET);
		for (progreso = 1;progreso <= (m_registros/40);progreso++)
		{
			dlgp.m_progreso.SetPos(progreso);
			write(file,&(app.relleno),320);
		}
		lseek(file,(long)hdrsize+nrecords*recordsize,SEEK_SET);


		ncampos = columnas - COLUMNA_INICIAL;

		
		strcpy(app.id,"APPFORG2");
		offset = 24+ncampos*6;
		stint((short)offset,(Uchar *)&(app.offsetdesc));
		offset += (strlen(hdr.name)+1);
		size = offset;

		int ml = 0;

		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			/*i = m_orderarray[ij];*/
			i = ij;
			try
			{	 
				memset((void *)&ccc,0,sizeof(ccc));
				ccc.mask = LVCF_TEXT;
				ccc.pszText = tmp2;
				ccc.cchTextMax = sizeof(tmp2)-1;
				GetColumn( i, &ccc );
				modocolumna(1,ij-COLUMNA_INICIAL+1,tmp);
            
				size += (strlen(tmp2)+10); /* incluye los nulos */

				switch((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf))
				{
				case 3:
					ml += 8;
					break;
				case 1:			
				case 2:
					ml += 4;					
					break;
				default:
					ml += 1;
					break;
				}		

			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}

		stint((short)size,(Uchar *)&(app.size));	
		stint((short)ncampos,(Uchar *)&(app.ncampos));
		stint((short)ml,(Uchar *)&(app.minlen));		

		write(file,app.relleno,sizeof(app.relleno));
		write(file,&(app.id),sizeof(app.id));
		write(file,&(app.size),sizeof(app.size));
		write(file,&(app.offsetdesc),sizeof(app.offsetdesc));
		write(file,&(app.ncampos),sizeof(app.ncampos));
		write(file,&(app.minlen),sizeof(app.minlen));
		write(file,&(app.unknow),sizeof(app.unknow));
		
		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			/*i = m_orderarray[ij];		*/
			i = ij;
			try
			{	 
				memset((void *)&ccc,0,sizeof(ccc));
				ccc.mask = LVCF_TEXT;
				ccc.pszText = tmp2;
				ccc.cchTextMax = sizeof(tmp2)-1;
				GetColumn( i, &ccc );
				/*modocolumna(1,ij-COLUMNA_INICIAL+1,tmp);*/
            
				memset(&cam,0,sizeof(cam));
				stint((short)offset,(Uchar *)&(cam.offsetdesc));
				offset += (strlen(tmp2)+1);		

				switch((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf))
				{
				case 3:
					stint((short)8,(Uchar *)&(cam.tipocampo));
					break;
				case 1:			
				case 2:
					stint((short)3,(Uchar *)&(cam.tipocampo));
					break;
				/*
				case 5:
					stint((short)7,&(cam.tipocampo));
					break;
				*/
				default:
					stint((short)5,(Uchar *)&(cam.tipocampo));
					break;
				}		

				write(file,&(cam.offsetdesc),sizeof(cam.offsetdesc));
				write(file,&(cam.tipocampo),sizeof(cam.tipocampo));
				write(file,&(cam.nulo),sizeof(cam.nulo));
			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}

		write(file,hdr.name,strlen(hdr.name)+1);	
		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			/*i = m_orderarray[ij];*/
			i = ij;
			try
			{	 
				memset((void *)&ccc,0,sizeof(ccc));
				ccc.mask = LVCF_TEXT;
				ccc.pszText = tmp2;
				ccc.cchTextMax = sizeof(tmp2)-1;
				GetColumn( i, &ccc );
				modocolumna(1,ij-COLUMNA_INICIAL+1,tmp);
            
				write(file,tmp2,strlen(tmp2)+1);	
			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}
		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			write(file,app.relleno2,9);
		}
		offsetdata = lseek(file,0L,SEEK_CUR);


		for (rr = 1;rr <= m_registros;rr++)
		{
			if (!((rr-1)%40))
			{
				progreso++;
				dlgp.m_progreso.SetPos(progreso);
			}
			try
			{	 
				ii = (int)rr-1;
				if (!m_cache.Lookup( ii , px )) 
				{
					Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
				}                

				memset(&rec,0,sizeof(rec));	
				lseek(file,hdrsize+ii*recordsize,SEEK_SET);
				stlong(offsetdata,(Uchar *)&(rec.recorddataoffset));
				stlong(rr,(Uchar *)rec.uniqueid);
				rec.uniqueid[0] = 0x40;/* atributes = dirty */
				write(file,&(rec.recorddataoffset),sizeof(rec.recorddataoffset));
				write(file,rec.uniqueid,sizeof(rec.uniqueid));
				lseek(file,offsetdata,SEEK_SET);
            
				for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
				{
					i = m_orderarray[ij];
					str = GetItemText( rr-1, i );
					/*
					for (px = (char *)(LPCSTR)str;*px;px++)
					{
					   oem_a_ansi((unsigned char *)px);
					}
					*/
					modocolumna(rr+1,ij-COLUMNA_INICIAL+1,tmp);

					if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))
					{
						str.Remove('.');
						str.Replace(',','.');
					}
                
					switch((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf))
					{
					case 3:	
						datod = atof(str);
						memcpy(idouble,&datod,8);
						ic = idouble[0];
						idouble[0] = idouble[7];
						idouble[7] = ic;
						ic = idouble[1];
						idouble[1] = idouble[6];
						idouble[6] = ic;
						ic = idouble[2];
						idouble[2] = idouble[5];
						idouble[5] = ic;
						ic = idouble[3];
						idouble[3] = idouble[4];
						idouble[4] = ic;
						offsetdata += write(file,idouble,8);
						break;
					case 1: /* los guardamos como longs */
					case 2:
						stlong(atol(str),(Uchar *)&datol);
						offsetdata += write(file,&datol,sizeof(datol));
						break;
					/*
					case FECHATIPO:
						stint((short)7,&(cam.tipocampo));
						break;
					*/
					default:					
						offsetdata += write(file,str,strlen(str)+1);
						break;
					}				
				}
			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}        	

		close(file);

		dlgp.DestroyWindow();

		QuitaWait();
	}

	if (!dlg.m_noabrir)
		return;

	if (!dlg.m_crystal)
	{
		/*
		HINSTANCE modulo = LoadLibrary("dscristal.dll");
		if (modulo)
		*/
		{
			typedef void (WINAPI *_EntryPoint2)(DWORD handle,
					  				  DWORD funcion,
									  DWORD extra
									  ); 
			extern void WINAPI DespachaInterface(void *parametro);

			/*
			_EntryPoint2 EntryPoint2 = (_EntryPoint2)GetProcAddress(modulo,"DsEntryPoint2");
			*/
			void WINAPI DscristalEntryPoint2(DWORD handle,DWORD funcion,DWORD extra);			

			_EntryPoint2 EntryPoint2 = (_EntryPoint2)DscristalEntryPoint2;
			if (EntryPoint2)
			{					
				 DWORD extra;				 
				 char tmp[1024];
				 int *p1;
				 char *p;
				 int tam = 0;

				 p1 = (int *)tmp;				 
				 *p1 = 1;
				 p1++;
				 *p1 = 0;
				 p1++;
				 *p1 = 0;
				 p = tmp + (sizeof(int)*3);

				 
				 strcpy(p,dlg.m_destino);
				 *(char *)(quita_path(p)) = 0;

				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);
				 
				 strcpy(p,dlg.m_programa);

				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				 /* 1 para sacarlo directamente */
				 *p = 0;
				 
				 p++;
				 (*p1)++;

				 CPrintDialog pdlg(FALSE);
	             theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );
				 strcpy(p,pdlg.GetDriverName());
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				 strcpy(p,pdlg.GetDeviceName());
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				 strcpy(p,pdlg.GetPortName( ));
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				
				 extra = (DWORD)tmp;
  

				 if (theApp.m_9wnd)
  					 pDsSys->DespachadorInterno9(801,NULL);
				 else
					 pDsSys->DespachadorInterno(801,NULL);    

				 EntryPoint2((DWORD)0,(DWORD)DespachaInterface,extra);

				 if (theApp.m_9wnd)
  					 pDsSys->DespachadorInterno9(802,NULL);
				 else
					 pDsSys->DespachadorInterno(802,NULL);				
			}
			/*
			else
			{
				AfxMessageBox("Modulo dscristal.dll obsoleto para esta funcion.");
			}
			FreeLibrary(modulo);*/
		}
		/*
		else
		{
			AfxMessageBox("Falta el modulo dscristal.dll");
		}
		*/
	}
	else if (!dlg.m_predeterminado)
	{
		::ShellExecute(NULL, "open",dlg.m_destino,NULL,NULL, SW_SHOWNORMAL);
	}
	else
	{
		/*
		char *envexcel = ::getenv("DS_EXCEL_REMOTE");
		if (envexcel) {
			char tmp[512];
			char tmp2[512];
			strcpy(tmp,envexcel);
			char *pp = strstr(tmp,"%1");
			if (pp)
				*(pp+1) = 's';
			sprintf(tmp2,tmp,dlg.m_destino);
			::ShellExecute(NULL, "open","mstsc.exe",tmp2,NULL, SW_SHOWNORMAL);
		}
		else */{
		if (!dlg.m_excel || dlg.m_programa.Trim().IsEmpty())
			{
				PathExcel(dlg.m_programa);
			}
			//::MessageBox(NULL,dlg.m_programa,dlg.m_destino,MB_OK);
			::ShellExecute(NULL, "open",dlg.m_programa,dlg.m_destino,NULL, SW_SHOWNORMAL);
		}
	}	
	
}

void CTablaView::OnExportaXml() 
{
	FILE *fout;
	int ij,i,rr,ii;
	LVCOLUMN ccc;
	char tmp2[1024];
	int columnas = m_columnas-COLUMNA_FINAL;
	CString str;
	char *px;
	CString nombre,s,sdir;

	/* grabamos formato */
	
	nombre = quita_path(m_def->m_RegPath);
	
	sdir = theApp.m_dirbase;
	sdir += "xml";
	mkdir(sdir);
	sdir += "\\";

	s = sdir+nombre;
	s += ".xsl";


	fout = fopen(s,"w");
	if (!fout)
	{
		return;
	}

    fprintf(fout,"<?xml version='1.0' encoding='ISO-8859-1'?>\r\n");
    fprintf(fout,"<xsl:stylesheet version='2.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:fn='http://www.w3.org/2005/02/xpath-functions' xmlns:xdt='http://www.w3.org/2005/02/xpath-datatypes'>\r\n");
    fprintf(fout,"<xsl:output version='1.0' encoding='ISO-8859-1' indent='no' omit-xml-declaration='no' media-type='text/html' />\r\n");
    fprintf(fout,"<xsl:template match='/'>\r\n");
	fprintf(fout,"<html>\r\n");
	GetParent()->GetWindowText(s);
	fprintf(fout,"<head><title>%s</title></head>\r\n",s);
	fprintf(fout,"<body>\r\n");	
    fprintf(fout,"<h5 align='right'>\r\n");	
	{
		CTime t = CTime::GetCurrentTime();
		fprintf(fout,"<h5>%02d-%02d-%04d</h5>\r\n",t.GetDay(),t.GetMonth(),t.GetYear());
	}
    fprintf(fout,"</h5>\r\n");	
    fprintf(fout,"<p>\r\n");	
    fprintf(fout,"<span style='font-family:Verdana; font-size:large; '>%s</span>\r\n",s);	
    fprintf(fout,"</p>\r\n");
    fprintf(fout,"<xsl:for-each select='consulta_%s'>\r\n",quita_path(m_def->m_RegPath));	
    fprintf(fout,"<center align='left'>\r\n");
	fprintf(fout,"<table border='1'>\r\n");                            
	fprintf(fout,"<thead>\r\n");
	fprintf(fout,"<tr>\r\n");

	for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
	{
		i = m_orderarray[ij];		
		try
		{	 

			memset((void *)&ccc,0,sizeof(ccc));
			ccc.mask = LVCF_TEXT;
			ccc.pszText = tmp2;
			ccc.cchTextMax = sizeof(tmp2)-1;
			GetColumn( i, &ccc );

			fprintf(fout,"<td style='background-color:#FFFFE5; '>\r\n");			
			fprintf(fout,"<span style='background-color:#FFFFE5; color:black; font-family:Verdana; font-weight:bold; '>%s</span>\r\n",tmp2);			
			fprintf(fout,"</td>\r\n");
		}
		catch (...)
		{
			//ASSERT(FALSE);
		}
	}

	if (m_totalizadores)
	{
        fprintf(fout,"<tfoot>\r\n");
        fprintf(fout,"<tr>\r\n");
		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			i = m_orderarray[ij];		
			try
			{	 
				int kp;
				s = "";
				for (kp = 1;kp <= m_totalizadores[0];kp++)
				{						
					if (m_totalizadores[kp] == i && ::IsWindow(m_totaledit[kp-1].m_hWnd))
					{
						m_totaledit[kp-1].GetWindowText(s);
						s.Remove('.');
						s.Replace(',','.');
						break;
					}
				}
                fprintf(fout,"<td align='right' style='border-top-color:gray; border-top-style:solid; border-top-width:thin; '>\r\n");
                fprintf(fout,"<span style='font-family:Verdana; font-weight:bold; '>%s</span>\r\n",s);
                fprintf(fout,"</td>\r\n");
			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}
        fprintf(fout,"</tr>\r\n");
        fprintf(fout,"</tfoot>\r\n");
	}

    fprintf(fout,"</tr>\r\n");
    fprintf(fout,"</thead>\r\n");


	fprintf(fout,"<tbody>\r\n");
    fprintf(fout,"<xsl:for-each select='registro'>\r\n");
	fprintf(fout,"<tr>\r\n");

	for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
	{
		i = m_orderarray[ij];		
		
		if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))		
			fprintf(fout,"<td align='right'>\r\n");
		else
			fprintf(fout,"<td>\r\n");

		
		fprintf(fout,"<xsl:for-each select='C%03d' >\r\n",i);			
		fprintf(fout,"<span style='font-family:Verdana; '>\r\n");
		fprintf(fout,"<xsl:apply-templates />\r\n");
		fprintf(fout,"</span>\r\n");        
		fprintf(fout,"</xsl:for-each>\r\n");        
        fprintf(fout,"</td>\r\n");
	}

    fprintf(fout,"</tr>\r\n");
    fprintf(fout,"</xsl:for-each>\r\n");
    fprintf(fout,"</tbody>\r\n");

    fprintf(fout,"</table>\r\n");
    fprintf(fout,"</center>\r\n");
    fprintf(fout,"</xsl:for-each>\r\n");

	fprintf(fout,"<center>\r\n");
    fprintf(fout,"<br /><br />\r\n"); // reservado "PIE DEL INFORME" :)
    fprintf(fout,"<span style='right:auto; '></span>\r\n"); // reservado "Numero de Página (solo para PDF):" :)
    fprintf(fout,"<br />\r\n");
    fprintf(fout,"</center>\r\n");

	fprintf(fout,"</body>\r\n");
	fprintf(fout,"</html>\r\n");
    fprintf(fout,"</xsl:template>\r\n");
    fprintf(fout,"</xsl:stylesheet>\r\n");

	fclose(fout);

	/* grabamos esquema */
    s = sdir+nombre;
	s += ".xsd";

	fout = fopen(s,"w");
	if (!fout)
	{
		return;
	}
	fprintf(fout,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\r\n");
	fprintf(fout,"<!--  XML Consulta: %s [%s] %s -->\r\n","",quita_path(m_def->m_RegPath),"");

	fprintf(fout,"<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\r\n");
	fprintf(fout,"<xsd:annotation>\r\n");
	fprintf(fout,"<xsd:documentation xml:lang=\"es\">XML Schema - Tabla %s (%s) .</xsd:documentation>\r\n",quita_path(m_def->m_RegPath),"");
    fprintf(fout,"</xsd:annotation>\r\n");	

	fprintf(fout,"<xsd:element name=\"consulta_%s\">\r\n",quita_path(m_def->m_RegPath));
	fprintf(fout,"<xsd:complexType>\r\n");
	fprintf(fout,"<xsd:sequence>\r\n");

	fprintf(fout,"<xsd:element name=\"registro\" minOccurs=\"0\" maxOccurs=\"unbounded\">\r\n");
	fprintf(fout,"<xsd:complexType>\r\n");
	fprintf(fout,"<xsd:sequence>\r\n");

	for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
	{
		i = m_orderarray[ij];		
		try
		{	 

			memset((void *)&ccc,0,sizeof(ccc));
			ccc.mask = LVCF_TEXT;
			ccc.pszText = tmp2;
			ccc.cchTextMax = sizeof(tmp2)-1;
			GetColumn( i, &ccc );			

			fprintf(fout,"<xsd:element name=\"C%03d\">\r\n",i);
			fprintf(fout,"<xsd:annotation>\r\n");
			fprintf(fout,"<xsd:documentation xml:lang=\"es\">%s</xsd:documentation>\r\n",tmp2);
			fprintf(fout,"</xsd:annotation>\r\n");
			fprintf(fout,"<xsd:simpleType>\r\n");
			

			switch((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf))
			{
			case 3:
				fprintf(fout,"<xsd:restriction base=\"xsd:double\" >");
				fprintf(fout,"</xsd:restriction>\r\n");
				break;
			case 1:			
			case 2:
				fprintf(fout,"<xsd:restriction base=\"xsd:integer\" >");
				fprintf(fout,"</xsd:restriction>\r\n");
				break;
			
			case 5:
				fprintf(fout,"<xsd:restriction base=\"xsd:date\" >");
				fprintf(fout,"</xsd:restriction>\r\n");
				break;
			
			default:
				fprintf(fout,"<xsd:restriction base=\"xsd:string\" >\r\n");
				fprintf(fout,"<xsd:minLength value=\"0\"/>\r\n");
				/*fprintf(fout,"<xsd:maxLength value=\"%d\"/>\r\n",pfi->f_campos[i].c_qc);*/				
				fprintf(fout,"</xsd:restriction>\r\n");
				break;
			}		

			fprintf(fout,"</xsd:simpleType>\r\n</xsd:element>\r\n");


		}
		catch (...)
		{
			//ASSERT(FALSE);
		}
	}	
	fprintf(fout,"</xsd:sequence>\r\n");
    fprintf(fout,"</xsd:complexType>\r\n");
    fprintf(fout,"</xsd:element>\r\n");
	fprintf(fout,"</xsd:sequence>\r\n");
    fprintf(fout,"</xsd:complexType>\r\n");
    fprintf(fout,"</xsd:element>\r\n");
	fprintf(fout,"</xsd:schema>\r\n\r\n");


	fclose(fout);


	/* grabamos data */
    s = sdir+nombre;
	s += ".xml";

	fout = fopen(s,"w");
	if (!fout)
	{
		return;
	}
	

	fprintf(fout,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\r\n");
	fprintf(fout,"<!--  Datos Consulta: %s [%s] %s -->\r\n","",quita_path(m_def->m_RegPath),"");  
    fprintf(fout,"<?xml-stylesheet type='text/xsl' href='%s.xsl' ?>\r\n",nombre);
	fprintf(fout,"<consulta_%s xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' ",quita_path(m_def->m_RegPath));
	fprintf(fout,"xsi:noNamespaceSchemaLocation='%s.xsd'>\r\n",nombre);

	for (rr = 1;rr <= m_registros;rr++)
	{
		try
		{	 
			ii = (int)rr-1;
			if (!m_cache.Lookup( ii , px )) 
			{
				Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
			}                
        
			fprintf(fout,"<registro>\n");

			for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
			{
				i = m_orderarray[ij];
				str = GetItemText( rr-1, i );

				fprintf(fout,"<C%03d>",i);
				if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))
				{
					str.Remove('.');
					str.Replace(',','.');
				}
				if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 5)
				{
					int an,ms,di;
					str.TrimLeft();
					an = atoi(str.Mid(6,4));
					ms = atoi(str.Mid(3,2));
					di = atoi(str.Mid(0,2));
					if (!an)
						an = 1;
					if (!di)
						di = 1;
					if (!ms)
						ms = 1;
					fprintf(fout,"%04d-%02d-%02d",an,ms,di);
				}
				else
				{
					/*
					char *texto;
					texto = str.LockBuffer( );
		            for (int o = 0;texto[o];o++)
					{
						oem_a_ansi((unsigned char *)texto+o);
					}					
					str.UnlockBuffer( );
					*/
					str.Replace((unsigned char)146,' ');

					str.Replace("&","{{amp}}");
					str.Replace("<","&lt;");
					str.Replace(">","&gt;");
					str.Replace("'","&apos;");
					str.Replace("\"","&quot;");
					str.Replace("{{amp}}","&amp;");
					fprintf(fout,"%s",(LPCSTR)str);
				}
				fprintf(fout,"</C%03d>\n",i);
			}
			fprintf(fout,"</registro>\n");
            
		}
		catch (...)
		{
			//ASSERT(FALSE);
		}
	}

	fprintf(fout,"</consulta_%s>\r\n",quita_path(m_def->m_RegPath));

	fclose(fout);

	sdir.Replace('/','\\');
	s.Replace('/','\\');
	::ShellExecute(NULL, "open",s,s,sdir,SW_SHOWNORMAL);
}

void CTablaW::OnExportaXml() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnExportaXml();
    }	
}

void CTablaView::OnGridimprime() 
{
	int m[300];
	int ij,i;
	char tmp[2040];
	char tmp2[512];
	int columnas = m_columnas-COLUMNA_FINAL;
	LVCOLUMN ccc;
	CString s;

	GetParent()->GetWindowText(s);

	memset((void *)&m,0,sizeof(m));
	/*
	ok ... falta crear el asunto para imprimir a traves de la dll para 
	compatibilidad con impresoras "custom"

    FuncionesImpresion(5,m);
	*/

	/* de momento impresion straigh on windows */
	FuncionesImpresion(1,m);
	if (m[0])
	{
		strcpy(tmp,s);
		strcat(tmp,"\r\n\r\n");
		FuncionesImpresion(2,tmp);

		for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
		{
			i = m_orderarray[ij];		
			try
			{	 

				memset((void *)&ccc,0,sizeof(ccc));
				ccc.mask = LVCF_TEXT;
				ccc.pszText = tmp2;
				ccc.cchTextMax = sizeof(tmp2)-1;
				GetColumn( i, &ccc );

				

			}
			catch (...)
			{
				//ASSERT(FALSE);
			}
		}

		FuncionesImpresion(3,m);
	}


#ifdef NADANADA	
	for (rr = 1;rr <= m_registros;rr++)
	{
		try
		{	 
			ii = (int)rr-1;
			if (!m_cache.Lookup( ii , px )) 
			{
				Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
			}                
        
			fprintf(fout,"<registro>\n");

			for (ij = COLUMNA_INICIAL;ij < columnas;ij++)
			{
				i = m_orderarray[ij];
				str = GetItemText( rr-1, i );

				fprintf(fout,"<C%03d>",i);
				if ((m_flagcampos[ i - COLUMNA_INICIAL] & 16))
				{
					str.Remove('.');
					str.Replace(',','.');
				}
				if ((m_flagcampos[ i - COLUMNA_INICIAL] & 0xf) == 5)
				{
					int an,ms,di;
					an = atoi(str.Mid(6,4));
					ms = atoi(str.Mid(3,2));
					di = atoi(str.Mid(0,2));
					if (!an)
						an = 1;
					if (!di)
						di = 1;
					if (!ms)
						ms = 1;
					fprintf(fout,"%04d-%02d-%02d",an,ms,di);
				}
				else
				{
					/*
					char *texto;
					texto = str.LockBuffer( );
		            for (int o = 0;texto[o];o++)
					{
						oem_a_ansi((unsigned char *)texto+o);
					}					
					str.UnlockBuffer( );
					*/
					str.Replace((unsigned char)146,' ');

					str.Replace("&","{{amp}}");
					str.Replace("<","&lt;");
					str.Replace(">","&gt;");
					str.Replace("'","&apos;");
					str.Replace("\"","&quot;");
					str.Replace("{{amp}}","&amp;");
					fprintf(fout,"%s",(LPCSTR)str);
				}
				fprintf(fout,"</C%03d>\n",i);
			}
			fprintf(fout,"</registro>\n");
            
		}
		catch (...)
		{
			//ASSERT(FALSE);
		}
	}	
#endif

}

void CTablaW::OnGridimprime() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnGridimprime();
    }
}

void CTablaW::OnExportaPlb() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnExportaPlb();
    }
}


void CTablaW::OnExportaRpt() 
{
	if (m_tabla.m_hWnd)
	{
        m_tabla.OnExportaRpt();
    }		
}

BOOL CTablaView::ExportaDbf(CString &destino)
{
	char tmp[512];
	char tmp2[512];
	int campo;
	int file;
	int len;
	char c;	
	long rr;
	CDlgProgreso dlgp;
	long progreso = 0,pos;
	int ii,ij;
	char *px;
	CString str;

	strcpy(tmp2,destino);
    strcat(tmp2,".dbf");

	destino = tmp2;

	file = open(tmp2,O_RDWR|O_CREAT|O_TRUNC|O_BINARY,S_IREAD|S_IWRITE);
	if (file < 0)
	   return FALSE;


	PonWait();
	dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
	str = "Grabando ";
	str += tmp2;	
	dlgp.SetWindowText(str);
	dlgp.m_progreso.SetRange32( 0,m_registros/40+1);
	dlgp.ShowWindow(SW_SHOW);
	dlgp.BringWindowToTop();

    struct dbf_desc dbf;
    struct dbf_campo dbc;

    memset(&(dbf.cabeza),0,sizeof(dbf.cabeza));
    dbf.cabeza.id = 0x3;
    dbf.cabeza.code_page = 0x01;

    time_t a; 
    struct tm *lc;
    a = time(NULL);
    if (a)
    {
        lc = localtime(&a);
        if (lc)
        {
			dbf.cabeza.any_update = lc->tm_year;
			dbf.cabeza.mes_update = (lc->tm_mon)+1;
			dbf.cabeza.dia_update = lc->tm_mday;
        }
    }
	
    lseek(file,0L,SEEK_SET);
    len = 0;
    write(file,&(dbf.cabeza),sizeof(dbf.cabeza));

    dbf.ncampos = m_columnas - COLUMNA_FINAL - COLUMNA_INICIAL;
	
	int i;
    for (i = 0;i < dbf.ncampos;i++)
    {
		campo = m_def->m_colcampos[i];
        memset(&dbc,0,sizeof(dbc));	  
		strcpy(tmp,m_def->c_nombre(campo,TRUE));
		tmp[11] = 0;
        strcpy((char *)dbc.nombre,tmp);
		switch((m_flagcampos[ i ] & 0xf))
		{
			/*
		case 3:
		case 4:
			// double 
            dbc.tipo = 'B';
            dbc.decimales = 6; //m_def->Campo(campo)->	
            dbc.len = 8; // ?
			break;
		case 1:			
		case 2:
			// short - int 
            dbc.tipo = 'I';
            dbc.decimales = 0;
            dbc.len = 4;
			break;		
			*/
		case 3:
		case 4:					
		case 1:			
		case 2:
			if ((m_flagcampos[ i ] & 0xf) > 2)
				dbc.decimales = 5;
			else
				dbc.decimales = 0;
			dbc.tipo = 'N';					
			dbc.len = (unsigned char)m_def->Campo(campo,TRUE)->c_maxqc;
			break;		
		case 5:
			/* fecha */			
            dbc.tipo = 'D';
            dbc.decimales = 0; 
            dbc.len = 8; /* siempre 8 !! */
			break;		
		default:
			/* alfa */
            dbc.tipo = 'C';
            dbc.flags = 0x04;
            dbc.decimales = 0;			
            dbc.len = (unsigned char)m_def->Campo(campo,TRUE)->c_qc;
			break;
		}		

        dbc.data_address = len;		  
        len += dbc.len;
        write(file,&dbc,sizeof(dbc));
	}

    c = 13;
    write(file,&c,1);
    memset(tmp2,0,263);
    write(file,tmp2,263);
    
    dbf.cabeza.len_header = (unsigned short)lseek(file,0L,SEEK_CUR);
    len++;
    dbf.cabeza.len_record = len;
    lseek(file,0L,SEEK_SET);
    write(file,&(dbf.cabeza),sizeof(dbf.cabeza));


    for (rr = 1;rr <= m_registros;rr++)
    {
		if (!((rr-1)%40))
		{
			progreso++;
			dlgp.m_progreso.SetPos(progreso);
		}
        try
        {	 
            ii = (int)rr-1;
            if (!m_cache.Lookup( ii , px )) 
            {
                Cacheamelo(rr,rr+MAX_CACHE-MAX_CACHE/5);
            }

			pos = (rr - 1L) * (dbf.cabeza.len_record)  + dbf.cabeza.len_header;   
			lseek(file,pos,SEEK_SET);
			write(file,&c,1);    
			dbf.cabeza.records++;
            
            for (ij = COLUMNA_INICIAL;ij < (m_columnas - COLUMNA_FINAL);ij++)
            {
                i = m_orderarray[ij];
	            str = GetItemText( rr-1, i );

				str.TrimLeft(' ');
				/*
                for (px = (char *)(LPCSTR)str;*px;px++)
                {
                   oem_a_ansi((unsigned char *)px);
                }
				*/                

				if ((m_flagcampos[ ij - COLUMNA_INICIAL] & 16))
				{
					str.Remove('.');
					str.Replace(',','.');
				}

				campo = m_def->m_colcampos[ij- COLUMNA_INICIAL];
				memset(&dbc,0,sizeof(dbc));	  
				strcpy(tmp,m_def->c_nombre(campo,TRUE));
				tmp[11] = 0;
				strcpy((char *)dbc.nombre,tmp);

				switch((m_flagcampos[ ij - COLUMNA_INICIAL] & 0xf))
				{
			    /*
				case 3:
				case 4:
					//double 			
					dbc.tipo = 'B';
					dbc.decimales = 4;//m_def->Campo(campo)->	
					dbc.len = 8; // ? 
					*((double *)tmp2) = (double)atof(str);
					break;
				case 1:			
				case 2:
					// short - int 
					dbc.tipo = 'I';
					dbc.decimales = 0;
					dbc.len = 4;
					*((long *)tmp2) = atol(str);
					break;		
				*/
				case 3:
				case 4:					
				case 1:			
				case 2:
					if ((m_flagcampos[ ij - COLUMNA_INICIAL] & 0xf) > 2)
					    dbc.decimales = 5;
					else
						dbc.decimales = 0;
					dbc.tipo = 'N';					
					dbc.len = (unsigned char)m_def->Campo(campo,TRUE)->c_maxqc;
					memset(tmp2,' ',dbc.len);
					memcpy(tmp2,str,str.GetLength());					
					break;		
				case 5:
					/* fecha */			
					dbc.tipo = 'D';
					dbc.decimales = 0; 
					dbc.len = 8; /* siempre 8 !! */
					
					memcpy(tmp2,((const char *)str)+6,4);
					memcpy(tmp2+4,((const char *)str)+3,2);
					memcpy(tmp2+6,((const char *)str),2);
					tmp2[8] = 0;
					break;		
				default:
					/* alfa */
					dbc.tipo = 'C';
					dbc.flags = 0x04;
					dbc.decimales = 0;			
					dbc.len = (unsigned char)m_def->Campo(campo,TRUE)->c_qc;
					memset(tmp2,' ',dbc.len);
					memcpy(tmp2,str,str.GetLength());
					break;
				}
				write(file,tmp2,dbc.len);				
			}
        }
	    catch (...)
	    {
            //ASSERT(FALSE);
	    }
    }

    lseek(file,0L,SEEK_SET);
    write(file,&(dbf.cabeza),sizeof(dbf.cabeza));
    close(file);
	dlgp.DestroyWindow();
	QuitaWait();


	return TRUE;
}

unsigned long CTablaView::DameIsrecCorriente()
{	
	char *px;
	if (m_cursel < m_registros)
	{		
		px = NULL;
		if (m_cache.Lookup( m_cursel , px ) && px)
		{
			unsigned long rec = 0L;
			memcpy((void *)&rec,(void *)(px+m_lenreg),sizeof(long));				
			return rec;
		}		
	}
	return 0L;
}
/*
void CTablaView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    TEXTMETRIC tm;
    HDC hDC = ::GetDC(NULL);
    CFont* pFont = GetFont();
    HFONT hFontOld = (HFONT)SelectObject(hDC, pFont->GetSafeHandle());
    GetTextMetrics(hDC, &tm);
    lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 1;
    SelectObject(hDC, hFontOld);
    ::ReleaseDC(NULL, hDC);
}
*/
