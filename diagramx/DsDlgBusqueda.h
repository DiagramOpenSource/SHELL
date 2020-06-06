#pragma once
#include "afxwin.h"


// CDsDlgBusqueda dialog

class CDsDlgBusqueda : public CDialog
{
	DECLARE_DYNAMIC(CDsDlgBusqueda)

public:
	CDsDlgBusqueda(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsDlgBusqueda();

// Dialog Data
	enum { IDD = IDD_BUSCAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_buscaren;
	CString m_desde;
	CString m_hasta;
	CString m_valor;
	afx_msg void OnBnClickedCheck3();
};
