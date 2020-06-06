
//
// DlgPresencia.cpp: implementation of the CDlgPresencia class.
//
//////////////////////////////////////////////////////////////////////

// Ojo:  El parte es Ticket y el Ticket es llamada

#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif

#include "wx/wxprec.h"
#include "wx/date.h"
#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "ListaUsers.h"
#include "ListDef.h"
#include "DlgExplorer.h"
#include "DlgVolume.h"
#include "DlgAtender.h"
#include "resource.h"
#ifdef WIN32
#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)
#endif

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <shlwapi.h>
#include <shellapi.h>
#include <process.h>
#include <direct.h>

#include <richedit.h>
#include <richole.h>
#include "ImageDataObject.h"

#define TIMER_ID (wxID_HIGHEST + 1)

BEGIN_EVENT_TABLE(CTexto, wxTextCtrl)       
   EVT_LEFT_DCLICK(CTexto::OnMouseEvent)	
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CDlgLista, wxDialog)          
   EVT_BUTTON(wxID_OK, CDlgLista::OnOK)
   EVT_BUTTON(4111, CDlgLista::OnBusca)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(CDlgId, wxDialog)     
   EVT_BUTTON(-1, CDlgId::OnButton)        
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CDlgAlarma, wxDialog)  
   EVT_BUTTON(-1, CDlgAlarma::OnButton)        
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CDlgParte, wxDialog)  
   EVT_BUTTON(-1, CDlgParte::OnButton)        
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CDlgMensaje, wxDialog)          
   EVT_CLOSE(CDlgMensaje::OnCloseWindow)
   EVT_MENU(IDCANCEL, CDlgMensaje::OnCancel)      
   EVT_BUTTON(-1, CDlgMensaje::OnButton)
   EVT_ACTIVATE(CDlgMensaje::OnActivate)
   EVT_SIZE(CDlgMensaje::OnSize)	
   EVT_DROP_FILES(CDlgMensaje::OnDropFiles)	
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CDlgTicket, wxDialog)          
   EVT_MENU(IDCANCEL, CDlgTicket::OnCancel)      
   EVT_BUTTON(-1, CDlgTicket::OnButton)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(CEnvia, wxTextCtrl)
   EVT_KEY_UP(CEnvia::OnKeyUp)
   EVT_KEY_DOWN(CEnvia::OnKeyDown)   
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(CDlgPresencia, wxDialog)     
   EVT_CLOSE(CDlgPresencia::OnCloseWindow)
   EVT_TIMER(TIMER_ID, CDlgPresencia::OnTimer)
   EVT_BUTTON(-1, CDlgPresencia::OnButton)
   EVT_MENU(2000, CDlgPresencia::OnPresente)
   EVT_MENU(2001, CDlgPresencia::OnIdentificar)
   EVT_MENU_RANGE(1001,1099,CDlgPresencia::OnAusente)   
   EVT_MENU(1120, CDlgPresencia::OnPassword)      
   EVT_MENU(1110, CDlgPresencia::OnSalida)   
   EVT_MENU(5110, CDlgPresencia::OnSalida)   
   EVT_MENU(1100, CDlgPresencia::OnActiva)   
   EVT_MENU(1101, CDlgPresencia::OnTareas)   
   EVT_MENU(6000, CDlgPresencia::OnPresente)
   EVT_MENU_RANGE(5001,5099,CDlgPresencia::OnAusente)
   EVT_MENU(1111, CDlgPresencia::OnVacaciones)   
   EVT_MENU(1112, CDlgPresencia::OnEnfermo)   
   EVT_MENU(5111, CDlgPresencia::OnVacaciones)   
   EVT_MENU(5112, CDlgPresencia::OnEnfermo)   
   EVT_MENU(1113, CDlgPresencia::OnOtros)   
   EVT_MENU(5113, CDlgPresencia::OnOtros)
   EVT_MENU(1114, CDlgPresencia::OnParte)
   EVT_MENU(5114, CDlgPresencia::OnParte)
   EVT_MENU(1115, CDlgPresencia::OnAlarma)
   EVT_MENU(5115, CDlgPresencia::OnAlarma)
   EVT_MENU(1116, CDlgPresencia::OnWeb)
   EVT_MENU(5116, CDlgPresencia::OnWeb)
   EVT_MENU(1117, CDlgPresencia::OnFinParte)
   EVT_MENU(5117, CDlgPresencia::OnFinParte)   
   EVT_MENU(2101, CDlgPresencia::OnOcultar)   
   EVT_MENU(2102, CDlgPresencia::OnDesocultar)   
   EVT_MENU(IDCANCEL, CDlgPresencia::OnCancel)         
   EVT_SIZE(CDlgPresencia::OnSize)	
END_EVENT_TABLE()


#ifdef WIN32
#define WM_MITRAYMESSAGE (WM_USER+1)

static HWND i_hWnd = NULL;
static HWND par_hWnd = NULL;
static CDlgPresencia *pdlg = NULL;

#define DAEMON_CLASS_NAME "DSTVNCviewer Daemon"
char *MENU_CLASS_NAME = "DsWinTvnc Icono";
UINT MENU_ADD_CLIENT_MSG;
UINT MENU_DISABLE_LOCAL_INPUT;
UINT MENU_ENABLE_LOCAL_INPUT;


extern long en_tiempo_telefono;

#define MAXIPS 24
extern "C"
{	
	extern HWND hwndMDIFrame;
	extern HWND hDlgOperacion;
	int KillOthers(char *amatar);
	extern int nvoces_activas;
	extern long voces_activas[MAXIPS];

	long ipvoz[4096];
	long codigovoz[4096];
	long rutavoz[4096];
	long iplocal[4096];
	int  nlista_users= 0;
	long IpPorCodigo(long codigo);
	long CodigoPorIp(long ip);

	void PonRuta(long codigo);
	void QuitaRuta(long codigo);
	void quitab(char *);
	void quitan(char *);
};

void lee_accesos(char *accesos);
void pon_accesos(char *accesos);
void pon_ultimo_acceso(char *accesos);


char *lista_users[4096];
unsigned long _userflags[4096];

int nmis_ips = 0;
long mis_ips[512];

static int maxpool = 10;
static int poolposicion = 0;
static CDlgMensaje *pooldlgs[10];
static int maxpoolc = 20;
static CDlgMensaje *conexionesdlgs[20];

static CDlgReloj *pDlgReloj = NULL;

static int midepartamento = -1;

extern unsigned char passwordinicial[25];
extern char usuarioinicial[128];
extern char accediocon[256];

int elflag = 0;

static char ultimo_parte[2048];
static int tam_ultimo_parte = 0;
static long id_ultimo_parte = 0;
static long tiempoparte = -1;


static char ultimo_ticket[2048];
static int tam_ultimo_ticket = 0;
static long id_ultimo_ticket = 0;
static long tiempoticket = -1;


static long tiempologin = 0;

static long tiempoalarma = 0;
static wxString horaalarma;
static wxString minutoalarma;
static wxString textoalarma;


static wxString NMiWeb;
static wxString MiWeb;
static wxString MiWebUrl;
static int lMiWeb = 0;

static int recibido_estados = 0;

static int idle_aviso = 0;

static long chunkrecibido = 0;
static long ult_chunkrecibido = 0;

extern "C"
{
void st_campo(int campo,FI_DEF *pfi,char *origen);
void ld_campo(int campo,register FI_DEF *pfi,char *destino);
}


static char *ElEstado[20] =
{
	"Presente",
	"Ocupado",
	"Presente",
	"No Esta",
	"Ocioso",
	"Vacaciones",
	"Enfermedad",
	"Otros",
	""
};

static char e_trabajo[50][200] =
{
	"Ocupado",
    "Al Telefono",
	"Visita a cliente",
	"Reunido",
	"Estoy con ",
	"Desplazamiento a cliente",
	"Regreso de cliente",
	""
};
static int flag_trabajo[50] =
{
	1,
	1,
	1,
	1,
	1,
	1,
	1
};

static char e_ocio[50][200] =
{
	"Desayunando",
	"Comiendo",
	"Enseguida estoy",
	"Salida breve",
	"Salida larga",
	"Ocio",
	""
};

static int flag_ocio[50] = 
{
	0,
	0,
	0,
	1,
	1,
	1
};

static char colores[50][10] =
{	
"000255000",
"255000000",
"000255000",
"128000000",
"255000000",
"000255255",
"128128128",
"255000000",
"255000000",
"255000000",
"255000000",
"255000000",
"",
};


#define TIPO_ACTIVO   0x000001
#define TIPO_NULO     0x000002

static int conectado = 0;
static int tipo_cliente = TIPO_ACTIVO;
static int dentro = 0;


static long tdiapre01gz = NULL;
static long td_diapre01 = NULL;

static int lfich_diapre01 = 0;

static long tdiapre02gz = NULL;
static long tdiapre01mas = NULL;
static long tdiapre02mas = NULL;
static long tdiapre03mas = NULL;
static long tdiapre04mas = NULL;
static long txiapre02gz = NULL;

static long ltdiapre01gz = NULL;
static long ltdiapre02gz = NULL;
static long ltdiapre01mas = NULL;
static long ltdiapre02mas = NULL;
static long ltdiapre03mas = NULL;
static long ltdiapre04mas = NULL;
static long ltxiapre02gz = NULL;

static char *diapre01gz = NULL;
static char *d_diapre01 = NULL;
static char *diapre02gz = NULL;
static char *diapre01mas = NULL;
static char *diapre02mas = NULL;
static char *diapre03mas = NULL;
static char *diapre04mas = NULL;

static char *xiapre02gz = NULL;

static char **ptfichs[7] = 
{
	&diapre01gz,
	&diapre02gz,
	&diapre01mas,
	&diapre02mas,
	&diapre03mas,
	&diapre04mas,
	&xiapre02gz
};
static long ppfichs[7] = {0,0,0,0,0,0,0};

static long controlcarga = NULL;
static long controlmio = NULL;

static char web_tmp[512] = "";
static long tiempo_web = 0;

CDlgLista *m_dlginfo = NULL;
static int m_ndiasinfo = 0;
wxString m_nombreinfo;


extern "C"
{
   int lee_def(char *nfi,FI_DEF *fi,int man);
   void free_def(FI_DEF *fi,int man);

   /*
   long rw_x_lseek_(int,long,int);
   int rw_x_read_(int,void *,unsigned int);
   int rw_x_open_(const char *,int,int);
   int rw_x_close_(int);
   int rw_x_write_(int,const void *,unsigned int);
   */
   long x_rw_lseek_(int,long,int);
   int x_rw_read_(int,void *,unsigned int);
   int x_share_open(const char *,int,int);
   int x_rw_close_(int);
   int x_rw_write_(int,const void *,unsigned int);
};

static long ucodigo = 0,uip = 0,uruta = 0,ulocal = 0;
static long mi_ipvoz = 0,mi_rutavoz = 0;

long IpRemotaCodigo(long codigo)
{
	int i;
	
	
	if (ucodigo == codigo)
		return uip;

	for (i = 0;i < nlista_users;i++)
	{
		if (codigovoz[i] == codigo)
		{
			ucodigo = codigovoz[i];
			uruta = rutavoz[i];
			ulocal = iplocal[i];
			return ((uip = ipvoz[i]));
		}
	}

	return codigo;
}


long IpPorCodigo(long codigo)
{
	int i;
	
	
	if (ucodigo == codigo)
	{
		return ulocal;

		if (uip == mi_ipvoz)
			return ulocal;
		else
			return uip;
	}

	for (i = 0;i < nlista_users;i++)
	{
		if (codigovoz[i] == codigo)
		{
			ucodigo = codigovoz[i];
			uruta = rutavoz[i];
			ulocal = iplocal[i];
			uip = ipvoz[i];			
			if (uip == mi_ipvoz)
				return ulocal;
			else
				return uip;
		}
	}

	return codigo;
}

long CodigoPorIp(long ip)
{
	int i;	

	if (uip == ip || ulocal == ip)
		return ucodigo;

	for (i = 0;i < nlista_users;i++)
	{
		if (ipvoz[i] == ip || iplocal[i] == ip)
		{
			uip = ipvoz[i];
			uruta = rutavoz[i];
			ulocal = iplocal[i];
			return ((ucodigo = codigovoz[i]));
		}
	}

	return ip;
}

long RutaPorCodigo(long codigo)
{
	int i;
	
	
	if (ucodigo == codigo)
		return uruta;

	for (i = 0;i < nlista_users;i++)
	{
		if (codigovoz[i] == codigo)
		{
			ucodigo = codigo;
			uip = ipvoz[i];
			ulocal = iplocal[i];
			return ((uruta = rutavoz[i]));
		}
	}

	return 0;
}

#ifndef ANY_SIZE

#define ANY_SIZE 1

#endif

typedef struct _MIB_IPFORWARDROW
{
    DWORD        dwForwardDest;
    DWORD        dwForwardMask;
    DWORD        dwForwardPolicy;
    DWORD        dwForwardNextHop;
    DWORD        dwForwardIfIndex;
    DWORD        dwForwardType;
    DWORD        dwForwardProto;
    DWORD        dwForwardAge;
    DWORD       dwForwardNextHopAS;
    DWORD        dwForwardMetric1;
    DWORD        dwForwardMetric2;
    DWORD        dwForwardMetric3;
    DWORD        dwForwardMetric4;
    DWORD        dwForwardMetric5;
}MIB_IPFORWARDROW, *PMIB_IPFORWARDROW;

typedef struct _MIB_IPFORWARDTABLE
{
    DWORD               dwNumEntries;
    MIB_IPFORWARDROW    table[ANY_SIZE];
}MIB_IPFORWARDTABLE, *PMIB_IPFORWARDTABLE;


typedef DWORD (WINAPI *__GetIpForwardTable)(PMIB_IPFORWARDTABLE pIpForwardTable,
											PULONG              pdwSize,
											BOOL                bOrder
						 );

typedef DWORD (WINAPI *__CreateIpForwardEntry)(PMIB_IPFORWARDROW pRoute);

typedef DWORD (WINAPI *__DeleteIpForwardEntry)(PMIB_IPFORWARDROW pRoute);


static HINSTANCE ll;
static __GetIpForwardTable GetIpForwardTable;
static __CreateIpForwardEntry CreateIpForwardEntry;
static __DeleteIpForwardEntry DeleteIpForwardEntry;

static void trata_ruta(int modo,long destino,long ruta)
{
	long quebusco;

	if (modo)
		quebusco = destino;
	else
		quebusco = 0;

	if (ll)
	{		
		if (GetIpForwardTable && CreateIpForwardEntry && DeleteIpForwardEntry)
		{
			PMIB_IPFORWARDTABLE pIpForwardTable = NULL;
			PMIB_IPFORWARDROW pRow = NULL;
			DWORD dwSize = 0;
			BOOL bOrder = FALSE;
			DWORD dwStatus = 0;
			unsigned long int i;			
				
			// Find out how big our buffer needs to be.
			dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
			if (dwStatus == ERROR_INSUFFICIENT_BUFFER) {
			  // Allocate the memory for the table
			  if (!(pIpForwardTable = (PMIB_IPFORWARDTABLE)malloc(dwSize))) 
			  {				
				return;
			  }
			  // Now get the table.
			  dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
			}

			if (dwStatus != ERROR_SUCCESS) 
			{			  
			  if (pIpForwardTable) 
				free(pIpForwardTable);
			  return;
			}
   
			// Search for the row in the table we want. The default gateway has a destination
			// of 0.0.0.0. Notice that we continue looking through the table, but copy only 
			// one row. This is so that if there happen to be multiple default gateways, we can
			// be sure to delete them all.
			for (i=0; i < pIpForwardTable->dwNumEntries; i++) 
			{
			  if (pIpForwardTable->table[i].dwForwardDest == (unsigned long)quebusco) 
			  {
				// We have found the default gateway.
				if (modo)
				{
					if (pIpForwardTable->table[i].dwForwardMask == 0xFFFFFFFF)
					{
						dwStatus = DeleteIpForwardEntry(&(pIpForwardTable->table[i]));					
						break;
					}
				}
				else if (!pRow) 
				{
				  // Allocate some memory to store the row in; this is easier than filling
				  // in the row structure ourselves, and we can be sure we change only the
				  // gateway address.
				  pRow = (PMIB_IPFORWARDROW)malloc(sizeof(MIB_IPFORWARDROW));
				  if (!pRow) 
				  {					
					return;
				  }
				  // Copy the row
				  memcpy(pRow, &(pIpForwardTable->table[i]), sizeof(MIB_IPFORWARDROW));				  
				  break;
				}				
			  }
			}           
   
			// all the other properties of the route will
			// be the same as they were previously.
			if (!modo && pRow)
			{
				pRow->dwForwardDest = destino;
				pRow->dwForwardMask = 0xFFFFFFFF;
				pRow->dwForwardNextHop = ruta;
			
				
				// Create a new route entry for the default gateway.
				dwStatus = CreateIpForwardEntry(pRow);
			}

			// Free resources
			if (pIpForwardTable) 
			  free(pIpForwardTable);
			if (pRow)
			  free(pRow);
		}
		/*FreeLibrary(ll);*/
	}
}


void PonRuta(long codigo)
{	
	long destino;
	if (mi_rutavoz)
	{
		destino = IpPorCodigo(codigo);
		trata_ruta(0,destino,mi_rutavoz);
	}
}

void QuitaRuta(long codigo)
{	
	long destino;
	if (mi_rutavoz)
	{
		destino = IpPorCodigo(codigo);
		trata_ruta(1,destino,mi_rutavoz);
	}
}


int DescomprimeDatos(char *datosc,long tdatosc,char **d_datos,long *td_datos);

static int CargaLosDatos()
{
	int file;
	long id = 0;
	long size = 0;	
	
	file = open(ajusta_a_home_dir("\\dsp\\data.cac"),O_RDONLY|O_BINARY);
	if (file < 0)
	{
		return 0;
	}

	read(file,(char *)&id,sizeof(id));
	if (id != 0x93171891)
	{
		close(file);
		return 0;
	}
	read(file,(char *)&size,sizeof(size));
	if (size != lseek(file,0L,SEEK_END))
	{
		close(file);
		return 0;
	}
	lseek(file,sizeof(id)+sizeof(size),SEEK_SET);
	read(file,(char *)&controlcarga,sizeof(long));
	read(file,(char *)&controlmio,sizeof(long));
	read(file,(char *)&tdiapre01gz,sizeof(long));
	read(file,(char *)&tdiapre02gz,sizeof(long));
	read(file,(char *)&tdiapre01mas,sizeof(long));
	read(file,(char *)&tdiapre02mas,sizeof(long));
	read(file,(char *)&tdiapre03mas,sizeof(long));
	read(file,(char *)&tdiapre04mas,sizeof(long));
	read(file,(char *)&txiapre02gz,sizeof(long));
	if (tdiapre01gz > 0)
	{
		diapre01gz  = (char *)malloc(tdiapre01gz);		
	}
	if (tdiapre02gz > 0)
	{
		diapre02gz  = (char *)malloc(tdiapre02gz);		
	}
	if (tdiapre01mas > 0)
	{
		diapre01mas  = (char *)malloc(tdiapre01mas);		
	}
	if (tdiapre02mas > 0)
	{
		diapre02mas  = (char *)malloc(tdiapre02mas);		
	}
	if (tdiapre03mas > 0)
	{
		diapre03mas  = (char *)malloc(tdiapre03mas);		
	}
	if (tdiapre04mas > 0)
	{
		diapre04mas  = (char *)malloc(tdiapre04mas);
	}
	if (txiapre02gz > 0)
	{
		xiapre02gz  = (char *)malloc(txiapre02gz);		
	}

	if (tdiapre01gz > 0)
		read(file,diapre01gz,tdiapre01gz);
	if (tdiapre02gz > 0)
		read(file,diapre02gz,tdiapre02gz);
	if (tdiapre01mas > 0)
		read(file,diapre01mas,tdiapre01mas);
	if (tdiapre02mas > 0)
		read(file,diapre02mas,tdiapre02mas);
	if (tdiapre03mas > 0)
		read(file,diapre03mas,tdiapre03mas);
	if (tdiapre04mas > 0)
		read(file,diapre04mas,tdiapre04mas);
	if (txiapre02gz > 0)
		read(file,xiapre02gz,txiapre02gz);	

	close(file);

	ltdiapre01gz  = tdiapre01gz;
	ltdiapre02gz  = tdiapre02gz;
	ltdiapre01mas = tdiapre01mas;
	ltdiapre02mas = tdiapre02mas;
	ltdiapre03mas = tdiapre03mas;
	ltdiapre04mas = tdiapre04mas;
	ltxiapre02gz  = txiapre02gz;

	return 1;
}

static int GrabaLosDatos()
{
	int file;
	long id = 0x93171891;
	long size = sizeof(id)+sizeof(size)+sizeof(long)*9+tdiapre01gz+tdiapre02gz+
		        tdiapre01mas+tdiapre02mas+tdiapre03mas+tdiapre04mas+txiapre02gz;

    if (chunkrecibido == ult_chunkrecibido)
		return 1;	

	file = open(ajusta_a_home_dir("\\dsp\\data.cac"),O_RDWR|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if (file < 0)
	{
		return 0;
	}

	write(file,(char *)&id,sizeof(id));
	write(file,(char *)&size,sizeof(size));	
	write(file,(char *)&controlcarga,sizeof(long));
	write(file,(char *)&controlmio,sizeof(long));
	write(file,(char *)&tdiapre01gz,sizeof(long));
	write(file,(char *)&tdiapre02gz,sizeof(long));
	write(file,(char *)&tdiapre01mas,sizeof(long));
	write(file,(char *)&tdiapre02mas,sizeof(long));
	write(file,(char *)&tdiapre03mas,sizeof(long));
	write(file,(char *)&tdiapre04mas,sizeof(long));
	write(file,(char *)&txiapre02gz,sizeof(long));
	if (tdiapre01gz > 0)
		write(file,diapre01gz,tdiapre01gz);
	if (tdiapre02gz > 0)
		write(file,diapre02gz,tdiapre02gz);
	if (tdiapre01mas > 0)
		write(file,diapre01mas,tdiapre01mas);
	if (tdiapre02mas > 0)
		write(file,diapre02mas,tdiapre02mas);
	if (tdiapre03mas > 0)
		write(file,diapre03mas,tdiapre03mas);
	if (tdiapre04mas > 0)
		write(file,diapre04mas,tdiapre04mas);
	if (txiapre02gz > 0)
		write(file,xiapre02gz,txiapre02gz);	

	close(file);

	chunkrecibido = ult_chunkrecibido;
	return 1;
}

static int SizeEstado(char *p)
{
	int tama = sizeof(long)+sizeof(int)*2;	
	tama += (strlen(p+tama)+1);	
	tama += (strlen(p+tama)+1);
	tama += sizeof(int)*2;
	tama += sizeof(long); /* Ip de voz */
	tama += sizeof(long); /* Ruta ip de voz */
	tama += sizeof(long); /* iplocal */
	return tama;
}

int ListaPorIp(long de,wxString &nombre)
{
	int item,k;
	for (item = 0;item < nlista_users;item++)
	{
		if (*((long *)lista_users[item]) == de)
		{
			nombre = lista_users[item]+sizeof(long)+sizeof(int)*2;
			if ((k = nombre.Find(1)) > -1)
			{
				nombre.Truncate(k);
			}
			return item;
		}
	}
	return -1;
}


int ListaPorNombre(wxString &nombre)
{
	wxString nombre2;
	int item;
	for (item = 0;item < nlista_users;item++)
	{
		nombre2 = lista_users[item]+sizeof(long)+sizeof(int)*2;
		{
			int k;
			if ((k = nombre2.Find(1)) > -1)
			{
				nombre2.Truncate(k);
			}
		}
		if (!nombre.Cmp(nombre2))
		{
			return item;	
		}
	}
	return -1;
}

void system_hebrado(void *nombre)
{
	char *p;
	char *par1 = NULL;

	p = (char *)nombre;
	p += strlen(p);
	p--;

	while(p > nombre && *p != '"') p--;
	if (*p == '"')
	{
		*p = 0;
		p--;
		while(p > nombre && *p != '"') p--;
		if (*p == '"')
		{
			*p = 0;			
			quitab((char *)nombre);
			p++;
			par1 = p;
			quitab(par1);
		}
	}

	_spawnl( _P_WAIT, (const char *)nombre, (const char *)nombre, par1 ,NULL );
	
	free(nombre);
}

#ifdef CONSKIPE
int llama_a_skype(CDlgPresencia *pdlg,unsigned long ip)
{
	void *nombre;
	DWORD Size;
	char tmp[1024];
	wxString s,s1;	
	memset(tmp,0,sizeof(tmp));	

	Size = sizeof(tmp);
	RegQueryValue(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\Voz",tmp,(PLONG)&Size);
	if (!tmp[0])
	{	
		Size = sizeof(tmp);
		RegQueryValue(HKEY_CLASSES_ROOT,"callto\\shell\\open\\command",tmp,(PLONG)&Size);
	}

	s1 = pdlg->m_textoestado.GetValue();
	int pi = s1.Find("Voz:");
	if (pi > -1)
	{
		int n;
		pi += 4;
		n = 0;
		while(s1[pi+n] && s1[pi+n] != ' ') n++;
		s = s1.Mid(pi,n);
	}

	s1 = tmp;
	s1. Replace("%1",s);

	nombre = malloc(strlen(s1)+1);
	strcpy((char *)nombre,s1);
    _beginthread(system_hebrado,NULL,(void *)nombre);
	return 0;
}
#endif


static wxString auto_escapa(const char *mensaje)
{
	wxString r;	
	while(*mensaje)
	{
		if (*mensaje == '{' || *mensaje == '}' || *mensaje == '"' || *mensaje == '\\')
		{
			r += (char)'\\';
		}
		r += *mensaje;		
		mensaje++;
	}
	return r;
}

static void SacaIpVoz(char *p,int i)
{
	codigovoz[i] = *((long *)p);
	p += sizeof(long);
	p += sizeof(int);
	p += sizeof(int);
	p += (strlen(p)+1);
	p += (strlen(p)+1);
	p += sizeof(int);
	p += sizeof(int);
	ipvoz[i] = *((long *)p);	
	p += sizeof(long);
	rutavoz[i] = *((long *)p);
	p += sizeof(long);
	iplocal[i] = *((long *)p);
	if (ucodigo == codigovoz[i])
	{
		uip = ipvoz[i];		
		uruta = rutavoz[i];
		ulocal = iplocal[i];
	}
	if (codigovoz[i] == dame_the_name())
	{
		mi_ipvoz = ipvoz[i];
		mi_rutavoz = rutavoz[i];
	}
}

void EstadoItem(char *p,int &estado,int &flag,int &privilegio,wxString &nombre,wxString &destado,int &eltipocliente,int &departamento)
{
	p += sizeof(long);
	estado = *((int *)p);
	flag = estado & 0xffff0000;
	flag >>= 16;
	estado &= 0xffff;
	p += sizeof(int);
	privilegio = *((int *)p);
	p += sizeof(int);
	nombre = p;
	{
		int k;
		if ((k = nombre.Find(1)) > -1)
		{
			nombre.Truncate(k);
		}
	}
	p += (strlen(p)+1);
	destado = p;
	p += (strlen(p)+1);
	eltipocliente = *((int *)p);
	p += sizeof(int);
	departamento = *((int *)p);	
}

int EstadoPorIp(long de,int &flag,int &privilegio,wxString &nombre,wxString &destado,int &eltipocliente,int &departamento,unsigned long &userflag)
{
	int item;
	int estado;
	for (item = 0;item < nlista_users;item++)
	{
		if (*((long *)lista_users[item]) == de)
		{
			break;
		}
	}
	if (item >= nlista_users)
	{
		return -1;
	}
	userflag = _userflags[item];
	EstadoItem(lista_users[item],estado,flag,privilegio,nombre,destado,eltipocliente,departamento);
	return estado;
}

bool ListaCambiaEstado(char *p1,char *p2,wxString &destado,int &dep)
{
	bool cambia = false;
	int estado1,flag1,privilegio1,tipo1,dep1;
	wxString nombre1,destado1;
	int estado2,flag2,privilegio2,tipo2;
	wxString nombre2;
	EstadoItem(p1,estado1,flag1,privilegio1,nombre1,destado1,tipo1,dep1);
	EstadoItem(p2,estado2,flag2,privilegio2,nombre2,destado,tipo2,dep);

	if (nombre1.Cmp(nombre2) || destado1.Cmp(destado))
	{
		cambia = true;
	}
	else
	{
		if (estado1 != estado2)
		{
			cambia = true;
		}
	}
	if (flag1 != flag2 || tipo1 != tipo2 || dep1 != dep)
	{
		cambia = true;
	}			
	return cambia;
}

void Suena(void *par)
{
	PlayResource("WRING");
}


static HINSTANCE mapil = NULL;	
void Email(void *_email)
{	
	static char address[512];
	char *email = (char *)_email;

	if (!mapil)
	{
		return;
	}

	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(mapil, "MAPISendMail");
	if (lpfnSendMail == NULL)
	{		
		return;
	}	
	
	MapiRecipDesc to;
	memset(&to, 0, sizeof(to));
	to.ulRecipClass = MAPI_TO;
	to.lpszAddress = address;
	MapiMessage message;
	memset(&message, 0, sizeof(message));
	if (email)
	{
		strcpy(address,email);
		message.nRecipCount = 1;
		message.lpRecips = &to;
	}

	int nError = lpfnSendMail(0, (ULONG)NULL/*par_hWnd*/,
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);
	Sleep(300000);
}


extern HINSTANCE wxhInstance;

static void SacaDelTray(unsigned long id)
{
    NOTIFYICONDATA m_nid;
    HINSTANCE mI = wxhInstance;	

    memset(&m_nid, 0 , sizeof(m_nid));
    m_nid.cbSize = sizeof(m_nid);   
    m_nid.uID = id;
    m_nid.hWnd = i_hWnd;
    m_nid.uFlags = 0;    
    Shell_NotifyIcon(NIM_DELETE,&m_nid);    
}

int x_share_open(const char *file,int mode,int createmode)
{
	int fd = -1;
	if (*file >= 0 && *file < 7)
	{
		fd = (int)*file;
		if (*ptfichs[fd])
		{
			ppfichs[fd] = 0L;
			return fd;
		}
	}
	return fd;
}

int x_rw_close_(int fd)
{
	return 0;
}

long x_rw_lseek_(int fd,long pos ,int mode)
{
	switch(mode)
	{
	case SEEK_SET:
		ppfichs[fd] = pos;
		break;
	case SEEK_CUR:
		ppfichs[fd] += pos;
		break;
	case SEEK_END:
		switch(fd)
		{
		case 0:
			ppfichs[fd] = tdiapre01gz;
			break;
		case 1:
			ppfichs[fd] = tdiapre02gz;
			break;
		case 2:
			ppfichs[fd] = tdiapre01mas;
			break;
		case 3:
			ppfichs[fd] = tdiapre02mas;
			break;
		case 4:
			ppfichs[fd] = tdiapre03mas;
			break;
		case 5:
			ppfichs[fd] = tdiapre04mas;
			break;
		case 6:
			ppfichs[fd] = txiapre02gz;
			break;
		}
		ppfichs[fd] += pos;
		break;
	}
	return ppfichs[fd];
}

int x_rw_read_(int fd,void *buffer,unsigned int len)
{
	char *p = NULL;	
	long fin = ppfichs[fd] + len;

	switch(fd)
	{
	case 0:
		p = diapre01gz;
		if (fin > tdiapre01gz)
			fin = tdiapre01gz;
		break;
	case 1:
		p = diapre02gz;
		if (fin > tdiapre02gz)
			fin = tdiapre02gz;
		break;
	case 2:
		p = diapre01mas;
		if (fin > tdiapre01mas)
			fin = tdiapre01mas;
		break;
	case 3:
		p = diapre02mas;
		if (fin > tdiapre02mas)
			fin = tdiapre02mas;
		break;
	case 4:
		p = diapre03mas;
		if (fin > tdiapre03mas)
			fin = tdiapre03mas;
		break;
	case 5:
		p = diapre04mas;
		if (fin > tdiapre04mas)
			fin = tdiapre04mas;
		break;
	case 6:
		p = xiapre02gz;
		if (fin > txiapre02gz)
			fin = txiapre02gz;
		break;
	}
	if (!p)
		return -1;

	len = fin - ppfichs[fd];
	memcpy(buffer,p+ppfichs[fd],len);

	ppfichs[fd] += len;

	return len;
}

int x_rw_write_(int fd,const void *buffer,unsigned int len)
{
	char *p = NULL;	
	long fin = ppfichs[fd] + len;

	switch(fd)
	{
	case 0:
		p = diapre01gz;
		if (fin > tdiapre01gz)
			fin = tdiapre01gz;
		break;
	case 1:
		p = diapre02gz;
		if (fin > tdiapre02gz)
			fin = tdiapre02gz;
		break;
	case 2:
		p = diapre01mas;
		if (fin > tdiapre01mas)
			fin = tdiapre01mas;
		break;
	case 3:
		p = diapre02mas;
		if (fin > tdiapre02mas)
			fin = tdiapre02mas;
		break;
	case 4:
		p = diapre03mas;
		if (fin > tdiapre03mas)
			fin = tdiapre03mas;
		break;
	case 5:
		p = diapre04mas;
		if (fin > tdiapre04mas)
			fin = tdiapre04mas;
		break;
	case 6:
		p = xiapre02gz;
		if (fin > txiapre02gz)
			fin = txiapre02gz;
		break;
	}
	if (!p)
		return -1;

	len = fin - ppfichs[fd];
	memcpy(p+ppfichs[fd],buffer,len);

	return len;
}


BOOL CALLBACK _INCONILLO(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
    case WM_SYSCOMMAND:      
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case ID_Echo:
				{
					unsigned long idecho = 0x12123310;
					unsigned long *p;
					int j;
					wxString text;
					char buff1[256];
					p = (unsigned long *)lParam;
					if (pdlg)
					{						
						for (int i = 0;i < 500;i++)
						{
							if (pdlg->m_dlgmensaje[i] && pdlg->m_dlgmensaje[i]->m_ip == (long)p[1])
							{								
								if (p[0] == 0x12123310 || p[0] == 0x12123313)
								{
									pdlg->m_dlgmensaje[i]->Show(TRUE);

									text = wxString::Format("ACTIVANDO VOZ: Tiempo:%6ld ms ",p[2]);
									if (p[0] == 0x12123313)
									{
										text += wxString::Format(" - %6ld ms",p[7]);
										if (p[7] > 450)
										{
											if (p[7] < 600)
												text += " VOZ NO RECOMENDABLE";
											else
												text += " NO USAR LA VOZ!! VELOCIDAD INSUFICIENTE!!";
										}
										else
										{
											if (p[7] > 300)
												text += " VELOCIDAD SUFICIENTE";
											else
												text += " VELOCIDAD OK";
										}
									}
									if ((p[3] & 0x1))
									{
										text += " ATENCION: SIN SONIDO ";
									}
									else 
									{									
										if ((p[3] & 0x4))
										{
											text += " ATENCION: HALF-DUPLEX ";
										}
										if (p[0] == 0x12123310 && (p[3] & 0x2))
										{
											text += " (no input) ";
										}
										text += " VOZ ACTIVADA ";
									}

									
									for (j = 0;j < (int)p[4] && j < 30;j++)
									{
										if (p[0] != 0x12123313/* || j != 7*/)
										{
											sprintf(buff1,"[%lX]",p[5+j]);
											text += buff1;
										}
									}
									text = auto_escapa(text);
									pdlg->m_dlgmensaje[i]->m_recibe.SetInsertionPoint(pdlg->m_dlgmensaje[i]->m_recibe.GetLastPosition() /*0*/ );
									text = "{\\rtf1 "+ text +" \\par}";
									pdlg->m_dlgmensaje[i]->m_recibe.WriteText("\n");
									pdlg->m_dlgmensaje[i]->m_recibe.WriteText(text);
									pdlg->m_dlgmensaje[i]->m_recibe.WriteText("\n");									
									pdlg->m_dlgmensaje[i]->m_recibe.ShowLastPosition();
									
									if (::IsWindowVisible((HWND)pdlg->m_dlgmensaje[i]->GetHandle()))
									{
										pdlg->m_dlgmensaje[i]->m_envia.SetFocus();
									}
									pdlg->m_dlgmensaje[i]->m_poractivar = FALSE;
									if (p[0] == 0x12123313)
									{
										pdlg->m_dlgmensaje[i]->m_iptlvoz = p[5];
										pdlg->m_dlgmensaje[i]->ActivaVoz();										

										/*
										LLAMADA(pdlg->m_flagq);
										pdlg->EnviaEstado();
										*/
									}
								}
							}
						}
					}

				}
				break;
			case ID_Mensaje:
				if (pdlg)
				{
					wxString text;
					for (int i = 0;i < 500;i++)
					{
						if (pdlg->m_dlgmensaje[i] && pdlg->m_dlgmensaje[i]->m_ip == (long)lParam)
						{
							pdlg->m_dlgmensaje[i]->Show(TRUE);
							if (!HIWORD(wParam))
							{
								text = "No tienes sonido";
							}
							else
								text = ""; // falta
							text = auto_escapa(text);
							pdlg->m_dlgmensaje[i]->m_recibe.SetInsertionPoint(pdlg->m_dlgmensaje[i]->m_recibe.GetLastPosition() /*0*/ );
							text = "\n{\\rtf1\\b "+ text +" \\par}\n";
							pdlg->m_dlgmensaje[i]->m_recibe.WriteText(text);
							pdlg->m_dlgmensaje[i]->m_recibe.SetInsertionPointEnd();
							pdlg->m_dlgmensaje[i]->m_recibe.ShowLastPosition();
							
							if (::IsWindowVisible((HWND)pdlg->m_dlgmensaje[i]->GetHandle()))
							{
								pdlg->m_dlgmensaje[i]->m_envia.SetFocus();
							}
							pdlg->m_dlgmensaje[i]->m_poractivar = FALSE;
						}
					}
				}
				break;
			case ID_Volumen:
				for (int i = 0;i < 500;i++)
				{
					if (pdlg->m_dlgmensaje[i] && pdlg->m_dlgmensaje[i]->m_dlgvolume)
					{								
						pdlg->m_dlgmensaje[i]->m_dlgvolume->AjustaVolumen(0);
					}
				}
				break;
			}
		}
		break;
    case WM_CHAR:
		break;
    case WM_MITRAYMESSAGE:
        if (lParam == WM_RBUTTONUP)
        {
			if (IsWindowVisible(par_hWnd))
			{
				ShowWindow(par_hWnd,SW_HIDE);
			}
			else
			{			
				ShowWindow(par_hWnd,SW_SHOW);
				BringWindowToTop(par_hWnd);
				SetForegroundWindow(par_hWnd);
				if (pdlg)
				{
					for (int i = 0;i < 500;i++)
					{
						if (pdlg->m_dlgmensaje[i] && pdlg->m_dlgmensaje[i]->m_poractivar)
						{
							pdlg->m_dlgmensaje[i]->Show(TRUE);
							pdlg->m_dlgmensaje[i]->m_envia.SetFocus();
							pdlg->m_dlgmensaje[i]->m_poractivar = FALSE;
						}
					}
				}
			}
            return 1;
        }
		else if (lParam == WM_LBUTTONDOWN && pdlg)
		{			
			pdlg->SwitchEstado();
		}
        return 0;
        break;
	}	
	return FALSE;
}


