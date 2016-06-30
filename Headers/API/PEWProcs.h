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

 PEWProcs.h

 - Catalog of functions exported by the PDFEditWrite HFT.

*********************************************************************/

/*------------------------------------------------------------------------
	PDFEdit Public Methods - Generic PDEContent and PDEElement methods.
------------------------------------------------------------------------*/

/**
	Creates an empty content object. 

	NOTE: Do not use this method to create a PDEContent to be 
	put into a PDPage. Instead, call PDPageAcquirePDEContent. 
	

	Call PDERelease to dispose of the returned content object 
	when finished with it. 
	@return An empty content object. 
	@exception peErrPStackUnderflow 
	@see PDEContentCreateFromCosObj 
	@see PDPageAcquirePDEContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEContent, PDEContentCreate, (void))

/**
	This is the main method for converting a PDEContent into 
	PDF contents and resources. 

	NOTE: Do not use this method to put a PDEContent into a 
	PDPage. Instead, call PDPageSetPDEContent. 

	This method does not change the PDEContent object nor its 
	reference count. 

	The caller of this function is responsible for adding the 
	contents and the resources returned from this method to 
	the Page Object. 
	@param pdeContent IN/OUT? A content object. 
	@param flags IN/OUT? Flags specifying the type of object to create 
	(page contents, form, or charproc) and how it is created. 
	Must be one or more of PDEContentToCosObjFlags. 
	@param attrs IN/OUT? A pointer to a PDEContentAttrs structure that 
	contains the appropriate form attributes or cache device/ 
	char-width attributes, and so on. If zero, no attributes 
	are set. 
	@param attrsSize IN/OUT? Size of the attrs buffer, in bytes. Zero 
	if attrs is zero. 
	@param cosDoc IN/OUT? Document in which the contents and resources 
	are created. 
	@param filtersP IN/OUT? A pointer to a PDEFilterArray structure 
	that specifies which filters to use in encoding the contents; 
	may be NULL. If filtersP contains any encodeParms, they 
	must belong to cosDoc. 
	@param contentsP IN/OUT? (Filled by the method) Cos object for 
	the resulting contents in pdeContent. 
	@param resourcesP IN/OUT? (Filled by the method) Cos object for 
	the resulting resources in pdeContent. NOTE: The client 
	is responsible for putting the resourcesP dictionary into 
	the contentsP stream for non-page objects. The client must 
	do this for XObject Forms and appearance dictionaries in 
	annotations. For Type 3 fonts, the resource dictionaries 
	must be merged and put into the Type 3 font dictionary. 
	For a page, the contents and resources must be put into 
	the page object. 
	@exception peErrUnknownResType 
	@exception pageErrErrorParsingImage 
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception peErrUnknownPDEColorSpace 
	@see PDEContentCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContentToCosObj, (
			IN	PDEContent pdeContent, 
			IN	ASUns32 flags, 			/* kPDEContentToPage, kPDEContentToForm, kPDEToContentCharProc */
			IN	PDEContentAttrsP attrs, 
			IN	ASUns32 attrsSize,
			IN	CosDoc cosDoc, 
			IN	PDEFilterArrayP filtersP,
			OUT	CosObj *contentsP, 
			OUT	CosObj *resourcesP)
	)

/**
	Removes an element from a PDEContent. 

	NOTE: This decrements the reference count of the element 
	removed. 
	@param pdeContent IN/OUT? A content object. 
	@param index IN/OUT? Index in pdeContent of the element to remove, 
	whose reference count is decremented. 
	@exception peErrWrongPDEObjectType 
	@see PDEContentAddElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContentRemoveElem,(
			IN	PDEContent pdeContent, 
			IN	ASInt32 index)
	)

/**
	Inserts an element into a PDEContent. 

	NOTE: This method increments the reference count of pdeElement. 
	
	@param pdeContent The content to which pdeElement is added. 
	
	@param addAfterIndex Location after which pdeElement is 
	added. Should be kPDEBeforeFirst to add to the beginning 
	of the display list. 
	@param pdeElement The element to add to pdeContent. The 
	reference count of pdeElement is incremented.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContentRemoveElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
	@see PDEContentRemoveElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEContentAddElem, (
			IN	PDEContent pdeContent, 
			IN	ASInt32 addAfterIndex, 
			IN	PDEElement pdeElement)
	)
/*------------------------------------------------------------------------
	General methods for PDEPath, PDEImage and PDEXObject.
------------------------------------------------------------------------*/

/**
	Sets the graphics state information for an element. 

	This method is valid only for PDEForm, PDEImage, PDEPath,and 
	PDEShading elements. 

	NOTE: This method causes any of stateP's color space or 
	ExtGState objects to have their reference count incremented 
	and previous graphic state objects to be decremented. 
	@param pdeElement An element whose graphics state is set. 
	
	@param stateP Pointer to a PDEGraphicStateP structure 
	with graphics state information to set for pdeElement. Any 
	of stateP's color space or ExtGState objects have their 
	reference count incremented. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm Will raise a genErrBadParm if the first parameter, pdeElement, 
	does not have a graphics state associated with it. 
	@see PDEElementGetGState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEElementSetGState, (
			IN	PDEElement pdeElement, 
			IN	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Sets the transformation matrix for an element. 

	The element may not be a PDEContainer, a PDEGroup, a PDEPlace, 
	or a PDEText. 
	@param pdeElement IN/OUT? An element whose transformation matrix 
	is set. 
	@param matrixP IN/OUT? Pointer to ASFixedMatrix that holds the 
	transformation matrix to set for pdeElement. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementGetMatrix 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEElementSetMatrix, (
			IN	PDEElement pdeElement, 
			IN	ASFixedMatrixP matrixP)
	)

/**
	Sets the current clip for an element. 

	pdeElement's previous clip's reference count is decremented 
	(if it had one), and pdeClip's reference count is incremented. 
	
	@param pdeElement IN/OUT? An element whose clip is set. 
	@param pdeClip IN/OUT? The clip to set for pdeElement. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementGetClip 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEElementSetClip, (
			IN	PDEElement pdeElement, 
			IN	PDEClip pdeClip)
	)

/**
	Makes a copy of an element. The caller is responsible for 
	releasing the copy with PDERelease. 
	@param pdeElement IN/OUT? The element to copy. 
	@param flags IN/OUT? Bit field of PDEElementCopyFlags. 
	@return A copy of pdeElement. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContentGetElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEElement, PDEElementCopy, (
			IN	PDEElement pdeElement,
			IN	ASUns32 flags)
	)
		
/*------------------------------------------------------------------------
	PDEText methods.
------------------------------------------------------------------------*/

/**
	Sets the graphics state of a text run. 

	NOTE: This method increments the reference count of objects 
	in the stateP. 
	@param pdeText Text object containing a text run whose 
	graphics state is set. 
	@param runIndex Index of the text run. 
	@param stateP Pointer to a PDEGraphicStateP structure 
	with graphics state to set. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetGState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetGState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Sets the text state of a text run. 

	NOTE: This method has the following side effect (Acrobat 
	5): It modifies the text matrix of the run. In order to 
	maintain backward compatibility, this method only directly 
	operates on the first four fields of PDETextState. When 
	it is called, it calculates a new text matrix with three 
	additional fields - fontSize, hScale, and textRise (see PDETextState). 
	To avoid this behavior, use PDETextRunSetState instead (which 
	was added as an API to address this problem). 
	@param pdeText Text object containing a text run whose 
	text state is set. 
	@param runIndex Index of the text run. 
	@param stateP Pointer to a PDETextState structure with 
	text state. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextState 
	@see PDETextRunSetState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetTextState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Sets the font of a text run. 

	NOTE: This method decrements the reference count of the 
	previous font and increments the reference count of the 
	new font. 
	@param pdeText IN/OUT? Text object containing a text run whose 
	font is set. 
	@param runIndex IN/OUT? Index of the text run. 
	@param font IN/OUT? Font set for the text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextRunSetFont, (
			IN	PDEText pdeText,
			IN	ASInt32 runIndex,
			IN	PDEFont font)
	)

/**
	Sets the text matrix of a text run. 
	@param pdeText IN/OUT? Text object containing a text run whose 
	text matrix is set. 
	@param runIndex IN/OUT? Index of the text run. 
	@param matrixP IN/OUT? Pointer to ASFixedMatrix that holds the 
	text matrix. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextMatrix 
	@see PDETextRunSetMatrix 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextRunSetTextMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)

/**
	Sets the stroke matrix of a text run. 

	NOTE: Currently this method is not implemented. (Acrobat 
	5) 
	@param pdeText Text object containing a text run whose 
	stroke matrix is set. 
	@param runIndex Index of the text run. 
	@param matrixP Pointer to ASFixedMatrix that holds the 
	stroke matrix.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetStrokeMatrix 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetStrokeMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)

/**
	Adds a character or a text run to a PDEText object. 

	NOTE: This method does not change the reference count of 
	pdeText; however, the reference count of the objects in 
	the gstateP are incremented. 
	@param pdeText Text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. Must be either: kPDETextChar - for a text character 
	kPDETextRun - for a text run 
	@param index Index after which to add character or text 
	run. 
	@param text Pointer to the characters to add. NOTE: Passing 
	NULL for text can invalidate the text object but will not 
	raise an error. Callers must not pass NULL for this parameter. 
	
	@param textLen Length of the text, in bytes. 
	@param font Font for the element. 
	@param gstateP Pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen Length of graphics state for the element.
	@param tstateP Pointer to a PDETextState structure with 
	text state for the element. NOTE: PDFEdit ignores the wasSetFlags 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen Length of text state for the element. 
	
	@param textMatrixP Pointer to ASFixedMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP Pointer to ASFixedMatrix that holds 
	the matrix for the line width when stroking text. May be 
	NULL. NOTE: Currently this field is not used. (Acrobat 5)
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextIsAtPoint 
	@see PDETextReplaceChars 
	@see PDETextSplitRunAt 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextAdd, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	ASUns8 *text, 
			IN	ASInt32 textLen, 
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASFixedMatrixP textMatrixP,
			IN	ASFixedMatrixP strokeMatrixP)
	)

/**
	Removes characters or text runs from a text object. 

	NOTE: This method decrements the reference count of objects 
	associated with the pdeText in the graphic state and font. 
	
	@param pdeText IN/OUT? Text object from which text is removed. 
	
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param count IN/OUT? Number of characters or text runs to remove. 
	
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextAdd 
	@see PDETextReplaceChars 
	@see PDETextSplitRunAt 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextRemove, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASInt32 count)
	)

/**
	Creates an empty text object. 

	Call PDERelease to dispose of the returned text object when 
	finished with it. 
	@return An empty text object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEText, PDETextCreate,
			(void)
	)

/*------------------------------------------------------------------------
	PDEPath methods.
------------------------------------------------------------------------*/
			

