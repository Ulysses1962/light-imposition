#pragma once
#include "HRuler.h"
#include "VRuler.h"
#include <math.h>
#include <vector>
#include "acrocalls.h"

using namespace std;


// CPreviewDoc document
typedef enum {SCALE_FIT_WINDOW, SCALE_FIT_W, SCALE_FIT_H, SCALE_50, SCALE_100, SCALE_200, SCALE_300, SCALE_400, SCALE_VAR} SCALE_FACTOR;
typedef enum {TOOL_ZOOM_IN, TOOL_ZOOM_OUT, TOOL_PAN, TOOL_GIDE_DRAG, TOOL_NONE} PREVIEW_TOOL_TYPE;
typedef enum {GRID_LINED, GRID_DOTTED} PREVIEW_GRID_TYPE;
typedef enum {MU_INCH, MU_MM} MEASURING_UNIT;
// ��� ��� �������� ������������ �����
typedef enum {VERTICAL, HORIZONTAL} GIDELINE_ORIENTATION;
typedef struct  
{
	GIDELINE_ORIENTATION m_Orientation;
	COLORREF m_Color;
	long m_Position;
	int m_SencitivityAreaWidth;
	CString m_Label;
}
GIDELINE;
typedef vector<GIDELINE> GIDELINE_ARRAY;

class CPreviewDoc : public CDocument
{
	DECLARE_DYNCREATE(CPreviewDoc)
	// �������� ��� �����������
	PDPage m_PreviewPage;
	// ����� �����������
	int m_IMapMode;
	// ������ ���������������
	SCALE_FACTOR m_ScaleFactor;
	// ������� ������������� ��������
	BOOL m_IsPreviewPage;
	// ������� ��������� �����
	BOOL m_DrawGrid;
	// ��� �����
	float m_GridSize;
	// ��� �����
	PREVIEW_GRID_TYPE m_GridType;
	// ���� �����
	COLORREF m_GridColor;
	// ������� ������ ��������� �� ���� ����
	int m_DX, m_DY;
	// �-� ��������������� ��������� �������������
	float m_MainViewScale;
	// ������� �������� � ���������� �������� (�������)
	CSize m_PageSize;
	// ������� �������� � ���������� �������� (��������)
	CSize m_PageRealSize;
	// ������ ����������������� �����
	float m_UnitSize;

	// ������� �������������� ���� ���������������� ��������� 
	bool m_DockingView;

	// ����������� ������
	MEASURING_UNIT m_MUnit;


	//---------------------------------------------------
	// �����������
	//---------------------------------------------------
	PREVIEW_TOOL_TYPE m_ToolType;

	//---------------------------------------------------
	// ������������
	//---------------------------------------------------
	GIDELINE_ARRAY m_Gides; 
public:
	CPreviewDoc();
	virtual ~CPreviewDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	// ��������� �������� ���������������� ���������
	void SetPDPage(PDPage page);
	// ��������� ��������
	PDPage GetPreviewPDPage() { return m_PreviewPage; };
	// ��������� ������ �����������
	int GetMapMode() { return m_IMapMode; };
	BOOL GetDockingView() { return m_DockingView; };
	void SetDockingView(BOOL view);
	// ���������� ��� ������� ���������������
	SCALE_FACTOR GetScaleFactor() { return m_ScaleFactor; };
	void SetScaleFactor(SCALE_FACTOR f);
	// ��������� ���������� ��������
	BOOL IsPageReady() { return m_IsPreviewPage; };
	// ��������� �����
	void SetDrawGrid(BOOL draw);
	PREVIEW_GRID_TYPE GetGridType() { return m_GridType; };
	void SetGridType(PREVIEW_GRID_TYPE type);
	BOOL IsGridPresent() { return m_DrawGrid; };
	float GetGridSize() { return m_GridSize; };
	void SetGridSize(float gsize);
	COLORREF GetGridColor() { return m_GridColor; };
	void SetGridColor(COLORREF color);
	// ���������� �������������
	void SetToolType(PREVIEW_TOOL_TYPE type);
	PREVIEW_TOOL_TYPE GetToolType() { return m_ToolType; };
	// ���������� ��������� ���������
	MEASURING_UNIT GetMeasuringUnit() { return m_MUnit; };
	void SetMeasuringUnit(MEASURING_UNIT unit);
	// ���������-������ ���������� ����������� ��������
	void SetDX(int dx);
	void SetDY(int dy) { m_DY = dy; };
	int GetDX() { return m_DX; };
	int GetDY() { return m_DY; };
	void SetMainViewScale(float scale) { m_MainViewScale = scale; };
	float GetMainViewScale() { return m_MainViewScale; };
	CSize GetPageSize() { return m_PageSize; };
	void SetPageSize(CSize size) { m_PageSize = size; };
	CSize GetPageRealSize() { return m_PageRealSize; };
	void SetPageRealSize(CSize size) { m_PageRealSize = size; };

	void SetUnitSize(float A) { m_UnitSize = A; }
	float GetUnitSize() { return m_UnitSize; };

	// ������� ���������� ����������� ��������� ����
	BOOL m_MainViewReady;

	// ���������� �������������
	void AddGideLine(GIDELINE line);
	GIDELINE_ARRAY* GetGidelines() { return &m_Gides; };
	void RemoveGideLine(int idx);
	void RemoveGideLine(CString name);
	int GetGidelineID(CString name);
	void ClearGidelines() { m_Gides.clear(); };
	GIDELINE GetGidelineInfo(int idx);
	GIDELINE GetGidelineInfo(CString name);
};

