// DlgPresencia.h: interface for the CDlgPresencia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGPRESENCIA_H__91CC980A_5919_4B14_94A3_E590BE071A9E__INCLUDED_)
#define AFX_DLGPRESENCIA_H__91CC980A_5919_4B14_94A3_E590BE071A9E__INCLUDED_

/*#define WINVER 0x5000*/

#include <commctrl.h>

#include <wx/dialog.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/timer.h>   
#include <wx/colordlg.h>
#include <wx/wxhtml.h>
#include <wx/protocol/http.h>
#include <wx/html/htmlwin.h>
#include <wx/html/winpars.h>
#include <wx/valgen.h>



extern "C"
{
#include <rdef.h>
}

#define _charseparador 10
#include "ListDef.h"

#define MILISTIMER      50 
#define FACTORTIMER     (100/MILISTIMER)

#define MAXTIEMPOQUIETO (3000*FACTORTIMER)
#define MINTIEMPOQUIETO (300*FACTORTIMER)
#define TIEMPOTIMEOUT   (300*FACTORTIMER)

#define TIEMPOREFRESCO  (5*FACTORTIMER)

/* flags de complementacion del estado */
/* 0FFF para estados de usuario */
#define FLAG_LLAMADA   0x001
#define FLAG_PETICION  0x004
#define FLAG_CONFAMO   0x008
#define FLAG_ENCONF    0x010

extern int elflag;

#define NO_LLAMADA(a) {elflag &= ~FLAG_LLAMADA;a = (elflag|FLAG_PETICION);}
#define LLAMADA(a) {elflag &= ~FLAG_PETICION;a = (elflag|FLAG_LLAMADA);}

#define NO_CONFAMO(a) {elflag &= ~FLAG_CONFAMO;a = (elflag|FLAG_PETICION);}
#define CONFAMO(a) {elflag &= ~FLAG_PETICION;a = (elflag|FLAG_CONFAMO);}

#define NO_ENCONF(a) {elflag &= ~FLAG_ENCONF;a = (elflag|FLAG_PETICION);}
#define ENCONF(a) {elflag &= ~FLAG_PETICION;a = (elflag|FLAG_ENCONF);}


/* F000 para estados de control interno */
#define FLAG_OCULTO    0x8000 

/* estos flags son indicadores de usuario a usuario */
#define UFLAG_RING      0x1
#define UFLAG_OKVOX     0x2
#define UFLAG_FINVOX    0x4
#define UFLAG_CONVOX    0x8
#define UFLAG_CONTROL  0x10
#define UFLAG_FICHEROS 0x20
#define UFLAG_NOFICHEROS 0x40
#define UFLAG_SIFICHEROS 0x80

/* tipos internos dspreser */
#define TIPO_ACTIVO       0x000001
#define TIPO_NULO         0x000002
#define TIPO_PIDECLOAK    0x000400
#define TIPO_PIDEDECLOAK  0x000800

#define TIPO_OCULTO       0x001000

#define TIPO_INACTIVO        0x100000
#define TIPO_OBSOLETO        0x200000
#define TIPO_OBSOLETOAVISADO 0x400000


enum ElEstado {Presente,Ausente,Inactivo,Desconectado,Ocioso,Vacaciones,Enfermo,Otros};

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListaUsers.h"
#include "TControl.h"
#include "Arbol.h"

