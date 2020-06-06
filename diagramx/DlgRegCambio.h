#pragma once
#include "afxwin.h"
#include "TablaView.h"

// DlgRegCambio dialog

class DlgRegCambio : public CExtNCW < CBASEDLG >
{
	DECLARE_DYNAMIC(DlgRegCambio)

public:
	DlgRegCambio(CTablaView *pTabla,CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgRegCambio();

	void pondesdehasta();
// Dialog Data
	enum { IDD = IDD_DIALOGCAMBIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDLGCOMBO Usuario;
	/*
	CDLGEDIT Desde;
	CDLGEDIT Hasta;
	*/
	/*
	CDateTimeCtrl Desde;
	CDateTimeCtrl Hasta;
	*/
	CExtDateTimeWnd Desde;
	CExtDateTimeWnd Hasta;

	CDLGCOMBO Campo;
	CDLGEDIT ValorAnterior;
	CDLGEDIT ValorNuevo;

	CDLGEDIT desdeh;
	CDLGEDIT hastah;

	CDLGRADIO m_radio1;
	CDLGRADIO m_radio2;
	CDLGRADIO m_radio3;	
	CDLGRADIO m_radio4;
	CDLGRADIO m_radio5;
	CDLGRADIO m_radio6;

	CDLGLABEL m_label1;
	CDLGLABEL m_label2;
	CDLGLABEL m_label3;
	CDLGLABEL m_label4;
	CDLGLABEL m_label5;
	CDLGLABEL m_label6;

	CDLGCHECK m_check1;


	CDLGBUTTON m_boton1;
	CDLGBUTTON m_boton2;

	CTablaView *m_pTabla;
	CString m_desdehora;
	CString m_hastahora;
	CString m_usu;
	CString m_desde;
	CString m_hasta;
	CString m_valant;
	CString m_valnue;
	int m_campo;
	int m_sel;
	BOOL m_notabla;
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
