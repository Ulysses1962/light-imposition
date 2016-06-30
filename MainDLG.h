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
	// ��������� �������
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
	// ��������� ���� ���������
	//--------------------------------------------------------------
	// ��������� ������ ���������
	CComboBox m_MUnitCTRL;
	// ��������� ���� ���������
	CComboBox m_ImpTypeCTRL;
	// ��������� ���������� ����� ���������
	CEdit m_NRowsCTRL;
	int m_NRowsVAL;
	// ��������� ���������� �������� ���������
	CEdit m_NColsCTRL;
	int m_NColsVAL;
	// ��������� ���� ������
	CComboBox m_BNDTypeCTRL;
	// ��������� ���� ������������ ������
	CComboBox m_DBLPrnCTRL;
	// ��������� ���������� ������� � �������
	int m_PpsVAL;
	CEdit m_PpsCTRL;
	CSpinButtonCtrl m_PpsSpinCTRL;
	// ��������� ������ ��������
	CEdit m_MediaWCTRL;
	double m_MediaWVAL;
	CNumSpinCtrl m_MediaWSpinCTRL;
	// ��������� ������ ��������
	CEdit m_MediaHCTRL;
	double m_MediaHVAL;
	CNumSpinCtrl m_MediaHSpinCTRL;
	// ��������� ������� ������
	CEdit m_PaperTCTRL;
	double m_PaperTVAL;
	CNumSpinCtrl m_PaperTSpinCTRL;
	//--------------------------------------------------------
	// ����������� ������
	//--------------------------------------------------------
	afx_msg void OnBnClickedCheck1();
	CButton m_ShowPreview;
	//--------------------------------------------------------
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnCbnSelchangeImpTypes3();
};
