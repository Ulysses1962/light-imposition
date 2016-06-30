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

 PERProcs.h

 - Catalog of functions exported by the PDFEditRead HFT.

*********************************************************************/

/*------------------------------------------------------------------------
	PDFEdit Object Hierarchy.
------------------------------------------------------------------------*/
/*
	PDEObject	- base subclass of all PDFEdit objects
	|	methods: GetType, Acquire, Release, AddTag, GetTag, RemoveTag
	|	
	--->PDEContent		- container of a display list composed of PDEElement's
	|
	|
	--->PDEElement		- base subclass of all PDEElements
	|	|	methods: GetClip, SetClip, etc.
	|	|
	|	--->PDEText			- element representing text
	|	|
	|	--->PDEPath			- element representing a PostScript path
	|	|
	|	--->PDEImage		- element representing an inline or XObject image
	|	|					  XObject images are listed in page XObject resources
	|	|
	|	--->PDEForm			- element representing an XObject Form
	|	|					  Forms are listed in page XObject resources
	|	|
	|	--->PDEPS			- element representing inline or XObject passthrough PS
	|	|					  XObject PS's are listed in page XObject resources
	|	|
	|	--->PDEXObject		- element representing an unknown type of XObject
	|	|
	|	--->PDEPlace		- element representing a place in the display list of 
	|	|					  a Marked Content MP or DP operator.
	|	|
	|	--->PDEContainer	- element representing a container of PDEElements
	|	|					  collected by a Marked Content BMC/DMC and EMC pair.
	|	|
	|   --->PDEGroup		- element representing a group (container) of PDEElements 
	|	
	--->PDEClip			- container of PDEPaths,  PDETexts, PDEContainers, PDEGroups,
	|					  and PDEPlaces describing paths and charpaths; attribute of a 
	|					  PDEElement
	|
	--->PDEFont			- font; part of a PDEText attributes
	|					  fonts are listed in page Font resources
	|
	--->PDEColorSpace	- color space attribute of a PDEElement
	|					  color spaces are listed in page ColorSpace resources					
	|
	--->PDEExtGState	- Extended Graphics State attribute of a PDEElement
						  ExtGStates are listed in page ExtGState resources					

*/

/*------------------------------------------------------------------------
	PDFEdit Public Methods - Generic PDEContent and PDEElement methods.
------------------------------------------------------------------------*/


/**
	Creates a content object from a Cos object. This is the 
	main method for obtaining a PDEContent. 

	Call PDERelease to dispose of the returned content object 
	when finished with it. 
	@param contents IN/OUT? Cos object that is the source for the content. 
	May be a page contents, a Form XObject, a Type 3 font CharProc, 
	or an appearance dictionary for an annotation. 
	@param resources IN/OUT? The object's Resources dictionary. If 
	the Form or Type 3 font or appearance dictionary contains 
	a Resources dictionary, this dictionary must be passed in 
	resources. Otherwise, it must be the page resources object 
	of the page containing the Form or Type 3 font contents 
	object. 
	@return The content from the Cos object. 
	@exception pdErrOpNotPermitted 
	@exception peErrPStackUnderflow 
	@see PDEContentCreate 
	@see PDEContentToCosObj 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(PDEContent, PDEContentCreateFromCosObj, (
			const CosObj *contents,
			const CosObj *resources)
	)

/**
	Gets the attributes of a content. 
	@param pdeContent IN/OUT? A content object. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to a PDEContentAttrs 
	structure containing the attributes of the content. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEContentGetAttrs,(
			IN	PDEContent pdeContent, 
			OUT	PDEContentAttrsP attrsP, 
			IN	ASUns32 attrsSize)
	)

/**
	Gets the number of resources of the specified type and, 
	optionally, pointers to the resource objects. 
	@param pdeContent IN/OUT? A content object. 
	@param type IN/OUT? Type of resources to query or obtain: PDEFont, 
	PDEXGroup, or PDEColorSpace. Must be one of PDEContentGetResourceFlags. 
	
	@param resourcesP IN/OUT? (Filled by the method) If non-NULL, it 
	must point to an array of PDEObject pointers. On return, 
	the array contains pointers to the requested resources. 
	If resourcesP is NULL, only the number of resources of type 
	is returned. NOTE: The object in resourcesP may only be 
	valid for this method. Use PDEAcquire if you need to hold 
	on to the object longer than the scope of resourcesP. 
	@return Number of resources of type returned in resourcesP. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEContentGetResources,(
			IN	PDEContent pdeContent, 
			IN	ASInt32 type,
			OUT	PDEObject *resourcesP)
	)

/**
	Gets the number of elements in a PDEContent. 
	@param pdeContent IN/OUT? A content object. 
	@return Number of elements in pdeContent. 
	@exception peErrWrongPDEObjectType 
	@see PDEContentGetElem 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEContentGetNumElems,(
			IN	PDEContent pdeContent)
	)

/**
	Gets the requested element from a content. 

	NOTE: This method does not change the reference count of 
	the element. 

	NOTE: This method does not copy the element. 
	@param pdeContent IN/OUT? A content object. 
	@param index IN/OUT? Index of element to obtain. 
	@return The requested element. 
	@exception peErrWrongPDEObjectType 
	@see PDEContentGetNumElems 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEElement, PDEContentGetElem,(
			IN	PDEContent pdeContent, 
			IN	ASInt32 index)
	)
/*------------------------------------------------------------------------
	General methods for PDEPath, PDEImage and PDEXObject.
------------------------------------------------------------------------*/


/**
	Gets the bounding box for an element. 

	The returned bounding box is guaranteed to encompass the 
	element, but is not guaranteed to be the smallest box that 
	could contain the element. For example, for an arc, bboxP 
	encloses the Bezier control points - not just the curve 
	itself. 
	@param pdeElement IN/OUT? An element whose bounding box is obtained. 
	
	@param bboxP IN/OUT? (Filled by the method) Pointer to a ASFixedRect 
	structure specifying the bounding box of pdeElement, specified 
	in user space coordinates. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@notify PDEElementGetClip 
	@notify PDEElementGetGState 
	@notify PDEElementGetMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEElementGetBBox, (
			IN	PDEElement pdeElement, 
			OUT	ASFixedRectP bboxP)
	)

/**
	Gets the graphics state information for an element. 

	This method is only valid for PDEForm, PDEImage, PDEPath,and 
	PDEShading elements. 
	@param pdeElement An element whose graphics state is obtained. 
	
	@param stateP (Filled by the method) Pointer to a PDEGraphicStateP 
	structure that contains graphics state information for pdeElement. 
	This PDEGraphicStateP may contain PDEObjects for color spaces 
	or an ExtGState. They are not acquired by this method. NOTE: 
	For a PDEImage, only the ExtGState value is used for images. 
	For indexed images, the fill color space and values are 
	categorized in the PDEImage object. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementSetGState 
	@see PDEElementGetBBox 
	@see PDEElementGetClip 
	@see PDEElementGetMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDEElementGetGState, (
			IN	PDEElement pdeElement, 
			OUT	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Gets the transformation matrix for an element. 

	This matrix provides the transformation from user space 
	to device space for the element. If there is no cm operator 
	(concatmatrix) in the page stream, the matrix is the identity 
	matrix. 

	For the Adobe PDF Library v1, the element may not be a PDEContainer, 
	a PDEGroup, a PDEPlace, or a PDEText. 

	For the Adobe PDF Library v4, the element may not be a PDEText. 
	
	@param pdeElement An element whose transformation matrix 
	is obtained. 
	@param matrixP (Filled by the method) Pointer to ASFixedMatrix 
	that holds a transformation matrix for pdeElement. If pdeElement 
	is a text object, returns the identity matrix.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementSetMatrix 
	@see PDEElementGetBBox 
	@see PDEElementGetGState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDEElementGetMatrix, (
			IN	PDEElement pdeElement, 
			OUT	ASFixedMatrixP matrixP)
	)

/**
	Gets the current clip for an element.  The current clipping path is part
	of the graphics state.  Element types that are not graphics elements (such
	as PDEContainer and PDEPlace) do not have an associated gstate and should
	not be expected to return valid results.

	NOTE: This method does not change the reference count of 
	the clip object. 
	@param pdeElement IN/OUT? An element whose clip is obtained. NOTE: 
	A clip may be shared by many elements. Use care when modifying 
	a clip. Copy it first if you want to modify the clip for 
	a specific element. 
	@return Clip object for pdeElement. 
	@exception peErrWrongPDEObjectType 
	@see PDEElementGetBBox 
	@see PDEElementGetGState 
	@see PDEElementGetMatrix 
	@see PDEElementIsAtRect 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEClip, PDEElementGetClip, (
			IN	PDEElement pdeElement)
	)
				
/*------------------------------------------------------------------------
	PDEText methods.
------------------------------------------------------------------------*/

/* Characters and show strings
** Each PDEtext object contains a set of text which does not 
** contain any intervening non-text objects, such as paths and XObjects.
** All the of text in a text block is subject to the same clip path.
**
** The text may be accessed as show strings (text runs) or as individual
** characters. The matrix, graphics state or text state may be retrieved for
** a text run or for an individual character. 
*/

/* flags and index
** Many of the PDEText methods operate on either a character or 
** a text element (show string). For such methods, a flag and an index
** must be specified. The flag will indicate whether the index is a character
** index or an element index.
*/


