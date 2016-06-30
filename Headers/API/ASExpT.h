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

 ASExpT.h

 - Types, macros, structures, etc. required to use the AcroSupport HFT.

*********************************************************************/

#ifndef _H_ASExpT
#define _H_ASExpT

#include "CoreExpT.h"
#include "acroassert.h"

#if MAC_PLATFORM
#include <QD/Quickdraw.h>
#include <HIToolbox/MacWindows.h>
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

#ifdef __cplusplus
extern "C" {
#endif

/* define types based on SDK_LEVEL */

/** File access modes used to specify how a file can be used when it is open. Not all modes can
	be specified individually; ASFILE_CREATE can be used only in conjunction with
	ASFILE_READ or ASFILE_WRITE. In addition, it is acceptable to specify
	ASFILE_READ and ASFILE_WRITE together, by OR-ing the two constants.
	ASFILE_SERIAL and ASFILE_LOCAL (present only in version 3.0 or later) are hints that
	help the Acrobat viewer optimize access to the file; they must be OR-ed with one or more
	of the other constants.
	Values:
	- ASFILE_READ Open the file for reading.
	- ASFILE_WRITE Open the file for writing.
	- ASFILE_CREATE Create the file if it does not exist.
	- ASFILE_SERIAL A hint indicating that the file will be accessed sequentially.
	- ASFILE_LOCAL A hint indicating that a local copy of the file will be needed.
	@see ASFileSysOpenFile
	@see ASFileReopen
*/
typedef ASUns16 ASFileMode;

/**
	A file position value for use in callback procedures - cannot exceed 2GB. 
	@see CosObjOffsetProc
*/
typedef ASUns32 ASFilePos;

/**
	A file offset value for use in callback procedures. 
	@see CosDocEnumEOFsProc
*/
typedef ASInt32 ASFileOffset; 

/** absolute position within a file - can exceed 2GB */
typedef ASUns64 ASFilePos64;

/**
	An error code value for use in ASFile and ASFileSys methods 
	and callbacks. Introduced in Acrobat 6.0. 
*/
typedef ASInt32 ASErrorCode;

/**
	A byte count value for use in ASProcStmRdExHandler and ASFileSysItemProps. 	
	@see ASUUIDGenFromName
*/
typedef ASUns32 ASByteCount;

/**
	An array size value for use in callback procedures. 
	@see AVDocSelectionAcquireQuadsProc 
	@see CosObjOffsetProc
*/
typedef ASUns32 ASArraySize;  

/**
	A flag-bits value for use . 
	@see ASFileSetMode 
	@see CosDocCreate 
	@see CosDocSaveToFile 
	@see CosDocSaveWithParams 
	@see HFTReplaceEntry 
	@see HFTReplaceEntryEx 
	@see PDAnnotInfo 
	@see ASFileSysGetFileFlags 
	@see ASFileSysGetStatusProc 
	@see PDAnnotHandlerGetAnnotInfoFlagsProc
*/
typedef ASUns32 ASFlagBits;

/** can only give values up to 4Gb */
typedef ASUns32 ASDiskSpace;  

/** */
typedef ASUns32 ASlFileMode;

/** */
typedef ASUns32 ASMaskBits;

/** */
typedef ASInt32 ASDuration;

/**
	An HFT version number. 
	@see ASExtensionMgrGetHFT 
	@see HFTServerProvideHFTProc 
	@see HFTGetVersion
*/
typedef ASUns32 ASVersion; 

/**  may not be larger than int16! */
typedef ASUns16 ASSmallBufferSize;  

/**
	A numeric count value. 
	@see ASGetSecs 
	@see ASIsValidUTF8 
	@see AVAppCreateIconBundle6 
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews
*/
typedef ASUns32 ASCount;  

/** */
typedef ASUns8  ASByte;

/* types that may transition to unsigned types */

/** signed int, but negative never used */
typedef ASInt16 ASSmallCount; 

/**
	A numeric count value for use in I/O methods and data structures. 
	
	@see ASFileGetEOF 
	@see ASFileGetPos 
	@see ASFilePushData 
	@see ASFileSetPos 
	@see ASFileCompletionProc 
	@see ASFileSysMReadRequestProc
*/
typedef ASInt32 ASTFilePos; 

/**
	A numeric array size value for use in AS and Cos-level I/O 
	methods and data structures. 
	@see numerous 
	@see ASFileCompletionProc 
	@see ASFileSysGetNameProc 
	@see ASFileSysMReadRequestProc 
	@see ASStmProc
*/
typedef ASInt32 ASTArraySize; 

/**
	Cryptographic version number. 
	@see CosCryptGetVersion 
	@see CosDecryptGetMaxKeyBytes 
	@see CosEncryptGetMaxKeyBytes
*/
typedef ASInt32 ASTVersion; 

/**
	A numeric count value for use in stream methods. 
	@see ASIsValidUTF8 
	@see ASStmFlush 
	@see ASStmRead 
	@see ASStmWrite 
	@see CosCopyStringValue 
	@see CosDocGetID 
	@see CosStreamPos 
	@see CosStringValue 
	@see CosStringValueSafe 
	@see HFTNew 
	@see ASStmProc
*/
typedef ASInt32 ASTCount; 

#if !defined(ACRO_SDK_LEVEL)  || (ACRO_SDK_LEVEL < 2)
#pragma message("Warning: Using older Acrobat SDK.  Define ACRO_SDK_LEVEL to 0x00070000") 

/**
	A coordinate for a point in device space, for use in mouse-click 
	callbacks. Values are conditionally compiled as 16-bit or 
	32-bit integers, depending on the Acrobat version level. 
	
	A numeric count value. 
	@see ASGetSecs 
	@see ASIsValidUTF8 
	@see AVAppCreateIconBundle6 
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews
*/
typedef ASInt16 ASCoord;  
#else

/**
	A coordinate for a point in device space, for use in mouse-click 
	callbacks. Values are conditionally compiled as 16-bit or 
	32-bit integers, depending on the Acrobat version level. 
	
	A numeric count value. 
	@see ASGetSecs 
	@see ASIsValidUTF8 
	@see AVAppCreateIconBundle6 
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews
*/
typedef ASInt32 ASCoord;  


#if 0
 /* potential future transitions */
typedef ASUns16 ASSmallCount; 
typedef ASUns32 ASTFilePos; 
typedef ASUns32 ASTArraySize; /*elements in array */
typedef ASUns32 ASTVersion; 
typedef ASUns32 ASTCount;  
#endif

#endif /* ...ACRO_SDK_LEVEL < 2 */


/** */
typedef ASEnum8 ASErrSeverity;


/** Stream object definition. See ASStream.h
	A data stream that may be a buffer in memory, a file, or an arbitrary user-written
	procedure. Typically used to extract data from a PDF file. When writing or extracting
	data streams, the ASStm must be connected to a Cos stream.
	@see ASFileStmRdOpen
	@see ASFileStmWrOpen
	@see ASMemStmRdOpen
	@see ASProcStmRdOpen
	@see CosStreamOpenStm
	@see ASStmClose
*/
typedef struct _t_ASStmRec ASStmRec, *ASStm;
#define ASCRYPTSTM_EOF (-1)

/* ASCryptStm is essentially ASStm object for security handler plug-in.
** ASCryptStm callbacks are trapped by PD layer and converted into ASStm.
** See PDRegisterCryptFilter() definition on how ASCryptStm is used.
** 
** These #define are used for setting modeFlag in ASCryptStmRec structure. For decryption, read flag, 
** for encryption, write flag should be set. EOF and error flag should also be set as appropriate.
*/
#define	ASCryptStmModeRead	0x0001
#define	ASCryptStmModeWrite	0x0002
#define	ASCryptStmModeEOF	0x0004
#define	ASCryptStmModeError	0x0008

/**
	An ASStm object cover used for a cryptographic filter's 
	stream callbacks. 
	@see PDCryptFilterStreamProc 
*/
typedef struct _t_ASCryptStmRec *ASCryptStm;

/* Prototypes for ASCryptStm callback routines */

/**
	Callback for ASCryptStm. Called by getc when buffer is empty. 
	Called only during decryption (that is, reading from the 
	stream, not writing). 
	@param pistm The security stream to fill.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmFResetProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFilBufProc)(ASCryptStm pistm);

/**
	Callback for ASCryptStm. Called by putc when buffer is full. 
	Called only during encryption (that is, writing to the stream, 
	not reading). 
	@param ch The character being put to the full stream. 
	
	@param stm The security stream that is full.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmFResetProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFlsBufProc)(ASInt32 ch, ASCryptStm stm);

/**
	Callback for ASCryptStm. Goes back one character in the 
	input stream, undoing a character get operation. Called 
	only during decryption (that is, reading from the stream, 
	not writing). 
	@param ch The character being put to the stream. 
	@param stm The security stream to which the character 
	is put.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmFResetProc 
*/
typedef ASInt32	(*ASCryptStmUnGetcProc)(ASInt32 ch, ASCryptStm stm);

/**
	Callback for ASCryptStm. Flushes a dirty buffer if necessary. 
	
	@param stm The security stream to be flushed.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmFResetProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFFlushProc)(ASCryptStm stm);

/**
	Callback for ASCryptStm. Closes a security stream. 
	@param stm The security stream to be closed.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmFResetProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFCloseProc)(ASCryptStm stm);

/**
	Callback for ASCryptStm. Resets a security stream, discarding 
	any buffered data. Called only during encryption (that is, 
	writing to the stream, not reading). 
	@param stm The security stream to be reset.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFPutEOFProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFResetProc)(ASCryptStm stm);

/**
	Callback for ASCryptStm. Puts an end-of-file marker to a 
	security stream. 
	@param stm The security stream to receive the EOF.
	@return 0 when successful, otherwise a non-zero error code. 
	@see ASCryptStmFCloseProc 
	@see ASCryptStmFFlushProc 
	@see ASCryptStmFilBufProc 
	@see ASCryptStmFlsBufProc 
	@see ASCryptStmFResetProc 
	@see ASCryptStmUnGetcProc 
*/
typedef ASInt32	(*ASCryptStmFPutEOFProc)(ASCryptStm stm);

/** Callback procs for ASCryptStm */
typedef struct {
	/** called by getc when buffer is empty */
	ASCryptStmFilBufProc EmptyBuff;
	/** called by putc when buffer is full */
	ASCryptStmFlsBufProc FullBuff;
	/** back up input stream */
	ASCryptStmUnGetcProc UnGetCh;
	/** flush dirty buffer if necessary */
	ASCryptStmFFlushProc FlushBuff;
	/** close stream */
	ASCryptStmFCloseProc Close;
	/** discard any buffered data */
	ASCryptStmFResetProc Reset;	
	/** Put EOF marker */
	ASCryptStmFPutEOFProc PutEOF;
} ASCryptStmProcs;

/**
	An ASStm object cover used for a cryptographic filter's 
	stream callbacks. 
	@see PDCryptFilterStreamProc 
*/
typedef struct _t_ASCryptStmRec {
	
	/** The number of characters remaining in the buffer.  */
	ASInt32 count;			
	
	/** The next character to get or put.  */
	char *currentPointer;	
	
	/** The base of buffer (if any) */
	char *basePointer;		
	
	/** Flag to indicate mode
		- ASCryptStmModeRead 0x0001 (decryption) 
		- ASCryptStmModeWrite 0x0002 (encryption) 
		- ASCryptStmModeEOF 0x0004 
		- ASCryptStmModeError 0x0008 
	*/
	ASUns32 modeFlag;		
	
	/** Handlers for security stream access. */
	ASCryptStmProcs *procs;
	
	/** The base ASStm object. */
	ASStm	baseStm;
	
	/** number of bytes being requested for decryption.  */
	ASInt32 nBytesWanted;	
	
	/** A pointer to arbitrary user-defined data.*/
	void *clientData;
} ASCryptStmRec;

/**
	Callback for use by ASProcStmRdOpenEx and ASProcStmWrOpen.
	This should place data in the buffer specified by the
	parameter data.

	If your procedure reads data from a file, it is generally
	quite inefficient to open the file, read the bytes, then
	close the file each time bytes are requested. Instead, consider
	opening the file the first time bytes are requested from
	it, reading the entire file into a secondary buffer, and
	closing the file. When subsequent requests for data from
	the file are received, simply copy data from the secondary
	buffer, rather than re-opening the file.
	@param data (Filled by the callback) Buffer into which
	your procedure must place up to the number of bytes specified
	by nData.
	@param nData Number of bytes to read from the stream and
	place into data.
	@param clientData User-supplied data that was passed in
	the call to ASProcStmRdOpenEx or ASProcStmWrOpen.
	@return The number of bytes actually read or written. This should be
	equal to the nData parameter unless EOF has been reached, an error
	has occurred, or this is short block of data just before EOF. If
	EOF has been reached, a zero should be returned. If an error has
	occurred, a negative value should be returned.
	@see ASProcStmDestroyProc
	@see ASProcStmRdOpenEx
	@see ASProcStmWrOpen
*/
typedef ACCBPROTO1 ASTCount (ACCBPROTO2 *ASStmProc)(char *data,
	ASTArraySize nData, void *clientData);


/**
	Callback for use by ASProcStmWrOpen and ASProcStmRdOpenEx.

	Called at end of stream so you can do clean up and free
	allocated memory.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to ASProcStmWrOpen or ASProcStmRdOpenEx.
	@see ASStmProc
	@see ASProcStmWrOpen
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASProcStmDestroyProc)(void *clientData);

/**
	Callback for use by ASProcStmRdOpenEx.

	Called to set the stm position to a new location. Maybe NULL if the stream
	cannot be set to a new position. ASProcStmSeekProc and ASProcStmGetLength must
	be provided together. If either is NULL, the stream will not be set to a new
	position.
	@param newPos IN
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to ASProcStmRdOpenEx.
	@see ASStmProc
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASProcStmSeekProc)(ASFilePos64 newPos, void *clientData);

/**
	Callback for use by ASProcStmRdOpenEx.

	Called to get the length of the stream. Maybe NULL if the stream cannot be
	set to a new position. ASProcStmSeekProc and ASProcStmGetLength must be
	provided together. If either is NULL, the stream will not be set to a new
	position.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to ASProcStmRdOpenEx.
	@return The length of the stream in bytes.
	@see ASStmProc
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 ASFilePos64 (ACCBPROTO2 *ASProcStmGetLength)(void *clientData);

/** For use by ASProcStmRdOpenEx */
typedef struct _s_ASProcStmRdExHandler
{
	/** Set to sizeof(ASProcStmRdExHandlerRec) */
	ASByteCount size;
	/** */
	ASStmProc readProc;
	/** */
	ASProcStmDestroyProc destroyProc;
	/** */
	ASProcStmSeekProc seekProc;
	/** */
	ASProcStmGetLength getLengthProc;
	/** The size of the buffer to use for the stream. If this field
		is missing, the default is 65535. If this field is present and
		has a value of 0, then the default is implentation-specific.
	*/
	ASByteCount bufSize;
} ASProcStmRdExHandlerRec, *ASProcStmRdExHandler;

/** */
#define HFT_ERROR_NO_VERSION (0xFFFFFFFF)

/**
	A data structure to pass to an HFT server to create a new 
	HFT. New in Acrobat 6.0. 
	@see HFTNewEx 
*/
typedef struct _t_HFTData
{
	/** Set to sizeof(HFTDataRec) */
	ASUns32 size; 
	/** The number of entries in the new HFT. This determines 
		the number of methods that the HFT can contain; 
		each method occupies one entry. 
	*/
	ASCount numSelectors;
	/** The version number. */
	ASVersion version;
} HFTDataRec;
typedef const HFTDataRec *HFTData;

/** Each HFT is serviced by an HFT server. The HFT server is responsible for handling
	requests to obtain or destroy its HFT. An HFTServer is an object that manages several versions of an
	HFT for different clients which may have been compiled with different notions of the HFT's API.
	@see HFTServerNew
	@see HFTServerDestroy
*/
typedef struct _t_HFTServer *HFTServer;

/**
	Callback for an HFT server. Returns an HFT with the specified 
	version number. If the HFT has not yet been created, create 
	and return it. If the HFT already exists, do not create 
	a new copy of it; simply return the existing copy. 

	NOTE: The version numeric type has changed in Acrobat 6.0. 
	
	@param hftServer The HFT server associated with this proc. 	
	@param version The HFT version being requested. 
	@param rock User-supplied data passed in the call 
	to HFTServerNew.
	@return The requested version of the HFT. 
	@see HFTServerNew 
*/
typedef ACCBPROTO1 HFT (ACCBPROTO2 *HFTServerProvideHFTProc)(HFTServer hftServer, ASVersion version, void *rock);


/**
	Callback for an HFT server. Destroys the specified HFT (
	for example, by calling HFTServerDestroy). 
	@param hftServer IN/OUT? The HFT server associated with this destroy 
	proc. 
	@param rock IN/OUT? User-supplied data that was passed in 
	the call to HFTServerNew. 
	@see HFTServerNew 
	@see HFTDestroy 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *HFTServerDestroyProc)(HFTServer hftServer, void *rock);

/**
	The ASFixed type is a 32-bit quantity representing a rational 
	number with the high (low on little-endian machines) 16 
	bits representing the number's mantissa and the low (high) 
	16 bits representing the fractional part. The definition 
	is platform-dependent. 

	ASFixedP is a pointer to an ASFixed. 

	Addition, subtraction, and negation with ASFixed types can 
	be done with + and -, unless you care about overflow. (Overflow 
	in ASFixed-value operations is indicated by the values fixedPositiveInfinity 
	and fixedNegativeInfinity.) 
	@see ASFixedDiv 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedMul 
	@see ASFixedToCString
*/
typedef ASInt32 ASFixed, *ASFixedP;

/*
** CONVERSION to/from standard C types and ASFixed
*/

/** */
#define ASFixedPosInf ASMAXInt32

/** */
#define ASFixedNegInf ASMINInt32

/*#define ASInt32ToFixed(i)	((ASFixed)(i) << 16)
 note that an expression argument get evaluated several times and this should be avoided */
/**
	Converts an ASInt32 to a fixed point number. 
	@param i The ASInt32 to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFloatToFixed 
	@see ASFixedToFloat 
	@see ASInt16ToFixed 
*/
#define ASInt32ToFixed(i)     (((i) < (-32767)) ? ASFixedNegInf : ((i) > 32767) ? ASFixedPosInf : ((ASFixed)(i) << 16))

/**
	Converts a fixed point number to an ASInt32, rounding the 
	result. 
	@param f The fixed point number to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
*/
#define ASFixedRoundToInt32(f)	((ASInt32) (((f) + fixedHalf) >> 16)) 

/**
	Converts a fixed point number to an ASInt32, truncating 
	the result. 
	@param f The fixed point number to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt16 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
*/
#define ASFixedTruncToInt32(f)	((ASInt32) ((f) >> 16)) 

/**
	Converts an ASInt16 to a fixed point number. 
	@param i The ASInt16 to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFloatToFixed 
	@see ASInt32ToFixed 
*/
#define ASInt16ToFixed(i)	((ASFixed)(i) << 16)

/**
	Converts a fixed point number to an ASInt16, rounding the 
	result. 
	@param f The fixed point number to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt32 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
*/
#define ASFixedRoundToInt16(f)	((ASInt16) (((f) + fixedHalf) >> 16))

/**
	Converts a fixed point number to an ASInt16, truncating 
	the result. 
	@param f The fixed point number to convert.
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedToFloat 
	@see ASFixedTruncToInt32 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
*/
#define ASFixedTruncToInt16(f)	((ASInt16) ((f) >> 16))

/* Float <-> ASFixed */
#ifdef __cplusplus

/**
	The ASFixed type is a 32-bit quantity representing a rational 
	number with the high (low on little-endian machines) 16 
	bits representing the number's mantissa and the low (high) 
	16 bits representing the fractional part. The definition 
	is platform-dependent. 

	ASFixedP is a pointer to an ASFixed. 

	Addition, subtraction, and negation with ASFixed types can 
	be done with + and -, unless you care about overflow. (Overflow 
	in ASFixed-value operations is indicated by the values fixedPositiveInfinity 
	and fixedNegativeInfinity.) 
	@see ASFixedDiv 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedMul 
	@see ASFixedToCString
*/
inline ASFixed FloatNToFixed(const double& x) {return (ACROASSERT(x>=0),((x)>32767)?ASFixedPosInf:(ASFixed)(((x)*65536.0f +0.5f))); }
/**
	The ASFixed type is a 32-bit quantity representing a rational 
	number with the high (low on little-endian machines) 16 
	bits representing the number's mantissa and the low (high) 
	16 bits representing the fractional part. The definition 
	is platform-dependent. 

	ASFixedP is a pointer to an ASFixed. 

	Addition, subtraction, and negation with ASFixed types can 
	be done with + and -, unless you care about overflow. (Overflow 
	in ASFixed-value operations is indicated by the values fixedPositiveInfinity 
	and fixedNegativeInfinity.) 
	@see ASFixedDiv 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedMul 
	@see ASFixedToCString
*/
inline ASFixed FloatIToFixed(const double& x) {return (ACROASSERT((x)*65536.0f==(double)(((ASFixed)x)<<16)),((x)<(-32767))?ASFixedNegInf:((x)>32767)?ASFixedPosInf:((ASFixed)x)<<16); }
#else

/** FloatN -> ASFixed (for when you know float numbers are non-negative) */
#define FloatNToFixed(x) ((x)<(-32767))?ASFixedNegInf:((x)>32767)?ASFixedPosInf:(((ASFixed)(((x)*65536.0f +0.5f)))

/** FloatI <-> ASFixed (for when you know float numbers are ints) */
#define FloatIToFixed(x) ((x)>32767)?ASFixedPosInf:(((ASFixed)x)<<16)


#endif /* __cplusplus */

/* Backward compatibility #defines. DO NOT use these in new code */
#define Int32ToFixed ASInt32ToFixed
#define FixedRoundToInt32 ASFixedRoundToInt32
#define FixedTruncToInt32 ASFixedTruncToInt32
#define Int16ToFixed ASInt16ToFixed
#define FixedRoundToInt16 ASFixedRoundToInt16
#define FixedTruncToInt16 ASFixedTruncToInt16

/** @name ASFixedConstants
*	various useful ASFixed constants
*/
/*@{*/

/** */
#define fixedZero					((ASFixed) 0x00000000L)

/** */
#define fixedHundredth				((ASFixed) 0x0000028FL)

/** */
#define fixedSixteenth				((ASFixed) 0x00001000L)

/** */
#define fixedTwelfth				((ASFixed) 0x00001555L)

/** */
#define fixedTenth					((ASFixed) 0x00001999L)

/** */
#define fixedEighth					((ASFixed) 0x00002000L)

/** */
#define fixedQuarter 				((ASFixed) 0x00004000L)

/** */
#define fixedThird					((ASFixed) 0x00005555L)

/** */
#define fixedHalf 					((ASFixed) 0x00008000L)

/** */
#define fixedTwoThirds				((ASFixed) 0x0000AAAAL)

/** */
#define fixedThreeQuarters 			((ASFixed) 0x0000C000L)

/** */
#define fixedPi4 					((ASFixed) 0x0000c910L)

/** */
#define fixedSevenEighths			((ASFixed) 0x0000E000L)

/** */
#define fixedOne1					((ASFixed) 0x0000ffffL)

/** */
#define fixedOne 					((ASFixed) 0x00010000L)

/** */
#define fixedOneAndQuarter			((ASFixed) 0x00014000L)

/** */
#define fixedFourThirds				((ASFixed) 0x00015555L)

/** */
#define fixedSqrtTwo				((ASFixed) 0x00016A0AL)

/** */
#define fixedThreeHalves			((ASFixed) 0x00018000L)

/** */
#define fixedOneAnd3Qtr				((ASFixed) 0x0001C000L)

/** */
#define fixedPi2 					((ASFixed) 0x00019220L)

/** */
#define fixedGolden 				((ASFixed) 0x00019e37L)

/** */
#define fixedTwo					((ASFixed) 0x00020000L)

/** */
#define fixedThree					((ASFixed) 0x00030000L)

/** */
#define fixedFour 					((ASFixed) 0x00040000L)

/** */
#define fixedFive					((ASFixed) 0x00050000L)

/** */
#define fixedSix					((ASFixed) 0x00060000L)

/** */
#define fixedSeven					((ASFixed) 0x00070000L)

/** */
#define fixedEight					((ASFixed) 0x00080000L)

/** */
#define fixedNine					((ASFixed) 0x00090000L)

/** */
#define fixedTen					((ASFixed) 0x000A0000L)

/** */
#define fixedEleven					((ASFixed) 0x000B0000L)

/** */
#define fixedTwelve					((ASFixed) 0x000C0000L)

/** */
#define fixedSixteen				((ASFixed) 0x00100000L)

/** */
#define fixedThirtyTwo				((ASFixed) 0x00200000L)

/** */
#define fixedFifty 					((ASFixed) 0x00320000L)

/** */
#define fixedSeventyTwo				((ASFixed) 0x00480000L)

/** */
#define fixedNinety 				((ASFixed) 0x005a0000L)

/** */
#define fixedHundred				((ASFixed) 0x00640000L)

/** */
#define fixedHundredFifty			((ASFixed) 0x00960000L)

/** */
#define fixedOneEighty				((ASFixed) 0x00b40000L)

/** */
#define fixedTwoSeventy				((ASFixed) 0x010e0000L)

/** */
#define fixedFiveHundred			((ASFixed) 0x01F40000L) 

/** */
#define fixedThousand				((ASFixed) 0x03E80000L) 

/** */
#define fixedTenThousand			((ASFixed) 0x27100000L)

/** */
#define fixedNegativeInfinity		ASFixedNegInf

/** */
#define fixedPositiveInfinity 		ASFixedPosInf
/*@}*/


#ifndef __ASTypes__ /* Newer ASTypes.h defines these types */
/**
	Matrix containing fixed numbers. 
*/
typedef struct _t_ASFixedMatrix
{

	/** */
	ASFixed a;

	/** */
	ASFixed b;

	/** */
	ASFixed c;

	/** */
	ASFixed d;

	/** */
	ASFixed h;

	/** */
	ASFixed v;
}
ASFixedMatrix, *ASFixedMatrixP;


/**
	Point (in two-dimensional space) represented by two fixed 
	numbers. 
*/
typedef struct _t_ASFixedPoint
{

	/** */
	ASFixed h;

	/** */
	ASFixed v;
}
ASFixedPoint, *ASFixedPointP;


/**
	A rectangle represented by the coordinates of its four sides. 
	In the Acrobat viewer, a rectangle differs from a quadrilateral 
	in that the former must always have horizontal and vertical 
	sides, and opposite sides must be parallel. 
*/
typedef struct _t_ASFixedRect
{

	/** */
	ASFixed left;

	/** */
	ASFixed top;

	/** */
	ASFixed right;

	/** */
	ASFixed bottom;
}
ASFixedRect, *ASFixedRectP;
#endif /* ndef __ASTypes */

/**
	A structure that represents a quadrilateral defined by four 
	corner points. 

	NOTE: The coordinate numeric types changed in Acrobat 6. 
	
	@see AVPageViewInvertQuad
*/
typedef struct _t_Quad {
	/** */
	ASCoord tlh, tlv;
	/** */
	ASCoord trh, trv;
	/** */
	ASCoord blh, blv;
	/** */
	ASCoord brh, brv;
} Quad, *QuadP;


/**
	Quadrilateral represented by four fixed points (one at each 
	corner). In the Acrobat viewer, a quadrilateral differs 
	from a rectangle in that the latter must always have horizontal 
	and vertical sides, and opposite sides must be parallel. 
*/
typedef struct _t_ASFixedQuad {
	/** */
	ASFixedPoint tl, tr, bl, br;
} ASFixedQuad, *ASFixedQuadP;


/** Definition of ASFract */
typedef long ASFract;

/** Definition of ASReal */
typedef float ASReal;

#ifndef __ASTypes__ /* Newer ASTypes.h defines these types */
/** */
typedef struct _t_ASRealPoint {
	/** */
	ASReal h, v;
} ASRealPoint;

/** */
typedef struct _t_ASRealRect {
	/** */
	ASReal left, top, right, bottom;
} ASRealRect;

/** */
typedef struct _t_ASRealMatrix {
	/** */
	ASReal a, b, c, d, tx, ty;
} ASRealMatrix;
#endif /* ndef __ASTypes.h__ */

/**
	Time/Date structure. 

	The millisecond field is currently unused. 
	@see ASDateGetLocalTime 
	@see ASDateGetUTCTime 
	@see ASDateSetTimeFromRec 
	@see PDAnnotGetDate 
	@see PDAnnotSetDate
*/
typedef struct _t_ASTimeRec
{

	/**  only common era years displayed appropriately 
	*/
	ASInt16 year;		

	/** Jan = 1, ..., Dec = 12 
	*/
	ASInt16 month;		

	/** 1..31 
	*/
	ASInt16 date;		

	/** 0..23 
	*/
	ASInt16 hour;		

	/** 0..59
	*/
	ASInt16 minute;		

	/** 0..59
	*/
	ASInt16 second;		

	/** 0..999 
	*/
	ASInt16 millisecond;	

	/** Sun = 0, ... Sat = 6 
	*/
	ASInt16 day;		

	/** GMT offset in half hours east of Greenwich
		-48 == unknown
	*/
	ASInt16 gmtOffset;	
}
ASTimeRec, *ASTimeRecP;

/* ASCab
**
** An opaque type.
*/

/** ASCab objects ("cabinets") can be used to store arbitrary key/value pairs. The keys
	are always null-terminated strings containing only low-ASCII alphanumeric characters
	and spaces (ASCII character 32). Key names cannot begin or end with a space.

	Every time you place a non-scalar value inside a cabinet you are handing that value to
	the ASCab implementation to manage - i.e., putting a value in a cabinet is always a
	hand-off operation. For example, if you create an ASText object and add it as a value
	in an ASCab, the ASText object is no longer managed by you; it is managed by the
	ASCab. The ASCab will destroy the ASText object when its associated key is
	removed or the key's value is over-written. Pointer values are a special case
	discussed in more detail below.

	The routine naming convention is as follows:
	
	- "Get" routines return a value. These objects are owned by the ASCab and should
	not be destroyed by the caller of "Get". 
	- "GetCopy" routines make a copy of the data; the "GetCopy" client owns the
	resulting information and can modify it at will; he is also responsible for destroying
	it.
	- "Detach" routines work the same way as "Get" routines but the key is removed from
	the ASCab without destroying the associated value that is passed back to the client
	of "Detach". The client is responsible for destroying the returned object. 
	

	Normally pointers are treated the same way as scalars; the ASCab passes the pointer
	value back and forth but doesn't manage the data to which it points. This all changes
	if the pointer has an associated "destroyProc". If the "destroyProc" is set, the ASCab
	will reference count the pointer to track how many times the pointer is referenced from
	any ASCab (e.g., the reference count will be bumped up whenever the pointer is
	copied via ASCabCopy or added to another ASCab via ASCabPutPointer) and will
	destroy the data associated with the pointer when the ref count goes to 0. The data is
	destroyed by calling the ¡°destroyProc¡±. Detaching a pointer removes one reference to
	the pointer without ever destroying the information pointed to.
	ASCabDetachPointer returns a separate value indicating whether the pointer can
	safely be destroyed by the client or is still referred to by other key/value pairs inside
	any ASCabs - i.e., whether the reference count went to zero when the pointer was
	detached from the ASCab.

	Any of the ASCab API's can take a compound name: a string consisting of multiple
	keys separated by the colon (:) character - e.g., "Grandparent:Parent:Child:Key". The
	implementation will burrow down through such a compound string until it reaches the
	most deeply nested cabinet. Also, any of the "Put" routines can take a NULL key
	name. If the key name is NULL, the routine creates a new, numeric key name. If the
	cabinet is empty, the first generated key name will be "0" and subsequent names will
	increase in ascending order. This is useful when treating an ASCab as a bag of
	unnamed items, or when adding an ordered list of items to an empty ASCab.
	@see ASCabNew
	@see ASCabDestroy
 */
typedef struct _t_ASCabinet *ASCab;
typedef const struct _t_ASCabinet *ASConstCab;

/*------------------------------------------------------------------------
	typedefs for ASText
------------------------------------------------------------------------*/

/**  An opaque object holding encoded text.
	An ASText object represents a Unicode string. ASText objects can also be used to
	convert between Unicode and various platform-specific text encodings as well as
	conversions between various Unicode formats (e.g., UTF-16, UTF-8). Since it is
	common for a Unicode string to be repeatedly converted to or from the same
	platform-specific text encoding, ASText objects are optimized for this operation -
	e.g., they can cache both the Unicode and platform-specific text strings.

	There are several ways of creating an ASText object depending on the type and
	format of the original text data. The following terminology is used throughout this API
	to describe the various text formats.
	
	- Encoded - A multi-byte string terminated with a single 0 character and coupled
	with a specific host encoding indicator. In the Macintosh OS, the text encoding is
	specified using a script code. In the Windows OS, the text encoding is specified
	using a CHARSET code. On Unix the only valid host encoding indicator is 0, which
	specifies text in the platform's default Roman encoding. On all platforms Asian text
	is typically specified using multi-byte strings. 
	- ScriptText - A multi-byte string terminated with a single 0 character and coupled
	with an ASScript code. This is merely another way of specifying the Encoded
	case; the ASScript code is converted to a host encoding using
	ASScriptToHostEncoding. 
	- Unicode - Text specified using UTF-16 or UTF-8. In the UTF-16 case the bytes can
	be in either big-endian format or the endian-ness that matches the platform and
	are always terminated with a single ASUns16 0 value. In the UTF-8 case the text is
	always terminated with a trailing 0 byte. Unicode refers to straight Unicode without
	the 0xFE 0xFF prefix or language and country codes that can be encoded inside a
	PDF document. 
	- PDText - A string of text pulled out of a PDF document. This will either be a bigendian
	Unicode string pre-appended with the bytes 0xFE 0xFF or a string in
	PDFDocEncoding. In the Unicode case, this string may have embedded language
	and country identifiers. ASText objects strip language and country information out
	of the PDText string and track them separately. See below for more details. 
	

	ASTexts can also be used to accomplish encoding and format conversions; you can
	request a string in any of the formats specified above.
	In all cases the ASText code attempts to preserve all characters. For example, if you
	attempt to concatenate strings in separate host encodings the implementation may
	convert both to Unicode and perform the concatenation in Unicode space.

	When creating a new ASText object, or putting new data into an existing object, the
	implementation will always copy the supplied data into the ASText object. The
	original data is yours to do with as you will (and release if necessary).

	The size of ASText data is always specified in bytes¡ªe.g., the len argument to
	ASTextFromSizedUnicode specifies the number of bytes in the string, not the
	number of Unicode characters.

	Host encoding and Unicode strings are always terminated with a null character
	(which consists of one null byte for host-encoded strings and two null bytes for
	Unicode strings). You cannot create a string with an embedded NULL character even
	using the calls which take an explicit length parameter.

	The "Getxxx" calls return pointers to data held by the ASText object. You cannot free
	or manipulate this data directly. The "GetxxxCopy" calls return data you can
	manipulate at will and that you're responsible for freeing.

	An ASText object can have language and country codes associated with it. A
	language code is a 2-character ISO 639 language code. A country code is a 2-
	character ISO 3166 country code. In both cases the 2-character codes are packed
	into an ASUns16 value - the first character in bits 8-15, and the second character in
	bits 0-7. These language and country codes can be encoded into a UTF-16 variant of
	PDText encoding using an escape sequence; see Section 3.8 in the PDF Reference.
	The ASText calls will automatically parse the language and country codes
	embedded inside UTF-16 PDText and will also author appropriate escape
	sequences to embed the language and country codes (if present) when generating
	UTF-16 PDText.
	@see ASTextNew
	@see ASTextFromEncoded
	@see ASTextFromInt32
	@see ASTextFromPDText
*/
typedef struct _t_ASTextRec *ASText;

/**
	An opaque object holding constant encoded text. 
	@see ASText
*/
typedef const struct _t_ASTextRec *ASConstText;

/*------------------------------------------------------------------------
	typedefs for ASFile.h
------------------------------------------------------------------------*/

/**	@name ASFileOpenModes
	File access modes used to specify how a file can be used when it is open. Not all
	modes can be specified individually; ASFILE_CREATE can be used only in
	conjunction with ASFILE_READ or ASFILE_WRITE. In addition, it is acceptable to
	specify ASFILE_READ and ASFILE_WRITE together, by OR-ing the two constants.
	ASFILE_SERIAL and ASFILE_LOCAL (present only in version 3.0 or later) are hints
	that help the Acrobat viewer optimize access to the file; they must be OR-ed with one
	or more of the other constants.	
	@see ASFileSysOpenFile 
	@see ASFileReopen
*/
/*@{*/

/** Open the file for reading. */
#define ASFILE_READ		1	

/** Open the file for writing. */
#define ASFILE_WRITE	2	

/** Create the file if it does not exist. */
#define ASFILE_CREATE	4	


/** A hint indicating that the file will be accessed sequentially. */
#define ASFILE_SERIAL	8	

/** A hint indicating that a local copy of the file will be needed. */
#define ASFILE_LOCAL	16 
/*@}*/


/** @name ASFileFlags
	Flags to describe file transfers. Set by external file systems. 

	@see ASFileSysOpenFile 
*/
/*@{*/

/** Set if the file's data transfer rate 
	is generally slow, for example, because it is on a floppy 
	disk or being accessed via modem. 
*/
#define kASFileSlowTransfer		0x00000001L	

/**	Initiating each access to the file is slow, for example, 
	because the file is served by an HTTP server that spawns 
	a new process for each request. 
	
*/
#define kASFileSlowConnect		0x00000002L 

/** Use multi-read commands to access the file. */
#define kASFileUseMRead	 		0x00000004L 

/** Set if the file is to be cached (requires kASFileUseMRead to be set as well) */
#define kASFileDoCaching 		0x00000008L 

/**	(New for Acrobat 5.0) Set if media/access is a dial up connection. 
	This flag is only fully implemented on Windows. On the Macintosh, 
	this flag is always conservatively set to true.
*/
#define kASFileDialUp			0x00000010L	

/** Set if the file is still being loaded */
#define kASFileStillFetching	0x00000020L	

/** true if the file has outstanding MReads */
#define kASFileHasOutstandingMReads	0x00000040L
/*@}*/

/* ASFileMode flags */

/** @name ASFileModeFlags
	Flags to describe file modes. 
	@see ASFileRead 
	@see ASFileSetMode 
*/
/*@{*/

/**	If set, ASFileRead does not yield if bytes 
	are not ready (which raises the fileErrBytesNotReady exception). 
*/
#define kASFileModeDoNotYieldIfBytesNotReady		0x0001

/**	If set, the file will be read all at once regardless of 
	multiple read requests. 
	
*/
#define kASFileModeDisableExplicitMReadRequests		0x0002

/**	If set, ASFileRead will raise fileErrBytesNotReady when 
	trying to read from a file with a cache for which the requested 
	bytes are not yet present. 	
*/
#define kASFileRaiseIfBytesNotReady					0x0004

/**	If set, no read requests are issued if bytes are not ready 
	(that is, in the cache).
*/
#define kASFileNoRequestIfBytesNotReady				0x0008
/*@}*/

/* Type/Creator codes for ASFileSysSetTypeAndCreator */
#if MAC_PLATFORM
#define ASFourCharCode(x)	(x)
#else
#define ASFourCharCode(x)	(0U)
#endif

/** @name TypesCreators */
/*@{*/

/** @name Creators
 Creator codes for ASFileSysSetTypeAndCreator method.
*/
/*@{*/

/** Acrobat Creator Code */
#define kAcrobatCreatorCode		ASFourCharCode('CARO')	

/** Photoshop Creator Code */
#define kPhotoshopCreatorCode	ASFourCharCode('8BIM')	

/** ImageReady Creator Code */
#define kImageReadyCreatorCode	ASFourCharCode('MeSa')	

/** Illustrator Creator Code */
#define kIllustratorCreatorCode	ASFourCharCode('ART5')	
/*@}*/


/** @name AcrobatTypes
	Acrobat Types -- NOTE: Set creator to kAcrobatCreatorCode!
*/

/** Portable Document Format */
#define kPDFTypeCode			ASFourCharCode('PDF ')	

/** Forms Data Format */
#define kFDFTypeCode			ASFourCharCode('FDF ')	

/** XML Forms Data Format */
#define kXFDFTypeCode			ASFourCharCode('XFDF')	

/** XML Data Package */
#define kXDPTypeCode			ASFourCharCode('XDP ')	

/** Preferences File */
#define kPrefsTypeCode			ASFourCharCode('PREF')	

/** Acrobat Catalog Index file */
#define kPDXTypeCode			ASFourCharCode('PDX ')	

/** WebBuy Rights Management File */
#define kRMFTypeCode			ASFourCharCode('RMF ')	

/** Acrobat Profile Format (PPKLite) */
#define kAPFTypeCode			ASFourCharCode('APF ')

/** Acrobat Sequence File */
#define kSequenceTypeCode		ASFourCharCode('SEQU')	

/** Spelling Dictionary File */
#define kDictionaryTypeCode		ASFourCharCode('DICT')	

/** Web-Hosted Apps File */
#define kWHATypeCode			ASFourCharCode('WHA ')	

/** Locale File */
#define kLocaleTypeCode			ASFourCharCode('LANG')

/** Plug-In File */
#define kPluginTypeCode			ASFourCharCode('XTND')	

/** Plug-In File (Preferred, supported by 5.0.5 and later) 
	Using this filetype will allow a plugin developer to ship 
	a Carbonized plugin and not worry that it will try to load 
	and show an error when installed under Acrobat 4 or earlier
*/
#define kPluginNewTypeCode		ASFourCharCode('XTNc')

/** eBook Exchange Transfer Data (ETD) file */	
#define kETDTypeCode			ASFourCharCode('fETD')

/** eBook EDN Activation file */
#define kEDNTypeCode			ASFourCharCode('fEDN')
/*@}*/

/** @name PhotoshopTypes
 Photoshop Types -- NOTE: Set creator to kPhotoshopCreatorCode! 
*/
/*@{*/

/** Photoshop PSD File */
#define kPSDTypeCode			ASFourCharCode('8BIM')	

/** Mac PICT File */
#define kPICTTypeCode			ASFourCharCode('PICT')	

/** TIFF File */
#define kTIFFTypeCode			ASFourCharCode('TIFF')	

/** GIF File */
#define kGIFTypeCode			ASFourCharCode('GIFf')	

/** JPEG File */
#define kJPEGTypeCode			ASFourCharCode('JPEG')	

/** PNG File */
#define kPNGTypeCode			ASFourCharCode('PNGf')	
/*@}*/

/** @name IllustratorTypes
 Illustrator Types -- NOTE: Set creator to kIllustratorCreatorCode! 
*/
/*@{*/

/** Illustrator AI File */
#define KAITypeCode				ASFourCharCode('TEXT')	

/** EPS File */
#define kEPSTypeCode			ASFourCharCode('EPSF')	
/*@}*/

/** @name MiscCratorTypes 
*/

/** Text File */
#define kTextTypeCode			ASFourCharCode('TEXT')

/** Text File */
#define kRTFTypeCode			ASFourCharCode('RTF ')

/** SimpleText */
#define kTextCreatorCode		ASFourCharCode('ttxt')	

/** QuickTime File */
#define kQuickTimeTypeCode		ASFourCharCode('MooV')	

/** QuickTime Player */
#define kQuickTimeCreatorCode	ASFourCharCode('TVOD')	

/** HTML File */
#define kHTMLTypeCode			ASFourCharCode('TEXT')	

/** Microsoft IE */
#define kHTMLCreatorCode		ASFourCharCode('MSIE')	

/** XML File */
#define kXMLTypeCode			ASFourCharCode('TEXT')	

/** Microsoft Excel */
#define kExcelCreatorCode		ASFourCharCode('XCEL')	

/** Microsoft Word */
#define kWordCreatorCode		ASFourCharCode('W8BN')	

/** Microsoft PowerPoint */
#define kPowerPointCreatorCode	ASFourCharCode('SLD8')	

/** Unknown File */
#define kUnknownTypeCode		0x3f3f3f3f  

/** Unknown App */
#define kUnknownCreatorCode		0x3f3f3f3f  
/*@}*/
/*@}*/

/**
	Data structure containing callbacks that implement a file 
	system. 
	@see ASFileSys 
*/
typedef struct _t_ASFileSysRec *ASFileSys;


/**
	@see ASFileAcquirePathName 
	@see ASFileSysAcquireParent 
	@see ASFileSysCreatePathName 
	@see ASFileSysPathFromDIPath 
	@see ASPathFromPlatformPath 
	@see PDFileSpecAcquireASPath 
	@see ASFileSysReleasePath 
	@see ASFileSysDIPathFromPath
*/
typedef struct _t_ASPathNameRec *ASPathName;	

/**
	An ASFile is an opaque representation of a particular open
	file. Each open file has a unique ASFile. The ASFile value
	has meaning only to the common ASFile implementation and bears
	no relationship to platform-specific file objects.
	@see PDDocGetFile
	@see ASFileSysOpenFile
	@see ASFileFromMDFile
*/
typedef	void * ASFile;

/**
	Called when an asynchronous read or write request has completed. 
	
	@param aFile IN/OUT? The ASFile for which data is read or written. 
	
	@param p IN/OUT? Pointer to the buffer provided by the client. 
	Contains nBytesRead of data if error is zero. 
	@param fileOffsetRequested IN/OUT? File offset requested by the 
	client. 
	@param countRequested IN/OUT? Number of bytes requested by the 
	client. 
	@param nBytesRead IN/OUT? Number of bytes actually read or written. 
	
	@param error IN/OUT? Error condition if nonzero; see AcroErr. h. 
	
	@param compProcClientData IN/OUT? Client data parameter provided 
	by client. 
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysAsyncWriteProc 
	@see ASFileSysYieldProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileCompletionProc)(ASFile aFile, const char *p, 
				ASTFilePos fileOffsetRequested, ASTArraySize countRequested, 
				ASTArraySize nBytesRead, ASErrorCode error, void *compProcClientData);

/*------------------------------------------------------------------------
	typedefs: see ASTypes.h for definitions of ASFileSys, ASPathName, and ASFileNum
	as well as flags for ASFileSysOpenFile mode.
------------------------------------------------------------------------*/
 

/**
	ASMDFile replaces MDFile. MDFile is an obsolete 
	name for this data type for backward compatibility. 

	An MDFile is an opaque representation of a file instance 
	for a particular file system. File system implementors may 
	choose any convenient representation for an MDFile; in particular, 
	file systems need not worry about MDFile-space conflicts; 
	the ASFile object exported by the common implementation 
	is guaranteed to be unique across all open files, and the 
	common implementation maps calls to ASFile methods to calls 
	to ASFileSystem callbacks with the corresponding MDFile. 
	
	@see affected by this name change 
	@see ASFileFromMDFile 
	@see ASFileGetMDFile 
	@see affected by this name change 
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysGetFileFlags 
	@see ASFileSysYieldProc 
	@see ASFileSysMReadRequestProc 
	@see ASFileSysClearOutstandingMReadsProc 
	@see ASFileSysGetStatusProc 
	@see ASFileSysOpenProc 
	@see ASFileSysCloseProc 
	@see ASFileSysFlushProc 
	@see ASFileSysSetPosProc 
	@see ASFileSysGetPosProc 
	@see ASFileSysSetEofProc 
	@see ASFileSysGetEofProc 
	@see ASFileSysReadProc 
	@see ASFileSysWriteProc 
	@see ASFileSysRenameProc 
	@see ASFileSysIsSameFileProc
*/
typedef void *ASMDFile;
/* Obsolete name for this data type, for backward compatibility */
#define MDFile ASMDFile 


/**
	Data structure representing an I/O request. 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysAsyncWriteProc 
	@see ASIODoneProc 
*/
typedef struct _t_ASIORequestRec *ASIORequest;
 

/**
	Values returned by ASFileSysGetStatusProc. 
	@see ASFileRead 
	@see ASFileSysGetStatusProc
*/
typedef enum {

	/** The MDFile is in a valid state. 
	*/
	kASFileOkay				= 0x0000,	

	/** The MDFile is being closed, for example, 
	because the file is being displayed in a Web browser's window 
	and the user canceled downloading.
	*/
	kASFileIsTerminating	= 0x0001	
	} ASFileStatusFlags;


/**
	Callback in ASIORequest. Used by the asynchronous read/write 
	ASFileSys implementation. Provided by the ASFile implementation 
	to the ASFileSys. The ASFileSys must call this method when 
	an asynchronous request is completed: 
	
	- When an I/O request has some or all of its data. 

	- If the request is successfully queued but an error prevents 
	it from completing. 

	- If the request is aborted by calling ASFileSysAsyncAbortProc. 
	In this case, the totalBytesCompleted=0 and pError=-1. 
	
	If the request fails, this method must still be called, 
	with the error. It is not called if there is an error queueing 
	the read or write request, however. 
	@param req The I/O request for which data has been read/written.
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysAsyncWriteProc 
	@see ASFileSysYieldProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASIODoneProc)(ASIORequest req);

/**
	The first 5 items in the ASIORequestSingleRec structure exactly
	match the parameters of an ASFileSys read or write call. 

	The totalBytesRead is filled in before the IODoneProc is called.  
	If totalBytesRead is zero, or if pError is non-zero, the read was
	either terminated or didn't complete.

	If IODoneProc is non-zero, it points to a procedure that MUST be
	called either when the request has terminated due to error or other
	condition, or when all of the bytes have been received for this
	request.
*/
typedef struct _t_ASIORequestRec
{

	/** The MDFile corresponding to the ASFile this request is for.
	*/
	ASMDFile		mdFile;		

	/** Pointer to data to write to or read from mdFile
	*/
	void*			ptr;				

	/** offset Offset (specified in bytes) into mdFile of the first byte to read or write.
	*/
	ASTFilePos		offset;

	/** Number of bytes to read/ write. Must be filled 
	in before IODoneProc is called. If zero, the read was either 
	terminated or did not complete.
	*/
	ASTArraySize	count;


	/** Number of bytes actually read or written.
	*/
	ASTArraySize	totalBytesCompleted;

	/** Error code. This code is filled by the ASFileSys before IODoneProc is called. 
	If nonzero, the read was either terminated or did not complete.
	*/
	ASErrorCode		pError;			

	/** User-supplied data that the ASFileSys can use for anything it wishes.
	*/
	void*			clientData;		


	/** User-supplied callback to call by the ASFileSys when the operation has completed. If non-NULL, 
	it points to a procedure that must be called either when the request has terminated due to 
	error or other condition, or when all of the bytes have been received for this request. 
	NOTE: This callback may be called at interrupt time.
	*/
	ASIODoneProc	IODoneProc;			

	/** User-supplied that is available for IODoneProc to use.
	*/
	void*			IODoneProcData; 	
} ASIORequestRec;

/**
	Enumerated data type used to categorize an object associated 
	with an ASPathName. 
	@see ASFileSysGetItemPropsProc 
	@see ASFileSysFirstFolderItemProc 
	@see ASFileSysNextFolderItemProc 
	@see ASFileSysGetItemProps 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
*/
enum {

	/** Object is associated with a file. 
	*/
	kASFileSysFile,

	/** Object is associated with a folder.
	*/
	kASFileSysFolder,

	/** Object type is unknown. 
	*/
	kASFileSysUnknown = -1
};
typedef ASEnum16 ASFileSysItemType;


/**
	A list of properties for the object referenced by an ASPathName. 
	Used in ASFileSysGetItemProps and the folder enumeration 
	routines. 
	@see ASFileSysGetItemProps 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
*/
typedef struct _t_ASFileSysItemProps {

	/** Size of the data structure. Must be set to sizeof( ASFileSysItemPropsRec).
	*/
	ASSize_t			size;

	/** true if the object exists. If false none of the following fields are valid.
	*/
	ASBool				isThere;

	/** One of the ASFileSysItemTypes.
	*/
	ASFileSysItemType	type;

	/**  true if the object's hidden bit is set
	*/
	ASBool				isHidden;

	/** true if the object is read only.
	*/
	ASBool				isReadOnly;

	/** true if the file system could determine the creation date of the object.
	*/
	ASBool				creationDateKnown;

	/** The creation date of the object. Valid only if creationDateKnown is true.
	*/
	ASTimeRec			creationDate;

	/** true if the file system could determine the last modification date of the object.
	*/
	ASBool				modDateKnown;

	/** The modification date of the object. Valid only if modDateKnown is true. 
	*/
	ASTimeRec			modDate;

	/** If type is kASFileSysFile, this field holds the lower 32 bits of the size of the file (the upper 32
		bits are maintained by fileSizeHigh.
	*/
	ASByteCount			fileSize;

	/** If type is kASFileSysFile, this field holds the upper 32 bits of the size of the file (the lower 32
		bits are maintained by fileSize.
	*/
	ASByteCount			fileSizeHigh;

	/** If type is kASFileSysFolder, this field specifies how many items are in the folder. If this value 
		is -1 the file system was unable to easily determine the number of objects. You will need to explicitly 
		enumerate the objects to determine how many are in the folder.
	*/
	ASTCount			folderSize;

	/** The Mac OS creator code for the file. 
		For non-Mac OS file systems, this will be zero.
	*/
	ASUns32				creatorCode;

	/** The Mac OS type code for the file. 
		For	non-Mac OS file systems, this will be zero.
	*/
	ASUns32				typeCode;
} ASFileSysItemPropsRec, *ASFileSysItemProps;


/**
	An opaque object used to iterate through the contents of 
	a folder. ASFileSysFirstFolderItem returns the first item 
	in the folder along with an ASFolderIterator object for 
	iterating through the rest of the items in the folder. Call 
	ASFileSysNextFolderItem with this object to return the next 
	object in the folder until the routine returns false. To 
	discard the ASFolderIterator object, call ASFileSysDestroyFolderIterator. 
	
	@see ASFileSysFirstFolderItemProc 
	@see ASFileSysNextFolderItemProc 
	@see ASFileSysDestroyFolderIteratorProc 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator
*/
typedef struct _t_ASFolderIterator *ASFolderIterator;


/** ASPlatformPath and associated platform path types
	An opaque object used to retrieve a platform path object
	ASFileSysAcquirePlatformPath allocates and initializes this
	object. ASPlatformPath* calls are used to get at the contents.
	To discard this object call ASFileSysReleasePlatformPath.
*/
typedef struct _t_ASPlatformPath *ASPlatformPath;

/** A UNIX or Windows platform-specific path value. 
	@see ASPlatformPathGetCstringPtr
*/
typedef char*							Cstring_Ptr;		/* generally for Win_K, Unix	*/
/** 
	A C string containing a POSIX path (UTF-8 encoding).
	@see ASPlatformPathGetPOSIXPathPtr
*/
typedef char*							POSIXPath_Ptr;

#if MAC_PLATFORM

/** 
	Apointer to a Mac OS platform-specific FSSpec.
	@see ASPlatformPathGetFSSpecPtr
*/
typedef struct FSSpec					*FSSpec_Ptr;		/* for MacOS_K & MacOSX_K		*/

/** 
	Apointer to a Mac OS platform-specific FSRef
	@see ASPlatformPathGetFSRefPtr
*/
typedef struct FSRef					*FSRef_Ptr;			/* for MacOSX_K (here on down)	*/

/** 
	A structure containing the equivalent of two Mac OS platform-specific types: a pointer to
	an FSRef and a CFStringRef.

	@see ASPlatformPathGetFSRefWithCFStringRefRecPtr
*/
typedef struct _t_FSRefWithCFStringRefRec {
	/** */
    struct FSRef						*ref;
	/** definition of a CFStringRef */
    const struct __CFString				*str;
} FSRefWithCFStringRefRec;

/** */
typedef	FSRefWithCFStringRefRec			*FSRefWithCFStringRefRec_Ptr;

/** 
	A structure containing the equivalent of a Mac OS platform-specific CFURLRef.

	@see ASPlatformPathGetCFURLRefRecPtr
*/
typedef struct _t_CFURLRefRec {
	/** */
	const struct __CFURL				*url;				/* definition of a CFURLRef */
} CFURLRefRec;

/** */
typedef	CFURLRefRec						*CFURLRefRec_Ptr;	

#else	/* this is to prevent type redefinitions, e.g. with PhotoshopSDK which defines FSSpec for Win */

typedef struct FSSpecPlacebo			*FSSpec_Ptr;
typedef struct FSRefPlacebo				*FSRef_Ptr;
typedef struct _t_FSRefWithCFStringRefRecPlacebo {
    struct FSRefPlacebo					*ref;
    const struct __CFStringPlacebo		*str;
} FSRefWithCFStringRefRecPlacebo;
typedef	FSRefWithCFStringRefRecPlacebo	*FSRefWithCFStringRefRec_Ptr;
typedef struct _t_CFURLRefRecPlacebo {
	const struct __CFURLPlacebo			*url;
} CFURLRefRecPlacebo;
typedef	CFURLRefRecPlacebo				*CFURLRefRec_Ptr;	

#endif	/* #if MAC_PLATFORM */


/**
	Callback for ASFileSysRec. Gets the flags for the specified 
	file. 
	@param f IN/OUT? The file whose flags are obtained. 
	@return Bit field using ASFile Flags. 
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *ASFileSysGetFileFlags)(ASMDFile f);

/**
	Callback for ASFileSysRec. Asynchronously reads the specified 
	data, returning immediately after the request has been queued. 
	The ASFileSys must call the ASIODoneProc (if one was provided) 
	when the specified data has been read. 

	This callback is similar to the ASFileSysMReadRequestProc, 
	except that this callback contains a caller-provided ASIODoneProc, 
	and can only be used for a single byte range. 
	@param req Data structure specifying the data to read. 
	Contains information about the request including the ASMDFile, 
	the file offset, the buffer for the request, the number 
	of bytes in the request and an ASIODoneProc and clientData 
	for the ASIODoneProc. If the ASIODoneProc in req is non-NULL, 
	and there is an error queueing the read request, the ASIODoneProc 
	must not be called.
	@return 0 if the request was successfully queued, otherwise returns 
	a nonzero platform-dependent error code. 
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysAsyncWriteProc 
	@see ASFileSysYieldProc 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysAsyncReadProc)(ASIORequest req);

/**
	Callback for ASFileSysRec. Asynchronously writes the specified 
	data, returning immediately after the request has been queued. 
	The ASFileSys must call the ASIODoneProc (if one was provided) 
	when the specified data has been written. 
	@param req Data structure specifying the data to write. 
	Contains information about the request including the ASMDFile, 
	the file offset, the buffer for the request, the number 
	of bytes in the request and an ASIODoneProc and clientData 
	for the ASIODoneProc. If the ASIODoneProc in req is non-NULL, 
	and there is an error queueing the write request, the ASIODoneProc 
	must not be called.
	@return 0 if the request was successfully queued, otherwise returns 
	a nonzero platform-dependent error code. 
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysYieldProc 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysAsyncWriteProc)(ASIORequest req);

/**
	Callback for ASFileSysRec. Aborts all uncompleted asynchronous 
	I/ O requests for the specified file. This callback can 
	be called at any time. 

	This callback calls each outstanding ASIORequest's ASIODoneProc 
	to be called with the totalBytes = 0 and error = -1. 
	@param f IN/OUT? The file for which all uncompleted asynchronous 
	read/ write requests are aborted. 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysAsyncWriteProc 
	@see ASFileSysYieldProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysAsyncAbortProc)(ASMDFile f);


/**
	Callback for ASFileSysRec. Yields on the asynchronous I/ 
	O requests for the specified file to allow other processes 
	a chance to process events that may be required for a file 
	read to complete. An ASFileSys should implement a yield 
	mechanism to complement asynchronous read/ write requests. 

	In Windows, this could be a normal PeekMessage based yield. 
	
	In UNIX, it could mean using select on a file descriptor. 
	
	@param f IN/OUT? The file whose asynchronous I/ O requests are 
	yielded. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysAsyncAbortProc 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysAsyncWriteProc 
	@see ASFileRead 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysYieldProc)(ASMDFile f);


/**
	Callback for ASFileSysRec. Queues asynchronous requests 
	for one or more byte ranges that the caller (usually the 
	Acrobat viewer or Library) will need in the near future. 
	This callback is important for slow file systems, such as 
	the Web, to improve overall performance by allowing the 
	file system to begin retrieving bytes before they are actually 
	needed, while the Acrobat software continues processing 
	as much as it can with the data that has already been downloaded. 
	

	This callback does not actually read the data, but merely 
	queues the requests, starts the asynchronous code that reads 
	the data, and returns. The asynchronous code that reads 
	the data must use ASFilePushData to push the data from each 
	byte range to the Acrobat software as soon as the data is 
	ready. 

	This callback is similar to the ASFileSysAsyncReadProc, 
	except that this callback contains a caller-provided ASIODoneProc, 
	and can only be used for a single byte range. 
	@param f The file whose data is read. 
	@param aFile The corresponding ASFile, for use with ASFilePushData. 
	
	@param blockPairs An array of file offsets and byte lengths. 
	
	@param nBlockPairs The number of block pairs in blockPairs.
	@return 0 if the request was successfully queued, otherwise returns 
	a nonzero platform-dependent error code. 
	@see ASFileSysAsyncReadProc 
	@see ASFileSysClearOutstandingMReadsProc 
	@see ASFileRead 
	@see ASFileHasOutstandingMReads 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysMReadRequestProc)
							(ASMDFile f, ASFile aFile, ASTFilePos *blockPairs, ASTArraySize nBlockPairs);
 

/**
	Callback for ASFileSysRec. Used to advise a file system 
	that the previous range of bytes requested to read are not 
	needed, so that it may drop the read requests. The file 
	system can continue pushing the bytes if it cannot stop 
	the reads. 
	@param f The file that was being read.
	@see ASFileSysMReadRequestProc 
	@see ASFileRead 
	@see ASFileHasOutstandingMReads 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysClearOutstandingMReadsProc)(ASMDFile f);

/**
	Callback for ASFileSysRec. Gets the status of the specified 
	file. This callback is used for asynchronous I/ O. For example, 
	it can indicate that an underlying file connection has been 
	closed. 
	@param f IN/OUT? The file whose status is obtained. 
	@return The file's status. Must be one of the ASFileStatus Flags 
	values. 
	@see ASFileRead 
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *ASFileSysGetStatusProc)(ASMDFile f);

/**
	Callback for ASFileSysRec. Used for non-local file systems. 
	Returns an ASPathName on the new ASFileSys that refers to 
	an image (possibly cached) of the remote file. Because of 
	the possibility of cache flushing, you must hold a copy 
	of the remote file's ASPathName for the duration of use 
	of the local file. 

	NOTE: Do not remove the local file copy, since the default 
	file system does not know about the linkage to the remote 
	file. Removing this temporary file is left to the file system. 
	
	NOTE: The ASPathName returned should be released with the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param pathName IN/OUT? The ASPathName for which an equivalent 
	in newFileSys is obtained. 
	@param newFileSys IN/OUT? The file system in which an equivalent 
	of pathName is obtained. 
	@return The ASPathName (in newFileSys) for the specified file. Returns 
	NULL if one can not be made. 
	@see ASFileSysCreatePathNameProc 
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysAcquireFileSysPathProc)
							(ASPathName pathName, ASFileSys newFileSys);

/**
	Callback for ASFileSysRec. Opens the specified file. It 
	is called by ASFileSysOpenFile and ASFileReopen. This callback
	returns an error if the file is over 2GB in length.

	@param pathName IN/OUT? The pathname for the file to open. 
	@param mode IN/OUT? The mode in which the file is opened. Must 
	be an OR of the ASFile Open Modes. 
	@param fP IN/OUT? (Filled by the callback) The newly-opened file. 
	
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysCloseProc 
	@see ASFileSysOpenFile 
	@see ASFileReopen 
	@see ASFileClose 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysOpenProc)(ASPathName pathName, ASFileMode mode, ASMDFile *fP);

/**
	Callback for ASFileSysRec. This callback is responsible 
	for closing the specified file. It is called by ASFileClose. 
	
	@param f IN/OUT? The file to close. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysOpenProc 
	@see ASFileClose 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysCloseProc)(ASMDFile f);

/**
	Callback for ASFileSysRec. Flushes data for the specified 
	file. It is called by ASFileFlush. 
	@param f IN/OUT? The file to flush. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysFlushVolumeProc 
	@see ASFileFlush 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysFlushProc)(ASMDFile f);

/**
	Callback for ASFileSysRec. Sets the current position in 
	a file (that is, the point from which data will next be 
	read). It is called by ASFileSetPos. 
	@param f IN/OUT? The file in which the position is set. 
	@param pos IN/OUT? The desired new position (specified in bytes 
	from the beginning of the file). 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileGetPos 
	@see ASFileSetPos 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetPosProc)(ASMDFile f, ASFilePos pos);

/**
	Callback for ASFileSysRec. Gets the current position for 
	the specified file. Called by ASFileGetPos. Not capable of handling file positions over 2 GB.
	@param f The file whose current position is obtained
	@param pos (Must by filled by the callback) The current 
	position.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysSetPosProc 
	@see ASFileGetPos 
	@see ASFileSetPos 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetPosProc)(ASMDFile f, ASFilePos *pos);

/**
	Callback for ASFileSysRec. Increases or decreases the logical 
	size of a file. It is called by ASFileSetEOF. Returns an error
	if the current file position is over 2 GB.
	@param f The file to expand/shrink. 
	@param pos The desired size, in bytes.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysCanSetEofProc 
	@see ASFileGetEOF 
	@see ASFileSetEOF 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetEofProc)(ASMDFile f, ASFilePos pos);

/**
	Callback for ASFileSysRec. Gets a file's current logical 
	size. Called by ASFileGetEOF. Not capable of handling file sizes over
	2 GB.
	@param f IN/OUT? The file whose logical size is obtained. 
	@param pos IN/OUT? (Filled by the callback) The file's logical 
	size, in bytes. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileGetEOF 
	@see ASFileSetEOF 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetEofProc)(ASMDFile f, ASFilePos *pos);

/**
	Callback for ASFileSysRec. Reads data from the specified 
	file. It is called by ASFileRead. Returns an error if the file size
	is over 2 GB.
	@param ptr IN/OUT? (Filled by the callback) The data read from 
	the file. 
	@param size IN/OUT? The size of each item to read. 
	@param count IN/OUT? The number of items to read. 
	@param f IN/OUT? The file from which data is read. 
	@param pError IN/OUT? (Filled by the callback) Error code. This 
	value is filled only if an error occurred. In that case, 
	it should contain an error code. 
	@return The number of bytes read, or 0 if there was an error. 
	@see ASFileSysWriteProc 
	@see ASFileRead 
*/
typedef	ACCBPROTO1 ASSize_t (ACCBPROTO2 *ASFileSysReadProc)(void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASErrorCode *pError);

/**
	Callback for ASFileSysRec. Writes data to the specified 
	file. 
	@param ptr IN/OUT? Buffer containing data to write. 
	@param size IN/OUT? The size of each item to write. 
	@param count IN/OUT? The number of items to write. 
	@param f IN/OUT? The file into which data is written. 
	@param pError IN/OUT? (Filled by the callback) Error. 
	@return The number of bytes written. Returns 0 if there was an error. 
	
	@see ASFileSysReadProc 
	@see ASFileWrite 
*/
typedef	ACCBPROTO1 ASSize_t (ACCBPROTO2 *ASFileSysWriteProc)(void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASErrorCode *pError);

/**
	Callback for ASFileSysRec. Deletes a file. It is called 
	by ASFileSysRemoveFile. 
	@param pathName IN/OUT? The file to delete. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysRemoveFile 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRemoveProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Renames a file. It is not called 
	directly by any method in the client API, but is used internally 
	by the Acrobat viewer. 
	@param f The file to rename. 
	@param oldPath The file's old pathname. 
	@param newPath The file's new pathname.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysGetNameProc 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRenameProc)(ASMDFile* f, ASPathName oldPath, ASPathName newPath);

/**
	Callback for ASFileSysRec. Tests whether two files are the 
	same. 
	@param f IN/OUT? The ASFile of first file to compare (in many implementations, 
	it may be unnecessary to use this information, pathName 
	and newPathName provide sufficient information). 
	@param pathName IN/OUT? The ASPathName of first file to compare. 
	
	@param newPathName IN/OUT? The ASPathName of second file to compare. 
	
	@return 0 if the files are different, nonzero if they are the same. 
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysIsSameFileProc)(ASMDFile f, ASPathName pathName, ASPathName newPathName);

/**
	Callback for ASFileSysRec. Returns a character string containing 
	the filename for the specified ASPathName. The character 
	string contains only the filename; it is not a complete 
	pathname. 

	This callback is not called directly from any plug-in API 
	method. It is used internally by the Acrobat viewer. 
	@param pathName IN/OUT? The ASPathName for which the filename is 
	returned. 
	@param name IN/OUT? (Filled by the callback) Character string containing 
	the filename for pathName. 
	@param maxLength IN/OUT? Maximum number of characters that name 
	can hold. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysGetFileSysNameProc 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameProc)(ASPathName pathName, char *name, ASTArraySize maxLength);

/**
	Callback for ASFileSysRec. Returns an unique pathname suitable 
	for use in creating temporary files. 

	NOTE: The ASPathName returned should be released by the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param pathName IN/OUT? If pathName is non-NULL, the temporary 
	file must be stored such that a rename of pathName will 
	succeed (for example, on the same volume if renames across 
	volumes are illegal on this file system). 
	@return Pathname for a temporary file. 
	@see ASFileSysCopyPathNameProc 
*/
typedef	ACCBPROTO1 ASPathName	(ACCBPROTO2 *ASFileSysGetTempPathNameProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Copies a pathname (not the underlying 
	file). It is called by ASFileSysCopyPath. 

	Copying a pathname does not result in any file-level operations, 
	and is unaffected by whether there is or is not an open 
	file for the pathname. 

	NOTE: The ASPathName returned should be released by the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param pathName IN/OUT? The pathname to copy. 
	@return New copy of the pathname. 
	@see ASFileSysCopyPath 
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysCopyPathNameProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Converts a pathname to a device-
	independent pathname. It is called by ASFileSysDIPathFromPath. 
	

	NOTE: The memory for the char* returned should be freed 
	with the ASfree method when it is no longer needed. 
	@param path IN/OUT? The pathname to convert to a device-independent 
	pathname. 
	@param relativeToThisPath IN/OUT? The path relative to which the 
	device-independent pathname is specified. Pass NULL if the 
	deviceindependent pathname (for example, c:\ dir1\ dir2\ ...
	dir3\ myfile. pdf) is an absolute pathname instead of a 
	relative pathname (for example, ../../ dir3/ myfile. pdf). 
	
	@return The device-independent pathname. 
	@see ASFileSysDIPathFromPath 
*/
typedef	ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysDiPathFromPathProc)(
		ASPathName path,
		ASPathName relativeToThisPath
		);

/**
	Callback for ASFileSysRec. Converts a device-independent 
	pathname to an ASPathName. It is called by ASFileSysPathFromDIPath. 
	

	NOTE: The ASPathName returned should be released by the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param diPath IN/OUT? Device-independent pathname to convert to 
	an ASPathName. 
	@param relativeToThisPath IN/OUT? If diPath is an absolute pathname, 
	NULL. If diPath is a relative pathname, the pathname relative 
	to which it is specified. 
	@return The ASPathName corresponding to the specified device-independent 
	pathname. 
	@see ASFileSysPathFromDIPath 
	@see ASFileSysDIPathFromPath 
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysPathFromDIPathProc)(
		char * diPath,
		ASPathName relativeToThisPath

		);

/**
	Callback for ASFileSysRec. It is called by ASFileSysReleasePath. 
	

	This callback frees any memory occupied by a pathname. It 
	does not result in any file-level operations. 
	@param pathName The pathname to release.
	@see ASFileSysReleasePath 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysDisposePathNameProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Gets this file system's name. 
	

	This callback is not called directly by any method in the 
	client API, but is used internally by the Acrobat viewer. 
	
	@return The ASAtom containing the name of this file system. 
	@see ASFileRegisterFileSys
*/
typedef	ACCBPROTO1 ASAtom (ACCBPROTO2 *ASFileSysGetFileSysNameProc)(void);

/**
	Callback for ASFileSysRec. Gets the amount of free space 
	on the volume containing the specified ASPathName. 
	@param pathName The ASPathName for a file on the volume 
	whose free space is obtained.
	@return Free space, in bytes. Because the free space is returned 
	as an ASInt32, it is limited to 4 GB. 
*/
typedef	ACCBPROTO1 ASDiskSpace (ACCBPROTO2 *ASFileSysGetStorageFreeSpaceProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Flushes the volume on which the 
	specified file resides. This ensures that any data written 
	to the system for the volume containing pathName is flushed 
	out to the physical volume (equivalent to the Macintosh 
	FlushVol, or to the UNIX sync). Call this after you're finished 
	writing a complete 'transaction' to force a commit. 

	This callback is not called directly from any client API 
	method, but is used internally by the Acrobat viewer. 
	@param pathName The pathname for the file whose volume 
	is flushed.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysFlushProc 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysFlushVolumeProc)(ASPathName pathName);

/**
	Callback for ASFileSysRec. Creates an ASPathName based on 
	the input type and PDFileSpec. Each ASFileSys implementation 
	must publish the input types that it accepts. For example, 
	the Macintosh ASFileSys may accept the type FSSpecPtr, and 
	the MS-DOS ASFileSys may only accept types of Cstring. 

	NOTE: The ASPathName returned should be released by the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param pathSpecType The type of the input PDFileSpec. 
	@param pathSpec The file specification from which to create 
	an ASPathName. 
	@param mustBeZero Reserved for future use.
	@return The newly created pathname. 
	@see ASFileSysCreatePathName 
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysCreatePathNameProc)(ASAtom pathSpecType, const void *pathSpec, 
			const void *mustBeZero);


/**
	Callback for ASFileSysRec. Called to retrieve a full description 
	of the file system object associated with path. 
	@param pathName The ASPathName associated with the object. 
	
	@param props (Filled by the callback) Properties structure 
	describing the object.
	@return 0 to denote success, some error code otherwise. 
	@see ASFileSysGetItemProps 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetItemPropsProc)
	(ASPathName pathName, ASFileSysItemProps props);


/**
	Callback for ASFileSysRec. Begins the process of iterating 
	through the contents of a folder. 
	@param folderPath Path to the folder to be iterated through. 
	
	@param props (Filled by the callback) Properties structure 
	describing the object. 
	@param itemPath (Filled by the callback) A newly-allocated 
	ASPathName associated with the object (which the client 
	must free). Contains an absolute path.
	@return ASFolderIterator object used for iterating through subsequent 
	items. If there are no items in the folder this procedure 
	returns NULL. 
	@see ASFileSysNextFolderItemProc 
	@see ASFileSysDestroyFolderIteratorProc 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator 
*/
typedef ACCBPROTO1 ASFolderIterator (ACCBPROTO2 *ASFileSysFirstFolderItemProc)
	(ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath);


/**
	Callback for ASFileSysRec. Called to continue the iteration 
	process associated with the ASFolderIterator object. Both 
	itemPath and props are optional and can be NULL if the caller 
	is not interested in that information. 
	@param folderIter An ASFolderIterator object returned 
	from a previous call to ASFileSysFirstFolderItemProc. 
	@param props (Filled by the callback) Properties structure 
	describing the object. 
	@param itemPath (Filled by the callback) A newly-allocated 
	ASPathName associated with the object (which the client 
	must free). Contains an absolute path.
	@return false if no other objects were found, true otherwise. 
	@see ASFileSysFirstFolderItemProc 
	@see ASFileSysDestroyFolderIteratorProc 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysNextFolderItemProc)
	(ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath);


/**
	Callback for ASFileSysRec. Called to release the resources 
	associated with folderIter. 
	@param folderIter An ASFolderIterator object returned 
	from a previous call to ASFileSysFirstFolderItem.
	@see ASFileSysFirstFolderItemProc 
	@see ASFileSysNextFolderItemProc 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysDestroyFolderIteratorProc)
	(ASFolderIterator folderIter);


/**
	Callback for ASFileSysRec. Called to obtain the URL associated 
	with the given ASPathName. 
	@param path The ASPathName in question.
	@return The URL or NULL if it cannot be determined. Allocated memory 
	must be freeable with ASfree. 
	@see ASFileSysURLFromPath 
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysURLFromPathProc)
	(ASPathName path);

/**
** ASFileSysSetMode sets and gets parameters for the specified file.
**
**   Parameters
**      asFile - the file handle
**      modeValue - the value of bits to be set or cleared
**      modeMask - a mask indicating which bits are to be set or cleared
**
** To get the current mode:
**    ASFileSetMode(aFile, 0, 0);
** To set the mode: 
**    ASFileSetMode( aFile, kASFileModeDoNotYieldIfBytesNotReady,
**  	  kASFileModeDoNotYieldIfBytesNotReady );
** To clear the mode:     
**    ASFileSetMode( aFile, 0, kASFileModeDoNotYieldIfBytesNotReady );
**
**
**         kASFileModeDoNotYieldIfBytesNotReady
**               if set, then ASFileRead will NOT perform a
**               fileSys->yield() if RaiseIfBytesNotReady is true.
**               Otherwise, it may call yield before raising the
**               exception fileErrBytesNotReady.
**
**		   kASFileModeDisableExplicitMReadRequests 
**			   if set, mread requests made via ASFileMReadRequest
**			   become NOPs.
**
**		   kASFileRaiseIfBytesNotReady
**		       if set, ASFileRead will raise fileErrBytesNotReady
**			   when trying to read from a file with a cache for
**			   which the requested bytes are not yet present.
**
*/
typedef ACCBPROTO1 ASlFileMode (ACCBPROTO2 *ASFileSysSetModeProc)
	(ASMDFile f, ASlFileMode modeValue, ASMaskBits modeMask);

/**
	Callback for ASFileSysRec. Called to obtain the parent of 
	the input path. 
	@param path The ASPathName in question.
	@return The parent path, or NULL if path is a root directory. It 
	is the client's responsibility to free the returned ASPathName. 
	
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysGetParentProc)
	(ASPathName path);

/**
	Callback for ASFileSysRec. Called to create an empty folder 
	at the specified path. 
	@param path The path of the folder to create.
	@return 0 to denote success, some error code otherwise. 
	@see ASFileSysRemoveFolderProc 
	@see ASFileSysCreateFolder 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysCreateFolderProc)
	(ASPathName path);

/**
	Callback for ASFileSysRec. Called to delete the folder at 
	the specified path. 
	@param path The path of the folder to remove.
	@return 0 to denote success, some error code otherwise 
	@see ASFileSysCreateFolderProc 
	@see ASFileSysRemoveFolder 
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRemoveFolderProc)
	(ASPathName path);

/**
	Callback for ASFileSysRec. Called to obtain a representation 
	of a path that can be displayed by the user. 
	@param path The ASPathName in question.
	@return The display string or NULL if some error occurred. Allocated 
	memory must be free-able with ASfree. 
	@see ASFileSysDisplayStringFromPath 
*/
typedef	ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysDisplayStringFromPathProc)
	(ASPathName path);

