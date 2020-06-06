#pragma once


// CDlgBusqueda dialog
class CTablaView;

class CDlgBusqueda : public CBASEDLG
{
	DECLARE_DYNAMIC(CDlgBusqueda)

public:
	CDlgBusqueda(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBusqueda();

// Dialog Data
	enum { IDD = IDD_BUSCAR };

    CDLGCOMBO m_buscarpor;
	CDLGEDIT m_edit1;
	CDLGEDIT m_edit2;
	CDLGEDIT m_edit3;

	CDLGGROUP m_label1;
	CDLGLABEL m_label2;
	CDLGLABEL m_label3;
	CDLGGROUP m_label4;
	CDLGLABEL m_label5;

	CDLGRADIO m_boton1;
	CDLGRADIO m_boton2;
	CDLGRADIO m_boton3;
	CDLGCHECK m_boton4;
	CDLGCHECK m_boton5;

	CDLGBUTTON m_boton6;
	CDLGBUTTON m_boton7;


	CTablaView *m_pTabla;
	int m_col;
	int m_sel;

    CString m_limitei;
	CString m_limites;
	CString m_valor;
    int m_mayus;
    int m_inversa;
	int m_tipovalor;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnChangeDesde();
	afx_msg void OnChangeHasta();
	afx_msg void OnChangeValor();
	afx_msg void OnRadioexacto();
	afx_msg void OnRadioparcial();
	afx_msg void OnRadiocontenido();

	DECLARE_MESSAGE_MAP()
};
