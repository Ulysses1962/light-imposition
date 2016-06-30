#pragma once
#include "LightImpositionTypes.h"
#include <vector>

using namespace std;
typedef pair<int, FORMAT> STAT_PAIR;

class CDataShield
{
	//---------------------------------------------------------
	//                     ВАЖНО!
	// Все внутренние переменные выражены в ДЮЙМАХ
	//---------------------------------------------------------
	BOOL m_PreviewMode;
	//---------------------------------------------------------
	// Внуренние переменные
	//---------------------------------------------------------
	// ПАРАМЕТРЫ ЯДРА СОЗДАНИЯ ШАБЛОНА
	//---------------------------------------------------------
	int m_NRows;				// Количество строк на спуске
	int m_NCols;				// Количество столбцов на спуске
	PRINTING_TYPE m_PRNType;	// Тип печати
	FOLDING_TYPE m_FLDType;		// Тип сгиба
	BINDING_TYPE m_BNDType;		// Тип сшивки брошюры 
	//---------------------------------------------------------
	// ПАРАМЕТРЫ МЕНЕДЖЕРА ФОРМИРОВАНИЯ ЗАДАНИЯ
	//---------------------------------------------------------
	int m_NPagesPerSection;		// Количество страниц в тетради
	int m_WhitePages;			// Количество пустых страниц необходимое для формирования тетради
	int m_Sections;				// Количество тетрадей на брошюру
	int m_PagesInDocument;		// Количество страниц в документе
	int m_PlatesPerSection;		// Количество пластин для печати одной тетради
	int m_Places;				// Количество шаблонов на пластине
	int m_PlatesTotal;          // Полное количество пластин в задании
	//---------------------------------------------------------
	// ПАРАМЕТРЫ ГЕНЕРАТОРА ПДФ
	//---------------------------------------------------------
	MEASURING_UNIT_TYPE m_UnitType;		// Тип единицы измерения
	double m_UnitK;						// Коеффициент преобразования единиц измерения
	ASFixedRect m_PlaceholderSize;      // Статистически определяемый усредненный для документа размер плейсхолдера

	//=========================================================
	// Технологические параметры формирования файла
	//=========================================================
	FORMAT m_OutputMediaSize;			// Формат листа печати
	MEDIA_ORIENTATION m_MediaOrient;	// Ориентация листа печати
	bool m_InversePrinting;				// Признак печати зеркальной копии страниц
	OPERATION_MODE m_OPMode;			// Режим работы генератора
	ASFixedRect m_PrintingArea;			// Область печати выходного листа (по умолчанию равна размеру листа, но может быть меньше)
	ASFixedRect m_OutputMediaBox;		// Вспомогательная переменная - хранит в себе размеры листа, только в более удобном для вычислений виде	
	ASFixed m_PlaceholdersGap;			// Отступ между соседними страницами на пластине	
	//=========================================================
	// Позиционирование, масштабирование, повороты для пластины в целом
	//=========================================================
	ASFixed	m_X_Scale_Total;			// Масштаб по оси абсцисс общий
	ASFixed m_Y_Scale_Total;			// Масштаб по оси ординат общий
	ASFixed m_X_Translation_Total;		// Перемещение по оси абсцисс общее
	ASFixed m_Y_Translation_Total;		// Перемещение по оси ординат общее
	double m_Rotation_Angle_Total;		// Угол поворота пластины общий в радианах 
	double m_ALFA_Skew_Total;			// Поворот оси абсцисс для пластины в целом
	double m_BETA_Skew_Total;			// Поворот оси ординат для пластины в целом
	//=========================================================
	// Технологические параметры
	//=========================================================
	ASFixed m_TechAllowence_Y_width;    // Ширина вертикальной технологической области
	ASFixed m_TechAllowence_X_width;    // Ширина горизонтальной технологической области
	ASFixed m_TechAllowence_M_width;	// Ширина межполосной технологической области 
	bool m_PrintRegMarks;				// Признак печати меток позиционирования пластин (registration marks)
	bool m_PrintCMYKColorBar;			// Признак печати таблицы цвета CMYK
	bool m_PrintPANTONEcolorBar;        // Признак печати таблицы цвета PANTONE 
	bool m_PrintProgressiveGray;		// Признак печати таблицы градиента для серого
	bool m_PrintCFMarks;				// Признак печати меток разреза страниц
	// Вектор описаний меток. Применяется для каждой пластины
	vector<TECH_MARK_SPEC> m_TechMarks;	
	// Имя файла выходного документа
	CString m_FileName;
	//=========================================================
	// Параметры расчета CREEP
	//=========================================================
	double m_PaperThickness;

