// GridView.cpp : implementation file
//

#include "../stdafx.h"
#include "GridView.h"


// CGridView

IMPLEMENT_DYNCREATE(CGridView, CScrollView)

CGridView::CGridView()
{
	m_PageSize = new CSize(210, 297);
	delta_X = 0;
	delta_Y = 0;
	m_Scale = 1.0;
	m_VirtScreenBMP = new CBitmap();
	m_CursorOnGide = FALSE;
	m_SelGideIDX = -1;
	m_GidesVisible = FALSE;
	m_GideMoving = FALSE;
	m_UnitSize = 1.0;
	m_OldToolType = TOOL_PAN;
	m_GidesDRAW = TRUE;
}

CGridView::~CGridView()
{
	m_VirtScreenBMP->DeleteObject();
	delete m_PageSize;
}


BEGIN_MESSAGE_MAP(CGridView, CScrollView)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	if (m_Doc->IsPageReady())
	{
		//-----------------------------------------------------------
		// Отрисовка образа страницы. При необходимости - центрируем
		//-----------------------------------------------------------
		CRect m_Client; GetClientRect(&m_Client);
		// Выводим образ с учетом смещения
		if (m_PageSize->cx < m_Client.Width()) delta_X = (m_Client.Width() - m_PageSize->cx) / 2;
		else delta_X = 0;
		if (m_PageSize->cy < m_Client.Height()) delta_Y = (m_Client.Height() - m_PageSize->cy) / 2;
		else delta_Y = 0;
		pDC->StretchBlt(delta_X, delta_Y, m_PageSize->cx, m_PageSize->cy, &m_VirtScreenDC, 
			0, m_PageSize->cy, m_PageSize->cx, -m_PageSize->cy, SRCCOPY);

		// Запоминаем смещения
		m_Doc->SetDX(delta_X);
		m_Doc->SetDY(delta_Y);

		if (!m_Doc->m_MainViewReady)
		{
			// Форсируем перерисовку линеек
			m_Doc->m_MainViewReady = TRUE;
			m_Doc->UpdateAllViews(NULL);
		}

		// Отрисовываем границы
		if (delta_X > 0)
		{
			int m_MargineH;
			m_Client.Height() > m_PageSize->cy ? m_MargineH = m_Client.Height(): m_MargineH = m_PageSize->cy;
			pDC->FillSolidRect(0, 0, delta_X, m_MargineH, RGB(105, 119, 151));
			pDC->FillSolidRect(delta_X + m_PageSize->cx, 0, m_Client.Width() - delta_X - m_PageSize->cx, m_MargineH, RGB(105, 119, 151));
		}
		if (delta_Y > 0)
		{
			pDC->FillSolidRect(delta_X, 0, m_PageSize->cx, delta_Y, RGB(105, 119, 151));
			pDC->FillSolidRect(delta_X, delta_Y + m_PageSize->cy - 1, m_PageSize->cx, m_Client.Height() - delta_Y - m_PageSize->cy + 1, RGB(105, 119, 151));
		}
	}
	//-----------------------------------------------------------
	// Рисуем сетку, если необходимо
	//-----------------------------------------------------------
	DrawGrid(pDC);
	//-----------------------------------------------------------
	// Рисуем направляющие линии, если есть такие
	//-----------------------------------------------------------
	GIDELINE_ARRAY* m_GA = m_Doc->GetGidelines();
	if (!(*m_GA).empty() && m_GidesVisible && m_GidesDRAW) DrawGideLines(pDC, m_GA);
}


// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGridView message handlers

void CGridView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	BOOL res = UpdateVirtualScreen();
	if (res) m_MapMode = ((CPreviewDoc*)GetDocument())->GetMapMode(); 
	else m_MapMode = MM_TEXT;
	SetScrollSizes(m_MapMode, CSize(m_PageSize->cx, m_PageSize->cy));

	CScrollView::OnUpdate(pSender, lHint, pHint);
}

int CGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Создаем виртуальный экран, совместимый с дисплеем
	CPaintDC dc(this); 
	m_VirtScreenDC.CreateCompatibleDC(&dc);
	
	return 0;
}

