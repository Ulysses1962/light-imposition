#pragma once
#include "PreviewDoc.h"
#include "PreviewFrame.h"
#include "GridView.h"
#include "acrocalls.h"

class CPreview
{
	CPreviewFrame* m_PFrame;
	CWnd* creator;
	CPreviewDoc* m_CurrentDoc;
public:
	CPreview(CDialog* owner);
	~CPreview(void);
	//-------------------------------------------------------------
	// ���������� ����� ���������������� ���������
	//-------------------------------------------------------------
	void ShowPreview(int nCmdShow);
	void SetDockingView(bool view);
	// ������������ ����������� ����-������������
	void OnOwnerMove();
	// ��������� �������� ���������������� ���������
	void SetPreviewPDPage(PDPage page);
	CPreviewFrame* GetPreviewFramePtr() { return m_PFrame; }
	//-------------------------------------------------------------
	// ���������� ������������� �������
	//-------------------------------------------------------------
	void AddGideline(GIDELINE line);
	void RemoveGideline(CString name);
	GIDELINE GetGidelineInfo(CString name);
	GIDELINE GetGidelineInfo(int idx);
	void SetGidesVisible(BOOL visible);
};


