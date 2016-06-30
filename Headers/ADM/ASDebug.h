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

#ifndef __ASDebug__
#define __ASDebug__

	// ASAPI
#include "ASTypes.h"


#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


#ifdef __cplusplus
extern "C" {
#endif



// -----------------------------------------------------------------------------

// ASDebugAction enum defines what to do when an exception or assertion happens.

typedef enum {
	kASDebugAction_Nothing				= 0,
 	kASDebugAction_Alert				= 1,
	kASDebugAction_LowLevelDebugger		= 2,
	kASDebugAction_SourceDebugger		= 3,
	kASDebugAction_Log					= 4,
	kASDebugAction_DummyAction			= 0xFFFFFFFF
} ASDebugAction;


// =============================================================================
//		* ASDebugSuite
// =============================================================================

#define kASDebugSuite			"AS Debug Suite"
#define kASDebugSuiteVersion1	1

// -----------------------------------------------------------------------------

typedef struct
{

	// debugging traps
	
	void ASAPI (*RaiseSignal)(const char* inMessage, const char* inPlugin,
				const char* inFile, ASUInt32 inLineNumber);

	void ASAPI (*AboutToThrow)(const char* inMessage, const char* inPlugin,
				const char* inFile, ASUInt32 inLineNumber);

	// debugging log

	void ASAPI (*LogMessage)(const char* inMessage);
	void ASAPI (*IndentLog)();
	void ASAPI (*UnindentLog)();

	// debugging behavior control
	
	ASDebugAction ASAPI (*GetSignalAction)();
	void ASAPI (*SetSignalAction)(ASDebugAction inSignalAction);
	
	ASDebugAction ASAPI (*GetThrowAction)();
	void ASAPI (*SetThrowAction)(ASDebugAction inThrowAction);
	
	// debugging message dialog

	void ASAPI (*ShowDebugAlert)(const char* inHeading, const char* inMessage,
				const char* inPlugin, const char* inFile, ASUInt32 inLineNumber);

} ASDebugSuite1;


// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
