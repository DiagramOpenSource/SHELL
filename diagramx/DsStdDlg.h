#if !defined(AFX_DSSTDDLG_H__94901DF6_9746_11D3_B29F_004F4905CBCF__INCLUDED_)
#define AFX_DSSTDDLG_H__94901DF6_9746_11D3_B29F_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsStdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsStdDlg window

class CDsStdDlg;
class CDsTabCtrl;
class CDsStdCtrl;

class CDsOCX : public COleDispatchDriver
{
public:
	CDsOCX() {m_punk = NULL;m_pdisp = NULL;}
	CDsOCX(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDsOCX(const CDsOCX& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}
    CDsOCX(LPCSTR NombreOCX);
    ~CDsOCX();
    BOOL Invoca(LPCSTR funcion,HWND hdc,long x,long y,long dx,long dy,void *parametros = NULL);    

    CLSID m_clsid;
    LPUNKNOWN m_punk; 
    LPDISPATCH m_pdisp;
    ITypeInfo FAR* m_TypeInfo;

};

class CDsPushV
{
public:
    CDsPushV(int pi,int pf);
    ~CDsPushV();
    
    int InsertaDlg(CDsStdDlg *pdlg);
    int InsertaCtrl(CDsStdCtrl *pctrl);
    BOOL EsInsertableCtrl(CDsStdCtrl *pctrl);
    int Anulacion();
    int ListaSids(int *lista);
    CStdDlgLst m_push_dlglist;
    CStdControlLst m_push_ctrllst;
    int m_pi;
    int m_pf;
    BOOL m_locked;
	HWND m_focus;
};

class CDsStdCtrl : public CObject
{
public:	
	CWnd *SetFocus();
	void RellenaToolTip(char *pp);
	BOOL EnsureVisible();
	BOOL TeclaStd(int tecla);
	CRect MarcoControl(BOOL estab = FALSE);
	BOOL DisparaEventoUsuario(int operacion = 1,LPCSTR contenido = NULL);
	int ControlEdicion(int flag,void *datos,int pre_id = -1);
	CWnd * Create(int tipo,int lpos,int lpf,void *dato,CDsStdDlg *pDlg,int Atrib = (int)' ', int Cc = -1, int Cf = -1, int pre_id = -1);
    CDsStdCtrl();
    ~CDsStdCtrl();
    CWnd *m_ctrl;
	BOOL m_previovisible;
	int m_nivel;
	int m_nivel9;
    int m_tipo;
    int m_ctrlid;
    int m_lpi;
    int m_lpf;
    CDsStdDlg *m_pdlg;
    Cid m_cid;
    int m_p_id;
    int m_evento;
    HWND m_cthwnd; //
    CString m_tooltiptext;
    int m_antcid_ctrlid;
    int m_alto;
    int m_ancho;
    int m_stdx;
    int m_stdy;
    int m_offx;
    int m_offy;
    BOOL m_esw;
	int m_esde9;
	BOOL m_muriendo;
	BOOL m_protegido;	

	int m_helpflag;

	int m_usado;

	CDsStdCtrl *m_ctrlbefore;
	CDsStdCtrl *m_ctrlafter;	

	//CToolTipCtrl m_tooltip;

	int m_nivelfoco;

