#include "StdAfx.h"
#include "DataShield.h"


CDataShield::CDataShield(void)
{
	// Определяем количество страниц в активном докменте
	m_PagesInDocument = PDDocGetNumPages(AVDocGetPDDoc(AVAppGetActiveDoc()));
	//-------------------------------------------------------------
	// Создаем набор непротиворечивых данных, используемых по умолчанию
	//-------------------------------------------------------------
	// Единицы измерения - дюймы
	m_UnitType = INCH;
	m_UnitK = 1.0;
	// Режим работы - по умолчанию фиксированный размер страницы, фиксированная схема, подбор размера пластины
	m_OPMode = C;
	// По умолчанию используем печать STANDARD, SADDLE STITCHED, GERMAN FOLD, 8-UP Imposition scheme
	m_PRNType	= STANDARD;
	m_BNDType	= SADDLE_STITCHED;
	m_FLDType	= GERMAN;
	m_NRows		= 2;
	m_NCols		= 4;
	m_Places    = m_NCols * m_NRows;
	// Сборка тетрадями по 16 листов (одна двустороняя пластина для заданого типа печати)
	m_NPagesPerSection = 16;
	m_PlatesPerSection = 1;
	// Количество тетрадей на документ и количество чистых страниц
	m_WhitePages = m_NPagesPerSection - (m_PagesInDocument % m_NPagesPerSection);
	m_Sections   = (m_PagesInDocument + m_WhitePages) / m_NPagesPerSection;
	// Полное число пластин в задании
	m_PlatesTotal = m_Sections * m_PlatesPerSection;
	//=============================================================
	// Технологические параметры
	//=============================================================
	// Устанавливаем принимаемые по умолчанию технологические параметры генератора
	m_TechAllowence_Y_width = m_TechAllowence_X_width = m_TechAllowence_M_width = ASFloatToFixed(72 * 10 / 25.4);
	// Отступ между страницами. По умолчанию 2мм = 0.079 дюйма
	m_PlaceholdersGap = ASFloatToFixed(72 * 0.079);
	// Размер страницы документа
	ASFixedRect m_PageCropBox;
	PDPageGetCropBox(PDDocAquirePage(AVDocGetPDDoc(AVAppGetActiveDoc()), 0), &m_PageCropBox);
	// Вычисляем размер области печати
	m_PrintingArea.left		= m_TechAllowence_Y_width;
	m_PrintingArea.right	= m_PrintingArea.left + 
		ASFixedMul((m_PageCropBox.right - m_PageCropBox.left), ASInt16ToFixed(m_NCols)) + 
		ASFixedMul(m_PlaceholdersGap, ASInt16ToFixed(m_NCols - 1));
	m_PrintingArea.bottom	= m_TechAllowence_X_width;
	m_PrintingArea.top		= m_PrintingArea.bottom +
		ASFixedMul((m_PageCropBox.top - m_PageCropBox.bottom), ASInt16ToFixed(m_NRows)) +
		ASFixedMul(m_TechAllowence_M_width, ASInt16ToFixed(m_NRows - 1));
	// Размер листа
	m_OutputMediaBox.bottom		= fixedZero;
	m_OutputMediaBox.top		= m_OutputMediaBox.bottom +
		ASFixedMul(m_TechAllowence_X_width, fixedTwo) + (m_PrintingArea.top - m_PrintingArea.bottom);
	m_OutputMediaBox.left		= fixedZero;
	m_OutputMediaBox.right		= m_OutputMediaBox.left +
		ASFixedMul(m_TechAllowence_Y_width, fixedTwo) + (m_PrintingArea.right - m_PrintingArea.left);
	// Формат печати - вычисляемый, ориентация - ЛАНДШАФТ
	m_OutputMediaSize.H = ASFloatToFixed(m_OutputMediaBox.top - m_OutputMediaBox.bottom) / 72.0;
	m_OutputMediaSize.W = ASFloatToFixed(m_OutputMediaBox.right - m_OutputMediaBox.left) / 72.0;
	m_MediaOrient		= LANDSCAPE;
	// Отмена зеркальной печати
	m_InversePrinting = false;
	// Устанавливаем общие к-ты масштабирования-перемещения-поворота для страницы
	m_X_Scale_Total = fixedOne;
	m_Y_Scale_Total = fixedOne;
	m_X_Translation_Total = fixedZero;
	m_Y_Translation_Total = fixedZero;
	m_Rotation_Angle_Total = 0.0;
	m_ALFA_Skew_Total = 0.0;
	m_BETA_Skew_Total = 0.0;
	// По умолчанию печатаем все технологические метки
	m_PrintRegMarks				= true;
	m_PrintCMYKColorBar			= true;
	m_PrintPANTONEcolorBar		= true;
	m_PrintProgressiveGray		= true;
	m_PrintCFMarks				= true;
	// Создаем массив технологических меток
	PrepareTMArray();
	// Текущий номер страницы предварительного просмотра
	m_CurrentPreviewPN = 0;
	m_CurrentPreviewPDPage = NULL;
	// Толщина бумаги
	m_PaperThickness = 0.002;
	// Режим предварительного просмотра - включен
	m_PreviewMode = TRUE;
}