/**
	Callback for ASFileSysRec. Sets the file type and creator 
	on the file. Currently only implemented on Macintosh. Does 
	not raise. 
	@param path Path to the file. 
	@param type File type. 
	@param creator File creator.
	@see ASFileSysGetTypeAndCreatorProc 
	@see ASFileSysGetTypeAndCreator 
	@see ASFileSysSetTypeAndCreator 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysSetTypeAndCreatorProc)
	(ASPathName path, ASlFileMode type, ASlFileMode creator);

/**
	Callback for ASFileSysRec. Gets the file type and creator 
	on the file. Currently only implemented on Mac. 

	Does not raise. 
	@param path Path to file. 
	@param type (Filled by the callback) The file type. 
	@param creator (Filled by the callback) The file creator.
	@see ASFileSysSetTypeAndCreatorProc 
	@see ASFileSysGetTypeAndCreator 
	@see ASFileSysSetTypeAndCreator 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysGetTypeAndCreatorProc)
	(ASPathName path, ASlFileMode *type, ASlFileMode *creator);

/**
	Callback for ASFileSysRec. Reopens a file in the specified 
	mode. ASFileReopen calls this method if it is present. If 
	this method is not present, or if it returns NULL and error 
	is 0, ASFileReopen does a close followed by an open. If 
	error is nonzero, ASFileReopen ignores the return value 
	and fails with that error. 

	On success, the old file should not need to be closed. On 
	failure, the old file should remain unchanged. 
	@param f The file to reopen. 
	@param newMode The mode for the new session. 
	@param error The error code for the operation. 0 if the 
	operation was successful, otherwise returns a nonzero error 
	code. The error is platform- and file-system specific. 
	@return The newly reopened file or NULL. 
	@see ASFileReopen 
	@see ASFileSysOpenFile 
*/
typedef ACCBPROTO1 ASMDFile (ACCBPROTO2 *ASFileSysReopenProc)
	(ASMDFile f, ASFileMode newMode, ASErrorCode *error);

