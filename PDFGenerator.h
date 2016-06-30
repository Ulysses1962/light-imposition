#pragma once
#include "LightImpositionTypes.h"
#include "DataShield.h"

class CPDFGenerator
{
	CDataShield* m_Data;
	// Задание на формирование документа
	JDS* m_JobStruct;
	//==============================================================
	// Параметры страницы-исходника
	//==============================================================
	ASFixedRect m_PageRect;				// Текущая страница
	ASFixedRect m_SrcPageMBox;			// Медиабокс исходной страницы
	ASFixedRect m_SrcCropBox;			// Кроп бокс исходной страницы
	ASFixedMatrix m_PageMatrix;			// Матрица преобразования страницы исходная
	//==============================================================
	// Служебные процедуры
	//==============================================================
	MATRIX_BUILD_MODE m_MatrixBuildMode;					// Режим построения матрицы - для плейсхолдера или для страницы в целомS
	ASFixedMatrix BuildTransitionMatrix();					// Построитель матрицы перемещений
	ASFixedMatrix BuildRotationMatrix();					// Построитель иатрицы вращений
	ASFixedMatrix BuildScalingMatrix();						// Построитель матрицы масштабирования
	ASFixedMatrix BuildSkewMatrix();						// Построитель матрицы сдвигов
	ASFixedMatrix BuildTransformMatrix();					// Построитель матрицы комплексного преобразования
	ASFixed CalcDelta(ASFixedRect dest, ASFixedRect src);	// Вспомогательная расчетная функция
	void InsertCFMarks();									// Вставка меток сгиба-разреза
	void InsertRegMarks();									// Вставка меток регистрации
	void InsertTintColorBar();								// Вставка таблицы цветоделения
	void InsertPogressiveColorBar();						// Вставка таблицы полутонов
	void InsertPANTONEColorTable();							// Вставка таблицы цветов PANTONE
	void InsertPages();										// Метод вставки страниц исходного документа (для одной пластины)
	void InsertPreviewPages();                              // Вставка страниц предварительного просмотра
	void SaveDSTDocToFile();								// Сохраняет итоговый документ в файл
	void CalcPlaceholdersMBox();							// Подготовка задания - расчет размеров шаблонов
	void CalcCreepArray() {};								// Расчет массива Creep
	//==============================================================
	// Разное
	//==============================================================
	AVDoc m_ActAVDoc;					// Активный AVDoc
	PDDoc m_ActPDDoc;					// Активный PDDoc	
	PDDoc m_DestPDDoc;					// Результирующий документ
	//==============================================================
	// Переменные, непосредственно связанные с созданием ПДФ-файла
	//==============================================================
	PDPage m_CurrentSrcPDPage;			// Инициализируется генератором при обработке файла
	PDPage m_CurrentDstPDPage;			// Инициализируется аналогичным образом
	int m_CurrentPlateNum;				// Номер текущей пластины, которая создается
	int m_CurrentPlaceholder;			// Текущий плейсхолдер	
	int m_CurrentColumn;                // Текущая колонка матрицы
	//==============================================================
	// Служебные методы 
	//==============================================================
	// Утилита формирования формы
	void UT_PDEFormSetContent(PDEContent pdeContent, CosDoc cosDoc, ASFixedRect* bBox, ASFixedMatrixP elMatrix, PDEForm* pForm);
public:
	CPDFGenerator(CDataShield* shield);
	~CPDFGenerator(void);
	//==============================================================
	// Метод создания файла со спуском полос
	//==============================================================
	void CreatePDF();
	//==============================================================
	// Подготовка исходных данных для спуска полос
	//==============================================================
	void SetJobStructure(JDS* job) { m_JobStruct = job; };	 

};