CDataShield::~CDataShield(void)
{
}

void CDataShield::PrepareTMArray()
{
	// Обнуляем массив меток
	m_TechMarks.clear();
	//--------------------------------------------------------------------
	// Заполняем массив меток. 
	//--------------------------------------------------------------------
	int m_Elems = m_NRows * m_NCols * 4;	
	for (int i = 0; i < m_Elems + 7; i++)
	{
		TECH_MARK_SPEC m_TMS;
		// Метки сгиб-разрез
		// На каждый шаблон - по четыре метки
		if (i < m_Elems)
		{

			m_PrintCFMarks ? m_TMS.m_MarkIsActive = true : m_TMS.m_MarkIsActive = false;
			m_TMS.m_TMType			= CROP_FOLD_MARK;
			m_TMS.m_TM_VLen			= ASFloatToFixed(72 * 5 / 25.4);
			m_TMS.m_TM_HLen			= m_TMS.m_TM_VLen;
			m_TMS.m_TM_VPageGAP		= ASFloatToFixed(72 * 2 / 25.4);
			m_TMS.m_TM_HPageGAP		= m_TMS.m_TM_VPageGAP;
			switch(i % 4)
			{
			case 0: m_TMS.m_TM_Orientation = pdRotate0; break;
			case 1: m_TMS.m_TM_Orientation = pdRotate90; break;
			case 2: m_TMS.m_TM_Orientation = pdRotate180; break;
			case 3: m_TMS.m_TM_Orientation = pdRotate270; break;
			}
		}
		// Регистрационные метки - всего четыре
		if (i >= m_Elems && i < m_Elems + 4)
		{
			m_PrintRegMarks ? m_TMS.m_MarkIsActive = true : m_TMS.m_MarkIsActive = false;
			m_TMS.m_TMType			= REGISTRATION_MARK;
			m_TMS.m_TM_InnerCircleD	= ASFloatToFixed(72 * 1.5 / 25.4);
			m_TMS.m_TM_OuterCircleD	= ASFloatToFixed(72 * 3 / 25.4);
			m_TMS.m_TM_CrosshairLen = ASFloatToFixed(72 * 7 / 25.4);
			m_TMS.m_TM_PageGAP		= ASFloatToFixed(72 * 2.5 / 25.4);
			// Координаты привязки центра
			switch(i - m_Elems)
			{
			case 0:
				{
					m_TMS.m_TM_Origin_X = m_PrintingArea.left - m_TMS.m_TM_PageGAP - ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
					m_TMS.m_TM_Origin_Y = m_PrintingArea.top + m_TMS.m_TM_PageGAP + ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
				}
				break;
			case 1:
				{
					m_TMS.m_TM_Origin_X = m_PrintingArea.left - m_TMS.m_TM_PageGAP - ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
					m_TMS.m_TM_Origin_Y = m_PrintingArea.bottom - m_TMS.m_TM_PageGAP - ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
				}
				break;
			case 2:
				{
					m_TMS.m_TM_Origin_X = m_PrintingArea.right + m_TMS.m_TM_PageGAP + ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
					m_TMS.m_TM_Origin_Y = m_PrintingArea.bottom - m_TMS.m_TM_PageGAP - ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
				}
				break;
			case 3:
				{
					m_TMS.m_TM_Origin_X = m_PrintingArea.right + m_TMS.m_TM_PageGAP + ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
					m_TMS.m_TM_Origin_Y = m_PrintingArea.top + m_TMS.m_TM_PageGAP + ASFixedDiv(m_TMS.m_TM_CrosshairLen, fixedTwo);
				}
				break;
			}
		}
		// Таблица цветоделения
		if (i == m_Elems + 4)
		{
			m_PrintCMYKColorBar ? m_TMS.m_MarkIsActive = true : m_TMS.m_MarkIsActive = false;
			m_TMS.m_TM_SquareSize		= ASFloatToFixed(72 * 5 / 25.4);
			m_TMS.m_TM_Orientation		= pdRotate270;
			m_TMS.m_TMType				= GRADIENT_TINT_BAR;
			m_TMS.m_TM_PageGAP			= ASFloatToFixed(72 * 2.5 / 25.4);
			m_TMS.m_TM_Origin_X			= m_PrintingArea.right + m_TMS.m_TM_PageGAP;
			m_TMS.m_TM_Origin_Y			= m_PrintingArea.bottom + ASFixedDiv((m_PrintingArea.top - m_PrintingArea.bottom), fixedTwo) - ASFloatToFixed(72 * 20 / 25.4);
		}
		// Таблица градаций серого
		if (i == m_Elems + 5)
		{
			m_PrintProgressiveGray ? m_TMS.m_MarkIsActive = true : m_TMS.m_MarkIsActive = false;
			m_TMS.m_TMType				= PROGRESSIVE_COLOR_BAR_GRAY;
			m_TMS.m_TM_SquareSize		= ASFloatToFixed(72 * 5 / 25.4);
			m_TMS.m_TM_PageGAP			= ASFloatToFixed(72 * 2.5 / 25.4);
			m_TMS.m_TM_Origin_X			= m_PrintingArea.left - m_TMS.m_TM_PageGAP - m_TMS.m_TM_SquareSize;
			m_TMS.m_TM_Origin_Y			= m_PrintingArea.top - ASFloatToFixed(72 * 15 / 25.4);
			m_TMS.m_TM_Orientation		= pdRotate270;
		}
		// Таблица цветов PANTONE
		if (i == m_Elems + 6)
		{
			m_PrintPANTONEcolorBar ? m_TMS.m_MarkIsActive = true : m_TMS.m_MarkIsActive = false;
			m_TMS.m_TM_SquareSize		= ASFloatToFixed(72 * 5 / 25.4);
			m_TMS.m_TM_Orientation		= pdRotate270;
			m_TMS.m_TMType				= PANTONE_COLOR_BAR;
			m_TMS.m_TM_PageGAP			= ASFloatToFixed(72 * 2.5 / 25.4);
			// TO DO... Расчет точки привязки
		}
		m_TechMarks.push_back(m_TMS);
	}
}

