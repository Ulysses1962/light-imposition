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

 NSelExpT.h

 - This is the list of NSelector constants and function-prototypes
   required to register for notifications.

*********************************************************************/

#ifndef _H_NSelExpT
#define _H_NSelExpT

#ifdef __cplusplus
extern "C" {
#endif

#include "PIRequir.h"
#if ACROBAT_LIBRARY || TOOLKIT
typedef struct _t_AVDoc *AVDoc;
typedef struct _t_AVPageView *AVPageView;
typedef ASUns8 AVPrefsType;
typedef ASEnum16 AVAnnotOp;
typedef struct _t_AVBatchContext *AVBatchContext;
typedef struct AVRect *AVDevRectP;
typedef struct _t_AVTool *AVTool;
#else
#include "AVExpT.h"
#endif /* ACROBAT_LIBRARY || TOOLKIT */

/* Enumerate the selectors */
#define POKE(name, formals, formalsWithRock, actualsWithRock) name##NSEL,
#define NOPOKE(name, formals, formsalsWithRock, actualsWithRock) OBSOLETE##name##NSEL,
enum {
	#include "PIPokes.h"
	numAcroNotificationSelectors
};
#undef POKE
#undef NOPOKE

/* Define the prototypes */
#define POKE(name, formals, formalsWithRock, actualsWithRock)		\
	typedef ACCBPROTO1 void (ACCBPROTO2 *name##NPROTO)formalsWithRock;
#define NOPOKE(name, formals, formsalsWithRock, actualsWithRock)
#include "PIPokes.h"
#undef POKE
#undef NOPOKE

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_NSelExpT) */
