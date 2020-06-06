
// DlgExplorer.cpp: implementation of the CDlgExplorer class.
//
//////////////////////////////////////////////////////////////////////

#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif

#include "wx/wxprec.h"
#include "wx/date.h"


#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "DlgExplorer.h"
#include "DlgAtender.h"


extern "C"
{
extern void (*write_interno)(int fd,unsigned char *buf,unsigned tam);
extern unsigned (*read_interno)(int *pfd,unsigned char *buf,unsigned tam);
extern int gzip_asignada_memoria;
void write_gunzip(int fd,unsigned char *buf,unsigned tam);
unsigned read_gunzip(int *pfd,unsigned char *buf,unsigned tam);
int ds___gzip(int,int,int,int);

extern long iplocal[4096];
extern long ipvoz[4096];

extern HWND hwndMDIFrame;

char *quita_path(char *ff);

}

int ListaPorIp(long de,wxString &nombre);



static void FastSocket(SOCKET s)
{
   int i;
   char tmp[256];
   i = 1;
   if (setsockopt(s,IPPROTO_TCP,TCP_NODELAY,(const char *)&i,sizeof(i)) < 0)
   {
      i = 1;
      memcpy(tmp,&i,sizeof(int));
      setsockopt(s,IPPROTO_TCP,TCP_NODELAY,tmp,sizeof(tmp));
   }  
}


#define TIMER_ID (wxID_HIGHEST + 2)
#define TIMER_IDTOUT (wxID_HIGHEST + 3)

extern char *write_datos;
extern long write_long;

void write_gunzip(int fd,unsigned char *buf,unsigned tam);

extern char *read_datos;
extern long read_long;

unsigned read_gunzip(int *pfd,unsigned char *buf,unsigned tam);

static int EnviaPPaquete(SOCKET sock,PPaquete *sendbuf,int *enviados)
{
    int bytessent,totalbytes;
    int i;

	if (!*enviados)
	{
		totalbytes = LEN_CABECERA;
		bytessent = 0;
		while(bytessent < totalbytes)
		{
		   i = send(sock,((char *)&(sendbuf->c)) + bytessent,totalbytes - bytessent,0);
		   if (i == SOCKET_ERROR)
		   {
	#ifdef FGVDOS
			  if ((i = WSAGetLastError()) != WSAEWOULDBLOCK && i != WSAEINPROGRESS && i != WSAENETRESET && i != WSAEINTR && i != WSAENOBUFS)
			  {             
				 return FALSE;
			  }
	#else
			  if (errno != EWOULDBLOCK)
			  {
				 return FALSE;
			  }
	#endif

			  Sleep(10);
			  return TRUE;
			  /*continue;*/
		   }
		   else
		   {
			  bytessent += i;			  
			  if (!bytessent)
				  return TRUE;
			  if (bytessent < totalbytes)
			  {
				  Sleep(200);
			  }
		   }
		}
		*enviados = totalbytes;
	}

	bytessent  = (*enviados)-LEN_CABECERA;	
	totalbytes = LEN_DATA(sendbuf) - bytessent;
	
	while(bytessent < totalbytes)
	{
	   i = send(sock,((char *)(sendbuf->buffer)) + bytessent,totalbytes - bytessent,0);
	   if (i == SOCKET_ERROR)
	   {
#ifdef FGVDOS
		  if ((i = WSAGetLastError()) != WSAEWOULDBLOCK && i != WSAEINPROGRESS && i != WSAENETRESET && i != WSAEINTR && i != WSAENOBUFS)
		  {         
			 return FALSE;
		  }
#else
		  if (errno != EWOULDBLOCK)
		  {
			 return FALSE;
		  }
#endif

		  Sleep(10);
		  return TRUE;
		  /*continue;*/
	   }
	   else
	   {
		  bytessent += i;
		  *enviados += i;
		  return TRUE;
	   }
	}
	
    return TRUE;            
}

/* OJO: esta cola es simple no multihebrada ... es para uso por sockect asyncrona o imitacion de la misma */
struct ColaPPaquete
{
	PPaquete *ppaquete;
	int tam;
	int enviados;
	DWORD cuandocola;
	void *siguiente;
	/*struct ColaPPaquete *siguiente;*/
};
static int ElementosCola = 0;
static struct ColaPPaquete *inicolapaquete = NULL;
static struct ColaPPaquete *fincolapaquete = NULL;
static int EncolaPPaquete(PPaquete *sendbuf)
{
	struct ColaPPaquete *cola;		
	cola = (struct ColaPPaquete *)malloc(sizeof(struct ColaPPaquete));
	cola->tam = LEN_CABECERA + LEN_DATA(sendbuf);
	cola->ppaquete = (PPaquete *)malloc(sizeof(PPaquete)+LEN_DATA(sendbuf));	
	memcpy((void *)cola->ppaquete,(void *)sendbuf,sizeof(PPaquete));	
	cola->ppaquete->buffer = (unsigned char *)cola->ppaquete + sizeof(PPaquete);	
	memcpy(cola->ppaquete->buffer,sendbuf->buffer,LEN_DATA(sendbuf));
	cola->enviados = 0;
	cola->siguiente = NULL;
	cola->cuandocola = ::GetTickCount();
	if (!inicolapaquete)
	{
		inicolapaquete = cola;
	}
	else
	{
		fincolapaquete->siguiente = (void *)cola;
	}
	fincolapaquete = cola;	
	ElementosCola++;
	return 1;
}

static int EnviaColaPPaquete(SOCKET sock)
{
	struct ColaPPaquete *cola;
	while(inicolapaquete)
	{
		cola = inicolapaquete;
		if (!EnviaPPaquete(sock,cola->ppaquete,&cola->enviados))
		{
			return -1;
		}
		if (cola->enviados < cola->tam)
			return 0;
		inicolapaquete = (struct ColaPPaquete *)cola->siguiente;
		free((void *)cola->ppaquete);
		free((void *)cola);
		ElementosCola--;
	}
	if (!inicolapaquete)
	{
		fincolapaquete = NULL;
		return 2;
	}	
	return 1;
}

static void BorraColaPPaquete()
{
	struct ColaPPaquete *cola;
	while(inicolapaquete)
	{
		cola = inicolapaquete;
		inicolapaquete = (struct ColaPPaquete *)cola->siguiente;
		free((void *)cola->ppaquete);
		free((void *)cola);
	}
	inicolapaquete = NULL;
	fincolapaquete = NULL;	
	ElementosCola = 0;
}

static int RecibePaquete(SOCKET sock, Paquete *recvbuf,int tout,int *puntoread)
{	
	/*
	fd_set setread;
	fd_set seterror;
	struct timeval timeout;
	struct timeval *ptm = NULL;
	*/
	unsigned long cuanto;
	int readsize, totalbytesread, msgsize, bytesread/*,err*/;
	int maximo = 0;

	/*
	if (tout)
	{
		timeout.tv_sec  = tout/1000;
		timeout.tv_usec = (tout%1000)*1000;
		ptm = &timeout;
	}
	*/
	if (!*puntoread)
	{
		/*
		do
		{
			FD_ZERO(&setread);
			FD_SET(sock,&setread);
			FD_ZERO(&seterror);
			FD_SET(sock,&seterror);
			maximo = sock+1;
			if ((err = select(maximo,&setread,NULL,&seterror,ptm)) == SOCKET_ERROR)
			{
				return(SOCKET_ERROR);
			}
			if (!err)  
			{
				return(0); // timeout 
			}
	*/
		
			cuanto = 0L;
			if (ioctlsocket(sock,FIONREAD,&cuanto) == SOCKET_ERROR)
			{
				return SOCKET_ERROR;
			}
			if (!cuanto)
			{
				return(0);
			}
			/*
			if (cuanto >= LEN_CABECERA)
			{
				break;
			}
		} while(-1);	
		*/
			if (cuanto < LEN_CABECERA)
				return 0;

		readsize = LEN_CABECERA;	
		if((totalbytesread = recv(sock, (char *)&(recvbuf->c), readsize,0/*MSG_WAITALL*/)) == SOCKET_ERROR)
		{		
			return SOCKET_ERROR;
		}	

		if(recvbuf->c.id != DSWEBCALLID)
		{		
			if (recvbuf->c.id == 0x7117)
			{
				/* debug, no deberia recibir esto */
				readsize = recvbuf->c.command; 
				bytesread = recv(sock,((char *)recvbuf->buffer),readsize,0);
				return 0;
			}
			return SOCKET_ERROR;		
		}	
		*puntoread = readsize;
	}	
	

	totalbytesread = (*puntoread)-LEN_CABECERA;
	msgsize = LEN_DATA(recvbuf);
	readsize = msgsize-totalbytesread;
	
	while(totalbytesread < msgsize)
	{		
		cuanto = 0L;
		ioctlsocket(sock,FIONREAD,&cuanto);
		if (cuanto <= 0)
			return 0;
		if ((int)cuanto > readsize)
			cuanto = readsize;

		if((bytesread = recv(sock,
			((char *)(recvbuf->buffer)) + totalbytesread,
			cuanto,
			0)) == SOCKET_ERROR)
		{
			int i;
#ifdef FGVDOS
		  if ((i = WSAGetLastError()) != WSAEWOULDBLOCK && i != WSAEINPROGRESS && i != WSAENETRESET && i != WSAEINTR && i != WSAENOBUFS)
		  {
			  
			  return SOCKET_ERROR;
		  }										  
#else
		  if (errno != EWOULDBLOCK)
		  {
			  return SOCKET_ERROR;
		  }
#endif
		  Sleep(10);
		  continue;
		}
		totalbytesread += bytesread;
		readsize -= bytesread;
		*puntoread += bytesread;
	}
	
	*puntoread = 0;
	return 1;
}

