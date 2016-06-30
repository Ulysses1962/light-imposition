/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PICommon.h

 - Globals and function declarations used by every plug-in.

*********************************************************************/

#ifndef _H_PICommon
#define _H_PICommon

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PLUGIN
#define PLUGIN 1
#endif

#include "PIVersn.h"
#include "PIRequir.h"	/* PIRequir.h may be edited by authors to reduce Plug-in's footprint */
extern ExtensionID gExtensionID;


/* 
** Protype for PIHandshake(), which  must be provided by the plug-in author.
*/
extern ACCB1 ASBool ACCB2 PIHandshake(ASUns32 handshakeVersion, void *handshakeData);

/*
** Backwards compatibility section.
These macros reflect early API architecture. Callback-related things ended up part of the AS
mechanism. The other three macros are miscellaneous convenience macros. 
*/
#define CallbackCreate(a,b) ASCallbackCreate(b)
#define CallbackCreateProto(funcType, proc) ASCallbackCreateProto(funcType, proc)
#define CallbackCreateReplacement(sel, proc) ASCallbackCreateProto(sel##PROTO, proc)
#define CallbackCreateNotification(nsel, proc)	ASCallbackCreateProto(nsel##NPROTO, proc)
#if PI_ACROSUPPORT_VERSION >= 0x00050000
#define REPLACE(hft, sel, proc) HFTReplaceEntryEx((hft), sel, (proc), gExtensionID, HFTEntryReplaceable)
#else
#define REPLACE(hft, sel, proc) HFTReplaceEntry((hft), sel, (proc), HFTEntryReplaceable)
#endif
#define CALL_REPLACED_PROC(hft, sel, oldProc) (*((sel##PROTO)(HFTGetReplacedEntry((hft), sel, (oldProc)))))

/* assuming most people won't cast the second parameter... */
#define REGISTER_NOTIFICATION(nselName, proc, rock) AVAppRegisterNotification(nselName##NSEL, gExtensionID, (void*)(proc), (rock))

#ifdef __cplusplus
}
#endif

#endif /* _H_PICOMMON */
