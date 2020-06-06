// DlgVolume.cpp: implementation of the CDlgVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#include "resource.h"
#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "DlgVolume.h"


#define VALOR_VOLUMEN(a) (10-(a->dwActualVolume*10/a->dwMaximalVolume))

////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
	wxSlider m_rem_rmic;
	wxSlider m_rem_rmas;
	wxSlider m_rem_pwav;
	wxSlider m_rem_pmas;
	
	wxCheckBox m_rem_ronmic;
	wxCheckBox m_rem_ronmas;
	wxCheckBox m_rem_lonwav;
	wxCheckBox m_rem_lonmas;


	wxStaticBox m_local;
	wxSlider m_loc_rmic;
	wxSlider m_loc_rmas;	
	wxSlider m_loc_pwav;
	wxSlider m_loc_pmas;
	
	wxCheckBox m_loc_ronmic;
	wxCheckBox m_loc_ronmas;	
	wxCheckBox m_loc_lonwav;
	wxCheckBox m_loc_lonmas;
*/

#define DLGVOLUME_IDLOCRMIC 6253
#define DLGVOLUME_IDLOCRMAS 6254
#define DLGVOLUME_IDLOCPWAV 6255
#define DLGVOLUME_IDLOCPMAS 6256
#define DLGVOLUME_IDREMRMIC 6257
#define DLGVOLUME_IDREMRMAS 6258
#define DLGVOLUME_IDREMPWAV 6259
#define DLGVOLUME_IDREMPMAS 6260
#define DLGVOLUME_IDLOCRONMIC 6261
#define DLGVOLUME_IDLOCRONMAS 6262
#define DLGVOLUME_IDLOCLONWAV 6263
#define DLGVOLUME_IDLOCLONMAS 6264
#define DLGVOLUME_IDREMRONMIC 6265
#define DLGVOLUME_IDREMRONMAS 6266
#define DLGVOLUME_IDREMLONWAV 6267
#define DLGVOLUME_IDREMLONMAS 6268


#define DLGVOLUME_IDREMRONCTR 6269
#define DLGVOLUME_IDREMRONMUT 6270
#define DLGVOLUME_IDLOCRONCTR 6271
#define DLGVOLUME_IDLOCRONMUT 6272


/*
    *id = LOWORD(wParam);
    *hwnd = (WXHWND)lParam;
    *cmd = HIWORD(wParam);
*/


BEGIN_EVENT_TABLE(CDlgVolume, wxWindow)    
  EVT_COMMAND_RANGE(DLGVOLUME_IDLOCRONMIC, DLGVOLUME_IDREMLONMAS, wxEVT_COMMAND_CHECKBOX_CLICKED, CDlgVolume::OnCheck)
  EVT_COMMAND_RANGE(DLGVOLUME_IDLOCRMIC, DLGVOLUME_IDREMPMAS, wxEVT_COMMAND_SLIDER_UPDATED, CDlgVolume::OnSlider)
  EVT_COMMAND_RANGE(DLGVOLUME_IDREMRONCTR, DLGVOLUME_IDLOCRONMUT, wxEVT_COMMAND_CHECKBOX_CLICKED, CDlgVolume::OnMutes)
END_EVENT_TABLE()

