/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AVExpt.h

 - Types, macros, structures, etc. required to use the AcroView HFT.

   The functions and types can be thought of those that are current and those that are
   obsolete.  Code written using obsolete functions will still compile and work, but those
   definitions have been moved out of this files and into AVExpTObsolete1.h and 
   AVExpTObsolete2.h which are included by this file.
*********************************************************************/

#ifndef _H_AVExpT
#define _H_AVExpT

#include "CoreExpT.h"
#include "ASExpT.h"
#include "PDExpT.h"
#include "AVPrefsD.h"	/* Using AV_PREFERENCES */
#include "ASExtraExpT.h"

#if MAC_PLATFORM
#define QDOFFSCREEN <QD/QDOffscreen.h>
#include QDOFFSCREEN
#endif

#if WIN_PLATFORM
#ifndef _WINDOWS_ /* Check if 32-bit MFC already included <windows.h> */
#define WINDOWS <windows.h>
#include WINDOWS
#endif

#elif UNIX_PLATFORM
#define X11X <X11/X.h>
#include X11X

#elif OS2_PLATFORM
#define INCL_WIN
#define INCL_DOS
#define OS2  <os2.h>
#include OS2
#endif

#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************\
|*																					*|
|*								Notification Types									*|
|*																					*|
\************************************************************************************/
#ifndef _T_NSELECTOR
#define _T_NSELECTOR
typedef ASInt32 NSelector; 	/* Selector for HNT functions */
#define BAD_NSELECTOR		(-1)
#endif

/************************************************************************************\
|*																					*|
|*									AVApp											*|
|*																					*|
\************************************************************************************/
/* 
** AV Application Preferences data type, see AVAppGetPreference()/AVAppSetPreference().
*/
enum {
#define AVP(a, b) a,
#define AVX AVP
#define AVS AVP
#define AVPVOIDP(a) a,
#define AVPSTR(a) a,
#if PLUGIN
#define AVU(a, b, c) avpUnused##c,
#else
#define AVU(a, b, c) a,
#endif
	AV_PREFERENCES
	avpNumPrefs
#undef AVP
#undef AVX
#undef AVS
#undef AVU
#undef AVPVOIDP
#undef AVPSTR
};

#ifndef _T_AVPREFSTYPE
#define _T_AVPREFSTYPE
#if WIN_PLATFORM

/*
 To allow AVPrefsType to run up to 255 entries while still
   maintaining its original size it has been changed from an ASEnum8
   to an ASUns8 on Mac and Unix. On Windows it's alwasy been a 16-bit
   value (even though it's defined as ASEnum8) and so will remain
   unchanged to ensure that the size remains constant.

   You may wonder why ASEnum8 yields a 16-bit value on Window but an
   8-bit value on other platform.  This is due to a historical mix-up
   that's best left in the past.
*/
/**
	A structure containing values that specfiy the Acrobat viewer's 
	preferences settings. Can contain up to 255 entries. 
	@see AVAppGetPreference 
	@see AVAppSetPreference 
	Bit-flag ASUns32 constants for use with AVDocPrintParams. 
	
*/
typedef ASEnum8 AVPrefsType;
#else
typedef ASUns8 AVPrefsType;
#endif
#endif /* _T_AVPREFSTYPE */

/**
	Constants used to specify measurement units in AVPrefsType. 
	
	Do not set the avpPageUnits preference to any value larger 
	than Picas. 
	@see AVAppGetPreference 
	@see AVAppSetPreference 
*/
typedef enum _t_PageUnitsType {
	/** */
	Points,
	/** */
	Inches,		
	/** */
	Millimeters,	
	/** */
	Centimeters,
	/** don't set avpPageUnits pref any larger than this value */
	Picas,			
	/** used only for kKeyPrefsDisplayMeasure pref */
	Feet,		
	/** used only for kKeyPrefsDisplayMeasure pref */
	Yards,
	/** used only for kKeyPrefsDisplayMeasure pref */
	Meters,
	/** used only for kKeyPrefsDisplayMeasure pref */
	Kilometers,
	/** used only for kKeyPrefsDisplayMeasure pref */
	Miles,
	/** used only for kKeyPrefsDisplayMeasure pref */
	Custom
} PageUnitsType;

/* These types are provided to abstract the bitwise implementation.  Thus, the same source
   could be compiled to different implementations for 16-bit, 32-bit, Win, Mac, Unix, etc. */

/**
	A pixel offset value for use in an AVDoc view definition. 
	
	@see AVDocGetViewDef
*/
typedef ASInt16 AVPixelOffset;  

/**
	An array size value for AV methods. 
	@see AVAppChooseFolderDialog 
	@see AVAppOpenDialog 
	@see AVAppSaveDialog 
	@see AVExtensionAcquireInfo 
	@see AVExtensionGetNumPlugIns 

*/
typedef ASUns16 AVArraySize; 

/**
	A flag-bits value for use in callback procedures. 
	@see AVAnnotHandlerDoClickProc 
	@see AVAnnotHandlerDoKeyDownExProc 
	@see AVAnnotHandlerDoKeyDownProc 
	@see AVDocSelectionKeyDownProc 
	@see AVPageViewClickProc 
	@see AVPageViewKeyDownProc 
*/
typedef ASInt16 AVFlagBits16;

/**
	A flag-bits value for use in callback procedures. 
	@see AVAnnotHandlerGetFlagsProc 
	@see AVIconHandlerGetFlagsProc 
	@see AVPageViewAppearanceGetAVMatrix 
	@see AVSysGetModifiers 
	@see AVWindowNew 
	@see AVWindowNewFromPlatformThing
*/
typedef ASUns32 AVFlagBits32;

/** Not used. */
typedef ASInt16 AVMouseEventCode;

/**
	A key code value for use in key-down callback procedures. 
	
	@see AVAnnotHandlerDoKeyDownExProc 
	@see AVAnnotHandlerDoKeyDownProc 
	@see AVDocSelectionKeyDownProc 
	@see AVPageViewKeyDownProc 
*/
typedef ASUns16 AVKeyCode;

/** signed int for historical reasons */
typedef ASInt16 AVPriority; 

/** An version-number part. */
typedef ASUns16 AVVersionNumPart; 

/** number of bytes */
typedef ASUns32 AVBufferSize; 

/** AVDragType or -1 for err */
typedef ASInt16 AVDragTypeEx; 

/**
	A menu index value that indicates a user's choice in a pop 
	up menu. Uses negative indexes for errors.
	@see AVMenubarAcquireMenuByIndex 
	@see AVMenubarGetMenuIndex 
	@see AVMenubarAddMenu 
	@see AVMenuAcquireMenuItemByIndex 
	@see AVMenuGetMenuItemIndex 
	@see AVMenuAddMenuItem 
	@see AVPageViewDoPopupMenu
*/
typedef ASInt32 AVMenuIndex; 

/** Not used. */
typedef ASInt32 AVIconType;

/**
	A filter index value for AV methods. Uses 0+ for valid, -1 for all.
	@see AVAppOpenDialog 
	@see AVAppSaveDialog
*/
typedef ASInt16 AVFilterIndex; 

/**
	A page index value for use in an AVDoc view definition. 
	uses 0+ for valid, -1 for invalid/NA.
	
	@see AVDocGetViewDef
*/
typedef ASInt32 AVPageIndex; 

/** uses 0+ for valid */
typedef ASInt32 AVCursorID; 

/* These types are signed for historical reasons.  They may change to unsigned in the future */
/* When possible use the equivalent unsigned type above. */

/**
	The number of items (not the number of bytes) in an array, 
	for use in page view methods. 
	@see AVAppGetNumDocs 
	@see AVDocGetClientName 
	@see AVMenubarGetNumMenus 
	@see AVMenuGetNumMenuItems 
	@see AVMenuGetTitle 
	@see AVMenuItemGetTitle 
	@see AVPageViewDrawRectOutline 
	@see AVPageViewDrawRectOutlineWithHandles 
	@see AVPageViewGetThreadIndex 
	@see AVToolBarGetNumButtons 
	@see AVUtilGetBaseNameAndExtensionByPathName 
	@see AVUtilGetBaseNameAndExtensionByString 
	@see AVActionGetInstructionsProc 
	@see AVActionGetButtonTextProc 
	@see AVActionGetStringOneTextProc 
	@see AVActionGetStringTwoTextProc
*/
typedef ASInt32 AVTArraySize;  

/**
	The number of bytes in a buffer, for use in page view methods. 
	
	@see AVAppGetNumDocs
*/
typedef ASInt32 AVTBufferSize; 

/**
	An array size value for use in page view methods. 
	@see AVPageViewDragOutNewRectSnapped 
	@see AVPageViewDragRectSnapped 
*/
typedef ASInt16 AVTSmallArraySize;  

/**
	A flag-bits value for use in page-view methods. 
	@see AVPageViewDeviceRectToPageRZ 
	@see AVPageViewDragOutNewRectSnapped 
	@see AVPageViewDragRectSnapped 
	@see AVPageViewDragRectSnappedEx
*/
typedef ASInt32 AVTFlagBits; 

/**
	A flag-bits value for use in tool button methods. 
	@see AVToolButtonSetExternal
*/
typedef ASUns16 AVTFlagBits16; 

/**
	A click-number value for use in page-view callback procedures. 
	
	@see AVPageViewClickProc 
	@see AVPageViewKeyDownProc
*/
typedef ASInt16 AVTCount; 

/* signed int for historical reasons */
/**
	A version-number part. 
	@see AVAppGetVersion
*/
typedef ASInt16 AVTVersionNumPart; 

#if !defined(ACRO_SDK_LEVEL)  || (ACRO_SDK_LEVEL < 0x00060000)
typedef ASInt16 AVSDKDependentInteger;
#else
typedef ASInt32 AVSDKDependentInteger;
#endif

/**
	A size in the page view's device space. 
	@see AVPageViewDrawRectOutline 
	@see AVPageViewScrollToRect 
*/
typedef AVSDKDependentInteger AVDevSize;

/** 
	An AVDevCoord contains an x or y coordinate in the page view's
	device space. 
*/
typedef AVSDKDependentInteger AVDevCoord;

/**
	An x or y coordinate in the window's space. (0,0) is at 
	the top left, and units are in pixels. 
	@see AVWindowAdjustCursorProc 
	@see AVWindowMouseDownProc
*/
typedef AVSDKDependentInteger AVWindowCoord;

/**
	An x or y coordinate in the screen space. (0,0) is at the 
	top left of the main monitor. 
*/
typedef AVSDKDependentInteger AVScreenCoord;


/************************************************************************************\
|*																					*|
|*								    AVIcon											*|
|*																					*|
\************************************************************************************/

/**@name AVIcon*/
/*@{*/

/**
	An icon on a menu item or toolbar button. 
	
	On the Macintosh this is a handle to an SICN resource, an IconSuiteRef,
	or an AVIconBundle (AVIconBundles are new in Acrobat 5.0)

	On Windows this is an HBITMAP or an AVIconBundle.
	@see AVMenuItemNew 
	@see AVToolButtonGetIcon 
	@see AVToolButtonNew 
	@see AVToolButtonSetIcon 
*/
typedef const char * AVIconID;
typedef void * AVIcon;

#if WIN_PLATFORM

	/** */
	typedef HICON			AVIconBundleIconRef;
#elif MAC_PLATFORM
	typedef IconSuiteRef	AVIconBundleIconRef;
#else
	typedef void*			AVIconBundleIconRef;
#endif /* WIN_PLATFORM */


/**
	An icon bundle allows you to gather up multiple icons and 
	present them to Acrobat as a single AVIcon. For example, 
	when creating a toolbar button you can pass in an icon bundle 
	specifying both gray and color icons; the gray icon will 
	be used to draw the button in its normal state, the color 
	icon will be used to draw the button when the pointer is 
	over it. 

	The format for icon bundles is platform-specific (primarily 
	since the format for AVIcons is platform-specific). On Windows 
	the icons can be specified using HICONs, not HBITMAPs. On 
	the Mac, they are IconSuiteRef, never SICN resources. Both 
	platforms support the PNG format. 

	The tags at the front are there so the implementation can 
	determine beyond a shadow of a doubt that the information 
	passed in is an icon bundle and not an Acrobat 4-compatible 
	AVIcon. 
*/
typedef struct _t_AVIconBundleRec {

	/** Set to AVIC (for example, bundle.tag1 = 'AVIC')  */
	ASUns32				tag1;		

	/** Set to ONBU (for example, bundle.tag2 = 'ONBU') */
	ASUns32				tag2;		

	/** Set to version of app (for example, 0x00050000 for Acrobat 5.0) */
	ASInt32				version;	

	/** Defined according to the AVIconBundleRef typedef. */ 
	AVIconBundleIconRef	grayIcon;

	/** Defined according to the AVIconBundleRef typedef. */
	AVIconBundleIconRef	colorIcon;
} AVIconBundleRec, *AVIconBundle;



/** */
enum {
	/** */
	kAVIconColor,
	/** */
	kAVIconGrayscale,
	/** */
	kAVIconHighContrast
};
typedef ASEnum16 AVIconColorFormat;

/** */
enum
{
#ifdef MAC_PLATFORM
	/** 16x16 icon ref */
    kAVIconMacIconRef16,  
    /** 32x32 icon ref */
    kAVIconMacIconRef32, 
    /** 128x128 icon ref */
    kAVIconMacIconRef128, 
#endif
	/** */
    kAVIconPNG,
    /** */
    kAVIconLayered
};

/**
	Constants that specify a data format for an AVIconBundle6. 
	
	@see AVAppCreateIconBundle6
*/
typedef ASEnum16 AVIconDataFormat;


/**
	An icon bundle allows you to gather up multiple icons and 
	present them to Acrobat as a single AVIcon. For example, 
	when creating a toolbar button you can pass in an icon bundle 
	specifying both gray and color icons; the gray icon will 
	be used to draw the button in its normal state, the color 
	icon will be used to draw the button when the pointer is 
	over it. The format for icon bundles is platform-specific 
	(primarily since the format for AVIcons is platform-specific). 
	On Windows the icons are specified using HICONs, not HBITMAPs. 
	On the Mac, they are IconSuiteRef, never SICN resources. 
	The tags at the front are there so the implementation can 
	determine beyond a shadow of a doubt that the information 
	passed in is an icon bundle and not an Acrobat 4-compatible 
	AVIcon. 

	NOTE: This form of the icon bundle is new in Acrobat 6.0. 
	
*/
typedef struct _t_AVIconBundleRec6 AVIconBundleRec6, *AVIconBundle6;


/**
	Flags returned by AVIconHandler->getFlags 
*/
#define AVICON_DONT_CACHE 1

/**
	Callback for AVIconHandler that retrieves the measurements 
	of the icon. All icons in the bundle are assumed to be the 
	same size. 
	@param bundle The icon bundle for which the icon size 
	is measured. 
	@param w (Filled by the method) The icon width in pixels. 
	@param h (Filled by the method) The icon height in pixels. 
	@see AVAppCreateIconBundle6 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVIconHandlerMeasureProc)(AVIconBundle6 bundle, ASInt32 *w, ASInt32 *h);

/**
	Callback for AVIconHandler. It opens a stream so that a 
	drawing function can read the data contained in the icon 
	set. 
	@param bundle The icon bundle for which the stream is 
	opened. 
	@param colorFormat The color format for the icon bundle.
	@return The stream object. 
	@see AVAppCreateIconBundle6 
*/
typedef ACCBPROTO1 ASStm (ACCBPROTO2 *AVIconHandlerOpenStmProc)(AVIconBundle6 bundle, AVIconColorFormat colorFormat);

/**
	Callback for AVIconHandler. It returns the flags value for 
	the icon. 
	@param bundle The icon bundle for which the flags are 
	obtained. The following flags are defined: AVICON_DONT_CACHE=1
	@return The flags value. 
	@see AVAppCreateIconBundle6 
*/
typedef ACCBPROTO1 AVFlagBits32 (ACCBPROTO2 *AVIconHandlerGetFlagsProc)(AVIconBundle6 bundle);

/**
	Callback for AVIconHandler. It releases the icon object. 
	
	@param bundle The icon bundle that is released.
	@see AVAppCreateIconBundle6 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVIconHandlerReleaseProc)(AVIconBundle6 bundle);


/** 

*/
typedef struct _t_AVIconHandlerRec {
	/** Size of structure. Set to sizeof(AVIconHandlerRec). */
	ASInt32 size;
	/** A pointer to user-defined data passed to the callback. */
	void* clientData;
	/** */
	AVIconHandlerMeasureProc Measure;
	/** */
	AVIconHandlerOpenStmProc OpenStm;
	/** */
	AVIconHandlerGetFlagsProc GetFlags;
	/** */
	AVIconHandlerReleaseProc Release;
} AVIconHandlerRec, *AVIconHandler;


/** 
	An icon bundle allows you to gather up multiple icons and present them to Acrobat as a
	single AVIcon. For example, when creating a toolbar button you can pass in an icon
	bundle specifying both gray and color icons; the gray icon will be used to draw the button
	in its normal state, the color icon will be used to draw the button when the pointer is over
	it. The format for icon bundles is platform-specific (primarily since the format for AVIcons
	is platform-specific). On Windows the icons are specified using HICONs, not HBITMAPs. On
	the Mac, they are IconSuiteRef, never SICN resources. The tags at the front are there so
	the implementation can determine beyond a shadow of a doubt that the information
	passed in is an icon bundle and not an Acrobat 4-compatible AVIcon.
	NOTE: This form of the icon bundle is new in Acrobat 6.0.
*/
struct _t_AVIconBundleRec6 {
	/** Set to AVIC (for example, bundle.tag1 = 'AVIC') */
	ASUns32					tag1;
	/** Set to ONBU (for example, bundle.tag2 = 'ONBU') */
	ASUns32					tag2;
	/** Set to version of app (for example, 0x00060000 for Acrobat 6.0) */
	ASInt32					version;
	/** The icon handler. */
	AVIconHandler			handler;
	/** A pointer to the icon data record structure. */
	void					*iconData;
};


/**
	A data record for an AVIconBundle6. 
	@see AVAppCreateIconBundle6 
*/
typedef struct _t_AVIconData {
	/** A data stream containing the binary-encoded data */
	ASStm					dataStm;
	/** The color this icon represents. */
	AVIconColorFormat		eColorFormat; 
} AVIconDataRec, *AVIconData;

/*@}*/

/************************************************************************************\
|*																					*|
|*								  AVPageView										*|
|*																					*|
\************************************************************************************/
#ifndef _T_AVPAGEVIEW
#define _T_AVPAGEVIEW

#ifdef __cplusplus
class AVPageViewObj;
/** */
typedef AVPageViewObj* AVPageView;
#else

/**
	The area of the Acrobat viewer’s window that displays the contents of a document
	page. Every AVDoc has an AVPageView and vice versa. It contains references to the
	PDDoc and PDPage objects for the document being displayed.
	@see AVDocGetPageView
	@see AVPageViewAppearanceGetAVMatrix 
	@see AVPageViewAcquireMachinePort 
	@see AVPageViewReleaseMachinePort
	@see AVPageViewGetActiveBead 
	@see AVPageViewGetAnnotRect 
	@see AVPageViewSetAnnotLocation
	@see AVPageViewGetAperture 
	@see AVPageViewGetAVDoc 
	@see AVPageViewGetColor 
	@see AVPageViewSetColor
	@see AVPageViewShowControl
	@see AVPageViewGetDevToPageMatrix
	@see AVPageViewGetFirstVisiblePageNum
	@see AVPageViewGetFocusAnnot 
	@see AVPageViewSetFocusAnnot
	@see AVPageViewGetLastVisiblePageNum
	@see AVPageViewGetLayoutMode 
	@see AVPageViewSetLayoutMode
	@see AVPageViewGetMousePosition
	@see AVPageViewGetMousePositionSnapped
	@see AVPageViewGetNextView
	@see AVPageViewGetPage
	@see AVPageViewGetPageNum 
	@see AVPageViewSetPageNum
	@see AVPageViewGoBack
	@see AVPageViewGoForward
	@see AVPageViewGoTo
	@see AVPageViewGetPageToDevMatrix
	@see AVPageViewGetSelectedAnnotPageNum
	@see AVPageViewGetThreadIndex
	@see AVPageViewGetVisibleAnnotPage
	@see AVPageViewGetZoom 
	@see AVPageViewZoomTo
	@see AVPageViewGetZoomType 
	@see AVPageViewDeviceRectToPageRZ
	@see AVPageViewDrawCosObj
	@see AVPageViewDragRect
	@see AVPageViewDrawNow
	@see AVPageViewDrawRect
	@see AVPageViewDrawRectOutline
	@see AVPageViewHighlightText
	@see AVPageViewInsetRect
	@see AVPageViewInvalidateRect
	@see AVPageViewInvalidateText
	@see AVPageViewInvertRect
	@see AVPageViewInvertRectOutline
	@see AVPageViewInvertQuad
	@see AVPageViewIsBeadAtPoint
	@see AVPageViewPageNumIsVisible
	@see AVPageViewPointInText
	@see AVPageViewReadPageDown
	@see AVPageViewReadPageUp
	@see AVPageViewGoForward
	@see AVPageViewGoTo
	@see AVPageViewGetPageToDevMatrix
	@see AVPageViewGetSelectedAnnotPageNum
	@see AVPageViewGetThreadIndex
	@see AVPageViewGetVisibleAnnotPage
	@see AVPageViewGetZoom 
	@see AVPageViewZoomTo
	@see AVPageViewGetZoomType
	@see AVPageViewDeviceRectToPageRZ
	@see AVPageViewDrawCosObj
	@see AVPageViewDragRect
	@see AVPageViewDrawNow
	@see AVPageViewDrawRect
	@see AVPageViewDrawRectOutline
	@see AVPageViewHighlightText
	@see AVPageViewInsetRect
	@see AVPageViewInvalidateRect
	@see AVPageViewInvalidateText
	@see AVPageViewInvertRect
	@see AVPageViewInvertRectOutline
	@see AVPageViewInvertQuad
	@see AVPageViewIsBeadAtPoint
	@see AVPageViewPageNumIsVisible
	@see AVPageViewPointInText
	@see AVPageViewReadPageDown
	@see AVPageViewReadPageUp
*/
typedef struct _t_AVPageView *AVPageView;
#endif /* __cplusplus */

#endif /* _T_AVPAGEVIEW */


#if WIN_PLATFORM

/* we have to use this old definition because the Windows 3.0 GM build
   did not pick up the new definition below. */

/**
	Constants that specify the zoom strategy that Acrobat is 
	to follow. 
	@see AVPageViewZoomTo 
	@see AVPageViewGetZoomType 
*/
typedef enum _t_AVZoomType {

	/** No variable zoom (that is, zoom is a fixed value such as 1.0 for 100%). */
	AVZoomNoVary,				

	/** Fits the page to the window. */
	AVZoomFitPage,				

	/** Fits the page width to the window. */
	AVZoomFitWidth,					

	/** Fits the page height to the window. */
	AVZoomFitHeight,			

	/** Fits the width of the portion of the page upon which marks are made to the window. */
	AVZoomFitVisibleWidth,		

	/** Uses the page's preferred zoom. */
	AVZoomPreferred,				

	/** (New in Acrobat 5.0) Reflow page to window width. */
	AVZoomReflowWidth			
} AVZoomType;

#else


/** 

*/
enum {
	/** no variable zoom */
	AVZoomNoVary,					
	/** fit page to window */
	AVZoomFitPage,					
	/** fit page width to window */
	AVZoomFitWidth,					
	/** fit page height to window */
	AVZoomFitHeight,				
	/** fit visible width to window */
	AVZoomFitVisibleWidth,			
	/** use page's preferred zoom */
	AVZoomPreferred,				
	/** reflow page to window width */
	AVZoomReflowWidth				
};
typedef ASEnum8 AVZoomType;

#endif


/**
	Used with AVPageViewShowControl to allow a plug-in author 
	to turn on and off the controls shown in the status area 
	at the bottom of a page view. New for Acrobat 5.0.
	@see AVPageViewShowControl 
*/
enum {

	/** The zoom control. */
	kAVPageViewZoomControl,

	/** The page flip control. */
	kAVPageViewPageFlipControls,

	/** The page number control. */
	kAVPageViewPageNumberControl,

	/** The page size control. */
	kAVPageViewPageSizeControl,

	/** The splitter bar control. */
	kAVPageViewSplitterBar,

	/** The horizontal scroll */
	kAVPageViewHorizontalScrollBar,

	/** The vertical scroll */
	kAVPageViewVerticalScrollBar,

	/** The gray border control. */
	kAVPageViewGrayBorder
};
typedef ASEnum16 AVPageViewControlID;

#if WIN_PLATFORM

/**
	The HWND is that of the document window's AVPageView region 
	(the portion of the window in which the PDF file's pages 
	are drawn). 
	@see AVPageViewAcquireMachinePort
*/
typedef struct _t_WinPort {

	/** */
	HWND	hWnd;

	/** */
	HDC		hDC;
} WinPortRec, *WinPort;
#endif

#if OS2_ENV
typedef struct _t_OS2Port {
	HWND	hWnd;
	HPS	hPS;
} OS2PortRec, *OS2Port;
#endif


/**@name AVPageViewDidChangeFlags 
	flags for the "how" parameter to AVPageViewDidChange notification 
*/
/*@{*/

/** The view has been scrolled. */
#define PAGEVIEW_UPDATE_SCROLL		1

/** The page number has changed. */
#define PAGEVIEW_UPDATE_PAGENUM		2

/** A new view has been created. */
#define PAGEVIEW_UPDATE_PAGESIZE	4

/** The zoom has been changed. */
#define PAGEVIEW_UPDATE_ZOOM		8
/*@}*/


/** Constants identifying mouse events in AVMouseInfoRec.event */
enum {

	/** */
	kAVLeftButtonDown = 1,

	/** */
	kAVLeftButtonUp,

	/** */
	kAVLeftButtonClick,

	/** */
	kAVLeftButtonDblClick,

	/** */
	kAVRightButtonDown,

	/** */
	kAVRightButtonUp,

	/** */
	kAVRightButtonClick,

	/** */
	kAVRightButtonDblClick,

	/** */
	kAVMouseMove,

	/** */
	kAVMouseEnter,

	/** */
	kAVMouseLeave
};
typedef ASEnum16 AVMouseEvent;

#ifndef _T_AVDOC
#define _T_AVDOC


/** A view of a PDF document in a window. There is one AVDoc per displayed document.
	Unlike a PDDoc, an AVDoc has a window associated with it.
	@see AVAppGetActiveDoc
	@see AVAppEnumDocs
	@see AVDocOpenFromASFileWithParams
	@see AVDocOpenFromFile
	@see AVDocOpenFromFileWithParams
	@see AVDocOpenFromPDDoc
	@see AVDocOpenFromPDDocWithParams
	@see AVPageViewGetAVDoc
	@see AVDocClose
	@see AVAppEnumDocs
	@see AVDocEnumSelection
	@see AVDocDoActionPropsDialog
	@see AVDocDoCopyAs
	@see AVDocDoPrint
	@see AVDocDoSaveAs
	@see AVDocDoSaveAsWithParams
	@see AVDocDoSelectionProperties
	@see AVDocGetAVWindow 
	@see AVDocGetClientName 
	@see AVDocSetClientName
	@see AVDocGetPageText 
	@see AVDocGetPageView 
	@see AVDocGetPDDoc 
	@see AVDocClearSelection
	@see AVDocDeleteSelection
	@see AVDocShowSelection
	@see AVDocGetSelection 
	@see AVDocSetSelection
	@see AVDocGetSelectionServerByType
	@see AVDocGetSelectionType
	@see AVDocGetSplitterPosition 
	@see AVDocSetSplitterPosition
	@see AVDocGetViewDef 
	@see AVDocSetViewDef
	@see AVDocGetViewMode 
	@see AVDocSetViewMode
	@see AVDocIsExternal
	@see AVDocPerformAction
	@see AVDocRegisterSelectionServer
	@see AVDocSetDead
	@see AVDocIsReadOnly 
	@see AVDocSetReadOnly
*/
typedef struct _t_AVDoc *AVDoc;
#endif /* _T_AVDOC */

#include "AVExpTObsolete1.h"  /* types for old versions of Acrobat */

/* 
** AcroView defines a coordinate system in which (0,0) is at the top,
** x increases to the right, and y increases down (the same as GDI and
** Quickdraw but opposite PostScript).  An AVRect is defined so that
** its top is above its bottom, but this means that 0 < top < bottom.
*/

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure representing a rectangle (a quadrilateral 
	having only horizontal and vertical sides). 

	The AcroView coordinate system is defined so that (0,0) 
	is at the top, x increases to the right, and y increases 
	down (the same as GDI and QuickDraw but opposite to the 
	PostScript language). An AVRect is defined so that its top 
	is above its bottom, but this means that 0 < top < bottom. 
	

	NOTE: Types of numeric coordinate values have changed in 
	Acrobat 6. These types are conditionally compiled as ASInt16 
	or ASInt32 depending on the Acrobat version level. 
	@see Numerous 
*/
typedef struct AVRect {

	/** */
	AVSDKDependentInteger left;

	/** */
	AVSDKDependentInteger top;

	/** */
	AVSDKDependentInteger right;

	/** */
	AVSDKDependentInteger bottom;
} AVRect, *AVRectP;
#endif

/**
	Data structure representing a rectangle (a quadrilateral 
	having only horizontal and vertical sides) in a page view's 
	device space. 
	@see AVPageViewDragOutNewRect 
	@see AVPageViewDragOutNewRectSnapped 
	@see AVPageViewDragRect 
	@see AVPageViewDragRectSnapped 
	@see AVPageViewDragRectSnappedEx 
	@see AVPageViewDrawAnnotSequence 
	@see AVPageViewDrawRect 
	@see AVPageViewDrawRectOutline 
	@see AVPageViewDrawRectOutlineWithHandles 
	@see AVPageViewGetAnnotRect 
	@see AVPageViewGetAperture 
	@see AVPageViewInvalidateRect 
	@see AVPageViewInvertRect 
	@see AVPageViewInvertRectOutline 
	@see AVPageViewScrollToRect 
	@see AVPageViewSnapRect 
	@see AVRectHandleHitTest 
	@see AVAnnotHandlerDrawExProc 
	@see AVAnnotHandlerGetAnnotViewBBoxProc 
	@see AVPageViewDrawRectOutline 
	@see AVPageViewScrollToRect 
*/
typedef AVRect AVDevRect;
/* The _T_AVDEVRECT macro prevents AVDevRectP from being multiply defined */
/* AVDevRectP is also defined in PINotify.h */
#ifndef _T_AVDEVRECT
#define _T_AVDEVRECT
typedef AVRectP AVDevRectP;
#endif /* _T_AVDEVRECT */

/**
	Data structure representing a rectangle (a quadrilateral 
	having only horizontal and vertical sides) in a window's 
	coordinate space. 
	@see AVWindowGetInterior 
	@see AVWindowInvalidateRect 
	@see AVWindowDrawProc 
*/
typedef AVRect AVWindowRect;

/**
	Data structure representing a rectangle (a quadrilateral 
	having only horizontal and vertical sides) in a screen's 
	coordinate space. 
	@see AVWindowGetFrame 
	@see AVWindowSetFrame 
	@see AVWindowDidResizeProc 
	@see AVWindowWillBeResizedProc 
*/
typedef AVRect AVScreenRect;


/** */
typedef struct _t_AVPoint
{
	/** */
	ASInt32	h;
	/** */
	ASInt32 v;
} AVPoint;

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	User-supplied callback that is called whenever the AVPageView 
	is drawn. This callback is registered using AVAppRegisterForPageViewDrawing. 
	
	NOTE: The numeric types have changed in Acrobat 6.0. 
	@param pageView The AVPageView to redraw. 
	@param updateRect The rectangle enclosing the region to 
	redraw. 
	@param data User-supplied data that was passed in the 
	call to AVAppRegisterForPageViewDrawing.
	@see AVAppRegisterForPageViewDrawing 
	@see AVAppUnregisterForPageViewDrawing 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVPageViewDrawProc)(AVPageView pageView, AVDevRect* updateRect, void*data);

/**
	User-supplied callback that is called whenever there is 
	a mouse click in its AVPageView. This callback is registered 
	using AVAppRegisterForPageViewClicks. 
	@param pageView IN/OUT? The AVPageView in which the click occurred. 
	@param x IN/OUT? The click's x-coordinate. 
	@param y IN/OUT? The click's y-coordinate. 
	@param flags IN/OUT? Modifier keys that are held down while the 
	mouse was clicked. They must be an OR of the Modifier Keys 
	value. 
	@param clickNo IN/OUT? 1 if single click, 2 if double-click, 3 
	if triple-click. 
	@param data IN/OUT? User-supplied data that was passed in the call 
	to AVAppRegisterForPageViewClicks. 
	@return true if the callback handled the mouse click, false if it 
	does not and the click should be passed on to the next click 
	handler. 
	@see AVAppRegisterForPageViewClicks 
	@see AVAppUnregisterForPageViewClicks 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVPageViewClickProc)(AVPageView pageView, AVDevCoord x, AVDevCoord y, AVFlagBits16 flags, AVTCount clickNo, void*data);

/**
	User-supplied callback that is called whenever the cursor 
	is moved. This callback is registered using AVAppRegisterForPageViewAdjustCursor. 
	
	NOTE: The coordinate numeric types have changed in Acrobat 
	6.0. 
	@param pageView The AVPageView in which the cursor is 
	located. 
	@param x The cursor's x-coordinate. 
	@param y The cursor's y-coordinate. 
	@param data User-supplied data that was passed in the 
	call to AVAppRegisterForPageViewAdjustCursor.
	@return true if the callback handled the cursor movement, false 
	if it did not and the cursor handler should be allowed to. 
	
	@see AVAppRegisterForPageViewAdjustCursor 
	@see AVAppUnregisterForPageViewAdjustCursor 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVPageViewCursorProc)(AVPageView pageView, AVDevCoord x, AVDevCoord y, void* data);
#endif

/**
	Called whenever there is a key down in its AVPageView. This 
	callback is registered using AVAppRegisterForPageViewKeyDown. 
	
	NOTE: The numeric argument types have changed in Acrobat 
	6.0. 
	@param pageView The AVPageView in which the keystroke 
	occurred. 
	@param keyCode An ASCII code representing the key that 
	was pressed. In some cases a different code results from 
	a key combination (for example, a Control+key combination 
	results in an ASCII code <= 31). 
	@param flags Modifier keys that are held down while the 
	key was pressed. They must be an OR of the Modifier Keys 
	value. 
	@param data User-supplied data that was passed in the 
	call to AVAppRegisterForPageViewKeyDown.
	@return false to process the keydown event, true otherwise. 
	@see AVAppRegisterForPageViewKeyDown 
	@see AVAppUnregisterForPageViewKeyDown 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVPageViewKeyDownProc)(AVPageView pageView, AVKeyCode keyCode, AVFlagBits16 flags, void* data);

/**
	Callback for AVDocGetPageText. Text is passed to it in the 
	specified format. 
	@param format IN/OUT? Text format. See the description of the format 
	parameter of AVDocGetPageText for a list of the allowed 
	types. 
	@param buf IN/OUT? The text. 
	@param bufLen IN/OUT? Length of buf, in bytes. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVDocGetPageText. 
	@see AVDocGetPageText 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVTextCopyProc)(ASAtom format, void *buf, AVTBufferSize bufLen, void *clientData);

/************************************************************************************\
|*																					*|
|*									AVMenu											*|
|*																					*|
\************************************************************************************/


/** A menu in the Acrobat viewer’s menubar. Plug-ins can create new menus, add menu
	items at any location in any menu, and remove menu items. Deleting an AVMenu
	removes it from the menubar (if it was attached) and deletes all the menu items it
	contains.
	@see AVMenuAcquire
	@see AVMenuNew
	@see AVMenuItemAcquireSubmenu
	@see AVMenuItemGetParentMenu
	@see AVMenubarAcquireMenuByName
	@see AVMenubarAcquireMenuByIndex
	@see AVMenubarAcquireMenuByPredicate
	@see AVMenuRelease
	@see AVMenuRemove
	@see AVMenuAddMenuItem
	@see AVMenuGetMenuItemIndex 
	@see AVMenuGetName 
	@see AVMenuGetNumMenuItems 
	@see AVMenuGetParentMenubar 
	@see AVMenuGetParentMenuItem 
	@see AVMenuGetTitle
*/
typedef struct _t_AVMenu *AVMenu;


/**
	Callback that is called for each menu enumerated by AVMenubarAcquireMenuByPredicate. 
	The first menu for which this callback returns true is acquired. 
	
	@param menu IN/OUT? The current menu in the enumeration. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVMenubarAcquireMenuByPredicate. 
	@return true to acquire the current menu and halt enumeration, false 
	to continue enumeration. 
	@see AVMenubarAcquireMenuByPredicate 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVMenuPredicate)(AVMenu menu, void *clientData);

/**
	Called after a context menu has been created but before 
	it is shown to the user. The callback can add menu items 
	to or remove menu items from the menu. The client must register 
	the callback using AVAppRegisterForContextMenuAddition. 
	

	This callback should not raise an error. 
	@param menuName The menu name. One of: - Page:The standard 
	context menu for an AVPageView. - Select:The context menu 
	for selected text. 
	@param menu The menu object. 
	@param menuData The menu data as an AVDoc pointer. 
	@param clientData User-supplied data that was passed in 
	the call to AVAppRegisterForContextMenuAddition.
	@see AVAppRegisterForContextMenuAddition 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVContextMenuAdditionProc)
	(ASAtom menuName, AVMenu menu, void *menuData, void *clientData);

/************************************************************************************\
|*																					*|
|*								  AVMenubar											*|
|*																					*|
\************************************************************************************/


