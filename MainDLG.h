#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include "Preview/NumSpinCtrl.h"
#include "DataShield.h"
#include "Preview/Preview.h"
#include "ImpositionCore.h"
#include "ImpositionManager.h"
#include "PDFGenerator.h"
#include "resource.h"

// CMainDLG dialog

class CMainDLG : public CDialog
{
	//--------------------------------------------------------------
	// Служебные объекты
	//--------------------------------------------------------------
	CPreview* m_Preview;
	CDataShield* m_Data;
	CImpositionCore* ICore;
	CImpositionManager* IManager;
	CPDFGenerator* IGenerator;


	DECLARE_DYNAMIC(CMainDLG)
public:
	CMainDLG(CWnd* pParent = NULL);   
	virtual ~CMainDLG();
	BOOL OnInitDialog();
	// Dialog Data
	enum { IDD = IDD_MAIN_DLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()
public:
	//--------------------------------------------------------------
	// Настройки ядра импозиции
	//--------------------------------------------------------------
	// Настройка единиц измерения
	CComboBox m_MUnitCTRL;
	// Настройка типа импозиции
	CComboBox m_ImpTypeCTRL;
	// Настройка количества строк импозиции
	CEdit m_NRowsCTRL;
	int m_NRowsVAL;
	// Настройка количества столбцов импозиции
	CEdit m_NColsCTRL;
	int m_NColsVAL;
	// Настройка типа сшивки
	CComboBox m_BNDTypeCTRL;
	// Настройка типа двусторонней печати
	CComboBox m_DBLPrnCTRL;
	// Настройка количества страниц в тетради
	int m_PpsVAL;
	CEdit m_PpsCTRL;
	CSpinButtonCtrl m_PpsSpinCTRL;
	// Настройка ширины пластины
	CEdit m_MediaWCTRL;
	double m_MediaWVAL;
	CNumSpinCtrl m_MediaWSpinCTRL;
	// Настройка высоты пластины
	CEdit m_MediaHCTRL;
	double m_MediaHVAL;
	CNumSpinCtrl m_MediaHSpinCTRL;
	// Настройка толщины бумаги
	CEdit m_PaperTCTRL;
	double m_PaperTVAL;
	CNumSpinCtrl m_PaperTSpinCTRL;
	//--------------------------------------------------------
	// Отображение превью
	//--------------------------------------------------------
	afx_msg void OnBnClickedCheck1();
	CButton m_ShowPreview;
	//--------------------------------------------------------
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnCbnSelchangeImpTypes3();
};
