#pragma once
#include "LightImpositionTypes.h"
#include <vector>

using namespace std;
typedef pair<int, FORMAT> STAT_PAIR;

class CDataShield
{
	//---------------------------------------------------------
	//                     �����!
	// ��� ���������� ���������� �������� � ������
	//---------------------------------------------------------
	BOOL m_PreviewMode;
	//---------------------------------------------------------
	// ��������� ����������
	//---------------------------------------------------------
	// ��������� ���� �������� �������
	//---------------------------------------------------------
	int m_NRows;				// ���������� ����� �� ������
	int m_NCols;				// ���������� �������� �� ������
	PRINTING_TYPE m_PRNType;	// ��� ������
	FOLDING_TYPE m_FLDType;		// ��� �����
	BINDING_TYPE m_BNDType;		// ��� ������ ������� 
	//---------------------------------------------------------
	// ��������� ��������� ������������ �������
	//---------------------------------------------------------
	int m_NPagesPerSection;		// ���������� ������� � �������
	int m_WhitePages;			// ���������� ������ ������� ����������� ��� ������������ �������
	int m_Sections;				// ���������� �������� �� �������
	int m_PagesInDocument;		// ���������� ������� � ���������
	int m_PlatesPerSection;		// ���������� ������� ��� ������ ����� �������
	int m_Places;				// ���������� �������� �� ��������
	int m_PlatesTotal;          // ������ ���������� ������� � �������
	//---------------------------------------------------------
	// ��������� ���������� ���
	//---------------------------------------------------------
	MEASURING_UNIT_TYPE m_UnitType;		// ��� ������� ���������
	double m_UnitK;						// ����������� �������������� ������ ���������
	ASFixedRect m_PlaceholderSize;      // ������������� ������������ ����������� ��� ��������� ������ ������������

	//=========================================================
	// ��������������� ��������� ������������ �����
	//=========================================================
	FORMAT m_OutputMediaSize;			// ������ ����� ������
	MEDIA_ORIENTATION m_MediaOrient;	// ���������� ����� ������
	bool m_InversePrinting;				// ������� ������ ���������� ����� �������
	OPERATION_MODE m_OPMode;			// ����� ������ ����������
	ASFixedRect m_PrintingArea;			// ������� ������ ��������� ����� (�� ��������� ����� ������� �����, �� ����� ���� ������)
	ASFixedRect m_OutputMediaBox;		// ��������������� ���������� - ������ � ���� ������� �����, ������ � ����� ������� ��� ���������� ����	
	ASFixed m_PlaceholdersGap;			// ������ ����� ��������� ���������� �� ��������	
	//=========================================================
	// ����������������, ���������������, �������� ��� �������� � �����
	//=========================================================
	ASFixed	m_X_Scale_Total;			// ������� �� ��� ������� �����
	ASFixed m_Y_Scale_Total;			// ������� �� ��� ������� �����
	ASFixed m_X_Translation_Total;		// ����������� �� ��� ������� �����
	ASFixed m_Y_Translation_Total;		// ����������� �� ��� ������� �����
	double m_Rotation_Angle_Total;		// ���� �������� �������� ����� � �������� 
	double m_ALFA_Skew_Total;			// ������� ��� ������� ��� �������� � �����
	double m_BETA_Skew_Total;			// ������� ��� ������� ��� �������� � �����
	//=========================================================
	// ��������������� ���������
	//=========================================================
	ASFixed m_TechAllowence_Y_width;    // ������ ������������ ��������������� �������
	ASFixed m_TechAllowence_X_width;    // ������ �������������� ��������������� �������
	ASFixed m_TechAllowence_M_width;	// ������ ����������� ��������������� ������� 
	bool m_PrintRegMarks;				// ������� ������ ����� ���������������� ������� (registration marks)
	bool m_PrintCMYKColorBar;			// ������� ������ ������� ����� CMYK
	bool m_PrintPANTONEcolorBar;        // ������� ������ ������� ����� PANTONE 
	bool m_PrintProgressiveGray;		// ������� ������ ������� ��������� ��� ������
	bool m_PrintCFMarks;				// ������� ������ ����� ������� �������
	// ������ �������� �����. ����������� ��� ������ ��������
	vector<TECH_MARK_SPEC> m_TechMarks;	
	// ��� ����� ��������� ���������
	CString m_FileName;
	//=========================================================
	// ��������� ������� CREEP
	//=========================================================
	double m_PaperThickness;

	//---------------------------------------------------------
	// ��������������� ���������� � ������
	//---------------------------------------------------------
	int m_CurrentPreviewPN;             // ������� ����� �������� ���������������� ���������
	PDPage m_CurrentPreviewPDPage;      // ������� �������� ���������������� ���������
	JDS* m_Job;							// ��������� �� ������ �������

