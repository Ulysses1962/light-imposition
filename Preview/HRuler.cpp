// HRuler.cpp : implementation file
//

#include "../stdafx.h"
#include "PreviewFrame.h"
#include "HRuler.h"


// CHRuler

IMPLEMENT_DYNCREATE(CHRuler, CView)

CHRuler::CHRuler()
{
	m_StartPos = 0;
	m_MousePos = 0;
	m_OMousePos = 0;
	m_MouseDRAW = TRUE;
	m_ScreenH = 20;

}

CHRuler::~CHRuler()
{
}

BEGIN_MESSAGE_MAP(CHRuler, CView)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CHRuler drawing

void CHRuler::OnDraw(CDC* pDC)
{
	CPreviewDoc* pDoc = (CPreviewDoc*)GetDocument();
	if (pDoc->IsPageReady())
	{
		pDC->StretchBlt(-m_StartPos, 0, m_ScreenW, m_ScreenH, &m_VirtScreenDC,
			0, 0, m_ScreenW, m_ScreenH, SRCCOPY);
		
		//--------------------------------------------------
		// Отрисовка маркера мыши
		//--------------------------------------------------
		if (m_MouseDRAW)
		{
			COLORREF m_MColor = RGB(200, 0, 0);

			CPen* m_OldPen;
			CPen m_MPen_A(PS_SOLID, 3, m_MColor);
			CPen m_MPen_B(PS_SOLID, 1, m_MColor);
			m_OldPen = pDC->SelectObject(&m_MPen_A);
			pDC->MoveTo(m_MousePos, 0);
			pDC->LineTo(m_MousePos, 5);
			pDC->SelectObject(&m_MPen_B);
			pDC->MoveTo(m_MousePos, 5);
			pDC->LineTo(m_MousePos, 20);
			pDC->SelectObject(m_OldPen);

		}
	}
}

void CHRuler::DrawLPoints(CDC* pDC, CDocument* pDoc)
{
	CPoint m_StartPoint, m_EndPoint;
	CPen m_GridPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* m_OldPen = pDC->SelectObject(&m_GridPen);

	float scale = ((CPreviewDoc*)pDoc)->GetMainViewScale();
	CSize m_Page = ((CPreviewDoc*)pDoc)->GetPageRealSize();

	CFont m_rulerFont;
	CFont* m_OldFont;
	CString m_Mark;
	// Создаем шрифт для отрисовки меток шкалы
	m_rulerFont.CreateFont(10, 6, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		DEFAULT_PITCH | FF_MODERN, _T("Arial"));
	
	for (int i = 0; i <= m_NUMLongLines; i++)
	{
		float m_DivK; 
		m_StartPoint.x = m_DX + i * m_NUMLongLinesStep * scale;
		m_StartPoint.y = 5;
		m_EndPoint.x = m_StartPoint.x;
		m_EndPoint.y = 20;
		// Рисуем крупные деления
		pDC->MoveTo(m_StartPoint);
		pDC->LineTo(m_EndPoint);
	
		// Рисуем мелкие деления
		CPen m_LightGridPen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen* m_OldGridPen = pDC->SelectObject(&m_LightGridPen);
		
		MEASURING_UNIT m_U = ((CPreviewDoc*)pDoc)->GetMeasuringUnit();
		m_U == MU_INCH ? m_DivK = 4.0 : m_DivK = 2.0;
		float m_ShortLinesStep;
		m_U == MU_INCH ? m_ShortLinesStep = (float)m_NUMLongLinesStep / 8.0 : m_ShortLinesStep = (float)m_NUMLongLinesStep / 10.0;
		float m_PreciseStep = m_ShortLinesStep / m_DivK;

		int m_NUMSLines;
		if (i < m_NUMLongLines)
		{
			m_U == MU_INCH ? m_NUMSLines = 8 : m_NUMSLines = 10;
		}
		else
		{
			float delta = m_Page.cx - i * m_NUMLongLinesStep;
			m_NUMSLines = (int)ceilf(delta / m_ShortLinesStep);
		}
		for (int j = 0; j < m_NUMSLines; j++)
		{
			int m_SmallLineStart;
			if (j > 0)
			{
				int m_SLTreshold;
				m_U == MU_INCH ? m_SLTreshold = 4 : m_SLTreshold = 5;
				j == m_SLTreshold ? m_SmallLineStart = 10 : m_SmallLineStart = 12;
				pDC->MoveTo((int)((float)m_StartPoint.x + j * m_ShortLinesStep * scale), m_SmallLineStart);
				pDC->LineTo((int)((float)m_StartPoint.x + j * m_ShortLinesStep * scale), 20);
			}
			// Если большой масштаб - рисуем точную шкалу
			if (scale > 1.0)
			{
				m_SmallLineStart = 15;
				CPen m_PrecisePen(PS_SOLID, 1, RGB(200, 200, 200));
				CPen* m_OldPPen;
				m_OldPPen = pDC->SelectObject(&m_PrecisePen);
				for (int k = 1; k < (int)m_DivK; k++)
				{
					pDC->MoveTo((int)((float)m_StartPoint.x + j * m_ShortLinesStep * scale + k * m_PreciseStep * scale), m_SmallLineStart);
					pDC->LineTo((int)((float)m_StartPoint.x + j * m_ShortLinesStep * scale + k * m_PreciseStep * scale), 20);
				}
				pDC->SelectObject(m_OldPPen);
			}
		}
		pDC->SelectObject(m_OldGridPen);

		if (i > 0)
		{
			// Рисуем метки шкалы
			m_OldFont = pDC->SelectObject(&m_rulerFont);
			pDC->SetTextAlign(TA_TOP | TA_LEFT);
			m_Mark.Format(_T("%d"), i);
			pDC->TextOut(m_StartPoint.x - 10, 3, m_Mark);
			pDC->SelectObject(m_OldFont);
		}
	}
	pDC->SelectObject(&m_OldPen);
}

