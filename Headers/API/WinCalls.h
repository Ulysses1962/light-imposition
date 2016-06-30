/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 WinCalls.h

 ******************************************************************************************
                        **** Instructions for Plugin Developers **** 
   (In the instructions below ~ is used to refer to the HFT this file is for.  For
     example this file would be called "~Calls.h")
  
   To use this file you must declare two global variables g~HFT and g~Version.  You can
   see them declared as extern about one page down from here.  Your plugin should set a 
   #define of PI_~_VERSION to some non zero value.  Suggested values are given below in
   the "for public use" section.  ~HFT_LATEST_VERSION is not recommended because you will 
   not be able to support backwards compatible versions.  Better is to use the lowest 
   ~HFT_VERSION you require.  Later versions are compatible with earlier versions so if 
   you require ~HFT_VERSION_4 your plugin will work with ~HFT_VERSION_5, ~HFT_VERSION_6, etc.
  
   You can support old versions, yet still use newer versions of this HFT by checking the 
   value of g~Version.  If you use the standard PiMain.c supplied in the SDK this will be
   set to the actual version of the HFT returned to you (For example, you require version 4,
   you are returned version 7 which is compatible; g~Version is set to 7).  You can write
   code that looks something like this:
     if (g~Version >= ~HFT_VERSION_5) 
        CallNewSpeedyCode();
     else {
        assert(g~Version >= ~HFT_VERSION_4);  //PI_~_VERSION was defined as ~HFT_VERSION_4
        CallOldSlowCode();
     }
 ******************************************************************************************
                        **** Instructions for HFT Developer **** 
   (In the instructions below ~ is used to refer to the HFT this file is for.  For
     example this file would be called "~Calls.h")
  
   Most importantly, routines that have been released can never be deleted or changed.
   If you want to make a new version create a new call, add to the end of this file and
   increment the _~_LATEST_VERSION (note the leading underscore).  
   
   If this is the first new routine in a new version, you should change the _~_IS_BETA flag
   to 1.  Next, create a new ~_VERSION_# for plugins to use and set it to 
   ~HFT_LATEST_VERSION.  For example the last release of Acrobat was version 20.  Version 21
   is under development.  You add a new routine for version 21.  Change _~HFT_LATEST_VERSION 
   to 0x00200001 and _~_IS_BETA to 1.  Add "#define ~HFT_VERSION_21 ~HFT_LATEST_VERSION".  
   Add your routine and assert that g~Version >= ~HFT_VERSION_21.
  
   If the ~_IS_BETA flag is set to 1, you may change or delete the beta routines at will.
   Before checking in the modifications however, increment the _~HFT_LATEST_VERSION number.
   Plugins that require a BETA HFT will be refused unless they ask for the *exact* version
   since later versions may not support the calls they were using.  By incrementing the
   version number you ensure the plugin will refuse to load until it has been recompiled
   with your changes.  You also ensure plugins compiled with your changes will not load on
   older versions of Acrobat.  In other words in makes sure both sides are in sync.  
  
   Again, whenever you make a change to this file, you must increment _~HFT_LATEST_VERSION.
 
   Once the product reaches RC or similar milestone, change the _~_IS_BETA flag to 0, jump
   The _~HFT_LATEST_VERSION to the final version (0x00210000 in our example), and set the 
   ~HFT_VERSION_# to the final version number (0x00210000 in our example).  Once the HFT
   has left beta, the routines cannot be changed and a new beta must ensue (beta for version
   22 in our example).

*********************************************************************/
#ifndef _H_WinCalls
#define _H_WinCalls
#pragma once
#include "acroassert.h"

/* for Adobe use only */
#define _WINHFT_LATEST_VERSION 0x00050000
#define _WINHFT_IS_BETA 0

/* for public use */
#define WINHFT_LATEST_VERSION (_WINHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _WINHFT_LATEST_VERSION) : _WINHFT_LATEST_VERSION)

#define WINHFT_VERSION_4   0x00040000
#define WINHFT_VERSION_5   0x00050000


