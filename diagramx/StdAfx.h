// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C5CBD6B9_C954_4D94_92B8_9600DD76F46E__INCLUDED_)
#define AFX_STDAFX_H__C5CBD6B9_C954_4D94_92B8_9600DD76F46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x5000

//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>
#include <atlconv.h>


#include <process.h>

#ifdef DSXTLIB
#include <XTToolkitPro.h> // Xtreme Toolkit Pro component library
#endif

#ifdef DSPROFUIS

#define DSPROFUIS9

#include < Prof-UIS.h >
#include <../ProfSkin/ProfSkin.h>


#define CBASEDLG   CExtResizableDialog
#define CDLGBUTTON CDsExtButton
#define CDLGLABEL  CExtLabel
#define CDLGEDIT   CExtEdit
#define CDLGCOMBO  CExtComboBox
#define CDLGGROUP  CExtGroupBox
#define CDLGRADIO  CExtRadioButton
#define CDLGCHECK  CExtCheckBox
#define BASELISTCTRL CListCtrl
//#define CBASELISTCTRL CExtNCSB < CListCtrl >
#define CBASELISTCTRL CListCtrl
#define DLGCONS m_bShowResizingGripper = false

#ifdef DSPROFUIS9
#define BASEDsTab9 CDsTabCtrl
#else
#define BASEDsTab9 CTabCtrl
#endif

class CDsExtButton : public CExtButton
{
	DECLARE_DYNCREATE(CDsExtButton)
public:
	CDsExtButton();
};


#else


#define CBASEDLG CDialog
#define CDLGBUTTON CButton
#define CDLGLABEL  CStatic
#define CDLGEDIT   CEdit
#define CDLGCOMBO CComboBox
#define BASELISTCTRL CListCtrl
#define CBASELISTCTRL CListCtrl
#define DLGCONS /* */
#define BASEDsTab9 CTabCtrl
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C5CBD6B9_C954_4D94_92B8_9600DD76F46E__INCLUDED_)