/**
	Sets new path data for a path element. 
	@param path IN/OUT? The path whose data is set. 
	@param data IN/OUT? Pointer to path data. It is a variable-sized 
	array of path operators and operands. The format is a 32-
	bit operator followed by 0 to 3 ASFixedPoint values, depending 
	on the operator. Operators are codes for moveto, lineto, 
	curveto, rect, or closepath operators and must be one of 
	PDEPathElementType. Operands are ASFixedPoint values. The 
	data is copied into the PDEPath object. 
	@param dataSize IN/OUT? Size of the new path data, in bytes. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEPathGetData 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPathSetData, (
			IN	PDEPath path, 
			IN	ASInt32 *data, 
			IN	ASUns32 dataSize)
	)

/**
	Sets the fill and stroke attributes of a path. 
	@param path IN/OUT? The path whose fill and stroke attributes are 
	set. 
	@param op IN/OUT? The operation to set; must be one of PDEPathOpFlags. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEPathGetPaintOp 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPathSetPaintOp, (
			IN	PDEPath path, 
			IN	ASUns32 op)
	)

/**
	Creates an empty path element. 

	Call PDERelease to dispose of the returned path object when 
	finished with it. 
	@return An empty path element. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPath, PDEPathCreate, (void)
	)

/*------------------------------------------------------------------------
	PDEImage methods.
------------------------------------------------------------------------*/

/* Image filtering -- see the notes in PERProcs.h
*/

/**
	Sets data for an image. 
	@param image IN/OUT? Image whose data is set. 
	@param flags IN/OUT? A set of PDEImageDataFlags flags. If kPDEImageEncodedData 
	is set, the data must be encoded for the current filters, 
	and encodedLen is the length of the encoded data. If the 
	kPDEImageEncodedData flag is not set, data is not encoded 
	and encodedLen is the size of the decoded data. 
	@param buffer IN/OUT? Image data. 
	@param encodedLen IN/OUT? Length of the encoded data. 
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageSetData, (
			IN	PDEImage image, 
			IN	ASUns32 flags,
			IN	ASUns8 *buffer,
			IN	ASUns32 encodedLen)
	)

/**
	Sets a data stream for an image. Can only be used for XObject 
	images. 

	The caller must dispose of the stream by calling ASStmClose. 
	
	@param image IN/OUT? Image whose data stream is set. 
	@param flags IN/OUT? PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, the stream must be encoded. 
	@param filtersP IN/OUT? Pointer to PDEFilterArray structure. If 
	not NULL, is used to build Cos objects for the Filter, DecodeParms, 
	and EncodeParms objects. If filtersP is NULL, the existing 
	Filter and DecodeParms are used. EncodeParms is set to DecodeParms 
	if it exists. 
	@param stm IN/OUT? Stream for the image data. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageSetDataStm, (
			IN	PDEImage image, 
			IN	ASUns32 flags,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm stm)
	)

/**
	Creates an image object. 

	The image data may be specified as a stream or as a buffer. 
	If dataStm is non-NULL, data is ignored. 

	See PDEImageSetDataStm for information on handling the stream. 
	

	The caller must dispose of datStm after calling this method. 
	

	Call PDERelease to dispose of the returned image object 
	when finished with it. 
	@param attrsP IN/OUT? Pointer to PDEImageAttrs with attributes 
	of the image. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@param matrixP IN/OUT? Pointer to ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT? PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then encodedLen must specify the length of data. 
	
	@param colorSpace IN/OUT? Color space of the image. When the image 
	is an imagemask, colorSpace is the color space of the colorValueP 
	argument. 
	@param colorValueP IN/OUT? Pointer to PDEColorValue structure. 
	If the image is an image mask, colorValueP must be provided. 
	
	@param filtersP IN/OUT? Pointer to PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	may be NULL. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT? Stream holding the image data. 
	@param data IN/OUT? Image data. If dataStm is non-NULL, data is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the dataStm parameter for 
	the image data or use the PDEImageCreateFromCosObj method. 
	
	@param encodedLen IN/OUT? Encoded length of data, in bytes. 
	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEImage, PDEImageCreate, (
			IN	PDEImageAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASFixedMatrixP matrixP,
			IN	ASUns32 flags,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm dataStm, 
			IN	ASUns8 *data,
			IN	ASUns32 encodedLen)
	)

/**
	Creates an image object from a Cos object. 

	Call PDERelease to dispose of the returned image object 
	when finished with it. 
	@param imageObjP IN/OUT? Cos object for the image. 
	@param matrixP IN/OUT? Pointer to ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param colorSpace IN/OUT? Color space used for the image, if the 
	image is an image mask; otherwise, set to NULL. 
	@param colorValueP IN/OUT? Pointer to PDEColorValue structure. 
	If the image is an imagemask, colorValueP must be provided. 
	
	@return Image corresponding to the Cos object. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreate 
	@see PDEImageGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEImage, PDEImageCreateFromCosObj, (
			IN	const CosObj *imageObjP,
			IN	ASFixedMatrixP matrixP,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP)
	)


/*------------------------------------------------------------------------
	PDEClip methods.
------------------------------------------------------------------------*/

/* PDEClip objects can contain PDEContainers and PDEGroups to an  
** arbitrary level of nesting.  This allows PDEContainers to be used
** to mark clip objects.  
**
** PDEGroups inside PDEClips which contain at least one PDEText,
** and no PDEPaths have a special meaning.  All	PDEText objects contained 
** in such a PDEGroup are considered to be part of the same BT,ET 
** block.  This means that the union of these PDETexts makes up a single
** clipping path, as opposed to the intersection of the PDETexts.
*/

/**
	Adds an element to a clip path. 
	@param clip IN/OUT? The clip path to which an element is added. 
	
	@param addAfterIndex IN/OUT? The index after which to add pdeElement. 
	Use kPDEBeforeFirst to insert an element at the beginning 
	of the clip object. 
	@param pdeElement IN/OUT? The element added, which may be a PDEPath, 
	a PDEText, a PDEContainer, a PDEGroup, or a PDEPlace object. 
	NOTE: This method increments the reference count of pdeElement. 
	
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipRemoveElems 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEClipAddElem, (
			IN	PDEClip clip, 
			IN	ASInt32 addAfterIndex, 		/* kPDEBeforeFirst */
			IN	PDEElement pdeElement)
	)

/**
	Removes one or more elements from a clip object. 

	NOTE: This method decrements the reference count of each 
	of the elements. 
	@param clip IN/OUT? The clip object from which an element is removed. 
	
	@param index IN/OUT? First element to remove. 
	@param count IN/OUT? Number of elements to remove. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipAddElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEClipRemoveElems, (
			IN	PDEClip clip, 
			IN	ASInt32 index, 
			IN	ASInt32 count)
	)

/**
	Creates an empty clip object. This represents a clipping 
	object that has no effect on elements that refer to it. 
	

	Call PDERelease to dispose of the returned clip object when 
	finished with it. 
	@return The newly created clip object. 
	@notify Raises if unable to allocate memory. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEClip, PDEClipCreate, (void)
	)

/*------------------------------------------------------------------------
	PDEXObject methods.
------------------------------------------------------------------------*/

/**
	Creates a new PDEXObject from a Cos object. 

	Call PDERelease to dispose of the returned PDEXObject when 
	finished with it. 
	@param cosObjP IN/OUT? Cos object for the PDEXObject. 
	@return PDEXObject corresponding to the cosObjP. 
	@see PDEXObjectGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEXObject, PDEXObjectCreate, (
			IN	const CosObj *cosObjP)
	)

/*------------------------------------------------------------------------
	PDEForm methods.
------------------------------------------------------------------------*/

/**
	Creates a new form from an existing Cos object. 

	Call PDERelease to dispose of the returned form object when 
	finished with it. 
	@param xObjectP Cos object from which a PDEForm is created. 
	
	@param resourcesP The xObjectP's Resources dictionary. 
	If you do not pass in a Resource object, subsequent calls 
	to PDPageAcquirePDEContent will fail (after the file is 
	saved). 
	@param matrixP Pointer to ASFixedMatrix that holds the 
	transformation matrix to use for the form.
	@return The newly created form object. 
	@see PDEFormCreateClone 
	@see PDEFormGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEForm, PDEFormCreateFromCosObj, (
			IN	const CosObj *xObjectP,
			IN	const CosObj *resourcesP,
			IN	ASFixedMatrixP matrixP)
	)

/**
	Gets a PDEContent object for a form. 

	NOTE: Unlike other 'GetContent' methods, this method does 
	increment the reference count of the returned PDEContent. 
	
	@param form The form whose content is obtained.
	@return Content for form. 
	@exception peErrWrongPDEObjectType 
	@exception peErrPStackUnderflow 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEContent, PDEFormGetContent, (
 			IN	PDEForm form)
	  )

					
/*------------------------------------------------------------------------
	PDEPS methods. (Passthrough PostScript)
------------------------------------------------------------------------*/