/** The Acrobat viewer’s menubar and a list of all menus. There is only one AVMenubar.
	Plug-ins can add new menus to or remove any menu from the menubar. The menubar
	can be hidden from the user’s view.
	@see AVAppGetMenubar the standard way to obtain the menubar.
	@see AVMenuGetParentMenubar
	@see AVMenubarAddMenu
	@see AVMenuRemove
	@see AVMenubarAcquireMenuByIndex
	@see AVMenubarAcquireMenuByName 
	@see AVMenubarAcquireMenuByPredicate
	@see AVMenubarAcquireMenuItemByName 
	@see AVMenubarAcquireMenuItemByPredicate
	@see AVMenubarGetMenuIndex
	@see AVMenubarGetNumMenus
	@see AVMenuItemRemove
	@see AVMenubarHide
	@see AVMenubarShow
*/
typedef struct _t_AVMenubar *AVMenubar;

#define BAD_MENUITEM_INDEX (-1)
#define PREPEND_MENUITEM (-2)
#define APPEND_MENUITEM (9999)

#define BAD_MENU_INDEX (-1)
#define PREPEND_MENU (-2)
#define APPEND_MENU (9999)

#define NO_SHORTCUT	('\0')

/************************************************************************************\
|*																					*|
|*								  AVMenuItem										*|
|*																					*|
\************************************************************************************/

/** A menu item under a menu in the Acrobat viewer. It has a number of attributes,
	including a name, a keyboard shortcut, a procedure to execute when the menu item is
	selected, a procedure to compute whether or not the menu item is enabled, a
	procedure to compute whether or not the menu item is check marked, and whether or
	not it has a submenu.
	@see AVMenuItemNew
	@see AVMenuItemAcquire
	@see AVMenubarAcquireMenuByName
	@see AVMenubarAcquireMenuItemByPredicate
	@see AVMenuAcquireMenuItemByIndex
	@see AVMenuGetParentMenuItem
	@see AVMenuItemRelease
	@see AVMenuItemRemove
	@see AVMenuGetMenuItemIndex 
	@see AVMenuItemAcquireSubmenu
	@see AVMenuItemGetLongOnly
	@see AVMenuItemGetName
	@see AVMenuItemGetParentMenu
	@see AVMenuItemGetShortcut
	@see AVMenuItemGetTitle AVMenuItemSetTitle
	@see AVMenuItemIsEnabled
	@see AVMenuItemIsMarked
	@see AVMenuItemSetComputeEnabledProc
	@see AVMenuItemSetComputeMarkedProc
	@see AVMenuItemSetComputeVisibleProc
	@see AVMenuItemSetExecuteProc
*/
typedef struct _t_AVMenuItem *AVMenuItem;

/**
	Callback that is called whenever a menu item or toolbar 
	button is executed. It implements whatever the menu item 
	or toolbar button does (for example, opening a file or initiating 
	a search). 

	This method may also be called from an external application 
	displaying a PDF file in its window, using the ExternalDocServerCreationData 
	structure. 
	@param data IN/OUT? User-supplied data that was passed when AVMenuItemSetExecuteProc 
	or AVToolButtonSetExecuteProc were called. 
	@see AVMenuItemSetExecuteProc 
	@see AVToolButtonSetExecuteProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVExecuteProc)(void *data);

/**
	Callback that is used to determine whether a menu item, 
	toolbar button, or tool is enabled. If used for a tool, 
	it is one of the optional callbacks for AVTool. 

	This procedure is called every time the menu or toolbar 
	button is displayed, so it should not do compute-time-intensive 
	processing. It is called before the menu item or toolbar 
	button is displayed, or before a tool is activated. If it 
	returns false, the menu item, toolbar button, or tool is 
	disabled; otherwise it is enabled. If this callback is NULL, 
	the menu item, toolbar button, or tool is always enabled. 
	
	Each menu item, toolbar button, or tool can have its own 
	AVComputeEnabledProc, or they can be shared. 
	@param data User-supplied data that was passed in the 
	call to AVMenuItemSetComputeEnabledProc or AVToolButtonSetComputeEnabledProc.
	@return true if the menu item, toolbar button, or tool is enabled, 
	false otherwise. 
	@see AVMenuItemSetComputeEnabledProc 
	@see AVToolButtonSetComputeEnabledProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVComputeEnabledProc)(void *data);

/**
	Callback that is used to determine whether or not a menu 
	item or toolbar button is marked (a marked menu item has 
	a check mark next to it, and a marked toolbar button appears 
	selected). It is called before the menu item or toolbar 
	button is displayed. If it returns false, the menu item 
	of toolbar button is not marked, otherwise it is marked. 
	
	Each menu item and toolbar button can have is own AVComputeMarkedProc, 
	or they can be shared. 
	@param data IN/OUT? User-supplied data that was passed in the call 
	to AVMenuItemSetComputeMarkedProc or AVToolButtonSetComputeMarkedProc. 
	
	@return true if the menu item or toolbar button is marked, false 
	otherwise. 
	@see AVMenuItemSetComputeMarkedProc 
	@see AVToolButtonSetComputeMarkedProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVComputeMarkedProc)(void *data);

/**
	Callback that is used to determine whether a toolbar button, 
	menu item, or HowTo panel is visible when its parent is 
	opened. It is called before the item is displayed. If it 
	returns true, the item is visible, otherwise it is not visible. 
	

	Each toolbar button, menu item, or HowTo panel can have 
	its own visibility procedure, or they can be shared. 

	Because the procedure is called whenever the item is displayed, 
	it should not do any resource-intensive computing. 
	@param data User-supplied data that was passed in the 
	call to AVMenuItemSetComputeVisibleProc, AVToolButtonSetComputeVisibleProc, 
	or AVAppSetHowToPanelComputeVisibleProc.
	@return true if the item is visible, false otherwise. 
	@see AVAppSetHowToPanelComputeVisibleProc 
	@see AVMenuItemSetComputeVisibleProc 
	@see AVToolButtonSetComputeVisibleProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVComputeVisibleProc)(void *data);


/**
	Callback that is called for each menu item enumerated by 
	AVMenubarAcquireMenuItemByPredicate. The first menu item 
	for which this callback returns true is acquired. 
	@param menuItem IN/OUT? The current menu item in the enumeration. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVMenubarAcquireMenuItemByPredicate. 
	@return true to acquire the current menu item and halt enumeration, 
	false to continue enumeration. 
	@see AVMenubarAcquireMenuItemByPredicate 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVMenuItemPredicate)(AVMenuItem menuItem, void *clientData);


/**
	Callback that is used to get the mega-tooltip help text
	for a toolbar button. 

	It is called whenever the mouse hovers long enough to display
	the mega-tooltip. 

	Each toolbar button can have its own AVGetTooltipProc, or
	they can be shared. 
	@param data IN/OUT? User-supplied data that was passed in the call 
	to AVToolButtonSetGetTooltipProc or AVToolButtonSetComputeEnabledProc. 
	@param enabled IN specifies whether the button is enabled or not
	
	@return true if the menu item, toolbar button, or tool is enabled, 
	false otherwise. 
	@see AVMenuItemSetComputeEnabledProc 
	@see AVToolButtonSetComputeEnabledProc 
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *AVComputeTooltipProc)(void *data, ASBool enabled);


/**
	Callback that is called whenever the mouse hovers long enough 
	to display the tooltip text. It returns text that is displayed 
	in the tooltip. 

	Each toolbar button can have its own tooltip procedure, 
	or they can be shared. 
	@param data User-supplied data that was passed in the 
	call to AVToolButtonSetNotifyTooltipProc.
	@see AVToolButtonSetNotifyTooltipProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVNotifyTooltipProc)(void *data);


/************************************************************************************\
|*																					*|
|*									AVDoc											*|
|*																					*|
\************************************************************************************/


/**
	Callback used by AVAppEnumDocs. It is called once for each 
	open AVDoc. 
	@param doc IN/OUT? The current document. Do not close this AVDoc 
	in this callback function. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppEnumDocs. 
	@return true to continue enumeration, false to halt enumeration. 	
	@see AVAppEnumDocs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocEnumProc)(AVDoc doc, void *clientData);

/**
	User-supplied callback that is passed in the call to AVDocEnumSelection. 
	It is called once for each 'item' in the selection. 

	AVDocEnumSelection calls the AVDocSelectionEnumSelectionProc 
	for the current selection's server to actually enumerate 
	the selection. 
	@param doc The document whose selection is enumerated. 
	
	@param clientData User-supplied data that was passed in 
	the call to AVDocEnumSelection. 
	@param aSelectedObject The selected 'item' currently being 
	enumerated. The format of the data is up to the selection 
	server. See Selection Types for a list of the data formats 
	for the Acrobat viewer's built-in selection servers.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see AVDocEnumSelection 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVSelectionEnumProc)(AVDoc doc, void *clientData, void *aSelectedObject);

/**
	User-supplied callback that is passed in the call to AVDocSelectionEnumPageRanges. 
	It is called once for each page in the selection, and consecutive 
	pages are grouped into a single page range. 

	NOTE: The page number numeric type has changed in Acrobat 
	6.0. 
	@param doc The document whose selection is enumerated. 
	
	@param clientData User-supplied data that was passed in 
	the call to AVDocSelectionEnumPageRanges. 
	@param firstPage The first page in a consecutive range 
	of pages with a selection. 
	@param lastPage The first page in a consecutive range 
	of pages with a selection.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see AVDocEnumSelection 
	@see AVDocSelectionEnumPageRanges 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVSelectionPageRangeEnumProc)(
	AVDoc doc, void *clientData, PDPageNumber firstPage, PDPageNumber lastPage);

/**
	Callback for AVDocSelectionServer. Returns the selection 
	type this server handles (for example, 'Text' or 'Bookmark'). 
	This information is used so that the Acrobat viewer knows 
	which selection server to call. 
	@return The selection type this selection server handles. 
	@see AVDocGetSelectionServerByType 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVDocSelectionGetTypeProc)( void);

/**
	Callback for AVDocSelectionServer. It is called when the 
	selection is set (for example, via AVDocSetSelection). 

	Along with whatever else this callback chooses to do, it 
	must highlight the specified selection (if requested), using 
	the selection server's AVDocSelectionHighlightSelectionProc 
	callback. 
	@param doc IN/OUT? The document containing the selection. 
	@param selData IN/OUT? The selection data being added. 
	@param highlight IN/OUT? If true, highlight the selection, false 
	otherwise. 
	@see AVDocSetSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionGettingSelectionProc)(AVDoc doc, void *selData, ASBool highlight);

/**
	Callback for AVDocSelectionServer. Adds the specified item 
	to the selection, highlights it, and returns the new selection 
	containing the newly-added item. 
	@param doc IN/OUT? The document containing the data to add to the 
	selection. 
	@param curData IN/OUT? Data representing the current selection. 
	Its format is specific to the selection server. 
	@param addData IN/OUT? Item to add to the selection. 
	@param highlight IN/OUT? true if the selection should be highlighted 
	(because it has not already been highlighted), false if 
	the selection should not be highlighted (because it has 
	already been highlighted by whoever called this callback). 
	See AVDocSetSelection for additional information on highlighting. 
	
	@return New selection data containing all current selections (that 
	is, the previous selection plus the newly-added selection), 
	or NULL if failure. If the selection server allows only 
	a single item to be selected at a time, clear the previous 
	selection, highlight the selection specified by addData 
	(if highlight is true), and simply return addData. 
	@see AVDocSelectionRemovedFromSelectionProc 
*/
typedef ACCBPROTO1 void* (ACCBPROTO2 *AVDocSelectionAddedToSelectionProc)( AVDoc doc, void *curData, void *addData, ASBool highlight);

/**
	Callback for AVDocSelectionServer. This method is called 
	by (among others) AVDocClearSelection, to let the selection 
	server responsible for the old selection do whatever cleanup 
	it needs. 

	Along with whatever else this callback chooses to do, it 
	must de-highlight the specified selection (if requested), 
	using the selection server's AVDocSelectionHighlightSelectionProc 
	callback. 
	@param doc The document whose selection is cleared. 
	@param selData The current selection data. 
	@param highlight If true, the selection specified by selData 
	should be de-highlighted, false otherwise.
	@see AVDocClearSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionLosingSelectionProc)( AVDoc doc, void* selData, ASBool highlight);

/**
	Callback for AVDocSelectionServer. De-highlights the old 
	item given in remData, and returns a new curData or NULL 
	if failure. 
	@param doc IN/OUT? The document in which an item is removed from 
	the selection. 
	@param curData IN/OUT? The current selection data. 
	@param remData IN/OUT? The item to remove from the selection. The 
	content and format of selData differs for each selection 
	server, and is up to the selection server's implementors. 
	@param highlight IN/OUT? If true, the item removed should be de-
	highlighted. If false, it should not. 
	@return The new selection data (after the specified item has been 
	removed). 
	@see AVDocSelectionAddedToSelectionProc 
*/
typedef ACCBPROTO1 void* (ACCBPROTO2 *AVDocSelectionRemovedFromSelectionProc)( AVDoc doc, void *curData, void *remData, ASBool highlight);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether the current selection type can perform a 'select 
	all' operation. This controls whether the Select All menu 
	item is enabled. 
	@param doc The document containing the current selection. 
	@param selData The current selection's data.
	@return true if 'select all' can be performed on the current selection 
	type, false otherwise. 
	@see AVDocSelectionSelectAllProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanSelectAllProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. Selects all items of 
	the current type. 
	@param doc IN/OUT? The document in which the 'Select All' is performed. 
	@param selData IN/OUT? The current selection data in doc. 
	@return The new selection data, after all items of the specified 
	type have been selected. 
	@see AVDocSelectionCanSelectAllProc 
*/
typedef ACCBPROTO1 void* (ACCBPROTO2 *AVDocSelectionSelectAllProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether the current selection has user-specified properties. 
	This controls whether the 'Properties? menu item is enabled. 
	
	The 'Properties? menu item will not be enabled if the selection 
	server does not have a AVDocSelectionPropertiesProc callback. 
	
	@param doc The document containing the current selection. 
	@param selData The current selection data.
	@return true if the current selection has a Properties UI, false 
	otherwise. 
	@see AVDocDoSaveAsWithParams 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanPropertiesProc)( AVDoc doc, void* selData);

/**
	(Optional) Callback for AVDocSelectionServer. Displays the 
	'set properties' user interface, if any, for the selection 
	server and lets the user set the server's properties. This 
	callback is not needed unless the selection server has properties 
	that can be set by the user (for example, text highlight 
	color). This callback is called by AVDocDoSaveAsWithParams. 
	@param doc IN/OUT? The document in which the selection server's 
	properties are set. 
	@param selData IN/OUT? The current selection data. 
	@see AVDocDoSaveAsWithParams 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionPropertiesProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether the current selection can be deleted. This controls, 
	for example, whether the Delete menu item is enabled. 

	The Delete menu item is only enabled if the selection server's 
	AVDocSelectionCanDeleteProc returns true and the selection 
	server has an AVDocSelectionDeleteProc. 
	@param doc The document containing the current selection. 	
	@param selData The current selection data.
	@return true if the current selection can be deleted, false otherwise. 
	@see AVDocSelectionCanDeleteProc 
	@see AVDocDeleteSelection 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanDeleteProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. Deletes the current selection. 
	
	@param doc IN/OUT? Document whose selection is deleted. 
	@param selData IN/OUT? The current selection in doc. 
	@return true if the data was actually deleted, false otherwise. 
	@see AVDocDeleteSelection 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionDeleteProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether or not the current selection can be copied. This 
	controls, for example, whether or not the Copy menu item 
	is enabled. 

	The Copy menu item is only enabled if the selection server's 
	AVDocSelectionCanCopyProc returns true and the selection 
	server has an AVDocSelectionCopyProc. 
	@param doc IN/OUT? The document containing the selection. 
	@param selData IN/OUT? The current selection data. 
	@return true if the current selection can be copied, false otherwise. 
	@see AVDocSelectionCopyProc 
	@see AVDocCopySelection 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanCopyProc)( AVDoc doc, void* selData);

/**
	Callback for AVDocSelectionServer. It copies the selected 
	item to the clipboard. The Acrobat viewer will have already 
	cleared the clipboard and placed some private data onto 
	it so that it can identify the selection server that put 
	data onto the clipboard. Because of this, a plug-in must 
	not clear the clipboard, but only add its private data. 
	In addition, if the current selection can reasonably be 
	represented as text, plug-ins are strongly encouraged to 
	place a text representation of the selection onto the clipboard, 
	in addition to their own private format. 
	@param doc IN/OUT? The document whose selection is copied. 
	@param selData IN/OUT? The current selection data in doc. 
	@return true if the data was actually copied, false otherwise. 
	@see AVDocCopySelection 
	@see UnixAppClipboardGetItemId 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCopyProc)( AVDoc doc, void* selData);

/**
	(Optional) Callback for AVDocSelectionServer. Called by 
	AVDocEnumSelection. This callback enumerates the current 
	selection, calling the specified AVSelectionEnumProc for 
	each 'item' in the selection (the selection server is free 
	to decide what constitutes an item). 

	If omitted, the selection is enumerated by calling proc 
	once, passing the entire selection to it. 
	@param doc IN/OUT? The document whose selection is enumerated. 	
	@param data IN/OUT? The current selection in doc. 
	@param proc IN/OUT? The procedure to call for each item in the 
	selection. This callback must halt enumeration if proc returns 
	false, and continue enumeration if proc returns true. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVDocEnumSelection. Pass this as the clientData 
	each time proc is called. 
	@see AVDocSelectionEnumPageRangesProc 
	@see AVDocEnumSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionEnumSelectionProc)( AVDoc doc, void *data, AVSelectionEnumProc proc, void *clientData);

/**
	Callback for AVDocSelectionServer. Changes the view (for 
	example, by scrolling the current page or moving to the 
	appropriate page) so that the current selection is visible. 
	
	@param doc IN/OUT? The document whose selection is displayed. 
	@param data IN/OUT? The current selection data in doc. 
	@see AVDocShowSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionShowSelectionProc)( AVDoc doc, void *data);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether the current selection can be cut. This controls, 
	for example, whether the Cut menu item is enabled. 

	The Cut menu item is only enabled if the selection server's 
	AVDocSelectionCanCutProc returns true and the selection 
	server has an AVDocSelectionCutProc. 
	@param doc The document containing the current selection. 	
	@param data The current selection data.
	@return true if the current selection can be cut, false otherwise. 	
	@see AVDocSelectionCutProc 
	@see AVDocSelectionCanPasteProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanCutProc)( AVDoc doc, void* data);

/**
	Callback for AVDocSelectionServer. Cuts the current selection. 
	See the discussion under AVDocSelectionCopyProc for information 
	on how the selection server must use the clipboard. 
	@param doc IN/OUT? Document whose selection is cut. 
	@param data IN/OUT? The current selection data in doc. 
	@return true if the data was actually cut, false otherwise. 
	@see UnixAppClipboardGetItemId 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCutProc)( AVDoc doc, void* data);

/**
	Callback for AVDocSelectionServer. It is used to determine 
	whether or not the current selection can be pasted. This 
	controls, for example, whether or not the Paste menu item 
	is enabled. 

	The Paste menu item is only enabled if the selection server's 
	AVDocSelectionCanPasteProc returns true and the selection 
	server has an AVDocSelectionPasteProc. 
	@param doc IN/OUT? The document into which the selection is pasted. 	
	@return true if the data currently on the clipboard can be pasted, 
	false otherwise. 
	@see AVDocSelectionPasteProc 
	@see AVDocSelectionCanCutProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanPasteProc)( AVDoc doc );

/**
	Callback for AVDocSelectionServer. Pastes the current selection 
	from the clipboard. 
	@param doc IN/OUT? Document into whose selection the clipboard 
	is pasted. 
	@see AVDocSelectionCutProc 
	@see AVDocSelectionCanPasteProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionPasteProc)( AVDoc doc );

/**
	(Optional) Callback for AVDocSelectionServer. Handles a 
	key press. Needed only if the selection server processes 
	key presses. 

	NOTE: The key and flags numeric types have changed in Acrobat 
	6.0. 
	@param doc The document in which the click occurred. 
	@param data The current selection data for doc. 
	@param key The key that was pressed. 
	@param flags Modifier keys that were pressed with key. 
	Must be an OR of the Modifier Keys values.
	@return true if it the keypress was handled, false if it was not 
	and therefore needs to be passed to the next procedure in 
	the key handling chain. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionKeyDownProc)( AVDoc doc, void* data, AVKeyCode key, AVFlagBits16 flags);

/**
	NOTE: Superceded by AVDocSelectionHighlightSelectionExProc 
	in Acrobat 6.0. 

	(Previously known as AVDocHighlightSelectionProc) Callback 
	for AVDocSelectionServer. It highlights the selection. This 
	method is unnecessary if the selection type highlights itself 
	as, for example, annotations do. 
	@param doc The document containing the selection. 
	@param data The current selection data. Its content and 
	organization is up to the selection server for the current 
	selection type.
	@see AVDocSelectionHighlightSelectionExProc 
	@see AVDocSelectionGettingSelectionProc 
	@see AVDocSelectionLosingSelectionProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionHighlightSelectionProc)(AVDoc doc, void* data);

/* When this call is made you should highlight your selection. It is
   normally called when an update event is being processed for a page
   view. The page view passed into this call is the one being updated
   and might not be the active page view returned by AVDocGetPageView.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionHighlightSelectionExProc)(AVDoc doc, AVPageView pageView, void* data);

/**
	Callback for AVDocSelectionServer. It provides a way for 
	a single selection server to register different selection 
	types based on the selection data. If this callback is not 
	supplied, the selection type defaults to the return value 
	from AVDocSelectionGetTypeProc. 

	This callback does not affect existing selection servers. 
	
	@param doc IN/OUT? The document containing the selection. 
	@param data IN/OUT? The current selection data. Its content and 
	organization is up to the selection server for the current 
	selection type. 
	@see AVDocSelectionGetTypeProc 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVDocSelectionGetSelectionTypeProc)(AVDoc doc, void* data);

/**
	Callback for AVDocSelectionServer. It allows enumeration 
	of the set of pages the selection covers. 
	@param doc IN/OUT? The document containing the selection. 
	@param selectionData IN/OUT? The current selection data. Its content 
	and organization is up to the selection server for the current 
	selection type. 
	@param enumProc IN/OUT? The current selection data. Its content 
	and organization is up to the selection server for the current 
	selection type. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVDocSelectionEnumPageRanges. 
	@see AVDocSelectionEnumSelectionProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionEnumPageRangesProc)(
	AVDoc doc, void *selectionData,
	AVSelectionPageRangeEnumProc enumProc, void *clientData);

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Called to identify the bounding rectangle of a selection. 
	Used by the Info palette to display the width and height 
	of the selection. 

	Rectangle coordinates are in device space. 

	NOTE: The page number numeric type has changed in Acrobat 
	6.0. 
	@param doc The document containing the selection. 
	@param pageNo The number of the page containing the bounding 
	rectangle. 
	@param rect (Filled by the callback) The bounding rect 
	of the selection. 
	@param data Server-dependent selection data.
	@return true if the bounding rect was successfully determined, false 
	otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionGetAVRectProc)(AVDoc doc, PDPageNumber pageNo, AVDevRect* rect, void* data);

/**
	Called to request that the selection server display a context 
	menu appropriate for the current selection. 

	The given coordinates provide a suggested location for displaying 
	the menu and are in device space for the current AVPageView. 
	
	NOTE: The coordinate types have changed in Acrobat 6.0. 
	
	@param doc The document containing the selection. 
	@param data Server-dependent selection data. 
	@param x The x-coordinate of the point specifying the 
	upper left corner of the menu. 
	@param y The y-coordinate of the point specifying the 
	upper left corner of the menu.
	@return true if the server showed the menu successfully, false otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionShowMenuProc)(AVDoc doc, void *data, AVDevCoord x, AVDevCoord y);
#endif

/**
	Callback for AVDocSelectionServer that gets a quad-based 
	region for the selection. 

	NOTE: This procedure is new in Acrobat 6.0. 
	@param doc The document containing the selection. 
	@param data The current selection data. Its content and 
	organization is up to the selection server for the current 
	selection type. 
	@param pageNum The page number of the selection. 
	@param quads (Filled by the method.) A pointer to an array 
	of quads, or NULL. If non-NULL, the selection server allocates 
	an array of ASFixedQuads in user space describing the selection, 
	and stores a pointer to the array at *quad. If NULL, the 
	selection server returns the number of quads in the selection 
	without allocating an array. 
	@return The number of quads in the selection. 
*/
typedef ACCBPROTO1 ASArraySize (ACCBPROTO2 *AVDocSelectionAcquireQuadsProc)(AVDoc doc, void *data, ASInt32 pageNum, ASFixedQuad **quads);

/**
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionCanPasteFromPlatformProc)( AVDoc doc );

/**
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVDocSelectionPasteFromPlatformProc)( AVDoc doc );

/**
	(Optional) Callback for AVDocSelectionServer. Handles a 
	key press using Unicode values. Needed only if the selection server
	processes key presses. 

	If this callback is provided all key events will be passed to
	it; the KeyDown callback will never be called.

	The application may pass several Unicode characters to the
	UnicodeKeyDown callback. For example, if the user invokes
	an IME to compose a string of text the entire text string
	may be passed to the UnicodeKeyDown callback at once. This
	behavior may vary across platforms, also. If the callback
	returns true the application assumes the selection server
	processed all of the characters; if the callback returns
	false the viewer assumes the selection server processed
	none of the characters.

	@param doc The document in which the keyboard event occurred. 
	@param data The current selection data for doc. 
	@param numUTF16Vals The number of UTF-16 values being passed.
	@param utf16Vals The host-endian UTF-16 values.
	@param flags Modifier keys that were pressed with key. 
	Must be an OR of the Modifier Keys values.
	@return true if it the keypress was handled, false if it was not 
	and therefore needs to be passed to the next procedure in 
	the key handling chain.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVDocSelectionUnicodeKeyDownProc)( AVDoc doc, void* data, ASCount numUTF16Vals, const ASUTF16Val* utf16Vals, AVFlagBits16 flags);

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure containing callbacks that implement a selection 
	server. The callbacks implement the selection server functions. 
	For example, add an item to the selection, remove an item 
	from the selection, copy the current selection to the clipboard. 
	
	@see AVDocRegisterSelectionServer 
	@see AVDocGetSelectionServerByType 
*/
typedef struct _t_AVDocSelectionServer {


	/** Size of the data structure. Must be set to sizeof(AVDocSelectionServerRec).*/
	ASSize_t 							size;	

	/** */
	AVDocSelectionGetTypeProc				GetType;

	/** */
	AVDocSelectionGettingSelectionProc		GettingSelection;

	/** */
	AVDocSelectionAddedToSelectionProc		AddedToSelection;

	/** */
	AVDocSelectionLosingSelectionProc		LosingSelection;

	/** */
	AVDocSelectionRemovedFromSelectionProc	RemovedFromSelection;

	/** */
	AVDocSelectionCanSelectAllProc			CanSelectAll;

	/** */
	AVDocSelectionSelectAllProc				SelectAll;

	/** */
	AVDocSelectionCanPropertiesProc			CanProperties;

	/** */
	AVDocSelectionPropertiesProc			Properties;

	/** */
	AVDocSelectionCanDeleteProc				CanDelete;

	/** */
	AVDocSelectionDeleteProc				Delete;

	/** */
	AVDocSelectionCanCopyProc				CanCopy;

	/** */
	AVDocSelectionCopyProc					Copy;

	/** */
	AVDocSelectionEnumSelectionProc			EnumSelection;

	/** */
	AVDocSelectionShowSelectionProc			ShowSelection;

	/** */
	AVDocSelectionCanCutProc				CanCut;

	/** */
	AVDocSelectionCutProc					Cut;

	/** */
	AVDocSelectionCanPasteProc				CanPaste;

	/** */
	AVDocSelectionPasteProc					Paste;

	/** */
	AVDocSelectionKeyDownProc				KeyDown;

	/** */
	AVDocSelectionHighlightSelectionProc	HighlightSelection;

	/** */
	AVDocSelectionGetSelectionTypeProc		GetSelectionType;

	/** */
	AVDocSelectionEnumPageRangesProc		EnumPageRanges;

	/** obsolete */
	oldAVDocSelectionGetAVRectProc			oldGetAVRect; 

	/** obsolete */
	oldAVDocSelectionShowMenuProc			oldShowMenu; 

	/** */
	AVDocSelectionGetAVRectProc				GetAVRect;

	/** */
	AVDocSelectionShowMenuProc				ShowMenu;

	/** */
	AVDocSelectionHighlightSelectionExProc	HighlightSelectionEx;

	/** */
	AVDocSelectionAcquireQuadsProc			AcquireQuads;

	/** */
	AVDocSelectionCanPasteFromPlatformProc		CanPasteFromPlatform;

	/** */
	AVDocSelectionPasteFromPlatformProc			PasteFromPlatform;
	
	/** */
	AVDocSelectionUnicodeKeyDownProc			UnicodeKeyDown;

} AVDocSelectionServerRec, *AVDocSelectionServer;
#endif



/**@name AVDocViewDef
* type which defines a view of a document, including
* page, zoom, and everything else you could ever want.
*/
/*@{*/
/* Keys for the new Cab-based ViewDef; for use with the ViewDefEx methods */

/** boolean */
#define kAVDocViewDefKeyBringToFront		"bringToFront"				

/** int */
#define kAVDocViewDefKeyPageViewLayoutMode	"pageViewLayoutMode"	

/** int */
#define kAVDocViewDefKeyPageViewPageNum		"pageViewPageNum"		

/** int */
#define kAVDocViewDefKeyPageViewZoomType	"pageViewZoomType"		

/** int */
#define kAVDocViewDefKeyPageViewZoom		"pageViewZoom"			

/** int */
#define kAVDocViewDefKeyPageViewX			"pageViewX"				

/** int */
#define kAVDocViewDefKeyPageViewY			"pageViewY"			

/** int */
#define kAVDocViewDefKeyPageViewStartThread	"pageViewStartThread"		

/** int */
#define kAVDocViewDefKeyPageViewThreadIndex	"pageViewThreadIndex"	

/** binary: PDBead */
#define kAVDocViewDefKeyPageViewBead		"pageViewBead"				

/** int */
#define kAVDocViewDefKeyOverViewMode		"overViewMode"			

/** int */
#define kAVDocViewDefKeyOverViewPos			"overViewPos"			

/** int */
#define kAVDocViewDefKeyOverViewX			"overViewX"				

/** int */
#define kAVDocViewDefKeyOverViewY			"overViewY"			

/** binary: AVRect */
#define kAVDocViewDefKeyWindowFrame			"windowFrame"		

/** cab */
#define kAVDocViewDefKeyOCGStates			"ocgStates"				


#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Structure that defines a view of a document, including page, 
	zoom, and so on. 

	NOTE: Numeric types have changed in Acrobat 6.0, and the 
	AVDocGetViewDef and AVDocSetViewDef methods have been superceded 
	by AVDocGetViewDefEx and AVDocSetViewDefEx. 
	@see AVDocGetViewDef 
	@see AVDocSetViewDef 
*/
typedef struct _t_AVDocViewDef
{

	/** Size of the data structure. Must be set to sizeof( AVDocViewDef). */
	ASSize_t	size;


	/** If true, bring window to front; if false, don't bring window to front. */
	ASBool		bringToFront;


	/** If true, use the next 6 page view fields. If false, it does not use them. */
	ASBool			usePageViewInfo;	/* pageview info */

	/** Page layout mode; must be one of PDLayoutMode. @see PDLayoutMode */
	PDLayoutMode	pageViewLayoutMode;

	/** Page number. */
	PDPageNumber	pageViewPageNum;

	/** Zoom type; must be one of AVZoomType. @see AVZoomType */
	AVZoomType 		pageViewZoomType;

	/** Zoom factor; used if pageViewZoomType is AVZoomNoVary. Use zero to inherit zoom. */
	ASFixed 		pageViewZoom;

	/** The x-coordinate to scroll to. */
	AVDevCoord			pageViewX;

	/** The y-coordinate to scroll to. */
	AVDevCoord			pageViewY;


	/** If true, use the next two article thread fields. If false, it does not use them. */
	ASBool			pageViewStartThread;

	/** Current thread index. */
	AVPageIndex		pageViewThreadIndex;

	/** Current PDBead. */
	PDBead 			pageViewBead;


	/** If true, use the next four view fields. If false, it does not use them. */
	ASBool		useOverViewInfo;	

	/** The PDPageMode to use. @see PDPageMode */
	PDPageMode	overViewMode;			

	/** Position of splitter. */
	AVPixelOffset	overViewPos;   

	/** The x-coordinate to scroll to in bookmark or thumbnail pane.*/
	ASInt32		overViewX;

	/** The y-coordinate to scroll to in bookmark or thumbnail pane. */
	ASInt32		overViewY;

	/** If true, use the windowFrame field. If false, it does not use them.*/
	ASBool		useWindowInfo;	

	/** New window frame in which to display the document.*/
	AVScreenRect	windowFrame;				

	/** Currently unused. */
	ASBool			unused1;		/* obsolete */

	/** Currently unused. */
	const char*		unused2;		/* obsolete */

} AVDocViewDefRec, *AVDocViewDef;

#endif


/**
	Data structure representing a destination in a PDF document. 
	An AVDestInfo carries all the information that a PDViewDestination 
	can. Used for ensuring that cross-document links in external 
	windows act as expected, so a client can go to a destination 
	without building it via PDViewDestCreate, which does not 
	work on read-only documents. 
	@see AVPageViewToDestInfo 
	@see AVPageViewUseDestInfo 
	@see AVDestInfoDestroy 
*/
typedef struct _t_AVDestInfo
{

	/** Size of the data structure. Must be set to sizeof( AVDestInfo).*/
	ASSize_t		size;
	

	/** The named destination associated with this destination. If this is non-NULL, the other attributes 
	are ignored. This destination may contain multi-byte characters.*/
	const char*		namedDest;

	/** Length of namedDest, in bytes. */
	AVTArraySize   nameLength;


	/** The page number of the destination view. */
	PDPageNumber	pageNum;

	/** The fit type of the destination view. Must be one of View Destination Fit Types. */
	ASAtom			fitType;

	/** A rectangle enclosing the destination view. */
	ASFixedRect		destRect;

	/** The zoom factor of the destination view. Use zero to inherit the zoom. */
	ASFixed			zoom;
	
} AVDestInfoRec, *AVDestInfo;


/**
	A callback that can be associated with an AVDoc when it 
	is opened (via an AVDocOpenParamsRec). It can restrict the 
	set operations allowed on the document. When AVDocPermRequest 
	is called, this callback is be consulted to deny or grant 
	the permission. If it denies permission, AVDocPermRequest 
	will also deny permission. If it grants permission, the 
	security handler for the document will be consulted to determine 
	the result of AVDocPermRequest. This callback can only deny 
	permissions allowed by the security handler; it cannot grant 
	permissions that the security handler does not grant. 
	@param doc IN/OUT? The current document. 
	@param obj IN/OUT? Description of target object. 
	@param opr IN/OUT? Description of target operation. 
	@return The status. 
*/
typedef ACCBPROTO1 PDPermReqStatus (ACCBPROTO2 *AVDocPermReqProc)(AVDoc doc, PDPermReqObj obj, PDPermReqOpr opr);

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Parameters used when opening a file. 

	In UNIX, it is not possible to set the frame of the NULL 
	document (that is, the window to show when no document is 
	open) using this data structure. 
	@see AVAppOpenHelpFileWithParams 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams
*/
typedef struct _t_AVDocOpenParams
{

	/** Size of the data structure. Must be set to sizeof( AVDocOpenParamsRec). */
	ASSize_t		size;
	

	/** If true, frame is used to specify the size 
		and location of the window into which the document is opened. 
		If false, frame is ignored and the default frame is used 
		instead. See also visible. 
	*/
	ASBool			useFrame;	

	/** An AVRect specifying the size and location 
		of the window into which the document is opened. In Mac 
		OS, the coordinates are global screen coordinates. In Windows, 
		the coordinates are MDI client coordinates. See also visible.
	*/
	AVScreenRect    frame;		
	

	/** If true, visible is used to determine 
		whether or not the window is visible after the document 
		is opened. If false, visible is ignored. See also visible.
	*/
	ASBool			useVisible;	

	/** Specifies the window's visibility. If visible 
		is false and useVisible is true, frame is ignored - regardless 
		of the setting of useFrame. In Mac OS, if true, the document 
		is opened into a visible window. If false, the document 
		is opened into a hidden window. In Windows, if true, the 
		document is opened in a visible window. If false, the document 
		is opened in a minimized window. 
	*/
	ASBool			visible;		

	/** Indicates whether the serverType and serverCreationData fields are used. */
	ASBool			useServerType;			

	/** Name of AVDoc server for this AVDoc: 
		'EXTERNAL' - the AVDoc server for an external window 
	*/
	const char*		serverType;				

	/** Platform-dependent server data 
		to associate with the AVDoc server. For a serverType of 
		'EXTERNAL', must be of type ExternalDocServerCreationData
	*/
	void*			serverCreationData;		


	/** Indicates whether the sourceDoc field is used. */
	ASBool			useSourceDoc;		

	/** AVDoc whose window will be taken over 
		by new document. sourceDoc will be closed at the same time.
	*/
	AVDoc			sourceDoc;				
											
	/** Indicates whether the readOnly field is used. */
	ASBool			useReadOnly;		

	/** If true, open the document in read-only mode. */
	ASBool			readOnly;			

	/** Indicates whether the viewType field is used. */
	ASBool			useViewType;			

	/** Type of view to open on document. Permissible values are: 
		- 'AVPageView' - Displays only the AVPageView, that is, the window that displays the PDF file. Does not 
		display scrollbars, the toolbar, and bookmark or thumbnails	pane. Annotations, such as links, are active.
		- 'AVDocView' - Displays the AVPageView, scrollbars, and bookmark or thumbnails pane. Annotations, such as 
		links, are active.
		- 'AVExternalView' - Displays the AVPageView, scrollbars, toolbar, and bookmark or thumbnails pane. Annotations, 
		such as links, are active.
		- 'AVEmbeddedView' - Embeds the PDF file in an external document, an HTML file, for example. Show the first page of the PDF 
		file; no scrollbars, the toolbar, and bookmark or thumbnails pane are visible. Annotations, such as links, are neither 
		displayed nor active.
	*/
	const char* 	viewType;				

	/** Indicates whether the viewDef field is used. */
	ASBool			useViewDef;				

	/** Initial view with which to open the document. Must be an AVDocViewDef */
	AVDocViewDef	viewDef;			
	
	/** Boolean indicating whether the permReqProc field be used. */
	ASBool			usePermReqProc;			

	/** Return PDPermReqDenied to deny a permission, or PDPermReqGranted to grant one. */
	AVDocPermReqProc permReqProc;			

	/** Expanded and more flexible version of the viewDef field. If both are specified, the older AVDocViewDef takes precedence */
	ASCab			 openActions;			
											
	/** Should we suppress any non-alert dialogs that may be triggered by opening the doc? */
	ASBool			suppressDialogs;		
}
AVDocOpenParamsRec, *AVDocOpenParams;
#endif

