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

 CosExpT.h

 - Types, macros, structures, etc. required to use the Cos HFT.

*********************************************************************/

#ifndef _H_CosExpT
#define _H_CosExpT

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

/** */
typedef ASInt32 CosCryptVersion;
/** */
typedef ASUns16 CosGeneration;
/** */
typedef ASUns32 CosID;
/** 0 is not valid */
typedef ASUns32 CosHashCode;
/** */
typedef ASInt32 CosStreamStartAndCode;
/** -1 for none, error, or other special meaning */
typedef ASInt32 CosByteMax;
/**
	Used for an array of bytes in CosDocGetID. 
	@see CosDocGetID
*/
typedef ASUns8  CosByte;  /* used for arrays of bytes */

/* Type codes for Cos objects */

enum {
	/** A NULL object, or an invalid object. */
	CosNull		= 0,
	/** An integer object. */
	CosInteger	= 1,
	/** A fixed number object.  deprecated type-name for real numbers */
	CosFixed	= 2,	
	/** preferred type-name for real numbers */
	CosReal		= 2,
	/** An ASBool object. */
	CosBoolean	= 3,
	/** A name object. */
	CosName		= 4,
	/** A string object. */
	CosString	= 5,
	/** A dictionary object. */
	CosDict		= 6,
	/** An array object. */
	CosArray	= 7,
	/** A stream object. */
	CosStream	= 8
};

/**
	Constants that specify a Cos object's type (string, number, 
	dictionary, and so on). 

	@see CosObjGetType 
*/
typedef ASInt32 CosType;

/* Opaque. Represents an Acrobat document */
/** */
typedef struct _t_CosDoc *CosDoc;

/* COSSRCIMPL must be defined only when this header file is included
   to construct the COS implementation. Client code that includes this
   header file must not define COSSRCIMPL */
#ifndef COSSRCIMPL

	#if PLUGIN || ACROBAT_LIBRARY
		typedef OPAQUE_64_BITS CosObj;
	#else
		typedef struct _t_CosObj {ASInt32 a,b;} CosObj;
	#endif

	typedef OPAQUE_64_BITS CosObjCollection;

#endif


/**
	Callback for CosObjEnum, CosDocEnumIndirect, and PDDocEnumOCGs. 
	Called once for each component of a composite Cos object 
	(dictionary, array, and stream). 
	@param obj Dictionary - Key. Array - Array element. Stream 
	 -  The stream's dictionary (the whole thing, not one key 
	at a time). 
	@param value Dictionary - The value associated with the 
	Key. Array - a null Cos object. Stream - a null Cos object. 
	For CosDocEnumIndirect and PDDocEnumOCGs, this is always 
	the NULL Cos object. 
	@param clientData User-supplied data that was passed in 
	the call to CosObjEnum, CosDocEnumIndirect, or PDDocEnumOCGs.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see CosObjEnum 
	@see CosDocEnumIndirect 
	@see PDDocEnumOCGs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosObjEnumProc)(
			CosObj obj, CosObj value, void *clientData);

	/*	The data in a CosStream may be filtered and encrypted.
	If CosOpenRaw is specified to CosStreamOpenStm, the data will
	be delivered by the returned ASStm as it is in the source ASStm,
	neither filtered nor decrypted.
	If CosOpenUnfiltered is specified, the data will be decrypted
	but not filtered.
	If CosOpenFiltered is specified, the data will be both
	decrypted and filtered.  (This is the usual case.)
 */
enum {
	/**	The data will be decrypted but not filtered. 
	
*/
	cosOpenRaw,
	/**
*/
	cosOpenUnfiltered,
	/**	The data will be both decrypted and filtered. (This is the 
	usual case.) */
	cosOpenFiltered,
	/** */
	cosOpenRawIgnoreFKey,
	/** */
	cosOpenUnfilteredIgnoreFKey,
	/** */
	cosOpenFilteredIgnoreFKey
};
/**
	Constants that specify whether filters and decryption should 
	be applied to the stream's data. 
	@see CosStreamOpenStm 
*/
typedef ASEnum8 CosStreamOpenMode;

/* CosDocOpenParams.openFlags bitfield values */
/** */
#define kCosDocOpenDoRepair	0x0001

/* CosDocOpenParams
** Used in the call to CosDocOpenWithParams.
*/
/**
	Parameters used when saving a file using CosDocOpenWithParams. 
	
	@see CosDocOpenWithParams 
*/
typedef struct _t_CosDocOpenParams {
	/** size of this struct */
	ASSize_t	size;	
	/** bitfield of kCosDocOpen flags */
	ASFlagBits	openFlags;	
	/** may be NULL if using default fileSys */
	ASFileSys	fileSys;	
	/** must be provided */
	ASPathName	pathName;	
	/** expected header string, i.e. "%ADF-"; if NULL, assumes "%PDF-" */
	const char *headerString;	
	} CosDocOpenParamsRec, *CosDocOpenParams;

/* Flags for CosDocCreate createFlags parameter */
/** */
#define cosDocCreateInfoDict	0x01

/* Flags for CosDocSave saveFlags parameter */
/** delete unreferenced objects before save */
#define cosSaveGarbageCollect	0x01	
/** write all objects, not just changes */
#define cosSaveFullSave			0x02
/** ok to store binary in file */
#define cosSaveBinaryOK			0x08	
/** If there are any object streams, write them
	in a way that is hidden from PDF 1.4 
	(and earlier) viewers. This is used for
	hybrid files, for example. */
