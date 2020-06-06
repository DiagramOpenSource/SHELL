

/* Sistema de interface con el dll que contiene la programacion antigua */
/* es una manera de aprovechar la facilidad del C para obviar las declaraciones */


#include <windows.h>
#include "resource.h"
#include "/fgv_10/include/fgvmnexp.h"
#include <stdio.h>
#include "hpdf.h"
#include <setjmp.h>
#ifdef WIN32
#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)
#endif

#include <fcntl.h>

extern void RellenaMiMenu();
extern void PonEnWinsysMenu();
extern void RefrescaIra();
extern void DespachoEnLineas();
extern void CreaMiControl();
extern void EliminaMiControl();
extern void AvisoAControl();
extern void SobreponePantalla();
extern void ReponPantalla();
extern void MensajeEnCabecera();
extern void FuncionesImpresion();
extern void HazVisible();
extern void ControlTrasmision();
extern void PresentaProgreso();
extern void GestionUsuarios();
extern int  ActivarEdita();
extern void PonPantallasEnTab();
extern void MainTitulo();
extern void CargaTraza();
extern void DescargaTraza();
extern void PuntoTraza();
extern void EntradaBinaria();
extern int DesactivaControles();
extern int ActivaControles();

void DespachadorLocal(int despacho,int entero,void *puntero);
void DespachadorLocal9(int despacho,int entero,void *puntero);

BOOL llama_fPreInicializa(void *fff,int *argc,char **argv)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   
   r = (BOOL)funcion(DespachadorLocal,(int *)argc,(char **)argv);
   
   return(r);
}

/****** REALMENTE NO SE USA EN  9 *****************************/
BOOL llama_fPreInicializa9(void *fff,int *argc,char **argv)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   
   r = (BOOL)funcion(DespachadorLocal9,(int *)argc,(char **)argv);
   
   return(r);
}
/**************************************************************/


BOOL llama_fInicializa(void *fff,HINSTANCE hInst,HWND Ventana,int argc,char **argv,BMP_BASE_PAR *marco)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   
   r = (BOOL)funcion((HINSTANCE)hInst,(HWND)Ventana,(int)argc,(char **)argv,(BMP_BASE_PAR *)marco,DespachadorLocal9);
   
   return(r);
}


BOOL llama_fFinaliza(void *fff)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion();
   return(r);
}


int llama_fLogin(void *fff,char *usuario,char *password,char *cambio)
{
   int r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = funcion((char *)usuario,(char *)password,(char *)cambio);
   return(r);
}


BOOL llama_fOpcion(void *fff)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion(DespachadorLocal);
   return(r);
}


BOOL llama_fOpcion9(void *fff)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion(DespachadorLocal9);   
   return(r);
}

BOOL llama_fLogout(void *fff)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion();
   return(r);
}

HWND llama_fDevuelveVentana(void *fff)
{
   HWND r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (HWND)funcion();
   return(r);
}

BOOL llama_fNuevaVentana(void *fff,HINSTANCE hInst,HWND Ventana,BMP_BASE_PAR *marco)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion((HINSTANCE)hInst,(HWND)Ventana,(BMP_BASE_PAR *)marco);
   return r;
}


BOOL llama_fPreparaEjecucion(void *fff,char *exe,char *tmp,char *user,char *pass,int opcion)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion((char *)exe,(char *)tmp,(char *)user,(char *)pass,(int)opcion);
   return r;
}


void llama_fpinta_zona(void *fff,HDC hdc,RECT zona,void *pan)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(hdc,zona,pan);
}


void llama_fmensaje_char(void *fff,UINT nChar, UINT nRepCnt, UINT nFlags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(nChar,nRepCnt,nFlags);
}

void llama_fmensaje_keydown(void *fff,UINT nChar, UINT nRepCnt, UINT nFlags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(nChar,nRepCnt,nFlags);
}

void llama_fmensaje_keyup(void *fff,UINT nChar, UINT nRepCnt, UINT nFlags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(nChar,nRepCnt,nFlags);
}