#endif

void CEnvia::OnKeyUp(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_RETURN)
	{
		SetInsertionPoint(0);
	}
	else if (event.GetKeyCode() == WXK_ESCAPE)
	{
		
	}
	else
	{
		event.Skip();
	}
}


void CEnvia::OnKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_F5)
	{	
		wxString nombre;
		int item;
		wxString s;
		int estado = -1;		
		if (!((CDlgMensaje *)GetParent())->m_conferencia)
		{
			item = ListaPorIp(((CDlgMensaje *)GetParent())->m_ip,nombre);
		}		
		((CDlgMensaje *)GetParent())->Oculta(nombre,item,s,estado);
		((CDlgMensaje *)GetParent())->m_padre->EnviaComando(0);
	}
	else if (event.GetKeyCode() == WXK_RETURN)
	{
		((CDlgMensaje *)GetParent())->EnviaMensaje();
		SetInsertionPoint(0);
	}
	else if (event.GetKeyCode() == WXK_ESCAPE)
	{
		Clear();
	}
	else
	{
		event.Skip();
	}
}

CDlgLista::CDlgLista(FI_DEF *fi,char *datos,long ldatos,wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 BOOL comprimido,
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{

	m_modeless = FALSE;
	m_ninfo = 0;

	m_list.m_def = fi;
	m_list.Create(this,1014,wxDefaultPosition,size,wxLC_REPORT|wxLC_SINGLE_SEL);
	{
		DWORD styles = LVS_EX_GRIDLINES ;	
		ListView_SetExtendedListViewStyleEx((HWND)m_list.GetHandle(), styles, styles );
	}
	m_list.Rellena((void *)datos,ldatos,comprimido);

	m_sizer = new wxBoxSizer(wxVERTICAL);    
    m_sizer->Add(&m_list, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);

	m_ok.Create(this, wxID_OK, "Ok");
	m_cancel.Create(this, wxID_CANCEL, "Cancelar");
	m_texto.Create(this,-1,"",wxDefaultPosition,wxSize(100,25),0);
	m_busca.Create(this, 4111 , "Busca");

	m_sizerBottom = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBottom->Add(&m_ok, 0, wxALIGN_LEFT | wxLEFT | wxGROW, 0);
	m_sizerBottom->Add(&m_cancel, 0, wxALIGN_RIGHT | wxRIGHT | wxGROW, 0);	
	m_sizerBottom->Add(&m_texto,1,wxALIGN_RIGHT | wxLEFT | wxGROW, 20);
	m_sizerBottom->Add(&m_busca, 0, wxALIGN_RIGHT | wxLEFT| wxGROW, 20);

	m_sizer->Add(m_sizerBottom,0,wxALIGN_BOTTOM | wxBOTTOM | wxGROW);	

    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);	


}

wxString CDlgLista::SacaCampo(wxString idcampo,int item)
{
	wxString valor;
    wxListItem info;
	char *p;
	for (int i = 0;i < m_list.m_def->f_ncampos;i++)
	{
		p = m_list.m_def->f_campos[i]->c_lista;
		if (*p == 13)
			p++;
		if (!idcampo.Cmp(p))
		{
			info.SetMask(wxLIST_MASK_TEXT);
			info.SetId(item);
			info.SetColumn(i);
			m_list.GetItem(info);
			valor = info.GetText();
			break;
		}
	}
	return valor;
}

void CDlgLista::PonSeleccion()
{
	m_codcli = "";
	int item = m_list.GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if (item >= 0)
	{
		 m_codcli    = SacaCampo("CodCli",item);
		 m_nomcli    = SacaCampo("NomCli",item);
		 m_fecha     = SacaCampo("Fecha",item);
		 m_ticket    = SacaCampo("Ticket",item);
		 m_subticket = SacaCampo("SubTicket",item);
	}
}

void CDlgLista::OnCancel(wxCommandEvent& event)
{
	if (m_modeless)
	{
		Show(FALSE);
		return;
	}
	wxDialog::OnCancel(event);
}

void CDlgLista::OnOK(wxCommandEvent& event)
{
	if (m_modeless)
	{
		m_ndiasinfo++;
		((CDlgPresencia *)GetParent())->EnviaComando(0);
		return;
	}
	PonSeleccion();
	wxDialog::OnOK(event);
}

void CDlgLista::OnBusca(wxCommandEvent& event)
{
	wxString s,s1;
	s = m_texto.GetValue();
	s.MakeUpper();

	long tope = m_list.GetItemCount();
	long pasos = 0;
	long item = m_list.GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if (item < 0)
	{
		item = 0;
	}
	else
	{
		item++;
	}

	int i;
	int nc;
	if (!m_list.m_def)
		nc = m_ninfo;
	else
	    nc = m_list.m_def->f_ncampos;
	wxListItem info;
	while (pasos < tope)
	{
		if (item >= tope)
		{
			item = 0;
		}
		for (i = 0;i < nc;i++)
		{
			info.SetMask(wxLIST_MASK_TEXT);
			info.SetId(item);
			info.SetColumn(i);
			m_list.GetItem(info);
			s1 = info.GetText();			
			s1.MakeUpper();
			if (s1.Find(s) >= 0)
			{
				m_list.SetItemState(item,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
				m_list.EnsureVisible(item);
				pasos = tope;
				break;
			}
		}
		item++;
		pasos++;
	}
	
}

CDlgLista::~CDlgLista()
{	
	/*
	if (m_sizerBottom)
		delete m_sizerBottom;
	*/
	/*
	if (m_sizer)
		delete m_sizer;
	*/
}

CDlgReloj::CDlgReloj(CDlgPresencia* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{	
	
	m_segundo.Create(this,-1,"",wxDefaultPosition,wxSize(20,20),0);
	m_minuto.Create(this,-1,"",wxDefaultPosition,wxSize(20,20),0);

	
	wxStaticText *statico0 = new wxStaticText(this,-1,"Tiempo:",wxDefaultPosition,wxSize(40,25),0);		
	
	m_segundo.SetValue("00");
	m_minuto.SetValue("00");	

	m_sizer = new wxBoxSizer(wxVERTICAL);

	m_sizerTop = new wxBoxSizer(wxHORIZONTAL);

	m_sizerTop->Add(statico0, 0, wxALIGN_CENTER | wxRIGHT, 5);	

	m_sizerTop->Add(&m_minuto, 0, wxALIGN_CENTER | wxRIGHT, 5);	

	m_sizerTop->Add(&m_segundo, 0, wxALIGN_CENTER | wxLEFT, 5);	


	m_sizer->Add(m_sizerTop, 1, wxALIGN_CENTER | wxTOP, 10);
    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);		
}

CDlgReloj::~CDlgReloj()
{
}


CDlgId::CDlgId(CDlgPresencia* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{	
	
	m_codigo.Create(this,-1,"",wxDefaultPosition,wxSize(100,20),0);
	m_codigob.Create(this,-1,"",wxDefaultPosition,wxSize(100,20),0);
	m_usuario.Create(this,-1,"",wxDefaultPosition,wxSize(100,20),0);
	m_password.Create(this,-1,"",wxDefaultPosition,wxSize(100,20),wxTE_PASSWORD);

	m_ok.Create(this, wxID_OK, "Aceptar");
	m_cancel.Create(this, wxID_CANCEL, "Cancelar");
	
	wxStaticText *statico0 = new wxStaticText(this,-1,"Codigos A/B:",wxDefaultPosition,wxSize(100,25),0);
	wxStaticText *statico1 = new wxStaticText(this,-1,"Usuario/Password:",wxDefaultPosition,wxSize(100,25),0);			
	

	m_sizer = new wxBoxSizer(wxVERTICAL);

	m_sizerTop = new wxBoxSizer(wxHORIZONTAL);
	m_sizerMid = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBottom = new wxBoxSizer(wxHORIZONTAL);

	m_sizerTop->Add(statico0, 0, wxALIGN_CENTER | wxRIGHT, 5);	
	m_sizerTop->Add(&m_codigo, 0, wxALIGN_CENTER | wxRIGHT, 5);	
	m_sizerTop->Add(&m_codigob, 1, wxALIGN_CENTER | wxLEFT, 5);	

	m_sizerMid->Add(statico1, 0, wxALIGN_CENTER | wxRIGHT, 5);	
	m_sizerMid->Add(&m_usuario, 0, wxALIGN_CENTER | wxRIGHT, 5);	
	m_sizerMid->Add(&m_password, 1, wxALIGN_CENTER | wxLEFT, 5);	

	m_sizerBottom->Add(&m_ok, 0, wxALIGN_CENTER | wxRIGHT , 10);
	m_sizerBottom->Add(&m_cancel, 0, wxALIGN_CENTER | wxLEFT , 10);	

	m_sizer->Add(m_sizerTop, 0, wxALIGN_CENTER | wxTOP, 10);
    m_sizer->Add(m_sizerMid, 0, wxALIGN_CENTER | wxTOP, 10);
    m_sizer->Add(m_sizerBottom, 1, wxALIGN_CENTER | wxTOP, 10);

    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);		

	m_codigo.SetFocus();
}

void CDlgId::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == &m_ok )
    {
		EndModal(wxID_OK);
	}
	else if ( event.GetEventObject() == &m_cancel )
	{
		EndModal(wxID_CANCEL);
	}
}


CDlgId::~CDlgId()
{
}


CDlgAlarma::CDlgAlarma(CDlgPresencia* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{


	m_ok.Create(this, wxID_OK, "Activar");
	m_cancel.Create(this, wxID_CANCEL, "Cancelar");
	m_hora.Create(this,1113,"",wxDefaultPosition,wxSize(40,25),0);
	m_minuto.Create(this,1115,"",wxDefaultPosition,wxSize(40,25),0);

	
	wxStaticText *statico0 = new wxStaticText(this,-1,"Hora:",wxDefaultPosition,wxSize(40,25),0);
	wxStaticText *statico1 = new wxStaticText(this,-1,"Texto:",wxDefaultPosition,wxSize(40,25),0);

	m_texto.Create(this,-1,"",wxDefaultPosition,wxSize(300,25),0);
	
	m_hora.SetValue(horaalarma);
	m_minuto.SetValue(minutoalarma);
	m_texto.SetValue(textoalarma);		

	m_sizer = new wxBoxSizer(wxVERTICAL);

	m_sizerTop = new wxBoxSizer(wxHORIZONTAL);

	m_sizerTop->Add(statico0, 0, wxALIGN_CENTER | wxRIGHT, 5);	

	m_sizerTop->Add(&m_hora, 0, wxALIGN_CENTER | wxRIGHT, 5);	

	m_sizerTop->Add(&m_minuto, 0, wxALIGN_CENTER | wxLEFT, 5);	

	m_sizerMid = new wxBoxSizer(wxHORIZONTAL);
	m_sizerMid->Add(statico1, 0, wxALIGN_CENTER | wxRIGHT, 5);	
	m_sizerMid->Add(&m_texto, 0, wxALIGN_CENTER | wxLEFT, 5);	

	m_sizerBottom = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBottom->Add(&m_ok, 0, wxALIGN_CENTER | wxRIGHT , 10);
	m_sizerBottom->Add(&m_cancel, 0, wxALIGN_CENTER | wxLEFT , 10);

	m_sizer->Add(m_sizerTop, 1, wxALIGN_CENTER | wxTOP, 10);
	m_sizer->Add(m_sizerMid, 0, wxALIGN_CENTER | wxTOP ,15);
	m_sizer->Add(m_sizerBottom, 0, wxALIGN_CENTER | wxTOP ,15);

    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);	

	m_hora.SetFocus();
	
}

CDlgAlarma::~CDlgAlarma()
{
}

void CDlgAlarma::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == &m_ok )
    {
		struct tm *lc;
		long a = time(NULL);
		lc = localtime( &a );
	    horaalarma = m_hora.GetValue();
		minutoalarma = m_minuto.GetValue();
		textoalarma = m_texto.GetValue();

		lc->tm_hour = atoi(horaalarma);
		lc->tm_min = atoi(minutoalarma);
		lc->tm_sec = 0;

		tiempoalarma = mktime( lc );
		
		EndModal(wxID_OK);
	}
	else if ( event.GetEventObject() == &m_cancel )
	{
		tiempoalarma = 0;
		EndModal(wxID_CANCEL);
	}
}


CDlgParte::CDlgParte(FI_DEF *fi,CDlgPresencia* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{

	m_espedido = FALSE;
	m_escliente = FALSE;

	m_cliente.Create(this,-1,"Cliente");
	m_pedido.Create(this,-1,"Ticket");	
	m_ok.Create(this, wxID_OK, "Ok");
	m_cancel.Create(this, wxID_CANCEL, "Cancelar");
	m_codigo.Create(this,1113,"",wxDefaultPosition,wxSize(100,30),wxTE_READONLY);
	m_nombre.Create(this,1115,"",wxDefaultPosition,wxSize(200,25),wxTE_READONLY);
	m_fecha.Create(this,1116,"",wxDefaultPosition,wxSize(80,25),wxTE_READONLY);
	m_texto.Create(this,1114,"",wxDefaultPosition,wxSize(300,100),wxTE_MULTILINE);
	

	m_sizer = new wxBoxSizer(wxVERTICAL);    

	m_sizerCli = new wxBoxSizer(wxVERTICAL);    

	m_sizerB = new wxBoxSizer(wxHORIZONTAL);

	m_sizerCli->Add(&m_codigo, 0, wxALIGN_CENTER | wxCENTER, 0);	
	m_sizerB->Add(&m_cliente, 0, wxALIGN_CENTER | wxRIGHT , 0);
	m_sizerB->Add(&m_pedido, 0, wxALIGN_CENTER | wxCENTER, 0);
	m_sizerCli->Add(m_sizerB, 0, wxALIGN_BOTTOM | wxBOTTOM, 0);	
	m_sizerCli->Add(&m_fecha, 0, wxALIGN_CENTER | wxCENTER, 0);	
	m_sizerCli->Add(&m_nombre, 0, wxALIGN_CENTER | wxCENTER, 0);


	m_sizerTop = new wxBoxSizer(wxHORIZONTAL);

	m_sizerTop->Add(m_sizerCli, 0, wxALIGN_CENTER | wxCENTER, 0);	

	m_sizerTop->Add(&m_texto, 1, wxALIGN_RIGHT | wxRIGHT | wxGROW, 0);	

	m_sizerBottom = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBottom->Add(&m_ok, 0, wxALIGN_CENTER | wxCENTER , 0);
	m_sizerBottom->Add(&m_cancel, 0, wxALIGN_CENTER | wxCENTER , 0);

	m_sizer->Add(m_sizerTop, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);
	m_sizer->Add(m_sizerBottom, 0, wxALIGN_CENTER | wxCENTER , 0);

    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);	
	
}