	static CDsStdCtrl *m_ctrlfirst;
	static CDsStdCtrl *m_ctrllast;
private:
	CWnd * real_create(int tipo, int lpos, int lpf, void *dato,CDsStdDlg *pDlg,int Atrib = (int)' ', int Cc = -1, int Cf = -1, int pre_id = -1);
};

/*
class CCid : public CObject
{
public:
    CCid() {m_cid.m_lcid = -1;m_cid.m_lref = -1;m_cid.m_tipo = -1;}
    CCid(CCid &p2) {m_cid.m_lcid = p2.m_cid.m_lcid;m_cid.m_lref = p2.m_cid.m_lref;m_cid.m_tipo = p2.m_cid.m_tipo;}
    CCid(Cid cid) {m_cid.m_lcid = cid.m_lcid;m_cid.m_lref = cid.m_lref;m_cid.m_tipo = cid.m_tipo;}    
    Cid m_cid;
    BOOL operator==(CCid &p2)
    {
        if (this->m_cid.m_lcid != p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lref != p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_tipo != p2.m_cid.m_tipo)
            return FALSE;
        return TRUE;
    }
    BOOL operator==(const CCid &p2)
    {
        if (this->m_cid.m_lcid != p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lref != p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_tipo != p2.m_cid.m_tipo)
            return FALSE;
        return TRUE;
    }
    BOOL operator==(CCid *p2)
    {
        if (this->m_cid.m_lcid != p2->m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lref != p2->m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_tipo != p2->m_cid.m_tipo)
            return FALSE;
        return TRUE;
    }
    BOOL operator>(CCid &p2)
    {
        if (this->m_cid.m_lcid < p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lcid > p2.m_cid.m_lcid)
            return TRUE;
        if (this->m_cid.m_lref < p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_lref > p2.m_cid.m_lref)
            return TRUE;
        if (this->m_cid.m_tipo < p2.m_cid.m_tipo)
            return FALSE;
        if (this->m_cid.m_tipo > p2.m_cid.m_tipo)
            return TRUE;
        return FALSE;
    }
    BOOL operator>=(CCid &p2)
    {
        if (this->m_cid.m_lcid < p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lcid > p2.m_cid.m_lcid)
            return TRUE;
        if (this->m_cid.m_lref < p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_lref > p2.m_cid.m_lref)
            return TRUE;
        if (this->m_cid.m_tipo < p2.m_cid.m_tipo)
            return FALSE;
        if (this->m_cid.m_tipo > p2.m_cid.m_tipo)
            return TRUE;
        return TRUE;
    }
    BOOL operator<(CCid &p2)
    {
        if (this->m_cid.m_lcid < p2.m_cid.m_lcid)
            return TRUE;
        if (this->m_cid.m_lcid > p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lref < p2.m_cid.m_lref)
            return TRUE;
        if (this->m_cid.m_lref > p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_tipo < p2.m_cid.m_tipo)
            return TRUE;
        if (this->m_cid.m_tipo > p2.m_cid.m_tipo)
            return FALSE;
        return FALSE;
    }
    BOOL operator<=(CCid &p2)
    {
        if (this->m_cid.m_lcid < p2.m_cid.m_lcid)
            return TRUE;
        if (this->m_cid.m_lcid > p2.m_cid.m_lcid)
            return FALSE;
        if (this->m_cid.m_lref < p2.m_cid.m_lref)
            return TRUE;
        if (this->m_cid.m_lref > p2.m_cid.m_lref)
            return FALSE;
        if (this->m_cid.m_tipo < p2.m_cid.m_tipo)
            return TRUE;
        if (this->m_cid.m_tipo > p2.m_cid.m_tipo)
            return FALSE;
        return TRUE;
    }
    CCid& operator=(CCid &p2)
    {
        this->m_cid.m_lcid = p2.m_cid.m_lcid;
        this->m_cid.m_lref = p2.m_cid.m_lref;
        this->m_cid.m_tipo = p2.m_cid.m_tipo;
        return *this;
    }
};
*/


template <
	class CDSWRBBase = CExtWS //CExtADLG // CWnd
	>
class CDSWRB : public CDSWRBBase
{
protected:
	virtual LRESULT WindowProc( 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		//DWORD style;
		if (((CDsStdDlg *)this)->m_modoespecial)
		{
			return CDSWRBBase::WindowProc( message, wParam, lParam );
		}		
		if ((CWnd *)theApp.m_iStdView != GetParent())
		{
			return CDSWRBBase::WindowProc( message, wParam, lParam );
		}		
		/*
		style = GetStyle();
		if ((style & WS_DLGFRAME) || !(style & WS_BORDER))
			return CDSWRBBase::WindowProc( message, wParam, lParam );
		*/

		switch( message )
		{
		case WM_NCCALCSIZE:
			{
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );
				
				CRect rcInBarWnd( pNCCSP->rgrc[0] );
				rcInBarWnd.DeflateRect(
					1, 1, 1, 1
					);
				::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
				
				return 0;
			} // case WM_NCCALCSIZE
		case WM_NCPAINT:
			{
				CRect rcInBarWnd, rcInBarClient;
				GetWindowRect( &rcInBarWnd );
				GetClientRect( &rcInBarClient );
				ClientToScreen( &rcInBarClient );
				if( rcInBarWnd == rcInBarClient )
					return 0;
				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect( ptDevOffset );
				rcInBarClient.OffsetRect( ptDevOffset );

				CWindowDC dc( this );
				ASSERT( dc.GetSafeHdc() != NULL );
				dc.ExcludeClipRect( &rcInBarClient );
				
				if (theApp.m_colorset8)
				{
					dc.Draw3dRect(
						&rcInBarWnd,
						theApp.m_color8,
						theApp.m_color8
						);
				}
				else				
				{
					g_PaintManager->PaintResizableBarChildNcAreaRect(
						dc,
						rcInBarWnd,
						this
						);
				}
				return 0;
			} // case WM_NCPAINT
		} // switch( message )
		return CDSWRBBase::WindowProc( message, wParam, lParam );
	}
}; // class CExtWRB