/**
	Sets the data for an object of type PDEPS. 
	@param ps IN/OUT? An object of type PDEPS. 
	@param buffer IN/OUT? Contains the data. 
	@param bufferSize IN/OUT? Length of the data in bytes. 
	@see PDEPSGetData 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00020000

*/
UNPROC(void, PDEPSSetData, (
			IN	PDEPS ps, 
			IN	ASUns8 *buffer, 
			IN	ASUns32 bufferSize)
	)

/**
	Sets a stream for the data. The data must be un-encoded 
	(no filters). The caller must dispose of the stream. 
	@param ps IN/OUT? An object of type PDEPS. 
	@param stm IN/OUT? stm is a stream for the data. 
	@see PDEPSGetDataStm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00020000

*/
UNPROC(void, PDEPSSetDataStm, (
			IN	PDEPS ps, 
			IN	ASStm stm )
	)

/**
	Creates a PDEPS object. data and dataStm may be NULL. If 
	so, use PDEPSSetData and PDEPSSetDataStm to attach data 
	to the object. If dataStm is non-NULL, then data will be 
	ignored. 

	See the description of PDEPSSetDataStm for how the stream 
	is handled. If data is non-NULL and dataStm is NULL, the 
	data must contain dataSize number of bytes as specified 
	in the PDEPSAttrsP. 

	The caller must dispose of the stream after calling this 
	method. 
	@param attrsP IN/OUT? Pointer to PDEPSAttrs attributes data structure. 
	
	@param attrsSize IN/OUT? Size of attributes data structure (sizeof( 
	PDEPSAttrs)). 
	@param dataStm IN/OUT? (May be NULL) Data. See above. 
	@param data IN/OUT? (May be NULL) Data stream. See above. 
	@param dataSize IN/OUT? Number of bytes of data. 
	@return An object of type PDEPS. 
	@see PDEPSCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00020000

*/
UNPROC(PDEPS, PDEPSCreate, (
			IN	PDEPSAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASStm dataStm,
			IN	ASUns8 *data, 
			IN	ASUns32 dataSize)
	)

/**
	Creates a PDEPS object from a CosObj object. 
	@param cosObjP IN/OUT? Object of type CosObj. 
	@return An object of type PDEPS. 
	@exception genErrBadParm 
	@see PDEPSCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00020000

*/
UNPROC(PDEPS, PDEPSCreateFromCosObj, (
			const CosObj *cosObjP)
	)
			
/*------------------------------------------------------------------------
	PDEFont methods.
------------------------------------------------------------------------*/

/**
	Creates a new PDEFont from the specified parameters. 

	The PDEFont may be represented as an embedded font (a FontFile 
	entry in the font descriptor of the PDF file). To create 
	a PDEFont that is stored as an embedded font, the FontFile 
	stream may be passed in fontStm, and the len1, len2, and 
	len3 parameters contain the Length1, Length2, and Length3 
	values of the FontFile stream attributes dictionary. See 
	Section 5.8 in the PDF Reference for more information about 
	embedded fonts. 

	The caller must close fontStm with ASStmClose after invoking 
	PDEFontCreate. 

	Call PDERelease to dispose of the returned font object when 
	finished with it. 
	@param attrsP Pointer to a PDEFontAttrs structure for 
	the font attributes. 
	@param attrsSize Size of the attrsP buffer, in bytes. 
	
	@param firstChar First character index for the widths 
	array, widthsP. 
	@param lastChar Last character index for the widths array, 
	widthsP. 
	@param widthsP Pointer to widths array. 
	@param encoding An array of 256 pointers to glyph names 
	specifying the custom encoding. If any pointer is NULL, 
	no encoding information is written for that entry. 
	@param encodingBaseName Encoding base name if the encoding 
	is a custom encoding. If encoding is NULL, encodingBaseName 
	is used as the value of the encoding, and must be one of 
	WinAnsiEncoding, MacRomanEncoding, or MacExpertEncoding. 
	If no encoding value is desired, use ASAtomNull. 
	@param fontStm Stream with font information.
	@param len1 Length in bytes of the ASCII portion of the 
	Type 1 font file after it has been decoded. For other font 
	formats, such as TrueType or CFF, only len1 is used, and 
	it is the size of the font. 
	@param len2 Length in bytes of the encrypted portion of 
	the Type 1 font file after it has been decoded. 
	@param len3 Length in bytes of the portion of the Type 
	1 font file that contains the 512 zeros, plus the cleartomark 
	operator, plus any following data. 
	@return The specified PDEFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEFont, PDEFontCreate, (
			IN	PDEFontAttrsP attrsP,
			IN	ASUns32 attrsSize,
			IN	ASInt32 firstChar,
			IN	ASInt32 lastChar,
			IN	ASInt16 *widthsP, 
			IN	char **encoding,
			IN	ASAtom encodingBaseName, 
			IN	ASStm fontStm,
			IN	ASInt32 len1,
			IN	ASInt32 len2,
			IN	ASInt32 len3)
	)

/**
	Creates a PDEFont corresponding to a Cos object of type 
	Font. 

	Call PDERelease to dispose of the returned font object when 
	finished with it. 
	@param cosObjP IN/OUT? Cos object for which a PDEFont is created. 
	
	@return The PDEFont created from cosObjP. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@see PDEFontGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDEFontCreateFromCosObj, (
			const CosObj *cosObjP)
	)

/**
	Gets a PDEFont corresponding to a font in the system. 

	Call PDERelease to dispose of the returned font object when 
	finished with it. 

	The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font. 

	If you create a PDEFont that is a subset, call PDEFontSubsetNow 
	on this font afterwards. 

	NOTE: If you have environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the OS fonts. In other 
	words, if you use PDEnumSysFonts to get the system font 
	attributes, not all of the system fonts will necessarily 
	be used to create the PDEFont. 

	NOTE: If you want to use WinAnsiEncoding on UNIX, do not 
	use this method. Use PDEFontCreateFromSysFontWithParams 
	or PDEFontCreateFromSysFontAndEncoding instead. 
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param flags Indicates whether to embed the font and whether 
	to subset the font. Must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags.
	@return The PDEFont corresponding to sysFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFontAndEncoding 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEnumSysFonts 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFont, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags)
	)			
			
/*------------------------------------------------------------------------
	PDEColorSpace methods.
------------------------------------------------------------------------*/

/**
	Creates a new color space object. 

	Call PDERelease to dispose of the returned color space object 
	when finished with it. 
	@param name IN/OUT? The ASAtom for the name of the color space 
	created. The name must be one of the following: DeviceCMYK, 
	DeviceGray, or DeviceRGB. 
	@return The newly created color space object. 
	@exception cosErrExpectedName 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreate 
	@see PDEColorSpaceCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEColorSpaceCreateFromName, (
			IN	ASAtom	name)
	)

/**
	Creates a new color space object from a Cos object. 

	Call PDERelease to dispose of the returned color space object 
	when finished with it. 
	@param cosObjP IN/OUT? Supports all PDF 1.3 color spaces, which 
	include: Device-dependent names: DeviceCMYK, DeviceGray, 
	DeviceN, or DeviceRGB. Device-independent names: CalGray, 
	CalRGB, Lab, or ICCBased. Special names: Indexed, Pattern, 
	or Separation. 
	@return The newly created color space object. 
	@exception cosErrExpectedArray 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreate 
	@see PDEColorSpaceCreateFromName 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEColorSpaceCreateFromCosObj, (
			IN	const CosObj *cosObjP)
	)
/*------------------------------------------------------------------------
	Client Tag methods.
------------------------------------------------------------------------*/

/**
	Adds an identifier-value pair to an object. 

	The clientID-tag combination is a unique identifier for 
	the value. Each client has its own identifier space. It 
	is often convenient to use ASAtoms as tags. 

	NOTE: Tags are a purely memory-resident feature. In addition, 
	management of tags are the responsibility of the client. 
	A client must manage any memory pointed to by a tag. This 
	method only contains a pointer to the data passed in by 
	the client. The data and the pointer will not be saved to 
	a file; the generic pointer type is not in the PDF specification. 
	
	@param object The element to tag. object may be a PDEElement, 
	PDEContent, PDEFont, PDEColorSpace, and so on. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, clientID should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative clientID. 
	(A negative clientID is reserved for the implementation.) 
	
	@param tag The tag to add to object. If tag is 0, this 
	is the same as calling PDERemoveTag. In other words, you 
	cannot tell the difference between a tag whose value is 
	zero and a tag that is nonexistent. 
	@param value Pointer to a value to associate with object. 
	Only the pointer is stored. If the pointer points to data, 
	it is the responsibility of the client to manage the data 
	and its memory.
	@exception peErrWrongPDEObjectType 
	@see PDEGetTag 
	@see PDERemoveTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEAddTag, (
			IN	PDEObject object,
			IN	ASInt32 clientID,
			IN	ASUns32 tag,
			IN	void *value)
	)

/**
	Gets an object's value for a given clientID-tag identifier 
	that was added by PDEAddTag. 
	@param object The element whose value is obtained. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, clientID should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative clientID. 
	(A negative clientID is reserved for the implementation.) 
	
	@param tag The object's tag. If object has no tag, this 
	is 0.
	@return The value associated with the clientID-tag identifier. 
	@exception peErrWrongPDEObjectType 
	@see PDEAddTag 
	@see PDERemoveTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void *, PDEGetTag, (
			IN	PDEObject object,
			IN	ASInt32 clientID,
			IN	ASUns32 tag)
	)

/**
	Removes an object's value for a given clientID-tag identifier 
	that was added by PDEAddTag. 

	If PDEAddTag is called with a 0 tag, this is the same as 
	calling PDERemoveTag. 
	@param object The element whose tag is removed. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, clientID should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative clientID. 
	(A negative clientID is reserved for the implementation.) 
	
	@param tag The tag value.
	@exception peErrWrongPDEObjectType 
	@see PDEAddTag 
	@see PDEGetTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDERemoveTag, (
			IN	PDEObject object,
			IN	ASInt32 clientID,
			IN	ASUns32 tag)
	)

/*------------------------------------------------------------------------
	Utility methods.
------------------------------------------------------------------------*/