/** 
	Does a hard flush on the file.  A hard flush makes sure
	the data is flushed even if the file is remote.  This
	proc should succeed and do nothing if it is not supported.

	Does not raise.
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysHardFlushProc)
	(ASMDFile f);

/** 
	Returns a platform file system representation of the ASPathName
	passed according to the atom selector. Allocates memory for the
	returned structure which the caller must release with ASfree.

	Does not raise.
*/
typedef ACCBPROTO1 void * (ACCBPROTO2 *ASFileSysGetPlatformThingProc)
	(ASPathName path, ASAtom thing);

/**
	Callback for ASFileSysRec. Gets a full description of the 
	file system object associated with pathName, returning the 
	item properties in the ASCab format. 

	If the ASCab has no keys on entry, every property known 
	is filled in. If it is not empty, only properties corresponding 
	to keys in the ASCab are filled in. Keys that do not map 
	to a property of the object are removed. The ASCab has the 
	following potential entries: 

	ASBool isThere; 
	ASInt32 type; 
	ASBool isHidden; 
	ASBool isReadOnly; 
	char * creationDate; // PDF style date 
	string char * modDate; 	// PDF style date string 
	ASUns32 fileSizeHigh; 
	ASUns32 fileSizeLow; 
	ASInt32 folderSize; 
	ASUns32 creatorCode; 
	ASUns32 typeCode; 
	ASUns32 versionMajor; 
	ASUns32 versionMinor; 
	ASBool isCheckedOut; 
	ASBool isPublished; 
	@param pathName The ASPathName associated with the object. 
	
	@param theCab (Filled by the method) Properties describing 
	the object, in cabinet format. 
	@return 0 if no error was encountered; otherwise an error code is 
	returned. If an error code is returned, theCab is not filled 
	with valid values. If the path name does not point to an 
	object on the file system, returns asFileErrFNF and a valid 
	ASCab with isThere set to false. 
	@see ASFileSysGetItemPropsAsCab 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysGetItemPropsAsCabProc)(ASPathName pathName, ASCab theCab);

/**
	Callback for ASFileSysRec. Tests whether a specified operation 
	can be performed on the file - that is, whether a handler 
	is defined for that operation in ASFileSysPerformOpOnItemProc. 
	
	@param pathName The ASPathName of the file. 
	@param op The name of the operation to test. 
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysPerformOpOnItemProc 
	@see ASFileSysCanPerformOpOnItem 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysCanPerformOpOnItemProc)(ASPathName pathName, const char *op);

/**
	Callback for ASFileSysRec. Performs the specified operation 
	on a particular file. 
	@param pathName The ASPathName of the file. 
	@param op The name of the operation to perform (a filesystem-defined 
	string). 
	@param params An ASCab object containing parameters to 
	pass to the operation.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysCanPerformOpOnItemProc 
	@see ASFileSysPerformOpOnItem 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysPerformOpOnItemProc)(ASPathName pathName, const char *op, ASCab params);

/**
	Callback for ASFileSysRec. Acquires a platform-specific 
	file-system representation of the specified path, according 
	to the specified type, wrapped in an allocated ASPlatformPath 
	object. Use the ASPlatformPath* calls to get the actual 
	platform object. 
	@param path The ASPathName in the current file system. 
	
	@param platformPathType The platform path type, one of 
	the following constants: FSSpec FSRef FSRefWithCFStringRefRec 
	CFURLRefRec 
	@param platformPath (Filled by the method) The new platform 
	path object.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysAcquireFileSysPathProc 
	@see ASFileSysAcquirePlatformPath 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysAcquirePlatformPathProc)
	(ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath);

/**
	Callback for ASFileSysRec. Releases the specified platform 
	path object when the client is done with it. 
	@param platformPath The ASPathName of the file.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysAcquirePlatformPathProc 
	@see ASFileSysReleasePlatformPath 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysReleasePlatformPathProc)
	(ASPlatformPath platformPath);

/**
	Callback for ASFileSysRec. Gets the filename for the specified 
	ASPathName as an ASText object. 

	NOTE: Supercedes ASFileSysGetNameProc for Acrobat 6.0. 
	@param pathName The ASPathName for which the filename 
	is returned. 
	@param name (Filled by the callback) ASText object for 
	the filename for pathName.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysGetFileSysNameProc 
	@see ASFileSysGetNameProc 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameAsASTextProc)
	(ASPathName pathName, ASText name);

/** 
	Places a user-displayable representation of a path into displayText.

	Does not raise.
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysDisplayASTextFromPathProc)
	(ASPathName path, ASText displayText);

/**
	Callback for ASFileSysRec. Called when a byte range has 
	arrived during a file load operation. 
	@param start The offset from the beginning of the file 
	to the start of the byte range. 
	@param length The number of bytes in the byte range. 
	@param clientData A pointer to data to pass to the callback.
*/
	typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysRangeArrivedProc)
			(ASInt32 start, ASInt32 length, void* clientData);