void CDataShield::SetPRNType(PRINTING_TYPE type)
{
	m_PRNType = type;
}

void CDataShield::SetBNDType(BINDING_TYPE type)
{
	m_BNDType = type;
}

void CDataShield::SetFLDType(FOLDING_TYPE type)
{
	m_FLDType = type;
}

int CDataShield::SetNColumns(int columns)
{
	if ((m_BNDType == SADDLE_STITCHED || m_BNDType == PERFECT_BOUND) && m_PRNType == WORK_TUMBLE)
	{
		if (columns % 4 == 0) 
		{
			m_NCols = columns;
		}
		else
			return -1;
	}
	else
	{
		m_NCols = columns;
	}
	m_Places = m_NRows * m_NCols;
	//--------------------------------------------------------------
	// Перерасчет области печати в зависимости от ориентации 
	//--------------------------------------------------------------
	RecalcLayout();

	return 0;
}

int CDataShield::SetNRows(int rows)
{
	if ((m_BNDType == SADDLE_STITCHED || m_BNDType == PERFECT_BOUND) && m_PRNType == WORK_TURN)
	{
		if (rows % 4 == 0) 
		{
			m_NRows = rows;
		}
		else
			return -1;
	}
	else
	{
		m_NRows = rows;
	}
	m_Places = m_NRows * m_NCols;
	//--------------------------------------------------------------
	// Перерасчет области печати в зависимости от ориентации 
	//--------------------------------------------------------------
	RecalcLayout();

	return 0;
}

