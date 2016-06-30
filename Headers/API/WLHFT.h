/**********************************************************************


 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 WLHFT.h

 - Contains all of the public definitions for using WebLink.

*********************************************************************/

#ifndef __WLHFT__
#define __WLHFT__

/* -------------------------------- includes ---------------------------------*/

#if PLUGIN
#include "ASExpT.h"
#include "PDExpT.h"
#endif /* PLUGIN */

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

/* ------------------------- WebLink datatypes ------------------------------ */

/** Data structure that specifies the pathname to the Web browser.
	@see WDAppSupportPredicate
	@see GetAppSpecifier
*/
#if MAC_PLATFORM
typedef FSSpec *WDAppSpecifier;
#endif

#if WIN_PLATFORM || OS2_PLATFORM
typedef char *WDAppSpecifier;
#endif

#if UNIX_PLATFORM
typedef char *WDAppSpecifier;
#endif

#define WDVCURRENTREV (0L)

/** @name flag options for the progress monitor.
*	
*/
/*@{*/
/** Put a thermometer-style meter in the progress display.*/
#define PROGRESS_HAS_METER (1L << 0)
/** Add either a Cancel button or a message directing the user how to abort the operation.*/
#define PROGRESS_HAS_CANCEL (1L << 1)
/*@}*/


/** A parameter block specifying the target frame of a URL.
	@see WWWOpenURLWithParams
	@see WDFollowLinkWithParamsFunction
*/
typedef struct {
	/** Should be initialized to sizeof (WWWOpenURLParamsRec). */
	ASSize_t size;  
	/** The name of the frame to open.
		NOTE:cFrame is exactly the same as the TARGET attribute of a link
		in HTML. You can target a frame by a name you have
		assigned to it. In HTML, TARGET also has four predefined
		values which can be used as if certain windows and frames
		already have names without you having to assign them:
		_blank, _parent, _self, and _top. In Weblink, only _blank and
		_self are supported.
	*/
	char * cFrame;	
} 
WWWOpenURLParamsRec, *WWWOpenURLParams;


/** Data structure that implements a progress monitor. */
typedef void *WebProgressMonitor;

