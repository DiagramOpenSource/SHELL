// Tcontrol.cpp: implementation of the CTcontrol class.
//
//////////////////////////////////////////////////////////////////////

#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "Tcontrol.h"


#ifndef MSG_WAITALL
#define MSG_WAITALL 0
#endif

#define DS0SIGNATURE 0x7117
#define MAXIMO_BUFFER 128000
wxCriticalSection critico;

int xxxavisar = 0;

SOCKET servidor = (SOCKET)-1;
SOCKET escucha = (SOCKET)-1;

struct _Elemento_
{
	void          *_dato;
	unsigned short _size;
	struct _Elemento_ *m_siguiente;
};

typedef struct _Elemento_ C_Elemento;

static int noname = 0;

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

struct _Lista_
{
	C_Elemento *m_primero;	
	C_Elemento *m_ultimo;	
	int         m_nelementos;
};

static struct _Lista_ ColaLocal,ColaEnvio;
static struct sockaddr_in  _thename;
static bool conectado = FALSE;
static bool _envia_estado = FALSE;

void ini_cola()
{
	memset(&ColaLocal,0,sizeof(ColaLocal));
	memset(&ColaEnvio,0,sizeof(ColaEnvio));
}


static void pon_en_cola_local(char *p,unsigned short size)
{
	C_Elemento *pele;
	critico.Enter();

    pele = (C_Elemento *)malloc(sizeof(C_Elemento));
	memset((void *)pele,0,sizeof(C_Elemento));
	pele->_dato = malloc(size);
	pele->_size = size;
	memcpy(pele->_dato,p,size);

	if (!ColaLocal.m_primero)
	{
		ColaLocal.m_primero = pele;		
	}
	if (ColaLocal.m_ultimo)
	{
		ColaLocal.m_ultimo->m_siguiente = pele;
	}
	ColaLocal.m_ultimo = pele;

	critico.Leave();
}

char *saca_de_cola_local(int *size)
{
	char *p = NULL;
	C_Elemento *pele;
	critico.Enter();
	pele = ColaLocal.m_primero;
	if (pele)
	{		
		p = (char *)pele->_dato;
		*size = (int)pele->_size;
		ColaLocal.m_primero = pele->m_siguiente;
		if (!ColaLocal.m_primero)
		{
			ColaLocal.m_ultimo = NULL;
		}
		free((void *)pele);
	}
	critico.Leave();
	return p;
}

void pon_en_cola_envio(char *p,unsigned short size,bool destructivo)
{
	C_Elemento *pele;
	critico.Enter();

    pele = (C_Elemento *)malloc(sizeof(C_Elemento));
	memset((void *)pele,0,sizeof(C_Elemento));
	pele->_dato = malloc(size);
	pele->_size = size;
	memcpy(pele->_dato,p,size);

	if (!ColaEnvio.m_primero)
	{
		ColaEnvio.m_primero = pele;
	}
	else
	{
		if (destructivo)
		{
			C_Elemento *muere;
			muere = ColaEnvio.m_primero;
			ColaEnvio.m_primero = muere->m_siguiente;
			free((void *)muere->_dato);
			free((void *)muere);
			if (!ColaEnvio.m_primero)
			{
				ColaEnvio.m_ultimo = NULL;
				ColaEnvio.m_primero = pele;
			}
		}
	}
	if (ColaEnvio.m_ultimo)
	{
		ColaEnvio.m_ultimo->m_siguiente = pele;
	}
	ColaEnvio.m_ultimo = pele;

	critico.Leave();
}

static long myname = 0L;
void PonTheName(long nuevoname)
{
	myname = nuevoname;
	{
		HKEY hKey;
		LONG retv;
		DWORD Disposition;
		DWORD Type;
		DWORD Size;
		if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "ID",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS)
		{
		  Size = sizeof(myname);
		  Type = REG_DWORD;		  
		  retv = RegSetValueEx(  hKey,"id",NULL,Type,(unsigned char *)&myname,Size);
		  RegCloseKey(hKey);
		}
	}
}	

static long myserver = 0L;

void OverrideServer(long nuevoserver)
{
	myserver = nuevoserver;
}

void PonTheServer(long nuevoserver)
{
	myserver = nuevoserver;
	{
		HKEY hKey;
		LONG retv;
		DWORD Disposition;
		DWORD Type;
		DWORD Size;
		if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "ID",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS)
		{
		  Size = sizeof(myserver);
		  Type = REG_DWORD;		  
		  retv = RegSetValueEx(  hKey,"srv",NULL,Type,(unsigned char *)&myserver,Size);
		  RegCloseKey(hKey);
		}
	}
}

