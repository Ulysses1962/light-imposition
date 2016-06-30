// \Preview\PreviewSettings.cpp : implementation file
//

#include "../stdafx.h"
#include "PreviewSettings.h"
#include "PreviewFrame.h"


// CPreviewSettings dialog

IMPLEMENT_DYNAMIC(CPreviewSettings, CDialog)

CPreviewSettings::CPreviewSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewSettings::IDD, pParent)
	, m_GridSizeVAL(0.125)
	, m_GideSVAL(0)
{
}

CPreviewSettings::~CPreviewSettings()
{
}

void CPreviewSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GR_SIZE_SPIN, m_GridSizeSPIN);
	DDX_Control(pDX, IDC_GRID_SIZE_ED, m_GridSizeCTRL);
	DDX_Text(pDX, IDC_GRID_SIZE_ED, m_GridSizeVAL);
	DDX_Control(pDX, IDC_GRID_LINED, m_GridLined);
	DDX_Control(pDX, IDC_GRID_DOTTED, m_GridDotted);
	DDX_Control(pDX, IDC_GRID_COLOR_INDICATOR, m_GColorIndicator);
	DDX_Control(pDX, IDC_GIDE_NAME, m_GideName);
	DDX_Control(pDX, IDC_GIDE_SENS, m_GideSCTRL);
	DDX_Text(pDX, IDC_GIDE_SENS, m_GideSVAL);
	DDX_Control(pDX, IDC_GIDE_SENS_SPIN, m_GideSSPIN);
	DDX_Control(pDX, IDC_GIDE_COLOR_INDICATOR, m_GideColorIndicator);
}


BEGIN_MESSAGE_MAP(CPreviewSettings, CDialog)
	ON_BN_CLICKED(IDC_CH_GRID_COLOR_BTN, &CPreviewSettings::OnBnClickedChangeGridColor)
	ON_EN_CHANGE(IDC_GRID_SIZE_ED, &CPreviewSettings::OnEnChangeGridSizeEd)
	ON_BN_CLICKED(IDC_GRID_LINED, &CPreviewSettings::OnBnClickedGridLined)
	ON_BN_CLICKED(IDC_GRID_DOTTED, &CPreviewSettings::OnBnClickedGridDotted)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_GIDE_SENS, &CPreviewSettings::OnEnChangeGideSens)
	ON_BN_CLICKED(IDC_CH_GIDE_COLOR_BTN, &CPreviewSettings::OnBnClickedChGideColor)
	ON_CBN_SELCHANGE(IDC_GIDE_NAME, &CPreviewSettings::OnCbnSelchangeGideName)
END_MESSAGE_MAP()


// CPreviewSettings message handlers

BOOL CPreviewSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FPaint = TRUE;
	// Инициализация управления шагом сетки
	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();

	m_GridSizeVAL = m_Doc->GetGridSize();
	m_GridSizeSPIN.SetBuddy(&m_GridSizeCTRL);
	m_GridSizeSPIN.SetDecimalPlaces(6);
	m_GridSizeSPIN.SetTrimTrailingZeros(FALSE);
	m_GridSizeSPIN.SetPos(m_Doc->GetGridSize());
	m_GridSizeSPIN.SetRangeAndDelta(0.03125, 1.0, 0.03125);

	m_GridLined.SetCheck(BST_UNCHECKED);
	m_GridDotted.SetCheck(BST_CHECKED);

	// Инициализация управления гайд-лайнами
	GIDELINE_ARRAY* A = m_Doc->GetGidelines();
	if (!A->empty())
	{
		for (int i = 0; i < A->size(); i++)
		{
			m_GideName.InsertString(i, (*A)[i].m_Label);
		}
		m_GideName.SetCurSel(0);
		m_CurrentGide = 0;
		m_GideSVAL = (*A)[m_CurrentGide].m_SencitivityAreaWidth;
		m_GideSSPIN.SetBuddy(&m_GideSCTRL);
		m_GideSSPIN.SetRange(1, 5);
		m_GideSSPIN.SetPos((*A)[m_CurrentGide].m_SencitivityAreaWidth);
	}

	UpdateData(FALSE);
	return TRUE;  
}

void CPreviewSettings::OnBnClickedChangeGridColor()
{
	// Инициализация управления цветом сетки
	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();

	COLORREF m_CustColors[16];

	CHOOSECOLOR m_ColorStruct;
	m_ColorStruct.lStructSize		= sizeof(CHOOSECOLOR);
	m_ColorStruct.hwndOwner			= this->GetSafeHwnd();
	m_ColorStruct.hInstance			= NULL;
	m_ColorStruct.rgbResult			= m_Doc->GetGridColor();
	m_ColorStruct.lpCustColors		= m_CustColors;
	m_ColorStruct.Flags				= CC_RGBINIT | CC_FULLOPEN;
	m_ColorStruct.lCustData			= 0;
	m_ColorStruct.lpfnHook			= NULL;
	m_ColorStruct.lpTemplateName	= NULL;

	BOOL res = ChooseColor(&m_ColorStruct);

	if (res)
	{
		CDC* A = m_GColorIndicator.GetDC();
		CRect R; m_GColorIndicator.GetClientRect(&R);
		A->FillSolidRect(&R, m_ColorStruct.rgbResult);
		m_GColorIndicator.ReleaseDC(A);

		m_Doc->SetGridColor(m_ColorStruct.rgbResult);
	}
}