CDlgVolume::CDlgVolume(CDlgMensaje* dlgmensaje,wxWindow* parent,
							 wxWindowID id,
							 const wxString& title, 
							 const wxPoint& pos,
							 const wxSize& size ,
							 long style) : 	wxWindow(parent, id,pos,size,style,title)
{	
	int p;
	int tam = 45;
	int sep = 2;

	m_ticksrvolume = 0;

	for (int i = 0;i < 5;i++)
	{
		VolumesR[i] = NULL;
	}
	m_ronctr = 0;
	m_lonmut = 0;

	m_dlgmensaje = dlgmensaje;
	m_remoto.Create(this, -1, "Remoto",wxPoint(2,0),wxSize((tam+sep)*4+4,135),0);

	/*volumen    *Volumes[5] = {&VolSpeaker,&VolWaveOut,&VolWaveIn,&VolMicrophone,NULL};*/
	p = 6;
    m_rem_rmic.Create(this, DLGVOLUME_IDREMRMIC , 0 , 0, 10,  wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_LEFT,wxDefaultValidator);	
	if (VolumesR[3])
		m_rem_rmic.Enable(TRUE);
	else
		m_rem_rmic.Enable(FALSE);	
	p += tam;
	p += sep;
	m_rem_rmas.Create(this, DLGVOLUME_IDREMRMAS , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_LEFT, wxDefaultValidator);
	if (VolumesR[2])
		m_rem_rmas.Enable(TRUE);
	else
		m_rem_rmas.Enable(FALSE);
	p += tam;
	p += sep;
	m_rem_pwav.Create(this, DLGVOLUME_IDREMPWAV , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_LEFT, wxDefaultValidator);
	if (VolumesR[1])
		m_rem_pwav.Enable(TRUE);
	else
		m_rem_pwav.Enable(FALSE);
	p += tam;
	p += sep;
	m_rem_pmas.Create(this, DLGVOLUME_IDREMPMAS , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_LEFT, wxDefaultValidator);
	if (VolumesR[0])
		m_rem_pmas.Enable(TRUE);
	else
		m_rem_pmas.Enable(FALSE);

	p = 6;
	m_rem_ronmic.Create(this, DLGVOLUME_IDREMRONMIC , "Mic",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);	
	m_rem_ronctr.Create(this, DLGVOLUME_IDREMRONCTR , "Pulsar Ctrl",wxPoint(p,120), wxSize(80,10), 0, wxDefaultValidator);	
	
	if (VolumesR[3])
		m_rem_ronmic.Enable(TRUE);
	else
		m_rem_ronmic.Enable(FALSE);

	p += tam;
	p += sep;
	m_rem_ronmas.Create(this, DLGVOLUME_IDREMRONMAS , "Rec",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);	
	if (VolumesR[2])
		m_rem_ronmas.Enable(TRUE);
	else
		m_rem_ronmas.Enable(FALSE);
	p += tam;
	p += sep;
	m_rem_lonwav.Create(this, DLGVOLUME_IDREMLONWAV , "Wav",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);
	m_rem_lonmut.Create(this, DLGVOLUME_IDREMRONMUT , "Silenciar",wxPoint(p,120), wxSize(80,10), 0, wxDefaultValidator);

	if (VolumesR[1])
		m_rem_lonwav.Enable(TRUE);
	else
		m_rem_lonwav.Enable(FALSE);

		m_rem_ronctr.Enable(FALSE);

		m_rem_lonmut.Enable(FALSE);
	
	p += tam;
	p += sep;
	m_rem_lonmas.Create(this, DLGVOLUME_IDREMLONMAS , "Mas",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);
	if (VolumesR[0])
		m_rem_lonmas.Enable(TRUE);
	else
		m_rem_lonmas.Enable(FALSE);

	m_local.Create(this, -1, "Local",wxPoint((tam+sep)*4+6,0),wxSize((tam+sep)*4+6,135),0);

	p = (tam+sep)*4+10;
    m_loc_rmic.Create(this, DLGVOLUME_IDLOCRMIC , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_RIGHT, wxDefaultValidator);	
	p += tam;
	p += sep;
	m_loc_rmas.Create(this, DLGVOLUME_IDLOCRMAS , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_RIGHT, wxDefaultValidator);
	p += tam;
	p += sep;
	m_loc_pwav.Create(this, DLGVOLUME_IDLOCPWAV , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_RIGHT, wxDefaultValidator);
	p += tam;
	p += sep;
	m_loc_pmas.Create(this, DLGVOLUME_IDLOCPMAS , 0 , 0, 10, wxPoint(p,20), wxSize(tam,80), wxSL_VERTICAL | wxSL_AUTOTICKS | wxSL_RIGHT, wxDefaultValidator);	
	p = (tam+sep)*4+10;
	m_loc_ronmic.Create(this, DLGVOLUME_IDLOCRONMIC , "Mic",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);	
	m_loc_ronctr.Create(this, DLGVOLUME_IDLOCRONCTR , "Pulsar Ctrl",wxPoint(p,120), wxSize(80,10), 0, wxDefaultValidator);	

	p += tam;
	p += sep;
	m_loc_ronmas.Create(this, DLGVOLUME_IDLOCRONMAS , "Rec",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);	
	p += tam;
	p += sep;
	m_loc_lonwav.Create(this, DLGVOLUME_IDLOCLONWAV , "Wav",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);
	m_loc_lonmut.Create(this, DLGVOLUME_IDLOCRONMUT , "Silenciar",wxPoint(p,120), wxSize(80,10), 0, wxDefaultValidator);

	p += tam;
	p += sep;
	m_loc_lonmas.Create(this, DLGVOLUME_IDLOCLONMAS , "Mas",wxPoint(p,102), wxSize(tam,10), 0, wxDefaultValidator);

	AjustaVolumen(0);
	AjustaVolumen(1);
	/*hwndVolumen = (HWND)GetHandle();*/
	m_ticksrvolume = GetTickCount() - 251;
	EnviaRVolumens();
}

