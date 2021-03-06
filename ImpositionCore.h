#pragma once
#include "LightImpositionTypes.h"
#include "DataShield.h"

/******************************************************************************************
		���������� � ����������� ����� ������� �������
������ �������� ���������� � ������ ������� ����. ����������� ���������� ������� ����������
����� 2 - �.�. ������ ��� - �����-�����, ������ ��� - ������-���� � �.�.
*******************************************************************************************/
//-----------------------------------------------------------------------------------------
// ������� ����� ������� ������ �����
//-----------------------------------------------------------------------------------------
class CImpositionCore
{
	// �������
	M_MATRIX m_MatrixA;     // ������� ������� ������� (��� ������ ��� ������ � ����� ��������)
	M_MATRIX m_MatrixB;     // ������� �������� ������� ��� ������ STANDARD
	T_MATRIX m_TMatrix;     // ������ ������� ������� ���������

	CDataShield* m_Data;	// ��������� �� ��������� ����������
	//=====================================================================================
	// ��������� ������ ������
	//=====================================================================================
	// ������ ��������������� ���������
	void ImposeA();				// ��������������� ����� ������������ ������ �����������
	void ImposeB();				// ��������������� ����� ������������ ������ �����������
	void ComposeFullMatrix();	// ��������������� ����� ������������ ������ ������� �������
	// ���������� ���������� ��� ����������
	void PrepareMatrices();
public:
	CImpositionCore(CDataShield* shield);
	~CImpositionCore(void);
	//=====================================================================================
	// ��������������� �������� ������������ ������� ������ �����
	//=====================================================================================
	void Impose();
	//=====================================================================================
	// ��������� ������� ������� � ��� �������������
	//=====================================================================================
	T_MATRIX GetTemlateMatrix();
};

