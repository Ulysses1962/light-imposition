/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1995-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 UnixCalls.h

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

#ifndef _H_UnixCalls
#define _H_UnixCalls
#pragma once
#include "acroassert.h"

/* for Adobe use only */
#define _UnixHFT_LATEST_VERSION 0x00040000
#define _UnixHFT_IS_BETA 0

/* for public use */
#define UnixHFT_LATEST_VERSION (_UnixHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _UnixHFT_LATEST_VERSION) : _UnixHFT_LATEST_VERSION)

#define UnixHFT_VERSION_4   0x00040000

#include <X11/Intrinsic.h>

#include "PIRequir.h"
#include "CoreExpT.h" /* HFT */

#ifdef __cplusplus
extern "C" {
#endif

#if PI_UNIX_VERSION != 0

extern HFT gUnixHFT;
extern ASUns32 gUnixVersion;

/* UnixAppGetAppShellWidget */
#define UnixAppGetAppShellWidgetSEL 1

/**
	Gets the application shell widget created when the Acrobat 
	viewer called XtAppInitialize. The widget can be used to 
	get the display, screen, XtAppContext, and so forth. Each 
	plug-in should create its own application shell widget, 
	which will be the parent widget for all dialogs the plug-
	in creates. 
	@return The Acrobat viewer's application shell widget. 
	@see UnixAppAddModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 Widget (ACCBPROTO2 *UnixAppGetAppShellWidgetSELPROTO)
	(void);
#define UnixAppGetAppShellWidget (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppGetAppShellWidgetSELPROTO)(gUnixHFT[1])))

/* UnixAppLoadPlugInAppDefaults */
#define UnixAppLoadPlugInAppDefaultsSEL 2

/**
	Loads the system and user application defaults for a plug-
	in into the screen resource database. 

	The screen resource database is shared by all plug-ins, 
	and the Acrobat viewer, so name space is important. 

	Application default filenames are generated from the XFILESEARCHPATH 
	and XUSERFILESEARCHPATH environment variables, and the className 
	parameter. 
	@param className IN/OUT? Should be the same as the class name used 
	to create the plug-in's application shell widget. 
	@param fallbackResources IN/OUT? NULL-terminated array of resource 
	specification strings to use if the system application default 
	file is not found. See XtAppInitialize for more information. 
	fallbackResources should always start with the class name 
	of the plug-in so they do not interfere with other plug-
	ins, or Acrobat's resources. That is why it is suggested 
	to create an application shell widget for each plug-in, 
	with a unique class name. 
	@exception genErrNoMemory 
	@see UnixSysPrefInit 
	@see UnixSysPrefUpdate 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixAppLoadPlugInAppDefaultsSELPROTO)
        (String className, String *fallbackResources);
#define UnixAppLoadPlugInAppDefaults (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppLoadPlugInAppDefaultsSELPROTO)(gUnixHFT[2])))

/* UnixAppClipboardGetItemId */
#define UnixAppClipboardGetItemIdSEL 3

/**
	Gets the item_id, display, and window needed to call XmClipboardCopy 
	during a Copy or Cut operation of a custom selection server 
	(see AVDocSelectionServer). 

	The Acrobat viewer will have already started the Motif Clipboard 
	Copy or Cut operation by calling XmClipboardStartCopy before 
	calling the selection server's AVDocSelectionCopyProc or 
	AVDocSelectionCutProc function. When the Copy or Cut function 
	returns, the Acrobat viewer calls XmClipboardEndCopy. 
	@param displayPtr IN/OUT? (Filled by the method) The clipboard's 
	display. 
	@param windowPtr IN/OUT? (Filled by the method) The clipboard's 
	window. 
	@return The item_id. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 long (ACCBPROTO2 *UnixAppClipboardGetItemIdSELPROTO)
	(Display **displayPtr, Window *windowPtr);
#define UnixAppClipboardGetItemId (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppClipboardGetItemIdSELPROTO)(gUnixHFT[3])))

/* UnixAppDispatchEvent */
#define UnixAppDispatchEventSEL 4

/**
	A wrapper function for XtDispatchEvent. 

	The Acrobat viewer needs to look at every event before it 
	is dispatched by libXt. If a plug-in needs to dispatch events, 
	it must dispatch them by either calling UnixAppDispatchEvent 
	or UnixAppProcessEvent. 
	@param eventPtr IN/OUT? The event. 
	@return true if the event was dispatched, false if no handler was 
	found to dispatch the event to. 
	@see UnixAppProcessEvent 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 Boolean (ACCBPROTO2 *UnixAppDispatchEventSELPROTO)
	(XEvent *eventPtr);
#define UnixAppDispatchEvent (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppDispatchEventSELPROTO)(gUnixHFT[4])))

/* UnixAppProcessEvent */
#define UnixAppProcessEventSEL 5

/**
	A wrapper function for XtAppProcessEvent. 

	The Acrobat viewer needs to look at every event before it 
	is dispatched by libXt. If a plug-in needs to dispatch events, 
	it must dispatch them by either calling UnixAppDispatchEvent 
	or UnixAppProcessEvent. 
	@param appContext IN/OUT? The application context that identifies 
	the application; same as the parameter passed to XtAppProcessEvent. 
	
	@param mask IN/OUT? A mask specifying which events to handle; same 
	as the parameter passed to XtAppProcessEvent. 
	@see UnixAppDispatchEvent 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixAppProcessEventSELPROTO)
	(XtAppContext appContext, XtInputMask mask);
#define UnixAppProcessEvent (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppProcessEventSELPROTO)(gUnixHFT[5])))

/* UnixAppWaitForWm */
#define UnixAppWaitForWmSEL 6

/**
	Dispatches events until the shell is mapped or the time 
	out specified by the shell's XtNwaitForWm and XmNwmTimeout 
	has expired. 

	This method can be used to wait for the window manager to 
	map the shell window when bringing up a dialog. 
	@param shellWidget IN/OUT? Must be a ShellWidgetClass widget, and 
	it must be realized. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixAppWaitForWmSELPROTO)
	(Widget shellWidget);
#define UnixAppWaitForWm (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppWaitForWmSELPROTO)(gUnixHFT[6])))

/* UnixAppGetPlugInFilename */
#define UnixAppGetPlugInFilenameSEL 7

/**
	Gets the plug-in's filename. The directory can be used to 
	find auxiliary files for that plug-in. 

	The Acrobat viewer searches all the directories specified 
	in the systemPlugInPath and userPlugInPath resources to 
	find plug-ins. 
	@param thePI IN/OUT? The gExtensionID extension registering the 
	plug-in. 
	@return The plug-in's filename. The string returned must not be 
	altered or freed. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixAppGetPlugInFilenameSELPROTO)
	(ExtensionID thePI);
#define UnixAppGetPlugInFilename (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppGetPlugInFilenameSELPROTO)(gUnixHFT[7])))

/* UnixSysGetHomeDirectory */
#define UnixSysGetHomeDirectorySEL 8

/**
	Gets the home directory of the user running the Acrobat 
	viewer. If the HOME environment variable is set, its value 
	is returned. Otherwise the method looks in the passwd database. 
	
	@return The user's home directory. The string returned by this method 
	must not be altered or freed. 
	@see UnixSysGetCwd 
	@see UnixSysGetHostname 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetHomeDirectorySELPROTO)
	(void);
#define UnixSysGetHomeDirectory (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetHomeDirectorySELPROTO)(gUnixHFT[8])))

/* UnixSysGetInstallDirectory */
#define UnixSysGetInstallDirectorySEL 9

/**
	Gets the directory in which the Acrobat viewer is installed. 
	The launch shell script sets the installation directory 
	in the environment variable ACRO_INSTALL_DIR. 

	Auxiliary files can be found by concatenating the installation 
	directory with the configuration name sub-directory: 

	"< installation_dir>/< config_name>" 
	@return The Acrobat viewer's installation directory. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetInstallDirectorySELPROTO)
	(void);
#define UnixSysGetInstallDirectory (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetInstallDirectorySELPROTO)(gUnixHFT[9])))

/* UnixSysGetConfigName */
#define UnixSysGetConfigNameSEL 10

/**
	Gets the Acrobat viewer's configuration name. The name will 
	be one of: 

	SunOS - sparcsun Solaris - sparcsolaris HP-UX - hppahpux 
	

	The Acrobat viewer's launch shell script uses uname to set 
	the configuration name in the environment variable ACRO_
	CONFIG. 

	Auxiliary files can be found by concatenating the installation 
	directory with the configuration name sub-directory: 

	"< installation_dir>/< config_name>" 
	@return The Acrobat viewer's configuration name. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetConfigNameSELPROTO)
	(void);
#define UnixSysGetConfigName (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetConfigNameSELPROTO)(gUnixHFT[10])))

/* UnixSysGetHostname */
#define UnixSysGetHostnameSEL 11

/**
	Gets the host name. 
	@return The host name. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetHostnameSELPROTO)
	(void);
#define UnixSysGetHostname (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetHostnameSELPROTO)(gUnixHFT[11])))

/* UnixSysGetTempFileDirectory */
#define UnixSysGetTempFileDirectorySEL 12

/**
	Gets the temporary file directory specified by the user. 
	The default is '/tmp.' 
	@return The temporary file directory. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetTempFileDirectorySELPROTO)
	(void);
#define UnixSysGetTempFileDirectory (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetTempFileDirectorySELPROTO)(gUnixHFT[12])))

/* UnixSysGetCwd */
#define UnixSysGetCwdSEL 13

/**
	Gets the current working directory. This method tries to 
	eliminate automounter tmp directories and symbol links. 
	Using stat, it checks if the environment variable PWD specifies 
	the same directory returned by getcwd. 
	@return The current working directory. 
	@exception genErrNoMemory 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetCwdSELPROTO)
	(void);
#define UnixSysGetCwd (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetCwdSELPROTO)(gUnixHFT[13])))

/* UnixSysGetString */
#define UnixSysGetStringSEL 14

/**
	A convenience method for getting internationalized strings. 
	It sets up an XtResource specification with the resourceName 
	and defaultString parameters, then calls XtGetSubresources 
	with the name string and class String. 

	This method does not cache its results. 
	@param widget IN/OUT? The widget, passed to XtGetSubresources. 
	
	@param resourceName IN/OUT? Used to create an XtResource specification 
	to get the string. 
	@param defaultString IN/OUT? Default string, used if the requested 
	string is not found. 
	@return The requested string, or the default string if the requested 
	string cannot be located. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *UnixSysGetStringSELPROTO)
	(Widget widget, char *resourceName, char *defaultString);
#define UnixSysGetString (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetStringSELPROTO)(gUnixHFT[14])))

/* UnixSysGetCursor */
#define UnixSysGetCursorSEL 15

/**
	A convenience method for getting internationalized cursors. 
	It sets up a XtResource specification with the resourceName 
	and defaultName parameters, then calls XtGetSubresources 
	with the name cursor and class Cursor. 

	The cursor name returned by XtGetSubresources is used to 
	find the cursor bitmap by calling XmGetPixmapByDepth, which 
	first checks the Motif image cache. If not found in the 
	image cache, then XmGetPixmapByDepth searches for an xbm 
	file. If the cursor name is not an absolute filename (that 
	is, does not start with Ô/'), then XmGetPixmapByDepth uses 
	the environment variable XBMLANGPATH to find it. 

	If a bitmap is found, then UnixSysGetCursor appends Mask 
	to the bitmap name to find the cursor mask bitmap, otherwise 
	it uses the defaultBits and defaultMaskBits to create a 
	cursor. 

	This method does not cache its results; a new cursor is 
	created each time it is called. 
	@param widget IN/OUT? The widget to pass to XtGetSubresources. 
	
	@param resourceName IN/OUT? Used to create an XtResource specification 
	to locate the cursor. 
	@param defaultName IN/OUT? Used to create an XtResource specification 
	to locate the cursor. 
	@param defaultBits IN/OUT? Bitmap used for the cursor if the requested 
	cursor cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultMaskBits IN/OUT? Bitmap used as the cursor mask if 
	the requested cursor cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultWidth IN/OUT? Cursor width if the requested cursor 
	cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultHeight IN/OUT? Cursor width if the requested cursor 
	cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultHotX IN/OUT? x-coordinate of the cursor's hot spot 
	if the requested cursor cannot be found. It is passed to 
	XCreatePixmapCursor. 
	@param defaultHotY IN/OUT? y-coordinate of the cursor's hot spot 
	if the requested cursor cannot be found. It is passed to 
	XCreatePixmapCursor. 
	@return The requested cursor. 
	@exception genErrNoMemory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 Cursor (ACCBPROTO2 *UnixSysGetCursorSELPROTO)
	(Widget widget, char *resourceName, char *defaultName, char *defaultBits, char *defaultMaskBits, unsigned int defaultWidth, unsigned int defaultHeight, int defaultHotX, int defaultHotY);
#define UnixSysGetCursor (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetCursorSELPROTO)(gUnixHFT[15])))

/* UnixSysGetIcon */
#define UnixSysGetIconSEL 16

/**
	A convenience method for getting internationalized icons, 
	which are bitmaps (pixel maps with depth 1). Use UnixSysGetPixmap 
	for pixel maps with depth greater than 1. 

	This method sets up an XtResource specification with the 
	resourceName and defaultName parameters, then calls XtGetSubresources 
	with the name icon and class Icon. 

	The icon name returned by XtGetSubresources is used to find 
	the icon by calling XmGetPixmapByDepth, which first checks 
	the Motif image cache. If not found in the image cache, 
	then XmGetPixmapByDepth searches for an xbm file. If the 
	cursor name is not an absolute filename (that is, does not 
	start with Ô/'), then XmGetPixmapByDepth uses the environment 
	variable XBMLANGPATH to find it. 

	If an icon is not found, defaultBits is used to create an 
	icon. 

	This method does not cache its results, a new icon will 
	be created each time it is called. 
	@param widget IN/OUT? Widget, as passed to XtGetSubresources. 
	@param resourceName IN/OUT? Used to create an XtResource specification 
	for the cursor. 
	@param defaultName IN/OUT? Used to create an XtResource specification 
	for the cursor. 
	@param defaultBits IN/OUT? Bitmap used for the icon if the requested 
	icon is not found. Passed to XCreateBitmapFromData. 
	@param defaultWidth IN/OUT? Width used for the default icon if 
	the requested icon is not found. Passed to XCreateBitmapFromData. 
	
	@param defaultHeight IN/OUT? Height used for the default icon if 
	the requested icon is not found. Passed to XCreateBitmapFromData. 
	
	@return The requested icon. 
	@see UnixSysGetPixmap 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 Pixmap (ACCBPROTO2 *UnixSysGetIconSELPROTO)
	(Widget widget, char *resourceName, char *defaultName, char *defaultBits, unsigned int defaultWidth, unsigned int defaultHeight);
#define UnixSysGetIcon (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetIconSELPROTO)(gUnixHFT[16])))

/* UnixSysGetPixmap */
#define UnixSysGetPixmapSEL 17

/**
	A convenience method for getting internationalized pixel 
	maps of depth greater than 1. This method is intended for 
	displaying graphics in an About box. The pixel map will 
	be created with the same depth as the widget. Use UnixSysGetIcon 
	for bitmaps (that is, pixel maps with depth 1). 

	This method sets up an XtResource specification with the 
	resourceName and defaultFilename parameters, then calls 
	XtGetSubresources with the name pixmap and class Pixmap. 
	

	If the pixmap filename returned by XtGetSubresources exists 
	and is a valid xbm or xpm file, then a pixmap will be created. 
	Otherwise defaultXpmData is used. 

	If a gray scale visual (or the application is in grayscale 
	mode because it could not allocate enough colors, see color 
	cube resources for more info), then the color map key m 
	will be used in the xpm file (or data), otherwise c will 
	be used. 

	XLookUpColor is used to translate the colors specified in 
	the xpm color map. Because the color map is a valuable shared 
	resource, and UnixSysGetPixmap was only intended for the 
	graphics in an About box, XAllocColor is not called. Instead 
	the closest color in the Acrobat viewer's color cube is 
	used. Therefore, it is suggested that:
	<ul>
	<li> All xpm files (and data) have both c and m color map keys, 
	for both color and grayscale modes. 
	<li> Specify all colors in hexadecimal form (#rgb) to avoid 
	problems with the Xserver's RGB database. 
	<li> Specify only the 8 fully saturated colors (black, white, 
	red, green, ...) # 000, # fff, # f00, # 0f0, # 00f, # ff0, 
	# 0ff, # f0f, since only these colors (which are the corners 
	of the color cube) can be guaranteed. 
	</ul>

	This method does not cache its results, a new pixmap will 
	be created each time it is called. 
	@param widget IN/OUT? The widget, passed to XtGetSubresources. 
	
	@param resourceName IN/OUT? Used to create an XtResource specification 
	for the pixmap. 
	@param defaultFilename IN/OUT? Name of a file containing a default 
	pixmap to use if the requested pixmap is not found. Used 
	to create an XtResource specification for the pixmap. 
	@param defaultXpmData IN/OUT? A default pixmap used if the requested 
	pixmap cannot be found. 
	@param defaultXpmCount IN/OUT? The number of strings in the defaultXpmData 
	array, typically XtNumber( defaultXpmData). 
	@param widthPtr IN/OUT? Width of the image. Used both if the requested 
	pixmap is found, and for the default pixmap, if the requested 
	pixmap is not found. 
	@param heightPtr IN/OUT? Height of the image. Used both if the 
	requested pixmap is found, and for the default pixmap, if 
	the requested pixmap is not found. 
	@return The requested pixmap. 
	@see UnixSysGetIcon 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 Pixmap (ACCBPROTO2 *UnixSysGetPixmapSELPROTO)
	(Widget widget, char *resourceName, char *defaultFilename, char **defaultXpmData, int defaultXpmDataCount, unsigned int *widthPtr, unsigned int *heightPtr);
#define UnixSysGetPixmap (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysGetPixmapSELPROTO)(gUnixHFT[17])))

/* UnixAppAddModifierCallback */
#define UnixAppAddModifierCallbackSEL 18

/**UNIX-specific Methods 
	Registers a callback to call when a KeyEvent that specifies 
	a modifier key is dispatched. The modifier state can be 
	queried with AVSysGetModifiers or XQueryPointer. 

	The callback will be called even though the state of the 
	modifiers may not have actually changed. For example, if 
	both the left and right control keys are depressed, then 
	releasing one of them causes the callbacks to be called, 
	even though the state has not changed. 
	@param callback IN/OUT? The procedure to call. Must be declared 
	in the form: static void UnixPageViewModifierCallback (Widget 
	widget, XtPointer clientData, XtPointer callData) where 
	callData is an eventPtr, and widget is the acrobat viewer's 
	shell widget (as returned by UnixAppGetAppShellWidget). 
	
	@param closure IN/OUT? User-supplied data to pass (as clientData) 
	to proc each time it is called. 
	@exception genErrNoMemory 
	@see UnixAppRemoveModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixAppAddModifierCallbackSELPROTO)
	(XtCallbackProc callback, XtPointer closure);
#define UnixAppAddModifierCallback (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppAddModifierCallbackSELPROTO)(gUnixHFT[18])))

/* UnixAppRemoveModifierCallback */
#define UnixAppRemoveModifierCallbackSEL 19

/**
	Removes a callback added by UnixAppAddModifierCallback. 
	Both closure and callback must match the values passed in 
	the call to UnixAppAddModifierCallback. 
	@param callback IN/OUT? The callback to remove. 
	@param closure IN/OUT? User-supplied data that was passed in the 
	call to UnixAppAddModifierCallback. 
	@see UnixAppAddModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixAppRemoveModifierCallbackSELPROTO)
	(XtCallbackProc callback, XtPointer closure);
#define UnixAppRemoveModifierCallback (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixAppRemoveModifierCallbackSELPROTO)(gUnixHFT[19])))

/* UnixSysPrefInit */
#define UnixSysPrefInitSEL 20

/**
	Reads preferences from the specified file into a resource 
	manager database. The file format is the same as an application 
	defaults file. 

	The database is then used to initialize the structure pointed 
	to by base, which is described by resources. See XtGetApplicationResources 
	for more information on the format of the resources parameter. 
	

	UnixSysPrefInit uses string resource converters to convert 
	the string data in the preference file to the appropriate 
	data type, as described in the resources parameter. See 
	XtConvertAndStore for a list of standard converters, which 
	are automatically registered. For other data types described 
	in the resources parameter, a converter needs to register 
	with XtSetTypeConverter prior to calling UnixSysPrefInit. 
	

	Furthermore, a reverse string converter needs to register 
	prior to calling UnixSysPrefUpdate. The Acrobat viewer has 
	already registered reverse string converters for the following 
	general data types: XtRBoolean, XtRBool, XtRDimension, XtRFloat, 
	XtRInt, XtRPosition, XtRShort, XtRUnsignedChar. 

	Also the following Acrobat viewer data types have both forward 
	and reverse string converters:\n\n
	\t#define XtRBoolean16 "Boolean16" \n
	\t#define XtRAVZoomType "AVZoomType" \n
	\t#define XtRFixed " ASFixed" \n
	\t#define XtRPDColorValue "PDColorValue" \n
	\t#define XtRPDPageMode "PDPageMode" \n
	Finally, Motif provides the function XmRepTypeRegister, 
	which will create a string converter for a list of strings 
	to an unsigned char. Also, XmRepTypeAddReverse will create 
	the reverse string converter. 
	@param widget IN/OUT? The plug-in's application shell. This value 
	is used for calling XtConvertAndStore. 
	@param name IN/OUT? The name used to create the plug-in's application 
	shell. This value is used to match items in the database. 
	
	@param class IN/OUT? The class used to create the plug-in's application 
	shell. This value is used to match items in the database. 
	
	@param prefFilename IN/OUT? The file from which preferences are 
	read. If non-NULL, prefFileName must be a resource file, 
	and its contents are merged into the database pointed to 
	by dbPtr. 
	@param resources IN/OUT? List of resources to read from the file. 
	
	@param numResources IN/OUT? Number of resources to read from the 
	file (that is, the number of resources specified in the 
	resources parameter). 
	@param base IN/OUT? All string data put into the structure pointed 
	to by base must not be altered or freed. It is suggested 
	that a copy of all string data be made, as shown in the 
	Example. 
	@param dbPtr IN/OUT? Pointer to a resource manager database. The 
	database is created from the specified items in the preferences 
	file. Plug-ins typically do not need access to the resource 
	manager database created from the preference file, and can 
	pass NULL for this parameter. If dbPtr is not NULL, it must 
	point to an XrmDatabase variable which is initialized to 
	NULL, or is a valid resource manager database with which 
	the preference file will be combined. 
	@param resourceDataPtr IN/OUT? Pointer to a data buffer. Must be 
	a valid pointer. If non-NULL, all resources specified in 
	the resources parameter or contained in the file specified 
	by prefFilename are copied into this buffer. 
	@see UnixSysPrefUpdate 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixSysPrefInitSELPROTO)
	(Widget widget, String name, String Xclass, char *prefFilename, XtResourceList resources, Cardinal numResources, XtPointer base, XrmDatabase *dbPtr, void **resourceDataPtr);
#define UnixSysPrefInit (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysPrefInitSELPROTO)(gUnixHFT[20])))

/* UnixSysPrefUpdate */
#define UnixSysPrefUpdateSEL 21

/**
	Updates a preference file read in by UnixSysPrefInit. All 
	the remaining parameters are the same as those passed to 
	UnixSysPrefInit. 

	Data in the preference file are strings. UnixSysPrefUpdate 
	uses reverse string converters (for example, int to string) 
	to update the file. Therefore, reverse string converters 
	need to register for all data types listed in resources 
	prior to calling UnixSysPrefUpdate. The Acrobat viewer has 
	already registered reverse string converters for the following 
	general data types: XtRBoolean, XtRBool, XtRDimension, XtRFloat, 
	XtRInt, XtRPosition, XtRShort, XtRUnsignedChar. 

	and for the following Acrobat viewer data types: XtRBoolean16, 
	XtRAVZoomType, XtRFixed, XtRPDColorValue, XtRPDPageMode. 
	
	@param widget IN/OUT? The plug-in's application shell. This value 
	is used for calling XtConvertAndStore. 
	@param prefFileName IN/OUT? The filename containing the preferences. 
	
	@param base IN/OUT? Base. 
	@param resources IN/OUT? Resources. 
	@param numResources IN/OUT? Number of resources. 
	@param resourceData IN/OUT? Must be the data returned by UnixSysPrefInit. 
	
	@see UnixSysPrefInit 
	@since PI_UNIX_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *UnixSysPrefUpdateSELPROTO)
	(Widget widget, char *prefFilename, XtPointer base, XtResourceList resources, Cardinal numResources, void *resourceData);
#define UnixSysPrefUpdate (ACROASSERT(gUnixVersion >=UnixHFT_VERSION_4), *((UnixSysPrefUpdateSELPROTO)(gUnixHFT[21])))


#endif /* PI_UNIX_VERSION != 0 */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_UnixCalls) */