#ifdef __cplusplus
extern "C" {
#endif

#include "WinExpT.h"

#if PI_WIN_VERSION != 0

#include <windows.h>

extern HFT gWinHFT;
extern ASUns32 gWinVersion;

#define WinAppRegisterModelessDialogSEL 1

/**
	(Windows only) Registers mode-less dialogs with the viewer 
	so that the dialog gets the correct messages. 
	@param dialog IN/OUT? HWND for the dialog to register. 
	@see WinAppUnRegisterModelessDialog 
	@since PI_WIN_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *WinAppRegisterModelessDialogSELPROTO)
	(HWND dialog);
#define WinAppRegisterModelessDialog (ACROASSERT(gWinVersion >=WINHFT_VERSION_4), *((WinAppRegisterModelessDialogSELPROTO)(gWinHFT[WinAppRegisterModelessDialogSEL])))

#define WinAppUnRegisterModelessDialogSEL 2

/**
	(Windows only) Un-registers mode-less dialogs with the viewer. 
	
	@param dialog IN/OUT? HWND for the dialog to un-register. 
	@see WinAppRegisterModelessDialog 
	@since PI_WIN_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *WinAppUnRegisterModelessDialogSELPROTO)
	(HWND dialog);
#define WinAppUnRegisterModelessDialog (ACROASSERT(gWinVersion >=WINHFT_VERSION_4), *((WinAppUnRegisterModelessDialogSELPROTO)(gWinHFT[WinAppUnRegisterModelessDialogSEL])))

#define WinAppEnableIdleTimerSEL 3

/**Windows-specific Methods 
	(Windows only) Allows a plug-in to turn the AVAppIdle timer 
	on and off, which is needed when a plug-in calls another 
	process and thus blocks Acrobat for an extended period of 
	time. 
	@param enable IN/OUT? true to turn the timer on, false to turn 
	it off. 
	@return The previous state of the AVAppIdle timer. 
	@since PI_WIN_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 BOOL (ACCBPROTO2 *WinAppEnableIdleTimerSELPROTO)
	(BOOL enable);
#define WinAppEnableIdleTimer (ACROASSERT(gWinVersion >=WINHFT_VERSION_4), *((WinAppEnableIdleTimerSELPROTO)(gWinHFT[WinAppEnableIdleTimerSEL])))

#define WinAppGetModalParentSEL 4

/**
	(Windows only) Gets appropriate parent for any modal dialogs 
	created by a plug-in. This method is only useful if there 
	is an AVDoc; it cannot be used, for example, to put up a 
	modal dialog while a file is being opened. 

	In circumstances where there is no AVDoc, use the gHWND 
	provided in PIMAIN. C. Although this does not give perfect 
	results in some cases, there is no real alternative. (For 
	example, if a file is opened in an external application's 
	window, the dialog is not hidden if the external application 
	is hidden.) 
	@param doc IN/OUT? The AVDoc for a PDF file, if the dialog is acting 
	on an PDF document, which is generally the case. The AVDoc 
	must be provided so that for external documents, the viewer 
	can parent the dialog off the external application - instead 
	of the viewer. 
	@return HWND for modal dialogs' parent. 
	@see AVAppBeginModal 
	@see AVAppEndModal 
	@since PI_WIN_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 HWND (ACCBPROTO2 *WinAppGetModalParentSELPROTO)
	(AVDoc doc);
#define WinAppGetModalParent (ACROASSERT(gWinVersion >=WINHFT_VERSION_4), *((WinAppGetModalParentSELPROTO)(gWinHFT[WinAppGetModalParentSEL])))

/* Return the Application's palette. Return is NULL if no palette is being used */
/* DON'T release this palette handle -- it may be used by other plugins */
#define WinAppGetPaletteSEL 5

/**
	(Windows only) Gets the application's color palette in the 
	case where the system is running in 256 color mode or less. 
	Used when you want to set and realize a palette in an external 
	window before drawing to it. 

	Do not release this palette handle - it may be in use by 
	other plug-in's. 
	@return The application's color palette. NULL if the system is running 
	direct colors (15/ 16/ 24/ 32-bit) or no palette is being 
	used. 
	@since PI_WIN_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 HPALETTE (ACCBPROTO2 *WinAppGetPaletteSELPROTO)
	(void);
#define WinAppGetPalette (ACROASSERT(gWinVersion >=WINHFT_VERSION_4), *((WinAppGetPaletteSELPROTO)(gWinHFT[WinAppGetPaletteSEL])))

/* 
** If you need to use these calls from within your plug-in, you will
** need to bump up the value of PI_WIN_VERSION in PIRequir.h to
** 0x00040000.
*/

/* WinAppGetPrinterHDC */
#define WinAppGetPrinterHDCSEL 6

/**
	Gets the device context for a printer, which is the HDC 
	used to print a document. 

	Used if you need to modify the device context Acrobat creates 
	when printing to a non-PostScript printer. You should register 
	for the notification PDDocWillPrintPage and acquire the 
	printer DC for the page you wish to modify. 
	@return The printer device context. 
	@since PI_WIN_VERSION >= 0x00040000
*/
typedef ACCBPROTO1 HDC (ACCBPROTO2 *WinAppGetPrinterHDCSELPROTO)
	(void);
#define WinAppGetPrinterHDC (ACROASSERT(gWinVersion >=WINHFT_VERSION_5), *((WinAppGetPrinterHDCSELPROTO)(gWinHFT[WinAppGetPrinterHDCSEL])))


/* 
** If you need to use these calls from within your plug-in, you will
** need to bump up the value of PI_WIN_VERSION in PIRequir.h to
** 0x00050000.
*/

/* WinAppGetPrinterHDC */
#define WinAppRegisterInterfaceSEL 7

/**
	(Windows only) Register a COM interface. 
	@param COMServer IN/OUT? Pointer to COMServerRec. 
	@return true if the interface was registered with Acrobat; false 
	otherwise. 
	@since PI_WIN_VERSION >= 0x00050000
*/
typedef ACCBPROTO1 BOOL (ACCBPROTO2 *WinAppRegisterInterfaceSELPROTO)
	(COMServer);

#define WinAppRegisterInterface (ACROASSERT(gWinVersion >=WINHFT_VERSION_5), *((WinAppRegisterInterfaceSELPROTO)(gWinHFT[WinAppRegisterInterfaceSEL])))

/**
	WnAppLockViewer
	This function is designed for internal use only. It is deprecated in 
	Acrobat 6.0 and will not be available in future releases.
*/
#define WinAppLockViewerSEL 8
typedef ACCBPROTO1 void (ACCBPROTO2 *WinAppLockViewerSELPROTO)
	(BOOL);
#define  WinAppLockViewer (ACROASSERT(gWinVersion >=WINHFT_VERSION_5), *((WinAppLockViewerSELPROTO)		\
                               (gWinHFT[WinAppLockViewerSEL])))



#endif /* PI_WIN_VERSION */

#ifdef __cplusplus
}
#endif

#endif /* _H_WinCalls */

