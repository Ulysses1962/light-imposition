/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PagePDEContentProcs.h

 - Header for for PDF page content server methods.

*********************************************************************/


/**
	Creates a PDEContent from the PDPage's contents and resources. 
	The PDEContent is cached, so that subsequent calls on the 
	same PDPage return the same PDEContent, even if the request 
	is from another PDFEdit client. The PDEContent remains in 
	the cache as long as someone has it acquired - until someone 
	not using the PDFEdit API changes the PDPage's contents, 
	such as the viewer rotating a page 90 degrees. 

	Do not call PDERelease on PDEContent you have acquired with 
	PDPageAcquirePDEContent; call PDPageReleasePDEContent to 
	release it. 
	@param pdPage The page whose content object is acquired. 
	
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@return A PDEContent representing the page's contents. 
	@see PDEContentCreateFromCosObj 
	@see PDPageReleasePDEContent 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/

NPROC(PDEContent, PDPageAcquirePDEContent, (
			IN PDPage pdPage, IN ASExtension self)
	)

/**
	Decrements a PDPage's PDEContent internal reference count. 
	

	The PDEContent is not automatically deleted when the reference 
	count becomes zero -  it remains in the cache until the cache 
	slot is needed for another PDPage. Thus, you do not need 
	to keep a PDEContent acquired for performance reasons. There 
	is a notification you can register for, that is sent when 
	a PDEContent is actually removed from the cache, thus enabling 
	the use of PDFEdit's tagging methods PDEAddTag, PDEGetTag, 
	PDERemoveTag on the PDEContent object. 
	@param pdPage The page whose content object's use count 
	is decremented. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@return Updated reference count. 
	@see PDPageAcquirePDEContent 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASInt32, PDPageReleasePDEContent,(
			IN PDPage pdPage, IN ASExtension self)
	)

/**
	Sets the page's PDEContent back into the PDPage's Cos object, 
	using the same compression filters with which the content 
	was previously encoded.
	In order to properly synchronize the page's contents after setting
	them with PDPageSetPDEContent, you must call PDPageNotifyContentsDidChange.
	If you do not call PDPageNotifyContentsDidChange, the page displayed will
	use the old page contents.
	@param pdPage The page whose PDEContent is set. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@return Returns true if PDEContent successfully set, false otherwise. 
	
	@notify PDPageContentsDidChangeEx 
	@see PDEContentToCosObj 
	@see PDPageAcquirePDEContent 
	@see PDPageGetPDEContentFlags
	@see PDPageNotifyContentsDidChange
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageSetPDEContent, (
			IN PDPage pdPage, IN ASExtension self)
	)

/**
	Indicates a page's PDEContent has changed. 

	Call this after you alter a PDPage's PDEContent but do not 
	call PDPageSetPDEContent, so others who have acquired the 
	PDEContent know it has changed. 
	@param pdPage The page whose content was changed. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@notify PagePDEContentDidChange 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(void, PDPagePDEContentWasChanged, (
			IN PDPage pdPage, IN ASExtension self)
	)

/**
	Registers for the PagePDEContentDidChange notification. 
	
	@param proc Callback for function to call when an acquired 
	PDPage's PDEContent has changed. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@notify PagePDEContentDidChange 
	@see PDPageRegisterForPDEContentNotCached 
	@see PDPageUnRegisterForPDEContentChanged 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(void, PDPageRegisterForPDEContentChanged, (
			IN PagePDEContentDidChangeNPROTO proc, IN ASExtension self)
	)

/**
	Un-registers for the PagePDEContentDidChange notification. 
	
	@param proc Callback for function to call when an acquired 
	PDPage's PDEContent has changed. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@notify PagePDEContentDidChange 
	@see PDPageRegisterForPDEContentChanged 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(void, PDPageUnRegisterForPDEContentChanged, (
			IN PagePDEContentDidChangeNPROTO proc, IN ASExtension self)
	)

/**
	Register for the PagePDEContentNotCached notification. 

	This notification is also sent when others change (or delete) 
	a PDPage's contents without using PDFEdit methods. For instance, 
	rotating or deleting a page in the viewer results in this 
	notification being sent. 

	PDFEdit registers for almost a half dozen different notifications 
	for the different ways Acrobat can alter page contents - you 
	may need only this notification. 
	@param proc Callback for function to call when an acquired 
	PDPage's PDEContent is no longer valid. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@notify PagePDEContentNotCached 
	@see PDPageRegisterForPDEContentChanged 
	@see PDPageUnRegisterForPDEContentNotCached 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(void, PDPageRegisterForPDEContentNotCached, (
			IN PagePDEContentNotCachedNPROTO proc, IN ASExtension self)
	)

/**
	Un-registers for the PagePDEContentNotCached notification. 
	
	@param proc IN/OUT? Callback for function to call when an acquired 
	PDPage's PDEContent is no longer valid. 
	@param self IN/OUT? Identifies the caller/ client. For plug-
	ins, this should be the gExtensionID extension. For the 
	Adobe PDF Library, if there is only one client of the PDFEdit 
	subsystem, clientID should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative clientID. 
	(A negative clientID is reserved for the implementation.) 
	
	@notify PagePDEContentNotCached 
	@see PDPageRegisterForPDEContentNotCached 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000

*/
NPROC(void, PDPageUnRegisterForPDEContentNotCached, (
			IN PagePDEContentNotCachedNPROTO proc, IN ASExtension self)
	)