int CDataShield::SetPAGESPerSection(int pages)
{
	// Первое - проверяем чтобы делилось на 4
	if (pages % 4 != 0) return -1;
	m_NPagesPerSection = pages;
	// Производим перерасчет количества пустых страниц и количества пластин на тетрадь
	switch(m_PRNType)
	{
	case STANDARD: m_PlatesPerSection = m_NPagesPerSection / m_Places; break;
	case WORK_TURN: case WORK_TUMBLE: m_PlatesPerSection = (m_NPagesPerSection / m_Places) / 2; break;
	}
	// Количество тетрадей на документ и количество чистых страниц
	m_WhitePages = m_NPagesPerSection - (m_PagesInDocument % m_NPagesPerSection);
	m_Sections   = (m_PagesInDocument + m_WhitePages) / m_NPagesPerSection;
	return 0;
}

void CDataShield::SetMUnit(MEASURING_UNIT_TYPE type)
{
	m_UnitType = type;
	switch(m_UnitType)
	{
	case INCH: m_UnitK = 1.0; break;
	case MM: m_UnitK = 25.4; break;
	}
}

void CDataShield::SetOutputFormat(FORMAT media_size)
{
	if (m_OPMode == A || m_OPMode == B)
	{
		m_OutputMediaSize.H = media_size.H / m_UnitK;
		m_OutputMediaSize.W = media_size.W / m_UnitK;
		//--------------------------------------------------------------
		// Перерасчет размеров
		//--------------------------------------------------------------
		RecalcLayout();
	}
}

void CDataShield::SetOrientation(MEDIA_ORIENTATION m_orient)
{
	m_MediaOrient = m_orient;
}

void CDataShield::SetInversePrinting(bool inverse_printing)
{
	m_InversePrinting = inverse_printing;
}

void CDataShield::SetOPMode(OPERATION_MODE m_mode)
{
	m_OPMode = m_mode;
	// Перерасчет размеров
	RecalcLayout();
}

void CDataShield::SetPlaceholdersGap(double placeholders_gap)
{
	m_PlaceholdersGap = ASFloatToFixed(placeholders_gap / m_UnitK);
}

void CDataShield::SetXScaleT(double X_Scale_Total)
{
	m_X_Scale_Total = ASFloatToFixed(X_Scale_Total);
	RecalcLayout();
}

void CDataShield::SetYScaleT(double Y_Scale_Total)
{
	m_Y_Scale_Total = ASFloatToFixed(Y_Scale_Total);
	RecalcLayout();
}

void CDataShield::SetXTransT(double X_Trans_Total)
{
	m_X_Translation_Total = ASFloatToFixed(X_Trans_Total / m_UnitK);
	RecalcLayout();
}

