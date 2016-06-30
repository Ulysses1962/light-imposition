// MainDLG.cpp : implementation file
//

#include "stdafx.h"
#include "LightImposition.h"
#include "MainDLG.h"

// CMainDLG dialog

IMPLEMENT_DYNAMIC(CMainDLG, CDialog)

CMainDLG::CMainDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDLG::IDD, pParent)
	, m_NRowsVAL(0)
	, m_NColsVAL(0)
	, m_PpsVAL(0)
	, m_MediaWVAL(0)
	, m_MediaHVAL(0)
	, m_PaperTVAL(0)
{
	m_Preview = NULL;
	m_Data = NULL;
}

CMainDLG::~CMainDLG()
{
	if (m_Preview) delete m_Preview;
	if (m_Data) delete m_Data;
	if (ICore) delete ICore;
	if (IManager) delete IManager;
	if (IGenerator) delete IGenerator;
}

BOOL CMainDLG::OnInitDialog()
{
	CDialog::OnInitDialog();
	//----------------------------------------------------------
	// Инициализация служебных объектов
	//----------------------------------------------------------
	m_Preview	= new CPreview(this);
	m_Data		= new CDataShield();
	ICore		= new CImpositionCore(m_Data);
	IManager	= new CImpositionManager(m_Data);
	IGenerator	= new CPDFGenerator(m_Data);
	//----------------------------------------------------------
	// Инициализация элементов управления
	//----------------------------------------------------------
	// Инициализация единиц измерения
	m_MUnitCTRL.InsertString(0, "inch");
	m_MUnitCTRL.InsertString(1, "mm");
	MEASURING_UNIT_TYPE m_UnitType = m_Data->GetMUnitType(); 
	m_UnitType == INCH ? m_MUnitCTRL.SetCurSel(0) : m_MUnitCTRL.SetCurSel(1);
	// Инициализация типов раскладок
	m_ImpTypeCTRL.InsertString(0, "2-UP Imposition");
	m_ImpTypeCTRL.InsertString(1, "4-UP Imposition");
	m_ImpTypeCTRL.InsertString(2, "8-UP Imposition");
	m_ImpTypeCTRL.InsertString(3, "Custom Imposition");
	switch(m_Data->GetNPlaces())
	{
	case 8:	m_ImpTypeCTRL.SetCurSel(2); break;
 	case 4: m_ImpTypeCTRL.SetCurSel(1); break;
	case 2: m_ImpTypeCTRL.SetCurSel(0); break;
	default:
		{
			m_ImpTypeCTRL.SetCurSel(3);
			m_NRowsCTRL.EnableWindow(TRUE);
			m_NColsCTRL.EnableWindow(TRUE);
		}
	}
	m_NRowsVAL = m_Data->GetNRows();
	m_NColsVAL = m_Data->GetNCols();
	UpdateData(FALSE);
	// Инизиализация типов сшивок
	m_BNDTypeCTRL.InsertString(0, "Saddle stitched");
	m_BNDTypeCTRL.InsertString(1, "Perfect bound");
	m_BNDTypeCTRL.InsertString(2, "Stacked");
	BINDING_TYPE m_BType = m_Data->GetBNDType();
	switch(m_BType)
	{
	case SADDLE_STITCHED: m_BNDTypeCTRL.SetCurSel(0); break;
	case PERFECT_BOUND: m_BNDTypeCTRL.SetCurSel(1); break;
	case STACKED: m_BNDTypeCTRL.SetCurSel(2); break;
	}
	// Инициализация типа двусторонней печати
	m_DBLPrnCTRL.InsertString(0, "Standard");
	m_DBLPrnCTRL.InsertString(1, "Work & Turn");
	m_DBLPrnCTRL.InsertString(2, "Work & Tumble");
	m_DBLPrnCTRL.InsertString(3, "Single sided");
	m_DBLPrnCTRL.InsertString(4, "Single sided card");
	PRINTING_TYPE m_PType = m_Data->GetPRNType();
	switch(m_PType)
	{
	case STANDARD: m_DBLPrnCTRL.SetCurSel(0); break;
	case WORK_TUMBLE:  m_DBLPrnCTRL.SetCurSel(2); break;
	case WORK_TURN:  m_DBLPrnCTRL.SetCurSel(1); break;
	case SINGLE_SIDED:  m_DBLPrnCTRL.SetCurSel(3); break;
	case SINGLE_SIDED_CARD:  m_DBLPrnCTRL.SetCurSel(4); break;
	}
	// Инициализация управления количеством страниц в тетради
	m_PpsSpinCTRL.SetRange(4, 96);
	m_PpsSpinCTRL.SetBuddy(&m_PpsCTRL);
	m_PpsSpinCTRL.SetPos(m_Data->GetPAGESPerSection());
	// Число листов в теради должно быть кратно 4, т.к. добавление
	// одного разворота в тетрадь равнозначно добавлению 4 страниц
	// в задание
	UDACCEL m_Accel;
	m_Accel.nSec = 0;
	m_Accel.nInc = 4;
	m_PpsSpinCTRL.SetAccel(1, &m_Accel);
	// Инициализация управления размерами пластины
	double m_MUK = m_Data->GetUnitConvertionValue();
	// Ширина
	m_MediaWSpinCTRL.SetBuddy(&m_MediaWCTRL);
	m_MediaWSpinCTRL.SetDecimalPlaces(3);
	m_MediaWSpinCTRL.SetTrimTrailingZeros(FALSE);
	m_MediaWSpinCTRL.SetRangeAndDelta(0.079 * m_MUK, 3.937 * m_MUK, 0.002 * m_MUK);
	m_MediaWSpinCTRL.SetPos(m_Data->GetOutputFormat().W * m_MUK);
	// Высота
	m_MediaHSpinCTRL.SetBuddy(&m_MediaHCTRL);
	m_MediaHSpinCTRL.SetDecimalPlaces(3);
	m_MediaHSpinCTRL.SetTrimTrailingZeros(FALSE);
	m_MediaHSpinCTRL.SetRangeAndDelta(0.079 * m_MUK, 3.937 * m_MUK, 0.002 * m_MUK);
	m_MediaHSpinCTRL.SetPos(m_Data->GetOutputFormat().H * m_MUK);
	// Инициализация управления толщиной бумаги
	m_PaperTSpinCTRL.SetBuddy(&m_PaperTCTRL);
	m_PaperTSpinCTRL.SetDecimalPlaces(5);
	m_PaperTSpinCTRL.SetTrimTrailingZeros(FALSE);
	m_PaperTSpinCTRL.SetRangeAndDelta(0.00004 * m_MUK, 0.002 * m_MUK, 0.00004 * m_MUK);
	m_PaperTSpinCTRL.SetPos(m_Data->GetPaperThickness() * m_MUK);
	UpdateData(TRUE);

	// Запускаем создание спуска полос по умолчанию
	ICore->Impose();
	IManager->SetTemplate(ICore->GetTemlateMatrix());
	IManager->Create_Job();
	IGenerator->SetJobStructure(IManager->Get_Job());
	IGenerator->CreatePDF();
	
	return TRUE; 
}

void CMainDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_M_UNITS4, m_MUnitCTRL);
	DDX_Control(pDX, IDC_IMP_TYPES3, m_ImpTypeCTRL);
	DDX_Control(pDX, IDC_EDIT29, m_NRowsCTRL);
	DDX_Text(pDX, IDC_EDIT29, m_NRowsVAL);
	DDX_Control(pDX, IDC_EDIT30, m_NColsCTRL);
	DDX_Text(pDX, IDC_EDIT30, m_NColsVAL);
	DDX_Control(pDX, IDC_BIND_TYPES4, m_BNDTypeCTRL);
	DDX_Control(pDX, IDC_DS_PRINTING3, m_DBLPrnCTRL);
	DDX_Text(pDX, IDC_PPSECTION2, m_PpsVAL);
	DDX_Control(pDX, IDC_PPSECTION2, m_PpsCTRL);
	DDX_Control(pDX, IDC_PPS_SPIN2, m_PpsSpinCTRL);
	DDX_Control(pDX, IDC_P_WDTH3, m_MediaWCTRL);
	DDX_Text(pDX, IDC_P_WDTH3, m_MediaWVAL);
	DDX_Control(pDX, IDC_MW_SPIN3, m_MediaWSpinCTRL);
	DDX_Control(pDX, IDC_P_HEIGHT2, m_MediaHCTRL);
	DDX_Text(pDX, IDC_P_HEIGHT2, m_MediaHVAL);
	DDX_Control(pDX, IDC_MH_SPIN3, m_MediaHSpinCTRL);
	DDX_Control(pDX, IDC_P_THICKNESS, m_PaperTCTRL);
	DDX_Text(pDX, IDC_P_THICKNESS, m_PaperTVAL);
	DDX_Control(pDX, IDC_PT_SPIN, m_PaperTSpinCTRL);
	DDX_Control(pDX, IDC_CHECK1, m_ShowPreview);
}