/* typedefs for function defintions */
/**	Callback functions to test a driver. 
	Tests whether or not a driver can support a given Web browser application.
	@param driverRock IN Pointer to a driver-defined data structure provided in
			RegisterWebDriver.
			WDAppSpecifier Platform-specific data structure used to identify the Web
			browser application. On the Mac OS, it is a pointer to an
	@param timeoutTime IN data structure. On Windows, it is a pointer to a
			string (char*) representing the full path of the executable
			application.
	@return true if the driver is compatible with the given Web browser application, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WDAppSupportPredicateProc)(void *driverRock, WDAppSpecifier timeoutTime);

/** Callback functions for to Follow a Link
		Follows and retrieves the link specified by URL from the PDF document specified by
	theDoc. Weblink supplies the driver a fully resolved URL. The driver is solely
	responsible for launching a Web browser, if it requires one. If the driver depends on an
	external Web browser, the driver or the Web browser is responsible for bringing that
	browser to the foreground if the link data is not displayed by the Acrobat viewer.
	For relative links, Weblink prepends a base URL if specified; otherwise, it prepends
	the appropriate portion from the current document’s URL to resolve a relative link.
	Weblink automatically handles mapped links, that is, links that generate different
	results based on the location of the mouse within the links. (Such links are specified
	by checking the “Mapped Coordinates Link” box in the Edit URL dialog box. These
	links have the isMap attribute.) Weblink appends “?” to the URL, followed by the x-
	and y-coordinates of the mouse at the time the link was selected, relative to the
	upper-left corner of the link’s rectangle.
	@param driverRock IN Pointer to a driver-defined data structure provided in	RegisterWebDriver.
	@param theDoc IN The PDF document that contains the link.
	@param cURL IN A NULL-terminated C string containing the URL the link points to.
	@param cFormData IN Data from a form to be sent with the URL as post data. The
			default MIME type of form data is “application/x-www-formunencoded”.
	@return true if the driver has successfully begun the process of following the link, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WDFollowLinkFunctionProc)(void *driverRock, AVDoc theDoc, char *cURL, char *cFormData);

/** Callback functions for to Follow a Link with parameters
	Follows and retrieves the link specified by URL from the PDF document specified by
	theDoc. Weblink supplies the driver a fully resolved URL. The driver is solely
	responsible for launching a Web browser, if it requires one. If the driver depends on an
	external Web browser, the driver or the Web browser is responsible for bringing that
	browser to the foreground if the link data is not displayed by the Acrobat viewer.
	For relative links, Weblink prepends a base URL if specified; otherwise, it prepends
	the appropriate portion from the current document’s URL to resolve a relative link.
	Weblink automatically handles mapped links, that is, links that generate different
	results based on the location of the mouse within the links. (Such links are specified
	by checking the "Mapped Coordinates Link" box in the Edit URL dialog box. These
	links have the isMap attribute.) Weblink appends "?" to the URL, followed by the x-
	and y-coordinates of the mouse at the time the link was selected, relative to the
	upper-left corner of the link’s rectangle.	
	
	@param driverRock IN Pointer to a driver-defined data structure provided in RegisterWebDriver.
	@param theDoc IN The PDF document that contains the link.
	@param cURL A NULL-terminated C string containing the URL the link points to.
	@param cFormData Data from a form to be sent with the URL as post data. The
				default MIME type of form data is "application/x-www-formunencoded".
	@param params IN Parameter block specifying a target frame.
	
	@return true if the driver has successfully begun the process of following the link, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WDFollowLinkWithParamsFunctionProc)(void *driverRock, AVDoc theDoc, char *cURL, char *cFormData, WWWOpenURLParams params);

/** Callback functions for driver state changes
	
	Called whenever the state of the driver changes from inactive to current, or vice versa.
	
	@param driverRock IN Pointer to a driver-defined data structure provided in RegisterWebDriver.
	@param bComing IN Tells the driver whether it should become current or relinquish control.
					If true, the driver is being requested to become the current
					Weblink driver, and it should take any action that is
					appropriate, such as installing IAC message handlers,
					opening TCP drivers, and so on. If false, the driver is being
					asked to relinquish its status as current driver and should
					take whatever action is necessary to accomplish this.
	@return false if the driver fails to change state to that requested in bComing, otherwise true.
*/
typedef ACCB1 ASBool (ACCB2 WDBecomeDriverFunctionProc)(void *driverRock, ASBool bComing);


/** Callback functions for Weblink toolbar button click
	
	Called when the user clicks on the Weblink toolbar button to switch to the browser application.
	@param driverRock IN Pointer to a driver-defined data structure provided in RegisterWebDriver.
	@return None
*/
typedef ACCB1 void (ACCB2 WDBringFrontFunctionProc)(void *driverRock);

/** Callback functions for Weblink Options button click
	Called when the user clicks on the Options button in the Weblink
	Preferences dialog box. If this function is NULL, the Options button is disabled.
	Optional, Set to null if no options dialog.
	@param driverRock IN Pointer to a driver-defined data structure provided in RegisterWebDriver.
	@return None
*/
typedef ACCB1 void (ACCB2 WDOptionsFunctionProc)(void *driverRock);		

/** Callback functions to check driver transaction status 
	(Optional) Called to determine if the driver is performing a transaction or not. Weblink
	uses this to check that the driver is not busy before allowing a user to change preferences.
	@param driverRock IN Pointer to a driver-defined data structure provided in RegisterWebDriver.
	@return true if the driver is busy, false otherwise. Busy is driver-defined. The driver should
			return busy whenever it is not convenient for it to become the current driver as
			requested by \<DXRef\>WDBecomeDriverFunction.
*/
typedef ACCB1 ASBool (ACCB2 WDIsDriverBusyFunctionProc)(void *driverRock);

