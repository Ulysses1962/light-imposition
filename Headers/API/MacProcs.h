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

 MacProcs.h

 - Catalog of functions exported by the Mac Viewer.

*********************************************************************/


/**
	Converts an AVRect into a QuickDraw rect. 
	@param avr IN/OUT? Pointer to the AVRect to convert to a Rect. 
	
	@param r IN/OUT? (Filled by the method) The Rect corresponding 
	to avr. 
	@exception Numerous 
	@notify Numerous 
	@see RectToAVRect 
	@since PI_MACINTOSH_VERSION >= 0x00020000

*/
NPROC(void, AVRectToRect, (const AVRect *avr, Rect *r))

/**
	Converts a Quickdraw Rect into an AVRect. 
	@param r Pointer to a Rect to convert to an AVRect. 
	
	@param avr (Filled by the method) Pointer to the AVRect 
	corresponding to rect.
	@see AVRectToRect 
	@since PI_MACINTOSH_VERSION >= 0x00020000
*/
NPROC(void, RectToAVRect, (const Rect *r, AVRect *avr))
#if HAS_APPLE_EVENTS

/**
	Handles Apple events that are sent to the Acrobat viewer. 
	Clients that wish to handle their own Apple events do so 
	by using HFTReplaceEntry to replace this method. If your 
	replacement method does not wish to handle an Apple event 
	it receives, it must pass the Apple event along to the Acrobat 
	viewer by using the CALL_REPLACED_PROC macro. 

	For further information, see 'Application-Defined Routines' 
	in the 'Responding to Apple Events' chapter of Inside Macintosh: 
	Interapplication Communication. 

	If a client wishes to get/replace any of the four required 
	Apple events, it must use Macintosh toolbox calls such as 
	AEGetEventHandler and AESetEventHandler. 

	Clients can also support AppleScript; the Acrobat viewer 
	contains an 'scsz' resource, and clients can handle the 
	GetAETE event to append information about scriptable events 
	they provide. 
	@param appleEvent The Apple event that was sent to the 
	Acrobat viewer. 
	@param eventClass The event class of appleEvent. 
	@param eventId The event ID of appleEvent. 
	@param replyEvent An event to send back to the application 
	that sent appleEvent. Add parameters to this event, if appropriate. 
	
	@param err A value indicating whether the Apple event 
	was handled successfully. Return the value noErr (defined 
	by Apple, not in the Acrobat SDK) if the Apple event was 
	handled successfully. See Inside Macintosh: Interapplication 
	Communication. err is the value returned by the function 
	MyEventHandler in that section. In the Acrobat core API, 
	it is returned as a parameter rather than as a function's 
	return value.
	@since PI_MACINTOSH_VERSION >= 0x00020000
*/
PROC(void, AVAppHandleAppleEvent, (AppleEvent *appleEvent, DescType eventClass, DescType eventId, AppleEvent *replyEvent, OSErr *err))
#endif

/**
	Enumerates a list of fonts that are installed in the system. 
	These are the fonts that are available for use by Acrobat. 
	This method does not enumerate fonts that have been extracted 
	or fauxed by Acrobat. 
	@param flags For future expansion. Must be zero. 
	@param enumProc Client provided callback to call for each 
	system font. 
	@param clientData Client data for enumProc.
	@see PDEnumSysFonts 
	@since PI_MACINTOSH_VERSION >= 0x00020002
*/
NPROC(void, AVAppEnumSystemFonts, (Uns32 flags,	AVSystemFontEnumProc enumProc, void *clientData))

/**
	This function returns the Carbon Print Manager Page Format object to other clients during a print job.\n
	Restrictions:
	The restriction on using this function is that is that it can ONLY be called after printing has started. e.g. PDDocWillPrintDoc
	notification has already been sent.
	Also, clients who use this function should not do anything with the PageFormat object that will interfere with Acrobat's
	print functionality. USE IS VERY RISKY! Please use all Acrobat API's to accomplish whatever you're trying to do before
	resorting to using this function.
	@return PMPageFormat
	This is a Carbon Print Manager type that describes the format of a page during a print job. See Carbon Print Manager API
	documentation for more information.
	@since PI_MACINTOSH_VERSION >= 0x00060000
*/
NPROC(PMPageFormat, AVAppGetPMPageFormat, (void))

/**
	This function returns the Carbon Print Manager Print Settings object to other clients during a print job.\n
	Restrictions:
	The restriction on using this function is that is that it can ONLY be called after printing has started. e.g. PDDocWillPrintDoc
	notification has already been sent.
	Also, clients who use this function should not do anything with the PrintSettings object that will interfere with Acrobat's
	print functionality. USE IS VERY RISKY! Please use all Acrobat API's to accomplish whatever you're trying to do before
	resorting to using this function.
	@return PMPrintSettings
	This is a Carbon Print Manager type that describes the print job settings. See Carbon Print Manager API	documentation
	for more information.
	@since PI_MACINTOSH_VERSION >= 0x00060000
*/
NPROC(PMPrintSettings, AVAppGetPMPrintSettings, (void))
