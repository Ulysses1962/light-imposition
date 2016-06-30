/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PETypes.h

 - Defines PDFEdit error codes.

*********************************************************************/

#ifndef _H_PETypes
#define _H_PETypes

#include "ASBasic.h"
#include "ASStm.h"
#include "ASTypes.h"
#include "CosTypes.h"

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

/* Build Type-specific error codes.
** Use these macros with the errors defined in the header files listed below,
** For example: RAISE(CosError(cosErrDocTableFull), NULL);
**
** Note that the default severity of an error changes depending on the
** category.
*/

#define PDFEditError(e)		ASErrBuildCode(ASErrAlways, ASErrSysPDFEdit, e)

/* Error definitions */
#define DefineErr(name,string)	name, 

enum {
	#include "PEError.h"
	pdfEditErrLast
};

#undef DefineErr


#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_PETypes */