/**
	Tests whether or not a driver can support a given Web browser 
	application. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver. 
	@param WDAppSpecifier Platform-specific data structure 
	used to identify the Web browser application. On the Mac 
	OS, it is a pointer to an FSSpec data structure. On Windows, 
	it is a pointer to a string (char*) representing the full 
	path of the executable application.
	@return true if the driver is compatible with the given Web browser 
	application, false otherwise. 
*/
typedef WDAppSupportPredicateProc			*WDAppSupportPredicate;
/**
	Follows and retrieves the link specified by a URL from the 
	PDF document specified by theDoc. Weblink supplies the driver 
	a fully resolved the URL. The driver is solely responsible 
	for launching a Web browser, if it requires one. If the 
	driver depends on an external Web browser, the driver or 
	the Web browser is responsible for bringing that browser 
	to the foreground if the link data is not displayed by the 
	Acrobat viewer. 

	For relative links, Weblink prepends a base URL if specified; 
	otherwise, it prepends the appropriate portion from the 
	current document's URL to resolve a relative link. 

	Weblink automatically handles mapped links, that is, links 
	that generate different results based on the location of 
	the mouse within the links. (Such links are specified by 
	checking the 'Mapped Coordinates Link' box in the Edit URL 
	dialog box. These links have the isMap attribute.) Weblink 
	appends '?' to the URL, followed by the x- and y-coordinates 
	of the mouse at the time the link was selected, relative 
	to the upper-left corner of the link's rectangle. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver. 
	@param theDoc The PDF document that contains the link. 
	
	@param URL A NULL-terminated C string containing the URL 
	the link points to. 
	@param cFormData Data from a form to be sent with the 
	URL as post data. The default MIME type of form data is 
	'application/x-www-form-unencoded'.
	@return true if the driver has successfully begun the process of 
	following the link, false otherwise. 
*/
typedef WDFollowLinkFunctionProc			*WDFollowLinkFunction;
/**
	Follows and retrieves the link specified by a URL from the 
	PDF document specified by theDoc, going to a target frame. 
	Weblink supplies the driver a fully resolved the URL. The 
	driver is solely responsible for launching a Web browser, 
	if it requires one. If the driver depends on an external 
	Web browser, the driver or the Web browser is responsible 
	for bringing that browser to the foreground if the link 
	data is not displayed by the Acrobat viewer. 

	For relative links, Weblink prepends a base URL if specified; 
	otherwise, it prepends the appropriate portion from the 
	current document's URL to resolve a relative link. 

	Weblink automatically handles mapped links, that is, links 
	that generate different results based on the location of 
	the mouse within the links. (Such links are specified by 
	checking the 'Mapped Coordinates Link' box in the Edit URL 
	dialog box. These links have the isMap attribute.) Weblink 
	appends '?' to the URL, followed by the x- and y-coordinates 
	of the mouse at the time the link was selected, relative 
	to the upper-left corner of the link's rectangle. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver. 
	@param theDoc The PDF document that contains the link. 
	
	@param cURL A NULL-terminated C string containing the 
	URL the link points to. 
	@param cFormData Data from a form to be sent with the 
	URL as post data. The default MIME type of form data is 
	'application/x-www-form-unencoded'. 
	@param params Parameter block specifying a target frame.
	@return true if the driver has successfully begun the process of 
	following the link, false otherwise. 

*/
typedef WDFollowLinkWithParamsFunctionProc	*WDFollowLinkWithParamsFunction;
/**
	Called whenever the state of the driver changes from inactive 
	to current, or vice versa. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver. 
	@param bComing Tells the driver whether it should become 
	current or relinquish control. If true, the driver is being 
	requested to become the current Weblink driver, and it should 
	take any action that is appropriate, such as installing 
	IAC message handlers, opening TCP drivers, and so on. If 
	false, the driver is being asked to relinquish its status 
	as current driver and should take whatever action is necessary 
	to accomplish this.
	@return false if the driver fails to change state to that requested 
	in bComing, otherwise true. 
*/
typedef WDBecomeDriverFunctionProc			*WDBecomeDriverFunction;
/**
	Called when the user clicks on the Weblink toolbar button 
	to switch to the browser application. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver.

*/
typedef WDBringFrontFunctionProc			*WDBringFrontFunction;
/**
	(Optional) Called when the user clicks the Options button 
	in the Weblink Preferences dialog box. If this function 
	is NULL, the Options button is disabled. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver.
*/
typedef WDOptionsFunctionProc				*WDOptionsFunction;
/**
	(Optional) Called to determine if the driver is performing 
	a transaction or not. Weblink uses this to check that the 
	driver is not busy before allowing a user to change preferences. 
	
	Busy is driver-defined. The driver should return busy whenever 
	it is not convenient for it to become the current driver 
	as requested by WDBecomeDriverFunction. 
	@param driverRock Pointer to a driver-defined data structure 
	provided in RegisterWebDriver.
	@return true if the driver is busy, false otherwise. 
*/
typedef WDIsDriverBusyFunctionProc			*WDIsDriverBusyFunction;