long dame_the_localip()
{
	return (long)_thename.sin_addr.S_un.S_addr;
}

void no_name() {
	noname = 1;
}

long dame_the_name()
{	

	if (!myname && !noname)
	{
		HKEY hKey;
		LONG retv;
		DWORD Disposition;
		DWORD Type;
		DWORD Size;
		if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "ID",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS)
		{
		  Size = sizeof(myname);
		  Type = REG_DWORD;		  
		  retv = RegQueryValueEx(  hKey,"id",NULL,&Type,(unsigned char *)&myname,&Size);
		  /*
		  if (!myname)
		  {
			  myname = (long)_thename.sin_addr.S_un.S_addr;
		  }
		  */
		  RegCloseKey(hKey);
		}
	}	
	return (long)myname;
}

void lee_accesos(char *accesos) {
HKEY hKey;
LONG retv;
DWORD Disposition;
DWORD Type;
DWORD Size;
	if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
					 "ACCESOS",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
					 &hKey,&Disposition)) == ERROR_SUCCESS)
	{
	  Size = 2048;
	  Type = REG_SZ;		  
	  retv = RegQueryValueEx(  hKey,"accesos",NULL,&Type,(unsigned char *)accesos,&Size);
	  /*
	  if (!myname)
	  {
		  myname = (long)_thename.sin_addr.S_un.S_addr;
	  }
	  */
	  RegCloseKey(hKey);
	}
}


void pon_accesos(char *accesos) {
HKEY hKey;
LONG retv;
DWORD Disposition;
DWORD Type;
DWORD Size;
	if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
					 "ACCESOS",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
					 &hKey,&Disposition)) == ERROR_SUCCESS)
	{
	  Size = strlen(accesos)+1;
	  Type = REG_SZ;		  
	  retv = RegSetValueEx(  hKey,"accesos",NULL,Type,(unsigned char *)accesos,Size);	  
	  RegCloseKey(hKey);
	}
}


void lee_ultimo_acceso(char *accesos) {
HKEY hKey;
LONG retv;
DWORD Disposition;
DWORD Type;
DWORD Size;
	if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
					 "ACCESOS",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
					 &hKey,&Disposition)) == ERROR_SUCCESS)
	{
	  Size = 255;
	  Type = REG_SZ;		  
	  retv = RegQueryValueEx(  hKey,"ultimo",NULL,&Type,(unsigned char *)accesos,&Size);
	  /*
	  if (!myname)
	  {
		  myname = (long)_thename.sin_addr.S_un.S_addr;
	  }
	  */
	  RegCloseKey(hKey);
	}
}


void pon_ultimo_acceso(char *accesos) {
HKEY hKey;
LONG retv;
DWORD Disposition;
DWORD Type;
DWORD Size;
	if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
					 "ACCESOS",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
					 &hKey,&Disposition)) == ERROR_SUCCESS)
	{
	  Size = strlen(accesos)+1;
	  Type = REG_SZ;		  
	  retv = RegSetValueEx(  hKey,"ultimo",NULL,Type,(unsigned char *)accesos,Size);	  
	  RegCloseKey(hKey);
	}
}


long dame_the_server()
{	

	if (!myserver)
	{
		HKEY hKey;
		LONG retv;
		DWORD Disposition;
		DWORD Type;
		DWORD Size;
		if ((retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\DSP",0,
						 "ID",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
						 &hKey,&Disposition)) == ERROR_SUCCESS)
		{
		  Size = sizeof(myserver);
		  Type = REG_DWORD;		  
		  retv = RegQueryValueEx(  hKey,"srv",NULL,&Type,(unsigned char *)&myserver,&Size);
		  if (!myserver)
		  {
#ifdef OTP
				myserver = (long)inet_addr("10.255.39.200"); 
#else
#ifdef ADADE		
				myserver = (long)inet_addr("172.0.0.230"); 
#else
				/*addr.sin_addr.s_addr = inet_addr("10.255.0.24"); */
				myserver = (long)inet_addr("217.13.81.39");
#endif
#endif			  
		  }
		  RegCloseKey(hKey);
		}
	}	
	return (long)myserver;
}


bool esta_conectado()
{
	return conectado;
}

bool envia_estado()
{
	bool r = _envia_estado;
	_envia_estado = FALSE;
	return r;
}

