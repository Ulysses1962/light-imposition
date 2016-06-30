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

 PDSWriteProcs.h

 - Catalog of functions exported by the PDSWrite HFT.

*********************************************************************/
/*
 * Structure Tree Root methods.
 */



#if !PLUGIN 
#undef XNPROC
#if !READER
#define XNPROC NPROC
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#endif
#endif

/**
	Creates a new StructTreeRoot element. 

	If PDDocCreateStructTreeRoot is called on a PDDoc that already 
	has a structure tree root, it returns without modifying 
	the document. 
	@param pdDoc IN/OUT? The PDDoc for which StructTreeRoot element 
	is created. 
	@param treeRoot IN/OUT? (Filled by the method) The newly-created 
	StructTreeRoot element. 
	@exception Raises an exception if pdDoc already has a StructTreeRoot. 
	
	@see PDDocGetStructTreeRoot 
	@see PDSTreeRootGetRoleMap 
	@see PDSTreeRootGetClassMap 
	@see PDDocRemoveStructTreeRoot 
	@see PDSTreeRootCreateRoleMap 
	@see PDSTreeRootRemoveRoleMap 
	@see PDSTreeRootCreateClassMap 
	@see PDSTreeRootRemoveClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDDocCreateStructTreeRoot,	(IN PDDoc pdDoc, OUT PDSTreeRoot *treeRoot))

/**
	Removes, but does not destroy, the specified StructTreeRoot 
	element from the specified PDDoc. 
	@param pdDoc IN/OUT? The PDDoc for which the StructTreeRoot element 
	is removed. 
	@see PDDocCreateStructTreeRoot 
	@see PDDocGetStructTreeRoot 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDDocRemoveStructTreeRoot,	(IN PDDoc pdDoc))


/**
	Inserts the specified kid element after the given position 
	as a kid of the specified structure tree root. 
	@param treeRoot IN/OUT? The structure tree root in which a kid 
	is inserted. 
	@param kid IN/OUT? The kid to insert. 
	@param insertAfter IN/OUT? Position after which the kid is inserted. 
	If element currently has no kids, insertAfter is ignored. 
	
	@exception Various 
	@see PDSTreeRootRemoveKid 
	@see PDSTreeRootReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootInsertKid,           (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement kid,
                                                 IN ASInt32 insertAfter))


/**
	Removes the specified kid element from the specified structure 
	tree root. 
	@param treeRoot IN/OUT? The structure tree root whose kid is removed. 
	
	@param kid IN/OUT? The kid to remove. 
	@exception Various 
	@see PDSTreeRootInsertKid 
	@see PDSTreeRootReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootRemoveKid,           (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement kid))

/**
	Replaces structural element oldKid with element newKid as 
	a kid of treeRoot. 
	@param treeRoot IN/OUT? The structure tree root whose kid is replaced. 
	
	@param oldKid IN/OUT? The kid to replace. 
	@param newKid IN/OUT? The kid that is replacing oldKid. 
	@exception Various 
	@see PDSTreeRootInsertKid 
	@see PDSTreeRootRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootReplaceKid,          (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement oldKid,
                                                 IN PDSElement newKid))

/**
	Creates and sets the PDSRoleMap of the specified StructTreeRoot 
	element. Any previously existing PDSRoleMap is unlinked. 
	
	@param treeRoot The structure tree root in which to create 
	a PDSRoleMap. 
	@param roleMap (Filled by the method) The newly created 
	PDSRoleMap.
	@exception Various 
	@see PDSTreeRootGetRoleMap 
	@see PDSTreeRootRemoveRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootCreateRoleMap,	(IN PDSTreeRoot treeRoot, OUT PDSRoleMap *roleMap))


/**
	Removes the PDSRoleMap of the specified structure tree root 
	element. Does nothing if one does not exist. 
	@param treeRoot The structure tree root whose PDSRoleMap 
	is removed.
	@exception Various 
	@see PDSTreeRootCreateRoleMap 
	@see PDSTreeRootGetRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootRemoveRoleMap,	(IN PDSTreeRoot treeRoot))


/**
	Creates a PDSClassMap in the specified tree root. 

	Any previously existing PDSClassMap is unlinked. 
	@param treeRoot The structure tree root in which to create 
	a PDSClassMap. 
	@param classMap (Filled by the method) The newly created 
	PDSClassMap.
	@exception Various 
	@see PDSTreeRootGetClassMap 
	@see PDSTreeRootRemoveClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootCreateClassMap,	(IN PDSTreeRoot treeRoot, OUT PDSClassMap *classMap))


/**
	Removes the PDSClassMap of the specified structure tree 
	root element. Does nothing if one does not exist. 
	@param treeRoot The structure tree root whose PDSClassMap 
	is removed.
	@exception Various 
	@see PDSTreeRootCreateClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootRemoveClassMap,	(IN PDSTreeRoot treeRoot))
/*
 * PDSElement methods.
 */


