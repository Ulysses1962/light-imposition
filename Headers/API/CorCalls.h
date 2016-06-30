/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CorCalls.h

 - Exception handling methods (ASPushExceptionFrame and
   ASPopExceptionFrame) should typically be accessed through the
   DURING/HANDLER/END_HANDLER macros.

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

#ifndef _H_CorCalls
#define _H_CorCalls

#include "acroassert.h"

/* This header includes templates and hence would not compile if 
   included inside of a extern "C" block. Hence CorCalls.h must 
   not be included inside of a extern "C" block */
#include "EReturnValidator.h"

/* for Adobe use only */
#define _CoreHFT_LATEST_VERSION 0x00050000
#define _CoreHFT_LAST_BETA_COMPATIBLE_VERSION 0x00050000
#define _CoreHFT_IS_BETA 0

/* for public use */
#define CoreHFT_LATEST_VERSION (_CoreHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _CoreHFT_LATEST_VERSION) : _CoreHFT_LATEST_VERSION)

#define CoreHFT_VERSION_2   0x00020000
#define CoreHFT_VERSION_4   0x00040000
#define CoreHFT_VERSION_5   0x00050000  /* note: version 5 is same as 4.  Provided to support old plugins that erroneously required 5 */

#ifndef _CORVERSION_ID_ONLY
#include "PIExcept.h"
#include "CoreExpT.h"
#include "ASExpT.h"

#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if ACROBAT_LIBRARY
/* Toolkit specific Error-handling function prototypes */

/* RestorePlugInFrame
** This is a function that the plug-in must provide (if you are using the
** standard government-issue PIMain.c, this function is implemented there.)
** It takes a jmp_buf as registered through ASPushExceptionFrame and restores
** state based on that jmp_buf, usually through using longjmp.
*/
extern ACCB1 void ACCB2 RestorePlugInFrame(void *environ);
typedef ACCBPROTO1 void (ACCBPROTO2 *ACRestoreEnvironProc)(void *excEnviron);

extern ACCB1 void ACCB2 ACPushExceptionFrame(void *excEnviron, ACRestoreEnvironProc restoreFunc);
extern ACCB1 void ACCB2 ACPopExceptionFrame(void);
extern ACCB1 ASInt32 ACCB2 ACGetExceptionErrorCode(void);