/* Data types for "EXTERNAL" serverType documents */

/**
	Callback in ExternalDocServerCreationData. Called when a 
	cross-document link is clicked in an AVDoc in an external 
	application's window. 
	@param path Path to document to which the link points. 
	@param fileSys The ASFileSys with which to open document. 
	@param viewDef The AVDocViewDef with which to open document. 
	@param srcDoc The AVDoc that contains the cross-document 
	link. 
	@param data User-supplied data that was passed in the 
	call to CrossDocLinkProc.
	@return The AVDoc for the new document. 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef ACCBPROTO1 AVDoc (ACCBPROTO2 *CrossDocLinkProc)(ASPathName path, ASFileSys fileSys, AVDocViewDef viewDef, AVDoc srcDoc, void* data);

/**
	(Unused) Callback in ExternalDocServerCreationData for opening 
	PDF files in external windows. 
	@param msg IN/OUT? Message for external application. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVSetMessageProc. 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSetMessageProc)(char* msg, void* clientData);

/**
	Callback in ExternalDocServerCreationData to return focus 
	to the browser displaying the document. 
	@param clientData IN/OUT? User-supplied data. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSetFocusProc)(void* clientData);

/**
	Callback in ExternalDocServerCreationData. Called when a 
	cross-document link is clicked in an AVDoc in an external 
	application's window. 
	@param path IN/OUT? Path to document to which the link points. 
	@param fileSys IN/OUT? The ASFileSys with which to open document. 
	@param viewDef IN/OUT? The AVDocViewDef with which to open document. 
	@param destInfo IN/OUT? A destination in a PDF document. 
	@param srcDoc IN/OUT? The AVDoc that contains the cross-document 
	link. 
	@param data IN/OUT? User-supplied data that was passed in the call 
	to CrossDocLinkProc. 
	@return The AVDoc for the new document. 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef ACCBPROTO1 AVDoc (ACCBPROTO2 *CrossDocLinkWithDestProc)(
	ASPathName		path,
	ASFileSys 		fileSys,
	AVDocViewDef 	viewDef,
	AVDestInfo		destInfo,
	AVDoc			srcDoc,
	void* 			data);

#if WIN_PLATFORM
/** */
typedef HWND ExternalDocWindowData;

#elif MAC_PLATFORM


/**
	(Macintosh only) Data for an external window. Platform-dependent 
	structure used in ExternalDocWindowData when opening an 
	AVDoc with AVDocOpenFromASFileWithParamString, AVDocOpenFromASFileWithParams, 
	or AVDocOpenFromPDDocWithParams. 

	Coordinates specified in this structure are in application 
	space. Use AVRectToRect to translate from user space to 
	device space coordinates, then use the Macintosh GlobalToLocal 
	function to translate from device space coordinates to application 
	space coordinates. 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef struct _t_ExternalDocWindowRefData
{

	/** WindowPtr for the external window. */
	WindowPtr 	*window;

	/** x-displacement in application space coordinates 
		from the application's window to the AVPageView in which 
		Acrobat renders the PDF file.
	*/
	ASUns32		*x;

	/** y-displacement in application space coordinates 
		from the application's window to the AVPageView in which 
		Acrobat renders the PDF file.
	*/
	ASUns32		*y;

	/** Width of external window, specified in device space units. */
	ASUns32		*width;

	/** Height of external window, specified in device space units. */
	ASUns32		*height;

	/** Clipping rectangle (Macintosh Rect) for external window in application space units. Usually the entire window. */
	Rect		*clip;

	/** x-displacement in application space coordinates from the AVPageView in which Acrobat renders the PDF file 
		to the actual PDF file page. Should usually be 0.
	*/
	ASInt32	*portx;

	/** y-displacement in application space coordinates from the AVPageView in which Acrobat renders the PDF file 
		to the actual PDF file page. Should usually be 0.
	*/
	ASInt32 *porty;

} ExternalDocWindowRefDataRec, *ExternalDocWindowRefData;


/**
	(Macintosh only) Callback in ExternalDocWindowData for opening 
	PDF files in external windows. Called for a mouse-related 
	event, such as mouse down or mouse movement. 
	@param curs Handle to cursor. It is a CursHandle. 
	@param clientData User-supplied data that was passed in 
	ExternalDocWindowData.
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSetCursorProc)(CursHandle curs, void* clientData);


/**
	Data for an AVDoc in an external window. Platform-dependent 
	structure used in ExternalDocServerCreationData when opening 
	an AVDoc with AVDocOpenFromASFileWithParamString, AVDocOpenFromASFileWithParams, 
	or AVDocOpenFromPDDocWithParams. 

	In Mac OS, a plug-in must handle events that effect the 
	window, such as resize and mouse events. 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef struct _t_ExternalDocWindowData
{

	/** Pointer to external window data. Must be of type ExternalDocWindowRefData. */
	ExternalDocWindowRefDataRec	*ref;


	/** Callback for handling mouse-related events, such as mouse down or mouse movement.*/
	AVSetCursorProc		setCursor;

	/** Optional client-specified data  */
	void*							setCursorProcData;
	
} ExternalDocWindowDataRec, *ExternalDocWindowData;

#elif UNIX_PLATFORM

typedef Window ExternalDocWindowData;

#elif OS2_PLATFORM

typedef HWND ExternalDocWindowData;

#endif

/**
	Data for an AVDoc in an external window. Platform-dependent 
	structure used in AVDocOpenParams when opening an AVDoc 
	with AVDocOpenFromASFileWithParamString, AVDocOpenFromASFileWithParams, 
	or AVDocOpenFromPDDocWithParams. 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDocWithParams 
*/
typedef struct _t_ExternalDocServerCreationData
{
	/** Size of the data structure. Must be set to sizeof( ExternalDocServerCreationData Rec) */
	ASSize_t 					size;

	/** platformWindow Platform-dependent structure of type ExternalDocWindowData representing a window. 
		- Macintosh - ExternalDocWindowData structure
		- Windows - HWND cast as ExternalDocWindowData
		- UNIX - Widget cast as ExternalDocWindowData 
	*/
	ExternalDocWindowData 		platformWindow;

	/** Optional callback. Called when the "Acrobat button" (if present) is clicked in the external 
		application.
	*/
	AVExecuteProc				acrobatProc;

	/** Client-specified data for acrobatProc. */
	void* 						acrobatProcData;

	/** Callback of type CrossDocLinkProc */
	CrossDocLinkProc			crossDocLinkProc;

	/** Client-specified data for crossDocLinkProc. */
	void*						crossDocLinkProcData;

	/** Currently unused. */
	AVSetMessageProc			setMessage;

	/** Currently unused. Client-specified data for setMessage. */
	void*						setMessageProcData;

	/** Callback to call when a cross-document link occurs. */
	CrossDocLinkWithDestProc	crossDocLinkWithDestProc;

	/** Client-specified data for crossDocLinkWithDestProc.*/
	void*						crossDocLinkWithDestData;


	/** (New in Acrobat 5.0) Callback to call when Acrobat wishes to return focus to the browser displaying 
		the document.
	*/
	AVSetFocusProc			setFocus;

	/** (New in Acrobat 5.0) Client-specified data for setFocus. */
	void*						setFocusProcData;

	/** (New in Acrobat 7.0) for extended TAB handling in browser*/
	AVSetFocusProc			setFocusSHIFTTAB;
	void*					setFocusSHIFTTABProcData;

	AVSetFocusProc			setFocusCTRLTAB;
	void*					setFocusCTRLTABProcData;

} ExternalDocServerCreationDataRec, *ExternalDocServerCreationData;


/**
	Constant values indicating a type of document server being 
	used for a document. 
	@see AVDocGetServerType 
*/
typedef enum _t_AVDocServerType {
	/** Unknown server type. */
	AVDocServerUnknown,				
	/** The default document server, used for most documents. */
	AVDocServerDefault,	
	/** The same as the default document server. */
	AVDocServerInternal = AVDocServerDefault,				
	/** Used for external documents, shown in a web browser. */
	AVDocServerExternal,				
	/** A server used for documents displayed in the help window. */
	AVDocServerHelp					
} AVDocServerType;

/**
   Specifies a special value for
   AVDocSetSplitterPosition/AVDocGetSplitterPosition.
   
   @see AVDocSetSplitterPosition
   @see AVDocGetSplitterPosition
*/
#define kAVDocSplitterPinnedLeft (0)
/**
   Specifies a special value for
   AVDocSetSplitterPosition/AVDocGetSplitterPosition.
   
   @see AVDocSetSplitterPosition
   @see AVDocGetSplitterPosition
*/
#define kAVDocSplitterPinnedRight (ASMAXInt16)

/************************************************************************************\
|*																					*|
|*								AVAnnotHandler										*|
|*																					*|
\************************************************************************************/

/* Flags returned by AVAnnotHandler->GetFlags */
#define AV_ANNOT_POPUPISREADONLY			0x0001
#define AV_ANNOT_SHOW_OFFSCREEN_INDICATOR	0x0002
#define AV_ANNOT_SUPPORTS_REPLIES			0x0004
#define AV_ANNOT_SUPPRESS_POPUP				0x0008

/*------------------------------------------------------------------------
   The layer parameter of Annotation Handlers defines the draw order and (in reverse)
   the order of mouse hit detection.  Valid ranges are fixedZero - fixedPositiveInfinity.
   Notice that any form of non-update driven drawing (like during a DoClick method) 
   can make an annotation in a lower level appear to be above those in higher levels.
 ------------------------------------------------------------------------*/
/** Links live at this level */  
#define LINK_LAYER			fixedOne		
/** Closed Notes live here, with open notes just above */
#define NOTE_LAYER			fixedThree		
/** Set this bit of 'flags' to prevent the standard text selection tool from inverting your annotation */
#define ANNOT_CLIP_TEXT_SELECTION	0x0001	
/**Set this bit of 'flags' to prevent the standard shift-key ignores annots behaviour. */
#define ANNOT_WANTS_SHIFT_KEY		0x0002	
											

/**
	Structure used to describe information for a particular 
	annotation type. 
	@see AVAnnotHandlerDeleteInfoProc 
	@see AVAnnotHandlerGetInfoProc 
	@see AVAnnotHandlerGetInfo 
*/
typedef struct _AVAnnotHandlerInfoRec {

	/** Size of the data structure. Must be set to sizeof(AVAnnotHandlerInfoRec). */
	ASSize_t size;

	/** User interface name of annotation type in the host encoding. */
	unsigned char *cName;	

	/** Platform-dependent bitmap used as the annotation icon. If NULL, the annotation 
		manager uses the unknown annotation icon for the annotation. 
	*/
	void *vBitmap;			
} AVAnnotHandlerInfoRec, *AVAnnotHandlerInfo;

/* UI friendly name is in host encoding. Bitmap is platform dependent, if set
** to null, the annotation manager uses the Unknown annotation icon. */
#define AVAnnotHandlerInfoInit(x)				\
	do { \
	memset(x, 0, sizeof(AVAnnotHandlerInfoRec));	\
	x->size = sizeof(AVAnnotHandlerInfoRec); \
	}while(0)

/* Operations we can ask an annotation to perform */

/**
	An enumeration detailing operations that you can ask the 
	annotation to react to.

	Operation codes containing the word "Do" are sent to
	notify the annotation handler of a significant event.
	All other operation codes are sent to tell the annotation
	to perform a specific action.

	@see AVPageViewFocusAnnotPerformOp 
*/
enum {
	/** Accept input focus. */
	kAVAnnotAcceptFocus,		
	/** You lost the input focus. */
	kAVAnnotLostFocus,			
	/** The user hits Enter while you're the focus. */
	kAVAnnotDefaultAction,		 
	/** Show a context-menu for the annot. */
	kAVAnnotShowMenu,			
	/** Focus is temporarily lost; you may get it back. */
	kAVAnnotSuspendFocus,		
	/** Focus has been restored */
	kAVAnnotResumeFocus,
	/** The page the annotation is on has become the current page. */
	kAVAnnotDoPageHasOpened,
	/** The page the annotation is on is no longer the current page. */
	kAVAnnotDoPageHasClosed,
	/** This page this annot is on has become visible to the user. */
	kAVAnnotDoPageIsInView,
	/** This page this annot is on is no longer visible to the user. */
	kAVAnnotDoPageIsNotInView
};
#ifndef _T_AVANNOTOP
#define _T_AVANNOTOP
typedef ASEnum16 AVAnnotOp;
#endif /* _T_AVANNOTOP */

/* Additional information passed to the annotation when performing an
   operation. For some operations a NULL will be passed; in others a
   pointer to an AVAnnotOpData structure will be passed.
*/
#if (ACRO_SDK_LEVEL >= 0x00060000)
/** Constants that specify how an annotation operation request was triggered.
	@see AVPageViewFocusAnnotPerformOp
*/
typedef enum {
	/** first entry for memset of AVAnnotOpDataRec to 0 */
	kAVAnnotUnknown = 0, 
	/** Operation triggered by a mouse click. */
	kAVAnnotClick,
	/** Operation triggered by a Tab key press. */
	kAVAnnotTab
} AVAnnotOpReason;

/**
	Additional information passed to the annotation when performing 
	an operation. For some operations a NULL will be passed; 
	in others a pointer to an AVAnnotOpData structure will be 
	passed. 

	NOTE: The coordinate numeric type has changed in Acrobat 
	6.0. 
	@see AVPageViewSetFocusAnnot 
*/
typedef struct _t_AVAnnotOpData {
	/** Set by Acrobat to the size of this record. */
	ASSize_t size;
	/** If the operation is kAVAnnotShowMenu, provides the default location of the menu in AV device coordinates (x) */
	AVDevCoord x;
	/** If the operation is kAVAnnotShowMenu, provides the default location of the menu in AV device coordinates (y) */
	AVDevCoord y;
	/** Used by Forms and Annots to determine when an annotation is getting focus via a mouse click.*/
	void * clientData;
	/** Reason for this operation */
	AVAnnotOpReason reason;
	/** Page number this annotation is on. */
	ASInt32	pageNum;
} AVAnnotOpDataRec, *AVAnnotOpData;
#endif

  /* Concrete instantiation of an annot handler object. Annot handler implementations will
     usually "subclass" this by pass a ptr to struct with an AVAnnotHandlerRec as
     its first member to AVAppRegisterAnnotHandler, cast as an AVAnnotHandler.
   */

/* AVAnnotHandlerEnumProc -- used by AVAppEnumAnnotHandlers.
** If the proc returns false, the enumeration will terminate.
*/
#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure containing callbacks that implement an annotation 
	handler. The callbacks implement the annotation handler 
	functions. For example, draw the annotation, highlight the 
	annotation when it is selected, and the data specifies properties 
	of the annotation (for example, text selection behavior). 
	
	Appearance drawing is new in Acrobat 6.0. If the handler 
	implements the GetAppearance callback, the new drawing style 
	is used. If the GetAppearance callback returns false or 
	is not implemented, the viewer calls the drawing procedure 
	(DrawEx if present, otherwise Draw) to draw the annotation 
	using Acrobat 5.0-style drawing. 

	For the new style of appearance drawing, the viewer calls 
	the handler's BeginAppearanceDrawing callback, then draws 
	the appearance object. If the drawing operation completes 
	without errors, it call the the handler's BeginAppearanceDrawing 
	callback; if an error occurs, or the operation is canceled, 
	it calls the CancelAppearanceDrawing callback. This can 
	happen if, for example, the user switches to another page 
	before the drawing is completed. 

	The handler must not destroy or modify the appearance object 
	while it is being drawn. It can do so only after the EndAppearanceDrawing 
	or CancelAppearanceDrawing callback is exercised. 

	NOTE: Types of numeric values in callbacks have changed 
	in Acrobat 6.0. 
	@see AVAnnotHandlerEnumProc 
	@see PDAnnotHandlerDeleteAnnotInfoProc 
	@see PDAnnotHandlerGetAnnotInfoFlagsProc 
	@see PDAnnotHandlerGetAnnotInfoProc 
	@see PDAnnotHandlerGetTypeProc 
	@see AVAppRegisterAnnotHandler 
	@see AVAppGetAnnotHandlerByName 
	@see AVDocCopyAnnot 
*/
typedef struct _t_AVAnnotHandler *AVAnnotHandler;

/**
	Called to request that the annotation appearance be drawn. 
	
	@param annotHandler IN/OUT? The annotation handler responsible 
	for this annotation. 
	@param anAnnot IN/OUT? The annotation. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@param updateRect IN/OUT? The portion of the annotation's bounding 
	rect that should be drawn. 
	@return true if the callback completed drawing successfully, false 
	otherwise. 
	@see AVAnnotHandlerDrawProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerDrawExProc)(
	AVAnnotHandler annotHandler, 
	PDAnnot anAnnot, 
	AVPageView pageView,
	AVDevRect *updateRect);

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerDoClickExProc. 
	
	(Optional) Callback for AVAnnotHandler. It handles both 
	left and right mouse button clicks within the annotation. 
	If NULL, the annotation behaves as if the callback returned 
	false. 
	@param annotHandler The annotation handler responsible 
	for this annotation. 
	@param hitAnnot The annotation in which the mouse was 
	clicked. 
	@param pageView The AVPageView in which the annotation 
	is located. 
	@param xHit The x-coordinate of the mouse click. 
	@param yHit The y-coordinate of the mouse click. 
	@param flags Indicates which modifier keys are pressed. 
	Must be an OR of the Modifier Keys values. 
	@param clickNo 1 if this is a single click, 2 if a double 
	click, 3 if triple click.
	@return true if the callback handled the mouse click, false if it 
	did not. If the callback does not handle the click, it is 
	passed to any annotation at the same location in lower layers. 
	
	@see AVAnnotHandlerDoKeyDownExProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerDoClickProc)(AVAnnotHandler annotHandler, PDAnnot hitAnnot,
                     AVPageView pageView,
					AVDevCoord xHit, AVDevCoord yHit,
					AVFlagBits16 flags,
					AVTCount clickNo);

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerAdjustCursorExProc. 
	
	(Optional) Callback for AVAnnotHandler. It controls the 
	cursor shape when the cursor is within the annotation. If 
	NULL, the annotation behaves as if the AdjustCursor callback 
	returned false. 
	@param annotHandler The annotation handler responsible 
	for this annotation. 
	@param anAnnot The annotation containing the cursor. 
	@param pageView The AVPageView in which the annotation 
	is located. 
	@param xHit The cursor's current x-coordinate. 
	@param yHit The cursor's current y-coordinate.
	@return true if the callback handled the adjust cursor event, false 
	otherwise. The callback would return false, for example, 
	if the annotation is irregularly shaped and the cursor is 
	not currently over the real annotation even though it is 
	within the rectangular bounding box that the Acrobat viewer 
	uses to specify annotations. 
	@see AVAnnotHandlerAdjustCursorExProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerAdjustCursorProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot,
  					AVPageView pageView, AVDevCoord xHit, AVDevCoord yHit);

/**
	Callback for AVAnnotHandler. It is called by AVPageViewIsAnnotAtPoint 
	to determine whether a point is within an annotation. The 
	annotation handler is free to determine what it means for 
	the point to be 'in' the annotation. For example, if the 
	annotation appears only as the outline of a circle, the 
	point may be 'in' the annotation only when it is near the 
	border of the circle, but not elsewhere within the circle. 

	NOTE: The coordinate numeric types have changed in Acrobat 
	6.0. 
	@param annotHandler The annotation handler responsible 
	for this annotation. 
	@param pageView The AVPageView in which the annotation 
	appears. 
	@param anAnnot The annotation being tested. 
	@param xHit The x-coordinate of the point to test. 
	@param yHit The y-coordinate of the point to test.
	@return true if the point is in the annotation, false otherwise. 
	@see AVPageViewIsAnnotAtPoint 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerPtInAnnotViewBBoxProc)(AVAnnotHandler annotHandler, AVPageView pageView,
                   PDAnnot anAnnot, AVDevCoord xHit, AVDevCoord yHit);
                  

/**
	Callback for AVAnnotHandler. It returns the rectangle enclosing 
	the annotation on the screen. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for the annotation. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@param anAnnot IN/OUT? The annotation whose bounding box is returned. 
	
	@param bbox IN/OUT? (Filled by the callback) The annotation's bounding 
	rectangle. 
	@see AVAnnotHandlerPtInAnnotViewBBoxProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerGetAnnotViewBBoxProc)(AVAnnotHandler annotHandler, AVPageView pageView,
                   PDAnnot anAnnot, AVDevRect *bbox); 
#endif /* (ACRO_SDK_LEVEL >= 0x00060000) */


/**
	Callback for AVAppEnumAnnotHandlers. It is called once for 
	each annotation handler currently registered with the Acrobat 
	viewer (see AVAppRegisterAnnotHandler). 
	@param annotHandler IN/OUT? The annotation handler. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppEnumAnnotHandlers. 
	@return true to continue enumeration, false to halt enumeration. 	
	@see AVAppEnumAnnotHandlers 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerEnumProc)(AVAnnotHandler annotHandler, void *clientData);


/**
	(Optional) Callback for AVAnnotHandler. It is called when 
	an annotation is removed from the selection, and should 
	unhighlight the annotation. Set it to NULL if omitted. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for the annotation that was removed from the selection. 
	
	@param anAnnot IN/OUT? The annotation that was removed from the 
	selection. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	appears. 
	@see AVDocClearSelection 
	@see AVDocSetSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerNotifyAnnotRemovedFromSelectionProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);                                 

/**
	(Optional) Callback for AVAnnotHandler. It is called when 
	an annotation is added to the selection, and should highlight 
	the annotation. Set it to NULL if omitted. 

	To allow only a single annotation to select at a time, keep 
	a global variable containing the selected annotation and 
	on each invocation of NotifyAnnotAddedToSelection first 
	deselect the current selection, if any (that is, if selectedAnnot 
	is non-NULL, call its AVAnnotHandlerNotifyAnnotRemovedFromSelectionProcselect 
	the new annotation, and set selectedAnnot. Of course, RemovedFrom 
	should set selectedAnnot to NULL. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for the annotation that was added to the selection. 
	@param anAnnot IN/OUT? The annotation that was added to the selection. 	
	@param pageView IN/OUT? The AVPageVIew containing the annotation 
	that was added to the selection. 
	@see AVDocSetSelection 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerNotifyAnnotAddedToSelectionProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);                                 

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerDrawExProc. 
	
	(Optional) Callback for AVAnnotHandler that draws the annotation. 
	Set it to NULL if the annotation handler has no Draw method. 
	
	If the annotation has an appearance (AP) entry, use this 
	information to draw the annotation. Read the annotation's 
	appearance state (AS) entry to determine which appearance 
	to use. Then read the Cos stream for the appropriate appearance 
	and display it with AVPageViewDrawCosObj. See the code example 
	below. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation to draw. 
	@param pageView The AVPageView containing the annotation.
	@see AVAnnotHandlerDrawExProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerDrawProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);   

/**
	(Unused) Callback for AVAnnotHandler. Allows the annotation 
	handler to add any attributes to the annotation object to 
	make sure the new annotation is in a valid initial state 
	for its subclass. 
	@param annotHandler The annotation handler. 
	@param anAnnot Annotation to modify. 
	@param pageView The AVPageView in which the annotation 
	is located.
	@return true if the new annotation handler is in a valid initial 
	state for its subclass, false otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerNewProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);       

/**
	(Required) Callback for AVAnnotHandler. It returns an ASAtom 
	indicating the annotation type for which the handler is 
	responsible. This corresponds to the annotation's Subtype 
	key in the PDF file. 

	This is the method that AVAppGetAnnotHandlerByName uses 
	to find the correct handler. 
	@param annotHandler IN/OUT? The annotation handler whose type is 
	returned. 
	@return The annotation type for which this handler is responsible. 
	
	@see AVAppRegisterAnnotHandler 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVAnnotHandlerGetTypeProc)(AVAnnotHandler annotHandler);

/**
	Currently unused. 
	@param annotHandler IN/OUT? The annotation handler. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerNotifyDestroyProc)(AVAnnotHandler annotHandler);

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerDoPropertiesExProc. 
	
	(Optional) Callback for AVAnnotHandler. It displays whatever 
	user interface it wishes to allow a user to change an annotation's 
	properties. Set it to NULL if the annotation type has no 
	user-specified properties. 

	Called when the user selects the 'Properties? item from 
	the 'Edit' menu while an annotation of this type is selected. 
	If NULL, the 'Properties' menu item is dimmed when a corresponding 
	object is selected. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation whose properties are set. 
	@param doc The document containing the annotation.
	@see AVAnnotHandlerDoPropertiesExProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerDoPropertiesProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVDoc doc);

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerDoKeyDownExProc. 
	
	(Optional) Callback for AVAnnotHandler. It is called to 
	handle key presses in the annotation. If NULL, it is as 
	if the callback returned false. 

	Called when there is a key-down event and the annotation 
	is selected and the active tool does not want the event. 
	@param annotHandler The handler responsible for this annotation. 	
	@param anAnnot The annotation in which the key press occurred. 
	@param key The key that was pressed 
	@param flags Indicates which modifier keys are pressed. 
	Must be an OR of the Modifier Keys values.
	@return true if the callback handled the key press, false otherwise. 
	@see AVAnnotHandlerDoKeyDownExProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerDoKeyDownProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVKeyCode key, AVFlagBits16 flags);

/**
	NOTE: Deprecated in Acrobat 6.0. Use AVAnnotHandlerGetLayerExProc. 
	
	Callback for AVAnnotHandler. It returns the annotation's 
	layer. The layer need not be a constant. For example, the 
	Acrobat viewer's built-in text annotations have a different 
	layer depending on whether they are opened or closed. This 
	ensures that a closed text annotation never appears on top 
	of an open text annotation. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation whose layer is returned.
	@return The annotation's layer. 
	@see AVAnnotHandlerGetLayerExProc 
*/
typedef ACCBPROTO1 ASFixed (ACCBPROTO2 *AVAnnotHandlerGetLayerProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot);

/**
	(Optional) Callback for AVAnnotHandler. Called whenever 
	the cursor moves over an annotation handled by this annotation 
	handler. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for this annotation. 
	@param anAnnot IN/OUT? The annotation. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@see AVAnnotHandlerCursorExitProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerCursorEnterProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);

/**
	(Optional) Callback for AVAnnotHandler. Called whenever 
	the cursor moves off an annotation handled by this annotation 
	handler. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for this annotation. 
	@param anAnnot IN/OUT? The annotation the cursor is exiting from. 	
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@see AVAnnotHandlerCursorEnterProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerCursorExitProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);

/**
	(Optional) Callback for AVAnnotHandler. Called upon to copy 
	an annotation, possibly to another document. Annotation 
	handlers should provide this callback to allow for copying 
	their annotations. Called by AVDocCopyAnnot. 
	@param annotHandler The annotation handler responsible 
	for this annotation. 
	@param fromDoc The document whose annotation is copied. 	
	@param anAnnot The annotation to copy. 
	@param toDoc The document to which the annotation is copied.
	@return The newly created PDAnnot copy. 
	@see AVDocCopyAnnot 
*/
typedef ACCBPROTO1 PDAnnot (ACCBPROTO2 *AVAnnotHandlerCopyProc)(AVAnnotHandler annotHandler, AVDoc fromDoc, PDAnnot anAnnot, AVDoc toDoc);

/* Acrobat 4.0 additions. */

/**
	(Optional) Callback for AVAnnotHandler. Gets information 
	associated with an annotation. 

	To effectively use a PDAnnotHandler associated with an annotation, 
	its AVAnnotHandler must have its AVAnnotHandlerGetInfoProc 
	and AVAnnotHandlerDeleteInfoProc callbacks defined. 
	@param avanh IN/OUT? The annotation handler responsible for this 
	annotation. 
	@return Information associated with the annotation. 
	@see AVAnnotHandlerDeleteInfoProc 
	@see AVAppRegisterAnnotHandler 
*/
typedef ACCBPROTO1 AVAnnotHandlerInfo (ACCBPROTO2 *AVAnnotHandlerGetInfoProc)(AVAnnotHandler avanh);

/**
	(Optional) Callback for AVAnnotHandler. Deletes information 
	associated with an annotation. 

	To effectively use a PDAnnotHandler associated with an annotation, 
	its AVAnnotHandler must have its AVAnnotHandlerGetInfoProc 
	and AVAnnotHandlerDeleteInfoProc callbacks defined. 
	@param avanh IN/OUT? The annotation handler responsible for this 
	annotation. 
	@param info IN/OUT? Information associated with the annotation. 
	@see AVAnnotHandlerGetInfoProc 
	@see PDAnnotHandlerDeleteAnnotInfoProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerDeleteInfoProc)(AVAnnotHandler avanh, AVAnnotHandlerInfo info);

/* Acrobat 5.0 additions */

/**
	Called to determine if this annotation can perform the specified 
	operation. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for this annotation. 
	@param annot IN/OUT? The annotation. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@param annotOp IN/OUT? The operation to be performed. 
	@param opData IN/OUT? The data associated with the operation. 
	@return true if the operation can be performed; false otherwise. 
	@see AVAnnotHandlerPerformOpProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerCanPerformOpProc)(
	AVAnnotHandler annotHandler,
	PDAnnot annot,
	AVPageView pageView,
	AVAnnotOp annotOp,
	AVAnnotOpData opData);

/**
	Called to initiate the operation. 
	@param annotHandler IN/OUT? The annotation handler responsible 
	for this annotation. 
	@param annot IN/OUT? The annotation. 
	@param pageView IN/OUT? The AVPageView in which the annotation 
	is located. 
	@param annotOp IN/OUT? The operation to be performed. 
	@param opData IN/OUT? The data associated with the operation. 
	@return true if the operation is performed; false otherwise. 
	@see AVAnnotHandlerCanPerformOpProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerPerformOpProc)(
	AVAnnotHandler annotHandler,
	PDAnnot annot,
	AVPageView pageView,
	AVAnnotOp annotOp,
	AVAnnotOpData opData);

/**
	Called for each keystroke received when an annotation has 
	focus. 

	NOTE: The key and flags numeric types have changed in Acrobat 
	6.0. Supercedes AVAnnotHandlerDoKeyDownProc. 
	@param annotHandler The annotation handler responsible 
	for this annotation. 
	@param annot The annotation. 
	@param pageView The AVPageView in which the annotation 
	is located. 
	@param key The operation to be performed. 
	@param flags Indicates which modifier keys are pressed. 
	See Modifier Keys.
	@return true if the callback handled the keystroke, false otherwise. 
	@see AVAnnotHandlerDoKeyDownProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerDoKeyDownExProc)(
	AVAnnotHandler annotHandler,
	PDAnnot annot,
	AVPageView pageView,
	AVKeyCode key,
	AVFlagBits16 flags);

/**
	Callback for AVAnnotHandler. It returns the flags value 
	for the annotation on the screen. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation whose flags value is returned.
	@return The flags value. 
	@see AVAnnotHandlerGetAppearanceProc 
*/
typedef ACCBPROTO1 AVFlagBits32 (ACCBPROTO2 *AVAnnotHandlerGetFlagsProc)(
	AVAnnotHandler annotHandler, 
	PDAnnot anAnnot);

/* Acrobat 6.0 additions */

/* Fill in the apprObj with a CosObj representing a Form field and
   return 'true' and the viewer will position and draw the appearance
   for you.

   On entry, *flags will be set to reflect the annot's
   NoZoom and NoRotate bits. You can modify those bits and they will take
   effect when the appearance is drawn e.g. if you clear the NoZoom bit
   the appearance will be drawn as if the original annot did not have the
   NoZoom bit set.

   You can also OR in any combination of the following bits;

   DrawNow - indicates that you want you do not want the appearance to
   be drawn at intervals.

   Smooth - indicates that the appearance should be smoothed
   (anti-aliased) when drawn.

   Return 'false' and your Draw or DrawEx callback will be exercised
   to draw the annotation instead.
*/
#define kAVAppearanceDrawNow	0x01
#define kAVAppearanceNoZoom		0x02
#define kAVAppearanceNoRotate	0x04
#define kAVAppearanceSmooth		0x08


/**
	Callback for AVAnnotHandler. Fills in a CosObj representing 
	a Form field. Upon return, the annotation appearance is 
	drawn according to the returned flag values 

	Appearance drawing is new in Acrobat 6.0. If the handler 
	implements this method and it returns true, the new drawing 
	style is used. If this function returns false, the viewer 
	calls the drawing procedure to draw the annotation appearance. 
	See AVAnnotHandler. 

	The handler must not destroy or modify the appearance object 
	while it is being drawn. It can do so only after the EndAppearanceDrawing 
	or CancelAppearanceDrawing callback is exercised. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param annot The annotation whose appearance is drawn. 
	@param flags A pointer to an OR of the AVAnnot Appearance 
	Flags. On entry, Acrobat modifies the flags to reflect the 
	annotation's current NoZoom and NoRotate bits. The method 
	can modify these to change the viewer's drawing behavior 
	for this annotation. The modifications take effect when 
	the appearance is drawn, for example, if the callback clears 
	the NoZoom bit, the appearance is drawn as if the original 
	annotation did not have that bit set. 
	@param pageView The page view in which the annotation 
	is drawn. 
	@param apprObj (Filled by the method) A CosObj representing 
	a Form field. 
	@return true to tell the viewer to position and draw the annotation 
	appearance, false to call the DrawEx or Draw procedure. 
	
	@see AVAnnotHandlerAppearanceDrawingProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerGetAppearanceProc)(
	AVAnnotHandler annotHandler,
	PDAnnot annot, ASUns32 *flags,
	AVPageView pageView,
	CosObj *apprObj);


/**
	Callback for AVAnnotHandler that draws the appearance of 
	the annotation for the given page view. Use this prototype 
	for three callbacks, BeginAppearanceDrawing, FinishAppearanceDrawing, 
	and CancelAppearanceDrawing. 

	If the viewer has to abandon the drawing of the appearance, 
	it calls the CancelAppearanceDrawing callback. If the viewer 
	succeeds in drawing the appearance, it calls the FinishAppearanceDrawing 
	callback. In either case, the handler can destroy the appearance 
	object. See AVAnnotHandler. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param annot The annotation whose appearance is drawn. 
	@param pageView The page view containing the annotation. 
	@param annotRect A pointer to a rectangle in the page 
	view's device space that contains the currently drawn appearance. 
	The procedure must not modify this object. 
	@param updateRect A pointer to a rectangle in the page 
	view's device space that contains the newly drawn appearance. 
	The procedure must not modify this object. As an optimization, 
	the handler can avoid drawing anything outside this rectangle, 
	which the viewer will clip.
	@see AVAnnotHandlerGetAppearanceProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerAppearanceDrawingProc)(
	AVAnnotHandler annotHandler,
	PDAnnot annot,
	AVPageView pageView,
	const AVDevRect* annotRect,
	const AVDevRect* updateRect);

/**
	(Optional) Callback for AVAnnotHandler. It displays whatever 
	user interface it wishes to allow a user to change an annotation's 
	properties. Set it to NULL if the annotation type has no 
	user-specified properties. 

	Called when the user selects the 'Properties' item from 
	the 'Edit' menu while an annotation of this type is selected. 
	If NULL, the 'Properties' menu item is dimmed when a corresponding 
	object is selected. 

	NOTE: Supercedes AVAnnotHandlerDoPropertiesProc in Acrobat 
	6.0. 
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation whose properties are set. 
	@param pageView The page view containing the annotation.
	@see AVAnnotHandlerDoPropertiesProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAnnotHandlerDoPropertiesExProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);

/**
	Callback for AVAnnotHandler. It returns the annotation's 
	layer. The layer need not be a constant. For example, the 
	Acrobat viewer's built-in text annotations have a different 
	layer depending on whether they are opened or closed. This 
	ensures that a closed text annotation never appears on top 
	of an open text annotation. 

	NOTE: Introduced in Acrobat 6.0. Supercedes AVAnnotHandlerGetLayerProc. 
	
	@param annotHandler The annotation handler responsible 
	for the annotation. 
	@param anAnnot The annotation whose layer is returned. 
	@param pageView The page view containing the annotation.
	@return The annotation's layer. 
	@see AVAnnotHandlerGetLayerProc 
*/
typedef ACCBPROTO1 ASFixed (ACCBPROTO2 *AVAnnotHandlerGetLayerExProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView);


