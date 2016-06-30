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

 ASProcs.h

 - Catalog of functions exported by AcroSupport.

*********************************************************************/

#if DEBUG

/**
	Allocates and returns a pointer to a memory block containing 
	the specified number of bytes. 
	@param nBytes IN/OUT? The number of bytes for which space is allocated. 
	
	@return Pointer to the allocated memory. Returns NULL on failure. 
	
	@see ASrealloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void *, ASmalloc, (os_size_t nBytes), ASHFTClientASmalloc)

/**
	If possible, extends the given block and simply returns 
	ptr. Otherwise, allocates a new block of newNBytes bytes, 
	copies the contents at the old pointer into the new block, 
	frees the old pointer, and returns the pointer to the new 
	block. If a new block cannot be allocated, the call fails 
	and ptr is not freed. Reallocating a block to a smaller 
	size will never fail. 
	@param ptr IN/OUT? The existing memory block. 
	@param newNBytes IN/OUT? The number of bytes the memory block must 
	be able to hold. 
	@return Pointer to memory block. 
	@see ASmalloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void *, ASrealloc, (void *ptr, os_size_t newNBytes), ASHFTClientASrealloc)

/**
	Frees the specified memory block. 
	@param ptr IN/OUT? The block of memory to free. 
	@see ASmalloc 
	@see ASrealloc 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
SPROC(void, ASfree, (void *ptr), ASHFTClientASfree)
#else

/**
	Allocates and returns a pointer to a memory block containing 
	the specified number of bytes. 
	@param nBytes IN/OUT? The number of bytes for which space is allocated. 
	
	@return Pointer to the allocated memory. Returns NULL on failure. 
	
	@see ASrealloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void *, ASmalloc, (os_size_t nBytes))

/**
	If possible, extends the given block and simply returns 
	ptr. Otherwise, allocates a new block of newNBytes bytes, 
	copies the contents at the old pointer into the new block, 
	frees the old pointer, and returns the pointer to the new 
	block. If a new block cannot be allocated, the call fails 
	and ptr is not freed. Reallocating a block to a smaller 
	size will never fail. 
	@param ptr IN/OUT? The existing memory block. 
	@param newNBytes IN/OUT? The number of bytes the memory block must 
	be able to hold. 
	@return Pointer to memory block. 
	@see ASmalloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void *, ASrealloc, (void *ptr, os_size_t newNBytes))

/**
	Frees the specified memory block. 
	@param ptr IN/OUT? The block of memory to free. 
	@see ASmalloc 
	@see ASrealloc 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, ASfree, (void *ptr))
#endif


/**
	Gets a string describing the specified error/exception. 
	
	@param errorCode The exception whose error string is obtained. 
	This must be a full error code, built with the ErrBuildCode 
	macro or a user-defined exception returned from ASRegisterErrorString. 
	See Errors for a list of predefined exceptions. 
	@param buffer (Filled by the method) Buffer into which 
	the string is written.  Make sure to memset the buffer to 0 before calling
	ASGetErrorString.
	@param lenBuffer The number of characters that buffer 
	can hold.
	@return A useful pointer to buffer.  This does not validate whether the function worked.
	You must call strlen on the returned buffer (as long as you memset the buffer to 0) to
	validate whether the error code was valid.
	@see ASGetExceptionErrorCode 
	@see ASRegisterErrorString 
	@see ASRaise 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(const char *, ASGetErrorString, (ASErrorCode errorCode, char *buffer, ASTArraySize lenBuffer))

/**
	Registers a new error and string. The error can be used 
	to raise a plug-in-specific exception using ASRaise. When 
	the exception is raised, its error string can be retrieved 
	using ASGetErrorString and reported to the user using AVAlertNote. 
	

	The error system is automatically forced to be ErrSysXtn. 
	(See the list of Error Systems.) 

	The error is automatically assigned an error code that is 
	not used by any other plug-in (in the current implementation, 
	the Acrobat viewer increments a counter each time any plug-in 
	requests an error code, and returns the value of the counter). 
	As a result, plug-ins cannot rely on being assigned the 
	same error code each time the Acrobat viewer is launched. 
	
	@param severity The severity of the error being defined. 
	Must be one of the Severities. 
	@param errorString The string describing the exception. 
	This string is used by ASGetErrorString. errorString is 
	copied by ASRegisterErrorString; it may be freed by the 
	plug-in after registering the error.
	@return The newly created error code. Plug-ins should assign the 
	error code returned by this method to a variable if they 
	wish to use the error code later in the current session. 
	
	@see ASGetErrorString 
	@see ASRaise 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASRegisterErrorString, (ASErrSeverity severity, const char *errorString))

/**
	Creates a new Host Function Table (HFT) server. An HFT server 
	is responsible for creating an instance of an HFT with the 
	specified version number, and destroying the HFT. 
	@param name The new HFT server's name. 
	@param serverProc (Required) User-supplied callback that 
	provides an HFT when given a version number. This procedure 
	is called by ASExtensionMgrGetHFT when another plug-in imports 
	the HFT. 
	@param destroyProc (Optional) User-supplied callback that 
	destroys the specified HFT (generally, this means de-allocating 
	the memory associated with the HFT). This procedure is called 
	by HFTDestroy. 
	@param clientData Pointer to user-supplied data to pass 
	to the HFT server.
	@return The newly created HFT server. 
	@see HFTServerDestroy 
	@see HFTNewEx
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFTServer, HFTServerNew, (const char *name, HFTServerProvideHFTProc serverProc, HFTServerDestroyProc destroyProc, void *clientData))

/**
	Destroys an HFT server. Call this method only if the HFT 
	will not be used again. 
	@param hftServer IN/OUT? The HFT server to destroy. 
	@see HFTServerNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, HFTServerDestroy, (HFTServer hftServer))


/**
	Obsolete.  See HFTNewEx. Creates a new HFT by calling the specified HFT server's 
	HFTServerProvideHFTProc. 
	@param hftServer The HFT server for the HFT being created. 
	The HFT server must have been created previously using HFTServerNew. 
	
	@param numSelectors The number of entries in the new HFT. 
	This determines the number of methods that the HFT can contain; 
	each method occupies one entry.
	@return The newly created HFT. 
	@see ASExtensionMgrGetHFT 
	@see HFTDestroy
	@see HFTNewEx
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFT, HFTNew, (HFTServer hftServer, ASTCount numSelectors))

/**
	Destroys an existing HFT by freeing all the HFT's memory. 
	Call this method only if you are absolutely sure that neither 
	your plug-in nor any other plug-in will use the HFT again. 
	Because this is usually impossible to know, plug-ins should 
	not destroy HFTs. It is even dangerous to destroy an HFT 
	at unload time, because the order in which plug-ins are 
	unloaded is not specified. 
	@param hft The HFT to destroy.
	@see HFTNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, HFTDestroy, (HFT hft))

/**
	Replaces the specified entry in the specified HFT. This 
	allows a plug-in to override and replace certain methods 
	in Acrobat's API. See Replaceable Methods for a list of 
	replaceable methods. This method can be used from anywhere 
	in the plug-in, but it makes the most sense for most plug-ins 
	to replace methods in the importReplaceAndRegisterCallback 
	procedure. Plug-ins register their HFTs in the export callback, 
	but the code to populate the function table is only executed 
	when the first client requests the HFT. 

	Plug-ins can use the REPLACE macro instead of calling HFTReplaceEntry 
	directly. 

	All plug-ins, and Acrobat itself, share a single copy of 
	each HFT. As a result, when a plug-in replaces the implementation 
	of a method, all other plug-ins and Acrobat also use the 
	new implementation of that method. In addition, once a method's 
	implementation has been replaced, there is no way to remove 
	the new implementation without restarting Acrobat. 

	NOTE: The CALL_REPLACED_PROC macro is available to call 
	the previous HFT entry function that was replaced. 
	@param hft The HFT in which a method is replaced. Use 
	ASExtensionMgrGetHFT to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT for these HFTs. See HFT Values for 
	a list of these global variables. 
	@param sel The entry in the HFT to replace, derived from 
	the method's name by appending SEL. For example, to replace 
	AVAlert, sel must be AVAlertSEL. 
	@param newEntry The function to replace the current one. 
	The function pointer must be converted to an HFTEntry using 
	the ASCallbackCreateReplacement macro. 
	@param flags The new entry's properties. Currently, only 
	HFTEntryReplaceable is defined.
	@exception xmErrCannotReplaceSelector 
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see CALL_REPLACED_PROC 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, HFTReplaceEntry, (HFT hft, Selector sel, HFTEntry newEntry, ASFlagBits flags))

/**
	Gets the HFTEntry that was replaced by the specified HFTEntry 
	in the specified entry. Plug-ins should generally not use 
	this method directly, but use the CALL_REPLACED_PROC macro 
	instead. 

	It is necessary to specify both a selector (the index of 
	an entry in the HFT's table of callback pointers) and an 
	HFTEntry (a callback pointer) because a method may be replaced 
	several times, and the various replacement methods are kept 
	in a linked list. The selector determines which linked list 
	is examined, and the HFTEntry determines the entry in the 
	linked list to return. 
	@param hft The HFT in which a replaced entry is retrieved. 
	See HFTReplaceEntry for more information. 
	@param sel The selector whose previous value is obtained. 
	See HFTReplaceEntry for more information. 
	@param oldEntry The HFTEntry for which the previous value 
	is obtained.
	@return The entry present prior to being replaced by replacer. Returns 
	NULL if the entry has not been replaced. 
	@see ASExtensionMgrGetHFT 
	@see CALL_REPLACED_PROC 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFTEntry, HFTGetReplacedEntry, (HFT hft, Selector sel, HFTEntry oldEntry))

#if ACROBAT_LIBRARY

/**
	Multiplies two fixed numbers. 
	@param a, b The two numbers to multiply.
	@return The product of a and b. 
	@see ASFixedDiv 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASFixedMul, (ASFixed a, ASFixed b))

/**
	Divides two fixed numbers. 
	@param a The dividend. 
	@param b The divisor.
	@return The quotient a / b. 
	@see ASFixedMul 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASFixedDiv, (ASFixed a, ASFixed b))

/**
	Converts a fixed number to a Cstring. 
	@param f The fixed number to convert. 
	@param s (Filled by the method) The string corresponding 
	to f. 
	@param maxLength The maximum number of characters that 
	s can contain. 
	@param precision The number of digits to retain in the 
	converted number. NOTE: The precision for Mac OS numbers 
	is valid to 9 significant digits.
	@see ASCStringToFixed 
	@see Fixed Numbers 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedToCString, (ASFixed f, char *s, os_size_t maxLength, ASSmallCount precision))

/**
	Converts a Cstring to a fixed point number. Processes the 
	string from left to right only until the first invalid character 
	is located (for example, a-z, A-Z). 
	@param s A Cstring to convert.
	@return Fixed number corresponding to s. Returns 0 if the string 
	does not contain any valid number. 
	@see ASFixedToCString 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASCStringToFixed, (const char *s))

/**
	Multiplies two matrices. 
	@param result (Filled by the method) Pointer to matrix 
	m2 x m1. It is OK for result to point to the same location 
	as either m1 or m2. 
	@param m1 Pointer to the ASFixedMatrix value for the first 
	matrix to multiply. 
	@param m2 Pointer to the ASFixedMatrix value for the second 
	matrix to multiply.
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixConcat, (ASFixedMatrixP result, const ASFixedMatrix *m1,const ASFixedMatrix *m2))

/**
	Inverts a matrix. 

	If a matrix is nearly singular (that is, has a determinant 
	nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. 
	@param result (Filled by the method) Pointer to m-1. It 
	is OK for result to point to the same location as m. 
	@param m Pointer to the ASFixedMatrix to invert.
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixInvert, (ASFixedMatrixP result, const ASFixedMatrixP m))

/**
	Transforms the point p through the matrix m, puts result 
	in result. p and result can point to the same place. 
	@param result (Filled by the method) Pointer to the ASFixedPoint 
	containing the result of transforming p through m. It is 
	OK for result to point to the same location as m. 
	@param m Pointer to the ASFixedMatrix through which p 
	is transformed. 
	@param p Pointer to the ASFixedPoint representing the 
	point to transform through m.
	@see ASFixedMatrixTransformRect 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransform, (ASFixedPointP result, const ASFixedMatrixP m,const ASFixedPointP p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) Pointer to the ASFixedRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is OK for result to point to the same location 
	as m. result will always have bottom < top and left < right. 
	
	@param m Pointer to the ASFixedMatrix containing the matrix 
	through which r is transformed. 
	@param r Pointer to the ASFixedRect containing the 
	rectangle to transform through m.
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransformRect, (ASFixedRectP result, const ASFixedMatrixP m,const ASFixedRectP r))
#else

/**
	Multiplies two fixed numbers. 
	@param a, b The two numbers to multiply.
	@return The product of a and b. 
	@see ASFixedDiv 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASFixedMul, (ASFixed a, ASFixed b), ASHFTClientASFixedMul)

/**
	Divides two fixed numbers. 
	@param a The dividend. 
	@param b The divisor.
	@return The quotient a / b. 
	@see ASFixedMul 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASFixedDiv, (ASFixed a, ASFixed b), ASHFTClientASFixedDiv)

/**
	Converts a fixed number to a Cstring. 
	@param f The fixed number to convert. 
	@param s (Filled by the method) The string corresponding 
	to f. 
	@param maxLength The maximum number of characters that 
	s can contain. 
	@param precision The number of digits to retain in the 
	converted number. NOTE: The precision for Mac OS numbers 
	is valid to 9 significant digits.
	@see ASCStringToFixed 
	@see Fixed Numbers 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASFixedToCString, (ASFixed f, char *s, os_size_t maxLength, ASSmallCount precision), ASHFTClientASFixedToCString)

/**
	Converts a Cstring to a fixed point number. Processes the 
	string from left to right only until the first invalid character 
	is located (for example, a-z, A-Z). 
	@param s A Cstring to convert.
	@return Fixed number corresponding to s. Returns 0 if the string 
	does not contain any valid number. 
	@see ASFixedToCString 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASCStringToFixed, (const char *s), ASHFTClientCStringToASFixed)

/**
	Multiplies two matrices. 
	@param result (Filled by the method) Pointer to matrix 
	m2 x m1. It is OK for result to point to the same location 
	as either m1 or m2. 
	@param m1 Pointer to the ASFixedMatrix value for the first 
	matrix to multiply. 
	@param m2 Pointer to the ASFixedMatrix value for the second 
	matrix to multiply.
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixConcat, (ASFixedMatrixP result, const ASFixedMatrix *m1,const ASFixedMatrix *m2))

/**
	Inverts a matrix. 

	If a matrix is nearly singular (that is, has a determinant 
	nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. 
	@param result (Filled by the method) Pointer to m-1. It 
	is OK for result to point to the same location as m. 
	@param m Pointer to the ASFixedMatrix to invert.
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixInvert, (ASFixedMatrixP result, const ASFixedMatrixP m))

/**
	Transforms the point p through the matrix m, puts result 
	in result. p and result can point to the same place. 
	@param result (Filled by the method) Pointer to the ASFixedPoint 
	containing the result of transforming p through m. It is 
	OK for result to point to the same location as m. 
	@param m Pointer to the ASFixedMatrix through which p 
	is transformed. 
	@param p Pointer to the ASFixedPoint representing the 
	point to transform through m.
	@see ASFixedMatrixTransformRect 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransform, (ASFixedPointP result, const ASFixedMatrixP m,const ASFixedPointP p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) Pointer to the ASFixedRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is OK for result to point to the same location 
	as m. result will always have bottom < top and left < right. 
	
	@param m Pointer to the ASFixedMatrix containing the matrix 
	through which r is transformed. 
	@param rectIn Pointer to the ASFixedRect containing the 
	rectangle to transform through m.
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransformRect, (ASFixedRectP result, const ASFixedMatrixP m,const ASFixedRectP rectIn))
#endif

/**
	NOTE: This method was deprecated in version 5.0. Use ASFileSysCreatePathName 
	instead. 

	Converts a platform-specific pathname to an ASPathName. 
	It can create an ASPathName from a file path where the file 
	does not already exist. It works for Windows UNC pathnames 
	as well. 

	It is the caller's responsibility to release the returned 
	ASPathName. 
	@param platformPath Pointer to a platform-specific pathname. 
	In Windows and Unix, it is a null-terminated string containing 
	the full pathname with the appropriate path separators for 
	each platform. 
	@return The ASPathName corresponding to platformPath. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASPathName, ASPathFromPlatformPath, (void *platformPath))

/**
	Gets the default/ standard file system implementation for 
	a platform. 
	@return The platform's default file system. 
	@see ASFileRegisterFileSys 
	@see ASPathFromPlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASFileSys, ASGetDefaultFileSys, (void))

/**
	Converts a filename, specified as an ASPathName, to a device-independent 
	pathname. It is the caller's responsibility to free the 
	memory associated with the returned string using ASfree. 
	

	NOTE: This method can only be used to get host encoding. 
	For any other encoding, use ASFileSysDIPathFromPathEx in 
	Acrobat 6.0. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The ASPathName to convert. 
	@param relativeToThisPath (May be NULL) The pathname relative 
	to which the device-independent pathname is specified. If 
	NULL, the device-independent pathname will be an absolute, 
	not a relative, pathname.
	@return Device-independent pathname corresponding to the parameter 
	values supplied, or NULL if the operation is not supported 
	by the file system. 

	See Section 3.10 in the PDF Reference for a description 
	of the device-independent pathname format. This pathname 
	may not be understood on another platform since drive specifiers 
	may be prepended. 
	@see ASGetDefaultFileSys 
	@see ASFileSysPathFromDIPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(char *, ASFileSysDIPathFromPath, ( ASFileSys fileSys, ASPathName path, ASPathName relativeToThisPath))

/**
	Converts a device-independent pathname to an ASPathName. 
	This method can only be used for files that already exist 
	(that is, it cannot be used to create a placeholder pathname 
	for files that a plug-in intends to create in the future). 
	

	It is the caller's responsibility to release the returned 
	ASPathName. 

	NOTE: In Acrobat 6.0, use ASFileSysPathFromDIPathEx instead 
	for anything other than host encoding. 
	@param fileSys (May be NULL) The file system that the 
	ASPathName will be created within. Pass NULL to use the 
	default file system. 
	@param diPath The device-independent pathname to convert. 
	See Section 3.10 in the PDF Reference for a description 
	of the device-independent pathname format. This pathname 
	may not be understood on another platform since drive specifiers 
	may be prepended. In Windows, you cannot specify a UNC pathname. 
	You must have a file mounted on the file server. For example, 
	the following path is valid: /f/dirname/file.pdf where f 
	is \\server\\people. The following does not work: /server/people/dirname/file.pdf 
	
	@param relativeToThisPath Pathname relative to which diPath 
	is interpreted. If NULL, diPath is interpreted as an absolute 
	pathname, not a relative pathname.
	@return ASPathName corresponding to the parameter values supplied. 
	Returns NULL if diPath cannot be converted to an ASPathName 
	or if the specified file does not already exist. 
	@exception genErrNoMemory 
	@see ASFileSysDIPathFromPath 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASPathName, ASFileSysPathFromDIPath, (ASFileSys fileSys, char * diPath, ASPathName relativeToThisPath))

/**
	Generates and copies the specified ASPathName (but does 
	not copy the file specified by the pathname). The ASPathName 
	must have been obtained through the specified file system. 
	This method may be used regardless of whether the file specified 
	by pathname is open. 

	It is the caller's responsibility to release the ASPathName 
	using ASFileSysReleasePath when it is no longer needed. 
	
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName to copy.
	@return A copy of pathName. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASPathName, ASFileSysCopyPath, (ASFileSys fileSys, ASPathName pathName), ASFileSysCopyPathName)

/**
	Decrements the internal reference count for pathname and 
	disposes of the pathname (but not the file itself ) if the 
	reference count is zero. This does not result in any file-level 
	operations, and is unaffected by whether there is an open 
	file for this pathname. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName to release.
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASFileSysReleasePath, (ASFileSys fileSys, ASPathName pathName), ASFileSysReleasePathName)

/**
	Attempts to open a file in the specified file system, in 
	the specified read/write/create mode. If the file is already 
	open, the existing file handle is returned. The caller retains 
	ownership of pathName.
	
	This call returns an error if a file over 2GB in length is opened.
	ASFileSysOpenFile64 should be used instead of this call wherever
	possible and must be used if files over 2GB in length may be
	encountered

	In Mac OS, when this method creates a file, the file's creator 
	is set to 'CARO' and its type is set to 'PDF ' (with a 'space' 
	after PDF). 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The pathname of the file to open. 
	@param mode An open-mode value as specified for ASFileMode. 
	
	@param fP (Filled by the method) The ASFile that was 
	opened. 
	@return 0 if the operation was successful, otherwise returns a nonzero 
	error code. The error is platform- and file-system specific.\n\n
	Windows:\n
	Returns fileErrWrPerm if trying to open a read-only file with write permissions. 
	ErrSysXtnMgr (GetLastError()) - Platform-specific (any error condition 
	that CreateFile may use).
	Returns fileErrGeneral if the developer passed in an invalid ASPathName.\n\n
	Macintosh:\n
	Returns fileErrFNF if trying to open file for reading that does not exist. 
	ErrSysMDSystem (iErr) - Platform-specific error  (any error that FSpCreate, 
	 FSpSetFInfo, FSpOpenRF, FSpOpenDF, or SetFPos may use). 
	
	@exception genErrNoError 
	@see ASFileClose 
	@see ASFileReopen 
	@see ASGetDefaultFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASFileSysOpenFile, (ASFileSys fileSys, ASPathName pathName, ASFileMode mode, ASFile *fP))

/**
	Attempts to delete the file referred to by pathName. 

	NOTE: If a file is already open for this pathName, the semantics 
	of ASFileSysRemoveFile are file system-dependent. Make sure 
	you have closed all ASFiles for pathName before calling 
	ASFileSysRemoveFile. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The file to delete.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASErrorCode, ASFileSysRemoveFile, (ASFileSys fileSys, ASPathName pathName), ASFileSysRemove)


/**
	Attempts to reopen a file using the specified read/write 
	mode. On some platforms, this may result in the file being 
	closed and then reopened, and thus some error conditions 
	may leave file invalid.
	NOTE: The file mode and return types changed in 0x00060000.
	@param aFile The file to reopen. 
	@param mode An open-mode value as specified for ASFileMode.
	@return Zero if the operation was successful, otherwise some file 
	system/platform dependent error code is returned. 
	@see ASFileSysOpenFile 
	@see ASFileClose 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASFileReopen, ( ASFile aFile, ASFileMode mode ))

/**
	Closes the specified file. After a call to ASFileClose, 
	the file handle is no longer valid but may be reused as 
	the result of a subsequent call to ASFileSysOpenFile. 
	@param aFile IN/OUT? The file to close. The file must have been 
	opened previously using ASFileSysOpenFile. 
	@return Zero if the operation was successful, otherwise some file 
	system/ platform dependent error code is returned. 
	@see ASFileFlush 
	@see ASFileReopen 
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASFileSysOpenFile 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASErrorCode, ASFileClose, ( ASFile aFile ))

/**
	Seeks to the specified position in a file. This is the position 
	at which the next read or write will begin. This call only works
	when the desired file position is less than 2GB.
	@param aFile IN/OUT? The file in which to seek. 
	@param pos IN/OUT? The position to seek. 
	@exception fileErrIO
	@see ASFileSetPos64
	@see ASFileGetPos 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, ASFileSetPos, ( ASFile aFile, ASTFilePos pos ))

/**
	Gets the current seek position in a file. This is the position 
	at which the next read or write will begin. This call returns an
	error if the file position is greater than 2GB.
	@param aFile IN/OUT? The file in which to get the seek position. 
	
	@return The current seek position. 
	@exception fileErrIO
	@see ASFileGetPos64
	@see ASFileSetPos 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTFilePos, ASFileGetPos, ( ASFile aFile ))

/**
	Changes the size of a file. The new size may by larger or 
	smaller than the original size. This method may raise file 
	system/platform specific exceptions. This call only works
	when the desired file size is less than 2GB.
	@param aFile The file whose size is changed. 
	@param newFileSize The new size of file.
	@return fileErrIO
	@see ASFileSetEOF64
	@see ASFileCanSetEOF 
	@see ASFileGetEOF 
	@see ASFileGetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFileSetEOF, ( ASFile aFile, ASTFilePos newFileSize ))

/**
	Gets the current size of a file. Calls ASFileSysGetEofProc. 
	This call returns an error if the file size is greater than 2GB.
	@param aFile The ASFile whose size is obtained.
	@return The size of the file. 
	@exception fileErrIO
	@see ASFileGetEOF64
	@see ASFileSetEOF 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTFilePos, ASFileGetEOF, ( ASFile aFile ))

/**
	Reads data from a file, beginning at the current seek position. 
	
	@param aFile IN/OUT? The file from which data is read. 
	@param p IN/OUT? (Filled by the method) A buffer into which 
	data is written. The buffer must be able to hold at least 
	count bytes. 
	@param count IN/OUT? The number of bytes to read. 
	@return The number of bytes actually read from the file. 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@see ASFileSetPos 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTArraySize, ASFileRead, ( ASFile aFile, char *p, ASTArraySize count ))

/**
	Writes data to a file, beginning at the current seek position. 
	
	@param aFile IN/OUT? The file to which data is written. 
	@param p IN/OUT? A buffer holding the data that is to be written. 
	The buffer must be able to hold at least count bytes. 
	@param count IN/OUT? The number of bytes to write. 
	@return The number of bytes actually written to the file. 
	@exception fileErrIO 
	@exception fileErrWrite 
	@see ASFileRead 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTArraySize, ASFileWrite, ( ASFile aFile, const char *p, ASTArraySize count ))

/**
	Flushes any buffered data to a file. This method may raise 
	file system/platform specific exceptions. 
	@param aFile The file whose data is flushed.
	@exception fileErrIO 
	@see ASFileHardFlush 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFileFlush, ( ASFile aFile ))

/**
	Gets the pathname for file and increments an internal reference 
	count. It is the caller's responsibility to release the 
	ASPathName using ASFileSysReleasePath when it is no longer 
	needed. 
	@param aFile IN/OUT? The file whose pathname is acquired. 
	@return The ASPathName. associated with asFile. You can use ASFileSysDIPathFromPath 
	to convert this to a device-independent pathname. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASPathName, ASFileAcquirePathName, (ASFile aFile))

/**
	Gets the file system that file was opened through. 
	@param aFile IN/OUT? The open file whose file system is obtained. 
	
	@return The file's ASFileSys. 
	@see ASFileGetFileSysByName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASFileSys, ASFileGetFileSys, (ASFile aFile))

/**
	Internal only function
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void *, ASDebug, (ASInt32 op, void *parm, ASTArraySize parmLen, void *clientData))


/**
	Creates a read-only ASStm from a file. The stream is seek-able. 
	
	@param afile The open file to associate with the stream. 
	The file must have been opened previously using ASFileSysOpenFile. 
	Each open file has an unique ASFile. The ASFile value has 
	meaning only to the common ASFile implementation and bears 
	no relationship to platform-specific file objects. 
	@param bufSize Length of data buffer, in bytes. If bufSize 
	= 0, the default buffer size (currently 4kB) will be used. 
	The default is generally reasonable. A larger buffer size 
	should be used only when data in the file will be accessed 
	in chunks larger than the default buffer. Although bufSize 
	is passed as an ASUns16, it is treated internally as an 
	ASInt16. As a result, buffer sizes above 32kB are not permitted.
	@return The newly created ASStm. 
	@see ASFileSysOpenFile 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmClose 
	@see ASStmRead 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASFileStmRdOpen, (ASFile afile, ASSmallBufferSize bufSize))

/**
	Creates a read-only ASStm from a memory-resident buffer. 
	The stream is seek-able. 
	@param data Buffer containing the data to read into the 
	stream. This data buffer must not be disposed of until the 
	ASStm is closed. 
	@param len Length of data, in bytes.
	@return The newly created ASStm. 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASMemStmRdOpen, (char *data, ASArraySize len))

/**
	Creates a read-only ASStm from an arbitrary data-producing 
	procedure. The stream is not seek-able. 

	readProc is called when the client of the stream attempts 
	to read data from it. 
	@param readProc User-supplied callback that supplies the 
	stream's data. 
	@param clientData Pointer to user-supplied data to pass 
	to readProc each time it is called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@see ASFileStmRdOpen 
	@see ASMemStmRdOpen 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASProcStmRdOpen, (ASStmProc readProc, void *clientData))


/**
	Reads data from stm into memory. 
	@param ptr (Filled by the method) Buffer into which 
	data is written. 
	@param itemSize Number of bytes in an item in the stream. 
	See the description of nItems for further information. 
	@param nItems Number of items to read. The amount of data 
	read into the memory buffer will be itemSize x nItems, unless 
	an EOF is encountered first. The relative values of itemSize 
	and nItems really do not matter; the only thing that matters 
	is their product. It is often convenient to set itemSize 
	to 1, so that nItems is the number of bytes to read. 
	@param stm The stream from which data is read.
	@return The number of items (not bytes) read. 
	@see ASStmWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTCount, ASStmRead, (char *ptr, ASTArraySize itemSize, ASTCount nItems, ASStm stm))

/**
	Writes data from a memory buffer into an ASStm. 

	You cannot use this method to change a PDF page contents 
	stream - only a print stream. 

	Historically, this method was provided to allow plug-ins 
	to write data into the print stream when printing to a PostScript 
	printer (see the PDDocWillPrintPage notification). However, 
	ASStm is a general purpose I/O mechanism in Acrobat, although 
	only limited open and read/write methods are provided in 
	the plug-in API. For instance, not all ASStm objects are 
	seek-able. 
	@param ptr Buffer from which data is read. 
	@param itemSize Number of bytes in an item in the stream. 
	See the description of nItems for additional information. 
	
	@param nItems Number of items to write. The amount of 
	data written into the stream will be itemSize x nItems. 
	The relative values of itemSize and nItems really do not 
	matter; the only thing that matters is their product. It 
	is often convenient to set itemSize to 1, so that nItems 
	is the number of bytes to read. 
	@param stm The stream into which data is written.
	@return The number of items (not bytes) written. 
	@see ASStmRead 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTCount, ASStmWrite, (const char *ptr, ASTArraySize itemSize, ASTCount nItems, ASStm stm))

/**
	Closes the specified stream. 
	@param stm The stream to close.
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASStmClose, (ASStm stm))

/* Acrobat 2.2 additions */

