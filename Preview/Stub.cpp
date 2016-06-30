 // Stub.cpp : implementation file
//

#include "../stdafx.h"
#include "Stub.h"


// CStub

IMPLEMENT_DYNCREATE(CStub, CView)

CStub::CStub()
{
}

CStub::~CStub()
{
}

BEGIN_MESSAGE_MAP(CStub, CView)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CStub drawing

void CStub::OnDraw(CDC* pDC)
{
	CPreviewDoc* pDoc = (CPreviewDoc*)GetDocument();
	// Рисуем заглушку
	RECT c_rect;
	GetClientRect(&c_rect);
	if (pDoc)
	{
		// Отображаем единицы измерения
		CFont m_rulerFont;
		CFont* m_OldFont;
		CString m_Mark;
		// Создаем шрифт для отрисовки меток шкалы
		m_rulerFont.CreateFont(12, 0, 0, 0, FW_BOLD, 0, 0, 0, 
			DEFAULT_CHARSET, OUT_TT_PRECIS, 
			CLIP_DEFAULT_PRECIS, 
			ANTIALIASED_QUALITY, 
			DEFAULT_PITCH | FF_MODERN, _T("Arial"));
		// Рисуем метки шкалы
		m_OldFont = pDC->SelectObject(&m_rulerFont);
		pDC->SetTextColor(RGB(255, 255, 255));
		MEASURING_UNIT m_U = pDoc->GetMeasuringUnit();
		m_U == MU_INCH ? m_Mark = "E" : m_Mark =  "M";
		COLORREF m_EMetricColor = RGB(0, 150, 0);
		COLORREF m_MMetricColor = RGB(200, 0, 200);
		m_U == MU_INCH ? pDC->FillSolidRect(&c_rect, m_EMetricColor) : pDC->FillSolidRect(&c_rect, m_MMetricColor); 
		BOOL res = pDC->DrawText(m_Mark, &c_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SelectObject(m_OldFont);
	}

}


// CStub diagnostics

#ifdef _DEBUG
void CStub::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStub::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStub message handlers

BOOL CStub::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR m_NEW_Cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)); 
	::SetClassLong(this->m_hWnd, GCL_HCURSOR, (LONG)m_NEW_Cursor);

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CStub::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CPreviewDoc* pDoc = (CPreviewDoc*)GetDocument();
	MEASURING_UNIT m_U = pDoc->GetMeasuringUnit();
	m_U == MU_INCH ? m_U = MU_MM : m_U = MU_INCH;
	pDoc->SetMeasuringUnit(m_U);
	((CPreviewFrame*)GetParentFrame())->m_Splitter.SetActivePane(1, 1);

	CView::OnLButtonDblClk(nFlags, point);
}