/**
	Callback for ASFileSysRec. Determines whether ASFileSys 
	can set EOF to a new offset for the specified file. 
	@param f The file. 
	@param pos The new EOF offset.
	@return true if EOF can be set for the file. 
	@see ASFileSysSetEofProc 
	@see ASFileGetEOF 
	@see ASFileSetEOF 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysCanSetEofProc)
	(ASMDFile f, ASFilePos pos);

/**
	Callback for ASFileSysRec. Converts a pathname to a device-independent 
	pathname, returned as an ASText object. It is called by 
	ASFileSysDIPathFromPathEx. 
	@param path The pathname to convert to a device-independent 
	pathname. 
	@param relativeToThisPath The path relative to which the 
	device-independent pathname is specified. Pass NULL if the 
	device-independent pathname (for example, c:\\dir1\\dir2\\dir3\\myfile.pdf) 
	is an absolute pathname instead of a relative pathname (for 
	example, ../../dir3/myfile.pdf). 
	@param diPathText (Filled by the method) The ASText object 
	to contain the device-independent path. Must be allocated 
	and freed by the client.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysPathFromDIPathExProc 
	@see ASFileSysDIPathFromPathEx 
	@see ASFileSysPathFromDIPathEx 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysDIPathFromPathExProc)(
		ASPathName path,
		ASPathName relativeToThisPath,
		ASText diPathText
		);

/**
	Callback for ASFileSysRec. Converts a device-independent 
	pathname from an ASText object to an ASPathName. It is called 
	by ASFileSysPathFromDIPathEx. 

	NOTE: The ASPathName returned should be released by the 
	ASFileSysReleasePath method when it is no longer needed. 
	
	@param diPathText Device-independent pathname to convert to 
	an ASPathName, as an ASText object. 
	@param relativeToThisPath If diPath is an absolute pathname, 
	NULL. If diPath is a relative pathname, the pathname relative 
	to which it is specified.
	@return The ASPathName corresponding to the specified device-independent 
	pathname. 
	@see ASFileSysDIPathFromPathExProc 
	@see ASFileSysPathFromDIPathEx 
	@see ASFileSysDIPathFromPathEx 
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysPathFromDIPathExProc)(
		ASConstText diPathText,
		ASPathName relativeToThisPath
		);

/**
	Callback for ASFileSysRec. Opens the specified file. It 
	is called by ASFileSysOpen64. This callback must be used if the file
	is over 2GB in length.
	@param pathName IN/OUT? The pathname for the file to open. 
	@param mode IN/OUT? The mode in which the file is opened. Must 
	be an OR of the ASFile Open Modes. 
	@param fP IN/OUT? (Filled by the callback) The newly-opened file. 	
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysCloseProc 
	@see ASFileSysOpenFile 
	@see ASFileReopen 
	@see ASFileClose 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysOpen64Proc)(ASPathName pathName, ASFileMode mode, ASMDFile *fP);

/**
	Callback for ASFileSysRec. Returns the maximum file position that
	can be processed by this file system. This is not maximum size file
	that can be created, but the maximum file position that can be handled
	by the arithmetic in the file system implementation. This will typically
	be (2 ^ 31) - 1 or (2 ^ 63) -1. If this entry is not present, a value of
	(2 ^ 31) - 1 should be assumed.
	@param pos IN/OUT? The maximum file position that can be handled. 
	
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysGetFilePosLimit 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetFilePositionLimitProc)(ASFilePos64 *pos);

/**
	Callback for ASFileSysRec. Sets the current position in 
	a file (that is, the point from which data will next be 
	read). It is called by ASFileSetPos. Capable of handling file postions
	over 2 GB.
	@param f IN/OUT? The file in which the position is set. 
	@param pos IN/OUT? The desired new position (specified in bytes 
	from the beginning of the file). 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileGetPos 
	@see ASFileSetPos 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetPos64Proc)(ASMDFile f, ASFilePos64 pos);

/**
	Gets the current position for the specified file. Called 
	by ASFileGetPos. Capable of handling file postions over 2 GB.
	@param f IN/OUT? The file whose current position is obtained. 
	@param pos IN/OUT? (Must by filled by the callback) The current 
	position. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysSetPosProc 
	@see ASFileGetPos 
	@see ASFileSetPos 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetPos64Proc)(ASMDFile f, ASFilePos64 *pos);

/**
	Callback for ASFileSysRec. Increases or decreases the logical 
	size of a file. It is called by ASFileSetEOF. Capable of handling
	file sizes over 2 GB.
	@param f IN/OUT? The file to expand/ shrink. 
	@param pos IN/OUT? The desired size, in bytes. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileGetEOF 
	@see ASFileSetEOF 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetEof64Proc)(ASMDFile f, ASFilePos64 pos);

/**
	Callback for ASFileSysRec. Gets a file's current logical 
	size. Called by ASFileGetEOF. Capable of handling file sizes over
	2 GB.
	@param f IN/OUT? The file whose logical size is obtained. 
	@param pos IN/OUT? (Filled by the callback) The file's logical 
	size, in bytes. 
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileGetEOF 
	@see ASFileSetEOF 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetEof64Proc)(ASMDFile f, ASFilePos64 *pos);

/**
	Callback for ASFileSysRec. Gets the Windows Explorer/Macintosh Finder
	representation for the specified ASPathName as an ASText object. This
	may be a localized and/or extension-stripped version of the filename.

	@param pathName The ASPathName for which the name 
	is returned. 
	@param nameForDisplay (Filled by the callback) ASText object for 
	the name for pathName.
	@return 0 if the request was successful, otherwise returns a nonzero 
	platform-dependent error code. 
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameForDisplayProc)
	(ASPathName pathName, ASText nameForDisplay);

/**
	Data structure containing callbacks that implement a file 
	system. 
	@see ASFileSys 
*/
typedef struct _t_ASFileSysRec {

	/** Size of the data structure. Must be set to sizeof( ASFileSysRec). 
	*/
	ASSize_t  size;	

	/** */
	ASFileSysOpenProc					open;

	/** */
	ASFileSysCloseProc					close;

	/** */
	ASFileSysFlushProc					flush;

	/** */
	ASFileSysSetPosProc					setpos;

	/** */
	ASFileSysGetPosProc					getpos;

	/** */
	ASFileSysSetEofProc					seteof;

	/** */
	ASFileSysGetEofProc					geteof;

	/** */
	ASFileSysReadProc					read;

	/** */
	ASFileSysWriteProc					write;

	/** */
	ASFileSysRemoveProc					remove;

	/** */
	ASFileSysRenameProc					rename;

	/** */
	ASFileSysIsSameFileProc				isSameFile;

	/** */
	ASFileSysGetNameProc				getName;

	/** */
	ASFileSysGetTempPathNameProc		getTempPathName;

	/** */
	ASFileSysCopyPathNameProc			copyPathName;

	/** */
	ASFileSysDiPathFromPathProc			diPathFromPath;

	/** */
	ASFileSysPathFromDIPathProc			pathFromDIPath;

	/** */
	ASFileSysDisposePathNameProc		disposePathName;

	/** */
	ASFileSysGetFileSysNameProc			getFileSysName;

	/** */
	ASFileSysGetStorageFreeSpaceProc	getStorageFreeSpace;

	/** */
	ASFileSysFlushVolumeProc			flushVolume;
	/* Added for Acrobat 3.0) */

	/** */
	ASFileSysGetFileFlags				getFileFlags;
	/*
	** These functions return zero on successful queue, and a non-zero
	** platform dependent error code on failure to queue.  If IODoneProc is
	** non-zero (meaning try to operate async), and Read or Write returns
	** non-zero (meaning failure to queue), IODoneProc MUST NOT BE CALLED.
	*/

	/** */
	ASFileSysAsyncReadProc				readAsync;

	/** */
	ASFileSysAsyncWriteProc				writeAsync;

	/** */
	ASFileSysAsyncAbortProc				abortAsync;

	/** */
	ASFileSysYieldProc					yield;

	/** */
	ASFileSysMReadRequestProc			mreadRequest;

	/** */
	ASFileSysGetStatusProc				getStatus;
    

	/** */
	ASFileSysCreatePathNameProc			createPathName;

	/** */
	ASFileSysAcquireFileSysPathProc		acquireFileSysPath;

	/** */
	ASFileSysClearOutstandingMReadsProc	clearOutstandingMReads;
	/* END of Acrobat 3.0 ASFileSys definition */

	/* Added for Acrobat 5.0 */

	/** */
	ASFileSysGetItemPropsProc			getItemProps;

	/** */
	ASFileSysFirstFolderItemProc		firstFolderItem;

	/** */
	ASFileSysNextFolderItemProc			nextFolderItem;

	/** */
	ASFileSysDestroyFolderIteratorProc	destroyFolderIterator;

	/** */
	ASFileSysSetModeProc				setFileMode;

	/** */
	ASFileSysURLFromPathProc			urlFromPath;

	/** */
	ASFileSysGetParentProc				getParent;

	/** */
	ASFileSysCreateFolderProc			createFolder;

	/** */
	ASFileSysRemoveFolderProc			removeFolder;

	/** */
	ASFileSysDisplayStringFromPathProc	displayStringFromPath;

	/** */
	ASFileSysSetTypeAndCreatorProc		setTypeAndCreator;

	/** */
	ASFileSysGetTypeAndCreatorProc		getTypeAndCreator;

	/** */
	ASFileSysReopenProc					reopen;

	/** */
	ASFileSysHardFlushProc				hardFlush;
	
	/* Added for Acrobat 6.0 */

	/** */
	ASFileSysGetPlatformThingProc		getPlatformThing;	/* deprecated */

	/** */
	ASFileSysGetItemPropsAsCabProc		getItemPropsAsCab;

	/** */
	ASFileSysCanPerformOpOnItemProc		canPerformOpOnItem;

	/** */
	ASFileSysPerformOpOnItemProc		performOpOnItem;

	/** */
	ASFileSysAcquirePlatformPathProc	acquirePlatformPath;

	/** */
	ASFileSysReleasePlatformPathProc	releasePlatformPath;

	/** */
	ASFileSysGetNameAsASTextProc		getNameAsASText;

	/** */
	ASFileSysDisplayASTextFromPathProc	displayASTextFromPath;

	/** */
	ASFileSysRangeArrivedProc			rangeArrived;

	/** */
	ASFileSysCanSetEofProc				canSetEof;

	/** */
	ASFileSysDIPathFromPathExProc		diPathFromPathEx;

	/** */
	ASFileSysPathFromDIPathExProc		pathFromDIPathEx;
	
	/** */
	ASFileSysGetFilePositionLimitProc	getfileposlimit;

	/** */
	ASFileSysOpen64Proc					open64;

	/** */
	ASFileSysSetPos64Proc				setpos64;

	/** */
	ASFileSysGetPos64Proc				getpos64;

	/** */
	ASFileSysSetEof64Proc				seteof64;

	/** */
	ASFileSysGetEof64Proc				geteof64;

	/** */
	ASFileSysGetNameForDisplayProc		getNameForDisplay;

} ASFileSysRec;