/**	Data structure containing callbacks that implement a Weblink driver.
	@see RegisterWebDriver
*/
typedef struct {
	/** Driver version number. */
	ASUns32 WDVRev;
	/** Currently unused. */
	ASUns32 nReserved;
/** */
	WDAppSupportPredicate doYouSupport;
/** */
	WDFollowLinkFunction followLink;
/** */
	WDBecomeDriverFunction becomeDriver;
/** */
	WDBringFrontFunction bringFront;
/** */
	WDOptionsFunction options;
/** */
	WDIsDriverBusyFunction isDriverBusy;
/** */
	WDFollowLinkWithParamsFunction followLinkWithParams;
} WebDriverVector;

/* -------------------------- WebLink defines ------------------------------- */


#define WEB_LINK_HFT_LATEST_VERSION		(2L)

/** Enumerate the selectors */
enum 
{ 
	WLBAD_SELECTOR,
	RegisterWebDriverSEL,
	BeginWebProgressSEL,
	UpdateWebProgressSEL,
	EndWebProgressSEL,
	GetAppSpecifierSEL,
	WebProgressDidCancelSEL,
	WWWOpenURLSEL,
	WLCreateOrDeleteWebLinksSEL,
	WWWOpenURLWithParamsSEL,
	WLNUMSELECTORSplusOne 
};

#define WebLinkNUMSELECTORS (WLNUMSELECTORSplusOne - 1)

extern HFT gWLHFT;

 
#define WLHFTNAME "WebLink"

#define Init_gWLHFT ASExtensionMgrGetHFT( ASAtomFromString(WLHFTNAME), WEB_LINK_HFT_LATEST_VERSION);



/** Register Web Driver 
	Registers a driver with the Weblink plug-in. Must be called during the import, replace,
	and register phase of the plug-in initialization process of any plug-ins that want to use
	Weblink services.

	@param driverName IN The driver's name. A NULL-terminated C string shown to
				the user to identify the current driver. The string is limited
				to 255 characters and is copied internally.
	@param driverRock IN Pointer to a driver-defined data structure. This value is
				passed to the driver for every driver-supplied function call.
	@param wdVec IN Pointer to a data structure of driver-supplied callback
				functions. It is not copied; therefore, the structure must be
				either statically or dynamically allocated—rather than
				automatically allocated as a local variable. It must not be
				modified, moved, or deallocated until the plug-in
				terminates within Acrobat.
	@return Registration status. true if the registration was successful, false otherwise. Failure
			occurs if there is insufficient memory or the version number in wdVec is incompatible
			with the Weblink plug-in.

*/
typedef ACCB1 ASBool (ACCB2 RegisterWebDriverPROTO)(char *driverName, void *driverRock, WebDriverVector *wdVec);

