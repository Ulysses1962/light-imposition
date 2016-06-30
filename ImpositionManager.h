#pragma once
#include "LightImpositionTypes.h"
#include "DataShield.h"

class CImpositionManager
{
	CDataShield* m_Data;
	//=========================================================
	// Служебные параметры
	//=========================================================
	T_MATRIX m_Template;
	JDS* m_Job;
public:
	CImpositionManager(CDataShield* shield);
   ~CImpositionManager(void);
	// Установка шаблона	
   void SetTemplate(T_MATRIX temp) { m_Template = temp; };
	// Создание задания
   void Create_Job();
	// Получение задания
   JDS* Get_Job(){ return m_Job; }; 
};

