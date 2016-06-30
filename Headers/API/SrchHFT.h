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

 SrchHFT.h

 - Catalog of functions exported by Acrobat Search.

*********************************************************************/

#ifndef _H_SrchHFT
#define _H_SrchHFT

#include "SrchType.h"

/* Extension name : "Search" */

#define SearchHFT_NAME		"Search"
#define SearchHFT_LATEST_VERSION		(0L)

#define Init_SearchHFT ASExtensionMgrGetHFT(ASAtomFromString(SearchHFT_NAME), SearchHFT_LATEST_VERSION)

/* Enumerate the selectors */
#define PROC(returnType, name, params)			\
	name##SEL,
#define NPROC PROC
#define SPROC(returnType, name, params, stubProc) PROC(returnType, name, params)
#define NOPROC(name) PROC(ignored, DONOTUSE_##name, ignored)

typedef enum _SearchSelector {
	SearchBAD_SELECTOR,
	#include "SrchPrcs.h"
	SearchNUMSELECTORSminusOne
} SearchSelector;

#define SearchNUMSELECTORS (SearchNUMSELECTORSminusOne + 1)

#undef PROC
#undef NPROC
#undef SPROC
#undef NOPROC

#define NPROC(returnType, name, params)		\
	typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
#define PROC NPROC
#define SPROC(returnType, name, params, stubProc) PROC(returnType, name, params)
#define NOPROC(name)
	#include "SrchPrcs.h"
#undef PROC
#undef NPROC
#undef SPROC
#undef NOPROC

extern HFT gSearchHFT;
extern ASBool SetUpSearchHFTServer(void);
#endif

