// PreviewFrame.cpp : implementation file
//

#include "../stdafx.h"
#include "PreviewFrame.h"


// CPreviewFrame

IMPLEMENT_DYNCREATE(CPreviewFrame, CFrameWnd)

CPreviewFrame::CPreviewFrame()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_Close		= FALSE;
	m_Doc		= new CPreviewDoc();
	m_bAutoMenuEnable = FALSE;
	m_ClientCreated = FALSE;
	m_DialogIsMain = TRUE;
	m_SetDLG = new CPreviewSettings(this);
}

CPreviewFrame::CPreviewFrame(CDialog* dlg)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_Close		= FALSE;
	m_Doc		= new CPreviewDoc();
	m_bAutoMenuEnable = FALSE;
	m_MainDLG	= dlg;
	m_ClientCreated = FALSE;
	m_DialogIsMain = TRUE;
	m_SetDLG = new CPreviewSettings(this);
}

CPreviewFrame::~CPreviewFrame()
{
	delete m_SetDLG;
}


BEGIN_MESSAGE_MAP(CPreviewFrame, CFrameWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOL_ZOOM_IN, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_TOOL_ZOOM_OUT, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_TOOL_PAN, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_TOOL_NONE, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_FIT_HEIGHT, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_SHOW_GRID, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_STICK_TO_MAIN, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_FIRST_PAGE, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_PREV_PAGE, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_NEXT_PAGE, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_LAST_PAGE, (AFX_PMSG)OnToolbarCmd)
	ON_COMMAND(ID_SHOW_GIDELINES, (AFX_PMSG)OnToolbarCmd)
	ON_WM_MOVE()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// CPreviewFrame message handlers
BOOL CPreviewFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//-------------------------------------------------------------
	// Создаем четыре панели - 2 колонки и 2 строчки. 
	// Это две линейки, заглушка и основное окно просмотра
	//-------------------------------------------------------------
	BOOL res = m_Splitter.CreateStatic(this, 2, 2, WS_CHILD | WS_VISIBLE);
	// Устанавливаем размеры панелей
	if (!res)
	{
		return res;
	}
	RECT c_rect;
	GetClientRect(&c_rect);
	m_Splitter.SetRowInfo(0, 20, 20);
	m_Splitter.SetColumnInfo(0, 20, 20);
	// Создаем панели
	m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CStub), CSize(20, 20), pContext);
	m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CHRuler), CSize(0, 20), pContext);
	m_Splitter.CreateView(1, 0, RUNTIME_CLASS(CVRuler), CSize(20, 0), pContext);
	m_Splitter.CreateView(1, 1, RUNTIME_CLASS(CGridView), CSize(0, 0), pContext);
	// Устанавливаем активную панель
	m_Splitter.SetActivePane(1, 1);

	m_Doc->AddView((CGridView*)m_Splitter.GetPane(1, 1));
	m_Doc->AddView((CHRuler*)m_Splitter.GetPane(0, 1));
	m_Doc->AddView((CVRuler*)m_Splitter.GetPane(1, 0));
	m_Doc->AddView((CStub*)m_Splitter.GetPane(0, 0));

	CMenu* m_Menu = GetMenu();
	m_Menu->CheckMenuItem(ID_STICK_TO_MAIN, MF_CHECKED | MF_BYCOMMAND);

	m_ClientCreated = TRUE;

	return res;
}

void CPreviewFrame::OnClose()
{
	if (!m_Close)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CFrameWnd::OnClose();
	}
}


