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

 CorProcs.h

 - Catalog of the "core" expored functions; this table is handed off
   to the plug-in at initialization time,

*********************************************************************/


/**
	Raises an exception. Plug-ins can raise any exception defined 
	in the AcroErr.h header file using the ErrBuildCode macro, 
	or can define their own exceptions using ASRegisterErrorString. 
	See Errors for a list of predefined exceptions. 

	If the code that calls ASRaise gets control as a result 
	of a non-Acrobat event (such as a drag and drop event on 
	some platforms), this method fails since there is no Acrobat 
	viewer code in the stack to handle the exception. 
	@param error Error code for the exception to raise. Error 
	codes have three parts: severity, system, and error number. 
	Use ErrBuildCode to build an error code for an existing 
	error.
	@see ASGetErrorString 
	@see ASRegisterErrorString 
	@see RERAISE 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASRaise, (ASErrorCode error))
#if !ACROBAT_LIBRARY

/**
	Pushes an exception frame buffer and a frame-restoration 
	callback onto the stack. The restoreFunc should be a function 
	matching the following prototype: 

	NOTE: You will probably never call ASPushExceptionFrame 
	directly; use the DURING macro instead. 
	@param asEnviron IN/OUT? Represents a stack environment that is 
	restored if an exception occurs. On Windows and Macintosh, 
	this is a jmp_buf; A jmp_buf is an array of integers. 
	@param restoreFunc IN/OUT? Should be a function matching the following 
	prototype: ACCB1 void ACCB2 RestorePlugInFrame( void* asEnviron) 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASPushExceptionFrame, (void *asEnviron, ACRestoreEnvironProc restoreFunc), ACPushExceptionFrame)

/**
	Pops an exception frame off the stack. 

	NOTE: You will probably never call ASPopExceptionFrame directly; 
	it is called for you as appropriate from within the HANDLER, 
	E_RETURN and E_RTRN_VOID macros. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASPopExceptionFrame, (void), ACPopExceptionFrame)

/**
	Gets the error code for the exception most recently raised. 
	See Errors for a list of predefined exceptions. 
	@return Exception error code. 
	@see ASRaise 
	@see ASGetErrorString 
	@see ASRegisterErrorString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASErrorCode, ASGetExceptionErrorCode, (void), ACGetExceptionErrorCode)
#endif


/**
	Gets the ASAtom for the specified string. You can also use 
	this method to create an ASAtom, since it creates one for 
	the string if one does not already exist. 

	If an ASAtom already exists for str, the existing ASAtom 
	is returned. Thus ASAtoms may be compared for equality of 
	the underlying string. 

	Because ASAtoms cannot be deleted, they are useful for strings 
	that are used many times in an Acrobat viewer session, but 
	are not advisable for strings that have a short lifetime. 
	For the same reason, it is not a good idea to create large 
	numbers of ASAtoms. 
	@param nameStr The string for which an ASAtom is created. 
	@return The ASAtom corresponding to str. 
	@see ASAtomExistsForString 
	@see ASAtomGetCount (Only available with PDF Library SDK) 
	@see ASAtomGetString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASAtom, ASAtomFromString, (const char *nameStr))

/**
	Tests whether an ASAtom exists for the specified string. 
	
	@param nameStr The string to test. 
	@param atom (Filled by the method, may be NULL) If the ASAtom corresponding 
	to nameStr already exists, it is returned in atom. Pass 
	NULL to simply check whether the ASAtom already exists.
	@return true if an ASAtom already exists for str, false otherwise. 
	
	@see ASAtomFromString 
	@see ASAtomGetCount (Only available with PDF Library SDK) 
	@see ASAtomGetString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASBool, ASAtomExistsForString, (const char *nameStr, ASAtom *atom))

/**
	Gets the string associated with the specified ASAtom. 
	@param atm The ASAtom whose string is obtained. 
	@return The string corresponding to atom. Returns an empty string 
	if atom == ASAtomNull or NULL if the atom has not been defined. 
	
	@see ASAtomExistsForString 
	@see ASAtomFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(const char *, ASAtomGetString, (ASAtom atm))


/**
	Creates a callback that allows the Acrobat viewer to call 
	a function in a plug-in. All plug-in functions that are 
	called by the Acrobat viewer must be converted to callbacks 
	before being passed to the viewer. 

	Whenever possible, plug-ins should not call ASCallbackCreate 
	directly, but should use the macros ASCallbackCreateProto, 
	ASCallbackCreateNotification, and ASCallbackCreateReplacement. 
	These macros (which eventually call ASCallbackCreate) have 
	two advantages: 
	<ul>
	<li> They allow compilers to perform type checking, eliminating 
	one extremely common source of plug-in bugs. 
	<li> They handle extensionID automatically. 
	</ul>
	NOTE: If you call ASCallbackCreate directly, you are actually 
	invoking the ASCallbackCreate macro not this HFT routine. 
	The ASCallbackCreate macro takes only one parameter, the 
	proc, and passes that information into this underlying HFT 
	routine as the second argument. The first argument is always 
	set to gExtensionID, which should be the extension ID of 
	your plug-in. 

	Plug-ins must use ASCallbackCreate directly, for example, 
	when calling a Macintosh toolbox routine that expects a 
	ProcPtr. 
	@param extensionID IN/OUT? The gExtensionID extension that calls 
	proc. 
	@param proc IN/OUT? The user-supplied procedure for which a callback 
	is created. 
	@return The newly-created callback. 
	@see ASCallbackDestroy 
	@see AVAppRegisterNotification 
	@see AVAppUnregisterNotification 
	@see ASCallbackDestroy 
	@see ASCallbackCreateReplacement 
	@see ASCallbackCreateProto 
	@see ACCB1 
	@see ACCB2 
	@see DEBUG 
	@see ASCallbackCreate 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(ASCallback, ASCallbackCreate, (ASExtension extensionID, void *proc))

/**
	Destroys a callback. 
	@param callback IN/OUT? The callback to destroy. 
	@see ASCallbackCreate 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(void, ASCallbackDestroy, (ASCallback callback))

/**
	Gets the specified version of the Host Function Table (HFT) 
	that has the specified name. If you want to get one of the 
	Acrobat viewer's built-in HFTs, use the predefined global 
	variables for the HFT Values instead of this method. 
	@param name The name of the HFT to obtain. 
	@param version The version number of the HFT to obtain.
	@return The specified HFT, or NULL if the HFT does not exist. 
	@see HFTReplaceEntry 
	@see HFTReplaceEntryEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(HFT, ASExtensionMgrGetHFT, (ASAtom name, ASVersion version), ASGetHFTByNameAndVersion)

/**
	Gets information about the Acrobat viewer application under 
	which the plug-in is running. Use this method if your plug-ins 
	functionality depends on which Acrobat viewer it is running 
	under. 

	The method can return a product name, or check whether the 
	current product allows editing. Do not rely on the product 
	name to determine whether the product can edit files, as 
	product names and feature sets may vary; use the CanEdit 
	selector to do this. 
	@param key The key determines whether the method tests 
	editability, or finds which product configuration is running. 
	Values are:
	<ul>
	<li> CanEdit - Checks whether editing is allowed 
	in the current environment (regardless of the product name). 
	<li> Product - Checks which Acrobat application is running. 
	</ul>	
	@return The return value's type depends on the request key. Cast 
	the return value to the type you are expecting, based on 
	the key you pass in: 
	<ul>
	<li> 
	<li> For the key CanEdit, returns an ASBool value, true if 
	the current application allows editing, false otherwise. 
	<li> For the key Product, returns a const char* value, one 
	of the following strings:\n\n
	'Reader' Adobe Reader\n
	'Exchange' Adobe Acrobat Standard\n
	'Exchange-Pro' Adobe Acrobat Professional\n
	'Acrobat PDF LIbrary' Adobe Acrobat PDF Library\n
	</ul>
	@exception If 
	@exception an unknown value is passed as key, the value 
	@exception UNDEFINED_CONFIGURATION_SELECTOR is returned (see CoreExpT.h). 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(void *, ASGetConfiguration, (ASAtom key))

/* Acrobat 4.0 additions */