#define DURING {												\
	ACROjmp_buf ASException;										\
	ACPushExceptionFrame(&ASException, (ACRestoreEnvironProc)&RestorePlugInFrame);		\
	if (!ACROsetjmp(ASException)) {

#define _E_RESTORE      ACPopExceptionFrame()

#define HANDLER _E_RESTORE; } else { _E_RESTORE;
#define END_HANDLER }}

#define ERRORCODE	(ACGetExceptionErrorCode())

#define E_RETURN(x) { _E_RESTORE; return(x); }
#define E_RTRN_VOID { _E_RESTORE; return; }

#else /* ACROBAT_LIBRARY */

#define ACRestoreEnvironProc restoreEnvironProc
#define ACPushExceptionFrame ASPushExceptionFrame
#define ACPopExceptionFrame  ASPopExceptionFrame
#define ACGetExceptionErrorCode ASGetExceptionErrorCode

#endif /* ACROBAT_LIBRARY */

#ifdef NPROC /* may be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
  		extern ACEX1 returnType ACEX2 name params;
	
   #define ANPROC NPROC

	/* These calls have a different internal name */
   #define ASPushExceptionFrame ACPushExceptionFrame
   #define ASPopExceptionFrame ACPopExceptionFrame
   #define ASGetExceptionErrorCode ACGetExceptionErrorCode
   #define ASExtensionMgrGetHFT ASGetHFTByNameAndVersion
   #include "CorProcs.h"
   #undef NPROC
   #undef ANPROC
   #undef SPROC	
   #define ASCallbackCreateProto(proto, proc) (proc)
   #define ASCallbackCreate(extID, proc) (proc)
   #define ASCallbackDestroy(proc) 
#else

	/* HFT version */
	#include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define SPROC(returnType, name, params, stubProc) \
		name##SEL,
	
   #define ANPROC NPROC	
	enum {
		CoreBAD_SELECTOR,
		#include "CorProcs.h"
		CoreNUMSELECTORSplusOne
	};
	
	#define CoreNUMSELECTORS (CoreNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
   #undef ANPROC
   #undef SPROC	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	
   #define ANPROC NPROC	
		#include "CorProcs.h"
	#undef NPROC
   #undef ANPROC
   #undef SPROC
	

#if PI_CORE_VERSION != 0

#ifdef THREAD_SAFE_PDFL
	#define gCoreHFT (GetHFTLocations()->coreHFT)
	#define gCoreVersion (GetHFTLocations()->coreVersion)
#else
	extern HFT gCoreHFT;
	extern ASUns32 gCoreVersion;
#endif

#include "PICommon.h"
#include "PIExcept.h"

/* Exception-handling functions */

/* ASRaise
** Raises an exception.  Subsequent calls to ASGetExceptionErrorCode() will
** return the error value passed in to this function.
** This function is called directly by the macro RERAISE().
*/
#define ASRaise (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASRaiseSELPROTO)(gCoreHFT[ASRaiseSEL])))

#define RERAISE() ASRaise(ERRORCODE)

/* ASPushExceptionFrame
**
** Push an exception frame buffer and a frame-restoration callback onto the stack.
** The restoreFunc should be a function matching the following prototype:
**
** ACCB1 void ACCB2 RestorePlugInFrame(void *asEnviron);
**
** You will probably never call ASPushExceptionFrame() directly; use the DURING
** macro instead.
*/
#define ASPushExceptionFrame (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASPushExceptionFrameSELPROTO)(gCoreHFT[ASPushExceptionFrameSEL])))

/* RestorePlugInFrame
** This is a function that the plug-in must provide (if you are using the
** standard government-issue PIMain.c, this function is implemented there.)
** It takes a jmp_buf as registered through ASPushExceptionFrame and restores
** state based on that jmp_buf, usually through using longjmp.
*/
extern ACCB1 void ACCB2 RestorePlugInFrame(void *asEnviron);


/* ASPopExceptionFrame
** Pop and exception frame off the stack.
** You will probably never call ASPopExceptionFrame() directly; it is called for
** you as appropriate from within the HANDLER, E_RETURN and E_RTRN_VOID macros.
*/
#define ASPopExceptionFrame (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASPopExceptionFrameSELPROTO)(gCoreHFT[ASPopExceptionFrameSEL])))

#define _E_RESTORE      ASPopExceptionFrame()


#if DEBUG

/* **ER - 11/29/1997

   For debug builds, the macros below enable compile-time checking of common problems
   with DURING and HANDLER blocks (in order of frequency):

        1. "normal" return statements inside DURING blocks
        2. E_RETURN or E_RTRN_VOID statements outside any DURING block
        3. E_RETURN or E_RTRN_VOID statements in HANDLER blocks
        4. HANDLER without matching DURING 
        5. HANDLER without matching END_HANDLER 
*/

/* Declaration of global integer used below.  This variable needs to be defined
   somewhere in the plug-in.  If you are using the standard government-issue PIMain.c,
   it will already be defined there. */
extern int gBadReturnCatcher;

struct _BadReturnCatcherStruct {
	int E_RETURNOutsideDURINGBlock;		/* This name chosen so it rings a bell if you
										   see it in the compiler output */
};

/* What's the point of this? Since E_RETURN and E_RTRN_VOID need to actually
   call return, we need to redefine gBadReturnCatcher as an integer in the
   scope around the actual call to return so our macro-replaced version of
   return will work.  Otherwise, when the "return" in the E_RETURN macro 
   (for example), was replaced by our redefined macro, it'd fail to compile
   just as if return was placed inside the DURING block */ 
#define DO_RETURN(x)			{ int gBadReturnCatcher; return(x); }
#define DO_RETURN_NO_ARGS		{ int gBadReturnCatcher; return; }

/* This definition of DURING includes a struct with the same name as the global 
   integer above.  The struct must be declared inside the inner scope of the DURING
   so it is NOT defined in the HANDLER! */
#define DURING {												\
			   ACROjmp_buf ASException;										\
			   ASPushExceptionFrame(&ASException, &RestorePlugInFrame);		\
			   if (!ACROsetjmp(ASException)) {								   \
			   struct _BadReturnCatcherStruct gBadReturnCatcher;			   

#if DO_ADDITIONAL_RAISEAWARE_CHECKS

/* Make sure that ASRaiseAware classes are not returned as part of the E_RETURN
   mechanism as it leaves the wrong handler on the top of the exception stack.*/
#define E_RETURN(x) { IsNotRaiseAwareClassHelperFunction( x ); gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN(x); }

#else

/* Return calls which are legal inside a DURING block attempt
   to modify the member field of the gBadReturnCatcher struct in the local 
   scope.  This will cause a compile-time error if there is no such struct 
   (and thus, the return call is not enclosed by a DURING block) */
#define E_RETURN(x) { gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN(x); }

#endif

#define E_RTRN_VOID { gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN_NO_ARGS; }

/* returns which are illegal inside a DURING block attempt
   to modify the global integer gBadReturnCatcher.  This will cause a 
   compile-time error if the return is inside a DURING block because
   it will actually be an attempt to modify the local-scope struct of the
   same name.  The = operator is not defined for assigning constant integers to structs.

   Why are we using a for loop? Here are the considerations involved in choosing
   this macro:

   1. It must support return statements of the forms "return;", "return <expr>;", and
      "return( <expr> );".  This is why "return" is present at the end of the macro
   2. We must use only 1 statement in replacing return.  Why? Consider this code:

	  if ( <expr> ) 
		  return; 
	  else {
		  ...
	  }

	  In order to support this case, we must not replace the "return" statement with
	  more than one statement, because this will cause the "else" statement to become
	  untied from the "if" statement.  The following code snippet is an example of
	  the above code snippet after a macro expansion where return is defined as multiple
	  statements.  

	  if ( <expr> )
	       gBadReturnCatcher = 1;
		   return;
	  else {
		...
	  }
	3. If we are constrained to replace the "return" statement with only one statement,
	   there may only be one path of control, and it must reach the return statement
	   at the end of the macro (so that it can pick up its arguments, if there are any)

    The do-nothing for loop below meets these criteria.   
*/
#define return for( gBadReturnCatcher = 1;; ) return

/* Why is HANDLER redefined? 
   1. So you can't put a HANDLER in without a matching DURING.  This would likely be caught
   by the compiler anyhow, since there'd likely be an "else" without a matching "if"
   2. To facilitate catching END_HANDLERs without matching HANDLERs
   3) To get rid of pesky "unreferenced local variable" warnings stemming from 
   DURING blocks which don't contain E_RETURN or E_RTRN_VOID and thus don't 
   reference the local gBadReturnCatcher structure. */
#define HANDLER _E_RESTORE; gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; } else { int ENDHANDLEROutsideHANDLER; _E_RESTORE;

/* END_HANDLER is redefined to modify the BadEndHandlerCatcher variable defined by the HANDLER
   macro.  This will prevent END_HANDLERs without matching HANDLERs (unless, of course, 
   multiple END_HANDLER statements are found inside the same HANDLER, END_HANDLER block */
#define END_HANDLER ENDHANDLEROutsideHANDLER = 1; }}

#else	/* !DEBUG */

#define DURING {												\
	ACROjmp_buf ASException;										\
	ASPushExceptionFrame(&ASException, &RestorePlugInFrame);		\
	if (!ACROsetjmp(ASException)) {


#define HANDLER _E_RESTORE; } else { _E_RESTORE;
#define END_HANDLER }}

#define E_RETURN(x) { _E_RESTORE; return(x); }
#define E_RTRN_VOID { _E_RESTORE; return; }

#endif
/* ASGetExceptionErrorCode
** Returns the value of the error parameter as passed in to the most
** recent call to ASRaise().  You might then pass this value to ASGetErrorString()
** in order to report the exception to the user.
*/
#define ASGetExceptionErrorCode (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASGetExceptionErrorCodeSELPROTO)(gCoreHFT[ASGetExceptionErrorCodeSEL])))

#define ERRORCODE	(ASGetExceptionErrorCode())

/* ASAtom management */

/* ASAtomFromString
** Returns the ASAtom associated with the given string.  You can compare
** ASAtoms directly, rather than passing their associated strings to
** strcmp().
*/
#define ASAtomFromString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomFromStringSELPROTO)(gCoreHFT[ASAtomFromStringSEL])))

/* ASAtomExistsForString
** Returns true if and only if there already exists an ASAtom for the
** given string.  You might use this to prevent capricious overpopulation
** of the internal ASAtom table (a non-renewable resource!)
*/
#define ASAtomExistsForString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomExistsForStringSELPROTO)(gCoreHFT[ASAtomExistsForStringSEL])))

/* ASAtomGetString
** Returns the char * associated with the given ASAtom.  This routine
** permanently allocates storage for new strings, so don't call this function
** unnecessarily!
*/
#define ASAtomGetString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomGetStringSELPROTO)(gCoreHFT[ASAtomGetStringSEL])))

/* ASCallbackCreate
** Takes a pointer to a function and returns an ASCallback--a universally
** entrant function pointer which can be called from any globals context.
*/
#define ASCallbackCreate(x) (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASCallbackCreateSELPROTO)(gCoreHFT[ASCallbackCreateSEL])))(gExtensionID, (void *)(x))

/* ASCallbackCreateProto
** Type-checking callback creation.  Will cause a compiler error if the
** proc is not of the given function-type.  DEBUG must be on for checking to occur.
** Requires four bytes of scratch global space in which to make a bogus
** assignment; this is declared in PIMain.c.
*/
#if DEBUG
#define ASCallbackCreateProto(funcType, proc)				\
	((funcType)ASCallbackCreate((CHECKTYPE(funcType, proc))))
#else
#define ASCallbackCreateProto(funcType, proc)				\
	((funcType)ASCallbackCreate((proc)))
#endif

/* ASCallbackDestroy
** De-allocates memory associated with an ASCallback.  Use this if you're
** sure you're completely done with an ASCallback.
*/
#define ASCallbackDestroy (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASCallbackDestroySELPROTO)(gCoreHFT[ASCallbackDestroySEL])))

/* ASExtensionMgrGetHFT
** Get an HFT object with the given name and version number.
*/
#define ASExtensionMgrGetHFT (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASExtensionMgrGetHFTSELPROTO)(gCoreHFT[ASExtensionMgrGetHFTSEL])))

/* ASGetConfiguration
** Returns a void * based on the ASAtom passed in.  The void *
** may be cast to a meaningful value based on the input parameter:
**
** key                 result   	                 return type
** Product             Name of product type 	     const char *
**                       (Exchange, Reader, etc.)
** CanEdit             Whether or not editing   	 ASBool
**                       is allowed
** <<anything else>>   UNDEFINED_CONFIGURATION_SELECTOR
*/
#define ASGetConfiguration (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASGetConfigurationSELPROTO)(gCoreHFT[ASGetConfigurationSEL])))


/* PI_CORE_VERSION >= 0x00040000 */

/* ASEnumExtensions */
#define ASEnumExtensions (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASEnumExtensionsSELPROTO)(gCoreHFT[ASEnumExtensionsSEL])))

/* ASExtensionGetFileName */
#define ASExtensionGetFileName (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASExtensionGetFileNameSELPROTO) (gCoreHFT[ASExtensionGetFileNameSEL])))

/* ASExtensionGetRegisteredName */
#define ASExtensionGetRegisteredName (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASExtensionGetRegisteredNameSELPROTO) (gCoreHFT[ASExtensionGetRegisteredNameSEL])))


#endif /* PI_CORE_VERSION != 0 */

#endif /* PLUGIN */

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef __cplusplus
}
#endif
#endif /* _CORVERSION_ID_ONLY */
#endif /* !defined(_H_CorCalls) */

