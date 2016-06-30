/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 SafeResources.cpp

 - Glue implementation of "safe" version of Macintosh toolbox routines
   that put the plug-in's resource file on top of the resource chain
   before calling the "real" implementation of the trap. Note that all
   routines also save and restore the current resource file.

*********************************************************************/

#include <Resources.h>
#include <TextUtils.h>
#include <ToolUtils.h>
#include <Files.h>
#include <String.h>
#include <LowMem.h>
#include <Gestalt.h>
#include <string.h>

#include "PICommon.h"
#include "SafeResources.h"

#if TARGET_API_MAC_CARBON
extern CFBundleRef gPluginBundle;
#endif /* TARGET_API_MAC_CARBON */
extern short gResFile;
short gLocaleResFile = 0;
static ASInt16 GetResRefNumFromFCB(ASInt16 vRefNum, ASInt32	dirID, ConstStr255Param fileName);
static Boolean SearchDirectory( UInt16 volID, UInt32 dirID, UInt8 * targetName, OSType signature, UInt32 depth );

#pragma options align=mac68k
struct LanguageDescriptor
{
	OSType				mSignature;			/* distinguish between Acrobat application kinds */
	OSType				mLocale3Letters;	/* 'FRA ', 'ITA ', ... */
	NumVersionVariant	mVersion;			/* a copy of the 'vers' 1 version field */
	short				mLangCode;			/* the language code */
	short				mScriptCode;		/* the script code */
	short				mRegionCode;		/* the region code (same as in 'vers' resource) */
/*	Str255				mEnglishName;	*/	/* same in english */
/*	Str255				mLocaleName; 	*/	/* The localized language name */

//	short				Compare(const LanguageDescriptor& inOther) const; /* Not needed */
};
#pragma options align=reset

/****************************************************************************

	SetupResourceChain
		
		Makes sure that the locale and plugin files are on the top of the
		resource chain.
		
****************************************************************************/
		
#if TARGET_API_MAC_OS8
short PreCarbonSetupResourceChain()
{
	short oldResFile = 0;		// Save the current res file and return
	oldResFile = CurResFile();		// Save the current res file and return
											// to caller
	UseResFile( gResFile );					// Push plugin file onto resource chain
	if( gLocaleResFile )					// If there is a locale file, make sure
		UseResFile( gLocaleResFile );		// it is above the plugin on the chain.
		
	return oldResFile;		
}
#endif /* TARGET_API_MAC_OS8 */

pascal Handle SafeGet1Resource(ResType type, short resNum)
{
#if TARGET_API_MAC_CARBON
	Handle result = NULL;
	
	if (gLocaleResFile)
	{
		StAcroResourceContext resContext(gLocaleResFile);
		result = Get1Resource(type, resNum);
	}
	if (result == NULL && gResFile)
	{
		StAcroResourceContext resContext(gResFile);
		result = Get1Resource(type, resNum);
	}
	return result;
#else

	Handle result;
	short oldResFile = CurResFile();

	if( gLocaleResFile ) {					// Look in the Locale file first

		UseResFile( gLocaleResFile );
		result = Get1Resource(type, resNum);
		if( result ) {						// If the resource was there, then
			UseResFile( oldResFile );		// restore the chain and exit
			return result;
		}
	}

	UseResFile( gResFile );					// Default to base resource file
	result = Get1Resource(type, resNum);	// and get resource.

	UseResFile(oldResFile);					// restore and return
	return result;
#endif
}

pascal Handle SafeGet1NamedResource(ResType type, ConstStr255Param name)
{
#if TARGET_API_MAC_CARBON
	Handle result = NULL;
	
	if (gLocaleResFile)
	{
		StAcroResourceContext resContext(gLocaleResFile);
		result = Get1NamedResource(type, name);
	}
	if (result == NULL && gResFile)
	{
		StAcroResourceContext resContext(gResFile);
		result = Get1NamedResource(type, name);
	}
	return result;
#else
	Handle result;
	short oldResFile = CurResFile();

	if( gLocaleResFile ) {					// Look in the Locale file first

		UseResFile( gLocaleResFile );
		result = Get1NamedResource(type, name);
		if( result ) {						// If the resource was there, then
			UseResFile( oldResFile );		// restore the chain and exit
			return result;
		}
	}

	UseResFile( gResFile );					// Default to base resource file
	result = Get1NamedResource(type, name);	// and get resource

	UseResFile(oldResFile);					// Restore and exit
	return result;
#endif
}