	// ���������� ������� ��������������� �����
	void PrepareTMArray();
	// ���������� �������������� �������� �������� � ����������� �� ������ ������
	void RecalcLayout();
	// ����� ��������������� ������� ������� ��������� ���������
	void CalcPlaceholderSize();
public:
	CDataShield(void);
	~CDataShield(void);
	BOOL IsPreviewMode() { return m_PreviewMode; };
	void SetPreviewMode(BOOL mode) { m_PreviewMode = mode; };
	//---------------------------------------------------------
	// ��������� ������������� ���� �������� �������
	//---------------------------------------------------------
	void SetPRNType(PRINTING_TYPE type);
	void SetBNDType(BINDING_TYPE type);
	void SetFLDType(FOLDING_TYPE type);
	int SetNRows(int rows);
	int SetNColumns(int columns);
	// ��������� �������
	PRINTING_TYPE GetPRNType(){ return m_PRNType; };
	BINDING_TYPE  GetBNDType(){ return m_BNDType; };
	FOLDING_TYPE  GetFLDType(){ return m_FLDType; };
	int GetNCols(){ return m_NCols; };
	int GetNRows(){ return m_NRows; };
	int GetNPlaces(){ return m_Places; };
	void SetPaperThickness(double thickness) { m_PaperThickness = thickness / m_UnitK; };
	double GetPaperThickness() { return m_PaperThickness * m_UnitK; };
	//---------------------------------------------------------
	// ��������� ������������� ��������� �������
	//---------------------------------------------------------
	int SetPAGESPerSection(int pages);
	// ��������� �������
	int GetPLATESPerSection(){ return m_PlatesPerSection; };
	int GetSections(){ return m_Sections; };
	int GetPAGESPerSection(){ return m_NPagesPerSection; };
	int GetWhitePages(){ return m_WhitePages; };
	int GetPAGESInDoc(){ return m_PagesInDocument; };
	//---------------------------------------------------------
	// ��������� ������������� ���������� ���
	//---------------------------------------------------------
	// ��������� �������
	void SetMUnit(MEASURING_UNIT_TYPE type);
	MEASURING_UNIT_TYPE GetMUnitType() { return m_UnitType; };
	//=========================================================
	// ��������������� ��������� ������������ �����
	//=========================================================
	void SetOutputFormat(FORMAT media_size);
	void SetOrientation(MEDIA_ORIENTATION m_orient);
	void SetInversePrinting(bool inverse_printing);
	void SetOPMode(OPERATION_MODE m_mode);
	void SetPlaceholdersGap(double placeholders_gap);	
	//=========================================================
	// ����������������, ���������������, �������� ��� �������� � �����
	//=========================================================
	void SetXScaleT(double	X_Scale_Total);
	void SetYScaleT(double Y_Scale_Total);
	void SetXTransT(double X_Trans_Total);
	void SetYTransT(double Y_Trans_Total);
	void SetRAngleT(double RAngle_Total); 
	void SetALFASkewT(double ALFA_Skew_Total);
	void SetBETASkewT(double BETA_Skew_Total);
	//=========================================================
	// ��������������� ���������
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

	// ��������� ����������
	double GetUnitConvertionValue() { return m_UnitK; };
	MEDIA_ORIENTATION GetMediaOrientation() { return m_MediaOrient; };
	//=========================================================
	// ��������������� ��������� ������������ �����
	//=========================================================
	bool GetInversePrinting() { return m_InversePrinting; };
	OPERATION_MODE GetOPMode() { return m_OPMode;	};
	ASFixedRect GetPrintingArea() { return m_PrintingArea; };
	ASFixedRect GetOutputMediaBox() { return m_OutputMediaBox; };
	FORMAT GetOutputFormat() { return m_OutputMediaSize; }
	ASFixed GetPlaceholderGap() { return m_PlaceholdersGap;	};	
	double GetPlaceholderGapD() { return ASFixedToFloat(m_PlaceholdersGap) * m_UnitK; };
	//=========================================================
	// ����������������, ���������������, �������� ��� �������� � �����
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
	// ��������������� ���������
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
	// ������ �������� �����
	vector<TECH_MARK_SPEC>* GetTechMarks() { return &m_TechMarks; };	
	//=========================================================
	// ��������� �����
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
	// ��������� ��������� ��������
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
	// ���� ����������� ���������
	//=======================================================================
	void SetCurrentPreviewPN(PREVIEW_PAGE page);
	int GetCurrentPreviewPN() { return m_CurrentPreviewPN; };
	void SetPreviewPDPage(PDPage page) { m_CurrentPreviewPDPage = page; };
	PDPage GetCurrentPreviewPDPage() { return m_CurrentPreviewPDPage; };
	void SaveJobPreset() {};
	void LoadJobPreset() {};
};