/*------------------------------------------------------------------------
	Generic monitor typedefs
-------------------------------------------------------------------------*/


/**
	Callback used in ProgressMonitor. Initialize the progress 
	monitor and displays it with a current value of zero. This 
	method must be called first when the progress monitor is 
	used. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@see PMEndOperationProc 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMBeginOperationProc)(void *clientData);

/**
	Callback used in ProgressMonitor. Draws the progress monitor 
	with its current value set to the progress monitor's duration 
	(a full progress monitor), then removes the progress monitor 
	from the display. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@see PMBeginOperationProc 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMEndOperationProc)(void *clientData);

/**
	Callback used in ProgressMonitor. Sets the value that corresponds 
	to a full progress monitor display. The progress monitor 
	is subsequently filled in by setting its current value. 
	This method must be called before you can set the progress 
	monitor's current value. 
	@param duration IN/OUT? The maximum value the progress monitor 
	will be allowed to have. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@see PMGetDurationProc 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMSetDurationProc)(ASDuration duration, void *clientData);

/**
	Callback used in ProgressMonitor. Sets the current value 
	of the progress monitor and updates the display. The allowed 
	value ranges from 0 (empty) to the value passed to setDuration. 
	For example, if the progress monitor's duration is 10, the 
	current value must be between 0 and 10, inclusive. 
	@param currValue IN/OUT? The progress monitor's current value. 
	
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@see PMGetCurrValueProc 
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMSetCurrValueProc)(ASDuration currValue, void *clientData);

/**
	Callback used in ProgressMonitor. Gets the progress monitor's 
	duration, set by the most recent call the progress monitor's 
	PMSetDurationProc. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@return The progress monitor's maximum value. 
	@see PMSetDurationProc 
*/
typedef	ACCBPROTO1 ASDuration (ACCBPROTO2 *PMGetDurationProc)(void *clientData);