/**
	Creates a new (but empty) PDSElement. 
	@param pdDoc The PDDoc in which the PDSElement is created. 
	
	@param element (Filled by the method) The newly created 
	PDSElement.
	@exception Various 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementCreate,			(IN PDDoc pdDoc, OUT PDSElement *element))

/**
	Sets an element's type value to the specified type. The 
	type corresponds to the Subtype key in the structure element 
	dictionary. 

	PDSElementSetType sets the value of the Subtype key - not 
	the Type key - in the structure element dictionary. All PDSElements 
	have a Type value of StructElem. 
	@param element Element whose type is set. 
	@param type The ASAtom representing the element's type.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetType 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetType,			(IN PDSElement element, IN ASAtom type))

/**
	Sets an element's title. 
	@param element IN/OUT? Element whose title is set. 
	@param buffer IN/OUT? Pointer to a buffer containing a string to 
	be made the element's title. 
	@param nBytes IN/OUT? Number of bytes in buffer to use as element's 
	new title. May be zero. Sets a title even if the buffer 
	length is zero, but such a title looks like no title according 
	to PDSElementGetTitle. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetTitle 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetTitle,			(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Increments an element's revision count by one. 
	@param element Element whose revision count is incremented.
	@exception Various 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementIncrementRevision,(IN PDSElement element))

/**
	Associates the specified attribute object with an element 
	at the element's current revision value. 
	@param element Element with which attrObj is associated. 
	
	@param attrObj Attribute object to associate with element.
	@exception Various 
	@see PDSElementGetAttrObj 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementAddAttrObj,		(IN PDSElement element, IN PDSAttrObj attrObj))

/**
	Removes the specified attribute object from an element. 
	If element does not have an attrObj attribute, this method 
	does nothing. 

	NOTE: Calling PDSElementRemoveAttrObj while iterating over 
	the attribute objects of an element will change the relationship 
	between attribute object indices and attribute objects. 
	Although it is possible to track this change in indices 
	in a single loop, it is more straightforward to accumulate 
	a list of attribute objects to remove during one pass over 
	the attribute objects and to carry out the actual removals 
	during a subsequent iteration over the accumulated list. 
	
	@param element Element whose attribute is removed. 
	@param attrObj Attribute object to remove.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement or 
	@exception attrObj is not a valid attribute object. 
	@see PDSElementAddAttrObj 
	@see PDSElementGetAttrObj 
	@see PDSElementRemoveAllAttrObjs 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAttrObj,	(IN PDSElement element, IN PDSAttrObj attrObj))

/**
	Removes all attribute objects directly associated with the 
	specified element. 
	@param element Element whose attributes are removed.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAllAttrObjs,(IN PDSElement element))

/**
	Adds a class name to the element's list of classes to which 
	it belongs at the element's current revision value. 
	@param element IN/OUT? Element to which a class is added. 
	@param classAtom IN/OUT? The ASAtom representing the class to add 
	to element. If classAtom is already present among element's 
	classes, it will not be added again. 
	@exception Various 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementAddClass,			(IN PDSElement element, IN ASAtom classAtom))

/**
	Removes the specified class name from the element's list 
	of classes to which it belongs. 

	NOTE: Calling PDSElementRemoveClass while iterating over 
	the classes of an element will change the relationship between 
	class indices and classes. Although it is possible to track 
	this change in indices in a single loop, it is more straightforward 
	to accumulate a list of classes to remove during one pass 
	over the classes and to carry out the actual removals during 
	a subsequent iteration over the accumulated list. 
	@param element Element from which the specified class 
	is removed. 
	@param classAtom The ASAtom representing the class to 
	remove.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveClass,		(IN PDSElement element, IN ASAtom classAtom))

/**
	Removes all classes from the specified element. 
	@param element IN/OUT? Element whose classes are removed. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAllClasses,	(IN PDSElement element))

/**
	Sets the alternate text representation of an element's contents. 
	
	@param element IN/OUT? Element whose alternate text representation 
	is set. 
	@param buffer IN/OUT? Pointer to a buffer containing a string to 
	be made the element's alternate text representation. 
	@param nBytes IN/OUT? Number of bytes in buffer to use as element's 
	new alternate text representation. May be zero. Sets an 
	Alt string even if the buffer length is zero, but such an 
	Alt string looks like no Alt string according to PDSElementGetAlt. 
	
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetAlt 
	@see PDSElementHasAlt 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetAlt,			(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Inserts the specified kid PDSElement object into the specified 
	element after position insertAfter. 
	@param element Element in which the specified kid is inserted. 
	
	@param kid The kid to insert. 
	@param insertAfter Position after which the kid is inserted. 
	If element currently has no kids, insertAfter is ignored.
	@exception pdsErrWrongTypeParameter 
	@see PDSElementGetFirstPage 
	@see PDSElementGetKid 
	@see PDSElementInsertMCAsKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementInsertStmMCAsKid 
	@see PDSElementRemoveKid 
	@see PDSElementReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementInsertKid,		(IN PDSElement element, IN PDSElement kid, IN ASInt32 insertAfter))

/**
	Inserts a reference to the specified PDSMC (marked content) 
	in the specified element after position insertAfter. 

	This method automatically creates MCR objects if needed. 
	
	@param element Element in which the reference is inserted. 
	
	@param cosPage The CosObj for the page containing the 
	reference to insert. 
	@param mc The marked content to insert. 
	@param insertAfter Position after which the reference 
	is inserted. If element currently has no kids, insertAfter 
	is ignored.
	@exception Various 
	@see PDSElementInsertKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementInsertStmMCAsKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementInsertMCAsKid,	(IN PDSElement element,
                                                 IN CosObj cosPage,
                                                 IN PDSMC mc,
                                                 IN ASInt32 insertAfter))

/**
	Inserts a reference to the specified PDF object as a kid 
	into the specified element. 
	@param element IN/OUT? Element in which the reference is inserted. 
	
	@param cosPage IN/OUT? The CosObj for the page containing the reference 
	to insert. 
	@param obj IN/OUT? The CosObj to insert. 
	@param insertAfter IN/OUT? Position after which the reference is 
	inserted in element. If element currently has no kids, insertAfter 
	is ignored. 
	@exception Various 
	@see PDSElementReplaceKidOBJ 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementInsertOBJAsKid,       (IN PDSElement element,
                                                 IN CosObj cosPage,
                                                 IN CosObj obj,
                                                 IN ASInt32 insertAfter))

/**
	Removes the specified kid from an element. 

	NOTE: Approved method of removing OBJ kids is PDSElementRemoveKidOBJ. 
	
	@param element Element whose kid is removed. 
	@param kid Kid to remove.
	@exception Various 
	@see PDSElementGetKid 
	@see PDSElementInsertKid 
	@see PDSElementRemoveKidMC 
	@see PDSElementRemoveKidOBJ 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveKid,		(IN PDSElement element, IN CosObj kid))

/**
	Removes the specified PDSMC (marked content) from an element's 
	kids, if it has any. 

	After calling this method, use PDPageSetPDEContent to commit 
	any changes that have been made to the page contents. 
	@param element Element whose reference is removed. 
	@param cosPage The CosObj for the page containing the 
	reference to remove. 
	@param mc The marked content to remove.
	@exception Various 
	@see PDSElementInsertMCAsKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveKidMC,		(IN PDSElement element, IN CosObj cosPage, IN PDSMC mc))

/**
	Replaces the specified kid in the specified element. 

	NOTE: Approved method of replacing OBJ kids is PDSElementReplaceKidOBJ. 
	
	@param element IN/OUT? Element whose kid is replaced. 
	@param oldKid IN/OUT? Kid to replace. 
	@param newKid IN/OUT? Kid that is replacing oldKid. 
	@exception Various 
	@see PDSElementInsertKid 
	@see PDSElementGetFirstPage 
	@see PDSElementGetKid 
	@see PDSElementRemoveKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementReplaceKidOBJ 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementReplaceKid,		(IN PDSElement element, IN CosObj oldKid, IN CosObj newKid))

/**
	Replaces the specified PDSMC (on oldCosPage) with a new 
	PDSMC (on newCosPage) in the specified element. 
	@param element Element whose reference is replaced. 
	@param oldCosPage The CosObj for the page holding the 
	reference to replace. 
	@param oldMC The marked content to replace. 
	@param newCosPage The CosObj for the page holding the 
	reference that is replacing oldMC. 
	@param newMC The marked content that is replacing oldMC.
	@exception Various 
	@see PDSElementInsertMCAsKid 
	@see PDSElementRemoveKidMC 
	@see PDSElementReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementReplaceKidMC,		(IN PDSElement element, IN CosObj oldCosPage, IN PDSMC oldMC, IN CosObj newCosPage, IN PDSMC newMC))

/**
	Replaces oldObj with newObj on the specified page in the 
	specified element. 
	@param element IN/OUT? Element whose object is replaced. 
	@param oldObj IN/OUT? Object to replace. 
	@param newObj IN/OUT? Object that is replacing oldObj. 
	@param newPage IN/OUT? The CosObj for the page holding the reference 
	that is replacing oldObj. 
	@exception Various 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementReplaceKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementReplaceKidOBJ,        (IN PDSElement element,
                                                 IN CosObj oldObj,
                                                 IN CosObj newObj,
                                                 IN CosObj newPage))

/**
	Sets the ID of an element to the given Cos string. 
	@param element Element whose ID is set. 
	@param buffer Pointer to a buffer containing a string 
	to be made the element's ID. 
	@param nBytes Number of bytes in buffer to use as element's 
	new ID. May be zero. Sets an ID even if the buffer length 
	is zero, but such an ID looks like no ID according to PDSElementGetID.
	@exception Raises ErrSysPDSEdit if another element already has id as 
	its ID. 
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementGetID 
	@see PDSElementClearID 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementSetID,                (IN PDSElement element,
                                                 IN const ASUns8 *buffer,
                                                 IN ASInt32 nBytes))

/**
	Removes an element's ID, if it exists. 
	@param element Element whose ID is removed.
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSElementGetID 
	@see PDSElementSetID 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementClearID,              (IN PDSElement element))

/*
 * Attribute Object methods
 */

