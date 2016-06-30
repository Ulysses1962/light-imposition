#include "PDFGenerator.h"
#include "CosCalls.h"

CPDFGenerator::CPDFGenerator(CDataShield* shield)
{
	// �������������� ��������� �� ��������� ���������� �������
	m_Data	   = shield;	
	// �������� ������ �� ��������, ���������� ���������
	m_ActAVDoc = AVAppGetActiveDoc();
	m_ActPDDoc = AVDocGetPDDoc(m_ActAVDoc);
	// �������� ��������� �� ��������� �������
	m_JobStruct = NULL;
	// �������� ��������� �� ������� �������� ��������� ���������
	m_CurrentDstPDPage = NULL;
	// �������� ��������� �� ������� �������� ��������
	m_DestPDDoc = NULL;
}

CPDFGenerator::~CPDFGenerator(void)
{
}

//------------------------------------------------------------------------
// ����� �������� PDF-����� �� ������� �����
//------------------------------------------------------------------------
void CPDFGenerator::CreatePDF()
{
	// ��������� ��������� ��������
	if (m_DestPDDoc == NULL) m_DestPDDoc = PDDocCreate();
	CalcPlaceholdersMBox();
	CalcCreepArray();
	// ��������� ����� ������ ����������
	if (m_Data->GetOPMode() == BATCH_MODE)
	{
		//-------------------------------------------------------------------
		// �������� ���� ������������� ����� ������ �����
		//-------------------------------------------------------------------
		for (int i = 0; i < m_JobStruct->m_NPlates; i++)
		{
			// � �������� �������� ��������� ����� ��������
			m_CurrentDstPDPage = PDDocCreatePage(m_DestPDDoc, PDBeforeFirstPage, m_Data->GetOutputMediaBox());
			// ������������ ������� ����� ��������
			m_CurrentPlateNum = i;
			// ����������� ��������
			InsertPages();
			// ����������� �����
			if (m_Data->GetPrintCFMarks()) InsertCFMarks();
			if (m_Data->GetPrintRegMarks()) InsertRegMarks();
			if (m_Data->GetPrintColorBar()) InsertTintColorBar();
			if (m_Data->GetPrintProgressiveGray()) InsertPogressiveColorBar();
			// ���� ���������� ���������� - ������������� �������� �� 90 ��������
			if (m_Data->GetMediaOrientation() == PORTRAIT) PDPageSetRotate(m_CurrentDstPDPage, pdRotate90);
		}
		// ��������� �������� ��������
		SaveDSTDocToFile();
		PDDocClose(m_DestPDDoc);
		// �������� ��������� ������� �������� ��������� ��������� � �� ��� ��������
		m_CurrentDstPDPage = NULL;
		m_DestPDDoc = NULL;
	}
	else // �������� � ������ ���������������� ���������
	{
		if (m_CurrentDstPDPage != NULL)
		{
			PDPageRelease(m_CurrentDstPDPage);
			PDDocDeletePages(m_DestPDDoc, 0, 0, NULL, NULL);
		}
		// � �������� �������� ��������� ����� �������� - ��� �� � ������������
		m_CurrentDstPDPage = PDDocCreatePage(m_DestPDDoc, PDBeforeFirstPage, m_Data->GetOutputMediaBox());
		// ������������ ������� ����� ��������
		m_CurrentPlateNum = m_Data->GetCurrentPreviewPN();
		// ����������� ��������
		InsertPages();
		// ����������� ��������������� �����
		if (m_Data->GetPrintCFMarks()) InsertCFMarks();
		if (m_Data->GetPrintRegMarks()) InsertRegMarks();
		if (m_Data->GetPrintColorBar()) InsertTintColorBar();
		if (m_Data->GetPrintProgressiveGray()) InsertPogressiveColorBar();
		// ���� ���������� ���������� - ������������� �������� �� 90 ��������
		if (m_Data->GetMediaOrientation() == PORTRAIT) PDPageSetRotate(m_CurrentDstPDPage, pdRotate90);
		// ���������� ������ �� ������� �������� ��������� ������
		m_Data->SetPreviewPDPage(m_CurrentDstPDPage);
	}
}

//------------------------------------------------------------------------
// ��������� ���������
//------------------------------------------------------------------------
ASFixedMatrix CPDFGenerator::BuildTransitionMatrix()
{
	ASFixedMatrix A;
	// ����������� ����� ������� �� ������� �� ������ �������
	A.a	= fixedOne;
	A.b	= fixedZero;
	A.c	= fixedZero;
	A.d	= fixedOne;
	// ���������� ����� ������� - ������� �� ������ �������
	if(m_MatrixBuildMode == LOCAL)
	{
		ASFixed delta;
		ASFixedRect dest;
		PDRotate rot;
		dest = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][m_CurrentColumn]))[m_CurrentPlaceholder].m_MBox; 
		// �������� ������ ��������
		rot = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][m_CurrentColumn]))[m_CurrentPlaceholder].m_POrt;
		//====================================================================
		// ������ ������� �����������
		//====================================================================
		// ��������� ��������� ������� � ��������
		float ASRatioT = ASFixedToFloat(ASFixedDiv((dest.top - dest.bottom), (dest.right - dest.left)));
		float ASRatioP = ASFixedToFloat(ASFixedDiv((m_PageRect.top - m_PageRect.bottom), (m_PageRect.right - m_PageRect.left)));
		// � ����������� �� ����������� ��������� ���������� �������� ������ ������� ���� ��������
		if(ASRatioT == ASRatioP)
		{
			A.h	= dest.left;
			A.v = dest.bottom;
		}
		else if(ASRatioT > ASRatioP)
		{
			delta	= CalcDelta(dest, m_PageRect);
			A.h		= dest.left;
			A.v		= dest.bottom + delta;
			// ������ �������� �� ��������
			if(rot == pdRotate180)
			{
				A.h += (dest.right - dest.left);
				A.v += (dest.top - dest.bottom) - ASFixedMul(delta, fixedTwo);
			}
			
		}
		else
		{
			delta	= CalcDelta(dest, m_PageRect);
			A.h		= dest.left + delta;
			A.v		= dest.bottom;
			// ������ �������� �� ��������
			if(rot == pdRotate180)
			{
				A.h += (dest.right - dest.left - ASFixedMul(delta, fixedTwo));
				A.v += (dest.top - dest.bottom);
			}
		}
		// ��������� ������ �������� ��� �������� ������ ���������
		if(m_Data->GetInversePrinting())
		{
			rot == pdRotate180 ? A.h -= (dest.right - dest.left) : A.h += (dest.right - dest.left);
		}
		// ��������� creep-������
		//A.h += (*(m_JobStruct->m_Templates[m_CurrentPlateNum][m_CurrentColumn]))[m_CurrentPlaceholder].m_Creep;
	}
	else
	{
		// ������ ������� ����� ��� �������� � ����� � ������ �������� ������������.
		A.h = m_Data->GetXTransT();
		A.v = m_Data->GetYTransT();
	}
	return A;
}