/**
	Callback used in ProgressMonitor. Gets the progress monitor's 
	duration, set by the most recent call the progress monitor's 
	PMSetCurrValueProc. 
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to whatever API method required the progress monitor. 
	
	@see PMSetCurrValueProc 
*/
typedef	ACCBPROTO1 ASDuration (ACCBPROTO2 *PMGetCurrValueProc)(void *clientData);

/**
	Callback within ASProgressMonitorRec that sets the text 
	string that is displayed by the progress monitor. 

	The built-in document progress monitor (see AVAppGetDocProgressMonitor) 
	makes a copy of text. As such, it is the client's responsibility 
	to destroy it. 
	@param text IN/OUT? The string to display. 
	@param clientData IN/OUT? A pointer to the data associated with 
	the progress monitor (which should be passed to you with 
	the progress monitor). 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PMSetTextProc)(ASText text, void *clientData);

/**
	Replaced by ASProgressMonitor in Acrobat 5.0. 

	Data structure containing callbacks that implement a progress 
	monitor. The callbacks implement the progress monitor functions. 
	A progress monitor is used to display progress during potentially 
	time-consuming operations. Progress monitors are included 
	as parameters in many API calls. Acrobat's built-in progress 
	monitor can be obtained by calling AVAppGetDocProgressMonitor. 
	
	@see AVAppGetDocProgressMonitor 
	@see PDDocCreateThumbs 
	@see PDDocSave 
*/
typedef struct _t_ProgressMonitor
{

	/** Size of the data structure. Must be set to sizeof(ProgressMonitorRec).
	*/
	ASSize_t size;

	/** */
	PMBeginOperationProc	beginOperation;

	/** */
	PMEndOperationProc	endOperation;

	/** */
	PMSetDurationProc	setDuration;

	/** */
	PMSetCurrValueProc	setCurrValue;

	/** */
	PMGetDurationProc	getDuration;

	/** */
	PMGetCurrValueProc	getCurrValue;

	/** */
	PMSetTextProc		setText;
}
ASProgressMonitorRec, *ASProgressMonitor;

