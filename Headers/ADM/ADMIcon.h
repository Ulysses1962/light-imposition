/***********************************************************************/
/*                                                                     */
/* ADMIcon.h                                                           */
/* ADM Icon Suite                                                      */
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
/*                                                                     */
/***********************************************************************/

#ifndef __ADMIcon__
#define __ADMIcon__

#ifndef __ADMTypes__
#include "ADMTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// -----------------------------------------------------------------------------
//	Icon types

typedef enum {

		// Mac types
	kCICN, kPICT, kIconSuite,
	
		// Windows types
	kWinIcon, kBMP,

		// Either type
	kADMImageIcon,
	kUnknown

}
ADMIconType;


// =============================================================================
//		* ADM Icon Suite
// =============================================================================

#define kADMIconSuite				"ADM Icon Suite"
#define kADMIconSuiteVersion2		2

// -----------------------------------------------------------------------------

typedef struct ADMIconSuite2
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// icon creation/destruction

	ADMIconRef ASAPI (*GetFromResource)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inIconID, ASInt32 inIconIndex);

	ADMIconRef ASAPI (*Create)(ADMIconType inIconType, ASInt32 inWidth,
				ASInt32 inHeight, void* inData);

	void ASAPI (*Destroy)(ADMIconRef inIcon);
	
	// icon data accessors

	ADMIconType ASAPI (*GetType)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetWidth)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetHeight)(ADMIconRef inIcon);
	ASAPI void* (*GetData)(ADMIconRef inIcon);

	ASBoolean ASAPI (*IsFromResource)(ADMIconRef inIcon);

	// conversion from ADMImage

	ADMIconRef ASAPI (*CreateFromImage)(ADMImageRef inImage);
		// The ADMIcon takes ownership of the image.


}
ADMIconSuite2;


// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif
 
// -----------------------------------------------------------------------------

#endif	// __ADMIcon__