#define cosSaveConcealObjStreams	0x10	
/** */
typedef ASFlagBits CosDocSaveFlags;

/* CosDocSaveParams
** Used in call to CosDocSaveToFile */
/**
	Parameters used when saving a file using CosDocSaveToFile 
	and CosDocSaveWithParams. 
	@see CosDocSaveToFile 
	@see CosDocSaveWithParams 
*/
typedef struct _t_CosDocSaveParams {
	/** size of this struct */
	ASSize_t		size;
	/** complete header string, such as, "%ADF-1.0" */
	char			*header;
	/** The encryption key to pass into the PDCryptHandler 
	if security has been set on the document. */
	char			*cryptData;
	/** Length of the encryption key, in bytes. 
	Cannot be greater than 5. */
	ASTArraySize	cryptDataLen;
	/** Progress monitor. Use AVAppGetDocProgressMonitor 
	to obtain the default progress monitor. */
	ASProgressMonitor mon;	
	/**  Pointer to user-supplied data to 
	pass to mon each time it is called. */
	void			*monClientData;	
	/** The Cos cryptographic version - the 
	version of the algorithm that is used to encrypt and decrypt 
	document data. cryptVersion equal to 0 is treated as cryptVersion 
	equal to 1 to maintain backward compatibility. */
 	CosCryptVersion	cryptVersion;
} CosDocSaveParamsRec, *CosDocSaveParams;


/**
	Callback for CosDocEnumEOFs. Called once for each position in a CosDoc
	after a %%EOF keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. See CosDocEnumEOFs for more details.
	@param cosDoc The CosDoc in which the EOF is found. 
	@param fileOffset The 31-bit offset into the file directly following 
	the %%EOF keyword. If the procedure is called more than 
	once, the file positions passed to it are in decreasing 
	order (that is, the EOF positions are treated as 'rollback 
	points'). NOTE: The precise value passed to the procedure 
	is not defined. It is at least one byte past the %%EOF keyword, 
	but may include one or more white space characters. When 
	the procedure is called only once, there is no guarantee 
	that fileOffset is the same as the length of the file.
	@param clientData User-supplied data that was passed in 
	the call to CosDocEnumEOFs.
	@return true to continue enumeration, false to halt the enumeration. 
	
	@see CosDocEnumEOFs 
	@see CosDocEnumEOFsProc64
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosDocEnumEOFsProc)(CosDoc cosDoc,
	ASFileOffset fileOffset, void *clientData);

typedef ASInt64 ASFileOffset64;

/**
	Callback for CosDocEnumEOFs64. Called once for each position in a CosDoc
	after a %%EOF keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. See CosDocEnumEOFs for more details.
	This is similar to CosDocEnumEOFsProc, except that the
	fileOffset parameter is a 64-bit value instead of a 31-bit value.
	@param cosDoc The CosDoc in which the EOF is found. 
	@param fileOffset The 64-bit offset into the file directly following 
	the %%EOF keyword.  
	@param clientData User-supplied data that was passed in 
	the call to CosDocEnumEOFs64.
	@return true to continue enumeration, false to halt the enumeration. 
	
	@see CosDocEnumEOFs64
	@see CosDocEnumEOFsProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosDocEnumEOFsProc64)(CosDoc cosDoc,
	ASFileOffset64 fileOffset, void *clientData);

/**
	Callback for PDDocSaveParams used by PDDocSaveWithParams. 
	Use this to get information about Cos objects of interest 
	while a PDDoc is being saved. 
	@param obj IN/OUT? The CosObj found. 
	@param fileOffset IN/OUT? The offset of obj into the PDF file. 
	
	@param length IN/OUT? length of obj. 
	@param clientData IN/OUT? Pointer to user-supplied data passed 
	in the offsetProcClientData parameter of the PDDocSaveParams 
	structure. 
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *CosObjOffsetProc)(
	CosObj obj, ASFilePos fileOffset, ASArraySize length, void *clientData);

typedef ACCBPROTO1 void (ACCBPROTO2 *CosObjOffsetProc64)(
	CosObj obj, ASFilePos64 fileOffset, ASUns64 length, void *clientData);

/**
	Callback in PDDocPreSaveInfo, which is used by the PDDocPreSaveProc 
	callback. Use this callback to set a flag in each CosObj 
	that you care about, so that you will be called back during 
	the PDDoc's save and given the Cos object's offset and length. 
	After a PDF file is saved, the Cos objects previously obtained 
	are no longer valid. 
	@param obj IN/OUT? The CosObj marked. 
	@param set IN/OUT? true to set the flag to be called back during 
	the save, false otherwise. 
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 ASBool  (ACCBPROTO2 *CosObjSetCallbackFlagProc)(
	CosObj obj, ASBool set);

/** Prototype for string encryption/decryption callback. This is part of Crypt Filter mechanism */
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *CosCryptStringProc)(CosDoc dP, ASAtom filterName, char *dest, char *src, 
		ASInt32 dstSize, ASInt32 srcLength, ASUns32 genNumber, ASUns32 objNumber);

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_CosExpT */
