/*
** AcroColorCalls.h
**
**
/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2002-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.
*/

#ifndef _H_AcroColorCalls
#define _H_AcroColorCalls

/* for Adobe use only */
#define _AcroColorHFT_LATEST_VERSION 0x00060000
#define AcroColorHFT_LATEST_VERSION _AcroColorHFT_LATEST_VERSION 

/* for public use */
#define AcroColorHFT_VERSION_6   0x00060000


#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif
	
#define EXTERNAL_ACROCOLORPROCS_USER 1 /* External user of AcroColorProcs.h header file */
#include "AcroColorExpT.h"

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)	\
		extern ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
  		extern ACEX1 returnType ACEX2 name params;
   #define XNPROC NPROC
   #define PROC	NPROC
   #define XPROC NPROC
	#define ENPROC	NPROC		
	#define NOPROC(name)
	#include "AcroColorProcs.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC
	#undef ENPROC	

#else

	/* HFT version */
   #include "PIRequir.h"
#ifdef THREAD_SAFE_PDFL
   #include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define SPROC(returnType, name, params, stubProc) \
		name##SEL,
   #define NOPROC(name)	\
			name##SEL,
	
   #define XNPROC NPROC
   #define PROC	NPROC
   #define XPROC NPROC
	#define ENPROC	NPROC
	enum {
		AcroColorBAD_SELECTOR,
		#include "AcroColorProcs.h"
		AcroColorNUMSELECTORSplusOne
	};
	
	#define AcroColorNUMSELECTORS (AcroColorNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */

	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef XPROC
   #undef NOPROC
	#undef ENPROC
	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	
   #define PROC	NPROC
   #define ENPROC NPROC
	#define NOPROC(name)
#if READER_PLUGIN
   /* Force an error for Exchange procs */
   #define XNPROC(returnType, name, params)
   #define XPROC(returnType, name, params)
#else
   #define XNPROC NPROC
   #define XPROC NPROC
#endif
	#include "AcroColorProcs.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC		
	#undef ENPROC
	#include "PIRequir.h"

#if PI_ACROCOLOR_VERSION != 0

#include "AcroColorExpT.h"

#include "CorCalls.h"		/* For ASCallbackCreateProto */

#ifdef THREAD_SAFE_PDFL
	#define gAcroColorHFT (GetHFTLocations()->acroColorHFT)
	#define gAcroColorVersion (GetHFTLocations()->acroColorVersion)
#else /* defined THREAD_SAFE_PDFL */
	extern HFT gAcroColorHFT;
#endif /* defined THREAD_SAFE_PDFL */


#define ACEngineCount (*((ACEngineCountSELPROTO) (gAcroColorHFT[ACEngineCountSEL])))
#define ACEngineInfo (*((ACEngineInfoSELPROTO) (gAcroColorHFT[ACEngineInfoSEL])))
#define ACSetEngine (*((ACSetEngineSELPROTO) (gAcroColorHFT[ACSetEngineSEL])))

#define ACMakeProfileList (*((ACMakeProfileListSELPROTO) (gAcroColorHFT[ACMakeProfileListSEL])))
#define ACProfileListCount (*((ACProfileListCountSELPROTO) (gAcroColorHFT[ACProfileListCountSEL])))
#define ACProfileListItemDescription (*((ACProfileListItemDescriptionSELPROTO) (gAcroColorHFT[ACProfileListItemDescriptionSEL])))
#define ACProfileListItemCode (*((ACProfileListItemCodeSELPROTO) (gAcroColorHFT[ACProfileListItemCodeSEL])))
#define ACUnReferenceProfileList (*((ACUnReferenceProfileListSELPROTO) (gAcroColorHFT[ACUnReferenceProfileListSEL])))

#define ACMakePresetList (*((ACMakePresetListSELPROTO) (gAcroColorHFT[ACMakePresetListSEL])))
#define ACPresetListCount (*((ACPresetListCountSELPROTO) (gAcroColorHFT[ACPresetListCountSEL])))
#define ACPresetListItemFile (*((ACPresetListItemFileSELPROTO) (gAcroColorHFT[ACPresetListItemFileSEL])))
#define ACPresetFileToName (*((ACPresetFileToNameSELPROTO) (gAcroColorHFT[ACPresetFileToNameSEL])))
#define ACUnReferencePresetList (*((ACUnReferencePresetListSELPROTO) (gAcroColorHFT[ACUnReferencePresetListSEL])))


