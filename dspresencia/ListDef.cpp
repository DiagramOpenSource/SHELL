// ListDef.cpp: implementation of the CListDef class.
//
//////////////////////////////////////////////////////////////////////

#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "ListDef.h"

#include "resource.h"
#ifdef WIN32
#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)
#endif


BEGIN_EVENT_TABLE(CListDef, wxListCtrl)
    EVT_LIST_COL_CLICK(1014, CListDef::OnSelected)
	EVT_LEFT_DCLICK(CListDef::OnDblClick)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern "C"
{
extern void (*write_interno)(int fd,unsigned char *buf,unsigned tam);
extern unsigned (*read_interno)(int *pfd,unsigned char *buf,unsigned tam);
extern int gzip_asignada_memoria;
void write_gunzip(int fd,unsigned char *buf,unsigned tam);
unsigned read_gunzip(int *pfd,unsigned char *buf,unsigned tam);
int ds___gzip(int,int,int,int);
void ld_campo(int campo,register FI_DEF *pfi,char *destino);
void st_campo(int campo,FI_DEF *pfi,char *origen);
double rwatof(char *s);
}


typedef unsigned char Uchar;

char *write_datos = NULL;
long write_long = 0;

void write_gunzip(int fd,unsigned char *buf,unsigned tam)
{
	if (!write_datos)
		return;
	memcpy(write_datos+write_long,buf,tam);
	write_long += tam;
}

char *read_datos = NULL;
long read_long = 0;

unsigned read_gunzip(int *pfd,unsigned char *buf,unsigned tam)
{
   if (!read_datos)
	   return 0;

   if (tam > (unsigned)read_long)
	   tam = read_long;

   if (tam)
   {
	   memcpy(buf,read_datos,tam);
	   read_datos += tam;
	   read_long -= tam;
   }
   return tam;
}

                
long ldlong( char *s )                       /* create a long from an MIF */                    
{
    union { long r;
    char p[4];
    } x;
    x.r = 0;
    
    *x.p = s[3];
    x.p[1] = s[2];
    x.p[2] = s[1];
    x.p[3] = *s;
    return x.r;
}
                                
int stlong( long l,char *p )              /* make MIF from a long */
{
    register char *q;
    
    q = ( char *)&l;
    *p = q[3];
    p[1] = q[2];
    p[2] = q[1];
    p[3] = *q;
    return( 0 );
}

int stfloat(float fl,char *p)
{
    *((float *)p) = fl;
    return( 0 );
}

float ldfloat( char *p )
{
    return(*((float *)p));
}

double lddbl( char *p )
{
    return(*((double *)p));
}

int stdbl( double f,char *p )
{
    *((double *)p) = f;
    return( 0 );
}


int stchar( char *from,char *to,int len )
{
    register char *p,*q;
    register int a;
    
    for( a = len,p = from,q = to;a && *p;a--,p++,q++ )
    { *q = *p; }
    for( ;a;a--,q++ ) *q = ' ';
    return( 0 );
}
                
int ldchar( char *from,int len,char *to )
{
    register char *p;
    memcpy( to,from,len );
    for( p = to + len - 1; p >= to;p-- )
    { if( *p != ' ' )
    { p++; *p = 0; return( 0 ); }
    }
    *(++p) = 0;
    return( 0 );
}


void ldfecha(char *origen,char *destino)
{
   memcpy(destino  ,origen+6,2);
   *(destino+2) = '.';
   memcpy(destino+3,origen+4,2);
   *(destino+5) = '.';
   memcpy(destino+6,origen  ,4);
   *(destino+10) = '\0';
}

void stfecha(char *origen,char *destino)
{
   memcpy(destino  ,origen+6,4);
   memcpy(destino+4,origen+3,2);
   memcpy(destino+6,origen  ,2);
}


void st_campo(int campo,FI_DEF *pfi,char *origen)
{
   char *destino;
   int tipo,offset,longit;
   
   destino = pfi->f_buf;
   tipo = pfi->f_campos[campo]->c_tipo;
   offset = pfi->f_campos[campo]->c_offset;
   longit = pfi->f_campos[campo]->c_long;
   switch (tipo) {
   case CHARTIPO :
      stchar(origen,destino+offset,longit);
      break;
   case FECHATIPO :
      stfecha(origen,destino+offset);
      break;
   case INTTIPO :
      stint(atoi(origen),destino+offset);
      break;
   case LONGTIPO :
      stlong(atol(origen),destino+offset);
      break;
   case DOUBLETIPO :
      stdbl(rwatof(origen),destino+offset);
      break;
   default:
      break;
   }   
}

void ld_campo(int campo,register FI_DEF *pfi,char *destino)
{
   char *origen;
   int tipo,offset,longit;
   char *usi,*min,*max;
   char t[MAXDEFECTO];
   double numd;
   
   origen = pfi->f_buf;
   tipo = pfi->f_campos[campo]->c_tipo;
   offset = pfi->f_campos[campo]->c_offset;
   longit = pfi->f_campos[campo]->c_long;
   usi    = pfi->f_campos[campo]->c_usi;
   min    = pfi->f_campos[campo]->c_minimo;
   max    = pfi->f_campos[campo]->c_maximo;
   switch (tipo) {
   case CHARTIPO :
      ldchar(origen+offset,longit,t);
      t[longit] = '\0';
      /******************************
      sprintf(destino,usi,t);
      *******************************/
      memset(destino,' ',longit);
      destino[longit] = '\0';
      memcpy(destino,t,strlen(t));
      break;
   case FECHATIPO :
      ldfecha(origen+offset,destino);
      break;
   case INTTIPO :
      sprintf(destino,usi,ldint(origen+offset));
      break;
   case LONGTIPO :
      sprintf(destino,usi,ldlong(origen+offset));
      break;
   case DOUBLETIPO :
      numd = lddbl(origen+offset);      
      sprintf(destino,usi,numd);
      break;
   default:
      break;
   }
}