void llama_fmensaje_ldblclk(void *fff,UINT x,UINT y,UINT Flags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(x,y,Flags);
}

void llama_fmensaje_lbuttondown(void *fff,UINT x,UINT y,UINT Flags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(x,y,Flags);
}

void llama_fmensaje_mousemove(void *fff,UINT x,UINT y,UINT Flags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(x,y,Flags);
}

void llama_fmensaje_lbuttonup(void *fff,UINT x,UINT y,UINT Flags)
{
   //void (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   funcion(x,y,Flags);
}

BOOL llama_fmensaje_command(void *fff,WPARAM wParam,LPARAM lParam)
{
   BOOL r;
   //BOOL (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (BOOL)funcion(wParam,lParam);
   return(r);
}

int  llama_fDespachadorInterno(void *fff,int f,void *p)
{
   int r;
   //int (* WINAPI funcion)() = fff;
   FARPROC funcion = fff;
   r = (int)funcion(f,p);
   return(r);
}

/* Interface PDF */

jmp_buf env;


#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
/*    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);*/
    /*longjmp(env, 1);*/
}



static HPDF_Doc  pdf = NULL;
static HPDF_Font font = NULL;
static HPDF_Font fontn = NULL;
static HPDF_Font fontb = NULL;
static HPDF_Font fontc = NULL;
static HPDF_Font fonte = NULL;
static HPDF_Page page = NULL;
static int tipof = 0;
static int pdfposx, pdfposy;
static int margenx = 10;
static int margeny = 30;
//210 297
static int tamx = 700;
static int tamy = 990;
static int sfont = 12;
static HPDF_REAL linea_y = 0.0,linea_x = 0.0;


static char dirfax[81] = "\\\\cefax\\\\envios\\";


static int oem_a_ansi(unsigned char *c)
{
   switch (*c)
   {
   case     130 :*c=233;break;
   case     129 :*c=252;break;
   case     160 :*c=225;break;
   case     161 :*c=237;break;
   case     162 :*c=243;break;
   case     163 :*c=250;break;
   case     164 :*c=241;break;
   case     165 :*c=209;break;
   case     166 :*c=170;break;
   case     167 :*c=186;break;
   case     168 :*c=191;break;
   case     239 :*c=146;break;
   case     241 :*c=177;break;
   case     246 :*c=247;break;
   case     128 :*c=199;break;
   case     131 :*c=226;break;
   case     132 :*c=228;break;
   case     133 :*c=224;break;
   case     134 :*c=229;break;
   case     135 :*c=231;break;
   case     136 :*c=234;break;
   case     137 :*c=235;break;
   case     138 :*c=232;break;
   case     139 :*c=239;break;
   case     140 :*c=238;break;
   case     141 :*c=236;break;
   case     142 :*c=196;break;
   case     143 :*c=197;break;
   case     144 :*c=201;break;
   case     145 :*c=230;break;
   case     146 :*c=198;break;
   case     147 :*c=244;break;
   case     148 :*c=245;break;
   case     149 :*c=242;break;
   case     150 :*c=251;break;
   case     151 :*c=249;break;
   case     152 :*c=255;break;
   case     153 :*c=214;break;
   case     154 :*c=220;break;
   case     155 :*c=248;break;
   case     156 :*c=163;break;
   case     157 :*c=216;break;
   case     158 :*c=215;break;
   case     159 :*c=131;break;
   case     169 :*c=174;break;
   case     170 :*c=172;break;
   case     171 :*c=189;break;
   case     172 :*c=188;break;
   case     173 :*c=161;break;
   case     174 :*c=171;break;
   case     175 :*c=187;break;
   case     176 :*c=128;break;
   case     177 :*c=20; break;
   case     178 :*c=21; break;
   case     179 :*c=124;break;
   case     180 :*c=22; break;
   case     181 :*c=193;break;
   case     182 :*c=194;break;
   case     183 :*c=192;break;
   case     184 :*c=169;break;
   case     185 :*c=23; break;
   case     186 :*c=24; break;
   case     187 :*c=25; break;
   case     188 :*c=26; break;
   case     189 :*c=162;break;
   case     190 :*c=165;break;
   case     198 :*c=227;break;
   case     199 :*c=195;break;
   case     196 :*c=151;break;
   /*case     196 :*c='-';break;*/
   case     207 :*c=156;break;
   case     210 :*c=202;break;
   case     211 :*c=203;break;
   case     212 :*c=200;break;
   case     214 :*c=205;break;
   case     224 :*c=211;break;
   case     225 :*c=223;break;
   case     227 :*c=210;break;
   case     230 :*c=181;break;
   case     233 :*c=218;break;
   case     235 :*c=217;break;  
   }
   return 0;
}



