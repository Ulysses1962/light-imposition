#pragma once
#include "LightImpositionTypes.h"
#include "DataShield.h"

class CImpositionManager
{
	CDataShield* m_Data;
	//=========================================================
	// ��������� ���������
	//=========================================================
	T_MATRIX m_Template;
	JDS* m_Job;
public:
	CImpositionManager(CDataShield* shield);
   ~CImpositionManager(void);
	// ��������� �������	
   void SetTemplate(T_MATRIX temp) { m_Template = temp; };
	// �������� �������
   void Create_Job();
	// ��������� �������
   JDS* Get_Job(){ return m_Job; }; 
};