static BOOL rwi_senddata(SOCKET sock,char  *sendbuf,int totalbytes)
{
		 int bytessent;
         int i;			         
         
         do 
         {
            bytessent = 0;			  
            while(bytessent < totalbytes)
            {
               i = send(sock,(char *)sendbuf + bytessent,totalbytes - bytessent,0);
               if (i == SOCKET_ERROR)
               {
#ifdef WIN32
                  if (WSAGetLastError() != WSAEWOULDBLOCK)
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
                  
                  continue;
               }
               else
                  bytessent += i;
            }
            
            return TRUE;
            
         } while(-1);
         return FALSE;
}

static void envia_de_cola(SOCKET servidor)
{	
	unsigned short firma = (unsigned short)DS0SIGNATURE;
	C_Elemento *pele;
	critico.Enter();
	pele = ColaEnvio.m_primero;
	if (pele)
	{	
		if (rwi_senddata(servidor,(char *)&firma,sizeof(firma)))
		{
		   if (rwi_senddata(servidor,(char *)&(pele->_size),sizeof(pele->_size)))
		   {
			  if (rwi_senddata(servidor,(char *)pele->_dato,(int)pele->_size))
			  {				
				ColaEnvio.m_primero = pele->m_siguiente;
				free((void *)pele->_dato);
				free((void *)pele);
				if (!ColaEnvio.m_primero)
				{
					ColaEnvio.m_ultimo = NULL;
				}
			  }
		   }
		}
	}
	critico.Leave();	
}


static char *saca_mensajes_(SOCKET servidor,char *rcv_buffer,char *pbuffer)
{	
	char *p;
	unsigned short size,resto;		
	int hay = 0;
	for (p = rcv_buffer;p < (pbuffer-3);p++)
	{
		if ( (*(unsigned short *)p) == DS0SIGNATURE )
		{
			hay = 1;
			p += sizeof(unsigned short);
			size = (*(unsigned short *)p);
			p += sizeof(unsigned short);
			resto = (unsigned short)(pbuffer - p);
			if (resto >= size)
			{
				if (xxxavisar)
				{
					char xxx[128];
					sprintf(xxx,"tam= %d",size);
					::MessageBox(NULL,xxx,"ojo",MB_OK);
				}
				pon_en_cola_local(p,size);
				p += size;
				resto -= size;
				if (resto)
				{
					memcpy(rcv_buffer,p,resto);
					pbuffer = rcv_buffer+resto;
					p = rcv_buffer - 1;
				}
				else
				{
					pbuffer = rcv_buffer;
					break;
				}
			}
			else
			{				
				p -= sizeof(unsigned short);				
				p -= sizeof(unsigned short);
				/*
				if (p > rcv_buffer)
				{
					resto += (sizeof(unsigned short)*2);
					memcpy(rcv_buffer,p,resto);
					pbuffer = rcv_buffer+resto;
				}
				*/
				break;
			}
		}
	}
	if (!hay && (pbuffer-rcv_buffer) > (MAXIMO_BUFFER/2))
	{
		memcpy(rcv_buffer,pbuffer-4,4);
		pbuffer = rcv_buffer+4;
	}
	/*
	if (!hay && (pbuffer-rcv_buffer) > 128)
	{
		memcpy(rcv_buffer,pbuffer-4,4);
		pbuffer = rcv_buffer+4;
	}
	*/
	return pbuffer;
}

