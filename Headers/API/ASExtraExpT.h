/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASExtraExpT.h

 - Types, macros, structures, etc. required to use the ASExtra HFT.

*********************************************************************/

#ifndef _H_ASExtraExpT
#define _H_ASExtraExpT

#include "Environ.h"

#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#include "ASExpT.h"
#else
#include "ASBasic.h"
#include "ASTypes.h"
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

/** */
typedef ASUns16 ASUniChar;
/** */
typedef ASUns16 ASCountryCode;
/** */
typedef ASUns16 ASLanguageCode;


/** 
*/
enum {

	/** does nothing */
	kASTextFilterIdentity,					

	/** normalizes line endings (equivalent to ASTextNormalizeEndOfLine) */
	kASTextFilterLineEndings,

	/** make all text upper case */
	kASTextFilterUpperCase, 				

	/** changes any ASText to "XXX". For debugging. */
	kASTextFilterXXXDebug,					

	/** makes all text except scanf format strings upper case. */
	kASTextFilterUpperCaseDebug,			

	/** removes stand-alone ampersands, turns '&&' into '&' */
	kASTextFilterRemoveAmpersands,			

	/** changes any fullwidth ASCII variants to their lower-ASCII version */
	/* For example, 0xFF21 (FullWidth 'A') becomes 0x0041 (ASCII 'A') */
	kASTextFilterNormalizeFullWidthASCIIVariants,			

	/** removes line endings and replaces them with spaces */
	kASTextRemoveLineEndings,

	/** reserved...  do not use */
	kASTextFilterRsvd1 = 1000,				

	/** an invalid filter type */
	kASTextFilterUnknown = -1				
};
/**
	Constants that specify filter types used to modify text 
	objects. 
	@see ASTextFilter 
*/
typedef ASEnum16 ASTextFilterType;


/** 
	ASCabinets can be used to store arbitrary key/value pairs.  The
	keys are always null-terminated strings containing only low-ASCII
	alphanumeric characters. The various types of values are enumerated
	here.
*/
enum {

	/** An ASBool */
	kASValueBool,		

	/** An ASInt32 */
	kASValueInteger,	

	/** An ASAtom */
	kASValueAtom,		

	/** A double-precision floating point number */
	kASValueDouble,		

	/** Null-terminated, unencoded string */
	kASValueString,		

	/** An ASText object */
	kASValueText,		

	/** A binary blob of any size */
	kASValueBinary,		

	/** A pointer to something outside the cabinet */
	kASValuePointer,	

	/** Another cabinet */
	kASValueCabinet,	

	/** Key exists but has no useful value i.e. placeholder */
	kASValueNull,		

	/** An ASUns32 */
	kASValueUns,		

	/** An ASInt64 */
	kASValueInt64,	

	/** An ASUns64 */
	kASValueUns64,
			
	/** An invalid type */
	kASValueUnknown	= -1	
};
/**
	ASCabs can be used to store arbitrary key-value pairs. The 
	keys are always null-terminated strings containing only 
	low-ASCII alphanumeric characters. These constants specify 
	the various types of values. 
	@see ASCabFromEntryList 
	@see ASCabGetType 
*/
typedef ASEnum16 ASCabValueType;


/** Cabinet keys are null-terminated C strings. This constant
	declares the maximum length of one component of that string.

	The characters in the key string must all be low-ASCII
	alphanumeric characters e.g 0 - 9, a - z, A-Z.

	You can burrow through multiple levels of a cabinet heirarchy by
	passing in a string of individual key names separated by
	colons. For example,
	<pre>
		ASCabGetInt(cab, "Hello:World", -1)
	</pre>
	is equivalent to
	<pre>
		ASCabGetInt(ASCabGetCab(cab, "Hello"), "World", -1).
	</pre>
	Similarly,
	<pre>
		ASCabPutInt(theCab, "Hello:World", 33);
	</pre>
	will create an integer key named "World" inside the "Hello" cabinet
	inside theCab, creating the "Hello" key and cabinet if
	necessary.
*/
#define MAX_ASCAB_KEY 1024