ASFixedMatrix CPDFGenerator::BuildRotationMatrix()
{
	ASFixedMatrix A;
	double angle;
	PDRotate rot;
	// ��������� pi 
	const double PI = 3.1415926535897932384626433832795;
	// ����������� ����� ������� - �� ������� �� ������ �������
	A.h	= fixedZero;
	A.v	= fixedZero;
	// �������� �������� ���� �������� �������� - ������� �� ������ �������
	if(m_MatrixBuildMode == LOCAL)
	{
		rot = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][m_CurrentColumn]))[m_CurrentPlaceholder].m_POrt;
		switch(rot)
		{
		case pdRotate0:		angle = 0; break;
		case pdRotate90:	angle = PI / 2.0; break;
		case pdRotate180:	angle = PI; break;
		case pdRotate270:	angle = 3.0 * PI / 2.0; break; 
		}
	}
	else
	{
		angle = m_Data->GetRAngleT();
	}
	// ��������� �������
	A.a	= ASFloatToFixed(cos(angle));
	A.b	= ASFloatToFixed(sin(angle));
	A.c	= ASFloatToFixed(-sin(angle));
	A.d	= ASFloatToFixed(cos(angle));

	return A;
}

ASFixedMatrix CPDFGenerator::BuildScalingMatrix()
{
	ASFixedMatrix A;
	ASFixedRect dest;
	// �������� �������������� ��� ������� ���������������
	dest = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][m_CurrentColumn]))[m_CurrentPlaceholder].m_MBox;
	// ����������� ����� ������� - �� ������� �� ������ ����������
	A.b	= fixedZero;
	A.c	= fixedZero;
	A.h	= fixedZero;
	A.v	= fixedZero;
	// ���������� ����� ������� - ������� �� ������ ������
	if(m_MatrixBuildMode == LOCAL)
	{
		// ��������� ��������� ������� � ��������
		float ASRatioT = ASFixedToFloat(ASFixedDiv((dest.top - dest.bottom), (dest.right - dest.left)));
		float ASRatioP = ASFixedToFloat(ASFixedDiv((m_PageRect.top - m_PageRect.bottom), (m_PageRect.right - m_PageRect.left)));
		// � ����������� �� ��������� � �� ����������� ��������� ������������ ���������������
		if(ASRatioT <= ASRatioP)
		{
			A.a = A.d = ASFixedDiv((dest.top - dest.bottom), (m_PageRect.top - m_PageRect.bottom));
		}
		else
		{
			A.a	= A.d = ASFixedDiv((dest.right - dest.left), (m_PageRect.right - m_PageRect.left));
		}
		// ��������� ������������� ���������� ������
		if (m_Data->GetInversePrinting()) A.a = ASFixedMul(A.a, -fixedOne);
	}
	else
	{
		A.a = m_Data->GetXScaleT();
		A.d = m_Data->GetYScaleT();
	}

	return A;
}

ASFixedMatrix CPDFGenerator::BuildSkewMatrix()
{
	ASFixedMatrix A;
	// ����������� ����� ������� - �� ������� �� ������ ����������
	A.h	= fixedZero;
	A.v = fixedZero;
	A.a = fixedOne;
	A.d = fixedOne;

	A.b = ASFloatToFixed(tan(m_Data->GetALFASkewT()));
	A.c = ASFloatToFixed(tan(m_Data->GetBETASkewT()));

	return A;
}

ASFixedMatrix CPDFGenerator::BuildTransformMatrix()
{
	// ��������� ������� �����������
	ASFixedMatrix T_M = BuildTransitionMatrix();
	// ��������� ������� ���������������
	ASFixedMatrix S_M = BuildScalingMatrix();
	// ��������� ������� ��������
	ASFixedMatrix R_M = BuildRotationMatrix();
	// ��������� ������� ������
	//ASFixedMatrix K_M = BuildSkewMatrix();
	// ��������� ������� ������������ ��������������
	ASFixedMatrixConcat(&T_M, &T_M, &S_M);
	ASFixedMatrixConcat(&T_M, &T_M, &R_M);
	//ASFixedMatrixConcat(&T_M, &T_M, &K_M);

	return T_M;
}

