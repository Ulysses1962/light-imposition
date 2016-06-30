/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CoreExpT.h

 - Types, macros, structures, etc. required to use the Core HFT.

*********************************************************************/

#ifndef _H_CoreExpT
#define _H_CoreExpT

#include "Environ.h"

#if UNIX_PLATFORM
#include <sys/types.h>
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

/* integer types */
/**
	1-byte signed char 
*/
typedef signed char			ASInt8, *ASInt8P;
/**
	2-byte signed short numeric value. 
*/
typedef short int			ASInt16, *ASInt16P;

#if POINTER_64_BITS
/**
	4-byte signed long numeric value. 
*/
	typedef int				ASInt32, *ASInt32P;
#else
	typedef long int		ASInt32, *ASInt32P;
#endif

/* CodeWarrior, gcc, and VC 7 (1330) support "long long". VC 6 uses __int64. */
#if ! defined(_MSC_VER) || _MSC_VER >= 1330
	typedef signed long long int		ASInt64;
#elif defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64
	typedef signed __int64				ASInt64;
#else
/**
	4-byte signed long numeric value. 
*/
	#error 64-bit integer type not supported
#endif

/** */
#define ASMAXInt8			((ASInt8)0x7F)
/** */
#define ASMINInt8			((ASInt8)0x80)
/** */
#define ASMAXInt16			((ASInt16)0x7FFF)
/** */
#define ASMINInt16			((ASInt16)0x8000)
/** */
#define ASMAXInt32			((ASInt32)0x7FFFFFFF)
/** */
#define ASMINInt32			((ASInt32)0x80000000)
#define ASMAXInt64			((ASInt64)0x7FFFFFFFFFFFFFFF)
#define ASMINInt64			((ASInt64)0x8000000000000000)

/* cardinal types */
/**
	1-byte unsigned char. 
*/
typedef unsigned char		ASUns8, *ASUns8P;
/**
	2-byte unsigned short numeric value. 
*/
typedef unsigned short int	ASUns16, *ASUns16P;

#if POINTER_64_BITS
/**
	4-byte unsigned long numeric value. 
*/
	typedef unsigned int		ASUns32, *ASUns32P;
#else
	typedef unsigned long int	ASUns32, *ASUns32P;
#endif

#if ! defined(_MSC_VER) || _MSC_VER >= 1330
	typedef unsigned long long int		ASUns64;
#elif defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64
	typedef unsigned __int64			ASUns64;
#else
/**
	4-byte unsigned long numeric value. 
*/
	#error 64-bit integer type not supported
#endif

/** */
typedef long int			ASIntOrPtr;
/** */
typedef unsigned long int	ASUnsOrPtr;

/** */
#define ASMAXUns8			((ASUns8)0xFF)
/** */
#define ASMINUns8			((ASUns8)0x00)
/** */
#define ASMAXUns16			((ASUns16)0xFFFF)
/** */
#define ASMINUns16			((ASUns16)0x0000)
/** */
#define ASMAXUns32			((ASUns32)0xFFFFFFFF)
/** */
#define ASMINUns32			((ASUns32)0x00000000)
#define ASMAXUns64			((ASUns64)0xFFFFFFFFFFFFFFFF)
#define ASMINUns64			((ASUns64)0x0000000000000000)


/** ASBool */
typedef ASUns16 ASBool;

/* These types have been introduced to give us explicit control over
** the sizes of various enums we exported as such through our 2.0 plug-in
** API.  Enums are not sized the same way on the two platforms.
*/
#if WIN_PLATFORM

/**
	1-byte enumeration with values from 0 to 127, used in data 
	structures. 
*/
typedef ASInt16 ASEnum8;
/**
	2-byte enumeration with values from 0 to 32,767, used in 
	data structures. 
*/
typedef ASInt16 ASEnum16;

/** */
#define kASMAXEnum8 ASMAXInt16
/** */
#define kASMAXEnum16 ASMAXInt16

#else

/** */
typedef ASInt8 ASEnum8;
/**
	2-byte enumeration with values from 0 to 32,767, used in 
	data structures. 
*/
typedef ASInt16 ASEnum16;

/** */
#define kASMAXEnum8 ASMAXInt8
/** */
#define kASMAXEnum16 ASMAXInt16

#endif

#if PLUGIN || ACROBAT_LIBRARY
#include "AcroErr.h"
#endif

#define HAS_BOOL_SUPPORT defined(__cplusplus)


#if !HAS_BOOL_SUPPORT
   #ifndef true
	   #define true    1
   #endif
   #ifndef false
	   #define false    0
   #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ASSize_t - canonical type for sizes of things in bytes. */
/** */
typedef ASUnsOrPtr				ASSize_t;

/* Generic pointers */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

/* Opaque basetypes */
typedef ASInt32				OPAQUE_32_BITS;
typedef struct OPAQUE_64_BITS
{
	OPAQUE_32_BITS a, b;
}
OPAQUE_64_BITS;