/**
	Gets flags used by PDPageSetPDEContent. 
	@param pdPage The page whose content flags are obtained. 
	
	@param flags (Filled by the method) PDEContentToCosObjFlags 
	flags. 
	@return Returns true if flags obtained, false if page's contents 
	are not cached. 
	@see PDPageSetPDEContent 
	@see PDPageSetPDEContentFlags 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageGetPDEContentFlags, (
			IN PDPage pdPage, OUT ASUns32 *flags))

/**
	Sets flags used by PDPageSetPDEContent. The flags are not 
	instantiated until PDPageSetPDEContent is called. 
	@param pdPage The page whose content flags are set. 
	@param flags PDEContentToCosObjFlags flags. The following 
	flags are ignored, since content is always a page:
	- kPDEContentToForm 
	- kPDEContentToCharProc 
	@return Returns true if flags were set, false if page's contents 
	are not cached (and so nothing was done). 
	@see PDPageGetPDEContentFlags 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageSetPDEContentFlags, (
			IN PDPage pdPage, IN ASUns32 flags))

/**
	Gets filters used by PDPageSetPDEContent. 

	The caller is responsible for allocating the filter array 
	filters that receives the filters. filters can be NULL to 
	just obtain the number of filters. 
	@param pdPage The page whose content filters are obtained. 
	
	@param numFilters (Filled by the method) Number of filters 
	used by PDPageSetPDEContent. 
	@param filters (Filled by the method) Filters used by 
	PDPageSetPDEContent. If NULL, numFilters contains the number 
	of filters.
	@return Returns true if filters are obtained, false if page's contents 
	are not cached. 
	@see PDPageSetPDEContent 
	@see PDPageSetPDEContentFilters 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageGetPDEContentFilters, (
			IN PDPage pdPage, OUT ASInt32 *numFilters, OUT ASAtom **filters))

/**
	Sets filters used by PDPageSetPDEContent. The filters are 
	not instantiated until PDPageSetPDEContent is called. 
	@param pdPage The page whose content filters are set. 
	@param numFilters Number of filters used by PDPageSetPDEContent. 
	
	@param filters Array of filters to use by PDPageSetPDEContent. 
	
	@return Returns true if filters were set, false if page's contents 
	are not cached (and so nothing was done). 
	@see PDPageGetPDEContentFilters 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageSetPDEContentFilters, (
			IN PDPage pdPage, IN ASInt32 numFilters, IN ASAtom *filters))

/**
	Suspends destruction of PDEContent objects when a PagePDEContentDidChange 
	notification occurs. Only use this API if you are about 
	to call PDPageNotifyContentsDidChange and you do not want 
	PDFEdit to destroy all PDEContent objects associated with 
	that PDPage. This is used, for example, when AVAppSetPreference 
	is called. Make sure to call PDPageSuspendPDEContentChanged 
	on the PDPage object after you call PDPageNotifyContentsDidChange. 
	
	@param pdPage IN/OUT? The page whose content is changed. 
	@see PDPageResumePDEContentChanged 
	@since PI_PDMODEL_VERSION >= 0x00050000

*/
NPROC(void, PDPageSuspendPDEContentChanged, (
			IN PDPage pdPage))

/**
	Resumes destruction of PDEContent objects when a PDPageContentsDidChange 
	notification occurs. Only use this API if you called PDPageSuspendPDEContentChanged. 
	
	@param pdPage IN/OUT? The page whose content is changed. 
	@see PDPageSuspendPDEContentChanged 
	@since PI_PDMODEL_VERSION >= 0x00050000

*/
NPROC(void, PDPageResumePDEContentChanged, (
			IN PDPage pdPage)) 

/**
	Sets the page's PDEContent back into the PDPage's Cos object, 
	using the same compression filters with which the content 
	was previously encoded. This method calls PDPageNotifyContentsDidChangeEx. 
	

	This method differs from PDPageSetPDEContent in that it 
	returns no value, but does raise an exception if it is unable 
	to set the content. 
	@param pdPage The page whose PDEContent is set. 
	@param self Identifies the caller or client. For plug-ins, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, this should be zero. If there are multiple clients, 
	each should specify a nonzero, non-negative value. (A negative 
	value is reserved for the implementation.) 
	@notify PDPageContentsDidChangeEx 
	@see PDEContentToCosObj 
	@see PDPageAcquirePDEContent 
	@see PDPageGetPDEContentFlags 
	@see PDPageSetPDEContent 
	@since PI_PAGE_PDE_CONTENT_VERSION >= 0x00060000
*/
UNPROC(void, PDPageSetPDEContentCanRaise, (
			IN PDPage pdPage, IN ASExtension self)
	)
