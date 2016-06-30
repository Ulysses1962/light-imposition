#pragma once
#include "LightImpositionTypes.h"
#include "DataShield.h"

class CPDFGenerator
{
	CDataShield* m_Data;
	// ������� �� ������������ ���������
	JDS* m_JobStruct;
	//==============================================================
	// ��������� ��������-���������
	//==============================================================
	ASFixedRect m_PageRect;				// ������� ��������
	ASFixedRect m_SrcPageMBox;			// ��������� �������� ��������
	ASFixedRect m_SrcCropBox;			// ���� ���� �������� ��������
	ASFixedMatrix m_PageMatrix;			// ������� �������������� �������� ��������
	//==============================================================
	// ��������� ���������
	//==============================================================
	MATRIX_BUILD_MODE m_MatrixBuildMode;					// ����� ���������� ������� - ��� ������������ ��� ��� �������� � �����S
	ASFixedMatrix BuildTransitionMatrix();					// ����������� ������� �����������
	ASFixedMatrix BuildRotationMatrix();					// ����������� ������� ��������
	ASFixedMatrix BuildScalingMatrix();						// ����������� ������� ���������������
	ASFixedMatrix BuildSkewMatrix();						// ����������� ������� �������
	ASFixedMatrix BuildTransformMatrix();					// ����������� ������� ������������ ��������������
	ASFixed CalcDelta(ASFixedRect dest, ASFixedRect src);	// ��������������� ��������� �������
	void InsertCFMarks();									// ������� ����� �����-�������
	void InsertRegMarks();									// ������� ����� �����������
	void InsertTintColorBar();								// ������� ������� ������������
	void InsertPogressiveColorBar();						// ������� ������� ���������
	void InsertPANTONEColorTable();							// ������� ������� ������ PANTONE
	void InsertPages();										// ����� ������� ������� ��������� ��������� (��� ����� ��������)
	void InsertPreviewPages();                              // ������� ������� ���������������� ���������
	void SaveDSTDocToFile();								// ��������� �������� �������� � ����
	void CalcPlaceholdersMBox();							// ���������� ������� - ������ �������� ��������
	void CalcCreepArray() {};								// ������ ������� Creep
	//==============================================================
	// ������
	//==============================================================
	AVDoc m_ActAVDoc;					// �������� AVDoc
	PDDoc m_ActPDDoc;					// �������� PDDoc	
	PDDoc m_DestPDDoc;					// �������������� ��������
	//==============================================================
	// ����������, ��������������� ��������� � ��������� ���-�����
	//==============================================================
	PDPage m_CurrentSrcPDPage;			// ���������������� ����������� ��� ��������� �����
	PDPage m_CurrentDstPDPage;			// ���������������� ����������� �������
	int m_CurrentPlateNum;				// ����� ������� ��������, ������� ���������
	int m_CurrentPlaceholder;			// ������� �����������	
	int m_CurrentColumn;                // ������� ������� �������
	//==============================================================
	// ��������� ������ 
	//==============================================================
	// ������� ������������ �����
	void UT_PDEFormSetContent(PDEContent pdeContent, CosDoc cosDoc, ASFixedRect* bBox, ASFixedMatrixP elMatrix, PDEForm* pForm);
public:
	CPDFGenerator(CDataShield* shield);
	~CPDFGenerator(void);
	//==============================================================
	// ����� �������� ����� �� ������� �����
	//==============================================================
	void CreatePDF();
	//==============================================================
	// ���������� �������� ������ ��� ������ �����
	//==============================================================
	void SetJobStructure(JDS* job) { m_JobStruct = job; };	 

};