/**
	Gets the number of characters in a text object. 
	@param pdeText IN/OUT? Text object whose number of characters is 
	found. 
	@return Total number of characters in pdeText. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetNumRuns 
	@see PDETextGetRunForChar 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextGetNumChars, (
			IN	PDEText pdeText)
	)

/**
	Gets the number of text runs (show strings) in a text object. 
	
	@param pdeText IN/OUT? Text object whose number of text runs is 
	found. 
	@return Number of text runs in pdeText. 
	@exception peErrWrongPDEObjectType 
	@see PDETextGetNumBytes 
	@see PDETextGetRunForChar 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextGetNumRuns, (
			IN	PDEText pdeText)
	)

/**
	Gets the character offset of the first character of the 
	specified text run. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose graphics state is found. 
	@param runIndex IN/OUT? Index of the text run whose first character's 
	index is returned. 
	@return Character offset of the first character of the specified 
	text run in pdeText. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetNumBytes 
	@see PDETextGetNumRuns 
	@see PDETextGetRunForChar 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextRunGetCharOffset, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex)
	)

/**
	Gets the index of the text run that contains the nth character 
	in a text object. 
	@param pdeText IN/OUT? Text object to examine. 
	@param charIndex IN/OUT? Number of the character to find in pdeText. 
	
	@return Index of the text run with the specified character index 
	into pdeText. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextGetRunForChar, (
			IN	PDEText pdeText, 
			IN	ASInt32 charIndex)
	)

/**
	Gets the number of characters in a text run. 
	@param pdeText IN/OUT? Text object containing a text run whose 
	number of characters is found. 
	@param runIndex IN/OUT? Index of the text run whose number of characters 
	is returned. 
	@return Number of characters in the specified text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetNumRuns 
	@see PDETextGetRunForChar 
	@see PDETextRunGetCharOffset 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextRunGetNumChars, (
			IN  PDEText pdeText,
			IN  ASInt32 runIndex)
	)

/**
	Gets the bounding box of a character or a text run. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose bounding box is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param bboxP IN/OUT? (Filled by the method) Pointer to ASFixedRect 
	to set to the bounding box of specified character or text 
	run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDETextGetBBox, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	ASFixedRectP bboxP)
	)

/**
	Gets the graphics state of a character or a text run. 

	NOTE: This method does not increment the reference count 
	of the objects in stateP. 
	@param pdeText Text object containing a character or text 
	run whose graphics state is found. 
	@param flags A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index Index of the character or text run in pdeText. 
	
	@param stateP (Filled by the method) Pointer to a PDEGraphicStateP 
	structure with graphics state of specified character or 
	text run. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetGState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDETextGetGState, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Gets the text state of a character or a text element. 

	NOTE: This function handles only charSpacing, wordSpacing, 
	and renderMode for backward compatibility. For all attributes, 
	use PDETextGetState instead. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose text state is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param stateP IN/OUT? (Filled by the method) Pointer to a PDETextState 
	structure to fill with the text state of the specified character 
	or text run. 
	@param stateSize IN/OUT? Size of the stateP buffer, in bytes. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetState 
	@see PDETextRunSetTextState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDETextGetTextState, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Gets the font for a text character or element. 

	NOTE: This method does not change the reference count of 
	the returned PDEFont. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose font is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@return Font of the specified character or text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextRunSetFont 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDETextGetFont, (
			IN	PDEText pdeText,
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index)
	)

/**
	Gets the matrix of a character or a text run. 
	@param pdeText Text object containing a character or text 
	run whose matrix is found. 
	@param flags A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index Index of the character or text run in pdeText. 
	
	@param matrixP (Filled by the method) Pointer to ASFixedMatrix 
	that holds the matrix of specified character or text run. 
	This is the transformation matrix from user space to the 
	current text space. The h and v values of the matrix indicate 
	the origin of the first character.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextRunSetTextMatrix 
	@see PDETextGetMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDETextGetTextMatrix, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	ASFixedMatrixP matrixP)
	)

/**
	Gets the stroke matrix of a character or a text run. 

	NOTE: Currently this method returns no valid information. 
	(Acrobat 5) 
	@param pdeText Text object containing a character or text 
	run whose stroke matrix is found. 
	@param flags A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index Index of the character or text run in pdeText. 
	
	@param matrixP (Filled by the method) Pointer to ASFixedMatrix 
	that holds the stroke matrix of specified character or text 
	run. This matrix is the transformation for line widths when 
	stroking. The h and v values of the matrix are ignored.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetStrokeMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDETextGetStrokeMatrix, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	ASFixedMatrixP matrixP)
	)

/**
	Gets the advance width of a character or a text element. 
	Advance width is returned in either character space or user 
	space. The advance width is the amount by which the current 
	point advances when the character is drawn. 

	Advance width may be horizontal or vertical, depending on 
	the writing style. Thus advanceP has both a horizontal and 
	vertical component. 
	@param pdeText Text object containing a character or text 
	run whose advance width is found. 
	@param flags A PDETextFlags value that specifies whether 
	index refers to the character offset from the beginning 
	of the text object or the index of the text run in the text 
	object. Must be either:
	<ul>
	<li> kPDETextChar - for a text character
	<li> kPDETextRun - for a text run
	</ul>
	In addition, set the kPDETextPageSpace 
	flag to obtain the advance width in user space. If it is 
	not set, the advance width is in character space. 
	@param index Index of the character or text run in pdeText. 
	
	@param advanceP (Filled by the method) Pointer to a ASFixedPoint 
	value indicating the advance width. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDETextGetAdvanceWidth, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar, kPDETextPageSpace */ 
			IN	ASInt32 index, 
			OUT	ASFixedPointP advanceP)
	)

/**
	Gets the text for a text run or character. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose text is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param textBuffer IN/OUT? (Filled by the method) Text of specified 
	character or text run. textBuffer must be large enough to 
	hold the returned text. If textBuffer is NULL, returns the 
	number of bytes required to hold the data. 
	@return Number of bytes in text run or character. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDETextGetTextMatrix 
	@see PDETextGetTextState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDETextGetText, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	ASUns8 *textBuffer)
	)
/*------------------------------------------------------------------------
	PDEPath methods.
------------------------------------------------------------------------*/

/* PDEPath data format
** 
** A path object consists of the path gstate and the path data.
** The path data is an array of path operators, such as, moveto, lineto,
** and path operands. Each operator is a 32-bit integer value, defined
** by the PDEPathElementType enum. Each operator is followed by its 
** operands, which are zero to three FixedPoint values, which are 
** pairs of 32-bit values, consisting of the x value followed by the y value.
**
** For example, to draw a horizontal line from (100,100) to (200, 100), 
** the data elements would be as follows:
**		0:		kPDEMoveTo
**		1:		100 (x value in Fixed representation)
**		2:		100 (y)
**		3:		kPDELineTo
**		4:		200	(x)
**		5:		100	(y)
**
** A convenient algorithm for accessing the data values in a path is:
**
** 	ASInt32 *pData;
**	ASInt32 *pEnd;
**	Fixed x, y;
**
**	pathLen = PDPathGetData(pdePath, pathData, pathDataSize);
**	pData = pathData;
**	pDend = pData + pathLen/sizeof(ASInt32);
**	while (pData < pEnd)
**	{
**		switch (*pData++)
**		{
**		case kPDEMoveTo:
**			x = *pData++;
**			y = *pData++;
**			(do something with the data)
**			break;
**		case kPDELineTo:
**			(and so on)
**		}
**	}
*/


/**
	Gets the size of the path data and, optionally, the path 
	data. 
	@param path IN/OUT? The path whose data is obtained. 
	@param data IN/OUT? (Filled by the method) Pointer to path data. 
	If data is non-NULL, it contains a variable-sized array 
	of path operators and operands. The format is a 32-bit operator 
	followed by 0 to 3 ASFixedPoint values, depending on the 
	operator. Opcodes are codes for moveto, lineto, curveto, 
	rect, or closepath operators; operands are ASFixedPoint 
	values. If data is NULL, the number of bytes required for 
	data is returned by the method. NOTE: Returns 'raw' path 
	data. If you want the points in page coordinates, concatenate 
	the path data points with the PDEElement matrix obtained 
	from PDEElementGetMatrix. 
	@param dataSize IN/OUT? Specifies the size of the buffer provided 
	in data. If it is less than the length of the path data, 
	the method copies datasize bytes. 
	@return Length of data of path. 
	@exception peErrWrongPDEObjectType 
	@see PDEPathSetData 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASUns32, PDEPathGetData, (
			IN	PDEPath path, 
			OUT	ASInt32 *data,
			IN	ASUns32 dataSize)
	)

/**
	Gets the fill and stroke attributes of a path. 
	@param path The path whose fill and stroke attributes 
	are obtained.
	@return A set of PDEPathOpFlags flags describing fill and stroke 
	attributes. 
	@exception peErrWrongPDEObjectType 
	@see PDEPathSetPaintOp 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(ASUns32, PDEPathGetPaintOp, (
			IN	PDEPath path)
	)
		
/*------------------------------------------------------------------------
	PDEImage methods.
------------------------------------------------------------------------*/

/* Image filtering
** The GetData, SetData, GetDataStm and SetDataStm methods take a flags parameter.
** If flags & kPDEImageEncodedData, then the data is interpreted as being
** encoded for SetData/SetDataStm.
** For XObject images, the flag value may be set to kPDEImageEncodedData when
** calling GetData/GetDataStm. This flag is not allowed when getting 
** inline image data.
*/

