/***********************************************************************/
/*                                                                     */
/* Copyright 1996-2003 Adobe Systems Incorporated.                     */
/* All Rights Reserved.                                                */
/*                                                                     */
/* Patents Pending                                                     */
/*                                                                     */
/* NOTICE: All information contained herein is the property of Adobe   */
/* Systems Incorporated. Many of the intellectual and technical        */
/* concepts contained herein are proprietary to Adobe, are protected   */
/* as trade secrets, and are made available only to Adobe licensees    */
/* for their internal use. Any reproduction or dissemination of this   */
/* software is strictly forbidden unless prior written permission is   */
/* obtained from Adobe.                                                */
/*                                                                     */
/***********************************************************************/



#ifndef __ADMNotifier__

#define __ADMNotifier__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN



// Mac OS headers #define GetItem, but we want to use it as a name.

#ifdef GetItem

#undef GetItem

#endif





// -----------------------------------------------------------------------------

//	Notifier type constants



#define kADMUserChangedNotifier "ADM User Changed Notifier"

#define kADMIntermediateChangedNotifier "ADM Intermediate Changed Notifier"

#define kADMBoundsChangedNotifier "ADM Bounds Changed Notifier"

#define kADMEntryTextChangedNotifier "ADM Entry Text Changed Notifier"

#define kADMCloseHitNotifier "ADM Close Hit Notifier"

#define kADMZoomHitNotifier "ADM Zoom Hit Notifier"

#define kADMCycleNotifier "ADM Cycle Notifier"

#define kADMCollapseNotifier "ADM Collapse Notifier"

#define kADMExpandNotifier "ADM Expand Notifier"

#define kADMContextMenuChangedNotifier "ADM Context Menu Changed Notifier"

#define kADMWindowShowNotifier "ADM Show Window Notifier"

#define kADMWindowHideNotifier "ADM Hide Window Notifier"

#define kADMGroupShowNotifier "ADM Show Group Notifier"

#define kADMGroupHideNotifier "ADM Hide Group Notifier"

#define kADMWindowActivateNotifier "ADM Activate Window Notifier"

#define	kADMWindowDeactivateNotifier "ADM Deactivate Window Notifier"

#define kADMNumberOutOfBoundsNotifier "ADM Number Out Of Bounds Notifier"

#define kADMWindowDragMovedNotifier "ADM Window Moved By Drag"



	// text item notifiers

#define	kADMPreClipboardCutNotifier "ADM Pre Clipboard Cut Notifier"

#define	kADMPostClipboardCutNotifier "ADM Post Clipboard Cut Notifier"

#define	kADMPreClipboardCopyNotifier "ADM Pre Clipboard Copy Notifier"

#define	kADMPostClipboardCopyNotifier "ADM Post Clipboard Copy Notifier"

#define	kADMPreClipboardPasteNotifier "ADM Pre Clipboard Paste Notifier"

#define	kADMPostClipboardPasteNotifier "ADM Post Clipboard Paste Notifier"

#define	kADMPreClipboardClearNotifier "ADM Pre Clipboard Clear Notifier"

#define	kADMPostClipboardClearNotifier "ADM Post Clipboard Clear Notifier"

#define kADMPreTextSelectionChangedNotifier "ADM Pre Selection Change Notification"

#define kADMTextSelectionChangedNotifier "ADM Text Selection Change Notification"



// [cpaduan] 12/04/00: new notifiers for undo/redo

#define	kADMPreClipboardRedoNotifier "ADM Pre Clipboard Redo Notifier"

#define	kADMPostClipboardRedoNotifier "ADM Post Clipboard Redo Notifier"

#define	kADMPreClipboardUndoNotifier "ADM Pre Clipboard Undo Notifier"

#define	kADMPostClipboardUndoNotifier "ADM Post Clipboard Undo Notifier"





// =============================================================================

//		* ADM Notifier Suite

// =============================================================================



#define kADMNotifierSuite "ADM Notifier Suite"

#define kADMNotifierSuiteVersion2 2



// -----------------------------------------------------------------------------



typedef struct ADMNotifierSuite2

{



	// *** This suite is FROZEN.

	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.



	// notifier context



	ADMItemRef ASAPI (*GetItem)(ADMNotifierRef inNotifier);

	ADMDialogRef ASAPI (*GetDialog)(ADMNotifierRef inNotifier);



	// notifier type

	

	ASBoolean ASAPI (*IsNotifierType)(ADMNotifierRef inNotifier, const char* inNotifierType);

	void ASAPI (*GetNotifierType)(ADMNotifierRef inNotifier, char* outNotifierType,

				ASUInt32 inMaxLength);



	// notifier flags



	void ASAPI (*SkipNextClipboardOperation)(ADMNotifierRef inNotifier, ASBoolean inSkip);

		

}

ADMNotifierSuite2;


// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



// -----------------------------------------------------------------------------



#endif	// __ADMNotifier__

