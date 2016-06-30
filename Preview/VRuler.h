#pragma once
#include "PreviewDoc.h"


// CVRuler view

class CVRuler : public CView
{
	DECLARE_DYNCREATE(CVRuler)
	int m_NUMLongLines;
	float m_NUMLongLinesStep;

	int m_StartPos;
	int m_MousePos;
	int m_OMousePos;
	BOOL m_MouseDRAW;

	BOOL UpdateRulerScale();
	CDC m_VirtScreenDC;
	CBitmap m_ScaleBMP;
	int m_ScreenH, m_ScreenW, m_DY;


protected:
	CVRuler();           // protected constructor used by dynamic creation
	virtual ~CVRuler();

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
	void SetMousePos(int pos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