/**
	Gets the attributes for an image. 
	@param image IN/OUT? Image whose attributes are obtained. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to PDEImageAttrs 
	structure with attributes of image. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrUnknownPDEColorSpace 
	@see PDEImageGetColorSpace 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@see PDEImageGetDecodeArray 
	@see PDEImageGetFilterArray 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageGetAttrs, (
			IN	PDEImage image, 
			IN	PDEImageAttrsP attrsP, 
			IN	ASUns32 attrsSize)
	)

/**
	Gets the color space object for an image. 

	NOTE: (For the Adobe PDF Library v1 only) If you get the 
	PDEColorSpace for an inline image, then get the CosObj for 
	that color space with PDEColorSpaceGetCosObj, this CosObj 
	is limited. Cos objects that are the result of parsing inline 
	dictionaries in the PDF page contents are a special class 
	of Cos objects. You should never depend on these objects 
	lasting the lifetime of the document. You should extract 
	the information you need from the object immediately and 
	refer to it no further in your code. 

	NOTE: This method does not change the reference count of 
	the returned PDEColorSpace. 
	@param image IN/OUT? Image whose color space is obtained. 
	@return Color space for image. Returns NULL if image is an image 
	mask. 
	@see PDEImageGetAttrs 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@see PDEImageGetDecodeArray 
	@see PDEImageGetFilterArray 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEImageGetColorSpace, (
			IN	PDEImage image)
	)

/**
	Determines if an image is an XObject image. 
	@param image IN/OUT? Image to examine. 
	@return true if the image is an XObject image, false otherwise. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetCosObj 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEImageIsCosObj, (
			IN	PDEImage image)
	)

/**
	Determines if image data is encoded or not. Used only for 
	inline images; not relevant to XObject images. 

	Always returns false for XObject images; XObject image data 
	can be obtained from PDEImageGetData or PDEImageGetDataStm, 
	either encoded or decoded. 

	If an inline image is obtained via the PDEContentCreateFromCosObj 
	or related methods, the inline image data is always decoded. 
	That is, if PDFEdit parses the stream, the data is always 
	decoded. Only if PDEImageCreate is used to explicitly create 
	a new image using encoded data does PDEImageDataIsEncoded 
	return true. 
	@param image IN/OUT? Image to examine. 
	@param encodedLenP IN/OUT? (Filled by the method) Length of the 
	encoded data - if the data is encoded, that is, the method 
	returns true. 
	@return true if PDEImageGetData returns encoded data, false otherwise. 
	Returns false for XObject images. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetData 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEImageDataIsEncoded, (
			IN	PDEImage image,
			OUT	ASUns32 *encodedLenP)
	)

/**
	Gets an image's data. 

	If the image is an XObject image, data is always returned 
	as decoded data. 

	See the note about inline images under PDEImageDataIsEncoded. 
	
	@param image IN/OUT? Image whose data is obtained. 
	@param flags IN/OUT? Unused - must be zero. 
	@param buffer IN/OUT? Image data. If the data is decoded, buffer 
	must be large enough to contain the number of bytes specified 
	in the PDEImageAttrs structure obtained by PDEImageGetAttrs. 
	If the data is encoded, buffer must be large enough to contain 
	the number of bytes in the encodedLenP parameter obtained 
	by PDEImageDataIsEncoded. 
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEImageDataIsEncoded 
	@see PDEImageSetColorSpace 
	@see PDEImageGetAttrs 
	@see PDEImageGetColorSpace 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@see PDEImageGetDecodeArray 
	@see PDEImageGetFilterArray 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageGetData, (
			IN	PDEImage image,
			IN	ASUns32 flags,
			IN	ASUns8 *buffer)
	)

/**
	Gets a data stream for an image. May only be called for 
	XObject images. 

	The caller must dispose of the returned ASStm by calling 
	ASStmClose. 
	@param image IN/OUT? Image whose data stream is obtained. 
	@param flags IN/OUT? PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, data is returned in encoded form. Otherwise, 
	data is decoded. 
	@return Stream for image. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageSetDataStm 
	@see PDEImageGetDataLen 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASStm, PDEImageGetDataStm, (
			IN	PDEImage image,
			IN	ASUns32 flags)
	)

/**
	Gets the length of data for an image. 
	@param image IN/OUT? Image whose data length is obtained. 
	@return Number of bytes of image data, specified by the width, height, 
	bits per component, and color space of the image. 
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetData 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEImageGetDataLen, (
			IN	PDEImage image)
	)

/**
	Gets the filter array for an image. 
	@param image IN/OUT? Image whose filter array is obtained. 
	@param filtersP IN/OUT? (Filled by the method) Pointer to PDEFilterArray 
	structure to fill with the current filter array for the 
	image. filtersP must be large enough to contain all of the 
	elements. May be NULL to obtain the number of filter elements. 
	
	@return Number of filter elements. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetAttrs 
	@see PDEImageGetColorSpace 
	@see PDEImageGetDataLen 
	@see PDEImageGetDecodeArray 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEImageGetFilterArray, (
			IN	PDEImage image,
			OUT	PDEFilterArrayP filtersP)
	)

/**
	Gets a Cos object for an image. 
	@param image IN/OUT? Image whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	image. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageCreateFromCosObj 
	@see PDEImageIsCosObj 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageGetCosObj, (
			IN	PDEImage image,
			OUT	CosObj *cosObjP)
	)
/*------------------------------------------------------------------------
	PDEClip methods.
------------------------------------------------------------------------*/


/**
	Gets the number of top-level elements in a clip object. 
	Top-level elements may be a path or char-path, a marked 
	content container or place, or a group. 

	Paths are represented as PDEPath objects; char-paths are 
	represented as PDEText objects. 
	@param clip IN/OUT? The clip object to examine. 
	@return Number of path and charpath elements in clip. If clip contains 
	PDEGroups, this method returns the top-level PDEPath, PDEText, 
	PDEContainer, PDEGroup, or PDEPlace object. Use PDEClipFlattenedEnumElems 
	to see only the PDEPath and PDEText objects. 

	NOTE: PDEGroup is not a persistent object. You cannot save 
	to PDF and re-get group objects. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipFlattenedEnumElems 
	@see PDEClipGetElem 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEClipGetNumElems, (
			IN	PDEClip clip)
	)

/**
	Gets an element from a clip object. 

	NOTE: This method does not change the reference count of 
	the returned PDEElement. 
	@param clip IN/OUT? The clip object from which an element is obtained. 
	
	@param index IN/OUT? Index of element to get from clip. 
	@return The element from the clip object. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipGetNumElems 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEElement, PDEClipGetElem, (
			IN	PDEClip clip,
			IN	ASInt32 index)
	)
/*------------------------------------------------------------------------
	PDEXObject methods.
------------------------------------------------------------------------*/

/* PDEXObject
** Any unrecognized XObject references in a page stream will be
** provided in the display list as a PDEXObject.
*/

/**
	Gets a Cos object corresponding to a PDEXObject. 
	@param xObject IN/OUT? The PDEXobject whose Cos object is obtained. 
	
	@param cosObjP IN/OUT? (Filled by the method) Cos object for xObject. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEXObjectCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEXObjectGetCosObj, (
			IN	PDEXObject xObject,
			OUT	CosObj *cosObjP)
	)
		
/*------------------------------------------------------------------------
	PDEForm methods.
------------------------------------------------------------------------*/


/**
	Gets a Cos object for a form. 
	@param form IN/OUT? The form whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	form. 
	@exception peErrWrongPDEObjectType 
	@see PDEFormCreateFromCosObj 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEFormGetCosObj, (
			IN	PDEForm form,
			OUT	CosObj *cosObjP)
	)
							
/*------------------------------------------------------------------------
	PDEPS methods. (Passthrough PostScript)
------------------------------------------------------------------------*/


/**
	Returns a PDEPS object's attributes. 
	@param ps IN/OUT? An object of type PDEPS. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to PDEPSAttrs 
	data structure containing the attributes information. 
	@param attrsSize IN/OUT? The size of the attrsP buffer (sizeof( 
	PDEPSAttrs)). 
	@return A pointer to a data structure of type PDEPSAttrs. 
	@see PDEPSCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00020000

*/
UNPROC(void, PDEPSGetAttrs, (
			IN	PDEPS ps, 
			OUT	PDEPSAttrsP attrsP, 
			IN	ASUns32 attrsSize)
	)

/**
	Gets all or part of the image data. 
	@param ps IN/OUT? An object of type PDEPS. 
	@param buffer IN/OUT? (Filled by the method) Receives the data. 
	
	@param bufferSize IN/OUT? Size of the buffer. 
	@param offset IN/OUT? Offset into the source data at which to start 
	filling buffer. 
	@return Returns the number of bytes written into the buffer. If 
	the return value is less than bufferSize, then there is 
	no more data. 
	@see PDEPSSetData 
	@since PI_PDFEDIT_READ_VERSION >= 0x00020000

*/
UNPROC(ASUns32, PDEPSGetData, (
			IN	PDEPS ps, 
			OUT	ASUns8 *buffer, 
			IN	ASUns32 bufferSize, 
			IN	ASInt32 offset)
	)

/**
	Gets a stream for the data. The data in the stream is decoded 
	(no filters). The caller must dispose of the stream. 
	@param ps IN/OUT? An object of type PDEPS. 
	@return An object of type ASStm. 
	@see PDEPSSetDataStm 
	@since PI_PDFEDIT_READ_VERSION >= 0x00020000

*/
UNPROC(ASStm, PDEPSGetDataStm, (
			IN	PDEPS ps)
	)
/*------------------------------------------------------------------------
	PDEFont methods.
------------------------------------------------------------------------*/


/**
	Gets the attributes for a font object.
	Important note:  PDEFontGetAttrs cannot fill in the cantEmbed and protection
	fields.  PDSysFontAttrs can return this	information to you for system fonts.
	NOTE:  PDEFontGetAttrs fills in the fontBBox portion of the PDEFontAttrs as
	ASInt16's, even though the member says it is an ASFixedRect.  Make sure to
	properly convert those values using ASInt16ToFixed so that you get the proper
	ASFixedRect associated with that font.
	@param font IN/OUT? A PDEFont whose attributes are found. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to a PDEFontAttrs 
	structure for the font attributes. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrCantGetAttrs 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontGetNumCodeBytes 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontGetAttrs, (
			IN	PDEFont font,
			OUT	PDEFontAttrsP attrsP,
			IN	ASUns32 attrsSize)
	)

/**
	Gets the widths for a font object. 
	@param font IN/OUT? A PDEFont whose widths are found. 
	@param widthsP IN/OUT? (Filled by the method) Pointer to widths 
	array. widthsP must have room for 256 values. The widths 
	are returned in character space (1000 EM units). An EM is 
	a typographic unit of measurement equal to the size of a 
	font. To convert to text space, divide the value returned 
	by 1000. To convert to user space, multiply the text space 
	value by the font size. 
	@exception peErrCantGetWidths 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateWithParams 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontGetWidths, (
			IN	PDEFont font,
			OUT	ASInt16 *widthsP)
	)

/**
	Gets a Cos object for a PDEFont. 
	@param font IN/OUT? A PDEFont whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) The Cos object corresponding 
	to font. 
	@exception genErrResourceLoadFailed 
	@exception peErrWrongPDEObjectType 
	@see PDEFontCreateFromCosObj 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontGetCosObj, (
			IN	PDEFont font,
			OUT	CosObj *cosObjP)
	)

/*------------------------------------------------------------------------
	PDEColorSpace methods.
------------------------------------------------------------------------*/

