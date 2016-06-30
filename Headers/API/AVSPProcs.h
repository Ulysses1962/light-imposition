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

 AVSPProcs.h

 - Catalog of SweetPea functions exported by AcroView.

*********************************************************************/

#if !EXTERNAL_AVSPPROCS_USER  /* External user of this header file, e.g. PISDK */

#if CAN_EDIT && !READER
#define XNPROC(returnType, name, params) NPROC(returnType, name, params)
#define XPROC(returnType, name, params) PROC(returnType, name, params)
#define XSPROC(returnType, name, params, stubProc) SPROC(returnType, name, params, stubProc)
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#define XPROC(returnType, name, params) NOPROC(name)
#define XSPROC(returnType, name, params, stubProc) NOPROC(name)
#endif /* CAN_EDIT */

#endif /* EXTERNAL_AVSPPROCS_USER */

#if HAS_ADM || PI_ACROVIEW_SWEETPEA_VERSION >= 0x00050000

/**
	Used to implement the Adobe Dialog Manager (ADM). Accesses 
	basic suite. For complete details, see Adobe Dialog Manager 
	Reference. 
	@return Pointer to the structure that contains the 'SweetPea' functions. 
	
	@see AVSweetPeaGetPluginRef 
	@see AVSweetPeaGetResourceAccess 
	@see AVSweetPeaIsADMAvailable 
	@see AVSweetPeaProcessADMEvent 
	@see AVSweetPeaSetResourceAccess 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(SPBasicSuite*, AVSweetPeaGetBasicSuiteP, (void))

/**
	Used to implement the Adobe Dialog Manager (ADM). This method 
	is used to obtain a reference to the ADM client itself (not 
	the client you are currently developing). For complete details, 
	see Adobe Dialog Manager Reference. 
	@return A reference to the ADM client itself. 
	@see AVSweetPeaGetBasicSuiteP 
	@see AVSweetPeaGetResourceAccess 
	@see AVSweetPeaIsADMAvailable 
	@see AVSweetPeaProcessADMEvent 
	@see AVSweetPeaSetResourceAccess 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(SPPluginRef, AVSweetPeaGetPluginRef, (void))

/**
	Used to implement the Adobe Dialog Manager (ADM). Call this 
	function to store away the current resource access, which 
	you will restore after your dialog is closed. For complete 
	details, see Adobe Dialog Manager Reference. 
	@param pluginRef The value returned from the AVSweetPeaGetPluginRef 
	method. 
	@param resourceAccess Platform-specific resource information.
	@return ADM error codes. 
	@see AVSweetPeaGetBasicSuiteP 
	@see AVSweetPeaGetPluginRef 
	@see AVSweetPeaIsADMAvailable 
	@see AVSweetPeaProcessADMEvent 
	@see AVSweetPeaSetResourceAccess 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(SPErr, AVSweetPeaGetResourceAccess, (SPPluginRef pluginRef, SPPlatformAccessRef *resourceAccess))

/**
	Used to implement the Adobe Dialog Manager (ADM). This function 
	tells ADM which resource file contains your dialog resources. 
	For complete details, see Adobe Dialog Manager Reference. 
	
	@param pluginRef The value returned from the AVSweetPeaGetPluginRef 
	method. 
	@param resourceAccess Platform-specific resource information.
	@return Error code. 
	@see AVSweetPeaGetBasicSuiteP 
	@see AVSweetPeaGetPluginRef 
	@see AVSweetPeaGetResourceAccess 
	@see AVSweetPeaIsADMAvailable 
	@see AVSweetPeaProcessADMEvent 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(SPErr, AVSweetPeaSetResourceAccess, (SPPluginRef pluginRef, SPPlatformAccessRef resourceAccess))

/**
	Used to implement the Adobe Dialog Manager (ADM). This method 
	is used do determine whether ADM is available. For complete 
	details, see Adobe Dialog Manager Reference. 
	@return true if ADM is available. false if not. 
	@see AVSweetPeaGetBasicSuiteP 
	@see AVSweetPeaGetPluginRef 
	@see AVSweetPeaGetResourceAccess 
	@see AVSweetPeaProcessADMEvent 
	@see AVSweetPeaSetResourceAccess 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(ASBool, AVSweetPeaIsADMAvailable, (void))

/**
	Used to implement the Adobe Dialog Manager (ADM). Call this 
	function to have a given ASWindowRef process events. This 
	is useful for modeless dialogs. For complete details, see 
	Adobe Dialog Manager Reference. 
	@param window A platform-dependent window handle.
	@see AVSweetPeaGetBasicSuiteP 
	@see AVSweetPeaGetPluginRef 
	@see AVSweetPeaGetResourceAccess 
	@see AVSweetPeaIsADMAvailable 
	@see AVSweetPeaSetResourceAccess 
	@since PI_ACROVIEW_VERSION >= 0x00050000
*/
NPROC(void, AVSweetPeaProcessADMEvent, (ASWindowRef window))
#else
NOPROC(AVSweetPeaGetBasicSuiteP)
NOPROC(AVSweetPeaGetPluginRef)
NOPROC(AVSweetPeaGetResourceAccess)
NOPROC(AVSweetPeaSetResourceAccess)
NOPROC(AVSweetPeaIsADMAvailable)
NOPROC(AVSweetPeaProcessADMEvent)
#endif /* HAS_ADM */

#undef XSPROC
#undef XNPROC
#undef XPROC