CDlgVolume::~CDlgVolume()
{

}


void CDlgVolume::OnSlider(wxCommandEvent &event)
{
  int vol = event.GetId() - DLGVOLUME_IDLOCRMIC;
  wxSlider* slider = (wxSlider *)FindWindow(event.GetId());
  if (!slider || vol < 0 || vol > 7)
	  return;
  if (vol < 4)
  {
	  vol = 3 - vol;
	  if (!Volumes[vol])
		  return;
	  int valor = slider->GetValue();
	  SetCurrentVolumen(Volumes[vol], Volumes[vol]->dwMaximalVolume*(10-valor)/10 ,(DWORD)Volumes[vol]->dwMute);
  }
  else
  {
	  vol -= 4;
	  vol = 3 - vol;
	  int valor = slider->GetValue();
	  if (VolumesR[vol])
	  {
		 VolumesR[vol]->dwActualVolume = VolumesR[vol]->dwMaximalVolume*(10-valor)/10;
		 m_ticksrvolume = ::GetTickCount();		 
	  }
  }
}

void CDlgVolume::OnMutes(wxCommandEvent &event)
{
	int vol = event.GetId() - DLGVOLUME_IDREMRONCTR;
	int valor = (int)event.IsChecked();
	switch(vol)
	{
	case 0:
		m_ronctr = valor;
		break;
	case 1:
		m_lonmut = valor;
		break;
	case 2:
		SetUsarElCtrl(valor);
		break;
	case 3:
        SetNoEnviarAudio(valor);
		break;
	}
	if (vol < 2)
	{
		m_ticksrvolume = ::GetTickCount();
	}	
}


void CDlgVolume::OnCheck(wxCommandEvent &event)
{
  int vol = event.GetId() - DLGVOLUME_IDLOCRONMIC;
  bool valor = event.IsChecked();
  wxCheckBox* box = (wxCheckBox *)FindWindow(event.GetId());
  if (!box || vol < 0 || vol > 7)
	  return;
  if (vol < 4)
  {
	  vol = 3 - vol;
	  if (!Volumes[vol])
		  return;
	  SetCurrentVolumen(Volumes[vol], Volumes[vol]->dwActualVolume ,(DWORD)!valor);
	  box->SetValue(!(BOOL)(Volumes[vol]->dwMute));
  }
  else
  {
	  vol -= 4;
	  vol = 3 - vol;
	  if (VolumesR[vol])
	  {
		 VolumesR[vol]->dwMute = (DWORD)!valor;
		 m_ticksrvolume = ::GetTickCount();
	  }
  }
}

