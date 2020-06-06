#if !defined(AFX_CRVTRACKCURSORINFO_H__022277A7_5E99_4D25_9568_6D0FC938159F__INCLUDED_)
#define AFX_CRVTRACKCURSORINFO_H__022277A7_5E99_4D25_9568_6D0FC938159F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCRVTrackCursorInfo wrapper class

class CCRVTrackCursorInfo : public COleDispatchDriver
{
public:
	CCRVTrackCursorInfo() {}		// Calls COleDispatchDriver default constructor
	CCRVTrackCursorInfo(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCRVTrackCursorInfo(const CCRVTrackCursorInfo& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetDetailAreaCursor();
	void SetDetailAreaCursor(long nNewValue);
	long GetDetailAreaFieldCursor();
	void SetDetailAreaFieldCursor(long nNewValue);
	long GetGraphCursor();
	void SetGraphCursor(long nNewValue);
	long GetGroupAreaCursor();
	void SetGroupAreaCursor(long nNewValue);
	long GetGroupAreaFieldCursor();
	void SetGroupAreaFieldCursor(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRVTRACKCURSORINFO_H__022277A7_5E99_4D25_9568_6D0FC938159F__INCLUDED_)