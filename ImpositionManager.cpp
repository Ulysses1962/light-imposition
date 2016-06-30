#include "ImpositionManager.h"

CImpositionManager::CImpositionManager(CDataShield* shield)
{
	m_Data = shield;
	m_Job = new JDS;
}

CImpositionManager::~CImpositionManager(void)
{
	delete m_Job;
}

//------------------------------------------------------------------------
// РЕАЛИЗАЦИЯ МЕТОДОВ ПОДГОТОВКИ ШАБЛОНОВ
//------------------------------------------------------------------------
void CImpositionManager::Create_Job()
{
	m_Job->m_Templates.clear();
	// Проверяем готовность шаблона
	if(m_Template.empty()) return;
	//----------------------------------------------------------------------------
	// Основной цикл подготовки шаблона
	//----------------------------------------------------------------------------
	int m_Elems = m_Data->GetNCols() * m_Data->GetNRows(); 
	m_Job->m_Rows = m_Data->GetNRows();
	m_Job->m_Cols = m_Data->GetNCols();
	m_Job->m_NPlates = 0;
	int m_StartPage;
	// Формирование спуска для стандартной двусторонней печати SADDLE STITCHED
	if (m_Data->GetBNDType() == SADDLE_STITCHED)
	{
		if (m_Data->GetPRNType() == STANDARD)
		{
			for (int i = 0; i < m_Data->GetSections(); i++)
			{
				for (int j = 0; j < m_Data->GetPLATESPerSection(); j++)
				{
					// Определяем начальную и конечную страницу диапазона
					m_StartPage = m_Data->GetPAGESPerSection() * i + j * m_Elems;
					// Цикл формирования шаблона фронтальной пластины
					T_MATRIX m_TPL; m_TPL.clear();
					for (int m = 0; m < m_Data->GetNCols() * 2; m++)
					{
						A_MATRIX* M = new A_MATRIX;
						for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
						{
							PLACEHOLDER P;
							P.m_PTNum = (*(m_Template[m]))[n].m_PTNum;
							P.m_POrt = (*(m_Template[m]))[n].m_POrt;
							P.m_TPos  = (*(m_Template[m]))[n].m_TPos;
							P.m_Creep = (*(m_Template[m]))[n].m_Creep;
							M->push_back(P);
						}
						m_TPL.push_back(M);
					}

					for (int m = 0; m < m_Data->GetNCols() * 2; m++)
					{
						for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
						{
							int m_RealPageN = m_StartPage + (*(m_TPL[m]))[n].m_PTNum - 1;
							if (m_RealPageN > m_Data->GetPAGESInDoc()) m_RealPageN = -1;
							(*(m_TPL[m]))[n].m_PRNum = m_RealPageN;
						}
					}
					//---------------------------------------------------------------
					// Добавляем пластины в задание
					//---------------------------------------------------------------
					// При этом исходим из предположения,что одна пластина лицевая
					// или оборотная - соответствует одному элементу задания
					//---------------------------------------------------------------
					T_MATRIX A, B;
					for (int m = 0; m < m_Data->GetNCols(); m++)
					{
						A.push_back(m_TPL[m]);
						B.push_back(m_TPL[m_Data->GetNCols() + m]);
					}
					m_Job->m_Templates.push_back(A);
					m_Job->m_NPlates++;
					m_Job->m_Templates.push_back(B);
					m_Job->m_NPlates++;
				}
			}
		}
		// Формирование спуска для печати с разворотом и переворотом страницы
		else
		{
			for (int i = 0; i < m_Data->GetSections(); i++)
			{
				for (int j = 0; j < m_Data->GetPLATESPerSection(); j++)
				{
					// Определяем начальную и конечную страницу диапазона
					m_StartPage = m_Data->GetPAGESPerSection() * i + j * m_Elems / 2;
					// Цикл формирования шаблона
					T_MATRIX m_TPL; m_TPL.clear();
					for (int m = 0; m < m_Data->GetNCols(); m++)
					{
						A_MATRIX* M = new A_MATRIX;
						for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
						{
							PLACEHOLDER P;
							P.m_PTNum = (*(m_Template[m]))[n].m_PTNum;
							P.m_POrt  = (*(m_Template[m]))[n].m_POrt;
							P.m_TPos  = (*(m_Template[m]))[n].m_TPos;
							P.m_Creep = (*(m_Template[m]))[n].m_Creep;
							M->push_back(P);
						}
						m_TPL.push_back(M);
					}

					for (int m = 0; m < m_Data->GetNCols(); m++)
					{
						for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
						{
							(*(m_TPL[m]))[n].m_PRNum = m_StartPage + (*(m_TPL[m]))[n].m_PTNum - 1;
						}
					}
					// Добавляем пластину в задание
					m_Job->m_Templates.push_back(m_TPL);
					m_Job->m_NPlates++;
				}
			}
		}
	}
	else // PERFECT BOUND
	{
		if (m_Data->GetPRNType() == STANDARD)
		{
			int m_Plates = m_Data->GetPAGESInDoc() / m_Elems * 2;
			m_StartPage = 0;
			for (int j = 0; j < m_Plates; j++)
			{
				// Создаем шаблоны
				T_MATRIX m_TPL; m_TPL.clear();
				for (int m = 0; m < m_Data->GetNCols() * 2; m++)
				{
					A_MATRIX* M = new A_MATRIX;
					for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
					{
						PLACEHOLDER P;
						P.m_PTNum = (*(m_Template[m]))[n].m_PTNum;
						P.m_POrt  = (*(m_Template[m]))[n].m_POrt;
						P.m_TPos  = (*(m_Template[m]))[n].m_TPos;
						P.m_Creep = (*(m_Template[m]))[n].m_Creep;
						M->push_back(P);
					}
					m_TPL.push_back(M);
				}

				for (int i = 0; i < m_Data->GetNCols() * 2; i++)
				{
					for (int m = m_Data->GetNRows() - 1; m >= 0; m--)
					{
						(*(m_TPL[i]))[m].m_PRNum = m_StartPage + (*(m_TPL[i]))[m].m_PTNum - 1;
					}
				}
				T_MATRIX A, B;
				for (int i = 0; i < m_Data->GetNCols(); i++)
				{
					A.push_back(m_TPL[i]);
					B.push_back(m_TPL[m_Data->GetNCols() + i]);
				}
				m_Job->m_Templates.push_back(A);
				m_Job->m_NPlates++;
				m_Job->m_Templates.push_back(B);
				m_Job->m_NPlates++;
				m_StartPage += m_Elems * 2;
			}
		}
		else
		{
			int m_Plates = m_Data->GetPAGESInDoc() / (m_Elems / 2);
			m_StartPage = 0;
			for (int j = 0; j < m_Plates; j++)
			{
				// Создаем шаблоны
				T_MATRIX m_TPL; m_TPL.clear();
				for (int m = 0; m < m_Data->GetNCols(); m++)
				{
					A_MATRIX* M = new A_MATRIX;
					for (int n = m_Data->GetNRows() - 1; n >= 0; n--)
					{
						PLACEHOLDER P;
						P.m_PTNum = (*(m_Template[m]))[n].m_PTNum;
						P.m_POrt  = (*(m_Template[m]))[n].m_POrt;
						P.m_TPos  = (*(m_Template[m]))[n].m_TPos;
						P.m_Creep = (*(m_Template[m]))[n].m_Creep;
						M->push_back(P);
					}
					m_TPL.push_back(M);
				}

				for (int i = 0; i < m_Data->GetNCols(); i++)
				{
					for (int m = 0; m < m_Data->GetNRows(); m++)
					{
						(*(m_TPL[i]))[m].m_PRNum = m_StartPage + (*(m_TPL[i]))[m].m_PTNum - 1;
					}
				}
				m_Job->m_Templates.push_back(m_TPL);
				m_Job->m_NPlates++;
				m_StartPage += m_Elems / 2;
			}
		}
	}
	m_Job->m_White = m_Data->GetWhitePages();
}