/**
	Allows a fileSys to be unregistered. In general, a fileSys 
	is only unregistered by the extension that registered it. 
	
	@param extension IN/OUT? The gExtensionID of the plug-in un-registering 
	fileSys. 
	@param fileSys IN/OUT? The ASFileSys to un-register. 
	@return true if fileSys successfully unregistered, false if any 
	there are any open files that were opened through fileSys. 
	
	@see ASFileRegisterFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileUnregisterFileSys, (ASExtension extension, ASFileSys fileSys))

/**
	Sends data from a file system implementation to an ASFile. 
	The data may be for an multi-read request call, or may be 
	unsolicited. 

	This method can only be called from within file system implementation. 
	It must not be called by clients of the ASFile, for example, 
	by a caller that acquired the file with ASFileSysOpenFile. 
	
	@param aFile IN/OUT? The file to which data is sent. 
	@param p IN/OUT? The data being pushed. 
	@param offset IN/OUT? Byte offset into file at which the data should 
	be written. 
	@param length IN/OUT? The number of bytes held in buffer. 
	@exception fileErrGeneral 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(void, ASFilePushData, (ASFile aFile, const char *p, ASTFilePos offset, ASTArraySize length))

/**
	Allows an implementor to provide a file system for use by 
	external clients. An external client can locate the file 
	system using ASFileGetFileSysByName. fileSys provides its 
	name via the ASFileSysGetFileSysNameProc callback. This 
	method returns false if a file system with the same name 
	is already registered. 
	@param extension IN/OUT? The gExtensionID of the plug-in registering 
	the fileSys. 
	@param fileSys IN/OUT? The fileSys being registered. 
	@return true if fileSys is successfully registered, false otherwise. 
	
	@see ASFileUnregisterFileSys 
	@see ASFileGetFileSysByName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileRegisterFileSys, (ASExtension extension, ASFileSys fileSys))

/**
	Gets the file system that was registered with the specified 
	name. 
	@param name IN/OUT? The ASAtom corresponding to the name of the 
	file system to obtain. 
	@return The file system, otherwise NULL if no matching file system 
	was found. 
	@see ASFileGetFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASFileSys, ASFileGetFileSysByName, (ASAtom name))

/**
	Gets the ASFile associated with the specified ASMDFile and 
	ASFileSys. 
	@param mdFile IN/OUT? The ASMDFile for which the information is 
	desired. 
	@param fileSys IN/OUT? The ASFileSys that fileID was opened through. 
	
	@param pfN IN/OUT? (Filled by the method, may be NULL) The ASFile 
	representing fileID within fileSys. 
	@return true if fileID is determined to be a valid file opened through 
	fileSys, false otherwise. 
	@see ASFileGetFileSys 
	@see ASFileGetMDFile 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileFromMDFile, (ASMDFile mdFile, ASFileSys fileSys, ASFile *pfN))

/**
	Given an ASFile, returns the fileSys and the ASMDFile identification 
	in that fileSys. This call is needed for a file system in 
	a plug-in to be able to call the inner routines in another 
	file system. 
	@param fN IN/OUT? The ASFile for which the information is desired. 
	
	@param pFileID IN/OUT? (Filled by the method, may be NULL) The 
	ASMDFile identifier associated with file. 
	@param pFileSys IN/OUT? (Filled by the method, may be NULL) The 
	file system that this file was opened through. 
	@return true if file is an open file, false otherwise. 
	@see ASFileFromMDFile 
	@see ASFileGetFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileGetMDFile, (ASFile fN, ASMDFile *pFileID, ASFileSys *pFileSys))


/**
	Creates an ASPathName based on the input type and pathSpec. 
	Each fileSys implementation must publish the input types 
	that it accepts. 

	It is the caller's responsibility to release the ASPathName 
	using ASFileSysReleasePath when it is no longer needed. 
	

	Developers should consider using the simpler helper macros 
	instead of using the call directly. See the Related Macros 
	section below. 
	@param fileSys (May be NULL) The ASFileSys in which you 
	are trying to create an ASPathName. Pass NULL to use the 
	default file system. 
	@param pathSpecType An ASAtom specifying the data type 
	in pathSpec, as follows:
	
	-  "Cstring" - Accepted by the 
	default file system on all platforms. pathSpec is a null-
	terminated char*. On the Mac it must be an absolute path 
	separated by colons, as in "VolumeName:Folder:file.pdf". 
	On Windows the path may be absolute, as in "C:\\folder\\file.pdf" 
	or relative as in "...\folder\file.pdf". On Unix the 
	path may be absolute as in "/folder/file.pdf" or relative 
	as in ".../folder/file.pdf". 
	-  "FSSpec" - Accepted by the default file system on the
	Mac. pathSpec is a pointer to a valid FSSpec.  
	-  "FSRef" - Accepted by the default file system on the Mac.
	pathSpec is a valid FSRef.  
	-  "FSRefWithCFStringRef" - Accepted by 
	the default file system on the Mac. pathSpec is a pointer 
	to a valid FSRefWithCFStringRefRec.  
	-  "CFURLRef" - Accepted by 
	the default file system on the Mac. pathSpec is a 
	valid CFURLRef.  
	-  "POSIXPath" - Accepted by 
	the default file system on the Mac. pathSpec is a null-
	terminated char* containing a POSIX-style, UTF-8 encoded path 
	string. 
	-  "SFReply" - In the past was accepted 
	by the default file system on the Mac. This type is deprecated 
	and should not be used. 
	-  "DIPath" - Accepted by the default 
	file system on Windows and Mac. pathSpec is a device-independent 
	path as documented in the PDF Reference. pathSpec can contain 
	an absolute or relative path. If a relative path is used, 
	the method will evaluate that path against an ASPathName 
	passed in the mustBeZero parameter. 
	-  "DIPathWithASText" - Accepted by the default 
	file system on Windows and Mac. pathSpec is a device-independent 
	path as documented in the PDF Reference in the form of an ASText. pathSpec can contain 
	an absolute or relative path. If a relative path is used, 
	the method will evaluate that path against an ASPathName 
	passed in the mustBeZero parameter. 
	-  "FolderPathName" - Accepted by the default file system on Windows and Mac. 
	pathSpec is an ASPathName that contains the path of a folder. 
	mustBeZero is a C string containing the name of the file. 
	The returned ASPathName contains the result of appending 
	mustBeZero to pathSpec.  
	-  "FolderPathNameWithASText" - Accepted by the default file system on Windows and Mac. 
	pathSpec is an ASPathName that contains the path of a folder. 
	mustBeZero is an ASText containing the name of the file. 
	The returned ASPathName contains the result of appending 
	mustBeZero to pathSpec. 
	
	@param pathSpec The file specification from which to create 
	an ASPathName. Relative paths are evaluated from the directory 
	containing the executable (if used with the PDF Library), 
	or the directory containing Acrobat (if used in a plug-in). 
	
	@param additionalData See pathSpecType parameter description.
	@return The newly created pathname, or NULL on failure. 
	@exception genErrBadParm on Windows if the pathSpecType is not recognized. 
	
	@exception genErrMethodNotImplemented 
	@see ASFileSysCopyPath 
	@see ASFileSysCreatePathFromCString 
	@see ASFileSysCreatePathFromDIPath 
	@see ASFileSysCreatePathFromFSSpec 
	@see ASFileSysCreatePathWithFolderName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
*/
NPROC(ASPathName, ASFileSysCreatePathName, (const ASFileSys fileSys, ASAtom pathSpecType, const void *pathSpec,
							const void *additionalData))