BOOL CGridView::UpdateVirtualScreen()
{
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	if (m_Doc == NULL) return FALSE;

	//----------------------------------------------------------
	// Строим изображение на виртуальном экране
	//----------------------------------------------------------
	if (m_Doc->IsPageReady())
	{
		PDPage m_Page = m_Doc->GetPreviewPDPage();

		// Получим кроп-бокс страницы
		ASFixedRect m_CBox;
		PDPageGetCropBox(m_Page, &m_CBox);
		// Получим матрицу преобразования
		ASFixedMatrix m_DefMatrix;
		PDPageGetDefaultMatrix(m_Page, &m_DefMatrix);
		m_UnitSize = PDPageGetUserUnitSize(m_Page); 
		ASFixedMatrixTransformRect(&m_CBox, &m_DefMatrix, &m_CBox);

		//-------------------------------------------------------------
		// Определимся с размерами битовой карты для хранения образа страницы
		//-------------------------------------------------------------
		// Высота и ширина кропа страницы в пользовательской системе координат
		float pc_h, pc_w;		
		pc_h = ASFixedToFloat(m_CBox.top - m_CBox.bottom);
		pc_w = ASFixedToFloat(m_CBox.right - m_CBox.left);
		// Переводим в дюймы
		pc_h = pc_h * m_UnitSize / 72.0;
		pc_w = pc_w * m_UnitSize / 72.0;
		// Переводим в пиксели
		pc_h *= m_VirtScreenDC.GetDeviceCaps(LOGPIXELSY);
		pc_w *= m_VirtScreenDC.GetDeviceCaps(LOGPIXELSX);
		m_Doc->SetPageRealSize(CSize(pc_w, pc_h));

		// Размеры образа полагаем равными размерам страницы c учетом масштабирования
		SCALE_FACTOR f = m_Doc->GetScaleFactor();
		CRect m_client; GetClientRect(&m_client);
		switch(f)
		{
		case SCALE_FIT_WINDOW :
			{
				float m_Scale_Y = (float)m_client.Height() / (float)pc_h;
				float m_Scale_X = (float)m_client.Width() / (float)pc_w;
				m_Scale_X < m_Scale_Y ? m_Scale = m_Scale_X : m_Scale = m_Scale_Y;
			}
			break;
		case SCALE_FIT_W :
			m_Scale = (float)m_client.Width() / (float)pc_w; 
			break;
		case SCALE_FIT_H :
			m_Scale = (float)m_client.Height() / (float)pc_h; 
			break;
		case SCALE_50  : m_Scale = 0.5; break;
		case SCALE_100 : m_Scale = 1.0; break;
		case SCALE_200 : m_Scale = 2.0; break;
		case SCALE_300 : m_Scale = 3.0; break;
		case SCALE_400 : m_Scale = 4.0; break;
		}

		m_PageSize->cx = pc_w * m_Scale;  
		m_PageSize->cy = pc_h * m_Scale; 

		m_Doc->SetMainViewScale(m_Scale);
		m_Doc->SetPageSize(*m_PageSize);
		//-------------------------------------------------------------
		// Готовим контекст и битовую карту для образа страницы
		//-------------------------------------------------------------
		CPaintDC dc(this); int m_Steps;
		// Создаем виртуальный экран, совместимый с дисплеем
		// Если растр существует - удаляем
		if (m_VirtScreenBMP->GetSafeHandle())
		{
			m_Steps = 1;
			CBitmap m_TMP; m_TMP.CreateCompatibleBitmap(&dc, 1, 1);
			m_VirtScreenDC.SelectObject(&m_TMP);
			// Удаляем старый растр
			m_VirtScreenBMP->DeleteObject();
			// На его месте создаем новый 
			BOOL res = m_VirtScreenBMP->CreateCompatibleBitmap(&dc, m_PageSize->cx, m_PageSize->cy);
			// Выбираем новый растр в контекст виртуального экрана
			m_VirtScreenDC.SelectObject(m_VirtScreenBMP);
		}
		else
		{
			m_Steps = 2;
			BOOL res = m_VirtScreenBMP->CreateCompatibleBitmap(&dc, m_PageSize->cx, m_PageSize->cy);
			// Выбираем новый растр в контекст виртуального экрана
			m_VirtScreenDC.SelectObject(m_VirtScreenBMP);
		}
		//-------------------------------------------------------------
		// Рисуем страницу в буферный контекст
		//-------------------------------------------------------------
		// Получим матрицу масштабирования
		ASFixedMatrix M;
		M.a		= ASFixedDiv(ASFloatToFixed(m_PageSize->cx), m_CBox.right - m_CBox.left);
		M.b		= fixedZero;
		M.c		= fixedZero;
		M.d		= ASFixedDiv(ASFloatToFixed(m_PageSize->cy), m_CBox.top - m_CBox.bottom); 
		M.h		= fixedZero;
		M.v		= fixedZero;
		ASFixedMatrixConcat(&m_DefMatrix, &M, &m_DefMatrix);
		// Отрисовка страницы
		/*      П Р О Б Л Е М А
		В этом месте есть проблема. При обработке некоторых файлов (пока известен единственный
		случай) функция неправильно создает растр при ПЕРВОМ вхождении. Со второго раза
		все рисуется правильно. Причина такого явления пока неизвестна. Потому на случай
		первого вхождения предусмотрено двухкратное повторение процедуры.
		*/
		for (int i = 0; i < m_Steps; i++)
		{
			PDPageDrawContentsToWindow(m_Page, NULL, m_VirtScreenDC.GetSafeHdc(), false, &m_DefMatrix, NULL, NULL, NULL);
		}
		
		return TRUE;
	}

	return FALSE;
}