static int saca_texto(unsigned char *p,int escritos,HPDF_REAL x)
{
	unsigned char c;
	//int i;

	c = p[escritos];
	p[escritos] = 0;
	if ((x + (strlen(p)*linea_x)) > tamx)
	{		
		tamx = (int)(x + (strlen(p)*linea_x));
		HPDF_Page_SetWidth (page, (float)tamx);		
	}	
/*
	for (i = 0;i < escritos;i++)
	{
		oem_a_ansi(p+i);
	}
*/
	HPDF_Page_BeginText (page);
	HPDF_Page_TextOut(page,x,(HPDF_REAL)pdfposy,p);
	HPDF_Page_EndText (page);
	p[escritos] = c;
	return 0;
}



static char *quita_path(char *ff)
{
   char *f0 = ff;
   ff += strlen(ff);
   while (ff >= f0 && *ff != '\\' && *ff != '/') ff--;
   ff++;
   if (ff == f0 && *f0 && *(f0+1) == ':')
   {
      ff += 2;
   }
   return(ff);
}

static HINSTANCE mapil = NULL;	
ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG) = NULL;
void Email(char *email,char *subject,char *pdf)
{	
	static char address[512];		
	MapiRecipDesc to[2];		
	int nError;
	MapiMessage message;
	MapiFileDesc  File;

	if (!mapil)
	{
		mapil = LoadLibraryA("MAPI32.DLL");
		if (!mapil)
		{
		   MessageBox(NULL,"No se puede abrir el programa de correo.","ATENCION",MB_ICONHAND|MB_OK);
		   return;
		}
		lpfnSendMail = (ULONG (PASCAL *)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG))GetProcAddress(mapil, "MAPISendMail");
		if (lpfnSendMail == NULL)
		{		
		    MessageBox(NULL,"No se puede abrir el programa de correo.","ATENCION",MB_ICONHAND|MB_OK);
			FreeLibrary(mapil);
			mapil = NULL;
			return; 
		}	
	}		

	memset(to, 0, sizeof(to));

	/*
	to[0].ulRecipClass = MAPI_ORIG;
	to[0].lpszAddress = "SMTP:francesc@diagram.es";
	to[0].lpszName  = "SMTP:francesc@diagram.es";
	*/

	to[0].ulRecipClass = MAPI_TO;
	sprintf(address,"SMTP:%s",email);
	to[0].lpszAddress = address;
	to[0].lpszName  = email;
	
	memset(&message, 0, sizeof(message));
	if (email)
	{
		/*
		char *buff = NULL;
		long n;
		int f;
		f = open("d:\\test.rtf",O_RDONLY|O_BINARY);
		if (f >= 0)
		{
			n = lseek(f,SEEK_END,0);
			lseek(f,SEEK_SET,0);
			buff = malloc(n+1);
			read(f,buff,n);
			close(f);
		}
		*/		
		message.nRecipCount = 1;
		message.lpRecips = to;
		message.lpszSubject = subject;
		message.lpszNoteText =NULL;
		
		memset(&File,0,sizeof(File));
        File.flFlags = 0;
        File.nPosition = -1;
        File.lpszPathName = pdf;
        File.lpszFileName = quita_path(pdf);
        File.lpFileType = NULL;
		message.nFileCount = 1;
		message.lpFiles = &File;
		
		/*** TEST ***/
		/*MessageBox(NULL,to[0].lpszAddress,to[0].lpszName,MB_ICONHAND|MB_OK);
		MessageBox(NULL,message.lpszSubject,File.lpszPathName,MB_ICONHAND|MB_OK);
		MessageBox(NULL,File.lpszFileName,File.lpszPathName,MB_ICONHAND|MB_OK);*/
		/*** /TEST ***/

	    nError = lpfnSendMail(0, (ULONG)NULL,
		&message, MAPI_LOGON_UI|MAPI_DIALOG|MAPI_NEW_SESSION, 0);
		if (nError != SUCCESS_SUCCESS && nError != MAPI_E_USER_ABORT)
		{
			char tmp[512];
			sprintf(tmp,"No se puede abrir el programa de correo (%d).",nError);
			MessageBox(NULL,tmp,"ATENCION",MB_ICONHAND|MB_OK);
		}
		/*
		if (buff)
			free(buff);
		*/
		/*
		else
			Sleep(100000^);
		*/
	}
	unlink(pdf);
}