/**
	Merges two Resources dictionaries in the same CosDoc - you 
	cannot merge two resource dictionaries from different CosDocs. 
	

	Both dictionaries and what they reference must be in cosDoc. 
	The objects referenced by newResP are appended to resDictP. 
	

	This method only operates on the Cos dictionaries. It assumes 
	there are no resource name conflicts. 

	NOTE: Since PDFEdit resolves resource names across PDEContent 
	objects, this routine is safe for using with PDFEdit methods. 
	This method may be unsafe if you modify streams and dictionaries 
	outside of the PDFEdit API. 

	This method is useful for adding form resources to page 
	resource dictionaries. This is only necessary if creating 
	PDF 1.1 or earlier files for use with Acrobat 2.1 or earlier. 
	This is unnecessary if creating PDF 1.2 or later documents. 
	
	@param resDictP IN/OUT? (Filled by the method) Dictionary to which 
	newResP is merged. When the method completes, resDictP is 
	the merged dictionary result. 
	@param cosDoc IN/OUT? CosDoc containing both dictionaries. 
	@param newResP IN/OUT? Dictionary to merge with resDictP. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEMergeResourcesDict, (
			OUT	CosObj *resDictP,
			IN	CosDoc cosDoc,
			IN	const CosObj *newResP)
	)

/*------------------------------------------------------------------------
	Notification methods.
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
	Methods added after version 0.2
------------------------------------------------------------------------*/

/**
	Creates a new PDEExtGState from a Cos object. See Section 
	4.3.4 in the PDF Reference for more information about extended 
	graphics states. 

	Call PDERelease to dispose of the returned PDEExtGState 
	when finished with it. 
	@param cosObjP A Cos object for a dictionary of type ExtGState.
	@return The PDEExtGState for cosObjP. 
	@see PDEElementSetGState 
	@see PDEExtGStateGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEExtGState, PDEExtGStateCreate, (
			IN CosObj *cosObjP)
	)

/**
	Creates a place object. 

	Call PDERelease to dispose of the returned place object 
	when finished with it. 
	@param mcTag IN/OUT? Tag name for the place. Must not contain any 
	white space characters (for example, spaces or tabs). 
	@param cosObjP IN/OUT? Optional Marked Content dictionary associated 
	with the place. 
	@param isInline If true, emits the place's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the PDF Reference for more details.
	@return The place object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPlace, PDEPlaceCreate, (
			IN ASAtom mcTag,
			IN CosObj *cosObjP,
			IN ASBool isInline)
	)

/**
	Sets the Marked Content tag for a PDEPlace. 
	@param pdePlace IN/OUT? The place whose Marked Content tag is set. 
	
	@param mcTag IN/OUT? The tag for pdePlace. 
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceGetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPlaceSetMCTag, (
			IN	PDEPlace pdePlace,
			IN	ASAtom mcTag)
	)

/** 
	Sets the Marked Content dictionary for a PDEPlace. Dictionary can be 
	emitted inline or referenced from the \ Properties key in the \ Resources 
	dictionary of the containing stream.

	@param pdePlace IN/OUT? The place whose Marked Content dictionary 
	is set. 
	@param placeDictP IN/OUT? Marked Content dictionary for pdePlace. 
	
	@param isInline If true, emits the place's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the PDF Reference for more details. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceGetDict 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPlaceSetDict, (
			IN	PDEPlace pdePlace,
			IN	CosObj *placeDictP,
			IN	ASBool isInline)
	)

/**
	Creates a container object. 

	Call PDERelease to dispose of the returned container object 
	when finished with it. 
	@param mcTag IN/OUT? Tag name for the container. 
	@param cosObjP IN/OUT? Optional Marked Content dictionary for the 
	container. 
	@param isInline If true, emits the container's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name. 
	See the Property Lists section in the PDF Reference for more details.
	@return The newly created container object. 
	@exception pdErrOpNotPermitted 
	@see PDEContainerGetMCTag 
	@see PDEContainerSetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEContainer, PDEContainerCreate, (
			IN ASAtom mcTag,
			IN CosObj *cosObjP,
			IN ASBool isInline)
	)

/**
	Sets the Marked Content tag for a PDEContainer. 
	@param pdeContainer IN/OUT? The container to tag. 
	@param mcTag IN/OUT? Marked Content tag. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerCreate 
	@see PDEContainerGetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContainerSetMCTag, (
			IN	PDEContainer pdeContainer,
			IN	ASAtom mcTag)
	)

/**
	Sets Marked Content dictionary for a PDEContainer. Dictionary can be emitted inline 
	or referenced from the \ Properties key in the \ Resources 
	dictionary of the containing stream.
 
	@param pdeContainer The container whose dictionary is 
	changed. 
	@param placeDictP Marked Content dictionary being set 
	into pdeContainer. 
	@param isInline If true, emits the container's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the PDF Reference for more details.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerGetDict 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEContainerSetDict, (
			IN	PDEContainer pdeContainer,
			IN	CosObj *placeDictP,
			IN	ASBool isInline)
	)

/**
	Sets the content for a container. The existing PDEContent 
	is released by this method. 

	NOTE: This method decrements the reference count of the 
	previous content of the container and increments the reference 
	count of the new PDEContent. 
	@param pdeContainer IN/OUT? A container. 
	@param pdeContent IN/OUT? The content of pdeContainer. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContainerSetContent, (
			IN	PDEContainer pdeContainer,
			IN	PDEContent pdeContent)
	)

/**
	Splits a text run into two text runs. 
	@param pdeText Text object containing a text run to split. 
	
	@param splitLoc Split location, relative to the text object. 
	The first text run is from character index 0 up to splitLoc. 
	The second text run is from splitLoc + 1 to the end of the 
	run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextIsAtPoint 
	@see PDETextReplaceChars 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextSplitRunAt, (
			IN	PDEText 	pdeText, 
			IN	ASInt32 		splitLoc)
	)

/**
	Creates a pattern object that can be used for a Pattern 
	color space. See Section 4.6 in the PDF Reference for more 
	information about patterns. 

	Call PDERelease to dispose of the returned pattern object 
	when finished with it. 
	@param cosObjP IN/OUT? A CosStream for the pattern. 
	@return A pattern. 
	@see PDEPatternGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPattern, PDEPatternCreate, (
			const CosObj *cosObjP)
	)

/**
	Replaces characters in a text object. 

	This method does not change the number of characters in 
	the text object - extra characters are ignored. 
	@param pdeText IN/OUT? Text object in which characters are replaced. 
	
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param textBuffer IN/OUT? Replacement text. 
	@param numChars IN/OUT? Number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd 
	@see PDETextIsAtPoint 
	@see PDETextSplitRunAt 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextReplaceChars, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASUns8 *textBuffer,
			IN	ASInt32 numChars)
	)

/**
	Clears the PDE Cache of this PDDoc. This method is only 
	of interest to clients. 

	NOTE: We do not recommend calling this method directly; 
	it is provided only for backwards compatibility. 
	@param doc A PDDoc whose cache is purged.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEPurgeCache, (
			IN	PDDoc doc)
	)

/**
	Subsets a given PDEFont in a CosDoc. 

	If you created font with PDEFontCreateFromSysFont, you must 
	have set both the kPDEFontCreateEmbedded and kPDEFontWillSubset 
	set in the flags parameter to be able to subset font. 

	NOTE: This method does not change the reference count. 
	@param font IN/OUT? The PDEFont to subset. 
	@param cosDoc IN/OUT? The CosDoc whose font is subsetted. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromSysFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontSubsetNow, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)

/**
	Adds a segment to a path. The number of ASFixed values used 
	depends upon segType:\n
	kPDEMoveTo: x1, y1\n
	kPDELineTo: x1, y1\n
	kPDECurveTo: x1, y1, x2, y2, x3, y3\n
	kPDECurveToV: x1, y1, x2, y2\n
	kPDECurveToY: x1, y1, x2, y2\n
	kPDERect: x1, y1, x2 (width), y2 (height)\n
	kPDEClosePath: None 
	@param path IN/OUT? The path to which a segment is added. 
	@param segType IN/OUT? A PDEPathElementType value indicating the type 
	of path to add. 
	@param x1 IN/OUT? x-coordinate of first point. 
	@param y1 IN/OUT? y-coordinate of first point. 
	@param x2 IN/OUT? x-coordinate of second point. 
	@param y2 IN/OUT? y-coordinate of second point. 
	@param x3 IN/OUT? x-coordinate of third point. 
	@param y3 IN/OUT? y-coordinate of third point. 
	@exception genErrBadParm 
	@see PDEPathSetData 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPathAddSegment, (
			IN	PDEPath path, 
			IN	ASUns32 segType, 
			IN	ASFixed	x1,
			IN	ASFixed	y1,
			IN	ASFixed	x2,
			IN	ASFixed	y2,
			IN	ASFixed	x3,
			IN	ASFixed	y3)
	)

/*------------------------------------------------------------------------
	Methods added after version 0.9
------------------------------------------------------------------------*/