void CDlgParte::OnButton(wxCommandEvent& event)
{
	FI_DEF *fi = NULL;
	wxSize size(600,400);	
    if ( event.GetEventObject() == &m_pedido )
    {
		fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\3",fi,1) < 0)
		{
			free((void *)fi);
			wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}		
		if (!xiapre02gz)
		{
			free_def(fi,1);
			wxMessageBox("Atención","Sin Datos O Faltan por Recibir",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}
	    CDlgLista dlg(fi,xiapre02gz,txiapre02gz,this,-1, "Tickets",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString s;
			s = dlg.m_ticket+"-"+dlg.m_subticket;
			m_codigo.SetValue(s);
			m_fecha.SetValue(dlg.m_fecha);
			m_nombre.SetValue(dlg.m_nomcli);
			m_espedido = TRUE;
			m_serie = dlg.m_ticket;
			m_codped = dlg.m_subticket;
			m_codcli = dlg.m_codcli;

		}
    }	
    else if ( event.GetEventObject() == &m_cliente )
    {
		fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\2",fi,1) < 0)
		{
			free((void *)fi);
			wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}		
		if (!d_diapre01)
		{
			lfich_diapre01 = fi->f_lfich+1;
			DescomprimeDatos(diapre01gz,tdiapre01gz,&d_diapre01,&td_diapre01);
		}
		CDlgLista dlg(fi,d_diapre01,td_diapre01,this,-1, "Clientes",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP,FALSE);
		if (dlg.ShowModal() == wxID_OK)
		{
			m_codigo.SetValue(dlg.m_codcli);
			m_fecha.SetValue("");
			m_nombre.SetValue(dlg.m_nomcli);
			m_escliente = TRUE;
			m_serie = "";
			m_codped = "";
			m_codcli = dlg.m_codcli;
		}
    }
	else if ( event.GetEventObject() == &m_ok )
	{
		if (!m_escliente && !m_espedido)
		{
			wxMessageBox("Debe Seleccionar Cliente o Pedido","ATENCION",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}
		if (id_ultimo_parte)
		{
			wxMessageBox("Debe esperar a que el anterior parte se envie","ATENCION",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}
		fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\4",fi,1) < 0)
		{
			free((void *)fi);
			wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}		
		/* Enviar Parte */
		{
			char mensaje[2048];
			char *p;
			int e;
			long yo = dame_the_name();
			int tam;
			long id = time(NULL);
			char *_buf;
			struct   in_addr in;  

			_buf = fi->f_buf;

			p = mensaje;
			e = 9; // Parte
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&id,sizeof(long));
			p += sizeof(long);
			tam = (fi->f_lfich+1);
			memcpy(p,(void *)&tam,sizeof(int));
			p += sizeof(int);
			fi->f_buf = p;
			st_campo(0,fi,"           ");
			st_campo(1,fi,"           ");
			st_campo(2,fi,"           ");
			in.S_un.S_addr = (unsigned long)yo;
			st_campo(3,fi,inet_ntoa(in));
			st_campo(4,fi,(char *)(const char *)m_codcli);
			st_campo(5,fi,(char *)(const char *)m_serie);
			st_campo(6,fi,(char *)(const char *)m_codped);
			st_campo(7,fi,(char *)(const char *)m_texto.GetValue());

			p += tam;
			p--;
			*p = 10;
			p++;
			fi->f_buf = _buf;
			
			tam_ultimo_parte = (int)(p-mensaje);
			id_ultimo_parte = id;
			memcpy(ultimo_parte,mensaje,tam_ultimo_parte);

			tiempoparte = time(NULL) + 30;

			pon_en_cola_envio(mensaje,tam_ultimo_parte,FALSE);
		}
		EndModal(wxID_OK);
	}
	else if ( event.GetEventObject() == &m_cancel )
	{
		EndModal(wxID_CANCEL);
	}
	if (fi)
	{
		free_def(fi,1);
		m_texto.SetFocus();
	}
}


CDlgParte::~CDlgParte()
{
	/*
	if (m_sizerTop)
		delete m_sizerTop;
	if (m_sizerBottom)
		delete m_sizerBottom;
	if (m_sizer)
		delete m_sizer;
	*/
}



BEGIN_EVENT_TABLE(wxdsTextCtrl, wxTextCtrl)     
   EVT_MENU(11111, wxdsTextCtrl::OnTodo)   
   EVT_MENU(11112, wxdsTextCtrl::OnCopiar)   
END_EVENT_TABLE()


void wxdsTextCtrl::ShowLastPosition() // problemas con la implemetacion standard de estas wx
{
    RECT r;

	if (::IsIconic((HWND)GetParent()->GetHandle()))
		return;

	::SendMessage((HWND)GetHandle(),EM_GETRECT,0,(LPARAM)&r);
	POINT p;
	p.x = r.left+1;
	p.y = r.bottom - 2;
	long posc = ::SendMessage((HWND)GetHandle(),EM_CHARFROMPOS,0,(LPARAM)&p);
	long linea = ::SendMessage((HWND)GetHandle(),EM_EXLINEFROMCHAR,0,(LPARAM)posc);
	long total = ::SendMessage((HWND)GetHandle(),EM_GETLINECOUNT,0,0);
	long lineas = total - linea;
	if (lineas > 0)
		::SendMessage((HWND)GetHandle(),EM_LINESCROLL,0,(LPARAM)lineas);
}

void wxdsTextCtrl::OnTodo(wxCommandEvent &event)
{
	SetSelection(0, (long)-1);
}

void wxdsTextCtrl::OnCopiar(wxCommandEvent &event)
{
	Copy();
}


bool wxdsTextCtrl::MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result)
{
MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 

	if ((idCtrl == 3003))  
    {
	   if (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
	   {
		   if (lpMsgFilter->msg == WM_RBUTTONDOWN)
		   {
				 wxPoint point;                                            

				 POINT pp;
				 ::GetCursorPos(&pp);
				 ::ScreenToClient((HWND)GetHandle(),&pp);
				 point = wxPoint(pp.x,pp.y);
     

				 wxMenu menu;     

				 menu.Append(11111,"Seleccionar todo");				 
				 menu.Append(11112,"Copiar");
				 if (!CanCopy())
					 menu.Enable(11112, false);					

				 PopupMenu( &menu, point);

				 return true;
		   }
	   }
	   else if (lpMsgFilter->nmhdr.code == EN_LINK)
	   {
		   ENLINK * lpEnlink = (ENLINK *)lParam; 
		   if (lpEnlink->msg == WM_LBUTTONDOWN)
		   {
			   LONG cpMin;
			   LONG cpMax;
			   cpMin = lpEnlink->chrg.cpMin;
			   cpMax = lpEnlink->chrg.cpMax;			   				   

			   /*
				DWORD dwSize = MAX_PATH;
				TCHAR tchApplicationPath[ MAX_PATH ] = { 0 };				
				HRESULT hr = AssocQueryString( 0,
											ASSOCSTR_EXECUTABLE,
											_T( ".html" ),
											_T( "open" ),
											tchApplicationPath,
											&dwSize );

				if( !FAILED( hr ))
				*/
				{					
				   char parametros[2048];
				   SetSelection(cpMin, cpMax);
				   parametros[0] = 0;
				   ::SendMessage((HWND)GetHandle(),EM_GETSELTEXT,0,(LPARAM)(char *)parametros);
				   ::ShellExecute(NULL, "open",parametros,NULL,NULL, SW_SHOWNORMAL);
				   //::ShellExecute(NULL, "open",tchApplicationPath,parametros,NULL, SW_SHOWNORMAL);
				}
			   return true;
		   }
	   }
     }
	return false;
}


CDlgMensaje::CDlgMensaje(wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name,BOOL conferencia,BOOL general) : 	wxDialog(NULL, id,title,pos,size,style,name)
{
	m_dlgvolume = NULL;
	m_dlgatender = NULL;
	m_explorer = NULL;	
	m_envoz = FALSE;
	m_users = NULL;
	m_conferencia = conferencia;
	m_propietario = NULL;
	m_poractivar = FALSE;
	m_ip = 0;
	m_iptlvoz = 0;
	m_soyyo = false;
	m_general = general; /* neutra o general */
	

	m_padre = (CDlgPresencia *)parent;
	wxPanel *staticbox;
	if (m_conferencia)
	{
		m_users = new CListaUsers();
		m_users->m_padre = (CDlgPresencia *)parent;
		wxSize lsize = GetClientSize();
		lsize.SetHeight(105);
		m_users->Create(this,LIST_CTRL,wxDefaultPosition,lsize,wxLC_ICON | wxLC_SINGLE_SEL	| wxLC_AUTOARRANGE );	
		m_users->DragAcceptFiles(true);
		ListView_SetIconSpacing((HWND)m_users->GetHandle(),70,95);
		m_users->SetSingleStyle(wxLC_ICON);
		m_users->SetImageList(m_users->m_padre->m_imageListNormal, wxIMAGE_LIST_NORMAL);


		m_recibe.Create(this,3003,"",wxDefaultPosition,wxSize(250,200),wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);	
	}
	else
	{		
		if (!general)
		{
			m_recibe.Create(this,3003,"",wxDefaultPosition,wxSize(250,400),wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);
			staticbox = new wxPanel(this,-1,wxDefaultPosition,wxSize(64,64));
			m_imagenestado.Create(staticbox,-1,wxBitmap(),wxDefaultPosition,wxSize(58,58));	
			{
				DWORD ex;
				ex = GetWindowLong((HWND)staticbox->GetHandle(),GWL_EXSTYLE);
				SetWindowLong((HWND)staticbox->GetHandle(),GWL_EXSTYLE,ex | WS_EX_CLIENTEDGE);
				SetWindowPos((HWND)staticbox->GetHandle(),NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
			}
			m_imagenestado.CenterOnParent();
		}
		else
		{
			m_recibe.Create(this,3003,"",wxDefaultPosition,wxSize(450,400),wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);
		}
	}

    LPARAM mask = ::SendMessage((HWND)m_recibe.GetHandle(), EM_GETEVENTMASK, 0, 0);
    ::SendMessage((HWND)m_recibe.GetHandle(), EM_SETEVENTMASK, 0, mask | ENM_LINK | ENM_MOUSEEVENTS);
    ::SendMessage((HWND)m_recibe.GetHandle(), EM_AUTOURLDETECT, true, 0);

	if (!general)
	{
		m_envia.Create(this,3002,"",wxDefaultPosition,wxSize(200,50),wxTE_MULTILINE);
	}
	if (m_conferencia)
	{
		m_ocultar.Create(this, -1, "Fin",wxDefaultPosition,wxSize(-1,18));
	}
	else
	{
		if (!general)
		{
			m_minimizar.Create(this, -1, "Minimizar",wxDefaultPosition,wxSize(-1,18));
			m_ocultar.Create(this, -1, "Ocultar",wxDefaultPosition,wxSize(-1,18));		
		}
		else
		{
			m_ocultar.Create(this, -1, "Ocultar",wxDefaultPosition,wxSize(-1,22));		
			m_ardillas.Create(this, -1, "F.Con.",wxDefaultPosition,wxSize(75,22));
		}

		if (!general)
		{

			m_control.Create(this, -1, "Control",wxDefaultPosition,wxSize(60,18));		
			m_ficheros.Create(this, -1, "Ficheros",wxDefaultPosition,wxSize(60,18));		
			
			m_agrupo.Create(this,-1, "Grupo");
			m_afavoritos.Create(this,-1, "Favoritos");
			m_agrupo.Show(false);
			m_afavoritos.Show(false);
			m_ficheros.Enable(TRUE);
		}
	}
	if (!general)
	{
		m_enviar.Create(this, -1, "Enviar",wxDefaultPosition,wxSize(-1,18));
		m_llamar.Create(this, -1, "Voz",wxDefaultPosition,wxSize(-1,18));
		if (!m_conferencia)
		{
			m_aviso.Create(this,-1, "Aviso");
		}
	}

	m_sizerTop = new wxBoxSizer(wxVERTICAL);    
	if (m_conferencia)
	{
		m_sizerTop->Add(m_users, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);
		m_sizerTop->Add(&m_recibe, 0, wxALIGN_TOP | wxTOP | wxGROW, 0);
	}
	else
	{
		if (!general)
		{
			m_sizerTop->Add(&m_recibe, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);		
		}
		else {
			m_sizerTop->Add(&m_recibe, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);
		}
	}

	m_sizerEnvia = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBoton = new wxBoxSizer(wxVERTICAL);
	if (!general)
	{
		m_sizerImagen = new wxBoxSizer(wxVERTICAL);
		if (!m_conferencia)
		{		
			m_sizerBoton->Add(&m_aviso,1,wxALIGN_BOTTOM | wxBOTTOM| wxGROW,0);
		}
	}

	if (!general)
	{
		m_sizerBoton->Add(&m_enviar,0,wxALIGN_CENTER | wxTOP,1);	
		m_sizerBoton->Add(&m_llamar,0,wxALIGN_CENTER | wxTOP,2);
		if (!m_conferencia)
		{		
			m_sizerBoton->Add(&m_minimizar,0,wxALIGN_CENTER | wxTOP,3);
		}
		m_sizerBoton->Add(&m_ocultar,0,wxALIGN_CENTER | wxTOP,4);
	}
	else {
		m_sizerEnvia->Add(&m_ardillas,0,wxALIGN_LEFT | wxLEFT,20);
		m_sizerEnvia->Add(100,22);
		m_sizerEnvia->Add(&m_ocultar,1,wxALIGN_RIGHT | wxRIGHT,20);
	}
	
	
	if (!m_conferencia)
	{		
		if (!general)
		{
			m_sizerImagen->Add(staticbox,0,wxALIGN_TOP | wxTOP | wxGROW,0);		
			m_sizerImagen->Add(&m_control,0,wxALIGN_CENTER | wxTOP,2);
			m_sizerImagen->Add(&m_ficheros,0,wxALIGN_CENTER | wxTOP,2);
			m_sizerEnvia->Add(m_sizerImagen,0,wxALIGN_LEFT | wxLEFT | wxGROW,0);
		}
	}
	if (!general)
	{
		m_sizerEnvia->Add(&m_envia, 1, wxALIGN_TOP | wxTOP | wxGROW, 1);
		m_sizerEnvia->Add(m_sizerBoton,0,wxALIGN_LEFT | wxLEFT | wxGROW,2);			
		m_sizerTop->Add(m_sizerEnvia,0,wxALIGN_LEFT | wxLEFT | wxGROW);
	}
	else
	{		
		m_sizerTop->Add(m_sizerEnvia,0,wxALIGN_RIGHT | wxRIGHT | wxGROW,2);
		m_ocultar.SetDefault();
	}
	

    SetAutoLayout(TRUE);
    SetSizer(m_sizerTop);
    
	m_sizerTop->SetSizeHints(this);
    m_sizerTop->Fit(this);	

	DragAcceptFiles(true);

}

int EstaEnPantalla(wxPoint pp,wxSize ss)
{
	if (pp.x < 0 || pp.y < 0 || ss.x < 1 || ss.y < 1)
		return 0;
    
    int maxx = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int maxy = ::GetSystemMetrics(SM_CYFULLSCREEN);

	int xx = pp.x+ss.x;
	int yy = pp.y+ss.y;

	if (xx > maxx)
		return 0;

	if (yy > maxy)
		return 0;


	return 1;
}

void CDlgMensaje::OnDropFiles(wxDropFilesEvent& event)
{
	int i;
	
	for (i = 0 ;i  < event.m_noFiles;i++)
	{		
		EnviaFile(event.m_files[i]);
	}
}

void CDlgMensaje::EnviaFile(wxString file)
{	
	if (m_ip == dame_the_name())
	{
		return;
	}
	if (m_explorer && m_explorer->m_directo == INVALID_SOCKET)
	{
		/*
		if (
		&& (m_explorer->m_directo == INVALID_SOCKET || m_explorer->m_recibe))
		*/
		delete m_explorer;
		m_explorer = NULL;
	}
	if (!m_explorer)
	{
		wxString s;
		s = "Trasferir ficheros con " + GetLabel();
		m_explorer = new CDlgExplorer(m_padre,this,m_padre,-1,s,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox",TRUE,FALSE);
		if (m_dlgatender)
		{
			m_explorer->Center();
			m_explorer->Show(TRUE);		
			m_explorer->SetFocus();
		}
		else
		{
			if (m_explorer->m_directo == INVALID_SOCKET)
			{
				delete m_explorer;
				m_explorer = NULL;
				return;
			}
		}
	}
	m_explorer->EnviaFile(file);
}

void CDlgMensaje::CenterOnScreen()
{
	wxDialog::CenterOnScreen();

	DWORD tama;
	DWORD posi;	
	wxPoint pp = GetPosition();
	wxSize  ss = GetSize();
	tama = MAKELONG((WORD)ss.x,(WORD)ss.y);
	posi = MAKELONG((WORD)pp.x,(WORD)pp.y);	

	wxString clave = "Software\\Diagram Software S.L.\\DSP\\POPUPS\\"+GetTitle();

	{
		HKEY hKey;
		LONG retv;		
		DWORD Type;
		DWORD Size;		
		DWORD Disposition;

		
		if (						   /*  */
		  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,(char *)(const char *)clave,0,
						 "valores",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS
			)
		{
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegQueryValueEx(  hKey,"Tamano_a",NULL,&Type,(unsigned char *)&tama,&Size); 

		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegQueryValueEx(  hKey,"Posicion_a",NULL,&Type,(unsigned char *)&posi,&Size); 
		   

		   RegCloseKey(hKey);
		}
	}
	pp.x = LOWORD(posi);
	pp.y = HIWORD(posi);
	ss.x = LOWORD(tama);
	ss.y = HIWORD(tama);
	if (!EstaEnPantalla(pp,ss))
		return;

	SetSize(-1,-1, LOWORD(tama), HIWORD(tama));

	Move((int)LOWORD(posi),(int)HIWORD(posi));	
}


void CDlgMensaje::OnCloseWindow(wxCloseEvent& event)
{
	GuardaPosicion();
}

void CDlgMensaje::GuardaPosicion()
{
	DWORD tama;
	DWORD posi;	
	wxPoint pp = GetPosition();
	wxSize  ss = GetSize();
	tama = MAKELONG((WORD)ss.x,(WORD)ss.y);
	posi = MAKELONG((WORD)pp.x,(WORD)pp.y);	

	wxString clave = "Software\\Diagram Software S.L.\\DSP\\POPUPS\\"+GetTitle();

	if (!IsIconized() && EstaEnPantalla(pp,ss))
	{
		HKEY hKey;
		LONG retv;		
		DWORD Type;
		DWORD Size;		
		DWORD Disposition;

		if (						   /*  */
		  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,(char *)(const char *)clave,0,
						 "valores",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS
			)
		{
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegSetValueEx(  hKey,"Tamano_a",NULL,Type,(unsigned char *)&tama,Size); 

		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegSetValueEx(  hKey,"Posicion_a",NULL,Type,(unsigned char *)&posi,Size); 
		   

		   RegCloseKey(hKey);
		}
	}
}

void CDlgMensaje::SoyYo(bool soy)
{
  m_afavoritos.SetValue(FALSE);
  m_agrupo.SetValue(FALSE);
  if (m_soyyo != soy)
  {
	  if (m_soyyo)
	  {
		  m_sizerImagen->Remove(&m_afavoritos);
		  m_sizerImagen->Remove(&m_agrupo);
		  m_afavoritos.Show(false);
		  m_agrupo.Show(false);
	  }
	  else
	  {
		  m_sizerImagen->Remove(&m_ficheros);
		  m_sizerImagen->Remove(&m_control);
		  m_ficheros.Show(false);
		  m_control.Show(false);
	  }
  }
  else
	  return;
  m_soyyo = soy;
  if (soy)
  {
	  m_agrupo.Show(true);
	  m_afavoritos.Show(true);
	  m_sizerImagen->Add(&m_agrupo,0,wxALIGN_LEFT | wxTOP,2);
	  m_sizerImagen->Add(&m_afavoritos,0,wxALIGN_LEFT | wxTOP,2);      
  }
  else
  {
	  m_ficheros.Show(true);
	  m_control.Show(true);
	  m_sizerImagen->Add(&m_control,0,wxALIGN_CENTER | wxTOP,2);
	  m_sizerImagen->Add(&m_ficheros,0,wxALIGN_CENTER | wxTOP,2);      
  }
  m_sizerImagen->RecalcSizes();	  
}

bool CDlgMensaje::Show(bool show)
{
	if (show)
	{
		PonRuta(m_ip);
	}
	else
	{
		QuitaRuta(m_ip);
	}
    return wxDialog::Show(show);
}

void CDlgMensaje::ActivaVoz()
{
#ifdef CONSKIPE
	llama_a_skype(m_padre,m_ip);
	return;
#else
	m_envoz = TRUE;
	m_llamar.SetLabel("Colgar");
	CDlgVolume *dlgvolume = m_dlgvolume;
	if (dlgvolume)
	{
		dlgvolume->Show(FALSE);
		m_sizerTop->Remove(dlgvolume);					
		m_sizerTop->Layout();
		delete dlgvolume;											
	}
	dlgvolume = new CDlgVolume(this,this,-1,"Volumen",wxPoint(0,0),wxSize(396,145),wxSUNKEN_BORDER);
	m_sizerTop->Add(dlgvolume,0,wxALIGN_LEFT | wxLEFT | wxGROW);
	dlgvolume->Show(TRUE);
	//dlgvolume->Center();										
	m_dlgvolume = dlgvolume;										
	if (GetSize().x < 405)
	{
		SetSize(405,GetSize().y);
	}										
	if (m_conferencia)
	{
		if (GetSize().y < (420+145))
		{
			SetSize(GetSize().x,420+145);
		}
	}
	m_sizerTop->Layout();
#endif
}

void CDlgMensaje::DesactivaVoz()
{
#ifndef CONSKIPE
    m_envoz = FALSE;
	if (m_dlgvolume)
	{					
		m_dlgvolume->Show(FALSE);
		m_sizerTop->Remove(m_dlgvolume);					
		m_sizerTop->Layout();
		delete m_dlgvolume;
		m_dlgvolume = NULL;
	}				
	m_llamar.SetLabel("Voz");
#endif
}


void CDlgMensaje::RepintaTodo()
{
	HWND hija = GetTopWindow((HWND)GetHandle());
	InvalidateRect(hija,NULL,FALSE);
	UpdateWindow(hija);
	while(hija)
	{
		InvalidateRect(hija,NULL,FALSE);
		UpdateWindow(hija);
		hija = GetNextWindow(hija,GW_HWNDNEXT);
	}
}

void CDlgMensaje::SetSize(int x, int y, int width, int height, int sizeFlags)
{
	wxDialog::SetSize(x,y,width,height,sizeFlags);
	RepintaTodo();
}

void CDlgMensaje::SetSize(const wxRect& rect)
{
   wxDialog::SetSize(rect);
   RepintaTodo();   
}

void CDlgMensaje::SetSize(int width, int height)
{
	wxDialog::SetSize(width, height);
	RepintaTodo();	
}

void CDlgMensaje::SetSize(const wxSize& size)
{
	wxDialog::SetSize(size);
	RepintaTodo();	
}

void CDlgMensaje::OnSize(wxSizeEvent& event)
{
	wxDialog::OnSize(event);	
	RepintaTodo();	
}


void CDlgMensaje::OnActivate(wxActivateEvent& event)
{
	if (event.GetActive())
	{		
		::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Activa,m_ip);
	}
	if (!::IsIconic((HWND)GetHandle()))
	{
		m_recibe.ShowLastPosition();
	}
}

void EnviaParam(int cuenta,WPARAM *wparams,LPARAM *lparams)
{ // Envia mensajes de la cola de windows para su analisis en caso de problemas raros
	char mensaje[5100];
	char *p;
	int e;		
	long yo = dame_the_name();	
	int i;
	long t;

	p = mensaje;
	e = 22; // autocomprobacion
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	e = 0; // test-bucle
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&yo,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&cuenta,sizeof(int));
	p += sizeof(int);
	for (i = 0;i < cuenta;i++)
	{
		t = ((long)*wparams) ^ 0x1f1e;
		memcpy(p,(void *)&t,sizeof(long));
		p += sizeof(long);
		t = ((long)*lparams) ^ 0x1f1e;
		memcpy(p,(void *)&t,sizeof(long));
		p += sizeof(long);
		wparams++;
		lparams++;
	}
	pon_en_cola_envio(mensaje,sizeof(int)*3+sizeof(long)+sizeof(long)*cuenta*2,FALSE);
}

void CDlgMensaje::EnviaMensaje()
{
	wxString envia = m_envia.GetValue();
	envia.Trim();
	if (envia.Len() > 0)
	{
		char mensaje[2048];
		char *p;
		int e;
		long yo = dame_the_name();
		int i,j,nips = 0;
		long ips[512];

		if (m_conferencia)
		{			
			nips = m_users->GetItemCount();
			j = 0;
			for (i = 0;i < nips;i++)
			{
				ips[j] = m_users->GetItemData(i);
				if (ips[j] != dame_the_name())
				{
					j++;
				}
			}
			nips = j;
		}
		else if (m_agrupo.GetValue())
		{			
			nips = m_padre->m_tree.m_users->GetItemCount();
			j = 0;
			for (i = 0;i < nips;i++)
			{
				ips[j] = m_padre->m_tree.m_users->GetItemData(i);
				if (ips[j] != dame_the_name())
				{
					j++;
				}
			}
			nips = j;			
		}
		else if (m_afavoritos.GetValue())
		{			
			nips = m_padre->m_listausers.GetItemCount();
			j = 0;
			for (i = 0;i < nips;i++)
			{
				ips[j] = m_padre->m_listausers.GetItemData(i);
				if (ips[j] != dame_the_name())
				{
					j++;
				}
			}
			nips = j;			
		}

		envia.Truncate(1000);

		p = mensaje;
		if (nips)
		{			
			e = 15;
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&yo,sizeof(long));
			p += sizeof(long);
			memcpy(p,(void *)&nips,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)ips,sizeof(long)*nips);
			p += (sizeof(long)*nips);
			*p = 11;
			p++;
			strcpy(p,envia);
			p += (strlen(envia)+1);
			pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long)+sizeof(long)*nips+strlen(envia)+2,FALSE);
		}
		else 
		{
			if (!m_conferencia)
			{
				e = 5; // mensaje
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);
				memcpy(p,(void *)&yo,sizeof(long));
				p += sizeof(long);
				memcpy(p,(void *)&m_ip,sizeof(long));
				p += sizeof(long);
				strcpy(p,envia);	
				p += (strlen(envia)+1);
				pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)*2+strlen(envia)+1,FALSE);
			}
		}		

		m_recibe.SetInsertionPointEnd();		
		//if (!m_conferencia)
			m_recibe.WriteText("\n");
		wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf2\\lang1034\\langfe3082\\langnp1034 "+m_padre->m_minombre;
		if (!m_conferencia)
			text += "\\par}";
		else
			text += ":";
		m_recibe.WriteText(text);
		//envia = auto_escapa(envia);
		m_recibe.WriteText(envia);	
		//if (m_conferencia)
			m_recibe.WriteText("\n");
		m_recibe.ShowLastPosition();
	}

	m_envia.Clear();
	if (::IsWindowVisible((HWND)GetHandle()))
	{
		m_envia.SetFocus();
	}
}

void CDlgMensaje::OnButton(wxCommandEvent& event)
{	
	wxString nombre;
	int item = -1;
	wxString s;
	int estado = -1;
	
	if (!m_conferencia)
	{
		item = ListaPorIp(m_ip,nombre);
		if (item >= 0)
		{
			int flag,privilegio;
			wxString nombre;
			wxString destado;
			int eltipocliente;		
			int departamento;
			EstadoItem(lista_users[item],estado,flag,privilegio,nombre,destado,eltipocliente,departamento);			
		}
	}

	if ( event.GetEventObject() ==  &m_ficheros)
	{	
		if (!m_dlgatender)
		{
			m_padre->EnviaFlag(m_ip,UFLAG_FICHEROS,FALSE);
		}
		else
		{
			if (m_explorer)
			{
				int i;
				for (i = 0;i < MAXENVIOS;i++)
				{
					if (m_explorer->m_envio[i])
						break;
				}
				if (i < MAXENVIOS)
				{
					if (::MessageBox((HWND)GetHandle(),"¿Desea interrumpir los envios en curso?","ATENCION TRANSFERENCIA FICHEROS",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)
					{
						delete m_explorer;
						m_explorer = NULL;
					}
				}
				else
				{
					delete m_explorer;
					m_explorer = NULL;
				}
			}
			if (!m_explorer)
			{
				s = "Trasferir ficheros con " + GetLabel();
				m_explorer = new CDlgExplorer(m_padre,this,m_padre,-1,s,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox");
			}		
			m_explorer->Center();
			m_explorer->Show(TRUE);		
			m_explorer->SetFocus();
		}
	}
	else if ( event.GetEventObject() ==  &m_control )
	{
		if (m_departamento >= 90000 && estado == 4)
		{
			char mensaje[10240];
			char *p;
			int e;		
			long yo = dame_the_name();
			unsigned short comando;
			
			extern int xxxavisar;

			/*xxxavisar = 1;*/			

			
			CDlgAtender*dlgatender = m_dlgatender;
			if (dlgatender)
			{
				dlgatender->Show(FALSE);
				m_sizerTop->Remove(dlgatender);
				m_sizerTop->Layout();
				delete dlgatender;											
			}
			dlgatender = new CDlgAtender(this,this,-1,"Atender",wxPoint(0,0),wxSize(396,50),wxRAISED_BORDER);
			m_sizerTop->Add(dlgatender,0,wxALIGN_LEFT | wxLEFT | wxGROW);
			dlgatender->Show(TRUE);			
			m_dlgatender = dlgatender;										
			if (GetSize().x < 405)
			{
				SetSize(405,GetSize().y);
			}										
			m_sizerTop->Layout();/*Fit(pdlg->m_dlgmensaje[i]);*/

			p = mensaje;
			e = 18; // comando telesoporte
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&m_ip,sizeof(long));
			p += sizeof(long);
			comando = C_HOLA/* | (sizeof(int)+sizeof(long))*/;
			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);

			e = TIPO_OPERADOR;
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);

			memcpy(p,(void *)&yo,sizeof(long));
			p += sizeof(long);	
			
			pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long)*2+sizeof(comando),FALSE);			

		}
		else
		{
			unsigned short puerto;
			long ip = htonl(dame_the_name());
			puerto = 5550 + (ip & 0xff);
			HWND hwnd = ::FindWindow(DAEMON_CLASS_NAME,DAEMON_CLASS_NAME);		
			if (!hwnd)
			{
				 STARTUPINFO stinfo;
				 PROCESS_INFORMATION proi;
				 char tmpx[1024];

				 sprintf(tmpx,"dstvncvw -listen %d",(int)puerto);         
				 memset(&stinfo,0,sizeof(stinfo));
				 stinfo.cb = sizeof(stinfo);
				 stinfo.wShowWindow = SW_SHOWMINIMIZED;
				 CreateProcess(NULL,tmpx,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS,NULL,NULL,&stinfo,&proi);
				 Sleep(500);
			}
			if (m_departamento >= 90000)
			{
				char mensaje[1024];
				char *p;
				int e;		
				long yo = dame_the_name();
				unsigned short comando;

				p = mensaje;
				e = 18; // comando telesoporte
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);
				memcpy(p,(void *)&m_ip,sizeof(long));
				p += sizeof(long);
				comando = C_DATA/* | (sizeof(comando)*2)*/;
				memcpy(p,(void *)&comando,sizeof(comando));
				p += sizeof(comando);
				comando = 0; /* Tipo DATA 0 ... CONTROL!!! */
				memcpy(p,(void *)&comando,sizeof(comando));
				p += sizeof(comando);			
				memcpy(p,(void *)&puerto,sizeof(puerto)); /* puerto */
				p += sizeof(puerto);
				pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)+sizeof(comando)*3,FALSE);

				p = mensaje;
				e = 18; // comando telesoporte 
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);
				memcpy(p,(void *)&m_ip,sizeof(long));
				p += sizeof(long);				
				
				comando = C_DATA;
				memcpy(p,(void *)&comando,sizeof(comando));
				
				p += sizeof(comando);
				comando = 29; // enable input
				memcpy(p,(void *)&comando,sizeof(comando));
				p += sizeof(comando);		
				pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
			}
			else
			{
				m_padre->EnviaFlag(m_ip,UFLAG_CONTROL,FALSE);
			}
		}
	}
	else if ( event.GetEventObject() == &m_minimizar )
	{
		::ShowWindow((HWND)GetHandle(),SW_MINIMIZE);
	}
    else if ( event.GetEventObject() == &m_ardillas ) {				
				long name = dame_the_name();
				char mensaje[256];
				char *p;
				int e;

				p = mensaje;
				e = 122; 
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);

				pon_en_cola_envio(mensaje,sizeof(int),FALSE);
	}
	else if ( event.GetEventObject() == &m_ocultar )
    {		
		if (m_departamento >= 90000)
		{
			s = m_control.GetLabel();
			if (!s.Cmp("Control"))
			{
				if (::MessageBox((HWND)GetHandle(),"Si oculta dejará de antender, ¿Seguro?","ATENCION AL OCULTAR",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)
				{
					Oculta(nombre,item,s,estado);
				}				
			}
			else
				Oculta(nombre,item,s,estado);
		}
		else
			Oculta(nombre,item,s,estado);
    }	
    else if ( event.GetEventObject() == &m_enviar )
    {
		EnviaMensaje();
    }
    else if ( event.GetEventObject() == &m_llamar )
    {		
		s = m_llamar.GetLabel();
		if (m_departamento >= 90000)
		{
				unsigned short puerto;
				long ip = htonl(dame_the_name());
				char mensaje[1024];
				char *p;
				int e;		
				long yo = dame_the_name();
				unsigned short comando;
				
				puerto = 5900 + (ip & 0xff);

				p = mensaje;
				e = 18; // comando telesoporte
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);
				memcpy(p,(void *)&m_ip,sizeof(long));				
				p += sizeof(long);
				comando = C_DATA/* | (sizeof(comando)*2)*/;
				memcpy(p,(void *)&comando,sizeof(comando));
				p += sizeof(comando);
				if (!s.Cmp("Colgar"))
				{
					::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_iptlvoz);
					comando = 27; /* Tipo DATA 27 ... COLGAR!!! */										
					/*
					NO_LLAMADA(m_padre->m_flagq);
					m_padre->EnviaEstado();
					*/
				}
				else
				{
					comando = 26; /* Tipo DATA 26 ... VOZ!!! */
				}
				memcpy(p,(void *)&comando,sizeof(comando));
				p += sizeof(comando);			
				memcpy(p,(void *)&puerto,sizeof(puerto)); /* puerto */
				p += sizeof(puerto);
				*((long *)p) = IpRemotaCodigo(dame_the_name());
				p += sizeof(long);
				pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)*2+sizeof(comando)*3,FALSE);
				DesactivaVoz();
		}
		else if (m_conferencia)
		{
			if (m_propietario == dame_the_name())
			{
				int i,nips = m_users->GetItemCount();
				if (!s.Cmp("Colgar"))
				{
					//m_envoz = FALSE;
					for (i = 0;i < nips;i++)
					{
						if (m_users->GetItemData(i) != dame_the_name())
						{
							m_padre->EnviaFlag(m_users->GetItemData(i),UFLAG_FINVOX,FALSE);
							::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_users->GetItemData(i));
						}
					}			
					DesactivaVoz();
					/*m_llamar.SetLabel("Voz");*/
					/*NO_LLAMADA(m_padre->m_flagq);*/
				}
				else
				{
					int activadas;
					activadas = 0;
					for (i = 0;i < nvoces_activas;i++)
					{
						if (voces_activas[i] != dame_the_name())
						{
							m_padre->EnviaFlag(voces_activas[i],UFLAG_FINVOX,FALSE);
						}
						::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,voces_activas[i]);
					}					
					for (i = 0;i < nips;i++)
					{					
						if (m_users->GetItemData(i) != dame_the_name())
						{
							m_padre->EnviaFlag(m_users->GetItemData(i),UFLAG_CONVOX,FALSE);
							::SendMessage(hwndMDIFrame,WM_COMMAND,MAKEWPARAM(ID_Conectar,0x2),m_users->GetItemData(i));
							activadas++;
						}
					}	
					/*LLAMADA(m_padre->m_flagq);*/
					/*
					if (activadas)
					{
						
					}
					else
					{
						NO_LLAMADA(m_padre->m_flagq);					
					}
					*/
					//m_envoz = TRUE;
					ActivaVoz();
					/*m_llamar.SetLabel("Colgar");*/
				}
				for (i = 0;i < 500;i++)
				{
					wxString s;				
					if (m_padre->m_dlgmensaje[i])						
					{
						s = m_padre->m_dlgmensaje[i]->m_llamar.GetLabel();
						if (!s.Cmp("Colgar"))
						{
							m_padre->m_dlgmensaje[i]->DesactivaVoz();
							/*m_padre->m_dlgmensaje[i]->m_llamar.SetLabel("Voz");*/
						}
						//m_padre->m_dlgmensaje[i]->m_envoz = FALSE;
					}
				}
			}
		}
		else
		{
			if (!m_padre->m_conferencia || !m_padre->m_conferencia->m_envoz)
			{
				if (!s.Cmp("Colgar"))
				{
					/*
					if (nvoces_activas == 1 && voces_activas[0] == m_ip)
					{
						NO_LLAMADA(m_padre->m_flagq);
					}
					*/					
					DesactivaVoz();
#ifndef CONSKIPE					
					/*m_llamar.SetLabel("Voz");*/
					m_padre->EnviaFlag(m_ip,UFLAG_FINVOX,FALSE);
					::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_ip);
#endif
				}
				else
				{	
#ifndef CONSKIPE					
					char flag = 0;
					if ((_userflags[item] & UFLAG_RING))
					{
						_userflags[item] &= ~UFLAG_RING;
						m_padre->EnviaFlag(m_ip,UFLAG_OKVOX,FALSE);
						flag |= 0x1;
					}
					else
					{
						m_padre->EnviaFlag(m_ip,UFLAG_RING,FALSE);
					}
					//m_envoz = TRUE;
					/*LLAMADA(m_padre->m_flagq);*/
					::PostMessage(hwndMDIFrame,WM_COMMAND,MAKEWPARAM(ID_Conectar,flag),m_ip);
#endif
					ActivaVoz();
					/*m_llamar.SetLabel("Colgar");*/
				}
			}
		}
    }
	else
    {
        event.Skip();
    }
	if (::IsWindowVisible((HWND)GetHandle()))
	{

		m_envia.SetFocus();
	}
}

void CDlgMensaje::OnCancel(wxCommandEvent &event)
{	
}

CDlgMensaje::~CDlgMensaje()
{
}

// Tickets

CDlgTicket::CDlgTicket(wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{
	m_poractivar = FALSE;
	m_ip = 0;
	m_recibe.Create(this,3003,"",wxDefaultPosition,wxSize(250,400),wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);	
	m_envia.Create(this,3002,"",wxDefaultPosition,wxSize(200,50),wxTE_MULTILINE);	
	
	m_cliente.Create(this,-1, "Cliente");
	m_ocultar.Create(this, -1, "Ocultar");	
	m_enviar.Create(this, -1, "Enviar");
	m_codigo.Create(this,1113,"",wxDefaultPosition,wxSize(60,25),wxTE_READONLY);
	m_nombre.Create(this,1115,"",wxDefaultPosition,wxSize(200,25),wxTE_READONLY);	

	m_sizerTop = new wxBoxSizer(wxVERTICAL);    
    m_sizerTop->Add(&m_recibe, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);		

	m_sizerEnvia = new wxBoxSizer(wxHORIZONTAL);
	m_sizerBoton = new wxBoxSizer(wxVERTICAL);
	m_sizerCli   = new wxBoxSizer(wxHORIZONTAL);

	m_sizerBoton->Add(&m_cliente,0,wxALIGN_CENTER | wxTOP,0);	
	m_sizerBoton->Add(&m_ocultar,0,wxALIGN_CENTER | wxTOP,2);
	m_sizerBoton->Add(&m_enviar,0,wxALIGN_CENTER | wxTOP,2);	

	m_sizerCli->Add(&m_codigo,0,wxALIGN_CENTER | wxLEFT,0);	
	m_sizerCli->Add(&m_nombre,1,wxALIGN_CENTER | wxLEFT,0);	
	
	m_sizerEnvia->Add(&m_envia, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);
	m_sizerEnvia->Add(m_sizerBoton,0,wxALIGN_LEFT | wxLEFT | wxGROW);

	m_sizerTop->Add(m_sizerCli,0,wxALIGN_LEFT | wxLEFT | wxGROW);
	m_sizerTop->Add(m_sizerEnvia,0,wxALIGN_LEFT | wxLEFT | wxGROW);
	

    SetAutoLayout(TRUE);
    SetSizer(m_sizerTop);
    
	m_sizerTop->SetSizeHints(this);
    m_sizerTop->Fit(this);	
}

void CDlgTicket::EnviaMensaje()
{
	wxString envia = m_envia.GetValue();
	wxString codigo = m_codigo.GetValue();
	wxString nombre = m_nombre.GetValue();
	envia.Trim();
	if (codigo.Len() > 0 && envia.Len() > 0)
	{		
		FI_DEF *fi;

		if (id_ultimo_ticket)
		{
			wxMessageBox("Debe esperar a que el anterior parte se ticket","ATENCION",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}

		envia.Truncate(250);

		fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\5",fi,1) < 0)
		{
			free((void *)fi);
			wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}		
		{
			char mensaje[2048];
			char *p;
			int e;
			long yo = dame_the_name();
			int tam;
			long id = time(NULL);
			char *_buf;
			struct   in_addr in;  

			_buf = fi->f_buf;

			p = mensaje;
			e = 11; // Ticket
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&id,sizeof(long));
			p += sizeof(long);
			tam = (fi->f_lfich+1);
			memcpy(p,(void *)&tam,sizeof(int));
			p += sizeof(int);
			fi->f_buf = p;
			st_campo(0,fi,"           ");
			st_campo(1,fi,"           ");
			st_campo(2,fi,"           ");
			in.S_un.S_addr = (unsigned long)yo;
			st_campo(3,fi,inet_ntoa(in));
			in.S_un.S_addr = (unsigned long)m_ip;
			st_campo(4,fi,inet_ntoa(in));
			st_campo(5,fi,(char *)(const char *)codigo);
			st_campo(6,fi,(char *)(const char *)envia);
			p += tam;
			p--;
			*p = 10;
			p++;
			fi->f_buf = _buf;

			
			tam_ultimo_ticket = (int)(p-mensaje);
			id_ultimo_ticket = id;
			memcpy(ultimo_ticket,mensaje,tam_ultimo_ticket);

			tiempoticket = time(NULL) + 30;

			pon_en_cola_envio(mensaje,tam_ultimo_ticket,FALSE);
		}
		free_def(fi,1);
		
		m_recibe.SetInsertionPoint(m_recibe.GetLastPosition() /*0*/ );
		m_recibe.WriteText("\n");		
		//
		wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf2\\lang1034\\langfe3082\\langnp1034 Ticket Abierto de "+((CDlgPresencia *)GetParent())->m_minombre +" a "+m_destino+"\\par}";
		m_recibe.WriteText(text);
		text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf6\\lang1034\\langfe3082\\langnp1034 Cliente:"+codigo+"-"+nombre+ "\\par}";
		m_recibe.WriteText(text);
		m_recibe.WriteText("\n");
		//envia = auto_escapa(envia);
		m_recibe.WriteText(envia);
		m_recibe.WriteText("\n");
		m_recibe.ShowLastPosition();
	}

	m_envia.Clear();
	m_codigo.Clear();
	m_nombre.Clear();
	if (::IsWindowVisible((HWND)GetHandle()))
	{
		m_envia.SetFocus();
	}
}

