// DlgExplorer.h: interface for the CDlgExplorer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGEXPLORER_H__F2BF5AAC_E750_45E1_8B8D_A3283E4DA95E__INCLUDED_)
#define AFX_DLGEXPLORER_H__F2BF5AAC_E750_45E1_8B8D_A3283E4DA95E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgExplorer;

class CListFile : public wxListCtrl  
{
public:
	CListFile();
	int Rellena();
	int PonDirectorio(WIN32_FIND_DATA *dir);
	virtual ~CListFile();
	CDlgExplorer *m_explorer;
	void OnKeyDown(wxKeyEvent& event);
	void OnLDClick(wxMouseEvent& event);
	void OnRDClick(wxMouseEvent& event);
	void OnMenu(wxCommandEvent &event);
	void OnBeginEdit(wxListEvent &event);
	void OnEndEdit(wxListEvent &event);
	int CopiaFichero(int item,BOOL mover);
	int BorraFichero(int item);
	int CreaDirectorio(int item);
	void OnColClick(wxListEvent& event);
	int DamePuntoInsercion(wxString &nombre,int ini,int fin);
	int DamePuntoInsercionCol(wxString &nombre,int ini,int fin,int col);	
	BOOL m_remoto;
	BOOL m_local;	
private:
    DECLARE_EVENT_TABLE()
};

class CComboDir : public wxComboBox
{
public:
	CComboDir();
	virtual ~CComboDir();
	void OnKeyDown(wxKeyEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

class CDlgPanelE : public wxWindow  
{
public:
    CDlgPanelE(  CDlgExplorer* dlgexplorer,wxWindow* parent,CListFile *lista,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxTRANSPARENT_WINDOW,const wxString& name = "ventana");
	virtual ~CDlgPanelE();

	void OnSize(wxSizeEvent& event);
	void OnDrive(wxCommandEvent &event);
	void OnDir(wxCommandEvent &event);
    void OnText(wxCommandEvent &event);	
	void OnTimer(wxTimerEvent& event);
	void EnviaDir();	
	void EnviaFile(wxString &nom,long size);	

	wxBoxSizer *m_sizer; 
	wxComboBox m_drives;
	CComboDir m_dir;
	wxTimer m_timer;

	CDlgExplorer *m_explorer;

	BOOL m_remoto;
	BOOL m_local;

    unsigned long m_cambiodir;
	unsigned long m_puntotimer;
private:
    DECLARE_EVENT_TABLE()
};

#include "wx/progdlg.h"

#define MAXPEDIDOS 100
#define MAXPORSERVER 5

class CDlgExplorer;

class CEnvioHandle : public wxProgressDialog
{
public:
	CEnvioHandle(
		BOOL recibe,
		int size,
		const wxString &origen,
		const wxString &destino,
		CDlgExplorer *explorer,
		int idx,
        const wxString& title, 
		const wxString& message,  
		int maximum = 100,
		wxWindow * parent = NULL,
		int style = wxPD_AUTO_HIDE | wxPD_APP_MODAL
		);
	~CEnvioHandle();
	int PideOpen();
	int Open(int fd);
	int Recibe(int *p2);
	int Envia(int *p2);
	int Acabado();
	int Timeouts();
	BOOL m_recibe;
	wxString m_origen;
	wxString m_destino;
	int m_handleo;
	int m_handled;
	long m_size;
	long m_pedidohasta;	
	long m_recibidototal;
	long m_time;	
	int m_npedidos;
	int m_status;
	int m_pedidos[MAXPEDIDOS];
	long m_timeout[MAXPEDIDOS];
	CDlgExplorer *m_explorer;
	int m_idx;
private:
    DECLARE_EVENT_TABLE()
};

#define MAXENVIOS 100

class CDlgExplorer : public wxDialog  
{
	/*DECLARE_DYNAMIC_CLASS(CDlgExplorer)*/
public:
    CDlgExplorer(  CDlgPresencia* dlgpresencia,CDlgMensaje * dlgmensaje,wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox",BOOL oculto = FALSE,BOOL recibe = FALSE);
	virtual ~CDlgExplorer();

	wxBoxSizer *m_sizer; 
	wxSplitterWindow *m_split;

	CDlgPresencia* m_dlgpresencia;
	CDlgMensaje  * m_dlgmensaje;

	CDlgPanelE *m_panel1;
	CDlgPanelE *m_panel2;

	CListFile  m_list1;
	CListFile  m_list2;

	CEnvioHandle *m_envio[MAXENVIOS];

	int Recibe(Paquete *ppaquete);
	int ProcesaRecibe(unsigned short *p1);
	int EnviaCDATA(unsigned short comando,char *buffer,int len = -1);
	int EnviaDato(unsigned short comando,char *buffer,int len = -1);
	int ExploraDir(char *dir);
	int EnviaFile(wxString &nom,long size);
	int EnviaFile(wxString &nom);
	int RecibeFile(wxString &nom,long size);	
	void OnTimer(wxTimerEvent& event);
	void CompruebaSocket();

	SOCKET m_directo;

	int SocketSelect(WPARAM wParam,LPARAM lParam);
	int LocalizaEnvio(int modo,int *p1,int size);

	wxTimer m_timer;

	Paquete m_recvbuf;
	int m_punrec;
	int m_asincrono;
	int m_esperaopen;
	int m_version;

	void OnCloseWindow(wxCloseEvent& event);

	BOOL m_oculto;
	BOOL m_recibe;	

private:
    DECLARE_EVENT_TABLE()	
};

#endif // !defined(AFX_DLGEXPLORER_H__F2BF5AAC_E750_45E1_8B8D_A3283E4DA95E__INCLUDED_)