/**
	Gets the name of a color space object. 
	@param colorSpace IN/OUT? A color space object. 
	@return The color space object's name. Supports all PDF 1.3 color 
	spaces, which include: Device-dependent names: DeviceCMYK, 
	DeviceGray, DeviceN, or DeviceRGB. Device-independent names: 
	CalGray, CalRGB, Lab, or ICCBased. Special names: Indexed, 
	Pattern, or Separation. 
	@exception peErrUnknownPDEColorSpace 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASAtom, PDEColorSpaceGetName, (
			IN	PDEColorSpace colorSpace)
	)

/**
	Gets the color space object for an image. 

	Use only for images. For image masks, use PDEElementGetGState 
	to obtain color information. 

	NOTE: (For the Adobe PDF Library v1 only) If you get the 
	PDEColorSpace for an inline image, then get the CosObj for 
	that color space with PDEColorSpaceGetCosObj, this CosObj 
	is limited. Cos objects that are the result of parsing inline 
	dictionaries in the PDF page contents are a special class 
	of Cos objects. You should never depend on these objects 
	lasting the lifetime of the document. You should extract 
	the information you need from the object immediately and 
	refer to it no further in your code. 
	@param colorSpace IN/OUT? The color space whose Cos object is obtained. 
	
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	color space. 
	@return Cos object for colorSpace. Any color space that is in the 
	Resources dictionary of the page is returned as a Cos object. 
	

	NOTE: This Cos object is subject to the warning above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEColorSpaceGetCosObj, (
			IN	PDEColorSpace colorSpace,
			OUT	CosObj *cosObjP)
	)

/**
	Calculates the number of components in a color space. 
	@param colorSpace IN/OUT? A color space object. 
	@return Number of components in colorSpace. For: DeviceGray, CalGray, 
	Separation: Returns 1. DeviceRGB, CalRGB: Returns 3. DeviceCMYK, 
	Lab: Returns 4. DeviceN, ICCBased: Returns the number of 
	components dependent on the specific color space object. 
	Indexed: Returns 1. Call PDEColorSpaceGetBaseNumComps to 
	get the number of components in the base color space. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@see PDEColorSpaceGetBaseNumComps 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEColorSpaceGetNumComps, (
			IN	PDEColorSpace colorSpace)
	)

/**
	Gets the name of the base color space. This is a helper 
	routine for indexed color spaces. 

	Call this method to obtain the base color space and color 
	values for an uncolored pattern in PDFEdit. 

	NOTE: The base color values are in the color array in the 
	PDEColorValue field for stroke and fill of a PDEGraphicStateP. 
	Or, they are in the colorObj2 object if the base color space 
	is DeviceN. To get the color values, a client gets the base 
	color space, determines the type and number of components 
	of the value, and looks them up in the PDEColorValue field. 
	
	@param colorSpace The base color space.
	@return The ASAtom for the name of the base color space. Use ASAtomGetString 
	to obtain a C string for the ASAtom. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@see PDEColorSpaceGetBaseNumComps 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(ASAtom, PDEColorSpaceGetBase, (
			IN	PDEColorSpace colorSpace)
	)

/**
	Gets the highest index for the color lookup table for an 
	indexed color space. Since the color table is indexed from 
	zero to hiVal, the actual number of entries is hiVal + 1. 
	
	@param colorSpace IN/OUT? An indexed color space. 
	@return The highest index (hiVal) in the color lookup table. 
	@exception peErrUnknownPDEColorSpace 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEColorSpaceGetHiVal, (
			IN	PDEColorSpace colorSpace)
	)

/**
	Gets the component information for an indexed color space. 
	
	@param colorSpace IN/OUT? The color space whose component information 
	table is obtained. 
	@param colorTableP IN/OUT? (Filled by the method) The color lookup 
	table, which is numComps * (hiVal + 1) bytes long, where 
	numComps = number of components in the base colorSpace. 
	Each entry in the table contains numComps bytes, and the 
	table is indexed 0 to hiVal, where hiVal is the highest 
	index in the color table. The table is indexed from 0 to 
	hival, thus the table contains hival + 1 entries. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEColorSpaceGetCTable, (
			IN	PDEColorSpace colorSpace,
			OUT	ASUns8 *colorTableP)
	)

/*------------------------------------------------------------------------
	Get Type.
------------------------------------------------------------------------*/

/**
	Gets the type of an element. 
	@param obj IN/OUT? The element whose type is obtained. 
	@return The object type, which is one of PDEType. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEObjectGetType, (
			IN	PDEObject obj)
	)
	
/*------------------------------------------------------------------------
	Acquire and Release methods.
------------------------------------------------------------------------*/

/**
	Increments the reference count for an object. 
	@param obj IN/OUT? The element whose count is incremented. 
	@exception peErrWrongPDEObjectType 
	@see PDERelease 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEAcquire, (
			IN	PDEObject obj)
	)

/**
	Decrements the reference count for the object. If the count 
	becomes zero, the object is destroyed. 

	Do not call PDERelease on PDEContent that you acquired with 
	PDPageAcquirePDEContent; call PDPageReleasePDEContent instead. 
	

	NOTE: Objects should only be disposed of with PDERelease 
	if the method by which they were obtained incremented the 
	reference count for the object. In general, methods that 
	'get' an object do not increment the reference count. Methods 
	that increment the reference count typically contain the 
	word 'acquire' or 'create' in the method name and specifically 
	state that you must release the object. 
	@param obj IN/OUT? The element released. 
	@see PDEAcquire 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDERelease, (
			IN	PDEObject obj)
	)

/*------------------------------------------------------------------------
	Dump methods.
------------------------------------------------------------------------*/

/**
	The object, its children and attributes are dumped. The 
	dump contains information about each individual object. 
	The output for child elements is indented with respect to 
	their parents. 
	<ul>
	<li> The information for each object is char* - The string 
	describing Object Type. (See PDEObjectGetType.) 
	<li> The number representing Object Type. (See PEExpT.h PDEType 
	enum.)
	<li> The object reference count.
	<li> The memory location for the object. 
	</ul>
	@param obj The PDEObject to dump. 
	@param levels Depth of children to dump. 
	@param proc Callback with the dump information; it may 
	be called more than once per object. 
	@param clientData Provided by the caller as the parameter 
	of the same name for proc.
	@exception genErrBadParm 
	@see PDELogDump 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDEObjectDump, (
			IN	PDEObject obj,
			IN	ASInt32 levels,
			IN	PDEObjectDumpProc proc,
			IN	void *clientData)
	)

/**
	Enumerates the PDEObjects. This is useful when looking for 
	orphaned objects. 
	@param proc Callback to call once for each PDEObject. 
	
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@exception peErrWrongPDEObjectType 
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@see PDEObjectDump 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void, PDELogDump, (
		IN	PDEObjectDumpProc proc,
		IN	void *clientData)
	)

/**
	Enumerates the table of attributes. This method enumerates 
	the shared resource objects. It is useful when looking for 
	orphaned attributes. 
	@param enumProc IN/OUT? Callback to call for each attribute. 
	@param clientData IN/OUT? Pointer to user-supplied data to pass 
	to enumProc each time it is called. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEAttrEnumTable, (
			IN	PDEAttrEnumProc enumProc,
			IN	void *clientData)
	)
/*------------------------------------------------------------------------
	Methods added after version 0.2
------------------------------------------------------------------------*/


/**
	Gets a Cos object for a PDEExtGState. 
	@param extGState IN/OUT? A PDEExtGState whose Cos object is obtained. 
	
	@param cosObjP IN/OUT? (Filled by the method) Cos object for extGState. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEExtGStateCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEExtGStateGetCosObj, (
			IN PDEExtGState extGState,
			OUT CosObj *cosObjP)
	)

/**
	Gets the quad bounding the specified text run or character. 
	

	The advance portion of the quad is based on the left side 
	bearing and advance width. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose quad is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run In addition, if the kPDETextBounding flag 
	is set, PDETextGetQuad uses the font descriptor's FontBBox, 
	which is the smallest rectangle that encloses all characters 
	in the font. The advance portion is based on the x-coordinates 
	of the left and right sides of FontBBox and the advance 
	width. 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param quadP IN/OUT? (Filled by the method) Pointer to ASFixedQuad 
	that bounds the specified character or text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDETextGetQuad, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar, kPDETextBounding */ 
			IN	ASInt32 index, 
			OUT	ASFixedQuadP quadP)
	)

/**
	Gets the Marked Content tag for a PDEPlace. 
	@param pdePlace IN/OUT? The place whose Marked Content tag is obtained. 
	
	@return Tag for pdePlace. 
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceSetMCTag 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASAtom, PDEPlaceGetMCTag, (
			IN	PDEPlace pdePlace)
	)

/**
	Gets the Marked Content dictionary for a PDEPlace. 
	@param pdePlace IN/OUT? The place whose Marked Content dictionary 
	is obtained. 
	@param placeDictP IN/OUT? (Filled by the method) Pointer to the 
	Marked Content dictionary; may be NULL. 
	@param isInline IN/OUT? (Filled by the method) If true, the Marked 
	Content dictionary is inline; may be NULL. 
	@return true if dictionary is obtained, false if no dictionary is 
	present. 
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceSetDict 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEPlaceGetDict, (
			IN	PDEPlace pdePlace,
			OUT	CosObj *placeDictP,
			OUT ASBool *isInline)
	)

/**
	Gets the Marked Content tag for a container. 
	@param pdeContainer IN/OUT? A container. 
	@return Marked Content tag of pdeContainer. Returns ASAtomNull if 
	pdeContainer has no Marked Content tag. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerCreate 
	@see PDEContainerSetMCTag 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASAtom, PDEContainerGetMCTag, (
			IN	PDEContainer pdeContainer)
	)

/**
	Gets the Marked Content dictionary for a container. 
	@param pdeContainer IN/OUT? A container. 
	@param placeDictP IN/OUT? (Filled by the method) Marked Content 
	dictionary for pdeContainer. NULL if pdeContainer has no 
	Marked Content dictionary. 
	@param isInline IN/OUT? (Filled by the method) true if the dictionary 
	is inline, false otherwise. Undefined if pdeContainer has 
	no Marked Content dictionary. 
	@return true if pdeContainer has a Marked Content dictionary, false 
	otherwise. 
	@exception peErrWrongPDEObjectType 
	@exception cosErrInvalidObj 
	@see PDEContainerSetDict 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEContainerGetDict, (
			IN	PDEContainer pdeContainer,
			OUT	CosObj *placeDictP,
			OUT ASBool *isInline)
	)

/**
	Gets the PDEContent for a PDEContainer. 

	NOTE: This method does not change the reference count of 
	the returned PDEContent. 
	@param pdeContainer IN/OUT? The container whose content is obtained. 
	
	@return The PDEContent for the pdeContainer. 
	@exception pdErrOpNotPermitted 
	@exception peErrWrongPDEObjectType 
	@see PDEContainerSetContent 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEContent, PDEContainerGetContent, (
			IN	PDEContainer pdeContainer)
	)

/**
	Gets the number of components in the base color space of 
	an indexed color space. 

	For example, for [/ Indexed / DeviceRGB...], the number 
	of components is 3. 
	@param colorSpace IN/OUT? The indexed color space. 
	@return Number of components in colorSpace. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@see PDEColorSpaceGetBase 
	@see PDEColorSpaceGetNumComps 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt32, PDEColorSpaceGetBaseNumComps, (
			IN	PDEColorSpace colorSpace)
	)

/**
	Fills out a PDEGraphicStateP structure with the default 
	graphic state. 

	NOTE: Non-NULL objects in the graphic state, such as the 
	fill and stroke color spaces, have their reference counts 
	incremented by this method. Be sure to release these non- 
	NULL objects when disposing of stateP. 
	@param stateP (Filled by the method) Pointer to a PDEGraphicStateP 
	structure with the default graphic state. 
	@param stateSize Size of the stateP structure, in bytes.
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(void , PDEDefaultGState, (
		OUT	PDEGraphicStateP stateP,
		IN	ASInt32 stateSize)
	)

/**
	Gets a Cos object corresponding to a pattern object. 
	@param pattern IN/OUT? Pattern whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	pattern. 
	@see PDEPatternCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEPatternGetCosObj, (
			IN	PDEPattern pattern,
			OUT CosObj *cosObjP)
	)

/**
	Enumerates all the PDEElements in a given stream. Similar 
	to PDEContentCreateFromCosObj, but provides enumeration 
	instead of a list of elements. 

	If marked content is not ignored, each PDEContainer contains 
	a PDEContent list within itself. 
	@param contents IN/OUT? Cos object that is the source for the content 
	stream. May be page contents, a Form XObject, a Type 3 font 
	CharProc, or an appearance object from an annotation. 
	@param resources IN/OUT? The object's Resources dictionary. If 
	the Form or Type 3 font or appearance dictionary contains 
	a Resources dictionary, this dictionary must be passed in 
	resources. Otherwise, it must be the page resources object 
	of the page containing the Form or Type 3 font contents 
	object. 
	@param flags IN/OUT? Flags from PDEEnumElementsFlags. 
	@param enumProc IN/OUT? User-supplied callback to call once for 
	each top-level element. NOTE: The element in the enumProc 
	may only be valid for this method. Use PDEAcquire if you 
	need to hold on to the element longer than the scope of 
	enumProc. 
	@param enumProcClientData IN/OUT? Pointer to user-supplied data 
	to pass to enumProc each time it is called. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception peErrPStackUnderflow 
	@exception peErrCantGetImageDict 
	@see PDEContentCreateFromCosObj 
	@see PDEContentGetNumElems 
	@see PDEContentGetElem 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
NPROC (void, PDEEnumElements, (
			IN const CosObj *contents, 
			IN const CosObj *resources,
			IN ASUns32 flags,
			IN PDEElementEnumProc enumProc,
			IN void *enumProcClientData)
	)

/*------------------------------------------------------------------------
	Methods added after version 0.7
------------------------------------------------------------------------*/