/**
	Converts an ASPathName from one file system to another. 
	Returns an ASPathName acquired through newFileSys that refers 
	to an image (possibly cached) of the file in oldfileSys. 
	Use this call to get a local file that is an image of a 
	remote file (in a URL file system, for example). This is 
	needed by programs such as the Movie Player, because they 
	can only work from local file-system files. The returned 
	ASPathName may be a reference to a cache, so the file should 
	be treated as read-only. 

	Because of the possibility of cache flushing, you must hold 
	a copy of the remote file's ASPathName for the duration 
	of use of the local file. 

	Do not remove the local file copy, since the newFileSys 
	system does not know about the linkage to the remote (oldFileSys) 
	file! 

	The source file does not have to be open. 

	This call is handled by oldFileSys if oldFileSys contains 
	the appropriate procedure. Otherwise it is handled by copying 
	the file. The source file is closed at the end of the copy 
	if it was not open prior to the call. 

	It is the caller's responsibility to release the ASPathName 
	using ASFileSysReleasePath when it is no longer needed. 
	
	@param oldFileSys IN/OUT? (May be NULL) The file system from which 
	oldPathName was obtained. Pass NULL to use the default file 
	system. 
	@param oldPathName IN/OUT? The ASPathname in the current file system. 
	
	@param newFileSys IN/OUT? (May be NULL) The file system to which 
	the oldPathName is converted. Pass NULL to use the default 
	file system. 
	@return The ASPathName in newFileSys or NULL if one can not be made. 
	
	@exception ERR_NOMEMORY 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@exception fileErrWrite 
	@see ASFileSysCreatePathName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASPathName, ASFileSysAcquireFileSysPath, (ASFileSys oldFileSys, ASPathName oldPathName, ASFileSys newFileSys))

/**
	Gets and/or sets the mode flags for a file. Pass 0 for modeValue 
	and modeMask to simply get the current mode flags. 
	@param fN The file for which to get and/or set the mode. 
	
	@param modeValue The mode flag values to get or set, which 
	are described in ASFileMode Flags. 
	@param modeMask Mask for the mode flags to get or set.
	@return The previous value of the mode, or 0 if the file system 
	does not support this operation. 

	NOTE: This operation is primarily intended for slow file 
	systems such as the internet, where there can potentially 
	be an appreciable wait between requesting and retrieving 
	bytes. 
	@see ASFileRead 
	@see ASFileSysOpenFile 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
*/
NPROC(ASFlagBits, ASFileSetMode, (ASFile fN, ASFlagBits modeValue, ASFlagBits modeMask))