typedef RegisterWebDriverPROTO* RegisterWebDriverSELPROTO;
#define RegisterWebDriver ((*((RegisterWebDriverSELPROTO)(gWLHFT[RegisterWebDriverSEL]))))



/** Creates a new Weblink progress
	Creates a new Weblink progress monitor to provide feedback during long operations.
	Current Web browsers follow links asynchronously and use BeginWebProgress,
	UpdateWebProgress, and EndWebProgress to assure a user that a request is making progress.
	
	@param message IN Message text. A NULL-terminated C string that represents a
					message to display with the progress monitor.
	@param flags IN Possible flag options for the progress monitor. There are two
					flags defined (in WLHft.h):
					PROGRESS_HAS_METER — Put a thermometer-style meter in the
					progress display.
					PROGRESS_HAS_CANCEL — Add either a Cancel button or a
					message directing the user how to abort the operation.
	@param timeoutTime IN Time Timeout period. The amount of time in seconds before the
					progress monitor indicates that progress is not being made.
	@return Monitor creation status. A non-NULL value if the monitor’s creation was successful,
			NULL otherwise. In the current implementation, there can be at most one progress monitor.
*/
typedef ACCB1 WebProgressMonitor (ACCB2 BeginWebProgressPROTO)(char *message, ASUns32 flags, ASInt32 timeoutTime);

typedef BeginWebProgressPROTO* BeginWebProgressSELPROTO;
#define BeginWebProgress ((*((BeginWebProgressSELPROTO)(gWLHFT[BeginWebProgressSEL]))))


/** Updates Web Progress
	Updates the state of the current progress monitor.

	@param	progMon IN Current progress monitor returned from BeginWebProgress.
	@param	message IN Message text. If non-NULL, the text displayed is changed to
			that represented by message. If message is NULL, the text is not changed.
	@param from IN Lower bound of a subrange between 0 and outOf to
			highlight: from and to are the lower and upper bounds of
			a subrange between 0 and outOf to highlight. Typically the
			lower bound, from, is zero, but a driver may wish to
			indicate progress of an operation (that has no magnitude
			known) as a priority. It could do so with a “chaser light”
			display by cycling the lower bound, from, through the range
			between 0 and outOf - 1, and defining the upper bound,
			to, as from + 1.
	@param	to IN Upper bound of subrange between 0 and outOf to highlight.
	@param	outOf IN If progMon was created with the PROGRESS_HAS_METER
			flag, the from, to, and outOf arguments are used to
			change the state of the bar. The range of the meter is
			defined from 0 to outOf.
	@param	timeoutTime IN (Optional) New timeout period in seconds. If non-zero, a
			new timeout is set.
	
	@return Update status. true if the update was successful, false if the monitor timed out or
		the user canceled the operation. The latter case only happens when the progress
		monitor was created with the PROGRESS_HAS_CANCEL flag in BeginWebProgress.
 
*/
typedef ACCB1 ASBool (ACCB2 UpdateWebProgressPROTO)(WebProgressMonitor progMon, char *message, ASInt32 from, ASInt32 to, ASInt32 outOf, ASInt32 timeoutTime);

typedef UpdateWebProgressPROTO* UpdateWebProgressSELPROTO;
#define UpdateWebProgress ((*((UpdateWebProgressSELPROTO)(gWLHFT[UpdateWebProgressSEL]))))


/** Ends Web Progress
	Terminates the current progress monitor.
	@param progMon IN Current progress monitor.
*/
typedef ACCB1 void (ACCB2 EndWebProgressPROTO)(WebProgressMonitor progMon);

typedef EndWebProgressPROTO* EndWebProgressSELPROTO;
#define EndWebProgress ((*((EndWebProgressSELPROTO)(gWLHFT[EndWebProgressSEL]))))