void CDataShield::SetYTransT(double Y_Trans_Total)
{
	m_Y_Translation_Total = ASFloatToFixed(Y_Trans_Total / m_UnitK);
	RecalcLayout();
}

void CDataShield::SetRAngleT(double RAngle_Total)
{
	// ????????? А нужно ли?
	m_Rotation_Angle_Total = RAngle_Total;
	RecalcLayout();
}

void CDataShield::SetALFASkewT(double ALFA_Skew_Total)
{
	m_ALFA_Skew_Total = ALFA_Skew_Total;
}

void CDataShield::SetBETASkewT(double BETA_Skew_Total)
{
	m_BETA_Skew_Total = BETA_Skew_Total;
}

void CDataShield::SetTAYWidth(double TechAllowence_Y_width)
{
	m_TechAllowence_Y_width = ASFloatToFixed(TechAllowence_Y_width / m_UnitK);
}

void CDataShield::SetTAXWidth(double TechAllowence_X_width)
{
	m_TechAllowence_X_width = ASFloatToFixed(TechAllowence_X_width / m_UnitK);
}

void CDataShield::SetTAMWidth(double TechAllowence_M_width)
{
	m_TechAllowence_M_width = ASFloatToFixed(TechAllowence_M_width / m_UnitK);
}

void CDataShield::SetPrintRegMarks(bool PrintRegMarks)
{
	m_PrintRegMarks = PrintRegMarks;
	for (int i = m_Places * 4; i < m_Places * 4 + 4; i++)
	{
		m_TechMarks[i].m_MarkIsActive = PrintRegMarks;
	}
}

void CDataShield::SetPrintCMYKColorBar(bool PrintCMYKColorBar)
{
	m_PrintCMYKColorBar = PrintCMYKColorBar;
	m_TechMarks[m_Places * 4 + 4].m_MarkIsActive = PrintCMYKColorBar;
}

void CDataShield::SetPrintPANTONEColorBar(bool PrintPANTONEcolorBar)
{
	m_PrintPANTONEcolorBar = PrintPANTONEcolorBar;
	m_TechMarks[m_Places * 4 + 6].m_MarkIsActive = PrintPANTONEcolorBar;
}

void CDataShield::SetPrintProgressiveGray(bool PrintProgressiveGray)
{
	m_PrintProgressiveGray = PrintProgressiveGray;
	m_TechMarks[m_Places * 4 + 5].m_MarkIsActive = PrintProgressiveGray;
}

void CDataShield::SetPrintCFMarks(bool PrintCFMarks)
{
	m_PrintCFMarks = PrintCFMarks;
	for (int i = 0; i < m_Places * 4; i++)
	{
		m_TechMarks[i].m_MarkIsActive = PrintCFMarks;
	}
}

void CDataShield::SetOutputFileName(CString name)
{
	m_FileName = name;
}

void CDataShield::SetCFMarksVlen(double v_len)
{
	for (int i = 0; i < m_Places * 4; i++)
	{
		m_TechMarks[i].m_TM_VLen = ASFloatToFixed(v_len / m_UnitK);
	}
}

void CDataShield::SetCFMarksHLen(double h_len)
{
	for (int i = 0; i < m_Places * 4; i++)
	{
		m_TechMarks[i].m_TM_HLen = ASFloatToFixed(h_len / m_UnitK);
	}
}

void CDataShield::SetCFMarksVPageGAP(double v_gap)
{
	for (int i = 0; i < m_Places * 4; i++)
	{
		m_TechMarks[i].m_TM_VPageGAP = ASFloatToFixed(v_gap / m_UnitK);
	}
}

void CDataShield::SetCFMarksHPageGAP(double h_gap)
{
	for (int i = 0; i < m_Places * 4; i++)
	{
		m_TechMarks[i].m_TM_HPageGAP = ASFloatToFixed(h_gap / m_UnitK);
	}
}