/* Acrobat 4.0 additions */

/**
	Creates an ASStm from an arbitrary data-producing procedure. 
	The stream is not seek-able. 
	@param writeProc User-supplied callback that provides 
	the data for the stream. 
	@param destroyProc User-supplied callback that destroys 
	the specified ASStm. (Generally, this means de-allocating 
	the memory associated with the ASStm.) 
	@param clientData Pointer to user-supplied data to pass 
	to writeProc each time it is called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmWrite 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASStm, ASProcStmWrOpen, (ASStmProc writeProc, ASProcStmDestroyProc destroyProc, void *clientData))

/**
	Creates a writable ASStm from a file. The stream is seek-able. 
	
	@param afile The open file to associate with the stream. 
	The file must have been opened previously using ASFileSysOpenFile. 
	Each open file has an unique ASFile. The ASFile value has 
	meaning only to the common ASFile implementation and bears 
	no relationship to platform-specific file objects. 
	@param bufSize Length of a data buffer, in bytes. If bufSize 
	= 0, the default buffer size (currently 4kB) is used. The 
	default is generally reasonable. A larger buffer size should 
	be used only when data in the file will be accessed in chunks 
	larger than the default buffer. Although bufSize is passed 
	as an ASUns16, it is treated internally as an ASInt16. As 
	a result, buffer sizes above 32 KB are not permitted.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASProcStmWrOpen 
	@see ASFileStmRdOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmWrite 
	@see ASStmRead 
	@see ASStmClose 
	@see ASFileSysOpenFile 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASStm, ASFileStmWrOpen, (ASFile afile, ASSmallBufferSize bufSize))

/**
	Tests whether an HFT is valid. 
	@param hft IN/OUT? The HFT to test. 
	@return true if hft is valid, false otherwise. 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
	
*/
NPROC(ASBool, HFTIsValid, (HFT hft))


