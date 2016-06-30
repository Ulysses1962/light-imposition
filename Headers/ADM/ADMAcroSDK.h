/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ADMAcroSDk.h

 - Utility file for SDK developers to use ADM. 
 
*********************************************************************/

#ifndef _H_ADMACROSDK
#define _H_ADMACROSDK

#include "Environ.h"
#include "ASExtraCalls.h"
#include "AVExpT.h"
#include "AVSPCalls.h"

#include "ADMBasic.h"
#include "ADMDialog.h"
#include "ADMDialogGroup.h"
#include "ADMDrawer.h"
#include "ADMEntry.h"
#include "ADMHierarchyList.h"
#include "ADMIcon.h"
#include "ADMImage.h"
#include "ADMItem.h"
#include "ADMList.h"
#include "ADMListEntry.h"
#include "ADMNotifier.h"
#include "ADMTracker.h"
#include "ADMHost.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ADMBasicSuite7	 		*sADMBasic;
extern ADMDialogSuite7	 		*sADMDialog;
extern ADMDialogGroupSuite3 	*sADMDialogGroup;
extern ADMDrawerSuite4	 		*sADMDrawer;
extern ADMEntrySuite5	 		*sADMEntry;
extern ADMHierarchyListSuite3	*sADMHierarchyList;
extern ADMIconSuite2			*sADMIcon;
extern ADMImageSuite2			*sADMImage;
extern ADMItemSuite6	 		*sADMItem;
extern ADMListSuite3	 		*sADMList;
extern ADMListEntrySuite4		*sADMListEntry;
extern ADMNotifierSuite2 		*sADMNotifier;
extern ADMTrackerSuite1	 		*sADMTracker;
#if WIN_PLATFORM
extern ADMWinHostSuite4 *sWinHostSuite;
#endif

#ifdef __cplusplus
}
#endif

class StADMResourceContext;
extern StADMResourceContext *resourceContext;

extern SPBasicSuite *sBasicSuite;
extern SPPluginRef sADMPluginRef;

// Unicode Types
typedef unsigned short ADMUInt16;
typedef ADMUInt16 ADMUnicode;
// ADMBoolean is the same a Macintosh Boolean.
typedef unsigned char ADMBoolean; 

#if MAC_PLATFORM
#include "SafeResources.h"
#endif /* MAC_PLATFORM */

// globat functions used by Acrobat SDK plug-ins
void ADMInitialize(void);
void ADMRelease(void);
ASBoolean ADMIsAvailable (void);

// ADM utility class 
class ADMUtils
{
public:
	static void InitializeADM();
	static void ReleaseADM();
	static ASBoolean IsADMAvailable ();
};

// utility class 
class StADMResourceContext
{
public:

	StADMResourceContext (AVDoc activeDoc = NULL, SPPlatformAccessRef accessRef = NULL);
	virtual ~StADMResourceContext();

protected:
	SPPlatformAccessRef		mAccessRef;				// access ref to be used for this dialog session.
	SPPlatformAccessRef		mOldAccessRef;			// access ref saved prior to this dialog session.

#if MAC_PLATFORM
	StAcroResourceContext	*mAcroResourceContext;	// PlugIn resource context when accessRef == NULL.
	SPMacResChain			mMacResChain;			// Macintosh resource chain information.
#elif WIN_PLATFORM
	ASWindowRef				mOldWindowRef;
	ASInt32					mOldResource;
#endif
};


#endif