BEGIN_EVENT_TABLE(CEnvioHandle, wxProgressDialog)
END_EVENT_TABLE()

CEnvioHandle::CEnvioHandle(
		BOOL recibe,
		int size,
		const wxString &origen,
		const wxString &destino,		
		CDlgExplorer *explorer,
		int idx,
        const wxString& title, 
		const wxString& message,  
		int maximum,
		wxWindow * parent,
		int style
		) : wxProgressDialog(title,message,maximum,parent,style)
{
	memset((void *)m_timeout,0,sizeof(DWORD)*MAXPEDIDOS);
	m_npedidos = 0;
	m_explorer = explorer;
	m_idx = idx;
	m_handleo = -1;
	m_handled = -1;
	m_recibe = recibe;
	if (!size && !recibe)
	{
		struct _stat buf;
	    if (_stat( origen, &buf ) >= 0)
		{
			size = buf.st_size;
		}
	}
	m_size = size;
	m_pedidohasta = 0;
	m_recibidototal = 0;
	m_origen = origen;
	m_destino = destino;
	m_time = time(NULL);	
	m_status = 0;
	SetWindowStyleFlag(GetWindowStyle() | wxSTAY_ON_TOP);	
	SetFocus();	
	explorer->m_envio[m_idx] = this;	
	if (m_explorer->m_version < 5)
	{
		if (m_explorer->m_esperaopen == -1)
		{
			m_explorer->m_esperaopen = m_idx;
			PideOpen();
		}
	}
	else
		PideOpen();
}

CEnvioHandle::~CEnvioHandle()
{
	if (m_recibe)
	{
		if (m_handled > -1)
			close(m_handled);
		if (m_handleo > -1)
			m_explorer->EnviaCDATA(4,(char *)&m_handleo,sizeof(int));
		m_handled = -1;
		m_handleo = -1;
	}
	else
	{
		if (m_handleo > -1)
			close(m_handled);
		if (m_handled > -1)
			m_explorer->EnviaCDATA(4,(char *)&m_handled,sizeof(int));
		m_handleo = -1;
		m_handled = -1;
	}
}

int CEnvioHandle::PideOpen()
{
	if (m_status > 1)
		return -1;

	char tmp[1024],*p;	
	p = tmp;
	if (m_recibe)
	{
		*((int *)p) = O_RDONLY | O_BINARY;
		p += sizeof(int);
		*((int *)p) = S_IREAD | S_IWRITE;
		p += sizeof(int);
		strcpy(p,m_origen);
	}
	else
	{
		*((int *)p) = O_CREAT|O_RDWR|O_BINARY|O_TRUNC;
		p += sizeof(int);
		*((int *)p) = S_IREAD | S_IWRITE;
		p += sizeof(int);
		strcpy(p,m_destino);
	}
	m_time = time(NULL);
	m_status = 1;
	m_explorer->EnviaCDATA(3,tmp,sizeof(int)*2+strlen(p)+1);
	return 0;

}

int CEnvioHandle::Open(int fd)
{
	char tmp[4080];
	int *p2;	
	/*int e;*/

	if (m_status != 1)
		return -1;
	if (fd < 0)
	{
		{
			wxString s;
			if (m_recibe)
				s = m_origen;
			else
				s = m_destino;
			s = "No se puede abrir " + s;
			::MessageBox((HWND)GetHandle(),s,"ATENCION",MB_TOPMOST|MB_ICONHAND|MB_OK);
		}
		m_recibidototal = m_size;
		return -1;
	}
	m_status = 2;		

	if (m_explorer->m_esperaopen == m_idx)
	{
		m_explorer->m_esperaopen = -1;
		for (int i = 0;i < MAXENVIOS;i++)
		{
			if (m_explorer->m_envio[i] && m_explorer->m_envio[i]->m_status == 0)
			{
				m_explorer->m_esperaopen = m_explorer->m_envio[i]->m_idx;
				m_explorer->m_envio[i]->PideOpen();				
				break;
			}
		}
	}

	if (m_recibe)
	{
		m_handleo = fd;
		m_handled = open(m_destino,O_CREAT|O_RDWR|O_BINARY|O_TRUNC,S_IREAD|S_IWRITE);
		if (m_handled < 0)
		{
			wxString s;
			s = m_destino;
			s = "No se puede abrir " + s;
			::MessageBox((HWND)GetHandle(),s,"ATENCION",MB_TOPMOST|MB_ICONHAND|MB_OK);
			m_recibidototal = m_size;
		}
		else for (int i = 0;i < MAXPEDIDOS && m_pedidohasta < m_size;i++)
		{
			p2 = (int *)tmp;
			p2[0] = m_handleo;
			p2[1] = m_pedidohasta;
			p2[2] = 4000;
			m_time = time(NULL);
			m_explorer->EnviaCDATA(22,tmp,sizeof(int)*3);
			m_pedidos[i] = m_pedidohasta;
			m_timeout[i] = m_time;			
			m_pedidohasta += 4000;
			m_npedidos = i + 1;			
			if (m_explorer->m_directo != INVALID_SOCKET)
			{				
				/*
				if ((e = EnviaColaPPaquete(m_explorer->m_directo)) < 0)
				{
					BorraColaPPaquete();
					closesocket(m_explorer->m_directo);
					m_explorer->m_punrec = 0;
					m_explorer->m_directo = INVALID_SOCKET;						
					break;
				}
				if (!e)
				{
					break;
				}
				*/
			}
			else if (i >= MAXPORSERVER)
				break;
		}
	}
	else
	{
		m_handleo = open(m_origen,O_RDONLY|O_BINARY);						
		m_handled = fd;
		int tam;
		if (m_handleo < 0)
		{
			wxString s;
			s = m_origen;
			s = "No se puede abrir " + s;
			::MessageBox((HWND)GetHandle(),s,"ATENCION",MB_TOPMOST|MB_ICONHAND|MB_OK);
			m_recibidototal = m_size;
		}
		else for (int i = 0;i < MAXPEDIDOS && m_pedidohasta < m_size;i++)
		{
			p2 = (int *)tmp;
			p2[0] = m_handled;
			p2[1] = m_pedidohasta;
			lseek(m_handleo,m_pedidohasta,SEEK_SET);
			tam = read(m_handleo,(char *)(p2+3),4000);
			p2[2] = tam;
			m_time = time(NULL);
			m_explorer->EnviaCDATA(23,tmp,sizeof(int)*3+tam);
			m_pedidos[i] = m_pedidohasta;
			m_timeout[i] = m_time;
			m_pedidohasta += tam;
			m_npedidos = i + 1;
			if (tam < 4000)
			{
				m_pedidohasta = m_size;
				break;
			}
			if (m_explorer->m_directo != INVALID_SOCKET)
			{	
				/*
				if ((e = EnviaColaPPaquete(m_explorer->m_directo)) < 0)
				{
					BorraColaPPaquete();
					closesocket(m_explorer->m_directo);
					m_explorer->m_punrec = 0;
					m_explorer->m_directo = INVALID_SOCKET;
					break;
				}
				if (!e)
				{
					break;
				}
				*/
			}
			else if (i >= MAXPORSERVER)
				break;
		}
	}
	return 0;
}

int CEnvioHandle::Recibe(int *p2)
{
	if (m_status != 2)
		return 0;

	if (p2[0] == m_handleo)
	{
		for (int i = 0;i < m_npedidos;i++)
		{
			if (p2[1] == m_pedidos[i])
				break;
		}
		if (i >= m_npedidos)
		{
			return 1; /* paquete dupñicado ? */
		}
		if (p2[2] >= 0)
		{
			wxString s;

			m_pedidos[i] = -1;

			lseek(m_handled,p2[1],SEEK_SET);
			write(m_handled,(char *)(p2+3),p2[2]);
			m_recibidototal += p2[2];
			s = "Transfiriendo ";						
			s += wxString::Format(" %ld de %ld",m_recibidototal,m_size);
			SetLabel(s);
			if (!Update(m_recibidototal * 100 / m_size))
			{
				m_recibidototal = m_size;
			}
			else if (m_pedidohasta < m_size)
			{
				char tmp[512];
				p2 = (int *)tmp;
				p2[0] = m_handleo;
				p2[1] = m_pedidohasta;
				p2[2] = 4000;
				m_time = time(NULL);
				m_explorer->EnviaCDATA(22,tmp,sizeof(int)*3);
				m_pedidos[i] = m_pedidohasta;
				m_timeout[i] = m_time;
				m_pedidohasta += 4000;
			}
		}
		else
		{
			/* error en origen */			
			m_recibidototal = m_size;
			m_npedidos = 0;
		}
		if (m_recibidototal < m_size)
		{
			return 1;
		}
	}
	return 0;
}



int CEnvioHandle::Envia(int *p2)
{
	if (m_status != 2)
		return 0;
	if (p2[0] == m_handled)
	{
		for (int i = 0;i < m_npedidos;i++)
		{
			if (p2[1] == m_pedidos[i])
				break;
		}
		if (i >= m_npedidos)
		{
			return 1; /* paquete dupñicado ? */
		}
		if (p2[2] >= 0)
		{
			wxString s;

			m_pedidos[i] = -1;

			m_recibidototal += p2[2];
			s = "Transfiriendo ";						
			s += wxString::Format(" %ld de %ld",m_recibidototal,m_size);
			SetLabel(s);
			if (!Update(m_recibidototal * 100 / m_size))
			{
				m_recibidototal = m_size;
			}
			else if (m_pedidohasta < m_size)
			{
				char tmp[4080];
				int tam;
				p2 = (int *)tmp;
				p2[0] = m_handled;
				p2[1] = m_pedidohasta;
				lseek(m_handleo,m_pedidohasta,SEEK_SET);
				tam = read(m_handleo,(char *)(p2+3),4000);
				p2[2] = tam;
				m_time = time(NULL);
				m_explorer->EnviaCDATA(23,tmp,sizeof(int)*3+tam);
				m_pedidos[i] = m_pedidohasta;
				m_timeout[i] = m_time;
				m_pedidohasta += tam;
				if (tam < 4000)
				{
					m_pedidohasta = m_size;
				}
			}
		}
		else
		{
			/* error ? */
			m_recibidototal = m_size;
		}
		if (m_recibidototal < m_size)
		{
			return 1;
		}
	}
	return 0;
}