/**
	Gets the sum to the widths of len characters from a string 
	of single or multi-byte characters. 
	@param font A PDEFont object returned from one of the 
	PDEFontCreate methods. 
	@param text Pointer into a string of characters. 
	@param len Number of characters in the string.
	@return Width of text string in EM space. (In EM space, the width 
	of 'M' is about 1000 EM units). 
	@exception genErrNoMemory 
	@exception pdErrBadResMetrics 
	@exception genErrResourceLoadFailed 
	@exception peErrWrongPDEObjectType 
	@see PDEFontGetNumCodeBytes 
	@see PDEFontIsMultiByte 
	@see PDEFontGetOneByteEncoding 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(ASInt32, PDEFontSumWidths, (
			IN	PDEFont font,
			IN  ASUns8 *text,
			IN	ASInt32 len)
	)

/**
	Gets the number of bytes comprising the next code in a string 
	of single or multi-byte character codes. 
	@param font IN/OUT? A PDEFont object returned from one of the PDEFontCreate 
	methods. 
	@param text IN/OUT? Pointer into a string of characters. 
	@param len IN/OUT? The length, in bytes, of the string of characters, 
	starting with the character pointed to by text. 
	@return Number of bytes in the next character code pointed to by 
	text. 
	@exception genErrNoMemory 
	@see PDEFontIsMultiByte 
	@see PDEFontSumWidths 
	@see PDEFontGetOneByteEncoding 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASInt16, PDEFontGetNumCodeBytes, (
			IN	PDEFont font,
			IN  ASUns8 *text,
			IN	ASInt32 len)
	)

/**
	Gets the value of a color component of a PDEDeviceNColors 
	color space. 
	@param colors IN/OUT? A PDEDeviceNColors object returned by PDEDeviceNColorsCreate. 
	
	@param index IN/OUT? Index of the color component to return. 
	@return The value of the requested color component. 
	@exception genErrBadParm 
	@see PDEDeviceNColorsCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASFixed, PDEDeviceNColorsGetColorValue, (
			IN  PDEDeviceNColors colors,
			IN  ASInt32 index)
	)

/**
	Tests whether a font contains any multi-byte characters. 
	
	@param font IN/OUT? A PDEFont object returned from one of the PDEFontCreate 
	methods to test. 
	@return true if the font contains any multi-byte characters, false 
	otherwise. 
	@see PDEFontGetNumCodeBytes 
	@see PDEFontSumWidths 
	@see PDEFontGetOneByteEncoding 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
NPROC (ASBool, PDEFontIsMultiByte, (
			IN PDEFont font)
	)

/**
	Gets the number of bytes occupied by the character code 
	or text run. 
	@param pdeText IN/OUT? A PDEText object returned from one of the 
	PDETextCreate methods whose text is examined. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@return Number of bytes occupied by the text run or character. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEFontGetNumCodeBytes 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
NPROC (ASInt32, PDETextGetNumBytes, (
			IN PDEText pdeText,
			IN ASUns32 flags,	/* kPDETextRun or kPDETextChar */
			IN ASInt32 index)
	)

/**
	Gets the PDEContent for a PDEGroup. 

	NOTE: This method does not change the reference count of 
	the returned PDEContent. 
	@param pdeGroup IN/OUT? The group whose content is obtained. 
	@return The PDEContent in pdeGroup. 
	@exception peErrWrongPDEObjectType 
	@see PDEGroupSetContent 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEContent, PDEGroupGetContent, (
			IN	PDEGroup pdeGroup)
	)

/**
	For a given PDEClip, enumerates all of the PDEElements in 
	a flattened manner. In other words, PDEContainers and PDEGroups 
	nested in the PDEClip will not be handed back, but any PDEPaths 
	and PDETexts nested in them will be. Additionally, PDEPlace 
	objects inside the PDEClip are not returned. 
	@param clip The PDEClip to enumerate. 
	@param enumProc Called with each flattened element. Enumeration 
	continues until all elements have been enumerated, or until 
	enumProc returns false. 
	@param enumProcClientData Pointer to user-supplied data 
	to pass to enumProc each time it is called.
	@return Returns value of enumProc. true if successful, false otherwise. 
	
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipCreate 
	@see PDEClipGetElem 
	@see PDEClipGetNumElems 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(ASBool, PDEClipFlattenedEnumElems, (
			IN	PDEClip clip,
			IN	PDEClipEnumProc enumProc,
			IN	void *enumProcClientData)
	)

/**
	Tests whether a point is on an element. 
	@param elem IN/OUT? The element to test. If PDEElement is a PDEText 
	or PDEImage, it uses the bounding box of the PDEElement 
	to make the check. If the PDEElement is a PDEPath and it 
	is stroked, it checks if the point is on the path. If the 
	PDEElement is a PDEPath and it is filled, it checks if the 
	point is in the fill area, taking into consideration whether 
	it is filled using the non-zero winding number rule or the 
	even-odd rule. 
	@param point IN/OUT? The point, specified in user space coordinates. 
	
	@return true if the point is on the element, false otherwise. 
	@exception peErrWrongPDEObjectType 
	@see PDEElementIsAtRect 
	@see PDETextIsAtPoint 
	@see PDETextIsAtRect 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEElementIsAtPoint, (
			IN	PDEElement elem, 
			IN	ASFixedPoint point)
	)

/**
	Tests whether any part of a rectangle is on an element. 
	
	@param elem IN/OUT? The element to test. If PDEElement is a PDEText 
	or PDEImage, it uses the bounding box of the PDEElement 
	to make the check. If the PDEElement is a PDEPath and it 
	is stroked, it checks if the rectangle is on the path. If 
	the PDEElement is a PDEPath and it is filled, it checks 
	if the rectangle is in the fill area, taking into consideration 
	whether it is filled using the non-zero winding number rule 
	or the even-odd rule. 
	@param rect IN/OUT? The rectangle, specified in user space coordinates. 
	
	@return true if any part of the rectangle is on the element, false 
	otherwise. 
	@exception peErrWrongPDEObjectType 
	@see PDEElementIsAtPoint 
	@see PDETextIsAtPoint 
	@see PDETextIsAtRect 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEElementIsAtRect, (
			IN	PDEElement elem, 
			IN	ASFixedRect rect)
	)

/**
	Tests whether a point is on specified text. Checks if the 
	point is in a bounding box for the PDEText. 
	@param pdeText IN/OUT? The text to test. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param point IN/OUT? The point, specified in user space coordinates. 
	
	@return true if the point is on the text, false otherwise. 
	@see PDEElementIsAtPoint 
	@see PDEElementIsAtRect 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDETextIsAtPoint, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASFixedPoint point)
	)

/**
	Tests whether any part of a rectangle is on the specified 
	text. 
	@param pdeText IN/OUT? The text to test. 
	@param flags IN/OUT? A PDETextFlags flag that specifies whether 
	index refers to the character offset from the beginning 
	of the text object or the index of the text run in the text 
	object. Must be either: kPDETextChar - for a text character 
	kPDETextRun - for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param rect IN/OUT? The rectangle, specified in user space coordinates. 
	
	@return true if the text is on the rectangle, false otherwise. 
	@see PDEElementIsAtPoint 
	@see PDEElementIsAtRect 
	@see PDETextIsAtPoint 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDETextIsAtRect, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASFixedRect rect)
	)

/**
	Gets an array of delta encodings for the given one byte 
	PDEFont. 
	@param font IN/OUT? A PDEFont object returned from one of the PDEFontCreate 
	methods. 
	@param encodingDelta IN/OUT? (Filled by the method) Pointer to 
	an ASAtom array that is filled with the delta encodings 
	for font. Each entry is the ASAtom for a glyph name that 
	differs from the base encoding. See Section 5.5.5 in the 
	PDF Reference for more information about font encodings. 
	The array must be allocated to hold 256 entries. 
	@return true if encodingDelta is filled, false otherwise. 
	@exception genErrNoMemory 
	@see PDEFontIsMultiByte 
	@see PDEFontSumWidths 
	@see PDEFontGetNumCodeBytes 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(ASBool, PDEFontGetOneByteEncoding, (
			IN PDEFont font, 
			OUT ASAtom *encodingDelta)
	)


/**
	Gets the CosObj for a PDEShading. 
	@param shading IN/OUT? A smooth shading object. 
	@param cosObjP IN/OUT? The Cos dictionary corresponding to shading. 
	
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(void, PDEShadingGetCosObj, (
			IN	PDEShading shading,
			OUT CosObj *cosObjP)
	)

/**
	Gets the operator name of an unknown operator. 
	@param pdeUnknown IN/OUT? Unknown element whose operator name is 
	obtained. 
	@return An ASAtom for the name of the operator for pdeUnknown. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(ASAtom, PDEUnknownGetOpName, (
			IN	PDEUnknown pdeUnknown)
	)

/**
	Gets a default color space from a PDEContent. 

	See Section 4.5.4 in the PDF Reference for more information 
	about default color spaces. 

	NOTE: This method does not change the reference count of 
	the returned PDEColorSpace. 
	@param pdeContent IN/OUT? A content object. 
	@param colorSpaceName IN/OUT? An ASAtom for the name of the desired 
	color space. Must be an ASAtom for one of DefaultRGB, DefaultCMYK, 
	or DefaultGray. 
	@return The desired color space in pdeContent. Returns NULL if colorSpaceName 
	does not correspond to a known default, such as DefaultRGB. 
	
	@see PDEContentGetNumElems 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEContentGetDefaultColorSpace, (
			IN	PDEContent pdeContent, 
            IN  ASAtom colorSpaceName)
    )

/**
	Gets the decode array from the attributes of the image. 
	This array specifies the parameters used with the array 
	of filters used to decode the image.  This should be called
	first with a NULL decode to obtain the number of elements that
	may be returned so that a properly sized array can be allocated 
	for a subsequent call.
	There are two decode entries per colorant in normal use.
	@param image The image whose decode array is obtained. 
	
	@param decode (Filled by the method) Pointer to the decode 
	array. If NULL, the number of decode elements required is 
	returned. 
	@param decodeSize Number of elements in decode.
	@return Number of elements in the decode array. 
	@see PDEImageSetDecodeArray 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/
UNPROC(ASUns32, PDEImageGetDecodeArray, (
              IN   PDEImage image,
              OUT  ASFixed  *decode,
              IN   ASUns32  decodeSize)
    )
/* APIs added for 5.0 start here */