// CHRuler diagnostics

#ifdef _DEBUG
void CHRuler::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHRuler::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHRuler message handlers

BOOL CHRuler::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR m_NEW_Cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)); 
	::SetClassLong(this->m_hWnd, GCL_HCURSOR, (LONG)m_NEW_Cursor);

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CHRuler::SetRulerStart(int start)
{
	m_StartPos = start; 
	Invalidate();
	UpdateWindow();
}


BOOL CHRuler::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

int CHRuler::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Создаем виртуальный экран, совместимый с дисплеем
	CPaintDC dc(this); 
	m_VirtScreenDC.CreateCompatibleDC(&dc);

	return 0;
}

BOOL CHRuler::UpdateRulerScale()
{
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	if (m_Doc == NULL) return FALSE;

	//----------------------------------------------------------
	// Строим изображение на виртуальном экране
	//----------------------------------------------------------
	if (m_Doc->m_MainViewReady)
	{
		CRect R, M; GetClientRect(&R);
		((CView*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(1, 1))->GetClientRect(&M);
		CSize m_Page = m_Doc->GetPageSize();
		m_Page.cx < M.Width() ? m_DX = (M.Width() - m_Page.cx) / 2 : m_DX = 0;
		R.Width() > m_Page.cx ? m_ScreenW = R.Width() : m_ScreenW = m_Page.cx + (R.Width() - M.Width());
		//-------------------------------------------------------------
		// Готовим контекст и битовую карту для образа шкалы
		//-------------------------------------------------------------
		CPaintDC dc(this); 
		// Создаем виртуальный экран, совместимый с дисплеем
		// Если растр существует - удаляем
		if (m_ScaleBMP.GetSafeHandle())
		{
			CBitmap m_TMP; m_TMP.CreateCompatibleBitmap(&dc, 1, 1);
			m_VirtScreenDC.SelectObject(&m_TMP);
			// Удаляем старый растр
			m_ScaleBMP.DeleteObject();
			// На его месте создаем новый 
			BOOL res = m_ScaleBMP.CreateCompatibleBitmap(&dc, m_ScreenW, m_ScreenH);
			// Выбираем новый растр в контекст виртуального экрана
			m_VirtScreenDC.SelectObject(m_ScaleBMP);
		}
		else
		{
			BOOL res = m_ScaleBMP.CreateCompatibleBitmap(&dc, m_ScreenW, m_ScreenH);
			// Выбираем новый растр в контекст виртуального экрана
			m_VirtScreenDC.SelectObject(m_ScaleBMP);
		}
		//-------------------------------------------------------------
		// Рисуем шкалу
		//-------------------------------------------------------------
		// Рисуем поля
		if (m_DX > 0)
		{
			m_VirtScreenDC.FillSolidRect(0, 0, m_DX, 20, RGB(105, 119, 151));
			m_VirtScreenDC.FillSolidRect(m_DX + m_Page.cx, 0, R.Width() - m_DX - m_Page.cx, 20, RGB(105, 119, 151));
		}
		else
		{
			m_VirtScreenDC.FillSolidRect(m_Page.cx, 0, R.Width() - M.Width(), 20, RGB(105, 119, 151));
		}
		// Готовим поле для рисования
		m_VirtScreenDC.FillSolidRect(m_DX, 0, m_Page.cx, 20, RGB(255, 250, 235));
		// Определим шаг линейки
		CSize m_RPage = m_Doc->GetPageRealSize();
		MEASURING_UNIT m_U = m_Doc->GetMeasuringUnit();
		float K;
		m_U == MU_INCH ? K = 1.0 : K = 2.54;
		// Определим количество штрихов линейки
		m_NUMLongLinesStep = (float)m_VirtScreenDC.GetDeviceCaps(LOGPIXELSX) / K;
		m_NUMLongLines = (float)m_RPage.cx / m_NUMLongLinesStep; 
		// Рисуем собственно шкалу
		DrawLPoints(&m_VirtScreenDC, m_Doc);
		return TRUE;
	}

	return FALSE;
}
void CHRuler::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	BOOL res = UpdateRulerScale();
	CView::OnUpdate(pSender, lHint, pHint);
}

void CHRuler::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_OMousePos != point.x)
	{
		CRect R;
		R.bottom	= 20;
		R.top		= 0;
		R.left		= m_OMousePos - 5;
		R.right		= m_OMousePos + 5;
		m_OMousePos = point.x;
		m_MouseDRAW = FALSE;
		InvalidateRect(&R, FALSE);
		UpdateWindow();
		m_MousePos = point.x;
		R.bottom	= 20;
		R.top		= 0;
		R.left		= m_MousePos - 5;
		R.right		= m_MousePos + 5;
		m_MouseDRAW = TRUE;
		InvalidateRect(&R, FALSE);
		UpdateWindow();
	}
}