/**
	Data structure representing a cabinet entry. The first entry 
	in each descriptor specifies the name of the key; the second 
	field contains the type; the following fields contain the 
	values. The entry list must end with a descriptor containing 
	NULL for the key name. It can be used as shown below to 
	construct an ASCab: 

	ASCabEntryRec cabData[] = {{"key1", kASValueInteger, 1}, 
	{"key2", kASValueInteger, -1}, {"key3", kASValueBool, false}, 
	{NULL}}; 

	ASCab CreateDefaultCab() { return ASCabFromEntryList (cabData); 
	} 

	This example uses just three values for each record. However, 
	more may be required - for example, for a double: 

	{"keyDouble", kASValueDouble, 0, NULL, doub} 

	For a string: 

	{"keyString", kASValueString, 0, (void*)string} 
	@see ASCabFromEntryList 
*/
typedef struct _t_ASCabEntryRec {

	/** The name of the key.
	*/
	const char *	keyName;

	/** The supported ASCabValueTypes are: 
		- kASValueBool: intVal contains the value. 
		- kASValueInteger: intVal contains the value. 
		- kASValueAtom: intVal contains the value. 
		- kASValueDouble: doubleVal contains the value. 
		- kASValueString: ptrVal points to a null-terminated C string. 
		- kASValueText: ptrVal points to a null-terminated string containing script text, 
		intVal specifies the ASScript code for the text. 
		- kASValueBinary: ptrVal points to the binary data, intVal specifies the size of the data. 
		- kASValueNull: Creates an entry with a NULL value. 
		No other types are supported (specifically kASValueCabinet and kASValuePointer). You can build 
		nested cabinets using the "key: key" syntax for the keyNames.
	*/
	ASCabValueType	type;

	/** See above.
	*/
	ASInt32			intVal;
	/** See above.
	*/
	const void *	ptrVal;
	/** See above.
	*/
	double			doubleVal;
} ASCabEntryRec;


/**
	Used when enumerating the values inside a cabinet. 
	@param theCab The cabinet being enumerated. 
	@param theKey The key name of an entry in the cabinet. 
	
	@param itsType The type of the value associated with theKey. 
	
	@param clientData User-supplied data that was passed in 
	to ASCabEnum.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see ASCabEnum 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASCabEnumProc)
	(ASCab theCab, const char *theKey, ASCabValueType itsType, void *clientData);


/**
	Used when enumerating the values inside a constant cabinet. 
	The callback procedure must not add, delete, or modify items 
	in theCab during the enumeration. 
	@param theCab The cabinet being enumerated. 
	@param theKey The key name of an entry in the cabinet. 
	
	@param itsType The type of the value associated with theKey. 
	
	@param clientData User-supplied data that was passed in 
	to ASCabEnum.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see ASConstCabEnum 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASConstCabEnumProc)
	(ASConstCab theCab, const char *theKey, ASCabValueType itsType, void *clientData);


/** A value that determines the actions to be taken when ASCabMunge is called. keyCab
	is the ASCab that provides the keys determining how theCab is to be changed.
	During an ASCabMunge operation the key cab will not be altered.
*/
enum {

	/** Any keys in keyCab are removed from theCab	*/
	kASMungeRemove,

	/** Any keys in theCab which aren't also in keyCab are removed.	*/
	kASMungeRemoveUnknown,

	/** Any keys in keyCab which are also in theCaband have the same value in theCab are removed. */
	kASMungeRemoveDefaults,

	/** Any keys in theCab which are also in keyCab but have different values are removed.	*/
	kASMungeRemoveBadValues,

	/** All key/value pairs in keyCab are copied into theCab. */
	kASMungeCopy,

	/** Any keys in theCab which are also in keyCab are replaced with the values in keyCab.	*/
	kASMungeReplace,

	/** Any keys in keyCab which are not in theCab are copied over to theCab. */
	kASMungeCopyMissing,

	/** Any keys in keyCab with a value of null are removed from theCab. */
	kASMungeRemoveNulls
};
typedef ASEnum16 ASCabMungeAction;