int CEnvioHandle::Timeouts()
{
	if (m_status == 1)
	{
		long tiempo = time(NULL) - 60;
		if (m_time < tiempo)
		{
			PideOpen();
			return 1;
		}
		return 0;
	}
	else if (m_status != 2)
		return -1;

	char tmp[4080];
	int tam;
	int ntout = 0;
	int *p2;
	long tiempo = time(NULL) - 60;

	for (int i = 0;i < m_npedidos;i++)
	{
		if (m_pedidos[i] != -1 && m_timeout[i] < tiempo)
		{
			if (inicolapaquete)
			{
				if ((::GetTickCount() - inicolapaquete->cuandocola) > 20000)
				{
					/* si la cola lleva mas de 20 segundos sin salir no la saturamos mas */
					m_timeout[i] =  tiempo + 21;
					continue;
				}
			}
			if (ElementosCola >= m_npedidos)
			{
				m_timeout[i] =  tiempo + 10;
				continue;
			}
			if (m_recibe)
			{
				p2 = (int *)tmp;
				p2[0] = m_handleo;
				p2[1] = m_pedidos[i];
				p2[2] = 4000;
				m_time = time(NULL);
				m_explorer->EnviaCDATA(22,tmp,sizeof(int)*3);				
				m_timeout[i] = m_time;
			}
			else
			{
				p2 = (int *)tmp;
				p2[0] = m_handled;
				p2[1] = m_pedidos[i];
				lseek(m_handleo,m_pedidos[i],SEEK_SET);
				tam = read(m_handleo,(char *)(p2+3),4000);
				p2[2] = tam;
				m_time = time(NULL);
				m_explorer->EnviaCDATA(23,tmp,sizeof(int)*3+tam);				
				m_timeout[i] = m_time;
			}
			ntout++;
		}
	}
	if (ntout > 0)
	{
		if (!Update(m_recibidototal * 100 / m_size))
		{
			m_recibidototal = m_size;
		}
	}
	return ntout;
}

int CEnvioHandle::Acabado()
{	
	if (!m_size || m_recibidototal >= m_size)
	{
		if (m_recibe)
		{
			if (m_handled > -1)
				close(m_handled);
			m_handled = -1;			
			if (m_handleo > -1)
				m_explorer->EnviaCDATA(4,(char *)&m_handleo,sizeof(int));
			m_handleo = -1;
			if (m_explorer->m_panel2)
				m_explorer->m_panel2->EnviaDir();
		}
		else		
		{
			if (m_handleo > -1)
				close(m_handleo);
			m_handleo = -1;
			if (m_handled > -1)
				m_explorer->EnviaCDATA(4,(char *)&m_handled,sizeof(int));
			m_handled = -1;
			if (m_explorer->m_panel1)
				m_explorer->m_panel1->EnviaDir();
		}
		m_status = 3;
		return 1;
	}
	return 0;
}


BEGIN_EVENT_TABLE(CListFile, wxListCtrl)
   EVT_KEY_DOWN(CListFile::OnKeyDown)   
   EVT_RIGHT_DCLICK(CListFile::OnRDClick)
   EVT_LEFT_DCLICK(CListFile::OnLDClick)
   EVT_MENU_RANGE(15001,15006,CListFile::OnMenu)
   EVT_LIST_BEGIN_LABEL_EDIT(-1, CListFile::OnBeginEdit)
   EVT_LIST_END_LABEL_EDIT(-1, CListFile::OnEndEdit)
   EVT_LIST_COL_CLICK(-1, CListFile::OnColClick)
END_EVENT_TABLE()

CListFile::CListFile()
{
	m_explorer = NULL;
	m_remoto = FALSE;
	m_local = FALSE;
}

CListFile::~CListFile()
{

}


void CListFile::OnColClick(wxListEvent& event)
{	
	int col = event.GetColumn();
	int n = GetItemCount(),i,j,item;
	wxListItem IT;
	char **datos[6];
	char **p,**pc;
	wxString buf[6];
	wxString nombre,ss;

	if (col < 0 || col > 5)
		return;

	for (j = 0;j < 6;j++) {	
		datos[j] = (char **)malloc(sizeof(char **) * n);
		memset(datos[j],0,sizeof(char **) * n);
	}		
	for (i = 0;i < n;i++) {
		for (j = 0;j < 6;j++) {
			IT.m_mask	= wxLIST_MASK_TEXT;
			IT.m_itemId	= i;
			IT.m_col = j;			
			if (GetItem(IT))
				buf[j] = (const char *)IT.m_text;
			else
				buf[j] = "!";
		}
		nombre = (const char *)buf[col];
		pc = datos[col];
		for (item = i;item > 0;item--) {
			ss = pc[item-1];
			if (ss.CmpNoCase(nombre) <= 0)
				break;
			for (j = 0;j < 6;j++) {
				p = datos[j];
				p[item] = p[item-1];				
			}
		}
		for (j = 0;j < 6;j++) {
			p = datos[j];
			p[item] = (char *)malloc(strlen(buf[j])+1);
			strcpy(p[item],buf[j]);
		}
	}
	DeleteAllItems();	
	for (i = 0;i < n;i++) {
		p = datos[0];		
		InsertItem(i,p[i]);
		for (j = 1;j < 6;j++) {
			p = datos[j];			
			SetItem(i,j,p[i]);
		}
	}
	for (j = 0;j < 6;j++) {
		p = datos[j];
		for (i = 0;i < n;i++) {			
			if (p[i])
				free(p[i]);
		}
		free(p);
	}
	Refresh();
}

int CListFile::Rellena()
{
	InsertColumn(0,"Nombre",wxLIST_FORMAT_LEFT,-1);	
	InsertColumn(1,"Tamaño",wxLIST_FORMAT_RIGHT,-1);
	InsertColumn(2,"Atributos",wxLIST_FORMAT_LEFT,-1);	
	InsertColumn(3,"Creación",wxLIST_FORMAT_LEFT,-1);	
	InsertColumn(4,"Modificación",wxLIST_FORMAT_LEFT,-1);	
	InsertColumn(5,"Ext",wxLIST_FORMAT_LEFT,-1);	

	if (m_explorer->m_oculto || m_explorer->m_recibe)
		return 6;

	if (m_remoto)
	{
		m_explorer->EnviaCDATA(13,""); /* pedir unidades */
		m_explorer->EnviaCDATA(11,""); /* pedir directorio corriente */		
	}
	if (m_local)
	{
	    char tmp[2048];
		char *p;
		int n;
		
		n = GetLogicalDriveStrings(2047,tmp);
		if (n > 0)
		{
			m_explorer->m_panel2->m_drives.Clear();
			p = tmp;
			while(*p)
			{
				m_explorer->m_panel2->m_drives.Append(p);
				p += (strlen(p)+1);
			}
		}
		
		n = GetCurrentDirectory(2047,tmp);
		if (n > 0)
		{
			p = tmp;
			strcat(tmp,"\\*.*");
			m_explorer->m_panel2->m_dir.Append(p);
			m_explorer->m_panel2->m_dir.SetValue(p);

			for (int i = 0;i < m_explorer->m_panel2->m_drives.Number();i++)
			{
				if (*p == m_explorer->m_panel2->m_drives.GetString(i)[0])
				{
					m_explorer->m_panel2->m_drives.SetSelection(i);
					break;
				}
			}
		}
		m_explorer->ExploraDir(tmp);
	}
	return 6;
}


int CListFile::DamePuntoInsercionCol(wxString &nombre,int ini,int fin,int col) {
	if (!fin || !(fin-ini))
		return fin;

	wxListItem IT;

	IT.m_mask	= wxLIST_MASK_TEXT;
	IT.m_itemId	= fin-1;
	IT.m_col = col;
	GetItem(IT);
    wxString nom2 =	IT.m_text;
	
	if (nom2.CmpNoCase(nombre) <= 0)
		return fin;

	IT.m_mask	= wxLIST_MASK_TEXT;
	IT.m_itemId	= ini;
	IT.m_col = col;
	GetItem(IT);
	wxString nom1 = IT.m_text;
	if (nom1.CmpNoCase(nombre) >= 0)
		return ini;
	int m = ini+((fin-ini)/2);

	IT.m_mask	= wxLIST_MASK_TEXT;
	IT.m_itemId	= m;
	IT.m_col = col;
	GetItem(IT);
	nom1 = IT.m_text;
	
	int c = nom1.CmpNoCase(nombre);
	if (!c)
		return m+1;
	if (c < 0) 
		return DamePuntoInsercionCol(nombre,ini,m,col);
	return DamePuntoInsercionCol(nombre,m+1,fin,col);
}


int CListFile::DamePuntoInsercion(wxString &nombre,int ini,int fin) {
	if (!fin || !(fin-ini))
		return fin;
	
	wxString nom2 = GetItemText(fin-1);
	if (nom2.CmpNoCase(nombre) <= 0)
		return fin;
	wxString nom1 = GetItemText(ini);
	if (nom1.CmpNoCase(nombre) >= 0)
		return ini;
	int m = ini+((fin-ini)/2);
	nom1 = GetItemText(m);
	int c = nom1.CmpNoCase(nombre);
	if (!c)
		return m+1;
	if (c < 0) 
		return DamePuntoInsercion(nombre,ini,m);
	return DamePuntoInsercion(nombre,m+1,fin);
}


