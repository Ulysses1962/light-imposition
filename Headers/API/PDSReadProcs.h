/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDSReadProcs.h

 - Catalog of functions exported by PDSEdit.

*********************************************************************/

/*------------------------------------------------------------------------
	PDS Objects (see PDSExpT.h).
------------------------------------------------------------------------*/
/*
	PDSTreeRoot	- Root of a structural tree.
	PDSElement	- Member of the structural tree (and super-class of PDSTreeRoot)
	PDSAttrObj	- Attribute Object
	PDSMC		- Marked Content (cover for PDEContainer)
	PDSMCR		- Marked Content Refence
	PDSOBJR		- PDF Object Reference
	PDSRoleMap	- RoleMap
	PDSClassMap	- ClassMap
*/

/* 
 * Struct Tree Root methods.
 */


/**
	Gets the structure tree root for a document. 
	@param pdDoc The PDDoc whose root is obtained. 
	@param treeRoot (Filled by the method) The structure tree 
	root.
	@return Returns true if structure tree root found, false otherwise. 
	
	@see PDDocCreateStructTreeRoot 
	@see PDDocRemoveStructTreeRoot 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDDocGetStructTreeRoot,		(IN  PDDoc pdDoc,
											 OUT PDSTreeRoot *treeRoot))

/**
	Gets the number of kids of the structure tree root. 
	@param treeRoot IN/OUT? The structure tree root whose number of 
	kids is obtained. 
	@return The number of kids of the structure tree root. 
	@exception Various 
	@see PDSTreeRootGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSTreeRootGetNumKids,		(IN  PDSTreeRoot treeRoot))

/**
	Gets the kid at an array index in the specified structure 
	tree root. 
	@param treeRoot The structure tree root whose kid is obtained. 
	
	@param index Index of the kid to obtain. 
	@param kid (Filled by the method) Pointer to the kid at 
	index.
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSTreeRootGetNumKids 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSTreeRootGetKid,			(IN  PDSTreeRoot treeRoot,
											 IN  ASInt32 index,
											 OUT PDSElement *kid))

/**
	Gets the PDSRoleMap object for the specified structure tree 
	root. 
	@param treeRoot The structure tree root whose PDSRoleMap 
	is obtained. 
	@param roleMap (Filled by the method) Pointer to a location 
	in which to return the role map, if one exists. Set to CosNull 
	if there is no role map. If a NULL pointer is passed, no 
	retrieval will take place.
	@return true if there is a role map, false otherwise. 
	@exception Various 
	@see PDSTreeRootCreateRoleMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetRoleMap,		(IN  PDSTreeRoot treeRoot,
											 OUT PDSRoleMap *roleMap))

/**
	Gets the PDSClassMap object for the specified structure 
	tree root. 
	@param treeRoot The structure tree root whose PDSClassMap 
	is obtained. 
	@param classMap (Filled by the method) Pointer to a location 
	in which to return the class map, if one exists. Set to 
	CosNull if there is no class map. If a NULL pointer is passed, 
	no retrieval will take place.
	@return true if there is a class map, false otherwise. 
	@exception Various 
	@see PDSTreeRootCreateClassMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetClassMap,		(IN  PDSTreeRoot treeRoot,
											 OUT PDSClassMap *classMap))

/**
	Gets the element associated with the given ID, if any. 
	@param treeRoot The structure tree root in which to search 
	for id. 
	@param id Pointer to a buffer containing the ID to search 
	for. 
	@param numChars Number of characters in id. 
	@param element (Filled by the method) The element corresponding 
	to id. Undefined if no element has the specified id.
	@return true if an element for id found, or false with element undefined 
	if the tree root contains no IDTree value. 
	@exception Raises pdsErrWrongTypeParameter if id is NULL or numChars 
	is zero or less. 
	@exception Raises pdsErrWrongTypeEntry if the IDTree value in treeRoot 
	is not a dictionary. 
	@see PDSElementGetID 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetElementFromID,(IN  PDSTreeRoot treeRoot,
											 IN  const char *id,
											 IN  ASInt32 numChars,
											 OUT PDSElement *element))
/*
 * PDSElement methods.
 */