void CDlgTicket::OnButton(wxCommandEvent& event)
{
	FI_DEF *fi = NULL;
	wxSize size(600,400);	

    if ( event.GetEventObject() == &m_ocultar )
    {
		Show(FALSE);
    }	
    else if ( event.GetEventObject() == &m_enviar )
    {
		EnviaMensaje();
    }
    else if ( event.GetEventObject() == &m_cliente )
    {
		fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\2",fi,1) < 0)
		{
			free((void *)fi);
			wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
			return;
		}		
		CDlgLista dlg(fi,diapre01gz,tdiapre01gz,this,-1, "Clientes",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
		if (dlg.ShowModal() == wxID_OK)
		{
			m_codigo.SetValue(dlg.m_codcli);
			m_nombre.SetValue(dlg.m_nomcli);
		}		
		m_envia.SetFocus();
    }
	else
    {
        event.Skip();
    }
	if (fi)
	{
		free_def(fi,1);		
	}
	m_envia.SetFocus();
}

void CDlgTicket::OnCancel(wxCommandEvent &event)
{	
}

CDlgTicket::~CDlgTicket()
{
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgPresencia::CDlgPresencia(wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{	
	int i;

	ll = LoadLibrary("iphlpapi.dll");
	if (ll)
	{
		GetIpForwardTable = (__GetIpForwardTable)GetProcAddress(ll,"GetIpForwardTable");
		CreateIpForwardEntry = (__CreateIpForwardEntry)GetProcAddress(ll,"CreateIpForwardEntry");
		DeleteIpForwardEntry = (__DeleteIpForwardEntry)GetProcAddress(ll,"DeleteIpForwardEntry");
	}

	{
		char tmp[256];
		long r = 0;
		int i;
		wxGetApp().SetVendorName("Diagram Software S.L.");
		wxConfig *config = new wxConfig("DSP");
		config->Read("nFavoritos", &r);
		nmis_ips = (int)r;
		for (i = 0;i < nmis_ips;i++)
		{
			sprintf(tmp,"Fav%03d",i);
			config->Read(tmp, &r);
			mis_ips[i] = r;
		}
		delete config;
	}
	

	MENU_ADD_CLIENT_MSG = ::RegisterWindowMessage("WinVNC.AddClient.Message");
	MENU_DISABLE_LOCAL_INPUT = RegisterWindowMessage("WinVNC.DisableInput.Message");
    MENU_ENABLE_LOCAL_INPUT = RegisterWindowMessage("WinVNC.EnableInput.Message");

	mkdir(ajusta_a_home_dir("\\dsp"));
	mkdir(ajusta_a_home_dir("\\dsp\\tmp"));

	m_itemdep = NULL;
	m_itemtut = NULL;
	m_itemlpd = NULL;

	m_refrescar = 0;

	m_tiempomuerte = 0;

	m_conferencia = NULL;

	{
		wxSize size(440,420);	
		m_avisos = new CDlgMensaje(this,-1, "AVISOS" ,wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP,"dialogBox",FALSE,TRUE);
		m_avisos->CenterOnScreen();
	}

	m_partefi = NULL;

	m_antibug = 0;

	m_otraip = 0L;

	m_selip = 0L;	

	memset(m_Passwd,0,sizeof(m_Passwd));
	if (passwordinicial[0] && (!usuarioinicial[0] || dame_the_name() != 0))
	{
		memcpy(m_Passwd,passwordinicial,20);
	}

	wxSize lsize;
	lsize.SetHeight(15);
	lsize.SetWidth(-1);
	m_tcontrol = NULL;
	m_nomolestar.Create(this,-1, "No Molestar",wxDefaultPosition,lsize);
	
	lsize.SetHeight(18);
	m_correo.Create(this, -1, "E-Mail",wxDefaultPosition,lsize);	
	m_ocultar.Create(this, -1, "Ocultar",wxDefaultPosition,lsize);
	m_veravisos.Create(this, -1, "Avisos",wxDefaultPosition,lsize);
#ifndef CLIENTE
	m_ticket.Create(this, -1, "Pasar Llam.",wxDefaultPosition,lsize);
#endif
	m_conferenciar.Create(this, -1, "Conferenciar",wxDefaultPosition,lsize);	

	wxPanel *staticbox = new wxPanel(this,-1,wxDefaultPosition,wxSize(64,64));
	m_imagenestado.Create(staticbox,-1,wxBitmap(),wxDefaultPosition,wxSize(58,58));	
	{
		DWORD ex;
		ex = GetWindowLong((HWND)staticbox->GetHandle(),GWL_EXSTYLE);
		SetWindowLong((HWND)staticbox->GetHandle(),GWL_EXSTYLE,ex | WS_EX_CLIENTEDGE);
		SetWindowPos((HWND)staticbox->GetHandle(),NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
	}
	m_imagenestado.CenterOnParent();
	m_textoestado.Create(this,3000,"",wxDefaultPosition,wxSize(100,50),wxTE_MULTILINE|wxTE_READONLY);
	m_textoestado.SetBackgroundColour(wxColor(210,192,196));

	wxFont ff;
	ff = m_textoestado.GetFont();
	wxFont font(ff.GetPointSize(),ff.GetFamily(),ff.GetStyle(), wxBOLD,ff.GetUnderlined(),ff.GetFaceName(),ff.GetDefaultEncoding());
    m_textoestado.SetFont(font);

	m_tiemposinmovimiento = 0;
	if (m_Passwd[0])
	{
		m_estado = Presente;
		m_estadoq = Presente;
	}
	else
	{
		m_estado = Desconectado;
		m_estadoq = Desconectado;
	}
	m_flagq = 0;
	elflag = 0;
	m_destado = "";
	m_destadoq = "";	

    m_sizerTop = new wxBoxSizer(wxVERTICAL);    

	m_sizerTop->SetMinSize(1,1);	
    
    m_imageListNormal = new wxImageList(58, 58, TRUE);	

	m_imageListPatron = new wxImageList(58, 58, TRUE);

	m_imageListTree = new wxImageList(18, 18, TRUE);

#ifdef OTP
	m_imageListTree->Add(wxBitmap("otp",wxBITMAP_TYPE_BMP_RESOURCE));
#else
#ifdef ADADE
	m_imageListTree->Add(wxBitmap("adade",wxBITMAP_TYPE_BMP_RESOURCE));
#else
	m_imageListTree->Add(wxBitmap("root",wxBITMAP_TYPE_BMP_RESOURCE));
#endif
#endif
	m_imageListTree->Add(wxBitmap("item",wxBITMAP_TYPE_BMP_RESOURCE));
	

	/*
#ifdef __WXMSW__
    m_imageListNormal->Add( wxIcon("icon1", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon2", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon3", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon4", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon6", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon7", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon8", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon2", wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon("icon2", wxBITMAP_TYPE_ICO_RESOURCE) );    

#else
    m_imageListNormal->Add( wxIcon( toolbrai_xpm ) );
    m_imageListNormal->Add( wxIcon( toolchar_xpm ) );
    m_imageListNormal->Add( wxIcon( tooldata_xpm ) );
    m_imageListNormal->Add( wxIcon( toolnote_xpm ) );
    m_imageListNormal->Add( wxIcon( tooltodo_xpm ) );
    m_imageListNormal->Add( wxIcon( toolchec_xpm ) );
    m_imageListNormal->Add( wxIcon( toolgame_xpm ) );
    m_imageListNormal->Add( wxIcon( tooltime_xpm ) );
    m_imageListNormal->Add( wxIcon( toolword_xpm ) );

    
#endif

  */
    
	lsize = GetClientSize();
	lsize.SetHeight(lsize.GetHeight()-210);

	m_splitH.Create(this, ID_MISPLITH,wxDefaultPosition,lsize,wxSP_FULLSASH	|wxSP_LIVE_UPDATE);	
	
	m_listausers.m_padre = this;
	m_listausersg.m_padre = this;
	
	lsize = m_splitH.GetClientSize();

	m_listausers.Create(&m_splitH,LIST_CTRL,wxDefaultPosition,lsize,wxLC_LIST/*wxLC_ICON */| wxLC_SINGLE_SEL	| wxLC_AUTOARRANGE);	
	ListView_SetIconSpacing((HWND)m_listausers.GetHandle(),70,95);
	m_listausers.DragAcceptFiles(true);

	m_splitV.Create(&m_splitH, ID_MISPLITV,wxDefaultPosition,lsize,wxSP_FULLSASH |wxSP_LIVE_UPDATE);
	

	m_splitH.SplitHorizontally(&m_listausers, &m_splitV,115);
	m_splitH.SetMinimumPaneSize(1);

	lsize.SetHeight(105);

	m_tree.m_users = &m_listausersg;

	m_listausersg.Create(&m_splitV,LIST_CTRL,wxDefaultPosition,lsize,wxLC_ICON | wxLC_SINGLE_SEL	| wxLC_AUTOARRANGE );
	ListView_SetIconSpacing((HWND)m_listausersg.GetHandle(),70,95);
	m_listausersg.DragAcceptFiles(true);

	m_listausersg.SetBackgroundColour(wxColor(196,210,255));
	m_splitV.SplitVertically(&m_tree,&m_listausersg,180);
	m_splitV.SetMinimumPaneSize(1);

	m_tree.Create(&m_splitV,ID_TREE,wxDefaultPosition,lsize,wxTR_HAS_BUTTONS);

	m_tree.SetBackgroundColour(wxColor(196,210,255));

	m_tree.SetImageList(m_imageListTree);

	{
		wxTreeItemId root = NULL;
		m_itemdep = m_tree.AppendItem((root = m_tree.AddRoot("ORGANIZACION",0,0,new CItemData(-1))),"{TELESOPORTE CLIENTES}",1,1,new CItemData(99999));
		m_itemtut = m_tree.AppendItem(root,"{TUTORIA}",1,1,new CItemData(91999));
		m_itemlpd = m_tree.AppendItem(root,"{DSLPD}",1,1,new CItemData(92999));
	}
	
    m_sizerTop->Add(&m_splitH, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);	
	
	m_sizerData = new wxBoxSizer(wxHORIZONTAL);	
	m_sizerBoton = new wxBoxSizer(wxVERTICAL);	
	m_sizerImagen = new wxBoxSizer(wxVERTICAL);	
	
	m_sizerImagen->Add(staticbox,0,wxALIGN_CENTER | wxTOP,0);
	m_sizerImagen->Add(&m_nomolestar,0,wxALIGN_CENTER | wxTOP,0);
	m_sizerImagen->Add(&m_veravisos,0,wxALIGN_CENTER | wxTOP,0);	
	m_sizerData->Add(m_sizerImagen,0,wxALIGN_LEFT | wxLEFT| wxGROW,0);
	m_sizerData->Add(&m_textoestado,1,wxALIGN_LEFT | wxLEFT| wxGROW,0);	
	m_sizerBoton->Add(&m_correo,0,wxALIGN_CENTER | wxTOP,0);		
	m_sizerBoton->Add(&m_ocultar,0,wxALIGN_CENTER | wxTOP,0);	
#ifndef CLIENTE
	m_sizerBoton->Add(&m_ticket,0,wxALIGN_CENTER | wxTOP,0);
#endif
	m_sizerBoton->Add(&m_conferenciar,0,wxALIGN_CENTER | wxTOP,2);		
	m_sizerData->Add(m_sizerBoton,0,wxALIGN_CENTER | wxRIGHT);
	m_sizerTop->Add(m_sizerData,0,wxALIGN_LEFT | wxLEFT | wxGROW);

    m_listausers.SetFocus();   

	m_listausers.SetSingleStyle(wxLC_ICON);

	m_listausers.SetImageList(m_imageListNormal, wxIMAGE_LIST_NORMAL);

	m_listausersg.SetSingleStyle(wxLC_ICON);

	m_listausersg.SetImageList(m_imageListNormal, wxIMAGE_LIST_NORMAL);

	pdlg = this;	

	m_privilegio = 0;	

    SetAutoLayout(TRUE);
    SetSizer(m_sizerTop);
    
	m_sizerTop->SetSizeHints(this);
    m_sizerTop->Fit(this);	


	
	tiempoparte = -1;
	tiempoticket = -1;
	m_tiempochunk = -1;
	m_tcolores = 0;
	m_tmenu = 0;
	m_torganiza = 0;
	m_tweb = time(NULL);
	{
		for (i = 0; i < 500;i++)
		{
			m_tiempofoto[i] = 0;
			m_dlgmensaje[i] = NULL;
			m_dlgticket[i] = NULL;
		}
		for (i = 0; i < 500;i++)
		{
			m_imagenes[i] = 0;
			m_faltaimagen[i] = 0;
			m_imaestados[i] = 0xfffffff;
		}
	}
	
	LimpiaDatos();
	CargaLosDatos();

	{
		wxSize size(300,420);	
		for (i =0 ;i < maxpool;i++)
		{
			pooldlgs[i] = new CDlgMensaje(this,-1, "",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP|wxMINIMIZE_BOX);
		}
	}


#ifdef WIN32
	extern HINSTANCE wxhInstance;
	

    NOTIFYICONDATA m_nid;
    HINSTANCE mI = wxhInstance;	

	i_hWnd = CreateDialogParam(  mI,  // handle to application instance
		"WXCAPTIONDIALOG",  // identifies dialog box template
		NULL,      // handle to owner window
		_INCONILLO, // pointer to dialog box procedure
		(LPARAM)NULL);   // initialization value

    hDlgOperacion = (HWND)i_hWnd;
	hwndVolumen = (HWND)i_hWnd;

	par_hWnd = (HWND)GetHandle();


    memset(&m_nid, 0 , sizeof(m_nid));   
    m_nid.cbSize = sizeof(m_nid);   
    m_nid.uID = GetId();
    m_nid.hWnd = i_hWnd;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_MITRAYMESSAGE;
    m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONPRESENTE));
    strncpy(m_nid.szTip,ElEstado[(int)m_estado],sizeof(m_nid.szTip)-1);
	m_nid.szTip[sizeof(m_nid.szTip)-1] = 0;
    
    Shell_NotifyIcon(NIM_ADD,&m_nid);    

#endif

	conectado = 2; /* para que avise ... */
	ActualizaIcono();

	m_mousepoint = wxPoint(0,0);

	m_timer.SetOwner(this,TIMER_ID);

	m_timer.Start(MILISTIMER);

	if (dame_the_name() == 0 && usuarioinicial[0]) {
		if (passwordinicial[0])
			memcpy(m_Passwd,passwordinicial,20);
		PideUser(usuarioinicial);
	}
	m_tcontrol = new  CTcontrol();
	m_tcontrol->Create();
	m_tcontrol->Run();


	DWORD tama;
	DWORD posi;
	DWORD hashp;
	tama = MAKELONG(800,600);
	posi = MAKELONG(0,0);
	hashp = 100;
	{
		HKEY hKey;
		LONG retv;		
		DWORD Type;
		DWORD Size;		
		DWORD Disposition;

		if (						   /*  */
		  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "valores",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS
			)
		{
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegQueryValueEx(  hKey,"Tamano_a",NULL,&Type,(unsigned char *)&tama,&Size); 

		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegQueryValueEx(  hKey,"Posicion_a",NULL,&Type,(unsigned char *)&posi,&Size); 
		   
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegQueryValueEx(  hKey,"Posicion_b",NULL,&Type,(unsigned char *)&hashp,&Size); 

		   RegCloseKey(hKey);
		}
	}

	SetSize(-1,-1, 800, 600);
	m_splitH.SetSashPosition((int)100, TRUE);


	{
		wxPoint pp;
		wxSize ss;
		pp.x = LOWORD(posi);
		pp.y = HIWORD(posi);
		ss.x = LOWORD(tama);
		ss.y = HIWORD(tama);
		if (!EstaEnPantalla(pp,ss))
			CenterOnScreen();
		else
		{
			if (LOWORD(posi) == 0 && HIWORD(posi) == 0)
			{
				CenterOnScreen();
			}
			else
			{
				SetSize(-1,-1, LOWORD(tama), HIWORD(tama));
				m_splitH.SetSashPosition((int)hashp, TRUE);
				Move((int)LOWORD(posi),(int)HIWORD(posi));
			}
		}
	}


	/*
	{
		wxSize size(185,220);

		wxDialog *pdlg;

		wxHtmlWindow *phtml;
		wxBoxSizer *psizer = new wxBoxSizer(wxVERTICAL);    

		pdlg = new wxDialog(NULL,-1, "Prueba",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
		phtml = new wxHtmlWindow(pdlg);
		psizer->Add(phtml, 1, wxALIGN_TOP | wxTOP | wxGROW, 0);

		pdlg->SetAutoLayout(TRUE);
		pdlg->SetSizer(psizer);    
		psizer->SetSizeHints(this);
		psizer->Fit(this);	
		
		pdlg->SetSize(-1,-1, 800, 600);
		pdlg->Centre();
		pdlg->Show(TRUE);

		//phtml->SetPage("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><html><head><title>Precarga</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\"></head><body><p align=\"center\"><font face=\"Verdana, Arial, Helvetica,sans-serif\">Cargando  ...</font></p><form name=\"form1\" method=\"post\"action=\"http://www.alma.in/webinterna/index.php\"><input name=\"usuario\" type=\"hidden\" value=\"CESC\"><input name=\"password\" type=\"hidden\" value=\"C3PO\"> </form></body><script languaje=\"JavaScript\"> document.forms[0].submit();</script></html>");
		phtml->LoadPage("e:/xbox/pcsat/manual/DVB Hacking With Multidec 8_0.htm");
	}
	*/	
}

void CDlgPresencia::GuardaPosicion()
{

	DWORD tama;
	DWORD posi;
	DWORD hashp;
	wxPoint pp = GetPosition();
	wxSize  ss = GetSize();
	tama = MAKELONG((WORD)ss.x,(WORD)ss.y);
	posi = MAKELONG((WORD)pp.x,(WORD)pp.y);
	hashp = m_splitH.GetSashPosition();
	if (!IsIconized() && EstaEnPantalla(pp,ss))
	{
		HKEY hKey;
		LONG retv;		
		DWORD Type;
		DWORD Size;		
		DWORD Disposition;

		if (						   /*  */
		  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "valores",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS
			)
		{
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegSetValueEx(  hKey,"Tamano_a",NULL,Type,(unsigned char *)&tama,Size); 

		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegSetValueEx(  hKey,"Posicion_a",NULL,Type,(unsigned char *)&posi,Size); 
		   
		   Size = sizeof(DWORD);
		   Type = REG_DWORD;				   
		   
		   RegSetValueEx(  hKey,"Posicion_b",NULL,Type,(unsigned char *)&hashp,Size); 

		   RegCloseKey(hKey);
		}
	}

	for (int i = 0; i < 500;i++)
	{
		if (m_dlgmensaje[i])
		{
			m_dlgmensaje[i]->GuardaPosicion();
		}
		//if (m_dlgticket[i])
		//	delete m_dlgticket[i];
	}	
}

CDlgPresencia::~CDlgPresencia()
{

	if (m_conferencia)
		delete m_conferencia;

	delete m_imageListTree;
	delete m_imageListPatron;
	delete m_imageListNormal;
	if (m_sizerBoton)
	{
		delete m_sizerBoton;
	}
	if (m_sizerData)
	{
		delete m_sizerData;
	}
	if (m_sizerTop)
	{
		delete m_sizerTop;
	}
	for (int i = 0; i < 500;i++)
	{
		if (m_dlgmensaje[i])
		{
			delete m_dlgmensaje[i];
		}
		if (m_dlgticket[i])
			delete m_dlgticket[i];
	}
}


void CDlgPresencia::PonEnConferencia(long ip,BOOL borrar)
{	
	m_conferencia->m_users->BorraIP(ip);
	if (!borrar)
	{
		int departamento = 0;		
		int flag;
		int privilegio;
		int eltipocliente;
		unsigned long userflag;
		wxString nombre,destado;

		EstadoPorIp(ip,flag,privilegio,nombre,destado,eltipocliente,departamento,userflag);
		m_conferencia->m_users->CreaItem(nombre,ip,-1,flag,eltipocliente);
	}
	else
	{
		QuitaDeConferencia(1,&ip);
	}
	long ips[512];
	int nips = m_conferencia->m_users->GetItemCount();
	for (int i = 0;i < nips;i++)
	{
		ips[i] = m_conferencia->m_users->GetItemData(i);
	}
	EnviaConferencia(nips,ips);
}


void CDlgPresencia::RepintaTodo()
{
	HWND hija = GetTopWindow((HWND)GetHandle());
	InvalidateRect(hija,NULL,FALSE);
	UpdateWindow(hija);
	while(hija)
	{
		if (hija != (HWND)m_splitH.GetHandle())
		{
			InvalidateRect(hija,NULL,FALSE);
			UpdateWindow(hija);
		}
		hija = GetNextWindow(hija,GW_HWNDNEXT);
	}
}

void CDlgPresencia::SetSize(int x, int y, int width, int height, int sizeFlags)
{
	wxDialog::SetSize(x,y,width,height,sizeFlags);
	RepintaTodo();
	//m_refrescar = 1;	
}

void CDlgPresencia::SetSize(const wxRect& rect)
{
   wxDialog::SetSize(rect);
   RepintaTodo();
   //m_refrescar = 1;	
}

void CDlgPresencia::SetSize(int width, int height)
{
	wxDialog::SetSize(width, height);
	RepintaTodo();
	//m_refrescar = 1;
}

void CDlgPresencia::SetSize(const wxSize& size)
{
	wxDialog::SetSize(size);
	RepintaTodo();
	//m_refrescar = 1;	
}

void CDlgPresencia::OnSize(wxSizeEvent& event)
{
	wxDialog::OnSize(event);	
	RepintaTodo();
	//m_refrescar = 1;	
}

void CDlgPresencia::OnCloseWindow(wxCloseEvent& event)
{
	GuardaPosicion();
#ifdef WIN32	
	extern HINSTANCE wxhInstance;

	{
		if (hwndMDIFrame)
		{
			DestroyWindow(hwndMDIFrame);
		}
	}

	for (int i =poolposicion ;i < maxpool;i++)
	{
		delete pooldlgs[i];
	} 


	SacaDelTray(GetId());

	if (::IsWindow(i_hWnd))
	{
		DestroyWindow(i_hWnd);
	}
#endif
	m_timer.Stop();

	wxDialog::OnCloseWindow(event);		
	wxGetApp().OnExit();
	exit(0);	
}

void CDlgPresencia::CompruebaParte()
{
char tmp[512];
FILE *f;
FI_DEF *fi = NULL;     

     if (m_estado == Desconectado)
		 return;

	 f = fopen(ajusta_a_home_dir("\\dsp\\parte.txt"),"r");
	 if (f)
	 {
		 fi = (FI_DEF *)malloc(sizeof(FI_DEF));
		 if (lee_def("\4",fi,1) < 0)
		 {
			free((void *)fi);
			fclose(f);
			return;
		 }
		 tmp[0] = 0;
		 fgets(tmp,500,f);
		 quitan(tmp);
		 quitab(tmp);
		 if (!tmp[0])
		 {
				long name = dame_the_name();
				char mensaje[256];
				char *p;
				int e;

				p = mensaje;
				e = 12; // fin parte
				memcpy(p,(void *)&e,sizeof(int));
				p += sizeof(int);

				pon_en_cola_envio(mensaje,sizeof(int),FALSE);
		 }
		 else
		 {
			char mensaje[2048];
			char *p;
			int e;
			long yo = dame_the_name();
			int tam;
			long id = time(NULL);
			char *_buf;
			struct   in_addr in;  
			wxString codcli,serie,codped,texto;
			int i,j;
			char *pr;

			pr = tmp;
			j = 0;
			for (i = 0;tmp[i];i++)
			{
				if (tmp[i] == ';' || !tmp[i+1])
				{
					if (tmp[i] == ';')
						tmp[i] = 0;
					switch(j)
					{
					case 0:
						codcli = pr;
						break;
					case 1:
						serie = pr;
						break;
					case 2:
						codped = pr;
						break;
					case 3:
						texto = pr;
						break;
					}
					j++;
					pr = tmp+i+1;
				}
			}

			_buf = fi->f_buf;

			p = mensaje;
			e = 9; // Parte
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&id,sizeof(long));
			p += sizeof(long);
			tam = (fi->f_lfich+1);
			memcpy(p,(void *)&tam,sizeof(int));
			p += sizeof(int);
			fi->f_buf = p;
			st_campo(0,fi,"           ");
			st_campo(1,fi,"           ");
			st_campo(2,fi,"           ");
			in.S_un.S_addr = (unsigned long)yo;
			st_campo(3,fi,inet_ntoa(in));
			st_campo(4,fi,(char *)(const char *)codcli);
			st_campo(5,fi,(char *)(const char *)serie);
			st_campo(6,fi,(char *)(const char *)codped);
			st_campo(7,fi,(char *)(const char *)texto);

			p += tam;
			p--;
			*p = 10;
			p++;
			fi->f_buf = _buf;
			
			tam_ultimo_parte = (int)(p-mensaje);
			id_ultimo_parte = id;
			memcpy(ultimo_parte,mensaje,tam_ultimo_parte);

			tiempoparte = time(NULL) + 30;
			pon_en_cola_envio(mensaje,tam_ultimo_parte,FALSE);
		 }
		 if (fi)
		 {
			free_def(fi,1);
		 }
		 fclose(f);
		 unlink(ajusta_a_home_dir("\\dsp\\parte.txt"));
	 }
}

void CDlgPresencia::OnTimer(wxTimerEvent& event)
{	
	wxPoint p;
#ifdef WIN32
	POINT pp;
    ::GetCursorPos(&pp);
	p = wxPoint(pp.x,pp.y);
#else
	p = wxPoint(0,0);
#endif


	if (m_tiempomuerte && m_tiempomuerte < time(NULL)) {
		wxGetApp().OnExit();
		exit(0);
	}

    if (dame_the_name() == 0 && usuarioinicial[0]) {
		if (tiempologin > 0 && tiempologin < time(NULL))
			PideUser(usuarioinicial);
	}


	CompruebaParte();

	if (en_tiempo_telefono)
	{
		static long atiempo = 0;
		int tiempo = (time(NULL)-en_tiempo_telefono);
		if (!pDlgReloj)
		{
			atiempo = -1;
			wxSize size(100,50);
			pDlgReloj = new CDlgReloj(NULL,-1, "En Llamada",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
			pDlgReloj->Show(TRUE);
		}
		if (pDlgReloj && tiempo != atiempo)
		{
			atiempo = tiempo;
	        pDlgReloj->m_minuto.SetValue(wxString::Format("%02d",tiempo/60));	
			pDlgReloj->m_segundo.SetValue(wxString::Format("%02d",tiempo%60));			
			if (tiempo > 1620)
			{
				FlashWindow((HWND)pDlgReloj->GetHandle(),TRUE);
				FlashWindow((HWND)pDlgReloj->GetHandle(),FALSE);
				MessageBeep(0);
			}
		}		
	}
	else
	{
		if (!en_tiempo_telefono)
		{
			if (pDlgReloj)
			{
				delete pDlgReloj;
				pDlgReloj = NULL;
			}
		}
	}
	if (m_refrescar)
	{
		if (m_refrescar < TIEMPOREFRESCO)
			m_refrescar++;
		else
		{			
			m_refrescar = 0;
			if (::IsWindowVisible((HWND)GetHandle()))
			{
				Show(FALSE);
				Show(TRUE);
				/*
				InvalidateRect((HWND)GetHandle(),NULL,TRUE);
				UpdateWindow((HWND)GetHandle());
				*/
			}
		}
	}

	if (idle_aviso > 0 && idle_aviso < (1+(7*FACTORTIMER)))
	{
		switch(idle_aviso)
		{
		case 1:			
			FlashIcono(Presente);
		break;
		case 1+(1*FACTORTIMER):
			FlashIcono(Desconectado);
		break;
		case 1+(2*FACTORTIMER):
			FlashIcono(m_estado);			
			MessageBeep(0);
			FlashIcono(Presente);
		break;
		case 1+(3*FACTORTIMER):
			FlashIcono(Desconectado);
		break;
		case 1+(4*FACTORTIMER):
			FlashIcono(m_estado);
			MessageBeep(0);
			FlashIcono(Presente);
		break;
		case 1+(5*FACTORTIMER):
			FlashIcono(Desconectado);
		break;
		case 1+(6*FACTORTIMER):
			FlashIcono(m_estado);
		break;
		}
		idle_aviso++;
	}
	else
		idle_aviso = 0;

	PonPosicion(p);

	if (!m_conferencia)
	{
		wxString titulo;
		wxSize size(420,420);
		titulo = "CONFERENCIAR";		
		m_conferencia = new CDlgMensaje(this,-1, titulo ,wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP|wxMINIMIZE_BOX,"dialogBox",TRUE);
	}

	if (esta_conectado())
	{
		if ((elflag & FLAG_LLAMADA))
		{
			if (!nvoces_activas)
				NO_LLAMADA(m_flagq);
		}
		else
		{
			if (nvoces_activas)
				LLAMADA(m_flagq);			
		}
		if (m_conferencia && !m_conferencia->m_propietario && (elflag & FLAG_ENCONF))
		{
			NO_ENCONF(m_flagq);
		}
		if (m_conferencia && !m_conferencia->m_propietario && (elflag & FLAG_CONFAMO))
		{
			NO_CONFAMO(m_flagq);
		}
		EnviaEstado();
	}


	RellenaEstados();
	
	if (m_torganiza <= time(NULL))
	{
		m_torganiza = time(NULL)+10;
		PideInfo("organizacion",TRUE);
	}
	if (m_tcolores <= time(NULL))
	{
		m_tcolores = time(NULL)+5;
		PideInfo("colores",TRUE);
	}
	if (m_tmenu <= time(NULL))
	{
		m_tmenu = time(NULL)+5;
		PideInfo("menus",TRUE);
	}
	if (!lMiWeb && !m_minombre.IsEmpty())
	{
		NMiWeb = m_minombre + "WEB";
		MiWeb = "::" + NMiWeb;
		MiWebUrl = "";
		lMiWeb = MiWeb.Len();
	}
	if (m_tweb && lMiWeb && m_tweb <= time(NULL))
	{
		m_tweb = time(NULL)+5;
		PideInfo(NMiWeb,TRUE);
	}

	if (web_tmp[0] && tiempo_web <= time(NULL))
	{
		unlink(web_tmp);
		web_tmp[0] = 0;
	}

	if (m_Passwd[0] && recibido_estados)	
	{
		long ip;
		char mensaje[256];
		char *p;
		int e;
		//int pedidas;

		//pedidas = 0;		
		for (int i = 0;i < 500;i++)
		{
			if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_dlgvolume)
			{
				m_dlgmensaje[i]->m_dlgvolume->EnviaRVolumens();
			}
			if ( (m_faltaimagen[i]&0xf) && m_tiempofoto[i] <= time(NULL))
			{				
				ip = m_imagenes[i];
				m_tiempofoto[i] = time(NULL)+30;
				//if (ip != dame_the_name())
				{
					p = mensaje;
					e = 6; // foto
					memcpy(p,(void *)&e,sizeof(int));
					p += sizeof(int);
					memcpy(p,(void *)&ip,sizeof(long));
					p += sizeof(long);
					if ((m_faltaimagen[i]&0xf) == 2)
					{
						e = m_faltaimagen[i]; 
						m_faltaimagen[i] = 0x10;
						m_tiempofoto[i] = 0;
					}
					else
					{
						e = 0; // control foto
					}
					memcpy(p,(void *)&e,sizeof(int));
					p += sizeof(int);
					pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long),FALSE);
				}				
				//pedidas++;
				//if (pedidas >= 10)
				//	break;
			}			
		}
	}

	if (m_tiempochunk > -1 && m_tiempochunk <= time(NULL))	
	{
		PideChunk();
	}

	if (id_ultimo_parte && tiempoparte > -1 && tiempoparte <= time(NULL))
	{
		tiempoparte = time(NULL) + 30;
		pon_en_cola_envio(ultimo_parte,tam_ultimo_parte,FALSE);
	}

	if (id_ultimo_ticket && tiempoticket > -1 && tiempoticket <= time(NULL))
	{
		tiempoticket = time(NULL) + 30;
		pon_en_cola_envio(ultimo_ticket,tam_ultimo_ticket,FALSE);
	}

	if (tiempoalarma && tiempoalarma <= time(NULL))
	{
		char tmpx[512];
		char *p1;

		tiempoalarma = 0;

		p1 = tmpx;

		*(long *)p1 = dame_the_name();
		p1 += sizeof(long);
		*(long *)p1 = dame_the_name();						
		p1 += sizeof(long);
		strcpy(p1,"Alarma a las ");
		strcat(p1,horaalarma);
		strcat(p1,":");
		strcat(p1,minutoalarma);
		strcat(p1,"   ");
		strcat(p1,textoalarma);		

		EsMensaje(tmpx);
		MessageBeep(0);
		MessageBeep(0);
		MessageBeep(0);
		MessageBeep(0);
	}
}

void CDlgPresencia::Movimiento(bool avisar)
{
	if (m_tiemposinmovimiento > MINTIEMPOQUIETO)
	{
		if (m_estado == Inactivo)
		{
			idle_aviso = 0;
			m_estadoq = Presente;
			m_destadoq = "";
			Actualiza();	
		}
		else
		{
			if (m_estado != Presente)
			{
				if (avisar)
				{
					if (!idle_aviso)
					{
						char tmpx[512];
						char *p1;
						p1 = tmpx;

						*(long *)p1 = dame_the_name();
						p1 += sizeof(long);
						*(long *)p1 = dame_the_name();						
						p1 += sizeof(long);
						strcpy(p1,"Recordatorio de estado: ");
						strcat(p1,m_destado);

						EsMensaje(tmpx);
					}					
					idle_aviso = 1;					
				}
			}
		}
	}
	m_tiemposinmovimiento = 0;
}

void CDlgPresencia::PonPosicion(wxPoint &p)
{
	if ((p.x != m_mousepoint.x || p.y != m_mousepoint.y))
	{		
		m_mousepoint = p;
		Movimiento(TRUE);
	}
	else
		m_tiemposinmovimiento++;
	if (m_tiemposinmovimiento > MAXTIEMPOQUIETO)
	{		
		if (m_estado == Presente)
		{
			m_estadoq = Inactivo;
			m_destadoq = "";
			Actualiza();
		}		
	}
}

void CDlgPresencia::FlashIcono(enum ElEstado est)
{
#ifdef WIN32
	extern HINSTANCE wxhInstance;
	

    NOTIFYICONDATA m_nid;
    HINSTANCE mI = wxhInstance;	
	par_hWnd = (HWND)GetHandle();

    memset(&m_nid, 0 , sizeof(m_nid));   
    m_nid.cbSize = sizeof(m_nid);   
    m_nid.uID = GetId();
    m_nid.hWnd = i_hWnd;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_MITRAYMESSAGE;
	switch (est)
	{
	case Presente:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONPRESENTE));				
		break;	
	case Inactivo:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONINACTIVO));
		break;
	default:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONAUSENTE));				
		break;
	}    
	strcpy(m_nid.szTip,"");
    Shell_NotifyIcon(NIM_MODIFY,&m_nid);
#endif	 
}

void CDlgPresencia::ActualizaIcono()
{
	if (m_destado.IsEmpty())
	{
		m_destado = ElEstado[(int)m_estado];
	}
#ifdef WIN32
	extern HINSTANCE wxhInstance;
	

    NOTIFYICONDATA m_nid;
    HINSTANCE mI = wxhInstance;	
	par_hWnd = (HWND)GetHandle();

    memset(&m_nid, 0 , sizeof(m_nid));   
    m_nid.cbSize = sizeof(m_nid);   
    m_nid.uID = GetId();
    m_nid.hWnd = i_hWnd;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_MITRAYMESSAGE;
	switch (m_estado)
	{
	case Presente:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONPRESENTE));				
		break;	
	case Inactivo:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONINACTIVO));
		break;
	default:
		m_nid.hIcon = ::LoadIcon(mI,MAKEINTRESOURCE(IDI_ICONAUSENTE));				
		break;
	}    
	strncpy(m_nid.szTip,m_destado,sizeof(m_nid.szTip)-1);
	m_nid.szTip[sizeof(m_nid.szTip)-1] = 0;
    Shell_NotifyIcon(NIM_MODIFY,&m_nid);