int CListFile::PonDirectorio(WIN32_FIND_DATA *dir)
{	
	char tmp[1024];
	SYSTEMTIME tm;
	char *p;
	wxString nombre;

	nombre = dir->cFileName;

	int item = DamePuntoInsercion(nombre,0,GetItemCount());/*GetItemCount();*/

	InsertItem(item,dir->cFileName);
	if (!(dir->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		sprintf(tmp,"%9ld",dir->nFileSizeLow);
		SetItem(item,1,tmp);
	}
	strcpy(tmp,"     ");
	if ((dir->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		tmp[0] = 'D';
	}
	if ((dir->dwFileAttributes & FILE_ATTRIBUTE_READONLY))
	{
		tmp[1] = 'R';
	}
	if ((dir->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
	{
		tmp[2] = 'H';
	}
	if ((dir->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED))
	{
		tmp[3] = 'C';
	}
	if ((dir->dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED))
	{
		tmp[4] = 'E';
	}	
	SetItem(item,2,tmp);

	FileTimeToSystemTime(&dir->ftCreationTime,&tm);
	sprintf(tmp,"%04d/%02d/%02d %02d.%02d.%02d",tm.wYear,tm.wMonth,tm.wDay,tm.wHour,
		                                        tm.wMinute,tm.wSecond);
	SetItem(item,3,tmp);

	FileTimeToSystemTime(&dir->ftLastWriteTime,&tm);
	sprintf(tmp,"%04d/%02d/%02d %02d.%02d.%02d",tm.wYear,tm.wMonth,tm.wDay,tm.wHour,
		                                        tm.wMinute,tm.wSecond);
	
	SetItem(item,4,tmp);
	
	p = dir->cFileName + strlen(dir->cFileName);
	while(p > dir->cFileName && *p != '.') p--;
	if (*p == '.')
	{
		SetItem(item,5,p+1);
	}
	return 0;
}

void CListFile::OnKeyDown(wxKeyEvent& event)
{
}

int CListFile::CopiaFichero(int item,BOOL mover)
{
	wxListItem IT;
	IT.m_mask	= wxLIST_MASK_TEXT;
	IT.m_itemId	= item;
	IT.m_col = 2;
	GetItem(IT);
	{			
		wxString nom = GetItemText(item);			
		IT.m_mask	= wxLIST_MASK_TEXT;
		IT.m_itemId	= item;
		IT.m_col = 1;
		GetItem(IT);
		((CDlgPanelE *)GetParent())->EnviaFile(nom,atol(IT.m_text));
	}
	return 0;
}

void CListFile::OnLDClick(wxMouseEvent& event)
{
	int flags = wxLIST_HITTEST_ONITEM;
	wxPoint point = event.GetPosition();
	int item  = HitTest(point, flags);
	if (item >= 0 && item < GetItemCount())
	{
		wxListItem IT;
		IT.m_mask	= wxLIST_MASK_TEXT;
		IT.m_itemId	= item;
		IT.m_col = 2;
		GetItem(IT);
		if (IT.m_text[0] == 'D')
		{
			wxString nom = GetItemText(item);
			wxString dir =  ((CDlgPanelE *)GetParent())->m_dir.GetValue();
			if (!nom.Cmp(".."))
			{
				int idx = dir.Find('\\',TRUE);
				if (idx >= 0)
				{
					dir.Truncate(idx);
					idx = dir.Find('\\',TRUE);
					if (idx >= 0)
					{
						dir.Truncate(idx+1);
						dir += "*.*";
					}
				}
			}
			else if (nom.Cmp("."))
			{
				int idx = dir.Find('\\',TRUE);
				if (idx >= 0)
				{
					dir.Truncate(idx+1);					
					dir += nom;
					dir += "\\*.*";
				}
			}
			((CDlgPanelE *)GetParent())->m_dir.SetValue(dir);
			((CDlgPanelE *)GetParent())->EnviaDir();			
		}
		else
			CopiaFichero(item,FALSE);		
	}
	event.Skip();
}

void CListFile::OnBeginEdit(wxListEvent &event)
{	
}

void CListFile::OnEndEdit(wxListEvent &event)
{
	wxListItem Item = event.GetItem();
	int item = Item.m_itemId;/*event.GetOldIndex();*/
	wxString anterior =  GetItemText(item);
	wxString nuevo = event.GetText();
	wxString dir =  ((CDlgPanelE *)GetParent())->m_dir.GetValue();
	int idx = dir.Find('\\',TRUE);
	if (idx >= 0)
	{
		dir.Truncate(idx);		
	}
	dir += "\\";
	anterior = dir + anterior;
	nuevo = dir + nuevo;

	if (((CDlgPanelE *)GetParent())->m_local)
	{
		MoveFile(anterior,nuevo);
	}
	if (((CDlgPanelE *)GetParent())->m_remoto)
	{
		char tmp[512];
		int n;
		strcpy(tmp,anterior);
		n = strlen(tmp) + 1;
		strcpy(tmp+n,nuevo);
		n += (strlen(nuevo)+1);
		((CDlgPanelE *)GetParent())->m_explorer->EnviaCDATA(19,tmp,n);
	}
	((CDlgPanelE *)GetParent())->EnviaDir();
	/*event.Veto();*/
}

int CListFile::BorraFichero(int item)
{
	wxString dir =  ((CDlgPanelE *)GetParent())->m_dir.GetValue();
	int idx = dir.Find('\\',TRUE);
	wxString nombre =  GetItemText(item);
	if (idx >= 0)
	{
		dir.Truncate(idx);		
	}
	dir += "\\";
	nombre = dir + nombre;
	int esdir = 0;
	{
		wxListItem IT;
		IT.m_mask	= wxLIST_MASK_TEXT;
		IT.m_itemId	= item;
		IT.m_col = 2;
		GetItem(IT);
		if (IT.m_text[0] == 'D')
		{
			esdir = 1;
		}
	}

	if (((CDlgPanelE *)GetParent())->m_local)
	{
		if (esdir)
		{
			RemoveDirectory(nombre);
		}
		else
		{
			DeleteFile(nombre);
		}		
	}
	if (((CDlgPanelE *)GetParent())->m_remoto)
	{
		if (esdir)
		{
			((CDlgPanelE *)GetParent())->m_explorer->EnviaCDATA(32,(char *)(const char *)nombre);
		}
		else
		{
			((CDlgPanelE *)GetParent())->m_explorer->EnviaCDATA(31,(char *)(const char *)nombre);
		}
	}
	((CDlgPanelE *)GetParent())->EnviaDir();
	return 0;
}

int CListFile::CreaDirectorio(int item)
{	
	wxString dir =  ((CDlgPanelE *)GetParent())->m_dir.GetValue();
	int idx = dir.Find('\\',TRUE);
	wxString nombre;
	int indice = 1,i;
	if (idx >= 0)
	{
		dir.Truncate(idx);		
	}
	dir += "\\";
	for (item = 0; item < GetItemCount();item++)
	{
		nombre = GetItemText(item);
		if (!nombre.BeforeFirst('_').Cmp("tmp"))
		{
			i = atoi(nombre.AfterFirst('_')) + 1;
			if (i > indice)
				indice = i;
		}
	}
	nombre = wxString::Format("tmp_%d",indice);
	nombre = dir + nombre;
	if (((CDlgPanelE *)GetParent())->m_local)
	{
		CreateDirectory(nombre,NULL);
	}
	if (((CDlgPanelE *)GetParent())->m_remoto)
	{
		((CDlgPanelE *)GetParent())->m_explorer->EnviaCDATA(33,(char *)(const char *)nombre);
	}
	((CDlgPanelE *)GetParent())->EnviaDir();
	return 0;
}

void CListFile::OnMenu(wxCommandEvent &event)
{
	int id = event.GetId();
	int item = -1;
	for (int i = 0;i < GetSelectedItemCount();i++)
	{
		item = GetNextItem(item,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		if (id == 15001)
		{
			EditLabel(item);
			break;
		}
		else if (id == 15002)
		{
			CopiaFichero(item,FALSE);
		}
		else if (id == 15003)
		{
			CopiaFichero(item,TRUE);
		}
		else if (id == 15004)
		{
			BorraFichero(item);
		}
		else if (id == 15005)
		{
			CreaDirectorio(item);
			break;
		}
	}
	event.Skip();
}

void CListFile::OnRDClick(wxMouseEvent& event)
{
	wxMenu menu;
	/*menu.Append(15001,"Renombrar");*/
    menu.Append(15002,"Copiar");
	/*menu.Append(15003,"Mover");*/
	menu.Append(15004,"Borrar");
	menu.Append(15005,"Crear Directorio");
	/*menu.Append(15006,"Cambiar Atributos");*/
	wxPoint p(0,0);
#ifdef WIN32
	POINT pp;
	RECT rect;
    ::GetCursorPos(&pp);
	::GetWindowRect((HWND)GetHandle(),&rect);
	pp.x -= rect.left;
	pp.y -= rect.top;	
	p = wxPoint(pp.x,pp.y);	
#endif
	PopupMenu( &menu, p);
	event.Skip();
}


BEGIN_EVENT_TABLE(CComboDir, wxComboBox)
   EVT_KEY_DOWN(CComboDir::OnKeyDown)   
END_EVENT_TABLE()

CComboDir::CComboDir()
{
}

CComboDir::~CComboDir()
{

}

void CComboDir::OnKeyDown(wxKeyEvent& event)
{
}

BEGIN_EVENT_TABLE(CDlgPanelE, wxWindow)
  EVT_SIZE(CDlgPanelE::OnSize)	    
  EVT_COMBOBOX(1066, CDlgPanelE::OnDrive)	
  EVT_COMBOBOX(1067, CDlgPanelE::OnDir)	
  EVT_TEXT(1067, CDlgPanelE::OnText)	  
  EVT_TIMER(TIMER_ID, CDlgPanelE::OnTimer)
END_EVENT_TABLE()

CDlgPanelE::CDlgPanelE(  CDlgExplorer* dlgexplorer,
					         wxWindow* parent,
					         CListFile* lista,							 
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxWindow(parent, id,pos,size,style,name)
{
    m_cambiodir = 0;
	m_puntotimer = 0;

	m_remoto = FALSE;
	m_local = FALSE;

	m_explorer = dlgexplorer;
	m_sizer = new wxBoxSizer(wxVERTICAL);

	lista->Create(this,1086,wxDefaultPosition,size,wxLC_REPORT|wxRAISED_BORDER|wxLC_EDIT_LABELS/*|wxLC_SORT_ASCENDING*/);
	{
		DWORD styles = LVS_EX_GRIDLINES ;	
		ListView_SetExtendedListViewStyleEx((HWND)lista->GetHandle(), styles, styles );
	}
	
	wxString vacio[1];
	m_drives.Create(this, 1066,"", wxDefaultPosition, wxSize(10,30), 0,vacio,wxCB_SORT|wxCB_READONLY);
	m_dir.Create(this, 1067,"", wxDefaultPosition, wxSize(10,30), 0,vacio,wxCB_SORT|wxCB_DROPDOWN);

	m_sizer->Add(&m_drives,0,wxALIGN_TOP | wxTOP | wxGROW);	        
	m_sizer->Add(lista,1,wxALIGN_BOTTOM | wxBOTTOM | wxGROW);	    
	m_sizer->Add(&m_dir,0,wxALIGN_BOTTOM | wxBOTTOM | wxGROW);
    SetSizer(m_sizer);

	m_timer.SetOwner(this,TIMER_ID);
	m_timer.Start(25);
}

void CDlgPanelE::OnSize(wxSizeEvent& event)
{		
	m_sizer->SetDimension(0,0,event.m_size.x,event.m_size.y);
	m_sizer->Layout();	
	m_drives.Refresh();
	m_dir.Refresh();
}

void CDlgPanelE::OnDrive(wxCommandEvent &event)
{
    char tmp[512];
	int n = m_drives.GetSelection();
	m_drives.SetValue(m_drives.GetString(n));
	strcpy(tmp,m_drives.GetValue());
	strcat(tmp,"*.*");
	m_dir.SetValue(tmp);
	if (m_local)
	{
		m_explorer->ExploraDir(tmp);
	}
	if (m_remoto)
	{
		m_explorer->EnviaCDATA(21,tmp);
	}
}

void CDlgPanelE::OnDir(wxCommandEvent &event)
{
	int n = m_dir.GetSelection();
	m_dir.SetValue(m_dir.GetString(n));

	EnviaDir();
}

void CDlgPanelE::OnText(wxCommandEvent &event)
{
	m_cambiodir = m_puntotimer;
}

void CDlgPanelE::OnTimer(wxTimerEvent& event)
{	
	m_puntotimer++;
	if (m_cambiodir)
	{
		if ((m_puntotimer-m_cambiodir) > 20)
		{			
			EnviaDir();
		}
	}

	m_explorer->CompruebaSocket();
}

void CDlgPanelE::EnviaDir()
{
	char p[2048];
	int i;

	m_cambiodir = 0;

	strcpy(p,m_dir.GetValue());
	if (m_dir.Number() < 200)
	{
		for (i = 0;i < m_dir.Number();i++)
		{
			if (!m_dir.GetString(i).Cmp(p))
			{
				break;
			}
		}
		if (i >= m_dir.Number())
		{
			m_dir.Append(p);
		}
	}

	for (i = 0;i < m_drives.Number();i++)
	{
		if (p[0] == m_drives.GetString(i)[0])
		{
			m_drives.SetSelection(i);
			break;
		}
	}
	if (m_local)
	{
		m_explorer->ExploraDir(p);
	}
	if (m_remoto)
	{
		m_explorer->EnviaCDATA(21,p);
	}
}

void CDlgPanelE::EnviaFile(wxString &nom,long size)
{
	if (m_local)
	{
		m_explorer->EnviaFile(nom,size);
	}
	if (m_remoto)
	{
		m_explorer->RecibeFile(nom,size);
	}
}

CDlgPanelE::~CDlgPanelE()
{

}

BEGIN_EVENT_TABLE(CDlgExplorer, wxDialog)          
    EVT_TIMER(TIMER_IDTOUT, CDlgExplorer::OnTimer)
    EVT_CLOSE(CDlgExplorer::OnCloseWindow)
END_EVENT_TABLE()

/*
wxObject* WXDLLEXPORT_CTORFN wxConstructorFor##CDlgExplorer(  CDlgPresencia* dlgpresencia,CDlgMensaje * dlgmensaje,wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos ,const wxSize& size ,long style ,const wxString& name )
{
	return new CDlgExplorer(  dlgpresencia,dlgmensaje,parent,id,title,pos ,size ,style ,name );
}
wxClassInfo CDlgExplorer::sm_class##CDlgExplorer((wxChar *) wxT(#CDlgExplorer), (wxChar *) wxT(#wxDialog), (wxChar *) NULL, (int) sizeof(CDlgExplorer), (wxObjectConstructorFn) wxConstructorFor##CDlgExplorer);
*/

CDlgExplorer::CDlgExplorer(  CDlgPresencia* dlgpresencia,
						     CDlgMensaje * dlgmensaje,							 
					         wxWindow* parent,
							 wxWindowID id,							 
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name,
							 BOOL oculto,
							 BOOL recibe
							 ) : 	wxDialog(parent, id,title,pos,size,style,name)
{
	unsigned short port;
	/*wxClassInfo CDlgExplorer::sm_class##CDlgExplorer((wxChar *) wxT(#CDlgExplorer), (wxChar *) wxT(#wxDialog), (wxChar *) NULL, (int) sizeof(CDlgExplorer), (wxObjectConstructorFn) wxConstructorFor##CDlgExplorer);	*/

	::SetWindowLong((HWND)GetHandle(),GWL_USERDATA,(long)this);
	
	m_dlgpresencia = dlgpresencia;
	m_dlgmensaje = dlgmensaje;
	m_asincrono = 0;
	m_esperaopen = -1;
	m_version = 0;
	m_directo = INVALID_SOCKET;
	m_oculto = oculto;
	m_recibe = recibe;
	m_panel1 = NULL;
	m_panel2 = NULL;

	if (m_dlgmensaje->m_dlgatender)
	{
		m_version = m_dlgmensaje->m_dlgatender->m_version;
	}
	else
	{
		wxString nombre;
		struct sockaddr_in  addr;
		char tmp[512];
		int i;
		m_version = 6;

		int idx = ListaPorIp(m_dlgmensaje->m_ip,nombre);

		if (!m_recibe)
		{
			m_directo =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			FastSocket(m_directo);

			addr.sin_addr.s_addr = iplocal[idx];
			addr.sin_family = AF_INET;

			port = 5900 + (htonl(m_dlgmensaje->m_ip) & 0xff);
			port = htons(port);

			addr.sin_port = port/*htons(2078)*/;
			
			if (connect(m_directo,(struct sockaddr *) &addr,sizeof (addr)) == SOCKET_ERROR)
			{
				addr.sin_addr.s_addr = ipvoz[idx];
				if (connect(m_directo,(struct sockaddr *) &addr,sizeof (addr)) == SOCKET_ERROR)
				{
					closesocket(m_directo);
					m_directo = INVALID_SOCKET;
					::MessageBox((HWND)GetHandle(),"SIN CONEXION","ATENCION",MB_TOPMOST|MB_ICONHAND|MB_OK);								
				}
			}

			if (m_directo != INVALID_SOCKET)
			{		
				long yo = dame_the_name();
				if (WSAAsyncSelect(m_directo,(HWND)GetHandle(), WM_SOCKET_SELECT, FD_READ|FD_WRITE) == 0) 
				{
					m_asincrono = 1;
				}
				else
				{
					m_asincrono = 0;
				}		

				i = TIPO_CLIENTE | 0x60000;
				memcpy(tmp,&i,sizeof(int));	
				memcpy(tmp+sizeof(int),&yo,sizeof(long));
				EnviaDato(C_HOLA,tmp,sizeof(int)+sizeof(long));
			}
		}
	}
	
	m_punrec = 0;
	for (int i = 0;i < MAXENVIOS;i++)
	{
		m_envio[i] = NULL;
	}
	if (!m_oculto && !m_recibe)
	{	

		m_sizer = new wxBoxSizer(wxVERTICAL);
		m_split = new wxSplitterWindow(this, 1089, wxDefaultPosition, size, wxSP_3D|wxCLIP_CHILDREN);    
		
		m_panel1 = new CDlgPanelE(this,m_split,&m_list1,1091,"panel1");
		m_panel2 = new CDlgPanelE(this,m_split,&m_list2,1092,"panel2");

		
		m_list1.m_explorer = this;
		m_list1.m_remoto = TRUE;
		m_panel1->m_remoto = TRUE;

		m_list2.m_explorer = this;
		m_list2.m_local = TRUE;
		m_panel2->m_local = TRUE;

		m_list1.Rellena();
		m_list2.Rellena();

		m_split->SplitVertically(m_panel1, m_panel2);
		
		m_sizer->Add(m_split,1,wxALIGN_BOTTOM | wxBOTTOM | wxGROW);	
		
		SetAutoLayout(TRUE);
		SetSizer(m_sizer);
    
		m_sizer->SetSizeHints(this);
		m_sizer->Fit(this);

		m_panel1->Show(TRUE);
		m_panel2->Show(TRUE);

		

		if (m_dlgmensaje->m_dlgatender)
		{			
			port = 5900 + (htonl(dame_the_name()) & 0xff);
			port = htons(port);
			EnviaCDATA(24,(char *)&port,sizeof(port));
		}
	}

	m_timer.SetOwner(this,TIMER_IDTOUT);
	m_timer.Start(1000);
}

CDlgExplorer::~CDlgExplorer()
{
	for (int i = 0;i < MAXENVIOS;i++)
	{
		if (m_envio[i])
		{
			delete m_envio[i];
			m_envio[i] = NULL;
		}
	}
	if (m_directo != INVALID_SOCKET)
	{
		m_punrec = 0;
		BorraColaPPaquete();
		closesocket(m_directo);
	}
	m_directo = INVALID_SOCKET;
}


void CDlgExplorer::OnCloseWindow(wxCloseEvent& event)
{
	if (m_directo != INVALID_SOCKET)
	{
		m_punrec = 0;
		BorraColaPPaquete();
		closesocket(m_directo);
	}
	m_directo = INVALID_SOCKET;
	wxDialog::OnCloseWindow(event);		
}

int CDlgExplorer::SocketSelect(WPARAM wParam,LPARAM lParam)
{
	WORD event = WSAGETSELECTEVENT(lParam);
	if (m_directo == (SOCKET)wParam && m_asincrono)
	{
		if (event == FD_READ)
		{
			if (RecibePaquete(m_directo,&m_recvbuf,0,&m_punrec) == 1)
			{
				Recibe(&m_recvbuf);
			}
		}
		else if (event == FD_WRITE)
		{
			int e;
			do
			{
				if ((e = EnviaColaPPaquete(m_directo)) < 0)
				{
					BorraColaPPaquete();
					closesocket(m_directo);
					m_punrec = 0;
					m_asincrono = 0;
					m_directo = INVALID_SOCKET;
				}
			} while(e == 1);
		}
		return 1;
	}
	return 0;
}


int CDlgExplorer::ProcesaRecibe(unsigned short *p1)
{
	if (*p1 == 1003) /* abrir fichero */
	{
		int flags1,flags2,file;
		int *p2;
		int tam = sizeof(int)*2;
		p2 = (int *)(p1+1);						
		flags1 = *p2++;
		flags2 = *p2++;		
		tam += strlen((char *)p2);
		tam++;
		if (quita_path((char *)p2) == (char *)p2)
		{
			wxString s;
			s = "Atencion, fichero entrante de ";
			s += m_dlgmensaje->GetLabel();
			s += "!";
			wxFileDialog dlg(NULL,s,"",(char *)p2,"*.*",wxSAVE|wxOVERWRITE_PROMPT);
			if (dlg.ShowModal() == wxID_OK)
			{
				s = dlg.GetPath();
				file = open(s,flags1,flags2);
			}
			else
				file = -1;
		}
		else
			file = open((char *)p2,flags1,flags2);
		p2 = (int *)(p1+1);
		*p2 = file;
		EnviaCDATA(*p1,(char *)(p1+1),tam+sizeof(short));
	}
	else if (*p1 == 1004) /* cerrar fichero */
	{
		int *p2;
		p2 = (int *)(p1+1);
		*p2 = close(*p2);
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else if (*p1 == 1005) /* posicionar fichero */
	{
		int *p2;
		int file;
		long posicion;						
		int flag;
		p2 = (int *)(p1+1);
		file = *p2++;
		posicion = *((long *)p2);
		p2++;
		flag = *p2++;
		p2 = (int *)(p1+1);
		*(long *)p2 = lseek(file,posicion,flag);
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(long)+sizeof(short));
	}
	else if (*p1 == 1006) /* leer de fichero */
	{
		int *p2;
		int file;
		int tam;						
		p2 = (int *)(p1+1);
		file = *p2++;
		tam = *p2;
		if (tam > 4080)
			tam = 4080;
		tam = read(file,p2,tam);
		p2--;
		*p2 = tam;

		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short)+tam);
	}
	else if (*p1 == 1007) /* grabar a fichero */
	{
		int *p2;
		int file;
		int tam;						
		p2 = (int *)(p1+1);
		file = *p2++;
		tam = *p2++;
		tam = write(file,p2,tam);
		p2 -= 2;
		*p2 = tam;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else if (*p1 == 1009) /* Recoger atributos de fichero */
	{
		DWORD att = GetFileAttributes((char *)(p1+1));
		*((DWORD *)(p1+1)) = att;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(DWORD)+sizeof(short));
	}
	else if (*p1 == 1010) /* Poner atributos de fichero */
	{
		int res;
		DWORD *p2;
		p2 = (DWORD *)(p1+1);							
		res = (int)SetFileAttributes((char *)(p2+1),*p2);
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else if (*p1 == 1011) /* Recoger Directorio corriente */
	{
		unsigned short res;
		res = (unsigned short)GetCurrentDirectory(4080,(char *)(p1+2));
		*(p1 + 1) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(short)*2+res+1);
	}
	else if (*p1 == 1012) /* Poner Directorio corriente */
	{
		int res;
		res = (int)SetCurrentDirectory((char *)(p1+1));
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int));
	}
	else if (*p1 == 1013) /* logical drives */
	{
		unsigned short res;
		res = (unsigned short)GetLogicalDriveStrings(4080,(char *)(p1+2));
		*(p1 + 1) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(short)*2+res+1);
	}
	else if (*p1 == 1014 || *p1 == 1021) /* open dir */
	{
		 WIN32_FIND_DATA findata;
		 HANDLE find;
		 int ndir;
		 int tam;
		 int *p2;
		 char *p3;
		 int modocompleto = 0;
		 if (*p1 == 1021)
			 modocompleto = 1;
		 do
		 {
			 ndir = 0;
			 tam = 0;
			 p2 = (int *)(p1+1);
			 p3 = (char *)(p2+1);							 
			 if (modocompleto > 1)							 
			 {
				 *p1 = 1015;							 
				 if (!FindNextFile(find,&findata))
					 break;
			 }
			 else
			 {
				memset(&findata,0,sizeof(findata));
				find =  FindFirstFile((char *)(p1+1),&findata);
			 }
			 p3  += sizeof(HANDLE);
			 tam += sizeof(HANDLE);
			 if (find != INVALID_HANDLE_VALUE)
			 {
				 do
				 {
					 memcpy(p3,&findata,sizeof(findata));
					 p3 += sizeof(findata);
					 tam += sizeof(findata);
					 ndir++;
				 } while(tam < (4080-sizeof(findata)) && FindNextFile(find,&findata));
				 if (tam < (4080-sizeof(findata)))
				 {
					 FindClose(find);
					 find = INVALID_HANDLE_VALUE;
				 }
			 }
			 p3 = (char *)(p2+1);
			 memcpy(p3,&find,sizeof(HANDLE));
			 *p2 = ndir;
			 EnviaCDATA(*p1,(char *)(p1+1),sizeof(short)+sizeof(int)+tam);						 
			 modocompleto++;
		 } while(modocompleto > 1 && find != INVALID_HANDLE_VALUE);
	}
	else if (*p1 == 1015) /* more dir */
	{
		 WIN32_FIND_DATA findata;
		 HANDLE find;
		 int ndir = 0;
		 int tam = 0;
		 int *p2 = (int *)(p1+1);
		 char *p3;

		 p3 = (char *)p2;
		 memcpy(&find,p3,sizeof(HANDLE));
		 memcpy(&findata,p3+sizeof(HANDLE),sizeof(findata));

		 p3 = (char *)(p2+1);						 
		 p3  += sizeof(HANDLE);
		 tam += sizeof(HANDLE);
		 while(tam < (4080-sizeof(findata)) && FindNextFile(find,&findata));
		 {
			 memcpy(p3,&findata,sizeof(findata));
			 p3 += sizeof(findata);
			 tam += sizeof(findata);
			 ndir++;
		 } 
		 if (tam >= (4080-sizeof(findata)))
		 {
			 FindClose(find);
			 find = INVALID_HANDLE_VALUE;
		 }
		 p3 = (char *)(p2+1);
		 memcpy(p3,&find,sizeof(HANDLE));
		 *p2 = ndir;
		 EnviaCDATA(*p1,(char *)(p1+1),sizeof(short)+sizeof(int)+tam);
	}
	else if (*p1 == 1016) /* close dir */
	{
		int res;
		HANDLE find;						
		memcpy(&find,(char *)(p1+1),sizeof(HANDLE));
		res = FindClose(find);						
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else if (*p1 == 1018) /* get drive tipe */
	{
		int res;
		res = (int)GetDriveType((char *)(p1+1));
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));						
	}
	else if (*p1 == 1019) /* move file */
	{
		int res;
		res = (int)MoveFile((char *)(p1+1),(char *)(p1+1)+strlen((char *)(p1+1))+1);
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));						
	}
	else if (*p1 == 1020) /* Copy file */
	{
		int res;
		res = (int)CopyFile((char *)(p1+1),(char *)(p1+1)+strlen((char *)(p1+1))+1,FALSE);
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));						
	}
	else if (*p1 == 1022) /* leer de fichero con posicion */
	{
		int *p2;
		int file;
		int tam;
		long pos;
		p2 = (int *)(p1+1);
		file = *p2++;
		pos = *p2++;
		tam = *p2++;
		if (tam > 4080)
			tam = 4080;						
		lseek(file,pos,SEEK_SET);
		tam = read(file,p2,tam);						
		p2--;
		*p2 = tam;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)*3+sizeof(short)+tam);
	}
	else if (*p1 == 1023) /* grabar a fichero con posicion */
	{
		int *p2;
		int file;
		int tam;		
		int pos;
		p2 = (int *)(p1+1);
		file = *p2++;						
		pos = *p2++;
		tam = *p2++;
		tam = write(file,p2,tam);
		p2--;
		*p2 = tam;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)*3+sizeof(short));
	}
	else if (*p1 == 1031)
	{ /* BORRAR FICHERO !!! */
		int res;
		res = (int)DeleteFile((char *)(p1+1));
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));						
	}
	else if (*p1 == 1032)
	{ /* BORRAR DIRECTORIO !!! */
		int res;
		res = (int)RemoveDirectory((char *)(p1+1));
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else if (*p1 == 1033)
	{ /* CREAR DIRECTORIO !!! */
		int res;
		res = (int)CreateDirectory((char *)(p1+1),NULL);
		*((int *)(p1+1)) = res;
		EnviaCDATA(*p1,(char *)(p1+1),sizeof(int)+sizeof(short));
	}
	else
		return -1;


	return 0;
}


