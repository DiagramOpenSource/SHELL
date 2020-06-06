// DlgAtender.h: interface for the CDlgAtender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGATENDER_H__E2647CE0_60FC_4874_A7A2_1E0C54928136__INCLUDED_)
#define AFX_DLGATENDER_H__E2647CE0_60FC_4874_A7A2_1E0C54928136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgAtender : public wxWindow
{
public:
	CDlgAtender(CDlgMensaje* dlgmensaje,wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = 0);
	~CDlgAtender();

	CDlgMensaje *m_dlgmensaje;

	wxButton m_solover;
	wxButton m_exclusivo;
	wxButton m_compartido;
	wxButton m_reiniciar;
	wxButton m_actualizar;
	wxTextCtrl m_info;
	wxBoxSizer *m_sizer;
	wxBoxSizer *m_sizerLeft;
	wxBoxSizer *m_sizerRight;

	int m_version;

	void OnButton(wxCommandEvent& event);
	int Recibe(Paquete *ppaquete);
	virtual void OnSize(wxSizeEvent& event);	
private:
    DECLARE_EVENT_TABLE()
};


#endif // !defined(AFX_DLGATENDER_H__E2647CE0_60FC_4874_A7A2_1E0C54928136__INCLUDED_)