void CPreviewSettings::OnEnChangeGridSizeEd()
{
	UpdateData();

	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();

	m_Doc->SetGridSize(m_GridSizeVAL);
}

void CPreviewSettings::OnBnClickedGridLined()
{
	m_GridLined.SetCheck(BST_CHECKED);
	m_GridDotted.SetCheck(BST_UNCHECKED);

	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();
	
	m_Doc->SetGridType(GRID_LINED);
}

void CPreviewSettings::OnBnClickedGridDotted()
{
	m_GridLined.SetCheck(BST_UNCHECKED);
	m_GridDotted.SetCheck(BST_CHECKED);

	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();

	m_Doc->SetGridType(GRID_DOTTED);
}

void CPreviewSettings::OnPaint()
{
	CPaintDC dc(this); 

	if (m_FPaint)
	{
		CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
		CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
		CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();

		CDC* A = m_GColorIndicator.GetDC();
		CRect R; m_GColorIndicator.GetClientRect(&R);
		A->FillSolidRect(&R, m_Doc->GetGridColor());
		m_GColorIndicator.ReleaseDC(A);

		// Отрисовка индикатора цвета текущего гайда
		GIDELINE_ARRAY* GA = m_Doc->GetGidelines();
		CDC* B = m_GideColorIndicator.GetDC();
		m_GideColorIndicator.GetClientRect(&R);
		B->FillSolidRect(&R, (*GA)[m_CurrentGide].m_Color);
		m_GideColorIndicator.ReleaseDC(B);

		m_FPaint = FALSE;
	}
}

void CPreviewSettings::OnEnChangeGideSens()
{
	UpdateData();

	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();
	GIDELINE_ARRAY* A = m_Doc->GetGidelines();

	(*A)[m_CurrentGide].m_SencitivityAreaWidth = m_GideSVAL;
}

void CPreviewSettings::OnBnClickedChGideColor()
{
	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();
	GIDELINE_ARRAY* GA		= m_Doc->GetGidelines();

	COLORREF m_CustColors[16];

	CHOOSECOLOR m_ColorStruct;
	m_ColorStruct.lStructSize		= sizeof(CHOOSECOLOR);
	m_ColorStruct.hwndOwner			= this->GetSafeHwnd();
	m_ColorStruct.hInstance			= NULL;
	m_ColorStruct.rgbResult			= (*GA)[m_CurrentGide].m_Color;
	m_ColorStruct.lpCustColors		= m_CustColors;
	m_ColorStruct.Flags				= CC_RGBINIT | CC_FULLOPEN;
	m_ColorStruct.lCustData			= 0;
	m_ColorStruct.lpfnHook			= NULL;
	m_ColorStruct.lpTemplateName	= NULL;

	BOOL res = ChooseColor(&m_ColorStruct);

	if (res)
	{
		CDC* A = m_GideColorIndicator.GetDC();
		CRect R; m_GideColorIndicator.GetClientRect(&R);
		A->FillSolidRect(&R, m_ColorStruct.rgbResult);
		m_GideColorIndicator.ReleaseDC(A);

		(*GA)[m_CurrentGide].m_Color = m_ColorStruct.rgbResult;

		m_Doc->UpdateAllViews(NULL);
	}
}

void CPreviewSettings::OnCbnSelchangeGideName()
{
	CPreviewFrame* m_Parent = (CPreviewFrame*)GetParent();
	CGridView* m_View		= (CGridView*)m_Parent->m_Splitter.GetPane(1, 1);
	CPreviewDoc* m_Doc		= (CPreviewDoc*)m_View->GetDocument();
	GIDELINE_ARRAY* GA		= m_Doc->GetGidelines();
	
	m_CurrentGide = m_GideName.GetCurSel();
	m_GideSVAL = (*GA)[m_CurrentGide].m_SencitivityAreaWidth;
	m_GideSSPIN.SetPos((*GA)[m_CurrentGide].m_SencitivityAreaWidth);

	CDC* B = m_GideColorIndicator.GetDC();
	CRect R; m_GideColorIndicator.GetClientRect(&R);
	B->FillSolidRect(&R, (*GA)[m_CurrentGide].m_Color);
	m_GideColorIndicator.ReleaseDC(B);

	UpdateData(FALSE);
}