int CDlgVolume::AjustaVolumen(int remoto)
{
	if (remoto)
	{
		if (VolumesR[0])
		{
			m_rem_pmas.Enable(TRUE);
			m_rem_pmas.SetValue(VALOR_VOLUMEN(VolumesR[0]));
		}
		else
			m_rem_pmas.Enable(FALSE);

		if (VolumesR[0] && VolumesR[0]->dwControlMute < (DWORD)-2)
		{
			m_rem_lonmas.Enable(TRUE);
			m_rem_lonmas.SetValue(!(BOOL)(VolumesR[0]->dwMute));
		}
		else
			m_rem_lonmas.Enable(FALSE);

		if (VolumesR[1])
		{
			m_rem_pwav.Enable(TRUE);
			m_rem_pwav.SetValue(VALOR_VOLUMEN(VolumesR[1]));
		}
		else
			m_rem_pwav.Enable(FALSE);
		if (VolumesR[1] && VolumesR[1]->dwControlMute < (DWORD)-2)
		{
			m_rem_lonwav.Enable(TRUE);
			m_rem_lonwav.SetValue(!(BOOL)(VolumesR[1]->dwMute));
		}
		else
			m_rem_lonwav.Enable(FALSE);

		if (VolumesR[2])
		{
			m_rem_rmas.Enable(TRUE);
			m_rem_rmas.SetValue(VALOR_VOLUMEN(VolumesR[2]));
		}
		else
			m_rem_rmas.Enable(FALSE);
		if (VolumesR[2] && VolumesR[2]->dwControlMute < (DWORD)-2)
		{
			m_rem_ronmas.Enable(TRUE);
			m_rem_ronmas.SetValue(!(BOOL)(VolumesR[2]->dwMute));
		}
		else
			m_rem_ronmas.Enable(FALSE);

		if (VolumesR[3])
		{
			m_rem_rmic.Enable(TRUE);
			m_rem_rmic.SetValue(VALOR_VOLUMEN(VolumesR[3]));
		}
		else
			m_rem_rmic.Enable(FALSE);
		if (VolumesR[3] && VolumesR[3]->dwControlMute < (DWORD)-2)
		{
			m_rem_ronmic.Enable(TRUE);
			m_rem_ronmic.SetValue(!(BOOL)(VolumesR[3]->dwMute));
		}
		else
			m_rem_ronmic.Enable(FALSE);

		if (VolumesR[0] || VolumesR[1] || VolumesR[2] || VolumesR[3])
		{
			m_rem_ronctr.Enable(TRUE);
			m_rem_lonmut.Enable(TRUE);
		}
		else
		{
			m_rem_ronctr.Enable(FALSE);
			m_rem_lonmut.Enable(FALSE);
		}

		m_rem_ronctr.SetValue((BOOL)m_ronctr);
		m_rem_lonmut.SetValue((BOOL)m_lonmut);

		return remoto;
	}
	if (Volumes[0])
	{
		m_loc_pmas.Enable(TRUE);
		m_loc_pmas.SetValue(VALOR_VOLUMEN(Volumes[0]));
	}
	else
		m_loc_pmas.Enable(FALSE);

	if (Volumes[0] && Volumes[0]->dwControlMute < (DWORD)-2)
	{
		m_loc_lonmas.Enable(TRUE);
		m_loc_lonmas.SetValue(!(BOOL)(Volumes[0]->dwMute));
	}
	else
		m_loc_lonmas.Enable(FALSE);

	m_loc_lonmut.SetValue((BOOL)NoEnviarAudio);

	if (Volumes[1])
	{
		m_loc_pwav.Enable(TRUE);
		m_loc_pwav.SetValue(VALOR_VOLUMEN(Volumes[1]));
	}
	else
		m_loc_pwav.Enable(FALSE);
	if (Volumes[1] && Volumes[1]->dwControlMute < (DWORD)-2)
	{
		m_loc_lonwav.Enable(TRUE);
		m_loc_lonwav.SetValue(!(BOOL)(Volumes[1]->dwMute));
	}
	else
		m_loc_lonwav.Enable(FALSE);

	if (Volumes[2])
	{
		m_loc_rmas.Enable(TRUE);
		m_loc_rmas.SetValue(VALOR_VOLUMEN(Volumes[2]));
	}
	else
		m_loc_rmas.Enable(FALSE);
	if (Volumes[2] && Volumes[2]->dwControlMute < (DWORD)-2)
	{
		m_loc_ronmas.Enable(TRUE);
		m_loc_ronmas.SetValue(!(BOOL)(Volumes[2]->dwMute));
	}
	else
		m_loc_ronmas.Enable(FALSE);

	if (Volumes[3])
	{
		m_loc_rmic.Enable(TRUE);
		m_loc_rmic.SetValue(VALOR_VOLUMEN(Volumes[3]));
	}
	else
		m_loc_rmic.Enable(FALSE);
	if (Volumes[3] && Volumes[3]->dwControlMute < (DWORD)-2)
	{
		m_loc_ronmic.Enable(TRUE);
		m_loc_ronmic.SetValue(!(BOOL)(Volumes[3]->dwMute));
	}
	else
		m_loc_ronmic.Enable(FALSE);

	m_loc_ronctr.SetValue((BOOL)UsarElCtrl);

	return 0;
}


