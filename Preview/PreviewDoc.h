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
// Все что касается направляющих линий
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
	// Страница для отображения
	PDPage m_PreviewPage;
	// Режим отображения
	int m_IMapMode;
	// Фактор масштабирования
	SCALE_FACTOR m_ScaleFactor;
	// Признак установленной страницы
	BOOL m_IsPreviewPage;
	// Признак рисования сетки
	BOOL m_DrawGrid;
	// Шаг сетки
	float m_GridSize;
	// Тип сетки
	PREVIEW_GRID_TYPE m_GridType;
	// Цвет сетки
	COLORREF m_GridColor;
	// Отступы начала координат от края окна
	int m_DX, m_DY;
	// К-т масштабирования основного представления
	float m_MainViewScale;
	// Размеры страницы в логических единицах (текущий)
	CSize m_PageSize;
	// Размеры страницы в логических единицах (реальный)
	CSize m_PageRealSize;
	// Размер пользовательского юнита
	float m_UnitSize;

	// Признак прикрепленного окна предварительного просмотра 
	bool m_DockingView;

	// Определения линеек
	MEASURING_UNIT m_MUnit;


	//---------------------------------------------------
	// Инструменты
	//---------------------------------------------------
	PREVIEW_TOOL_TYPE m_ToolType;

	//---------------------------------------------------
	// Направляющие
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
	// Установка страницы предварительного просмотра
	void SetPDPage(PDPage page);
	// Получение страницы
	PDPage GetPreviewPDPage() { return m_PreviewPage; };
	// Получение режима отображения
	int GetMapMode() { return m_IMapMode; };
	BOOL GetDockingView() { return m_DockingView; };
	void SetDockingView(BOOL view);
	// Аналогично для фактора масштабирования
	SCALE_FACTOR GetScaleFactor() { return m_ScaleFactor; };
	void SetScaleFactor(SCALE_FACTOR f);
	// Получение готовности страницы
	BOOL IsPageReady() { return m_IsPreviewPage; };
	// Рисование сетки
	void SetDrawGrid(BOOL draw);
	PREVIEW_GRID_TYPE GetGridType() { return m_GridType; };
	void SetGridType(PREVIEW_GRID_TYPE type);
	BOOL IsGridPresent() { return m_DrawGrid; };
	float GetGridSize() { return m_GridSize; };
	void SetGridSize(float gsize);
	COLORREF GetGridColor() { return m_GridColor; };
	void SetGridColor(COLORREF color);
	// Управление инструментами
	void SetToolType(PREVIEW_TOOL_TYPE type);
	PREVIEW_TOOL_TYPE GetToolType() { return m_ToolType; };
	// Управление единицами измерения
	MEASURING_UNIT GetMeasuringUnit() { return m_MUnit; };
	void SetMeasuringUnit(MEASURING_UNIT unit);
	// Установка-чтение параметров отображения страницы
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

	// Признак готовности изображения основного окна
	BOOL m_MainViewReady;

	// Управление направляющими
	void AddGideLine(GIDELINE line);
	GIDELINE_ARRAY* GetGidelines() { return &m_Gides; };
	void RemoveGideLine(int idx);
	void RemoveGideLine(CString name);
	int GetGidelineID(CString name);
	void ClearGidelines() { m_Gides.clear(); };
	GIDELINE GetGidelineInfo(int idx);
	GIDELINE GetGidelineInfo(CString name);
};

