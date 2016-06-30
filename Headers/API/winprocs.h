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

 WinProcs.h

 - Catalog of functions exported by the Windows Viewer.

*********************************************************************/


/**
	(Windows only) Registers mode-less dialogs with the viewer 
	so that the dialog gets the correct messages. 
	@param dialog IN/OUT? HWND for the dialog to register. 
	@see WinAppUnRegisterModelessDialog 
	@since PI_WIN_VERSION >= 0x00020000
*/
NPROC(void, WinAppRegisterModelessDialog, (HWND dialog))

/**
	(Windows only) Un-registers mode-less dialogs with the viewer. 
	
	@param dialog IN/OUT? HWND for the dialog to un-register. 
	@see WinAppRegisterModelessDialog 
	@since PI_WIN_VERSION >= 0x00020000
*/
NPROC(void, WinAppUnRegisterModelessDialog, (HWND dialog))

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
NPROC(BOOL, WinAppEnableIdleTimer, (BOOL enable))

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
NPROC(HWND, WinAppGetModalParent, (AVDoc doc))

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
NPROC(HPALETTE, WinAppGetPalette, (void))

/* New for Acrobat 4.0 */

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
NPROC(HDC, WinAppGetPrinterHDC, (void))

/* New for Acrobat 5.0 */

/**
	(Windows only) Register a COM interface. 
	@param pServer Pointer to COMServerRec. 
	@return true if the interface was registered with Acrobat; false 
	otherwise. 
	@since PI_WIN_VERSION >= 0x00050000
*/
NPROC(BOOL, WinAppRegisterInterface, (COMServer pServer))

/**
	This API is used to ensure that the app does not quit till the
	work that needs to be done by the interface registered with
	WinAppRegisterInterface is done.
	@param lock TRUE locks the Viewer, preventing it from quitting.
	FALSE allows the Viewer to quit.  Calling WinAppLockViewer with
	FALSE does not automatically quit the Viewer - it decrements an
	a ref count.
	@see WinAppRegisterInterface
	@since PI_WIN_VERSION >= 0x00050000
*/
NPROC(void, WinAppLockViewer, (BOOL lock))
