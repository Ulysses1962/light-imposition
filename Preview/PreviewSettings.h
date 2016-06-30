#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NumSpinCtrl.h"
#include "preview_res.h"


// CPreviewSettings dialog

class CPreviewSettings : public CDialog
{
	DECLARE_DYNAMIC(CPreviewSettings)
	BOOL m_FPaint;
	int m_CurrentGide;
public:
	CPreviewSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPreviewSettings();

// Dialog Data
	enum { IDD = IDD_PREVIEW_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CNumSpinCtrl m_GridSizeSPIN;
	CEdit m_GridSizeCTRL;
	float m_GridSizeVAL;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChangeGridColor();
	afx_msg void OnEnChangeGridSizeEd();
	CButton m_GridLined;
	afx_msg void OnBnClickedGridLined();
	CButton m_GridDotted;
	afx_msg void OnBnClickedGridDotted();
	CStatic m_GColorIndicator;
	afx_msg void OnPaint();
	CComboBox m_GideName;
	CEdit m_GideSCTRL;
	int m_GideSVAL;
	CSpinButtonCtrl m_GideSSPIN;
	CStatic m_GideColorIndicator;
	afx_msg void OnEnChangeGideSens();
	afx_msg void OnBnClickedChGideColor();
	afx_msg void OnCbnSelchangeGideName();
};
