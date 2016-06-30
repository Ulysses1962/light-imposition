#pragma once
#include "PreviewDoc.h"
#include "PreviewFrame.h"

// CStub view
#define ID_STUB 5000

class CStub : public CView
{
	DECLARE_DYNCREATE(CStub)

protected:
	CStub();           // protected constructor used by dynamic creation
	virtual ~CStub();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