/* Acrobat 5.0 additions */

/**
	Populates an ASFileSysItemProps record with a full description 
	of the file system object associated with pathName. Calls 
	ASFileSysGetItemPropsProc. 

	NOTE: The method clears the memory associated with itemProps, 
	so the caller need not. However, the caller must explicitly 
	set the props->size field of the ASFileSysItemProps structure 
	before calling this method. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName associated with the object. 
	
	@param props (Filled by the method) Properties structure 
	describing the object. The size field must be set on input.
	@return 0 if no error was encountered; otherwise an error code is 
	returned. If an error code is returned, props will not be 
	filled with valid values. If no file system object is present, 
	an error will not be reported - instead, the props.isThere 
	field will be false. 
	@exception genErrBadParm 
	@exception genErrMethodNotImplemented 
	@see ASFileSysConvertCabToItemProps 
	@see ASFileSysConvertItemPropsToCab 
	@see ASFileSysGetItemPropsAsCab 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysGetItemProps,			(ASFileSys fileSys, ASPathName pathName, ASFileSysItemProps props))

/**
	Creates an iterator which can be used to enumerate all objects 
	inside the specified folder, and also returns the properties 
	of the first item found in the folder. The iteration can 
	be continued by passing the returned ASFolderIterator to 
	ASFileSysNextFolderItem. 

	Both itemProps and itemPath are optional and may be NULL 
	if you are not interested in that information. 

	The client is obligated to eventually free the resources 
	associated with ASFolderIterator by calling ASFileSysDestroyFolderIterator. 
	

	NOTE: The order in which items are enumerated is implementation-
	dependent. In particular, note that items will probably 
	not be iterated in alphabetic order. 

	NOTE: If items are added to or removed from a folder during 
	iteration the results are implementation-dependent. 
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	folderPath was obtained. Pass NULL to use the default file 
	system. 
	@param folderPath IN/OUT? The path associated with the target folder. 
	
	@param props IN/OUT? (Filled by the method, may be NULL) Properties 
	structure describing the first object iterated. 
	@param itemPath (Filled by the method, may be NULL) An ASPathName which
	ASFileSysFirstFolderItem allocated which is associated with the object.
	The caller of ASFileSysFirstFolderItem must free the ASPathName.
	Contains an absolute path on Windows and UNIX.
	@return A valid ASFolderIterator object if folderPath contained 
	any files. NULL will be returned if the folder is empty 
	or the operation is not supported by the file system. 
	@exception genErrBadParm 
	@exception Windows default file system may raise: fileErrFNF, asFileErrNotADir 
	or 
	@exception ERR_NOMEMORY 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASFolderIterator, ASFileSysFirstFolderItem,		(ASFileSys fileSys, ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath))

/**
	Continues the iteration process associated with the ASFolderIterator 
	object. 

	Both itemPath and itemProps are optional and may be NULL 
	if you are not interested in that information. 
	@param fileSys (May be NULL) The file system with which 
	the iteration was started. Pass NULL to use the default 
	file system. 
	@param folderIter An ASFolderIterator object returned 
	from a previous call to ASFileSysFirstFolderItem. 
	@param props (Filled by the method, may be NULL) Properties 
	structure describing the next object in the iteration. 
	@param itemPath (Filled by the method, may be NULL) An ASPathName which
	ASFileSysNextFolderItem allocated which is associated with the object.
	The caller of ASFileSysNextFolderItem must free the ASPathName.
	Contains an absolute path on Windows and UNIX.
	@return true if another object was found, false otherwise. 
	@exception genErrBadParm 
	@exception fileErrGeneral 
	@exception ERR_NOMEMORY 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysDestroyFolderIterator 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASBool,			ASFileSysNextFolderItem,		(ASFileSys fileSys, ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath))

/**
	Releases the resources associated with folderIter. 
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	the iteration was started. Pass NULL to use the default 
	file system. 
	@param folderIter IN/OUT? An ASFolderIterator object returned from 
	a previous call to ASFileSysFirstFolderItem. 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(void,				ASFileSysDestroyFolderIterator, (ASFileSys fileSys, ASFolderIterator folderIter))

/**
	Returns the parent folder of the file system object associated 
	with pathName. The following rules apply in the default 
	file systems: 
	-  pathName may be associated with either a file or a folder. 
	-  the file system object associated with pathName need not 
	exist. 
	
	It is the caller's responsibility to release the returned 
	ASPathName. 
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName IN/OUT? The ASPathName. 
	@return The ASPathName associated with the parent folder. Will return 
	NULL if the parent could not be returned. 
	@exception genErrNoMemory 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@exception fileErrWrite 
	@see ASFileSysCreatePathName 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASPathName,		ASFileSysAcquireParent,			(ASFileSys fileSys, ASPathName pathName))


/**
	Performs a comparison between file and path to determine 
	if they represent the same file. This method will return 
	false if file was not opened through the fileSys file system. 
	

	NOTE: Adobe does not guarantee that this method will work 
	on all file systems. 
	@param fN IN/OUT? The file in question. 
	@param pathName IN/OUT? The ASPathName in question. 
	@param fileSys IN/OUT? The file system from which path was obtained. 
	
	@return false if the comparison fails, true otherwise. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASBool,			ASFileIsSame,					(ASFile fN, ASPathName pathName, ASFileSys fileSys))

/**
	Extracts the filename (including extension) from path. 

	NOTE: In Acrobat 6.0, this method can only be used to get 
	host encoding. For any other encoding, use ASFileSysGetNameFromPathAsASText. 
	
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName associated with the file 
	in question. 
	@param name (Filled by the method) Buffer used to store 
	the filename. 
	@param maxLength Maximum number of bytes that buffer can 
	hold.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. The buffer is returned as 
	a host-encoded C string. 
	@exception fileErrGeneral 
	@see ASFileSysGetNameFromPathAsASText 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysGetNameFromPath,		(ASFileSys fileSys, ASPathName pathName, char *name, ASTArraySize maxLength))

/**
	Returns an unique pathname suitable for use in creating 
	temporary files. It is the caller's responsibility to release 
	the returned object using ASFileSysReleasePath. 

	If siblingPath is non-NULL, the returned ASPathName is created 
	at the same folder level as this path. Otherwise the standard 
	temporary file location is used. 
	@param fileSys (May be NULL) The file system from which 
	siblingPath was obtained. Pass NULL to use the default file 
	system. 
	@param siblingPathName (May be NULL) An ASPathName indicating 
	the desired location of the temporary pathname. The returned 
	ASPathName is created at the same folder level as this path.
	@return The ASPathName if the operation was successful, NULL otherwise. 
	
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASPathName,		ASFileSysGetTempPathName,		(ASFileSys fileSys, ASPathName siblingPathName))

/**
	Gets the amount of free space on the volume containing pathName. 
	
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName IN/OUT? The ASPathName in question. 
	@return The amount of free space, in bytes, 0 otherwise. Because 
	the free space is returned as an ASUns32 it is limited to 
	4 GB. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASDiskSpace,		ASFileSysGetStorageFreeSpace,	(ASFileSys fileSys, ASPathName pathName))

/**
	Flushes the volume on which the specified file resides. 
	This ensures that any data written to the system for the 
	volume containing pathName is flushed out to the physical 
	volume (equivalent to the Macintosh FlushVol or to the UNIX 
	sync). 

	Only the Mac OS default file system implements the callback 
	associated with this method. This is a no-op on Windows 
	and Unix. 
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName IN/OUT? The ASPathName the volume information is 
	obtained from. 
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASErrorCode,		ASFileSysFlushVolume,			(ASFileSys fileSys, ASPathName pathName))

  
/**
	Initiates a byte range request for a given file, if the file is in the browser.
	@param fN The file you wish to make read requests for.
	@param blockPairs The array of Int32 pairs.  The first Int32 in the pair 
	is the offset into the file to read, and the second Int32 is the lengh of
	the range to request
	@param nBlockPairs The number of block pairs to request.
	@see ASFileHasOutstandingMReads
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void,				ASFileMReadRequest,				(ASFile fN, Int32 *blockPairs, ASTCount nBlockPairs ))

/**
	Clears all outstanding mreads for the given file.
	@param fN The file to clear mreads for. 
	@see ASFileHasOutstandingMReads
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void,				ASFileClearOutstandingMReads,	(ASFile fN))

/**
	Returns the URL corresponding to pathName. It is the caller's 
	responsibility to free the memory associated with the returned 
	string using ASfree. 
	@param fileSys IN/OUT? The file system from which path was obtained. 
	Pass NULL to use the default file system. 
	@param path IN/OUT? The ASPathName in question. 
	@return A buffer containing the URL, or NULL if some error occurred. 
	The URL is in the standard 'file:\/\/' URL style. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(char *,			ASFileSysURLFromPath,			(ASFileSys fileSys, ASPathName path))

/**
	Returns the URL associated with file. It is the caller's 
	responsibility to release the memory associated with the 
	returned string using ASfree. 
	@param asf IN/OUT? The file in question. 
	@return A buffer containing the URL or NULL if it could not be determined. 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(char *,			ASFileGetURL,					(ASFile asf))

/**
	Creates an empty folder at the specified pathName. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The path of the folder to create. 
	@param recurse Recursively create the parent folder if 
	necessary. 
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@exception genErrMethodNotImplemented 
	@exception fileErrFNF 
	@see ASFileSysRemoveFolder 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysCreateFolder,			(ASFileSys fileSys, ASPathName path, ASBool recurse))

/**
	Deletes the folder at the specified pathName only if it 
	is empty. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The path of the folder to remove.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@exception genErrMethodNotImplemented 
	@see ASFileSysCreateFolder 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode, 		ASFileSysRemoveFolder,			(ASFileSys fileSys, ASPathName path))

/**
	Gets the file access mode(s) specified for file when it 
	was opened. 
	@param fN The file in question. 
	@return Returns a value corresponding to one or more ASFileMode 
	used to access/create the file, as shown in the table. The 
	values that can be returned include combinations of the 
	following, OR'd with each other: 

	Return value from ASFileGetOpenMode 

	0 (created) 

	1 (readable) 

	2 (readable and writable) 

	8 (sequential access) 

	16 (local)
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASFileMode,		ASFileGetOpenMode,				(ASFile fN))

/**
	Returns a user-friendly representation of a path. It is 
	the caller's responsibility to release the memory associated 
	with the returned string using ASfree. 

	NOTE: In Acrobat 6.0, this method can only be used to get 
	host encoding. For any other encoding, use ASFileSysDisplayASTextFromPath. 
	
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The ASPathName in question.
	@return A buffer containing the display string, or NULL if this 
	operation is not supported by the file system or some error 
	occurred. For example: 

	Windows: "C:\Folder\File" Mac: "Hard Disk:Folder:File" UNIX: 
	"/Folder/File" 
	@see ASFileSysDisplayASTextFromPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(char *, 			ASFileSysDisplayStringFromPath,	(ASFileSys fileSys, ASPathName path))

/**
	Returns the number of seconds elapsed since midnight, January 
	1, 1970, coordinated universal time, up to the current time, 
	using Greenwich mean time (GMT). 
	@return See above. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
SPROC(ASCount,			ASGetSecs,	(void), ASSecs)

/**
	Sets the type and creator of a file. See Type/ Creator Codes. 
	

	NOTE: As is the case for ASFileSysGetTypeAndCreator, this 
	method only applies to the Macintosh default file system. 
	Windows and UNIX make this a no-op. 
	@param fileSys IN/OUT? The file system for which the type and creator 
	are needed. 
	@param path IN/OUT? The pathname of the file. 
	@param type IN/OUT? The type of the file. 
	@param creator IN/OUT? The creator of the file. 
	@return None. 
	@see ASFileSysGetTypeAndCreator 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(void, 			ASFileSysSetTypeAndCreator,		(ASFileSys fileSys, ASPathName path, unsigned long type, unsigned long creator))

/**
	Gets the type and creator of the file specified by the path. 
	See Type/Creator Codes. 

	NOTE: Only meaningful for the Macintosh default file system. 
	Windows and UNIX always return 0 for both type and creator. 
	
	@param fileSys The file system containing the file for 
	which the type and creator are needed. 
	@param path The pathname of the file. 
	@param type (Filled by method) The type of the file. 
	@param creator (Filled by method) The creator of the file.
	@return None. 
	@see ASFileSysSetTypeAndCreator 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, 			ASFileSysGetTypeAndCreator,		(ASFileSys fileSys, ASPathName path, unsigned long *type, unsigned long *creator))

/**
	Tells you whether the given byte is a lead byte of a multi-byte 
	character and how many tail bytes follow. 

	When parsing a string in a host encoding you must keep in 
	mind that the string could be in a variable length multi-byte 
	encoding. In such an encoding (for example, Shift-JIS) the 
	number of bytes required to represent a character varies 
	on a character-by-character basis. To parse such a string 
	you must start at the beginning and, for each byte, ask 
	whether that byte represents a character or is the first 
	byte of a multi-byte character. If the byte is a "lead byte" 
	for a multi-byte character you must also compute how many 
	bytes will follow the lead byte to make up the entire character. 
	Currently the API provides a call (PDHostMBLen) that performs 
	these computations but only if the encoding in question 
	is the OS encoding (as returned by PDGetHostEncoding). ASHostMBLen 
	allows you to ask this question for any byte in any host 
	encoding. 

	See below for an example of how to parse a multi-byte string. 
	
	@param encoding Host encoding type. 
	@param byte The first byte of a multi-byte character.
	@return The number of additional bytes required to form the character. 
	For example, if the encoding is a double-byte encoding the 
	return value will be 1 for a two-byte character and 0 for 
	a one-byte character. For Roman encodings the return value 
	will always be 0. 

	NOTE: ASHostMBLen cannot confirm that required number of 
	trailing bytes actually follow the first byte. If you are 
	parsing a multi-byte string make sure your code will stop 
	at the first null (zero) byte even if it appears immediately 
	after the lead byte of a multi-byte character. 
	@see PDGetHostEncoding 
	@see PDHostMBLen 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASInt32, ASHostMBLen, (ASHostEncoding encoding, ASUns8 byte))

/**
	Causes a hard flush on a file. A 'hard flush' means that 
	the file is flushed to the physical destination. For example, 
	if a WebDAV-based file is opened, ASFileFlush only flushes 
	changes to the local cached version of the file. ASFileHardFlush 
	would flush changes all the way to the WebDAV server. 
	@param aFile The file that is flushed.
	@return Zero if the operation succeeded; -1 if there was an error. 
	
	@exception fileErrIO 
	@see ASFileFlush 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode, ASFileHardFlush, ( ASFile aFile ))

/**
	New version of HFTReplaceEntry. Adds the extension argument. 
	

	Plug-ins can use the REPLACE macro instead of calling HFTReplaceEntryEx 
	directly. 

	NOTE: The CALL_REPLACED_PROC macro is available to call 
	the previous HFT entry function that was replaced. 
	@param hft The HFT in which a method is replaced. Use 
	ASExtensionMgrGetHFT to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT for these HFTs. See HFT Values for 
	a list of these global variables. 
	@param sel The entry in the HFT to replace, derived from 
	the method's name by appending SEL. For example, to replace 
	AVAlert, sel must be AVAlertSEL. 
	@param newEntry The function to replace the current one. 
	The function pointer must be converted to an HFTEntry using 
	the ASCallbackCreateReplacement macro. 
	@param extension Plug-ins should pass in gExtensionID 
	for this parameter (see the code for the Acrobat 5.0 version 
	of the REPLACE macro). This parameter is stored by Acrobat 
	so that any entries that were replaced by a plug-in can 
	be unreplaced in the event that the plug-in unloads. 
	@param flags The new entry's properties. Currently, only 
	HFTEntryReplaceable is defined.
	@exception xmErrCannotReplaceSelector 
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see HFTReplaceEntry 
	@see HFTUnreplaceEntry 
	@see CALL_REPLACED_PROC 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, HFTReplaceEntryEx, (HFT hft, Selector sel, HFTEntry newEntry, ASExtension extension, ASFlagBits flags))

/**
	Removes the oldEntry item from hft at sel if the extension 
	fields match. Allows HFT replacements to be undone in cases 
	such as with the DigSig plug-in, which replaces a method 
	that Acrobat could use after DigSig unloads.. 
	@param hft The HFT in which a method is un-replaced. Use 
	ASExtensionMgrGetHFT to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT for these HFTs. See HFT Values for 
	a list of these global variables. 
	@param sel The entry in the HFT to un-replace, derived 
	from the method's name by appending SEL. For example, to 
	replace AVAlert, sel must be AVAlertSEL. 
	@param oldEntry The old function to be replaced. The function 
	pointer must be converted to an HFTEntry using the ASCallbackCreateReplacement 
	macro. 
	@param extension Object of type ASExtension.
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see HFTReplaceEntry 
	@see HFTReplaceEntryEx 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, HFTUnreplaceEntry, (HFT hft, Selector sel, HFTEntry oldEntry, ASExtension extension))

/* Acrobat 6.0 additions */