/**
	Gets the element's structural element type. The type corresponds 
	to the Subtype key in the structure element dictionary. 
	

	PDSElementGetType gets the value of the Subtype key - not 
	the Type key - in the structure element dictionary. All PDSElements 
	have a Type value of StructElem. 
	@param element The element whose structural element type 
	is obtained.
	@return The ASAtom representing element's type. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementSetType 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSElementGetType,			(IN  PDSElement element))

/**
	Gets the immediate ancestor element of the specified element 
	in the tree. 

	If the element's parent is another element, parent is set 
	to that parent and parentIsTreeRoot is set to false. If 
	the element's parent is the structure tree root, parent 
	is set to CosNull and parentIsTreeRoot is set to true. If 
	parentIsTreeRoot is NULL, it is not set. 
	@param element The element whose parent is obtained. 
	@param parent (Filled by the method) The element's parent. 
	
	@param parentIsTreeRoot (Filled by the method) The element's 
	parent is the structure tree root.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetKid 
	@see PDSElementGetStructTreeRoot 
	@see PDSMCGetInfo 
	@see PDSOBJGetParent 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSElementGetParent,		(IN  PDSElement element,
									         OUT PDSElement *parent,
											 OUT ASBool *parentIsTreeRoot))

/**
	Gets the title of the specified element, returning the number 
	of bytes in the title. 

	Can first be called with a NULL buffer to find the title 
	size, so that buffer can be appropriately sized as one greater 
	than the title's length. 

	NOTE: Due to implementation issues, make the buffer one 
	byte larger than the required size. 
	@param element IN/OUT? Element whose title is obtained. 
	@param buffer IN/OUT? (Filled by the method) A buffer into which 
	the title text is placed. May be NULL, in which case the 
	number of bytes in the title is returned. 
	@return Number of bytes in element's title, or zero if element has 
	no title. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementSetTitle 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetTitle,			(IN  PDSElement element,
											 OUT ASUns8 *buffer))

/**
	Gets the revision number of an element. 
	@param element IN/OUT? The element whose revision is obtained. 
	
	@return Revision number of element. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementIncrementRevision 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetRevision,		(IN  PDSElement element))

/**
	Gets the number of attribute objects directly attached to 
	the specified element. 
	@param element IN/OUT? The element whose number of attributes is 
	obtained. 
	@return Number of attribute objects directly attached to element. 
	
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumAttrObjs,	(IN  PDSElement element))

/**
	Gets the attribute object at a specified array index in 
	the specified element. 

	If there is only one attribute object (that is, there is 
	no array of attributes), and index is zero, that attribute 
	object is obtained. 
	@param element IN/OUT? The element whose attribute is obtained. 
	
	@param index IN/OUT? Index of the attribute object to obtain. 
	@param attrObj IN/OUT? (Filled by the method) Attribute object 
	at index. 
	@return Revision number of element at time of last association. 
	
	@exception pdsErrRequiredMissing 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementAddAttrObj 
	@see PDSElementGetNumAttrObjs 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetAttrObj,		(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT PDSAttrObj *attrObj))

/**
	Gets the number of classes to which the specified element 
	belongs. 
	@param element The element whose number of classes is 
	obtained.
	@return Number of classes to which element belongs. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetClass 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumClasses,	(IN  PDSElement element))

/**
	Gets the class name at an array index in the specified element. 
	

	If there is only one attribute object (that is, there is 
	no array), and index is zero, that class name is obtained. 
	
	@param element The element whose class is obtained. 
	@param index Index of the class to obtain. 
	@param classAtom (Filled by the method) The ASAtom describing 
	the class.
	@return Revision number of element at time of last association. 
	
	@exception pdsErrRequiredMissing 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetClass,			(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT ASAtom *classAtom))

/**
	Gets the alternate text associated with an element. 

	Can first be called with a NULL buffer to find the size, 
	so that buffer can then be appropriately sized. 

	NOTE: The Alt text can be legally defined as an empty string. 
	To differentiate between an Alt text string of zero length 
	and no Alt text being defined, call PDSElementHasAlt first. 
	

	NOTE: Due to implementation issues, make the buffer one 
	byte larger than the required size. Code will not null-terminate 
	the string correctly in the case of Unicode strings. 
	@param element The element whose alternate text is obtained. 
	
	@param buffer (Filled by the method) A buffer into which 
	the alternate text is placed. May be NULL, if the method 
	is called only to find the length of the element's alternate 
	text. If not NULL, buffer contains the element's actual 
	text. The string is null-terminated (but not correctly for 
	Unicode). This is not a C-style string, so normal string 
	handling functions may not work; the buffer may contain 
	a Unicode string.
	@return Number of bytes in element's alternate text. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementSetAlt 
	@see PDSElementHasAlt 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetAlt,			(IN  PDSElement element,
											 IN  ASUns8 *buffer))

/**
	Gets the number of kids of the specified element. 
	@param element IN/OUT? Element whose number of kids is obtained. 
	
	@return Number of direct kids of element. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumKids,		(IN  PDSElement element))

/**
	Gets the kid at an array index in the specified element. 
	

	A PDF structural element - unlike the structure tree root - can 
	have several different kinds of children: marked content, 
	another element, or an entire PDF object. The parameter 
	in which the kid is placed depends on the type of kid. If 
	the kid is a structural element or an object reference, 
	PDSElementGetKid places the result in cosObjKid; if the 
	kid is page content, it is placed in pointerKid. 

	Any or all of cosObjKid, pointerKid, and cosPage can be 
	NULL to get the kid's type without setting that parameter. 
	

	NOTE: When the kid is an MC, it is actually a pointer of 
	the type PDEContainer. As with all PDFEdit objects, you 
	must be careful to manage the reference count of the object 
	by calling PDEAcquire and PDERelease. PDSElementGetKid does 
	not call PDEAcquire for you. 

	NOTE: This method cannot access marked content inside a 
	Form XObject. 
	@param element Element whose specified kid is found. 
	@param index Index of the kid to obtain. 
	@param cosObjKid (Filled by the method) The CosObj of 
	the specified kid - if that kid is a PDSElement or an OBJR. 
	If cosObjKid is NULL, it is not filled in, but the type 
	of the kid is returned regardless. NOTE: This CosObj can 
	be treated as a PDSElement or a PDSObjR. Use the return 
	type to decide which to use. 
	@param pointerKid (Filled by the method) Pointer to the 
	kid at index - if that kid is an MC. If pointerKid is NULL, 
	it is not filled in, but the type of the kid is returned 
	regardless. 
	@param cosPage (Filled by the method) Pointer to the CosObj 
	of the page containing the kid. If cosPage is NULL, it is 
	not filled in, but the type of the kid is returned regardless.
	@return The ASAtom representing the kid's Type value: StructElem, 
	MC or OBJR. MCR is never returned. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSElementGetFirstPage 
	@see PDSElementGetKidEx 
	@see PDSElementGetKidWithMCInfo 
	@see PDSElementGetNumKids 
	@see PDSElementInsertKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSElementGetKid,			(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT CosObj *cosObjKid,
											 OUT void **pointerKid,
											 OUT CosObj *cosPage))

/**
	Gets the Cos object for the page of the first kid of the 
	element. 

	NOTE: The order in which the returned page is first is the 
	order of kids, not the order of pages. That is, the first 
	descendant with page content determines which page is returned. 
	
	@param pdsElement IN/OUT? Element whose kid's first page is found. 
	
	@param firstKidType IN/OUT? (Filled by the method) Pointer to an 
	ASAtom for the name that appears as the Type entry of the 
	actual first kid of element. Possible values are the values 
	that PDSElementGetKid can return. Pass NULL to inhibit setting 
	firstKidType. 
	@param firstCosObjKidOnAPage IN/OUT? (Filled by the method) The 
	kid whose content determined that the page returned was 
	the first page with content - if that kid is a CosObj. Pass 
	NULL to inhibit setting firstCosObjKidOnAPage. 
	@param firstMCKidOnAPage IN/OUT? (Filled by the method) The kid 
	whose content determined that the page returned was the 
	first page with content - if that kid is marked content 
	that is not a CosObj. Pass NULL to inhibit setting firstMCKidOnAPage. 
	
	@return The CosObj of the page found, CosObjNull if the element 
	has no page content. 
	@exception Various 
	@see PDSElementGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (CosObj,	PDSElementGetFirstPage,		(IN  PDSElement pdsElement, 
											 OUT ASAtom *firstKidType, 
											 OUT CosObj *firstCosObjKidOnAPage, 
											 OUT PDEContainer *firstMCKidOnAPage))

/**
	Gets the ID of an element or CosObjNull if there is no ID 
	set. 
	@param pdsElement Element whose ID is obtained. 
	@param idBuf (Filled by the method) Pointer to the buffer 
	containing the element's ID.
	@return The number of bytes in the ID, or zero if the element has 
	no ID. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSElementClearID 
	@see PDSElementSetID 
	@see PDSTreeRootGetElementFromID 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32, PDSElementGetID,			(IN  PDSElement pdsElement,
											 OUT ASUns8 *idBuf))

/**
	Gets the structure tree root of the document containing 
	element. 
	@param element Element whose title is obtained. 
	@param treeRoot (Filled by the method) The structure tree 
	root.
	@return true if the document has a structure tree root, false otherwise. 
	If there is a structure tree root, sets treeRoot to be the 
	structure tree root. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSTreeRootGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSElementGetStructTreeRoot,(IN  PDSElement element,
											 OUT PDSTreeRoot *treeRoot))

/*
 * Atrribute Object method
*/