#endif	 
}



void CDlgPresencia::Ardillas()
{
#ifdef WIN32
	/*
	extern HINSTANCE wxhInstance;
	

    NOTIFYICONDATA m_nid;
    memset(&m_nid, 0 , sizeof(m_nid));   
	m_nid.cbSize=sizeof(NOTIFYICONDATA);
    m_nid.uID = GetId();
    m_nid.hWnd = i_hWnd;
	m_nid.uFlags = NIF_INFO;
	m_nid.uTimeout = 5000;
	m_nid.dwInfoFlags = NIIF_WARNING;
	strncpy(m_nid.szInfo,"ARDILLAS",sizeof(m_nid.szInfo)-1);
	m_nid.szInfo[sizeof(m_nid.szInfo)-1] = 0;
	strncpy(m_nid.szInfoTitle,"ARDILLAS",sizeof(m_nid.szInfoTitle)-1);
	m_nid.szInfoTitle[sizeof(m_nid.szInfoTitle)-1] = 0;
	Shell_NotifyIcon(NIM_MODIFY, &m_nid);
*/
#endif
		IRichEditOle* pRichEditOle = NULL;		
		m_avisos->m_recibe.SetInsertionPoint(m_avisos->m_recibe.GetLastPosition() /*0*/ );

		m_avisos->m_recibe.WriteText("\n");
		wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf6\\lang1034\\langfe3082\\langnp1034 Ardillas:}";
		m_avisos->m_recibe.WriteText(text);			
		text = auto_escapa("Ardillas");//pon_rtflinks(mensaje);
		text = "{\\rtf1\\cf0\\b "+ text +"}";
		m_avisos->m_recibe.WriteText(text);
		m_avisos->m_recibe.WriteText("\n");
		m_avisos->m_recibe.ShowLastPosition();
		m_avisos->Show(TRUE);

}

void CDlgPresencia::SwitchEstado()
{   	
	if (m_antibug || dentro > 1)
		return;
	wxMenu menu,*smenu1,*smenu2,*smenu3;
	wxPoint p(0,0);
#ifdef WIN32
	POINT pp;
    ::GetCursorPos(&pp);

	wxRect rr = GetRect();
	pp.x -= rr.x;
	pp.y -= rr.y;
	p = wxPoint(pp.x,pp.y);
#endif


    menu.Append(2000, "Entrada");
	menu.Append(-1, "");

	smenu1 = new wxMenu();
	smenu2 = new wxMenu();
	smenu3 = new wxMenu();
	smenu3->Append(1111,"Vacaciones");
	smenu3->Append(1112,"Enfermedad");
	smenu3->Append(1113,"Otros");
	

	int ii = 0;
	while(e_trabajo[ii][0])
	{
		smenu1->Append(1001+ii,e_trabajo[ii]);
		ii++;
	}
	ii = 0;
	while(e_ocio[ii][0])
	{
		smenu2->Append(1051+ii,e_ocio[ii]);
		ii++;
	}

	menu.Append(7000,"Ausente Trabajo", smenu1 );
	menu.Append(7001,"Ausente Ocio", smenu2 );
	menu.Append(7002,"Ausente Otros", smenu3 );

	

	menu.Append(-1, "");
#ifndef CLIENTE
	menu.Append(1114,"Inicio Tarea");
	menu.Append(1117,"Fin Tarea");
	menu.Append(1115,"Alarma");
	menu.Append(1116,"Web");
	menu.Append(-1, "");
#endif
	menu.Append(1110, "Salida");
	menu.Append(-1, "");    
	menu.Append(1120, "Password");	
	menu.Append(2001, "Identificación");


	if (EnBarraTareas)
	{
		menu.Append(-1, "");
		menu.Append(1101, "Modo Ventana");
	}
	else
	{
#ifndef CLIENTE
		menu.Append(-1, "");
		menu.Append(1101, "Modo Barra");
#endif
#ifdef WIN32
		if (!IsWindowVisible(par_hWnd))
		{
			menu.Append(-1, "");
			menu.Append(1100, "Mostrar");
		}	
#endif
	}
	

	menu.Append(-1, "");
	menu.Append(IDCANCEL, "Fin Programa");


	m_antibug = 2;
	PopupMenu( &menu, p);
	if (m_antibug == 2)
	{
		PopupMenu( &menu, p);
	}
	m_antibug = 0;	
}

void CDlgPresencia::OtroEstado(int item,CListaUsers *plista)
{
	if (m_antibug || dentro > 1)
		return;

	if (dame_the_name() == plista->GetItemData(item))
	{
		if (m_privilegio >= 20)
		{
			wxMenu menu;
			if ((plista->m_tipos[item] & TIPO_OCULTO))
			{
				menu.Append(2102,"Desactivar Ocultacion");
			}
			else
			{
				menu.Append(2101,"Activar Ocultacion");
			}			

			wxPoint p(0,0);
#ifdef WIN32
			POINT pp;
			::GetCursorPos(&pp);

			wxRect rr = GetRect();
			pp.x -= rr.x;
			pp.y -= rr.y;
			p = wxPoint(pp.x,pp.y);
#endif
			PopupMenu( &menu, p);
		}
		return;
	}

    if (m_privilegio < 10)
	{        
		return;
	}

	wxMenu menu,*smenu1,*smenu2,*smenu3;
	wxPoint p(0,0);
#ifdef WIN32
	POINT pp;
    ::GetCursorPos(&pp);

	wxRect rr = GetRect();
	pp.x -= rr.x;
	pp.y -= rr.y;
	p = wxPoint(pp.x,pp.y);
#endif


	/*
    menu.Append(6000, "Entrada");
	menu.Append(-1, "");
	*/

	smenu1 = new wxMenu();
	smenu2 = new wxMenu();
	smenu3 = new wxMenu();
	smenu3->Append(5111,"Vacaciones");
	smenu3->Append(5112,"Enfermedad");
	smenu3->Append(5113,"Otros");	


	int ii = 0;
	while(e_trabajo[ii][0])
	{
		smenu1->Append(5001+ii,e_trabajo[ii]);
		ii++;
	}
	ii = 0;
	while(e_ocio[ii][0])
	{
		smenu2->Append(5051+ii,e_ocio[ii]);
		ii++;
	}

	menu.Append(7000,"Ausente Trabajo", smenu1 );
	menu.Append(7001,"Ausente Ocio", smenu2 );
	menu.Append(7002,"Ausente Otros", smenu3 );

	menu.Append(-1, "");
	menu.Append(5110, "Salida");
	
	
	if (EnBarraTareas)
	{
		menu.Append(-1, "");
		menu.Append(1101, "Modo Ventana");
	}
	else
	{
		menu.Append(-1, "");
		menu.Append(1101, "Modo Barra");
#ifdef WIN32
		if (!IsWindowVisible(par_hWnd))
		{
			menu.Append(-1, "");
			menu.Append(1100, "Mostrar");
		}	
#endif
	}

	m_otraip = plista->GetItemData(item);
	m_antibug = 2;
	PopupMenu( &menu, p);
	if (m_antibug == 2)
	{
		PopupMenu( &menu, p);
	}
	m_antibug = 0;	
}

void CDlgPresencia::InfoItem(int item,CListaUsers *plista)
{
	wxString txt = plista->GetItemText(item);
	wxString nombre;
	long ip = plista->GetItemData(item);

	int pi = txt.Find(_charseparador);
	if (pi > -1)
	{
		nombre = txt.Mid(0,pi);
	}
	txt.Replace("\n",": ");
	m_textoestado.SetValue(txt);	
	PideInfo(nombre,FALSE,ip);
}

void CDlgPresencia::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == &m_ocultar )
    {
#ifdef WIN32
		::ShowWindow(par_hWnd,SW_HIDE);
#endif
    }	
    else if ( event.GetEventObject() == &m_correo )
    {
		EnviaCorreo();
    }
    else if ( event.GetEventObject() == &m_veravisos )
    {
		m_avisos->Show(TRUE);
	}
#ifndef CLIENTE
	else if ( event.GetEventObject() == &m_ticket )
	{
		if (m_selip && m_privilegio > 0)
		{
			ActivaTicket(m_selip,m_selNombre);
		}
	}
#endif
	else if ( event.GetEventObject() == &m_conferenciar )
	{
		if (m_conferencia)
		{
			if (!m_conferencia->m_propietario)
			{					
				m_conferencia->m_propietario = dame_the_name();
				
				m_conferencia->m_recibe.SetValue("");
				m_conferencia->m_recibe.SetInsertionPoint( m_conferencia->m_recibe.GetLastPosition() /*0*/ );
				wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf2\\lang1034\\langfe3082\\langnp1034 Arrastra ligeramente con el boton derecho sobre la persona a incluir en la conferencia.\\cf0\\par\n\n}";
				m_conferencia->m_recibe.WriteText(text);
				m_conferencia->m_recibe.ShowLastPosition();
				
				m_conferencia->m_llamar.Enable(true);				
				wxString nombre;

				long ip = m_conferencia->m_propietario;
				PonEnConferencia(ip);
				m_conferencia->Show(TRUE);	
				CONFAMO(m_flagq);
			}
			::BringWindowToTop((HWND)m_conferencia->GetHandle());
			::SetForegroundWindow((HWND)m_conferencia->GetHandle());
			m_conferencia->m_envia.SetFocus();
		}
	}	
	else
    {
        event.Skip();
    }
}

void CDlgPresencia::BorraIp(char *p)
{
	unsigned long ip = *((unsigned long *)p);

	for (int i = 0;i < 500;i++)
	{
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)ip)
		{
			wxString nombre;
			int item;
			wxString s;
			int estado = -1;		
			if (!m_dlgmensaje[i]->m_conferencia)
			{
				item = ListaPorIp(m_dlgmensaje[i]->m_ip,nombre);
			}		
			m_dlgmensaje[i]->Oculta(nombre,item,s,estado);			
			delete m_dlgmensaje[i];
			m_dlgmensaje[i] = NULL;
		}
	}

	if (m_otraip == (long)ip)
	{
		m_otraip = 0L;
	}
	if (m_selip == (long)ip)
	{
		PonSelIp(0L);		
	}

	m_listausers.BorraIP(ip);
	m_listausersg.BorraIP(ip);

	int estado1,flag1,privilegio1,tipo1,dep1;
	wxString nombre1,destado1;
	

	for (i = 0;i < nlista_users;i++)
	{
		if (*((unsigned long *)lista_users[i]) == ip)
		{
            EstadoItem(lista_users[i],estado1,flag1,privilegio1,nombre1,destado1,tipo1,dep1);
			free(lista_users[i]);
			nlista_users--;
			for (;i < nlista_users;i++)
			{
				lista_users[i] = lista_users[i+1];
				ipvoz[i] = ipvoz[i+1];
				codigovoz[i] = codigovoz[i+1];
			}
			if (dep1 >= 90000 && dep1 < 90999)
			{
				m_tree.BuscaSubdepartamento(m_itemdep,dep1,TRUE);
			}
			if (dep1 >= 91000 && dep1 < 91999)
			{
				m_tree.BuscaSubdepartamento(m_itemtut,dep1,TRUE);
			}
			if (dep1 >= 92000 && dep1 < 92999)
			{
				m_tree.BuscaSubdepartamento(m_itemlpd,dep1,TRUE);
			}
			break;
		}
	}	
}

void CDlgPresencia::CambiaIp(char *p)
{
	unsigned long ip = *((unsigned long *)p);
	unsigned long ip_new = *((unsigned long *)(p+sizeof(long)));	

	if (m_otraip == (long)ip)
	{
		m_otraip = ip_new;
	}
	m_listausers.CambiaIP(ip,ip_new);
	m_listausersg.CambiaIP(ip,ip_new);

	for (int i = 0;i < 500;i++)
	{
		if (m_imagenes[i] == (long)ip)
		{
			m_imagenes[i] = (long)ip_new;
		}
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)ip)
		{
			m_dlgmensaje[i]->m_ip = (long)ip_new;
		}
		if (m_dlgticket[i] && m_dlgticket[i]->m_ip == (long)ip)
		{
			m_dlgticket[i]->m_ip = (long)ip_new;
		}
	}
	for (i = 0;i < nlista_users;i++)
	{
		if (*((unsigned long *)lista_users[i]) == ip)
		{			
			*((unsigned long *)lista_users[i]) = ip_new;
			SacaIpVoz(lista_users[i],i);
			break;
		}
	}
	if (m_selip == (long)ip)
	{
		PonSelIp(ip_new);
	}
	// Conferencia?
}

void CDlgPresencia::PonSelIp(long ip)
{
	m_selip = ip;
	if (!m_selip)
	{
		m_imagenestado.SetBitmap(wxBitmap());
		return;
	}
	int departamento = 0;		
	int flag;
	int privilegio;
	int eltipocliente;
	unsigned long userflag;
	wxString nombre,destado;
	int estado = EstadoPorIp(ip,flag,privilegio,nombre,destado,eltipocliente,departamento,userflag);
	if (estado > -1)
	{
		int imagen = DameImagen(ip,estado,flag,eltipocliente);
		wxBitmap bmp1(58,58);	
		if (imagen > -1)
		{	        
			wxMemoryDC temp_dc;				
			temp_dc.SelectObject(bmp1);				
			m_imageListNormal->Draw(imagen,temp_dc,0,0,wxIMAGELIST_DRAW_NORMAL,TRUE);				
		}
		m_imagenestado.SetBitmap(bmp1);
	}
}

void CDlgPresencia::ActualizaImagenMensaje(char *p,BOOL esip)
{
	long ip;
	wxString nombre;
	if (esip)
	{
		ip = (long )p;
		int item = ListaPorIp(ip,nombre);
		if (item < 0)
			return;
		p = lista_users[item];
	}
	else
	    ip = *((long *)p);
	for (int i = 0;i < 500;i++)
	{
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)ip)
		{			
			int departamento;
			int flag;
			int privilegio;
			int eltipocliente;			
			wxString destado;
			int estado;
			EstadoItem(p,estado,flag,privilegio,nombre,destado,eltipocliente,departamento);
			if (estado > -1)
			{
				int imagen = DameImagen(ip,estado,flag,eltipocliente);
				wxBitmap bmp1(58,58);	
				if (imagen > -1)
				{	        
					wxMemoryDC temp_dc;				
					temp_dc.SelectObject(bmp1);				
					m_imageListNormal->Draw(imagen,temp_dc,0,0,wxIMAGELIST_DRAW_NORMAL,TRUE);				
				}
				if (!m_dlgmensaje[i]->m_general)
					m_dlgmensaje[i]->m_imagenestado.SetBitmap(bmp1);
			}			
			break;
		}
	}
}

void CDlgPresencia::PonFavorito(long ip)
{
	wxString nombre;
	if (m_listausers.PorIp(ip,nombre) >= 0)
		return;
	int item = ListaPorIp(ip,nombre);
	if (item >= 0)
	{
		char *p;
		wxString _destado;
		int _prevestado = 0;
		int _elprivilegio = 0;
		p = lista_users[item];
		m_listausers.PonEstado(&p,&_elprivilegio,&_destado,&_prevestado,_userflags[item]);
	}
	int i;
	for (i = 0;i < nmis_ips;i++)
	{
		if (mis_ips[i] == ip)
			break;
	}
	mis_ips[i] = ip;
	if (i == nmis_ips)
		nmis_ips++;
	char tmp[256];
    wxGetApp().SetVendorName("Diagram Software S.L.");
	wxConfig *config = new wxConfig("DSP");
	config->Write("nFavoritos", (long)nmis_ips);
	for (i = 0;i < nmis_ips;i++)
	{
		sprintf(tmp,"Fav%03d",i);
		config->Write(tmp, (long)mis_ips[i]);
	}
	delete config;
}

void CDlgPresencia::QuitaFavorito(long ip)
{
	wxString nombre;
	if (m_listausers.PorIp(ip,nombre) < 0)
		return;
	m_listausers.BorraIP(ip);
	int i;
	for (i = 0;i < nmis_ips;i++)
	{
		if (mis_ips[i] == ip)
		{
			nmis_ips--;
			for (;i < nmis_ips;i++)
				mis_ips[i] = mis_ips[i+1];
			break;
		}
	}
	char tmp[256];
	wxGetApp().SetVendorName("Diagram Software S.L.");
	wxConfig *config = new wxConfig("DSP");
	config->Write("nFavoritos", (long)nmis_ips);
	for (i = 0;i < nmis_ips;i++)
	{
		sprintf(tmp,"Fav%03d",i);
		config->Write(tmp, (long)mis_ips[i]);
	}
	delete config;
}

void CDlgPresencia::PonEstados(char *p,int *elprivilegio,int modo)
{
	bool cambia = false;
	int n = 1;
	extern int xxxavisar;	


	if (modo != 0 && modo != 17)
		return;

	if (modo == 17)
	{
		n = *((int *)p);
		p += sizeof(int);
	}
	for (int xx = 0;xx < n;xx++)
	{
		unsigned long ip = *((unsigned long *)p);
		int estado = *((int *)(p+sizeof(long)));
		int prevestado = 0;
		wxString destado;
		wxString nombre = p+sizeof(long)+sizeof(int)*2;
		int item  = -1;		
		int item2 = -1;
		int item3 = -1;
		long vieja_ip = 0;
		CListaUsers *plista = NULL;		
		CListaUsers *plista2 = NULL;		
		CListaUsers *plista3 = NULL;		
		int i;
		int departamento = 0;
		int flag = estado & 0xffff0000;
		flag >>= 16;
		estado &= 0xffff;

		{
			int k;
			if ((k = nombre.Find(1)) > -1)
			{				
				nombre.Truncate(k);
			}
		}

		i = ListaPorNombre(nombre);
		if (i >= 0)
		{
			prevestado = *((int *)(lista_users[i]+sizeof(long)));
			vieja_ip = *((unsigned long *)lista_users[i]);
			cambia = ListaCambiaEstado(lista_users[i],p,destado,departamento);
			free(lista_users[i]);
			if (ip != (unsigned long)vieja_ip)
			{
				cambia = true;
				if (m_selip == vieja_ip)
					m_selip = ip;
			}
		}
		else
		{
			i = nlista_users;
			ListaCambiaEstado(p,p,destado,departamento);
			
			
			if (getenv("SINDSPTELESOPORTE") && departamento == 99999 || (departamento >= 90000 && departamento < 90999))
			{
				p += SizeEstado(p);
				continue;
			}
			
			cambia = true;
			_userflags[i] = 0;
			nlista_users++;
		}
		lista_users[i] = (char *)malloc(SizeEstado(p));
		memcpy(lista_users[i],p,SizeEstado(p));
		SacaIpVoz(p,i);
		
		if (departamento >= 90000 && departamento < 90999 && !m_tree.BuscaSubdepartamento(m_itemdep,departamento,FALSE))
		{			
			wxString descdep = p+sizeof(long)+sizeof(int)*2;
			
			int k;
			k = descdep.Find(1);
			if (k > -1)
			{				
				descdep = descdep.Mid(k+1);
			}
			else
			{
				descdep = wxString::Format("Departamento %02d",departamento-90000);
			}			
			m_tree.AppendItem(m_itemdep,descdep,1,1,new CItemData(departamento));

			::InvalidateRect((HWND)m_tree.GetHandle(),NULL,FALSE);
		}

		if (departamento >= 91000 && departamento < 91999 && !m_tree.BuscaSubdepartamento(m_itemtut,departamento,FALSE))
		{			
			wxString descdep = p+sizeof(long)+sizeof(int)*2;
			
			int k;
			k = descdep.Find(1);
			if (k > -1)
			{				
				descdep = descdep.Mid(k+1);
			}
			else
			{
				descdep = wxString::Format("Departamento %02d",departamento-91000);
			}			
			m_tree.AppendItem(m_itemtut,descdep,1,1,new CItemData(departamento));

			::InvalidateRect((HWND)m_tree.GetHandle(),NULL,FALSE);
		}

		if (departamento >= 92000 && departamento < 92999 && !m_tree.BuscaSubdepartamento(m_itemlpd,departamento,FALSE))
		{			
			wxString descdep = p+sizeof(long)+sizeof(int)*2;
			
			int k;
			k = descdep.Find(1);
			if (k > -1)
			{				
				descdep = descdep.Mid(k+1);
			}
			else
			{
				descdep = wxString::Format("Departamento %02d",departamento-92000);
			}			
			m_tree.AppendItem(m_itemlpd,descdep,1,1,new CItemData(departamento));

			::InvalidateRect((HWND)m_tree.GetHandle(),NULL,FALSE);
		}
		
		
		if (xxxavisar == 2)
			::MessageBox(NULL,"4","punto 4",MB_OK);

		if (elprivilegio && *((long *)p) == dame_the_name())
		{
			if (*elprivilegio != *((int *)(p+sizeof(long)+sizeof(int))))
			{
				int i;
				*elprivilegio = *((int *)(p+sizeof(long)+sizeof(int)));
				for (i = 0;i < 500;i++)
				{
					if (!(m_faltaimagen[i]&0xf) && m_imagenes[i])
					{
						PonImagenEstado(i);
					}
				}
			}

			if (xxxavisar == 2)
				::MessageBox(NULL,"4-1","punto 4-1",MB_OK);

			m_minombre = nombre;
			m_estado = (enum ElEstado)estado;
			m_destado = destado;
			elflag = flag;
			if (xxxavisar == 2)
				::MessageBox(NULL,"4-2","punto 4-2",MB_OK);
			ActualizaIcono();			
			if (xxxavisar == 2)
				::MessageBox(NULL,"4-3","punto 4-3",MB_OK);


			if (midepartamento == -1)
			{
				m_tree.SeleccionaDepartamento(departamento);
			}
			midepartamento = departamento;
		}
		if (xxxavisar == 2)
			::MessageBox(NULL,"5","punto 5",MB_OK);


		{			
			item = m_listausers.PorNombre(nombre,vieja_ip);
			if (item >= 0)				
			   plista = &m_listausers;
			item2 = m_listausersg.PorNombre(nombre,vieja_ip);
			if (item2 >= 0)				
			   plista2 = &m_listausersg;
			if (m_conferencia)
			{
				item3 = m_conferencia->m_users->PorNombre(nombre,vieja_ip);
				if (item3 >= 0)				
				   plista3 = m_conferencia->m_users;
			}
		}

		
		if (plista && item > 0 && (long)ip != vieja_ip)
		{
			plista->BorraIP(vieja_ip);
		}		
		if (plista2 && item2 > 0 && (long)ip != vieja_ip)
		{
			plista2->BorraIP(vieja_ip);
		}		
		
		{
			char *_p = p;
			wxString _destado;
			int _prevestado = 0;
			int _elprivilegio = 0;
			int j;
			for (j = 0;j < nmis_ips;j++)
			{
				if (mis_ips[j] == *((long *)p))
				{
					break;
				}
			}			
			if (j < nmis_ips)
			{
				m_listausers.PonEstado(&_p,&_elprivilegio,&_destado,&_prevestado,_userflags[i]);
			}
		}

		if (m_tree.EstaIncluida(ip))
		{
			char *_p = p;
			wxString _destado;
			int _prevestado = 0;
			int _elprivilegio = 0;

			m_listausersg.PonEstado(&_p,&_elprivilegio,&_destado,&_prevestado,_userflags[i]);
		}
		if (plista3 && item3 >= 0)
		{
			char *_p = p;
			wxString _destado;
			int _prevestado = 0;
			int _elprivilegio = 0;
			plista3->PonEstado(&_p,&_elprivilegio,&_destado,&_prevestado,0);
		}		


		if (cambia)
		{
			if (m_selip == *((long *)p))
			{
				wxString txt;
				txt = nombre;
				txt += ": ";
				txt += destado;			
				txt.Replace("\n",": ");
				m_textoestado.SetValue(txt);	
				PideInfo(nombre,FALSE,m_selip);
				PonSelIp(m_selip);
			}
			ActualizaImagenMensaje(p);
		}

		p += SizeEstado(p);


		if (cambia && estado != (int)Inactivo && prevestado != (int)Inactivo)
		{
			char tmpx[512];
			char *p1;
			for (int i = 0;i < 500;i++)
			{
				if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)ip && m_dlgmensaje[i]->m_aviso.GetValue())
				{
							{
								p1 = tmpx;

								*(long *)p1 = ip;
								p1 += sizeof(long);
								*(long *)p1 = dame_the_name();						
								p1 += sizeof(long);
								strcpy(p1,destado);

								EsMensaje(tmpx);
							}
				}
			}
		}
	}


}


void CDlgPresencia::CambioParte(unsigned char *p)
{
	char tmpx[512];
	char *p1;
	long ip;
	int l;
	/*
	FI_DEF *fi;

	fi = (FI_DEF *)malloc(sizeof(FI_DEF));
	if (lee_def("\4",fi,1) < 0)
	{
		free((void *)fi);
		fi = NULL;
	}
	*/


	ip = *((long *)p);
	p += sizeof(long);
	l = *((long *)p);
	p += sizeof(long);

	for (int i = 0;i < 500;i++)
	{
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)ip && m_dlgmensaje[i]->m_aviso.GetValue())
		{
					{
						p1 = tmpx;

						*(long *)p1 = ip;
						p1 += sizeof(long);
						*(long *)p1 = dame_the_name();						
						p1 += sizeof(long);
						strcpy(p1,"PARTE: ");
						p1 += strlen(p1);

						memcpy(p1,p+36,6); // cliente
						p1 += 6;
						*p1 = ' ';
						p1++;
						sprintf(p1,"%05.0lf-",*((double*)(p+42)));
						p1 += strlen(p1);
						sprintf(p1,"%04d ",((unsigned int)p[50])*256+(unsigned int)p[51]);
						p1 += strlen(p1);
						memcpy(p1,p+52,70);
						p1 += 70;
						*p1 = 0;
						quitab(tmpx+sizeof(long)*2);
						EsMensaje(tmpx);
					}
		}
	}
	/*
	if (fi)
		free_def(fi,1);	
		*/
}