/**
	Extends ASProcStmRdOpen. Creates a read-only 
	ASStm from an arbitrary data-producing procedure. The stream 
	is optionably seek-able, although external clients do not
	have the ability to initiate a seek operation.

	The supplied handlers are called when the client of the 
	stream attempts to read data from it, seek it, find it's
	length, and when the client closes it. 
	@param handler Structure containing user-supplied callbacks 
	that supply the stream's data and destroy the stream. 
	@param clientData Pointer to user-supplied data to pass 
	to the procedures each time they are called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASProcStmRdOpen 
	@see ASStmClose 
	@see ASStmRead 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASStm, ASProcStmRdOpenEx, (ASProcStmRdExHandler handler, void *clientData))

/**
	Generates a unique identifier (UUID). 
	@param dst (Filled by the method) The unique identifier 
	created from the hash. 
	@return true if the UUID is successfully created, false otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenUnique, (ASUUID *dst))

/**
	Generates a universal unique identifier (UUID) for a block 
	of data (a name) in a context (a namespace). 
	@param dst (Filled by the method) The unique identifier 
	created from the name. 
	@param ns A namespace or context meaningful to the client. 
	
	@param name A pointer to an arbitrary block of data to 
	be identified by the UUID. 
	@param bytes The number of bytes in name. 
	@return true if the UUID is successfully created, false otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenFromName, (ASUUID *dst, const ASUUID *ns, void *name, ASByteCount bytes))

/**
	Generates a unique identifier (UUID) from a hash value. 
	
	@param dst (Filled by the method) The unique identifier 
	created from the hash. 
	@param hash A hash value, such as MD5.
	@return true if the UUID is successfully created, false otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenFromHash, (ASUUID *dst, ASUns8 hash[16]))

/**
	Parses a C string, such as one generated by ASUUIDToCString, 
	into a unique identifier (UUID). 
	@param dst (Filled by the method) The unique identifier 
	created from the string. 
	@param str A NULL-terminated string from which to generate 
	the unique identifier, in the following form: f81d4fae-7dec-11d0-a765-00a0c91e6bf6
	@return true if the UUID is successfully created, false otherwise. 
	
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDFromCString, (ASUUID *dst, const char *str))

/**
	Generates a NULL-terminated C string from the unique identifier 
	(UUID) for a user or session. 
	@param dst (Filled by the method) A NULL-terminated string 
	from which to generate the unique identifier, in the following 
	form: f81d4fae-7dec-11d0-a765-00a0c91e6bf6 The string must 
	be at least the length specified by ASUUIDMaxStringLen. 
	
	@param src The unique identifier from which to generate 
	the string . 
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void, ASUUIDToCString, (char *dst, const ASUUID *src))

/**
	Deprecated API - always returns NULL.
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void *, ASFileSysGetPlatformThing, (ASFileSys fileSys, ASPathName path, ASAtom thing))

/**
	Returns a platform-specific file-system representation of 
	the specified path, according to the specified type, wrapped 
	in an allocated ASPlatformPath object. Calls ASFileSysAcquirePlatformPathProc. 
	

	This method creates an equivalent platform-specific type 
	(such as FSRef on Mac) from an ASPathName. Use ASFileSysCreatePathName 
	for the reverse, to create an equivalent ASPathName from 
	a platform-specific type. 

	In previous releases, you could cast an ASPathName to an 
	FSSpec (for example), but that does not work in the Acrobat 
	6.0 SDK (because of changes to accommodate long, Unicode 
	filenames on Mac OS X). When developing for Mac OS, use 
	this call to get an FSSpec from an ASPathName safely on 
	Mac OS X, without casting. However, it is recommended that 
	you transition to using newer types such as FSRef to be 
	compatible with OS X filenames, or change to using all ASFileSys 
	methods. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The ASPathName in the file system specified 
	by fileSys. 
	@param platformPathType The platform path type, one of 
	the following ASAtom values: Mac:
	
	-  FSRefWithCFStringRef 
	-  FSSpec
	-  CFURLRef
	-  POSIXPath
	-  FSRef (pathName object must exist)
	-  Cstring (Windows/UNIX only)
	
	@param platformPath (Filled by the method) The new platform 
	path object. Always free this object with ASFileSysReleasePlatformPath 
	when done.
	@return 0 if the operation was successful, otherwise a nonzero error 
	code. 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32,	ASFileSysAcquirePlatformPath, (ASFileSys fileSys, ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath))

/**
	Releases the specified platform path object. Each call to 
	ASFileSysAcquirePlatformPath should have a corresponding 
	call to this method. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param platformPath The platform path object to release.
	@see ASFileSysAcquirePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, 	ASFileSysReleasePlatformPath, (ASFileSys fileSys, ASPlatformPath platformPath))

/**
	Gets a platform path object in the form of a C string for 
	Microsoft Windows or UNIX, if the ASPlatformPath object 
	was acquired with this type in the platformPathType parameter 
	of ASFileSysAcquirePlatformPath. 

	NOTE: Applications should use this a read-only pointer; 
	modifying the returned buffer can corrupt the ASPlatformPath. 
	Do not free the pointer. 

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to a C string of a platform-specific path. 
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(Cstring_Ptr,					ASPlatformPathGetCstringPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of an FSSpec for 
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the platformPathType parameter of ASFileSysAcquirePlatformPath. 
	

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to an FSSpec. 
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefPtr 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSSpec_Ptr,					ASPlatformPathGetFSSpecPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of an FSRef for 
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the platformPathType parameter of ASFileSysAcquirePlatformPath. 
	

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to an FSRef. 
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefWithCFStringRefRecPtr 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSRef_Ptr,					ASPlatformPathGetFSRefPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of an FSRef and 
	CFStringRef for Mac OS, if the ASPlatformPath object was 
	acquired with this type in the platformPathType parameter 
	of ASFileSysAcquirePlatformPath. 

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to a structure containing an FSRef and a CFStringRef. 
	
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefPtr 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSRefWithCFStringRefRec_Ptr,	ASPlatformPathGetFSRefWithCFStringRefRecPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of a CFURLRef for 
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the platformPathType parameter of ASFileSysAcquirePlatformPath. 
	

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to a structure containing a CFURLRef. 
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(CFURLRefRec_Ptr,				ASPlatformPathGetCFURLRefRecPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of a POSIX path 
	C string, if the ASPlatformPath object was acquired with 
	this type in the platformPathType parameter of ASFileSysAcquirePlatformPath. 
	

	NOTE: Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath 
	when finished with the object. 
	@param path The platform path.
	@return A pointer to a POSIX path (UTF-8 encoding) as a C string. 
	
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(POSIXPath_Ptr,				ASPlatformPathGetPOSIXPathPtr, (ASPlatformPath path))

/**
	Extracts the filename (including extension) from path as 
	an ASText object. Supercedes ASFileSysGetNameFromPath in 
	Acrobat 6.0. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName associated with the file 
	in question. 
	@param name (Filled by the method) The text object containing 
	the filename.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@exception fileErrGeneral 
	@see ASFileSysGetNameFromPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode,					ASFileSysGetNameFromPathAsASText, (ASFileSys fileSys, ASPathName pathName, ASText name))

/**
	Returns a user-friendly representation of a path as a text 
	object. Calls ASFileSysDisplayASTextFromPathProc. 

	NOTE: Supercedes ASFileSysDisplayStringFromPath in Acrobat 
	6.0. 
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The ASPathName in question. 
	@param displayText (Filled by method) The text object 
	containing the display representation of the path. 
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASFileSysDisplayStringFromPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode, 					ASFileSysDisplayASTextFromPath, (ASFileSys fileSys, ASPathName path, ASText displayText))


/**
	Flushes any buffered data to the specified stream. 
	@param stm The stream to flush.
	@return 0 if successful, otherwise non-zero. 
	@see ASStmClose 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTCount, ASStmFlush, (ASStm stm))

/**
	Determines whether there are any outstanding multibyte range 
	requests for a file. A document can have outstandings mreads 
	when it was opened in a browser, Acrobat requested some 
	byte ranges, and the byte ranges have not yet arrived. 
	@param fN The file in question.
	@return true if the file has outstanding mreads, false otherwise. 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASBool,						ASFileHasOutstandingMReads,(ASFile fN))

/**
	Checks if ASFileSetEOF can be done for this file with a specified new file size.
	@param file The file in question.
	@param newFileSize The proposed new file size.
	@see ASFileSetEOF
	@see ASFileGetEOF
	@see ASFileGetPos
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASBool, ASFileCanSetEOF, (ASFile file, ASInt32 newFileSize))

/**
	Returns the version of the HFT, if available. 
	@param hft The HFT whose version is obtained.
	@return The version number if the HFT is valid and the version is 
	available, HFT_ERROR_NO_VERSION otherwise. 
	@see HFTNewEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASVersion, HFTGetVersion, (HFT hft))


/**
	Extends HFTNew with version information in Acrobat 6. Creates 
	a new HFT by calling the specified HFT server's HFTServerProvideHFTProc. 
	
	@param hftServer The HFT server for the HFT being created. 
	The HFT server must have been created previously using HFTServerNew. 
	
	@param data The data to pass to the server, which includes: 
	-  The number of entries in the new HFT., which determines 
	the number of methods that the HFT can contain. Each method 
	occupies one entry.
	-  The HFT version.	
	@return The newly created HFT. 
	@see HFTNew 
	@see ASExtensionMgrGetHFT 
	@see HFTDestroy 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(HFT, HFTNewEx, (HFTServer hftServer, HFTData data))

/**
	Converts a filename, specified as an ASPathName, to a device-independent 
	pathname, returned as an ASText object. Calls ASFileSysDIPathFromPathExProc. 
	

	NOTE: Supercedes ASFileSysDIPathFromPath in Acrobat 6.0. 
	
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param path The ASPathName to convert. 
	@param relativeToThisPath (May be NULL) The pathname relative 
	to which the device-independent pathname is specified. If 
	NULL, the device-independent pathname will be an absolute, 
	not a relative, pathname. 
	@param diPathText (Filled by the method) The ASText object 
	to contain the device-independent path. Must be allocated 
	and freed by the client.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@see ASGetDefaultFileSys 
	@see ASFileSysPathFromDIPathEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode, ASFileSysDIPathFromPathEx, ( ASFileSys fileSys, ASPathName path, ASPathName relativeToThisPath, ASText diPathText))


/**
	Converts a device-independent pathname in an ASText object 
	to an ASPathName. This method can only be used for files 
	that already exist (that is, it cannot be used to create 
	a placeholder pathname for files that a plug-in intends 
	to create in the future). 

	It is the caller's responsibility to release the returned 
	ASPathName. 

	NOTE: Supercedes ASFileSysPathFromDIPath in Acrobat 6.0. 
	
	@param fileSys (May be NULL) The file system that the 
	ASPathName will be created within. Pass NULL to use the 
	default file system. 
	@param diPathText The device-independent pathname to convert, 
	as an ASText object. See Section 3.10 in the PDF Reference 
	for a description of the device-independent pathname format. 
	This pathname may not be understood on another platform 
	since drive specifiers may be prepended. In Windows, you 
	cannot specify a UNC pathname. You must have a file mounted 
	on the file server. For example, the following path is valid: 
	/f/dirname/file.pdf where f is \\server\\people. The following 
	does not work: /server/people/dirname/file.pdf 
	@param relativeToThisPath Pathname relative to which diPath 
	is interpreted. If NULL, diPath is interpreted as an absolute 
	pathname, not a relative pathname.
	@return ASPathName corresponding to the parameter values supplied. 
	Returns NULL if diPath cannot be converted to an ASPathName, 
	for example if the specified file does not already exist. 
	
	@exception genErrNoMemory 
	@see ASFileSysDIPathFromPathEx 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASPathName, ASFileSysPathFromDIPathEx, (ASFileSys fileSys, ASConstText diPathText, ASPathName relativeToThisPath))


/**
	Gets the temporary file system implementation for 
	a platform. 
	@return The platform's default file system. 
	@see ASSetTempFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASFileSys, ASGetTempFileSys, (void)) /* JGS */