/**
	Creates a new attribute object with the specified owner. 
	
	@param pdDoc Document in which the attribute object is 
	created. 
	@param owner Owner of the new attribute object. 
	@param indirect If true, creates the attribute object 
	as an indirect Cos object and sets pdDoc's PDDocNeedsSave 
	flag (see PDDocFlags). If false, creates the attribute object 
	as a direct object. 
	@param attrObj (Filled by the method) The newly created 
	attribute object.
	@exception Various 
	@see PDSAttrObjCreateFromStream 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSAttrObjCreate,			(IN PDDoc pdDoc, IN ASAtom owner, IN ASBool indirect, OUT PDSAttrObj *attrObj))

/**
	Creates an attribute object with the specified owner from 
	the specified Cos stream. 
	@param owner Owner of the new attribute object. 
	@param cosStreamObj The Cos stream containing the data 
	with which to create the attribute. The dictionary of this 
	stream is modified. 
	@param attrObj (Filled by the method) Pointer to the newly 
	created attribute object. This actually points to cosStreamObj. 
	May be NULL.
	@exception Among others, raises pdsErrWrongTypeParameter if cosStreamObj 
	is not a Cos 
	@exception stream. 
	@see PDSAttrObjCreate 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSAttrObjCreateFromStream,	(IN ASAtom owner, IN OUT CosObj cosStreamObj, OUT PDSAttrObj *attrObj))


/*
 * RoleMap methods.
 */