/* HugePtr definition
** This is required for Intel processors when 
** running in a segmented mode. The HUGETYPE specifies
** whatever keyword is required to make a pointer 
** valid when accessing blocks of memory greater than 64K.
** For non-Intel machines, it is defined as nothing.
*/
#ifndef HUGEPTRTYPE
#define HUGEPTRTYPE
#endif
#define HugePtr char HUGEPTRTYPE *

/*------------------------------------------------------------------------
	typedef of ASAtom object
------------------------------------------------------------------------*/
#ifndef HAS_32BIT_ATOMS
#if TOOLKIT || ACROBAT_LIBRARY || PDFLPI || PDFL_EXTENSION
#define HAS_32BIT_ATOMS 1
#else
#define HAS_32BIT_ATOMS 0
#endif
#endif

#if HAS_32BIT_ATOMS
typedef ASUns32 ASAtom;			/* PDFLib uses 32bit ASAtom :-) */
#define	ASAtomNull ASMAXUns32
#else
typedef ASUns16 ASAtom;			/* this could be an ASUns32. See RecLst.h */
#define	ASAtomNull ASMAXUns16
#endif


/** Selector
** The Selector data type uniquely identifies an entry within an HFT.
** It is simply the integer offset of the entry from the start of the HFT.
*/
typedef ASInt32 Selector;
#define BAD_SELECTOR		0


/** HFTEntry
 An HFTEntry may be cast to a pointer to a function whose prototype must
 be provided by the HFT's description file.
*/
typedef void *HFTEntry;

/** HFT
** An HFT is an object that describes a set of exported functions.
** It is an array of function pointers where the first element is unused.
**
** Note that an HFT object may be cast to an (HFTEntry *); you may then
** index directly into this object by a selector to obtain a pointer to
** a function.
*/
#if !defined(HFT_IMPL) || !HFT_IMPL
typedef HFTEntry *HFT;
#endif
/* bit to set for beta versions */
#define kHFT_IN_BETA_FLAG 0x80000000


/**
	A flag that specifies whether an HFT entry is replaceable. 
	<ul>
	<li> If set, the new entry can be replaced. Clients should 
	generally use this value, allowing other clients to subsequently 
	replace the method again. 
	<li> If not set, the new entry cannot be replaced. 
	</ul>
	@see HFTReplaceEntry
*/
#define HFTEntryReplaceable			(0x00000001)


/**
	An opaque pointer to an object that identifies a specific loaded plug-in. An unique
	ASExtension object is created for each plug-in when it is loaded. If the plug-in fails
	to initialize, the ASExtension remains but is marked as inactive.
	@see ASEnumExtensions
*/
typedef struct _t_ASExtension *ASExtension;
/** */
typedef ASExtension ExtensionID ;
/** */
typedef void *ASCallback;


/** This constant is returned by ASGetConfiguration when the selector
	passed in is unknown to the application.
*/
#define UNDEFINED_CONFIGURATION_SELECTOR ((void *)-1)


/**
	environment-restoration functions are called when an exception is raised. 
	@param asEnviron the environment to restrore 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *restoreEnvironProc)(void *asEnviron);

/* not documenting these as they are obsolete. */
#define ASUSE_OBSOLETE_TYPES 1	/* XXX - temp */
#if ASUSE_OBSOLETE_TYPES
#define Int8	ASInt8
#define Int8P	ASInt8P
#define Int16	ASInt16
#define Int16P	ASInt16P
#define Int32	ASInt32
#define Int32P	ASInt32P

#define Uns8	ASUns8
#define Uns8P	ASUns8P
#define Uns16	ASUns16
#define Uns16P	ASUns16P
#define Uns32	ASUns32
#define Uns32P	ASUns32P

#define MAXInt8 	ASMAXInt8
#define MINInt8 	ASMINInt8
#define MAXInt16	ASMAXInt16
#define MINInt16	ASMINInt16
#define MAXInt32	ASMAXInt32
#define MINInt32	ASMINInt32

#define MAXUns8		ASMAXUns8	
#define MINUns8		ASMINUns8	
#define MAXUns16	ASMAXUns16	
#define MINUns16	ASMINUns16	
#define MAXUns32	ASMAXUns32	
#define MINUns32	ASMINUns32	

#ifndef _WIN32    /* We cannot do this for WIN32 since it has a different size for boolean */
#define boolean ASBool
#endif

#define os_size_t			ASSize_t
#endif /* ASUSE_OBSOLETE_TYPES */

#define FixedPointP		..Use.ASFixedPointP.instead..
#define FixedRectP		..Use.ASFixedRectP.instead..
#define FixedQuad		..Use.ASFixedQuad.instead..
#define FixedQuadP		..Use.ASFixedQuadP.instead..
#define FixedMatrix		..Use.ASFixedMatrix.instead..
#define FixedMatrixP	..Use.ASFixedMatrixP.instead..



/**
	Enumeration function for ASEnumExtensions. 
	@param extension The ASExtension for a client. 
	@param clientData User-supplied data that was passed in 
	the call to ASEnumExtensions.
	@return If false, enumeration halts and the ASExtension on which 
	enumeration halted is returned. If true, enumeration continues. 
	
	@see ASEnumExtensions 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASExtensionEnumProc)(
	ASExtension extension,
	void *clientData);

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_CoreExpT */