ASFixed CPDFGenerator::CalcDelta(ASFixedRect dest, ASFixedRect src)
{
	ASFixed delta, scale;
	ASFixedMatrix A;
	ASFixedRect new_rect;	
	// ��������� ��������� ������� � ��������
	float ASRatioT = ASFixedToFloat(ASFixedDiv((dest.top - dest.bottom), (dest.right - dest.left)));
	float ASRatioP = ASFixedToFloat(ASFixedDiv((src.top - src.bottom), (src.right - src.left)));
	if(ASRatioT == ASRatioP) delta = fixedZero;
	else if (ASRatioT > ASRatioP)
	{
		scale = ASFixedDiv((dest.right - dest.left), (src.right - src.left));
		A.a = A.d = scale;
		A.b = A.c = A.h = A.v = fixedZero;
		ASFixedMatrixTransformRect(&new_rect, &A, &src);
		delta = ASFixedDiv(((dest.top - dest.bottom) - (new_rect.top - new_rect.bottom)), fixedTwo);
	}
	else
	{
		scale = ASFixedDiv((dest.top - dest.bottom), (src.top - src.bottom));
		A.a = A.d = scale;
		A.b = A.c = A.h = A.v = fixedZero;
		ASFixedMatrixTransformRect(&new_rect, &A, &src);
		delta = ASFixedDiv(((dest.right - dest.left) - (new_rect.right - new_rect.left)), fixedTwo);
	}

	return delta;
}

void CPDFGenerator::InsertCFMarks()
{
	ASFixedRect m_PL;
	ASFixed x_A, x_B, x_C, x_D, x_E, x_F, x_G, y_A, y_B, y_C, y_D, y_E, y_F, y_G;
	PDEGraphicState g_state;
	// �������� ������� ������� �������� ��������� ���������
	PDEContent content = PDPageAcquirePDEContent(m_CurrentDstPDPage, 0);
	vector<TECH_MARK_SPEC>* m_TechMarks = m_Data->GetTechMarks(); 
	//==================================================================
	// �������� ���� ������������ ����� �����-�������
	//==================================================================
	for (int i = 0; i < m_JobStruct->m_Cols; i++)
	{
		for (int j = 0; j < m_JobStruct->m_Rows; j++)
		{
			m_PL = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][i]))[j].m_MBox;
			for (int k = 0; k < 4; k++)
			{
				int mark_code = i * m_JobStruct->m_Rows * 4 + j * 4 + k;
				// ������� ������ �����
				if ((*m_TechMarks)[mark_code].m_MarkIsActive)
				{
					// ����������� ������� ����� ������
					ASFixed m_Mark_W = (*m_TechMarks)[mark_code].m_TM_HLen + (*m_TechMarks)[mark_code].m_TM_HPageGAP;
					ASFixed m_Mark_H = (*m_TechMarks)[mark_code].m_TM_VLen + (*m_TechMarks)[mark_code].m_TM_VPageGAP;
					// ���� ����� ������� - ������ �� � ������� ��������
					PDEPath mark = PDEPathCreate();
					// ��������� ���������� ������� ����� �����
					x_A = fixedZero;
					y_A = fixedZero;
					x_B = (*m_TechMarks)[mark_code].m_TM_HLen;
					y_B = fixedZero;
					x_C = m_Mark_W;
					y_C = (*m_TechMarks)[mark_code].m_TM_VPageGAP;
					x_D = x_C;
					y_D = m_Mark_H;
					x_E = (*m_TechMarks)[mark_code].m_TM_HPageGAP - (*m_TechMarks)[mark_code].m_TM_HInset;
					y_E = (*m_TechMarks)[mark_code].m_TM_VInset;
					x_F = m_Mark_W - (*m_TechMarks)[mark_code].m_TM_HInset;
					y_F = y_E;
					x_G = x_F;
					y_G = y_F + (*m_TechMarks)[mark_code].m_TM_VLen;
					// ��������� ���������� �����
					PDEPathAddSegment(mark, kPDEMoveTo, x_A, y_A, 0, 0, 0, 0);
					PDEPathAddSegment(mark, kPDELineTo, x_B, y_B, 0, 0, 0, 0);
					PDEPathAddSegment(mark, kPDEMoveTo, x_C, y_C, 0, 0, 0, 0);
					PDEPathAddSegment(mark, kPDELineTo, x_D, y_D, 0, 0, 0, 0);
					//----------------------------------------------------------------
					// ������
					//----------------------------------------------------------------
					//PDEPathAddSegment(mark, kPDEMoveTo, x_E, y_E, 0, 0, 0, 0);
					//PDEPathAddSegment(mark, kPDELineTo, x_F, y_F, 0, 0, 0, 0);
					//PDEPathAddSegment(mark, kPDELineTo, x_G, y_G, 0, 0, 0, 0);
					//----------------------------------------------------------------
					// ������������� ��� ���������� ��� �����
					PDEPathSetPaintOp(mark, kPDEStroke);
					// ����������� ����������� ����� �����
					memset (&g_state, 0, sizeof(PDEGraphicState));
					PDEColorSpace c_space			= PDEColorSpaceCreateFromName(ASAtomFromString("DeviceCMYK"));
					PDEExtGState pdeExtGState		= NULL;
					pdeExtGState					= PDEExtGStateCreateNew(PDDocGetCosDoc(m_DestPDDoc));
					PDEExtGStateSetOpacityFill(pdeExtGState, fixedOne);
					PDEExtGStateSetOpacityStroke(pdeExtGState, fixedOne);
					g_state.miterLimit				= fixedTen;
					g_state.flatness				= fixedOne;
					g_state.lineWidth				= ASFloatToFixed(0.1);
					g_state.extGState				= pdeExtGState;
					g_state.fillColorSpec.space				= c_space;
					g_state.fillColorSpec.value.color[0]	= fixedZero;
					g_state.fillColorSpec.value.color[1]	= g_state.fillColorSpec.value.color[2] = g_state.fillColorSpec.value.color[3] = fixedZero;
					g_state.strokeColorSpec.space			= c_space;
					g_state.strokeColorSpec.value.color[0]	= fixedOne;
					g_state.strokeColorSpec.value.color[1]	= g_state.strokeColorSpec.value.color[2] = g_state.strokeColorSpec.value.color[3] = fixedOne;
					g_state.wasSetFlags						= kPDEMiterLimitWasSet | kPDEFlatnessWasSet | kPDELineWidthWasSet | kPDEExtGStateWasSet |
						kPDEFillCSpaceWasSet | kPDEFillCValueWasSet | kPDEStrokeCSpaceWasSet | kPDEStrokeCValueWasSet;
					PDEElementSetGState((PDEElement)mark, &g_state, sizeof(g_state));
					// ����������� ������� ���������������� ��� �����
					ASFixedMatrix T, M, R;
					T.a = T.d = fixedOne;
					T.b = T.c = fixedZero;
					switch(k)
					{
					case 0:
						{
							T.h = m_PL.left - m_Mark_W;
							T.v = m_PL.top;
						}
						break;
					case 1:
						{
							T.h = m_PL.left;
							T.v = m_PL.bottom - m_Mark_H;
						}
						break;
					case 2:
						{
							T.h = m_PL.right + m_Mark_W;
							T.v = m_PL.bottom;
						}
						break;
					case 3:
						{
							T.h = m_PL.right;
							T.v = m_PL.top + m_Mark_H;
						}
						break;
					}
					// ����������� ������� ��������
					double angle;
					PDRotate rot;
					// ��������� pi 
					const double PI = 3.1415926535897932384626433832795;
					// ����������� ����� ������� - �� ������� �� ������ �������
					R.h	= fixedZero;
					R.v	= fixedZero;
					// �������� �������� ���� �������� �������� - ������� �� ������ �������
					rot = (*m_TechMarks)[mark_code].m_TM_Orientation;
					switch(rot)
					{
					case pdRotate0:		angle = 0; break;
					case pdRotate90:	angle = PI/ 2.0; break;
					case pdRotate180:	angle = PI; break;
					case pdRotate270:	angle = 3.0 * PI / 2.0; break; 
					}
					R.a	= ASFloatToFixed(cos(angle));
					R.b	= ASFloatToFixed(sin(angle));
					R.c	= ASFloatToFixed(-sin(angle));
					R.d	= ASFloatToFixed(cos(angle));


					// ����������� ������� �������������� �����
					PDEElementGetMatrix((PDEElement)mark, &M);
					ASFixedMatrixConcat(&M, &M, &T);
					ASFixedMatrixConcat(&M, &M, &R);
					PDEElementSetMatrix((PDEElement)mark, &M);

					// ��������� ����� � ������� ��������
					PDEContentAddElem(content, kPDEAfterLast, (PDEElement)mark);
				}
			}
		}
	}
	// ��������� ������� ������� ��������
	PDPageSetPDEContent(m_CurrentDstPDPage, 0);
	// ����������� �������
	PDPageReleasePDEContent(m_CurrentDstPDPage, 0);
}