/**
	Maps an element type src to another element type dst in 
	the specified PDSRoleMap. 
	@param roleMap The PDSRoleMap in which to create a new 
	mapping. 
	@param src Element type to map to dst. 
	@param dst Element type that src maps onto. NOTE: This 
	may be a standard element type, such as P.
	@exception Raises ErrSysPDSEdit if src is already mapped. 
	@see PDSRoleMapDoesMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapMap,				(IN PDSRoleMap roleMap, IN ASAtom src, IN ASAtom dst))

/**
	Makes the specified element type have no mapping. 
	@param roleMap IN/OUT? The PDSRoleMap in which to un-map the src 
	element type. 
	@param src IN/OUT? Element type whose mapping is removed. 
	@param fixupOthers IN/OUT? If true, any element type that was directly 
	mapped to src is mapped to whatever src previously mapped 
	to. If false, PDSRoleMapUnMapSrc only un-maps src. 
	@exception Various 
	@see PDSRoleMapUnMapDst 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapUnMapSrc,			(IN PDSRoleMap roleMap, IN ASAtom src, IN ASBool fixupOthers))

/**
	Makes the specified element type have no mapping. 
	@param roleMap The PDSRoleMap in which to un-map all element 
	types that map onto the dst element type. 
	@param dst Element type to which all mappings are removed. 
	All element types that map to the dst element type are unmapped.
	@exception pdsErrWrongTypeParameter 
	@see PDSRoleMapUnMapSrc 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapUnMapDst,			(IN PDSRoleMap roleMap, IN ASAtom dst))

/**
	Makes a copy of a PDSRoleMap, making it the PDSRoleMap of 
	the specified StructTreeRoot. 
	@param srcRoleMap The PDSRoleMap to copy. 
	@param dstTreeRoot The structure tree root in which to 
	place srcRoleMap. 
	@param dstRoleMap (Filled by the method) If not NULL, 
	points to the new, copied PDSRoleMap.
	@exception Various 
	@see PDSTreeRootGetRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapCopy,				(IN PDSRoleMap srcRoleMap, IN PDSTreeRoot dstTreeRoot, OUT PDSRoleMap *dstRoleMap))

/*
 * ClassMap methods.
 */