BOOL CGridView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bRes = CScrollView::DoMouseWheel(nFlags, zDelta, pt);
	
	CVRuler* m_VR = (CVRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(1, 0);
	m_VR->SetRulerStart(GetScrollPos(SB_VERT));

	return bRes;

}

BOOL CGridView::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CGridView::DrawGrid(CDC* pDC)
{
	// Длина и количество линий сетки
	int m_LineSize, m_NumVLines, m_NumHLines;
	// Прлучаем ссылку на документ
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	if (m_Doc)
	{
		if (m_Doc->IsGridPresent())
		{
			float m_GSize = m_Doc->GetGridSize() * m_Scale;
			// Определяем количество линий сетки по вертикали и горизонтали
			CRect m_Client; GetClientRect(&m_Client);
			// Количество линий вычисляется или по ширине клиентской части или по ширине образа
			// Это для того,чтобы при масштабировании шаг сетки правильно отображался и количество линий было правильное
			m_NumVLines = m_PageSize->cx / (m_GSize * pDC->GetDeviceCaps(LOGPIXELSX));
			m_NumHLines = m_PageSize->cy / (m_GSize * pDC->GetDeviceCaps(LOGPIXELSY));
			// Готовим инструментарий
			CPen m_GridPen(PS_SOLID, 1, m_Doc->GetGridColor());
			CPen* m_OldPen = pDC->SelectObject(&m_GridPen);
			// Рисуем сетку
			if (m_Doc->GetGridType() == GRID_LINED)
			{
				// Рисуем горизонтальные линии сетки
				for (int i = 1; i <= m_NumHLines; i++)
				{
					pDC->MoveTo(delta_X, i * m_GSize * pDC->GetDeviceCaps(LOGPIXELSY) + delta_Y);
					// Длину линий устанавливаем равной ширине образа
					m_LineSize = m_PageSize->cx;
					pDC->LineTo(m_LineSize + delta_X, i * m_GSize * pDC->GetDeviceCaps(LOGPIXELSY) + delta_Y);
				}
				// Рисуем вертикальные линии сетки
				for (int i = 1; i <= m_NumVLines; i++)
				{
					pDC->MoveTo(i * m_GSize * pDC->GetDeviceCaps(LOGPIXELSX) + delta_X, delta_Y);
					// Длина линии аналогично вычисляется
					m_LineSize = m_PageSize->cy;
					pDC->LineTo(i * m_GSize * pDC->GetDeviceCaps(LOGPIXELSX) + delta_X, m_LineSize + delta_Y);
				}
			}
			else
			{
				// Рисуем точечную сетку
				for (int i = 1; i <= m_NumHLines; i++)
				{
					for (int j = 1; j <= m_NumVLines; j++)
					{
						pDC->FillSolidRect(j * m_GSize * pDC->GetDeviceCaps(LOGPIXELSX) + delta_X, i * m_GSize * pDC->GetDeviceCaps(LOGPIXELSY) + delta_Y, 1, 1, m_Doc->GetGridColor());
					}
				}
			}
			// Восстанавливаем состояние контекста устройства
			pDC->SelectObject(&m_OldPen);
		}
	}
}