/**
	Creates a new PDEFont from params. 

	The PDEFont may be represented as an embedded font (a FontFile 
	value in PDF). To create a PDEFont that will be stored as 
	an embedded font, the FontFile stream may be passed as fontStm, 
	and the len1, len2, and len3 parameters contain the Length1, 
	Length2, and Length3 values of the FontFile. The caller 
	must close the fontStm after calling this method. This method 
	supports multi-byte fonts. 

	This method extends PDEFontCreate to support multi-byte 
	fonts. 

	Call PDERelease to dispose of the returned font object when 
	finished with it. 
	@param params IN/OUT? Pointer to a structure containing all font 
	parameters necessary to fully define a font. 
	@return A PDEFont object of the font described by the parameters. 
	
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDEFontCreateWithParams, (
			IN	PDEFontCreateParams params)
	)

/**
	Creates a new color space object of the specified type. 
	

	Call PDERelease to dispose of the returned color space object 
	when finished with it. 
	@param family IN/OUT? Supports all PDF 1.3 color spaces, which 
	include: Device-dependent names: DeviceCMYK, DeviceGray, 
	DeviceN, or DeviceRGB. Device-independent names: CalGray, 
	CalRGB, Lab, or ICCBased. Special names: Indexed, Pattern, 
	or Separation. 
	@param csStruct IN/OUT? Data for the type of color space you want 
	to create. 
	@return The newly created color space object. 
	@exception cosErrExpectedArray 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEColorSpaceCreate, (
			ASAtom family, PDEColorSpaceStruct *csStruct)
	)

/**
	Creates an object that can be used to store n color components 
	when in a DeviceN color space. 
	@param pColorValues IN/OUT? Pointer to an array of ASFixed values. 
	
	@param numValues IN/OUT? The length of the array. 
	@return An object containing values specifying a color in a PDEDeviceNColors 
	color space. 
	@exception genErrNoMemory 
	@see PDEDeviceNColorsGetColorValue 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEDeviceNColors, PDEDeviceNColorsCreate, (
			IN ASFixed *pColorValues,
			IN ASInt32 numValues)
	)

/**
	Creates a smooth shading object. 
	@param shadingP IN/OUT? The shading dictionary. 
	@param matrixP IN/OUT? The location and transformation matrix of 
	the shading object. 
	@return A smooth shading object. 
	@exception peErrUnknownPDEColorSpace 
	@exception cosErrInvalidObj 
	@exception cosErrExpectedName 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEShading, PDEShadingCreateFromCosObj, (
			IN	const CosObj *shadingP,
			IN	ASFixedMatrixP matrixP)
	)

/**
	Creates a PDEGroup object. 
	@return The newly created PDEGroup. 
	@see PDEGroupSetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEGroup, PDEGroupCreate, (void))

/**
	Sets the PDEContent for a PDEGroup. The existing PDEContent 
	is released by this method. 

	NOTE: This method increments the reference count of pdeContent. 
	
	@param pdeGroup IN/OUT? A container object. 
	@param pdeContent IN/OUT? The content to set for pdeGroup. 
	@exception peErrWrongPDEObjectType 
	@see PDEGroupGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEGroupSetContent, (
			IN	PDEGroup pdeGroup,
			IN	PDEContent pdeContent)
	)


/**
	Creates a PDEFont corresponding to a font in the system. 
	

	If the font is a multiple master font, mmDesignVector points 
	to the design vector, whose length must equal the number 
	of design axes of the font. 

	Call PDERelease to dispose of the returned font object when 
	finished with it. 

	The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font. 

	If you create a PDEFont that is subsetted, call PDEFontSubsetNow 
	on this font afterwards. 

	NOTE: If you have environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the system fonts. In 
	other words, if you use PDEnumSysFonts to get the system 
	font attributes, not all of the system fonts are necessarily 
	used to create the PDEFont. 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font. 
	@param flags IN/OUT? Indicates whether to embed the font and whether 
	to subset the font. Must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags. 
	@param snapshotName IN/OUT? Name to be associated with this particular 
	instantiation of the PDEFont. 
	@param mmDesignVec IN/OUT? Pointer to multiple master font design 
	vector. 
	@return The PDEFont corresponding to sysFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@see PDEnumSysFonts 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDEFontCreateFromSysFontEx, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags,
			IN	ASAtom snapshotName,
			IN	ASFixed *mmDesignVec)
	)

/**
	Makes a deep copy of a PDEClip object. 

	Call PDERelease to dispose of the returned clip object when 
	finished with it. 
	@param srcClip IN/OUT? The clipping path to copy. 
	@return The deep copy of srcClip. 
	@exception Raises if unable to allocate memory. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEClip, PDEClipCopy, (
			IN	PDEClip srcClip)
	)

/**
	Sets the decode array of an image. 

	Normally, the decode array is accessed through the decode 
	field in the PDEImageAttrs structure. However, this method 
	defines a decode array to handle images with a color space 
	that has more than 4 components. 
	@param image Image whose decode array is set. 
	@param decode Pointer to the decode array. 
	@param decodeSize Number of elements in decode array.
	@see PDEImageGetDecodeArray 
	@see PDEImageGetFilterArray 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEImageSetDecodeArray, (
			IN   PDEImage image,
			IN   ASFixed  *decode,
			IN   ASUns32  decodeSize)
    )

/**
	Adds the specfied PDPage to the PDEContent as an Xobject form.
	Adds a reference to the Xobject form at the indicated index in
	the PDE Content; the index may be < 0, which indicates the object is
	to be appended to the content.
	@param theContent The content to set for page
	@param insertAfterIndex index indicates the location after which the specified
	element is to be added. Index should be kPDBeforeFirst to add to the beginning
	of the display list.
	@param containerDoc CosDoc that the page is contained in to.
	@param srcPage Page that will be inserted at insertAfterIndex in theContent.
	@param dstMatrix (May be NULL) Matrix applied to default matrix of PDPage that is inserted
	in to the CosDoc.
	@param annotTypes If page contains annots, annotTypes list is used to determine
	which annotation types are pumped in to the page contents of the CosDoc.
	@param flags (May be 0) kAnnotAll specifies all annotation types.  If this is not set, then
	the annotTypes list will be consulted.
	@param bbox (May be NULL) specifies the destination BBox for the page being inserted.
	If NULL, the new pages' media box is used.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
XNPROC(void, PDEContentAddPage, (OUT PDEContent theContent, 
			IN	ASInt32 insertAfterIndex, 
			IN	CosDoc containerDoc,
			IN	PDPage srcPage, 
			IN	ASFixedMatrix * dstMatrix,
			IN	ASAtom annotTypes[],
			IN	ASInt32 flags,
			IN	ASFixedRect * bbox)
	  )

/**
	Embeds the given PDEFont inside doc without creating a subset. 
	Use this method instead of PDEFontSubsetNow if you created 
	font with the willSubset flag but changed your mind. 
	@param font The font to embed. 
	@param cosDoc The container document.
	@see PDEFontEmbedNow 
	@see PDEFontIsEmbedded 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontEmbedNowDontSubset, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)

/**
	Gets a Type0 font's width information for only the characters 
	used in the file. Call this routine when the font was created 
	with the kPDEFontDeferWidths flag but without the kPDEFontCreateEmbedded 
	flag (if the font is to be embedded, call PDEFontSubsetNow, 
	which also gets the width info). 
	@param font The font whose widths are found. 
	@param cosDoc The container document.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontGetWidthsNow, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)

/* APIs added for 5.0 start here */