/**
	Gets the marked content tag associated with a PDEBeginContainer 
	object. 
	@param pdeBeginContainer IN/OUT? A PDEBeginContainer object. 
	@return The mark content tag. 
	@exception peErrWrongPDEObjectType if pdeBeginContainer is null or 
	not the right type. 
	@see PDEBeginContainerSetMCTag 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASAtom, PDEBeginContainerGetMCTag, (
			IN	PDEBeginContainer pdeBeginContainer)
	)

/**
	Gets the property list dictionary associated with a PDEBeginContainer 
	object. The property list is stored in a Cos dictionary. 
	

	NOTE: Either dictP or isInlineP may be NULL is that information 
	is not required. 
	@param pdeBeginContainer IN/OUT? A PDEBeginContainer object. 
	@param dictP IN/OUT? (Filled by the method) The property list associated 
	with the PDEBeginContainer. 
	@param isInlineP IN/OUT? (Filled by the method) If true the dictionary 
	is emitted into the page content stream inline. 
	@return true if dictP points to a Cos dictionary; false otherwise. 
	
	@exception peErrWrongPDEObjectType if pdeBeginContainer is null or 
	not the right type. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASBool, PDEBeginContainerGetDict, (
			IN	PDEBeginContainer pdeBeginContainer,
			OUT	CosObj *dictP,
			OUT	ASBool *isInlineP)
	)

/*------------------------------------------------------------------------
	PDESoftMask Methods added to support Transparency
------------------------------------------------------------------------*/

/**
	Gets the associated CosObj of the soft mask. 
	@param pdeSoftMask IN/OUT? The soft mask. 
	@param cosObjP IN/OUT? (Filled by the method) A pointer to the 
	Cos object. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskGetCosObj, (
            IN  PDESoftMask pdeSoftMask,
            OUT	CosObj *cosObjP)
	)

/**
	Acquire sthe PDEForm that defines the soft mask. 
	@param pdeSoftMask IN/OUT? An object of type PDESoftMask. 
	@param matrixP IN/OUT? Matrix defining the transformation from 
	coordinate space to user space. 
	@return The XObject form of the soft mask. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( PDEForm, PDESoftMaskAcquireForm, (
			IN	PDESoftMask pdeSoftMask,
			IN	ASFixedMatrixP matrixP)
	)

/**
	Gets the array of color values of the backdrop color. Given 
	a pointer to an array and the length of the array, copies 
	the color values to that array and returns the number of 
	values copied. If the pointer to the array is NULL, the 
	number of color values is returned. 
	@param pdeSoftMask IN/OUT? An object of type PDESoftMask. 
	@param pColorValues IN/OUT? (Filled by the method) Pointer to an 
	array of color values. If NULL , the number of color values 
	is returned. 
	@param numValues IN/OUT? Length of the array pColorValues. 
	@return Number of values copied. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASInt32, PDESoftMaskGetBackdropColor, (
			IN	PDESoftMask pdeSoftMask,
			IN	ASFixed *pColorValues, 
            IN  ASInt32 numValues)
       )

/**
	Gets the transfer function as a CosObj. 
	@param pdeSoftMask IN/OUT? The soft mask. 
	@return The transfer function as a CosObj. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( CosObj, PDESoftMaskGetTransferFunction, (
			IN	PDESoftMask pdeSoftMask)
       )

/*------------------------------------------------------------------------
	PDEXGroup Methods added to support Transparency
------------------------------------------------------------------------*/

/**
	Gets the CosObj of the transparency group. 
	@param pdeXGroup The transparency group object. 
	@param cosObjP (Filled by the method) Pointer to the Cos 
	object.
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( void, PDEXGroupGetCosObj, (
			IN	PDEXGroup pdeXGroup,
			OUT	CosObj *cosObjP)
	)


/**
	Gets the knockout boolean value of the transparency group. 
	
	@param pdeXGroup The transparency group object.
	@return The knockout value. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( ASBool, PDEXGroupGetKnockout, (
            IN  PDEXGroup pdeXGroup)
	)

/**
	Gets the isolated boolean value of the transparency group. 
	
	@param pdeXGroup The transparency group object.
	@return true if the transparency group is isolated; false otherwise. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( ASBool, PDEXGroupGetIsolated, (
            IN  PDEXGroup pdeXGroup)
	)

/**
	Acquires the color space of the transparency group. 
	@param pdeXGroup The transparency group object.
	@return The color space; otherwise NULL. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( PDEColorSpace, PDEXGroupAcquireColorSpace, (
			IN	PDEXGroup pdeXGroup)
	)

/**
	Acquires the transparency group dictionary of the XObject 
	form. 
	@param pdeForm IN/OUT? The from. 
	@return Transparency group object. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( PDEXGroup, PDEFormAcquireXGroup, (
			IN	PDEForm pdeForm)
	)

/**
	Determines whether the XObject form has a Transparency XGroup 
	
	@param pdeForm IN/OUT? The form. 
	@return true if the XObject form has a Transparency XGroup. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEFormHasXGroup, (
			IN	PDEForm pdeForm)
	)

/**
	Gets the graphics state information for an element. 
	@param pdeElement The PDEElement whose graphics state 
	is to be obtained. 
	@param stateP (Filled by the method) Pointer to a PDEGraphicStateP 
	structure that contains graphics state information for pdeElement. 
	
	@param stateSize The size of the stateP buffer, sizeof(PDEGraphicState)
	@return Returns true if the element has a graphics state; false 
	otherwise. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC(ASBool, PDEElementHasGState, (
			IN	PDEElement pdeElement, 
			OUT	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)
			
/*------------------------------------------------------------------------
	Methods added to support ExtGState
------------------------------------------------------------------------*/

/**
	Returns the overprint mode used by this graphics state. 
	
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Cos integer value. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASInt32, PDEExtGStateGetOPM, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns whether overprint is enabled for painting operations 
	other than stroking. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / op key in the ExtGState dictionary. 
	If the value is not found, the default value of false is 
	returned. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEExtGStateGetOPFill, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns whether overprint is enabled for stroke painting 
	operations. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / OP key in the ExtGState dictionary. 
	If the value is not found, the default value of false is 
	returned. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEExtGStateGetOPStroke, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns the opacity value for painting operations other 
	than stroking. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / ca key in the ExtGState dictionary. 
	If the value is not found, the default value of 1 is returned. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASFixed, PDEExtGStateGetOpacityFill, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns the opacity value for stroke painting operations 
	for paths and glyph outlines. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / CA key in the ExtGState dictionary. 
	If the value is not found, the default value of 1 is returned. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASFixed, PDEExtGStateGetOpacityStroke, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns the blend mode for the color composite for each 
	object painted. Valid names are Compatible, Normal, Multiply, 
	Screen, Difference, Darken, Lighten, ColorDodge, ColorBurn, 
	Exclusion, HardLight, Overlay, SoftLight, Luminosity, Hue, 
	Saturation and Color. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return If the value has not been set a value of Compatible is returned. 
	See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASAtom, PDEExtGStateGetBlendMode, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns the value of the Alpha Is Shape (AIS) member of 
	the graphics state. If AIS is true, the sources of alpha 
	are treated as shape; otherwise they are treated as opacity 
	values. If the value is not set, the default value of false 
	is returned. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEExtGStateGetAIS, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Returns whether the graphics state contains a soft mask. 
	
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns true if the ExtGState dictionary contains the / 
	SMask key; otherwise false is returned. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEExtGStateHasSoftMask, (
			IN	PDEExtGState pdeExtGState)
	)

/**
	Acquires the soft mask of the extended graphic state. 
	@param pdeExtGState The extended graphics state object.
	@return The soft mask or NULL if the ExtGState dictionary does not 
	contain the SMask key. 
	@exception peErrWrongPDEObjectType if pdeExtGState is NULL or is not 
	of type 
	@exception kPDEExtGState. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( PDESoftMask, PDEExtGStateAcquireSoftMask, (
			IN	PDEExtGState pdeExtGState)
	)

/**
	Checks whether the image has a soft mask. 
	@param image IN/OUT? An object of type PDEImage. 
	@return true if the soft mask exists; false otherwise. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASBool, PDEImageHasSMask, (
            IN	PDEImage image)
      )

/**
	Gets the soft mask for an image. Use PDERelease to dispose 
	of the object when it is no longer referenced. 
	@param image An object of type PDEImage.
	@return An object of type PDEImage. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC(PDEImage, PDEImageGetSMask, (
            IN	PDEImage image)
      )

/**
	Gets the matte array for the image XObject. 
	@param image IN/OUT? The image XObject. 
	@param matte IN/OUT? (Filled by the method) An array of values. 
	
	@param numComp IN/OUT? The number of values in matte. 
	@return Number of values copied. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASUns32, PDEImageGetMatteArray, (
			IN   PDEImage image,
			OUT  ASFixed  *matte,
			IN   ASUns32  numComp)
	)

/**
	Returns the text state of a character or a text element. 
	
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose text state is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param stateP IN/OUT? (Filled by the method) Pointer to a PDETextState 
	structure to fill with the text state of the specified character 
	or text run. 
	@param stateSize IN/OUT? Size of the stateP buffer, in bytes. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDETextRunSetTextState 
	@see PDETextGetTextState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(void, PDETextGetState, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Returns whether text knockout is enabled in the graphics 
	state. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / TK key in the ExtGState dictionary. 
	If the value is not found, the default value of true is 
	returned. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASBool, PDEExtGStateGetTK, (
			IN	PDEExtGState pdeExtGState)
	)

/**
	Returns writing mode. 0 for horizontal writing and 1 for 
	vertical writing. 
	@param sysEnc IN/OUT? An object of type PDSysEncoding. 
	@return 0 for horizontal writing and 1 for vertical writing. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASInt16, PDSysEncodingGetWMode, (
			IN	PDSysEncoding	sysEnc)
	)

/**
	Returns true for Identity-H or Identity-V encoding; false 
	otherwise. 
	@param sysEnc IN/OUT? An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASBool, PDSysEncodingIsIdentity, (
			IN	PDSysEncoding	sysEnc)
	)

/**
	Returns true for CMap encoding; false otherwise. 
	@param sysEnc IN/OUT? An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(ASBool, PDSysEncodingIsMultiByte, (
			IN	PDSysEncoding	sysEnc)
	)

/**
	Returns whether stroke adjustment is enabled in the graphics 
	state. 
	@param pdeExtGState IN/OUT? The extended graphics state object. 
	
	@return Returns the value of the / SA key in the ExtGState dictionary. 
	If the value is not set, the default value of false is returned. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASBool, PDEExtGStateGetSA, (
			IN	PDEExtGState pdeExtGState)
    )

/**
	Gets the soft mask name. 
	@param pdeSoftMask IN/OUT? The soft mask. 
	@return Soft mask name if it is a name; returns ASAtomNull otherwise. 
	
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC( ASAtom, PDESoftMaskGetName, (
            IN  PDESoftMask pdeSoftMask)
	)

/**
	Returns the matrix of a character or a text element. Unlike 
	PDETextGetTextMatrix, this function doesn't take fontSize, 
	hScale, and textRise in the textState into account. 
	@param pdeText IN/OUT? Text object containing a character or text 
	run whose graphics state is found. 
	@param flags IN/OUT? A PDETextFlags that specifies whether index 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	Must be either: kPDETextChar - for a text character kPDETextRun 
	 -  for a text run 
	@param index IN/OUT? Index of the character or text run in pdeText. 
	
	@param matrixP IN/OUT? (Filled by the method) ASFixedMatrixP that 
	holds the matrix of specified character or text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000

*/
UNPROC(void, PDETextGetMatrix, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			OUT	ASFixedMatrixP matrixP)
	)