int DescomprimeDatos(char *datosc,long tdatosc,char **d_datos,long *td_datos)
{
	void *_write_interno;
	void *_read_interno;

	_write_interno = (void *)write_interno;
	_read_interno = (void *)read_interno;

	write_interno = write_gunzip;
	read_interno = read_gunzip;

	read_datos = (char *)datosc;
	read_long = tdatosc;

	write_datos = (char *)malloc(tdatosc*30);
	write_long = 0;

	ds___gzip(0,0,1,6);    
	
	write_interno = (void (*)(int,unsigned char *,unsigned))_write_interno;
	read_interno = (unsigned int (*)(int *,unsigned char *,unsigned))_read_interno;

	if (gzip_asignada_memoria > 0)
	{
		 gzip_asignada_memoria = -1;
		 ds___gzip(-1,-1,1,-1);
	}

	*d_datos = write_datos;

	*td_datos = write_long;


	return 0;
}

CListDef::CListDef()
{
	m_def = NULL;
	m_curcol = 0;
}

CListDef::~CListDef()
{

}

BOOL CListDef::Rellena(void *datos,long len,BOOL comprimido,char *path)
{
	void *_write_interno;
	void *_read_interno;
	FILE *fout = NULL;

	if (!m_def)
		return FALSE;

	wxString s;
	int i;

	if (!path)
	{
		for (i = 0;i < m_def->f_ncampos;i++)
		{
			s = m_def->f_campos[i]->c_nombre;
			InsertColumn(i,s,wxLIST_FORMAT_LEFT,-1);	
		}
	}
	else
	{
		fout = fopen(path,"w");
		if (!fout)
			return FALSE;
	}

	if (datos && len)
	{
		if (comprimido)
		{
			_write_interno = (void *)write_interno;
			_read_interno = (void *)read_interno;
    
			write_interno = write_gunzip;
			read_interno = read_gunzip;

			read_datos = (char *)datos;
			read_long = len;

			write_datos = (char *)malloc(len*30);
			write_long = 0;

			ds___gzip(0,0,1,6);    
			
			write_interno = (void (*)(int,unsigned char *,unsigned))_write_interno;
			read_interno = (unsigned int (*)(int *,unsigned char *,unsigned))_read_interno;

			if (gzip_asignada_memoria > 0)
			{
				 gzip_asignada_memoria = -1;
				 ds___gzip(-1,-1,1,-1);
			}
		}
		else
		{
			write_long = len;
			write_datos = (char *)datos;
		}

		
		long pos;
		long item = 0;
		char buf[512];
		char *_buf;
		_buf = m_def->f_buf;
		for (pos = 0;pos < write_long;pos += (m_def->f_lfich+1))
		{
			m_def->f_buf = write_datos + pos;
			if (!m_def->f_buf[m_def->f_lfich])
				continue;

			ld_campo(0,m_def,buf);
			if (!path)
			{
				s = buf;
				InsertItem(item,s);
				SetItemData(item,item);
			}
			else
			{
				fprintf(fout,"%s%c",buf,1);
			}
			for (i = 1;i < m_def->f_ncampos;i++)
			{
				ld_campo(i,m_def,buf);
				if (!path)
				{
					s = buf;
					SetItem(item,i,s);
				}
				else
				{
					if (i < (m_def->f_ncampos-1))
					{
						fprintf(fout,"%s%c",buf,1);
					}
					else
					{
						fprintf(fout,"%s\r\n",buf);
					}
				}
			}
			item++;
		}
		m_def->f_buf = _buf;
		if (!path)
		{
			for (i = 0;i < m_def->f_ncampos;i++)
			{
				SetColumnWidth(i,wxLIST_AUTOSIZE);
			}
		}
		if (comprimido)
		{
			free((void *)write_datos);
		}
	}
	else
	{
		if (!path)
		{
			s = "VACIO";
			InsertItem(0,s);
		}
	}
	if (!path)
	{
		SetItemState(0,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	}
	if (fout)
		fclose(fout);

	return TRUE;
}

int wxCALLBACK wxListCompareFunction(long item1, long item2, long sortData)
{
	CListDef *list = (CListDef *)sortData;
	static wxString s1;
	static wxListItem info;

	info.SetMask(wxLIST_MASK_TEXT);
	info.SetId(item1);
	info.SetColumn(list->m_curcol);
	list->GetItem(info);
	s1 = info.GetText();

	info.SetMask(wxLIST_MASK_TEXT);
	info.SetId(item2);
	info.SetColumn(list->m_curcol);
	list->GetItem(info);	

	return s1.CmpNoCase(info.GetText());
}

void CListDef::OnSelected( wxListEvent &event )
{
	m_curcol = event.GetColumn();
	SortItems(wxListCompareFunction,(long)this);
	for (long i = 0;i < GetItemCount();i++)
	{
		SetItemData(i,i);
	}
}


void CListDef::OnDblClick( wxMouseEvent &event )
{	
	CDlgLista *pdlg = (CDlgLista *)GetParent();
	if (pdlg->m_modeless)
	{
		return;
	}
	pdlg->PonSeleccion();
	pdlg->EndModal(wxID_OK);
}