int CDlgExplorer::Recibe(Paquete *ppaquete)
{	
	unsigned short *p1,comando;
	char *p;
	char buffer[MAXBUFFER*2];
	int size;
	int lookformore = 0;

	if (COMANDO(ppaquete) == C_DATA)
	{
		p1 = (unsigned short *)ppaquete->buffer;
		
		comando = *p1;		
		if (comando == 9999)
		{
			void *_write_interno;
			void *_read_interno;
			if (LEN_DATA(ppaquete) > sizeof(short))
			{
				/* paquete comprimido */
				_write_interno = (void *)write_interno;
				_read_interno = (void *)read_interno;

				write_interno = write_gunzip;
				read_interno = read_gunzip;

				read_datos = (char *)(p1+1);
				read_long = LEN_DATA(ppaquete) - sizeof(short);

				write_datos = buffer;
				write_long = 0;

				ds___gzip(0,0,1,6);    
				
				write_interno = (void (*)(int,unsigned char *,unsigned))_write_interno;
				read_interno = (unsigned int (*)(int *,unsigned char *,unsigned))_read_interno;
				
				p1 = (unsigned short *)buffer;
				//memcpy(ppaquete->buffer,buffer,write_long); // el tamaño puede no ser correcto
				PON_LEN_DATA(ppaquete,write_long);
				comando = *p1;		
			}
		}
		size = LEN_DATA(ppaquete) - sizeof(short);
		p1++; /* OJO!!! siempre usar p1 nunca paquete.buffer */
		if (ProcesaRecibe(p1-1) < 0)
		{
			switch(comando)
			{
			case 3:
				{
					int i;
					i = LocalizaEnvio(0,(int *)p1,size);
					if (i >= 0 && m_envio[i])
					{
						m_envio[i]->Open(*((int *)p1));
						if (m_envio[i]->Acabado())
						{
							delete m_envio[i];
							m_envio[i] = NULL;
						}
					}
					else
					{
						EnviaCDATA(4,(char *)p1,sizeof(int));
					}
				}
				break;
			case 4: /* cerrar fichero */
				break;
			case 11: /* directorio corriente */
				if (*p1 > 0 && m_panel1)
				{
					char tmp[2048];
					strcpy(tmp,(char *)(p1+1));
					p = tmp;
					if (*p && p[strlen(p)-1] == '\\')
					{
						p[strlen(p)-1] = 0;
					}
					strcat(p,"\\*.*");				
					m_panel1->m_dir.Append(p);
					m_panel1->m_dir.SetValue(p);
					EnviaCDATA(21,p); /* pedir todo el directorio */
					for (int i = 0;i < m_panel1->m_drives.Number();i++)
					{
						if (*p == m_panel1->m_drives.GetString(i)[0])
						{
							m_panel1->m_drives.SetSelection(i);
							break;
						}
					}
				}
				break;
			case 13: /* drives */
				if (!m_panel1)
					break;
				m_panel1->m_drives.Clear();
				if (*p1 > 0)
				{				
					p = (char *)(p1+1);
					while(*p)
					{
						m_panel1->m_drives.Append(p);
						p += (strlen(p)+1);
					}
				}
				break;
			case 14:			
				/* peticion paso a paso */
			case 21:
				m_list1.DeleteAllItems();
			case 15:
				{
					int ndir,i;
					HANDLE find;				
					p = (char *)p1;
					ndir = *(int *)p;
					p += sizeof(int);
					find = *(HANDLE *)p;
					p += sizeof(HANDLE);
					for (i = 0;i < ndir;i++)
					{
						m_list1.PonDirectorio((WIN32_FIND_DATA *)p);
						p += sizeof(WIN32_FIND_DATA);
					}				
				}
				break;
			case 22:
				{
					int i;
					i = LocalizaEnvio(1,(int *)p1,size);
					if (i >= 0 && m_envio[i])
					{
						lookformore = m_envio[i]->Recibe((int *)p1);
						if (m_envio[i] && m_envio[i]->Acabado())
						{
							delete m_envio[i];
							m_envio[i] = NULL;
						}
					}
				}
				break;
			case 23:
				{
					int i;
					i = LocalizaEnvio(2,(int *)p1,size);
					if (i >= 0 && m_envio[i])
					{
						lookformore = m_envio[i]->Envia((int *)p1);
						if (m_envio[i] && m_envio[i]->Acabado())
						{
							delete m_envio[i];
							m_envio[i] = NULL;
						}
					}
				}
				break;
			default:
				lookformore = -1;
				break;
			}
		}
	}
	else if (COMANDO(ppaquete) == C_HOLA)
	{
		long ip;
		/*if (m_dlgmensaje->m_dlgatender)*/
		{		
			/* comprobar si es para mi o pasar el testigo */
			ip = *(long *)(ppaquete->buffer+sizeof(int));		
			if (m_dlgmensaje->m_ip != ip)
			{
				for (int i = 0;i < 500;i++)
				{
					if (m_dlgpresencia->m_dlgmensaje[i] && m_dlgpresencia->m_dlgmensaje[i]->m_ip == ip)
						break;
				}
				if (i >= 500)
				{
					i = m_dlgpresencia->ActivaMensaje(ip,TRUE,TRUE);
					if (i < 0)
						i = 0;
				}
				for (;i < 500;i++)
				{
					if (m_dlgpresencia->m_dlgmensaje[i] && m_dlgpresencia->m_dlgmensaje[i]->m_ip == ip)
					{

						if (m_dlgpresencia->m_dlgmensaje[i]->m_explorer && 
                            m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_directo != INVALID_SOCKET &&
							(m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_directo != m_directo ||
							 (!m_dlgpresencia->m_dlgmensaje[i]->m_dlgatender && !m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_recibe))
							)
						{
							delete m_dlgpresencia->m_dlgmensaje[i]->m_explorer;
							m_dlgpresencia->m_dlgmensaje[i]->m_explorer = NULL;
						}
						if (!m_dlgpresencia->m_dlgmensaje[i]->m_explorer)
						{
							CDlgMensaje *dlgmensaje;
							wxString s;							
							dlgmensaje = m_dlgpresencia->m_dlgmensaje[i];
							s = "Trasferir ficheros con " + dlgmensaje->GetLabel();
							if (!dlgmensaje->m_dlgatender)
							{
								dlgmensaje->m_explorer = new CDlgExplorer(dlgmensaje->m_padre,dlgmensaje,dlgmensaje->m_padre,-1,s,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox",TRUE,TRUE);
							}
							else
							{
								dlgmensaje->m_explorer = new CDlgExplorer(dlgmensaje->m_padre,dlgmensaje,dlgmensaje->m_padre,-1,s,wxDefaultPosition,wxSize(600,400),wxCAPTION/*|wxSTAY_ON_TOP*/,"dialogBox");
								dlgmensaje->m_explorer->Center();
								dlgmensaje->m_explorer->Show(TRUE);		
								dlgmensaje->m_explorer->SetFocus();
							}
						}
						m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_directo = m_directo;
						WSAAsyncSelect(m_directo,(HWND)GetHandle(), 0, 0);					
						m_asincrono = 0;
						m_directo = INVALID_SOCKET;
						m_punrec = 0;					
						m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_punrec = 0;
						if (WSAAsyncSelect(m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_directo,(HWND)m_dlgpresencia->m_dlgmensaje[i]->m_explorer->GetHandle(), WM_SOCKET_SELECT, FD_READ|FD_WRITE) == 0) 
						{
							m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_asincrono = 1;
						}					
						else
						{
							m_dlgpresencia->m_dlgmensaje[i]->m_explorer->m_asincrono = 0;
						}
						break;
					}
				}
				if (i >= 500)
				{
					m_punrec = 0;
					BorraColaPPaquete();
					closesocket(m_directo);
					m_directo = INVALID_SOCKET;
				}
			}
		}
		lookformore = 0;
	}
	else
		lookformore = -1;

	return lookformore;
}

int CDlgExplorer::EnviaCDATA(unsigned short comando,char *buffer,int len)
{
	char buffer2[MAXBUFFER*2];
	int comprimido = 0;
	char mensaje[MAXBUFFER + 512];
	char *p;

	if (!m_dlgmensaje->m_dlgatender)
	{
		if (comando < 1000)
			comando += 1000;
		else if (comando < 9999)
			comando -= 1000;
	}
	if (len == -1)
	{
		len = strlen(buffer)+1;
	}
	
	if (len > 100)
		comprimido = 1;
	
	if (comprimido)
	{
		void *_write_interno;
		void *_read_interno;

		/* paquete comprimido */
		_write_interno = (void *)write_interno;
		_read_interno = (void *)read_interno;

		write_interno = write_gunzip;
		read_interno = read_gunzip;

		memcpy(mensaje+sizeof(short),(void *)buffer,len); /* puerto */
		*((unsigned short *)mensaje) = comando;
		read_datos = mensaje;
		read_long = len+sizeof(short);

		write_datos = buffer2;
		write_long = 0;

		ds___gzip(0,0,0,6);
		
		write_interno = (void (*)(int,unsigned char *,unsigned))_write_interno;
		read_interno = (unsigned int (*)(int *,unsigned char *,unsigned))_read_interno;

		len = write_long;
		buffer = buffer2;		
		comando = 9999;
	}

	if (m_directo != INVALID_SOCKET)
	{
		PPaquete paquete;
		
		memset(&(paquete.c),0,LEN_CABECERA);
		paquete.c.id = DSWEBCALLID;
		paquete.c.command = C_DATA;
		PON_LEN_DATA(&paquete,len+sizeof(short));
		p = mensaje;
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);	
		memcpy(p,(void *)buffer,len); 
		p += len;
		paquete.buffer = (unsigned char *)mensaje;

		if (!EncolaPPaquete(&paquete))
		{
			BorraColaPPaquete();
			closesocket(m_directo);
			m_directo = INVALID_SOCKET;
			m_punrec = 0;
		}
		else
			return 0;
	}	

	
	int e;		
	unsigned short com;
	//long yo = dame_the_name();

	p = mensaje;
	e = 18; // comando telesoporte
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&(m_dlgmensaje->m_ip),sizeof(long));
	p += sizeof(long);
	com = C_DATA;
	memcpy(p,(void *)&com,sizeof(comando));
	p += sizeof(com);	
	memcpy(p,(void *)&comando,sizeof(comando));
	p += sizeof(comando);	
	memcpy(p,(void *)buffer,len); /* puerto */
	p += len;
	pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);

	return 0;
}