/**
	Creates a new PDEBeginContainer object. 
	@param mcTag IN/OUT? The tag name for the marked-content sequence. 
	
	@param cosObjP IN/OUT? (May be NULL) A CosDict object containing 
	the property list for the sequence. 
	@param isInline If true, emits the container's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the PDF Reference for more details. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEBeginContainer, PDEBeginContainerCreate, (
			IN	ASAtom mcTag,
			IN	CosObj *cosObjP,
			IN	ASBool isInline)
	)

/**
	Sets the marked content tag for a PDEBeginContainer. 
	@param pdeBeginContainer IN/OUT? The PDEBeginContainer object. 
	
	@param mcTag IN/OUT? The tag name. 
	@exception peErrWrongPDEObjectType if pdeBeginContainer is NULL or 
	not the right type. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEBeginContainerSetMCTag, (
			IN	PDEBeginContainer pdeBeginContainer,
			IN	ASAtom mcTag)
	)

/**
	Sets the property list for a PDEBeginContainer. The property 
	list is passed as a Cos dictionary that can be emitted inline 
	or referenced from the \ Properties key in the \ Resources 
	dictionary of the containing stream.

	NOTE: If cosObjP is NULL, the property list is cleared. 
	@param pdeBeginContainer IN/OUT? The PDEBeginContainer object. 
	@param pdeBeginContainerDictP IN/OUT? (May be NULL) The Cos dictionary containing 
	the propertly list. 
	@param isInline If true, emits the container's dictionary 
	into the content stream inline. If false, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the PDF Reference for more details.
	@exception peErrWrongPDEObjectType if pdeBeginContainer is null or 
	not the right type. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEBeginContainerSetDict, (
			IN	PDEBeginContainer pdeBeginContainer,
			IN	CosObj *pdeBeginContainerDictP,
			IN	ASBool isInline)
	)

/**
	Creates a new soft mask object from its Cos representation. 
	
	@param cosObjP IN/OUT? The soft mask dictionary. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreateFromCosObj, (
			IN  const CosObj *cosObjP)
	)

/**
	Creates a new soft mask object. 
	@param cosDoc IN/OUT? The container document. 
	@param type IN/OUT? Specifies how the mask is to be computed. One 
	of the PDESoftMaskCreateFlags. 
	@param pdeForm IN/OUT? The form XObject that defines the soft mask. 
	It is the source of the mask values and the PDColorSpace 
	in which the composite computation is to be done. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreate, (
            IN  CosDoc cosDoc,
            IN  PDESoftMaskCreateFlags type,
            IN  PDEForm pdeForm)
	)

/**
	Sets the PDEForm that defines the soft mask. 
	@param pdeSoftMask IN/OUT? The soft mask object. 
	@param pdeForm IN/OUT? The form XObject. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetXGroup, (
			IN	PDESoftMask pdeSoftMask,
            IN  PDEForm pdeForm)
	)

/**
	Sets the backdrop color values. 
	@param pdeSoftMask IN/OUT? The soft mask object. 
	@param pColorValues IN/OUT? An series of color values. 
	@param numValues IN/OUT? The number of values pointed to by pColorValues. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetBackdropColor, (
			IN	PDESoftMask pdeSoftMask,
			IN	ASFixed *pColorValues, 
			IN  ASInt32 numValues)
	)

/**
	Sets the transfer function associated with the soft mask. 
	
	@param pdeSoftMask IN/OUT? The soft mask object. 
	@param cosTransferFunction IN/OUT? The transfer function dictionary. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetTransferFunction, (
			IN	PDESoftMask pdeSoftMask,
			IN	CosObj cosTransferFunction)
    )

/*------------------------------------------------------------------------
	PDEXGroup Methods added to support Transparency
------------------------------------------------------------------------*/

/**
	Creates a new XGroup object from its Cos representation. 
	
	@param cosObjP IN/OUT? The XGroup object dictionary. 
	@return The PDEXGroup object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEXGroup, PDEXGroupCreateFromCosObj, (
			IN	const CosObj *cosObjP)
	)

/**
	Create a new XGroup of the given type. 
	@param cosDoc The document the object will be created in. 
	
	@param type Must be kPDEXGroupTypeTransparency.
	@return The newly created transparency group object. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( PDEXGroup, PDEXGroupCreate, (
            IN  CosDoc cosDoc,
            IN  PDEXGroupCreateFlags type)
	)

/**
	Sets the knockout value. 
	@param pdeXGroup IN/OUT? The transparency group object. 
	@param knockout IN/OUT? The knockout value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEXGroupSetKnockout, (
			IN  PDEXGroup pdeXGroup,
			IN	ASBool knockout)
	)

/**
	Sets the XGroup to be isolated or not. Corresponds to the 
	/ I key within the XGroup's dictionary. 
	@param pdeXGroup IN/OUT? The transparency group object. 
	@param isolated IN/OUT? true to isolate the XGroup, false otherwise. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEXGroupSetIsolated, (
            IN  PDEXGroup pdeXGroup,
            IN	ASBool isolated)
	)

/**
	Sets the PDEXObject that defines the color space into which 
	colors are converted when painted into this group. 
	@param pdeXGroup The transparency group object. 
	@param pdeColorSpace The color space to associate with 
	the XGroup.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC( void, PDEXGroupSetColorSpace, (
			IN	PDEXGroup pdeXGroup,
			IN  PDEColorSpace pdeColorSpace)
	)

/**
	Sets the transparency group dictionary of the form XObject. 
	
	@param pdeForm IN/OUT? The font XObject. 
	@param pdeXGroup IN/OUT? The transparency dictionary. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEFormSetXGroup, (
			IN	PDEForm pdeForm,
			IN  PDEXGroup pdeXGroup)
	)

/*------------------------------------------------------------------------
	Methods added to support ExtGState
------------------------------------------------------------------------*/

/**
	Creates a new extended graphics state object. 
	@param cosDoc IN/OUT? The document the object will be used within. 
	
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEExtGState, PDEExtGStateCreateNew, (
            IN CosDoc cosDoc)
    )

/**
	Sets the overprint mode. Corresponds to the / OPM key within 
	the ExtGState's dictionary. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param opm IN/OUT? Overprint mode. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPM, (
			IN	PDEExtGState pdeExtGState,
            IN  ASInt32 opm)
    )

/**
	Specifies if overprint is enabled for painting operations 
	other than stroking. Corresponds to the / op key within 
	the ExtGState's dictionary. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param overprint IN/OUT? Pass true to enable; false to disable. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPFill, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool overprint)
    )

/**
	Specifies if overprint is enabled for stroke operations. 
	Corresponds to the / OP key within the ExtGState's dictionary. 
	
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param overprint IN/OUT? Pass true to enable; false to disable. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPStroke, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool overprint)
    )

/**
	Sets the opacity value for painting operations other than 
	stroking. The value must be in the range from 0 to 1 inclusive. 
	Corresponds to the / ca key within the ExtGState's dictionary. 
	The value from 0 to 1 refers to a float number (not an ASFixed 
	value) that should be converted to ASFixed using FloatToASFixed. 
	
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param opacity IN/OUT? The new opacity value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOpacityFill, (
			IN	PDEExtGState pdeExtGState,
            IN  ASFixed opacity)
    )

/**
	Sets the opacity value for stroke operations. The value 
	must be in the range from 0 to 1 inclusive. Corresponds 
	to the / CA key within the ExtGState's dictionary. The value 
	from 0 to 1 refers to a float number (not an ASFixed value) 
	that should be converted to ASFixed using FloatToASFixed. 
	
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param opacity IN/OUT? The new opacity value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOpacityStroke, (
			IN	PDEExtGState pdeExtGState,
            IN  ASFixed opacity)
    )

/**
	Sets the blend mode for the color composites for each object 
	painted. 

	Valid mode names are Compatible, Normal, Multiply, Screen, 
	Difference, Darken, Lighten, ColorDodge, ColorBurn, Exclusion, 
	HardLight, Overlay, SoftLight, Luminosity, Hue, Saturation 
	and Color. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param blendMode IN/OUT? The new blend mode. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetBlendMode, (
			IN	PDEExtGState pdeExtGState,
            IN  ASAtom blendMode)
    )

/**
	Specifies if the alpha is to be interpreted as a shape or 
	opacity mask. 
	@param pdeExtGState The extended graphics state object. 
	
	@param alphaIsShape Indicates whether the sources of alpha 
	are to be treated as shape (true) or opacity (false). This 
	determines the interpretation of the constant alpha (ca 
	or CA) and soft mask (SMask) parameters of the graphics 
	state, as well as a soft-mask image (Smask entry) of an 
	image XObject.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC( void, PDEExtGStateSetAIS, (
			IN	PDEExtGState pdeExtGState,
			IN ASBool alphaIsShape)
    )

/**
	Sets the soft mask of the extended graphics state. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param pdeSoftMask IN/OUT? The soft mask object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetSoftMask, (
			IN	PDEExtGState pdeExtGState,
            IN  PDESoftMask pdeSoftMask)
    )

/*------------------------------------------------------------------------
	PDEImage Methods added to support SMask and Mattes
------------------------------------------------------------------------*/