bool CDlgVolume::MSWCommand(WXUINT param, WXWORD id)
{
	if (id == ID_Volumen)
	{
		AjustaVolumen(0);
		return TRUE;
	}
    return FALSE;
}

int CDlgVolume::EnviaRVolumens()
{
	char mensaje[1024];
	int e;		
	unsigned short com,comando = 28;
	int i;
	char *p;
	int enviaflags = 0;
	//long yo = dame_the_name();

	if (m_dlgmensaje->m_departamento < 90000)
		return -1;

	if (!m_ticksrvolume || (GetTickCount() - m_ticksrvolume) < 250)
		return -1;

	m_ticksrvolume = 0;

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

	for (i = 0;i < 5;i++)
	{
		if (VolumesR[i])
		{
			*p = i+1;
			p++;
			memcpy(p,(void *)VolumesR[i],sizeof(volumen));
			p += sizeof(volumen);		
			enviaflags = 1;
		}
		else
		{
			*p = 0;
			p++;			
		}
	}
	if (enviaflags)
	{
		*p = 24;
		p++;
		*(int *)p = m_ronctr;
		p += sizeof(int);
		*p = 25;
		p++;
		*(int *)p = m_lonmut;
		p += sizeof(int);
	}
	*p = 26;
	p++;		

	pon_en_cola_envio(mensaje,(int)(p-mensaje),FALSE);
	return 0;
}

int CDlgVolume::Recibe(Paquete *ppaquete)
{	
	unsigned short *p1,comando;
	int lookformore = -1;

	if (COMANDO(ppaquete) == C_DATA)
	{
		p1 = (unsigned short *)ppaquete->buffer;
		comando = *p1;		
		if (comando == 28)
		{
		   char *p0;
		   p1++;
		   p0 = (char *)p1;

			int i;
			volumen *pvol;
			int vol;
			for (i = 0;i < 7;i++)
			{
				vol = *p0;
				p0++;
				if (vol == 26) 
					break;
				if (vol == 24)
				{
					m_ronctr = *(int *)p0;
					p0 += sizeof(int);
				}
				else if (vol == 25)
				{
					m_lonmut = *(int *)p0;
					p0 += sizeof(int);
				}
				else if (vol > 0 && vol < 6)
				{
					vol--;
					pvol = (volumen *)p0;
					p0 += sizeof(volumen);					
					/*volumen    *Volumes[5] = {&VolSpeaker,&VolWaveOut,&VolWaveIn,&VolMicrophone,NULL};*/
					if (!VolumesR[vol])
					{
						switch(vol)
						{
						case 0:
							VolumesR[vol] = &VolSpeakerR;
							break;
						case 1:
							VolumesR[vol] = &VolWaveOutR;
							break;
						case 2:
							VolumesR[vol] = &VolWaveInR;
							break;
						case 3:	
							VolumesR[vol] = &VolMicrophoneR;
							break;
						};
					}
					if (VolumesR[vol])
						*VolumesR[vol] = *pvol;
				}				
			}	
			AjustaVolumen(1);
			lookformore = 0;
		}
	}
	return lookformore;
}