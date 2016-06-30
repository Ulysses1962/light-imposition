/***********************************************************************/
/*                                                                     */
/* ADMHostWin.h                                                        */
/* ADM Host Window Suite                                               */
/*                                                                     */
/* Copyright 1998-2003 Adobe Systems Incorporated.                     */
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
/*                                                                     */
/***********************************************************************/

#ifndef __ADMHostWin__
#define __ADMHostWin__


#ifndef __ADMDialog__
#include "ADMDialog.h"
#endif

// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Host Window Suite
// =============================================================================
//	The host of ADM provides this suite for ADM as the window handler for
//	palettes. ADM will draw into a panel provided by the host.

#define kADMHostWindowSuite "ADM Host Window Suite"
#define kADMHostWindowSuiteVersion1 1
#define kADMHostWindowSuiteVersion kADMHostWindowSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMHostWindowSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	void ASAPI (*GetWindowRef)(SPPluginRef inPluginRef, ASWindowRef* inWindow,
				ADMDialogStyle inStyle);

	void ASAPI (*ShowWindow)(SPPluginRef inPluginRef, ASWindowRef* inWindow,
				ASBoolean inShowState);

	// Also need others depending on feedback. Hide, create, destroy, activate?
	// Separate from SetProcs?
	
}
ADMHostWindowSuite1;

typedef ADMHostWindowSuite1 ADMHostWindowSuite;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHostWin__