void CDlgPresencia::RellenaEstados()
{		
	int size;	
	char *p;	
	char *p1,*p2,*p3;	
	int lparte;
	char tmp[2100];
	extern int xxxavisar;
	DWORD noquedarse = ::GetTickCount();
	static int ultima_i_ = 0;


	if (dentro)
	{
		return;
	}

	dentro++;
	while((::GetTickCount()-noquedarse) < 50)
	{		
		if (!(p = saca_de_cola_local(&size)))
		{
			break;
		}
		if (xxxavisar)
		{
			char xxx[128];
			sprintf(xxx,"sacado de %d tipo = %d",size,*(int *)p);
			::MessageBox(NULL,xxx,"Ojo!",MB_OK);		
			if ((*(int *)p) == 0)
			{
				xxxavisar = 2;
			}
		}
		if (size > sizeof(int))
		{
			if ((*(int *)p) == 23) {
				p1 = p + sizeof(int);
				if (usuarioinicial[0] && !strcmp(p1,usuarioinicial)) {
					tiempologin = 0;
					p1 += (strlen(p1)+1);
					PonTheName(*(long *)p1);
					m_estado = Presente;
					m_destado = ElEstado[(int)m_estado];
					m_estadoq = Presente;
					m_destadoq = ElEstado[(int)m_estadoq];
					Actualiza();
				}
			}
			else if (!(*(int *)p) || (*(int *)p) == 17)
			{				
				PonEstados(p+sizeof(int),&m_privilegio,(*(int *)p));
				recibido_estados = 1;
				if (accediocon[0]) {
					tmp[0] = 0;
					lee_accesos(tmp);
					if (!strstr(tmp,accediocon) && (strlen(tmp)+strlen(accediocon)+3) < 2048) {
						if (tmp[0])
							strcat(tmp,"\n");
						strcat(tmp,accediocon);
						pon_accesos(tmp);
					}
					pon_ultimo_acceso(accediocon);
					accediocon[0] = 0;
				}
			}
			else if ((*(int *)p) == 1)
			{
				p1 = p + sizeof(int);
				if (strlen(p1) > 13 && !memcmp(p1,"::organizacion",14))
				{					
					RellenaDepartamentos(p1);
				}
				else if (strlen(p1) > 8 && !memcmp(p1,"::trabajo",9))
				{					
					ProcesaMenu(p1);
				} else if (strlen(p1) > 8 && !memcmp(p1,"::colores",9))
				{
					PonColores(p1);				
				} else if (lMiWeb && (int)strlen(p1) >= lMiWeb && !memcmp(p1,MiWeb,lMiWeb))
				{
					MiWebUrl = p1+lMiWeb;
					m_tweb = 0;
				}
				else
				{
					p2 = p1;
					p3 = NULL;
					lparte = 0;
					if (!memcmp(p1,"\3\2\2\3",4))
					{
						p2 += 4;
						lparte = *((int *)p2);
						p2 += sizeof(int);
						p3 = p2;
						p2 += lparte;
						p1 = p2;
					}
					if (!m_nombreinfo.IsEmpty() && strlen(p1) > m_nombreinfo.Len() && !memcmp(p1,m_nombreinfo,m_nombreinfo.Len()))
					{
						p1 += m_nombreinfo.Len();
						m_textoestado.AppendText("\n");
						p2 = p1 + strlen(p1);
						p2--;
						while(p2 > p1)
						{
							if (*p2 == 10 || *p2 == 13)
							{
								*p2 = 0;
							}
							else
								break;
							p2--;
						}
						m_textoestado.AppendText(p1);
						if (p3 && lparte)
						{
							if (!m_partefi && ltdiapre03mas && ltdiapre03mas == tdiapre03mas)
							{
								m_partefi = (FI_DEF *)malloc(sizeof(FI_DEF));
								if (lee_def("\4",m_partefi,1) < 0)
								{
									free((void *)m_partefi);
									m_partefi = NULL;
								}
							}
							if (m_partefi)
							{
								m_textoestado.AppendText("\n");
								p2 = m_partefi->f_buf;
								m_partefi->f_buf = p3;

								ld_campo(1,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText(" ");
								ld_campo(2,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText(" [");
								ld_campo(4,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText("] [");
								ld_campo(5,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText("-");
								ld_campo(6,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText("] [");
								ld_campo(7,m_partefi,tmp);
								m_textoestado.AppendText(tmp);
								m_textoestado.AppendText("]");

								m_partefi->f_buf = p2;
							}
						}
					}
				}
			}
			else if ((*(int *)p) == 2)
			{
				p1 = p + sizeof(int);
				EsMensaje(p1);
			}
			else if ((*(int *)p) == 3)
			{
				p1 = p + sizeof(int);
				EsFoto(p1,size-sizeof(int));
			}
			else if ((*(int *)p) == 4)
			{
				p1 = p + sizeof(int);
				NoHayFoto(p1);
			}
			else if ((*(int *)p) == 5)
			{
				p1 = p + sizeof(int);
				NuevosDatos(p1);
			}
			else if ((*(int *)p) == 6)
			{
				p1 = p + sizeof(int);
				RecibeChunk(p1);
			}
			else if ((*(int *)p) == 7)
			{
				p1 = p + sizeof(int);
				if (id_ultimo_parte == *((long *)p1))
				{
					id_ultimo_parte = 0;
					tiempoparte = -1;
				   // Ok parte
				}
			}
			else if ((*(int *)p) == 8)
			{
				p1 = p + sizeof(int);
				if (id_ultimo_ticket == *((long *)p1))
				{
					id_ultimo_ticket = 0;
					tiempoticket = -1;
				   // Ok ticket
				}
			}
			else if ((*(int *)p) == 9)
			{
				p1 = p + sizeof(int);
				CambiaIp(p1);
			}
			else if ((*(int *)p) == 10)
			{
				p1 = p + sizeof(int);
				BorraIp(p1);
			}
			else if ((*(int *)p) == 11)
			{
				long control;

				p1 = p + sizeof(int);
				control = *(long *)p1;
				p1 += sizeof(long);				
				if (control > controlmio)
				{
					ltxiapre02gz  = 0L;
					txiapre02gz = *(long *)p1;
					p1 += sizeof(long);
					if (xiapre02gz)
					   free(xiapre02gz);
					xiapre02gz  = (char *)malloc(txiapre02gz);
					controlmio = control;										
                    if (m_tiempochunk <= 0 || m_tiempochunk >= time(NULL))	
					{
						PideChunk();
					}					
				}				
				{
					/* enviar validacion */
					long name = dame_the_name();
					char mensaje[256];
					char *p;
					int e;

					p = mensaje;
					e = 13; // codigo mensaje (validacion datos)
					memcpy(p,(void *)&e,sizeof(int));
					p += sizeof(int);
					*((long *)p) = control;
					p += sizeof(long);	
					pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long),FALSE);
				}
			}
			else if ((*(int *)p) == 12)
			{
				p1 = p + sizeof(int);
				EsFlag(p1);
			}
			else if ((*(int *)p) == 13)
			{
				p1 = p + sizeof(int);
				EsConferencia(p1); // Mensaje de Conferencia
			}
			else if ((*(int *)p) == 14)
			{
				p1 = p + sizeof(int);
				CreaConferencia(p1); // Asignar Conferencia				
			}
			else if ((*(int *)p) == 15)
			{
				p1 = p + sizeof(int);
				QuitaConferencia(p1); // Asignar Conferencia				
			}
			else if ((*(int *)p) == 16) // Fichero gordo :-)
			{
				p1 = p + sizeof(int);
				fichero_gordo(p1);
			}
			else if ((*(int *)p) == 18 || (*(int *)p) == 180) // comando dswc
			{
				p1 = p + sizeof(int);
				if ((*(int *)p) == 180)
				{
					/* primero se asigna al pooldeconexiones */
					wxSize size(300,420);

					for (int i = 0;i < maxpoolc;i++)
					{
						if (!conexionesdlgs[i])
						{
							conexionesdlgs[i] = new CDlgMensaje(this,-1, "",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP|wxMINIMIZE_BOX,"dialogBox",FALSE,TRUE);							
						}
						if (!conexionesdlgs[i]->m_explorer)
						{
							wxString ss;
							ss = "Trasferir ficheros";
							conexionesdlgs[i]->m_explorer = new CDlgExplorer(this,conexionesdlgs[i],this,-1,ss,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox",TRUE,TRUE);
						}
						if (conexionesdlgs[i]->m_explorer->m_directo != INVALID_SOCKET)
							continue;
						
						conexionesdlgs[i]->m_explorer->m_directo = *(SOCKET *)p1;
						if (WSAAsyncSelect(*(SOCKET *)p1,(HWND)conexionesdlgs[i]->m_explorer->GetHandle(), WM_SOCKET_SELECT, FD_READ|FD_WRITE) == 0) 
						{
							conexionesdlgs[i]->m_explorer->m_asincrono = 1;
						}
						else
						{
							conexionesdlgs[i]->m_explorer->m_asincrono = 0;
						}
						break;
					}

					/* si no ... */
					if (i >= maxpoolc)
					{
						/* se asigna al primer libre ... luego al recibir la info de conexion,
						   la clase m_explorer ya se encarga de "pasarselo" a la instancia correspondiente */
						for (i = 0;i < 500;i++)
						{
							if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_explorer && m_dlgmensaje[i]->m_explorer->m_directo == INVALID_SOCKET)
							{							
								m_dlgmensaje[i]->m_explorer->m_directo = *(SOCKET *)p1;							
								if (WSAAsyncSelect(*(SOCKET *)p1,(HWND)m_dlgmensaje[i]->m_explorer->GetHandle(), WM_SOCKET_SELECT, FD_READ|FD_WRITE) == 0) 
								{
									m_dlgmensaje[i]->m_explorer->m_asincrono = 1;
								}
								else
								{
									m_dlgmensaje[i]->m_explorer->m_asincrono = 0;
								}
								break;
							}
						}
						if (i >= 500)
						{
							for (i = 0;i < 500;i++)
							{
								if (m_dlgmensaje[i] && !m_dlgmensaje[i]->m_dlgatender && !m_dlgmensaje[i]->m_explorer)
								{
									wxString ss;
									ss = "Trasferir ficheros";
									m_dlgmensaje[i]->m_explorer = new CDlgExplorer(this,m_dlgmensaje[i],this,-1,ss,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox",TRUE,TRUE);
									m_dlgmensaje[i]->m_explorer->m_directo = *(SOCKET *)p1;
									if (WSAAsyncSelect(*(SOCKET *)p1,(HWND)m_dlgmensaje[i]->m_explorer->GetHandle(), WM_SOCKET_SELECT, FD_READ|FD_WRITE) == 0) 
									{
										m_dlgmensaje[i]->m_explorer->m_asincrono = 1;
									}
									else
									{
										m_dlgmensaje[i]->m_explorer->m_asincrono = 0;
									}
									break;
								}
							}
							if (i >= 500) /* a fer la mar */
							{
								closesocket(*(SOCKET *)p1);
							}
						}
					}
				}
				else
				{					
					long ip = *(long *)p1;
					p1 += sizeof(long);
					if (m_dlgmensaje[ultima_i_] && m_dlgmensaje[ultima_i_]->m_ip ==ip)
					{							
						if (!m_dlgmensaje[ultima_i_]->m_explorer || m_dlgmensaje[ultima_i_]->m_explorer->Recibe((Paquete *)p1) == -1)
						{
							if (!m_dlgmensaje[ultima_i_]->m_dlgatender || m_dlgmensaje[ultima_i_]->m_dlgatender->Recibe((Paquete *)p1) == -1)
							{
								if (m_dlgmensaje[ultima_i_]->m_dlgvolume)
								{
									m_dlgmensaje[ultima_i_]->m_dlgvolume->Recibe((Paquete *)p1);									
								}
							}
						}						
					}
					else for (int i = 0;i < 500;i++)
					{
						if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip ==ip)
						{	
							ultima_i_ = i;
							if (!m_dlgmensaje[ultima_i_]->m_explorer || m_dlgmensaje[ultima_i_]->m_explorer->Recibe((Paquete *)p1) == -1)
							{
								if (!m_dlgmensaje[ultima_i_]->m_dlgatender || m_dlgmensaje[ultima_i_]->m_dlgatender->Recibe((Paquete *)p1) == -1)
								{
									if (m_dlgmensaje[ultima_i_]->m_dlgvolume)
									{
										m_dlgmensaje[ultima_i_]->m_dlgvolume->Recibe((Paquete *)p1);									
									}
								}
							}
							break;
						}
					}
				}
			}
			else if ((*(int *)p) == 19) // Informacion especial
			{
				p1 = p + sizeof(int);
				InformacionEspecial(p1);
			}
			else if ((*(int *)p) == 20)
			{
				p1 = p + sizeof(int);
				CambioParte((unsigned char *)p1);
			}
			else if ((*(int *)p) == 22)
			{
				p1 = p + sizeof(int);
				/* comprobar si todo es correcto */
			}
			else if ((*(int *)p) == 122)
			{
				p1 = p + sizeof(int);
				/* Ardilla */
				Ardillas();
				/*globo*/
				
				m_tiempomuerte = time(NULL) + 5;
			}
		}
		free((void *)p);		
	}    

	wxColor newcolor,oldcolor = m_listausers.GetBackgroundColour();
	if (!esta_conectado())
	{		
		if (conectado)
		{
			idle_aviso = 1;
		}
		conectado = 0;
		newcolor.Set(255,0,0);		
		/*
		FlashIcono(Presente);
		Sleep(200);		
		FlashIcono(Desconectado);
		Sleep(200);		
		ActualizaIcono();
		*/
	}
	else
	{
		if (conectado != 1)
		{
			envia_estado();
			conectado = 1;
			idle_aviso = 0;
			m_tcolores = time(NULL)+10;
			m_tmenu =  m_tcolores;
			m_torganiza = m_tcolores;
			PideInfo("organizacion",TRUE);
            PideInfo("menus",TRUE);
			PideInfo("colores",TRUE);
			m_estadoq = m_estado;
			m_destadoq = m_destado;
			EnviaEstado();						
		}
		newcolor.Set(196,210,238);
	}
	if (envia_estado())
	{
		newcolor.Set(196,210,238);
		conectado = 1;
		m_estadoq = m_estado;
		m_destadoq = m_destado;
		EnviaEstado();					
	}
	if (newcolor != oldcolor)
	{
		m_listausers.SetBackgroundColour( newcolor );
		InvalidateRect((HWND)m_listausers.GetHandle(),NULL,TRUE);	
		InvalidateRect((HWND)m_listausersg.GetHandle(),NULL,FALSE);	
	}
	dentro--;
}

void CDlgPresencia::EnviaEstado()
{
	static int nofirst = 0;
	static int tiempo = 0;
	if (!conectado)
	{
		return;
	}
	if (!esta_conectado())
	{
		nofirst = 0;
		return;
	}

	long name = dame_the_name();
	char mensaje[256];
	char *p;
	int e;

	if (!name)
		return;

	tiempo++;
	if (m_destadoq.IsEmpty() && !m_flagq)
	{		
		if (tiempo > TIEMPOTIMEOUT)
		{
			tiempo = 0;
			p = mensaje;
			e = 1; // codigo mensaje (ping)
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			pon_en_cola_envio(mensaje,sizeof(int),FALSE);
		}
		return;
	}
	tiempo = 0;
	if (m_destadoq.IsEmpty())
	{
		m_destadoq = m_destado;
	}
	nofirst = 1;
	tiempo = 0;
	
	if (!m_flagq)
	{
		m_flagq = elflag;
	}
	p = mensaje;

	e = 0; // codigo mensaje
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,m_Passwd,20);
	p += 20;
	e = tipo_cliente|control_dspversion; // ver y modo de programa
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&name,sizeof(long));
	p += sizeof(long);
	e = (int)m_estadoq | ((m_flagq << 16) & 0xffff0000);
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	strcpy(p,m_destadoq);	
	p += (strlen(m_destadoq)+1);
	*((long *)p) = dame_the_localip();

	m_flagq = 0;

	pon_en_cola_envio(mensaje,sizeof(long)*2+sizeof(int)*3+strlen(m_destadoq)+21,FALSE);


	idle_aviso = 0;

	if (m_estadoq == Desconectado)
	{
		m_estado = m_estadoq;
		m_destado = m_destadoq;
		elflag = 0;
		m_textoestado.Clear();
		ActualizaIcono();
		m_listausers.ClearAll();
		m_Passwd[0] = 0;
	}
	m_destadoq = "";
}



void CDlgPresencia::OnPresente(wxCommandEvent &event)
{	
	if (event.GetId() == 6000)
	{
		wxString ne = ElEstado[(int)Presente];
        EnviaOtroEstado((int)Presente,ne);
	}
	else
	{
		m_estadoq = Presente;
		m_destadoq = ElEstado[(int)m_estadoq];
		Actualiza();
	}
	m_antibug = 1;
}

void CDlgPresencia::OnIdentificar(wxCommandEvent &event)
{	
	m_estadoq = Presente;
	m_destadoq = ElEstado[(int)m_estadoq];
	/* dialogo identidad */
	CDlgId *pDlgId;
	wxSize size(200,150);
	pDlgId = new CDlgId(NULL,-1, "Identificación",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
	if (pDlgId->ShowModal() == wxID_OK)
	{
		unsigned char dstr[281];		
		unsigned long nuevoname;
		unsigned long nuevoserver;
		unsigned long mascara;
		char *pepe;
		extern SOCKET servidor;
		int i;
		wxString s;

		nuevoname  = (unsigned long)strtoul(pDlgId->m_codigo.GetValue(),&pepe,10);
		nuevoserver= (unsigned long)strtoul(pDlgId->m_codigob.GetValue(),&pepe,10);

		mascara = 0L;

		s = pDlgId->m_usuario.GetValue();
		for (i = 0;s[i];i++)
		{
			mascara += s[i];
		}
		mascara += i;

		nuevoname ^= mascara;
		nuevoserver ^= mascara;

		PonTheName(nuevoname);
		PonTheServer(nuevoserver);

		memset(dstr,' ',10);
		memcpy(dstr,pDlgId->m_password.GetValue(),pDlgId->m_password.GetValue().Len());
		dstr[10] = 0;

		int j = 0;
		int h;
		for (i = 0;i < 10;i++)
		{
			h = dstr[i] * 2;
			m_Passwd[j++] = (h/26) + 'A';		
			m_Passwd[j++] = (h%26) + 'A';
		}
		m_Passwd[20] = 0;
		for (i = 0;i < 20;i++)
		{
			m_Passwd[i] ^= 33;
		}

		closesocket(servidor);

	}
	delete pDlgId;

	Actualiza();
	m_antibug = 1;
}


void CDlgPresencia::OnAusente(wxCommandEvent &event)
{	
	enum ElEstado est;
	wxString ne;	
	int esotro = 0;
	int id = event.GetId();
	int pregunta =0;
	if (id > 5000)
	{
		esotro = 1;
		id -= 5001;
	}
	else
	{
		id -= 1001;
	}
	if (id >= 50)
	{
		id -= 50;
		est = Ocioso;
		ne = e_ocio[id];
		pregunta = flag_ocio[id];
	}
	else
	{
		est = Ausente;
		ne = e_trabajo[id];
		pregunta = flag_trabajo[id];
	}

	if (pregunta)
	{
		wxString one;
		one = "Descripcion " + ne + " :";
		one = wxGetTextFromUser(one,"Entre Descripcion",
							  ""/*, this*/);
		ne += (" " + one);
	}

	ne.Truncate(50);

	if (!ne.IsEmpty())
	{
		if (esotro)
		{
			EnviaOtroEstado((int)est,ne);
		}
		else
		{
			if (!m_Passwd[0])
			{				
				SetPassword();								
				if (!m_Passwd[0])
				{
					strcpy((char *)m_Passwd,"NOPE");
				}
			}
			m_destadoq = ne;
			m_estadoq = est;
			Actualiza();
		}
	}
	m_antibug = 1;
}

void CDlgPresencia::OnSalida(wxCommandEvent &event)
{	
	enum ElEstado est;
	wxString ne;	
	int id = event.GetId();
	int esotro = 0;
	if (id > 5000)
	{
		esotro = 1;		
	}
	est = Desconectado;
	ne = ElEstado[(int)est];

	if (esotro)
	{
		EnviaOtroEstado((int)est,ne);
	}
	else
	{
		long voces[256];
		int n,i;
		n = nvoces_activas;		
		for (i = 0;i < nvoces_activas;i++)
		{
			voces[i] = voces_activas[i];
		}
		for (i = 0;i < n;i++)
		{
			if (voces[i])
			{
				EnviaFlag(voces[i],UFLAG_FINVOX,FALSE);
				::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,voces[i]);
			}			
		}
		if (m_conferencia->m_propietario)
		{
			if (m_conferencia->m_propietario == dame_the_name())
			{			
				int i,nips = m_conferencia->m_users->GetItemCount();
				long ips[512];					
				for (i = 0;i < nips;i++)
				{
					ips[i] = m_conferencia->m_users->GetItemData(i);					
				}	
				QuitaDeConferencia(nips,ips);
			}
			else
			{
				QuitaDeConferencia(1,&(m_conferencia->m_propietario));
			}
		}
		elflag = 0;
		m_destadoq = ne;
		m_estadoq = est;
		Actualiza();
	}
	m_antibug = 1;
}


void CDlgPresencia::OnVacaciones(wxCommandEvent &event)
{	
	enum ElEstado est;
	wxString ne;	
	int id = event.GetId();
	int esotro = 0;
	if (id > 5000)
	{
		esotro = 1;		
	}
	est = Vacaciones;
	ne = ElEstado[(int)est];

	wxString one;
	one = "Descripcion " + ne + " :";
	one = wxGetTextFromUser(one,"Entre Descripcion",
						  ""/*, this*/);
	ne += (" " + one);

	ne.Truncate(50);

	if (esotro)
	{
		EnviaOtroEstado((int)est,ne);
	}
	else
	{
		m_destadoq = ne;
		m_estadoq = est;
		Actualiza();
	}
	m_antibug = 1;
}


void CDlgPresencia::OnEnfermo(wxCommandEvent &event)
{	
	enum ElEstado est;
	wxString ne;	
	int id = event.GetId();
	int esotro = 0;
	if (id > 5000)
	{
		esotro = 1;		
	}
	est = Enfermo;
	ne = ElEstado[(int)est];

	wxString one;
	one = "Descripcion " + ne + " :";
	one = wxGetTextFromUser(one,"Entre Descripcion",
						  ""/*, this*/);
	ne += (" " + one);

	ne.Truncate(50);

	if (esotro)
	{
		EnviaOtroEstado((int)est,ne);
	}
	else
	{
		m_destadoq = ne;
		m_estadoq = est;
		Actualiza();
	}
	m_antibug = 1;
}


void CDlgPresencia::OnTareas(wxCommandEvent &event)
{
	return;
	if (EnBarraTareas)
	{
		Show(FALSE);
		wxGetApp().QuitaBarra();
	}
	else
	{
		wxGetApp().PonBarra();
		Show(TRUE);
	}
	m_antibug = 1;
}

void CDlgPresencia::OnActiva(wxCommandEvent &event)
{	
	if (EnBarraTareas)
	{
		m_antibug = 1;
		return;
	}
#ifdef WIN32
	if (IsWindowVisible(par_hWnd))
	{
		ShowWindow(par_hWnd,SW_HIDE);
	}
	else
	{
		ShowWindow(par_hWnd,SW_SHOW);
		BringWindowToTop(par_hWnd);
		for (int i = 0;i < 500;i++)
		{
			if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_poractivar)
			{
				m_dlgmensaje[i]->Show(TRUE);
				m_dlgmensaje[i]->m_envia.SetFocus();
				m_dlgmensaje[i]->m_poractivar = FALSE;
			}
		}
	}
#endif
	m_antibug = 1;
}


void CDlgPresencia::OnOtros(wxCommandEvent &event)
{	
	enum ElEstado est;
	wxString ne;	
	int id = event.GetId();
	int esotro = 0;
	if (id > 5000)
	{
		esotro = 1;		
	}
	est = Otros;
	ne = ElEstado[(int)est];

	wxString one;
	one = "Descripcion " + ne + " :";
	one = wxGetTextFromUser(one,"Entre Descripcion",
						  ""/*, this*/);
	ne += (" " + one);

	ne.Truncate(50);

	if (esotro)
	{
		EnviaOtroEstado((int)est,ne);
	}
	else
	{
		m_destadoq = ne;
		m_estadoq = est;
		Actualiza();
	}
	m_antibug = 1;
}

void CDlgPresencia::OnParte(wxCommandEvent &event)
{
	m_antibug = 1;
	if (!m_partefi && ltdiapre03mas && ltdiapre03mas == tdiapre03mas)
	{		
		m_partefi = (FI_DEF *)malloc(sizeof(FI_DEF));
		if (lee_def("\4",m_partefi,1) < 0)
		{
			free((void *)m_partefi);
			m_partefi = NULL;
		}
	}

	if (!m_partefi)
	{
		wxMessageBox("Debe Esperar","Recibiendo Datos",wxOK /*| wxCENTRE */| wxICON_HAND,this);
		return;
	}
	
	{
		DWORD Size;
		char tmp[1024];
		wxString s,s1;	
		memset(tmp,0,sizeof(tmp));	
		Size = sizeof(tmp);
		RegQueryValue(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP\\IniTarea",tmp,(PLONG)&Size);
		if (tmp[0])
		{
			char *nombre;
			CListDef list1;
			CListDef list2;
			FI_DEF *fi1;
			FI_DEF *fi2;
			char path1[256];
			char path2[256];
			char path3[256];
			FILE *fin;

			strcpy(path1,ajusta_a_home_dir("\\dsp\\tmp\\tickets.txt"));
			strcpy(path2,ajusta_a_home_dir("\\dsp\\tmp\\clientes.txt"));
			strcpy(path3,ajusta_a_home_dir("\\dsp\\tmp\\parte.txt"));

			{
				HKEY hKey;
				LONG retv;
				DWORD Disposition;
				DWORD Type;
				DWORD Size;

				if (						   /*  */
				  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
								 "IniTarea",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
								 &hKey,&Disposition)) == ERROR_SUCCESS
					)
				{
				   Size = sizeof(path1);
				   Type = REG_SZ;				   
				   RegQueryValueEx(  hKey,"Tickets",NULL,&Type,(unsigned char *)path1,&Size);  
				   Size = sizeof(path1);
				   Type = REG_SZ;				   
				   RegQueryValueEx(  hKey,"Clientes",NULL,&Type,(unsigned char *)path2,&Size);  					
				   Size = sizeof(path1);
				   Type = REG_SZ;				   
				   RegQueryValueEx(  hKey,"Parte",NULL,&Type,(unsigned char *)path3,&Size);
				   RegCloseKey(hKey);
				}
			}


			fi1 = (FI_DEF *)malloc(sizeof(FI_DEF));
			fi2 = (FI_DEF *)malloc(sizeof(FI_DEF));
			lee_def("\3",fi1,1);
			lee_def("\2",fi2,1);

			/* grabar clientes y tickets y lanzar "externo" */

			list1.m_def = fi1;
			list1.Rellena((void *)xiapre02gz,txiapre02gz,TRUE,path1);

			list2.m_def = fi2;
			list2.Rellena((void *)d_diapre01,td_diapre01,FALSE,path2);

            free_def(fi1,1);
			free_def(fi2,1);

			nombre = (char *)malloc(strlen(s1)+1);
			strcpy((char *)nombre,tmp);

			system_hebrado((void *)nombre);	
			//_beginthread(system_hebrado,NULL,(void *)nombre);			

			fin = fopen(path3,"r");
			if (fin)
			{
				int c1 = 0,c2 = 0,c3 = 0,i;
				memset(tmp,0,1024);
				fgets(tmp,1000,fin);
				for (i = 0;tmp[i] && tmp[i] != 13 && tmp[i] != 10;i++)
				{
					if (tmp[i] == 1)
					{
						tmp[i] = 0;
						i++;
						c1 = i;
						break;
					}
				}
				for (;tmp[i] && tmp[i] != 13 && tmp[i] != 10;i++)
				{
					if (tmp[i] == 1)
					{
						tmp[i] = 0;
						i++;
						c2 = i;
						break;
					}
				}
				for (;tmp[i] && tmp[i] != 13 && tmp[i] != 10;i++)
				{
					if (tmp[i] == 1)
					{
						tmp[i] = 0;
						i++;
						c3 = i;
						break;
					}
				}
				tmp[i] = 0;

				fi1 = (FI_DEF *)malloc(sizeof(FI_DEF));
				lee_def("\4",fi1,1);
				/* Enviar Parte */
				{
					char mensaje[2048];
					char *p;
					int e;
					long yo = dame_the_name();
					int tam;
					long id = time(NULL);
					char *_buf;
					struct   in_addr in;  

					_buf = fi1->f_buf;

					p = mensaje;
					e = 9; // Parte
					memcpy(p,(void *)&e,sizeof(int));
					p += sizeof(int);
					memcpy(p,(void *)&id,sizeof(long));
					p += sizeof(long);
					tam = (fi1->f_lfich+1);
					memcpy(p,(void *)&tam,sizeof(int));
					p += sizeof(int);
					fi1->f_buf = p;
					st_campo(0,fi1,"           ");
					st_campo(1,fi1,"           ");
					st_campo(2,fi1,"           ");
					in.S_un.S_addr = (unsigned long)yo;
					st_campo(3,fi1,inet_ntoa(in));

					st_campo(4,fi1,(char *)(const char *)tmp);
					st_campo(5,fi1,(char *)(const char *)tmp+c1);
					st_campo(6,fi1,(char *)(const char *)tmp+c2);
					st_campo(7,fi1,(char *)(const char *)tmp+c3);

					p += tam;
					p--;
					*p = 10;
					p++;
					fi1->f_buf = _buf;
					
					tam_ultimo_parte = (int)(p-mensaje);
					id_ultimo_parte = id;
					memcpy(ultimo_parte,mensaje,tam_ultimo_parte);

					tiempoparte = time(NULL) + 30;

					pon_en_cola_envio(mensaje,tam_ultimo_parte,FALSE);
				}
				free_def(fi1,1);
				fclose(fin);
				unlink(path3);
			}
			return;
		}
	}

	wxSize size(600,400);
	CDlgParte dlg(m_partefi,this,-1, "Ticket",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
	

	dlg.ShowModal();	
	/*free_def(fi,1);*/
}

void CDlgPresencia::OnOcultar(wxCommandEvent& event)
{
	if (m_privilegio >= 20)
	{
		tipo_cliente &= ~TIPO_PIDEDECLOAK;
		tipo_cliente |= TIPO_PIDECLOAK;

		PideRefresco(FALSE);
	}
}


void CDlgPresencia::OnDesocultar(wxCommandEvent& event)
{
	if (m_privilegio >= 20)
	{
		tipo_cliente &= ~TIPO_PIDECLOAK;
		tipo_cliente |= TIPO_PIDEDECLOAK;

		PideRefresco(FALSE);
	}
}

void PresentaWeb(void *par)
{
	if ((int)ShellExecute(NULL,"open",web_tmp,"","",SW_SHOW) > 32)	
	{
		//::Sleep(500);
	}
}
void CDlgPresencia::OnWeb(wxCommandEvent &event)
{
	FILE *file;
	char *env = getenv("TEMP");
	

	m_antibug = 1;
	if (!env)
	{
		env = ".";
	}
	tiempo_web = time(NULL) + 30;
	sprintf(web_tmp,"%s\\.htm",env);
	file = fopen(web_tmp,"w");
	if (file)
	{
		fputs(MiWebUrl,file);
		fclose(file);
		_beginthread(PresentaWeb,NULL,(void *)NULL);        
	}	
}

void CDlgPresencia::OnFinParte(wxCommandEvent &event)
{
	{
		DWORD Size;
		char tmp[1024];		
		memset(tmp,0,sizeof(tmp));	
		Size = sizeof(tmp);
		RegQueryValue(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP\\FinTarea",tmp,(PLONG)&Size);
		if (tmp[0])
		{
			char *nombre;
			nombre = (char *)malloc(strlen(tmp)+1);
			strcpy((char *)nombre,tmp);
			_beginthread(system_hebrado,NULL,(void *)nombre);						
		}
	}
	{
		long name = dame_the_name();
		char mensaje[256];
		char *p;
		int e;

		p = mensaje;
		e = 12; // fin parte
		memcpy(p,(void *)&e,sizeof(int));
		p += sizeof(int);

		pon_en_cola_envio(mensaje,sizeof(int),FALSE);
	}
}


void CDlgPresencia::OnAlarma(wxCommandEvent &event)
{
	wxSize size(600,400);
	CDlgAlarma dlg(this,-1, "Alarma",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
    dlg.ShowModal();
}


void CDlgPresencia::OnCancel(wxCommandEvent &event)
{	
	GuardaPosicion();
	if (::MessageBox((HWND)GetHandle(),"¿Seguro?","Fin Programa",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)
	{
		Close(TRUE);
	}
	/*
    if (wxMessageBox(,"Fin Programa",wxYES_NO | wxICON_QUESTION|wxSTAY_ON_TOP,this) == wxYES)
	{
	   Close(TRUE);
	}
	*/
}

void CDlgPresencia::SetPassword()
{
	dentro += 2;
	unsigned char dstr[281];
	wxString ne;
	ne = wxGetPasswordFromUser("Clave Personal:","Entre la clave",""/*, this*/);
    memset(dstr,' ',10);
    memcpy(dstr,ne,ne.Len());
    dstr[10] = 0;

	int j = 0;
	int h;
	for (int i = 0;i < 10;i++)
	{
		h = dstr[i] * 2;
		m_Passwd[j++] = (h/26) + 'A';		
		m_Passwd[j++] = (h%26) + 'A';
	}
	m_Passwd[20] = 0;
	for (i = 0;i < 20;i++)
	{
		m_Passwd[i] ^= 33;
	}
	
	PideRefresco();

	dentro -= 2;
}

void CDlgPresencia::OnPassword(wxCommandEvent &event)
{
	SetPassword();
}

void CDlgPresencia::PideRefresco(bool urgencia)
{
	long name = dame_the_name();
	char mensaje[256];
	char *p;
	int e;

	p = mensaje;
	e = 3; // refresco
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	e = tipo_cliente|control_dspversion; // ver y modo de programa
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);

	pon_en_cola_envio(mensaje,sizeof(int)*2,urgencia);

}

void CDlgPresencia::PideInfo(const char *nombre,BOOL datos,long ip)
{	
	char mensaje[256];
	char *p;
	int e;

	if (!datos)
	{
		m_nombreinfo = nombre;
		m_nombreinfo += ',';
	    if ((m_privilegio < 10 && ip != dame_the_name()) || !ip)
		{        
		   e = 4;
		}
		else
		{
		   e = 10; // file+Parte
		}
	}
	else
	{
		e = 4; // file
	}

	p = mensaje;

	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	if (e == 10 || ip == dame_the_name())
	{
		memcpy(p,(void *)&ip,sizeof(long));
		p += sizeof(long);
		strcpy(p,nombre);
		p += (strlen(nombre)+1);
		pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)+strlen(nombre)+1,FALSE);
	}	
	else
	{
		strcpy(p,nombre);
		p += (strlen(nombre)+1);

		pon_en_cola_envio(mensaje,sizeof(int)+strlen(nombre)+1,FALSE);
	}

}


void CDlgPresencia::PideUser(const char *nombre)
{	
	char mensaje[256];
	char *p;
	int e;

	e = 24;

	p = mensaje;

	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	strcpy(p,nombre);
    p += (strlen(nombre)+1);
    pon_en_cola_envio(mensaje,sizeof(int)+strlen(nombre)+1,FALSE);
	tiempologin = time(NULL) + 10;
}


void CDlgPresencia::EnviaOtroEstado(int nest, wxString &ne)
{
	char mensaje[256];
	char *p;
	int e;	

	p = mensaje;
	e = 2; // codigo mensaje
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,m_Passwd,20);
	p += 20;
	memcpy(p,(void *)&m_otraip,sizeof(long));
	p += sizeof(long);
	e = nest;
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	strcpy(p,ne);
	pon_en_cola_envio(mensaje,sizeof(long)+sizeof(int)*2+strlen(p)+21,FALSE);
}

void CDlgPresencia::Actualiza()
{
	m_tiemposinmovimiento = 0;
	if (conectado)
	{
		if (!m_Passwd[0])
		{				
			SetPassword();								
			if (!m_Passwd[0])
			{
				strcpy((char *)m_Passwd,"NOPE");
			}
		}
		if (m_destadoq.IsEmpty())
		{
			m_destadoq = ElEstado[(int)m_estadoq];
		}
		EnviaEstado();
	}	
}

void CDlgPresencia::PonColores(char *p1)
{
	char *p = p1;
	int acabar = 0;
	int i = -1;
	m_tcolores = time(NULL) + 3600;
	while(!acabar)
	{
		if (*p == 10 || !*p)
		{
			if (!*p)
				acabar = 1;
			*p = 0;
			if (p > p1 && *(p-1) == 13)
			{
				*(p-1) = 0;
			}
			if (*p1)
			{
				if (!strcmp(p1,"::colores"))
				{
					i = 0;
				}
				else if (i >= 0 && i < 50)
				{
					strncpy(colores[i],p1,9);
					colores[i][9] = 0;
					i++;
				}
			}
			p1 = p + 1;
		}
		p++;
	}
}

void CDlgPresencia::RellenaDepartamentos(char *p1)
{
	char *p = p1;
	char *p2;
	int acabar = 0;
	int modo = 0;
	int departamento;
	int haycero = 0;
	int hay99999 = 0;
	int hay91999 = 0;
	int hay92999 = 0;
	int i,iemp;
	wxTreeItemId root = NULL;
	m_torganiza = time(NULL) + 86400;
	m_itemdep = NULL;

	m_tree.DeleteAllItems();

    for (i = 0;i < 100;i++)
		m_itememp[i] = NULL;

	while(!acabar)
	{
		if (*p == 10 || !*p)
		{
			if (!*p)
				acabar = 1;
			*p = 0;
			if (p > p1 && *(p-1) == 13)
			{
				*(p-1) = 0;
			}
			if (*p1)
			{
				switch(modo)
				{
				case 0:
					if (!strcmp(p1,"::organizacion"))
						modo = 1;
					break;
				case 1:
					root = m_tree.AddRoot(p1,0,0,new CItemData(-1));
					modo = 2;
					break;
				case 2:
					p2 = p1;
					departamento = 0;
					while(*p2 && *p2 != ',') p2++;
					if (*p2 == ',')
					{
						*p2 = 0;
						departamento = atoi(p1);
						p2++;
					}
					else
						p2 = p1;					
					if (!departamento)
						haycero = 1;
				
					if (departamento == 99999)
					{
						hay99999 = 1;
						m_itemdep = m_tree.AppendItem(root,p2,1,1,new CItemData(departamento));
					}
					else if (departamento == 91999)
					{
						hay91999 = 1;
						m_itemtut = m_tree.AppendItem(root,p2,1,1,new CItemData(departamento));
					}
					else if (departamento == 92999)
					{
						hay92999 = 1;
						m_itemlpd = m_tree.AppendItem(root,p2,1,1,new CItemData(departamento));
					}
					else
					{
						if (departamento > 999)
						{
							iemp = departamento/1000;
							if ((departamento%1000))
							{
								if (m_itememp[iemp])
								{
									m_tree.AppendItem(m_itememp[iemp],p2,1,1,new CItemData(departamento));
								}
							}							
							else
							{
								m_itememp[iemp] = m_tree.AppendItem(root,p2,1,1,new CItemData(departamento));
							}
						}
						else
						{
							m_tree.AppendItem(root,p2,1,1,new CItemData(departamento));
						}
					}
					break;
				}
			}
			p1 = p + 1;
		}
		p++;
	}

	if (!root)
	{
		root = m_tree.AddRoot("ORGANIZACION",0,0,new CItemData(-1));
	}

	if (!haycero)
	{
		m_tree.AppendItem(root,"Sin Departamento",1,1,new CItemData(0));
	}

	if (!hay99999)
	{
		m_itemdep = m_tree.AppendItem(root,"{TELESOPORTE CLIENTES}",1,1,new CItemData(99999));
	}
	if (!hay91999)
	{
		m_itemtut = m_tree.AppendItem(root,"{TUTORIA}",1,1,new CItemData(91999));
	}
	if (!hay92999)
	{
		m_itemlpd = m_tree.AppendItem(root,"{DSLPD}",1,1,new CItemData(92999));
	}

	m_itemestados = m_tree.AppendItem(root,"{Estados}",1,1,new CItemData(93999));
	for (i = 0;ElEstado[i][0];i++)
	{
		if (i == 2)
			continue;
		m_tree.AppendItem(m_itemestados,ElEstado[i],1,1,new CItemData(93000+i));
	}

	if (root)
	{
		m_tree.Expand(root);
		if (midepartamento != -1)
		{
			m_tree.SeleccionaDepartamento(midepartamento);
		}
		else
		{
			/*m_tree.SelectItem(root);*/
		}
	}
}