	//---------------------------------------------------------
	// ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ И МЕТОДЫ
	//---------------------------------------------------------
	int m_CurrentPreviewPN;             // Текущий номер страницы предварительного просмотра
	PDPage m_CurrentPreviewPDPage;      // Текущая страница предварительного просмотра
	JDS* m_Job;							// Указатель на массив задания

	// Подготовка массива технологических меток
	void PrepareTMArray();
	// Перерасчет геометрических размеров пластины в зависимости от режима работы
	void RecalcLayout();
	// Метод статистического анализа страниц исходного документа
	void CalcPlaceholderSize();
public:
	CDataShield(void);
	~CDataShield(void);
	BOOL IsPreviewMode() { return m_PreviewMode; };
	void SetPreviewMode(BOOL mode) { m_PreviewMode = mode; };
	//---------------------------------------------------------
	// ИНТЕРФЕЙС ИНИЦИАЛИЗАЦИИ ЯДРА СОЗДАНИЯ ШАБЛОНА
	//---------------------------------------------------------
	void SetPRNType(PRINTING_TYPE type);
	void SetBNDType(BINDING_TYPE type);
	void SetFLDType(FOLDING_TYPE type);
	int SetNRows(int rows);
	int SetNColumns(int columns);
	// Получение свойств
	PRINTING_TYPE GetPRNType(){ return m_PRNType; };
	BINDING_TYPE  GetBNDType(){ return m_BNDType; };
	FOLDING_TYPE  GetFLDType(){ return m_FLDType; };
	int GetNCols(){ return m_NCols; };
	int GetNRows(){ return m_NRows; };
	int GetNPlaces(){ return m_Places; };
	void SetPaperThickness(double thickness) { m_PaperThickness = thickness / m_UnitK; };
	double GetPaperThickness() { return m_PaperThickness * m_UnitK; };
	//---------------------------------------------------------
	// ИНТЕРФЕЙС ИНИЦИАЛИЗАЦИИ МЕНЕДЖЕРА ЗАДАНИЙ
	//---------------------------------------------------------
	int SetPAGESPerSection(int pages);
	// Получение свойств
	int GetPLATESPerSection(){ return m_PlatesPerSection; };
	int GetSections(){ return m_Sections; };
	int GetPAGESPerSection(){ return m_NPagesPerSection; };
	int GetWhitePages(){ return m_WhitePages; };
	int GetPAGESInDoc(){ return m_PagesInDocument; };
	//---------------------------------------------------------
	// ИНТЕРФЕЙС ИНИЦИАЛИЗАЦИИ ГЕНЕРАТОРА ПДФ
	//---------------------------------------------------------
	// Установка свойств
	void SetMUnit(MEASURING_UNIT_TYPE type);
	MEASURING_UNIT_TYPE GetMUnitType() { return m_UnitType; };
	//=========================================================
	// Технологические параметры формирования файла
	//=========================================================
	void SetOutputFormat(FORMAT media_size);
	void SetOrientation(MEDIA_ORIENTATION m_orient);
	void SetInversePrinting(bool inverse_printing);
	void SetOPMode(OPERATION_MODE m_mode);
	void SetPlaceholdersGap(double placeholders_gap);	
	//=========================================================
	// Позиционирование, масштабирование, повороты для пластины в целом
	//=========================================================
	void SetXScaleT(double	X_Scale_Total);
	void SetYScaleT(double Y_Scale_Total);
	void SetXTransT(double X_Trans_Total);
	void SetYTransT(double Y_Trans_Total);
	void SetRAngleT(double RAngle_Total); 
	void SetALFASkewT(double ALFA_Skew_Total);
	void SetBETASkewT(double BETA_Skew_Total);
	//=========================================================
	// Технологические параметры
	//=========================================================
	void SetTAYWidth(double TechAllowence_Y_width);
	void SetTAXWidth(double TechAllowence_X_width);
	void SetTAMWidth(double TechAllowence_M_width); 
	void SetPrintRegMarks(bool PrintRegMarks);
	void SetPrintCMYKColorBar(bool PrintCMYKColorBar);
	void SetPrintPANTONEColorBar(bool PrintPANTONEcolorBar); 
	void SetPrintProgressiveGray(bool PrintProgressiveGray);
	void SetPrintCFMarks(bool PrintCFMarks);
	void SetOutputFileName(CString name);