/**
	Sets the soft mask. 
	@param image IN/OUT? The image XObject. 
	@param sMask IN/OUT? The soft mask. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetSMask, (
            IN	PDEImage image,
            IN	PDEImage sMask)
      )

/**
	Sets the matte array for the image XObject. 
	@param image IN/OUT? The image XObject. 
	@param matte IN/OUT? An array of values. 
	@param numComp IN/OUT? The number of values in mArray. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetMatteArray, (
			IN   PDEImage image,
			IN   ASFixed  *matte,
			IN   ASUns32  numComp)
	)

/**
	Creates a new end container object. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEEndContainer, PDEEndContainerCreate, ()
	)

/**
	Creates a new begin group object. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEBeginGroup, PDEBeginGroupCreate, ()
	)

/**
	Creates a new end group object. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEEndGroup, PDEEndGroupCreate, ()
	)


/**
	Used to obtain a PDEFont corresponding to a font in the 
	system. 

	NOTE: If you want to use WinAnsiEncoding on UNIX, use this 
	method or PDEFontCreateFromSysFontAndEncoding instead. 
	@param sysFont The system font. 
	@param params The parameters structure.
	@return The newly created PDEFont object. 
	@exception peErrCantCreateFontSubset 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontWithParams, (
			IN	PDSysFont sysFont,
			IN	PDEFontCreateFromSysFontParams params)
	)

/**
	Translates a string to Unicode values. The PDEFont must 
	have a / ToUnicode table. 
	@param font IN/OUT? The font. 
	@param text IN/OUT? The string to convert. 
	@param textLen IN/OUT? The length of text, in bytes. 
	@param unicodeStr IN/OUT? (Filled by the method) Buffer to hold 
	the translated string. 
	@param size IN/OUT? The size of the unicodeStr buffer. 
	@return 0 if the string was successfully translated. If unicodeStr 
	is too small for the translated string, it returns the number 
	of bytes required. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(ASUns32, PDEFontTranslateGlyphIdsToUnicode, (
			IN	PDEFont font,
			IN	ASUns8 *text,
			IN	ASUns32 textLen,
			OUT	ASUns8 *unicodeStr,
			IN	ASUns32 size)
	)

/**
	Specifies whether text knockout is enabled in the graphics 
	state. This corresponds to the / TK key in the ExtGState's 
	dictionary. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param bk IN/OUT? Pass true to enable; false to disable. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetTK, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool bk)
    )

/**
	Sets the text state of a text run. 
	@param pdeText Text object containing a text run whose 
	state is set. 
	@param runIndex Index of the text run. 
	@param stateP Pointer to a PDETextState structure with 
	state to set. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetTextState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDETextRunSetState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Create an encoding object from base name. 
	@param baseEncName IN/OUT? The base encoding. See Section 5.5.5 
	in the PDF Reference. 
	@param diffEnc IN/OUT? Array of 256 const char* describing the 
	differences from the encoding specified by baseEncName. 
	May be NULL. 
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(PDSysEncoding, PDSysEncodingCreateFromBaseName, (
			IN	ASAtom			baseEncName,
			IN	const char		**diffEnc)
	)

/**
	Create an encoding object from a PDF CMap name. 
	@param cmapName The CMap name.
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDSysEncoding, PDSysEncodingCreateFromCMapName, (
			IN	ASAtom			cmapName)
	)

/**
	This function returns a createFlags that can be passed to 
	PDEFontCreateFromSysFontAndEncoding. If the combination 
	of sysFont and sysEnc is not allowed, -1 is returned. 
	@param sysFont IN/OUT? An object of type PDSysFont. 
	@param sysEnc IN/OUT? An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(ASInt32, PDSysFontGetCreateFlags, (
			IN	PDSysFont		sysFont,
			IN	PDSysEncoding	sysEnc)
	)

/**
	Similar to PDSysFontGetCreateFlags but avoids compatibility
	issues with changing PDSysFontGetCreateFlags. If the combination
	of sysFont and sysEnc is not allowed, -1 is returned. If the
	combination is ok, then 0 is returned. If the combination only
	works if the font is embedded, kPDEFontCreateEmbedded is returned.
	@param sysFont IN/OUT? An object of type PDSysFont. 
	@param sysEnc IN/OUT? An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000

*/
UNPROC(ASInt32, PDSysFontVerifyEncoding, (
			IN	PDSysFont	sysFont,
			IN	PDSysEncoding	sysEnc)
	)

/**
	Create a PDEFont from sysFont and sysEnc. If it fails, it 
	raises an exception. User can call PDSysFontGetCreateFlags 
	to see if the combination of sysFont and sysEnc makes sense. 
	

	NOTE: If you want to use WinAnsiEncoding on UNIX, use this 
	method or PDEFontCreateFromSysFontWithParams. 
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param sysEnc A PDSysEncoding object. 
	@param useThisBaseFont The base font. An exception will 
	be raised if the base font name passed is a subset name 
	(XXXXXX+FontName) or an empty string. 
	@param createFlags One of the PDEFontCreateFlags.
	@return The newly created PDEFont object. 
	@exception Numerous 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontAndEncoding, (
			IN	PDSysFont		sysFont,
			IN	PDSysEncoding	sysEnc,
			IN	ASAtom			useThisBaseFont,
			IN	ASUns32			createFlags)
	)

/**
	This function returns flags indicating what needs to be 
	done to make PDEFont complete. kPDEFontCreateNeedWidths 
	can be cleared by PDEFontCreateWidthsNow. kPDEFontCreateNeedToUnicode 
	can be cleared by PDEFontCreateToUnicodeNow. kPDEFontCreateNeedEmbed 
	can be cleared by PDEFontEmbedNow. 
	@param font The font object.
	@return A value corresponding to PDEFontCreateNeedFlags. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(ASUns32, PDEFontGetCreateNeedFlags, (
			IN	PDEFont			font)
	)

/**
	This function embeds font stream. User can check the return 
	value of PDEFontGetCreateNeedFlags to see if calling PDEFontEmbedNow 
	is needed. 
	@param font The font to embed. 
	@param cosDoc The container document.
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEFontEmbedNowDontSubset 
	@see PDEFontIsEmbedded 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontEmbedNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)

/**
	This function creates width entries for font. User can check 
	the return value of PDEFontGetCreateNeedFlags to see if 
	calling PDEFontCreateWidthsNow is needed. 
	@param font IN/OUT? The font to create width entries for. 
	@param cosDoc IN/OUT? The container document. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontCreateWidthsNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)

/**
	This function creates the / ToUnicode table. The user can 
	check the return value of PDEFontGetCreateNeedFlags to see 
	if calling PDEFontCreateToUnicodeNow is needed. 
	@param font IN/OUT? An object of type PDEFont. 
	@param cosDoc IN/OUT? The container document. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEFontCreateToUnicodeNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)

/**
	Sets the color space of the image. 
	@param image IN/OUT? Image whose color space is obtained. 
	@param space IN/OUT? An object of type PDEColorSpace. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetColorSpace 
	@see PDEImageSetColorSpace 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetColorSpace, (
			IN	PDEImage			image,
			IN	PDEColorSpace		space)
	)

/**
	Specifies whether stroke adjustment is enabled in the graphics 
	state. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@param strokeAdjust IN/OUT? Pass true to enable; false to disable. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetSA, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool strokeAdjust)
    )

/**
	Create a new soft mask from a name. 
	@param name IN/OUT? The new name for the soft mask. NOTE: Currently, 
	the only valid name is None. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreateFromName, (
			IN  ASAtom name)
	)

/**
	Sets the matrix of a text run. Unlike PDETextRunSetTextMatrix, 
	this function doesn't change fontSize, hScale, and textRise 
	in the textState of PDEText. 
	@param pdeText IN/OUT? Text object containing a text run. 
	@param runIndex IN/OUT? Index of the text run. 
	@param matrixP IN/OUT? ASFixedMatrixP pointer. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetTextMatrix 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDETextRunSetMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)

/* BEGIN Optional Content API calls */

/**
	Associates an optional-content membership dictionary (OCMD) 
	object with the element. The element must be a PDEForm, 
	PDEImage (XObject image), or PDEContainer. If it is not 
	one of these, nothing happens. 
	<ul>
	<li> If the element is a PDEForm or PDEImage, the method attaches 
	the dictionary to the element's Cos XObject dictionary. 
	<li> If the element is a PDEContainer, and it is already for 
	optional content, the optional-content information is replaced. 
	If it is not already for optional content, a new PDEContainer 
	for optional content is created and nested inside the specified 
	container. 
	@param elem The element for which to set the dictionary. 
	
	@param pdOCMD The new dictionary.
	@see PDEElementGetOCMD 
	@see PDEElementRemoveOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC( void, PDEElementSetOCMD, (PDEElement elem, PDOCMD pdOCMD) )

/**
	Dissociates an optional-content membership dictionary (OCMD) 
	object from the element. The element must be a PDEForm, 
	PDEImage (XObject image), or PDEContainer. If it is not 
	one of these, nothing happens. 
	<ul>
	<li> If the element is a PDEForm or PDEImage, the method removes 
	the dictionary from the element's Cos XObject dictionary. 
	<li> If the element is a PDEContainer for optional content, 
	the method removes the dictionary, but does not destroy 
	the container. 
	</ul>
	@param elem The element for which to remove the dictionary.
	@see PDEElementGetOCMD 
	@see PDEElementSetOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC( void, PDEElementRemoveOCMD, (PDEElement elem) )

/**
	Flattens the content, removing any PDEElements that are 
	not visible in the given optional-content context, and removing 
	the optional-content information from any visible PDEElements. 
	
	@param content The content to be modified. 
	@param context The optional-content context in which content 
	is checked for visibility.
	@return true if the operation is successful, false otherwise. 
	@see PDDocFlattenOC 
	@see PDPageFlattenOC 
*/
UNPROC( ASBool, PDEContentFlattenOC, (PDEContent content, PDOCContext context) )
/* END Optional Content API calls */