/**
	Parameters that describe where and how a mouse click occurred, 
	for the use of click handling callback procedures. 
	@see AVAnnotHandlerDoClickExProc 
*/
typedef struct _t_AVClickParams
{
	/** The size of this structure. */
	ASSize_t size;
	/** The x-coordinate of the mouse click. */
	AVDevCoord xHit;
	/** The y-coordinate of the mouse click. */
	AVDevCoord yHit;
	/** Indicates which modifier keys are pressed. Must be an OR of the Modifier Keys values. */
	AVFlagBits16 flags;
	/** 1 if this is a single click, 2 if a double click, 3 if triple click. */
	AVTCount clickNo;
	/** The tool type that received the click event.*/
	ASAtom toolType;
} AVClickParamsRec, *AVClickParams;


/**
	Parameters that describe where and how a cursor event occurred, 
	for the use of cursor handling callback procedures. 
	@see AVAnnotHandlerAdjustCursorExProc 
*/
typedef struct _t_AVAdjustCursorParams
{
	/** The size of this structure. */
	ASSize_t size;
	/** The x-coordinate of the cursor. */
	AVDevCoord xHit;
	/** The y-coordinate of the cursor. */
	AVDevCoord yHit;
	/** The tool type that received the cursor event */
	ASAtom toolType;
} AVAdjustCursorParamsRec, *AVAdjustCursorParams;

/** (Optional) Callback for AVAnnotHandler. It handles both left and right mouse button
	clicks within the annotation. If NULL, the annotation behaves as if the callback returned
	false.
	NOTE: Introduced in Acrobat 6.0. Supercedes AVAnnotHandlerDoClickProc.
	@param annotHandler IN/OUT? The annotation handler responsible for this annotation.
	@param hitAnnot IN/OUT? The annotation in which the mouse was clicked.
	@param pageView IN/OUT? The AVPageView in which the annotation is located.
	@param clickParams IN/OUT? The parameters for the click position and properties.
	@return true if the callback handled the mouse click, false if it did not. If the callback does not
	handle the click, it is passed to any annotation at the same location in lower layers.
	@see AVAnnotHandlerDoClickProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerDoClickExProc)(AVAnnotHandler annotHandler, PDAnnot hitAnnot,
                    AVPageView pageView,
					AVClickParams clickParams);
/** (Optional) Callback for AVAnnotHandler. It controls the cursor shape when the cursor is
	within the annotation. If NULL, the annotation behaves as if the AdjustCursor callback
	returned false.
	NOTE: Introduced in Acrobat 6.0. Supercedes AVAnnotHandlerAdjustCursorProc.
	@param annotHandler The annotation handler responsible for this annotation.
	@param anAnnot The annotation containing the cursor.
	@param pageView The AVPageView in which the annotation is located.
	@param params The parameters structure containing the cursor’s current x- and ycoordinates.
	@return true if the callback handled the adjust cursor event, false otherwise. The callback would
		return false, for example, if the annotation is irregularly shaped and the cursor is not
		currently over the real annotation even though it is within the rectangular bounding box
		that the Acrobat viewer uses to specify annotations.
	@see AVAnnotHandlerAdjustCursorProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerAdjustCursorExProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot,
  					AVPageView pageView, AVAdjustCursorParams params);

/* Acrobat 7.0 additions */

/** The type of accessibility string to be obtained */
enum {
	/** Name and brief description */
	kAVAnnotAccName,

	/** Content (or content description) */
	kAVAnnotAccValue,

	/** Description of default action */
	kAVAnnotAccDefaultAction
};
typedef ASEnum8 AVAnnotAccStrType;

/**
	Callback for AVAnnotHandler.  Gets the accessibility string 
	associated with an annotation. 
	@param annotHandler The annotation handler responsible for the annotation.
	@param anAnnot The annotation whose accessibility string is returned.
	@param pageNum The page number on which the annotation is drawn.
	@param strType The type of accessibility string.
	@param accString The accessibility string.
	@return true if the new annotation handler has set 
	   the accString successuflly, false otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerGetAccessibilityStringProc)(AVAnnotHandler annotHandler, 
					PDAnnot anAnnot, PDPageNumber pageNum, AVAnnotAccStrType strType, ASText accString);


typedef enum {
	kAVTabOrderFirst,
	kAVTabOrderLast,
	kAVTabOrderNext,
	kAVTabOrderPrev
} AVPageViewTabOrder;

/**
	Callback for AVAnnotHandler. Allows the annotation 
	handler to determine which annot is next in the tab order.
	@param annotHandler The annotation handler. 
	@param anAnnot Annotation to direct tab order. 
	@param pageView The AVPageView in which the annotation 
	is located.
	@param tabOrder The type of tab order to follow
	@param tabToAnnot annot to tab to
	@param tabToPageNo Indicates the page number to tab to.
	@return true if the new annotation handler has set
		the annot to tab to, false otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAnnotHandlerGetAnnotInTabOrderProc)(AVAnnotHandler annotHandler, PDAnnot anAnnot, AVPageView pageView, 
						AVPageViewTabOrder tabOrder, PDAnnot * tabToAnnot, ASInt32 * tabToPageNo );       

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure containing callbacks that implement an annotation 
	handler. The callbacks implement the annotation handler 
	functions. For example, draw the annotation, highlight the 
	annotation when it is selected, and the data specifies properties 
	of the annotation (for example, text selection behavior). 
	

	Appearance drawing is new in Acrobat 6.0. If the handler 
	implements the GetAppearance callback, the new drawing style 
	is used. If the GetAppearance callback returns false or 
	is not implemented, the viewer calls the drawing procedure 
	(DrawEx if present, otherwise Draw) to draw the annotation 
	using Acrobat 5.0-style drawing. 

	For the new style of appearance drawing, the viewer calls 
	the handler's BeginAppearanceDrawing callback, then draws 
	the appearance object. If the drawing operation completes 
	without errors, it call the the handler's BeginAppearanceDrawing 
	callback; if an error occurs, or the operation is canceled, 
	it calls the CancelAppearanceDrawing callback. This can 
	happen if, for example, the user switches to another page 
	before the drawing is completed. 

	The handler must not destroy or modify the appearance object 
	while it is being drawn. It can do so only after the EndAppearanceDrawing 
	or CancelAppearanceDrawing callback is exercised. 

	NOTE: Types of numeric values in callbacks have changed 
	in Acrobat 6.0. 
	@see AVAnnotHandlerEnumProc 
	@see PDAnnotHandlerDeleteAnnotInfoProc 
	@see PDAnnotHandlerGetAnnotInfoFlagsProc 
	@see PDAnnotHandlerGetAnnotInfoProc 
	@see PDAnnotHandlerGetTypeProc 
	@see AVAppRegisterAnnotHandler 
	@see AVAppGetAnnotHandlerByName 
	@see AVDocCopyAnnot 
*/
typedef struct _t_AVAnnotHandler 
{

	/** Size of the data structure. Must be set to sizeof( AVAnnotHandlerRec). */
	ASSize_t 	size;

	/** A collection of flags that affect the annotation's behavior. The flags may be OR-ed together. NOTE: These flags 
		are not the ones used in PDAnnotArray. Permissible flags include: ANNOT_CLIP_TEXT_SELECTION If this flag is set, 
		text selection in the main document never selects text within the annotation (that is, the annotation behaves like the 
		Acrobat viewer's text annotation). If this flag is not set, text selection in the main document can select text within 
		the annotation (that is, the annotation behaves like the Acrobat viewer's link annotation). ANNOT_WANTS_SHIFT_KEY 
		This flag is set to prevent the standard shift-key ignores annotation's behavior. 
	*/
	AVFlagBits32 flags;


	/** @deprecated */
	oldAVAnnotHandlerDoClickProc oldDoClick; 

	/** @deprecated */
	oldAVAnnotHandlerAdjustCursorProc oldAdjustCursor; 

	/** @deprecated */
	oldAVAnnotHandlerPtInAnnotViewBBoxProc oldPtInAnnotViewBBox; 

	/** @deprecated */
	oldAVAnnotHandlerGetAnnotViewBBoxProc oldGetAnnotViewBBox;  

	/**  */
	AVAnnotHandlerNotifyAnnotRemovedFromSelectionProc NotifyAnnotRemovedFromSelection;

	/** */
	AVAnnotHandlerNotifyAnnotAddedToSelectionProc NotifyAnnotAddedToSelection;

	/** @deprecated */
	AVAnnotHandlerDrawProc Draw; 

	/** */
	AVAnnotHandlerNewProc New;

	/** */
	AVAnnotHandlerGetTypeProc GetType;

	/** */
	AVAnnotHandlerNotifyDestroyProc NotifyDestroy;

	/** */
	AVAnnotHandlerDoPropertiesProc DoProperties;

	/** */
	AVAnnotHandlerDoKeyDownProc DoKeyDown;

	/** */
	AVAnnotHandlerGetLayerProc GetLayer;

	/** */
	AVAnnotHandlerCursorEnterProc CursorEnter;

	/** */
	AVAnnotHandlerCursorExitProc CursorExit;

	/** */
	AVAnnotHandlerCopyProc Copy;

	/** @deprecated */
	oldAVAnnotHandlerDoClickProc oldDoRightClick; 

	/** */
	AVAnnotHandlerGetInfoProc GetInfo;

	/** */
	AVAnnotHandlerDeleteInfoProc DeleteInfo;

	/** */
	AVAnnotHandlerCanPerformOpProc CanPerformOp;

	/** */
	AVAnnotHandlerPerformOpProc PerformOp;

	/** */
	AVAnnotHandlerDoKeyDownExProc DoKeyDownEx;

	/** @deprecated */
	oldAVAnnotHandlerDrawExProc oldDrawEx; 

	/** */
	AVAnnotHandlerGetFlagsProc GetFlags;

	/** */
	AVAnnotHandlerDoClickProc DoClick;

	/** */
	AVAnnotHandlerAdjustCursorProc AdjustCursor;

	/** */
	AVAnnotHandlerPtInAnnotViewBBoxProc PtInAnnotViewBBox;

	/** */
	AVAnnotHandlerGetAnnotViewBBoxProc GetAnnotViewBBox; 

	/** */
	AVAnnotHandlerDoClickProc DoRightClick;

	/** */
	AVAnnotHandlerDrawExProc DrawEx; 

	/** */
	AVAnnotHandlerGetAppearanceProc GetAppearance;

	/** */
	AVAnnotHandlerAppearanceDrawingProc BeginAppearanceDrawing;

	/** */
	AVAnnotHandlerAppearanceDrawingProc FinishAppearanceDrawing;

	/** */
	AVAnnotHandlerAppearanceDrawingProc CancelAppearanceDrawing;

	/** */
	AVAnnotHandlerDoPropertiesExProc DoPropertiesEx;

	/** */
	AVAnnotHandlerGetLayerExProc GetLayerEx;

	/** */
	AVAnnotHandlerDoClickExProc DoClickEx;

	/** */
	AVAnnotHandlerDoClickExProc DoRightClickEx;

	/** */
	AVAnnotHandlerAdjustCursorExProc AdjustCursorEx;
	
	/** */
	AVAnnotHandlerGetAccessibilityStringProc GetAccessibilityString;

	/** */
	AVAnnotHandlerGetAnnotInTabOrderProc GetAnnotInTabOrder;
} AVAnnotHandlerRec;
#endif /* (ACRO_SDK_LEVEL >= 0x00060000) */


/**
	Callback that is called periodically when the Acrobat viewer 
	is otherwise idle. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppRegisterIdleProc. 
	@see AVAppRegisterIdleProc 
	@see AVAppUnregisterIdleProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVIdleProc)(void *clientData);


/************************************************************************************\
|*																					*|
|*									AVSys											*|
|*																					*|
\************************************************************************************/

/**
	Data structure representing the cursor.  
	@see AVSysGetCursor 
	@see AVSysGetStandardCursor 
	@see AVSysSetCursor 
	@see cursorID for a list of already defined cursor shapes.
*/
typedef struct _t_AVCursor *AVCursor;


/**@name cursorID
	Possible values for the cursorID argument to AVSysGetStandardCursor()
	@see AVSysGetStandardCursor
*/
/*@{*/

/** */
#define ARROW_CURSOR			   	0

/** */
#define IBEAM_CURSOR 			   	1

/** */
#define CROSSHAIR_CURSOR 			1000

/** */
#define BOX_IBEAM_CURSOR 			1001

/** */
#define HAND_CURSOR 	 			1002

/** */
#define ZOOM_IN_CURSOR	 			1003

/** */
#define ZOOM_OUT_CURSOR	 			1004

/** */
#define ZOOM_MAX_CURSOR	 			1005

/** */
#define ZOOM_DRAG_CURSOR	 		1006

/** */
#define GROW_CURSOR		 			1008

/** */
#define BAR_IBEAM_CURSOR 			1011

/** */
#define FIST_CURSOR		 			1012

/** */
#define LINK_CURSOR		 			1100

/** */
#define MOVEPAGE_CURSOR	 			1111

/** */
#define WAIT_CURSOR		 			1112

/** */
#define COPYPAGE_CURSOR	 			1113

/** */
#define MOVEPAGES_CURSOR 			1114

/** */
#define COPYPAGES_CURSOR 			1115

/** */
#define REPLACEPAGE_CURSOR			1116

/** */
#define REPLACEPAGES_CURSOR			1117

/** */
#define NOP_CURSOR 					1118

/** */
#define THREAD_CURSOR 				1119

/** */
#define WORDFINDER_CURSOR 			1201

/** */
#define HAND_THREAD_CURSOR			1202

/** */
#define HIDDEN_CURSOR 				1203

/** */
#define GROWTOPLEFT_CURSOR 			1204

/** */
#define GROWBOTTOMLEFT_CURSOR 		1205

/** */
#define MOVE_CURSOR 				1206

/** */
#define HAND_THREAD_UP_CURSOR 		1207

/** */
#define HAND_THREAD_END_CURSOR 		1208

/** */
#define HAND_THREAD_UP_END_CURSOR 	1209

/** */
#define HAND_THREAD_BEGIN_CURSOR 	1210

/** */
#define THREAD_CONNECT_CURSOR		1211

/** */
#define THREAD_END_CURSOR 			1212

/** */
#define VERT_IBEAM_CURSOR 			1213

/** */
#define GROWLEFTRIGHT_CURSOR		33

/** */
#define HIGHLIGHT_CURSOR			34

/** */
#define GROWTOPBOTTOM_CURSOR		35

/** */
#define CROPTOOL_CURSOR				36

/** */
#define CROPTOOL_SCISSORS_CURSOR	37

/** */
#define DRAGLEFTRIGHT_CURSOR		1214

/** */
#define DRAGUPDOWN_CURSOR			1215

/** */
#define VERTBEAMNOBAR_CURSOR		1216

/** */
#define GROW_4WAY_CURSOR			1217

/** */
#define MEASURE_CURSOR				1300

/** */
#define MEASURECLOSE_CURSOR			1301

/** */
#define LOUPE_CURSOR				1302

/** */
#define SELECTOBJECT_CURSOR			1303

/** */
#define TABLE_CURSOR				1304

/** */
#define TOUCHUPOBJ_CURSOR			1305 

/** */
#define CROSSHAIR_ADD_CURSOR 		1306

/** */
#define CROSSHAIR_SUBTRACT_CURSOR 	1307
/*@}*/


/**@name KeyStates
flags used to indicate key states
*/
/*@{*/

/** */
#define AV_SHIFT		1

/** */
#define AV_CONTROL		2

/** */
#define AV_COMMAND		4

/** */
#define AV_OPTION		8

/** eg. right-alt,right-ctrl or Enter on enhanced keyboards */
#define AV_EXTENDED 	16 

/** cursor is inverted; using the "eraser" side of a pen input device */
#define AV_INVERTED 	32 

/** */
#define AV_SPACE		64

/** */
#define AV_CAPS			128

/** pen is used as input device */
#define AV_PENINUSE		256 

/*@}*/

/************************************************************************************\
|*																					*|
|*									AVTool											*|
|*																					*|
\************************************************************************************/

#if (ACRO_SDK_LEVEL >= 0x00060000)


/**
	Data structure for a tool. It contains callbacks that implement 
	the tool's functions (for example, handling mouse clicks 
	and controlling the cursor shape). 
	Handles key presses and mouse clicks in the content region of an AVPageView.
	AVTools do not handle mouse clicks in other parts of the viewer’s window, such as in
	the bookmark pane. At any time, there is one active tool.
	@see AVAppGetActiveTool 
	@see AVAppSetActiveTool 
	@see AVAppGetDefaultTool 
	@see AVAppGetLastActiveTool 
	@see AVAppGetToolByName 
	@see AVAppRegisterTool 
	@see AVToolGetType 
	@see AVToolIsPersistent
*/
typedef struct _t_AVTool *AVTool;

/**
	(Optional) Callback for AVTool. This callback controls the 
	cursor shape when the tool is the active tool. 

	If omitted, the cursor specified in the tool's cursorID 
	field is used. 
	@param tool IN/OUT? The currently active tool. 
	@param pageView IN/OUT? The page view in which the cursor is currently 
	located. 
	@param x IN/OUT? The x-coordinate of the current cursor location. 
	@param y IN/OUT? The y-coordinate of the current cursor location. 
	@return true if you handled the cursor shape (that is, do not allow 
	underlying layers to handle it), false if you did (that 
	is, allow underlying layers to handle it). 
	@see AVAppRegisterTool 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AdjustCursorProcType)(AVTool tool, AVPageView pageView, AVDevCoord x, AVDevCoord y);

/**
	Callback for AVTool. Handles mouse clicks when the tool 
	is active. For Mac OS, this handles button or option-button 
	mouse clicks. For Windows, this handles right or left button 
	mouse clicks. 

	NOTE: The numeric argument types have changed in Acrobat 
	6.0. 
	@param tool The tool. 
	@param pageView The AVPageView in which the click occurred. 
	@param xHit The click's x-coordinate. 
	@param yHit The click's y-coordinate. 
	@param flags Modifier keys that were held down while clicking. 
	Must be an OR of the Modifier Keys values. 
	@param clickNo 1 if single click, 2 if double click, 3 
	if triple click.
	@return true if the callback handled the click, false if it did 
	not and the click should be passed to the next click handling 
	procedure. 
	@see DoKeyDownProcType 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DoClickProcType)(AVTool tool, AVPageView pageView,
					AVDevCoord xHit, AVDevCoord yHit,
					AVFlagBits16 flags,
					AVTCount clickNo);


#if (ACRO_SDK_LEVEL >= 0x00070000)

/**
	Describes the direction of mouse wheel movement.
	New for Acrobat 7.0.
	@see HandleMouseWheelScroll 
*/
enum {

	/** */
	AVToolMouseWheelUp,

	/** */
	AVToolMouseWheelDown
};

typedef ASEnum16 AVToolMouseWheelDirection;

/**
	Callback for AVTool. Handles mouse wheel scroll events when 
	the tool is active. 

	@param tool			The tool. 
	@param pageView		The AVPageView in which the click occurred. 
	@param direction	The direction of the mouse wheel movement. 
	@param numLines		The number of lines moved. 
	@param flags		Modifier keys that were held down while wheeling. 
						Must be an OR of the Modifier Keys values. 

	@return true if the callback handled the click, false if it did 
	not and the wheel should be passed to the next wheel handling 
	procedure. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *HandleMouseWheelScrollProc)(AVTool tool, AVPageView pageView, 
																   AVToolMouseWheelDirection direction, 
																   AVTCount numLines, AVFlagBits16 flags);

#endif /* (ACRO_SDK_LEVEL >= 0x00070000) */


#endif /* (ACRO_SDK_LEVEL >= 0x00060000) */

/** Callback for AVTool. Called when this tool has become the active tool. It is legal to call
	AVAppGetActiveTool from within this method or from DeactivateProcType; it
	will return this tool object. Call AVAppGetLastActiveTool to get the formerly active
	tool object (its DeactivateProcType method will already have been called).
	@param tool IN/OUT? The tool to activate. 
	@param persistent IN/OUT? true if it should remain active through 
	arbitrarily many 'operations' (whatever that means for a 
	particular tool), rather than performing a single action 
	('one shot') and then restoring the previously active tool, 
	false otherwise. 
	@see DeactivateProcType
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ActivateProcType)(AVTool tool, ASBool persistent);

/**
	Callback for AVTool. Called when the tool will no longer 
	be the active tool. 
	@param tool IN/OUT? The tool to deactivate. 
	@see ActivateProcType 
	@see AVAppSetActiveTool 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DeactivateProcType)(AVTool tool);

/**
	Callback for AVTool. Handles key presses when the tool is 
	active. 
	@param tool IN/OUT? The tool. 
	@param key IN/OUT? The key that was pressed. 
	@param flags IN/OUT? Modifier keys that were also pressed. Must 
	be an OR of the Modifier Keys values. 
	@return true if the callback handled the key press, false if it 
	did not and the key press should be passed to the key press 
	handling procedure. 
	@see DoClickProcType 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DoKeyDownProcType)(AVTool tool, AVKeyCode key, AVFlagBits16 flags);

/**
	Callback for AVTool. Returns the tool's name. 
	@param tool IN/OUT? The tool. 
	@return The ASAtom corresponding to the tool's name. 
	@see AVAppGetToolByName 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *GetTypeProcType)(AVTool tool);

/**
	Callback for AVTool. Indicates whether the tool would like 
	to stay active after it has been used, or is a one-shot 
	tool. 

	The Acrobat viewer does not contain any code to enforce 
	a tool's request to be persistent; it is up to each tool 
	to be a good citizen. For example, if a tool is not persistent, 
	after that tool is used once it should get the previously 
	active tool (using AVAppGetLastActiveTool) and check whether 
	that tool wishes to be persistent (using AVToolIsPersistent). 
	If so, set the active tool to that tool. If not, set the 
	active tool to the default tool (obtained using AVAppGetDefaultTool). 
	@param tool The tool.
	@return true if the tool wishes to be persistent, false otherwise. 
	@see AVToolIsPersistent 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *IsPersistentProcType)(AVTool tool);

/**
	Callback for AVTool. Called when the tool leaves the page 
	view, that is, when the cursor is moved out of the page 
	view. 
	@param tool IN/OUT? The tool. 
	@param pageView IN/OUT? The AVPageView that the tool left. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DoLeaveProcType)(AVTool tool, AVPageView pageView);

/**
	Callback for AVTool. Gets the selection server associated 
	with the tool, if any. 
	@param tool IN/OUT? The tool. 
	@param doc IN/OUT? The active AVDoc. 
	@return The selection server associated with this tool. Returns 
	NULL if the tool has no selection server. 
*/
typedef ACCBPROTO1 AVDocSelectionServer (ACCBPROTO2 *GetSelectionServerProcType)(AVTool tool, AVDoc doc);

/**
	An AVToolRec callback. If provided, called at shutdown time 
	to allow the tool to free dynamic memory. 
	@param tool The tool to be destroyed.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVToolDestroyProc)(AVTool tool);

/**
	Callback for AVToolRec. If provided, called when text describing 
	the tool needs to be displayed. 
	@param tool The tool.
	@return The text object. 
*/
typedef ACCBPROTO1 ASConstText (ACCBPROTO2 *AVToolGetLabelProc)(AVTool tool);

/**
	Callback for AVToolRec. If provided, called when a static 
	tool icon is needed. This icon should be similar to, but 
	not the same as the tool button icon for this tool. 
	@param tool The tool.
	@return The icon object. 
*/
typedef ACCBPROTO1 AVIcon (ACCBPROTO2 *AVToolGetLabelIconProc)(AVTool tool);
/* Concrete instantiation of a tool object. Tool implementations will
   usually "subclass" this by pass a ptr to struct with an AVToolRec as
   its first member to AVAppRegisterTool, cast as an AVTool.
*/
#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure for a tool. It contains callbacks that implement 
	the tool's functions (for example, handling mouse clicks 
	and controlling the cursor shape). 
	@see AVAppGetActiveTool 
	@see AVAppSetActiveTool 
	@see AVAppGetDefaultTool 
	@see AVAppGetLastActiveTool 
	@see AVAppGetToolByName 
	@see AVAppRegisterTool 
	@see AVToolGetType 
	@see AVToolIsPersistent
*/
typedef struct _t_AVTool 
{

	/** Size of the data structure. Must be set to 	sizeof( AVToolRec). */
  ASSize_t size;
  

	/** called when this tool has become the active tool. It
        is legal to call AVAppGetActiveTool from within this
        method or from Deactivate; it will return this tool object. Call
        AVAppGetLastActiveTool to get the formerly active tool object 
        (its Deactivate method will already have been called).  
        persistent = true,
        is a hint to the tool that (if meaningful to this tool) it
        should remain active through arbitrarily many "operations"
        (whatever that means for a particula4 tool), rather than
        performing a single action (as a "one shot") and then restoring
        the previously active tool. persistent = false implies
        the opposite.
	*/
  ActivateProcType Activate;

 	/** */
  DeactivateProcType Deactivate;

	/** obsolete, use DoClick */
  oldDoClickProcType oldDoClick; 

	/** obsolete, use AdjustCursor */
  oldAdjustCursorProcType oldAdjustCursor; 

	/** */
  DoKeyDownProcType DoKeyDown;

	/** */
  GetTypeProcType GetType;

	/** return true if the tool is persistent at this time (this may be false
       even though Active was called with persistent = true, or visa-versa).
       If NULL, AVToolIsPersistent will return true.
	*/
  IsPersistentProcType IsPersistent;
 

  /** A default cursor, used if the tool does not have an AdjustCursorProcType callback. 
	Ignored if AdjustCursor is non-NULL; if AdjustCursor *is* NULL, the
    default AdjustCursor implementation will set the cursor to the
    result of calling AVSysGetStandardCursor(cursorId) unless a
    hit annotation wants to set the cursor. The default AdjustCursor
    is appropriate for most tool implementations. See ZoomTool impl for
    an example of a private AdjustCursor implementation.
  */
  AVCursorID cursorID;

 	/** computeEnabledProc should return true if tool is enabled (legal).
		Legal if front-most doc permits tool's operation.
		Will only be called if there is a front-most doc.
		If NULL, tool is always enabled.
	*/
	AVComputeEnabledProc ComputeEnabled;

	/** User-supplied data to pass to the tool's AVComputeEnabledProc callback each time it is called. */
	void *computeEnabledData;


	/** obsolete */
  oldDoClickProcType oldDoRightClick; 

	/** */
  DoLeaveProcType DoLeave;


	/** */
  GetSelectionServerProcType GetSelectionServer;

	/** */
  DoClickProcType DoClick;

	/** */
  AdjustCursorProcType AdjustCursor;

	/** */
  DoClickProcType DoRightClick;


	/** */
  void *clientData;

	/** */
  AVToolDestroyProc Destroy;

	/** */
  AVToolGetLabelProc GetLabel;

	/** */
  AVToolGetLabelIconProc GetLabelIcon;

#if (ACRO_SDK_LEVEL >= 0x00070000)

	/** */
	HandleMouseWheelScrollProc HandleMouseWheel;

#endif /* (ACRO_SDK_LEVEL >= 0x00070000) */

} AVToolRec;

#endif /* (ACRO_SDK_LEVEL >= 0x00060000) */

/**
	Callback for AVAppEnumTools. It is called once for each 
	tool. 
	@param tool IN/OUT? The tool currently being enumerated. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppEnumTools. 
	@return true to continue enumeration, false to halt enumeration. 
	@see AVAppEnumTools 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVToolEnumProc)(AVTool tool, void *clientData);


/************************************************************************************\
|*																					*|
|*									AVToolBar										*|
|*																					*|
\************************************************************************************/
#ifdef __cplusplus
class AVToolBarObj;
typedef AVToolBarObj *AVToolBar;
#else
typedef struct _t_AVToolBar *AVToolBar;
#endif

/************************************************************************************\
|*																					*|
|*								AVToolButton										*|
|*																					*|
\************************************************************************************/

/* bit-field values for toolbuttons */

#define TOOLBUTTON_INTERNAL 1
#define TOOLBUTTON_EXTERNAL 2

/**
	A button in the Acrobat viewer’s toolbar. Like menu items, the procedure that executes
	when the button is clicked can be set by a plug-in. Although not required, there is
	generally a menu item corresponding to each button, allowing users to select a
	function using either the button or the menu item. Buttons are added to the toolbar by
	specifying which existing button they appear before or after.
	@see AVToolBarGetButtonByName
	@see AVToolButtonNew
	@see AVToolBarEnumButtons
	@see AVToolButtonDestroy
	@see AVToolButtonRemove
	@see AVToolBarEnumButtons
	@see AVToolButtonGetFlyout 
	@see AVToolButtonSetFlyout
	@see AVToolButtonGetIcon 
	@see AVToolButtonSetIcon
	@see AVToolButtonGetMenu 
	@see AVToolButtonSetMenu
	@see AVToolButtonIsEnabled 
	@see AVToolButtonIsMarked 
	@see AVToolButtonIsSeparator 
	@see AVToolButtonSetComputeEnabledProc
	@see AVToolButtonSetComputeMarkedProc
	@see AVToolButtonSetExecuteProc
	@see AVToolButtonSetExternal
	@see AVToolButtonSetHelpText
*/
#ifdef __cplusplus
class AVToolButtonObj;
typedef AVToolButtonObj *AVToolButton;
#else
typedef struct _t_AVToolButton *AVToolButton;
#endif

/**
	Callback for AVToolBarEnumButtons. It is called once for 
	each toolbar button. 
	@param button IN/OUT? The toolbar button currently being enumerated. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVToolBarEnumButtons. 
	@return true to continue enumeration, false to halt enumeration. 
	@see AVToolBarEnumButtons 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVToolButtonEnumProc)(AVToolButton button, void *clientData);


/************************************************************************************\
|*																					*|
|*									AVWindow										*|
|*																					*|
\************************************************************************************/
#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Creates and manages windows. Plug-ins should use AVWindows for their own
	dialogs, floating palettes, and so forth, to ensure that those windows work well with
	the Acrobat viewer. For example, under Windows they are hidden when the Acrobat
	viewer is turned into an icon. Once the plug-in creates an AVWindow, it is free to use
	platform-dependent code to put whatever it wants in the window.
	@see AVWindowNew
	@see AVWindowNewFromPlatformThing
	@see AVDocGetAVWindow
	@see AVWindowDestroy
	@see AVWindowUserClose
	@see AVWindowDrawNow
	@see AVWindowGetFrame 
	@see AVWindowSetFrame
	@see AVWindowGetInterior 
	@see AVWindowGetOwnerData 
	@see AVWindowSetOwnerData
	@see AVWindowGetPlatformThing 
	@see AVWindowGetTitle 
	@see AVWindowSetTitle
	@see AVWindowIsKey 
	@see AVWindowBecomeKey
	@see AVWindowSetWantsKey
	@see AVWindowResignKey
	@see AVWindowBringToFront
	@see AVWindowIsVisible 
	@see AVWindowShow
	@see AVWindowHide
	@see AVWindowInvalidateRect
	@see AVWindowMaximize
*/
#ifndef _T_AVWINDOW
#define _T_AVWINDOW
typedef struct _t_AVWindow *AVWindow;
#endif // _T_AVWINDOW

#endif // ACRO_SDK_LEVEL


/**
	Constants that specify the layer in which a newly-created 
	AVWindow is to appear. 
	@see AVWindowNew 
	@see AVWindowNewFromPlatformThing 
*/
/*@{*/
typedef ASInt32 AVWindowLayer;

/** */
#define AVWLunknown						0

/** Regular window, such as a document window. */
#define AVWLnonfloating					1	

/** Floating window, such as a palette. */
#define AVWLfloating					2		

/** Modal dialog. */
#define AVWLmodal						3		

/** (New in Acrobat 5.0) Tool-tip window with a one-pixel border. */
#define AVWLpopup						4		

/** toolbar window - internal use only */
#define AVWLtoolbar						5		

/** help window - internal use only */
#define AVWLhelp						6	

/** Window that occupies the same layer as popup menus and has no title bar decorations. */
#define AVWLmenu						7
/*@}*/


/**@name AVWindowFlags
	These flags are or'd together to describe the properties of an AVWindow at creation time.
*/
/*@{*/

/** is the window resizeable? */
#define AVWIN_RESIZEABLE		0x80000000	

/** does the window have a close box? */
#define AVWIN_HASCLOSEBOX		0x40000000	

/** does it have a zoom box? */
#define AVWIN_HASZOOMBOX		0x20000000		

/** does it have a minimze box? */
#define AVWIN_HASMINIMIZEBOX	0x04000000		

/** does it have a drag bar? */
#define AVWIN_HASDRAGBAR		0x02000000	

/** does it have a small title bar? */
#define AVWIN_SMALLTITLEBAR		0x01000000	

/** Auxiliary - "auxiliary" windows pass unhandled menu commands to the frontmost document
	window.  For example, the Find dialog is "auxiliary," in that menu items like save,
	close, etc. should still be enabled.  A window that does not purport to operate on
	the topmost document window (say, an "Establish Connection to Library of Congress"
	dialog) would probably not be auxiliary.
*/
#define AVWIN_AUXILIARY			0x10000000

/** */
#define AVWIN_WANTSKEY			0x08000000

/** does the window hide itself when focus is lost? */
#define AVWIN_TRANSIENT			0x00800000

/** does this window size itself to fit its child views? */
#define AVWIN_FORMFIT			0x00400000
/*@}*/


/************************************************************************************\
|*																					*|
|*								 AVWindowHandler									*|
|*																					*|
\************************************************************************************/


#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Callback for AVWindowHandler. Mouse clicks in the AVWindow 
	are dispatched through this callback. 

	NOTE: The coordinate numeric types have changed in Acrobat 
	6.0. 
	@param win The window in which the click occurred. 
	@param x The click's x-coordinate. 
	@param y The click's y-coordinate. 
	@param platformEvent Platform-specific event record: Macintosh 
	- Pointer to an EventRecord. UNIX - eventPtr.
	@see AVWindowKeyDownProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowMouseDownProc)(AVWindow win, AVWindowCoord x, AVWindowCoord y, void *platformEvent);

/**
	Callback for AVWindowHandler. Called whenever the window 
	needs to refresh some part of its interior. It should redraw 
	the contents of the specified rectangle. 
	@param win IN/OUT? The window whose content is redrawn. 
	@param rect IN/OUT? The region of win to redraw. 
	@see AVWindowDrawNow 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDrawProc)(AVWindow win, const AVWindowRect *rect);

/**
	Callback for AVWindowHandler. Called when the window is 
	about to resize. 
	@param win IN/OUT? The window to resize. 
	@param newFrame IN/OUT? Rectangle specifying the size to which 
	the window will be resized. This callback may change the 
	new frame size. 
	@return true to permit the resizing, false to abort it. 
	@see AVWindowSetFrame 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVWindowWillBeResizedProc)(AVWindow win, AVScreenRect *newFrame);

/**
	Callback for AVWindowHandler. Called periodically while 
	the cursor is over the AVWindow (if the window is active). 
	Use this to adjust the cursor's appearance. 
	@param win IN/OUT? The window containing the cursor. 
	@param x IN/OUT? The cursor's x-coordinate. 
	@param y IN/OUT? The cursor's y-coordinate. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowAdjustCursorProc)(AVWindow win, AVWindowCoord x, AVWindowCoord y);

/**
	Callback for AVWindowHandler. Called after the window has 
	been resized. 
	@param win IN/OUT? The window that was resized. 
	@param newFrame IN/OUT? Rectangle specifying the window's new size 
	and location. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidResizeProc)(AVWindow win, const AVScreenRect *newFrame);

/**
	Callback for AVWindowHandler. Called after the window has 
	been maximized. 
	@param win IN/OUT? The window that was maximized. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidMaximizeProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called after the window has 
	been collapsed. 
	@param win IN/OUT? The window that was collapsed. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidCollapseProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called after the window has 
	been resized. 
	@param win IN/OUT? The window that was resized. 
	@param newFrame IN/OUT? Rectangle specifying the window's new size 
	and location. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidMoveProc)(AVWindow win, const AVScreenRect *newFrame);
#endif /* (ACRO_SDK_LEVEL >= 0x00060000) */