int CDlgExplorer::EnviaDato(unsigned short comando,char *buffer,int len)
{
	char mensaje[MAXBUFFER + 512];

	if (len < 0)
		len = strlen(buffer)+1;	

	if (m_directo != INVALID_SOCKET)
	{
		PPaquete paquete;
		
		memset(&(paquete.c),0,LEN_CABECERA);
		paquete.c.id = DSWEBCALLID;
		paquete.c.command = comando;
		PON_LEN_DATA(&paquete,len);
		paquete.buffer = (unsigned char *)buffer;

		if (!EncolaPPaquete(&paquete))
		{
			BorraColaPPaquete();
			closesocket(m_directo);
			m_directo = INVALID_SOCKET;
			m_punrec = 0;
		}
		else
			return 0;
	}	

	int e;		
	unsigned short com;
	//long yo = dame_the_name();

	char *p = mensaje;
	e = 18; // comando telesoporte
	memcpy(p,(void *)&e,sizeof(int));
	p += sizeof(int);
	memcpy(p,(void *)&(m_dlgmensaje->m_ip),sizeof(long));
	p += sizeof(long);
	com = comando;
	memcpy(p,(void *)&com,sizeof(comando));
	p += sizeof(com);	
	memcpy(p,(void *)buffer,len); /* puerto */
	p += len;
	pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);

	return 0;
}