/**
	Gets the value of the key (Owner) in the specified attribute 
	object. 
	@param element The attribute object whose owner is obtained.
	@return The ASAtom for the owner's name. 
	@exception Various 
	@see PDSAttrObjCreate 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSAttrObjGetOwner,			(IN PDSAttrObj element))
/*
 * Marked Content Container method
 */


/**
	Gets the parent element of the specified marked content. 
	
	@param containingObj The CosObj containing the MC whose 
	parent is obtained. For marked content on a page, this is 
	the Cos object representing the page. For marked content 
	elsewhere, this is the stream in which the marked content 
	resides. 
	@param mc The marked content whose parent is obtained. 
	
	@param parent (Filled by the method) Parent element of 
	containingObj.
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	Will also raise the error if the 
	@exception PDSMC passed to it is not in the structure tree. 
	@see PDSElementGetParent 
	@see PDSElementInsertMCAsKid 
	@see PDSElementRemoveKidMC 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSMCGetParent,				(IN  CosObj containingObj, 
											 IN  PDSMC mc,
											 OUT PDSElement *parent))
/*
 * PDSOBJR methods
 */

/**
	Gets the parent element of the specified PDF object. 
	@param obj IN/OUT? PDF object whose parent element is obtained. 
	Must be referred to via an OBJR from some element (that 
	is, it has a struct parent key), otherwise undefined. 
	@param parent IN/OUT? (Filled by the method) Parent element of 
	obj. 
	@exception Various 
	@see PDSElementGetParent 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSOBJGetParent,			(IN  CosObj obj, 
											 OUT PDSElement *parent))

/*
 * RoleMap methods.
 */