pascal Handle SafeGetResource(ResType type, short resNum)
{
	StAcroResourceContext resContext;
	
	return GetResource(type, resNum);
}

pascal void SafeGetIndString(Str255 str, short strListID, short index)
{
	StAcroResourceContext resContext;

	GetIndString(str, strListID, index);
}

pascal StringHandle SafeGetString(short strID)
{
	StAcroResourceContext resContext;

	return GetString(strID);
}

pascal WindowRef SafeGetNewWindow(short windowID, void *wStorage, WindowRef behind)
{
	StAcroResourceContext resContext;

	return GetNewWindow(windowID, wStorage, behind);
}

pascal WindowRef SafeGetNewCWindow(short windowID, void *wStorage, WindowRef behind)
{
	StAcroResourceContext resContext;

	return GetNewCWindow(windowID, wStorage, behind);
}

pascal DialogRef SafeGetNewDialog(short dialogID, void *dStorage, WindowRef behind)
{
	StAcroResourceContext resContext;
	
	return GetNewDialog(dialogID, dStorage, behind);
}

pascal short SafeAlert(short alertID, ModalFilterUPP modalFilter)
{
	StAcroResourceContext resContext;

	return Alert(alertID, modalFilter);
}

pascal short SafeStopAlert(short alertID, ModalFilterUPP modalFilter)
{
	StAcroResourceContext resContext;

	return StopAlert(alertID, modalFilter);
}

pascal short SafeNoteAlert(short alertID, ModalFilterUPP modalFilter)
{
	StAcroResourceContext resContext;

	return NoteAlert(alertID, modalFilter);
}

pascal short SafeCautionAlert(short alertID, ModalFilterUPP modalFilter)
{
	StAcroResourceContext resContext;

	return CautionAlert(alertID, modalFilter);
}

pascal ControlHandle SafeGetNewControl(short ctrlID, WindowPtr window)
{
	StAcroResourceContext resContext;

	return GetNewControl(ctrlID, window);
}

pascal CursHandle SafeGetCursor(short cursID)
{
	StAcroResourceContext resContext;
	
	// Under OS X, doing a GetCursor() has the side-effect of opening up QD.rsrc in
	// the OS -- so that it can provide the OS cursors just if we ask for them.  In most
	// cases we don't want 'em and don't want to take the performance hit (which is
	// pretty high).  Our solution is to check the app and locale contexts for the
	// cursors, and then, only if we fail, ask QD for them.  Hopefully the
	// latter will never happen, but we have to do this just in case 3rd parties
	// depend on that behavior.
	CursHandle	cursor = (CursHandle) GetResource('CURS', cursID);
	if (cursor == NULL)
		cursor = GetCursor(cursID);
	
	return cursor;
}

pascal Handle SafeGetIcon(short iconID)
{
	StAcroResourceContext resContext;

	return GetIcon(iconID);
}

static pascal OSErr AddIconsToSuite(ResType theType, Handle *theIcon, void *yourDataPtr)
{
	IconSuiteRef iconSuite = (IconSuiteRef)yourDataPtr;
	
	AddIconToSuite(*theIcon, iconSuite, theType);
	
	return noErr;
}

static ASBool SafeGetIconSuitePart(IconSuiteRef theIconSuite, short theResID, ResType theType)
{
	ASBool foundAnIcon = false;
	
	Handle h = SafeGet1Resource(theType, theResID);
	if (h)
	{
		DetachResource(h);
		
		AddIconToSuite(h, theIconSuite, theType);
		
		foundAnIcon = true;
	}
	
	return foundAnIcon;
}