void CDlgPresencia::ProcesaMenu(char *p1)
{
	char *p = p1,*p2;
	int pregunta;
	int tt;
	int oo;
	int modo = 0;
	int acabar = 0;
	tt = 0;
	oo = 0;
	m_tmenu = time(NULL) + 3600;
	e_trabajo[tt][0] = 0;
	e_ocio[oo][0] = 0;
	while(!acabar)
	{
		if (*p == 10 || !*p)
		{
			if (!*p)
				acabar = 1;
			*p = 0;
			if (p > p1 && *(p-1) == 13)
			{
				*(p-1) = 0;
			}
			if (*p1)
			{
				if (!strcmp(p1,"::trabajo"))
				{
					modo = 1;
				}
				else if (!strcmp(p1,"::ocio"))
				{
					modo = 2;
				}
				else 
				{
					p2 = p1 + strlen(p1) - 3;					
					if (p2 >= p1 && !memcmp(p2,"...",3))
					{
						*p2 = 0;
						pregunta = 1;
					}
					else
						pregunta = 0;
					if (modo == 1)
					{
						strcpy(e_trabajo[tt],p1);
						flag_trabajo[tt] = pregunta;
						tt++;
						e_trabajo[tt][0] = 0;
					}
					else if (modo == 2)
					{
						strcpy(e_ocio[oo],p1);
						flag_ocio[oo] = pregunta;
						oo++;
						e_ocio[oo][0] = 0;
					}				
				}
			}
			p1 = p + 1;
		}
		p++;
	}
}

void CDlgPresencia::EnviaCorreo(const char *email)
{
	wxString s;
	static char elmail[512];
	if (!mapil)
	{
		mapil = ::LoadLibraryA("MAPI32.DLL");
	}
	if (!email)
	{
		long from = -1,to = -1;
		m_textoestado.GetSelection(&from,&to);
		if (from > -1 && to > from)
		{		
			s = m_textoestado.GetValue();
			s.Replace("\n","\r\n",TRUE);
			s = s.Mid(from,to-from);
			s.Trim();
			if (s.Find("@") > -1)
			{				
				email = s;
			}
		}
	}
	if (!email)
	{		
		s = m_textoestado.GetValue();
		int p = s.Find("@");
		if (p > -1)
		{
			int p1 = p;
			while(s[p] && s[p] != ' ')
				p++;
			while(p1 > 0 && s[p1] != ' ')
				p1--;
			s = s.Mid(p1+1,p-p1);
			s.Trim();
			email = s;
		}
	}
	

#ifdef WIN32
	if (email)
	{
		strcpy(elmail,email);
		email = elmail;
	}
	_beginthread(Email,NULL,(void *)email);
#endif
}

void CTexto::OnMouseEvent(wxMouseEvent& event)
{	
	static int uno = 0;
	if (uno)
	{
		((CDlgPresencia *)GetParent())->EnviaCorreo();
		uno = 0;
	}
	else
	{
		uno++;
		AddPendingEvent(event);
	}
	event.Skip();
}

void CDlgMensaje::Ocultar()
{
	if (m_departamento >= 90000)
	{
		char mensaje[1024];
		char *p;
		int e;		
		long yo = dame_the_name();
		unsigned short comando;		

		
		if (!m_llamar.GetLabel().Cmp("Colgar") || m_dlgvolume)
		{			
			DesactivaVoz();
			/*m_llamar.SetLabel("Voz");*/
			/*NO_LLAMADA(m_padre->m_flagq);

			m_padre->EnviaEstado();*/

			p = mensaje;
			e = 18; // comando telesoporte
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&m_ip,sizeof(long));
			p += sizeof(long);
			comando = C_DATA;
			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);

			comando = 27; /* Tipo DATA 27 ... COLGAR!!! */					

			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);			
			pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)+sizeof(comando)*2,FALSE);
			::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_iptlvoz);			
		}

		m_control.SetLabel("Atender");
		m_ficheros.Enable(FALSE);
		m_llamar.Enable(FALSE);

		p = mensaje;
		e = 18; // comando telesoporte
		memcpy(p,(void *)&e,sizeof(int));
		p += sizeof(int);
		memcpy(p,(void *)&m_ip,sizeof(long));
		p += sizeof(long);
		comando = C_ADIOS;
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);
		
		pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long)+sizeof(comando),FALSE);
	}
	if (m_dlgvolume)
	{
		m_dlgvolume->Show(FALSE);
		m_sizerTop->Remove(m_dlgvolume);
		m_sizerTop->Layout();
		delete m_dlgvolume;
		m_dlgvolume = NULL;
	}
	if (m_dlgatender)
	{
		if (m_explorer)
		{
			delete m_explorer;		
			m_explorer = NULL;
		}
		m_dlgatender->Show(FALSE);
		m_sizerTop->Remove(m_dlgatender);
		m_sizerTop->Layout();
		delete m_dlgatender;
		m_dlgatender= NULL;
	}


	Show(FALSE);
}

void CDlgMensaje::Oculta(wxString &nombre,int &item,wxString &s,int &estado)
{
		s = m_llamar.GetLabel();
		if (m_conferencia)
		{
			if (!s.Cmp("Colgar"))
			{
				//m_envoz = FALSE;
				/*NO_LLAMADA(m_padre->m_flagq);*/
				DesactivaVoz();
				/*m_llamar.SetLabel("Voz");*/
				int i,nips = m_users->GetItemCount();				
				for (i = 0;i < nips;i++)
				{
					if (m_users->GetItemData(i) != dame_the_name())
					{
						m_padre->EnviaFlag(m_users->GetItemData(i),UFLAG_FINVOX,FALSE);
						::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_users->GetItemData(i));
					}
				}
			}
			if (m_propietario == dame_the_name())
			{
				int i,nips = m_users->GetItemCount();
				long ips[512];					
				for (i = 0;i < nips;i++)
				{
					ips[i] = m_users->GetItemData(i);					
				}	
				m_padre->QuitaDeConferencia(nips,ips);
				NO_CONFAMO(m_padre->m_flagq);
			}
			else
			{				
				m_padre->QuitaDeConferencia(1,&(m_propietario));
				NO_ENCONF(m_padre->m_flagq);
			}
			m_users->DeleteAllItems();
			m_propietario = NULL;
		}
		else
		{				
			if (!s.Cmp("Colgar"))
			{
				//m_envoz = FALSE;
				/*
				if (nvoces_activas == 1 && voces_activas[0] == m_ip)
				{
					NO_LLAMADA(m_padre->m_flagq);
				}
				^*/
				DesactivaVoz();
				/*m_llamar.SetLabel("Voz");*/
				m_padre->EnviaFlag(m_ip,UFLAG_FINVOX,FALSE);			
				::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_ip);
				if ((_userflags[item] & UFLAG_RING))
				{
					_userflags[item] &= ~UFLAG_RING;
				}
			}			
			else if ((_userflags[item] & UFLAG_RING))
			{
				/*
				if (nvoces_activas == 1 && voces_activas[0] == m_ip)
				{
					NO_LLAMADA(m_padre->m_flagq);
				}
				*/
				_userflags[item] &= ~UFLAG_RING;
				m_padre->EnviaFlag(m_ip,UFLAG_FINVOX,FALSE);
				::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_ip);
			}
			
		}				
		Ocultar();
		/*Show(FALSE);*/

}


int CDlgPresencia::ActivaMensaje(long ip,BOOL ocultar,BOOL foco)
{
	long otra = ip;
	long mia = dame_the_name();
	int item;

	HWND vieja = GetFocus();
	

	wxString titulo;
	int departamento = -1;
	int estado = -1;

	item = ListaPorIp(ip,titulo);
	if (item >= 0)
	{
		int flag,privilegio;
		wxString nombre;
		wxString destado;
		int eltipocliente;		
		EstadoItem(lista_users[item],estado,flag,privilegio,nombre,destado,eltipocliente,departamento);		
	}

	wxSize size(300,420);	
	for (int i = 0;i < 500;i++)
	{
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == otra)
		{
			wxString anttitle;
			anttitle = m_dlgmensaje[i]->GetTitle();
			m_dlgmensaje[i]->m_departamento = departamento;						
			if (anttitle.Cmp(titulo))
			{
				m_dlgmensaje[i]->SetTitle(titulo);
				m_dlgmensaje[i]->CenterOnScreen();			
			}
			


			if (ocultar)
			{
				/*
				for (int j = 0;j < 500;j++)
				{
					if (i != j && m_dlgmensaje[j])
					{
						wxString s;
						s = m_dlgmensaje[j]->m_llamar.GetLabel();
						if (!s.Cmp("Colgar"))
						{
							if (nvoces_activas < 2)
							{
								m_flagq = FLAG_NOLLAMADA;
							}				
							m_dlgmensaje[j]->m_llamar.SetLabel("Voz");
							EnviaFlag(m_dlgmensaje[j]->m_ip,UFLAG_FINVOX,FALSE);				
							::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_dlgmensaje[j]->m_ip);
						}
						m_dlgmensaje[j]->Show(FALSE);
					}
				}
				*/
				return i;
			}
			else
			{
				if (m_nomolestar.GetValue() && !m_dlgmensaje[i]->IsShown() && !IsWindowVisible(par_hWnd))
				{
					idle_aviso = 1;
					/*
					MessageBeep(0);
					FlashIcono(Presente);
					Sleep(200);		
					FlashIcono(Desconectado);
					Sleep(200);		
					FlashIcono(m_estado);
					*/
					m_dlgmensaje[i]->m_poractivar = TRUE;
					return i;
				}
			}						
			HWND hwnd;
			hwnd = ::GetFocus();
			m_dlgmensaje[i]->Show(TRUE);
			if (foco)
			{
				if (::IsWindowVisible((HWND)GetHandle())) 
				{					
					m_dlgmensaje[i]->m_envia.SetFocus();
				}
			}
			else
				::SetFocus(hwnd);
			/*
			else
				::SetFocus(vieja);
			*/
			m_dlgmensaje[i]->m_poractivar = FALSE;
			return i;
		}
	}

	for (i = 0;i < 500;i++)
	{
		if (!m_dlgmensaje[i])
		{			
			if (ocultar)
			{
				/*
				for (int j = 0;j < 500;j++)
				{
					if (i != j && m_dlgmensaje[j])
					{
						wxString s;
						s = m_dlgmensaje[j]->m_llamar.GetLabel();
						if (!s.Cmp("Colgar"))
						{
							if (nvoces_activas < 2)
							{
								m_flagq = FLAG_NOLLAMADA;
							}				
							m_dlgmensaje[j]->m_llamar.SetLabel("Voz");
							EnviaFlag(m_dlgmensaje[j]->m_ip,UFLAG_FINVOX,FALSE);				
							::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_dlgmensaje[j]->m_ip);
						}
						m_dlgmensaje[j]->Show(FALSE);
					}
				}
				*/
			}			

			if (ocultar)
			{
				if (poolposicion >= maxpool)
				{
					HWND vieja = ::GetForegroundWindow();
					for (i =0 ;i < maxpool;i++)
					{
						pooldlgs[i] = new CDlgMensaje(this,-1, "",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP|wxMINIMIZE_BOX);
					}
					poolposicion = 0;
					::SetForegroundWindow(vieja);
				}
				{					
					m_dlgmensaje[i] = pooldlgs[poolposicion];
					pooldlgs[poolposicion] = NULL;
					poolposicion++;
					m_dlgmensaje[i]->SetTitle(titulo);
				}
			}
			else
			{
				m_dlgmensaje[i] = new CDlgMensaje(this,-1, titulo,wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP|wxMINIMIZE_BOX);
			}

			m_dlgmensaje[i]->m_ip = otra;
			if (otra == dame_the_name())
			{
				m_dlgmensaje[i]->SoyYo(true);
			}
			else
			{
				m_dlgmensaje[i]->SoyYo(false);
			}			
			m_dlgmensaje[i]->m_departamento = departamento;
			if (departamento >= 90000)
			{
				m_dlgmensaje[i]->m_llamar.Enable(FALSE);
				m_dlgmensaje[i]->m_control.SetLabel("Atender");
				m_dlgmensaje[i]->m_ficheros.Enable(FALSE);
			}
			m_dlgmensaje[i]->CenterOnScreen();
			ActualizaImagenMensaje((char *)otra,TRUE);
			wxFont ff;
			ff =  m_dlgmensaje[i]->GetFont();
			ff.SetWeight(wxBOLD);
			m_dlgmensaje[i]->SetFont(ff);
			if (!ocultar && m_nomolestar.GetValue() && !IsWindowVisible(par_hWnd))
			{
				idle_aviso = 1;
				/*
				MessageBeep(0);
				FlashIcono(Presente);
				Sleep(200);		
				FlashIcono(Desconectado);
				Sleep(200);		
				FlashIcono(m_estado);
				*/
				m_dlgmensaje[i]->m_poractivar = TRUE;
			}
			else
			{
				m_dlgmensaje[i]->m_departamento = departamento;
				if (!ocultar)
				{
					m_dlgmensaje[i]->Show(TRUE);
					//if (::IsWindowVisible((HWND)GetHandle()))
					{
						m_dlgmensaje[i]->m_envia.SetFocus();
					}
					/*
					else
						::SetFocus(vieja);			
					*/
					::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Activa,m_dlgmensaje[i]->m_ip);
					m_dlgmensaje[i]->m_poractivar = FALSE;
				}
			}
			return i;
		}
	}
	return -1;
}



WORD APIENTRY DIBNumColors(LPSTR lpDIB)
{
    WORD wBitCount;  // DIB bit count
	
    // If this is a Windows-style DIB, the number of colors in the
    // color table can be less than the number of bits per pixel
    // allows for (i.e. lpbi->biClrUsed can be set to some value).
    // If this is the case, return the appropriate value.
    
	

    // Calculate the number of colors in the color table based on
    // the number of bits per pixel for the DIB.
    
    wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	
    // return number of colors based on bits per pixel
	
    switch (wBitCount)
    {
	case 1:
		return 2;
		
	case 4:
		return 16;
		
	case 8:
		return 256;
		
	default:
		return 0;
    }
}

WORD APIENTRY PaletteSize(LPSTR lpDIB)
{
    return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}

void CDlgPresencia::NoHayFoto(char *p1)
{
	long de;
	de = *(long *)p1;

	{
		char tmp[512];
		sprintf(tmp,ajusta_a_home_dir("\\dsp\\%lX.dsi"),de);
		unlink(tmp);
	}

	p1 += sizeof(long);
	int i;
	for (i = 0;i < 500;i++)
	{
		if (m_imagenes[i] == de)
		{
			break;
		}
	}
	if (i >= 500)
		return;
	if (!m_faltaimagen[i])
		return;
	m_faltaimagen[i] = 0;	
	m_tiempofoto[i] = 0;	

    m_imageListPatron->Replace(i,wxBitmap("BMPCOMICO",wxBITMAP_TYPE_BMP_RESOURCE));
	PonImagenEstado(i);

	if (m_selip == de)
	{
		PonSelIp(de);
	}
	ActualizaImagenMensaje((char *)de,TRUE);
	
	wxString nombre;
	int item;	
	if ((item = m_listausers.PorIp(de,nombre)) < 0)
	{
		if ((item = m_listausersg.PorIp(de,nombre)) < 0)
		{
			return;
		}		
	}	
	InvalidateRect((HWND)m_listausers.GetHandle(),NULL,FALSE);	
	InvalidateRect((HWND)m_listausersg.GetHandle(),NULL,FALSE);	
}

void CDlgPresencia::EsFoto(char *p1,int size)
{
	long de;
	int file;
	char tmp[512];

	if (!size)
		return;

	de = *(long *)p1;
	p1 += sizeof(long);

	int i;
	for (i = 0;i < 500;i++)
	{
		if (m_imagenes[i] == de)
		{
			break;
		}
	}
	if (i >= 500)
		return;
	if (!m_faltaimagen[i])
		return;

	if (size > 0)
	{
		sprintf(tmp,ajusta_a_home_dir("\\dsp\\%lX.dsi"),de);
		file = open(tmp,O_RDWR|O_CREAT|O_TRUNC|O_BINARY,S_IREAD|S_IWRITE);
		if (file >= 0)
		{
			write(file,p1-sizeof(long),size);
			close(file);
		}
		m_faltaimagen[i] = 0;
		m_tiempofoto[i] = 0;
	}
	else
	{		
		int g;
		unsigned int a;
		unsigned short *pp = (unsigned short *)p1;
		unsigned short xors = 0;

		for (g = 0;g < (int)(((-size) -sizeof(long))/2);g++)
		{
			xors ^= *pp;
			pp++;
		}
		a = (unsigned int)xors;
		a <<= 16;
		m_faltaimagen[i] = a;
		m_faltaimagen[i] |= 2;
		m_tiempofoto[i] = time(NULL) + 30 + (rand()%300);
	}	

	wxBitmap bmp1(58,58),bmp(44,44);
	

	BITMAPFILEHEADER  *bmfHeader;
	BITMAPINFOHEADER  *lpbi;
	bmfHeader = (BITMAPFILEHEADER *)p1;
	p1 += sizeof(BITMAPFILEHEADER);
	lpbi = (BITMAPINFOHEADER  *)p1;

	
	LPSTR       lpDIBBits;

	lpDIBBits = p1 + *(LPDWORD)lpbi + PaletteSize((LPSTR)lpbi);

	HDC hDC = GetDC(NULL);
    HANDLE hBitmap = CreateDIBitmap(hDC, (LPBITMAPINFOHEADER)lpbi, CBM_INIT,
		lpDIBBits, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	ReleaseDC(NULL,hDC);
	::DeleteObject((HGDIOBJ)bmp.GetHBITMAP());
	bmp.SetHBITMAP((WXHBITMAP)hBitmap);	
	
	{
		wxMemoryDC temp_dc;
		wxMemoryDC old_dc;
		temp_dc.SelectObject(bmp1);
		old_dc.SelectObject(bmp);
		
		temp_dc.Blit(7,7, 49, 49, &old_dc, 0, 0);
	}
	m_imageListPatron->Replace(i,bmp1);
	PonImagenEstado(i);

	if (m_selip == de)
	{
		PonSelIp(de);
	}
	ActualizaImagenMensaje((char *)de,TRUE);

	wxString nombre;    
	int item;	
	if ((item = m_listausers.PorIp(de,nombre)) < 0)
	{
		if ((item = m_listausersg.PorIp(de,nombre)) < 0)
		{
			return;
		}		
	}	
	
	InvalidateRect((HWND)m_listausers.GetHandle(),NULL,FALSE);	
	InvalidateRect((HWND)m_listausersg.GetHandle(),NULL,FALSE);	
}

int CDlgPresencia::DameEstado(long de,int &flag,unsigned long &userflag,wxString &nombre,int &tipo)
{
	int privilegio,departamento;	
	wxString destado;
	return EstadoPorIp(de,flag,privilegio,nombre,destado,tipo,departamento,userflag);
}


/* no hace falta ....
wxString pon_rtflinks(char *mensaje)
{
	wxString r;	
	while(*mensaje)
	{
		if (!memcmp(mensaje,"http://",7) || !memcmp(mensaje,"https://",8) || !memcmp(mensaje,"ftp://",6))
		{
			r += "{\\field {\\*\\fldinst HYPERLINK ";
			r += '"';
			while(*mensaje && *mensaje != ' ')
			{
				r += *mensaje;
				mensaje++;
			}
			r += '"';
			r += "}}";
		}
		r += *mensaje;		
		mensaje++;
	}
	return r;
}
*/


void CDlgPresencia::EsMensaje(char *p1)
{	
	long de,a;
	char *mensaje;
	BOOL ocultar = FALSE;
	de = *(long *)p1;
	p1 += sizeof(long);
	a = *(long *)p1;
	p1 += sizeof(long);
	mensaje = p1;	
	wxString nombre;

	int item;
	item = ListaPorIp(de,nombre);

	if (item < 0)
	{		
		return;
	}
	if (*mensaje == 1)
	{
		mensaje++;
		ocultar = TRUE;
	}
	if (*mensaje == 11)
	{
		mensaje++;
		/* dialogo broadcast */		
		IRichEditOle* pRichEditOle = NULL;		
		m_avisos->m_recibe.SetInsertionPoint(m_avisos->m_recibe.GetLastPosition() /*0*/ );

		m_avisos->m_recibe.WriteText("\n");

  		::SendMessage((HWND)m_avisos->m_recibe.GetHandle(),
		   EM_GETOLEINTERFACE,
		   0,
		   (LPARAM)&pRichEditOle);
		if (pRichEditOle)
		{			
			wxBitmap bmp1(46,46),bmp;
			for (int i = 0;i < 500;i++)
			{
				if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == (long)de)
				{					
					bmp = m_dlgmensaje[i]->m_imagenestado.GetBitmap();
					{
						wxMemoryDC temp_dc;
						wxMemoryDC old_dc;
						temp_dc.SelectObject(bmp1);
						old_dc.SelectObject(bmp);
						
						temp_dc.DrawRectangle(0,0,46,46);
						temp_dc.Blit(1,1, 44, 44, &old_dc, 7, 7);						
					}
					CImageDataObject::InsertBitmap(pRichEditOle,(HBITMAP)bmp1.GetHBITMAP());
					break;
				}
			}
		}		
		wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf6\\lang1034\\langfe3082\\langnp1034 " + nombre + ":}";
		m_avisos->m_recibe.WriteText(text);			
		text = auto_escapa(mensaje);//pon_rtflinks(mensaje);
		text = "{\\rtf1\\cf0\\b "+ text +"}";
		m_avisos->m_recibe.WriteText(text);
		m_avisos->m_recibe.WriteText("\n");
		m_avisos->m_recibe.ShowLastPosition();
		m_avisos->Show(TRUE);
		return;
	}
	item = ActivaMensaje(de,ocultar);		
	if (item >= 0)
	{	
		if (::IsIconic((HWND)m_dlgmensaje[item]->GetHandle()))
		{
			/*::FlashWindow((HWND)m_dlgmensaje[item]->GetHandle(),TRUE);
			::FlashWindow((HWND)m_dlgmensaje[item]->GetHandle(),TRUE);
			::FlashWindow((HWND)m_dlgmensaje[item]->GetHandle(),TRUE);
			::FlashWindow((HWND)m_dlgmensaje[item]->GetHandle(),TRUE);
			*/
			/*
			Sleep(100);
			::FlashWindow((HWND)m_dlgmensaje[item]->GetHandle(),FALSE);
			*/
			::ShowWindow((HWND)m_dlgmensaje[item]->GetHandle(),SW_RESTORE);
			m_dlgmensaje[item]->RepintaTodo();
			
		}		
		m_dlgmensaje[item]->m_recibe.SetInsertionPointEnd();						
		m_dlgmensaje[item]->m_recibe.WriteText("\n");
		wxString text = "{\\rtf1{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf6\\lang1034\\langfe3082\\langnp1034 " + nombre + ":\\par}";		
		m_dlgmensaje[item]->m_recibe.WriteText(text);	
		//m_dlgmensaje[item]->m_recibe.AppendText("\n");
		text = auto_escapa(mensaje);//pon_rtflinks(mensaje);
		text = "{\\rtf1\\b "+ text +"}";
		m_dlgmensaje[item]->m_recibe.WriteText(text);		
		m_dlgmensaje[item]->m_recibe.WriteText("\n");
		m_dlgmensaje[item]->m_recibe.ShowLastPosition();
		
		
		
		if (m_dlgmensaje[item]->m_poractivar)
		{
			m_dlgmensaje[item]->m_envia.SetValue("(No Molestar)");
			m_dlgmensaje[item]->EnviaMensaje();
		}
		/*
		else {
			if (m_dlgmensaje[item]->IsIconized()) {
				m_dlgmensaje[item]->Iconize(false);
			}
		}
		*/
		
	}
}


void CDlgPresencia::EsConferencia(char *_p1)
{
	char *p1 = _p1;
	long de,a;
	char *mensaje;
	de = *(long *)p1;
	p1 += sizeof(long);
	a = *(long *)p1;
	p1 += sizeof(long);
	mensaje = p1;

	CListaUsers *plista;
	wxString nombre;

	int item;
	if (!m_conferencia)
	{
		EsMensaje(_p1);
		return;
	}
	
	if ((item = m_conferencia->m_users->PorIp(de,nombre)) >= 0)
	{
		plista = m_conferencia->m_users;
	}
	if (item < 0)
	{
		EsMensaje(_p1);
		return;
	}	
	if (item >= 0)
	{
		m_conferencia->m_recibe.SetInsertionPoint(m_conferencia->m_recibe.GetLastPosition() /*0*/ );		
		m_conferencia->m_recibe.WriteText("\n");
		int color = 3 + item;
		char elcolor[21];
		if (color > 14)
			color = 14;
		sprintf(elcolor,"%d",color);
		wxString text = "{\\rtf1\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red128\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}\\b\\cf";
		text += elcolor;
		text += "\\lang1034\\langfe3082\\langnp1034 ";
		text += nombre;
		text += ":\\par}";				

		text = nombre + ":";
		text = "{\\rtf1\\b "+ text +"}";
		m_conferencia->m_recibe.WriteText(text);		
		text = auto_escapa(mensaje);
		text = "{\\rtf1\\b "+ text +"}";
		m_conferencia->m_recibe.WriteText(text);
		m_conferencia->m_recibe.WriteText("\n");
		m_conferencia->m_recibe.ShowLastPosition();
	}
	if (::IsWindowVisible((HWND)m_conferencia->GetHandle()))
	{
		m_conferencia->m_envia.SetFocus();
	}
}


void CDlgPresencia::CreaConferencia(char *p1)
{
	long de;
	int nips,i,tipo;
	int flag;
	int estado;
	unsigned long userflag;
	wxString nombre;
	long *ips;
	de = *(long *)p1;
	p1 += sizeof(long);
	nips = *(int *)p1;
	p1 += sizeof(int);
	ips = (long *)p1;

	
	
	if (!m_conferencia )
		return;

	if (!m_conferencia->m_propietario)
	{
		m_conferencia->m_propietario = de;
		m_conferencia->m_llamar.Enable(false);
		for (i = 0;i < nips;i++)
		{
		    estado = DameEstado(ips[i],flag,userflag,nombre,tipo);
			if (estado >= 0)
			{
				m_conferencia->m_users->CreaItem(nombre,ips[i],-1,flag,tipo);
			}
		}		
		m_conferencia->Show(TRUE);
		m_conferencia->m_envia.SetFocus();
		ENCONF(m_flagq);
	}
	else
	{
		if (de != dame_the_name())
		{
			if (m_conferencia->m_propietario == de)
			{
				// quitar las que sobran
				int cambios = 0;
				int j,ni = m_conferencia->m_users->GetItemCount();
				long ip;
				long aborrar[512];
				int naborrar = 0;
				for (i = 0;i < ni;i++)
				{
					ip = m_conferencia->m_users->GetItemData(i);
					for (j = 0;j < nips;j++)
					{
						if (ip == ips[j])
							break;
					}			
					if (j >= nips)
					{
						aborrar[naborrar] = ip;
						naborrar++;					
					}
				}
				for(i = 0;i < naborrar;i++)
				{
					m_conferencia->m_users->BorraIP(aborrar[i]);
					cambios++;
				}
				// poner las que faltan
				for (i = 0;i < nips;i++)
				{
					if (m_conferencia->m_users->PorIp(ips[i],nombre) < 0)
					{
						estado = DameEstado(ips[i],flag,userflag,nombre,tipo);
						if (estado >= 0)
						{
							m_conferencia->m_users->CreaItem(nombre,ips[i],-1,flag,tipo);
							cambios++;
						}
					}
				}
				if (cambios)
				{

				}
			}
			else
			{
				QuitaDeConferencia(1,&de);
			}
		}
	}	
}

void CDlgPresencia::QuitaConferencia(char *p1)
{
	long de,a;
	de = *(long *)p1;
	p1 += sizeof(long);
	a = *(long *)p1;
	p1 += sizeof(long);

	if (!m_conferencia )
		return;

	if (m_conferencia->m_envoz)
	{
		m_conferencia->DesactivaVoz();
		/*m_conferencia->m_llamar.SetLabel("Voz");*/
		//m_conferencia->m_envoz = FALSE;
	}

	if (m_conferencia->m_propietario)
	{
		// no voy a chequear si soy yo mismo ...		
		if (m_conferencia->m_propietario == de)
		{
			m_conferencia->m_users->DeleteAllItems();
			m_conferencia->m_propietario = NULL;
			m_conferencia->Show(FALSE);
			NO_ENCONF(m_flagq);
			NO_CONFAMO(m_flagq);
		}
		else
		{
			m_conferencia->m_users->BorraIP(de);
			if (m_conferencia->m_propietario == dame_the_name())
			{
				long ips[512];
				int nips = m_conferencia->m_users->GetItemCount();
				for (int i = 0;i < nips;i++)
				{
					ips[i] = m_conferencia->m_users->GetItemData(i);
				}
				EnviaConferencia(nips,ips);
			}
		}
	}
}

void CDlgPresencia::EnviaConferencia(int nips,long *ips)
{
	char mensaje[1024];
	char *p;
	int e;
	long yo = dame_the_name();			

	if (!nips) return;

	p = mensaje;
	e = 16; // conferencia create
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&yo,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&nips,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)ips,sizeof(long)*nips);
	
	pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long)+sizeof(long)*nips,FALSE);
}

void CDlgPresencia::QuitaDeConferencia(int nips,long *ips)
{
	char mensaje[1024];
	char *p;
	int e;
	long yo = dame_the_name();			

	if (!nips) return;

	p = mensaje;
	e = 17; // conferencia remove
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&yo,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&nips,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)ips,sizeof(long)*nips);
	
	pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long)+sizeof(long)*nips,FALSE);
}

void CDlgPresencia::EnviaFlag(long ip,unsigned long flag,int quitar)
{
	char mensaje[512];
	char *p;
	int e;
	long yo = dame_the_name();		
	p = mensaje;
	e = 14; // flag
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&yo,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&ip,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&flag,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&quitar,sizeof(int));
	p += sizeof(int);
	pon_en_cola_envio(mensaje,sizeof(int)*2+sizeof(long)*3,FALSE);
}

