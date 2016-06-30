/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDSysFontExpT.h

 - PDSysFont Types header file.

*********************************************************************/

#ifndef _H_PSFExpT
#define _H_PSFExpT


#include "Environ.h"
#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#else
#include "PubTypes.h"
#endif

#if WIN_PLATFORM
#include <Windows.h>
#endif

#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif

typedef ASInt16 PDSysFontMode;
typedef ASInt16 PDSysFontFID;
typedef ASInt16 PDSysFontFStyle;

/** A reference to a font installed in the host system. PDSysFont methods allow you to
	list the fonts available in the host system and to find a font in the system that matches
	a PDEFont, if it is present.
	@see PDEnumSysFonts
	@see PDFindSysFont
	@see PDFindSysFontEx
	@see PDFindSysFontForPDEFont
	@see PDEnumSysFonts
*/
typedef struct _t_PDSysFont *PDSysFont;


/** Font matching flags for PDFindSysFontForPDEFont and PDFindSysFont.
	@see PDFindSysFont
	@see PDFindSysFontForPDEFont
*/
typedef enum {

	/** Match the font name and character set. */
	kPDSysFontMatchNameAndCharSet = 0x0001,

	/** Match the font type */
	kPDSysFontMatchFontType		  = 0x0002,

	/** Match the writing mode, that is, horizontal or vertical. */
	kPDSysFontMatchWritingMode    = 0x0004
	} PDSysFontMatchFlags;

/* PDEFont information.
	@see PDSysFontGetInfo
*/
typedef struct _t_PDEFontInfo {

	/** An ASAtom for font name, as in "Times-Roman." */
	ASAtom	name;

	/**An ASAtom for font type, "Type 1," "TrueType," and so on.  */
	ASAtom	type;

	/** An ASAtom for "Roman" or ASAtomNull. If "Roman," the
		characters must be a subset of the Adobe Standard Roman Character Set. 
	*/
	ASAtom	charSet;

	/** An ASAtom for font encoding, as in WinAnsiEncoding. */
	ASAtom	encoding;

	/** Writing mode: 0 = horizontal; 1 = vertical. */
	PDSysFontMode	wMode;			
	} PDEFontInfoRec, *PDEFontInfoP;


/** PDSysFontPlatData */
#if WIN_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** sizeof(PDSysFontPlatData) */
	ASSize_t size;	

	/** (Windows only) Windows LOGFONT structure defining font attributes.*/
	LOGFONT *lf;

	/** Optional - only set if LOGFONT is not present */
	ASPathName fontPath;	

	/** Optional - only set if LOGFONT is not present */
	ASPathName afmPath; 
} PDSysFontPlatData, *PDSysFontPlatDataP;
#elif MAC_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** sizeof(PDSysFontPlatData) */
	ASSize_t size;	

	/** FontID */
	PDSysFontFID fontID;	

	/** Style value within that Font - default = 0 */
	PDSysFontFStyle fontStyle; 
} PDSysFontPlatData, *PDSysFontPlatDataP;
#elif UNIX_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** sizeof(PDSysFontPlatData) */
	ASSize_t size;	

	/** A path to the font file. Set only if lf is not present. */
	ASPathName fontPath;

	/** A path to the font AFM file. Set only if lf is not present. */
	ASPathName afmPath;
} PDSysFontPlatData, *PDSysFontPlatDataP;
#endif

/* PDSysFontEnumProc

/**
	Callback for PDEnumSysFonts. It is called once for each 
	system font. 
	@param sysFont IN/OUT? The system font. 
	@param clientData IN/OUT? User-supplied data that was specified 
	in the call to PDEnumSysFonts. 
	@return true to continue enumeration, false to halt enumeration. 
	
	@see PDEnumSysFonts 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDSysFontEnumProc)(PDSysFont sysFont, void *clientData);

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_PSFExpT */
