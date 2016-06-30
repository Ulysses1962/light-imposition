#pragma once
#include "Stub.h"
#include "VRuler.h"
#include "HRuler.h"
#include "GridView.h"
#include "FlatSplitterWnd.h"
#include "PreviewDoc.h"
#include "PreviewSettings.h"


// CPreviewFrame frame

class CPreviewFrame : public CFrameWnd
{
	BOOL m_Close;
	CPreviewDoc* m_Doc;
	BOOL m_ClientCreated;
	CPreviewSettings* m_SetDLG;

	DECLARE_DYNCREATE(CPreviewFrame)
public:
	CPreviewFrame();
	CPreviewFrame(CDialog* dlg);           // protected constructor used by dynamic creation
	virtual ~CPreviewFrame();

	CDialog* m_MainDLG;

protected:
	DECLARE_MESSAGE_MAP()
public:
	CFlatSplitterWnd m_Splitter;
	CToolBar m_TBar;
 
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnClose();
	void SetClose(BOOL w_close) { m_Close = w_close; };
	virtual CPreviewDoc* GetActiveDocument() { return m_Doc; } 
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolbarCmd(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnMove(int x, int y);
	BOOL m_DialogIsMain;
public:
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);

};