static int recibe_mensajes(SOCKET servidor,SOCKET escucha)
{
	int err;
	fd_set setread;
	fd_set seterror;
	struct timeval timeout;
	long cuanto;
	int bytesread;    
    char *pbuffer;
	int maximo;
	static char rcv_buffer[MAXIMO_BUFFER+2];
    
	pbuffer = rcv_buffer;

	do
	{
		FD_ZERO(&setread);
		FD_SET(servidor,&setread);
		FD_SET(escucha,&setread);
		FD_ZERO(&seterror);
		FD_SET(servidor,&seterror);
		FD_SET(escucha,&seterror);

		timeout.tv_sec = 0L;
		timeout.tv_usec = 100000L;
		maximo = servidor;
		if (escucha > (unsigned int)maximo)
			maximo = escucha;
		if ((err = select(maximo+1,&setread,NULL,&seterror,&timeout)) == SOCKET_ERROR)
		{
			break;
		}
		if (!err)  
		{
			/* enviar */
			envia_de_cola(servidor);
			continue;
		}
		if (FD_ISSET(escucha,&setread))
		{
			SOCKET conexion;
			int p1[2];
			conexion = accept(escucha,NULL,NULL);
			FastSocket(conexion);
			p1[0] = 180;
			p1[1] = (int)conexion;
			pon_en_cola_local((char *)p1,sizeof(p1));
			continue;
		}

		if (FD_ISSET(servidor,&seterror))
		{
			break;
		}
  	    cuanto = 0L;
		ioctlsocket(servidor,FIONREAD,(unsigned long *)&cuanto);
		if (!cuanto)
		{
			break;
		}
		maximo = MAXIMO_BUFFER - (pbuffer - rcv_buffer);
		if (cuanto > maximo)
		{
			cuanto = maximo;
		}
		if((bytesread = recv(servidor, pbuffer,cuanto,MSG_WAITALL)) == SOCKET_ERROR)
		{					
			continue;
		}
		pbuffer += bytesread;
		pbuffer = saca_mensajes_(servidor,rcv_buffer,pbuffer);
	} while(TRUE);

    return 0;
}


void *CTcontrol::Entry()
{		
	struct sockaddr_in  addr;
	struct sockaddr_in  addr2;
	struct sockaddr_in  addr3;
	int err;
	/*struct hostent *mihost;*/
	unsigned short port;

#ifdef WIN32
	WORD      wVersionRequested;
	WSADATA   wsaData;
#endif

#ifdef WIN32
	wVersionRequested = MAKEWORD(1,1);
	WSAStartup(wVersionRequested, &wsaData);
#endif
	

	do
	{

		/*
		if ( (mihost = gethostbyname("dsp.in")) && mihost->h_length == 4 && mihost->h_addr_list[0])
		{	    
		   addr.sin_addr.s_addr = *((unsigned long *)mihost->h_addr_list[0]);
		}
		*/

		/*addr.sin_addr.s_addr = inet_addr("10.255.0.24");*/
		/*addr.sin_addr.s_addr = dame_the_server();*/

		/*
		addr.sin_family = AF_INET;
		addr.sin_port = htons(21210);
		*/

		/*addr2 = addr;*/
		
		/*
		if ( (mihost = gethostbyname("dsp2.in")) && mihost->h_length == 4 && mihost->h_addr_list[0])
		{	    
		   addr2.sin_addr.s_addr = *((unsigned long *)mihost->h_addr_list[0]);
		}
		*/		
		/*
		addr.sin_family = AF_INET;
		addr.sin_port = htons(21);
		addr.sin_addr.s_addr = dame_the_server();
		for (int xx = 0;xx < 1000;xx++)
		{
			servidor =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			FastSocket(servidor);
			addr.sin_addr.s_addr++;
			connect(servidor,(struct sockaddr *) &addr,sizeof (addr));
		}

		getchar();
		*/


		conectado = FALSE;
		do
		{
			servidor =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

			FastSocket(servidor);

			addr.sin_addr.s_addr = dame_the_server();
			addr2 = addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(21210);

			if (connect(servidor,(struct sockaddr *) &addr,sizeof (addr)) != SOCKET_ERROR)
				break;

			closesocket(servidor);

			servidor =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

			FastSocket(servidor);

			addr.sin_addr.s_addr = dame_the_server();
			addr2 = addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(21210);

			
			if (connect(servidor,(struct sockaddr *) &addr2,sizeof (addr2)) != SOCKET_ERROR)
				break;

			closesocket(servidor);
#ifdef WIN32
			::Sleep(1000);
#else
			sleep(1);
#endif
		} while(-1);

	    err = sizeof(_thename);
        getsockname(servidor,(struct sockaddr *)&(_thename),&err);	

		{

			escucha = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			FastSocket(escucha);

			addr3.sin_addr.s_addr = INADDR_ANY; 
			addr3.sin_family = AF_INET;
			port = 5900 + (htonl(_thename.sin_addr.S_un.S_addr) & 0xff);
			addr3.sin_port = htons(port);

			bind(escucha,(struct sockaddr *) &(addr3),sizeof (addr3));
			listen(escucha, 1);
		}

		conectado = TRUE;
		_envia_estado = TRUE;

		FastSocket(servidor);
	
		recibe_mensajes(servidor,escucha);	

		closesocket(servidor);

		closesocket(escucha);

		conectado = FALSE;

	} while(-1);
	


#ifdef WIN32
	WSACleanup();
#endif

	return NULL;
}