/**
	A de-allocation callback that can be associated with a pointer 
	in an ASCab. When the reference count of the pointer falls 
	to zero, this callback is called to free the resources associated 
	with the object the pointer references. 
	@param ptr IN/OUT? The value stored in an ASCab. 
	@see ASCabPutPointer 
	@see ASCabGetPointerDestroyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASCabPointerDestroyProc)
	(void *ptr);

/* ASReportProc
**
** Used to report errors, warnings, and other messages to the user.
**
** The reportType indicates what sort of information is being
** reported. Perhaps the best way to think of this is to consider what
** sort of icon the user should see when they read the message.
**
** The errorCode argument should be set to one of the defined error
** codes (i.e. the same ones you would use when raising errors). It
** can also be set to 0 (indicating no error).
**
** The 'message' field specifies the text the user should read. If the
** 'message' field is NULL the system will retrieve the message
** associated with the errorCode. If both 'message' and 'errorCode'
** are specified the 'message' argument will be used.
**
** If the 'replacementText' is not NULL the system will attempt to
** replace the string "%s" in the message with the replacement
** text. This applies whether the text is specified via the 'message'
** argument or retrieved from the system using the 'errorCode'
** argument.
**
** The moreInfo field is not used as yet.
**
** The reportProcData is a pointer to whatever data is associated with
** the reportProc (which should be passed to you with the report
** proc).
**
** All of these arguments are handed off to the reportProc. It's the
** reportProc's responsibility to destroy all objects passed to it,
** and it may destroy them at any time.
*/


/** Used in an ASReportProc to indicate what kind of information is being reported.
	@see ASReportProc 
*/
enum {

	/** a note */
	kASReportNote,

	/** a warning */
	kASReportWarning,

	/** an error */
	kASReportError
};
typedef ASEnum16 ASReportType;

#define kMoreTextKey	"MoreText"


/**
	A report proc can be used to report errors, warnings, and 
	other messages to the user. Normally a report proc will 
	notify the user of an error using a dialog, but in some 
	contexts (such as when batch processing) it may log the 
	error or warning to a file, or ignore it. 

	It is the ASReportProc's responsibility to destroy all objects 
	passed to it, and it may do so at any time. 
	@param reportType The type of information that is reported. 
	
	@param errorCode An error code defined by the system or 
	by ASRegisterErrorString. If message is not NULL, the errorCode 
	can be 0. 
	@param message Specifies the text the user should read. 
	If the message field is NULL the system will retrieve the 
	message associated with the errorCode. If both message and 
	errorCode are specified the message argument is used. 
	@param replacementText If the replacementText is not NULL, 
	the system will attempt to replace the string '%s' in the 
	message with the replacement text. This applies whether 
	the text is specified via the message argument or retrieved 
	from the system using the errorCode argument. 
	@param moreInfo Not used currently. The report proc will 
	destroy this cabinet immediately. 
	@param reportProcData A pointer to the data associated 
	with the reportProc (which should be passed to you when 
	you acquire the report proc).
	@see AVAppGetReportProc 
	@see AVCommandGetReportProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASReportProc)
	(ASReportType reportType, ASInt32 errorCode, ASText message, ASText replacementText,
	 ASCab moreInfo, void *reportProcData);

/** */
typedef ACCBPROTO1 ASText (ACCBPROTO2 *ASTextEvalProc)(ASCab params);


typedef struct _t_ASStatusMonitorProcs {

	/** size of the structure. */
	ASSize_t			size;

	/** pointer to the progress monitor. */
	ASProgressMonitor	progMon;

	/** pointer to client data, can be any type. */
	void				*progMonClientData;

	/**
	This call has been replaced by ASCancelProc. 

	Callback to check for canceling operations. A CancelProc 
	is typically passed to some method that takes a long time 
	to complete. At frequent intervals, the method calls the 
	CancelProc. If it returns true, then the method cancels 
	its operation; if false, it continues. 
	@param clientData User-supplied data that was passed to 
	the CancelProc.
	@return true if the processing is canceled, false otherwise. 
	@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
	
	@see AVAppGetCancelProc 
*/
	ASCancelProc		cancelProc;

	/** pointer to client data for the cancel procedure. */
	void				*cancelProcClientData;

	/** the report procedure. */
	ASReportProc		reportProc;

	/** pointer to client data for the report procedure. */
	void				*reportProcClientData;
} ASStatusMonitorProcsRec, *ASStatusMonitorProcs;

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_ASExtraExpT */