/* Allow clients to use the older names, without the "AS" prefix.
These are present only for backward compatibility - they must
not be used for new code. */
#define ProgressMonitor ASProgressMonitor
#define ProgressMonitorRec ASProgressMonitorRec

/**
	This callback replaces CancelProc. 

	Callback to check for canceling operations. An ASCancelProc 
	is typically passed to some method that takes a long time 
	to complete. At frequent intervals, the method calls the 
	ASCancelProc. If it returns true, then the method cancels 
	its operation; if false, it continues. 
	@param clientData IN/OUT? User-supplied data that was passed to 
	the method that uses the ASCancelProc. 
	@return true if the processing is to be canceled, false otherwise. 
	
	@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
	
	@see AVAppGetCancelProc 
	@see PDDocCreateThumbs 
	@see PDDocInsertPages 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASCancelProc)(void *clientData);

/* Define older name for backward compatibilty. Do not use this older name
in new code */
#define CancelProc ASCancelProc

/* ASPortRef */
#if MAC_PLATFORM
typedef CGrafPtr	ASPortRef;
#elif WIN_PLATFORM

/**
	Provides access to a port. On Windows, ASPortRef is the same as a Windows 
	HDC. 
*/
typedef void*		ASPortRef;
#elif UNIX_PLATFORM
typedef void*		ASPortRef;
#endif