int linea_a_pdf(unsigned char *linea)
{
	int l,i;
	HPDF_REAL x,y,xtxt;	
	int pintados;
	int escritos;
	unsigned char *p;
	HPDF_Font nfont;	

	/*
	if (setjmp(env)) 
	{
		return 1;
	}
	*/

	if (!pdf)
	{
		pdf = HPDF_New (error_handler, NULL);
		if (!pdf) 
		{
			//printf ("error: cannot create PdfDoc object\n");
			return 1;
		}
		pdfposx = margenx;
		pdfposy = 0;
		// set compression mode 
		HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

		// create default-font
		
		fontn = HPDF_GetFont (pdf, "Courier", NULL);
		fontb = HPDF_GetFont (pdf, "Courier-Bold", NULL);
		fontc = HPDF_GetFont (pdf, "Courier", NULL);
		fonte = HPDF_GetFont (pdf, "Courier", NULL);
		tipof = 0;
		font = fontn;
	}

	if (pdfposy <= sfont)
	{	

		page = HPDF_AddPage (pdf);

		/*
		HPDF_Page_SetHeight (page, (float)tamy);
		HPDF_Page_SetWidth (page, (float)tamx);
		*/
	    tamy = (int)HPDF_Page_GetHeight (page);
        tamx = (int)HPDF_Page_GetWidth (page);

		pdfposx = margenx;
		pdfposy = tamy - margeny;

		if (tipof == 2)
			HPDF_Page_SetFontAndSize (page, font, ((float)sfont)*(float)2);
		else if (tipof == 1)
			HPDF_Page_SetFontAndSize (page, font, ((float)sfont)*(float)10.0/(float)17.0);
		else
			HPDF_Page_SetFontAndSize (page, font, (float)sfont);

		HPDF_Page_SetGrayFill (page, 0);
		HPDF_Page_SetGrayStroke (page, 0);	

		HPDF_Page_SetLineWidth (page, 1.0);

        linea_x = HPDF_Page_TextWidth(page,"MMMMMMMMMM") / 10;
		linea_y = (HPDF_REAL)sfont;
	}

	l = strlen(linea);
	while(l && (linea[l-1] == 10 || linea[l-1] == 13)) l--;
    if (l)
    {
        linea[l] = 0;
        if (linea[0] == '#' && linea[l-1] == '#')
        {
            if (!strcmp((char *)linea,"#FIN#"))
            {
				pdfposy = 0;				
            }
            if (!pdfposy || (l > 2 && linea[1] != '#'))
                return 0;
		}

		x = (HPDF_REAL)pdfposx;
		y = (HPDF_REAL)pdfposy + linea_y/2;
		xtxt = x;
	
		pintados = 0;
		escritos = 0;
		p = linea;
		nfont = NULL;		
		for (i = 0;linea[i];i++)
		{            
			if ((x + linea_x) > tamx)
			{				
				tamx = (int)(x + linea_x);
				HPDF_Page_SetWidth (page, (float)tamx);				
			}	

			if (linea[i] == 9)
			{
				if (pintados)
				{
					HPDF_Page_Stroke(page);
					pintados = 0;
				}
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_BeginText (page);
				HPDF_Page_TextOut(page,x,(HPDF_REAL)pdfposy,"        ");
				HPDF_Page_EndText (page);
				x += (linea_x*8);
				continue;
			}
            if (linea[i] == 139)
            {
                nfont = font;
				tipof = 1;
            }
            else if (linea[i] == 140)
            {
                nfont = font;
				tipof = 0;
            }
            else if (linea[i] == 141)
            {
				nfont = fontb;				
            }
            else if (linea[i] == 142)
            {
				nfont = fontn;				
			}
            else if (linea[i] == 143)
            {           
				nfont = font;
				tipof = 2;
            }
            else if (linea[i] == 144)
            {				      
                nfont = font;
				tipof = 0;
            }
			if (nfont)
			{
				if (pintados)
				{
					HPDF_Page_Stroke(page);
					pintados = 0;
				}
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				font = nfont;
				nfont = NULL;
				if (tipof == 2)
					HPDF_Page_SetFontAndSize (page, font, ((float)sfont)*(float)2);
				else if (tipof == 1)
					HPDF_Page_SetFontAndSize (page, font, ((float)sfont)*(float)10.0/(float)17.0);
				else
					HPDF_Page_SetFontAndSize (page, font, (float)sfont);
				linea_x = HPDF_Page_TextWidth(page,"MMMMMMMMMM") / 10;
				continue;
			}

			if (linea[i] ==  196 || linea[i] == 205)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  179 || linea[i] == 186)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				pintados++;
			}
			else if (linea[i] ==  218 || linea[i] == 201 || linea[i] == 214 || linea[i] == 213)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  194 || linea[i] == 203 || linea[i] == 210 || linea[i] == 209)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				pintados++;
			}
			else if (linea[i] ==  191 || linea[i] == 187 || linea[i] == 183 || linea[i] == 184)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x-1,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  195 || linea[i] == 204 || linea[i] == 199 || linea[i] == 198)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				HPDF_Page_MoveTo(page,x+linea_x/2,y);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				pintados++;
			}
			else if (linea[i] ==  197 || linea[i] == 206 || linea[i] == 215 || linea[i] == 216)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				HPDF_Page_MoveTo(page,x,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  180 || linea[i] == 185 || linea[i] == 182 || linea[i] == 181)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y);
				HPDF_Page_LineTo (page,x+linea_x/2,y-linea_y);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x-1,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  192 || linea[i] == 200 || linea[i] == 211 || linea[i] == 212)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y+1);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  193 || linea[i] == 202 || linea[i] == 208 || linea[i] == 207)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y+1);
				HPDF_Page_MoveTo(page,x,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x,y-linea_y/2);
				pintados++;
			}
			else if (linea[i] ==  217 || linea[i] == 188 || linea[i] == 189 || linea[i] == 190)
			{
				if (escritos)
					escritos = saca_texto(p,escritos,xtxt);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x+linea_x/2,y+1);
				HPDF_Page_MoveTo(page,x+linea_x/2,y-linea_y/2);
				HPDF_Page_LineTo (page,x-1,y-linea_y/2);
				pintados++;
			}
			else
			{
				if (pintados)
				{
					HPDF_Page_Stroke(page);
					pintados = 0;
				}
				if (!escritos)
				{
					xtxt = x;
					p = linea+i;
				}
				escritos++;
			}
			x += linea_x;
		}
		if (pintados)
		{
			HPDF_Page_Stroke(page);
			pintados = 0;
		}
		if (escritos)
			escritos = saca_texto(p,escritos,xtxt);
		pdfposy -= sfont;
	}
	else
		pdfposy -= sfont;

    return 0;	
}