/* BEGIN Optional Content API calls */

/**
	Gets an optional-content membership dictionary (OCMD) object 
	associated with the element. The element must be a PDEForm, 
	PDEImage (XObject image), or PDEContainer. If it is not 
	one of these, the method returns NULL. 
	<ul>
	<li> If the element is a PDEForm or PDEImage, the method returns 
	the dictionary attached to the element's Cos XObject dictionary. 
	<li> If the element is a PDEContainer, and it is for optional 
	content, the method returns the dictionary. If it is not 
	for optional content, the method returns NULL. 
	</ul>
	@param elem The element from which the dictionary is obtained.
	@return The dictionary object, or NULL if the element is not a PDEForm, 
	PDEImage (XObject image), or PDEContainer, or if it a container 
	that is not for optional content. 
	@see PDAnnotGetOCMD 
	@see PDEElementSetOCMD 
	@see PDEElementRemoveOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC( PDOCMD, PDEElementGetOCMD, (PDEElement elem) )

/**
	Tests whether an element is visible in a given content and 
	optional-content context. Traverses the content to find 
	the first occurrence of the element, in the supplied content 
	and in all nested contents. Returns true if the first occurrence 
	of the element is visible in the context, taking into account 
	the context's NonOCDrawing and PDOCDrawEnumType values. 
	

	The content can be NULL. In this case: 
	<ul>
	<li> If the element is a PDEForm, PDEImage, or PDEContainer, 
	the method checks the object to see if it has an optional-content 
	membership dictionary (OCMD) attached to it. If so, the 
	method returns true if the object is visible, without considering 
	whether the PDEContent that the element belongs to is visible. 
	<li> If the element is not one of these types, the method returns 
	true. 
	</ul>
	@param elem The element to test. 
	@param content The content containing the element. 
	@param ocContext The optional-content context in which 
	the element is tested.
	@return Returns true if the element is visible in the given content 
	and context, false if it is hidden. 
	@see PDEElementGetAllVisibilities 
	@see PDEElementMakeVisible 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC( ASBool, PDEElementIsCurrentlyVisible, (PDEElement elem, PDEContent content, PDOCContext ocContext) )

/**
	Tests whether all occurrences of the element are visible 
	in a given content and optional-content context. Traverses 
	the content to find each occurrence of the element, in the 
	supplied content and in all nested contents. To find the 
	visibility of a content element without considering its 
	parent, use PDEElementIsCurrentlyVisible. 

	Returns the number of occurrences and an array of boolean 
	values, true for each occurrence of the element that is 
	visible in the context, taking into account the context's 
	NonOCDrawing and PDOCDrawEnumType values. 
	@param elem The element for which to obtain visibilities. 
	
	@param content The content containing the element. 
	@param ocContext The optional-content context in which 
	the element is tested. 
	@param visibilities (Filled by the method) An array of 
	boolean values, true for each occurrence of the element 
	that is visible in the context. 
	@param capacity The size of the visibilities array.
	@return The number of occurrences of the element in the content. 
	
	@see PDEElementIsCurrentlyVisible 
	@see PDEElementMakeVisible 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC( ASUns32, PDEElementGetAllVisibilities, (PDEElement elem, PDEContent content, PDOCContext ocContext, ASBool *visibilities, ASUns32 capacity) )

/**
	Makes an element visible in a given content and optional-content 
	context, by manipulating the ON-OFF states of the optional-content 
	groups. 
	@param elem The element for which to set the visibility 
	state. 
	@param content The content containing the element. 
	@param ocContext The optional-content context in which 
	the element is made visible.
	@return true if the element is successfully made visible in the 
	given content and context, false otherwise. 
	@see PDEElementGetAllVisibilities 
	@see PDEElementIsCurrentlyVisible 
	@see PDOCMDsMakeContentVisible 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC( ASBool, PDEElementMakeVisible, (PDEElement elem, PDEContent content, PDOCContext ocContext) )
/* END Optional Content API calls */

/**
	Tests whether a font is an embedded font in the document 
	in which it was created. 
	@param pdeFont A PDEFont object to test.
	@return true if the font is embedded, false if it is not, or if 
	it was created in one document and embedded in a different 
	document. 
	@see PDEFontEmbedNow 
	@see PDEFontEmbedNowDontSubset 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(ASBool, PDEFontIsEmbedded, (
			IN	PDEFont	pdeFont)
	)

/**
	Gets the system font object associated with a font object. 
	
	@param pdeFont A PDEFont whose system font is found.
	@return The system font object. 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDFindSysFontForPDEFont 
	@see PDEFontSetSysFont 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(PDSysFont, PDEFontGetSysFont, (
			IN	PDEFont pdeFont)
	)

/**
	Gets the system encoding object associated with a font object. 
	
	@param pdeFont A PDEFont whose system encoding is found.
	@return The system encoding object. 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontSetSysEncoding 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(PDSysEncoding, PDEFontGetSysEncoding, (
			IN	PDEFont pdeFont)
	)

/**
	Gets the advance width of a character or a text element. 
	Advance width is returned in either character space or user 
	space. The advance width is the amount by which the current 
	point advances when the character is drawn. 

	Advance width may be horizontal or vertical, depending on 
	the writing style. Thus advanceP has both a horizontal and 
	vertical component. 
	@param pdeText Text object containing a character or text 
	run whose advance width is found. 
	@param flags A PDETextFlags value that specifies whether 
	index refers to the character offset from the beginning 
	of the text object or the index of the text run in the text 
	object. Must be either:
	<ul>
	<li> kPDETextChar - for a text character 
	<li> kPDETextRun - for a text run
	</ul>	
	In addition, set the kPDETextPageSpace 
	flag to obtain the advance width in user space. If it is 
	not set, the advance width is in character space. If this 
	flag is not set, this method returns a value that is independent 
	of any sizes, matrices, or scaling, simply adding up the 
	font's raw glyph widths, supplemented only by unscaled character 
	and word spacing. This differs from the behavior of the 
	older function, PDETextGetAdvanceWidth. 
	@param index Index of the character or text run in pdeText. 
	
	@param advanceP (Filled by the method) Pointer to a ASFixedPoint 
	value indicating the advance width. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextGetAdvanceWidth 
	@since PI_PDFEDIT_READ_VERSION >= 0x00040000
*/		
UNPROC(void, PDETextGetAdvance, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar, kPDETextPageSpace */ 
			IN	ASInt32 index, 
			OUT	ASFixedPointP advanceP)
	)

/**
	Gets the font for a text item. 

	NOTE: This method does not change the reference count of 
	the returned PDEFont. 
	@param textItem Text item whose font is obtained.
	@return Font of the specified text item. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemSetFont 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(PDEFont, PDETextItemGetFont, (
			IN	PDETextItem			textItem))

/**
	Gets the text matrix for a character in a text item. 
	@param textItem The text item. 
	@param charOffset The offset of the character whose text 
	matrix is obtained. 
	@param textMatrixP (Filled by the method) Pointer to a 
	ASFixedMatrix structure with the text matrix of the character.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemSetTextMatrix 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemGetTextMatrix, (
			IN	PDETextItem		textItem,
			IN	ASUns32			charOffset,
			OUT	ASFixedMatrix	*textMatrixP))

/**
	Gets the text state of a text item. 
	@param textItem The text item whose text state is obtained. 
	
	@param textStateP (Filled by the method) Pointer to a 
	PDETextStateP structure with text state of the text item. 
	
	@param textStateSize Size of the texStateP structure, 
	in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemSetTextState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemGetTextState, (
			IN	PDETextItem		textItem,
			OUT	PDETextStateP	textStateP,
			IN	ASUns32			textStateSize))

/**
	Gets the text length for a text item. 
	@param textItem Text item whose text length is obtained.
	@return The text length, in bytes. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemCopyText 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(ASUns32, PDETextItemGetTextLen, (
			IN	PDETextItem		textItem))

/**
	Copies the text from a text item element into a character 
	buffer. 
	@param textItem Pointer to the characters to add. NOTE: 
	Passing NULL for text can invalidate the text object but 
	will not raise an error. Callers must not pass NULL for 
	this parameter. 
	@param buffer (Filled by the method) A pointer to a buffer 
	in which to store the copy. 
	@param bufferSize Length of the text buffer, in bytes.
	@return The length in bytes of textItem. 
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetItem 
	@see PDETextAddItem 
	@see PDETextItemGetTextLen 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(ASUns32, PDETextItemCopyText, (
			IN	PDETextItem		textItem,
			OUT	ASUns8			*buffer,
			IN	ASUns32			bufferSize))

/**
	Gets the graphics state for a text item. 
	@param textItem Text item whose graphic state is obtained. 
	
	@param stateP (Filled by the method) Pointer to a 
	PDEGraphicStateP structure with graphics state of the text 
	item. 
	@param stateSize Size of the stateP buffer, in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemSetGState 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemGetGState, (
			IN	PDETextItem			textItem,
			OUT	PDEGraphicStateP	stateP,
			IN	ASUns32				stateSize))

/**
	Obtains a text item from a text element at a given index 
	position. 
	@param text Text object from which the text item is obtained. 
	
	@param index Index of the text item in pdeText. 
	@return The text item object. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextAddItem 
	@see PDETextItemCreate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
*/
UNPROC(PDETextItem, PDETextGetItem, (
			IN	PDEText				text,
			IN	ASUns32				index))

