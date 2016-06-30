#include "../StdAfx.h"
#include "Preview.h"

CPreview::CPreview(CDialog* owner)
{
	creator = owner;
	// «агружаем ресурсы предварительного просмотра
	m_PFrame = new CPreviewFrame(owner);
	m_PFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, (CWnd*)owner);
	
	m_CurrentDoc = m_PFrame->GetActiveDocument();
}

CPreview::~CPreview(void)
{
	((CPreviewFrame*)m_PFrame)->SetClose(TRUE);
}

void CPreview::ShowPreview(int nCmdShow)
{
	if (nCmdShow == SW_SHOW)
	{
		// ќтображаем окно предварительного просмотра
		if (m_CurrentDoc->GetDockingView())
		{
			RECT c_rect, w_rect;
			creator->GetWindowRect(&c_rect);
			m_PFrame->GetWindowRect(&w_rect);
			w_rect.top		= c_rect.top;
			w_rect.left		= c_rect.right;
			m_PFrame->MoveWindow(&w_rect);
		}
		m_PFrame->ShowWindow(SW_SHOW);
		m_PFrame->UpdateWindow();
	}
	else
	{
		// —крываем окно предварительного просмотра
		m_PFrame->ShowWindow(SW_HIDE);
	}
}

void CPreview::OnOwnerMove()
{
	// ѕередвигаем окно предварительного просмотра
	if (m_CurrentDoc->GetDockingView())
	{
		RECT c_rect, w_rect;
		creator->GetWindowRect(&c_rect);
		m_PFrame->GetWindowRect(&w_rect);
		int dx = w_rect.right - w_rect.left;
		int dy = w_rect.bottom - w_rect.top;
		w_rect.top		= c_rect.top;
		w_rect.left		= c_rect.right;
		w_rect.bottom	= w_rect.top + dy;
		w_rect.right	= w_rect.left + dx;
		m_PFrame->MoveWindow(&w_rect);
	}
}

void CPreview::SetPreviewPDPage(PDPage page)
{
	m_CurrentDoc->SetPDPage(page);
}

void CPreview::AddGideline(GIDELINE line)
{
	m_CurrentDoc->AddGideLine(line);
}

void CPreview::RemoveGideline(CString name)
{
	m_CurrentDoc->RemoveGideLine(name);
}

GIDELINE CPreview::GetGidelineInfo(CString name)
{
	return m_CurrentDoc->GetGidelineInfo(name);
}

GIDELINE CPreview::GetGidelineInfo(int idx)
{
	return m_CurrentDoc->GetGidelineInfo(idx);
}

void CPreview::SetGidesVisible(BOOL visible)
{
	((CGridView*)m_PFrame->m_Splitter.GetPane(1, 1))->SetGidesVisible(visible);
}