int fin_de_pdf(char *salida)
{
    char tmp[512];
	char correofax[512];
	char ejecuta[512];
	char asuntofax[256];
	int *p;
	char *pc;
	int i;
	FILE *f;
	char sale;
	HPDF_STATUS ok;
	int ii;

	if (!pdf)
		return 1;

	asuntofax[0] = 0;
	ejecuta[0] = 0;

	p = (int *)salida;
	salida += sizeof(int);
	sale = *salida;
	salida++;
	if (*p == 1) /* fax */
	{	
		if (*salida == '!' || *salida == '-')
		{
			sale = *salida;
			salida++;
		}
		else
			sale = '-';
		correofax[0] = 0;
		for (i = 0;salida[i] && salida[i] != '*';i++);
		if (salida[i])
		{
			strcpy(correofax,salida+i+1);
			salida[i] = 0;
		}
		i = 1;
		while(-1)
		{
			sprintf(tmp,"%s%s__%d.txt",dirfax,salida,i);
			f = fopen(tmp,"r");
			if (!f)
				break;
			fclose(f);
			i++;
		}
		for (ii =0;correofax[ii];ii++)
		{
			if (correofax[ii] == '&')
			{
				correofax[ii] = 0;
				strcpy(asuntofax,correofax+ii+1);
				break;
			}
		}
		f = fopen(tmp,"w");
		fprintf(f,"NUMERO FAX:%s\r\n",salida);
		fprintf(f,"Correo Notificacion:%s\r\n",correofax);
		if (asuntofax[0])
			fprintf(f,"Asunto Notificacion:%s\r\n",asuntofax);
		fclose(f);
		sprintf(tmp,"%s%s__%d.pdf",dirfax,salida,i);
	}
	else if (*p == 2) /* correo */
	{
		if (*salida == '!' || *salida == '-')
		{
			sale = *salida;
			salida++;
		}
		else
			sale = '-';
		correofax[0] = 0;
		for (i = 0;salida[i] && salida[i] != '*';i++);
		if (salida[i])
		{
			strcpy(correofax,salida+i+1);
			salida[i] = 0;
		}
		strcpy(ejecuta+1,salida);
		ejecuta[0] = 1;
		sprintf(tmp,"c:\\%08d.pdf",(int)rand());
	}
	else
	{
		strcpy(tmp,salida);
	}
	if (sale == '!')
	{
		strcpy(ejecuta,tmp);
	}
	/*
	if (setjmp(env)) 
	{
		HPDF_Free (pdf);
		pdf = NULL;
		return 1;
	}
	*/
	ii = 1;
	do
	{
		f = fopen(tmp,"r");		
		if (f)
		{
			fclose(f);
			f = fopen(tmp,"w");
			if (f)
			{
				fclose(f);
				break;
			}
			pc = strstr(tmp,".pdf");
			if (!pc)
				pc = strstr(tmp,".PDF");
			if (!pc)
				pc = tmp+strlen(tmp);
			sprintf(pc,"%02d.pdf",ii);
			if (ejecuta[0])
			{
				strcpy(ejecuta,tmp);
			}
			ii++;
			continue;
		}
		break;
	} while(ii < 99);
	ok = HPDF_SaveToFile (pdf, tmp);
	if (ok != HPDF_OK)
	{
		MessageBox(NULL,"No se puede crear el fichero PDF de destino, pruebe otro nombre o camino","ATENCION",MB_ICONHAND|MB_OK);
	}

    // clean up 
    HPDF_Free (pdf);

	pdf = NULL;

	if (ejecuta[0] == 1)
	{
		//_beginthread(Email,NULL,(void *)email);
		Email(correofax,ejecuta+1,tmp);		
	}
	else if (ejecuta[0])
	{
		ShellExecute(NULL, "open",ejecuta,NULL,NULL, SW_SHOWNORMAL);
	}
	return 0;
}