void CDataShield::SetRGMarksInnerD(double d)
{
	for (int i = m_Places * 4; i < m_Places * 4 + 4; i++)
	{
		m_TechMarks[i].m_TM_InnerCircleD = ASFloatToFixed(d / m_UnitK);
	}
}

void CDataShield::SetRGMarksOuterD(double d)
{
	for (int i = m_Places * 4; i < m_Places * 4 + 4; i++)
	{
		m_TechMarks[i].m_TM_OuterCircleD = ASFloatToFixed(d / m_UnitK);
	}
}

void CDataShield::SetRGMarksCrossHLen(double len)
{
	for (int i = m_Places * 4; i < m_Places * 4 + 4; i++)
	{
		m_TechMarks[i].m_TM_CrosshairLen = ASFloatToFixed(len / m_UnitK);
	}
}

void CDataShield::SetColorTablesSquareSize(double size)
{
	for (int i = m_Places * 4 + 4; i < m_Places * 4 + 7; i++)
	{
		m_TechMarks[i].m_TM_SquareSize = ASFloatToFixed(size / m_UnitK);
	}
}

void CDataShield::SetColorTablesPageGAP(double size)
{
	for (int i = m_Places * 4 + 4; i < m_Places * 4 + 7; i++)
	{
		m_TechMarks[i].m_TM_PageGAP = ASFloatToFixed(size / m_UnitK);
	}
}

double CDataShield::GetCFMarksVlen()
{
	return ASFixedToFloat(m_TechMarks[0].m_TM_VLen) * m_UnitK;
}

double CDataShield::GetCFMarksHLen()
{
	return ASFixedToFloat(m_TechMarks[0].m_TM_HLen) * m_UnitK;
}

double CDataShield::GetCFMarksVPageGAP()
{
	return ASFixedToFloat(m_TechMarks[0].m_TM_VPageGAP) * m_UnitK;
}

double CDataShield::GetCFMarksHPageGAP()
{
	return ASFixedToFloat(m_TechMarks[0].m_TM_HPageGAP) * m_UnitK;
}

double CDataShield::GetRGMarksInnerD()
{
	return ASFixedToFloat(m_TechMarks[m_Places * 4].m_TM_InnerCircleD) * m_UnitK;
}

double CDataShield::GetRGMarksOuterD()
{
	return ASFixedToFloat(m_TechMarks[m_Places * 4].m_TM_OuterCircleD) * m_UnitK;
}

double CDataShield::GetRGMarksCrossHLen()
{
	return ASFixedToFloat(m_TechMarks[m_Places * 4].m_TM_CrosshairLen) * m_UnitK;
}

double CDataShield::GetColorTablesSquareSize()
{
	return ASFixedToFloat(m_TechMarks[m_Places * 4 + 4].m_TM_SquareSize) * m_UnitK;
}

double CDataShield::GetColorTablesPageGAP()
{
	return ASFixedToFloat(m_TechMarks[m_Places * 4 + 4].m_TM_PageGAP) * m_UnitK;
}

void CDataShield::SetCurrentPreviewPN(PREVIEW_PAGE page)
{
	switch(page)
	{
	case FIRST: 
		{
			m_CurrentPreviewPN = 0;
		}
		break;
	case NEXT:	
		{
			m_CurrentPreviewPN++;
			if (m_CurrentPreviewPN == m_PlatesTotal) m_CurrentPreviewPN = m_PlatesTotal - 1;
		}
		break;
	case PREV:
		{
			m_CurrentPreviewPN--;
			if (m_CurrentPreviewPN < 0) m_CurrentPreviewPN = 0;
		}
		break;
	case LAST: 
		{
			m_CurrentPreviewPN = m_PlatesTotal - 1; 
		}
		break;
	}
}