/**
	Create an encoding object from a code page. 
	@param codePage The code page character-mapping construct. 
	See Code Page Values. 
	@param wMode 0 for horizontal writing, 1 for vertical 
	writiing.
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(PDSysEncoding, PDSysEncodingCreateFromCodePage, (
			IN	ASInt32			codePage,
			IN	ASInt16			wMode)
	)


/**
	Sets the system font object to be used with a font object 
	that does not currently have a system font associated with 
	it. 
	@param pdeFont A PDEFont whose system font is set. 
	@param sysFont The new system font object.
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontGetSysFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFontSetSysFont, (
			IN	PDEFont			pdeFont,
			IN	PDSysFont		sysFont)
	)

/**
	Sets the system encoding object associated with a font object. 
	

	NOTE: Changing the system encoding may produce unexpected 
	results. 
	@param pdeFont A PDEFont whose system encoding is set. 
	
	@param sysEnc The new system encoding object.
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontGetSysEncoding 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFontSetSysEncoding, (
			IN	PDEFont			pdeFont,
			IN	PDSysEncoding	sysEnc)
	)

/**
	Creates a text item element containing a character or text 
	run, which can be added to a PDEText object. 
	@param text Pointer to the characters to add. NOTE: Passing 
	NULL for text can invalidate the text object but will not 
	raise an error. Callers must not pass NULL for this parameter. 
	
	@param textLen Length of the text, in bytes. 
	@param font Font for the element. 
	@param gStateP Pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gStateLen Length of graphics state for the element. 
	
	@param textStateP Pointer to a PDETextState structure with 
	text state for the element. NOTE: PDFEdit ignores the wasSetFlags 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param textStateLen Length of text state for the element. 
	
	@param textMatrixP Pointer to ASFixedMatrix that holds 
	the matrix for the element.
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd 
	@see PDETextAddItem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(PDETextItem, PDETextItemCreate, (
			IN	ASUns8				*text,
			IN	ASUns32				textLen,
			IN	PDEFont				font,
			IN	PDEGraphicStateP	gStateP,
			IN	ASUns32				gStateLen,
			IN	PDETextStateP		textStateP,
			IN	ASUns32				textStateLen,
			IN	ASFixedMatrix		*textMatrixP))

/**
	Sets the font for a text item. 

	NOTE: This method decrements the reference count of the 
	previous font and increments the reference count of the 
	new font. 
	@param textItem Text item whose font is set. 
	@param font The new font object.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetFont, (
			IN	PDETextItem			textItem,
			IN	PDEFont				font))

/**
	Sets the text matrix for a text item. 
	@param textItem The text item whose text matrix is set. 
	
	@param textMatrixP Pointer to a ASFixedMatrix structure 
	with the new text matrix of the text item.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetTextMatrix 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetTextMatrix, (
			IN	PDETextItem			textItem,
			IN	ASFixedMatrix		*textMatrixP))

/**
	Sets the text state for a text item. 
	@param textItem The text item whose text state is set. 
	
	@param textStateP A PDETextStateP structure with the new 
	text state of the text item. 
	@param textStateSize Size of the texStateP structure, 
	in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetTextState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetTextState, (
			IN	PDETextItem			textItem,
			IN	PDETextStateP		textStateP,
			IN	ASUns32				textStateSize))

/**
	Sets the graphics state for a text item. 
	@param textItem Text item whose graphics state is set. 
	@param stateP Pointer to a PDEGraphicStateP structure 
	with graphics state of the text item. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetGState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetGState, (
			IN	PDETextItem			textItem,
			OUT	PDEGraphicStateP	stateP,
			IN	ASUns32				stateSize))

/**
	Replaces all of the text in a text item. 
	@param textItem The text item whose text are replaced. 
	
	@param newText Replacement text. 
	@param newTextLen Number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemRemoveChars 
	@see PDETextItemReplaceChars 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemReplaceText, (
			IN	PDETextItem			textItem,
			IN	ASUns8				*newText,
			IN	ASUns32				newTextLen))

/**
	Replaces characters in a text item. 

	This method does not change the number of characters in 
	the text item - extra characters are ignored. 
	@param textItem The text item whose characters are replaced. 
	
	@param charIndex Index position of the characters to replace. 
	
	@param newChar Replacement text. 
	@param newCharLen Number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemRemoveChars 
	@see PDETextItemReplaceText 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemReplaceChars, (
			IN	PDETextItem			textItem,
			IN	ASUns32				charIndex,
			IN	ASUns8				*newChar,
			IN	ASUns32				newCharLen))

/**
	Removes contiguous characters from a text item. 
	@param textItem The text item whose characters are removed. 
	
	@param charOffset Offset of the first character to remove. 
	
	@param count The number of characters to remove.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemReplaceChars 
	@see PDETextItemReplaceText 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemRemoveChars, (
			IN	PDETextItem			textItem,
			IN	ASUns32				charOffset,
			IN	ASUns32				count))

/**
	Adds a text item to a text element at a given index position. 
	@param text Text object to which the text item is added. 
	@param addIndex Index of the text item in pdeText. 
	@param textItem The text item to add.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextGetItem 
	@see PDETextRemoveItems 
	@see PDETextItemCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextAddItem, (
			IN	PDEText 			text, 
			IN	ASInt32 			addIndex,
			IN	PDETextItem			textItem))

/**
	Removes contiguous text items from a text element starting 
	at a given index position. 
	@param text Text object from which the text items are 
	removed. 
	@param index Index of the first text item in pdeText to 
	remove. 
	@param count The number of text items to remove.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextAddItem 
	@see PDETextGetItem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextRemoveItems, (
			IN	PDEText				 text, 
			IN	ASUns32				index, 
			IN	ASUns32				count))

/******************8 For Newport feature : 513825**********/

/**
	Sets the underlying CosStream of the form using the specified 
	content object. 
	@param form The form whose content is set. 
	@param content The new content for form.
	@exception peErrWrongPDEObjectType 
	@exception peErrPStackUnderflow 
	@see PDEFormGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFormSetContent, (
			IN	PDEForm form,
			IN	PDEContent content)
	)
	
/**
	Creates a new form from an existing form object. Creates 
	a copy of the PDEForm, including the underlying CosStream. 
	
	@param form Form object from which a new PDEForm is created.
	@return The newly created form object. 
	@see PDEFormCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(PDEForm, PDEFormCreateClone, (
			IN PDEForm form)
	)

/**
	Adds Unicode text to a PDEText object. 

	NOTE: This method does not change the reference count of 
	pdeText; however, the reference count of the objects in 
	the gstateP are incremented. 

	@param pdeText Text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. Must be either: kPDETextChar for a text character 
	kPDETextRun for a text run 
	@param index Index after which to add character or text 
	run. 
	@param glyphRun Pointer to a PDEGlyphRun structure
	with Unicode data, GlyphIDs and their correspondence.
	@param font Font for the element. 
	@param gstateP Pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen Length of graphics state for the element.
	@param tstateP Pointer to a PDETextState structure with 
	text state for the element. NOTE: PDFEdit ignores the wasSetFlags 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen Length of text state for the element. 
	@param textMatrixP Pointer to ASFixedMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP Pointer to ASFixedMatrix that holds 
	the matrix for the line width when stroking text. May be 
	NULL. NOTE: Currently this field is not used. (Acrobat 5)
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDETextAddGlyphs, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	PDEGlyphRunP glyphRun,
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASFixedMatrixP textMatrixP,
			IN	ASFixedMatrixP strokeMatrixP)
	)

/**
	Adds Glyphs to a PDEFont object for embedding a PDEFont.

	This is used by clients that use PDEFEdit calls to embed
	the font but create their own content stream. The glyphs
	added by this routine will be included in the font when
	PDEFontSubsetNow is called. It is up to the client to ensure
	that the encoding used by the PDEFont matches the character
	codes used in the string arguments to the text operators in
	the content stream.

	This routine is used to specify which glyphs should be included
	in the font when embedded. Additionally it specifies the mapping
	from the GlyphIDs to Unicode values. This mapping will be used
	to create the ToUnicode entry in the embedded font object.  In
	the cases where the ToUnicode table cannot accurately reproduce
	the Unicode string in the PDEGlyphRun structure, this routine
	will return an array of spans that describe the contents of the
	ActualText spans that must be included in the content stream.
	Each span indicates a contiguous range of glyphs and a
	corresponding contiguous range of Unicode values that correspond
	to the glyphs. For example the following ActualText span
	replace 2 glyphs with 3 Unicode values.

	/Span<</ActualText(U+vvvvU+xxxU+yyyy)>>
		BDC [Giii Gjjj] TJ EMC

NOTE: The routine must be called with the PDEGlyphRuns in display order.

	@param pdeFont Font for the element.

	@param glyphRun Pointer to a PDEGlyphRun structure
	with Unicode data, GlyphIDs and their correspondence. Note that
	the xPosition and yPosition fields in the PDEGlyphDescription
	structure are ignored.

	@param flags  Unused, reserved for later use.

	@return a pointer to a PDESpanSet. The Span can be released with, PDEReleaseSpan

	@exception genErrBadParm
	@see PDEFontSubsetNow
	@see PDEFontCreateFromSysFont
	@see PDEReleaseSpan
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(PDESpanSetP, PDEFontAddGlyphs, (
			IN	PDEFont pdeFont,
			IN	PDEGlyphRunP glyphRun,
			IN	ASUns32 flags)
	)

/**
	Releases a PDESpan object that is returned by PDEFontAddGlyphs

	@exception genErrBadParm
	@see PDEFontAddGlyphs
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEReleaseSpan, (
			IN	PDESpanSetP pdeSpan)
	)

/**
	Sets the page on which marked content is drawn upon for any marked content
	reference handles attached to containers within the content.

	Note: If content is set with PDPageSetPDEContent, PDEFormSetContent or
	PDEGroupSetContent this step occurs automatically.

	Note: This call should only be used when the content is being directly added to a
	page.

	@param pdeContent The content stream whose marked content reference
	handles should be updated.
	@param pageObj The page object upon which contents are drawn.
	@see PDEContentSetContainingStream
	@see PDEContentSetStreamOwner
	@see PDSMCRefCreate
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetPage, (
			IN PDEContent pdeContent,
			IN CosObj pageObj)
	)

/**
	Sets containing stream and owner stream for any marked content reference
	handles attached to containers within the content.

	Note: This call should not be used when the content is being directly added to a
	page.

	Note: If content is set with PDPageSetPDEContent, PDEFormSetContent or
	PDEGroupSetContent this step occurs automatically.

	@param pdeContent The content stream to update marked content references
	within.
	@param containingStm The containing stream object for the content stream.
	@see PDEContentSetPage
	@see PDEContentSetStreamOwner
	@see PDEContentSetStreamOwner
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetContainingStream, (
			IN PDEContent pdeContent,
			IN CosObj containingStm)
	)

/**
	Sets stream owner for any marked content reference handles 
	attached to containers within the content.

	Note: This call should not be used when the content is being directly added to a
	page.

	Note: If content is set with PDPageSetPDEContent, PDEFormSetContent or
	PDEGroupSetContent this step occurs automatically.

	@param pdeContent The content stream to update marked content references
	within.
	@param streamOwner The owner object for any references attached to the content.
	@see PDEContentSetPage
	@see PDEContentSetContainingStream
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetStreamOwner, (
			IN PDEContent pdeContent,
			IN CosObj streamOwner)
	)
