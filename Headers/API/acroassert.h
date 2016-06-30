/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

*********************************************************************/

#ifndef _H_CASSERT_ADOBE_API
#define _H_CASSERT_ADOBE_API

#if !DEBUG
/* ANSI assert.h requires preprocessor definition of NDEBUG in order to disable.
   We can use the DEBUG setting */
#define ACROASSERT(ignore) ((void*)0)

#else

#include "Environ.h"

#if WIN_PLATFORM || WIN_ENV
/* Win version of assert brings up dialog with file/line and allows user to go to debugger.  We'll use it */
#include <assert.h>
#define ACROASSERT assert

#elif MAC_PLATFORM || MAC_ENV
/* Mac version of assert just quits.  We prefer to give a message and break at the problem */
#include <CarbonCore/MacTypes.h>
#define ACROASSERT(b) ((b)?0: (DebugStr("\pACROASSERT Failed"), **(int**)0)) /* stop here, there is a problem */

#elif UNIX_PLATFORM || UNIX_ENV
#if TOOLKIT
/* Unix version of assert.  Use default implementation */
#include <assert.h>
#define ACROASSERT assert
#else
/*  asachdev: following is a workaround for a gcc 3.2 bug. gcc dumps core while compilation when it encounters
 *  the following situation:
 *  int foobar(int foo, int bar = (assert(f>=b), funcPtr)(), int foob=some_int);
 *  The problem is in the definition of __assert_fail used inside the standard assert definition. 
 *  ----------begin:excerpt from assert.h
 *  extern void __assert_fail (__const char *__assertion, __const char *__file,   unsigned int __line, 
 *  __const char *__function) __THROW __attribute__ ((__noreturn__));
 *  ----------end:excerpt from assert.h ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
 *                                    
 *  The highlighted construct is the cause of the problem. We will be defining our own assert macro. 
 *  The definition of AcroAssert_fail is added in another file at PDFLib/Source/Support/Util/Unix/acroassert.c
 */
#ifdef __cplusplus
	extern "C" {
 		extern int AcroAssert_fail(const char *exp, const char *fil, int lin);
 	}
#else 
 	extern int AcroAssert_fail(const char *exp, const char *fil, int lin);
#endif
 
#define ACROASSERT(expr) ((void) ((expr) ? 0 : (AcroAssert_fail (#expr, __FILE__, __LINE__))))
#endif
#else
/* should define one of the platforms above.  setting ACROASSERT to ANSI assert */
#pragma message("PLATFORM not defined")
#include <assert.h>
#define ACROASSERT assert
#endif /* PLATFORM */
#endif  /*!DEBUG */
   

#endif /*_H_CASSERT_ADOBE_API*/

