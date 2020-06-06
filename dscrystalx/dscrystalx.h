// dscrystalx.h : main header file for the DSCRYSTALX application
//

#if !defined(AFX_DSCRYSTALX_H__64A0835F_BA43_4490_98D6_BCF65D24B326__INCLUDED_)
#define AFX_DSCRYSTALX_H__64A0835F_BA43_4490_98D6_BCF65D24B326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDscrystalxApp:
// See dscrystalx.cpp for the implementation of this class
//

class CDscrystalxApp : public CWinApp
{
public:
	CDscrystalxApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDscrystalxApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDscrystalxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCRYSTALX_H__64A0835F_BA43_4490_98D6_BCF65D24B326__INCLUDED_)