/**
	Gets the type, if any, directly mapped in the specified 
	PDSRoleMap for the given element type. 
	@param roleMap The PDSRoleMap. 
	@param type The ASAtom for an element type whose mapping 
	is found.
	@return The ASAtom for the equivalent type specified in roleMap, 
	or ASAtomNull if type has no mapping in roleMap. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSRoleMapDoesMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSRoleMapGetDirectMap,		(IN  PDSRoleMap roleMap, 
											 IN  ASAtom type))

/**
	Determines whether the specified PDSRoleMap provides any 
	mapping path for two given element types. 
	@param roleMap IN/OUT? The PDSRoleMap. 
	@param src IN/OUT? The ASAtom for an element type whose mapping 
	is tested. 
	@param dst IN/OUT? The ASAtom for an element type. NOTE: This may 
	be a standard element type. 
	@return true if an mapping path was found, false otherwise. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSRoleMapMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSRoleMapDoesMap,			(IN  PDSRoleMap roleMap, 
											 IN  ASAtom src, 
											 IN  ASAtom dst))
/*
 * ClassMap methods.
 */


/**
	Gets the number of attribute objects associated with a class 
	name. 
	@param classMap IN/OUT? The PDSClassMap. 
	@param classAtom IN/OUT? The ASAtom of a class name for which the 
	number of associated attribute objects is found. 
	@return Number of attribute objects associated with the class in 
	classAtom. 
	@exception Various 
	@see PDSClassMapGetAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSClassMapGetNumAttrObjs,	(IN  PDSClassMap classMap, 
											 IN  ASAtom classAtom))

/**
	Gets the attribute object associated with the specified 
	class name at an index in the class. 

	If there is only one object and index is zero, that object 
	is retrieved. 
	@param classMap The PDSClassMap. 
	@param classAtom The ASAtom of a class name for which 
	an associated attribute objects is found. 
	@param index Index of the desired attribute object in 
	the class. 
	@param attrObj (Filled by the method) Attribute object 
	at index. Set to CosNull if there is no attribute object 
	at the specified location.
	@exception Various 
	@see PDSClassMapAddAttrObj 
	@see PDSClassMapGetNumAttrObjs 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSClassMapGetAttrObj,		(IN  PDSClassMap classMap, 
											 IN  ASAtom classAtom,
											 IN  ASInt32 index, 
											 OUT PDSAttrObj *attrObj))
/*
 * New in Acrobat 5
 */


