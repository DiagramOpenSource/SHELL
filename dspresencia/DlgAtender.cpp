

// DlgAtender.cpp: implementation of the CDlgAtender class.
//
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#include "resource.h"
#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "DlgAtender.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DAEMON_CLASS_NAME "DSTVNCviewer Daemon"
extern UINT MENU_ADD_CLIENT_MSG;
extern UINT MENU_DISABLE_LOCAL_INPUT;
extern UINT MENU_ENABLE_LOCAL_INPUT;



int ListaPorIp(long de,wxString &nombre);


BEGIN_EVENT_TABLE(CDlgAtender, wxWindow)    
EVT_SIZE(CDlgAtender::OnSize)	
EVT_BUTTON(-1, CDlgAtender::OnButton)
END_EVENT_TABLE()

CDlgAtender::CDlgAtender(CDlgMensaje* dlgmensaje,wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style) : 	wxWindow(parent, id,pos,size,style,title)
{	

	m_version = -1;

	m_dlgmensaje = dlgmensaje;

	m_actualizar.Create(this, -1, "Actualizar",wxPoint(210,0),wxSize(-1,18));
	/*m_actualizar.Enable(FALSE);*/
	m_reiniciar.Create(this, -1, "Reiniciar",wxPoint(210,0),wxSize(-1,18));

	m_solover.Create(this, -1, "Solo Ver",wxPoint(0,0),wxSize(60,18));
	m_exclusivo.Create(this, -1, "Exclusivo",wxPoint(0,0),wxSize(60,18));
	m_compartido.Create(this, -1, "Compartido",wxPoint(0,0),wxSize(60,18));

	m_info.Create(this,-1,"",wxPoint(60,0),wxSize(150,30),wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH);	
	
	m_sizerLeft = new wxBoxSizer(wxVERTICAL);
	m_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_sizerRight = new wxBoxSizer(wxVERTICAL);


	m_sizerLeft->Add(&m_solover, 0, wxALIGN_TOP | wxTOP|wxRIGHT, 0);
	m_sizerLeft->Add(&m_exclusivo, 0, wxALIGN_TOP | wxTOP|wxRIGHT, 0);
	m_sizerLeft->Add(&m_compartido, 0, wxALIGN_TOP | wxTOP|wxRIGHT, 0);

	m_sizerRight->Add(&m_actualizar, 0, wxALIGN_TOP | wxTOP, 0);
	m_sizerRight->Add(&m_reiniciar, 0, wxALIGN_TOP | wxTOP|wxRIGHT, 0);
	
	m_sizer->Add(m_sizerLeft, 0, wxALIGN_TOP | wxTOP, 10);	
	m_sizer->Add(&m_info, 1, wxALIGN_TOP | wxTOP | wxGROW, 5);
	m_sizer->Add(m_sizerRight, 0, wxALIGN_TOP | wxTOP, 10);	
	
	
    SetAutoLayout(TRUE);
    SetSizer(m_sizer);	
    m_sizer->Fit(this);	
	m_sizer->SetSizeHints(this);	
}

CDlgAtender::~CDlgAtender()
{

}

