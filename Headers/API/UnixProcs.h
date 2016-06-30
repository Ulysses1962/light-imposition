/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 UnixProcs.h

 - Catalog of functions exported by the Unix Viewer.

*********************************************************************/


/**
	Gets the application shell widget created when the Acrobat 
	viewer called XtAppInitialize. The widget can be used to 
	get the display, screen, XtAppContext, and so forth. Each 
	client should create its own application shell widget, which 
	will be the parent widget for all dialogs the client creates. 
	
	@return The Acrobat viewer's application shell widget.  If HAS_XT is defined, returns
	a Widget object.  If HAX_XT is not defined, returns a GtkWidget *.
	@see UnixAppAddModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(Widget , UnixAppGetAppShellWidget, (void))
#else // #ifdef HAS_XT
NPROC(GtkWidget *, UnixAppGetAppShellWidget, (void))
#endif // #ifdef HAS_XT

/**
	Loads the system and user application defaults for a client 
	into the screen resource database. 

	The screen resource database is shared by all clients, and 
	the Acrobat viewer, so name space is important. 

	Application default filenames are generated from the XFILESEARCHPATH 
	and XUSERFILESEARCHPATH environment variables, and the className 
	parameter. 
	@param className Should be the same as the class name 
	used to create the client's application shell widget. 
	@param fallbackResources NULL-terminated array of resource 
	specification strings to use if the system application default 
	file is not found. See XtAppInitialize for more information. 
	fallbackResources should always start with the class name 
	of the client so they do not interfere with other clients, 
	or Acrobat's resources. That is why it is suggested to create 
	an application shell widget for each client, with a unique 
	class name.
	@exception genErrNoMemory 
	@see UnixSysPrefInit 
	@see UnixSysPrefUpdate 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(void, UnixAppLoadPlugInAppDefaults, (String className, String *fallbackResources))

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
NPROC(long, UnixAppClipboardGetItemId, (Display **displayPtr, Window *windowPtr))

/**
	A wrapper function for XtDispatchEvent. 

	The Acrobat viewer needs to look at every event before it 
	is dispatched by libXt. If a client needs to dispatch events, 
	it must dispatch them by either calling UnixAppDispatchEvent 
	or UnixAppProcessEvent. 
	@param eventPtr The event.
	@return true if the event was dispatched, false if no handler was 
	found to dispatch the event to. 
	@see UnixAppProcessEvent 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(Boolean, UnixAppDispatchEvent, (XEvent *eventPtr))

/**
	A wrapper function for XtAppProcessEvent. 

	The Acrobat viewer needs to look at every event before it 
	is dispatched by libXt. If a client needs to dispatch events, 
	it must dispatch them by either calling UnixAppDispatchEvent 
	or UnixAppProcessEvent.

	This function is only available if HAS_XT is defined.  Otherwise it is a NOPROC.
	@param appContext The application context that identifies 
	the application; same as the parameter passed to XtAppProcessEvent. 
	
	@param mask A mask specifying which events to handle; 
	same as the parameter passed to XtAppProcessEvent.
	@see UnixAppDispatchEvent 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(void, UnixAppProcessEvent, (XtAppContext appContext, XtInputMask mask))
#else // #ifdef HAS_XT
NOPROC(UnixAppProcessEvent)
#endif // #ifdef HAS_XT

/**
	Dispatches events until the shell is mapped or the time 
	out specified by the shell's XtNwaitForWm and XmNwmTimeout 
	has expired. 

	This method can be used to wait for the window manager to 
	map the shell window when bringing up a dialog. 
	@param shellWidget IN/OUT? Must be a ShellWidgetClass widget, and 
	it must be realized.  If HAS_XT is defined, shellWidget is a Widget
	object.  If HAX_XT is not defined, shellWidget is a GtkWidget *.
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
SPROC(void, UnixAppWaitForWm, (Widget shellWidget), DialogWaitForWm)
#else // #ifdef HAS_XT
SPROC(void, UnixAppWaitForWm, (GtkWidget *shellWidget), DialogWaitForWm)
#endif // #ifdef HAS_XT

/**
	Gets the client's filename. The directory can be used to 
	find auxiliary files for that client. 

	The Acrobat viewer searches all the directories specified 
	in the systemPlugInPath and userPlugInPath resources to 
	find clients. 
	@param thePI The gExtensionID extension registering the 
	client.
	@return The client's filename. The string returned must not be altered 
	or freed. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(char *, UnixAppGetPlugInFilename, (ASExtension thePI))

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
NPROC(char *, UnixSysGetHomeDirectory, (void))

/**
	Gets the directory in which the Acrobat viewer is installed. 
	The launch shell script sets the installation directory 
	in the environment variable ACRO_INSTALL_DIR. 

	Auxiliary files can be found by concatenating the installation 
	directory with the configuration name sub-directory: 

	installation_dir/config_name 
	@return The Acrobat viewer's installation directory. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(char *, UnixSysGetInstallDirectory, (void))

/**
	Gets the Acrobat viewer's configuration name. The name is 
	one of the following: 
	<ul>
	<li> SunOS - sparcsun
	<li> Solaris - sparcsolaris
	<li> HP-UX - hppahpux
	</ul>
	The Acrobat viewer's launch shell script uses uname to set 
	the configuration name in the environment variable ACRO_CONFIG. 
	

	Auxiliary files can be found by concatenating the installation 
	directory with the configuration name sub-directory: 

	installation_dir/config_name 
	@return The Acrobat viewer's configuration name. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(char *, UnixSysGetConfigName, (void))

/**
	Gets the host name. 
	@return The host name. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(char *, UnixSysGetHostname, (void))

/**
	Gets the temporary file directory specified by the user. 
	The default is /tmp. 
	@return The temporary file directory. 
	@see UnixSysGetHomeDirectory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
NPROC(char *, UnixSysGetTempFileDirectory, (void))

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
NPROC(char *, UnixSysGetCwd, (void))

/**
	A convenience method for getting internationalized strings. 
	It sets up an XtResource specification with the resourceName 
	and defaultString parameters, then calls XtGetSubresources 
	with the name string and class String. 

	This method does not cache its results. 
	@param widget IN/OUT? The widget, passed to XtGetSubresources.
	If HAS_XT is defined, shellWidget is a Widget
	object.  If HAX_XT is not defined, shellWidget is a GtkWidget *.	
	@param resourceName IN/OUT? Used to create an XtResource specification 
	to get the string. 
	@param defaultString IN/OUT? Default string, used if the requested 
	string is not found. 
	@return The requested string, or the default string if the requested 
	string cannot be located. 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(char *, UnixSysGetString, (Widget widget, char *resourceName, char *defaultString))
#else // #ifdef HAS_XT
NPROC(char *, UnixSysGetString, (GtkWidget *widget, char *resourceName, char *defaultString))
#endif // #ifdef HAS_XT

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
	is, does not start with '/'), then XmGetPixmapByDepth uses 
	the environment variable XBMLANGPATH to find it. 

	If a bitmap is found, then UnixSysGetCursor appends Mask 
	to the bitmap name to find the cursor mask bitmap, otherwise 
	it uses the defaultBits and defaultMaskBits to create a 
	cursor. 

	This method does not cache its results; a new cursor is 
	created each time it is called. 
	@param widget The widget to pass to XtGetSubresources. 
	If HAS_XT is defined, shellWidget is a Widget
	object.  If HAX_XT is not defined, shellWidget is a GtkWidget *.
	@param resourceName Used to create an XtResource specification 
	to locate the cursor. 
	@param defaultName Used to create an XtResource specification 
	to locate the cursor. 
	@param defaultBits Bitmap used for the cursor if the requested 
	cursor cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultMaskBits Bitmap used as the cursor mask 
	if the requested cursor cannot be found. It is passed to 
	XCreateBitmapFromData. 
	@param defaultWidth Cursor width if the requested cursor 
	cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultHeight Cursor width if the requested cursor 
	cannot be found. It is passed to XCreateBitmapFromData. 
	
	@param defaultHotX The x-coordinate of the cursor's hot 
	spot if the requested cursor cannot be found. It is passed 
	to XCreatePixmapCursor.
	@param defaultHotY The y-coordinate of the cursor's hot 
	spot if the requested cursor cannot be found. It is passed 
	to XCreatePixmapCursor.
	@return The requested cursor. 
	@exception genErrNoMemory 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(Cursor, UnixSysGetCursor, (Widget widget, char *resourceName, char *defaultName, char *defaultBits, char *defaultMaskBits, unsigned int defaultWidth, unsigned int defaultHeight, int defaultHotX, int defaultHotY))
#else // #ifdef HAS_XT
NPROC(Cursor, UnixSysGetCursor, (GtkWidget *widget, char *resourceName, char *defaultName, char *defaultBits, char *defaultMaskBits, unsigned int defaultWidth, unsigned int defaultHeight, int defaultHotX, int defaultHotY))
#endif // #ifdef HAS_XT

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
	start with '/'), then XmGetPixmapByDepth uses the environment 
	variable XBMLANGPATH to find it. 

	If an icon is not found, defaultBits is used to create an 
	icon. 

	This method does not cache its results, a new icon will 
	be created each time it is called. 
	@param widget Widget, as passed to XtGetSubresources. 
	If HAS_XT is defined, shellWidget is a Widget
	object.  If HAX_XT is not defined, shellWidget is a GtkWidget *.
	@param resourceName Used to create an XtResource specification 
	for the cursor. 
	@param defaultName Used to create an XtResource specification 
	for the cursor. 
	@param defaultBits Bitmap used for the icon if the requested 
	icon is not found. Passed to XCreateBitmapFromData. 
	@param defaultWidth Width used for the default icon if 
	the requested icon is not found. Passed to XCreateBitmapFromData. 
	
	@param defaultHeight Height used for the default icon 
	if the requested icon is not found. Passed to XCreateBitmapFromData.
	@return The requested icon. 
	@see UnixSysGetPixmap 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(Pixmap, UnixSysGetIcon, (Widget widget, char *resourceName, char *defaultName, char *defaultBits, unsigned int defaultWidth, unsigned int defaultHeight))
#else // #ifdef HAS_XT
NPROC(Pixmap, UnixSysGetIcon, (GtkWidget *widget, char *resourceName, char *defaultName, char *defaultBits, unsigned int defaultWidth, unsigned int defaultHeight))
#endif // #ifdef HAS_XT

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
	red, green, ...) #000, #fff, #f00, #0f0, #00f, #ff0, #0ff, 
	#f0f, since only these colors (which are the corners of 
	the color cube) can be guaranteed. 
	</ul>

	This method does not cache its results, a new pixmap will 
	be created each time it is called. 
	@param widget The widget, passed to XtGetSubresources. 
	If HAS_XT is defined, shellWidget is a Widget
	object.  If HAX_XT is not defined, shellWidget is a GtkWidget *.
	@param resourceName Used to create an XtResource specification 
	for the pixmap. 
	@param defaultFilename Name of a file containing a default 
	pixmap to use if the requested pixmap is not found. Used 
	to create an XtResource specification for the pixmap.
	@param defaultXpmData A default pixmap used if the requested 
	pixmap cannot be found. 
	@param defaultXpmDataCount The number of strings in the defaultXpmData 
	array, typically XtNumber(defaultXpmData). 
	@param widthPtr Width of the image. Used both if the requested 
	pixmap is found, and for the default pixmap, if the requested 
	pixmap is not found. 
	@param heightPtr Height of the image. Used both if the 
	requested pixmap is found, and for the default pixmap, if 
	the requested pixmap is not found.
	@return The requested pixmap. 
	@see UnixSysGetIcon 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(Pixmap, UnixSysGetPixmap, (Widget widget, char *resourceName, char *defaultFilename, char **defaultXpmData, int defaultXpmDataCount, unsigned int *widthPtr, unsigned int *heightPtr))
#else // #ifdef HAS_XT
NPROC(Pixmap, UnixSysGetPixmap, (GtkWidget *widget, char *resourceName, char *defaultFilename, char **defaultXpmData, int defaultXpmDataCount, unsigned int *widthPtr, unsigned int *heightPtr))
#endif // #ifdef HAS_XT

/** 
	Registers a callback to call when a KeyEvent that specifies 
	a modifier key is dispatched. The modifier state can be 
	queried with AVSysGetModifiers or XQueryPointer. 

	The callback will be called even though the state of the 
	modifiers may not have actually changed. For example, if 
	both the left and right control keys are depressed, then 
	releasing one of them causes the callbacks to be called, 
	even though the state has not changed.
	This function is only available if HAS_XT is defined.  Otherwise it is a NOPROC.
	@param callback The procedure to call. Must be declared 
	in the form: static void UnixPageViewModifierCallback (Widget 
	widget, XtPointer clientData, XtPointer callData) where 
	callData is an eventPtr, and widget is the Acrobat viewer's 
	shell widget, as returned by UnixAppGetAppShellWidget. 
	@param closure User-supplied data to pass (as clientData) 
	to proc each time it is called.
	@exception genErrNoMemory 
	@see UnixAppRemoveModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(void, UnixAppAddModifierCallback, (XtCallbackProc callback, XtPointer closure))
#else // #ifdef HAS_XT
NOPROC(UnixAppAddModifierCallback)
#endif // #ifdef HAS_XT

/**
	Removes a callback added by UnixAppAddModifierCallback. 
	Both closure and callback must match the values passed in 
	the call to UnixAppAddModifierCallback.
	This function is only available if HAS_XT is defined.  Otherwise it is a NOPROC.
	@param callback IN/OUT? The callback to remove. 
	@param closure IN/OUT? User-supplied data that was passed in the 
	call to UnixAppAddModifierCallback. 
	@see UnixAppAddModifierCallback 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(void, UnixAppRemoveModifierCallback, (XtCallbackProc callback, XtPointer closure))
#else // #ifdef HAS_XT
NOPROC(UnixAppRemoveModifierCallback)
#endif // #ifdef HAS_XT


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
	#define XtRBoolean16 "Boolean16" \n
	#define XtRAVZoomType "AVZoomType" \n
	#define XtRFixed " ASFixed" \n
	#define XtRPDColorValue "PDColorValue" \n
	#define XtRPDPageMode "PDPageMode" \n
	Finally, Motif provides the function XmRepTypeRegister, 
	which will create a string converter for a list of strings 
	to an unsigned char. Also, XmRepTypeAddReverse will create 
	the reverse string converter.
	This function is only available if HAS_XT is defined.  Otherwise it is a NOPROC.
	@param widget The client's application shell. This value 
	is used for calling XtConvertAndStore. 
	@param name The name used to create the client's application 
	shell. This value is used to match items in the database.
	@param Xclass The class used to create the client's application 
	shell. This value is used to match items in the database. 
	
	@param prefFilename The file from which preferences are 
	read. If non-NULL, prefFileName must be a resource file, 
	and its contents are merged into the database pointed to 
	by dbPtr. 
	@param resources List of resources to read from the file. 
	
	@param numResources Number of resources to read from the 
	file (that is, the number of resources specified in the 
	resources parameter). 
	@param base All string data put into the structure pointed 
	to by base must not be altered or freed. It is suggested 
	that a copy of all string data be made, as shown in the 
	Example. 
	@param dbPtr Pointer to a resource manager database. The 
	database is created from the specified items in the preferences 
	file. Clients typically do not need access to the resource 
	manager database created from the preference file, and can 
	pass NULL for this parameter. If dbPtr is not NULL, it must 
	point to an XrmDatabase variable which is initialized to 
	NULL, or is a valid resource manager database with which 
	the preference file will be combined. 
	@param resourceDataPtr Pointer to a data buffer. Must 
	be a valid pointer. If non-NULL, all resources specified 
	in the resources parameter or contained in the file specified 
	by prefFilename are copied into this buffer.
	@see UnixSysPrefUpdate 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(void, UnixSysPrefInit, (Widget widget, String name, String Xclass, char *prefFilename, XtResourceList resources, Cardinal numResources, XtPointer base, XrmDatabase *dbPtr, void **resourceDataPtr))
#else // HAS_XT
NOPROC(UnixSysPrefInit)
#endif // #ifdef HAS_XT

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
	This function is only available if HAS_XT is defined.  Otherwise it is a NOPROC.
	@param widget The client's application shell. This value 
	is used for calling XtConvertAndStore. 
	@param prefFilename The filename containing the preferences. 
	@param base Base. 
	@param resources Resources. 
	@param numResources Number of resources. 
	@param resourceData Must be the data returned by UnixSysPrefInit.
	@see UnixSysPrefInit 
	@since PI_UNIX_VERSION >= 0x00020000
*/
#ifdef HAS_XT
NPROC(void, UnixSysPrefUpdate, (Widget widget, char *prefFilename, XtPointer base, XtResourceList resources, Cardinal numResources, void *resourceData))
#else // #ifdef HAS_XT
NOPROC(UnixSysPrefUpdate)
#endif // #ifdef HAS_XT

