#include "StdAfx.h"
#include "ImpositionCore.h"

CImpositionCore::CImpositionCore(CDataShield* shield)
{
	m_Data = shield;
}

CImpositionCore::~CImpositionCore(void)
{
	for (int i = 0; i < 2; i++)
	{
		delete  m_MatrixA[i];
		delete  m_MatrixB[i];
	}
}

//=====================================================================================
// Непосредственно алгоритм формирования матрицы спуска полос
//=====================================================================================
void CImpositionCore::Impose()
{
	// Готовим матрицы
	PrepareMatrices();
	// Производим подготовку частей шаблона
	ImposeA();
	// тыльную пластину готовим только в том случае, когда двухсторонняя печать STANDARD
	if(m_Data->GetPRNType() == STANDARD) ImposeB();
	// Готовим результирующую матрицу шаблона
	ComposeFullMatrix();
}

//=====================================================================================
// Методы импозиции полуматриц и создания полной матрицы шаблона импозиции
//=====================================================================================
void CImpositionCore::PrepareMatrices()
{
	m_MatrixA.clear();
	m_MatrixB.clear();
	m_TMatrix.clear();
	// Готовим матрицы
	for (int i = 0; i < m_Data->GetNCols(); i++)
	{
		A_MATRIX* A = new A_MATRIX;
		for (int j = 0; j < m_Data->GetNRows(); j++)
		{
			PLACEHOLDER PLACE;
			PLACE.m_TPos = i * m_Data->GetNRows() + j;
			PLACE.m_Creep = 0;
			A->push_back(PLACE);
		}
		m_MatrixA.push_back(A);
	}
	if (m_Data->GetPRNType() == STANDARD)
	{
		for (int i = 0; i < m_Data->GetNCols(); i++)
		{
			A_MATRIX* B = new A_MATRIX;
			for (int j = 0; j < m_Data->GetNRows(); j++)
			{
				PLACEHOLDER PLACE;
				PLACE.m_TPos = i * m_Data->GetNRows() + j;
				PLACE.m_Creep = 0;
				B->push_back(PLACE);
			}
			m_MatrixB.push_back(B);
		}
	}
}