void CDataShield::CalcPlaceholdersMBox()
{
	STAT_PAIR m_SizeStat;
	PDPage m_P;
	ASFixedRect m_CBox;
	FORMAT m_PF;
	// Статистическая таблица
	vector<STAT_PAIR> m_StatTbl;
	// Активный документ
	PDDoc m_ActPDDoc = AVDocGetPDDoc(AVAppGetActiveDoc());
	//------------------------------------------------------------
	// БЛОК СТАТИСТИЧЕСКОГО АНАЛИЗА
	//------------------------------------------------------------
	for (int i = 0; i < m_PagesInDocument; i++)
	{
		// Получаем страницу
		m_P = PDDocAcquirePage(m_ActPDDoc, i);
		// Получаем размер кроп-бокса страницы
		PDPageGetCropBox(m_P, &m_CBox);
		m_PF.H	= ASFixedToFloat(m_CBox.top - m_CBox.bottom);
		m_PF.W	= ASFixedToFloat(m_CBox.right - m_CBox.left);
		// Ищем, нет ли такого формата в статистическом массиве
		BOOL m_FormatPresent = FALSE;
		for (int j = 0; j < m_StatTbl.size(); j++)
		{
			if (m_StatTbl[j].second.H == m_PF.H && m_StatTbl[j].second.W == m_PF.W)
			{
				// Есть такой формат! Инкрементируем счетчик вхождений
				m_StatTbl[j].first++;
				m_FormatPresent = TRUE;
			}
		}
		// Формат в массиве отсутствует. Добавляем вхождение
		if (!m_FormatPresent)
		{
			m_SizeStat = make_pair(1, m_PF);
			m_StatTbl.push_back(m_SizeStat);
		}
		// Освобождаем страницу
		PDPageRelease(m_P);
	}
	// Сбор статистической информации завершен, приступаем к пределению размера плейсхолдера
	int m_KeyValueMax = 1;
	int m_IndexMax = -1;
	for (int i = 0; i < m_StatTbl.size(); i++)
	{
		if (m_StatTbl[i].first > m_KeyValueMax)
		{
			m_KeyValueMax = m_StatTbl[i].first;
			m_IndexMax = i;
		}
	}
	// Теперь обрабатываем ситуацию, когда ВСЕ страницы в документе РАЗНОГО размера
	// В этом случае за основу принимаем наибольший из размеров
	// Из рассмотрения стараемся исключить форматы с нестандартными пропорциями(сильно вытянутые по ширине или высоте)
	// В основу сравнения ложим соотношение сторон 0.7 (или 1.4), как характерное для
	// стандартных форматов А4, А3 и т.п. Максимальное отклонение принимаем 20%
	// Т.е. предельные пропорции находятся в диапазоне 0.56..0.84 (1.12..1.68)  
	if (m_IndexMax == -1)
	{
		double m_Prop;
		double m_Square = 0.0;
		for (int i = 0; i < m_StatTbl.size(); i++)
		{
			m_Prop = m_StatTbl[i].second.H / m_StatTbl[i].second.W;
			if ((m_Prop >= 0.56 && m_Prop <= 0.84 ) || (m_Prop >= 1.12 && m_Prop <= 1.68))
			{
				if (m_StatTbl[i].second.H * m_StatTbl[i].second.W > m_Square)
				{
					m_Square = m_StatTbl[i].second.H * m_StatTbl[i].second.W;
					m_IndexMax = i;
				}
			}
		}
	}
	// Анализ окончен. Проверяем не случилось ли так,что и поиск по формату не дал результатов.
	// Если так произошло (все страницы не удовлетворяют условиям соблюдения пропорций),
	// в качестве основного размера выбираем первый попавшийся формат
	if (m_IndexMax == -1) m_IndexMax = 0;
	// Теперь все. Приступаем к формированию размера плейсхолдера
	m_PlaceholderSize.bottom	= fixedZero;
	m_PlaceholderSize.left		= fixedZero;
	m_PlaceholderSize.right		= ASFloatToFixed(m_StatTbl[m_IndexMax].second.W);
	m_PlaceholderSize.top		= ASFloatToFixed(m_StatTbl[m_IndexMax].second.H);
}