/**
    Returns the type of image as "FlateDecode", "JPXDecode" or "Unknown"
    when the image filter is not one of these types.
	@param image IN/OUT? The PDEImage object.
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC(ASAtom, PDEImageGetType, (
			IN	PDEImage	image)
	)

/**
    Acquires the PDEImageFlate resource of the PDEImage content element
    when the image filter type is "FlateDecode" or 0 if it is not.
	@param image IN/OUT? The PDEImage object.
	@return a PDEImageFlate resource object.
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC(PDEImageFlate, PDEImageAcquireImageFlate, (
			IN	PDEImage	image)
	)


/**
    Acquires the PDEImageJPX resource of the PDEImage content element
    when the image filter type is "JPXDecode" or 0 if it is not.
	@param image IN/OUT? The PDEImage object.
	@return a PDEImageJPX resource object.
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC(PDEImageJPX, PDEImageAcquireImageJPX, (
			IN	PDEImage	image)
	)


/**
	Gets a Cos object for an image. 
	@param pdeImageJPX IN/OUT? JPX image whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	image. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetType 
	@see PDEImageAcquireImageJPX 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( void, PDEImageJPXGetCosObj, (
            IN	PDEImageJPX	pdeImageJPX,
            OUT	CosObj		*cosObjP)
    )


/**
	Gets a Cos object for an image. 
	@param pdeImageFlate IN/OUT? Flate image whose Cos object is obtained. 
	@param cosObjP IN/OUT? (Filled by the method) Cos object for the 
	image. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetType 
	@see PDEImageAcquireImageFlate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( void, PDEImageFlateGetCosObj, (
            IN	PDEImageFlate	pdeImageFlate,
            OUT	CosObj		*cosObjP)
    )


/**
	Gets the attributes of a flate image. 
	@param imgFlate IN/OUT? A flate image resource object. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to a PDEImageFlateAttrs 
	structure containing the attributes of the flate image. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( void, PDEImageFlateGetAttrs, (
            IN PDEImageFlate imgFlate,
            OUT PDEImageFlateAttrsP attrsP, 
            IN ASUns32 attrsSize)
    )


/**
	Acquires the color space of the flate image, PDERelease should be used
    to release the color space when it is no longer referenced by the caller. 
	@param imgFlate IN/OUT? An object of type PDEImageFlate. 
	@return The color space of the flate image; otherwise NULL. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000
	@see PDEImageGetType 
	@see PDEImageAcquireImageFlate 

*/
UNPROC( PDEColorSpace, PDEImageFlateAcquireColorSpace, (
            IN PDEImageFlate imgFlate)
    )


/**
	Gets a data stream for a flate compressed image, PDEImageFlate object. 

	The caller must dispose of the returned ASStm by calling 
	ASStmClose. 
	@param imgFlate IN/OUT? Flate image whose data stream is obtained. 
	@param flags IN/OUT? PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, data is returned in encoded form. Otherwise, 
	data is decoded. 
	@return Stream for image. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageGetType 
	@see PDEImageAcquireImageFlate 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASStm, PDEImageFlateGetDataStm, (
            IN PDEImageFlate imgFlate,
            IN ASUns32 flags)
    )



/**
   Gets the attributes of a JPX encoded PDEImage.

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to a PDEImageJPXAttrs 
	structure containing the attributes of the JPX encoded image. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetAttrs 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( void, PDEImageJPXGetAttrs, (
            IN PDEImageJPX pdeImageJPX,
            OUT PDEImageJPXAttrsP attrsP, 
            IN ASUns32 attrsSize)
    )


/**
   Acquires the PDEColorSpace associated with the JPX encoded image,
   if one exists. If a PDF color space has not been associated with the
   JPX encoded image, 0 will be returned. This object is acquired and must
   be released using PDERelease when it is no longer in use.

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@return A PDF colorspace associated with the JPX encoded image.
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetType 
	@see PDEImageAcquireJPX 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( PDEColorSpace, PDEImageJPXAcquireColorSpace, (
            IN PDEImageJPX pdeImageJPX)
    )


/**
   Returns a stream containing the image data. Color component values are
   interlaced. For images with greater then 8 bits per component, the component
   values occupy the least significant bits of a two byte value.
   Valid values of flags is 0.

	@param pdeImageJPX IN/OUT? A JPX encoded image object.
	@param flags Unused.
	@return Stream for image. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASStm, PDEImageJPXGetDataStm, (
            IN PDEImageJPX pdeImageJPX,
            IN ASUns32 flags)
    )


/**
   Returns the number of JPX color spaces reference by the JPX encoded image.

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@return number of JPX color spaces specified by the JPX encoded image.
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASInt32, PDEImageJPXGetNumColorSpaces, (
            IN PDEImageJPX pdeImageJPX)
    )


/**
   Acquires a link list of JPXColorSpace objects defined with the JPX encoded image.
   if one exists. This object is acquired and must
   be released using PDERelease when it is no longer in use.

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@return A JPX colorspace associated with the JPX encoded image.
	@exception peErrWrongPDEObjectType 
	@see JPXColorSpaceAcquireNext
	@see JPXColorSpaceGetType
	@see JPXColorSpaceGetEnumAttrs
	@see JPXColorSpaceGetProfile
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( JPXColorSpace, PDEImageJPXAcquireJPXColorSpace, (
            IN PDEImageJPX pdeImageJPX)
    )


/**
   Returns true if the JPX encoded image has a JPX palette

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@return True if the encoded JPX image contains a palette.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquirePalette
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASBool, PDEImageJPXHasPalette, (
            IN PDEImageJPX pdeImageJPX)
    )


/**
   Acquires the JPXPalette from the JPX image object
   This object is acquired and must be released using
   PDERelease when it is no longer in use.

	@param pdeImageJPX IN/OUT? A JPX encoded image object. 
	@return A JPX palette associated with the JPX encoded image.
	@exception peErrWrongPDEObjectType 
	@see JPXPaletteGetNumEntries
	@see JPXPaletteGetBitDepths
	@see JPXPaletteGetNumComponents
	@see JPXPaletteGetTable
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( JPXPalette, PDEImageJPXAcquirePalette, (
            IN PDEImageJPX pdeImageJPX)
    )



/**
   Returns the number of palette entries.

	@param jpxPalette IN/OUT? A JPX encoded image object. 
	@return Number of palette entries.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquirePalette
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASInt32, JPXPaletteGetNumEntries, (
            IN JPXPalette jpxPalette)
    )

/**
   Returns the bit depths of the color values represented in the palette.
   The length of the array must be at least the number of components.

	@param jpxPalette IN/OUT? A JPX encoded image object. 
	@param bitDepths IN/OUT? (Filled by the method) Array of bit depths for each component.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquirePalette
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( void, JPXPaletteGetBitDepths, (
            IN JPXPalette jpxPalette,
            IN ASInt32* bitDepths)
    )

/**
   Returns the number of color components represented by the palette

	@param jpxPalette IN/OUT? A JPX encoded image object. 
	@return Number of components of the JPX image represented by the palette.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquirePalette
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASInt32, JPXPaletteGetNumComponents, (
            IN JPXPalette jpxPalette)
    )

/**
   Returns the palette data as a read only non-seekable ASStm.
   The returned ASStm should be read with ASStmRead.
   Each component entry in the palette is represented by the number of bytes
   needed to contain the bit depth for that component.

	@param jpxPalette IN/OUT? A JPX encoded image object. 
	@param paletteLength IN/OUT? (Filled by the method) Length of the palette data.
	@return A stream for the palette data.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXHasPalette
	@see PDEImageJPXAcquirePalette
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASStm, JPXPaletteGetTable, (
            IN JPXPalette jpxPalette,
            OUT ASInt32 *paletteLength)
    )


/**
   Acquires the next JPX color space defined with the JPX
   encoded image in the link list, if one exists. This
   object is acquired and must be released using PDERelease
   when it is no longer in use.

	@param jpxColorSpace IN/OUT? A JPX color space object. 
	@return The next JPX colorspace associated with the JPX encoded image.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquireJPXColorSpace
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( JPXColorSpace, JPXColorSpaceAcquireNext, (
            IN JPXColorSpace jpxColorSpace)
        )

/**
   Returns the type of JPX color space, one of kJPXCSUnknown,
   kJPXCSEnumerated, kJPXCSRestrictedICC, kJPXCSAnyICC, kJPXCSVenderColor

	@param jpxColorSpace IN/OUT? A JPX color space object. 
	@return The JPX color space type, JPXColorSpaceType.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquireJPXColorSpace
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( JPXColorSpaceType, JPXColorSpaceGetType, (
            IN JPXColorSpace jpxColorSpace)
        )

/**
   Gets the attributes of an enumerated color space. Returns false if the
   color space is not kJPXCSEnumerated.

	@param jpxColorSpace IN/OUT? A JPX color space object. 
	@param jpxCSEnumAttrsP IN/OUT? (filled in by the method) Attributes of a JPX
    Enumerated color space.
	@return True if the JPX color space is kJPXCSEnumerated
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquireJPXColorSpace
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASBool, JPXColorSpaceGetEnumAttrs, (
            IN  JPXColorSpace jpxColorSpace,
            OUT JPXCSEnumAttrsP jpxCSEnumAttrsP)
        )

/**
   Gets the color profile of a ICC based JPX color space.
   If input parameter, profile is 0, returns the length of the profile in bytes,
   else returns the number of bytes copied to profile.

	@param jpxColorSpace IN/OUT? A JPX color space object. 
	@param profile IN/OUT? (Filled by the method) Profile of the JPX color space.
	@param profileLength IN/OUT? Byte length of the user supplied profile buffer.
	@return The byte length of the profile, if profile is 0,
    else returns the byte length copied to profile.
	@exception peErrWrongPDEObjectType 
	@see PDEImageJPXAcquireJPXColorSpace
	@since PI_PDFEDIT_READ_VERSION >= 0x00060000

*/
UNPROC( ASInt32, JPXColorSpaceGetProfile, (
            IN JPXColorSpace jpxColorSpace,
            OUT ASUns8 *profile,
            IN ASInt32 profileLength)
        )

