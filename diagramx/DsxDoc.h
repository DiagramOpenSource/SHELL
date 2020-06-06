// DsxDoc.h : interface of the CDsxDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSXDOC_H__2B025534_0754_4B3C_AB69_91F4F9B2DFC3__INCLUDED_)
#define AFX_DSXDOC_H__2B025534_0754_4B3C_AB69_91F4F9B2DFC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDsxSrvrItem;
class CDsxView;

class CDsxDoc : public COleServerDoc
{
protected: // create from serialization only
	CDsxDoc();
	DECLARE_DYNCREATE(CDsxDoc)

// Attributes
public:
	CDsxSrvrItem* GetEmbeddedItem()
		{ return (CDsxSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxDoc)
	public:
   virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
   virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   virtual void OnCloseDocument();
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual COleIPFrameWnd* CreateInPlaceFrame(CWnd* pParentWnd);
	virtual void DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd);
	//}}AFX_VIRTUAL

// Implementation
private:   
	void EraseFonts();
   int m_EnWinsysMenu;
   HTREEITEM m_DefItem;
public:
	BOOL CompruebaTareaTeclaFuncion(int tecla);
	CDsxView *m_curView;
	BOOL PideFuente(LOGFONT *pfont,BOOL Fixed = FALSE,BOOL update = TRUE);
	BOOL PideFuente9(UINT nID);
	int ControlDialogo(int modo,char *datos);
	void RegistraFonts();
	void UpdateFonts(BOOL registra = TRUE);
	BOOL FinErrorFichero();
	int ErrorFichero(char *data1, char *data2, char *data3, int ok);
	int ControlDeUsuario(int id,int sid,char *nom,int pi,int pf,void *dato,int prev_id = -1);
	int CreaGrid(void *datacontrol);
	int ControlBinario(int operacion,void *datacontrol);
	int  ControlEdicion(int wid,int id,int sid,int cwid,Cid cid,int flag,void *datos,int pre_id = -1);
	int  InsertaControl(int wid,int id,int sid,int tipo,int lpos,int lpf,void *datos,int Atrib = (int)' ', int Cc = -1, int Cf = -1,int prev_id = -1);
	void ControlDePantallas(int operacion,void *datos);
	void NuevoDoc();
	void PonFoco();
    BOOL TeclaStd(int tecla,int wid = -1,int id = -1,int sid = -1,int wcid = -1,int ctipo = -1,Cid cid = NULO_CID);
   void RellenaMenuStd(MenuStruct *mi_menu,int defid);
   void PonEnWinsysMenu(int status);
   int  StatusEnWinsysMenu(void);
   void Pon_Tree_Items(CTreeCtrl *tree,HTREEITEM Parent,MenuStruct *menu,int defid);
   void IluminaTreeOpcion(CTreeCtrl *tree = NULL);
   void RefrescaIra(void);
   void OpcionDeMenu(int punto);

   void InicializaDoc();
   static void LeeProfileFont(LOGFONT *logfont,LPCSTR nombre);
   static void GrabaProfileFont(LOGFONT *logfont,LPCSTR nombre);
   void DefaultFonts(BOOL update = TRUE);
   void DefaultFonts9(BOOL update = TRUE);

   LOGFONT   m_lfEdit;    // Fuente para edicion
   CFont    *m_cfEdit;     
   COLORREF  m_cEditBk;   // colores de cajas de edicion
   COLORREF  m_cEditFr;
   CBrush   *m_bEditBk;   

   COLORREF  m_cEditINBk; // colores cajas de edicion con el foco
   COLORREF  m_cEditINFr;
   CBrush   *m_bEditINBk;  

   LOGFONT   m_lfLabel;   //  font para labels normales (determina tamaño dialogo)
   CFont    *m_cfLabel;     
   COLORREF  m_cLabelBk;  //  colores labels
   COLORREF  m_cLabelFr;
   CBrush   *m_bLabelBk;
   
   LOGFONT   m_lfLabelI;  //  font para labels 'I'
   CFont    *m_cfLabelI;     
   COLORREF  m_cLabelIBk; //  colores labels 'I'
   COLORREF  m_cLabelIFr;
   CBrush   *m_bLabelIBk;

   LOGFONT   m_lfLabelR;  //  font para labels 'R'
   CFont    *m_cfLabelR;
   COLORREF  m_cLabelRBk; //  colores labels 'R'
   COLORREF  m_cLabelRFr;
   CBrush   *m_bLabelRBk;

   LOGFONT   m_lfGrid;       //  font para grids
   CFont    *m_cfGrid;
   COLORREF  m_cGridBk;      //  colores grid normales
   COLORREF  m_cGridFr;      //  colores grid
   CBrush   *m_bGridBk;
   COLORREF  m_cGridSelBk;   //  colores grid linea seleccionada
   COLORREF  m_cGridSelFr;   //  colores grid
   CBrush   *m_bGridSelBk;
   COLORREF  m_cGridCamBk;   //  colores grid campo seleccionado
   COLORREF  m_cGridCamFr;   //  colores grid
   CBrush   *m_bGridCamBk;

   LOGFONT   m_lfTitulos;
   CFont    *m_cfTitulos;

   //LOGFONT   m_lffontboton;

   CFont    *m_cf9Campo;
   LOGFONT m_font9texto;
   LOGFONT m_font9inten;
   LOGFONT m_font9campo;
   LOGFONT m_font9rever;
   LOGFONT m_font9mensa;
   LOGFONT m_font9boton;
   LOGFONT m_font9grid;
   LOGFONT m_font9edita;
   COLORREF  m_9cfondillo;
   COLORREF  m_9ctextillo;
   COLORREF  m_9cfondo_seleccion;
   COLORREF  m_9ctexto_seleccion;
   COLORREF  m_9cfondo_campo_seleccion;
   COLORREF  m_9c_custom_subrayado;

   int es_directa;
	virtual ~CDsxDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   CTreeCtrl * m_menustd;
   CTreeCtrl * m_menuser;

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CDsxDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
   afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
   afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
   afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
   afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
   afx_msg void OnUpdateFileSendMail(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDsxDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXDOC_H__2B025534_0754_4B3C_AB69_91F4F9B2DFC3__INCLUDED_)