/**
	Sets the temporary file system implementation for 
	a platform. 
	@return none 
	@see ASGetDefaultFileSys 
	@see ASGetTempFileSys 
	@see ASGetRamFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASSetTempFileSys, (ASFileSys fileSys)) /* JGS */

/**
	Gets the in-memory file system implementation for 
	a platform. 
	@return The platform's in-memory file system. 
	@see ASSetDefaultFileSys 
	@see ASSetTempFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASFileSys, ASGetRamFileSys, (void)) /* JGS */

/**
	Converts an ASFixed to a float
	
	@param inASFixed IN? The ASFixed value to convert
	@return The float representation of the ASFixed 
	
*/
NPROC(float, ASFixedToFloat, (ASFixed inASFixed))

/**
	Converts an float to a ASFixed
	
	@param inFloat IN? The float value to convert
	@return The ASFixed representation of the float 
	
*/
NPROC(ASFixed, FloatToASFixed, (double inFloat))

/**
	Attempts to open a file in the specified file system, in 
	the specified read/ write/ create mode. If the file is already 
	open, the existing file handle is returned. The caller retains 
	ownership of pathName.
	
	This call can open files over 2GB in length and should be used
	instead of ASFileSysOpenFile whenver possible.

	In Mac OS, when this method creates a file, the file's creator 
	is set to 'CARO' and its type is set to 'PDF ' (with a 
	space after PDF). 
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	pathname was obtained. Pass NULL to use the default file 
	system. 
	@param pathName IN/OUT? The pathname of the file to open. 
	@param mode IN/OUT? An OR of the ASFile Open Modes. 
	@param fP IN/OUT? (Filled by the method) The ASFile that was opened. 

	@return 0 if the operation was successful, otherwise returns a nonzero 
	error code. The error is platform- and file-system specific.\n\n
	Windows:\n
	Returns fileErrWrPerm if trying to open a read-only file with write permissions. 
	ErrSysXtnMgr (GetLastError()) - Platform-specific (any error condition 
	that CreateFile may use).
	Returns fileErrGeneral if the developer passed in an invalid ASPathName.\n\n
	Macintosh:\n
	Returns fileErrFNF if trying to open file for reading that does not exist. 
	ErrSysMDSystem (iErr) - Platform-specific error  (any error that FSpCreate, 
	 FSpSetFInfo, FSpOpenRF, FSpOpenDF, or SetFPos may use). Returns fileErrGeneral if 
	the developer passed in an invalid ASPathName. 
	@exception genErrNoError 
	@see ASFileClose 
	@see ASFileReopen 
	@see ASGetDefaultFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASErrorCode, ASFileSysOpenFile64, (ASFileSys fileSys, ASPathName pathName, ASFileMode mode, ASFile *fP))

/**
	Returns the maximum file position that can be processed by this file
	system. This is not the maximum size file that can be created or the
	amount of room left in the file system, but the maximum file position
	that can be handled by the arithmetic in the file system implementation.
	This will typically be (2 ^ 31) - 1 or (2 ^ 63) - 1.
	@return Maximum file position that can be processed.
	@param fileSys IN/OUT? (May be NULL) The file system from which 
	pathname was obtained. Pass NULL to use the default file 
	system. 
	@see ASFileSysOpenFile64 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileSysGetFilePosLimit, (ASFileSys fileSys))

/**
	Seeks to the specified position in a file. This is the position 
	at which the next read or write will begin. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT? The file in which to seek. 
	@param pos IN/OUT? The position to seek. 
	@exception fileErrIO 
	@see ASFileGetPos64 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(void, ASFileSetPos64, ( ASFile aFile, ASFilePos64 pos ))

/**
	Gets the current seek position in a file. This is the position 
	at which the next read or write will begin. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT? The file in which to get the seek position. 
	
	@return The current seek position. 
	@exception fileErrIO 
	@see ASFileSetPos64
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileGetPos64, ( ASFile aFile ))

/**
	Changes the size of a file. The new size may by larger or 
	smaller than the original size. This method may raise file 
	system/ platform specific exceptions. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT? The file whose size is changed. 
	@param newFileSize IN/OUT? The new size of file. 
	@exception fileErrIO 
	@see ASFileGetEOF64 
	@see ASFileGetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(void, ASFileSetEOF64, ( ASFile aFile, ASFilePos64 newFileSize ))

/**
	Gets the current size of a file. 
	@param aFile IN/OUT? The ASFile whose size is obtained. This
	call will work with files over 2 GB in length.
	@return The size of the file. 
	@exception fileErrIO 
	@see ASFileSetEOF64 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileGetEOF64, ( ASFile aFile ))

/**
	Sets the temporary file system implementation for 
	a platform. 
	@returns none 
	@see ASGetRamFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
*/
NPROC(void, ASRamFileSysSetLimitKB, (ASInt32 limit))

/**
	ASFileSysGetNameFromPathForDisplay writes into nameForDisplay
	the representation of that item as it would be shown in Windows Explorer
	or the Mac Finder. For example, it will provide the localized string for
	"My Documents" even though on disk, "My Documents" is always in English.
	It will also strip the extension if that is what Windows Explorer or the
	Mac Finder would do for that file.
	@param fileSys (May be NULL) The file system from which 
	pathName was obtained. Pass NULL to use the default file 
	system. 
	@param pathName The ASPathName associated with the file 
	in question. 
	@param nameForDisplay (Filled by the method) The text object containing 
	the name used for display.
	@return 0 if the operation was successful, otherwise returns a nonzero 
	platform-dependent error code. 
	@exception fileErrGeneral 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
*/
NPROC(ASErrorCode, ASFileSysGetNameFromPathForDisplay, (ASFileSys fileSys, ASPathName pathName, ASText nameForDisplay))