/**
	Callback for AVWindowHandler. The window is about to close. 
	@param win IN/OUT? The window to close. 
	@param quitting IN/OUT? If true, the application is trying to quit. 
	@return true if the window should close, false to abort the operation. 	
	@see AVWindowUserClose 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVWindowWillCloseProc)(AVWindow win, ASBool quitting);

/**
	Callback for AVWindowHandler. Called immediately after the 
	window has been closed, but before it has been freed. You 
	may want to explicitly destroy the window in this routine. 
	See also AVWindowWillCloseProc. 
	@param win IN/OUT? The window that was closed. 
	@see AVWindowWillCloseProc 
	@see AVWindowUserClose 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidCloseProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called after the window has 
	been activated. The window being activated will not always 
	become the front-most window. 
	@param win IN/OUT? The window being activated. 
	@see AVWindowWillDeactivateProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidActivateProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called after the window becomes 
	the key window. win IN/OUT? The window becoming the key 
	window. 
	@see AVWindowWillResignKeyProc 
	@see AVWindowSetWantsKey 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDidBecomeKeyProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called to handle keystrokes 
	when this is the key window. 
	@param win IN/OUT? The window. 
	@param key IN/OUT? The key that was pressed. 
	@param platformEvent IN/OUT? Platform-specific event record: Macintosh 
	- Pointer to an EventRecord. UNIX - eventPtr. 
	@see AVWindowMouseDownProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowKeyDownProc)(AVWindow win, char key, void *platformEvent);

/**
	Callback for AVWindowHandler. Called before the window ceases 
	to be the key window. 
	@param win IN/OUT? The window to resign key status. 
	@see AVWindowDidBecomeKeyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowWillResignKeyProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called before the window becomes 
	deactivated or hidden. 
	@param win IN/OUT? The window that will be deactivated. 
	@see AVWindowDidActivateProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowWillDeactivateProc)(AVWindow win);

/**
	Callback for AVWindowHandler. Called when the user has chosen 
	an Edit menu item, if the corresponding AVWindowCanPerformEditOpProc 
	returned true. 
	@param win IN/OUT? The window that the edit menu item is to act 
	on. 
	@param editOp IN/OUT? An ASAtom specifying the edit operation. 
	Must be an ASAtom corresponding to one the strings: Cut, 
	Copy, Paste, Clear, SelectAll, and Undo. 
	@see AVWindowCanPerformEditOpProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowPerformEditOpProc)(AVWindow win, ASAtom editOp);

/**
	Callback for AVWindowHandler. Called before showing the 
	Edit menu, to determine whether to enable the Edit menu 
	item corresponding to the given ASAtom. 
	@param win The current window. 
	@param editOp ASAtom specifying the edit operation. Must 
	be an ASAtom corresponding to one the strings: Cut, Copy, 
	Paste, Clear, SelectAll, and Undo.
	@return true if the specified operation can be performed, false 
	otherwise. 
	@see AVWindowPerformEditOpProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVWindowCanPerformEditOpProc)(AVWindow win, ASAtom editOp);

/**
	Callback for AVWindowHandler. Called when it's time to dispose 
	of the platformThing for the window passed to AVWindowNewFromPlatformThing. 
	@param win The window. 
	@param platformThing The platform-specific object (WindowPtr 
	in Mac OS, an HWND in Windows, and a Widget in UNIX) that 
	was used for this window.
	@see AVWindowNewFromPlatformThing 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVWindowDestroyPlatformThingProc)(AVWindow win, void *platformThing);
#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Data structure containing callbacks that implement a window 
	handler. The callbacks implement the window handler functions. 
	For example, resize the window, draw its contents, handle 
	mouse clicks, handle key presses. NULL values are acceptable; 
	default behavior is then used. 
	@see AVWindowNew 
	@see AVWindowNewFromPlatformThing 
*/
typedef struct _t_AVWindowHandler
{
	/** Size of the data structure. Must be set to sizeof( AVWindowHandlerRec). */
	ASSize_t size;

	/** Mouse clicks in the AVWindow are dispatched through this callback.*/
	AVWindowMouseDownProc MouseDown;

	/** 
		The window is about to close.  Returns true if the window should
		close; false to abort the operation.  If quitting is true,
		the application is in the middle of trying to quit.
	*/
	AVWindowWillCloseProc WillClose;

	/** 
		Called immediately after the window has closed, but before it's
		been freed (if one-shot).
	*/
	AVWindowDidCloseProc DidClose;

	/** Called after the window becomes the frontmost, active window.*/
	AVWindowDidActivateProc DidActivate;

	/** Called after the window becomes the key window */
	AVWindowDidBecomeKeyProc DidBecomeKey;

	/** Called to handle keystrokes when this is the key window */
	AVWindowKeyDownProc KeyDown;

	/** Called before the window ceases to be the key window */
	AVWindowWillResignKeyProc WillResignKey;
	
	/** Called before the window becomes deactivated or hidden. */
	AVWindowWillDeactivateProc WillDeactivate;

	/**
		called whenever the window needs to refresh some part of its
		interior.
	*/
	AVWindowDrawProc Draw;

	/**
		The window is about to be resized in accordance with the
		given rectangle.  This callback may alter the contents
		of newFrame to change the new frame size.  Returning false
		aborts the operation.
	*/
	AVWindowWillBeResizedProc WillBeResized;

	/**
		Called when the user has chosen an Edit menu item corresponding
		to the given ASAtom, assuming the corresponding CanPerformEditProc
		returned true.
	*/
	AVWindowPerformEditOpProc PerformEditOp;
	
	/**
		Called before showing the menus to determine whether or not a given
		edit menu item corresponding to the given ASAtom should be enabled.
	*/
	AVWindowCanPerformEditOpProc CanPerformEditOp;
	
	/**
		Called periodically while the cursor is over the AVWindow (if active).
		Use this to adjust the cursor's appearance.
	*/
	AVWindowAdjustCursorProc AdjustCursor;
	
	/**
		Called after a window-resize.
	*/
	AVWindowDidResizeProc DidResize;

	/**
		Called during DestroyWindow when it's time to dispose
		of the platformThing for the window passed into
		AVWindowNewFromPlatformThing.
	*/
	AVWindowDestroyPlatformThingProc DestroyPlatformThing;

	/**
		Called after a window is maximized.
	*/
	AVWindowDidMaximizeProc DidMaximize;

	/**
		Called after a window is collapsed.
	*/
	AVWindowDidCollapseProc DidCollapse;
	
	/**
		Called after a window is moved.
	*/
	AVWindowDidMoveProc DidMove;
} AVWindowHandlerRec, *AVWindowHandler;
#endif
/************************************************************************************\
|*																					*|
|*									AVGrafSelect									*|
|*																					*|
\************************************************************************************/
typedef struct _t_AVGrafSelect *AVGrafSelect;

/************************************************************************************\
|*																					*|
|*									AVAlert											*|
|*																					*|
\************************************************************************************/

/**@name AVAlertIcons Values for the IconType argument of AVAlert() 
	Standard icons used in alert boxes. 
	@see AVAlert 
	@see AVAlertConfirm 
	@see AVAlertNote  
*/
/*@{*/

/** */
#define ALERT_NOICON				0

/** */
#define ALERT_STOP					1

/** */
#define ALERT_CAUTION				2

/** */
#define ALERT_NOTE					3

/** */
#define ALERT_QUESTION				4
/*@}*/


/** Alert Types */
enum {

	/** */
	kAVAlertTypeNone = 0,

	/** */
	kAVAlertTypeOk,

	/** */
	kAVAlertTypeOkCancel,

	/** */
	kAVAlertTypeYesNo,

	/** */
	kAVAlertTypeYesNoCancel
};
typedef ASEnum16 AVAlertType;


/**
	Data structure containing information about a button used 
	in an Alert dialog. 
	@see AVAlertWithParams 
*/
typedef struct AVAlertButtonInfo {

	/** Pass true to show the button. */
	ASBool show;

	/** If non-NULL this text is used as the button caption, otherwise the default is used. The default values 
	for button1, button2 and button3 are "OK", "Cancel", and "" respectively.*/
	ASText title;	
} AVAlertButtonInfo;


/**
	Data structure containing information about a checkbox used 
	in an Alert dialog. 
	@see AVAlertWithParams 
*/
typedef struct AVAlertCheckBoxInfo {

	/** Pass true to show the button. */
	ASBool show;	

	/** If non-NULL this text is used as the checkbox caption, otherwise the default is used. The default value 
		for the checkbox is "Do not show this message again".
	*/
	ASText title;

	/** Pass true to initially check the box. The chosen value is returned in this parameter. */
	ASBool value;	
} AVAlertCheckBoxInfo;

/* AVAlertParams -- passed into AVAlertWithParams
*/

/**
	Data structure containing information about the format of 
	an Alert dialog.
	@see AVAlertWithParams 
*/
typedef struct _t_AVAlertParams
{

	/** The size of the structure. Must be set to sizeof(AVAlertParamsRec). */
	ASSize_t 			size;
	

	/** The AVDoc that is the modal parent of the alert dialog. May be NULL. The parentDoc can be NULL, 
		in which case the alert dialog is parented off of the currently active doc, if there is one. The parentDoc is a no-op on 
		the Mac. 
	*/
	AVDoc				parentDoc;

	/** The title of the dialog. May be NULL, in which case the default title, "Adobe Acrobat", is used. */
	ASText				windowTitle;

	/** The icon to display. Must be one of the AVAlert Icons. */
	AVIconType			iconType;

	/** The message to display. */
	ASText				message;

	/** Set to true to trigger a beep when the dialog is shown. */
	ASBool				beep;

	/** AVAlertButtonInfo structure describing the dialog's buttons. Any or all of these buttons may be NULL. If all 
		are NULL, the dialog is shown with an OK button. 
	*/
	AVAlertButtonInfo	button1;

	/** AVAlertButtonInfo structure describing the dialog's buttons. Any or all of these buttons may be NULL. If all 
		are NULL, the dialog is shown with an OK button. 
	*/
	AVAlertButtonInfo	button2;

	/** AVAlertButtonInfo structure describing the dialog's buttons. Any or all of these buttons may be NULL. If all 
		are NULL, the dialog is shown with an OK button. 
	*/
	AVAlertButtonInfo	button3;

	/** AVAlertCheckBoxInfo structure describing the dialog's checkbox. May be NULL. */
	AVAlertCheckBoxInfo	checkbox;

	/** The type of the alert. */
	AVAlertType			alertType;
	
	/** The window to parent the alert off of. Ignored if parentDoc is non-NULL. Ignored on Mac. */
	AVWindow			parentWindow;
}
AVAlertParamsRec, *AVAlertParams;

/************************************************************************************\
|*																																									*|
|*									AVAction																												*|
|*																																									*|
\************************************************************************************/

/*
** opaque action handler object: see AVAction.h
*/

/** Carries out an action. When the Acrobat viewer executes an action, it looks for the
	action handler with a type matching that of the action it is trying to execute. The
	Acrobat viewer invokes the matching handler to perform the action. If no match is
	found, the Acrobat viewer ignores the user action.
	@see AVAppGetActionHandlerByType
	@see AVAppEnumActionHandlers
	@see AVAppEnumActionHandlers
	@see AVAppGetActionHandlerByType 
	@see AVActionHandlerGetProcs 
	@see AVActionHandlerGetType 
	@see AVActionHandlerGetUIName
*/
typedef struct _t_AVActionHandler *AVActionHandler;

/*
** definition of action handler procs object used by AVAppRegisterActionHandler, also see AVActnIm.h
*/
typedef struct _t_AVActionHandlerProcs *AVActionHandlerProcs;


/**
	Callback used by AVAppEnumActionHandlers. It is called once 
	for each action handler. 
	@param type The action handler's name. 
	@param userName The action's name, as it appears in the 
	Acrobat viewer's user interface. 
	@param clientData User-supplied data that was passed in 
	the call to AVAppEnumActionHandlers.
	@return true to continue enumeration, false to halt enumeration. 
	@see AVActionHandlerGetProcs 
	@see AVAppEnumActionHandlers 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVActionEnumProc)(ASAtom type, char *userName, void *clientData);

/* An implementer of an action handler should fill out the functions in
 * this struct and call AVAppRegisterActionHandler.
 */

#define PDACTION_DESC_BUF_SIZE 256


/**
	Callback for AVActionHandlerProcs. Performs the action. 	
	@param actionHandlerObj IN/OUT? User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action IN/OUT? The action to perform. 
	@param doc IN/OUT? The document in which the action is located. 
	@see AVActionGetInstructionsProc 
	@see AVActionPerformExProc 
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *AVActionPerformProc)(void *actionHandlerObj, PDAction action, AVDoc doc);

/**
	Callback for AVActionHandlerProcs. Displays a user interface 
	that allows a user to set the action's properties (for example, 
	for a Launch action, set the file to launch; for a GoTo 
	action, select the destination page/ zoom/ coordinates). 
	@param actionHandlerObj IN/OUT? User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action IN/OUT? The action whose properties are set. 
	@param doc IN/OUT? The document in which the action is located. 
	@see AVActionHandlerGetProcs 
	@see AVActionDoPropertiesEx
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVActionDoPropertiesProc)(void *actionHandlerObj, PDAction action, AVDoc doc);

/**
	(Required) Callback for AVActionHandlerProcs. This required 
	function is called as soon as the user selects the action 
	from the action types pop-up menu. It allows an action handler 
	to populate a newly created action's actionDict. At the 
	time this method is called, the information needed to completely 
	specify an action is often not yet available. As a result, 
	this method is generally a good place to populate the actionDict 
	with default values. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param actionDict Action dictionary to populate with default 
	values. 
	@param doc The document in which the action is located.
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *AVActionFillActionDictProc)(void *actionHandlerObj, CosObj actionDict, AVDoc doc);

/**
	Callback for AVActionHandlerProcs. This optional function 
	should store into buffer a null-terminated C string which 
	contains localized instructions for the action creation/properties 
	dialog. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action The action whose instructions are returned. 
	@param buffer (Filled by the callback) The instruction 
	text. 
	@param bufLen Length of buffer, in bytes. 
	@param doc The document in which the action is located.
	@return The number of characters copied into buffer. 
	@see AVActionPerformExProc 
	@see AVActionPerformProc 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 AVTArraySize	(ACCBPROTO2 *AVActionGetInstructionsProc)(void *actionHandlerObj, PDAction action, char *buffer, AVTArraySize bufLen, AVDoc doc);

/**
	Callback for AVActionHandlerProcs. This optional function 
	should store into buffer a null-terminated C string which 
	is a localized string for the 'edit action' button in the 
	action dialog. 

	For example, the Acrobat viewer's built-in 'OpenFile' action 
	returns 'Select File' for this string. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action The action whose button text is returned. 
	@param buffer (Filled by the callback) The button text. 
	@param bufLen Length of buffer, in bytes. 
	@param doc The document in which the action is located.
	@return The number of characters copied into buffer. 
	@see AVActionGetStringOneTextProc 
	@see AVActionGetStringTwoTextProc 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 AVTArraySize	(ACCBPROTO2 *AVActionGetButtonTextProc)(void *actionHandlerObj, PDAction action, char *buffer, AVTArraySize bufLen, AVDoc doc);

/**
	(Optional) Callback for AVActionHandlerProcs. This function 
	should store into buffer a null-terminated C string which 
	is a localized string placed above the 'edit action' button 
	in the action dialog. A NULL proc will cause the button 
	to hide. 

	For example, the Acrobat viewer's built-in 'OpenFile' action 
	returns 'File N: \<the current filename\>' for this string. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action The action whose 'string 1' text is returned. 
	@param buffer (Filled by the callback) The string text 
	appearing above the button. 
	@param bufLen Length of buffer, in bytes. 
	@param doc The document in which the action is located.
	@return The number of characters copied into buffer. 
	@see AVActionGetButtonTextProc 
	@see AVActionGetStringTwoTextProc 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 AVTArraySize	(ACCBPROTO2 *AVActionGetStringOneTextProc)(void *actionHandlerObj, PDAction action, char *buffer, AVTArraySize bufLen, AVDoc doc);

/**
	Callback for AVActionHandlerProcs. This optional function 
	should store into buffer a null-terminated C string which 
	is a localized string placed below the 'edit action' button 
	in the action dialog. 

	For example, the Acrobat viewer's built-in 'OpenFile' action 
	returns nothing for this string, but the built-in 'GoToView' 
	action returns a description of the current zoom type. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action The action whose 'string 2' text is returned. 
	@param buffer (Filled by the callback) The string text 
	appearing below the button. 
	@param bufLen Length of buffer, in bytes. 
	@param doc The document in which the action is located.
	@return The number of characters copied into buffer. 
	@see AVActionGetButtonTextProc 
	@see AVActionGetStringOneTextProc 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 AVTArraySize	(ACCBPROTO2 *AVActionGetStringTwoTextProc)(void *actionHandlerObj, PDAction action, char *buffer, AVTArraySize bufLen, AVDoc doc);

/**
	(Optional) Callback for AVActionHandlerProcs. Called upon 
	to copy an action, possibly to another document. Action 
	handlers should provide this callback to allow for copying 
	their actions. Called by AVDocCopyAction. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param fromDoc The document whose action is copied. 
	@param anAction The action to copy. 
	@param toDoc The document to which the action is copied.
	@return The newly created PDAction copy. 
	@see AVDocCopyAction 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 PDAction (ACCBPROTO2 *AVActionCopyProc)(void *actionHandlerObj, AVDoc fromDoc, PDAction anAction, AVDoc toDoc);

/**
	Optional callback for AVActionHandlerProcs. Called to get 
	informational details to present to the user. The handler 
	can provide one or more ASText strings which are added to 
	the ASCab provided by the caller. Generally, each string 
	should form a single informational item, often provided 
	in a "key: value" format. The caller will organize the strings 
	into a list that is presented in the user interface. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param doc The document in which the action is located. 
	@param action The action whose details are returned. 
	@param details (Filled by the callback) The ASCab to populate 
	with the informational details, provided as ASText strings.
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVActionGetDetailsProc)(void *actionHandlerObj, AVDoc doc, PDAction action, ASCab details);

/**
	This structure gives the action handler some context in			
	terms of its execution. It specifies the " parent" object, 
	which initiated the action, and the trigger type of the 
	action. Trigger names should correspond to the key used 
	in the AA dictionary of the file. 
	@see AVActionPerformExProc 
*/
typedef struct _AVActionContextRec {

	/** Size of this record. context. size= sizeof(AVActionContextRec). */
	ASSize_t size;	

	/** Object that is performing the action - e. g., bookmark or annotation. */
	CosObj co;		

	/** Type of the object, using these defines: 
	<pre>
		#define AVTRIGGERTYPE_DOC ASAtomFromString("Doc") 
		#define AVTRIGGERTYPE_DEST ASAtomFromString("Dest") 
		#define AVTRIGGERTYPE_PAGE ASAtomFromString("Page") 
		#define AVTRIGGERTYPE_LINK ASAtomFromString("Link") 
		#define AVTRIGGERTYPE_ANNOT ASAtomFromString("Annot") 
		#define AVTRIGGERTYPE_BOOKMARK ASAtomFromString("Bookmark") 
	</pre>
	*/
	ASAtom asaType;	

	/** Trigger name for the object, using these defines: 
	<pre>
		#define AVTRIGGER_MOUSEUP ASAtomFromString("Mouse Up") 
		#define AVTRIGGER_OPEN ASAtomFromString("Open") 
		#define AVTRIGGER_CLOSE ASAtomFromString("Close") 
	</pre>
	*/
	ASAtom asaKey;	
} AVActionContextRec, *AVActionContext;

#define AVTRIGGERTYPE_DOC			ASAtomFromString("Doc")
#define AVTRIGGERTYPE_DEST			ASAtomFromString("Dest")
#define AVTRIGGERTYPE_PAGE			ASAtomFromString("Page")
#define AVTRIGGERTYPE_LINK			ASAtomFromString("Link")
#define AVTRIGGERTYPE_ANNOT			ASAtomFromString("Annot")
#define AVTRIGGERTYPE_BOOKMARK		ASAtomFromString("Bookmark")
#define AVTRIGGER_MOUSEUP			ASAtomFromString("Mouse Up")
#define AVTRIGGER_OPEN				ASAtomFromString("Open")
#define AVTRIGGER_CLOSE				ASAtomFromString("Close")
#define AVTRIGGER_SHOW				ASAtomFromString("Show")
#define AVTRIGGER_HIDE				ASAtomFromString("Hide")


/**
	Callback for AVActionHandlerProcs. If defined, it is called 
	instead of AVActionPerformProc. It gets passed the context 
	of an action, which provides information on who triggered 
	the action. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param action An action to perform. 
	@param doc The document in which the action is located. 
	@param context The context of the action.
	@see AVActionPerformProc 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *AVActionPerformExProc)(void *actionHandlerObj, PDAction action, AVDoc doc, AVActionContext context);

/**
	Callback for AVActionHandlerProcs. If defined, it is called 
	instead of DoProperties. 

	Called to allow the user to view or edit the properties 
	for an array of actions. Typically this presents a modal 
	OK/Cancel dialog to the user. The dialog should graphically 
	denote properties that do not have the same value for every 
	action in the array by using an "indeterminant" state. For 
	example, if the action has a flag property that is set to 
	true for one action and false for another, instead of "true" 
	or "false", the dialog could show the string "<varies>" 
	for that property. The user will still be able to change 
	the "<varies>" string to either true or false, which would 
	apply the new value to every action in the array. 
	@param actionHandlerObj User-supplied data that was passed 
	when the action handler was registered using AVAppRegisterActionHandler. 
	@param doc The document in which the actions are located. 
	@param actions An array of actions whose properties are 
	set. 
	@param numActions The number of actions in the array.
	@return true if the actions were changed, false if unchanged. 
	@see AVActionHandlerGetProcs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVActionDoPropertiesExProc)(void *actionHandlerObj, AVDoc doc, PDAction *actions, ASArraySize numActions);

/** Data structure containing callbacks that implement an action handler. The callbacks
	implement the action handler functions. For example, display user interface text,
	request the action’s properties from the user, perform the action.
	@see AVAppRegisterActionHandler
	@see AVActionHandlerGetProcs
	@see AVDocCopyAction
*/
typedef struct _t_AVActionHandlerProcs {

	/** Size of the data structure. Must be set to sizeof(AVActionHandlerProcsRec). */
	ASSize_t size;

	/** */
	AVActionPerformProc Perform;

	/** */
	AVActionDoPropertiesProc DoProperties;

	/** */
	AVActionFillActionDictProc FillActionDict;

	/** */
	AVActionGetInstructionsProc GetInstructions;

	/** */
	AVActionGetButtonTextProc GetButtonText;

	/** */
	AVActionGetStringOneTextProc GetStringOneText;

	/** */
	AVActionGetStringTwoTextProc GetStringTwoText;

	/** */
	AVActionCopyProc Copy;

	/** */
	AVActionPerformExProc PerformEx;

	/** */
	AVActionDoPropertiesExProc DoPropertiesEx;

	/** */
	AVActionGetDetailsProc GetDetails;

} AVActionHandlerProcsRec;


/************************************************************************************\
|*																					*|
|*									AVAuxData										*|
|*																					*|
\************************************************************************************/


/**
	(Optional) Callback for AVAuxDataHandler. It is called to 
	process auxiliary data sent to the AVDoc using AVDocSendAuxData. 
	This callback must process the data appropriately for whatever 
	auxDataType is sent. 

	If NULL, default behavior is used. 
	@param auxDataType IN/OUT? Specifies the type of auxData. This 
	determines how auxData is interpreted. 
	@param auxData IN/OUT? The auxiliary data. 
	@param auxDataLen IN/OUT? The length of auxData, in bytes. 
	@param avDoc IN/OUT? The document with which the auxiliary data 
	is associated. 
	@return true if the data is acted upon, false if nothing is done. 	
	@see AVDocSendAuxData 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVAuxDataPerformProc)(ASAtom auxDataType, void *auxData, AVTBufferSize auxDataLen, AVDoc avDoc);


/**
	Data structure containing callbacks and data representing 
	an auxiliary data handler.  NULL values are acceptable; default behavior is then used.
	@see AVDocSendAuxData 
	@see AVHasAuxDataHandler 
	@see AVRegisterAuxDataHandler 
*/
typedef struct _t_AVAuxDataHandler {                                          

	/** Size of the data structure. Must be set to sizeof( AVAuxDataHandlerRec). */
	ASSize_t size;

	/** Called with auxiliary data when a client calls AVDocSendAuxData. This proc should perform whatever 
		action it needs to do for the auxiliary data.
	*/
	AVAuxDataPerformProc PerformProc; 
} AVAuxDataHandlerRec, *AVAuxDataHandler;                                     

/************************************************************************************\
|*																					*|
|*									AVDocPrintParams								*|
|*																					*|
\************************************************************************************/

/* AVRect32 only needed for support of 32-bit coordinates before Acrobat 6, preferred method is to use AVRect with ACRO_SDK_LEVEL >= 0x00060000 */
#if (ACRO_SDK_LEVEL >= 0x00060000)
typedef AVRect AVRect32;

/**
	Data structure representing a rectangle (a quadrilateral 
	having only horizontal and vertical sides). 

	The AcroView coordinate system is defined so that (0,0) 
	is at the top, x increases to the right, and y increases 
	down (the same as GDI and QuickDraw but opposite to the 
	PostScript language). An AVRect32 is defined so that its 
	top is above its bottom, but this means that 0 < top < bottom. 
	

	NOTE: This structure supports 32-bit coordinates in versions 
	prior to Acrobat 6.0. For version 6.0 and higher, AVRect 
	is preferred. 
*/
typedef AVRectP AVRect32P;

/* Define a platform specification for a printer */


#if UNIX_PLATFORM

typedef struct _t_ASPlatformPrinterSpec {
	/** Size of the data structure. Must be set to
		sizeof(ASPlatformPrinterSpecRec).
	*/
	ASSize_t size;
	/** Print command, such as ?p -dMyPrinter -n4? If printerName is
		NULL, a default print command is used. The Acrobat viewer? built-in
		default is ?p?on most UNIX systems, and ?pr?on SunOS. This should
		print to the system? default printer. Some UNIX systems also look at
		the environment variable LPDEST or PRINTER. See the
		documentation for your platform to determine whether this is the
		case.
	*/
	char *printerName;	
	/* Currently unused. */
	ASUns8 *baseAddr;				
	/* Currently unused. */
	AVBufferSize rowBytes; 
	/* Currently unused. */
	AVBufferSize depth;		
	/* Currently unused. */
	AVRect bounds;            
} ASPlatformPrinterSpecRec, *ASPlatformPrinterSpec;
#endif /*UNIX_PLATFORM*/
#endif

/* Define a platform specification for a printer */

#if MAC_PLATFORM
#define kPrinterSpecNameLen 256	
typedef struct _t_ASPlatformPrinterSpec {
	/** Size of the data structure. Must be set to
		sizeof(ASPlatformPrinterSpecRec).
	*/
	ASSize_t size;
	/** Port to print to. */
	CGrafPtr *cGrafPtr;	
	/** Best known resolution of current printer. */
	short	hRes, vRes;	
	/** Name of printer. */
	UniChar printerName[kPrinterSpecNameLen];
} ASPlatformPrinterSpecRec, *ASPlatformPrinterSpec;
#endif


#if WIN_PLATFORM
#define kPrinterSpecNameLen 64	/* room for 32 Unicode chars */

/**
	Data structure representing a platform specification for 
	a printer. Used in AVDocPrintParams. 
	@see AVDocPrintPagesWithParams 
*/
typedef struct _t_ASPlatformPrinterSpec {
	/** Size of the data structure. Must be set to sizeof( ASPlatformPrinterSpecRec). */
	ASSize_t size;							

	/* The following should be provided for full, non-interative printing */

	/** See Windows. h DEVNAMES for a description of these fields. */
	char driverName[kPrinterSpecNameLen];

	/** Name of the printer. For example, "HPPCL", "HP LaserJet 4", or "LPT1".  */
	char printerName[kPrinterSpecNameLen];	

	/** Port to print to. */
	char portName[kPrinterSpecNameLen];		 

	/* The following must be provided for embedded printing */

	/** Must be true if Windows 32-bit platforms; optional for Windows 16-bit platforms. */
	ASBool		createMetaFile;			

	/** Pathname for the metafile. Only required if createMetaFile is true. */
	char		metaFileName[260];		

	/** HDC for Win16 embedded printing, unused on Windows 32-bit platforms */
	ASInt32		win16Hdc;				

	/** Horizontal resolution of printer; 300 dpi, for example. */
	ASInt32		hRes;					

	/** Vertical resolution of printer; 300 dpi, for example. */
	ASInt32		vRes;					

	/**  Color depth of device; typically 1, 8, or 24. This determines the depth of images created for the 
		printer. You may specify 24 when printing to a monochrome printer. The driver is expected to convert to the printer 
		depth. Not used if isPostScript is true.
	*/
	ASInt32		colorDepth;				


	/** Set to true if printing to a PostScript	printer. */
	ASBool		isPostScript;			
} ASPlatformPrinterSpecRec, *ASPlatformPrinterSpec;
#endif

#if OS2_PLATFORM
#define kPrinterSpecNameLen 64	/* room for 64 chars */
typedef struct _t_ASPlatformPrinterSpec {
	ASSize_t size;							/* set this to be the size of this struct */

	/* The following should be provided for full, non-interative printing */
	char driverName[kPrinterSpecNameLen];	/* See Windows.h DEVNAMES for a description of these fields */
	char printerName[kPrinterSpecNameLen];	/* For example, {"HPPCL", "HP LaserJet 4", "LPT1:"}, or, */
	char portName[kPrinterSpecNameLen];		/* 				{"HP LaserJet 4/4M PS", "\\PCCAROUSEL\4PLUS", "\\PCCAROUSEL\4PLUS"} */ 

	/* The following must be provided for embedded printing */
	ASBool		createMetaFile;			/* Must be true if Win32; may be optional for Win16 (pending testing). */
	char		metaFileName[260];		/* Pathname for the metafile. Only required if createMetaFile is true. */
	ASInt32		win16Hdc;				/* for possible use in Win16 */
	ASInt32		hRes;					/* horizontal resolution of printer, e.g. 300 dpi */
	ASInt32		vRes;					/* vertical resolution of printer, e.g. 300 dpi */
	ASInt32		colorDepth;				/* Color depth of device; typically 1, 8 or 24.
										** This determines the depth of images created for the 
										** printer. You may specify 24 when printing to 
										** a monochrome printer. The driver is expected to
										** convert to the printer depth.
										** Not used if isPostScript.
										*/
	ASBool		isPostScript;			/* Set to true if printing to PS printer. */
	ASInt32	psHeight;			/* Printer Presentation space height */
} ASPlatformPrinterSpecRec, *ASPlatformPrinterSpec;
#endif


/**@name emitFileOption
	enum for AVDocPrintParams.emitFileOption 
	@see AVDocPrintParams
*/
enum {

	/** PS file */
	kAVEmitFilePS,           

	/** EPS file with no preview */
	kAVEmitFileEPSNoPrev,     

	/** EPS file with standard preview */
	kAVEmitFileEPSMacStdPrev, 

	/** EPS file with extended preview */
	kAVEmitFileEPSMacExtPrev  
};


/**@name emitFontOption enum for AVDocPrintParams.emitFontOption 
	The following enumeration is obsolete. It is maintained
	here solely for backward-compatability.
*/
enum {

	/** Embed no fonts */
	kAVEmitFontNoFonts,        

	/** Embed all type 1 embedded fonts */
	kAVEmitFontAllEmbType1,    

	/**
	Constants that specify tile marking styles for AVDocPrintTileData, 
	used in AVDocPrintParams. 
	@see AVDocPrintPagesWithParams
	*/
	kAVEmitFontAllType1      
};

/** Constants that specify tile marking styles for AVDocPrintTileData, 
	used in AVDocPrintParams. 
	@see AVDocPrintPagesWithParams
*/
enum {
	/** */
	kAVTileMarkNone =0,
	/** */
	kAVTileMarkWestern,
	/** */
	kAVTileMarkEastern
};
typedef ASEnum16 AVTileMark;

/** The new EmitFont enumeration is covers
	all font types, not just Type 1.
*/
enum {
	/** Emit all embedded fonts */
	kAVEmitFontEmbeddedFonts = 1, 
	/** Emit all fonts */
	kAVEmitFontAllFonts        	   
};


/** 	Constants that specify page size types for AVDocPrintParams, 
	introduced in Acrobat 6.0. If the pageSize field if not 
	set, the old field of shrinkToFit is used instead. 
	@see AVDocPrintPagesWithParams
*/
enum {

	/** */
	kAVPageSizeUninitialized =0,

	/** kAVPageSizeNone - do no adjusting for size, pages are cropped by the printer */
	kAVPageSizeNone,

	/** */
	kAVPageSizeFitToPaper,

	/** */
	kAVPageSizeShrinkLargePages,

	/** */
	kAVPageSizeTileLargePages,

	/** */
	kAVPageSizeTileAllPages,

	/** N-up printing mode */
	kAVPageSizeNup
};
typedef ASEnum16 AVPageSize;


/** Constants that specify color forcing values for AVDocPrintOverrideData, 
	used in AVDocPrintParams. 

	Use AVForceColor when a print driver incorrectly handles 
	conversion of color to grayscale or monochrome. This overrides 
	the Windows printerspec colorDepth value. As of 1/1/02, 
	only WinFax is known to need this setting since it incorrectly 
	converts all colors to black. 
	@see AVDocPrintPagesWithParams
*/
enum {

	/** do not force color conversions */
	kAVForceColorNone =0,  

	/** */
	kAVForceColorToGrayscale,

	/** */
	kAVForceColorToMonochrome
};
typedef ASEnum16 AVColorForcing;

/**
	Constants that determine whether to let Acrobat decide whether 
	to use a feature, or whether to force Acrobat to use or 
	not to use the feature. Used in AVDocPrintOverrideData, 
	which is used in AVDocPrintParams. 
	@see AVDocPrintPagesWithParams 
*/
enum {
	/** Let Acrobat decide whether to use a feature. */
	kAVuseAuto =0,  
	/** Force Acrobat to use a feature, even if Acrobat thinks it will not work. */
	kAVuse, 
	/** Do not use a feature, even if Acrobat thinks it will work.*/
	kAVnoUse 
};
typedef ASEnum16 AVUseValue;

/** Font and resource policy values for AVDocPrintParams. */
enum {

	/**	Send fonts and resources on the first page they are used. 
		Pages must be printed in order they are created. Fonts are 
		downloaded in global VM. Fonts and resources are removed 
		when they are no longer needed. 
	*/
	kAVResPolicySendAtStart, 

	/**	Send fonts and resources on each page they are used. Fonts 
		are downloaded in local VM.
	*/
	kAVResPolicySendByRange,

	/** kAVResPolicySendPerPage - Send fonts and resources on each page they are used. 
		Fonts are downloaded in local VM.
    */
	kAVResPolicySendPerPage
};
typedef ASEnum16 AVResourcePolicy;

/** N-up printing page order for AVDocPrintNupData (used by AVDocPrintParams) */
enum {
	/** pages are placed from left to right, and then repeat it from top to bottom. */
	kAVnUpPageOrderHorizontal,
	/** Pages are placed from right to left, and then repeat it from top to bottom. */
	kAVnUpPageOrderHorizontalRev,
	/** pages are placed from top to bottom, and then repeat it from left to right. */
	kAVnUpPageOrderVertical,
	/** pages are placed from top to bottom, and then repeat it from right to left. */
	kAVnUpPageOrderVerticalRev
};
typedef ASEnum16 AVnUpPageOrder;

/**@name EmitFlags (used byAVDocPrintParams)
	Enumerated data types used to specify additional print emit 
	options. 
	@see AVDocPrintPagesWithParams 
*/
/*@{*/

/** Emit the halftones specified in the document. */
#define	kAVEmitHalftones			0x00000001	

/** Raise an error on images that 
	cannot be represented in EPS files, following the separation 
	conventions in Technical Note # 5044, Color Separation Conventions 
	for PostScript Language Programs.
*/
#define kAVEmitSeparableImagesOnly	0x00000002 

/** Do not emit the cropbox page clip. */
#define	kAVSuppressCropClip			0x00000004	

/** Do not emit the transfer functions in the document.*/
#define	kAVSuppressTransfer			0x00000008	

/** Do not emit the BlackGeneration in the document. */
#define	kAVSuppressBG				0x00000010	

/** Do not emit the UnderColorRemovals in the document. */
#define	kAVSuppressUCR				0x00000020	

/** If the field is set, calls to AVDocPrintPagesWithParams generate PostScript that suppresses printer-based font substitution 
	for CJK fonts.
*/
#define kAVSuppressCJKFontSubst		0x00000040 

/** (New for 5.0) Do not rotate the page. */
#define kAVSuppressRotate			0x00000080  

/** (New for 5.0) Do not center the page. */
#define kAVSuppressCenter			0x00000100  

/** (New for 5.0) Do not emit text annotations. */
#define kAVSuppressAnnots			0x00000200  

/** (New for 5.0) Enable setPageSize, choose paper tray by PDF page size. */
#define kAVSetPageSize				0x00000400  

/** (New for 5.0) Attempt to reduce amount of VM used on PostScript printers. */
#define kAVSaveVM					0x00000800  

/** (New for 5.0) PostScript only -If set, PostScript is optimized for speed otherwise pages must be independent.*/
#define kAVOptimizeForSpeed			0x00001000 

/** (New for 5.0) When printing via Windows' Dynamic Data Exchange (DDE), alerts are not generated; false is returned. */
#define kAVSilent					0x00002000	

/** (New for 5.0) Normally, we emit a clip to BoundingBox for EPS, for one client, this breaks their workflow. */
#define kAVEmitBBoxClip				0x00004000 
/* #define kAVEmitTransferFuncs		0x00008000	- obsolete, use kAVSuppressTransfer */

/* #define kAVPrintICCColorsAsDevice	0x00010000	- obsolete */

/** (New for 5.0) When set, the print path behaves as if the user had clicked the Apply Overprint Preview 
	checkbox in the Advanced print dialog.
*/
#define kAVApplyOverPrint			0x00020000	

/** (New for 5.0) When set, the print path behaves as if the user had clicked the Apply Working 
	Color Spaces checkbox in the Advanced print dialog.
*/
#define kAVApplyWorkingColorSpaces	0x00040000	

/** PostScript only - do include PostScript XObjects' content in output */
#define kAVEmitPostScriptXObjects	0x00080000 

/** emit only form fields, takes precedence over kAVSuppressAnnots */
#define kAVEmitFormFieldsOnly		0x00100000 

