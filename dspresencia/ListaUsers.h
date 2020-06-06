// ListaUsers.h: interface for the CListaUsers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTAUSERS_H__FCE49B5D_D318_408A_9200_9AC261647ACD__INCLUDED_)
#define AFX_LISTAUSERS_H__FCE49B5D_D318_408A_9200_9AC261647ACD__INCLUDED_

#include <wx/listctrl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgPresencia;

class CListaUsers : public wxListCtrl  
{
public:	
	int EstadoPorIp(long ip,int estado,int flag);
	int PorNombre(const char *Nombre,long &ip);
	int BorraIP(long vieja);
	int CambiaIP(long vieja,long nueva);
	int DameImagen(long ip,int estado,int flag,int tipo);
	int PorIp(long ip,wxString &nombre);
	void OnRDClick(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent &event);
	void OnDropFiles(wxDropFilesEvent& event);
	bool CreaItem(wxString &Nombre,long ip,int estado,int flag,int tipo,int departamento = 0,unsigned long userflag = 0L);
	bool PonEstado(char **_p,int *elprivilegio,wxString *pdestado = NULL,int *prevestado = NULL,unsigned long userflag = 0L);
	CListaUsers();
	virtual ~CListaUsers();    
	void OrdenaItems();	
	void OnSelect(wxListEvent& event);
	void OnActivate(wxListEvent& event);
	void OnRDrag(wxListEvent& event);
	void OnLDrag(wxListEvent& event);

	int m_estados[500];
    int m_flags[500];
	int m_tipos[500];
	int m_departamento[500];

	unsigned long m_userflags[500];

	CDlgPresencia *m_padre;

private:
    DECLARE_EVENT_TABLE()
};

enum
{
    LIST_QUIT                   = 1,
    LIST_LIST_VIEW              = 2,
    LIST_ICON_VIEW              = 3,
    LIST_ICON_TEXT_VIEW         = 4,
    LIST_SMALL_ICON_VIEW        = 5,
    LIST_SMALL_ICON_TEXT_VIEW   = 6,
    LIST_REPORT_VIEW            = 7,
    LIST_DESELECT_ALL           = 8,
    LIST_SELECT_ALL             = 9,
    LIST_ABOUT                  = 102,
    BUSY_ON                     = 10,
    BUSY_OFF                    = 11,
    LIST_DELETE_ALL             = 12,
    LIST_SORT,
    LIST_SET_FG_COL,
    LIST_SET_BG_COL,
    LIST_TOGGLE_MULTI_SEL,
    LIST_TOGGLE_FIRST,
    LIST_SHOW_COL_INFO,

    LIST_CTRL                   = 1000
};


#endif // !defined(AFX_LISTAUSERS_H__FCE49B5D_D318_408A_9200_9AC261647ACD__INCLUDED_)