BEGIN_MESSAGE_MAP(CMainDLG, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CMainDLG::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_IMP_TYPES3, &CMainDLG::OnCbnSelchangeImpTypes3)
END_MESSAGE_MAP()


// CMainDLG message handlers

void CMainDLG::OnBnClickedCheck1()
{
	int State = m_ShowPreview.GetCheck();
	if (State == BST_CHECKED)
	{
		m_Preview->ShowPreview(SW_SHOW);
		m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
	}
	else
	{
		m_Preview->ShowPreview(SW_HIDE);
	}
}

BOOL CMainDLG::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(message)
	{
	case WM_NCLBUTTONDOWN:
		if (m_Preview)
		{
			m_Preview->GetPreviewFramePtr()->m_DialogIsMain = TRUE;
		}
		break;
	case WM_MOVE:			// Движение окна
		if (m_Preview)
		{
			if (m_Preview->GetPreviewFramePtr()->m_DialogIsMain)
			{
				m_Preview->OnOwnerMove();
			}
		}
		break;
	case WM_USER + 1:		// Установка первой страницы документа
		{
			m_Data->SetCurrentPreviewPN(FIRST);
			if (m_Data->GetOPMode() == PREVIEW_MODE) IGenerator->CreatePDF();
			m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
		}
		break;
	case WM_USER + 2:		// Установка предыдущей страницы документа
		{
			m_Data->SetCurrentPreviewPN(PREV);
			if (m_Data->GetOPMode() == PREVIEW_MODE) IGenerator->CreatePDF();
			m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
		}
		break;
	case WM_USER + 3:		// Установка следующей страницы документа
		{
			m_Data->SetCurrentPreviewPN(NEXT);
			if (m_Data->GetOPMode() == PREVIEW_MODE) IGenerator->CreatePDF();
			m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
		}
		break;
	case WM_USER + 4:		// Установка последней страницы документа
		{
			m_Data->SetCurrentPreviewPN(LAST);
			if (m_Data->GetOPMode() == PREVIEW_MODE) IGenerator->CreatePDF();
			m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
		}
		break;
	}
	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CMainDLG::OnCbnSelchangeImpTypes3()
{
	int m_ImpTypeIdx = m_ImpTypeCTRL.GetCurSel();
	int m_OldRVal, m_OldCVal;
	// Запоминаем текущие значения
	m_OldRVal = m_NRowsVAL;
	m_OldCVal = m_NColsVAL; 
	switch (m_ImpTypeIdx)
	{
	case 0:
		m_NRowsVAL = 1;
		m_NColsVAL = 2;
		m_NRowsCTRL.EnableWindow(FALSE);
		m_NColsCTRL.EnableWindow(FALSE);
		break;
	case 1:
		m_NRowsVAL = 2;
		m_NColsVAL = 2;
		m_NRowsCTRL.EnableWindow(FALSE);
		m_NColsCTRL.EnableWindow(FALSE);
		break;
	case 2:
		m_NRowsVAL = 2;
		m_NColsVAL = 4;
		m_NRowsCTRL.EnableWindow(FALSE);
		m_NColsCTRL.EnableWindow(FALSE);
		break;
	case 3:
		m_NRowsVAL = 2;
		m_NColsVAL = 4;
		m_NRowsCTRL.EnableWindow(TRUE);
		m_NColsCTRL.EnableWindow(TRUE);
		break;
	}
	int A = m_Data->SetNRows(m_NRowsVAL);
	int B = m_Data->SetNColumns(m_NColsVAL);
	if (A == -1 || B == -1)
	{
		m_NColsVAL = m_OldCVal;
		m_NRowsVAL = m_OldRVal;
		::MessageBox(this->GetSafeHwnd(), "You have entered wrong columns or rows of imposition scheme. Check it and try again.", "ERROR!", MB_OK);
		return;
	}
	UpdateData(FALSE);
	// Обновляем превью
	// Запускаем создание спуска полос по умолчанию
	ICore->Impose();
	IManager->SetTemplate(ICore->GetTemlateMatrix());
	IManager->Create_Job();
	IGenerator->SetJobStructure(IManager->Get_Job());
	IGenerator->CreatePDF();
	m_Preview->SetPreviewPDPage(m_Data->GetCurrentPreviewPDPage());
}