/* ASWindowRef */
#if MAC_PLATFORM
typedef WindowRef	ASWindowRef;
#elif WIN_PLATFORM

/**
	A platform dependent window handle corresponding to a WindowPtr 
	in Mac OS, an HWND in Windows and a Widget in Unix. 
	@see AVSweetPeaProcessADMEvent 
*/
typedef void*		ASWindowRef;
#elif UNIX_PLATFORM
typedef void*		ASWindowRef;
#endif

#if DEBUG
#define CHECKTYPE(type, data) ((void *)((data) == ((type)data) ? data : data))
#else	/* !DEBUG */
#define CHECKTYPE(type, data) ((void *)data)
#endif	/* !DEBUG */

/* Helper defines for the mystical ASFileSysCreatePathName. */
/**
	Helper macro for the ASFileSysCreatePathName method. See 
	this method for more information. 
	@param asfs (May be NULL) The file system through which 
	the ASPathName is obtained. 
	@param cDIPath A C string containing the device-independent 
	path for which the ASPathName is obtained. 
	@param aspRelativeTo (May be NULL) An ASPathName that 
	cDIPath will be evaluated against if it contains a relative 
	path.
	@see ASFileSysCreatePathFromCString 
	@see ASFileSysCreatePathFromFSSpec 
	@see ASFileSysCreatePathWithFolderName 
*/
#define ASFileSysCreatePathFromDIPath(asfs, cDIPath, aspRelativeTo)		\
		ASFileSysCreatePathName(asfs, ASAtomFromString("DIPath"),		\
		(void *)CHECKTYPE(char *, cDIPath), (void *)CHECKTYPE(ASPathName, aspRelativeTo))
/** */
#define ASFileSysCreatePathFromDIPathText(asfs, tDIPath, aspRelativeTo)		\
		ASFileSysCreatePathName(asfs, ASAtomFromString("DIPathWithASText"),		\
		(void *)CHECKTYPE(ASText, tDIPath), (void *)CHECKTYPE(ASPathName, aspRelativeTo))
/**
	Helper macro for the ASFileSysCreatePathName method. See 
	this method for more information. 
	@param asfs (May be NULL) The file system through which 
	the ASPathName is obtained. 
	@param aspFolder ASPathName contained the path of the 
	folder. 
	@param cFileName A C string containing the name of the 
	file. The returned ASPathName contains the result of appending 
	cFileName to aspFolder.
	@see ASFileSysCreatePathFromCString 
	@see ASFileSysCreatePathFromDIPath 
	@see ASFileSysCreatePathFromFSSpec 
*/
#define ASFileSysCreatePathWithFolderName(asfs, aspFolder, cFileName)	\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FolderPathName"),	\
		(void *)CHECKTYPE(ASPathName, aspFolder), (void *)CHECKTYPE(char *, cFileName))
/** */
#define ASFileSysCreatePathWithFolderNameWithASText(asfs, aspFolder, tFileName)	\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FolderPathNameWithASText"),	\
		(void *)CHECKTYPE(ASPathName, aspFolder), (void *)CHECKTYPE(ASText, tFileName))
/**
	Helper macro for the ASFileSysCreatePathName method. See 
	this method for more information. 

	NOTE: This macro uses a local variable named scratchFourBytes 
	- (void* scratchFourBytes). PDF Library users need to provide 
	this variable in order to utilize the macro; the variable 
	must be local to the client application, not to the library. 
	Any client can use this macro provided he also has code 
	similar to the following in the same source file that uses 
	the macro: static void* scratchFourBytes; 
	@param asfs (May be NULL) The file system through which 
	the ASPathName is obtained. 
	@param cPath A C string containing the path for which 
	the ASPathName is obtained.
	@see ASFileSysCreatePathFromDIPath 
	@see ASFileSysCreatePathFromFSSpec 
	@see ASFileSysCreatePathWithFolderName 
*/
#define ASFileSysCreatePathFromCString(asfs, cPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("Cstring"),			\
			(void *)CHECKTYPE(char *, cPath), NULL);
#if MAC_PLATFORM
/**
	Helper macro for the ASFileSysCreatePathName method. See 
	this method for more information. 
	@param asfs (May be NULL) The file system through which 
	the ASPathName is obtained. 
	@param cPath The FSSpec for which the ASPathName is obtained.
	@see ASFileSysCreatePathFromCString 
	@see ASFileSysCreatePathFromDIPath 
	@see ASFileSysCreatePathWithFolderName 
*/
#define ASFileSysCreatePathFromFSSpec(asfs, cPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSSpec"),			\
			(void *)CHECKTYPE(FSSpec *, cPath), NULL);
/** */
#define ASFileSysCreatePathFromFSRef(asfs, fsRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSRef"),			\
			(void *)CHECKTYPE(FSRef, fsRef), NULL);
/** */
#define ASFileSysCreatePathFromFSRefWithCFStringRef(asfs, fsRefWithCFStringRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSRefWithCFStringRef"),			\
			(void *)CHECKTYPE(FSRefWithCFStringRefRec *, fsRefWithCFStringRef), NULL);
/** */
#define ASFileSysCreatePathFromCFURLRef(asfs, cfURLRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("CFURLRef"),			\
			(void *)CHECKTYPE(CFURLRef, cfURLRef), NULL);
/** */
#define ASFileSysCreatePathFromPOSIXPath(asfs, posixPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("POSIXPath"),			\
			(void *)CHECKTYPE(char *, posixPath), NULL);
#endif

/* Definitions related to encodings and encoding conversions */


/**
	Describes the various Unicode formats you can pour into 
	and read out of an ASText object. 
	@see ASTextFromUnicode 
	@see ASTextFromSizedUnicode 
	@see ASTextSetUnicode 
	@see ASTextSetSizedUnicode 
	@see ASTextGetUnicodeCopy 
*/
enum {
	/** Always returns the bytes in big-endian order.*/
	kUTF16BigEndian,
	/** Returns the bytes in the host's native endian, whatever is natural for an ASUns16.*/
	kUTF16HostEndian,
	/** Endian neutral.*/
	kUTF8,
	/** */
	kUTF32BigEndian,
	/** */
	kUTF32HostEndian
};
typedef ASEnum16 ASUnicodeFormat;

/**
	Integer specifying the host encoding for text. On the Mac 
	OS, it is a script code. On Windows, it is a CHARSET id. 
	On UNIX, Acrobat currently only supports English, so the 
	only valid ASHostEncoding is 0 (Roman). See ASScript. 
*/
typedef ASInt32 ASHostEncoding;

/** An ASUnicodeChar is large enough to hold any Unicode character
   (at least 21 bits wide).
*/
typedef ASUns32 ASUnicodeChar;
typedef ASUns32 ASUTF32Val;

/**
	Holds a single 16-bit value from a UTF-16 encoded Unicode 
	string. Typically used to point to the beginning of an UTF-16 
	string, for example: 
	<pre>
	ASUTF16Val *utf16String = ... 
	</pre>
	This datatype is not large enough to hold any arbitrary 
	Unicode character. Use ASUnicodeChar to pass individual 
	Unicode characters around. 
	@see ASTextGetUnicode 
	@see ASTextGetUnicodeCopy
*/
typedef ASUns16 ASUTF16Val;

/** An ASUTF8Val holds a single 8-bit value from a UTF-8 encoded
	Unicode string.
*/
typedef ASUns8 ASUTF8Val;

/**
	An enumeration of writing scripts. Not all of these scripts 
	are supported on all platforms. 
*/
enum {

	/** */
    kASRomanScript,
	/** */
    kASJapaneseScript,
	/** */
    kASTraditionalChineseScript,
	/** */
    kASKoreanScript,
	/** */
    kASArabicScript,
	/** */
    kASHebrewScript,
	/** */
    kASGreekScript,
	/** */
    kASCyrillicScript,
	/** */
    kASRightLeftScript,
	/** */
    kASDevanagariScript,
	/** */
    kASGurmukhiScript,
	/** */
    kASGujaratiScript,
	/** */
    kASOriyaScript,
	/** */
    kASBengaliScript,
	/** */
    kASTamilScript,
	/** */
    kASTeluguScript,
	/** */
    kASKannadaScript,
	/** */
    kASMalayalamScript,
	/** */
    kASSinhaleseScript,
	/** */
    kASBurmeseScript,
	/** */
    kASKhmerScript,
	/** */
    kASThaiScript,
	/** */
    kASLaotianScript,
	/** */
    kASGeorgianScript,
	/** */
    kASArmenianScript,
	/** */
    kASSimplifiedChineseScript,
	/** */
    kASTibetanScript,
	/** */
    kASMongolianScript,
	/** */
    kASGeezScript,
	/** */
    kASEastEuropeanRomanScript,
	/** */
    kASVietnameseScript,
	/** */
    kASExtendedArabicScript,
	/** */
    kASEUnicodeScript,
	/** */
    kASDontKnowScript = -1
};
typedef ASInt32 ASScript;

/* UUID structure */
/** 
	A structure representing a universal unique identifier (UUID) for the current user or the
	current session.
*/
typedef struct
{
	/** */
	ASUns32	timeLow;
	/** */
	ASUns16	timeMid;
	/** */
	ASUns16	timeHiAndVersion;
	/** */
	ASUns8	clockSeqHiAndReserved;
	/** */
	ASUns8	clockSeqLow;
	/** */
	ASUns8	node[6];
} ASUUID;

/**
	A constant for the maximum string length of a unique identifier 
	(UUID). 
	@see ASUUIDToCString
*/
#define ASUUIDMaxStringLen 40 /* leave a bit of padding just in case */

/*------------------------------------------------------------------------
	typedefs for ASDate
------------------------------------------------------------------------*/

/* ASDate
*
*/

/* string formats
   kASTimePDF = "D:20000911121643-08'00'"
   kASTimeUniversal = "2000.09.11 13:30:20 -08'00' DST"
   kASTimeUniversalH = "2000-09-11 13:30:20 -08'00' DST"
   kASTimeUTC_ASN1 = "000911203020Z"
   kASTimeGENERALIZED_ASN1 = "20000911203020Z" */
   
/** A constant indicating a string format for representing a date-time. */
enum { 
	/** */
	kASTimeNone=0, 
	/** PDF date format, as defined in the PDF Reference. */
	kASTimePDF,  
	/** Slight variations of the time format expressed in ISO 8601. */
	kASTimeUniversal, 
	/** Slight variations of the time format expressed in ISO 8601. */
	kASTimeUniversalH,
	/** UTC ASN1 format. */
	kASTimeUTC_ASN1, 
	/** ASN1 format. */
	kASTimeGENERALIZED_ASN1
    };
typedef ASEnum8 ASDateTimeFormat;

/** An opaque object holding information for a particular date and time.
	ASDate objects represents a particular date and time. All ASDate objects
	are guaranteed to give accurate representation of UTC time, unadjusted for
	leap seconds. This is due to the fact that the introduction of leap seconds
	to the international calendar does not happen according to a well-defined 
	rule. 
	Note that ASDate objects are NOT guaranteed to represent local time accurately,
	To be exact, on Mac and Unix ASDate cannot always determine the prevailing
	daylight saving rule for the OS' timeZone. See notes on ASDateGetCurrentLocalTime
	for further explanation. 
*/
typedef struct _t_ASDateRec *ASDate;

/**
	Represents a calendar time span, used to calculate ambiguous 
	time spans such as 1 year and 3 months. A calendar time 
	span cannot be negative. 
	@see ASCalendarTimeSpanAddWithBase 
	@see ASCalendarTimeSpanCompare 
	@see ASCalendarTimeSpanDiff
*/
typedef struct _t_ASCalendarTimeSpan{
	/** */
	ASUns32 Year;
	/** */
    ASUns32 Month;
	/** */
    ASUns32 Day;
	/** */
    ASUns32 Hour;
	/** */
    ASUns32 Minute;
	/** */
    ASUns32 Second;
} ASCalendarTimeSpanRec, *ASCalendarTimeSpan;

/** 
	ASTimeSpan represents an exact time span, measured in seconds. The internal representation uses
	64 bit signed integers to avoid the 2037 problem. Negative timespans are allowed.
*/
typedef struct _t_ASTimeSpanRec *ASTimeSpan;

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_ASExpT */