void CPDFGenerator::InsertRegMarks()
{
	// �������� ��������������� ����� � ������� 
	int m_Offset = m_JobStruct->m_Cols * m_JobStruct->m_Rows * 4;
	vector<TECH_MARK_SPEC>* m_TechMarks = m_Data->GetTechMarks(); 

	// �������� ������� ������� �������� ��������� ���������
	PDEContent content = PDPageAcquirePDEContent(m_CurrentDstPDPage, 0);
	// ����������� ��������� �����
	PDEGraphicState g_state;
	// ��������������� ���������
	ASFixed A, B, C;
	ASFixed x_A, y_A, x_B, y_B, x_C, y_C, x_D, y_D;
	ASFixed x_E, y_E, x_F, y_F, x_1, y_1, x_2, y_2;
	//==================================================================
	// �������� ���� ������������ ����� �����������
	//==================================================================
	for (int i = 0; i < 4; i++)
	{
		memset (&g_state, 0, sizeof(PDEGraphicState));
		PDEColorSpace c_space			= PDEColorSpaceCreateFromName(ASAtomFromString("DeviceCMYK"));
		PDEExtGState pdeExtGState		= NULL;
		pdeExtGState					= PDEExtGStateCreateNew(PDDocGetCosDoc(m_DestPDDoc));
		PDEExtGStateSetOpacityFill(pdeExtGState, fixedOne);
		PDEExtGStateSetOpacityStroke(pdeExtGState, fixedOne);
		g_state.miterLimit				= fixedTen;
		g_state.flatness				= fixedOne;
		g_state.lineWidth				= ASFloatToFixed(0.1);
		g_state.extGState				= pdeExtGState;
		g_state.wasSetFlags				= kPDEMiterLimitWasSet | kPDEFlatnessWasSet | kPDELineWidthWasSet | kPDEExtGStateWasSet |
			kPDEFillCSpaceWasSet | kPDEFillCValueWasSet | kPDEStrokeCSpaceWasSet | kPDEStrokeCValueWasSet;
		g_state.fillColorSpec.space				= c_space;
		g_state.fillColorSpec.value.color[0]	= fixedOne;
		g_state.fillColorSpec.value.color[1]	= g_state.fillColorSpec.value.color[2] = g_state.fillColorSpec.value.color[3] = fixedOne;
		g_state.strokeColorSpec.space			= c_space;
		g_state.strokeColorSpec.value.color[0]	= fixedOne;
		g_state.strokeColorSpec.value.color[1]	= g_state.strokeColorSpec.value.color[2] = g_state.strokeColorSpec.value.color[3] = fixedOne;

		PDEPath cross = PDEPathCreate();
		PDEPath i_crl = PDEPathCreate();
		PDEPath o_crl = PDEPathCreate();

		int mark_code = m_Offset + i;
		if ((*m_TechMarks)[mark_code].m_MarkIsActive)
		{
			// ��������� ��������������� ���������
			A = ASFixedDiv((*m_TechMarks)[mark_code].m_TM_InnerCircleD, fixedTwo);
			B = ASFixedDiv((*m_TechMarks)[mark_code].m_TM_OuterCircleD, fixedTwo);
			C = ASFixedDiv((*m_TechMarks)[mark_code].m_TM_CrosshairLen, fixedTwo);
			//------------------------------------------------------------------------------------
			// ������ �����������
			//------------------------------------------------------------------------------------
			x_A = -C;
			y_A = fixedZero;
			x_B = C;
			y_B = fixedZero;
			x_C = fixedZero;
			y_C = C;
			x_D = fixedZero;
			y_D = -C;
			PDEPathAddSegment(cross, kPDEMoveTo, x_A, y_A, 0, 0, 0, 0);
			PDEPathAddSegment(cross, kPDELineTo, x_B, y_B, 0, 0, 0, 0);
			PDEPathAddSegment(cross, kPDEMoveTo, x_C, y_C, 0, 0, 0, 0);
			PDEPathAddSegment(cross, kPDELineTo, x_D, y_D, 0, 0, 0, 0);
			PDEPathSetPaintOp(cross, kPDEStroke);
			PDEElementSetGState((PDEElement)cross, &g_state, sizeof(g_state));
			//------------------------------------------------------------------------------------
			// ��������� �������� ����������
			//------------------------------------------------------------------------------------
			x_E = -B;
			y_E = fixedZero;
			x_F = B;
			y_F = fixedZero;
			// ����������� �����
			x_1 = x_E;
			y_1 = ASFixedMul(ASFloatToFixed(1.3), B);
			x_2 = x_F;
			y_2 = y_1;
			PDEPathAddSegment(o_crl, kPDEMoveTo, x_E, y_E, 0, 0, 0, 0);
			PDEPathAddSegment(o_crl, kPDECurveTo, x_1, y_1, x_2, y_2, x_F, y_F);
			y_1 = -ASFixedMul(ASFloatToFixed(1.3), B);
			y_2 = y_1;
			PDEPathAddSegment(o_crl, kPDEMoveTo, x_F, y_F, 0, 0, 0, 0);
			PDEPathAddSegment(o_crl, kPDECurveTo, x_2, y_2, x_1, y_1, x_E, y_E);
			PDEPathSetPaintOp(o_crl, kPDEStroke);
			PDEElementSetGState((PDEElement)o_crl, &g_state, sizeof(g_state));
			//------------------------------------------------------------------------------------
			// ��������� ���������� ����
			//------------------------------------------------------------------------------------
			x_E = -A;
			y_E = fixedZero;
			x_F = A;
			y_F = fixedZero;
			// ����������� �����
			x_1 = x_E;
			y_1 = ASFixedMul(ASFloatToFixed(1.3), A);
			x_2 = x_F;
			y_2 = y_1;
			PDEPathAddSegment(i_crl, kPDEMoveTo, x_E, y_E, 0, 0, 0, 0);
			PDEPathAddSegment(i_crl, kPDECurveTo, x_1, y_1, x_2, y_2, x_F, y_F);
			y_1 = -ASFixedMul(ASFloatToFixed(1.3), A);
			y_2 = y_1;
			PDEPathAddSegment(i_crl, kPDEMoveTo, x_F, y_F, 0, 0, 0, 0);
			PDEPathAddSegment(i_crl, kPDECurveTo, x_2, y_2, x_1, y_1, x_E, y_E);
			PDEPathSetPaintOp(i_crl, kPDEFill);
			PDEElementSetGState((PDEElement)i_crl, &g_state, sizeof(g_state));
			
			// ��������� ������� ��������������
			ASFixedMatrix T, M;
			T.a = T.d = fixedOne;
			T.b = T.c = fixedZero;
			T.h = (*m_TechMarks)[mark_code].m_TM_Origin_X;
			T.v = (*m_TechMarks)[mark_code].m_TM_Origin_Y;

			PDEElementGetMatrix((PDEElement)cross, &M);
			ASFixedMatrixConcat(&M, &M, &T);
			PDEElementSetMatrix((PDEElement)cross, &M);
			PDEContentAddElem(content, kPDEAfterLast, (PDEElement)cross);

			PDEElementGetMatrix((PDEElement)o_crl, &M);
			ASFixedMatrixConcat(&M, &M, &T);
			PDEElementSetMatrix((PDEElement)o_crl, &M);
			PDEContentAddElem(content, kPDEAfterLast, (PDEElement)o_crl);

			PDEElementGetMatrix((PDEElement)i_crl, &M);
			ASFixedMatrixConcat(&M, &M, &T);
			PDEElementSetMatrix((PDEElement)i_crl, &M);
			PDEContentAddElem(content, kPDEAfterLast, (PDEElement)i_crl);
		}
	}

	// ��������� ������� ������� ��������
	PDPageSetPDEContent(m_CurrentDstPDPage, 0);
	// ����������� �������
	PDPageReleasePDEContent(m_CurrentDstPDPage, 0);
}

