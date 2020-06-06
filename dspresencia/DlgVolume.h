// DlgVolume.h: interface for the CDlgVolume class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGVOLUME_H__9D3577DF_F714_4F98_922A_927181B78571__INCLUDED_)
#define AFX_DLGVOLUME_H__9D3577DF_F714_4F98_922A_927181B78571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgMensaje;

extern "C"
{
typedef struct 
{ 
   DWORD dwLineID;
   DWORD dwControlID;   
   DWORD dwEnabled;
   DWORD dwChannelCount;
   DWORD dwMinimalVolume;
   DWORD dwMaximalVolume;
   DWORD dwVolumeStep;
   DWORD dwActualVolume;
   DWORD dwControlMute;   
   DWORD dwMute;
   DWORD MutedwChannelCount;
   DWORD MutecMultipleItems;
   DWORD MutedwSource;
} volumen;

extern volumen    *Volumes[5];
extern HWND hwndVolumen;
void SetCurrentVolumen( volumen *pvolumen, DWORD dwValue ,DWORD dwMute );
extern int UsarElCtrl;
extern int NoEnviarAudio;
int SetUsarElCtrl(int usar);
int SetNoEnviarAudio(int noenviar);
};

class CDlgVolume : public wxWindow
{
public:
	CDlgVolume(CDlgMensaje* dlgmensaje,wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = 0);
	~CDlgVolume();

	CDlgMensaje *m_dlgmensaje;

	wxStaticBox m_remoto;

	wxSlider m_rem_rmic;
	wxSlider m_rem_rmas;
	wxSlider m_rem_pwav;
	wxSlider m_rem_pmas;
	
	wxCheckBox m_rem_ronmic;
	wxCheckBox m_rem_ronctr;
	wxCheckBox m_rem_ronmas;
	wxCheckBox m_rem_lonwav;
	wxCheckBox m_rem_lonmas;
	wxCheckBox m_rem_lonmut;


	wxStaticBox m_local;
	wxSlider m_loc_rmic;
	wxSlider m_loc_rmas;	
	wxSlider m_loc_pwav;
	wxSlider m_loc_pmas;
	
	wxCheckBox m_loc_ronmic;
	wxCheckBox m_loc_ronctr;
	wxCheckBox m_loc_ronmas;	
	wxCheckBox m_loc_lonwav;
	wxCheckBox m_loc_lonmas;
	wxCheckBox m_loc_lonmut;

	int AjustaVolumen(int remoto);	
	void OnMutes(wxCommandEvent &event);
	void OnCheck(wxCommandEvent &event);
	void OnSlider(wxCommandEvent &event);
	bool MSWCommand(WXUINT param, WXWORD id);
	int Recibe(Paquete *ppaquete);
	int EnviaRVolumens();

	volumen    *VolumesR[5];
	volumen    VolSpeakerR;
	volumen    VolWaveOutR;
	volumen    VolWaveInR;
	volumen    VolMicrophoneR;

	DWORD m_ticksrvolume;

	int m_ronctr;
	int m_lonmut;
private:
    DECLARE_EVENT_TABLE()
};

#endif // !defined(AFX_DLGVOLUME_H__9D3577DF_F714_4F98_922A_927181B78571__INCLUDED_)
