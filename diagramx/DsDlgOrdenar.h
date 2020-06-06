#pragma once
#include "afxwin.h"


// CDsDlgOrdenar dialog

class CDsDlgOrdenar : public CDialog
{
	DECLARE_DYNAMIC(CDsDlgOrdenar)

public:
	CDsDlgOrdenar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsDlgOrdenar();

// Dialog Data
	enum { IDD = IDD_ORDENAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ordenar;
	CComboBox m_luegopor;
	CComboBox m_luegopor2;
};