void CImpositionCore::ImposeA()
{
	int m_NDelta, m_NStart_A, m_NStart_B, m_TotalPages;
	// Определяем не имеем ли мы дела со случаем односторонней печати
	switch(m_Data->GetPRNType())
	{
	case SINGLE_SIDED:
		{
			for (int i = 0; i < m_Data->GetNCols(); i++)
			{
				for (int j = 0; j < m_Data->GetNRows(); j++)
				{
					// Проставляем образцовые номера страниц
					(*(m_MatrixA[i]))[j].m_PTNum = i * m_Data->GetNRows() + j;
					// Проставляем ориентацию страниц
					(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
				}
			}
		}
		break;
	case SINGLE_SIDED_CARD:
		{
			for (int i = 0; i < m_Data->GetNCols(); i++)
			{
				for (int j = 0; j < m_Data->GetNRows(); j++)
				{
					// Проставляем образцовые номера страниц
					(*(m_MatrixA[i]))[j].m_PTNum = 1;
					// Проставляем ориентацию
					(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
				}
			}
		}
		break;
	case STANDARD:
		{
			switch (m_Data->GetBNDType())
			{
			case SADDLE_STITCHED:
				{
					// Вычисляем полное число страниц
					m_TotalPages = m_Data->GetNCols() * m_Data->GetNRows() * 2;
					// Определяем срединный элемент
					int m_N_Half = m_TotalPages / 2;
					// Определяем стартовые номера страниц для первой полосы
					m_NStart_A = m_N_Half;
					m_NStart_B = m_NStart_A + 1;
					bool direct = true;
					bool ort = true;
					for (int i = 0; i < m_Data->GetNCols(); i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							if (direct)
							{
								(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate180;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate180;
								}
							}
							else
							{
								(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
								}
							}
							if (j % 2 == 0) m_NDelta = 3; 
							else m_NDelta = 1;
							m_NStart_A -= m_NDelta;
							m_NStart_B += m_NDelta;
							if (j < m_Data->GetNRows() - 1) ort = !ort;
						}
						// Изменяем направление
						direct = !direct;
					}
				}
				break;
			case PERFECT_BOUND: 
				{
					m_NStart_A = 2;
					m_NStart_B = 3;
					for (int i = 0; i < m_Data->GetNCols(); i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
							// Ориентация
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;

							m_NStart_A += 4;
							m_NStart_B += 4;
						}
					}
				}
				break;
			case STACKED:
				{
					m_NStart_A = 2;
					for (int i = 0; i < m_Data->GetNCols(); i++)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							m_NStart_A += 2;
						}
					}
				}
				break;
			}
		}
		break;
	case WORK_TURN:
		{
			switch (m_Data->GetBNDType())
			{
			case SADDLE_STITCHED:
				{
					m_TotalPages = m_Data->GetNCols() * m_Data->GetNRows();
					// Определяем срединный элемент
					int m_N_Half = m_TotalPages / 2;
					// Определяем стартовые номера страниц для первой полосы
					m_NStart_A = m_N_Half;
					m_NStart_B = m_NStart_A + 1;
					bool direct = true;
					bool ort = true;
					//========================================================
					// Первая полуматрица
					//========================================================
					for (int i = 0; i < m_Data->GetNCols() / 2; i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							if (direct)
							{
								(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate180;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate180;
								}
							}
							else
							{
								(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
								}
							}
							if (j % 2 == 0) m_NDelta = 3; 
							else m_NDelta = 1;
							m_NStart_A -= m_NDelta;
							m_NStart_B += m_NDelta;
							if (j < m_Data->GetNRows() - 1) ort = !ort;
						}
						// Изменяем направление
						direct = !direct;
					}
					//========================================================
					// Вторая полуматрица
					//========================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols() / 2; i++)
					{
						bool plus_by_row = true;
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							if (!(plus_by_row ^ plus_by_col))
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols()/ 2 - i - 1]))[j].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols()/ 2 - i - 1]))[j].m_PTNum - 1;
							}
							(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_POrt = (*(m_MatrixA[m_Data->GetNCols()/ 2 - i - 1]))[j].m_POrt;
							plus_by_row = !plus_by_row;
						}
						plus_by_col = !plus_by_col;
					}
				}
				break;
			case PERFECT_BOUND:
				{
					//===============================================================
					// Первая полуматрица
					//===============================================================
					m_NStart_A = 2;
					m_NStart_B = 3;
					for (int i = 0; i < m_Data->GetNCols() / 2; i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
							// Ориентация
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;
							m_NStart_A += 4;
							m_NStart_B += 4;
						}
					}
					//===============================================================
					// Вторая полуматрица
					//===============================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols() / 2; i++)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							if (plus_by_col)
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols()/ 2 - i - 1]))[j].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols()/ 2 - i - 1]))[j].m_PTNum - 1;
							}
							(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_POrt = pdRotate0;
						}
						plus_by_col = !plus_by_col;
					}
				}
				break;
			case STACKED:
				{
					//====================================================
					// Первая полуматрица
					//====================================================
					m_NStart_A = 2;
					for (int i = 0; i < m_Data->GetNCols() / 2; i++)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							m_NStart_A += 2;
						}
					}
					//====================================================
					// Вторая полуматрица
					//====================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols() / 2; i++)
					{
						for (int j = 0; j < m_Data->GetNRows(); j++)
						{
							if (plus_by_col)
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() / 2 - i - 1]))[j].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() / 2 - i - 1]))[j].m_PTNum - 1;
							}
							(*(m_MatrixA[m_Data->GetNCols() / 2 + i]))[j].m_POrt = pdRotate0;
						}
						plus_by_col = !plus_by_col;
					}
				}
				break;
			}
		}
		break;
	case WORK_TUMBLE:
		{
			switch (m_Data->GetBNDType())
			{
			case SADDLE_STITCHED:
				{
					m_TotalPages = m_Data->GetNCols() * m_Data->GetNRows();
					// Определяем срединный элемент
					int m_N_Half = m_TotalPages / 2;
					// Определяем стартовые номера страниц для первой полосы
					m_NStart_A = m_N_Half;
					m_NStart_B = m_NStart_A + 1;
					bool direct = true;
					bool ort = true;
					//========================================================
					// Первая полуматрица
					//========================================================
					for (int i = 0; i < m_Data->GetNCols(); i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							if (direct)
							{
								(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i]))[j].m_POrt = pdRotate180;
									(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate180;
								}

							}
							else
							{
								(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_A;
								(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_PTNum = m_NStart_B;
								// Проставляем ориентацию страниц
								if (ort)
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate0;
								}
								else
								{
									(*(m_MatrixA[i + 1]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
									(*(m_MatrixA[i]))[m_Data->GetNRows() - j - 1].m_POrt = pdRotate180;
								}
							}
							if (j % 2 == 0) m_NDelta = 3; 
							else m_NDelta = 1;
							m_NStart_A -= m_NDelta;
							m_NStart_B += m_NDelta;
							if (j < m_Data->GetNRows() / 2 - 1) ort = !ort;
						}
						// Изменяем направление
						direct = !direct;
					}
					//========================================================
					// Вторая полуматрица
					//========================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols(); i++)
					{
						bool plus_by_row = true;
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							if (!(plus_by_row ^ plus_by_col))
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_PTNum - 1;
							}
							(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_POrt == pdRotate180 ? (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate0 : (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate180; 
							plus_by_row = !plus_by_row;
						}
						plus_by_col = !plus_by_col;
					}
				}
				break;
			case PERFECT_BOUND:
				{
					//===============================================================
					// Первая полуматрица
					//===============================================================
					m_NStart_A = 2;
					m_NStart_B = 3;
					for (int i = 0; i < m_Data->GetNCols(); i += 2)
					{
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i + 1]))[j].m_PTNum = m_NStart_B;
							// Ориентация
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							(*(m_MatrixA[i + 1]))[j].m_POrt = pdRotate0;

							m_NStart_A += 4;
							m_NStart_B += 4;
						}
					}
					//===============================================================
					// Вторая полуматрица
					//===============================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols(); i++)
					{
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							if (plus_by_col)
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows()/ 2 - j - 1].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_PTNum - 1;
							}
							(*(m_MatrixA[i]))[m_Data->GetNRows()/ 2 - j - 1].m_POrt == pdRotate180 ? (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate0 : (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate180;
						}
						plus_by_col = !plus_by_col;
					}
				}
				break;
			case STACKED:
				{
					//====================================================
					// Первая полуматрица
					//====================================================
					m_NStart_A = 2;
					for (int i = 0; i < m_Data->GetNCols(); i++)
					{
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							(*(m_MatrixA[i]))[j].m_PTNum = m_NStart_A;
							(*(m_MatrixA[i]))[j].m_POrt = pdRotate0;
							m_NStart_A += 2;
						}
					}
					//====================================================
					// Вторая полуматрица
					//====================================================
					bool plus_by_col = true;
					for (int i = 0; i < m_Data->GetNCols(); i++)
					{
						for (int j = 0; j < m_Data->GetNRows() / 2; j++)
						{
							if (plus_by_col)
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_PTNum + 1;
							}
							else
							{
								(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_PTNum = (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_PTNum - 1;
							}
							(*(m_MatrixA[i]))[m_Data->GetNRows() / 2 - j - 1].m_POrt == pdRotate180 ? (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate0 : (*(m_MatrixA[i]))[m_Data->GetNRows() / 2 + j].m_POrt = pdRotate180; 
						}
						plus_by_col = !plus_by_col;
					}

				}
				break;
			}
		}
		break;
	}
}