/**
	Adds the specified attribute object to the specified PDSClassMap 
	for the given class name. If the attribute object is already 
	present, it is not added a second time. 
	@param classMap The PDSClassMap to which the specified 
	attribute object is added. 
	@param classAtom The ASAtom representing the class name. 
	
	@param attrObj Attribute object to add to the class in 
	classAtom.
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSClassMapGetAttrObj 
	@see PDSClassMapRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapAddAttrObj,		(IN PDSClassMap classMap,
                                             IN ASAtom classAtom,
                                             IN PDSAttrObj attrObj))

/**
	Removes the specified class from the specified PDSClassMap, 
	if it exists. 
	@param classMap IN/OUT? The PDSClassMap from which a class is removed. 
	
	@param classAtom IN/OUT? The ASAtom representing the class to remove 
	from classMap. 
	@exception Various 
	@see PDSClassMapRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapRemoveClass,		(IN PDSClassMap classMap, IN ASAtom classAtom))

/**
	Removes the specified attribute object from the specified 
	PDSClassMap. If classAtom is ASAtomNull, removes all occurrences 
	of attrObj in the entire classMap. 
	@param classMap The PDSClassMap from which the specified 
	attribute object is removed. 
	@param classAtom The ASAtom of a class name for which 
	the associated attribute object is found. 
	@param attrObj Attribute object to remove from classMap.
	@exception Raises pdsErrBadPDF if an error is found in the PDF file. 
	
	@see PDSClassMapAddAttrObj 
	@see PDSClassMapRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapRemoveAttrObj,	(IN PDSClassMap classMap,
                                             IN ASAtom classAtom,
                                             IN PDSAttrObj attrObj))
/*
 * New in Acrobat 5
 */


