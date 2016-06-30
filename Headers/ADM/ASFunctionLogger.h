/***********************************************************************/
/*                                                                     */
/* Copyright 1996-2003 Adobe Systems Incorporated.                     */
/* All Rights Reserved.                                                */
/*                                                                     */
/* Patents Pending                                                     */
/*                                                                     */
/* NOTICE: All information contained herein is the property of Adobe   */
/* Systems Incorporated. Many of the intellectual and technical        */
/* concepts contained herein are proprietary to Adobe, are protected   */
/* as trade secrets, and are made available only to Adobe licensees    */
/* for their internal use. Any reproduction or dissemination of this   */
/* software is strictly forbidden unless prior written permission is   */
/* obtained from Adobe.                                                */
/*                                                                     */
/***********************************************************************/

#pragma once

#ifndef __ASFunctionLogger__
#define __ASFunctionLogger__

	// ASAPI
#include "ASTypes.h"


#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


#ifdef __cplusplus
extern "C" {
#endif



// =============================================================================
//		* ASFunctionLoggerSuite
// =============================================================================

#define kASFunctionLoggerSuite			"AS Function Logger Suite"
#define kASFunctionLoggerSuiteVersion1	1

// -----------------------------------------------------------------------------

typedef struct
{

	void ASAPI (*EnterFunction)(const char* inSuiteName, const char* inFunctionName);
	void ASAPI (*ExitFunction)(const char* inSuiteName, const char* inFunctionName);
	void ASAPI (*FunctionParameter)(const char* inParamName, const char* inParamValue);

} ASFunctionLoggerSuite1;


// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