/** Get App Specifier
	Obtains a platform-specific structure specifying the application the user has chosen
	as the Web browser.

	@param promptUser IN true if there is no current Web browser and the user is
						prompted to choose one, false otherwise.
	@return Pointer to a platform-specific data structure that specifies the Web browser. On the
			Mac OS, it is a pointer to an FSSpec data structure. On Windows and UNIX platforms,
			it is a pointer to a string (char*) representing the full path of the executable
			application.
*/
typedef ACCB1 WDAppSpecifier (ACCB2 GetAppSpecifierPROTO)(ASBool promptUser);

typedef GetAppSpecifierPROTO* GetAppSpecifierSELPROTO;
#define GetAppSpecifier ((*((GetAppSpecifierSELPROTO)(gWLHFT[GetAppSpecifierSEL]))))


/** Checks if cancel button clicked
	Tests whether or not the user clicked the Cancel button in the progress monitor dialog.

	@param progMon IN Current progress monitor.
	@return true if the progress monitor has been canceled, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WebProgressDidCancelPROTO)(WebProgressMonitor progMon);

typedef WebProgressDidCancelPROTO* WebProgressDidCancelSELPROTO;
#define WebProgressDidCancel ((*((WebProgressDidCancelSELPROTO)(gWLHFT[WebProgressDidCancelSEL]))))


/** Opens a new URL
	Asks Weblink to follow the specified URL. If the URL specifies a PDF document,
	opens it in a new window (an AVDoc view).  It is used by other plug-ins that wish to start a download.

	@param avd IN If specified, the AVDoc that contains the source of the URL.
	@param cURL IN The URL to be opened.
	@param cFormData IN Data for a form to be posted. 
	@return true if the request succeeded, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WWWOpenURLPROTO)(AVDoc avd, char *cURL,	char *cFormData);

typedef WWWOpenURLPROTO* WWWOpenURLSELPROTO;
#define WWWOpenURL ((*((WWWOpenURLSELPROTO)(gWLHFT[WWWOpenURLSEL]))))

/** Creates Or Deletes WebLinks
	Used by other plug-ins that want to create or delete weblinks in a document..
	@param bCreate IN Boolean to indicate whether links should be created. If false, links will be deleted. 
	@param pdd IN is the PDDoc to create or delete weblinks in
	@param nStart IN is the starting page
	@param nEnd IN is the ending page
	@param pm IN Current progress monitor.
	@param vPMData IN is data to be passed to the progress monitor
	@param cp IN is the cancel proc
	@param vCPData IN is the data to be passed to the cancel proc 
	@return the number of web links created or deleted 
*/
typedef ACCB1 ASInt32 (ACCB2 WLCreateOrDeleteWebLinksPROTO)(ASBool bCreate, PDDoc pdd, ASInt32 nStart, ASInt32 nEnd, ProgressMonitor pm, void *vPMData, CancelProc cp, void *vCPData);

typedef WLCreateOrDeleteWebLinksPROTO* WLCreateOrDeleteWebLinksSELPROTO;
#define WLCreateOrDeleteWebLinks ((*((WLCreateOrDeleteWebLinksSELPROTO)(gWLHFT[WLCreateOrDeleteWebLinksSEL]))))


/** Opens a new URL With Parameters 
	Asks Weblink to follow a URL, specifying a target frame. If the URL specifies a PDF
	document, opens it in a new window (an AVDoc view). It is used by other plug-ins that wish to start a download, and specify target frame.
	@param avd If specified, the AVDoc that contains the source of the URL.
	@param cURL The URL to be opened.
	@param cFormData Data for a form to be posted.
	@param params Parameter block specifying name of frame.	
	@return true if the request succeeded, false otherwise.
*/
typedef ACCB1 ASBool (ACCB2 WWWOpenURLWithParamsPROTO)(AVDoc avd, char *cURL, char *cFormData, WWWOpenURLParams params);

typedef WWWOpenURLWithParamsPROTO* WWWOpenURLWithParamsSELPROTO;
#define WWWOpenURLWithParams ((*((WWWOpenURLWithParamsSELPROTO)(gWLHFT[WWWOpenURLWithParamsSEL]))))


#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif


#endif		/************** End of WLHFT.h **************/
