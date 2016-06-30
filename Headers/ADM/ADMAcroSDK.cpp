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

 ADMAcroSDK.cpp

  - Source code to provide convenient functions to use ADM.

*********************************************************************/
#ifndef _CPP_ADMACROSDK
#define _CPP_ADMACROSDK

#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
#include "ADMAcroSDK.h"

// global ADM suites
ADMBasicSuite7* sADMBasic = NULL;
ADMDialogSuite7* sADMDialog = NULL;
ADMItemSuite6* sADMItem = NULL;
ADMListSuite3* sADMList = NULL;
ADMEntrySuite5* sADMEntry = NULL;
ADMImageSuite2* sADMImage = NULL;
ADMIconSuite2* sADMIcon = NULL;
ADMDialogGroupSuite3 *sADMDialogGroup = NULL;
ADMDrawerSuite4* sADMDrawer = NULL;
ADMNotifierSuite2* sADMNotifier = NULL;
ADMTrackerSuite1* sADMTracker = NULL;
ADMHierarchyListSuite3* sADMHierarchyList = NULL;
ADMListEntrySuite4* sADMListEntry = NULL;

#if WIN_PLATFORM
ADMWinHostSuite4 *sWinHostSuite = NULL;
#endif

// global SP suites
SPBasicSuite *sBasicSuite = NULL;

// global plugin reference 
SPPluginRef sADMPluginRef = NULL;

StADMResourceContext *resourceContext = NULL;

#if WIN_PLATFORM
extern "C" HINSTANCE gHINSTANCE;
#endif

#define ASTRY
#define END_ASTRY

/* ADMInitialize
** ----------------------------------------------------------------------
**
** function to initialize ADM 
*/
void ADMInitialize(void)
{
	sADMPluginRef = AVSweetPeaGetPluginRef();
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMBasicSuite, kADMBasicSuiteVersion7, (const void **)&sADMBasic);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMDialogSuite, kADMDialogSuiteVersion7, (const void **)&sADMDialog);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMItemSuite, kADMItemSuiteVersion6, (const void **)&sADMItem);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMListSuite, kADMListSuiteVersion3, (const void **)&sADMList);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMEntrySuite, kADMEntrySuiteVersion5, (const void **)&sADMEntry);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMImageSuite, kADMImageSuiteVersion2, (const void **)&sADMImage);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMIconSuite, kADMIconSuiteVersion2, (const void **)&sADMIcon);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMDialogGroupSuite, kADMDialogGroupSuiteVersion3, (const void **)&sADMDialogGroup);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMDrawerSuite, kADMDrawerSuiteVersion4, (const void **)&sADMDrawer);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMNotifierSuite, kADMNotifierSuiteVersion2, (const void **)&sADMNotifier);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMTrackerSuite, kADMTrackerSuiteVersion1, (const void **)&sADMTracker);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMHierarchyListSuite, kADMHierarchyListSuiteVersion3, (const void **)&sADMHierarchyList);
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMListEntrySuite, kADMListEntrySuiteVersion4, (const void **)&sADMListEntry);
#if WIN_PLATFORM
	AVSweetPeaGetBasicSuiteP()->AcquireSuite (kADMWinHostSuite, kADMWinHostSuiteVersion4, (const void **)&sWinHostSuite);
#endif /* WIN_PLATFORM */

	// if memory was assigned, clean it to avoid memory leak 
	if (resourceContext != NULL)
	{
		delete resourceContext;
	}
	resourceContext = new StADMResourceContext();
}

/* ReleaseADM
** ----------------------------------------------------------------------
**
** function to Release ADM 
*/
void ADMRelease(void)
{
	if (resourceContext) {
		delete resourceContext;
		resourceContext = NULL;
	}

	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMBasicSuite, kADMBasicSuiteVersion7);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMDialogSuite, kADMDialogSuiteVersion7);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMItemSuite, kADMItemSuiteVersion6);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMListSuite, kADMListSuiteVersion3);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMEntrySuite, kADMEntrySuiteVersion5);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMImageSuite, kADMImageSuiteVersion2);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMIconSuite, kADMIconSuiteVersion2);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMDialogGroupSuite, kADMDialogGroupSuiteVersion3);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMDrawerSuite, kADMDrawerSuiteVersion4);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMNotifierSuite, kADMNotifierSuiteVersion2);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMTrackerSuite, kADMTrackerSuiteVersion1);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMHierarchyListSuite, kADMHierarchyListSuiteVersion3);
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMListEntrySuite, kADMListEntrySuiteVersion4);
#if WIN_PLATFORM
	AVSweetPeaGetBasicSuiteP()->ReleaseSuite (kADMWinHostSuite, kADMWinHostSuiteVersion4);
#endif /* WIN_PLATFORM */
}


/* ADMIsAvailable
** ----------------------------------------------------------------------
**
** function to check ADM 
*/
ASBoolean ADMIsAvailable(void )
{
	/* if the HFT is still null after we try to get it, ADM can't be available... return false */
	if( gAcroViewSweetPeaHFT == NULL )
		return false;

	/* the HFT is available, now return the result from the AV call... */
	return AVSweetPeaIsADMAvailable();
}


/***********************************************************************
 *	ADMUtils Class static functions
 ***********************************************************************/
void ADMUtils::InitializeADM()
{
	ADMInitialize();
}

void ADMUtils::ReleaseADM()
{
	ADMRelease();
}

ASBoolean ADMUtils::IsADMAvailable()
{
	return ADMIsAvailable();
}

/***********************************************************************
 *	StADMResourceContext
 ***********************************************************************/
StADMResourceContext::StADMResourceContext(AVDoc activeDoc, SPPlatformAccessRef accessRef)
{
#if MAC_PLATFORM
	if (accessRef)
		mAcroResourceContext = new StAcroResourceContext(accessRef->fileRefNum);
	else
		mAcroResourceContext = new StAcroResourceContext();
#endif

	AVSweetPeaGetResourceAccess(AVSweetPeaGetPluginRef(), &mOldAccessRef);

	#if MAC_PLATFORM
	#pragma unused(activeDoc)

	if(!accessRef)
	{
		mMacResChain.fileRefNum = CurResFile();
		mAccessRef = &mMacResChain;
	}
	else
		mAccessRef = accessRef;
	#endif /* MAC_PLATFORM */

	#if WIN_PLATFORM
		mOldWindowRef = NULL;
		if (sWinHostSuite)
		{
			mOldWindowRef = sWinHostSuite->GetPlatformAppWindow();
			sWinHostSuite->SetPlatformAppWindow((ASWindowRef)WinAppGetModalParent(activeDoc ? activeDoc : AVAppGetActiveDoc()));
		}

		if(!accessRef)
		{
			mAccessRef = (SPPlatformAccessRef)gHINSTANCE;
		}
		else
			mAccessRef = accessRef;
	#endif /* WIN_PLATFORM */

	AVSweetPeaSetResourceAccess(AVSweetPeaGetPluginRef(), mAccessRef);
}

StADMResourceContext::~StADMResourceContext()
{
	AVSweetPeaSetResourceAccess(AVSweetPeaGetPluginRef(), mOldAccessRef);

#if WIN_PLATFORM
	if (sWinHostSuite)
		sWinHostSuite->SetPlatformAppWindow(mOldWindowRef);
#endif /* WIN_PLATFORM */

#if MAC_PLATFORM
	if (mAcroResourceContext)
		delete mAcroResourceContext;
#endif
}

#endif