// Tcontrol.h: interface for the CTcontrol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCONTROL_H__5CB33FFA_696B_4EEF_A92C_2372378E4E79__INCLUDED_)
#define AFX_TCONTROL_H__5CB33FFA_696B_4EEF_A92C_2372378E4E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTcontrol : public wxThread  
{
public:
	virtual void *Entry();
};

#endif // !defined(AFX_TCONTROL_H__5CB33FFA_696B_4EEF_A92C_2372378E4E79__INCLUDED_)
