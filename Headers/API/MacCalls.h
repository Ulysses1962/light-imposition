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

 MacCalls.h

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

#ifndef _H_MacCalls
#define _H_MacCalls
#pragma once
#include "acroassert.h"

/* for Adobe use only */
#define _MacintoshHFT_LATEST_VERSION 0x00020002
#define _MacintoshHFT_IS_BETA 0

/* for public use */
#define MacintoshHFT_LATEST_VERSION (_MacintoshHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _MacintoshHFT_LATEST_VERSION) : _MacintoshHFT_LATEST_VERSION)

#define MacintoshHFT_VERSION_2   0x00020000
#define MacintoshHFT_VERSION_2_2 0x00020002

#ifdef __cplusplus
extern "C" {
#endif

#include "PIRequir.h"
#if PI_MACINTOSH_VERSION != 0
#include <AppleEvents.h>
#include <PMCore.h>

extern HFT gMacintoshHFT;
extern ASUns32 gMacintoshVersion;

/* AVRectToRect */
#define AVRectToRectSEL 1

/**
	Converts an AVRect into a QuickDraw rect. 
	@param avr IN/OUT? Pointer to the AVRect to convert to a Rect. 
	
	@param rect IN/OUT? (Filled by the method) The Rect corresponding 
	to avr. 
	@exception Numerous 
	@notify Numerous 
	@see RectToAVRect 
	@since PI_MACINTOSH_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVRectToRectSELPROTO)
	(const AVRect *avr, Rect *r);
#define AVRectToRect (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2), *((AVRectToRectSELPROTO)(gMacintoshHFT[1])))

/* RectToAVRect */
#define RectToAVRectSEL 2

/**
	Converts a Quickdraw rect into an AVRect. 
	@param rect IN/OUT? Pointer to a Rect to convert to an AVRect. 
	
	@param avr IN/OUT? (Filled by the method) Pointer to the AVRect 
	corresponding to rect. 
	@see AVRectToRect 
	@since PI_MACINTOSH_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *RectToAVRectSELPROTO)
	(const Rect *r, AVRect *avr);
#define RectToAVRect (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2),  *((RectToAVRectSELPROTO)(gMacintoshHFT[2])))

/* AVAppHandleAppleEvent */
#define AVAppHandleAppleEventSEL 3

/**
	Handles Apple events that are sent to the Acrobat viewer. 
	Plug-ins that wish to handle their own Apple events do so 
	by using HFTReplaceEntry to replace this method. If your 
	replacement method does not wish to handle an Apple event 
	it receives, it must pass the Apple event along to the Acrobat 
	viewer by using the CALL_REPLACED_PROC macro. 

	For further information, see 'Application-Defined Routines' 
	in the 'Responding to Apple Events' chapter of Inside Macintosh: 
	Interapplication Communication. 

	If a plug-in wishes to get/ replace any of the four required 
	Apple events, it must use Macintosh toolbox calls such as 
	AEGetEventHandler and AESetEventHandler. 

	Plug-ins can also support AppleScript; the Acrobat viewer 
	contains an � scsz' resource, and plug-ins can handle the 
	GetAETE event to append information about scriptable events 
	they provide. 
	@param appleEvent IN/OUT? The Apple event that was sent to the 
	Acrobat viewer. 
	@param eventClass IN/OUT? The event class of appleEvent. 
	@param eventID IN/OUT? The event ID of appleEvent. 
	@param replyEvent IN/OUT? An event to send back to the application 
	that sent appleEvent. Add parameters to this event, if appropriate. 
	
	@param err IN/OUT? A value indicating whether or not the Apple 
	event was handled successfully. Return the value noErr (
	defined by Apple, not in the Acrobat SDK) if the Apple event 
	was handled successfully. See Inside Macintosh: Interapplication 
	Communication. err is the value returned by the function 
	MyEventHandler in that section. In the Acrobat core API, 
	it is returned as a parameter rather than as a function's 
	return value. 
	@since PI_MACINTOSH_VERSION >= 0x00020000
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAppHandleAppleEventSELPROTO)
	(AppleEvent *appleEvent, DescType eventClass, DescType eventId, AppleEvent *replyEvent, OSErr *err);
#define AVAppHandleAppleEvent (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2),  *((AVAppHandleAppleEventSELPROTO)(gMacintoshHFT[3])))

#include "AVExpT.h"
/* AVAppEnumSystemFonts
   Description
      Enumerates a list of fonts that are installed in the AVApp system. These are the
      fonts that are available for use by Acrobat. This call will NOT enumerate fonts 
      that have been extracted for fauxed by Acrobat.
      *** As of Acrobat 3.0, this is only available on the Macintosh. ***
   Parameters
      flags - for future expansion - must be zero
      enumProc - client provided method that gets called back with a systemFont
      clientData - client data for the enumProc
   Return value
       none
   Present in Reader
       yes
   Related methods
       none
*/

/* AVAppEnumSystemFonts */
#define AVAppEnumSystemFontsSEL 4

/**Macintosh-specific Methods 
	Enumerates a list of fonts that are installed in the system. 
	These are the fonts that are available for use by Acrobat. 
	This method does not enumerate fonts that have been extracted 
	or fauxed by Acrobat. 
	@param flags IN/OUT? For future expansion. Must be zero. 
	@param enumProc IN/OUT? Client provided callback to call for each 
	system font. 
	@param clientData IN/OUT? Client data for enumProc. 
	@see PDEnumSysFonts 
	@since PI_MACINTOSH_VERSION >= 0x00020002
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AVAppEnumSystemFontsSELPROTO)
	(ASUns32 flags, AVSystemFontEnumProc enumProc,	void *clientData);
#define AVAppEnumSystemFonts (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2_2),  *((AVAppEnumSystemFontsSELPROTO)(gMacintoshHFT[AVAppEnumSystemFontsSEL])))

/* AVAppGetPMPageFormat */
#define AVAppGetPMPageFormatSEL 5
/* 
	This function returns the Carbon Print Manager Page Format object to other clients during a print job. 

	@return PMPageFormat
	This is a Carbon Print Manager type that describes the format of a page during a print job. See Carbon Print Manager API
	documentation for more information.
	@since PI_MACINTOSH_VERSION >= 0x00060000
	@restrictions:
	The restriction on using this function is that is that it can ONLY be called after printing has started. e.g. PDDocWillPrintDoc
	notification has already been sent.
	Also, clients who use this function should not do anything with the PageFormat object that will interfere with Acrobat's
	print functionality. USE IS VERY RISKY! Please use all Acrobat API's to accomplish whatever you're trying to do before
	resorting to using this function.	
*/
typedef ACCBPROTO1 PMPageFormat	 (ACCBPROTO2 *AVAppGetPMPageFormatSELPROTO)
	(void);
#define AVAppGetPMPageFormat (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2_2),  *((AVAppGetPMPageFormatSELPROTO)(gMacintoshHFT[AVAppGetPMPageFormatSEL])))

/* AVAppGetPMPrintSettings */
#define AVAppGetPMPrintSettingsSEL 6
/* 
	This function returns the Carbon Print Manager Print Settings object to other clients during a print job. 

	@return PMPrintSettings
	This is a Carbon Print Manager type that describes the print job settings. See Carbon Print Manager API	documentation
	for more information.
	@since PI_MACINTOSH_VERSION >= 0x00060000
	@restrictions:
	The restriction on using this function is that is that it can ONLY be called after printing has started. e.g. PDDocWillPrintDoc
	notification has already been sent.
	Also, clients who use this function should not do anything with the PrintSettings object that will interfere with Acrobat's
	print functionality. USE IS VERY RISKY! Please use all Acrobat API's to accomplish whatever you're trying to do before
	resorting to using this function.	
*/
typedef ACCBPROTO1 PMPrintSettings (ACCBPROTO2 *AVAppGetPMPrintSettingsSELPROTO)
	(void);
#define AVAppGetPMPrintSettings (ACROASSERT(gMacintoshVersion >=MacintoshHFT_VERSION_2_2),  *((AVAppGetPMPrintSettingsSELPROTO)(gMacintoshHFT[AVAppGetPMPrintSettingsSEL])))


#endif /* PI_MACINTOSH_VERSION != 0 */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_MacCalls) */