/** Use the softProofing settings before doing color management */
#define kAVApplySoftProofSettings   0x00200000 

/** when emitting forms, use execform operator */
#define kAVEmitFormsAsPSForms		0x00400000	

/** when emitting JPEG2000 images, use maximum available resolution */
#define kAVMaxJP2KRes				0x00800000	

/** emit TrapNet annots, even if suppress annots is on */
#define kAVTrapAnnots				0x01000000	

/** emit printer's mark annots, even if suppress annots is on.  Default off */
#define kAVPrintersMarks			0x02000000	

/** provide control over the emission of the setflat operator */
#define	kAVEmitFlatness				0x04000000

/** Emit CJK TrueType fonts as CID Type 2 fonts */
#define	kAVEmitCJKTTAsType2			0x08000000

/** Soft proofing: simulate ink black and paper white when doing soft proofing*/
#define kAVSimulateInkBlack         0x10000000
#define kAVSimulatePaperWhite       0x20000000

/** emit document + stamp annotations only */
#define kAVEmitStampsOnly			0x40000000

/*@}*/


/**@name EmitFlags (used byAVDocPrintParams)
	Enumerated data types used to specify additional print emit 
	options. 
	@see AVDocPrintPagesWithParams 
*/
/*@{*/

enum {	/* marksStyleType */
	/** No flags == InDesign style printer marks */
	kAVDefaultMarkType = 0,		/* Acrobat defaults to InDesign style marks */
	kAVInDesignJ1MarkType,		/* InDesignJ1 */
	kAVInDesignJ2MarkType,		/* InDesignJ2 */
	kAVIllustratorMarkType,  	/* Illustrator */
	kAVIllustratorJ,			/* IllustratorJ */
	kAVQuarkXPress				/* QuarkXPress */
};

/* flags to fill in marksFlags field */
/**	Emit crop marks. */
#define kAVCropMarks			0x00000001	
/**	Emit trim marks. */
#define kAVTrimMarks			0x00000002
/**	Emit bleed marks. */
#define kAVBleedMarks			0x00000004
/**	Emit registration marks. */
#define kAVRegMarks			0x00000008
/**	Emit color bar marks. */
#define kAVColorBarMarks			0x00000010
/**	Emit page information marks. */
#define kAVPageInfoMarks			0x00000020

/**	Emit Eastern style marks (default is Western style). Obsolete for Acrobat 7,
replaced by marksStyle field */
#define kAVEasternStyleMarks		0x00000040
/**
	AVDocPrintParams - used in the call AVDocPrintPagesWithParams().
*/
typedef struct _t_AVDocPrintTileData {
	/** set this to be the size of this struct */
	ASSize_t size;							
	/** pts tiled pages overlap */
	ASUns32 overlap;  
	/** 1.0 = normal scale */
	ASFixed scale; 
	/** */
	ASBool label;
	/** uses AVTileMark enum */
	AVTileMark markType;  
} AVDocPrintTileDataRec, *AVDocPrintTileData;

typedef struct _t_AVDocPrintNupData {
	/** set this to be the size of this struct */
	ASSize_t	size;
	/** Number of pages on the horizontal direction of the page.
	    The total number of pages per sheet is (numPagesPerSheetH * numPagesPerSheetV).
	*/
	ASInt32		numPagesPerSheetH;
	/** Number of pages on the vertical direction of the page.
	    The total number of pages per sheet is (numPagesPerSheetH * numPagesPerSheetV).
	*/
	ASInt32		numPagesPerSheetV;
	/** N-up page order */
	AVnUpPageOrder	pageOrder;
	/** Print page borders during N-up printing. */
	ASBool		pageBorder;
	/** Automatically rotate each page to match the page orientation to the available paper area. */
	ASBool		autoRotate;
} AVDocPrintNupDataRec, *AVDocPrintNupData;

/**
	Constants that specify rasterization methods to use for 
	printing. Used in AVDocPrintRasterizeData, which is used 
	in AVDocPrintParams. 
	@see AVDocPrintPagesWithParams
*/
enum {
	/** No flags */
	kAVRasterizeNoFlags					= 0,	
	/** */
	kAVRasterizeAllTextToOutlines		= 1 << 0,	
	/** */
	kAVRasterizeAllStrokesToOutlines	= 1 << 1,
	/** */
	kAVRasterizeAllowComplexClipRegions	= 1 << 2,	
	/** */
	kAVRasterizePreserveOverprint		= 1 << 3
};
typedef ASUns32 AVRasterizeFlags;

#define kPrintMinDPI 1
#define kPrintMaxDPI 9600
#define kPrintDefaultDPI	300
#define kPrintMinGradDPI 1
#define kPrintMaxGradDPI 9600
#define kPrintDefaultGradDPI 150


/**
	Structure specifying rasterization parameters used by AVDocPrintParams. 
	Constant DPI values are defined in AVExpT.h. 
	@see AVDocPrintPagesWithParams 
*/
typedef struct _t_AVDocPrintRasterizeData {
	/** Size of the data structure. Must be set to 
		sizeof(AVDocPrintRasterizeData). 
	*/
	ASSize_t size;							
	/** An OR of bit flags that control the rasterization 
		method to use for printing. 
	*/
	AVRasterizeFlags flags; 
	/** Degree of transparency, in the range 
		1 to 100, where 1 means all raster, and 100 means preserve 
		as much transparency data as possible. 
	*/
	ASInt32 transparency; 
	/** DPI for bitmaps. Default is 300. 
		This value is also used when printAsImage is true. 
	*/
	ASUns32 bitmapResolution; 
	/** DPI for gradients interior to 
		the object (not edges). Can generally be lower than the 
		bitmapResolution. Default is 150.
	*/
	ASUns32 gradientResolution; 
	/** 300 default */
	ASUns32 printAsImageResolution;	
} AVDocPrintRasterizeDataRec, *AVDocPrintRasterizeData;

/**
	Structure specifying override parameters used by AVDocPrintParams. 	
	@see AVDocPrintPagesWithParams 
*/
typedef struct _t_AVDocPrintOverrideData {
	/** Size of the data structure. Must be set to sizeof(AVDocPrintOverrideDataRec). */
	ASSize_t size;					
	/** Whether to use color forcing. Default 
		is AVColorForceNone. Can be used to send 1 bit or 8 bit 
		data to printer instead of 24 bit. Ignored on Mac OS. 
	*/
	AVColorForcing colorOverride;
	/** Whether to allow the viewer to decide 
		if it should use the printer's PostScript Color Rendering 
		Dictionary. Default is kAVuseAuto, which is normally true. 
		but is false where the printer is listed as broken, as in 
		these cases: 
		- When printer-based color management is used, 
		printers whose CRD is incorrect produce light gray or peach 
		for white expressed in an ICC color space. 
		-the CRDs for Tektronix Phaser 740 and Xerox Docuprint N2125 
		are known to be incorrect, and this may be so for others, 
		especially in these printer families. It is possible to 
		change the printer defaults, to list additional printers, 
		for example, or to remove a printer from the broken list 
		if the driver if fixed. Contact Adobe technical support 
		for details.
	*/
	AVUseValue usePrinterCRD; 
	/** Whether to allow the viewer to 
		decide if it should use T1 color conversion (that is, convert 
		Type1 fonts to more efficient font representations) for 
		better performance . Ignored on Mac OS. Default is kAVuseAuto, 
		which is normally true, except in cases where the printer 
		cannot handle optimized fonts and needs full, slow, Type1 
		fonts. Only the Windows 16 bit HP LJ 4siMX 
		driver is known to be in this category. It is possible to 
		change the printer defaults, to list additional printers, 
		for example, or to remove a printer from the broken list 
		if the driver if fixed. Contact Adobe technical support 
		for details.
	*/
	AVUseValue useT1Conversion; 
} AVDocPrintOverrideDataRec, *AVDocPrintOverrideData;

/**
	Structure that specifies how to print a document. 
	@see AVDocPrintPagesWithParams 
	Structure specifying rasterization parameters used by AVDocPrintParams. 
	Constant DPI values are defined in AVExpT.h. 
	@see AVDocPrintPagesWithParams 
*/
typedef struct _t_AVDocPrintParams *AVDocPrintParams;
#if (ACRO_SDK_LEVEL >= 0x00060000)

#define kPrintUserNoteLen 256
/**
	Structure that specifies how to print a document. 
	@see AVDocPrintPagesWithParams 
*/
typedef struct _t_AVDocPrintParams {

	/** Size of the data structure. Must be set to sizeof( AVDocPrintParamsRec). */
	ASSize_t size;	

	/** Displays a Print dialog box and print status window while 
		printing. Mutually exclusive with embedded, emnitToPrinter and emitToFile. */
	ASBool interactive;						

	/** If interactive is false and cancelDialog is true, aCancel dialog box appears.*/
	ASBool cancelDialog;					

	/** Used if emitToPrinter or emitToFile are true. First page to be printed; zero-based; if -1, then all pages are printed */
	ASInt32 firstPage;						

	/** Used if emitToPrinter or emitToFile are true. Last page to be printed; if firstPage is -1, this page is ignored */
	ASInt32 lastPage;						

	/** Used if emitToPrinter or emitToFile are true. 
	If printing to PostScript, 2 means level 2, 3 means level 3 
	NOTE: With Acrobat 7, level 1 is no longer supported. Attempts to print with psLevel == 1 will
	generate a bad parameter exception. No output will be generated.
	*/
	ASInt32 psLevel;						

	/** Used if emitToPrinter or emitToFile are true. Set to true if a binary channel to the printer is supported */
	ASBool binaryOK;						

	/** Used if emitToPrinter or emitToFile are true. OBSOLETE.  Use pageSize instead.  If pageSize uninitialized and this is true, the page is shrunk or expanded to fit the printer page size */
	ASBool shrinkToFit;	

	/** Used if emitToPrinter or emitToFile are true. The file system name; see filePathName. For the operation of printing to a printer
		(emitToPrinter = true), if filePathName is specified, fileSysName must be the name of the
		default file system. You can get the file system’s name from the ASFileSysGetFileSysNameProc callback
		in the ASFileSysRec of the file system you are using.
	*/
	ASAtom fileSysName;					

	/** Used if emitToPrinter or emitToFile are true. If non-NULL, filePathName is a platform path for the specified fileSysName, or, if fileSysName is
		ASAtomNull, it is one of the following: In Windows: a C-string pathname In Mac OS: a FSSpecPtr
		In UNIX: a C-string pathname
	
		If emitToPrinter is true, and filePathName is non-NULL, then the system printer driver
		is used to emit the output stream to the file. This is implemented for Windows only.
	*/
	ASPathName filePathName;			

	/** Optionally used if interactive, embedded, or emitToPrinter is true. If NULL, a default system
		printer is used. If non-NULL, printerSpec is a platform-specific value. Must be an ASPlatformPrinterSpec.
	*/
	ASPlatformPrinterSpec printerSpec;	

	/** true if an embedded view of a page to print, false
		otherwise. Mutually exclusive with interactive, emitToPrinter and emitToFile.
		- firstPage only is used.  last page is ignored
		- The printer must be specified as a metafile or CGrafPtr.
	*/
	ASBool embedded;	

	/** size of embedded output in device units */
	AVRect embeddedRect;				

	/** If true, use the system printer driver for output. If filePathName is specified, uses the driver to create
		the file. Raises genErrBadParm if an invalid parameter is provided (for example, printing to PDFWriter, Distiller, or
		to a printer that has been un-installed). Mutually exclusive with interactive, embedded and emitToFile.
	*/
	ASBool emitToPrinter;					

	/* Parameters for emission of PS Level 1 Color Separations and EPS, or vanilla PS.
	** Creates and writes to filePathName (may not be NULL).
	** Does NOT use system printer driver.
	** Only has partial font emitting capabilities on some platforms:
	** 	Mac:	embedded and system Type 1 fonts only; no TrueType or substitution fonts.
	**	Win:	embedded and system Type 1 fonts only; no TrueType or substitution fonts.
	**	UNIX:	all fonts
	*/

	/** If true, emit non-interactive output to a file. Used to	emit color separations or EPS. This flag cannot be used
		with the Acrobat Reader. Mutually exclusive with interactive, embedded and emitToPrinter. 
	*/
	ASBool emitToFile;

	/** Perform level 1 color separations. See Color Separation Conventions for PostScript Language Programs, Technical Note #5044. */
	ASBool doColorSeparations;				

	/** File output options: PostScript or EPS, with or without a preview. Must be one of the following values:
		- kAVEmitFilePS - PostScript file
		- kAVEmitFileEPSNoPrev - EPS file with no preview
		- kAVEmitFileEPSMacStdPrev - EPS file with standard preview
		- kAVEmitFileEPSMacExtPrev - EPS file with extended preview
	*/
	ASEnum8 emitFileOption;					

	/** Font output options. Must be one of EmitFontOptions.
		@see EmitFontOptions
	*/
	ASEnum8 emitFontOption;					

	/* More emit options. */

	/** Additional emit options, such as kAVEmitHalftones. Must be one of Emit Flags.
		@see emitFlags
	*/
	ASUns32	emitFlags;					

	/** Must be a PDPageRange. Take precedence over firstPage, lastPage members */
	PDPageRange *ranges; 

	/** Range of pages to print. */
	AVTSmallArraySize numRanges;
	/* control over TrueType --> Type 1 conversion for PostScript printing */

	/** If true, send TrueType fonts as TrueType fonts (level 3 and most level 2 PostScript printers. If false, convert
		TrueType to Type 1. This is typically only desirable for Level 1 PostScript where no TrueType handling is present.
	*/
	ASBool TTasT42;			

	/** If true, print pages as an image. */
	ASBool	printAsImage;					

	/** If true, do not down load Far East fonts to printer. */
	ASBool	printerHasFarEastFonts;			

	/** Print from lastPage to firstPage. */
	ASBool reverse;				

	/** Indicates odd, even, or all pages to be printed within the range - only meaningful when firstPage and
		lastPage parameters are used. Can be either PDAllPages, PDEvenPagesOnly, or PDOddPagesOnly.*/
	ASInt32				pageSpec;

	/** Transparency level reflects the pull-down on the Advanced print dialog for controlling how much
		rasterization should be performed when flattening transparent objects during printing.
		Obsolete, set to 0 and use rasterData instead.
		-  1 = The entire page will be rasterized. Use this setting for printing or exporting complex pages with many
			transparent objects. Ideal for fast output at low resolution; higher resolution will yield higher quality but
			increase processing time. Size of saved files or print spool files may be large.
		- 2 = Maintains simpler vector objects, but rasterizes more complex areas involving transparency. Ideal for
			artwork with only a few transparent objects. Some printers may yield rough transitions between bordering
			vector and raster objects and make hairlines appear thicker. Appropriate for low-memory systems.
		- 3 = Maintains most objects as vector data, but rasterizes very complex transparent regions. Generally the best
			setting for printing and exporting most pages. With some printers, improves transition issues between
			bordering vector and raster objects.
		- 4 = Maintains most of the page content as vectors, rasterizing only extremely complex areas. Produces
			high quality output that is generally resolution independent. Higher occurrences of transparent regions
			will increase processing time. With some printers improves transition issues between bordering vector
			and raster objects.
		- 5 = The entire page is printed or exported as vector data, to the greatest extent possible. This produces the
			highest quality resolution-independent output. Processing of complex pages may be very time and
			memory intensive.
	*/
	ASInt32		transparencyLevel;	

	/** Represents the name of the destination profile to use when doing host-based color management.
	*/
	char destProfile[256];

	/** */
	AVPageSize pageSize;

	/** if null uses default values */
	AVDocPrintTileData tileData;  

	/** if null uses default values  */
	AVDocPrintRasterizeData rasterData; 

	/** if null uses default values  */
	AVDocPrintOverrideData overrideData;

	/** area of page(s) to print (in pts).  If null print whole page */
	ASFixedRect	*selectRect;


	/** optional content context to use. If null, use current on-screen OC state, but apply print-
		specific AutoState changes from the default occonfig's AS entry 
	*/
	PDOCContext ocContext; 
	/** short note to user that appears in print dialog */
	ASUTF16Val  userNote[kPrintUserNoteLen]; 


	/** Font and resource Policy, such as kAVResPolicySendAtStart. Must be one of AVResourcePolicy.
		@see AVResourcePolicy 
	*/
	AVResourcePolicy resPolicy; 
	/** which printer marks should be emitted, kAVCropMarks, etc. */
	ASUns32 marksFlags;

	/** N-up printing control paremiters; if null uses default values. */
	AVDocPrintNupData nUpData;

	/** When marksFlags is non-zero, use marksStyle to determine which printer mark style to use
	@see marksStyleType (makes kAVEasternStyleMarks obsolete) */	
	ASInt32 marksStyle;
	
	/** did user cancel the print dialog? */
	ASBool printDialogWasCancelled;
	
	/** Parent window of dialog (ignored on the Macintosh platform). May be NULL. */
	AVWindow parentWindow;		

    /** Represents the name of the proofing profile when doing proofing */
    char proofProfile[256];

} AVDocPrintParamsRec;
#endif

typedef struct _t_AVDocPrintSepsParams *AVDocPrintSepsParams;

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	Structure specifying color separation printing parameters 
	used by AVDocPrintSeparations. 
	@see AVDocPrintSeparations 
*/
typedef struct _t_AVDocPrintSepsParams 
{
	/** Size of the data structure. Must be set to sizeof(AVDocPrintSepsParamsRec). */
	ASSize_t size;
	/** The document for which to print color separations.  */
	AVDoc avDoc;
	/** firstPage, lastPage, psLevel, binaryOk and shrinkToFit are used if 
	** emitToPrinter or emitToFile are true.
	*/
	/** First page to be printed. First page number is 0. If -1, all pages are printed */
	ASInt32 firstPage;						
	/** Last page to be printed. this is ignored if the firstPage is -1. */
	ASInt32 lastPage;						

	/** The method creates an output file for each plate for each 
		page, using the fileSysName for the 
		file system, and the filePathName as the root file name 
		for each file. It appends the page number, then the colorant 
		name to each output file name. If filePathName is not a 
		valid pathname to a file Acrobat can open and write to, 
		the method raises an exception using the value returned 
		by ASFileSysOpenFile.
	*/
	ASAtom fileSysName;					
	/** The method creates an output file for each plate for each 
		page, using the fileSysName for the 
		file system, and the filePathName as the root file name 
		for each file. It appends the page number, then the colorant 
		name to each output file name. If filePathName is not a 
		valid pathname to a file Acrobat can open and write to, 
		the method raises an exception using the value returned 
		by ASFileSysOpenFile.
	*/
	ASPathName filePathName;
	
	/* Parameters for emission of PS Level 1 Color Separations and EPS, or vanilla PS.
	** Creates and writes to filePathName (may not be NULL).
	** Does NOT use system printer driver.
	** Only has partial font emitting capabilities on some platforms:
	** 	Mac:	embedded and system Type 1 fonts only; no TrueType or substitution fonts.
	**	Win:	embedded and system Type 1 fonts only; no TrueType or substitution fonts.
	**	UNIX:	all fonts
	*/
	/** Not used. */
	ASEnum8 emitFileOption;					
	
	/* More emit options. */
	/** Additional emit options. Must be one of the Emit Flags. */
	ASUns32	emitFlags;		
	
	/** Not used. */
	AVPageSize pageSize;
	/** NULL uses default values. */
	AVDocPrintRasterizeData rasterData; /* if null uses default values */
	/** NULL uses default values.  */
	AVDocPrintOverrideData overrideData; /* if null uses default values */
	/** The separations specification parameter structure. */
	PDHostSepsSpec sepsSpec;
	/** if true, generate in-RIP seps */
	ASBool inRip;					

}
AVDocPrintSepsParamsRec;

#endif

/************************************************************************************\
|*																					*|
|*									Transition Stuff								*|
|*																					*|
\************************************************************************************/


/**
	Data structure containing callbacks that implement a transition 
	handler. The callbacks implement the transition handler 
	functions. 
	@see AVAppRegisterTransHandler 
*/
typedef struct _t_AVTransHandler *AVTransHandler;

/**
	Platform-dependent data structure for a transition. 

	In general, a transition port specifies a bitmap and a rectangle 
	describing the portion of the bitmap affected by the transition. 
	The transition handler? AVTransHandlerExecuteProc callback 
	must copy all the bits from the source port? bitmap within 
	the source port? rectangle to the area in the destination 
	port's bitmap described by the destination port? rectangle. 
	The source and destination ports?rectangles are guaranteed 
	to be equal in size. 
	@see AVTransHandlerExecuteProc 
*/
typedef struct _t_AVTransitionPort {
	/** */
	void 	 		*reserved;
	/** Reserved. Do not alter. */
	AVRect32		rect32;		
#if WIN_PLATFORM

	/** */
	HDC			hDC;

	/** */
	RECT		rect;
#elif MAC_PLATFORM
	GWorldPtr 	port;
	Rect		rect;
#elif UNIX_PLATFORM
	void*		port;
	void*		rect;
#elif OS2_PLATFORM
	HPS		hPS;
	RECTL		rect;
#endif
} AVTransitionPortRec, *AVTransitionPort;
	


/**
	Callback for AVTransHandler. Gets the transition type serviced 
	by this handler. The handler for a given transition is found 
	by comparing the result of PDTransGetSubtype to the value 
	returned by the registered transition handler's AVTransHandlerGetTypeProc 
	callbacks. 
	@param avth IN/OUT? The transition handler. 
	@return Type of transition handler, which may be one of the types 
	provided in the Acrobat viewer or a new type registered 
	by a plug-in. 
	@see PDTransGetSubtype 

*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVTransHandlerGetTypeProc)(AVTransHandler avth);


/**
	Callback for AVTransHandler. Executes the specified transition. 
	The transition handler is responsible for copying the pixels 
	specified by srcTP to the location specified by dstTP. In 
	the process the handler can create any visual effect it 
	desires, as long as the source pixels are eventually copied 
	over the destination pixels in the end. 

	The handler should do its best to execute the visual effect 
	in the number of seconds specified by duration. 

	The implementation will ensure that the source and destination 
	rectangles are the same size, though their corners may not 
	coincide. 
	@param avth IN/OUT? The transition handler. 
	@param trans IN/OUT? The transition to execute. 
	@param srcTP IN/OUT? Source transition port. 
	@param dstTP IN/OUT? Destination transition port. 
	@param duration IN/OUT? Duration of the transition, in seconds. 
	
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVTransHandlerExecuteProc)(AVTransHandler avth, PDTrans trans,	AVTransitionPort srcTP, AVTransitionPort dstTP, ASFixed duration);


/**
	Callback for AVTransHandler. Retrieves the user-interface 
	name for an existing PDTrans. For example, if the transition 
	type is 'Wipe' and the direction is 180, AVTransHandlerGetUINameProc 
	would return 'Wipe Left,' localized. 

	A transition handler can handle several distinct transitions. 
	For example, the 'Wipe' transition handler can create four 
	distinct effects: wipe left, wipe right, wipe up, and wipe 
	down. 

	The transition setting dialog box creates a separate user-
	interface entry for each distinct transition. It determines 
	both the number and names of the distinct transition types 
	by repeatedly calling each transition handler's AVTransHandlerGetUINameProc 
	callback, starting with an item number of 0 and increasing 
	until the AVTransHandlerGetUINameProc callback returns an 
	empty string. 

	The string returned by AVTransHandlerGetUINameProc should 
	be localized. 

	The AVTransHandlerGetUINameProc is used to enumerate the 
	entire list of supported transition effects that the handler 
	wishes to display in the popup menu (for example, 'Wipe 
	Left' for item == 0, 'Wipe Right' for item == 1, and so 
	on). 
	@param avth IN/OUT? The transition handler. 
	@param trans IN/OUT? The transition whose name is obtained. 
	@param buffer IN/OUT? (Filled by the callback) The user-interface 
	name of the transition. 
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVTransHandlerGetItemUINameProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetUINameProc)(AVTransHandler avth, PDTrans trans, char* buffer, ASInt32 bufLen);


/**
	Callback for AVTransHandler. 

	A transition handler can handle several distinct transitions. 
	For example, the 'Wipe' transition handler can create four 
	distinct effects: wipe left, wipe right, wipe up, and wipe 
	down. 

	The transition setting dialog box should create a separate 
	user interface entry for each distinct transition. It determines 
	both the number and names of the distinct transition types 
	by repeatedly calling each transition handler's AVTransHandlerGetItemUINameProc 
	callback, starting with an item number of 0 and increasing 
	until AVTransHandlerGetItemUINameProc returns an empty string. 

	Thus when the transaction handler is selected from the list, 
	this callback is called. The transition handler should fill 
	in the Type and S fields. AVTransHandlerGetItemUINameProc 
	should fill in any default values. This information is passed 
	into the AVTransHandlerDoPropertiesProc in the form of a 
	PDTrans if that callback exists. 
	@param avth IN/OUT? The transition handler. 
	@param item IN/OUT? The item number. 
	@param buffer IN/OUT? (Filled by the callback) The name of the 
	transition in the user interface. This string should be 
	localized. 
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVTransHandlerGetUINameProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetItemUINameProc)(AVTransHandler avth, ASInt32 item, char* buffer, ASInt32 bufLen);

/**
	Returns the canonical name for the item. Should return ASAtomNull if the item number is invalid.
	This was added in Acrobat 5.0 so that different transitions can be handled in a language
	independent manner. If this call is not implemented, then it is acceptable to generate a 
	synthetic name from the transition type and the item index.
	E.g. The "Glitter" trans handler should return a canonical name like "GlitterDown". 
	If this call is not implemented then the name will be the trans handler type "Glitter" 
	plus a period plus the item number e.g. "Glitter.0". 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVTransHandlerGetItemNameProc)(AVTransHandler avth, ASInt32 item);

/** 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *AVTransHandlerGetNameProc)(AVTransHandler avth, PDTrans trans);


/**
	Callback for AVTransHandler. This method should set default 
	values in the transition dictionary, transDict. 

	As soon as the handler is selected from the list, AVTransHandlerInitTransDictProc 
	is called. This function should fill in the Type and S fields 
	of transDict. AVTransHandlerInitTransDictProc should also 
	fill in any default values. This information is passed to 
	AVTransHandlerDoPropertiesProc in the form of a PDTrans 
	if AVTransHandlerDoPropertiesProc exists. 

	Normally the Type and S fields are filled in when the transition 
	is created via PDTransNewFromCosDoc. The implementation 
	then calls AVTransHandlerInitTransDictProc and AVTransHandlerCompleteTransDictProc 
	immediately on the newly created PDTrans. 
	@param avth The transition handler. 
	@param transDict Transition dictionary to set.
	@see AVTransHandlerCompleteTransDictProc 
	@see PDTransNewFromCosDoc 
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *AVTransHandlerInitTransDictProc)(AVTransHandler avth, CosObj transDict);

/**
	Callback for AVTransHandler. This method is called after 
	the user has selected a distinct transition. The transition 
	handler must fill in any dictionary items necessary to create 
	the effect specified by the uiName passed in. For example, 
	if the 'Wipe' transition handler is passed an uiName of 
	'Wipe Left,' it would set the Dir key in transDict to the 
	value 180. 

	AVTransHandlerCompleteTransDictProc should fill in standard 
	information like direction, dimension, motion, and so forth 
	?information gathered entirely from the UI name. Other 
	specific information should be filled in by AVTransHandlerDoPropertiesProc. 
	
	@param avth IN/OUT? The transition handler. 
	@param name IN/OUT? User interface name of the transition. 
	@param transDict IN/OUT? Transition dictionary to set. 
	@see AVTransHandlerDoPropertiesProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVTransHandlerCompleteTransDictProc)(AVTransHandler avth, const char *name, CosObj transDict);

/**
	Callback for AVTransHandler. This method is called when 
	the user clicks the button in the transition settings dialog. 
	This allows the transition to bring up its own custom dialog 
	allowing the user to further specify the desired transition 
	effect. 

	Once the user selects a transition effect from the popup 
	menu, the viewer immediately creates a transition (using 
	PDTransNewFromCosDoc or PDTransNew) and calls AVTransHandlerInitTransDictProc 
	and AVTransHandlerCompleteTransDictProc. If the handler 
	provides both an AVTransHandlerDoPropertiesProc and AVTransHandlerGetButtonTextProc 
	callback, the dialog box displays a button. When the user 
	clicks on the button, the viewer calls the handler's AVTransHandlerDoPropertiesProc 
	callback. DoProperties is responsible for making any needed 
	alterations to the transition; InitTransDict and CompleteTransDict 
	are not called after DoProperties. 

	After the user clicks OK in the dialog box, trans is filled 
	in using the supplied data. 
	@param avth IN/OUT? The transition handler. 
	@param name IN/OUT? The user interface name for the transition 
	handled by avth. 
	@param trans IN/OUT? The PDTrans to initialize. 
	@see AVTransHandlerCompleteTransDictProc 
	@see AVTransHandlerInitTransDictProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVTransHandlerDoPropertiesProc)(AVTransHandler avth, const char *name, PDTrans trans);

/**
	(Unused) Callback for AVTransHandler. 
	@param avth IN/OUT? The transition handler. 
	@param buffer IN/OUT? (Filled by the callback) The instruction 
	text. 
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVActionGetInstructionsProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetInstructionsProc)(AVTransHandler avth, char *buffer, ASInt32 bufLen);

/**
	Callback for AVTransHandler. Gets a localized string that 
	appears in the button on the transition settings dialog 
	box. If AVTransHandlerGetButtonTextProc is NULL or the string 
	it returns is empty, no button will appear. 
	@param avth IN/OUT? The transition handler. 
	@param buffer IN/OUT? (Filled by the callback) The button text. 
	
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVTransHandlerDoPropertiesProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetButtonTextProc)(AVTransHandler avth, char *buffer, ASInt32 bufLen);

/**
	Callback for AVTransHandler. Gets a localized string that 
	appears above the button on the transition settings dialog 
	box. 
	@param avth IN/OUT? The transition handler. 
	@param buffer IN/OUT? (Filled by the callback) The string text 
	appearing above the button. 
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVTransHandlerGetStringTwoTextProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetStringOneTextProc)(AVTransHandler avth, char *buffer, ASInt32 bufLen);

/**
	Callback for AVTransHandler. Gets a localized string that 
	appears below the button on the transition settings dialog 
	box. 
	@param avth IN/OUT? The transition handler. 
	@param buffer IN/OUT? (Filled by the callback) The string text 
	appearing below the button. 
	@param bufLen IN/OUT? Length of buffer, in bytes. 
	@return The number of characters copied into buffer. 
	@see AVTransHandlerGetStringOneTextProc 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AVTransHandlerGetStringTwoTextProc)(AVTransHandler avth, char *buffer, ASInt32 bufLen);


/**
	Data structure containing callbacks that implement a transition 
	handler. The callbacks implement the transition handler 
	functions. 
	@see AVAppRegisterTransHandler 
*/
typedef struct _t_AVTransHandler
{

	/** Size of the data structure. Must be set to sizeof( AVTransHandlerRec). */
	ASSize_t 							size;

	/** return the transition handler type, e.g. "Wipe" */
	AVTransHandlerGetTypeProc			GetType; 

	/** given this PDTrans, execute this transition */
	AVTransHandlerExecuteProc			Execute; 

	/** given this PDTrans, what's the UIName */
	AVTransHandlerGetUINameProc			GetUIName; 

	/** return the UIName for Nth item, NULL if N is invalid */
	AVTransHandlerGetItemUINameProc		GetItemUIName; 

	/** fill this dict with generic info if you need to */
	AVTransHandlerInitTransDictProc		InitTransDict; 

	/** fill this dict with specific, standard info */
	AVTransHandlerCompleteTransDictProc	CompleteTransDict; 

	/** the proc to bring up the UI window, fill in the PDTrans - for non-standard plug-ins that have additional information*/
	AVTransHandlerDoPropertiesProc		DoProperties; 

	/** message1 - for non-standard plug-ins that have additional information */
	AVTransHandlerGetInstructionsProc	GetInstructions; 

	/** button text - for non-standard plug-ins that have additional information */
	AVTransHandlerGetButtonTextProc 	GetButtonText; 

	/** message2 - for non-standard plug-ins that have additional information */
	AVTransHandlerGetStringOneTextProc 	GetStringOneText; 

	/** message3 - for non-standard plug-ins that have additional information */
	AVTransHandlerGetStringTwoTextProc 	GetStringTwoText;

	/** ACROBAT5: given this PDTrans, what's its LI name. - for non-standard plug-ins that have additional information */
	AVTransHandlerGetNameProc			GetName;	

	/** ACROBAT5: return the LI name for Nth item, ASAtomNull if N is invalid. - for non-standard plug-ins that have additional information */
	AVTransHandlerGetItemNameProc		GetItemName;	

	/** ACROBAT6: is this transition OK to use with the Random transition? - for non-standard plug-ins that have additional information */
	ASBool								discludeFromRandom; 

	/** ACROBAT7: is this transition OK to set through the UI?  Certain effects (like /Fly) don't make sense in the general case */
	ASBool								discludeFromUI;
} AVTransHandlerRec;


/**
	Callback for AVAppEnumTransHandlers. It is called once for 
	each transition handler. 
	@param avth IN/OUT? The transition handler. 
	@param vClientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppEnumTransHandlers. 
	@return true to continue enumeration, false to halt enumeration. 
	
	@see AVAppEnumTransHandlers 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVTransHandlerEnumProc)(AVTransHandler avth, void *vClientData);

/*------------------------------------------------------------------------------
	System Font Stuff (Mac only)
------------------------------------------------------------------------------*/
#if MAC_ENV
/* AVSystemFont flags bitfield values */

/**@name AVSystemFontFlags
	Constants that are used to specify the attributes of an 
	AVSystemFont. 
	@see AVAppEnumSystemFonts 
*/
/*@{*/

/** Type 1 font. */
#define kFontIsType1		0x0001

/** Multiple Master font. */
#define kFontIsMMMaster		0x0002

/** Multiple Master font (completely-specified instance).*/
#define kFontIsMMInstance	0x0004

/** True type font. */
#define kFontIsTrueType		0x0008

/** Character ID Type 0 font. */
#define kFontIsCIDFontType0	0x0010

/**  OCF Type 1 font. */
#define kFontIsOCFType1		0x0020
/*@}*/

/**
	(Macintosh only, present only in version 3.0 or later) System 
	font. 
	@see AVAppEnumSystemFonts 
*/
typedef struct _t_AVSystemFont {

	/** Macintosh FOND id. */
	short fondID;	

	/** Macintosh style value: normal, italic, bold or bold | italic */
	short styleID;	

	/** Must be one of AVSystemFont Flags. 
		@see AVSystemFontFlags 
	*/
	ASUns32 flags;	

	/** PostScript name or TrueType styled name. */
	ASAtom pdfFontName;	
} AVSystemFontRec, *AVSystemFont;

/**
	(Macintosh only). Callback for AVAppEnumSystemFonts. It 
	is called once for each system font. 
	@param systemFont IN/OUT? The AVSystemFont currently being enumerated. 
	
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to AVAppEnumSystemFonts. 
	@return true to continue enumeration, false to halt enumeration. 
	
	@see AVAppEnumSystemFonts 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVSystemFontEnumProc)(AVSystemFont systemFont, void *clientData);
#endif


/**
	Structure used by AVDocDoSaveAsWithParams containing parameters 
	that a client wishing to save a file might want to specify. 
	It is passed in by address to AVDocDoSaveAsWithParams with 
	a size field so that current clients replacing AVDocDoSaveAsWithParams 
	won't break in the future if new open specifications are 
	provided. 
	@see AVDocDoSaveAsWithParams 
*/
typedef struct _t_AVDocSaveParams
{
	/** Size of the data structure. Must be set to sizeof( AVDocSaveParamsRec). */
	ASSize_t 		size;

	/** Use the standard file Save dialog box. */
	ASBool			useSaveDialog;

	/** (New for Acrobat 5.0) Don't use convert from PDF handlers. */
	ASBool			dontAllowConversions; 
}
AVDocSaveParamsRec, *AVDocSaveParams;



/**
	A structure that contains a progress monitor, cancel procedure, 
	and error report procedure. 
	@see AVConversionConvertToPDFWithHandler 
	@see AVConversionConvertFromPDFWithHandler 
	@see AVConversionConvertToPDF 
*/
typedef struct _t_AVStatusMonitorProcs {

	/** The size of this structure. Set to sizeof(AVStatusMonitorProcsRec). */
	ASSize_t			size;


	/** (May be NULL) The progress monitor. In general, clients of this structure test members for NULL. If a member 
		is found, they don't do anything. 
	*/
	ASProgressMonitor	progMon;

	/** The progress monitor client data that was acquired with the progress monitor. */
	void				*progMonClientData;

	/**
	This call has been replaced by ASCancelProc. 

	Callback to check for canceling operations. A CancelProc 
	is typically passed to some method that takes a long time 
	to complete. At frequent intervals, the method calls the 
	CancelProc. If it returns true, then the method cancels 
	its operation; if false, it continues. 
	@param clientData User-supplied data that was passed to 
	the CancelProc.
	@return true if the processing is canceled, false otherwise. 
	@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
	
	@see AVAppGetCancelProc 
	*/
	ASCancelProc		cancelProc;

	/** The cancellation procedure client data that was acquired with the cancellation procedure. */
	void				*cancelProcClientData;

	/** (May be NULL) The report procedure. In general, clients of this structure test members for NULL. 
		If a member is found, they don't do anything.
	*/
	ASReportProc		reportProc;

	/** The report procedure client data that was acquired with the report procedure.*/
	void				*reportProcClientData;
} AVStatusMonitorProcsRec, *AVStatusMonitorProcs;