void CDlgPresencia::EsFlag(char *p1)
{
	long de,a;
	int quitar;
	unsigned long flag,antflag;
	de = *(long *)p1;
	p1 += sizeof(long);
	a = *(long *)p1;
	p1 += sizeof(long);
	flag = *(unsigned long *)p1;	
	p1 += sizeof(unsigned long);
	quitar = *(int *)p1;	
	
	wxString ss,nombre;
	int i;

	int item;	
	if ((item = ListaPorIp(de,nombre)) < 0)
	{
		return;
	}

	for (i = 0;i < 500;i++)
	{
		if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == de)
		{							
			break;
		}
	}
	if (i >= 500)
	{
		i = ActivaMensaje(de,TRUE,TRUE);
		if (i < 0 || i >= 500)
			return;
	}

	if (flag == UFLAG_SIFICHEROS)
	{
		if (m_dlgmensaje[i]->m_explorer)
		{
			int j;
			for (j = 0;j < MAXENVIOS;j++)
			{
				if (m_dlgmensaje[i]->m_explorer->m_envio[j])
					break;
			}
			if (j < MAXENVIOS)
			{
				if (::MessageBox((HWND)GetHandle(),"¿Desea interrumpir los envios en curso?","ATENCION TRANSFERENCIA FICHEROS",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)
				{
					delete m_dlgmensaje[i]->m_explorer;
					m_dlgmensaje[i]->m_explorer = NULL;
				}
			}
			else
			{
				delete m_dlgmensaje[i]->m_explorer;
				m_dlgmensaje[i]->m_explorer = NULL;
			}
		}
		if (!m_dlgmensaje[i]->m_explorer)
		{
			ss = "Trasferir ficheros con " + GetLabel();
			m_dlgmensaje[i]->m_explorer = new CDlgExplorer(this,m_dlgmensaje[i],this,-1,ss,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox");
		}		
		else
			return;
		m_dlgmensaje[i]->m_explorer->Center();
		m_dlgmensaje[i]->m_explorer->Show(TRUE);		
		m_dlgmensaje[i]->m_explorer->SetFocus();
		return;
	}

	if (flag == UFLAG_NOFICHEROS)
	{
		if (m_dlgmensaje[i]->m_explorer)
		{
			delete m_dlgmensaje[i]->m_explorer;
			m_dlgmensaje[i]->m_explorer = NULL;
		}
		ss = nombre;
		ss += " No acepta la peticion";
		wxMessageBox(ss,"Peticion de Ficheros",wxOK /*| wxCENTRE */| wxICON_HAND,this);
		return;
	}

	if (flag == UFLAG_CONTROL || flag == UFLAG_FICHEROS)
	{	
		
		//ListaPorIp(de,nombre);
		if (flag == UFLAG_FICHEROS)
			ss = "¿Permite compartir ficheros con ";
		else
			ss = "¿Permite el control de su estación por ";
		ss += nombre;
		ss += "?";
		if (::MessageBox((HWND)GetHandle(),ss,"Petición de usuario",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)

	    /*if (wxMessageBox(ss,"Petición de telecontrol",wxYES_NO| wxICON_QUESTION,this) == wxYES)*/
		{
			HWND hwnd;
			unsigned long laip = htonl(de);
			unsigned short port;

			if (flag == UFLAG_FICHEROS)
			{				
				if (!m_dlgmensaje[i]->m_explorer)
				{					
					ss = "Trasferir ficheros con " + GetLabel();
					m_dlgmensaje[i]->m_explorer = new CDlgExplorer(this,m_dlgmensaje[i],this,-1,ss,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox",TRUE,TRUE);
				}
			    EnviaFlag(de,UFLAG_SIFICHEROS,FALSE);
			}
			else
			{
				port = 5550 + (laip & 0xff);
				hwnd = ::FindWindow(MENU_CLASS_NAME,MENU_CLASS_NAME);
				if (!hwnd)
				{
					 STARTUPINFO stinfo;
					 PROCESS_INFORMATION proi;
					 char tmpx[1024];
					 int n;

					 sprintf(tmpx,"windstvnc.exe");
					 memset(&stinfo,0,sizeof(stinfo));
					 stinfo.cb = sizeof(stinfo);
					 stinfo.wShowWindow = SW_SHOWMINIMIZED;
					 CreateProcess(NULL,tmpx,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS,NULL,NULL,&stinfo,&proi);

					 for (n = 0;n < 60 && !hwnd;n++)
					 {							
						Sleep(500);
						hwnd = ::FindWindow(MENU_CLASS_NAME,MENU_CLASS_NAME);
					 }
				}
				if (hwnd)
				{
					
					::PostMessage(hwnd,MENU_ADD_CLIENT_MSG,(WPARAM)port,(LPARAM)IpPorCodigo(de));
				}
			}
		}
		else
		{
			if (flag == UFLAG_FICHEROS)
			{
				if (m_dlgmensaje[i]->m_explorer)
				{
					delete m_dlgmensaje[i]->m_explorer;
					m_dlgmensaje[i]->m_explorer = NULL;
				}
				EnviaFlag(de,UFLAG_NOFICHEROS,FALSE);
			}
		}
		return;
	}

	antflag = _userflags[item];
	if (quitar)
	{
		_userflags[item] &= ~flag;
	}
	else
	{
		_userflags[item] |= flag;
	}
	if (_userflags[item] != antflag)
	{		
		if ((_userflags[item] & UFLAG_RING))
		{
			char tmpx[512];
			char *p1;
			p1 = tmpx;			

			/*
			if (m_conferencia && m_conferencia->m_envoz)
			{
				_userflags[item] &= ~UFLAG_RING;
				EnviaFlag(de,UFLAG_FINVOX,FALSE);
			}
			else
			*/
			{
				*(long *)p1 = de;
				p1 += sizeof(long);
				*(long *)p1 = dame_the_name();						
				p1 += sizeof(long);
				if (de == dame_the_name())
				{
					strcpy(p1,"Auto conversacion (para test).");
					_userflags[item] &= ~UFLAG_RING;
					EnviaFlag(de,UFLAG_OKVOX,FALSE);
				}
				else
				{
					strcpy(p1,"Peticion de Voz, Pulsa Voz para aceptarla.");
				}
				EsMensaje(tmpx);			
				_beginthread(Suena,NULL,NULL);
			}
		}
		if ((_userflags[item] & UFLAG_OKVOX))
		{			
			char tmpx[512];
			char *p1;
			p1 = tmpx;

			_userflags[item] &=  ~UFLAG_OKVOX;

			*(long *)p1 = de;
			p1 += sizeof(long);
			*(long *)p1 = dame_the_name();						
			p1 += sizeof(long);
			strcpy(p1,"Llamada aceptada.");			
			EsMensaje(tmpx);			
		}
		if ((_userflags[item] & UFLAG_FINVOX))
		{
			_userflags[item] &=  ~UFLAG_FINVOX;
			if (m_conferencia && m_conferencia->m_propietario == de && m_conferencia->m_envoz)
			{
				int i,j,nips = m_conferencia->m_users->GetItemCount();
				unsigned long otros[256];
				int notros;				
				notros = 0;
				for (j = 0; j < nvoces_activas;j++)
				{					
					for (i = 0;i < nips;i++)
					{
						if (m_conferencia->m_users->GetItemData(i) == voces_activas[j])
							break;
					}
					if (i >= nips)
					{
						otros[notros] = voces_activas[j];
						notros++;
					}
				}				
				for (i = 0;i < nips;i++)
				{					
					::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,m_conferencia->m_users->GetItemData(i));
				}
				for (i = 0;i < notros;i++)
				{
					EnviaFlag(otros[i],UFLAG_FINVOX,FALSE);
					::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,otros[i]);
				}				
				m_conferencia->m_envoz = FALSE;
				/*NO_LLAMADA(m_flagq);*/
				for (i = 0;i < 500;i++)
				{
					wxString s;				
					if (m_dlgmensaje[i])						
					{
						s = m_dlgmensaje[i]->m_llamar.GetLabel();
						if (!s.Cmp("Colgar"))
						{
							m_dlgmensaje[i]->DesactivaVoz();
							/*m_dlgmensaje[i]->m_llamar.SetLabel("Voz");*/
						}
						m_dlgmensaje[i]->m_envoz = FALSE;
					}
				}
			}
			else
			{
				/*
				if (nvoces_activas == 1 && voces_activas[0] == de)
				{
					NO_LLAMADA(m_flagq);
				}
				*/
				::PostMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,de);				
				for (int i = 0;i < 500;i++)
				{
					if (m_dlgmensaje[i] && m_dlgmensaje[i]->m_ip == de)
					{
						//m_dlgmensaje[i]->m_envoz = FALSE;
						m_dlgmensaje[i]->DesactivaVoz();
						/*m_dlgmensaje[i]->m_llamar.SetLabel("Voz");*/
						if (::IsWindowVisible((HWND)m_dlgmensaje[i]->GetHandle()))					
						{
							char tmpx[512];
							char *p1;
							p1 = tmpx;			

							*(long *)p1 = de;
							p1 += sizeof(long);
							*(long *)p1 = dame_the_name();						
							p1 += sizeof(long);
							strcpy(p1,"Fin llamada.");
							EsMensaje(tmpx);			
						}
						break;
					}
				}
			}
		}
		if ((_userflags[item] & UFLAG_CONVOX))
		{
			_userflags[item] &= ~UFLAG_RING;
			_userflags[item] &= ~UFLAG_CONVOX;
			if (m_conferencia && m_conferencia->m_propietario)
			{
				int i,j,nips = m_conferencia->m_users->GetItemCount();
				int activadas;
				unsigned long otros[256];
				int notros;				
				notros = 0;
				for (j = 0; j < nvoces_activas;j++)
				{					
					for (i = 0;i < nips;i++)
					{
						if (m_conferencia->m_users->GetItemData(i) == voces_activas[j])
							break;
					}
					if (i >= nips)
					{
						otros[notros] = voces_activas[j];
						notros++;
					}
				}				

				activadas = 0;
				/*NO_LLAMADA(m_flagq);*/
				for (i = 0; i < notros;i++)
				{	
					EnviaFlag(otros[i],UFLAG_FINVOX,FALSE);
					::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,otros[i]);
				}
				for (i = 0;i < nips;i++)
				{
					if (m_conferencia->m_users->GetItemData(i) != dame_the_name())
					{
						::SendMessage(hwndMDIFrame,WM_COMMAND,MAKEWPARAM(ID_Conectar,0x2),m_conferencia->m_users->GetItemData(i));
						activadas++;
					}
				}
				/*LLAMADA(m_flagq);*/
				/*
				if (activadas)
				{
					LLAMADA(m_flagq);
				}
				*/
				m_conferencia->m_envoz = TRUE;
				m_conferencia->m_llamar.SetLabel("Colgar");
				for (i = 0;i < 500;i++)
				{
					wxString s;				
					if (m_dlgmensaje[i])						
					{
						s = m_dlgmensaje[i]->m_llamar.GetLabel();
						if (!s.Cmp("Colgar"))
						{
							m_dlgmensaje[i]->DesactivaVoz();
							/*m_dlgmensaje[i]->m_llamar.SetLabel("Voz");*/
						}
						m_dlgmensaje[i]->m_envoz = FALSE;
					}
				}
			}
		}
	}


	CListaUsers *plista;		
	int it;
	if ((it = m_listausers.PorIp(de,nombre)) >= 0)
	{
		plista = &m_listausers;
	}
	if (it >= 0 && plista)
	{
		plista->m_userflags[it] = _userflags[item];
	}
	if ((it = m_listausersg.PorIp(de,nombre)) >= 0)
	{
		plista = &m_listausersg;
	}
	if (it >= 0 && plista)
	{
		plista->m_userflags[it] = _userflags[item];
	}	
}

void CDlgPresencia::NuevosDatos(char *p1)
{
	long control;
	control = *(long *)p1;
	p1 += sizeof(long);

	if (control > controlcarga)
	{		

		LimpiaDatos();

		controlcarga = control;

		tdiapre01gz = *(long *)p1;
		p1 += sizeof(long);
		tdiapre02gz = *(long *)p1;
		p1 += sizeof(long);
		tdiapre01mas = *(long *)p1;
		p1 += sizeof(long);
		tdiapre02mas = *(long *)p1;
		p1 += sizeof(long);
		tdiapre03mas = *(long *)p1;
		p1 += sizeof(long);
		tdiapre04mas = *(long *)p1;
		p1 += sizeof(long);

		diapre01gz  = (char *)malloc(tdiapre01gz);
		if (tdiapre02gz)
		{
			diapre02gz  = (char *)malloc(tdiapre02gz);
		}
		else
		{
			diapre02gz  = NULL;
		}
		diapre01mas = (char *)malloc(tdiapre01mas);
		diapre02mas = (char *)malloc(tdiapre02mas);
		diapre03mas = (char *)malloc(tdiapre03mas);
		diapre04mas = (char *)malloc(tdiapre04mas);
		
		
		PideChunk();
		
	}
	/* enviar validacion */
	long name = dame_the_name();
	char mensaje[256];
	char *p;
	int e;

	p = mensaje;
	e = 8; // codigo mensaje (validacion datos)
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	*((long *)p) = control;
	p += sizeof(long);	
	pon_en_cola_envio(mensaje,sizeof(int)+sizeof(long),FALSE);
	return;
}

void CDlgPresencia::RecibeChunk(char *p1)
{
	long control;
	long desde,hasta,tama;
	int cual;
	char *p;
	long *tot;
	int micual;

	control = *(long *)p1;
	p1 += sizeof(long);

	micual = *((int *)p1);		

	if (micual < 6)
	{
		if (control != controlcarga)
		{
			return;
		}
	}
	else
	{
		if (control != controlmio)
		{
			return;
		}
	}

	cual = -1;
	if (ltdiapre01gz < tdiapre01gz)
	{
		cual = 0;
		tama = tdiapre01gz - ltdiapre01gz;
		desde = ltdiapre01gz;		
		p = diapre01gz;
		tot = &ltdiapre01gz;
	}
	else if (ltdiapre02gz < tdiapre02gz)
	{
		cual = 1;
		tama = tdiapre02gz - ltdiapre02gz;
		desde = ltdiapre02gz;				
		p = diapre02gz;
		tot = &ltdiapre02gz;
	}
	else if (ltdiapre01mas < tdiapre01mas)
	{
		cual = 2;
		tama = tdiapre01mas - ltdiapre01mas;
		desde = ltdiapre01mas;				
		p = diapre01mas;
		tot = &ltdiapre01mas;
	}
	else if (ltdiapre02mas < tdiapre02mas)
	{
		cual = 3;
		tama = tdiapre02mas - ltdiapre02mas;
		desde = ltdiapre02mas;				
		p = diapre02mas;
		tot = &ltdiapre02mas;
	}
	else if (ltdiapre03mas < tdiapre03mas)
	{
		cual = 4;
		tama = tdiapre03mas - ltdiapre03mas;
		desde = ltdiapre03mas;				
		p = diapre03mas;
		tot = &ltdiapre03mas;
	}
	else if (ltdiapre04mas < tdiapre04mas)
	{
		cual = 5;
		tama = tdiapre04mas - ltdiapre04mas;
		desde = ltdiapre04mas;
		p = diapre04mas;
		tot = &ltdiapre04mas;
	}
	else if (ltxiapre02gz < txiapre02gz)
	{
		cual = 6;
		tama = txiapre02gz - ltxiapre02gz;
		desde = ltxiapre02gz;				
		p = xiapre02gz;
		tot = &ltxiapre02gz;
	}

	if (cual < 0)
	{
		return; /* ya esta todo recibido */
	}

	if (micual != cual)
		return;

	p1 += sizeof(int);

	if (*((long *)p1) != desde)
		return;
	p1 += sizeof(long);

	hasta = *((long *)p1);
	p1 += sizeof(long);

	if (hasta > (desde+tama))
		return;

	tama = hasta - desde;
	p += desde;
	memcpy(p,p1,tama);
	(*tot) = hasta;

	chunkrecibido++;

    PideChunk();
}


int CDlgPresencia::DameImagen(long ip,int estado,int flag,int tipo)
{
	int i;
	
	for (i = 0;i < 500;i++)
	{
		if (m_imagenes[i] == ip)
		{
			return EstadoImagen(i,estado,flag,tipo);
		}				
	}

	i = m_imageListPatron->Add(wxBitmap("BMPNULO",wxBITMAP_TYPE_BMP_RESOURCE));
	i = m_imageListNormal->Add(wxBitmap("BMPNULO",wxBITMAP_TYPE_BMP_RESOURCE));
	if (i >= 0)
	{
		int rr,size,file;
		char tmp[10000];

		m_imagenes[i] = ip;
		m_faltaimagen[i] = 1;		
		sprintf(tmp,ajusta_a_home_dir("\\dsp\\%lX.dsi"),ip);
		file = open(tmp,O_RDONLY|O_BINARY);
		if (file >= 0)
		{
			size = read(file,tmp,10000);
			close(file);
			EsFoto(tmp,-size);
		}
		rr = EstadoImagen(i,estado,flag,tipo);
		return rr;
	}

	return 0;
}

int CDlgPresencia::EstadoImagen(int imagen, int estado,int flag,int tipo)
{
	if (estado < 0)
	{
		return imagen;
	}
	if ((tipo & TIPO_OCULTO))
	{
	    flag |= FLAG_OCULTO;
	}
	else
	{
		flag &= ~FLAG_OCULTO;
	}
	estado |= ((flag << 16) & 0xffff0000);
	if (m_imaestados[imagen] != estado)
	{
		m_imaestados[imagen] = estado;
		PonImagenEstado(imagen);
	}
	return imagen;
}

void CDlgPresencia::PonImagenEstado(int imagen)
{
	int flag;
	int imaestado = m_imaestados[imagen];
	if (imaestado == 0xffffffff)
		return;

	flag = (((imaestado&0xffff0000) >> 16) & 0xffff);
	wxBitmap bmp1(58,58);
	{
		unsigned char r,g,b;
		unsigned char r1,g1,b1;
		unsigned char ri,gi,bi;
		unsigned char *p;
		int i;
		wxMemoryDC temp_dc;				
		wxColour color;
		temp_dc.SelectObject(bmp1);		
		
        m_imageListPatron->Draw(imagen,temp_dc,0,0,wxIMAGELIST_DRAW_NORMAL,TRUE);		

		p = (unsigned char *)&(colores[(imaestado & 0xffff)]);
		r = (*p++ - 48) * 100;
		r += (*p++ - 48) * 10;
		r += (*p++ - 48);
		g = (*p++ - 48) * 100;
		g += (*p++ - 48) * 10;
		g += (*p++ - 48);
		b = (*p++ - 48) * 100;
		b += (*p++ - 48) * 10;
		b += (*p++ - 48);

		color.Set(r,g,b);

		r1 = r/10;g1 = g/10;b1 = b/10;		
		/*
		r1 = 0;g1 = 0;b1 = 0;		
		if (r > g)
		{
			if (b > r)
				b1 = 1;
			else
				r1 = 1;
		}
		else
		{
			if (g > b)
				g1 = 1;
			else
				b1 = 1;
		}
		*/
		ri = (r - r1) / 20;
		gi = (g - g1) / 20;
		bi = (b - b1) / 20;
		
		wxPen pen(color,1,wxSOLID);
		temp_dc.SetPen(pen);

		wxColour face(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
		wxColour shadow(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DSHADOW));
		wxColour darkshadow(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DDKSHADOW));
		wxColour light(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DLIGHT));
		wxColour hilight(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DHILIGHT));

		for (i = 0;i < 58;i++)
		{
			pen.SetColour(r1,g1,b1);
			temp_dc.SetPen(pen);
			if (i < 20)
			{
				r1 += ri;g1 += gi;b1 += bi;
			}
			else if (i > 37)
			{
				r1 -= ri;g1 -= gi;b1 -= bi;
			}
			if (i < 7 || i > 50)
			{
				temp_dc.DrawLine(0,i,58,i);
			}
			else
			{
				temp_dc.DrawLine(0,i,7,i);
				temp_dc.DrawLine(51,i,58,i);
			}
		}

		
		int m_borderSize = 2;
		int w = 48;
		int h = 48;
		int x = 5;
		int y = 5;
		pen.SetColour(shadow);
		temp_dc.SetPen(pen);
        
        temp_dc.DrawLine(x+m_borderSize-2, y+m_borderSize-2, x+w-m_borderSize+1, y+m_borderSize-2);
        temp_dc.DrawLine(x+m_borderSize-2, y+m_borderSize-2, x+m_borderSize-2, y+h-m_borderSize+1);

		pen.SetColour(darkshadow);
		temp_dc.SetPen(pen);
        temp_dc.DrawLine(x+m_borderSize-1, y+m_borderSize-1, x+w-m_borderSize, y+m_borderSize-1);
        temp_dc.DrawLine(x+m_borderSize-1, y+m_borderSize-1, x+m_borderSize-1, y+h-m_borderSize);

		pen.SetColour(hilight);
		temp_dc.SetPen(pen);        
        temp_dc.DrawLine(x+m_borderSize - 2, y+h-m_borderSize+1, x+w-m_borderSize+1, y+h-m_borderSize+1);
        temp_dc.DrawLine(x+w-m_borderSize+1, y+m_borderSize - 2, x+w-m_borderSize+1, y+h-m_borderSize+2); 

		pen.SetColour(light);
		temp_dc.SetPen(pen);        
        temp_dc.DrawLine(x+w-m_borderSize, y+m_borderSize-1, x+w-m_borderSize, y+h-m_borderSize); 
        temp_dc.DrawLine(x+m_borderSize-1, y+h-m_borderSize, x+w-m_borderSize+1, y+h-m_borderSize);
		

		
		if ( (flag & FLAG_LLAMADA))
		{			
			wxBitmap bitmap;
			if (bitmap.LoadFile("LLAMADA",wxBITMAP_TYPE_BMP_RESOURCE))
			{
				temp_dc.DrawBitmap(bitmap,0,42, false);
			}
		} 
		if ( (flag & FLAG_CONFAMO))
		{			
			wxBitmap bitmap;
			if (bitmap.LoadFile("CONFAMO",wxBITMAP_TYPE_BMP_RESOURCE))
			{
				temp_dc.DrawBitmap(bitmap,42,42, false);
			}
		} 
		if ( (flag & FLAG_ENCONF))
		{			
			wxBitmap bitmap;
			if (bitmap.LoadFile("ENCONF",wxBITMAP_TYPE_BMP_RESOURCE))
			{
				temp_dc.DrawBitmap(bitmap,42,42, false);
			}
		} 
		if (m_privilegio >= 30 && (imaestado & 0xffff) == 2)
		{
			wxBitmap bitmap;
			if (bitmap.LoadFile("SLEEPING",wxBITMAP_TYPE_BMP_RESOURCE))
			{
				temp_dc.DrawBitmap(bitmap,42,0, false);
			}
		}
		if (m_privilegio >= 30 && (flag & FLAG_OCULTO))
		{
			wxBitmap bitmap;
			if (bitmap.LoadFile("OCULTO",wxBITMAP_TYPE_BMP_RESOURCE))
			{
				temp_dc.DrawBitmap(bitmap,0,0, false);
			}
		}
		
	}
	m_imageListNormal->Replace(imagen,bmp1);
}


void CDlgPresencia::PideChunk()
{
	long name = dame_the_name();
	char mensaje[256];
	char *p;
	int e;
	long desde,hasta,tama;
	int cual;
	

	p = mensaje;
	e = 7; // codigo mensaje (trozo fichero)
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);

	cual = -1;
	if (ltdiapre01gz < tdiapre01gz)
	{
		cual = 0;
		tama = tdiapre01gz - ltdiapre01gz;
		desde = ltdiapre01gz;		
	}
	else if (ltdiapre02gz < tdiapre02gz)
	{
		cual = 1;
		tama = tdiapre02gz - ltdiapre02gz;
		desde = ltdiapre02gz;				
	}
	else if (ltdiapre01mas < tdiapre01mas)
	{
		cual = 2;
		tama = tdiapre01mas - ltdiapre01mas;
		desde = ltdiapre01mas;				
	}
	else if (ltdiapre02mas < tdiapre02mas)
	{
		cual = 3;
		tama = tdiapre02mas - ltdiapre02mas;
		desde = ltdiapre02mas;				
	}
	else if (ltdiapre03mas < tdiapre03mas)
	{
		cual = 4;
		tama = tdiapre03mas - ltdiapre03mas;
		desde = ltdiapre03mas;				
	}
	else if (ltdiapre04mas < tdiapre04mas)
	{
		cual = 5;
		tama = tdiapre04mas - ltdiapre04mas;
		desde = ltdiapre04mas;				
	}
	else if (controlmio && ltxiapre02gz < txiapre02gz)
	{
		cual = 6;
		tama = txiapre02gz - ltxiapre02gz;
		desde = ltxiapre02gz;
	}

	if (cual < 0)
	{
		GrabaLosDatos();
		return;
	}

	if (tama > 6000)
	{
		tama = 6000;
	}
	hasta = desde+tama;


	if (cual < 6)	
		*((long *)p) = controlcarga;
	else
		*((long *)p) = controlmio;
	p += sizeof(long);
	*((int *)p) = cual;
	p += sizeof(int);
	*((long *)p) = desde;
	p += sizeof(long);
	*((long *)p) = hasta;
	p += sizeof(long);


	m_tiempochunk = time(NULL) + 180;

	pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
}


void CDlgPresencia::LimpiaDatos()
{
		tdiapre01gz = 0;
		
		tdiapre02gz = 0;
		
		tdiapre01mas = 0;
		
		tdiapre02mas = 0;
		
		tdiapre03mas = 0;		

		tdiapre04mas = 0;		

		if (diapre01gz)
			free(diapre01gz);
		if (d_diapre01)
			free(d_diapre01);

		d_diapre01 = NULL;
		td_diapre01 = 0;

		if (diapre02gz)
			free(diapre02gz);
		if (diapre01mas)
			free(diapre01mas);
		if (diapre02mas)
			free(diapre02mas);
		if (diapre03mas)
			free(diapre03mas);
		if (diapre04mas)
			free(diapre04mas);

		diapre01gz  = NULL;
		diapre02gz  = NULL;
		diapre01mas = NULL;
		diapre02mas = NULL;
		diapre03mas = NULL;
		diapre04mas = NULL;		

		ltdiapre01gz  = 0L;
		ltdiapre02gz  = 0L;
		ltdiapre01mas = 0L;
		ltdiapre02mas = 0L;
		ltdiapre03mas = 0L;
		ltdiapre04mas = 0L;

		/* Ojo, si provocamos el reenvio !!! */
		ltxiapre02gz  = 0L;
		


		controlcarga = NULL;
}

int CDlgPresencia::ActivaTicket(long otra, wxString &Nombre)
{
	long mia = dame_the_name();	

	wxSize size(300,420);
	wxString titulo = Nombre;
	titulo = "*LLAMADA PARA TECNICO: " + titulo;
	for (int i = 0;i < 500;i++)
	{
		if (m_dlgticket[i] && m_dlgticket[i]->m_ip == otra)
		{
			m_dlgticket[i]->SetTitle(titulo);
			m_dlgticket[i]->m_destino = Nombre;
			m_dlgticket[i]->Show(TRUE);
			m_dlgticket[i]->m_envia.SetFocus();
			m_dlgticket[i]->m_poractivar = FALSE;
			return i;
		}
	}
	for (i = 0;i < 500;i++)
	{
		if (!m_dlgticket[i])
		{			
			m_dlgticket[i] = new CDlgTicket(this,-1, titulo,wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
			m_dlgticket[i]->m_ip = otra;
			m_dlgticket[i]->m_destino = Nombre;
			m_dlgticket[i]-> CenterOnScreen();
			wxFont ff;
			ff =  m_dlgticket[i]->GetFont();
			ff.SetWeight(wxBOLD);
			m_dlgticket[i]->SetFont(ff);
			m_dlgticket[i]->Show(TRUE);
			m_dlgticket[i]->m_envia.SetFocus();
			m_dlgticket[i]->m_poractivar = FALSE;
			return i;
		}
	}
	return -1;    
}

extern "C"
{
extern SOCKET sCommand;
extern SOCKET sEnvio;
}

void CDlgPresencia::fichero_gordo(char *p)
{
	int modo;
	char path[256];
	int len;
	long desde;
	int file;

	memcpy(&modo,p,sizeof(int));
	p += sizeof(int);
	memcpy(path,p,128);
	p += 128;
	memcpy(&desde,p,sizeof(long));
	p += sizeof(long);
	memcpy(&len,p,sizeof(int));
	p += sizeof(int);

	if (!modo)
	{
		unlink(path);
	}
	file = open(path,O_RDWR | O_BINARY | O_CREAT,S_IWRITE|S_IREAD);
	if (file >= 0)
	{
		lseek(file,desde,SEEK_SET);
		write(file,p,len);
		close(file);
	}
	if (modo == 777)
	{
		extern SOCKET servidor;
		char tmp[512];
		int l;
		long voces[256];
		int i,n;
		n = nvoces_activas;
		// Actualizacion 
		for (i = 0;i < nvoces_activas;i++)
		{
			voces[i] = voces_activas[i];
		}
		for (i = 0;i < n;i++)
		{
			if (voces[i])
			{
				EnviaFlag(voces[i],UFLAG_FINVOX,FALSE);
				::SendMessage(hwndMDIFrame,WM_COMMAND,ID_Terminar,voces[i]);
			}			
		}
		if (m_conferencia->m_propietario)
		{
			if (m_conferencia->m_propietario == dame_the_name())
			{			
				int i,nips = m_conferencia->m_users->GetItemCount();
				long ips[512];					
				for (i = 0;i < nips;i++)
				{
					ips[i] = m_conferencia->m_users->GetItemData(i);					
				}	
				QuitaDeConferencia(nips,ips);
			}
			else
			{
				QuitaDeConferencia(1,&(m_conferencia->m_propietario));
			}
		}
		if (KillOthers("dspresenci") > 0)
		{
			Sleep(2000);
		}
		tmp[0] = '}';
		strcpy(tmp+1,file_Origen);
		l = strlen(tmp);
		tmp[l++] = ' ';
		tmp[l++] = ':';
		memcpy(tmp+l,m_Passwd,20);
		l += 20;
		tmp[l++] = ':';
		tmp[l] = 0;		
		m_tcontrol->Kill();
		m_timer.Stop();
		SacaDelTray(GetId());
		closesocket(servidor);
		{
			if (hwndMDIFrame)
			{
				DestroyWindow(hwndMDIFrame);				
			}
		}
		if (sCommand != (SOCKET)-1)
		{
			closesocket(sCommand);
			if (sCommand != sEnvio)
			{
				closesocket(sEnvio);
			}
		}
		closesocket(servidor);	

		_execl( path, path, tmp, NULL );
		exit(0);
	}
}


void CDlgPresencia::EnviaComando(int comando)
{

	char mensaje[1024];
	char *p;
	int e;				


	if (m_nombreinfo.Cmp(m_selNombre))
	{
		m_ndiasinfo = 0;
	}

	p = mensaje;
	e = 19; // informacion
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&m_selip,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&comando,sizeof(long));
	p += sizeof(long);
	memcpy(p,(void *)&m_ndiasinfo,sizeof(int));
	p += sizeof(int);
	
	pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);

}


void CDlgPresencia::InformacionEspecial(char *p)
{
	struct log_reg *reg;
	char *preg;
	long de;
	int tipo;
	char tmp[512];

	de = *((long *)p);
	p += sizeof(long);
	tipo = *((int *)p);
	p += sizeof(int);
	reg = (struct log_reg *)p;
	preg = p;
	wxString nombre;
	int item,i;	
	if ((item = ListaPorIp(de,nombre)) < 0)
	{
		return;
	}
	wxString s;
	if (!m_dlginfo)
	{		
		m_dlginfo = new CDlgLista(NULL,NULL,0,this,-1,"Log",wxDefaultPosition,wxSize(600,400));
		m_dlginfo->m_modeless = TRUE;		
		m_dlginfo->m_ok.SetLabel("Mas");
		m_dlginfo->m_cancel.SetLabel("Salir");
		m_dlginfo->m_list.InsertColumn(0,"Fecha",wxLIST_FORMAT_LEFT,-1);	
		m_dlginfo->m_list.InsertColumn(1,"Hora",wxLIST_FORMAT_LEFT,-1);	
		m_dlginfo->m_list.InsertColumn(2,"E",wxLIST_FORMAT_LEFT,-1);	
		m_dlginfo->m_list.InsertColumn(3,"Estado",wxLIST_FORMAT_LEFT,-1);	
		m_dlginfo->m_list.InsertColumn(4,"Parte",wxLIST_FORMAT_LEFT,-1);	
		m_dlginfo->m_ninfo = 5;
		m_dlginfo->Center();
	}
	if (!tipo)
	{		
		s = "Log de ";
		s += nombre;
		m_nombreinfo = nombre;
		s += " desde ";

		wxDate fecha1(reg->mes-1,reg->dia,2000+reg->ano);
		wxDate fecha2;
		fecha2.Set();

		long diff = fecha2.GetJulianDate() - fecha1.GetJulianDate();
		if (diff > m_ndiasinfo)
		{
			m_ndiasinfo = diff;
		}

		diff = fecha2.GetJulianDate() - m_ndiasinfo;
		fecha1.Set(diff);

		sprintf(tmp,"%02d-%02d-%02d",fecha1.GetYear()%100,fecha1.GetMonth(),fecha1.GetDay());

		s += tmp;

		m_dlginfo->SetLabel(s);
		m_dlginfo->m_list.DeleteAllItems();
		m_dlginfo->Show(TRUE);
	}

	int itm = m_dlginfo->m_list.GetItemCount();
	int idx = tipo & 0xffff0000;
	idx >>= 16;

	if (idx == -1)
	{
		/* es un parte */
		wxListItem info;
		wxString s1;
		char tmp2[10];

		if (!d_diapre01)
		{
			FI_DEF *fi;
			fi = (FI_DEF *)malloc(sizeof(FI_DEF));
			if (lee_def("\2",fi,1) < 0)
			{
				free((void *)fi);
			}
			else
			{
				lfich_diapre01 = fi->f_lfich+1;
				DescomprimeDatos(diapre01gz,tdiapre01gz,&d_diapre01,&td_diapre01);
				free_def(fi,1);
			}
		}

		memcpy(tmp,preg+8+2,2);
		tmp[2] = '-';
		memcpy(tmp+3,preg+8+4,2);
		tmp[5] = '-';
		memcpy(tmp+6,preg+8+6,2);
		tmp[8] = 0;

		memcpy(tmp2,preg+16,5);
		tmp2[5] = ':';
		tmp2[6] = 0;

		for (i = 0;i < itm;i++)
		{
			info.SetMask(wxLIST_MASK_TEXT);
			info.SetId(i);
			info.SetColumn(0);
			m_dlginfo->m_list.GetItem(info);
			s1 = info.GetText();						
			if (memcmp(tmp,s1,8) <= 0 || i == (itm-1))
			{
				info.SetMask(wxLIST_MASK_TEXT);
				info.SetId(i);
				info.SetColumn(1);
				m_dlginfo->m_list.GetItem(info);
				s1 = info.GetText();				
				if (memcmp(tmp2,s1,6) <= 0 || i == (itm-1))
				{
					tmp2[5] = 0;
					s = tmp2;
					s += " ";
					sprintf(tmp,"%06.0lf",*((double *)(preg+42)));
					s += tmp;
					s += "-";
					sprintf(tmp,"%d",((int)preg[50])*256+preg[51]);
					s += tmp;
					s += "(";
					memcpy(tmp,preg+36,6);					
					tmp[6] = 0;
					if (d_diapre01)
					{
						for (int j = 0;j < td_diapre01;j += lfich_diapre01)
						{
							if (!memcmp(tmp,d_diapre01+j,6))
							{
								tmp[6] = ' ';
								memcpy(tmp+7,d_diapre01+j+6,30);
								tmp[37] = 0;
								break;
							}
						}
					}
					s += tmp;
					s.Trim();
					s += ") ";
					memcpy(tmp,preg+52,80);
					tmp[80] = 0;
					s += tmp;
					s.Trim();
					m_dlginfo->m_list.SetItem(i,4,s);
					break;
				}				
			}
		}

	}
	else
	{	
		for (;itm < idx;itm++)
		{
			m_dlginfo->m_list.InsertItem(itm,"");
		}	

		sprintf(tmp,"%02d-%02d-%02d",reg->ano%100,reg->mes,reg->dia);
		m_dlginfo->m_list.InsertItem(idx,tmp);
		sprintf(tmp,"%02d:%02d:%02d",reg->hora,reg->minuto,reg->segundo);
		m_dlginfo->m_list.SetItem(idx,1,tmp);
		sprintf(tmp,"%d",reg->estado);
		m_dlginfo->m_list.SetItem(idx,2,tmp);
		s = reg->dEstado;
		int i;
		i = s.Find(10);
		if (i > -1)
		{
			s.Truncate(i);
		}
		m_dlginfo->m_list.SetItem(idx,3,s);
	}

	for (i = 0;i < m_dlginfo->m_ninfo;i++)
	{
		m_dlginfo->m_list.SetColumnWidth(i,wxLIST_AUTOSIZE);
	}
}