int CDlgExplorer::ExploraDir(char *dir)
{
	 WIN32_FIND_DATA findata;
	 HANDLE find;
	 m_list2.DeleteAllItems();
	 memset(&findata,0,sizeof(findata));
	 find =  FindFirstFile(dir,&findata);
	 if (find != INVALID_HANDLE_VALUE)
	 {
		 do
		 {
			 m_list2.PonDirectorio(&findata);
		 } while(FindNextFile(find,&findata));
	 }
	 FindClose(find);
	 return 0;
}



int CDlgExplorer::RecibeFile(wxString &nom,long size)
{
	for (int i = 0;i < MAXENVIOS;i++)
	{
		if (!m_envio[i])
			break;
	}
	if (i >= MAXENVIOS)
	{
		return 0;
	}
	if (!m_panel1)
		return -1;

	wxString dir1,dir2;
	dir1 = m_panel1->m_dir.GetValue();
	int idx = dir1.Find('\\',TRUE);
	if (idx >= 0)
	{
		dir1.Truncate(idx+1);
	}
	dir1 += nom;

	dir2 = m_panel2->m_dir.GetValue();
	idx = dir2.Find('\\',TRUE);
	if (idx >= 0)
	{
		dir2.Truncate(idx+1);
	}
	dir2 += nom;


	wxString s;
	s = "Recibiendo ";
	s += dir2;

	m_envio[i] = new CEnvioHandle(TRUE,size,dir1,dir2,this,i,"Transfiriendo",s,100,NULL/*this/*m_dlgpresencia*/,wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME| wxPD_AUTO_HIDE |wxSTAY_ON_TOP);
	return 0;
}

