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

 DebugWindow.h

 - Catalog of functions exported by the DebugWindow plug-in.

*********************************************************************/

#ifndef _H_DEBUGWINDOWHFT
#define _H_DEBUGWINDOWHFT

enum
{
	DebugWindowBAD_SELECTOR,
	ShowDebugWindowSEL,
	HideDebugWindowSEL,
	DebugEmitStringSEL,
	GetDebugWindowSEL,
	DebugWindowFlushSEL,
	DebugWindowNUMSELECTORSPlusOne
};

extern HFT gDebugWindowHFT;

#define DebugWindowNUMSELECTORS (DebugWindowNUMSELECTORSPlusOne - 1)

#define DebugWindowHFT_NAME "ADBE:DebugWindow"
#define DebugWindowHFT_LATEST_VERSION (0x00000001)
#define InitDebugWindowHFT ASExtensionMgrGetHFT (ASAtomFromString(DebugWindowHFT_NAME), DebugWindowHFT_LATEST_VERSION)

/* ShowDebugWindow
** Shows the Debug window.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ShowDebugWindowSELPROTO)(void);
#define ShowDebugWindow (*((ShowDebugWindowSELPROTO)(gDebugWindowHFT[ShowDebugWindowSEL])))

/* HideDebugWindow
** Hides the Debug window
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *HideDebugWindowSELPROTO)(void);
#define HideDebugWindow (*((HideDebugWindowSELPROTO)(gDebugWindowHFT[HideDebugWindowSEL])))

/* DebugEmitString
** Emits a C string to the Debug window.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DebugEmitStringSELPROTO)(const char *msg);
#define DebugEmitString (*((DebugEmitStringSELPROTO)(gDebugWindowHFT[DebugEmitStringSEL])))

/* GetDebugWindow
** Gets the Debug Window's AVWindow object.
*/
typedef ACCBPROTO1 AVWindow (ACCBPROTO2 *GetDebugWindowSELPROTO)(void);
#define GetDebugWindow (*((GetDebugWindowSELPROTO)(gDebugWindowHFT[GetDebugWindowSEL])))

/* DebugWindowFlush
** Empties the contents of the Debug Window
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DebugWindowFlushSELPROTO)(void);
#define DebugWindowFlush (*((DebugWindowFlushSELPROTO)(gDebugWindowHFT[DebugWindowFlushSEL])))

#endif
