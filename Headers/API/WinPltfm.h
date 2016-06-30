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

 WinPltfm.h

 - PLATFORM file for Windows development.

*********************************************************************/

#define MDSYSERR "wfileerr.h"
#define DefineMDSysErr(mdNum, mdName, msg) mdName = mdNum,

#define ASKEY "askeywin.h"

#define AS_LITTLEENDIAN 1
#define IEEEFLOAT 0
#define IEEESOFT 0
#define UNSIGNEDCHARS 0
#define FRACT_DEFINED 0  

#ifdef _WIN32
#define MEMSET_IMPLEMENTED      0
#define MEMCPY_IMPLEMENTED      0
#else
#define MEMSET_IMPLEMENTED      1
#define MEMCPY_IMPLEMENTED      1
#endif  // _WIN32

/*  32 bit mode changes */
#ifdef _WIN32
#define WORD_ALIGN 4
#define os_size_t_IS_Uns32      1
#define HUGEPTRTYPE 
#else
#define WORD_ALIGN 2
#define os_size_t_IS_Uns32      0
#define HUGEPTRTYPE __huge
#endif  /* _WIN32 */

/* Define the linefeed termination for this system */
#define nENDLINE_CHARS 2
#define sENDLINE "\015\012"

#define REPLACEABLE

#ifdef _WIN32  
#define far
#define _huge 
#define __huge
#define AVEX1 __declspec(dllexport)
#define AVEX2 
#else
#define AVEX1 
#define AVEX2 __export
#endif

#define ACROCALLBACKPROTO
#define ACCBPROTO1
#define ACCBPROTO2

#ifdef _WIN32
#define ACROCALLBACK  
#define ACCB2                  
#else
#define ACROCALLBACK    __loadds
#define ACCB2           __loadds
#endif
#define ACCB1

#ifdef _WIN32
#define ACROEXPORT AVEX1   
#define ACEX1      AVEX1
#define ACEX2      
#else
#define ACROEXPORT __loadds __export
#define ACEX1     
#define ACEX2      __loadds __export
#endif

#ifdef _WIN32
#define ACROEXPORTPRIV  AVEX1        
#define ACROEXPORTCDECL AVEX1
#define ACROEXPORTDATA  
#define ACROIMPORTDATA  extern __declspec(dllimport)
#define ACROIMPORTDATA1 __declspec(dllimport)
#define ACROIMPORTDATA2 
#else
#define ACROEXPORTPRIV  __loadds  __export
#define ACROEXPORTCDECL __cdecl   __loadds  __export
#define ACROEXPORTDATA  __export
#define ACROIMPORTDATA  extern
#define ACROIMPORTDATA1 
#define ACROIMPORTDATA2 
#endif

#define WIN_PLATFORM 1

/* XXX we should only have DEBUG in ACROCORE */
#if _DEBUG
	#define DEBUG 1
	#define DODEBUG 1
#endif

/* The following defines are required to compile the Filter sources */
#define ENVIRONMENT "PS2Prefi.h"
#define CAROUSEL 1
#define ANSI_C 1
#define WIN 1