class CTexto : public wxTextCtrl
{
public:
	void OnMouseEvent(wxMouseEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

class CEnvia : public wxTextCtrl
{
public:
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
private:
    DECLARE_EVENT_TABLE()		
};


class CDlgLista : public wxDialog  
{
public:
	wxString SacaCampo(wxString idcampo,int item);
	CDlgLista (FI_DEF *fi,char *datos,long ldatos,wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,BOOL comprimido = TRUE,const wxString& name = "dialogBox");
	~CDlgLista();
	BOOL m_modeless;
	int m_ninfo;
	wxBoxSizer *m_sizer;
	wxBoxSizer *m_sizerBottom;
	CListDef m_list;
	wxButton m_busca;
	wxTextCtrl m_texto;
	wxButton m_ok;
	wxButton m_cancel;	
	wxString m_codcli;
	wxString m_nomcli;
	wxString m_fecha;
	wxString m_ticket;
	wxString m_subticket;	
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);	
	void OnBusca(wxCommandEvent& event);
	void PonSeleccion();
private:
    DECLARE_EVENT_TABLE()
};


class CDlgParte : public wxDialog  
{
public:
	CDlgParte(FI_DEF *fi,CDlgPresencia* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");
	~CDlgParte();
	wxBoxSizer *m_sizer;	
	wxBoxSizer *m_sizerCli;	
	wxBoxSizer *m_sizerB;	
	wxBoxSizer *m_sizerTop;	
	wxBoxSizer *m_sizerBottom;	
	wxButton m_cliente;
	wxButton m_pedido;
	wxButton m_ok;
	wxButton m_cancel;
	wxTextCtrl m_codigo;
	wxTextCtrl m_texto;
	wxTextCtrl m_fecha;
	wxTextCtrl m_nombre;
	wxString m_serie;
	wxString m_codped;
	wxString m_codcli;

	BOOL m_espedido;
	BOOL m_escliente;
	void OnButton(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};


class CDlgReloj : public wxDialog  
{
public:
	CDlgReloj(CDlgPresencia* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");
	~CDlgReloj();
	wxBoxSizer *m_sizer;	
	wxBoxSizer *m_sizerTop;	
	wxBoxSizer *m_sizerMid;	
	wxBoxSizer *m_sizerBottom;			
	wxTextCtrl m_segundo;
	wxTextCtrl m_minuto;
};


class CDlgId : public wxDialog  
{
public:
	CDlgId(CDlgPresencia* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");
	~CDlgId();
	wxBoxSizer *m_sizer;	
	wxBoxSizer *m_sizerTop;	
	wxBoxSizer *m_sizerMid;	
	wxBoxSizer *m_sizerBottom;			
	wxTextCtrl m_codigo;
	wxTextCtrl m_codigob;
	wxTextCtrl m_usuario;
	wxTextCtrl m_password;
	wxButton m_ok;
	wxButton m_cancel;
	void OnButton(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};


class CDlgAlarma : public wxDialog  
{
public:
	CDlgAlarma(CDlgPresencia* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");
	~CDlgAlarma();
	wxBoxSizer *m_sizer;	
	wxBoxSizer *m_sizerTop;	
	wxBoxSizer *m_sizerMid;	
	wxBoxSizer *m_sizerBottom;	
	wxButton m_ok;
	wxButton m_cancel;
	wxTextCtrl m_hora;
	wxTextCtrl m_minuto;	
	wxTextCtrl m_texto;

	void OnButton(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

class CDlgExplorer;
class CDlgVolume;
class CDlgAtender;

class wxdsTextCtrl : public wxTextCtrl
{
public:
	void ShowLastPosition();
	void OnTodo(wxCommandEvent &event);
	void OnCopiar(wxCommandEvent &event);
	virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);	
private:
    DECLARE_EVENT_TABLE()
};

class CDlgMensaje : public wxDialog  
{
public:
	void CenterOnScreen();
	void RepintaTodo();
	virtual void OnSize(wxSizeEvent& event);
	virtual void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
    virtual void SetSize(const wxRect& rect);
    virtual void SetSize(int width, int height);
    virtual void SetSize(const wxSize& size);
	void OnDropFiles(wxDropFilesEvent& event);
	void EnviaMensaje();	
	CDlgMensaje(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox",BOOL conferencia = FALSE,BOOL general = FALSE);
	~CDlgMensaje();
	void OnCloseWindow(wxCloseEvent& event);
	void GuardaPosicion();
	CDlgPresencia *m_padre;
	wxBoxSizer *m_sizerTop;
	wxBoxSizer *m_sizerEnvia;
	wxBoxSizer *m_sizerBoton;
	wxBoxSizer *m_sizerImagen;
	wxButton m_enviar;
	wxButton m_llamar;
	wxButton m_ocultar;
	wxButton m_ardillas;
	wxButton m_minimizar;
	wxButton m_ocultacion;
	wxButton m_control;
	wxButton m_ficheros;
	wxStaticBitmap m_imagenestado;
	wxCheckBox m_aviso;
	wxCheckBox m_agrupo;
	wxCheckBox m_afavoritos;
	bool m_soyyo;	
	wxdsTextCtrl m_recibe;	
	CEnvia m_envia;
	int m_departamento;
	BOOL m_general;
	long m_ip;
	long m_iptlvoz;
	void Ocultar();	
	void Oculta(wxString &nombre,int &item,wxString &s,int &estado);
	void OnCancel(wxCommandEvent& event);
	void OnButton(wxCommandEvent& event);
	void OnActivate(wxActivateEvent& event);
	void OnChar(wxKeyEvent& event);
	BOOL m_poractivar;
	BOOL m_conferencia;
	long m_propietario;
	CListaUsers *m_users;
	BOOL m_envoz;
	CDlgExplorer *m_explorer;
	CDlgVolume *m_dlgvolume;
	CDlgAtender *m_dlgatender;
	void ActivaVoz();
	void DesactivaVoz();
	virtual bool Show(bool show);
	void SoyYo(bool soy);

	void EnviaFile(wxString file);	
private:
    DECLARE_EVENT_TABLE()
};


class CDlgTicket : public wxDialog  
{
public:
	void EnviaMensaje();
	CDlgTicket(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");	
	~CDlgTicket();
	wxBoxSizer *m_sizerTop;
	wxBoxSizer *m_sizerEnvia;
	wxBoxSizer *m_sizerBoton;
	wxBoxSizer *m_sizerCli;
	wxButton m_enviar;
	wxButton m_ocultar;
	wxButton m_cliente;
	wxString m_destino;
	wxdsTextCtrl m_recibe;	
	wxTextCtrl m_envia;
	wxTextCtrl m_codigo;
	wxTextCtrl m_nombre;
	long m_ip;
	void OnCancel(wxCommandEvent& event);
	void OnButton(wxCommandEvent& event);
	BOOL m_poractivar;
private:
    DECLARE_EVENT_TABLE()
};


class CDlgPresencia : public wxDialog  
{
public:			
	void ActualizaImagenMensaje(char *p,BOOL esip = FALSE);
	void PonSelIp(long ip);
	void PonFavorito(long ip);
	void QuitaFavorito(long ip);
	void PonEnConferencia(long ip,BOOL borrar = FALSE);
	void RepintaTodo();
	int ActivaTicket(long otra,wxString &Nombre);
	void LimpiaDatos();
	void RecibeChunk(char *p1);
	void PideChunk();
	void NuevosDatos(char *p1);
	void PonImagenEstado(int imagen);
	int EstadoImagen(int imagen,int estado,int flag,int tipo);
	int DameImagen(long ip,int estado,int flag,int tipo);
	void EsMensaje(char *p1);
	void EsConferencia(char *p1);	
	void CreaConferencia(char *p1);
	void QuitaConferencia(char *p1);
	void EnviaConferencia(int nips,long *ips);
	void QuitaDeConferencia(int nips,long *ips);
	void EsFoto(char *p1,int size);	
	void NoHayFoto(char *p1);
	int ActivaMensaje(long ip,BOOL ocultar = FALSE,BOOL foco = FALSE);
	void EnviaCorreo(const char *email = NULL);
	void ProcesaMenu(char *p1);
	void RellenaDepartamentos(char *p1);
	void PonColores(char *p1);
	void PonEstados(char *p,int *elprivilegio,int modo);
	void Actualiza();
	void Ardillas();
	void EnviaOtroEstado(int nest,wxString &ne);
	void PideInfo(const char *nombre,BOOL datos = FALSE,long ip = 0);
	void PideUser(const char *nombre);
	void PideRefresco(bool urgencia = TRUE);
	void SetPassword();
	void OnVacaciones(wxCommandEvent &event);
	void OnEnfermo(wxCommandEvent &event);
	void OnOtros(wxCommandEvent &event);
	void OnParte(wxCommandEvent &event);
	void OnWeb(wxCommandEvent &event);
	void OnFinParte(wxCommandEvent &event);		
	void OnAlarma(wxCommandEvent &event);
	void OnActiva(wxCommandEvent &event);
	void OnTareas(wxCommandEvent &event);	
	void OnSalida(wxCommandEvent &event);
	void OnAusente(wxCommandEvent &event);
	void OnPresente(wxCommandEvent& event);
	void OnIdentificar(wxCommandEvent& event);	
	void OnPassword(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnOcultar(wxCommandEvent& event);
	void OnDesocultar(wxCommandEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	virtual void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
    virtual void SetSize(const wxRect& rect);
    virtual void SetSize(int width, int height);
    virtual void SetSize(const wxSize& size);
	void EnviaEstado();
	void RellenaEstados();
	void PonPosicion(wxPoint &p);	
	void CambiaIp(char *p);
	void BorraIp(char *p);
	void fichero_gordo(char *p);
	void InformacionEspecial(char *p);
	void EnviaComando(int comando);
	CDlgPresencia(
		wxWindow* parent, 
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_DIALOG_STYLE, 
		const wxString& name = "dialogBox"
		);	
	void GuardaPosicion();
	~CDlgPresencia();
   void OnCloseWindow(wxCloseEvent& event);
   void OnTimer(wxTimerEvent& event);
   void OnButton(wxCommandEvent& event);
   void ActualizaIcono();
   void FlashIcono(enum ElEstado est);
   void SwitchEstado();
   void OtroEstado(int item,CListaUsers *plista);
   void InfoItem(int item,CListaUsers *plista);
   void EnviaFlag(long ip,unsigned long flag,int quitar);
   void EsFlag(char *p1);   
   void Movimiento(bool avisar);
   int DameEstado(long de,int &flag,unsigned long &userflag,wxString &nombre,int &tipo);
   void CambioParte(unsigned char *p);
   void CompruebaParte();

   int m_tiemposinmovimiento;   
// Atributos
    wxTreeItemId m_itememp[100];
    wxTreeItemId m_itemdep;
	wxTreeItemId m_itemtut;
	wxTreeItemId m_itemlpd;
	wxTreeItemId m_itemestados;
    wxSplitterWindow m_splitH;
	wxSplitterWindow m_splitV;
	CArbol  m_tree;
	CListaUsers m_listausers;
	CListaUsers m_listausersg;
	wxTimer m_timer;
	wxPoint m_mousepoint;
	ElEstado m_estado;	
	int m_privilegio;
	wxCheckBox m_nomolestar;
	wxButton m_veravisos;	
	wxString m_destado;
	wxButton m_ocultar;
	wxButton m_correo;
	wxButton m_ticket;
	wxButton m_conferenciar;	
	CTexto m_textoestado;	
	wxStaticBitmap m_imagenestado;
	wxBoxSizer *m_sizerTop;
	wxBoxSizer *m_sizerData;
	wxBoxSizer *m_sizerBoton;
	wxBoxSizer *m_sizerImagen;

	CTcontrol *m_tcontrol;
	CDlgMensaje *m_conferencia;

	CDlgMensaje *m_avisos;

	wxImageList *m_imageListNormal;
	wxImageList *m_imageListPatron;
	wxImageList *m_imageListTree;

	int m_refrescar;

	unsigned char m_Passwd[25];


	int m_antibug;	
	long m_otraip;

	long m_tiempomuerte;

	int m_flagq;
	ElEstado m_estadoq;
	wxString m_destadoq;

	wxString m_nombreinfo;

	wxString m_minombre;

	CDlgMensaje *m_dlgmensaje[500];
	CDlgTicket *m_dlgticket[500];

	long m_imagenes[500];
	int m_imaestados[500];	
	int m_faltaimagen[500];
	long m_tiempofoto[500];	
	long m_tiempochunk;
	long m_tcolores;
	long m_tmenu;
	long m_torganiza;
	long m_tweb;

	FI_DEF *m_partefi;

	long m_selip;
	wxString m_selNombre;
	
private:
    DECLARE_EVENT_TABLE()
};

extern char *lista_users[4096];
extern unsigned long _userflags[4096];

void EstadoItem(char *p,int &estado,int &flag,int &privilegio,wxString &nombre,wxString &destado,int &eltipocliente,int &departamento);
int EstadoPorIp(long de,int &flag,int &privilegio,wxString &nombre,wxString &destado,int &eltipocliente,int &departamento,unsigned long &userflag);

extern int EnBarraTareas;


extern "C"
{
extern int  nlista_users;
};

#endif // !defined(AFX_DLGPRESENCIA_H__91CC980A_5919_4B14_94A3_E590BE071A9E__INCLUDED_)
