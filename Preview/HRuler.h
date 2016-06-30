#pragma once
#include "PreviewDoc.h"
// CHRuler view


class CHRuler : public CView
{
	DECLARE_DYNCREATE(CHRuler)
	int m_NUMLongLines;
	float m_NUMLongLinesStep;

	int m_StartPos;
	int m_MousePos;
	int m_OMousePos;
	BOOL m_MouseDRAW;

	BOOL UpdateRulerScale();
	CDC m_VirtScreenDC;
	CBitmap m_ScaleBMP;
	int m_ScreenH, m_ScreenW, m_DX;


protected:
	CHRuler();           // protected constructor used by dynamic creation
	virtual ~CHRuler();

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
	void DrawLPoints(CDC* pDC, CDocument* pDoc);
	void SetRulerStart(int start);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