/**
	Functions identically to PDSElementGetKid, but for children 
	that are marked contents can return the mcid as well as 
	or instead of the actual object. 

	NOTE: This method cannot access marked content inside a 
	Form XObject. 
	@param element The PDSElement containing the kid that 
	is retrieved. 
	@param index The index of the kid. 
	@param cosObjKid (Filled in by method) The kid being accessed 
	(depending on the kid's type) or NULL. 
	@param mcid (Filled in by method) The kid's mcid or NULL. 
	
	@param pointerKid (Filled in by method) Pointer to the 
	kid or NULL. 
	@param cosPage (Filled in by method) The CosObj of the 
	page containing the kid or NULL.
	@return An ASAtom representing the Type value of the kid. See above. 
	
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSElementGetKid 
	@see PDSElementGetKidWithMCInfo 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASAtom,	PDSElementGetKidEx,			(IN  PDSElement	element,
											 IN  ASInt32	index,
											 OUT CosObj*	cosObjKid,
											 OUT ASInt32*	mcid,
											 OUT void**		pointerKid,
											 OUT CosObj*	cosPage))
/**
	Gets the actual text associated with the specified PDSElement. 
	Returns the number of bytes in the text or 0 if the element 
	has no actual text or has an empty string. 

	To check for the existence of alternate text, check for 
	a non-zero return value. To get the needed size of buffer, 
	call this method with a NULL buffer. 

	NOTE: Due to implementation issues, make the buffer one 
	byte larger than the required size. Code will not null-terminate 
	the string correctly in the case of Unicode strings. 
	@param element The structural element whose actual text 
	is sought. 
	@param buffer If not NULL, buffer contains the element's 
	actual text. The string is null-terminated (but not correctly 
	for Unicode). This is not a C-style string, so normal string 
	handling functions may not work; the buffer may contain 
	a Unicode string.
	@return An ASInt32 representing the number of bytes in the text 
	or 0 if the element has no actual text. 
	@see PDSElementSetActualText 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASInt32,	PDSElementGetActualText,	(IN  PDSElement element,
											 IN  ASUns8 *buffer))

/**
	Gets the language associated with the specified PDSElement. 
	

	Returns the number of bytes in the language string or 0 
	if the element has no language or has an empty string. 

	To check for the existence of expansion text, call PDSElementHasLanguage. 
	To get the needed buffer size, call this method with a NULL 
	buffer. 

	NOTE: Due to implementation issues, make the buffer one 
	byte larger than the required size. Code will not null-terminate 
	the string correctly in the case of Unicode strings. 
	@param element The structural element whose expansion 
	text is sought. 
	@param buffer (Filled by the method) A buffer containing 
	the element's expansion text or NULL. See PDSElementSetLanguage 
	for format and languages. If not NULL, buffer contains the 
	element's expansion text. The string is null-terminated 
	(but not correctly for Unicode). This is not a C-style string, 
	so normal string handling functions may not work; the buffer 
	may contain a Unicode string.
	@return An ASInt32 representing the number of bytes in the language 
	string. 
	@see PDSElementSetLanguage 
	@see PDSElementHasLanguage 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASInt32,	PDSElementGetLanguage,	(IN  PDSElement element,
										 IN  ASUns8 *buffer))

/**
	Tests whether Alt text is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return true if text exists (including the empty string); false 
	otherwise. 
	@see PDSElementGetAlt 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasAlt,			(IN PDSElement element))

/**
	Tests whether ActualText is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return true if text exists (including the empty string); false 
	otherwise. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasActualText,	(IN PDSElement element))

/**
	Tests whether a language string is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return true if text exists (including the empty string); false 
	otherwise. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasLanguage,	(IN PDSElement element))

/**
	Functions identically to PDSElementGetKidEx, but returns 
	additional information about marked content kids that are 
	in streams other than the page content streams. 
	@param element The PDSElement containing the kid that 
	is retrieved. 
	@param index The index of the kid. 
	@param cosObjKid (Filled in by method) The kid being accessed 
	(depending on the kid's type) or NULL. 
	@param mcidInfo (Filled in by method) The kid's information 
	object or NULL. 
	@param pointerKid (Filled in by method) Pointer to the 
	kid or NULL. 
	@param cosPage (Filled in by method) The CosObj of the 
	page containing the kid or NULL.
	@return An ASAtom representing the Type value of the kid. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSElementGetKid 
	@see PDSElementGetKidEx 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (ASAtom, PDSElementGetKidWithMCInfo, (PDSElement element,
                                            ASInt32 index,
                                            CosObj* cosObjKid,
                                            PDSMCInfoP mcidInfo,
                                            void** pointerKid,
                                            CosObj* cosPage))
/**
	Gets information about how the specified marked content 
	is contained in its parent. 

	NOTE: This method cannot access marked content inside a 
	Form XObject. 
	@param containingObj The CosObj containing the MC whose 
	information is obtained. For marked content on a page, this 
	is the Cos object representing the page. For marked content 
	elsewhere, this is the stream in which the marked content 
	resides. 
	@param mc The marked content whose information is obtained. 
	
	@param info (Filled by the method) A pointer to a structure 
	that the method fills with information about the containingObj.
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	Will also raise the error if the 
	@exception PDSMC passed to it is not in the structure tree. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (void, PDSMCGetInfo, (CosObj containingObj,
                            PDSMC mc,
                            PDSMCInfoP info))

/**
	Gets the parent element of the specified marked content, 
	referred to by its containing object and marked-content 
	identifier. 
	@param mcid The identifier (MCID) of the marked content 
	whose parent is obtained. 
	@param containingObj The CosObj containing the marked 
	content whose parent is obtained. For marked content on 
	a page, this is the Cos object representing the page. For 
	marked content elsewhere, this is the stream in which the 
	marked content resides. 
	@param parent (Filled by the method) Parent element of 
	containingObj.
	@return true if the parent is successfully obtained, false otherwise. 
	
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	Will also raise the error if the 
	@exception PDSMC passed to it is not in the structure tree. 
	@see PDSMCGetParent 
	@see PDSElementGetParent 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (ASBool, PDSMCIDGetParent, (ASInt32 mcid,
                                  CosObj containingObj,
                                  PDSElement* parent))
/**
	Gets the PDE container object for the specified marked content. 
	
	@param mc The marked content whose container is obtained.
	@return The PDE container object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (PDEContainer, PDSMCGetPDEContainer, (PDSMC mc))

/**
	Gets the Cos object corresponding to the specified element 
	object. This method does not copy the object, but is instead 
	the logical equivalent of a type cast. 
	@param element The element object whose Cos object is 
	obtained.
	@return The dictionary Cos object for the element object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (CosObj, PDSElementGetCosObj, (PDSElement element))
/**
	Gets the Cos object corresponding to the specified attribute 
	object. This method does not copy the object, but is instead 
	the logical equivalent of a type cast. 
	@param attrObj The attribute object whose Cos object is 
	obtained.
	@return The dictionary Cos object for the attribute object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (CosObj, PDSAttrObjGetCosObj, (PDSAttrObj attrObj))

/**
	Returns true if the document declares that it has
	structure elements that conform to the UserProperties
	attributes/class conventions.
	This is based on (1) the presence of StructTreeRoot,
	and (2) a value of "true" for the UserProperties
	key in the document's MarkInfo dictionary.

	@param doc The PDDoc to be examined
	@return An ASBool indicating whether the document
		declares that it has structure elements with
		UserProperties attributes / classes
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDDocHasUserProperties, (PDDoc doc))

/**
	Enumerates the elements in the document's structure
	tree that have UserProperties attributes or classes,
	calling the supplied enumeration procedure for each
	such element found. The procedure returns true to
	continue enumeration, or false to halt enumeration.

	@param doc The PDDoc whose structure elements are to
		be enumerated
	@param proc The procedure to call for each PDSElement
		found to have UserProperties
	@param clientData client-supplied data to be passed
		to the client callback
	@return true if the enumeration completes, false if
		the enumeration callback returns false.
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDDocEnumPDSElementsWithUserProperties, (PDDoc doc,
					EnumElementsWithUserPropertiesProc proc,
					void *clientData))

/**
	Returns true if the PDSElement has attribute objects
	or class objects with owner of UserProperties.

	@param elem The PDSElement to examine
	@return ASBool indicating that some attribute objects
		or class objects have owner of UserProperties
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementHasUserProperties, (PDSElement elem))

/**
	Enumerates the PDSElement's user properties by traversing
	the list of attribute objects and class objects, calling
	the caller-supplied procedure for each entry in the
	properties array. The enumeration proc receives the
	property information as a pair of ASTexts, for the
	property name, and the property value. The enumeration
	continues as long as the callback returns true, and halts
	when the proc returns false or all properties have been
	enumerated.

	@param elem The PDSElement whose user properties will be
		enumerated
	@param proc The callback that is called for each user
		property item
	@param clientData Client-supplied data to be passed to the
		client callback
	@param includeHidden Boolean indicating whether the client
		wants to be given property items that are marked as
		hidden.
	@return true if the enumeration completes, false if the
		enumeration callback returns false
	@see PDSElementEnumUserPropertiesAsCosObj
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumUserPropertiesAsASText, (PDSElement elem,
					PDSElementEnumUserPropertiesAsASTextProc proc,
					void *clientData,
					ASBool includeHidden))

/**
	Enumerates the PDSElement's user properties by traversing
	the list of attribute objects and class objects, calling
	the caller-supplied procedure for each entry in the
	properties array. The enumeration proc receives the
	property information as a Cos Dictionary, with contents
	as described in the PDF reference manual. The enumeration
	continues as long as the callback returns true, and halts
	when the proc returns false or all properties have been
	enumerated.

	@param elem The PDSElement whose user properties will be
		enumerated
	@param proc The callback that is called for each user
		property item
	@param clientData Client-supplied data to be passed to the
		client callback
	@param includeHidden Boolean indicating whether the client
		wants to be given property items that are marked as
		hidden.
	@return true if the enumeration completes, false if the
		enumeration callback returns false
	@see PDSElementEnumUserPropertiesAsASText
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumUserPropertiesAsCosObj, (PDSElement elem,
					PDSElementEnumUserPropertiesAsCosObjProc proc,
					void *clientData,
					ASBool includeHidden))

/**
	Starting at the supplied structure element, this procedure
	follows the chain of parents (see PDSElementGetParent) until
	a structure element is found that has user properties. If no
	such element is found (ie, the chain ended at the structure
	tree root), CosNull is returned.

	@param elem The PDSElement at which to start searching upwards
		through the tree.
	@return First ancestor of the elem that contains UserProperties
		attributes or class information, or CosNull if none is found.
	@see PDSElementEnumKidsWithUserProperties
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (PDSElement, PDSElementFindAncestorWithUserProperties, (PDSElement elem))

/**
	Enumerates PDSElements beneath the supplied PDSElement that
	have user properties attributes/classes.
	The elements in a structure tree that have user properties
	form a virtual tree themselves -- this procedure enumerates
	the children of the given structure element in this virtual
	tree. In other words, this procedure enumerates all the
	descendents(d) of the supplied structure element(e) such that
	PDSElementFindAncestorWithUserProperties(d) would return (e).
	The enumeration continues as long as the callback returns
	true, and halts when the proc returns false or all virtual
	children have been enumerated.

	@param elem The PDSElement below which to search for elements
		with user properties.
	@param proc The client-supplied callback to call for each
		element found.
	@param clientData Client-supplied data to be passed to the
		client callback
	@return true if the enumeration completes, false if the
		enumeration callback returns false.
	@see PDSElementFindAncestorWithUserProperties
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumKidsWithUserProperties, (PDSElement elem,
					EnumElementsWithUserPropertiesProc proc,
					void *clientData))