int CDlgExplorer::EnviaFile(wxString &nom)
{
	for (int i = 0;i < MAXENVIOS;i++)
	{
		if (!m_envio[i])
			break;
	}
	if (i >= MAXENVIOS)
	{
		return 0;
	}
	wxString dir2;
	
	int idx = nom.Find('\\',TRUE);
	if (idx >= 0)
	{
		idx++;
		dir2 = nom.Mid(idx);
	}
	else
		dir2 = nom;

	
	idx = dir2.Find('.');
	if (idx >= 0)
	{
		if (!dir2.Mid(idx).CmpNoCase(".lnk"))
		{			
			wxMessageBox("Por razones de seguridad no se pueden enviar lnk directamente, renombrelo por favor",dir2,wxOK /*| wxCENTRE */| wxICON_HAND,NULL);
			return -1;
		}
	}


	wxString s;
	s = "Enviando ";
	s += nom;

	m_envio[i] = new CEnvioHandle(FALSE,0,nom,dir2,this,i,"Transfiriendo",s,100,NULL/*this/*m_dlgpresencia*/,wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME| wxPD_AUTO_HIDE);
	return 0;
}

int CDlgExplorer::EnviaFile(wxString &nom,long size)
{
	if (!m_panel1)
		return -1;

	for (int i = 0;i < MAXENVIOS;i++)
	{
		if (!m_envio[i])
			break;
	}
	if (i >= MAXENVIOS)
	{
		return 0;
	}
	wxString dir1,dir2;
	dir1 = m_panel2->m_dir.GetValue();
	int idx = dir1.Find('\\',TRUE);
	if (idx >= 0)
	{
		dir1.Truncate(idx+1);
	}
	dir1 += nom;

	dir2 = m_panel1->m_dir.GetValue();
	idx = dir2.Find('\\',TRUE);
	if (idx >= 0)
	{
		dir2.Truncate(idx+1);
	}
	dir2 += nom;

	
	wxString s;
	s = "Enviando ";
	s += dir1;

	m_envio[i] = new CEnvioHandle(FALSE,size,dir1,dir2,this,i,"Transfiriendo",s,100,NULL/*this/*m_dlgpresencia*/,wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME| wxPD_AUTO_HIDE);
	return 0;
}

void CDlgExplorer::OnTimer(wxTimerEvent& event)
{
	CompruebaSocket();
	for (int i = 0;i < MAXENVIOS;i++)
	{
		if (m_envio[i])
		{
			if (m_envio[i]->Timeouts() > 0)
			{
				if (m_envio[i]->Acabado())
				{
					delete m_envio[i];
					m_envio[i] = NULL;
				}
			}
		}
	}
}


void CDlgExplorer::CompruebaSocket()
{
	if (m_directo != INVALID_SOCKET)
	{		
		/*
		if (wxGetApp().Pending())
			return;
		*/		
		long ticks = ::GetTickCount();
		unsigned long cuanto;        
		do
		{
			cuanto = 0L;
			if (ioctlsocket(m_directo,FIONREAD,&cuanto) == SOCKET_ERROR)
			{
				BorraColaPPaquete();
				closesocket(m_directo);
				m_punrec = 0;
				m_directo = INVALID_SOCKET;				
				break;
			}
			else
			{				
				int e = 0;
				if (cuanto > 0)
				{					
					if (RecibePaquete(m_directo,&m_recvbuf,0,&m_punrec) == 1)
					{
						Recibe(&m_recvbuf);
					}
				}
				if ((e = EnviaColaPPaquete(m_directo)) < 0)
				{
					BorraColaPPaquete();
					closesocket(m_directo);
					m_punrec = 0;
					m_directo = INVALID_SOCKET;		
					break;
				}
				if (m_asincrono)
					break;
				if (e != 1 && !cuanto)
					break;
			}			 
		} while((::GetTickCount()- ticks) < 200);
	}
}


int CDlgExplorer::LocalizaEnvio(int modo,int *p1,int size)
{
	int i;
	switch(modo)
	{
	case 0:
		if (size > ((sizeof(int)*2)+1))
		{			
			char *p = (char *)(p1+2);
			for (i = 0;i < MAXENVIOS;i++)
			{
				if (m_envio[i] && m_envio[i]->m_status == 1)
				{
					if (m_envio[i]->m_recibe)
					{
						if (!m_envio[i]->m_origen.Cmp(p))
						{
							return i;
						}
					}
					else
					{
						if (!m_envio[i]->m_destino.Cmp(p))
						{
							return i;
						}
					}
				}
			}
		}
		else
		{
			if (m_esperaopen > -1)
			{
				return m_esperaopen;
			}
			for (i = 0;i < MAXENVIOS;i++)
			{
				if (m_envio[i] && m_envio[i]->m_status == 1)
				{					
					return i;
				}
			}
		}
		break;
	case 1:
		for (i = 0;i < MAXENVIOS;i++)
		{
			if (m_envio[i] && m_envio[i]->m_handleo == *p1)
			{
				return i;
			}
		}
		break;
	case 2:
		for (i = 0;i < MAXENVIOS;i++)
		{
			if (m_envio[i] && m_envio[i]->m_handled == *p1)
			{
				return i;
			}
		}
		break;
	}

	return -1;
}