	// Получение параметров
	double GetUnitConvertionValue() { return m_UnitK; };
	MEDIA_ORIENTATION GetMediaOrientation() { return m_MediaOrient; };
	//=========================================================
	// Технологические параметры формирования файла
	//=========================================================
	bool GetInversePrinting() { return m_InversePrinting; };
	OPERATION_MODE GetOPMode() { return m_OPMode;	};
	ASFixedRect GetPrintingArea() { return m_PrintingArea; };
	ASFixedRect GetOutputMediaBox() { return m_OutputMediaBox; };
	FORMAT GetOutputFormat() { return m_OutputMediaSize; }
	ASFixed GetPlaceholderGap() { return m_PlaceholdersGap;	};	
	double GetPlaceholderGapD() { return ASFixedToFloat(m_PlaceholdersGap) * m_UnitK; };
	//=========================================================
	// Позиционирование, масштабирование, повороты для пластины в целом
	//=========================================================
	ASFixed	GetXScaleT() { return m_X_Scale_Total; };
	double GetXScaleTD() { return ASFixedToFloat(m_X_Scale_Total); };
	ASFixed GetYScaleT() { return m_Y_Scale_Total; };
	double GetYScaleTD() { return ASFixedToFloat(m_Y_Scale_Total); };
	ASFixed GetXTransT() { return m_X_Translation_Total; };
	double GetXTransTD() { return ASFixedToFloat(m_X_Translation_Total) * m_UnitK; };
	ASFixed GetYTransT() { return m_Y_Translation_Total; };
	double GetYTransTD() { return ASFixedToFloat(m_Y_Translation_Total) * m_UnitK; };
	double GetRAngleT() { return m_Rotation_Angle_Total; }; 
	double GetALFASkewT() { return m_ALFA_Skew_Total; };
	double GetBETASkewT() { return m_BETA_Skew_Total; };
	//=========================================================
	// Технологические параметры
	//=========================================================
	ASFixed GetTAXWidth() { return m_TechAllowence_Y_width; };
	double GetTAXWidthD() { return ASFixedToFloat(m_TechAllowence_Y_width) * m_UnitK; };
	ASFixed GetTAYWidth() { return m_TechAllowence_X_width; };
	double GetTAYWidthD() { return ASFixedToFloat(m_TechAllowence_X_width) * m_UnitK; };
	ASFixed GetTAMWidth() { return m_TechAllowence_M_width;	};
	double GetTAMWidthD() { return ASFixedToFloat(m_TechAllowence_M_width) * m_UnitK; };
	bool GetPrintRegMarks() { return m_PrintRegMarks; };
	bool GetPrintColorBar() { return m_PrintCMYKColorBar; };
	bool GetPrintPANTONEColorBar() { return m_PrintPANTONEcolorBar; }; 
	bool GetPrintProgressiveGray() { return m_PrintProgressiveGray; };
	bool GetPrintCFMarks() { return m_PrintCFMarks; };
	CString GetOutputFileName() { return m_FileName; };
	// Вектор описаний меток
	vector<TECH_MARK_SPEC>* GetTechMarks() { return &m_TechMarks; };	
	//=========================================================
	// Установка меток
	//=========================================================
	void SetCFMarksVlen(double v_len);						
	void SetCFMarksHLen(double h_len);
	void SetCFMarksVPageGAP(double v_gap);
	void SetCFMarksHPageGAP(double h_gap);
	void SetRGMarksInnerD(double d);
	void SetRGMarksOuterD(double d);
	void SetRGMarksCrossHLen(double len);
	void SetColorTablesSquareSize(double size);
	void SetColorTablesPageGAP(double size);
	// Получение групповых настроек
	double GetCFMarksVlen();						
	double GetCFMarksHLen();
	double GetCFMarksVPageGAP();
	double GetCFMarksHPageGAP();
	double GetRGMarksInnerD();
	double GetRGMarksOuterD();
	double GetRGMarksCrossHLen();
	double GetColorTablesSquareSize();
	double GetColorTablesPageGAP();

	int GetPlatesTotal() { return m_PlatesTotal; };
	//=======================================================================
	// БЛОК УПРАПВЛЕНИЯ ПРОЦЕССОМ
	//=======================================================================
	void SetCurrentPreviewPN(PREVIEW_PAGE page);
	int GetCurrentPreviewPN() { return m_CurrentPreviewPN; };
	void SetPreviewPDPage(PDPage page) { m_CurrentPreviewPDPage = page; };
	PDPage GetCurrentPreviewPDPage() { return m_CurrentPreviewPDPage; };
	void SaveJobPreset() {};
	void LoadJobPreset() {};
};