void CPDFGenerator::InsertTintColorBar()
{
	int m_Offset = m_JobStruct->m_Cols * m_JobStruct->m_Rows * 4 + 4;
	vector<TECH_MARK_SPEC>* m_TechMarks = m_Data->GetTechMarks(); 

	PDEPath tint_tbl[12];
	for (int i = 0; i < 12; i++) tint_tbl[i] = PDEPathCreate(); 

	// �������� ������� ������� �������� ��������� ���������
	PDEContent content = PDPageAcquirePDEContent(m_CurrentDstPDPage, 0);

	// ����������� ��������� ����� (����������� �����)
	PDEGraphicState g_state;
	memset (&g_state, 0, sizeof(PDEGraphicState));
	PDEColorSpace c_space			= PDEColorSpaceCreateFromName(ASAtomFromString("DeviceCMYK"));
	PDEExtGState pdeExtGState		= NULL;
	pdeExtGState					= PDEExtGStateCreateNew(PDDocGetCosDoc(m_DestPDDoc));
	PDEExtGStateSetOpacityFill(pdeExtGState, fixedOne);
	PDEExtGStateSetOpacityStroke(pdeExtGState, fixedOne);
	g_state.miterLimit				= fixedTen;
	g_state.flatness				= fixedOne;
	g_state.lineWidth				= ASFloatToFixed(0.1);
	g_state.extGState				= pdeExtGState;
	g_state.wasSetFlags				= kPDEMiterLimitWasSet | kPDEFlatnessWasSet | kPDELineWidthWasSet | kPDEExtGStateWasSet |
									  kPDEFillCSpaceWasSet | kPDEFillCValueWasSet | kPDEStrokeCSpaceWasSet | kPDEStrokeCValueWasSet;
	g_state.fillColorSpec.space		= c_space;
	g_state.strokeColorSpec.space	= c_space;
	// ��������������� ���������
	ASFixed x_A, y_A, x_1, y_1;

	// ������� �������������� ������
	double C[12] = {1.0, 0.60, 0.30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double M[12] = {0.0, 0.0, 0.0, 1.0, 0.60, 0.30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double Y[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.60, 0.30, 0.0, 0.0, 0.0};
	double K[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.60, 0.30};

	// ������� ��������������
	ASFixedMatrix T, MT;
	T.a = T.d = fixedOne;
	T.b = T.c = fixedZero;
	T.h = (*m_TechMarks)[m_Offset].m_TM_Origin_X;
	T.v = (*m_TechMarks)[m_Offset].m_TM_Origin_Y;

	// ��������� ���������� �����
	if ((*m_TechMarks)[m_Offset].m_MarkIsActive)
	{
		for (int i = 0; i < 12; i++)
		{
			x_A = fixedZero;
			y_A = -ASFixedMul(ASInt32ToFixed(i + 1), (*m_TechMarks)[m_Offset].m_TM_SquareSize);
			x_1 = y_1 = (*m_TechMarks)[m_Offset].m_TM_SquareSize;
			PDEPathAddSegment(tint_tbl[i], kPDERect, x_A, y_A, x_1, y_1, 0, 0);
			// ��������� ���������� ����� ������������ ���������
			g_state.fillColorSpec.value.color[0]	= ASFloatToFixed(C[i]);
			g_state.fillColorSpec.value.color[1]	= ASFloatToFixed(M[i]);
			g_state.fillColorSpec.value.color[2]	= ASFloatToFixed(Y[i]);
			g_state.fillColorSpec.value.color[3]	= ASFloatToFixed(K[i]);
			g_state.strokeColorSpec.value.color[0]	= ASFloatToFixed(C[i]);
			g_state.strokeColorSpec.value.color[1]	= ASFloatToFixed(M[i]);
			g_state.strokeColorSpec.value.color[2]	= ASFloatToFixed(Y[i]);
			g_state.strokeColorSpec.value.color[3]	= ASFloatToFixed(K[i]);
			PDEPathSetPaintOp(tint_tbl[i], kPDEFill | kPDEStroke);
			PDEElementSetGState((PDEElement)tint_tbl[i], &g_state, sizeof(g_state));
			// ������������� ������ ���������
			PDEElementGetMatrix((PDEElement)tint_tbl[i], &MT);
			ASFixedMatrixConcat(&MT, &MT, &T);
			PDEElementSetMatrix((PDEElement)tint_tbl[i], &MT);
			PDEContentAddElem(content, kPDEAfterLast, (PDEElement)tint_tbl[i]);
		}
	}

	// ��������� ������� ������� ��������
	PDPageSetPDEContent(m_CurrentDstPDPage, 0);
	// ����������� �������
	PDPageReleasePDEContent(m_CurrentDstPDPage, 0);
}

void CPDFGenerator::InsertPogressiveColorBar()
{
	int m_Offset = m_JobStruct->m_Cols * m_JobStruct->m_Rows * 4 + 5;
	vector<TECH_MARK_SPEC>* m_TechMarks = m_Data->GetTechMarks(); 

	PDEPath progressive_tbl[10];
	for (int i = 0; i < 9; i++) progressive_tbl[i] = PDEPathCreate(); 

	// �������� ������� ������� �������� ��������� ���������
	PDEContent content = PDPageAcquirePDEContent(m_CurrentDstPDPage, 0);

	// ����������� ��������� ����� (����������� �����)
	PDEGraphicState g_state;
	memset (&g_state, 0, sizeof(PDEGraphicState));
	PDEColorSpace c_space			= PDEColorSpaceCreateFromName(ASAtomFromString("DeviceCMYK"));
	PDEExtGState pdeExtGState		= NULL;
	pdeExtGState					= PDEExtGStateCreateNew(PDDocGetCosDoc(m_DestPDDoc));
	PDEExtGStateSetOpacityFill(pdeExtGState, fixedOne);
	PDEExtGStateSetOpacityStroke(pdeExtGState, fixedOne);
	g_state.miterLimit				= fixedTen;
	g_state.flatness				= fixedOne;
	g_state.lineWidth				= ASFloatToFixed(0.1);
	g_state.extGState				= pdeExtGState;
	g_state.wasSetFlags				= kPDEMiterLimitWasSet | kPDEFlatnessWasSet | kPDELineWidthWasSet | kPDEExtGStateWasSet |
		kPDEFillCSpaceWasSet | kPDEFillCValueWasSet | kPDEStrokeCSpaceWasSet | kPDEStrokeCValueWasSet;
	g_state.fillColorSpec.space		= c_space;
	g_state.strokeColorSpec.space	= c_space;
	// ��������������� ���������
	ASFixed x_A, y_A, x_1, y_1;

	// ������ ��������� ������
	double I[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

	// ������� ��������������
	ASFixedMatrix T, M;
	T.a = T.d = fixedOne;
	T.b = T.c = fixedZero;
	T.h = (*m_TechMarks)[m_Offset].m_TM_Origin_X;
	T.v = (*m_TechMarks)[m_Offset].m_TM_Origin_Y;

	// ��������� ���������� �����
	if ((*m_TechMarks)[m_Offset].m_MarkIsActive)
	{
		for (int i = 0; i < 9; i++)
		{
			x_A = fixedZero;
			y_A = -ASFixedMul(ASInt32ToFixed(i + 1), (*m_TechMarks)[m_Offset].m_TM_SquareSize);
			x_1 = y_1 = (*m_TechMarks)[m_Offset].m_TM_SquareSize;
			PDEPathAddSegment(progressive_tbl[i], kPDERect, x_A, y_A, x_1, y_1, 0, 0);
			// ��������� ���������� ����� ������������ ���������
			g_state.fillColorSpec.value.color[0]	= ASFloatToFixed(I[i]);
			g_state.fillColorSpec.value.color[1]	= ASFloatToFixed(I[i]);
			g_state.fillColorSpec.value.color[2]	= ASFloatToFixed(I[i]);
			g_state.fillColorSpec.value.color[3]	= ASFloatToFixed(I[i]);
			g_state.strokeColorSpec.value.color[0]	= ASFloatToFixed(I[i]);
			g_state.strokeColorSpec.value.color[1]	= ASFloatToFixed(I[i]);
			g_state.strokeColorSpec.value.color[2]	= ASFloatToFixed(I[i]);
			g_state.strokeColorSpec.value.color[3]	= ASFloatToFixed(I[i]);
			PDEPathSetPaintOp(progressive_tbl[i], kPDEFill | kPDEStroke);
			PDEElementSetGState((PDEElement)progressive_tbl[i], &g_state, sizeof(g_state));
			// ������������� ������ ���������
			PDEElementGetMatrix((PDEElement)progressive_tbl[i], &M);
			ASFixedMatrixConcat(&M, &M, &T);
			PDEElementSetMatrix((PDEElement)progressive_tbl[i], &M);
			PDEContentAddElem(content, kPDEAfterLast, (PDEElement)progressive_tbl[i]);
		}
	}

	// ��������� ������� ������� ��������
	PDPageSetPDEContent(m_CurrentDstPDPage, 0);
	// ����������� �������
	PDPageReleasePDEContent(m_CurrentDstPDPage, 0);
}

void CPDFGenerator::UT_PDEFormSetContent(PDEContent pdeContent, CosDoc cosDoc, ASFixedRect* bBox, ASFixedMatrixP elMatrix, PDEForm* pForm)
{
	CosObj cosStream, cosResources;
	PDEContentAttrs attrs;

	if (pForm == NULL) return ;

	ASFixedMatrix matrix;
	PDPageGetDefaultMatrix(m_CurrentSrcPDPage, &matrix);
	// Add Form xOblect Attributes Matrix
	attrs.bbox = *bBox;
	attrs.flags = kPDEFormMatrix;
	attrs.formType = 1;
	attrs.matrix.a = matrix.a;
	attrs.matrix.b = matrix.b;
	attrs.matrix.c = matrix.c;
	attrs.matrix.d = matrix.d;
	attrs.matrix.h = matrix.h;
	attrs.matrix.v = matrix.v;

	PDEContentToCosObj (pdeContent, kPDEContentToForm, &attrs, sizeof(attrs), cosDoc, NULL, &cosStream, &cosResources);
	*pForm = PDEFormCreateFromCosObj(&cosStream, &cosResources, elMatrix);
}

void CPDFGenerator::InsertPages()
{
	PDEContent content = PDPageAcquirePDEContent(m_CurrentDstPDPage, 0);
	// ��� ������� �������� ��������� �����
	// ������������� ����� ���������� ������� �������������
	m_MatrixBuildMode = LOCAL;
	for (int i = 0; i < m_JobStruct->m_Rows; i++)
	{
		for (int j = 0; j < m_JobStruct->m_Cols; j++)
		{
			m_CurrentPlaceholder = i;
			m_CurrentColumn = j;
			// �������� ������ �������� ��������������� ���������
			int m_PAGE = (*((m_JobStruct->m_Templates[m_CurrentPlateNum])[m_CurrentColumn]))[m_CurrentPlaceholder].m_PRNum;
			if (m_PAGE != -1)
			{
				m_CurrentSrcPDPage = PDDocAcquirePage(m_ActPDDoc, m_PAGE);
				// �������� ��������� ���������� ��������
				if (!PDPageGetBox(m_CurrentSrcPDPage, ASAtomFromString("CropBox"), &m_PageRect))
				{
					PDPageGetBox(m_CurrentSrcPDPage, ASAtomFromString("MediaBox"), &m_PageRect);
				}
				// �������� ������� ��������������
				ASFixedMatrix m_TM = BuildTransformMatrix();
				// ������� XForm
				PDEForm x_form;
				PDEContent src_content = PDPageAcquirePDEContent(m_CurrentSrcPDPage, 0);
				// ��������� � ����� �������� ��������� ���������
				ASInt32 docFlags = PDDocGetFlags(m_ActPDDoc);
				UT_PDEFormSetContent(src_content, PDDocGetCosDoc(m_ActPDDoc), &m_PageRect, &m_TM, &x_form);
				PDDocClearFlags(m_ActPDDoc, PDDocGetFlags(m_ActPDDoc));
				PDDocSetFlags(m_ActPDDoc, docFlags);

				// ��������� ����� � ������� ��������
				PDEContentAddElem(content, kPDEAfterLast, (PDEElement)x_form);
				// ����������� �������
				PDPageReleasePDEContent(m_CurrentSrcPDPage, 0);
				PDPageRelease(m_CurrentSrcPDPage);
				memset(&m_PageRect, 0, sizeof(ASFixedRect));
				memset(&m_TM, 0, sizeof(ASFixedMatrix));
			}
			// � ������, ���� �������� � ������ ���������������� ��������� - 
			// ������������ �������������� ������ �������
			if (m_Data->GetOPMode() == PREVIEW_MODE)
			{
				//------------------------------------------------------------
				// ������������ ������������� ������ ������������
				//------------------------------------------------------------
				PDEPath block = PDEPathCreate();
				PDEGraphicState g_state;
				memset (&g_state, 0, sizeof(PDEGraphicState));
				PDEColorSpace c_space			= PDEColorSpaceCreateFromName(ASAtomFromString("DeviceCMYK"));
				PDEExtGState pdeExtGState		= NULL;
				pdeExtGState					= PDEExtGStateCreateNew(PDDocGetCosDoc(m_DestPDDoc));
				PDEExtGStateSetOpacityFill(pdeExtGState, fixedOne);
				PDEExtGStateSetOpacityStroke(pdeExtGState, fixedOne);
				g_state.miterLimit				= fixedTen;
				g_state.flatness				= fixedOne;
				g_state.lineWidth				= ASFloatToFixed(0.1);
				g_state.extGState				= pdeExtGState;
				g_state.wasSetFlags				= kPDEMiterLimitWasSet | kPDEFlatnessWasSet | kPDELineWidthWasSet | kPDEExtGStateWasSet |
					kPDEFillCSpaceWasSet | kPDEFillCValueWasSet | kPDEStrokeCSpaceWasSet | kPDEStrokeCValueWasSet;
				g_state.fillColorSpec.space		= c_space;
				g_state.strokeColorSpec.space	= c_space;

				ASFixed or_X, or_Y, width, height;
				or_X = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.left;
				or_Y = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.bottom;
				width = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.right - (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.left;
				height = (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.top - (*(m_JobStruct->m_Templates[m_CurrentPlateNum][j]))[i].m_MBox.bottom;

				PDEPathAddSegment(block, kPDERect, or_X, or_Y, width, height, 0, 0);

				// ��������� ���������� ����� ������������ ���������
				g_state.fillColorSpec.value.color[0]	= ASFloatToFixed(1);
				g_state.fillColorSpec.value.color[1]	= ASFloatToFixed(1);
				g_state.fillColorSpec.value.color[2]	= ASFloatToFixed(1);
				g_state.fillColorSpec.value.color[3]	= ASFloatToFixed(1);
				g_state.strokeColorSpec.value.color[0]	= ASFloatToFixed(1);
				g_state.strokeColorSpec.value.color[1]	= ASFloatToFixed(1);
				g_state.strokeColorSpec.value.color[2]	= ASFloatToFixed(1);
				g_state.strokeColorSpec.value.color[3]	= ASFloatToFixed(1);

				PDEPathSetPaintOp(block, kPDEStroke);
				PDEElementSetGState((PDEElement)block, &g_state, sizeof(g_state));
				PDEContentAddElem(content, kPDEAfterLast, (PDEElement)block);
			}
		}
	}
	// ���������� ������� ����������� ��������
	PDPageSetPDEContent(m_CurrentDstPDPage, 0);
	// ����������� �������
	PDPageReleasePDEContent(m_CurrentDstPDPage, 0);

}

void CPDFGenerator::SaveDSTDocToFile()
{
	CString m_FName = m_Data->GetOutputFileName();  
	PDDocSave(m_DestPDDoc, PDSaveFull, ASPathFromPlatformPath((void *)((LPCTSTR)m_FName)), NULL, NULL, NULL);
}

void CPDFGenerator::CalcPlaceholdersMBox()
{
	// ���������� ��������������� ��������� ��� ������� - ������ � ������ ������� ������
	ASFixed m_PA_W = m_Data->GetPrintingArea().right - m_Data->GetPrintingArea().left;
	ASFixed m_PA_H = m_Data->GetPrintingArea().top - m_Data->GetPrintingArea().bottom;
	// ���������� ������ � ������ ������� �������
	ASFixed m_PL_W = ASFixedDiv((m_PA_W - ASFixedMul(ASFloatToFixed(m_Data->GetNCols() - 1), m_Data->GetPlaceholderGap())), ASFloatToFixed(m_Data->GetNCols()));
	ASFixed m_PL_H = ASFixedDiv((m_PA_H - ASFixedMul(ASFloatToFixed(m_Data->GetNRows() - 1), m_Data->GetTAMWidth())), ASFloatToFixed(m_Data->GetNRows()));
	// ������ ������� ������� ������� ��� ���� �������.
	for (int i = 0; i < m_JobStruct->m_NPlates; i++)
	{
		// ����������� ���������� ����� ������ �� ������� ������� ������ ������
		for (int m = 0; m < m_Data->GetNCols(); m++)
		{
			for (int n = 0; n < m_Data->GetNRows(); n++)
			{
				(*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.top    = m_Data->GetPrintingArea().top - ASFixedMul(ASFloatToFixed(n), (m_Data->GetTAMWidth() + m_PL_H));
				(*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.bottom = (*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.top - m_PL_H;
				(*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.left   = m_Data->GetPrintingArea().left + ASFixedMul(ASFloatToFixed(m), (m_Data->GetPlaceholderGap() + m_PL_W));
				(*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.right  = (*(m_JobStruct->m_Templates[i][m]))[n].m_MBox.left + m_PL_W;
			}
		}  
	}
}
