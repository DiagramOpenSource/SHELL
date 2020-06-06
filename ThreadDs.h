
#ifdef YANOEXISTE

#if !defined(AFX_THREADDS_H__B7BDBDC9_892F_11D2_BDF1_004F4905CBCF__INCLUDED_)
#define AFX_THREADDS_H__B7BDBDC9_892F_11D2_BDF1_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreadDs.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CThreadDs thread
class CEntornoDs;

class CThreadDs : public CWinThread
{
   friend CEntornoDs;
   DECLARE_DYNCREATE(CThreadDs)
protected:
   CThreadDs();           // protected constructor used by dynamic creation
   
   // Attributes
public:
   
   // Operations
public:
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CThreadDs)
public:
   virtual BOOL InitInstance();
   virtual int ExitInstance();
   virtual int Run();
   //}}AFX_VIRTUAL
   
   // Implementation
protected:
   virtual ~CThreadDs();
   
   // Generated message map functions
   //{{AFX_MSG(CThreadDs)
   // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG
   
   DECLARE_MESSAGE_MAP()
private:
   CEntornoDs * m_pEDs;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADDS_H__B7BDBDC9_892F_11D2_BDF1_004F4905CBCF__INCLUDED_)

#endif