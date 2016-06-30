#pragma once
#include <vector>
#include "HRuler.h"
#include "VRuler.h"
#include "PreviewFrame.h"
#include "PreviewDoc.h"

using namespace std;
// CGridView view


class CGridView : public CScrollView
{
	DECLARE_DYNCREATE(CGridView)
	// Размер образа страницы
	CSize* m_PageSize;
	// Виртуальный экран
	CBitmap* m_VirtScreenBMP;
	CDC m_VirtScreenDC;
	// Начальное смещение образа в представлении
	int delta_X;
	int delta_Y;
	// Размер шкалы пользовательского пространства страницы
	float m_UnitSize;
	// Режим отображения
	int m_MapMode;
	// Текущий к-т масштабирования
	float m_Scale;
	// Текущий инструмент
	PREVIEW_TOOL_TYPE m_OldToolType;
	// Координаты нажатия правой кнопки мыши
	CPoint m_StartPANPoint;


	// Метод отрисовки виртуального экрана
	BOOL UpdateVirtualScreen();
	// Метод рисования сетки
	void DrawGrid(CDC* pDC);

	// Выбранная направляющая линия
	BOOL m_CursorOnGide;
	int m_SelGideIDX;
	CPoint m_OldGideLinePos;
	BOOL m_GidesVisible;
	BOOL m_GideMoving;
	BOOL m_GidesDRAW;
	
	// Перерасчет координат от страничных к клиентским и обратно
	CPoint FromClientToPage(CPoint pt);
	CPoint FromPageToClient(CPoint pt);


protected:
	CGridView();           // protected constructor used by dynamic creation
	virtual ~CGridView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DrawGideLines(CDC* pDC, GIDELINE_ARRAY* A);
	void SetGidesVisible(BOOL visible);
	BOOL GetGidesVisible() { return m_GidesVisible; };
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


