// PreviewDoc.cpp : implementation file
//

#include "../stdafx.h"
#include "PreviewDoc.h"


// CPreviewDoc

IMPLEMENT_DYNCREATE(CPreviewDoc, CDocument)

CPreviewDoc::CPreviewDoc()
{
	m_IMapMode      = MM_TEXT;
	m_IsPreviewPage = FALSE;
	m_ScaleFactor   = SCALE_100;
	m_DrawGrid      = FALSE;
	m_GridSize		= 0.125;
	m_GridType		= GRID_DOTTED;
	m_ToolType      = TOOL_PAN;
	m_DockingView	= TRUE;
	m_MUnit			= MU_INCH;
	m_UnitSize		= 1.0;
	m_MainViewReady = FALSE;
	m_GridColor		= RGB(0, 0, 255);
}
	

BOOL CPreviewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPreviewDoc::~CPreviewDoc()
{
}


BEGIN_MESSAGE_MAP(CPreviewDoc, CDocument)
END_MESSAGE_MAP()


// CPreviewDoc diagnostics

#ifdef _DEBUG
void CPreviewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPreviewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPreviewDoc serialization

void CPreviewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CPreviewDoc commands
void CPreviewDoc::SetPDPage(PDPage page)
{
	
	m_PreviewPage = page;
	m_IsPreviewPage = TRUE;
	// Обновляем представления документа
	UpdateAllViews(NULL);	
}

void CPreviewDoc::SetScaleFactor(SCALE_FACTOR f)
{
	m_ScaleFactor = f;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetDrawGrid(BOOL draw)
{
	m_DrawGrid = draw;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetGridSize(float gsize)
{
	m_GridSize = gsize;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetGridType(PREVIEW_GRID_TYPE type)
{
	m_GridType = type;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetGridColor(COLORREF color)
{
	m_GridColor = color;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetToolType(PREVIEW_TOOL_TYPE type)
{
	m_ToolType = type;
}

void CPreviewDoc::SetDockingView(BOOL view)
{
	m_DockingView = view;
}

void CPreviewDoc::SetMeasuringUnit(MEASURING_UNIT unit)
{
	m_MUnit = unit;
	UpdateAllViews(NULL);
}

void CPreviewDoc::SetDX(int dx)
{
	m_DX = dx;
}

void CPreviewDoc::AddGideLine(GIDELINE line)
{
	// Проверяем уникальность имени гайда
	if (!m_Gides.empty())
	{
		for (int i = 0; i < m_Gides.size(); i++)
		{
			if (m_Gides[i].m_Label == line.m_Label) return;
		}
	}
	m_Gides.push_back(line);
	UpdateAllViews(NULL);
}

void CPreviewDoc::RemoveGideLine(int idx)
{
	if (idx < 0 && idx > m_Gides.size() - 1) return;
	if (!m_Gides.empty()) m_Gides.erase(m_Gides.begin() + idx);
	UpdateAllViews(NULL);
}

void CPreviewDoc::RemoveGideLine(CString name)
{
	if (!m_Gides.empty())
	{
		for (int i = 0; i < m_Gides.size(); i++)
		{
			if (m_Gides[i].m_Label == name)
			{
				m_Gides.erase(m_Gides.begin() + i);
				break;
			}
		}
	}
	UpdateAllViews(NULL);
}

int CPreviewDoc::GetGidelineID(CString name)
{
	if (!m_Gides.empty())
	{
		for (int i = 0; i < m_Gides.size(); i++)
		{
			if (m_Gides[i].m_Label == name)
			return i;
		}
	}
	return -1;
}

GIDELINE CPreviewDoc::GetGidelineInfo(int idx)
{
	if (idx > -1 && idx < m_Gides.size() && !m_Gides.empty()) return m_Gides[idx];
	else 
	{
		GIDELINE NULL_GIDE;
		NULL_GIDE.m_Color = RGB(0, 0, 0);
		NULL_GIDE.m_Label = "NULL_GIDE";
		NULL_GIDE.m_Orientation = HORIZONTAL;
		NULL_GIDE.m_Position = 0;
		NULL_GIDE.m_SencitivityAreaWidth = 0;
		return NULL_GIDE;
	}
}

GIDELINE CPreviewDoc::GetGidelineInfo(CString name)
{
	if (!m_Gides.empty())
	{
		for (int i = 0; i < m_Gides.size(); i++)
		{
			if (m_Gides[i].m_Label == name) return m_Gides[i];
		}
	}
	GIDELINE NULL_GIDE;
	NULL_GIDE.m_Color = RGB(0, 0, 0);
	NULL_GIDE.m_Label = "NULL_GIDE";
	NULL_GIDE.m_Orientation = HORIZONTAL;
	NULL_GIDE.m_Position = 0;
	NULL_GIDE.m_SencitivityAreaWidth = 0;
	return NULL_GIDE;
}