#define ACGetSettingsProfile (*((ACGetSettingsProfileSELPROTO) (gAcroColorHFT[ACGetSettingsProfileSEL])))
#define ACMakeSettings (*((ACMakeSettingsSELPROTO) (gAcroColorHFT[ACMakeSettingsSEL])))
#define ACLoadSettings (*((ACLoadSettingsSELPROTO) (gAcroColorHFT[ACLoadSettingsSEL])))
#define ACGetSettingsString (*((ACGetSettingsStringSELPROTO) (gAcroColorHFT[ACGetSettingsStringSEL])))
#define ACGetSettingsUnsigned32 (*((ACGetSettingsUnsigned32SELPROTO) (gAcroColorHFT[ACGetSettingsUnsigned32SEL])))
#define ACUnReferenceSettings (*((ACUnReferenceSettingsSELPROTO) (gAcroColorHFT[ACUnReferenceSettingsSEL])))

#define ACProfileDescription (*((ACProfileDescriptionSELPROTO) (gAcroColorHFT[ACProfileDescriptionSEL])))
#define ACProfileFromDescription (*((ACProfileFromDescriptionSELPROTO) (gAcroColorHFT[ACProfileFromDescriptionSEL])))
#define ACProfileFromCode (*((ACProfileFromCodeSELPROTO) (gAcroColorHFT[ACProfileFromCodeSEL])))
#define ACMonitorProfile (*((ACMonitorProfileSELPROTO) (gAcroColorHFT[ACMonitorProfileSEL])))
#define ACMakeBufferProfile (*((ACMakeBufferProfileSELPROTO) (gAcroColorHFT[ACMakeBufferProfileSEL])))

#define ACMakeCalRGB (*((ACMakeCalRGBSELPROTO) (gAcroColorHFT[ACMakeCalRGBSEL])))
#define ACMakeCalGray (*((ACMakeCalGraySELPROTO) (gAcroColorHFT[ACMakeCalGraySEL])))
#define ACMakeCalLab (*((ACMakeCalLabSELPROTO) (gAcroColorHFT[ACMakeCalLabSEL])))
#define ACProfileColorSpace (*((ACProfileColorSpaceSELPROTO) (gAcroColorHFT[ACProfileColorSpaceSEL])))
#define ACProfileSize (*((ACProfileSizeSELPROTO) (gAcroColorHFT[ACProfileSizeSEL])))
#define ACProfileData (*((ACProfileDataSELPROTO) (gAcroColorHFT[ACProfileDataSEL])))
#define ACUnReferenceProfile (*((ACUnReferenceProfileSELPROTO) (gAcroColorHFT[ACUnReferenceProfileSEL])))

#define ACMakeColorTransform (*((ACMakeColorTransformSELPROTO) (gAcroColorHFT[ACMakeColorTransformSEL])))
#define ACApplyTransform (*((ACApplyTransformSELPROTO) (gAcroColorHFT[ACApplyTransformSEL])))
#define ACUnReferenceTransform (*((ACUnReferenceTransformSELPROTO) (gAcroColorHFT[ACUnReferenceTransformSEL])))

#define ACMakeString (*((ACMakeStringSELPROTO) (gAcroColorHFT[ACMakeStringSEL])))
#define ACStringASCII (*((ACStringASCIISELPROTO) (gAcroColorHFT[ACStringASCIISEL])))
#define ACStringLocalized (*((ACStringLocalizedSELPROTO) (gAcroColorHFT[ACStringLocalizedSEL])))
#define ACStringUnicode (*((ACStringUnicodeSELPROTO) (gAcroColorHFT[ACStringUnicodeSEL])))
#define ACUnReferenceString (*((ACUnReferenceStringSELPROTO) (gAcroColorHFT[ACUnReferenceStringSEL])))

#define ACGetWorkingSpaceProfile (*((ACGetWorkingSpaceProfileSELPROTO) (gAcroColorHFT[ACGetWorkingSpaceProfileSEL])))
#define ACProfilesMatch (*((ACProfilesMatchSELPROTO) (gAcroColorHFT[ACProfilesMatchSEL])))

#endif /* PI_ACROCOLOR_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_AcroColorCalls) */
