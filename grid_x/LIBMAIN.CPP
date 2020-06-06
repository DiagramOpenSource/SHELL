#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE NEAR extensionDLL = { NULL, NULL };
static HINSTANCE hLibInst;

void __export InitWCDLL()
{
	TRACE(_T("\nInitialized WorldCom MFC library\n"));
	new CDynLinkLibrary(extensionDLL);
}


#ifdef WIN32
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH){
		TRACE(_T("Loading WorldCom MFC library\n"));
		hLibInst = hInstance;
		AfxInitExtensionModule(extensionDLL, hInstance);
		new CDynLinkLibrary(extensionDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0(_T("Unloading WorldCom MFC library\n"));
	}
	return 1;
}

#else
int CALLBACK LibMain(HINSTANCE hInstance, WORD, WORD, LPSTR)
{
	TRACE(_T("Loaded WorldCom MFC Library\n"));
	hLibInst = hInstance;
	AfxInitExtensionModule(extensionDLL, hInstance);
	return 1;
}
#endif



HINSTANCE AfxGetLibInstance()
{
	return hLibInst;
}