/**
	Enumerates all ASExtensions, that is, valid plug-ins. 
	@param proc User-supplied callback to call for each plug-in. 
	Enumeration halts if proc returns false. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called. 
	@param onlyLivingExtensions If true, ASExtensions that 
	have been unloaded or otherwise deactivated are not enumerated. 
	If false, all ASExtensions are enumerated.
	@return If proc returned false, the last ASExtension that was enumerated. 
	NULL otherwise. 
	@see ASExtensionGetFileName 
	@see ASExtensionGetRegisteredName 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASExtension, ASEnumExtensions, (ASExtensionEnumProc proc, void *clientData,
									  ASBool onlyLivingExtensions))

/**
	Gets the filename of an ASExtension. 
	@param extension IN/OUT? The ASExtension whose filename is obtained. 
	
	@param buffer IN/OUT? (Filled by the method, may be NULL) Pointer 
	to a buffer for the filename. Pass NULL to have this method 
	return the length of the filename (excluding a terminating 
	NULL character). 
	@param bufSize IN/OUT? Number of bytes in buffer. Ignored if buffer 
	is NULL. 
	@return The number of characters written into buffer, excluding 
	the NULL character. 
	@see ASExtensionGetRegisteredName 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASTArraySize, ASExtensionGetFileName, (ASExtension extension, char *buffer, ASTArraySize bufSize))

/**
	Gets the registered name associated with a plug-in. 
	@param extension IN/OUT? The ASExtension whose name is obtained. 
	
	@return An ASAtom representing the plug-in name, or NULL if the 
	name could not be identified. 
	@see ASExtensionGetFileName 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASAtom, ASExtensionGetRegisteredName, (ASExtension extension))