BOOL CGridView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR m_NEW_Cursor;
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	PREVIEW_TOOL_TYPE m_Type;
	if(m_CursorOnGide)
	{
		m_Type = TOOL_GIDE_DRAG;
	}
	else
	{
		m_Type = m_Doc->GetToolType();
	}

	switch(m_Type)
	{
	case TOOL_PAN:
		m_NEW_Cursor = ::LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_PAN)); break;
	case TOOL_NONE:
		m_NEW_Cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)); break;
	case TOOL_ZOOM_IN:
		m_NEW_Cursor = ::LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_ZOOM_IN)); break;
	case TOOL_ZOOM_OUT:
		m_NEW_Cursor = ::LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_ZOOM_OUT)); break;
	case TOOL_GIDE_DRAG:
		{
			GIDELINE_ARRAY* A = m_Doc->GetGidelines();
			if ((*A)[m_SelGideIDX].m_Orientation == VERTICAL)
			{
				m_NEW_Cursor = ::LoadCursor(NULL, IDC_SIZEWE);
			}
			else
			{
				m_NEW_Cursor = ::LoadCursor(NULL, IDC_SIZENS);
			}
		}
		break;
	}
	::SetClassLong(this->m_hWnd, GCL_HCURSOR, (LONG)m_NEW_Cursor);

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CGridView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Определяем каким инструментом клик
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	PREVIEW_TOOL_TYPE m_Type = m_Doc->GetToolType();
	// Производим действие в соответствии с выбранным инструментом
	SCALE_FACTOR f = m_Doc->GetScaleFactor();
	switch(m_Type)
	{
	case TOOL_ZOOM_IN:
		{
			// Устанавливаем новый масштаб
			if (f < SCALE_100) f = SCALE_100;
			else 
			{
				f = (SCALE_FACTOR)((int)f + 1); if (f > SCALE_400) f = SCALE_400;
			}
			m_Doc->SetScaleFactor(f);
		}
		break;
	case TOOL_ZOOM_OUT:
		{
			if (f < SCALE_50) f = SCALE_50;
			else
			{
				f = (SCALE_FACTOR)((int)f - 1); if (f < SCALE_50) f = SCALE_50;
			}
			m_Doc->SetScaleFactor(f);
		}
		break;
	}
	// Запоминаем начало перетаскивания страницы. Так, на всякий случай
	m_StartPANPoint		= point;
	m_OldGideLinePos	= point;
	if (m_CursorOnGide) m_GideMoving = TRUE;

	CHRuler* m_HR = (CHRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(0, 1);
	CVRuler* m_VR = (CVRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(1, 0);
	m_VR->SetRulerStart(GetScrollPos(SB_VERT));
	m_HR->SetRulerStart(GetScrollPos(SB_HORZ));

	CScrollView::OnLButtonDown(nFlags, point);
}

void CGridView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Регистрируем только первое срабатывание
	if (!(nFlags & 0x00004000))
	{
		CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
		// Запоминаем текущий инструмент
		m_OldToolType = m_Doc->GetToolType();
		switch(nChar)
		{
		case VK_SHIFT:		// Включаем инструмент ZOOM IN
			m_Doc->SetToolType(TOOL_ZOOM_IN); break;
		case VK_CONTROL:	// Включаем инструмент ZOOM OUT
			m_Doc->SetToolType(TOOL_ZOOM_OUT); break;
		}
		::SendMessage(this->GetSafeHwnd(), WM_SETCURSOR, (WPARAM)this->GetSafeHwnd(), MAKELPARAM(HTCLIENT, WM_SETCURSOR));
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGridView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT || nChar == VK_CONTROL)
	{
		// Восстанавливаем инструмент
		CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
		m_Doc->SetToolType(m_OldToolType);
		::SendMessage(this->GetSafeHwnd(), WM_SETCURSOR, (WPARAM)this->GetSafeHwnd(), MAKELPARAM(HTCLIENT, WM_SETCURSOR));
	}

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGridView::OnMouseMove(UINT nFlags, CPoint point)
{
	GIDELINE_ARRAY* A;
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	CHRuler* m_HR = (CHRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(0, 1);
	CVRuler* m_VR = (CVRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(1, 0);
	::PostMessage(m_HR->GetSafeHwnd(), WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
	::PostMessage(m_VR->GetSafeHwnd(), WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
	// Проверяем не находится ли курсор над гайдом
	if (m_Doc->IsPageReady() && m_GidesVisible && !m_GideMoving)
	{
		A = m_Doc->GetGidelines();

		m_CursorOnGide = FALSE;
		m_SelGideIDX = -1;

		if (!(*A).empty())
		{
			for (int i = 0; i < (*A).size(); i++)
			{
				switch((*A)[i].m_Orientation)
				{
				case HORIZONTAL:
					{
						int pos = (*A)[i].m_Position;
						if (FromClientToPage(point).y > (pos - (*A)[i].m_SencitivityAreaWidth) &&
							FromClientToPage(point).y < (pos + (*A)[i].m_SencitivityAreaWidth))
						{
							m_CursorOnGide = TRUE;
							m_SelGideIDX = i;
						}
					}
					break;
				case VERTICAL:
					{
						int pos = (*A)[i].m_Position;	
						if (FromClientToPage(point).x > (pos - (*A)[i].m_SencitivityAreaWidth) &&
							FromClientToPage(point).x < (pos + (*A)[i].m_SencitivityAreaWidth))
						{
							m_CursorOnGide = TRUE;
							m_SelGideIDX = i;
						}
					}
					break;
				}
			}
		}
	}

	if (nFlags == MK_LBUTTON)
	{
		int m_X_ScrollPos = GetScrollPos(SB_HORZ);
		int m_X_ScrollLim = GetScrollLimit(SB_HORZ);
		int m_Y_ScrollPos = GetScrollPos(SB_VERT);
		int m_Y_ScrollLim = GetScrollLimit(SB_VERT);
		PREVIEW_TOOL_TYPE m_Type = m_Doc->GetToolType();
		if (m_CursorOnGide || m_GideMoving) m_Type = TOOL_GIDE_DRAG;
		// PAN-операция
		if (m_Type == TOOL_PAN)
		{
			// Определяем смещение и условия скроллинга по оси Х
			int d_X = m_StartPANPoint.x - point.x;
			int new_X_Pos = m_X_ScrollPos + d_X;
			if (new_X_Pos >= m_X_ScrollLim) new_X_Pos = m_X_ScrollLim;
			if (new_X_Pos <= 0) new_X_Pos = 0;
			// Определяем смещение и условия скроллинга по оси Y
			int d_Y = m_StartPANPoint.y - point.y;
			int new_Y_Pos = m_Y_ScrollPos + d_Y;
			if (new_Y_Pos >= m_Y_ScrollLim) new_Y_Pos = m_Y_ScrollLim;
			if (new_Y_Pos <= 0) new_Y_Pos = 0;
			// Осуществляем скроллинг, если необходимо
			if (delta_X == 0)
			{
				::SendMessage(this->GetSafeHwnd(), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, new_X_Pos), NULL);
			}
			if (delta_Y == 0)
			{
				::SendMessage(this->GetSafeHwnd(), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, new_Y_Pos), NULL);
			}
			m_StartPANPoint = point;
		}
		// Перетаскивание гайд-лайн
		else if (m_Type == TOOL_GIDE_DRAG)
		{
			CRect m_Client; GetClientRect(&m_Client);
			// Длина активного гайд-лайна
			int m_GL;
			// Перетаскиваем выбранный гайд-лайн
			A = m_Doc->GetGidelines();
			switch((*A)[m_SelGideIDX].m_Orientation)
			{
			case VERTICAL:
				{
					(*A)[m_SelGideIDX].m_Position = FromClientToPage(point).x;
					::PostMessage(((CPreviewFrame*)GetParentFrame())->m_MainDLG->GetSafeHwnd(), WM_USER + 5, m_SelGideIDX, 0);
					// Определяем регион перерисовки
					m_Client.Height() > m_PageSize->cy ? m_GL = m_Client.Height() : m_GL = m_PageSize->cy;
					CPoint m_R[6];
					m_R[0].x	= m_OldGideLinePos.x - 10;
					m_R[0].y	= 0;
					m_R[1].x	= m_OldGideLinePos.x + 40;
					m_R[1].y	= 0;
					m_R[2].x	= m_R[1].x;
					m_R[2].y	= 19;
					m_R[3].x	= m_OldGideLinePos.x + 10;
					m_R[3].y	= m_R[2].y;
					m_R[4].x	= m_R[3].x;
					m_R[4].y	= m_GL;
					m_R[5].x	= m_R[0].x;
					m_R[5].y	= m_R[4].y;
					CRgn m_UR_A, m_UR_B;
					m_UR_A.CreatePolygonRgn(m_R, 6, ALTERNATE);
					m_GidesDRAW = FALSE;
					InvalidateRgn(&m_UR_A);
					m_R[0].x	= point.x - 10;
					m_R[0].y	= 0;
					m_R[1].x	= point.x + 40;
					m_R[1].y	= 0;
					m_R[2].x	= m_R[1].x;
					m_R[2].y	= 19;
					m_R[3].x	= point.x + 10;
					m_R[3].y	= m_R[2].y;
					m_R[4].x	= m_R[3].x;
					m_R[4].y	= m_GL;
					m_R[5].x	= m_R[0].x;
					m_R[5].y	= m_R[4].y;
					m_UR_B.CreatePolygonRgn(m_R, 6, ALTERNATE);
					m_GidesDRAW = TRUE;
					InvalidateRgn(&m_UR_B);
				}
				break;
			case HORIZONTAL:
				{
					(*A)[m_SelGideIDX].m_Position = FromClientToPage(point).y;
					::PostMessage(((CPreviewFrame*)GetParentFrame())->m_MainDLG->GetSafeHwnd(), WM_USER + 5, m_SelGideIDX, 0);
					// Определяем регион перерисовки
					m_Client.Width() > m_PageSize->cx ? m_GL = m_Client.Width() : m_GL = m_PageSize->cx;
					CPoint m_R[6];
					m_R[0].x	= 0;
					m_R[0].y	= m_OldGideLinePos.y + 10;
					m_R[1].x	= 0;
					m_R[1].y	= m_OldGideLinePos.y - 25;
					m_R[2].x	= 34;
					m_R[2].y	= m_R[1].y;
					m_R[3].x	= m_R[2].x;
					m_R[3].y	= m_OldGideLinePos.y - 10;
					m_R[4].x	= m_GL;
					m_R[4].y	= m_R[3].y;
					m_R[5].x	= m_R[4].x;
					m_R[5].y	= m_R[0].y;
					CRgn m_UR_A, m_UR_B;
					m_UR_A.CreatePolygonRgn(m_R, 6, ALTERNATE);
					m_GidesDRAW = FALSE;
					InvalidateRgn(&m_UR_A, FALSE);
					m_R[0].x	= 0;
					m_R[0].y	= point.y + 10;
					m_R[1].x	= 0;
					m_R[1].y	= point.y - 25;
					m_R[2].x	= 34;
					m_R[2].y	= m_R[1].y;
					m_R[3].x	= m_R[2].x;
					m_R[3].y	= point.y - 10;
					m_R[4].x	= m_GL;
					m_R[4].y	= m_R[3].y;
					m_R[5].x	= m_R[4].x;
					m_R[5].y	= m_R[0].y;
					m_UR_B.CreatePolygonRgn(m_R, 6, ALTERNATE);
					m_GidesDRAW = TRUE;
					InvalidateRgn(&m_UR_B, FALSE);
				}
				break;
			}
			m_OldGideLinePos = point;
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CGridView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (delta_X == 0)
	{
		CHRuler* m_HR = (CHRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(0, 1);
		m_HR->SetRulerStart(GetScrollPos(SB_HORZ));
	}

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (delta_Y == 0)
	{
		CVRuler* m_VR = (CVRuler*)((CPreviewFrame*)GetParentFrame())->m_Splitter.GetPane(1, 0);
		m_VR->SetRulerStart(GetScrollPos(SB_VERT));
	}
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CGridView::DrawGideLines(CDC* pDC, GIDELINE_ARRAY* A)
{
	CRect m_ClientRect; GetClientRect(&m_ClientRect);
	CFont m_gideFont;
	CFont* m_OldFont;
	CPen* m_OldPen;
	int m_GideLength;
	// Создаем шрифт для отрисовки метки направляющей
	m_gideFont.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		CLEARTYPE_QUALITY, 
		DEFAULT_PITCH | FF_MODERN, _T("Arial"));
	for (int i = 0; i < (*A).size(); i++)
	{
		// Выбираем перо
		CPen m_GidePen(PS_SOLID, 1, (*A)[i].m_Color);
		// Рисуем линию
		switch((*A)[i].m_Orientation)
		{
		case HORIZONTAL:
			{
				int pos = FromPageToClient(CPoint(0, (*A)[i].m_Position)).y;
				m_ClientRect.Width() > m_PageSize->cx ? m_GideLength = m_ClientRect.Width() : m_GideLength = m_PageSize->cx;
				pDC->MoveTo(0, pos);
				m_OldPen = pDC->SelectObject(&m_GidePen);
				pDC->LineTo(m_GideLength, pos);
				pDC->SelectObject(m_OldPen);
				// Отрисовываем текстовую метку
				if ((*A)[i].m_Label != "")
				{
					m_OldFont = pDC->SelectObject(&m_gideFont);
					pDC->SetTextColor(RGB(255, 255, 255));
					CRect TR;
					TR.left		= 2;
					TR.right	= TR.left + 30;
					TR.top		= pos - 15;
					TR.bottom	= pos;
					pDC->FillSolidRect(&TR, (*A)[i].m_Color);
					pDC->DrawText((*A)[i].m_Label, &TR, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					pDC->SelectObject(m_OldFont);
				}
			}
			break;
		case VERTICAL:
			{
				int pos = FromPageToClient(CPoint((*A)[i].m_Position, 0)).x;
				m_ClientRect.Height() > m_PageSize->cy ? m_GideLength = m_ClientRect.Height() : m_GideLength = m_PageSize->cy;
				pDC->MoveTo(pos, 0);
				m_OldPen = pDC->SelectObject(&m_GidePen);
				pDC->LineTo(pos, m_GideLength);
				pDC->SelectObject(m_OldPen);
				// Отрисовываем текстовую метку
				if ((*A)[i].m_Label != "")
				{
					m_OldFont = pDC->SelectObject(&m_gideFont);
					pDC->SetTextColor(RGB(255, 255, 255));
					CRect TR;
					TR.left		= pos;
					TR.right	= pos + 30;
					TR.top		= 2;
					TR.bottom	= TR.top + 15;
					pDC->FillSolidRect(&TR, (*A)[i].m_Color);
					pDC->DrawText((*A)[i].m_Label, &TR, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					pDC->SelectObject(m_OldFont);
				}
			}
			break;
		}
	}
}

CPoint CGridView::FromPageToClient(CPoint pt)
{
	CPoint ret;
	// Производим преобразование координат
	ret.x = (pt.x * m_UnitSize / 72.0) * m_VirtScreenDC.GetDeviceCaps(LOGPIXELSX) * m_Scale + delta_X;
	ret.y = (pt.y * m_UnitSize / 72.0) * m_VirtScreenDC.GetDeviceCaps(LOGPIXELSY) * m_Scale + delta_Y;
	// Возвращаем точку в системе координат клиентской области
	return ret;
}

CPoint CGridView::FromClientToPage(CPoint pt)
{
	CPoint ret;
	int DX, DY;
	CRect client; GetClientRect(&client);
	DY = (m_PageSize->cy - client.Height()) * GetScrollPos(SB_VERT) / GetScrollLimit(SB_VERT);
	DX = (m_PageSize->cx - client.Width()) * GetScrollPos(SB_HORZ) / GetScrollLimit(SB_HORZ);

	// Производим преобразование координат
	ret.x = ((pt.x + DX - delta_X) * 72.0) / (m_UnitSize * m_Scale * m_VirtScreenDC.GetDeviceCaps(LOGPIXELSX));
	ret.y = ((pt.y + DY - delta_Y) * 72.0) / (m_UnitSize * m_Scale * m_VirtScreenDC.GetDeviceCaps(LOGPIXELSY));
	// Возвращаем точку в системе координат клиентской области
	return ret;
}


void CGridView::SetGidesVisible(BOOL visible)
{
	m_GidesVisible = visible;
	Invalidate();
}
void CGridView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_GideMoving = FALSE;
	CScrollView::OnLButtonUp(nFlags, point);
}


void CGridView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	CPreviewDoc* m_Doc = (CPreviewDoc*)GetDocument();
	if (m_Doc)
	{
		if (m_Doc->IsPageReady() && m_Doc->m_MainViewReady)
		{
			m_Doc->UpdateAllViews(NULL);
		}
	}
}