pascal OSErr SafeGetIconSuite(IconSuiteRef *theIconSuite, short theResID, IconSelectorValue selector)
{
	ASBool foundAnIcon = false;
	
	*theIconSuite = NULL;
	OSErr err = noErr;
	
	err = NewIconSuite(theIconSuite);
	if (err == noErr)
	{
		if (selector & kSelectorLarge1Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'ICN#'))
				foundAnIcon = true;
		
		if (selector & kSelectorLarge4Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'icl4'))
				foundAnIcon = true;
		
		if (selector & kSelectorLarge8Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'icl8'))
				foundAnIcon = true;
		
		if (selector & kSelectorSmall1Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'ics#'))
				foundAnIcon = true;
		
		if (selector & kSelectorSmall4Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'ics4'))
				foundAnIcon = true;
		
		if (selector & kSelectorSmall8Bit)
			if (SafeGetIconSuitePart(*theIconSuite, theResID, 'ics8'))
				foundAnIcon = true;
		
		if (IconFamilyToIconSuite != NULL)
		{
			Handle iconFamily = SafeGet1Resource('icns', theResID);
			if (iconFamily)
			{
				IconSuiteRef icnsIconSuite = NULL;
				IconFamilyToIconSuite((IconFamilyHandle)iconFamily, selector, &icnsIconSuite);
				if (icnsIconSuite)
				{
					IconActionUPP AddIconsToSuiteUPP = NewIconActionUPP(AddIconsToSuite);
					ForEachIconDo(icnsIconSuite, selector, AddIconsToSuiteUPP, *theIconSuite);
					DisposeIconActionUPP(AddIconsToSuiteUPP);
					foundAnIcon = true;
				}
				
				ReleaseResource(iconFamily);
			}
		}
		
		if (!foundAnIcon)
		{
			DisposeIconSuite(*theIconSuite, true);
			*theIconSuite = NULL;
			err = noSuchIconErr;
		}
	}
	
	return err;
}

pascal CIconHandle SafeGetCIcon(short iconID)
{
	StAcroResourceContext resContext;

	return GetCIcon(iconID);
}

pascal PicHandle SafeGetPicture(short picID)
{
	StAcroResourceContext resContext;

	return GetPicture(picID);
}

pascal PatHandle SafeGetPattern(short patternID)
{
	StAcroResourceContext resContext;

	return GetPattern(patternID);
}

pascal void SafeGetIndPattern(Pattern *thePat, short patternListID, short index)
{
	StAcroResourceContext resContext;
	
	GetIndPattern(thePat, patternListID, index);
}

pascal MenuHandle SafeGetMenu(short resourceID)
{	
	StAcroResourceContext resContext;

	return GetMenu(resourceID);
}

pascal void SafeModalDialog(ModalFilterUPP modalFilter, short *itemHit)
{
	StAcroResourceContext resContext;
	
	ModalDialog(modalFilter, itemHit);
}

/****************************************************************************

	SetLocalFile
	
		While we still support the SetLocaleFile mechansim,
		we suggest you use Mac OS X's localization support
		and bundle your plugin instead.
		
		Scans the plugin folder for the specified locale file.  The locale file
		name must be the concatinatin of the plugin name and the language string
		returned by AVAppGetLanguage, i.e. ExportPS FRA.  If Debug is on, it
		attempts to remove ".Debug" from the plugin name.
		
		Input:
			Language String (c string)
			
		Output:
			gLocaleResFile set to locale fileref if found, otherwise it is zero

/****************************************************************************/

void SetLocaleFile( UInt8 * langStr, OSType signature )
{
	FCBPBRec	resPb;
	Str31		fileName;
	OSErr		err;
	
	resPb.ioCompletion 	= nil;			// Find the plugin directory using
	resPb.ioNamePtr		= fileName;		// the res file ref num as a base.
	resPb.ioRefNum		= gResFile;
	resPb.ioFCBIndx		= 0;			// Look up gResFile
	
	err = PBGetFCBInfoSync( &resPb );
	
	if( err != noErr ) {				// Something bad happened.  Set locale
		gLocaleResFile = 0;				// to nil and default to English
		return;
	}
	SearchDirectory(  resPb.ioFCBVRefNum, resPb.ioFCBParID, langStr, signature, 1 );
}

