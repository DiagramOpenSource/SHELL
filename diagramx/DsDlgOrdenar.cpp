// DsDlgOrdenar.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsDlgOrdenar.h"


// CDsDlgOrdenar dialog

IMPLEMENT_DYNAMIC(CDsDlgOrdenar, CDialog)
CDsDlgOrdenar::CDsDlgOrdenar(CWnd* pParent /*=NULL*/)
	: CDialog(CDsDlgOrdenar::IDD, pParent)
{
}

CDsDlgOrdenar::~CDsDlgOrdenar()
{
}

void CDsDlgOrdenar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORDENARPOR, m_ordenar);
	DDX_Control(pDX, IDC_LUEGOPOR, m_luegopor);
	DDX_Control(pDX, IDC_LUEGOPOR2, m_luegopor2);
}


BEGIN_MESSAGE_MAP(CDsDlgOrdenar, CDialog)
END_MESSAGE_MAP()


// CDsDlgOrdenar message handlers
