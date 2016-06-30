/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AVSPCalls.h
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
        ACROASSERT(g~Version >= ~HFT_VERSION_4);  //PI_~_VERSION was defined as ~HFT_VERSION_4
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
   is under development.  You add a new routine for version 21.  Increment _~HFT_LATEST_VERSION 
   to 0x00200001 and set _~_IS_BETA to 1.  Add "#define ~HFT_VERSION_21 ~HFT_LATEST_VERSION".  
   Add your routine and assert that g~Version >= ~HFT_VERSION_21.  Leave 
   _~_LAST_BETA_COMPATIBLE_VERSION unchanged (0x00200000 in our example).

   If the ~_IS_BETA flag is set to 1, you may change or delete the beta routines at will.
   Before checking in the modifications however, increment the _~HFT_LATEST_VERSION number.
   If the change is not compatible (delete, change, etc.) set _~_LAST_BETA_COMPATIBLE_VERSION equal
   to the new _~HFT_LATEST_VERSION.  If the change is compatible, leave the LAST_BETA version
   alone.

   Plugins that require a BETA HFT will be refused unless they ask for a beta version
   >= LAST_BETA_COMPATIBLE_VERSION and <= HFT_LATEST_VERSION.
   By incrementing the version number you ensure the plugin will refuse to load until it
   has been recompiled with your changes.  You also ensure plugins compiled with your changes
   will not load on older versions of Acrobat.  In other words in makes sure both sides are in sync.  

   Again, whenever you make a change to this file, you must increment _~HFT_LATEST_VERSION.
  
   Once the product reaches RC or similar milestone, change the _~_IS_BETA flag to 0, jump
   The _~HFT_LATEST_VERSION to the final version (0x00210000 in our example), do the same for
   _~_LAST_BETA_COMPATIBLE_VERSION, and set the 
   ~HFT_VERSION_# to the final version number (0x00210000 in our example).  Once the HFT
   has left beta, the routines cannot be changed and a new beta must ensue (beta for version
   22 in our example).

*********************************************************************/

#ifndef _H_AVSPCalls
#define _H_AVSPCalls

/* for Adobe use only */
#define _SweetPeaHFT_LATEST_VERSION 0x00050000
#define _SweetPeaHFT_LAST_BETA_COMPATIBLE_VERSION 0x00050000
#define _SweetPeaHFT_IS_BETA 0

/* for public use */
#define SweetPeaHFT_LATEST_VERSION (_SweetPeaHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _SweetPeaHFT_LATEST_VERSION) : _SweetPeaHFT_LATEST_VERSION)

#define SweetPeaHFT_VERSION_5   0x00050000

#include "CoreExpT.h"
#include "ASExpT.h"
#include "SPHost.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXTERNAL_AVSPPROCS_USER 1 /* External user of AVSPProcs.h header file */

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
	#include "AVSPProcs.h"
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
		AcroViewSweetPeaBAD_SELECTOR,
		#include "AVSPProcs.h"
		AcroViewSweetPeaNUMSELECTORSplusOne
	};
	
	#define AcroViewSweetPeaNUMSELECTORS (AcroViewSweetPeaNUMSELECTORSplusOne - 1)
	
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
	
   #define XNPROC NPROC
   #define PROC	NPROC
   #define ENPROC NPROC
   #define XPROC NPROC
	#define NOPROC(name)	
	#include "AVSPProcs.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC		
	#undef ENPROC
	#include "PIRequir.h"

#if PI_ACROVIEW_SWEETPEA_VERSION >= 0x00050000

#include "NSelExpT.h"

#include "CorCalls.h"		/* For ASCallbackCreateProto */

extern HFT gAcroViewSweetPeaHFT;
extern ASVersion gAcroViewSweetPeaVersion;

#define AVSweetPeaGetBasicSuiteP 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaGetBasicSuitePSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaGetBasicSuitePSEL])))

#define AVSweetPeaGetPluginRef 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaGetPluginRefSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaGetPluginRefSEL])))

#define AVSweetPeaGetResourceAccess 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaGetResourceAccessSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaGetResourceAccessSEL])))

#define AVSweetPeaSetResourceAccess 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaSetResourceAccessSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaSetResourceAccessSEL])))

#define AVSweetPeaIsADMAvailable 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaIsADMAvailableSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaIsADMAvailableSEL])))

#define AVSweetPeaProcessADMEvent 	(ACROASSERT(gAcroViewSweetPeaVersion >=SweetPeaHFT_VERSION_5),*((AVSweetPeaProcessADMEventSELPROTO) \
								   (gAcroViewSweetPeaHFT[AVSweetPeaProcessADMEventSEL])))


#endif /* PI_ACROVIEW_SWEETPEA_VERSION != 0x00050000 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_AVSPCalls) */