/**
	Sets the actual text representation of the specified PDSElement's 
	contents to buffer (from 0 to nBytes) 
	@param element The PDSElement whose contents are being 
	set to buffer. 
	@param buffer The buffer to which the PDSElement's contents 
	are being set. 
	@param nBytes The number of bytes in the text representation.
	@see PDSElementGetActualText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
UNPROC (void,	PDSElementSetActualText,	(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Sets the language field associated with the PDSElement to 
	buffer's contents (from 0 to nBytes). 
	@param element The PDSElement whose language field is 
	set to buffer. 
	@param buffer Pointer to a buffer containing a string 
	to be made the element's language field. The empty string 
	indicates that the language is unknown. String should be 
	in format <IETF RFC-1766-language-code>. NOTE: ISO 639 language 
	codes can be found at:\n
	http:\/\/lcweb.loc.gov\/standards\/iso639-2\n
	NOTE: IANA registered language codes can be found at:\n
	http:\/\/www.isi.edu\/in-notes\/iana\/assignments\/languages\n
	NOTE: The IETF Standard for Language Element Values (RFC 
	1766) can be found at:\n
	http:\/\/www.ietf.org\/rfc\/rfc1766.txt?number=1766 
	@param nBytes Size of buffer. May be zero. Sets the language 
	even if the buffer length is zero, but such a language setting 
	looks like no language according to PDSElementGetLanguage.
	@see PDSElementGetLanguage 
	@see PDSElementHasLanguage 
	@since PI_PDS_WRITE_VERSION >= 0x00050000
*/
UNPROC (void,	PDSElementSetLanguage,		(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Removes an OBJ from among the kids of a given element. Does 
	nothing if the given OBJ isn't a kid of the given element. 
	
	@param element Element whose kid is having an OBJ removed. 
	
	@param kid kid whose OBJ is removed.
	@exception Various 
	@see PDSElementInsertMCAsKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00050000
*/
UNPROC (void, PDSElementRemoveKidOBJ, (IN PDSElement element, IN CosObj kid))

/*
 * New since Acrobat 5
 */

/**
	Extends PDSElementInsertMCAsKid, inserting content that is in a
	stream other than a page content stream. This function is the same 
	as PDSElementInsertStmMCAsKid.
	@param element Element in which the reference is inserted.
	@param cosPage The CosObj for the page containing the reference to insert.
	@param mc The marked content to insert.
	@param insertAfter Position after which the reference is inserted. If
	element currently has no kids, insertAfter is ignored.
	@param cosStream The stream containing the content given by mc.
	@param streamOwner A Cos object to record as the owner of the content. Can
	be CosNull if owner is not important.
	@exception Various
	@see PDSElementInsertKid
	@see PDSElementInsertMCAsKid
	@see PDSElementInsertOBJAsKid
	@see PDSElementInsertStmMCAsKid
	@see PDSElementReplaceKidMC
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void,  PDSElementInsertMCAsKidEx, (IN PDSElement element,
                                          IN CosObj cosPage,
                                          IN PDSMC mc,
                                          IN ASInt32 insertAfter,
                                          IN CosObj cosStream,
                                          IN CosObj streamOwner))

/**
	Inserts a marked content sequence from a non-page-content 
	stream as a kid of the specified element. 
	@param element Element in which the reference is inserted. 
	
	@param cosPage The CosObj for the page containing the 
	reference to insert. 
	@param containingStm The stream containing the content given 
	by mc. 
	@param stmOwner The PDF object owning the stream given 
	in cosStream; for example, the annotation to which an appearance 
	stream belongs. Can be CosNull if owner is not important. 
	
	@param mc The marked content to insert. 
	@param insertAfter Position after which the reference 
	is inserted. If element currently has no kids, insertAfter 
	is ignored.
	@exception Various 
	@see PDSElementInsertKid 
	@see PDSElementInsertMCAsKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void, PDSElementInsertStmMCAsKid, (PDSElement element,
                                          CosObj cosPage,
                                          CosObj containingStm,
                                          CosObj stmOwner,
                                          PDSMC mc,
                                          ASInt32 insertAfter))

/**
	Updates the stream entries (Stm) in marked content reference 
	dictionaries to reference a new Cos stream object. Replaces 
	references to the old stream with refererences to the new 
	stream. 
	@param treeRoot The structure tree root in which stream 
	references are updated. 
	@param oldStream The stream reference to replace. 
	@param newStream The stream reference that is replacing 
	oldStream.
	@exception Various 
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void, PDSTreeRootReplaceStreamRef, (PDSTreeRoot treeRoot,
                                           CosObj oldStream,
                                           CosObj newStream))

/**
	Creates a reference handle to a piece of marked content that 
	can be used to associate the content with structure. The handle 
	can persist beyond the lifetime of the marked contents allowing 
	greater flexibility about when structure information can be 
	created.

	NOTE: This must be called before placing the container within
	the content stream that owns it.

	NOTE: The handle will persist until PDSMCRefDestroy is called.

	NOTE: All values in the PDSMCInfo object apart from mcid are currently ignored.

	@param container The marked content to create a reference for. 
	Must be either a PDEContainer or PDEBeginContainer.
	@param cosDoc	the document the reference will be used within.
	@param mcid		The mcid to set for the container
	@exception Various 
	@see PDSMCRefDestroy
	@see PDSElementInsertMCRefAsKid
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (PDSMCRef, PDSMCRefCreate,	(IN PDEElement container , 
									 IN CosDoc     cosDoc,
									 IN ASInt32	   mcid))

/**
	Destroys a marked content reference created with PDSMCRefCreate. 
	This should only be called once the reference has been placed in
	the structure tree or if the reference is no longer needed.

	NOTE: If the PDSMCRef is associated with a PDSMC it will be set as 
	invalid and ignored on subsequent processing.

	@param ref The marked content reference to destroy.
	@exception Unknown 
	@see PDSMCRefCreate
	@see PDSElementInsertMCRefAsKid
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (void, PDSMCRefDestroy, (IN PDSMCRef ref))

/**
	Takes a marked content reference and places the content 
	that it identifies in the structure as a child of the element.

	NOTE: the content reference handle will be filled out automatically
	if PDPageSetPDEContent, PDEFormSetContent, or PDEGroupSetContent is 
	called. Otherwise, PDEContentSetPage or PDEContentSetContainingStmAndOwner
	must be called explicitly.

	@param element The structure element to associate marked content with.
	@param ref The marked content reference describing the content on the page.
	Must have had a valid MCID and have been completed by subsequent content 
	stream processing calls.
	@param insertAfter The position after which the marked content is inserted into 
	the element’s kids. If the element has no children then insertAfter is ignored.
	@exception Various
	@see PDSMCRefCreate
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (ASBool, PDSElementInsertMCRefAsKid,	(IN PDSElement element, 
											IN PDSMCRef ref, 
											IN ASInt32 insertAfter))

#undef XNPROC