BOOL CPreviewFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case ID_FIT_WINDOW : 
		m_Doc->SetScaleFactor(SCALE_FIT_WINDOW); break;
	case ID_FIT_WIDTH :
		m_Doc->SetScaleFactor(SCALE_FIT_W); break;
	case ID_FIT_HEIGHT :
		m_Doc->SetScaleFactor(SCALE_FIT_H); break;
	case ID_ZOOM_50 : 
		m_Doc->SetScaleFactor(SCALE_50); break;
	case ID_ZOOM_100 : 
		m_Doc->SetScaleFactor(SCALE_100); break;
	case ID_ZOOM_200 : 
		m_Doc->SetScaleFactor(SCALE_200); break;
	case ID_ZOOM_300 : 
		m_Doc->SetScaleFactor(SCALE_300); break;
	case ID_ZOOM_400 :
		m_Doc->SetScaleFactor(SCALE_400); break;
	case ID_SHOW_GRID :
		{
			CMenu* m_Menu = GetMenu();
			UINT m_IState = m_Menu->GetMenuState(wParam, MF_BYCOMMAND);
			if (m_IState == MF_UNCHECKED)
			{
				m_Menu->CheckMenuItem(wParam, MF_CHECKED | MF_BYCOMMAND);
				m_Doc->SetDrawGrid(TRUE);
			}
			else
			{
				m_Menu->CheckMenuItem(wParam, MF_UNCHECKED | MF_BYCOMMAND);
				m_Doc->SetDrawGrid(FALSE);
			}
		}
		break;
	case ID_TOOL_ZOOM_IN :
		m_Doc->SetToolType(TOOL_ZOOM_IN); break;
	case ID_TOOL_ZOOM_OUT :
		m_Doc->SetToolType(TOOL_ZOOM_OUT); break;
	case ID_TOOL_NONE :
		m_Doc->SetToolType(TOOL_NONE); break;
	case ID_TOOL_PAN :
		m_Doc->SetToolType(TOOL_PAN); break;
	case ID_STICK_TO_MAIN:
		{
			CMenu* m_Menu = GetMenu();
			UINT m_IState = m_Menu->GetMenuState(wParam, MF_BYCOMMAND);
			if (m_IState == MF_UNCHECKED)
			{
				m_Menu->CheckMenuItem(wParam, MF_CHECKED | MF_BYCOMMAND);
				m_Doc->SetDockingView(TRUE);
				CRect c_rect, w_rect;
				m_MainDLG->GetWindowRect(&c_rect);
				this->GetWindowRect(&w_rect);
				w_rect.MoveToXY(c_rect.right, c_rect.top);
				this->MoveWindow(&w_rect);

			}
			else
			{
				m_Menu->CheckMenuItem(wParam, MF_UNCHECKED | MF_BYCOMMAND);
				m_Doc->SetDockingView(FALSE);
			}
			m_Splitter.SetActivePane(1, 1);
		}
		break;
	case ID_FIRST_PAGE:
		::SendMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 1, 0, 0);
		break;
	case ID_PREV_PAGE:
		::SendMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 2, 0, 0);
		break;
	case ID_NEXT_PAGE:
		::SendMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 3, 0, 0);
		break;
	case ID_LAST_PAGE:
		::SendMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 4, 0, 0);
		break;
	case ID_SHOW_GIDELINES:
		{
			CGridView* pView = (CGridView*)m_Splitter.GetPane(1, 1);
			if (pView->GetGidesVisible())
			{
				pView->SetGidesVisible(FALSE);
				::PostMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 6, 0, 0);
			}
			else
			{
				pView->SetGidesVisible(TRUE);
				::PostMessage(m_MainDLG->GetSafeHwnd(), WM_USER + 6, 1, 0);
			}
		}
		break;
	case ID_PREV_OPTIONS:
		m_SetDLG->DoModal();
		break;
	}
	return TRUE;
}

int CPreviewFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_TBar.Create(this);
	m_TBar.LoadToolBar(IDR_MAINFRAME);
	m_TBar.EnableToolTips(TRUE);
	
	return 0;
}

void CPreviewFrame::OnToolbarCmd(WPARAM wParam, LPARAM lParam)
{
	OnCommand(wParam, lParam);
}


void CPreviewFrame::OnMove(int x, int y)
{
	if (m_ClientCreated) m_Splitter.SetActivePane(1, 1);
	if (!m_DialogIsMain && m_Doc->GetDockingView())
	{
		CRect c_rect, w_rect;
		m_MainDLG->GetWindowRect(&c_rect);
		this->GetWindowRect(&w_rect);
		c_rect.MoveToXY(w_rect.left - c_rect.Width(), w_rect.top);
		m_MainDLG->MoveWindow(&c_rect);
	}
	CFrameWnd::OnMove(x, y);
}

void CPreviewFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (nHitTest == HTCAPTION)
	{
		m_DialogIsMain = FALSE;
	}
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