void CImpositionCore::ImposeB()
{
	switch (m_Data->GetBNDType())
	{
	case SADDLE_STITCHED:
		{
			bool plus_by_col = true;
			for (int i = 0; i < m_Data->GetNCols(); i++)
			{
				bool plus_by_row = true;
				for (int j = 0; j < m_Data->GetNRows(); j++)
				{
					if (!(plus_by_row ^ plus_by_col))
					{
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum + 1;
					}
					else
					{
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum - 1;
					}
					(*(m_MatrixB[i]))[j].m_POrt = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_POrt;
					plus_by_row = !plus_by_row;
				}
				plus_by_col = !plus_by_col;
			}
		}
		break;
	case PERFECT_BOUND:
		{
			bool plus_by_col = true;
			for (int i = 0; i < m_Data->GetNCols(); i++)
			{
				for (int j = 0; j < m_Data->GetNRows(); j++)
				{
					if (plus_by_col)
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum + 1;
					else
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum - 1;
					(*(m_MatrixB[i]))[j].m_POrt = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_POrt;
				}
				plus_by_col = !plus_by_col;
			}
		}
		break;
	case STACKED:
		{
			bool plus_by_col = true;
			for (int i = 0; i < m_Data->GetNCols(); i++)
			{
				for (int j = 0; j < m_Data->GetNRows(); j++)
				{
					if (plus_by_col)
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum + 1;
					else
						(*(m_MatrixB[i]))[j].m_PTNum = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_PTNum - 1;
					(*(m_MatrixB[i]))[j].m_POrt = (*(m_MatrixA[m_Data->GetNCols() - i - 1]))[j].m_POrt;
				}
				plus_by_col = !plus_by_col;
			}
		}
		break;
	}
}

void CImpositionCore::ComposeFullMatrix()
{
	for (int i = 0; i < m_Data->GetNCols(); i++)
	{
		m_TMatrix.push_back(m_MatrixA[i]);
	}
	if (m_Data->GetPRNType() == STANDARD)
	{
		for (int i = 0; i < m_Data->GetNCols(); i++)
		{
			m_TMatrix.push_back(m_MatrixB[i]);
		}
	}
}

//===================================================================
// Вспомогательные методы класса
//===================================================================
T_MATRIX CImpositionCore::GetTemlateMatrix()
{
	return m_TMatrix;
}