/****************************************************************************

	SearchDirectory
		
		Recursively searches a plugin directory for file targetName.  It is limited
		to a single level search, since all locale files should be in plugin folder.
		
****************************************************************************/
	
static Boolean SearchDirectory( UInt16 volID, UInt32 dirID, UInt8 * targetLang, OSType sig, UInt32 depth )
{	
	Str31		fileName;
	UInt16		fileIndex 	= 1;
	OSErr		err 		= noErr;
	CInfoPBRec	catPb;
	Boolean		found 		= false;
	struct LanguageDescriptor ** locaRSRC;
	
	gLocaleResFile = 0;		/* Set locale file to invalid number	*/
		
	while( err == noErr ) {
		catPb.dirInfo.ioCompletion 	= nil;
		catPb.dirInfo.ioNamePtr		= fileName;
		catPb.dirInfo.ioVRefNum		= volID;
		catPb.dirInfo.ioFDirIndex	= fileIndex++;
		catPb.dirInfo.ioDrDirID		= dirID;
		
		err = PBGetCatInfoSync( &catPb );
	
		// If I found a folder, then look inside.  But only go to the specified depth
		if( err == noErr) {			
			if( catPb.dirInfo.ioFlAttrib & (1<<4) ) {	// Look in the folder, if I haven't exceeded the depth
				
				if( depth ) {
					if( SearchDirectory( volID, catPb.dirInfo.ioDrDirID, targetLang, sig, depth - 1 ) )
						return true;		/* A locale file was found, so exit	*/
					else		
						continue;			/* Nothing so far, so keep looking	*/
				}
				else {
					continue;
				}
			}
			else {	// Check if its a locale file
				if( 	catPb.hFileInfo.ioFlFndrInfo.fdType 	== 'LANG'
					&&	catPb.hFileInfo.ioFlFndrInfo.fdCreator 	== 'CARO' ) {
						ASInt16 curLocaleFile;

						/* Fix for 408144.
						** Check for an open resource file.  Skip already open files.
						** If another Plugin has it open this will cause great confusion.
						** Example is "WHA" and "WHA Library".  "WHA Library" close the "WHA lang"
						** file while looking for the "WHA Library lang" file.
						*/
						curLocaleFile = GetResRefNumFromFCB(volID, dirID, fileName);
						if( curLocaleFile != -1  )
							continue;				/* continue on to the next file					*/

						curLocaleFile = HOpenResFile( volID, dirID, fileName, fsCurPerm );

						if( curLocaleFile == -1  )	/* Check for a valid file.  If its bad, then	*/
							continue;				/* continue on to the next file					*/
							
						locaRSRC = (struct LanguageDescriptor **) Get1Resource( 'LOCA', 2010 );
						if( locaRSRC ) {

							if( (*locaRSRC)->mSignature == sig ) {	/* Check the client signature	*/
								
								if( (*locaRSRC)->mLocale3Letters == *( UInt32 * )targetLang )
								{
									gLocaleResFile = curLocaleFile;
								}
							}
						
							ReleaseResource( ( Handle )locaRSRC );
						}
						
						if( gLocaleResFile != curLocaleFile ) {
							CloseResFile( curLocaleFile );	/* Release the locale file	*/
							continue;						/* Didn't find it, so try the next one	*/
						}
						else
							return true;				/* Got it, so exit	*/
				}
			}
		}
	}
	return false;	/* Found nothing	*/
}


