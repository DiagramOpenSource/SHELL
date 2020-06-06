#pragma once


#define MAXDSTREEDATA 1000
// CDsTreeCtrl

class CDsStdCtrl;


struct cachenivel {
	CArray< struct cachenivel *, struct cachenivel *> m_cache;
	HTREEITEM item;
};

class CDsTreeCtrl : public CTreeCtrl // CExtWRB < CTreeCtrl >
{
	DECLARE_DYNAMIC(CDsTreeCtrl)

public:
	CDsTreeCtrl();
	virtual ~CDsTreeCtrl();

	HTREEITEM LocalizaNivel(HTREEITEM primero,int item[],int nivel);	
	HTREEITEM BuscaItemData(HTREEITEM primero,LPCSTR data,BOOL contenido = FALSE);
	HTREEITEM DeterminaItem(const char *data,HTREEITEM &hParent,CString &nombre,CString &opcion);
	CString CreaRutaItem(HTREEITEM hitem);
	void DeleteCache(CArray< struct cachenivel *, struct cachenivel *> *pcache);
	

	void Inicializa();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	CImageList m_ImageList;

	BOOL        m_group;
	BOOL        m_acelerado;
	CDsStdCtrl *m_pstdctrl;
	int         m_ndata;
	CArray< struct cachenivel *, struct cachenivel *> m_cache;
	CArray< CString *, CString *> m_data;
	//CString    *m_data[MAXDSTREEDATA];
    
protected:
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


