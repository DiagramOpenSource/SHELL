// Define a new application type, each program should derive a class from wxApp
#include "DlgPresencia.h"

#define WM_SOCKET_SELECT	(WM_USER + 100)	

/*#define CONSKIPE*/

class dspresenciaApp : public wxApp
{
public:
	BOOL QuitaBarra();
	BOOL PonBarra();
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
	virtual int OnExit();	
	virtual bool ProcessMessage(WXMSG* pMsg);
	CDlgPresencia *m_pdlg;
};

char *saca_de_cola_local(int *size);
void pon_en_cola_envio(char *p,unsigned short size,bool destructivo);
long dame_the_name();
long dame_the_server();
long dame_the_localip();
void PonTheName(long nuevoname);
void PonTheServer(long nuevoserver);	
bool esta_conectado();
bool envia_estado();

struct log_reg
{	
	unsigned short firma;
	unsigned short size;
	long ip;
	long myhash1;
	long pos;
	char Nombre[30];
	int estado;
	char dEstado[128];	
	long ordenante;
	long myhash2;
	int ano;
	int mes;
	int dia;
	int hora;
	int minuto;	
	int segundo;	
};

extern int control_dspversion;
extern wxString dsp_version;

extern "C"
{
extern char file_Origen[512];
extern char file_Win[512];
bool PlayResource(const char *lpName);
extern char *ajusta_a_home_dir(char *);
};

#define C_HOLA    0x1000 // Inicio conexion al nodo
#define C_ADIOS   0x2000 // Fin conexion al nodo
#define C_CHAT    0x3000 // Mensaje de chat
#define C_DATA    0x4000 // Intercambio de datos entre cliente/operador
#define C_ESTADO  0x5000 // Cambio de estado del operador o cliente (ocupado etc)
#define C_PASAR   0x6000 // Pasar a otro operador
#define C_INFO    0x7000 // Pedir informacion al nodo
#define C_ACABADO 0x8000 // Acabado con el cliente/operador
#define C_RES9    0x9000
#define C_RESA    0xA000
#define C_RESB    0xB000
#define C_RESC    0xC000
#define C_RESD    0xD000
#define C_MULTI   0xE000
#define C_RESF    0xF000

#define TIPO_OPERADOR  0x1
#define TIPO_CLIENTE   0x2

#define ESTADO_VACIO        0x0
#define ESTADO_LIBRE        0x1
#define ESTADO_OCUPADO      0x2
#define ESTADO_NODISPONIBLE 0x3

#define LEN_CABECERA   sizeof(Cabecera)
#define LEN_DATA(a)    ((a)->c.command & 0xfff)
#define LEN_PAQUETE(a) (LEN_CABECERA + LEN_DATA(a))
#define COMANDO(a)     ((a)->c.command & 0xf000)
#define DATO_INT(a)    (*((int *)(a)->buffer))

#define PON_LEN_DATA(a,b) (a)->c.command = (((a)->c.command & 0xf000) | ((b) & 0xfff))

#define MAXBUFFER	  4096

typedef unsigned char  UChar;
typedef unsigned short UShort;
typedef unsigned long  ULONG;
typedef unsigned long  IPID;

#define DSWEBCALLID 0x7667

typedef struct 
{
	UShort id;
	UShort command;
} Cabecera;

typedef struct 
{
    Cabecera  c;
	UChar buffer[MAXBUFFER*2];
} Paquete;


typedef struct 
{
    Cabecera  c;
	UChar *buffer;
} PPaquete;


class CDlgLogin : public wxDialog  
{
public:
	CDlgLogin(CDlgPresencia* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = "dialogBox");
	~CDlgLogin();
	wxBoxSizer *m_sizer;		
	wxBoxSizer *m_sizerTop;	
	wxBoxSizer *m_sizerMid;	
	wxBoxSizer *m_sizerBottom;			
	//wxTextCtrl m_usuario;
	wxComboBox m_usuario;
	wxTextCtrl m_password;
	wxButton m_ok;
	wxButton m_cancel;
	void OnButton(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_APP(dspresenciaApp) 