/*********************************************************
 * AVOpenSaveDialog definitions
 *********************************************************/


/**
	Structure to handle file types and/or extensions in open 
	and save dialogs. 
	@see AVAppChooseFolderDialog 
	@see AVAppOpenDialog 
	@see AVAppSaveDialog 
*/
typedef struct _t_AVFileDescRec{

	/** Up to 32-character string for file extension. Use \ 0 on Windows for don't care (ignored on Windows only 
		if \ 0 is used). 
	*/
	char	extension[32];

	/** File type; used on Macintosh only. Use 0 for don't care. */
	ASUns32	macFileType;

	/** File creator; used on Macintosh only. Use 0 for don't care. */
	ASUns32	macFileCreator;
} AVFileDescRec;

/*
Examples of AVFileDescRec's:

HTMLFileDesc1:
"html"		- extension
'TEXT'	- type
0		- creator

HTMLFileDesc2:
"htm"	- extension
'TEXT'	- type
0		- creator

TextFileDesc1:
""		- extension
'TEXT'	- type
0		- creator

TextFileDesc2:
"txt"	- extension
'TEXT'	- type
0		- creator
*/


/**
	Structure to hold a series of file type descriptors that 
	form a file filter for an open or save dialog. 
	@see Various 
	@see AVAppChooseFolderDialog 
	@see AVAppOpenDialog 
	@see AVAppSaveDialog 
*/
typedef struct _t_AVFileFilterRec {

	/** Localized string describing this filter. It is the name that appears in the open or save dialog. */
	ASText			filterDescription;

	/** An array of AVFileDescRec. A single AVFileFilterRec can have as many AVFileDescRecs as needed. On Windows, the 
	filename is concatenated with the extension string of the relevant AVFileDescRec in the Open and Save dialogs. On 
	the Macintosh, the fileDescription string is used in the File Open and Save dialogs, and the AVFileDescRecs are used 
	to filter which files are displayed when that AVFileFilterRec is selected. */
	AVFileDescRec	*fileDescs;

	/** The number of AVFileDescRecs in fileDescs. */
	AVArraySize		numFileDescs;
} AVFileFilterRec;

/**
	An enumerated list of open and save dialog flags. 
*/
enum {

	/** Use "All Files (*.*)" file filter for dialog. Meaningful only for an open dialog.*/
	kAVOpenSaveAllowAllFlag					= 1 << 0,	

	/** Allow multiple files to be opened through this dialog. Meaningful only for an open dialog. */
	kAVOpenSaveAllowMultiple				= 1 << 1,	

	/** Allow file systems other than the default to be used to open the file( s). */
	kAVOpenSaveAllowForeignFileSystems		= 1 << 2,

	/** Adds a Settings button to the dialog. Meaningful for open and save dialogs. */
	kAVOpenSaveAllowSettingsButton			= 1 << 3,	

	/** Meaningful only for Open dialogs with more than one passed filter  */
	kAVOpenSaveMergeTogetherPassedFilters	= 1 << 4	
};
typedef ASUns32 AVOpenSaveDialogFlags;


/**
	A client can provides this optional callback if it wishes 
	to control whether the settings button in the open or save 
	dialog is enabled or disabled. If a user does not provide 
	this callback function, then the state of the settings button, 
	enabled or disabled, will be determined by whether the conversion 
	handler has a settings proc or not. See below for an example 
	of a ConvertToPDFComputeEnabledProc: 
	@param currentFilter IN/OUT? The currently selected filter in the 
	dialog. 
	@param data IN/OUT? void* of clientData, which is the last member 
	of the AVOpenSaveDialogParamsRec structure. 
	@return true if the Settings button should be enabled; false otherwise. 
	
	@see AVOpenSaveDialogSettingsExecuteProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVOpenSaveDialogSettingsComputeEnabledProc)(AVFileFilterRec *currentFilter, void *data);

/**
	A client provides this optional callback to decide what 
	action is taken when the user clicks on the settings button. 
	The function is called back with the currently selected 
	filter. See below for an example of a ConvertToPDFSettingsExecuteProc. 
	
	@param currentFilter IN/OUT? The currently selected filter in the 
	dialog. 
	@param data IN/OUT? void* of clientData, which is the last member 
	of the AVOpenSaveDialogParamsRec structure. 
	@return Boolean 
	@see AVOpenSaveDialogSettingsComputeEnabledProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVOpenSaveDialogSettingsExecuteProc)(AVFileFilterRec *currentFilter, void *data);

#if (ACRO_SDK_LEVEL >= 0x00060000)

/**
	A structure defining the properties and callbacks related 
	to a file open/ save dialog. Used for AVAppOpenDialog, AVAppSaveDialog, and AVAppChooseFolderDialog.
	@see AVAppOpenDialog 
	@see AVAppSaveDialog 
	@see AVAppChooseFolderDialog 
*/
typedef struct {								
	/** The size of this structure. Set to sizeof(AVOpenSaveDialogParamsRec). */
	ASSize_t    								size;   		

	/** A bitwise OR of the AVOpenSaveDialogFlags. */
	AVOpenSaveDialogFlags						flags;

	/** Parent window of dialog (ignored on the Macintosh platform). May be NULL. */
	AVWindow									parentWindow;		

	/** Title of dialog that is used for the prompt. May be NULL for default title. */
	ASText										windowTitle;	

	/** Title of action button (Open, Save, or Choose). May be NULL for default title. */
	ASText										actionButtonTitle;	

	/** Title of cancel button. May be NULL for default title */
	ASText										cancelButtonTitle;	

	/** May be NULL if flags does not contain kAVOpenSaveAllowForeignFileSystems. */
	ASFileSys									initialFileSys;		

	/** Used to specify initial location or selection. May be NULL if default location or selection is acceptable. */
	ASPathName									initialPathName;

	/** Ignored (may be NULL) for Open and ChooseFolder. For Save, filename portion is used for edit 
		field. May be NULL on Windows, but is required on the Mac. 
	*/
	ASText										initialFileName;	

	/** Array of AVFileFilterRecs. Ignored (may be NULL) for ChooseFolder. May be NULL for Open ONLY 
		if kAVOpenSaveAllowAllFlag is set. 
	*/
	AVFileFilterRec								**fileFilters;	

	/** Number of AVFileFilterRecs in fileFilters. */
	AVArraySize									numFileFilters;		

	/** (Optional) Called to determine whether or not the Settings button should be enabled. 
		May be NULL. Ignored if kAVOpenSaveAllowSettingsButton is not set. 
	*/
	AVOpenSaveDialogSettingsComputeEnabledProc	settingsComputeEnabledProc;

	/** Called when the user clicks on the (enabled) Settings button. May be NULL. Ignored if 
		kAVOpenSaveAllowSettingsButton is not set.
	*/
	AVOpenSaveDialogSettingsExecuteProc			settingsExecuteProc;

	/** Data that is passed to the settingsExecuteProc callback. Ignored if kAVOpenSaveAllowSettingsButton is not 
		set. 
	*/
	void										*settingsProcData;
} AVOpenSaveDialogParamsRec, *AVOpenSaveDialogParams;
#endif


/*********************************************************
 * AVAcquireSpecial definitions
 *********************************************************/

/*	AVAcquireSpecialFolderPathName and AVAcquireSpecialFilePathName. Use
	these routines to get a path to special folders that exist on the system.

	Valid combinations of cat/fld for these functions are detailed here:	
						kAVSCUser							kAVSCApp
	kAVSFRoot			User directory (1)					Viewer directory (9)
	kAVSFEBooks			User eBook license files (RMF) (2)	N/A
	kAVSFPreferences	User preferences (3)				N/A
	kAVSFSequences		User defined batch sequences (4)	App batch sequences (10)
	kAVSFDocuments		User documents (5)					N/A
	kAVSFJavaScript		User JavaScripts (6)				App JavaScripts (11)
	kAVSFStamps			User stamps folder (7)				App stamps folder (12)
	kAVSFDictionaries	User installed spelling dicts (8)	App installed dicts (13)
	kAVSFPlugIns		N/A									Plug-ins folder (14)
	kAVSFHelp			N/A									Help folder (15)
	kAVSFTemp			Temporary folder (16)				N/A									
	kAVSFMessages		User Messages folder (17)			Default Messages folder (18)
	kAVSFHelpLocale		Downloaded Reader Help (19)			Help Locale folder (20)

	(1) User directory
		Windows: Documents and Settings/<user>/Application Data/Acrobat
	(2) User eBook license files (RMF)
		License files used by WebBUY to indicate ownership/rights of particular 
		PDF documents.
		Windows: Documents and Settings/<user>/Application Data/Acrobat/EBook
	(3) User preferences
		Should contain files full of preferences (e.g. ini or Mac prefs files).
		Generally we don't want the user mucking with these. If a plug-in wants to 
		provide regular preferences it should use the miIni.c interface which would
		map to pref files in this folder on the Mac and the registry on Windows.
		Windows: Documents and Settings/<user>/Application Data/Acrobat/Preferences
	(4) User batch sequences
		Custom batch scripts that the user has written/defined and saved.
		Windows: Documents and Settings/<user>/Application Data/Acrobat/Sequences
	(5) User documents
		The viewer and all plug-ins should default to opening and saving in this folder.
		Windows: Documents and Settings/<user>/My Documents
	(6) User JavaScripts
		JavaScripts written by the user (.js files) that are loaded at application launch.
		These are editable directly by the user and as such are not usually found in
		the same folder as kAVSFEBooks, kAVSFPreferences, or kAVSFSequences.
		Windows: Documents and Settings/<user>/My Documents/Acrobat/JavaScript
	(7) User stamps
		Custom rubber stamps that the user creates are stored in this folder.
		Windows: Documents and Settings/<user>/My Documents/Acrobat/Stamps
	(8) User dictionaries
		User installed dictionaries for the spell checker.
		Windows: Documents and Settings/<user>/My Documents/Dictionaries
	(9) Viewer root
		Location of the application. Sometimes needed to sniff for DLLs and other files
		that live at this level.
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>
	(10)Application batch sequences
		Batch sequences that are shipped with the product as examples.
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>/Sequences/<locale>
	(11)Application JavaScripts
		JavaScripts that are shipped with the product (AForm.js, AFString<lang>.js, Annots.js)
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/EScript/JavaScripts
	(12)Application stamps
		Stamps that are shipped with the product.
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/Annotations/Stamps
	(13)Application Dictionaries
		Dictionaries that are shipped with the product.
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/Spelling/Dictionaries
	(14)Application plug-ins
		Where plug-ins are stored.
		Windows: Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins
	(15)Application help
		Windows: Program Files/Adobe/Acrobat 5.0/Help
	(16)Temp folder
		Windows: Windows\Temp
	(17)User Messages folder
		Windows: Documents and Settings/<user>/Application Data/Acrobat/Messages
	(18)App Messages folder
	    Windows: Program Files/Adobe/Acrobat 5.0/Messages
	(19)Downloaded Reader Help folder
		Used to store the full reader help file on systems that lock out access to 
		the application help folder.
		Windows: Documents and Settings/<user>/Application Data/Acrobat/Help/<Locale>
	(20)Application help locale
		Windows: Program Files/Adobe/Acrobat 5.0/Help/<Locale>
*/


/**
	Categories of special folders on the system. Used with folder 
	types to locate folders. Note that some combinations of 
	AVSpecialCategory and AVSpecialFolder are not valid. See 
	AVSpecialError for a list of valid combinations. 
	@see AVAcquireSpecialFilePathName 
	@see AVAcquireSpecialFolderPathName 
*/
typedef enum {

	/** User folders */
	kAVSCUser,

	/** Application folders */
	kAVSCApp,

	/** Often used to tag the end of an enumeration with a specific value. */
	kAVSCLast
} AVSpecialCategory;


/**
	Special folder types on the system. Used with folder categories 
	to locate folders. Note that some combination of AVSpecialCategory 
	and AVSpecialFolder are not valid. See AVSpecialError for a list of valid 
	combinations. 
	@see AVAcquireSpecialFilePathName 
	@see AVAcquireSpecialFolderPathName 
*/
typedef enum {

	/** User root, Windows: "Documents and Settings/\<user\>/Application 
		Data/Acrobat". Viewer root. Location of the application. Sometimes 
		needed to sniff for DLLs and other files that live at this 
		level. Windows: "Program Files/Adobe/Acrobat 5.0/\<viewer\>" 
	*/
	kAVSFRoot,

	/** User eBook license files (RMF) License files used by WebBUY to indicate ownership/ rights of particular 
		PDF documents. Windows: "Documents and Settings/<user>/Application Data/Acrobat/EBook"
	*/
	kAVSFEBooks,

	/** User preferences folder. Should contain files full of preferences (e.g., ini or Mac prefs 
		files). Generally you don't want the user touching these. If a plug-in wants to provide regular 
		preferences it should use the miIni.c interface which would map to pref files in this folder on 
		the Mac and the registry on Windows. Windows: "Documents and Settings/<user>/Application Data/Acrobat/Preferences"
	*/
	kAVSFPreferences,

	/** User-defined batch sequences: Custom batch scripts that the 
		user has written/ defined and saved. Windows: 
		"Documents and Settings/<user>/Application Data/Acrobat/Sequences" 
		Application batch sequences: Batch sequences that are shipped with the 
		product as examples. Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/Sequences/<locale>" 
	*/
	kAVSFSequences,

	/** User documents folder The viewer and all plug-ins should default to opening and saving in 
		this folder. Windows: "Documents and Settings/<user>/My Documents/Adobe/Acrobat"
	*/
	kAVSFDocuments,

	/** User JavaScripts folder: JavaScripts written by the user (.js files) that are loaded at 
		application launch. These are editable directly by the user and as such are not usually found in 
		the same folder as kAVSFEBooks, kAVSFPreferences, or kAVSFSequences. 
		Windows: "Documents and Settings/<user>/My Documents/Acrobat/JavaScript"
		
		Application JavaScripts folder: JavaScripts shipped with the product (AForm.js, AFString<lang>.js, 
		Annots.js) Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/EScript/JavaScripts"
	*/
	kAVSFJavaScript,

	/** User stamps folder: Custom rubber stamps that the user creates are stored in this  folder. 
		Windows: "Documents and Settings/<user>/My Documents/Acrobat/Stamps"
		Application stamps folder: Stamps that are shipped with the product. 
		Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/Annotations/Stamps"
	*/
	kAVSFStamps,

	/** User-installed dictionaries: User-installed dictionaries for the spell checker. 
		Windows: "Documents and Settings/<user>/My Documents/Dictionaries"
		Application-installed dictionaries: Dictionaries that are shipped with the product. 
		Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/plug-ins/Spelling/Dictionaries"
	*/
	kAVSFDictionaries,

	/** Application plug-ins folder Where plug-ins are stored. 
		Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/plugins"
	*/
	kAVSFPlugIns,

	/** Suite Pea plug-ins folder. */
	kAVSFSPPlugIns,

	/** Help folder Application help. 
		Windows: "Program Files/Adobe/Acrobat 5.0/Help"
	*/
	kAVSFHelp,

	/** Temporary folder Windows: "Windows\Temp" */
	kAVSFTemp,

	/** User messages folder:
		Windows: "Documents and Settings/<user>/Application Data/Acrobat/Messages"
		Application messages folder:
		Windows: "Program Files/Adobe/Acrobat 5.0/Messages"
	*/
	kAVSFMessages,

	/** Resources folder */
	kAVSFResource,

	/** Update folder */
	kAVSFUpdate,

	/** Downloaded Reader Help folder: 
		Used to store the full Reader help file on systems that lock out access to the application help 
		folder. Windows: "Documents and Settings/<user>/Application Data/Acrobat/Help/<Locale>"
		Application help locale folder 
		Windows: "Program Files/Adobe/Acrobat 5.0/Help/<Locale>"
	*/
	kAVSFHelpLocale,

	/*** New for Acrobat 6.0 ***/
	/** */
	kAVSFAuthoring,

	/** */
	kAVSFSecurity,

	/** */
	kAVSFLocalRoot,

	/** */
	kAVSFLocalCache,

	/** */
	kAVSFTasks,

	/** Linguistics Library (LILO) files shared among all Adobe applications. 
		Windows: "<drive>:\Program Files\Common Files\Adobe\Linguistics\"
		Macintosh: "<boot>:Library:Application Support:Adobe:Linguistics"
	*/
	kAVSFLinguistics,

	/** Used to create new PDF from document template.
		User templates folder: Custom template files that the user creates are stored in this folder. 	
		Windows: "Documents and Settings/<user>/My Documents/Acrobat/DocTemplates"
		Application doc template folder: Doc templates that are shipped with the product. 
		Windows: "Program Files/Adobe/Acrobat 5.0/<viewer>/DocTemplates"
	*/
	kAVSFDocTemplates,

	/** User desktop folder. Windows: "Documents and Settings/<user>/My Desktop/"
	*/
	kAVSFDesktop,

	/** Often used to tag the end of an enumeration with a specific value. */
	kAVSFLast
} AVSpecialFolder;


/**
	Operation status codes for the special folder methods. 
	@see AVAcquireSpecialFilePathName 
	@see AVAcquireSpecialFolderPathName 
*/
typedef enum {

	/** No error. */
	kAVSEOkay,				

	/** Invalid category/ folder combination. */
	kAVSEInvalidCombination,

	/** File or directory doesn't exist. */
	kAVSEDoesntExist,		

	/** File system error: directory couldn't be created. */
	kAVSECouldntCreate,		

	/** Some other generic error. */
	kAVSEError					
} AVSpecialError;

/*********************************************************
 * AVConversionHandler definitions
 *********************************************************/

/* Default UniqueID's for conversion handlers */
#define PDF_FILEFILTERREC_UNIQUEID "com.adobe.acrobat.pdf"
#define FDF_FILEFILTERREC_UNIQUEID "com.adobe.acrobat.fdf"


/**
	The user-defined data that is supplied when a conversion 
	handler is registered with the conversion server. This data 
	is provided to all AVConversionHandler callbacks. 
	@see AVConversionDefaultSettingsProc 
	@see AVConversionParamDescProc 
	@see AVConversionSettingsDialogProc 
	@see AVConversionConvertFromPDFProc 
	@see AVConversionConvertToPDFProc 
*/
typedef struct _t_AVConversionClientData *AVConversionClientData;

/**
	An enumerated list of flags that can be passed to AVConversionConvertTo/ 
	FromPDF to allow non-default behavior.
*/
enum {

	/** No flags. */
	kAVConversionNoFlags			= 0,		

	/** Asynchronous conversion is allowed. */
	kAVConversionAsyncOkay			= 1 << 0,	

	/** Pop the settings dialog, if one is provided for this conversion handler. */
	kAVConversionPopSettingsDialog	= 1 << 1,	

	/** Interactive mode. Indicates converter can pop additional dialogs if necessary. */
	kAVConversionInteractive		= 1 << 2,	

	/** Do not overwrite the existing files EXCEPT for the source file. This flag is only used in batch. */
	kAVConversionDontOverwrite		= 1 << 3
};
typedef ASUns32 AVConversionFlags;


/**
	Enumerated data type used to describe the status of a conversion 
	operation. 
	@see ASFileSysGetItemPropsProc 
	@see ASFileSysFirstFolderItemProc 
	@see ASFileSysNextFolderItemProc 
	@see ASFileSysGetItemProps 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
*/
enum {

	/** The conversion failed. */
	kAVConversionFailed,	

	/** The conversion succeeded. */
	kAVConversionSuccess,	

	/** The conversion will continue asynchronously. */
	kAVConversionSuccessAsync,	

	/** The conversion was cancelled. */
	kAVConversionCancelled		
};
typedef ASEnum16 AVConversionStatus;


/**
	Called to get the default settings for the conversion operation. 	

	It is the caller's responsibility to release the resources 
	associated with the returned ASCab. 
	@param uniqueID IN/OUT? A string that represents the filterDescription 
	parameter of the AVFileFilterRec for the conversion handler. 
	
	@param clientData IN/OUT? The user-defined data that is provided 
	to all AVConversionHandler callbacks. 
	@return An ASCab containing the default settings for the conversion 
	operation, return NULL to indicate none. 
*/
typedef ACCBPROTO1 ASCab (ACCBPROTO2 *AVConversionDefaultSettingsProc)
	(const char *uniqueID, AVConversionClientData clientData);


/**
	Called to obtain conversion parameter information. 
	@param settings IN/OUT? A read-only ASCab containing the requested 
	parameters. 
	@param paramDesc IN/OUT? (Filled by the callback) The parameter 
	descriptions (ASText objects) stored under numeric keys 
	starting with key " 1". For example, key=" 1", value=" Title: 
	API Reference" (ASText object). 
	@param clientData IN/OUT? The user-defined data that is provided 
	to all AVConversion callbacks. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVConversionParamDescProc)
	(const ASCab settings, ASCab paramDesc, AVConversionClientData clientData);

/**
	Called to request the handler to display its settings dialog, 
	if it has one. An ASCab containing conversion settings is 
	passed in to fill in the dialog. 

	The implementation should use these settings. Ensure to 
	use this cabinet of settings rather than defaults since 
	the batch framework may provide different settings. 

	If the user commits changes, the settings should be stored 
	in the ASCab that was provided. 

	For 'ConvertToPDF' handlers, two keys are present in the 
	settings ASCab: 

	ASPathName - Path to input file. 

	ASFileSys - The associated file system. 

	For 'ConvertFromPDF' handlers, three keys are present 
	in the settings ASCab: 

	PDDoc - Input PDDoc. 

	ASPathName - Output path. 

	ASFileSys - The associated file system. 
	@param settings The ASCab used to populate the dialog. 	
	@param clientData The user-defined data that is provided 
	to all AVConversion callbacks.
	@return true to proceed with the conversion, false otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVConversionSettingsDialogProc)
	(ASCab settings, AVConversionClientData clientData);


/**
	Called to convert a non-PDF file to a PDF file. 
	@param inSettings An ASCab containing the settings for the 
	conversion operation. Can be NULL. The implementation should 
	use these settings rather than defaults since the batch 
	framework may have provided custom settings.
	@param flags Indicates any non-default behavior to apply 
	to the conversion. By default, conversions are synchronous, 
	non-interactive, and do not display a settings dialog. The 
	conversion framework will automatically call your settings 
	dialog if kAVConversionSyncPopSettingsDialog is set. Do 
	not pop your settings dialog in your convert proc.
	@param inPath The location of the input file. 
	@param inFileSys The file system from which path was obtained. 	
	@param outPDDoc The output PDDoc. The implementation should 
	not clean up this PDDoc ?Acrobat will do this. 
	@param statusMonitor Contains the progress monitor, cancel 
	proc, and error reporting proc to be used by the converter. 
	Can be NULL. If an error occurs during conversion, the implementation 
	should not raise or throw an error but instead report the 
	error using the reportProc, if it is available. 
	@param clientData The user-defined data that is provided 
	to all AVConversionHandler callbacks. 
	@return One of the AVConversionStatus codes. 
	@see AVConversionConvertFromPDFProc 
	@see AVConversionConvertToPDFWithHandler 
*/
typedef ACCBPROTO1 AVConversionStatus (ACCBPROTO2 *AVConversionConvertToPDFProc)(
	ASCab						inSettings,
	AVConversionFlags			flags,
	ASPathName					inPath,
	ASFileSys					inFileSys,
	PDDoc						*outPDDoc,
	AVStatusMonitorProcs		statusMonitor,
	AVConversionClientData		clientData);

/**
	Called to convert a non-PDF file to a PDF file.
	@param inSettings IN/OUT? An ASCab containing the settings for the conversion
	operation. Can be NULL. The implementation should use these
	settings rather than defaults since the batch framework may
	have provided custom settings.
	@param flags IN/OUT? Bit flags for any non-default behavior to apply to the conversion.
	By default, conversions are synchronous, non-interactive, and
	do not display a settings dialog. The conversion framework calls
	your settings dialog if
	kAVConversionSyncPopSettingsDialog is set; this
	procedure should not open the settings dialog.
	@param stream IN/OUT? The input stream.
	@param metaData IN/OUT? An ASCab containing metadata about the input stream, such
	as, for example, a key url containing the URL of HTML data. Can
	be NULL.
	@param outPDDoc IN/OUT? The output PDDoc. The implementation should not clean up
	this PDDoc-Acrobat will do this.
	@param statusMonitor IN/OUT? Contains the progress monitor, cancel proc, and error reporting
	proc to be used by the converter. Can be NULL. If an error
	occurs during conversion, the implementation should not raise
	or throw an error but instead report the error using the
	reportProc, if it is available.
	@param clientData IN/OUT? The user-defined data that is provided to all
	AVConversionHandler callbacks.
*/
typedef ACCBPROTO1 AVConversionStatus (ACCBPROTO2 *AVConversionConvertStreamToPDFProc)(
	ASCab						inSettings,
	AVConversionFlags			flags,
	ASStm						stream,
	ASCab						metaData,
	PDDoc						*outPDDoc,
	AVStatusMonitorProcs		statusMonitor,
	AVConversionClientData		clientData);

/**
	Called to convert a PDF file to a another file format. 
	@param inSettings IN/OUT? An ASCab containing the settings for the 
	conversion operation. Can be NULL. The implementation should 
	use these settings rather than defaults since the batch 
	framework may have provided custom settings. 
	@param flags IN/OUT? Indicates any non-default behavior to apply 
	to the conversion. By default, conversions are synchronous, 
	non-interactive, and do not display a settings dialog. The 
	conversion framework will automatically call your settings 
	dialog if kAVConversionSyncPopSettingsDialog is set ?do 
	not pop your settings dialog in your convert proc. 
	@param inPDDoc IN/OUT? The document that is to be converted. 
	@param outPath IN/OUT? The desired location for the output file. 
	@param outFileSys IN/OUT? The file system from which path was obtained. 	
	@param statusMonitor IN/OUT? Contains the progress monitor, cancel 
	proc, and error reporting proc to be used by the converter. 
	Can be NULL. If an error occurs during conversion, the implementation 
	should not raise or throw an error but instead report the 
	error using the reportProc, if it is available. The report 
	proc member of the status monitor can be NULL, so developers 
	should check for that condition before calling it. 
	@param clientData IN/OUT? The user-defined data that is provided 
	to all AVConversionHandler callbacks. 
	@return One of the AVConversionStatus codes. 
	@see AVConversionConvertToPDFProc 
	@see AVConversionConvertFromPDFWithHandler 
*/
typedef ACCBPROTO1 AVConversionStatus (ACCBPROTO2 *AVConversionConvertFromPDFProc)(
	ASCab						inSettings,
	AVConversionFlags			flags,
	PDDoc						inPDDoc,
	ASPathName					outPath,
	ASFileSys					outFileSys,
	AVStatusMonitorProcs		statusMonitor,
	AVConversionClientData		clientData);


/**
	Is called to convert a PDF file to a another file format.  Must return an AVConversionStatus
	indicating success, failure, or cancel.
	@param inSettings IN/OUT? an ASCab of settings which can be NULL.  The converter must use these settings
		rather than default ones since the Batch framework may have provided custom settings.
	@param flags IN/OUT? flags - indicates any non-default behavior to apply to the conversion.  By default, conversions
		are synchronous, non-interactive, and do not display a settings dialog.  The conversion framework
		will automatically call your settings dialog if kAVConversionSyncPopSettingsDialog is set - do not
		pop your settings dialog in your convert proc.
	@param inPDDoc IN/OUT? input PDDoc that will be converted
	@param stream IN/OUT? output ASStm
	@param metaData IN/OUT? An empty ASCab in which metadata may be placed. For example, for HTML data, there might
		be a key "url" which contains the URL of the HMTL data.
	@param statusMonitor IN/OUT? contains the progress monitor, cancel proc, and error reporting proc that the
		converter should use.  This can be NULL.  If an error occurs during conversion, the converter
		should not raise or throw an error but instead report the error using the reportProc, if it is
		available.
	@param clientData IN/OUT? provided to all AVConversionHandler callbacks.
	@return One of the AVConversionStatus codes. 
*/
typedef ACCBPROTO1 AVConversionStatus (ACCBPROTO2 *AVConversionConvertStreamFromPDFProc)(
	ASCab						inSettings,
	AVConversionFlags			flags,
	PDDoc						inPDDoc,
	ASStm						stream,
	ASCab						metaData,
	AVStatusMonitorProcs		statusMonitor,
	AVConversionClientData		clientData);

/**
	An opaque object representing a node in a document structure tree.
	@see AVConversionConvertStreamFromStructNodeProc
	@see AVConversionConvertStreamFromStructNodeWithHandler
*/
typedef struct _t_AVStructNode*	AVStructNode;

/**
	Called to convert a structure subtree rooted at a given node to a stream.
	@param inSettings An ASCab containing the settings for the conversion
	operation. Can be NULL. The implementation should use these
	settings rather than defaults since the batch framework may
	have provided custom settings.
	@param flags Bit flags for any non-default behavior to apply to the conversion.
	By default, conversions are synchronous, non-interactive, and
	do not display a settings dialog. The conversion framework calls
	your settings dialog if
	kAVConversionSyncPopSettingsDialog is set; this
	procedure should not open the settings dialog.
	@param inStructNode The structure node to be converted.
	@param stream The output stream.
	@param metaData An ASCab containing metadata about the input stream, such
	as, for example, a key url containing the URL of HTML data. Can
	be NULL.
	@param statusMonitor Contains the progress monitor, cancel proc, and error reporting
	proc to be used by the converter. Can be NULL. If an error
	occurs during conversion, the implementation should not raise
	or throw an error but instead report the error using the
	reportProc, if it is available.
	@param clientData The user-defined data that is provided to all AVConversionHandler callbacks.
	@return AVConversionStatus indicating success, failure, or cancel. 
*/
typedef ACCBPROTO1 AVConversionStatus (ACCBPROTO2 *AVConversionConvertStreamFromStructNodeProc)(
	ASCab						inSettings,
	AVConversionFlags			flags,
	AVStructNode				inStructNode,
	ASStm						stream,
	ASCab						metaData,
	AVStatusMonitorProcs		statusMonitor,
	AVConversionClientData		clientData);

/**
	A Mime-type string for PDF conversion
	@see AVConversionFromPDFHandler
	@see AVConversionToPDFHandler
	@see AVAppRegisterFromPDFHandler
	@see AVAppRegisterToPDFHandler
*/
typedef char	AVConversionMimeTypeString[256];


/**
	Data structure containing callbacks that implement the 'ToPDF' 
	handler's functionality and data that describes the handler's 
	conversion capabilities. 
	@see AVConversionToPDFEnumProc 
	@see AVAppRegisterToPDFHandler 
*/
typedef struct _t_AVConversionToPDFHandler {

	/** An AVFileFilterRec that describes the types of files that this filter can convert. See description 
		of AVFileFilterRec and AVFileDescRec for more details.
		@see AVFileFilterRec
		@see AVFileDescRec
	*/
	AVFileFilterRec					convFilter;

	/** Size of the data structure. Must be set to sizeof( AVConversionFromPDFHandlerRec). */
	ASSize_t						size;

	/** Unique identifier for the conversion handler. Should be of the form com.companyname.productname.type. 
		See PDF_FILEFILTERREC_UNIQUEID and FDF_FILEFILTERREC_UNIQUEID in AVExpT. h. 
	*/
	char							uniqueID[256];

	/** true if the converter can perform synchronous conversion, false if the converter only does asynchronous 
		conversion. This capability is required for the handler to be accessible from the batch framework. 
	*/
	ASBool							canDoSync;

	/** An integer that indicates where converter should be registered in list of "FromPDF" handlers. A high 
		priority number indicates that this converter is checked earlier than other converters. All Acrobat 5.0 converters 
		have a priority of 0. 
	*/
	AVPriority						priority;

	/** AVConversionDefaultSettingsProc that is called when the handler is registered with the conversion 
		server. Can be NULL. 
	*/
	AVConversionDefaultSettingsProc	defaultSettings;

	/** AVConversionParamDescProc that is called when a parameter description of this handler is requested. 
		Can be NULL. 
	*/
	AVConversionParamDescProc		parameterDescription;

	/** AVConversionSettingsDialogProc that is called when the batch framework or the open dialog requests 
		a settings dialog for this handler. Can be NULL. 
	*/
	AVConversionSettingsDialogProc	settingsDialog;

	/** AVConversionConvertToPDFProc that is called to perform the conversion operation. */
	AVConversionConvertToPDFProc	convert;

	/** Provided to all AVConversion callbacks. */
	AVConversionClientData			clientData;

	/** Added with Acrobat 6. A string containing Mime types 
	that can be handled by the convertStream callback. */
	AVConversionMimeTypeString		*streamMimeTypes;

	/** Added with Acrobat 6.The number of Mime types in 
	streamMimeTypes. */
	AVArraySize						numStreamMimeTypes;

	/** Added with Acrobat 6 */
	AVConversionConvertStreamToPDFProc	convertStream;
} AVConversionToPDFHandlerRec, *AVConversionToPDFHandler;

/**
	Data structure containing callbacks that implement the 'FromPDF' 
	handler's functionality and data that describes the handler's 
	conversion capabilities. 
	@see AVConversionFromPDFEnumProc 
	@see AVAppRegisterFromPDFHandler 
*/
typedef struct _t_AVConversionFromPDFHandler {

	/** An AVFileFilterRec that describes the types of files that this filter can convert. See description 
		of AVFileFilterRec and AVFileDescRec for more details. 
		@see AVFileFilterRec
		@see AVFileDescRec
	*/
	AVFileFilterRec					convFilter;

	/** Size of the data structure. Must be set to sizeof( AVConversionFromPDFHandlerRec). */
	ASSize_t						size;

	/** Unique identifier for the conversion handler. Should be of the form com.companyname.productname.type. 
		See PDF_FILEFILTERREC_UNIQUEID and FDF_FILEFILTERREC_UNIQUEID in AVExpT. h. 
	*/
	char							uniqueID[256];

	/** true if the converter can perform synchronous conversion, false if the converter only does asynchronous 
		conversion. This capability is required for the handler to be accessible from the batch framework. 
	*/
	ASBool							canDoSync;

	/** An integer that indicates where converter should be registered in list of FromPDF handlers. A high 
		priority number indicates that this converter is checked earlier than other converters. All Acrobat 5.0 converters 
		have a priority of 0.
	*/
	AVPriority							priority;

	/** AVConversionDefaultSettingsProc that is called when the handler is registered with the conversion 
		server. Can be NULL. 
	*/
	AVConversionDefaultSettingsProc	defaultSettings;

	/** AVConversionParamDescProc that is called when a parameter description of this handler is 
		requested. Can be NULL. 
	*/
	AVConversionParamDescProc		parameterDescription;

	/** AVConversionSettingsDialogProc that is called when the batch framework or the open dialog requests 
		a settings dialog for this handler. Can be NULL.
	*/
	AVConversionSettingsDialogProc	settingsDialog;

	/** AVConversionConvertFromPDFProc that is called to perform the conversion operation. */
	AVConversionConvertFromPDFProc	convert;

	/** Provided to all AVConversion callbacks. */
	AVConversionClientData			clientData;

	/** Added with Acrobat 6. A string containing Mime types 
	that can be handled by the convertStream callback.  */
	AVConversionMimeTypeString		*streamMimeTypes;

	/** Added with Acrobat 6. The number of Mime types in 
	streamMimeTypes.  */
	AVArraySize						numStreamMimeTypes;

	/** Added with Acrobat 6. The stream-conversion handler procedure.  */
	AVConversionConvertStreamFromPDFProc		convertStream;

	/** Added with Acrobat 6. The structure-node-to-stream 
	conversion handler procedure. */
	AVConversionConvertStreamFromStructNodeProc	convertStructNode;
} AVConversionFromPDFHandlerRec, *AVConversionFromPDFHandler;


/**
	The user-defined data that is supplied to either of the 
	conversion handler enumeration routines. 
	@see AVConversionFromPDFEnumProc 
	@see AVConversionToPDFEnumProc 
	@see AVConversionEnumFromPDFConverters 
	@see AVConversionEnumToPDFConverters 
*/
typedef struct _t_AVConversionEnumProcData *AVConversionEnumProcData;

/**
	Called once for each AVConversionToPDFHandler registered 
	with Acrobat, or until the callback returns false to halt 
	the enumeration. 
	@param handler IN/OUT? The AVConversionToPDFHandler. 
	@param data IN/OUT? User-defined data passed to the call to AVConversionEnumToPDFConverters. 
	
	@return true to continue the enumeration, false otherwise. 
	@see AVConversionFromPDFEnumProc 
	@see AVConversionEnumToPDFConverters 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVConversionToPDFEnumProc)(AVConversionToPDFHandler handler, AVConversionEnumProcData data);

/**
	Called once for each AVConversionFromPDFHandler registered 
	with Acrobat, or until the callback returns false to halt 
	the enumeration. 
	@param handler IN/OUT? The AVConversionFromPDFHandler. 
	@param data IN/OUT? User-defined data passed to the call to AVConversionEnumFromPDFConverters. 
	
	@return true to continue the enumeration, false otherwise. 
	@see AVConversionToPDFEnumProc 
	@see AVConversionEnumFromPDFConverters 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVConversionFromPDFEnumProc)(AVConversionFromPDFHandler handler, AVConversionEnumProcData data);

/*********************************************************
 * AVIdentity definitions
 *********************************************************/

/**
	Enumerated data type used to identify the properties of 
	a user's identity. 
	@see AVIdentityGetText 
	@see AVIdentitySetText 
*/
typedef enum { 

	/** */
	kAVILoginName, 

	/** */
	kAVIName, 

	/** */
	kAVICorporation, 

	/** */
	kAVIEMail, 

	/** */
	kAVIDepartment,
	
	/** */
	kAVITitle,

	/** */
	kAVIFirstName,

	/** */
	kAVILastName,

	/** */
	kAVICorporationAbbr,

	/** */
	kAVILast
} AVIdentity;

