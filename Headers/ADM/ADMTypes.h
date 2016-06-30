/***********************************************************************/
/*                                                                     */
/* ADMTypes.h                                                          */
/* ADM standard interface types                                        */
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

#ifndef __ADMTypes__
#define __ADMTypes__

#ifndef __ASTypes__
#include "ASTypes.h"
#endif

#ifndef __SPPlugs__
#include "SPPlugs.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

// [cpaduan] 11/14/00: CWPro6 can't tell how big the raw hex values 0xFFFFFFFF
// and 0x8FFFFFFF are when you stick them in enums, though you and I can tell 
// that you'd need at least 32 bits. Or perhaps it's unsure as to whether the
// values are signed or unsigned. Anyway, making a couple of const variables to
// make things completely explicit.  These are they.  Wherever, you'd use the
// above constants for enums, use these instead.
const ASInt32 kADMAllFs32 	= 0xFFFFFFFF;
const ASInt32 kADM8F32		= 0x8FFFFFFF;

// -----------------------------------------------------------------------------

#define kADMMaxTextLength 256


// -----------------------------------------------------------------------------
//	References to ADM types

typedef struct _Opaque_ADMDialog* ADMDialogRef;
typedef struct _Opaque_ADMDrawer* ADMDrawerRef;
typedef struct _Opaque_ADMItem* ADMItemRef;
typedef struct _Opaque_ADMList* ADMListRef;
typedef struct _Opaque_ADMHierarchyList* ADMHierarchyListRef;
typedef struct _Opaque_ADMEntry* ADMEntryRef;
typedef struct _Opaque_ADMListEntry* ADMListEntryRef;
typedef struct _Opaque_ADMNotifier* ADMNotifierRef;
typedef struct _Opaque_ADMTracker* ADMTrackerRef;
typedef struct _Opaque_ADMIcon* ADMIconRef;
typedef struct _Opaque_ADMImage* ADMImageRef;

typedef void* ADMUserData;
typedef unsigned long ADMTimerRef;
typedef unsigned long ADMActionMask;
typedef unsigned short ADMChar;

typedef void* ADMRsrcInfo;

typedef SPPluginRef ADMPluginRef;

#ifndef _t_AGMPortPtr
typedef struct _t_AGMPort* _t_AGMPortPtr;
#endif


// -----------------------------------------------------------------------------
//	Measurement units

typedef enum
{

		// If you add new units, do not insert them into the list. Instead, add
		// them to the end of the "normal" units, and adjust kADMMaxNormalUnits.
		// (The actual values of these constants are used in STR# 4300, the
		// unit input strings, to associate units with input strings.)

	kADMNoUnits = 0,
	kADMPointUnits = 1,
	kADMInchUnits = 2,
	kADMMillimeterUnits = 3,
	kADMCentimeterUnits = 4,
	kADMPicaUnits = 5,
	kADMPercentUnits = 6,
	kADMDegreeUnits = 7,
	kADMQUnits = 8,
	kADMBase16Units = 9,
	kADMPixelUnits = 10,
	kADMTimeUnits = 11,
	kADMMinNormalUnits = kADMPointUnits,
	kADMMaxNormalUnits = kADMTimeUnits,
	
	kADMAppUnits1 = 101,
	kADMAppUnits2 = 102,
	kADMAppUnits3 = 103,
	kADMAppUnits4 = 104,
	kADMAppUnits5 = 105,
	kADMAppUnits6 = 106,
	kADMAppUnits7 = 107,
	kADMAppUnits8 = 108,
	kADMAppUnits9 = 109,
	kADMAppUnits10 = 110,
	kADMMaxAppUnits = kADMAppUnits10,
	
	kADMDummyUnits = kADMAllFs32	// cpaduan 11/14/00: CWPro6

}
ADMUnits;


// -----------------------------------------------------------------------------
//	Fonts

typedef enum
{
	kADMDefaultFont = 0,
	kADMDialogFont,
	kADMPaletteFont,
	kADMItalicDialogFont,
	kADMItalicPaletteFont,
	kADMBoldDialogFont,
	kADMBoldPaletteFont,
	kADMBoldItalicDialogFont,
	kADMBoldItalicPaletteFont,
	kADMFixedWidthFont,
	kADMItalicFixedWidthFont,
	kADMBoldFixedWidthFont,
	kADMBoldItalicFixedWidthFont,
	kADMDummyFont = kADMAllFs32		// cpaduan 11/14/00: CWPro6
}
ADMFont;

#ifdef WIN_ENV

typedef void* ADMPlatformFontRef;
#endif

#ifdef MAC_ENV

typedef struct
{
	
	ASInt32 textFont;			// font number
	ASInt32 textStyle;
	ASInt32 textSize;
	
} ADMPlatformFontRef;

#endif


// -----------------------------------------------------------------------------
//	Justify

typedef enum
{
	kADMLeftJustify = 0,
	kADMCenterJustify = 1,
	kADMRightJustify = 2,
	kADMDummyJustify = kADMAllFs32		// cpaduan 11/14/00: CWPro6
}
ADMJustify;


// -----------------------------------------------------------------------------
//	Color
//  kADMDummyColor = 0x000000fe, and an ADMColor > 0x000000fe specifies
//  an RGB value.  Shift off the lower 8 bits, and you have the RGB value.
//  using the high 24 bits.  The low 8 bits should be ignored, but should
//  be set to FF.  The format for the color is 0xRRGGBBFF.
//   black would be ADMColor color = (ADMColor)0x000000FF,
//     red would be ADMColor color = (ADMColor)0xFF0000FF,
//   green would be ADMColor color = (ADMColor)0x00FF00FF,
//    blue would be ADMColor color = (ADMColor)0x0000FFFF,
//   white would be ADMColor color = (ADMColor)0xFFFFFFFF

typedef enum 
{
	kADMBlackColor = 0,
	kADMWhiteColor,
	kADMHiliteColor,
	kADMHiliteTextColor,
	kADMLightColor,
	kADMBackgroundColor,
	kADMShadowColor,
	kADMDisabledColor,
	kADMButtonUpColor,
	kADMButtonDownColor,
	kADMButtonDownShadowColor,
	kADMToolTipBackgroundColor,
	kADMToolTipForegroundColor,
	kADMWindowColor,
	kADMForegroundColor,
	kADMTextColor,
	kADMRedColor,
	kADMTabBackgroundColor,
	kADMActiveTabColor,
	kADMInactiveTabColor,
	kADMDummyColor = (long)0x000000FE
}
ADMColor;


// -----------------------------------------------------------------------------
//	"Create" function option flags common across object types.

enum
{
	kADMIgnoreRomanFontForcing = (1L<<16)	/* Override kADMForceRomanFont host option for a particular object. */
};

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMTypes__