void CDlgAtender::OnButton(wxCommandEvent& event)
{
	char mensaje[10240];
	char *p;
	int e;		
	long yo = dame_the_name();
	unsigned short comando;
	HWND hwnd;

	if ( event.GetEventObject() ==  &m_solover)
	{		
		hwnd = ::FindWindow(DAEMON_CLASS_NAME,DAEMON_CLASS_NAME);
		if (hwnd)
		{
			::PostMessage(hwnd,MENU_DISABLE_LOCAL_INPUT,(WPARAM)0,(LPARAM)0);
			p = mensaje;
			e = 18; // comando telesoporte 
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&m_dlgmensaje->m_ip,sizeof(long));
			p += sizeof(long);				
			
			comando = C_DATA;
			memcpy(p,(void *)&comando,sizeof(comando));
			
			p += sizeof(comando);
			comando = 29; // enable input
			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);		
			pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
		}
	}
	else if ( event.GetEventObject() ==  &m_exclusivo)
	{
		hwnd = ::FindWindow(DAEMON_CLASS_NAME,DAEMON_CLASS_NAME);
		if (hwnd)
		{
			::PostMessage(hwnd,MENU_ENABLE_LOCAL_INPUT,(WPARAM)0,(LPARAM)0);
			p = mensaje;
			e = 18; // comando telesoporte 
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&m_dlgmensaje->m_ip,sizeof(long));
			p += sizeof(long);				
			
			comando = C_DATA;
			memcpy(p,(void *)&comando,sizeof(comando));
			
			p += sizeof(comando);
			comando = 30; // disable input
			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);		
			pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
		}
	}
	else if ( event.GetEventObject() ==  &m_compartido)
	{
		hwnd = ::FindWindow(DAEMON_CLASS_NAME,DAEMON_CLASS_NAME);
		if (hwnd)
		{
			::PostMessage(hwnd,MENU_ENABLE_LOCAL_INPUT,(WPARAM)0,(LPARAM)0);
			p = mensaje;
			e = 18; // comando telesoporte 
			memcpy(p,(void *)&e,sizeof(int));
			p += sizeof(int);
			memcpy(p,(void *)&m_dlgmensaje->m_ip,sizeof(long));
			p += sizeof(long);				
			
			comando = C_DATA;
			memcpy(p,(void *)&comando,sizeof(comando));
			
			p += sizeof(comando);
			comando = 29; // enable input
			memcpy(p,(void *)&comando,sizeof(comando));
			p += sizeof(comando);		
			pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
		}
	}
	else if ( event.GetEventObject() ==  &m_reiniciar )
	{
		p = mensaje;
		e = 18; // comando telesoporte 
		memcpy(p,(void *)&e,sizeof(int));
		p += sizeof(int);
		memcpy(p,(void *)&m_dlgmensaje->m_ip,sizeof(long));
		p += sizeof(long);		

		//plen = (unsigned short *)p;
		
		comando = C_DATA;
		memcpy(p,(void *)&comando,sizeof(comando));
		
		p += sizeof(comando);
		comando = 2; // Tipo DATA 2 reinicio
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);		
		pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
	}
	else if ( event.GetEventObject() ==  &m_actualizar )
	{
		::MessageBox((HWND)GetHandle(),"Enviar dstlsprt.new donde reside dstlsprt.exe, despues pulsar Reiniciar.","INFORMACION",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
}

void CDlgAtender::OnSize(wxSizeEvent& event)
{		
	m_sizer->SetDimension(0,0, GetSize().x,GetSize().y);
	m_sizer->Layout();
	wxWindow::Refresh();
}


int CDlgAtender::Recibe(Paquete *ppaquete)
{	
	unsigned int *p1;
	int lookformore = -1;
	char mensaje[10240],tmp[512];
	char *p;
	int e,file;		
	long yo = dame_the_name();
	unsigned short comando;
	char *dirbase = "";
	char *usuario = "";
	char *remoteip = "";
	
	extern int xxxavisar;


	if (COMANDO(ppaquete) == C_HOLA)
	{
		int version;
		p1 = (unsigned int *)ppaquete->buffer;

		m_dlgmensaje->m_llamar.Enable(TRUE);
		m_dlgmensaje->m_control.SetLabel("Control");
		m_dlgmensaje->m_ficheros.Enable(TRUE);
			
		wxString nombre;


		int item = ListaPorIp(yo,nombre);	
		//unsigned short *plen;

		nombre = "Servicio de Tele-Asistencia";

		p = mensaje;
		e = 18; // comando telesoporte 
		memcpy(p,(void *)&e,sizeof(int));
		p += sizeof(int);
		memcpy(p,(void *)&m_dlgmensaje->m_ip,sizeof(long));
		p += sizeof(long);		

		//plen = (unsigned short *)p;
		
		comando = C_DATA;
		memcpy(p,(void *)&comando,sizeof(comando));
		
		p += sizeof(comando);
		comando = 1; // Tipo DATA 1 foto e identificacion bmpstd
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);
		comando = strlen(nombre)+1; // len nombre 
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);	
		strcpy(p,nombre);
		p += comando;
		comando = 0;


		extern HINSTANCE wxhInstance;
		HRSRC hrsrc;
		hrsrc = FindResource(wxhInstance,MAKEINTRESOURCE(IDR_TEATIENDEN),"DLL");
		if (hrsrc)
		{
			HGLOBAL h;
			int len;
			len = (int)SizeofResource(wxhInstance,hrsrc);
			h = LoadResource(wxhInstance,hrsrc);
			if (h)
			{
				void *pxx;
				pxx = LockResource(h);
				if (pxx) {
					memcpy(p+sizeof(comando),pxx,len);
					comando = len;
				}
			}
			DeleteObject(hrsrc);
		}

		/*
		sprintf(tmp,"c:\\dsp\\%lX.dsi",yo);
		file = open(tmp,O_RDONLY|O_BINARY);
		if (file >= 0)
		{
			lseek(file,sizeof(long),SEEK_SET);
			comando = read(file,p+sizeof(comando),10000);
			close(file);
		}
		*/
		memcpy(p,(void *)&comando,sizeof(comando));
		p += sizeof(comando);
		p += comando;

		//*plen = C_DATA 
		pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);

		version = (*p1 & 0xffff0000) >> 16;
		remoteip = (char *)ppaquete->buffer+sizeof(int)*2;
		if (version >= 10)
		{
			dirbase = (char *)ppaquete->buffer+sizeof(int)*2+25;
			usuario = (char *)ppaquete->buffer+sizeof(int)*2+275;
		}
		sprintf(tmp,"Versión módulo telesoporte: %05d\nNúmero INI:%06d NumUsur:%s Dirbase:%s RemoteIp:%s",version,*(p1+1),usuario,dirbase,remoteip);
		if (version < 8)
		{
			strcat(tmp,"->ACTUALIZAR SOS<-");
		}
		m_info.SetValue(tmp);
		m_version = version;
		if (version > 0)
		{
			/*m_actualizar.Enable(TRUE);*/
		}
	}
	return lookformore;
}