/*typedef CMap < UINT , UINT , CDsStdCtrl*, CDsStdCtrl* > CStdCtrlCidLst;*/
/* por eficiencia se deberia crear un Map propio para tratar con Cid pero de momento apaña */
typedef CMapStringToPtr  CStdCtrlCidLst;

typedef CMap< HWND ,HWND&, CDsStdCtrl*, CDsStdCtrl* > CStdCtrlHwnd;

typedef CMap< int ,int&, void * , void *> CFreeHandles;

#ifdef DSPROFUIS
#define STDDLGBASE CWnd
#else
#define STDDLGBASE CWnd
#endif

#ifdef DSPROFUIS
class CDsStdDlg : public CDSWRB < STDDLGBASE  > // CExtWA // public CDSWRB < STDDLGBASE  > // 
#else
class CDsStdDlg : public STDDLGBASE
#endif
{
// Construction
public:
	CDsStdDlg();
	DECLARE_DYNCREATE(CDsStdDlg)
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsStdDlg)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_modoespecial;

	char *RecogeAyudaPorPosicion();
	int ControlDialogo(int modo,char *datos);
	BOOL SacameDelPushv(CDsStdDlg *pdlg);
	BOOL CambiaPane(int cambio,BOOL sube = FALSE);
	BOOL EnsureVisible(CRect *r);
	int InsertaPushv(CDsStdDlg *pdlg);
    int InsertaPushv(CDsStdCtrl *pctrl);
    BOOL EstaEnPushv(CDsStdCtrl *pctrl);
    char m_cidkeybuf[256];
	LPCSTR CidKey(Cid cid);

	BOOL EraseBkgnd(CDC* pDC);
	

	void AjustaSegunEstilo(CRect &rect,DWORD stylo);
	BOOL TeclaStd(int tecla,int wcid,int ctipo,Cid cid);

	static int ComparaLPos(int pi,int pf,BOOL dentro);
    static BOOL ComparaCid(Cid &c1,Cid &c2,BOOL Absoluto = TRUE);

	int OpcionesVentana(char *data);
	BOOL ScrollCtrls(int pi, int pf, int n);
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	BOOL AjustaScroll();
	int ControlUsuario(char *nom,int pi,int pf,void *dato,int pre_id = -1);
	BOOL RefrescaControl(CDsStdCtrl *pctrl);
	int BorraDescendencia(int *lista,int pi = -1, int pf = -1);
	BOOL ReajustaRect();
	int CompruebaPushV(CDsStdDlg *pdlg);
    int CompruebaPushV(CDsStdCtrl *pctrl);
	int PopV(int nulo,int *dest);
	int PushV(int pi,int pf);    
    CDsPushV *m_pushv[50];
    int m_npush;

	BOOL m_noscroll;
	BOOL m_noframe;
	BOOL m_forceframe;

    CStdDlgLst m_listahijos;

	BOOL DestruyeControl(int cwid,BOOL Siempre = FALSE);
	int BuscaControlPorCid(int tipo,Cid cid,CDsStdCtrl **rctrl = NULL);
	int ControlEdicion(int cwid,Cid cid,int flag,void *datos,int pre_id = -1);
	BOOL Crece(BOOL solorecalcula = FALSE);
	CWnd * CreaTab(int sid,int lpi,int lpf);
	int InsertaControl(int tipo,int lpos,int lpf,void *dato,int Atrib = (int)' ', int Cc = -1, int Cf = -1,CDsStdCtrl **pdctrl = NULL,int pre_id = -1);
	BOOL DisparaEventoUsuario(int operacion = 1,LPCSTR contenido = NULL);
	virtual BOOL Create(void *datos,CDsxView *pView, int& stdid);
	virtual ~CDsStdDlg();

    static int NewHandle();    

    static int DeterminaTecla(UINT nChar);

    static void PintaRayas(CPaintDC &dc,char *maparaya,COLORREF cbk,int alto,int ancho,int stdx,int stdy, int offx,int offy,CRect &ult_hijo,CPoint &orig,CBrush *bfondo,int msize);

    int m_id;
    int m_sid;

    int m_lpi;
    int m_lpf;
    int m_ancho;
    int m_alto;

    int m_ult_sid;

    int m_sid_padre;
    CDsStdDlg  *m_padre;

	CDsStdDlg *m_referencia;

    CDsTabCtrl *m_padretab;
    int m_opcionpadre;

    int m_wid;

    int m_stdx;
    int m_stdy;

    int m_offx;
    int m_offy;

    int m_nivel;    

    char *m_maparaya;
	int  m_maparaya_size;

	int m_evento;

	BOOL m_es9;

    BOOL m_falso;

    CDsxView *m_pView;
    CDsxDoc  *m_pDoc;

    CString m_nombre;

    CStdControlLst m_ctrlst;

    CStdCtrlCidLst m_cidctrllst; //se incluyen los que han de ser unicos por stddlg    

    CDsStdDlg  *m_pater;
    
    CRect m_ult_hijo;
    CRect m_ult_hijo_mosaico;
    BOOL  m_ult_vertical;
    BOOL  m_ult_grow;
    BOOL  m_ult_first;


    CRect m_padre_ult_hijo;
    BOOL  m_padre_ult_vertical;
    BOOL  m_padre_ult_grow;
    BOOL  m_padre_ult_first;    

    CRect m_creation_rect;
    CRect m_stdrect;
    CRect m_inistdrect;

    int   m_replicante;

	int m_IdAntFoco;

    COLORREF m_cLabelBk;
    COLORREF m_cLabelFr;
	CBrush   *m_bLabelBk;

	BOOL m_parchear;

    static int DsStdX;
    static int DsStdY;
    static int DsStdOffX;
    static int DsStdOffY;

    static int DsMnStdX;
    static int DsMnStdY;
    static int DsMnStdOffX;
    static int DsMnStdOffY;

    static int m_contaid;
    static CFreeHandles   m_freehandles;
    static CStdControlLst m_todosctrl;
    static CStdCtrlHwnd   m_hwndctrllst;

	// Generated message map functions
protected:
    afx_msg BOOL DsStdDlgToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	//{{AFX_MSG(CDsStdDlg)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern int pmask;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSSTDDLG_H__94901DF6_9746_11D3_B29F_004F4905CBCF__INCLUDED_)