/*
**	GetResRefNumFromFCB
**
**	Search open files for this resource file and return the file reference number if found.
**	Otherwise return -1.
**
*/	
static ASInt16 GetResRefNumFromFCB(ASInt16 vRefNum, ASInt32	dirID, ConstStr255Param fileName)
{
	StrFileName	fcbFileName;
	FCBPBRec	fcbPB;
	ASInt16		ioFCBIndx;
	ASInt16		plugResFile = -1;
	OSErr		err = noErr;

	/* search for open plugin resource file reference number */
/*	miMemset( &fcbPB, 0, sizeof(fcbPB) );	/* can not use miUtils in pdfviewer */
	std::memset( &fcbPB, 0, sizeof(fcbPB) );
	fcbPB.ioNamePtr = fcbFileName;
	fcbPB.ioVRefNum = vRefNum;
	for (ioFCBIndx = 1; err == noErr && plugResFile == -1; ioFCBIndx++) {
		fcbPB.ioFCBIndx = ioFCBIndx;
		err = PBGetFCBInfoSync(&fcbPB);
		if (err == noErr							&&
			fcbPB.ioFCBParID == dirID				&&
			fcbPB.ioFCBFlags & kioFCBResourceMask	&&
			EqualString(fcbFileName, fileName, true, true)	)
		{
			plugResFile = fcbPB.ioRefNum;
		}
	}

	return plugResFile;

}	/* GetResRefNumFromFCB */


StAcroResourceContext::StAcroResourceContext()
	: mCurResFile(gResFile)
{
#if TARGET_API_MAC_CARBON
	mOldResFile = CurResFile();
	
	if( mCurResFile )
		UseResFile(mCurResFile);
	
	if( gLocaleResFile )
		UseResFile(gLocaleResFile);
		
#else

	mOldResFile = PreCarbonSetupResourceChain( );
	
#endif
}

StAcroResourceContext::StAcroResourceContext(short resFile)
	: mCurResFile(resFile)
{
#if TARGET_API_MAC_CARBON
	mOldResFile = CurResFile();

	if( mCurResFile )
		UseResFile(mCurResFile);
			
#else

	mOldResFile = PreCarbonSetupResourceChain( );
	
#endif
}

StAcroResourceContext::~StAcroResourceContext( )
{
	::UseResFile( mOldResFile );
}

extern "C" OSErr	__initialize( const CFragInitBlock *	initBlock );
extern "C" void		__terminate( void );

#if !TARGET_RT_MAC_MACHO
OSErr AcroPluginCFragInitFunction( const CFragInitBlock *	initBlock )
{
	OSErr err = __initialize(initBlock);
	
	if (err == noErr)
	{	
#if TARGET_API_MAC_CARBON
		if (initBlock->fragLocator.where == kDataForkCFragLocator)
		{
			// Mac OS X 10.1 and earlier has a bug where packaged CFM libs are still passed 
			// kDataForkCFragLocator instead of kCFBundleCFragLocator. Apple claims this will
			// fixed in Jaguar.
			FSSpec spec = *initBlock->fragLocator.u.onDisk.fileSpec;
			
			// See if parent folder is named "MacOS"
			FSMakeFSSpec(spec.vRefNum, spec.parID, "\p", &spec);
			if (IdenticalString(spec.name, "\pMacOS", NULL) == 0)
			{
				// See if parent folder is named "Contents"
				FSMakeFSSpec(spec.vRefNum, spec.parID, "\p", &spec);
				if (IdenticalString(spec.name, "\pContents", NULL) == 0)
				{
					// Get Bundle Ref
					FSRef fsRef;
					FSMakeFSSpec(spec.vRefNum, spec.parID, "\p", &spec);
					if (noErr == FSpMakeFSRef(&spec, &fsRef))
					{
						CFURLRef cfURL = CFURLCreateFromFSRef(NULL, &fsRef);
						if (cfURL)
						{
							gPluginBundle = CFBundleCreate(NULL, cfURL);
#if DEBUG
							CFShow(cfURL);
							CFShow(gPluginBundle);
#endif
							
							if (gPluginBundle)
								gResFile = CFBundleOpenBundleResourceMap(gPluginBundle);
							
							CFRelease(cfURL);
						}
					}
				}
			}
		}
		else if (initBlock->fragLocator.where == kCFBundleCFragLocator)
		{
			gPluginBundle = initBlock->fragLocator.u.inBundle.fragmentBundle;
		}
		
#endif /* TARGET_API_MAC_CARBON */
        }
	
	return err;
}

void AcroPluginCFragTermProcedure( void )
{
	__terminate();
}
#endif // TARGET_RT_MAC_MACHO

