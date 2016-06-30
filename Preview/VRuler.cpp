// VRuler.cpp : implementation file
//

#include "../stdafx.h"
#include "PreviewFrame.h"
#include "VRuler.h"


// CVRuler

IMPLEMENT_DYNCREATE(CVRuler, CView)

CVRuler::CVRuler()
{
	m_StartPos = 0;
	m_MousePos = 0;
	m_OMousePos = 0;
	m_MouseDRAW = TRUE;
	m_ScreenW = 20;

}

CVRuler::~CVRuler()
{
}

BEGIN_MESSAGE_MAP(CVRuler, CView)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CVRuler drawing

void CVRuler::OnDraw(CDC* pDC)
{
	CPreviewDoc* pDoc = (CPreviewDoc*)GetDocument();
	if (pDoc->IsPageReady())
	{
		pDC->StretchBlt(0, -m_StartPos, m_ScreenW, m_ScreenH, &m_VirtScreenDC,
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
			pDC->MoveTo(0, m_MousePos);
			pDC->LineTo(5, m_MousePos);
			pDC->SelectObject(&m_MPen_B);
			pDC->MoveTo(5, m_MousePos);
			pDC->LineTo(20, m_MousePos);
			pDC->SelectObject(m_OldPen);
		}
		//--------------------------------------------------
	}
}


// CVRuler diagnostics

#ifdef _DEBUG
void CVRuler::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CVRuler::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVRuler message handlers

BOOL CVRuler::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR m_NEW_Cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)); 
	::SetClassLong(this->m_hWnd, GCL_HCURSOR, (LONG)m_NEW_Cursor);

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CVRuler::DrawLPoints(CDC* pDC, CDocument* pDoc)
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
	m_rulerFont.CreateFont(10, 6, 0, 900, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		DEFAULT_PITCH | FF_MODERN, _T("Arial"));

	for (int i = 0; i <= m_NUMLongLines; i++)
	{
		float m_DivK;
		m_StartPoint.y = m_DY + i * m_NUMLongLinesStep * scale;
		m_StartPoint.x = 5;
		m_EndPoint.y = m_StartPoint.y;
		m_EndPoint.x = 20;
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
			float delta = m_Page.cy - i * m_NUMLongLinesStep;
			m_NUMSLines = (int)ceilf(delta / m_ShortLinesStep);
		}
		for (int j = 0; j < m_NUMSLines; j++)
		{
			int m_SmallLineStart;
			if (j > 0)
			{
				int m_STreshold;
				m_U == MU_INCH ? m_STreshold = 4 : m_STreshold = 5;
				j == m_STreshold ? m_SmallLineStart = 10 : m_SmallLineStart = 12;
				pDC->MoveTo(m_SmallLineStart, (int)((float)m_StartPoint.y + j * m_ShortLinesStep * scale));
				pDC->LineTo(20, (int)((float)m_StartPoint.y + j * m_ShortLinesStep * scale));
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
					pDC->MoveTo(m_SmallLineStart, (int)((float)m_StartPoint.y + j * m_ShortLinesStep * scale + k * m_PreciseStep * scale));
					pDC->LineTo(20, (int)((float)m_StartPoint.y + j * m_ShortLinesStep * scale + k * m_PreciseStep * scale));
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
			pDC->TextOut(3, m_StartPoint.y - 10, m_Mark);
			pDC->SelectObject(m_OldFont);
		}
	}

	pDC->SelectObject(&m_OldPen);
}


void CVRuler::SetRulerStart(int start)
{
	m_StartPos = start; 
	Invalidate();
	UpdateWindow();
}

BOOL CVRuler::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

int CVRuler::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Создаем виртуальный экран, совместимый с дисплеем
	CPaintDC dc(this); 
	m_VirtScreenDC.CreateCompatibleDC(&dc);

	return 0;
}

void CVRuler::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_OMousePos != point.y)
	{
		CRect R;
		R.bottom	= m_OMousePos + 5;
		R.top		= m_OMousePos - 5;
		R.left		= 0;
		R.right		= 20;
		m_OMousePos = point.y;
		m_MouseDRAW = FALSE;
		InvalidateRect(&R, FALSE);
		UpdateWindow();
		m_MousePos  = point.y;
		R.bottom	= m_MousePos + 5;
		R.top		= m_MousePos - 5;
		R.left		= 0;
		R.right		= 20;
		m_MouseDRAW = TRUE;
		InvalidateRect(&R, FALSE);
		UpdateWindow();
	}
}

void CVRuler::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	BOOL res = UpdateRulerScale();
	CView::OnUpdate(pSender, lHint, pHint);
}

BOOL CVRuler::UpdateRulerScale()
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
		m_Page.cy < M.Height() ? m_DY = (M.Height() - m_Page.cy) / 2 : m_DY = 0;
		R.Height() > m_Page.cy ? m_ScreenH = R.Height() : m_ScreenH = m_Page.cy + (R.Height() - M.Height());
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
		if (m_DY > 0)
		{
			m_VirtScreenDC.FillSolidRect(0, 0, 20, m_DY, RGB(105, 119, 151));
			m_VirtScreenDC.FillSolidRect(0, m_DY + m_Page.cy, 20, R.Height() - m_DY - m_Page.cy, RGB(105, 119, 151));
		}
		else
		{
			m_VirtScreenDC.FillSolidRect(0, m_Page.cy, 20, R.Height() - M.Height(), RGB(105, 119, 151));
		}
		// Готовим поле для рисования
		m_VirtScreenDC.FillSolidRect(0, m_DY, 20, m_Page.cy, RGB(255, 250, 235));
		// Определим шаг линейки
		CSize m_RPage = m_Doc->GetPageRealSize();
		MEASURING_UNIT m_U = m_Doc->GetMeasuringUnit();
		float K;
		m_U == MU_INCH ? K = 1.0 : K = 2.54;
		// Определим количество штрихов линейки
		m_NUMLongLinesStep = (float)m_VirtScreenDC.GetDeviceCaps(LOGPIXELSY) / K;
		m_NUMLongLines = (float)m_RPage.cy / m_NUMLongLinesStep; 
		// Рисуем собственно шкалу
		DrawLPoints(&m_VirtScreenDC, m_Doc);
		return TRUE;
	}

	return FALSE;
}