/*********************************************************
 * AVCommand definitions
 *********************************************************/


/**@name AVCommandStringConstants
	Use these strings as keys when accessing AVCommand ASCabs.
*/
/*@{*/

/** */
#define kAVCommandKeyPDDoc			"PDDoc"

/** */
#define kAVCommandKeyAVDoc			"AVDoc"

/** if possible, use kAVCommandKeyBaseFileNameASText instead */
#define kAVCommandKeyBaseFileName	"BaseFileName" 

/** */
#define kAVCommandKeyBaseFileNameASText	"BaseFileNameASText"

/** */
#define kAVCommandKeyOrigExtension	"OrigExtension"

/** */
#define kAVCommandKeyGenericTitle	"GenericTitle"

/** */
#define kAVCommandKeyTitle			"Title"

/** */
#define kAVCommandKeyParams			"Params"

/** */
#define kAVCommandKeyParamsDesc		"ParamsDesc"

/** */
#define kAVCommandKeyCanDescribeParams	"CanDescribeParams"

/** */
#define kAVCommandKeyCanBatch		"CanBatch"

/** */
#define kAVCommandKeyCanShowDialog	"CanShowDialog"

/** */
#define kAVCommandKeyGroupTitle		"GroupTitle"

/** */
#define kAVCommandKeyIsStatus       "IsStatus"
/*@}*/

/**
	An enumerated list of status codes that can be returned 
	by various AVCommand methods. 
	@see AVCommandGetStatus 
*/
enum {

	/** Not working, but ready to. */
	kAVCommandReady,	

	/** Still working. */
	kAVCommandWorking,	

	/** Done working. */
	kAVCommandDone,		

	/** Canceled. */
	kAVCommandCanceled,	

	/** In error. */
	kAVCommandInError	
};
typedef ASEnum16 AVCommandStatus;


/**
	An enumeration detailing how the command is expected to 
	interact with the user. 
	@see AVCommandGetUIPolicy 
*/
enum {
	/** Fully interactive. Gather parameters based on currently active document at the time 
		the dialog goes up. Display errors and warnings. 
	*/
	kAVCommandUIInteractive = 0,	

	/** Interactive but under the control of the sequencing user interface. When showing 
		a dialog, use the parameters passed in rather than the parameters gathered from the document. 
	*/
	kAVCommandUISemiInteractive = 1,

	/** Display errors but no other	dialogs. */
	kAVCommandUIErrorsOnly = 2,	

	/** Never put up a dialog. */
	kAVCommandUISilent = 3		
};
typedef ASEnum16 AVCommandUIPolicy;


/** An AVCommand represents an action that the user can perform on the
	current document or the current selection in the current document.
	Specifically, an AVCommand represents a command which can be added
	to a command sequence and executed either interactively or via
	batch processing.
*/
typedef struct _t_AVCommandRec *AVCommand;


/**
	Initialize the command handler. Called once for each command 
	handler registered. 
	@param handlerName IN/OUT? The name of the command handler. 
	@return true if initialization succeeds, false otherwise. 
	@see AVCmdHandlerTermProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVCmdHandlerInitProc)
	(ASAtom handlerName);


/**
	Terminate the handler. Called once for each handler registered 
	when Acrobat shuts down. Called before clients are unloaded. 
	
	@param handlerName The name of the handler being terminated.
	@see AVCmdHandlerInitProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVCmdHandlerTermProc)
	(ASAtom handlerName);

/**
	Callback for AVCommandHandlerRec. The application maintains 
	a global list of commands that the user can choose from 
	when building a batch sequence. During the initialization 
	sequence, all registered command handlers are asked to build 
	and register all the commands that the handler wants included 
	in the global command list. This is done by calling the 
	command handler's RegisterCommands callback, if it's not 
	NULL. 
	@param handlerName IN/OUT? The name of the command handler. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVRegisterCommandsProc)
	(ASAtom handlerName);

/**
	Called after a command is created. The command handler can 
	establish default parameters, and so forth, for the newly 
	created command. 
	@param cmd IN/OUT? The command that was created. 
	@see AVCommandDestroyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVCommandCreatedProc)
	(AVCommand cmd);

/**
	Called before a command is destroyed. The command handler 
	should free any memory allocated by the command. 
	@param cmd IN/OUT? The command being destroyed. 
	@see AVCommandCreatedProc 
	@see AVCommandDestroy 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVCommandDestroyProc)
	(AVCommand cmd);

/**
	Called to set a cabinet within a command. Used in the SetParams 
	member of the AVCommandHandlerRec structure. The command 
	handler should copy any information from the cabinet into 
	the command. It must not destroy or modify the cabinet. 
	
	@param cmd IN/OUT? The command. 
	@param cab IN/OUT? The cabinet to store. 
	@return One of the AVCommandStatus codes. 
	@see AVCommandResetProc 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandSetProc)
	(AVCommand cmd, ASCab cab);

/**
	Called to retrieve a cabinet from a command. Used in the 
	GetParams and GetProps members of the AVCommandHandlerRec 
	structure. When retrieving command parameters, the handler 
	should first remove any existing items from theCab using 
	ASCabMakeEmpty and then copy all parameter values from the 
	command into theCab. When retrieving properties, the command 
	handler should replace any entries in theCab with key names 
	it recognizes with copies of the command-specific properties. 
	@param cmd IN/OUT? The command whose procedure is being retrieved. 
	@param theCab IN/OUT? (Filled by the callback) The appropriate 
	command cabinet. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVCommandGetProc)
	(AVCommand cmd, ASCab theCab);

/**
	Display this command's parameter setting dialog and allow 
	the user to alter the parameters. 
	@param cmd The command whose parameter setting dialog 
	is displayed.
	@return One of the AVCommandStatus codes. 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandShowDialogProc)
	(AVCommand cmd);


/**
	Do some work. If you don't finish your work, return kAVCommandWorking. 
	If you do finish your work, return kAVCommandDone. If the 
	user cancels the operation, return kAVCommandCanceled. If 
	an error occurs, return kAVCommandInError. 

	In most cases this method performs its work until it returns 
	kAVCommandDone, but in some cases it may be called on to 
	cancel or reset before its work is done. 
	@param cmd IN/OUT? The command doing some work. 
	@return One of the AVCommandStatus codes. 
	@see AVCommandWork 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandWorkProc)
	(AVCommand cmd);

/**
	Stop working and clean up as though the command executed 
	to completion. 
	@param cmd IN/OUT? The command being canceled. 
	@return One of the AVCommandStatus codes. 
	@see AVCommandGetCancelProc 
	@see AVCommandCancel 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandCancelProc)
	(AVCommand cmd);

/**
	Stop working, clear any errors, and try to get back into 
	a Ready state. For many commands this is equivalent to canceling. 
	
	@param cmd IN/OUT? The command being reset. 
	@return One of the AVCommandStatus codes. 
	@see AVCommandSetProc 
	@see AVCommandReset 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandResetProc)
	(AVCommand cmd);

/**
	Every command in a sequence gets its Preflight callback 
	called before the sequence is executed. If any of the Preflight 
	callbacks returns an error, the sequence is aborted. 

	Preflights and Postflights are good for getting user data 
	or preparing the command at the beginning of the sequence. 
	For example, you could use the pre-flight to ask what password 
	the 'Add Security' command should use. This is important 
	since you only want to ask once (not for every file), and 
	you don't want to store the password in the sequence file 
	(or the command's persistent parameters). 
	- Sequence Begins 
	- AVCommandPreflightSequenceProc for all commands are called 
	- Open File # 1 
	- AVCommandPreflightFileProcs for all commands are called 
	- Execute all commands on given file 
	- AVCommandPostflightFileProcs for all commands are called 
	- Close File # 1 
	- Open File # 2 
	- ... (repeat pre/ post file procs) 
	- Close last file 
	- AVCommandPostflightSequenceProcs for all commands are called. 
	- Sequence ends
	@param cmd IN/OUT? The command. 
	@return One of the AVCommandStatus codes. 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandPreflightSequenceProc)
	(AVCommand cmd);


/**
	Every command in a sequence gets its 'Preflight' callback 
	called after each file has been opened to be processed but 
	before any commands have been executed on that file. If 
	any of the Preflight callbacks returns an error, the sequence 
	is aborted. 

	Preflights and Postflights are good for getting user data 
	or preparing the command at the beginning of the sequence. 
	For example, you could use the pre-flight to ask what password 
	the 'Add Security' command should use. This is important 
	since you only want to ask once (not for every file), and 
	you do not want to store the password in the sequence file 
	(or the command's persistent parameters). 

	See AVCommandPreflightSequenceProc for the order in which 
	the AVCommand Pre/Postflight callbacks are called. 
	@param cmd The command. 
	@param doc The PDDoc.
	@return One of the AVCommandStatus codes. 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandPreflightFileProc)
	(AVCommand cmd, PDDoc doc);


/**
	Every command in a sequence gets its 'Postflight' command 
	callback called after all commands in a given sequence have 
	been executed but before the file is closed. 

	Preflights and Postflights are good for getting user data 
	or preparing the command at the beginning of the sequence. 
	For example, you could use the pre-flight to ask what password 
	the 'Add Security' command should use. This is important 
	since you only want to ask once (not for every file), and 
	you do not want to store the password in the sequence file 
	(or the command's persistent parameters). 

	See AVCommandPreflightSequenceProc for the order in which 
	the AVCommand Pre/Postflight callbacks are called. 
	@param cmd The command. 
	@param doc The PDDoc.
	@return One of the AVCommandStatus codes. 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandPostflightFileProc)
	(AVCommand cmd, PDDoc doc);


/**
	Every command in a sequence gets its Postflight command 
	callback called after the sequence is executed. 

	Preflights and Postflights are good for getting user data 
	or preparing the command at the beginning of the sequence. 
	For example, you could use the pre-flight to ask what password 
	the 'Add Security' command should use. This is important 
	since you only want to ask once (not for every file), and 
	you do not want to store the password in the sequence file 
	(or the command's persistent parameters). 

	See AVCommandPreflightSequenceProc for the order in which 
	the AVCommand Pre/Postflight callbacks are called. 
	@param cmd The command.
	@return One of the AVCommandStatus codes. 
*/
typedef ACCBPROTO1 AVCommandStatus (ACCBPROTO2 *AVCommandPostflightSequenceProc)
	(AVCommand cmd);

/**
	A set of callbacks that perform the actions required of 
	a command. 
*/
typedef struct _t_AVCommandHandler
{

	/** Size of structure. Set to sizeof(AVCommandHandlerRec). */
	ASSize_t					size;

	/** Called once for each handler registered. */
	AVCmdHandlerInitProc		Initialize;

	/** Called once for each handler registered when Acrobat shuts down. Called before plug-ins are unloaded */
	AVCmdHandlerTermProc		Terminate;

	/** The application maintains a global list of commands that you can choose from when building 
		your own command. During the initialization sequence, all registered command handlers are asked 
		to build and register all the commands that the handler wants included in the global command list. 
	*/
	AVRegisterCommandsProc		RegisterCommands;

	/** Called after a command is created. The command handler can establish default parameters, and so forth, 
		for the newly created command. 
	*/
	AVCommandCreatedProc		Created;

	/** Called before a command is destroyed. The command handler should free any memory allocated by the 
		command, and so forth. 
	*/
	AVCommandDestroyProc		Destroy;

	/** Called to set the command's parameters. */
	AVCommandSetProc			SetParams;

	/** Called to retrieve the command's parameters. */
	AVCommandGetProc			GetParams;

	/** Called to retrieve command properties. */
	AVCommandGetProc			GetProps;

	/** Displays the command's parameter setting dialog and allows the user to alter the parameters. */
	AVCommandShowDialogProc		ShowDialog;

	/** Do some work. If the command doesn't finish the work, return kAVCommandWorking. If the command 
		finishes the work, return kAVCommandDone. If the user cancels the operation, return 
		kAVCommandCanceled. If an error occurs, return kAVCommandInError. In most cases the work procedure 
		will be called until you return kAVCommandDone, but in some cases, you may be called on to Cancel or 
		Reset before the work is done. 
	*/
	AVCommandWorkProc			Work;

	/** Stop working and pretend you reached a point where all work was done. */
	AVCommandCancelProc		  	Cancel;

	/** Stop working, clear any errors, and try to get yourself back into a Ready state. For many commands 
		this is equivalent to canceling.
	*/
	AVCommandResetProc			Reset;


	/** See sequence description in callback reference. */
	AVCommandPreflightSequenceProc	PreflightSequence;


	/** See sequence description in callback reference. */
	AVCommandPreflightFileProc		PreflightFile;


	/** See sequence description in callback reference. */
	AVCommandPostflightFileProc		PostflightFile;


	/** See sequence description in callback reference. */
	AVCommandPostflightSequenceProc	PostflightSequence;

	/** Reserved for internal use. Must be NULL. */
	void *reserved;
} AVCommandHandlerRec;
typedef AVCommandHandlerRec *AVCommandHandler;


/**
	Enumerates the commands for moving and changing the size 
	of a rectangle. 
	@see AVRectHandleHitTest 
*/
enum {

	/** Move the whole rectangle. */
	kAVDragRect,				

	/** Top left corner. */
	kAVDragTopLeft,				

	/** Top right corner. */
	kAVDragTopRight,			

	/** Bottom right corner. */
	kAVDragBottomRight,			

	/** Bottom left corner. */
	kAVDragBottomLeft,			

	/** Top middle. */
	kAVDragTopMiddle,			

	/** Right middle. */
	kAVDragRightMiddle,			

	/** Bottom middle. */
	kAVDragBottomMiddle,		

	/** Left middle. */
	kAVDragLeftMiddle,			

	/** Snap to top left. */
	kAVDragSnapToTopLeft,		

	/** Snap to top. */
	kAVDragSnapToTop,			

	/** Snap to top right. */
	kAVDragSnapToTopRight,		

	/** Snap to right. */
	kAVDragSnapToRight,			

	/** Snap to bottom right. */
	kAVDragSnapToBottomRight,	

	/** Snap to bottom. */
	kAVDragSnapToBottom,		

	/** Snap to bottom left. */
	kAVDragSnapToBottomLeft,	

	/** Snap to left. */
	kAVDragSnapToLeft			
};
typedef ASEnum8 AVDragType;


/**
	An enumerated list of the types of AVRect handles. 
*/
enum {

	/** no handle */
	kAVRectHandleNone,			

	/** top left */
	kAVRectHandleTopLeft,		

	/** top right */
	kAVRectHandleTopRight,		

	/** bottom right */
	kAVRectHandleBottomRight,	

	/** bottom left */
	kAVRectHandleBottomLeft,	

	/** top middle */
	kAVRectHandleTopMiddle,		

	/** right middle */
	kAVRectHandleRightMiddle,	

	/** bottom middle */
	kAVRectHandleBottomMiddle,	

	/** left middle */
	kAVRectHandleLeftMiddle		
};
typedef ASEnum8 AVRectHandleType;


/**
	The parameters for AVPageViewDragRectSnappedEx, which supercedes 
	AVPageViewDragRectSnapped in Acrobat 6.0. In addition to 
	the parameters allowed by the earlier method, the new version 
	allows you to specify your own drawing procedure. 
*/
typedef struct _t_AVDragRectParams {

	/** set by caller to sizeof(AVDragRectParamsRec) */
	ASSize_t size;					

	/** The page view where the drag occurs */
	AVPageView pageView;			

	/** Starting x-coordinate in device space. */
	ASInt32 xStart;					

	/** Starting y-coordinate in device space. */
	ASInt32 yStart;					

	/** Initial rect in page space  */
	ASFixedRect *startRect;			

	/** Resulting rect in page space */
	ASFixedRect *resultRect;		

	/** Desired drag type, typically the result of AVRectHandleHitTest */
	AVDragType dragType;		

	/** The device-space drag bounds */
	AVDevRect *extrema;			

	/** Pass NULL for default cursors */
	AVCursor *cursorArray;		

	/** Number of cursors in cursorArray */
	ASInt32 nCursors;				

	/** Pass NULL for default Acrobat proc */
	AVPageViewDrawProc drawProc;
} AVDragRectParamsRec, *AVDragRectParams;


/* Definitions for Accessibility */

/**
	Definitions specifying the manner in which the background 
	and text colors are chosen when viewing a document. 
	@see AVAppSetPreference 
*/
enum {

	/** Use the colors specified within the document. */
	kAVAccessUseDocumentColors,

	/** Use the colors specified by the OS preferences. */
	kAVAccessUseSystemColors,

	/** Use the colors specified by the Acrobat preferences. */
	kAVAccessUsePreferenceColors,

	/** */
	kAVAccessForceSystemColors,
	
	/** Use accessible high-contrast colors **/
	kAVAccessUseHiContGreenBlack,
	kAVAccessUseHiContYellowBlack,
	kAVAccessUseHiContWhiteBlack,
	kAVAccessUseHiContBlackWhite

};
typedef ASEnum8 AVAccessColorPolicy;


/** */
enum {

	/** */
	kAVAccessInferredReadOrder,

	/** */
	kAVAccessTBLRReadOrder,

	/** */
	kAVAccessWordyReadOrder,

	/** */
	kAVAccessUndefinedReadOrder
};
typedef ASEnum8 AVAccessReadOrder;

/** */
/**
	An enumerated list of toolbar positions for registering 
	the preferred position of a toolbar. 
	@see AVAppRegisterToolBarPosition 
*/
enum {

	/** */
	kAVToolBarDockTop,

	/** */
	kAVToolBarDockBottom,

	/** */
	kAVToolBarDockLeft,

	/** */
	kAVToolBarDockRight,

	/** */
	kAVToolBarFloating
};
typedef ASEnum8 AVToolBarDockPosition;


/**
	An enumerated list of toolbar layouts. 
	@see AVAppRegisterToolBarPosition 
*/
enum {

	/** */
	kAVToolBarHorizontal,

	/** */
	kAVToolBarVertical,

	/** */
	kAVToolBarTwoColumn
};
typedef ASEnum8 AVToolBarLayout;

#if (ACRO_SDK_LEVEL >= 0x00060000)
/**
	A structure that describes the position of a toolbar. 
	@see AVAppRegisterToolBarPosition 

*/
typedef struct _t_AVToolBarPosition {

	/** The size of this structure. Set to sizeof (AVToolBarPositionRec). */
	ASSize_t	size;

	/** Specifies that the toolbar is to be in-doc (not shared). If inDoc is true, dockPosition cannot be floating. */
	ASBool	inDoc;

	/** The edge of the document window or monitor to attach this toolbar to. */
	AVToolBarDockPosition	dockPosition;

	/** If the toolbar is to be floating you can group it with another toolbar by specifying a name 
		for the floating window. You can set this to a constant string. 
	*/
	const char *floatingWindowName;

	/** The stack to insert the toolbar on. Make this -1 to open a new stack on the left or top, or ASMAXInt32 
		to open a new stack on the right or bottom. 
	*/
	ASInt32 stackNum;

	/** The number of pixels from the top or left edge of the stack from which to position the toolbar. If 
		ASMAXInt32, the toolbar will be positioned snugly behind other toolbars on the stack. If -1, it will 
		be positioned at the front. 
	*/
	ASInt32 offset;

	/** If multiple positions specify an offset of -1 or ASMAXInt32, this field is used to further order them. 
		It controls the order in which the bars will be placed, not the visual order on-screen. If, for example, 
		two bars have an offset of -1, the one associated with the value in the lower order field will be 
		positioned first at the front of the bar. Then the one associated by the value in the higher order field 
		also will be positioned at the front of the bar, but pushing the first one to the right. 
	*/
	ASInt32 order;

	/** If the toolbar isn't inDoc and dockPosition is floating, you may end up creating a new window. Here's its frame. */
	AVScreenRect windowFrame;

	/** If a new window is called for, here's its layout. */
	AVToolBarLayout layout;

	/** Set this to true if the toolbar should be hidden by default. */
	ASBool hidden;

	/** Set this to true if the floating window in which the toolbar is located should be hidden by default. */
	ASBool windowHidden;

} AVToolBarPositionRec, *AVToolBarPosition;
#endif


/**
	Describes the preferred monitor to use when going full-screen 
	on a multi-monitor system. 
*/
enum {

	/** Use the monitor with the largest intersection. */
	kAVFullScreenLargestIntersection,

	/** Use the monitor with the most colors. */
	kAVFullScreenMostColors,

	/** Use the monitor with the widest screen. */
	kAVFullScreenWidest,

	/** Use the monitor with the tallest screen. */
	kAVFullScreenTallest,

	/** Use the monitor with the largest screen. */
	kAVFullScreenLargest,

	/** Use the monitor with the main screen. */
	kAVFullScreenMain,

	/** Enum terminator.*/
	kAVFullScreen_END_ENUM
};
typedef ASEnum8 AVFullScreenMonitor;


/**
	Data structure containing information about a client loaded 
	by the viewer. 

	NOTE: For third-party (non-Adobe) clients, only asaName, 
	bLoaded, and bCertified are valid. 
	@see AVExtensionAcquireInfo 
	@see AVExtensionReleaseInfo 
*/
typedef struct _AVExtensionInfoRec {

	/** The registered name of the plug-in. */
	ASAtom asaName;					

	/** Always true indicating that the plug-in was loaded. */
	ASBool bLoaded;					

	/** true if the plug-in is certified, false otherwise. */
	ASBool bCertified;				

	/** The major and minor versions of the plug-in. */
	AVVersionNumPart nMajorVersion, nMinorVersion;

	/** The creation timestamp on the plug-in. */
	char *cDate;						

	/** The path to the plug-in. */
	ASPathName aspFile;					

	/** A description of the plug-in. May be NULL. */
	char *cDescription;					

	/** Legal text associated with the plug-in. May be NULL. */
	char *cLegal;						

	/** The dependencies of the plug-in. */
	char *cDependencies;				
} AVExtensionInfoRec, *AVExtensionInfo;


/**
	Constants for use with the AVPageViewUpdateInfoPanel. 
	@see AVPageViewUpdateInfoPanel 
*/
enum {

	/** Plug-in wishes to assume control over the output of the info panel. */
	kAVInfoPanelLock,

	/** Plug-in is transferring control back to Acrobat to update the info panel. */
	kAVInfoPanelUnlock,

	/** Plug-in is passing the values to Acrobat that should be displayed in the info panel. */
	kAVInfoPanelRect
};
typedef ASEnum8 AVInfoPanelUpdateType;

/*********************************************************
 * AVBatch definitions
 *********************************************************/

#ifndef _T_AVBATCHCONTEXT
#define _T_AVBATCHCONTEXT

/**
	Placeholder only. Not currently implemented. 
*/
typedef struct _t_AVBatchContext *AVBatchContext;

#endif /* _T_AVBATCHCONTEXT */

/** for use with AVAppGetUUID */
enum
{

	/** the UUID for this user FOR THIS INSTALL */
	kAVAppUserUUID,

	/** the UUID for the currently executing session  */
	kAVAppSessionUUID 
};
typedef ASEnum8 AVAppUUIDType;


/** icon types for AVSysGetIconFromFilename, etc. */
enum
{

	/** */
	kAVSysSmallIcon,

	/** */
	kAVSysLargeIcon,
	
	/** */
	kAVSysHugeIcon
};
typedef ASEnum8 AVSysIconType;

#include "AVExpTObsolete2.h"  //types for old versions of Acrobat

/*******************************************************************************
 * AVSimpleSelProcs definitions - Simple Selection Server for PDFindSupport API
 *******************************************************************************/

#define AVSIMPLESEL_TYPE	"SimpleSelectHub"
/* AVSIMPLESEL_TYPE is the main selection type of the Simple Selection Servers that
   are registered by AVRegisterSimpleHiliteProc(). This selection type should be
   used in AVDocSetSelection() to identify the selection should be handled by the
   Simple Selection Server. The actual (selection) object type is deteremined by
   the client selection type stored in the AVSimpleSelObj and a corresponding hilite
   function is called by the Simple Selection Server.
   Example:
			void HiliteMyObject(AVDoc doc, MyObject data)
			{
				AVSimpleSelObj	selObject;
				selObject = AVSimpleSelObjCreate(ASAtomFromString("ABCD_MySelType"), &data, sizeof(data));
				AVDocSetSelection(doc, ASAtomFromString(AVSIMPLESEL_TYPE), selObject, true);
				AVDocShowSelection(doc);
			}
*/
/** */
typedef struct _t_AVSimpleSelObj	*AVSimpleSelObj;

/* Callback functions for text highlight, de-highlight, and destroy selection object.  */
/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSimpleSelHiliteProc)(AVSimpleSelObj selObj, AVDoc doc);
/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSimpleSelDeHiliteProc)(AVSimpleSelObj selObj, AVDoc doc);
/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AVSimpleSelDataDestroyProc)(AVSimpleSelObj selObj);

/** */
typedef struct _t_AVSimpleSelProcs {
	/** sizeof(AVSimpleSelProcsRec) */
	ASSize_t					size;			
	/** sub-selection type */
	char						*type;			
	/** */
	AVSimpleSelHiliteProc		hiliteProc;
	/** */
	AVSimpleSelDeHiliteProc		deHiliteProc;
	/** */
	AVSimpleSelDataDestroyProc	destroyProc;
} AVSimpleSelProcsRec, *AVSimpleSelProcs;

/************************************************************************************\
|*																					*|
|*								  AVStatusItem										*|
|*																					*|
\************************************************************************************/
/** */
typedef struct _t_AVStatusItem *AVStatusItem;

/************************************************************************************\
|*																					*|
|*									 Undo 											*|
|*																					*|
\************************************************************************************/
/* Undo/redo stuff */
/** Opaque data type to be used with AVUndo APIs. */
typedef struct _t_AVUndo *AVUndo;	

/**
	Private data for use by callbacks in the AVUndoHandler. 
	
	@see AVUndoGetData 
	@see AVUndoNew 
	@see AVUndoSetData
*/
typedef void* AVUndoHandlerData;

/**
	Callback for AVUndoHandler. Called when the user initiates 
	an Undo or Redo command. Use this to verify that the undo 
	record is still valid and the operation can be performed. 
	
	@param undo The undo record
	@return true if the operation can be performed, false otherwise. 
	@see AVUndoExecuteProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVUndoVerifyProc)
	(AVUndo undo);

/**
	Callback for AVUndoHandler. Called when the undo object 
	is no longer needed. Use this to free any dynamic data that 
	was associated with the object by the handler. 
	@param undo The undo record
	@see AVDocClearUndos 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVUndoReleaseProc)
	(AVUndo undo);

/* Performs the AVUndo. */
/** Callback for AVUndoHandler. Called when the user initiates an Undo or Redo command
	and the AVUndoVerifyProc returns true. Use this to perform the requested operation.
	@param undo IN/OUT? the undo record.
	@return true if the requested operation is performed successfully, false otherwise.
	@see AVUndoVerifyProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AVUndoExecuteProc)
	(AVUndo undo);

/**
	Callback for AVUndoHandler. Called when the user initiates 
	an Undo or Redo command and the AVUndoVerifyProc returns 
	true. Use this to return the UI title string for the undo 
	record. 
	@param undo The undo record. 
	@param title (Filled by the method) The UI title string 
	for the undo record, as a text object.
	@see AVUndoVerifyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVUndoGetTitleProc)
	(AVUndo undo, ASText title);

/**
	Callback type for AVUndoHandler. The beginUndoRedo procedure 
	is called when the user initiates an Undo or Redo command 
	and the AVUndoVerifyProc returns true. The endUndoRedo procedure 
	is called when execution of the operation is complete. Use 
	callbacks of this type to notify the handler that an undo 
	or redo operation is beginning or ending. 

	These callbacks are optional. They can be used to allocate 
	and deallocate memory for the operations, for example, or, 
	when grouping undo records, to suspend UI updates during 
	the operation. 
	@param doc The document containing the undo record. 
	@param bUndo true if the user initiated an undo operation, 
	false if it is a redo operation.
	@see AVUndoGetTitleProc 
	@see AVUndoExecuteProc 
	@see AVUndoVerifyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVUndoBeginEndProc)
	(AVDoc doc, ASBool bUndo);

/**
	Contains callback procedure for an AVUndo record that perform 
	the undo and redo operations. 
	@see AVDocBeginUndoOperation 
	@see AVDocClearUndos 
	@see AVDocEndUndoOperation 
	@see AVDocGetTopUndo 
	@see AVUndoGetType 
	@see AVUndoNew 
*/
typedef struct _t_AVUndoHandler {
	/** Size of the data structure. Must be set to sizeof(AVUndoHandlerRec). */
	ASSize_t	size;
	/** The type of the undo record. Any client-defined string, which can be
	matched for retrieval by AVDocGetTopUndo. 
	*/
	const char *type;
	/** */
	AVUndoVerifyProc		VerifyUndo;
	/** */
	AVUndoExecuteProc		Undo;
	/** */
	AVUndoVerifyProc		VerifyRedo;
	/** */
	AVUndoExecuteProc		Redo;
	/** */
	AVUndoGetTitleProc		GetUndoTitle;
	/** */
	AVUndoGetTitleProc		GetRedoTitle;
	/** */
	AVUndoReleaseProc		Release;
	/** */
	AVUndoBeginEndProc		BeginUndoRedo;
	/** */
	AVUndoBeginEndProc		EndUndoRedo;
} AVUndoHandlerRec, *AVUndoHandler;

/**
	An ink value for use in color separation methods. 
	@see AVPageViewGetPixelInformationAtPoint
*/
typedef struct AVInkValue
{
	/** */
    ASAtom         inkName;
    /** */
    float          value;
} AVInkValue;

// If a command handler implements these properties
// a toolbar button or menu item can be generated
// from them directly using AVMenuItemFromCommand
// or AVToolButtonFromCommand.
#define kAVMenuKeyName "Menu:Name"
#define kAVMenuKeyTitle "Menu:Title"
#define kAVMenuKeyTitleChanges "Menu:TitleChanges"
#define kAVMenuKeyIcon "Menu:Icon"
#define kAVMenuKeyEnabled "Menu:Enabled"
#define kAVMenuKeyMarked "Menu:Marked"
#define kAVMenuKeyVisible "Menu:Visible"
#define kAVMenuKeyShortcut "Menu:Shortcut"
#define kAVMenuKeyShortcutFlags "Menu:ShortcutFlags"
#define kAVMenuKeyHelpText "Menu:HelpText"

#define kAVToolButtonKeyName "ToolButton:Name"
#define kAVToolButtonKeyIcon "ToolButton:Icon"
#define kAVToolButtonKeyEnabled "ToolButton:Enabled"
#define kAVToolButtonKeyMarked "ToolButton:Marked"
#define kAVToolButtonKeyVisible "ToolButton:Visible"
#define kAVToolButtonKeyShortcut "ToolButton:Shortcut"
#define kAVToolButtonKeyHelpText "ToolButton:HelpText"
#define kAVToolButtonKeyExternal "ToolButton:External"
#define kAVToolButtonKeyInternal "ToolButton:Internal"
#define kAVToolButtonKeyMenu "ToolButton:Menu"
#define kAVToolButtonKeyLabel "ToolButton:Label"
#define kAVToolButtonKeyLabelPriority "ToolButton:LabelPriority"

/**
	A set of priority values for a tool bar button label text. 
	The priority determines the preference order in which labels 
	are shown when a toolbar is too short to hold all of the 
	button labels. 
	@see AVToolButtonGetLabelText 
	@see AVToolButtonSetLabelText
*/
enum {
	/** */
	kAVButtonPriorityOffExtraLow = 100,
	/** */
	kAVButtonPriorityOffLow = 200,
	/** */
	kAVButtonPriorityOffNormal = 300,
	/** */
	kAVButtonPriorityOffHigh = 400,
	/** */
	kAVButtonPriorityOffExtraHigh = 500,

	/** */
	kAVButtonPriorityOnExtraLow = 600,
	/** */
	kAVButtonPriorityOnLow = 700,
	/** */
	kAVButtonPriorityOnNormal = 800,
	/** */
	kAVButtonPriorityOnHigh = 900,
	/** */
	kAVButtonPriorityOnExtraHigh = 1000,
	
	/** */
	kAVButtonPriorityAlwaysOn = 1100
};
typedef ASEnum16 AVToolButtonLabelPriority;

// If a command returns these properties it will automatically
// be included as a Task in the task bar. It must also implement
// the properties required to create a toolbar button, described
// above.
#define kAVCommandKeyIsTask "IsTask"
#define kAVTaskKeyToolBarName "Task:Toolbar"
#define kAVTaskKeyDoesWork "Task:DoesWork"
#define kAVTaskKeyButtonOrder "Task:ButtonOrder"
#define kAVTaskKeyHowToPanelName "Task:HowToPanel"
#define kAVTaskKeyButtonDefaultUserVisible "Task:ButtonDefaultUserVisible"

/** */
enum {
	/** */
	kAcrobatBasicsSortKey = 100,
	/** */
	kAVCreateSortKey = 200,
	/** */
	kAVReviewAndCommentSortKey = 300,
	/** */
	kAVFormSortKey = 400,
	/** */
	kAVSecureSortKey = 500,
	/** */
	kAVSignSortKey = 600,
	/** */
	kAVPrintProductionSortKey = 700,
	/** */
	kAVEngineeringToolsSortKey = 800,
	/** */
	kAVHowToSortKey = ASMAXUns32
};
typedef ASUns32 AVHowToTopicSortKey;

/** This enumerates the order of the built-in task buttons. */
enum {
	/** */
	kAVeBookTaskOrder = 100,
	/** */
	kAVNewDocumentTaskOrder = 200,
	/** */
	kAVReviewAndCommentTaskOrder = 300,
	/** */
	kAVSendForReviewTaskOrder = 350,
	/** */
	kAVSecureTaskOrder = 400,
	/** */
	kAVSignTaskOrder = 500,
	/** */
	kAVFormTaskOrder = 600,
	/** */
	kAVPictureTasksTaskOrder = 700
};
typedef ASEnum16 AVTaskOrderNumbers;

/** This enumerates the order of the toolbars listed in the Tools menu */
enum {
	/** */
	kAVCommentingMenuOrder = 100,
	/** */
	kAVAdvCommentingMenuOrder = 200,
	/** */
	kAVSelectionMenuOrder = 300,
	/** */
	kAVZoomMenuOrder = 400,
	/** */
	kAVNavigationMenuOrder = 500,
	/** */
	kAVAdvEditingMenuOrder = 600,
	/** */
	kAVMeasuringMenuOrder = 700,
	/** */
	kAVRotateViewMenuOrder = 800,
	/** */
	kAVFindMenuOrder = 900,
	/** */
	kAVEndMenuOrder = kASMAXEnum16
};
typedef ASEnum16 AVToolsMenuOrderNumbers;

// If a command handler implements these properties, doc status
// icons can be generated from the AVCommand.
#define kAVDocStatusKeyName "DocStatus:Name"
#define kAVDocStatusKeyTipText "DocStatus:TipText"
#define kAVDocStatusKeyHelpText "DocStatus:HelpText"
#define kAVDocStatusKeyAlwaysInformText "DocStatus:PrefText"
#define kAVDocStatusKeySmallIcon "DocStatus:SmallIcon"
#define kAVDocStatusKeyLargeIcon "DocStatus:LargeIcon"
#define kAVDocStatusKeyDefaultsToChecked "DocStatus:DefaultsToChecked"

// If a command returns these properties, it will automatically
// be queried when the document status display is updated.
#define kAVCommandKeyIsDocStatus "IsDocStatus"

/**
	Constants that specify language format values for use in 
	AVAppLanguageParams. 
	@see AVAppGetLanguageWithParams
*/
enum {
 /** */
 kAVAppLanguage_RFC1766,
 /** */
 kAVAppLanguage_LCID,
 /** */
 kAVAppLanguage_ISO4Char	
};
typedef ASEnum16 AVAppLanguageFormat;

#define kMaxLanguageNameLen 6
/** Data structure containing language format information, in which to return the language in
	use for an application.
*/
typedef struct _AVAppLanguageParamsRec {
	/** Size of the data structure. Must be set to sizeof(AVAppLanguageParamsRec).*/
	ASSize_t size;
	/** The format in which to specify the language.*/
	AVAppLanguageFormat kLangFormat;
	/** The returned language value in the specified format. For
		details of language values, see Language Codes.
	*/
	char szAVAppLanguage[kMaxLanguageNameLen];
} AVAppLanguageParamsRec, *AVAppLanguageParams;

/** Constants used to define the set of bookmarks retrieved by AVDocGetBookmarks. More than
	one constant can be passed into the API by OR'ing them. 
	Ex: kAVBookmarkFilterSelected | kAVBookmarkFilterFocus
*/
enum {
 kAVBookmarkFilterSelected = 1,
 kAVBookmarkFilterFocus = 2
};
typedef ASEnum16 AVBookmarkFilter;


enum
{
	kAVFavToolsFlagOkInternal	= 1 << 0,	// It is ok for the tool to be a favorite in a non-external situation
	kAVFavToolsFlagOkExternal	= 1 << 1	// It is ok for the tool to be a favorite in an external situation (eg: the web browser)
};

typedef AVTFlagBits AVFavToolFlags;

/**
	Describes the paragraph direction. 
*/
enum {

	kAVParagraphDirectionLeftToRight,

	kAVParagraphDirectionRightToLeft,

	kAVParagraphDirectionSameAsDocument,

	/** Enum terminator.*/
	kAVParagraphDirection_END_ENUM
};
typedef ASEnum8 kAVParagraphDirection;

#ifdef __cplusplus
}
#endif


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_AVExpT */
