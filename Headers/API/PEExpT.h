/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1995-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PEExpT.h

 - Types, macros, structures, etc. required to use the PDFEdit HFTs.

*********************************************************************/

#ifndef _H_PEExpT
#define _H_PEExpT

#include "Environ.h"
#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#include "ASExpT.h"
#include "CosExpT.h"
#include "PDExpT.h"	/* ASFixedQuad */
#else
#include "PubTypes.h"
#include "ASTypes.h"
#include "ASExtndT.h"
#include "CosTypes.h"
#include "ASFixed.h"
#include "FixedGeo.h"
#include "FixedMtx.h"
#include "ASStm.h"
#endif

#include "PDSysFontExpT.h"

#if PLUGIN
#define PEX1 ACCB1
#define PEX2 ACCB2
#else
#define PEX1 ACEX1
#define PEX2 ACEX2
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

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* 
** Function prototypes
** The following defines are used in function prototypes
** for labelling function parameters as input or output.
** Note that function arguments which are pointers and are
** marked as 'OUT' are pointers to buffers and the buffers 
** will contain the output data.
*/
#define IN
#define OUT 

/* Note on integer types.
** The following integer types are used for the following purposes.
**	Index into a structure or array:		ASInt32
**	Count, number of elements:				ASInt32
**	Variable or argument of enum:			ASInt32
**	Size in bytes of a structure or data:	ASUns32
**	Bitfield flag:							ASUns32
**
** I find it easier to use these existing types than to define
** new types such as Count, Index and Size, and to remember to 
** use them consistently.
**
** I use an ASInt32 for index and enum because both may be negative.
** (Although I don't use negative indixes in this API.)
** I use ASInt32 for enums because enums can have different sizes
** on different compilers; I want my API to be independent of compiler.
** ASInt32 is used for count because it cannot be greater than an index.
** ASUns32 is the obvious choice for size and bitfield (os_size_t has 
** too many underlines in it!-) 
*/

/*------------------------------------------------------------------------
	PDFEdit Public Types.
------------------------------------------------------------------------*/


/** The abstract super class of PDFEdit classes. You can find the type of any object with
	the PDEObjectGetType method. You can then cast and apply that class’s methods
	to the object. In addition, you can cast any of the PDFEdit objects to a PDEObject
	and use it anywhere a PDEObject is called for, such as in the PDEObject methods.
	@see PDERelease
	@see PDEObjectDump
*/
typedef struct _t_PDEObject *PDEObject;

/** Contains the modifiable contents of a PDPage. A PDEContent may be obtained from
	an existing page or from a Form XObject or from a Type 3 CharProc. You can create
	an empty PDEContent. A PDEContent contains PDEElements. In addition, a
	PDEContent may have attributes such as Form matrix and setcachedevice
	parameters.
	@see PDEContentCreate
	@see PDEContainerGetContent
	@see PDEContentCreateFromCosObj
	@see PDEFormGetContent
	@see PDPageAcquirePDEContent
	@see PDERelease
*/
typedef struct _t_PDEContent *PDEContent;

/** The base class for elements of a page display list (PDEContent) and for clip objects.
The general PDEElement methods allow you to get and set general element
properties.
@see PDEContainer (subclass)
@see PDEForm (subclass)
@see PDEGroup (subclass)
@see PDEImage (subclass)
@see PDEPath (subclass)
@see PDEPlace (subclass)
@see PDEPS (subclass)
@see PDEShading (subclass)
@see PDEText (subclass)
@see PDEUnknown (subclass)
@see PDEXObject (subclass)
@see PDEClipGetElem (subclass)
@see PDEContentGetElem (subclass)
@see PDEElementCopy (subclass)
@see PDERelease
*/
typedef struct _t_PDEElement *PDEElement;

/* Subclasses of PDEElement.
** PDEText			- a collection of text strings or words
** PDEPath			- a graphic path
** PDEImage			- an image
** PDEForm			- an XObject form
** PDEPS			- a pass-through PostScript object
** PDEXObject		- an XObject of unknown type
** PDEPlace	   		- a marked content place operator
** PDEContainer		- a marked content container
** PDEGroup			- a container of PDEElements	
** PDEUnknown		- an unknown element
*/

/**A PDEElement representing text. It is a container for text as show strings or as
	individual characters. Each sub-element may have different graphics state properties.
	However, the same clip applies to all sub-elements of a PDEText. Also, the
	charpath of a PDEText can be used to represent a clip.
	@see PDEElement (superclass)
	@see PDETextCreate
	@see PDERelease
*/
typedef struct _t_PDEText *PDEText;

/** A PDEElement that contains a path. Path objects can be stroked, filled, and/or serve
	as a clipping path
	@see PDEElement (superclass)
	@see PDEPathCreate
	@see PDERelease
*/
typedef struct _t_PDEPath *PDEPath;

/** A PDEElement that contains an Image XObject or in-line image. You can associate
	data or a stream with an image.
	@see PDEElement (superclass)
	@see PDEImageCreate
	@see PDEImageCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEImage *PDEImage;

/** A PDEElement that corresponds to an instance of an XObject Form on a page (or
	other containing stream such as another XObject Form or annotation form). The
	context associated with this instance includes the actual CosObj stream that
	represents the XObject Form and the initial conditions of the graphics state. The
	latter consists of the transformation matrix, initial color values, and so forth. It is
	possible to have two PDEForms that refer to the same XObject Form. The forms will
	exist at different places on the same page, depending on the transformation matrix.
	They may also have different colors or line stroking parameters. In the case of a
	transparency group, the opacity is specified in the gstate.
	Within a PDEForm, each PDEElement has it’s own gstate (or is a container, place,
	or group object). These gstates are independent of the parent PDEForm gstate.
	PDEForm elements within the PDEForm may have their own opacity.
	A PDEContent may be obtained from a PDEForm to edit the form’s display list.
	@see PDEElement (superclass)
	@see PDEFormCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEForm *PDEForm;

/** Element representing in-line or XObject pass-through PostScript object. XObject
	PostScripts are listed in page XObject resources.
	@see PDEElement (superclass)
	@see PDEPSCreate
	@see PDEPSCreateFromCosObj
*/
typedef struct _t_PDEPS *PDEPS;

/** A PDEElement representing an arbitrary XObject.
	@see PDEElement (superclass)
	@see PDEXObjectCreate
	@see PDERelease
*/
typedef struct _t_PDEXObject *PDEXObject;

/** A PDEElement that marks a place on a page in a PDF file. In a PDF file, a place is
	represented by the MP or DP Marked Content operators.
	Marked content is useful for adding structure information to a PDF file. For instance, a
	drawing program may want to mark a point with information, such as the start of a
	path of a certain type. Marked content provides a way to retain this information in the
	PDF file. A DP operator functions the same as the MP operator and, in addition,
	allows a property list dictionary to be associated with a place.
	@see PDEElement (superclass)
	@see PDEPlaceCreate
	@see PDERelease
*/
typedef struct _t_PDEPlace *PDEPlace;

/** A PDEElement representing an unknown element.
	@see PDEElement (superclass)
*/
typedef struct _t_PDEUnknown *PDEUnknown;

/** A group of PDEElements on a page in a PDF file. In the PDF file, containers are
	delimited by Marked Content BMC/EMC or BDC/EMC pairs. Every PDEContainer
	has a Marked Content tag associated with it. In addition to grouping a set of elements,
	a BDC/EMC pair specifies a property list to be associated with the grouping. Thus a
	PDEContainer corresponding to a BDC/EMC pair also has a property list dictionary
	associated with it.
	@see PDEContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEContainer *PDEContainer;

/** An in-memory representation of objects in a PDEContent. It has no state and is not
	represented in any way in a PDF content stream (that is, PDEContent).
	When used in a PDEClip, this object is used to associate PDEText objects into a
	single clipping object.
	@see PDEElement (superclass)
	@see PDEGroupCreate
*/
typedef struct _t_PDEGroup *PDEGroup;

/** A PDEElement that represents smooth shading.
	@see PDEElement (superclass)
	@see PDEShadingCreateFromCosObj
	@see PDEShadingCreateFromCosObj
	@see PDEShadingGetCosObj
*/
typedef struct _t_PDEShading *PDEShading;

/** The PDFEdit representation of the opening bracket of a marked-content sequence.
	Elements of this type must be paired with elements of type PDEEndContainer.
	@see PDEElement (superclass)
	@see PDEBeginContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEBeginContainer *PDEBeginContainer;

/** The PDFEdit representation of the closing bracket of a marked-content sequence.
	Elements of this type must be paired with elements of type PDEBeginContainer.
	@see PDEElement (superclass)
	@see PDEEndContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEEndContainer *PDEEndContainer;

/** A group of PDEElements on a page in a PDF file.
	@see PDEElement (superclass)
	@see PDEBeginGroupCreate
	@see PDERelease
*/
typedef struct _t_PDEBeginGroup *PDEBeginGroup ;

/** A group of PDEElements on a page in a PDF file.
	@see PDEElement (superclass)
	@see PDEEndGroupCreate
	@see PDERelease
*/
typedef struct _t_PDEEndGroup *PDEEndGroup;
 

/* This typedef appears in both PDExpT.h and PEExpT.h now, so avoid multiple typedef's here */
#ifndef _T_PDEFONT_
#define _T_PDEFONT_

/** A reference to a font used on a page in a PDF file. It may be equated with a font in the
	system. A PDEFont is not the same as a PDFont; a PDFont is associated with a
	particular document.
	@see PDEFontCreate
	@see PDEFontCreateFromCosObj
	@see PDEFontCreateFromSysFont
	@see PDEFontCreateFromSysFontEx
	@see PDEFontCreateWithParams
	@see PDETextGetFont
	@see PDERelease
*/
typedef struct _t_PDEFont *PDEFont;
#endif


/** A reference to a color space used on a page in a PDF file. The color space is part of
	the graphics state attributes of a PDEElement.
	@see PDEColorSpaceCreate
	@see PDEColorSpaceCreateFromName
	@see PDEImageGetColorSpace
	@see PDERelease
*/
typedef struct _t_PDEColorSpace *PDEColorSpace;


/** A list of PDEElements containing a list of PDEPaths and PDETexts that describe a
	clip state. PDEClips can be created and built up with PDEClip methods. Any
	PDEElement object can have PDEClip associated with it.
	PDEClip objects can contain PDEContainers and PDEGroups to an arbitrary level
	of nesting. This allows PDEContainers to be used to mark clip objects.
	PDEGroups inside PDEClips that contain at least one PDEText and no PDEPaths
	have a special meaning. All PDEText objects contained in such a PDEGroup are
	considered to be part of the same BT/ET block. This means that the union of these
	PDETexts makes up a single clipping path - as opposed to the intersection of the
	PDETexts.
	@see PDEClipCreate
	@see PDEElementGetClip
	@see PDERelease
	@see PDEClipFlattenedEnumElems
*/
typedef struct _t_PDEClip *PDEClip;


/** A reference to an ExtGState resource used on a page in a PDF file. It specifies a
	PDEElement’s extended graphics state, which is part of its graphics state.
	@see PDEExtGStateCreate
	@see PDERelease
*/
typedef struct _t_PDEExtGState *PDEExtGState;


/** A reference to a Pattern resource used on a page in a PDF file.
	@see PDEPatternCreate
	@see PDERelease
	@see PDEPatternGetCosObj
*/
typedef struct _t_PDEPattern *PDEPattern;


/** A color space with a variable number of device-dependent components. Usually used
	to store multiple spot colors in a single color space.
	@see PDEDeviceNColorsCreate
*/
typedef struct _t_PDEDeviceNColors *PDEDeviceNColors;


/** Reference to the state of a reader.
*/
typedef struct _t_PDEState *PDEState;


/** Reference to the state of a writer.
*/
typedef struct _t_PDEEmitState *PDEEmitStateP;


/** Object for creating and manipulating a soft mask in a PDF file.
	@see PDEElement (superclass)
	@see PDESoftMaskCreate
	@see PDESoftMaskCreateFromCosObj
	@see PDESoftMaskCreateFromName
	@see PDERelease
*/
typedef struct _t_PDESoftMask *PDESoftMask;


/** A transparency (XGroup) resource.
	@see PDEElement (superclass)
	@see PDEXGroupCreate
	@see PDEXGroupCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEXGroup *PDEXGroup;


/** An object used to read streams of PDEElements from page contents
*/
typedef struct _t_PDEReader *PDEReader;


/** An object used to write streams of PDEElements to a page content
*/
typedef struct _t_PDEWriter *PDEWriter;


/** A PDEElement that provides system encoding for a PDF file.
	@see PDEElement (superclass)
	@see PDSysEncodingCreateFromBaseName
	@see PDSysEncodingCreateFromCMapName
	@see PDERelease
*/
typedef struct _t_PDSysEncoding *PDSysEncoding;


/** reference to a PDEDoc
*/
typedef struct _t_PDEDoc *PDEDoc;


/** reference to a PDEPage
*/
typedef struct _t_PDEPage *PDEPage;


/** reference to a PDETextItem
*/
typedef struct _t_PDETextItem *PDETextItem;

/** reference to a PDEImageFlate
*/
typedef struct _t_PDEImageFlate *PDEImageFlate;


/** reference to a PDEImageJPX
*/
typedef struct _t_PDEImageJPX *PDEImageJPX;


/** reference to a JPXColorSpace
*/
typedef struct _t_JPXColorSpace *JPXColorSpace;


/** reference to a JPXPalette
*/
typedef struct _t_JPXPalette *JPXPalette;


/*------------------------------------------------------------------------
	PDFEdit Attributes.
------------------------------------------------------------------------*/


/**
	Structure describing attributes of a PDEContent object. 
	
	@see PDEContentGetAttrs 
	@see PDEContentToCosObj 
*/
typedef struct _t_PDEContentAttrs {

	/** PDEContentFlags */
	ASUns32 flags;						
	
	/* CharProc attributes */

	/** If flags has kPDESetCacheDevice set, the first 6 cache
		device values contain the operands for the d1 (setcachdevice)
		page operator. If flags has kPDESetCacheDevice set,
		cacheDevice contains 2 charwidth values. 
	*/
	ASFixed cacheDevice[8];				
	
	/* Form attributes */

	/** Only used if PDEContent contains a Form XObject.
		Corresponds to FormType key in the XObject Form attributes
		dictionary. 
	*/
	ASInt32			formType;

	/** Only used if PDEContent contains a Form. Bounding box of the
		PDEContent object. Corresponds to BBox key in the XObject
		Form attributes dictionary. 
	*/
	ASFixedRect		bbox;

	/** Only used if PDEContent contains a Form. Transformation
		matrix for the PDEContent object. Corresponds to Matrix key in
		the XObject Form attributes dictionary. 
	*/
	ASFixedMatrix	matrix;

	/** Only used if PDEContent contains a Form. The form’s XUID, an
		ID that uniquely identifies the form. Corresponds to XUID key in
		the XObject Form attributes dictionary. 
	*/
	CosObj			XUID;
	} PDEContentAttrs, *PDEContentAttrsP;
	

/**
	Structure describing a color value. 
	@see Numerous 
	@see Members 
	@see color Color value components. For instance, a Gray color 
	space has 1 component, an RGB color space has 3 components, 
	a CMYK has 4 components, and so on. 
	@see colorObj2 For a DeviceN color space. 
	@see colorObj For color spaces whose color values do not have 
	numeric values, such as the Pattern and Separation color 
	spaces.
*/
typedef struct _t_PDEColorValue {

	/** Color value components. For instance, a Gray color space has 1
		component, an RGB color space has 3 components, a CMYK
		has 4 components, and so on.
	*/
	ASFixed			color[7];			

	/** if color space is DeviceN */
	PDEObject		colorObj2;			

	/**  For color spaces whose color values do not have numeric
		values, such as the Pattern and Separation color spaces.
	*/
	PDEObject		colorObj;			
	} PDEColorValue, *PDEColorValueP;
	

/**
	Structure describing a color specification. 
	@see Numerous 
*/ 
typedef struct _t_PDEColorSpec {

	/** The specified PDEColorSpace. */
	PDEColorSpace	space;

	/** The color value. */
	PDEColorValue	value;
	} PDEColorSpec, *PDEColorSpecP;
	

/**
	Structure describing a dash specification, as described 
	in Table 4.8 in the PDF Reference. See Section 4.3.2 for 
	more information on line dash patterns. 
	@see Numerous 
*/
typedef struct _t_PDEDash {

	/** Dash phase. Phase is a number that specifies a distance in user
		space into the dash pattern at which to begin marking the path. 
	*/
	ASFixed			dashPhase;

	/** Number of entries in the dash array, an element of the Border array. */
	ASInt32			dashLen;

	/** Dash array, which specifies distances in user space for the
		length of dashes and gaps. 
	*/
	ASFixed			dashes[11];
	} PDEDash, *PDEDashP;


/** Structure describing the graphics state that was set.
	@see PDEDefaultGState
	@see PDETextAdd
*/
typedef enum {

	/**  A fill color space was set, corresponding to the cs (setcolorspace) operator. */
	kPDEFillCSpaceWasSet 	= 0x0001,

	/** A color fill value was set, corresponding to the sc (setcolor) operator. */
	kPDEFillCValueWasSet 	= 0x0002,

	/** A color space stroke value was set, corresponding to the CS (setcolorspace) operator. */
	kPDEStrokeCSpaceWasSet	= 0x0004,

	/** A color stroke value was set, corresponding to the SC (setcolor) operator. */
	kPDEStrokeCValueWasSet	= 0x0008,

	/** A dash specification was set, corresponding to the d (setdash) operator. */
	kPDEDashWasSet			= 0x0010,

	/** The line width was set, corresponding to the w (setlinewidth) operator. */
	kPDELineWidthWasSet		= 0x0020,

	/** The miter limit was set, corresponding to the M (setmiterlimit) operator. */
	kPDEMiterLimitWasSet	= 0x0040,

	/** Line flatness was set, corresponding to the i (setflat) operator. */
	kPDEFlatnessWasSet		= 0x0080,

	/** Line cap style was set, corresponding to the J (setlinecap) operator. */
	kPDELineCapWasSet		= 0x0100,

	/** Line join style was set, corresponding to the j (setlinejoin) operator. */
	kPDELineJoinWasSet		= 0x0200,

	/** A color rendering intent was set, corresponding to the Intent key in the image dictionary. */
	kPDERenderIntentWasSet	= 0x0400,

	/** An extended graphics state was set, corresponding to the gs operator. */
	kPDEExtGStateWasSet		= 0x0800
} PDEGraphicStateWasSetFlags;


/** Structure describing the text state that was set.
	@see PDETextAdd
	@see PDETextGetTextState
	@see PDETextRunSetTextState
*/
typedef enum {

	/** Character spacing was set, corresponding to the Tc operator. */
	kPDECharSpacingWasSet	= 0x0001,

	/** Word spacing was set, corresponding to the Tw operator. */
	kPDEWordSpacingWasSet	= 0x0002,

	/** Text rendering mode was set, corresponding to the Tr operator. */
	kPDERenderModeWasSet	= 0x0004,

	/** TBD */
	kPDEFontSizeWasSet		= 0x0008,	/* new in PDFLib 5.0 */

	/** TBD */
	kPDEHScaleWasSet		= 0x0010,	/* new in PDFLib 5.0 */

	/** TBD */
	kPDETextRiseWasSet		= 0x0020	/* new in PDFLib 5.0 */
} PDETextStateWasSetFlags;

#define kPDFStateSetAll	((ASUns32) -1)
	

/**
	Attributes of a PDEElement or a PDEText sub-element. 
	@see PDEDefaultGState 
	@see PDETextAdd 
*/
typedef struct _t_PDEGraphicState {

	/** PDEGraphicStateWasSetFlags indicating if an attribute has been set.
		NOTE:Support for these flags is not complete. For compatibility,
		you should set them, but do not depend on reading their
		values back. The intended use is with XObject Forms to
		indicate whether the value is inherited or explicitly set. 
	*/
	ASUns32			wasSetFlags;	

	/** Fill color specification. 
		The default value is DeviceGray, fixedZero. 
	*/
	PDEColorSpec	fillColorSpec;

	/** Stroke color specification. 
		The default value is DeviceGray, fixedZero. 
	*/
	PDEColorSpec	strokeColorSpec;

	/** Dash specification. 
		The default value is [0, 0]. 
	*/
	PDEDash			dash;

	/** Line width, corresponding to the w (setlinewidth) operator. 
		The default value is fixedOne. 
	*/
	ASFixed			lineWidth;

	/** Miter limit, corresponding to the M (setmiterlimit) operator. 
		The default value is fixedTen. 
	*/
	ASFixed			miterLimit;

	/** Line flatness, corresponding to the i (setflat) operator. 
		The default value is fixedZero. 
	*/
	ASFixed			flatness;

	/** Line cap style, corresponding to the J (setlinecap) operator. 
		The default value is 0. 
	*/
	ASInt32			lineCap;

	/** Line join style, corresponding to the j (setlinejoin) operator. 
		The default value is 0. 
	*/
	ASInt32			lineJoin;

	/** A color rendering intent, corresponding to the Intent key in the image dictionary. 
		The default value is 0. 
	*/
	ASAtom			renderIntent;

	/** An extended graphics, corresponding to the gs operator. 
		The default value is CosObj, NULL. 
	*/
	PDEExtGState	extGState;
	} PDEGraphicState, *PDEGraphicStateP;

/* Attributes of a PDEText subelement */
/* PDETextGetTextState and PDETextRunSetTextState can handle up to renderMode for backward
   compatibility with PDFLib 4.0 or below.
   Use PDETextGetState and PDETextRunSetState for new PETextState fields. */

/**
	Attributes of a PDEText element. 
	@see PDETextAdd 
	@see PDETextGetTextState 
	@see PDETextRunSetTextState 
*/
typedef struct _t_PDETextState {

	/** PDETextStateWasSetFlags indicating if an attribute has been set.
		NOTE:Support for these flags is not complete. For compatibility,
		you should set them, but do not depend on reading their
		values back. The intended use is with XObject Forms to
		indicate whether the value is inherited or explicitly set.

		NOTE:PDFEdit ignores the wasSetFlags flag, so you must
		initialize the PDETextState fields. 
	*/
	ASUns32			wasSetFlags;	

	/** Character spacing was set, corresponding to the Tc operator. */
	ASFixed			charSpacing;

	/** Word spacing, corresponding to the Tw operator. */
	ASFixed			wordSpacing;

	/** Text rendering mode, corresponding to the Tr operator. */
	ASInt32			renderMode;

	/** Default is 1. */
	ASFixed			fontSize;			/* new in PDFLib 5.0. Default = 1 */

	/** Default=100 (==100%) */
	ASFixed			hScale;				/* new in PDFLib 5.0. Default = 100 (== 100%) */

	/** Specifies the distance, in text space units that are not scaled, to
		move the baseline up or down from its default location. See Section 5.2.6 in the PDF Reference. 
	*/
	ASFixed			textRise;			/* new in PDFLib 5.0 */
	} PDETextState, *PDETextStateP;


/**
	Attributes of a PDEImage. 
	@see PDEImageCreate 
	@see PDEImageGetAttrs 
	@see PDEImageGetData 
*/
typedef struct _t_PDEImageAttrs {

	/** PDEImageAttrFlags indicating image attributes. */
	ASUns32			flags;		

	/** Width of the image, corresponding to the Width key in the image dictionary. */
	ASInt32			width;

	/** Height of the image, corresponding to the Height key in the image dictionary. */
	ASInt32			height;

	/** Number of bits used to represent each color component in the
		image, corresponding to the BitsPerComponent key in the image dictionary. 
	*/
	ASInt32			bitsPerComponent;

	/** An array of numbers specifying the mapping from sample values
		in the image to values appropriate for the current color space.
		These values correspond to the Decode key in the image dictionary. 
	*/
	ASFixed			decode[8];

	/** Color rendering intent, corresponding to the Intent key in the image dictionary. */
	ASAtom			intent;
	} PDEImageAttrs, *PDEImageAttrsP, PDEImageFlateAttrs, *PDEImageFlateAttrsP;


/**
	Filter element in a filter array. 
	@see CosNewStream 
	@see PDEContentToCosObj 
	@see PDEImageCreate 
*/
typedef struct _t_PDEFilterSpec {

	/** Parameters used by the decoding filters specified with the Filter
		key. Corresponds to the DecodeParms key in the stream
		dictionary.
		Must be set to NULL if PDEFilterSpec is specified but no
		encode or decode parameters are specified. This can be done
		by passing CosNewNull for the unused encode and/or decode
		params.
		Required decode params for DCTDecode are Columns, Rows,
		and Colors. The parameters should be passed in a CosDict. 
	*/
	CosObj	decodeParms;

	/** Parameters used when encoding the stream. Required for
		DCTDecode filter; optional for other filters.
		Must be set to NULL if PDEFilterSpec is specified but no
		encode or decode parameters are specified. This can be done
		by passing CosNewNull for the unused encode and/or decode params. 
	*/
	CosObj	encodeParms;

	/** Filter name. Supported filters are: ASCIIHexDecode,
		ASCII85Decode, LZWDecode, DCTDecode,
		CCITTFaxDecode, RunLengthDecode, and FlateDecode. 
	*/
	ASAtom	name;

	/** Reserved - used to align to 32 bits. */
	ASInt16	padding;			
	} PDEFilterSpec, *PDEFilterSpecP;
	

/** Filter information for streams.
	Although the PDEFilterSpec is declared as a one-member array in the header file,
	more members can be added by dynamically allocating the PDEFilterArray with
	space for as many filters as you would like to add.
	In practice, there is seldom need for more than two filters.
	@see PDEContentToCosObj
	@see PDEImageCreate
*/
typedef struct _t_PDEFilterArray {

	/** Number of filters in the array. */
	ASInt32 numFilters;

	/** Variable length array of filter spec. */
	PDEFilterSpec spec[1];	
	} PDEFilterArray, *PDEFilterArrayP;
	

/**
	Attributes of a PDEPS object. 
	@see PDEPSCreate 
	@see PDEPSGetAttrs 
*/
typedef struct _t_PDEPSAttrs {

	/** kPDEPSInline */
	ASUns32	flags;			
	} PDEPSAttrs, *PDEPSAttrsP;


/**
	Attributes of a PDEFont and of a PDSysFont. This structure 
	is also referenced in PDEFontCreateParams.
	@see PDEFontCreate 
	@see PDEFontCreateWithParams 
	@see PDEFontGetAttrs 
	@see PDFindSysFont 
	@see PDFindSysFontEx 
	@see PDSysFontGetAttrs 
	@see PDSysFontGetEncoding 
	@see PDSysFontAcquirePlatformData 
*/	
typedef struct _t_PDEFontAttrs {	

	/** An ASAtom for font name, as in "Times-Roman." Corresponds to
		the BaseFont key in the font dictionary of a PDF file (see
		Section 5.6.3 in the PDF Reference). 
		*/
	ASAtom	name;

	/** An ASAtom for font type, corresponding to the Subtype key in a
		a font dictionary. May be "Type1," "TrueType," "MMType1," or "Type0." 
	*/
	ASAtom	type;

	/** An ASAtom for "Roman" or ASAtomNull. If "Roman," the
		characters must be a subset of the Adobe Standard Roman Character Set. 
	*/
	ASAtom	charSet;

	/** An ASAtom for font encoding. May be MacRomanEncoding,
		WinAnsiEncoding, or ASAtomNull. In the case of
		ASAtomNull, call PDSysFontGetEncoding to get more
		information about the encoding. 
	*/
	ASAtom	encoding;
	
	/* Almost the same as PDFontMetrics in Acrobat 2.0 */

	/** Desired font flags, one or more of Font Flags. */
	ASUns32		flags;				/* Use PD_SCRIPT, etc. to get flags	*/

	/** Font bounding box in 1000 EM units.	*/
	ASFixedRect	fontBBox;			

	/** Width of missing character (.notdef). */
	ASInt16  	missingWidth;		

	/** Vertical stem width. */
	ASInt16  	stemV;				

	/** Horizontal stem width. */
	ASInt16  	stemH;				

	/** Capital height. */
	ASInt16  	capHeight;			

	/** X height. */
	ASInt16  	xHeight;			

	/** Max ascender height. */
	ASInt16  	ascent;				

	/** Max descender depth. */
	ASInt16  	descent;			

	/** Additional leading between lines. */
	ASInt16  	leading;			

	/** Maximum character width. */
	ASInt16  	maxWidth;			

	/** Average character width. */
	ASInt16  	avgWidth;			

	/** Italic angle in degrees, if any.	*/
	ASInt16  	italicAngle;		

	/** CIDFontType0 or CIDFontType2. */
	ASAtom		cidFontType;		

	/** Writing mode. Must be one of:
	<ul>
	<li> 0 for horizontal writing
	<li> 1 for vertical writing.
	</ul>	
	 */
	ASInt16		wMode;			

	/** ASAtom representing the PostScript name of a TrueType font. */
	ASAtom		psName;			

	/** the platform name  */
	ASAtom		platformName;	

	/** ASAtom representing the ISO 639 language code. These are available from http://www.iso.ch. */
	ASAtom		lang;			

	/** ASAtom representing the CIDFont's Registry information, as in "gAdobe-Japan". */
	ASAtom		registry;		

	/** ASAtom representing the CIDFont's Ordering information, for example, "g1". */
	ASAtom		ordering;

	/** The SystemSupplement field from the CIDFont. */
	ASInt32		supplement;

	/** A non-zero value means the font can't be embedded. */
	ASInt32		cantEmbed;

	/** The name of the base encoding; that is, the BaseEncoding entry
		in an encoding dictionary (see section 5.5.5 of the PDF
		Reference). The Differences entry of the encoding dictionary
		describes differences (deltas) from the base encoding.	 
	*/
	ASAtom		deltaEncoding;		

	/** protection Allows setting one of the following 
	bits to disable font embedding:
	<ul>
	<li> kPDEFontNoEmbedding = 1: font should not be embedded.
	<li> kPDEFontNoEditableEmbedding = 2: font should not be embedded
	for editing purposes.
	</ul>	
	*/
	ASUns32		protection;		

	/** PDSysFontPackageType */
	ASInt32		packageType;	
	} PDEFontAttrs, *PDEFontAttrsP;
	

/**
	Parameters used for PDEFontCreateWithParams to describe 
	a font. 
	@see PDEFontCreateWithParams 
*/
typedef struct _t_PDEFontCreateParams {

	/** Pointer to a PDEFontAttrs for the font attributes. */
	PDEFontAttrsP attrsP;

	/** Size of the data structure. Must be set to sizeof(PDEFontAttrs). */
	ASUns32 attrsSize;

	/** First character index for the widths array, widthsP. */
	ASInt32 firstChar;

	/** Last character index for the widths array, widthsP. */
	ASInt32 lastChar;

	/** Pointer to widths array. */
	ASInt16 *widthsP; 

	/** An array of 256 pointers to glyph names specifying the custom
		encoding. If any pointer is NULL, no encoding information is written for that entry. 
	*/
	char **encoding;

	/** An ASAtom representing the encoding base name if the
		encoding is a custom encoding. If encoding is NULL,
		encodingBaseName is used as the value of the encoding and
		must be one of "WinAnsiEncoding", "MacRomanEncoding", or
		"MacExpertEncoding". If no encoding value is desired, use
		ASAtomNull. However, for Type 0 fonts, this field must be a
		valid CMap name, or PDEFontCreateWithParams will fail. 
	*/
	ASAtom encodingBaseName;

	/** Stream with font information. */
	ASStm fontStm;

	/** Length in bytes of the ASCII portion of the Type 1 font file after it
		has been decoded. For other font formats, such as TrueType or
		CFF, only len1 is used, and it is the size of the font. 
	*/
	ASInt32 len1;

	/** Length in bytes of the encrypted portion of the Type 1 font file after it has been decoded. */
	ASInt32 len2;

	/** Length in bytes of the portion of the Type 1 font file that contains
		the 512 zeros, plus the cleartomark operator, plus any following data. 
	*/
	ASInt32 len3;

	/** If true, the dw and w fields are used; if false, they are not used. */
	ASBool hasDW;

	/** (Optional) Default width for glyphs in a CIDFont. See
		Section 5.6.3 in the PDF Reference for more information. 
	*/
	ASInt32 dw;

	/** A Cos array of a set of lists that define the widths for the glyphs
		in the CIDFont. Each list can specify individual widths for
		consecutive CIDs, or one width for a range of CIDs. See
		Section 5.6.3 on character widths in CIDFonts in the PDF
		Reference for information on the format of this array. 
	*/
	CosObj w;

	/** If true, the dw2 and w2 fields are used; if false, they are not used. */
	ASBool hasDW2;

	/** (Optional; applies only to CIDFonts that are used for vertical
		writing) The default metric for writing mode 1. This entry is an
		array of two numbers: the y component of the position vector
		and the y component of the displacement vector for writing
		mode 1. The x component of the position vector is always half
		the width of the character. The x component of the displacement
		vector is always 0. The default value is [880 -1000]. For
		information on writing mode 1, see Section 5.6.3 on vertical
		writing in the PDF Reference 
	*/
	ASInt32 dw2[2];

	/** (Optional; applies only to CIDFonts that are used for vertical
		writing) A Cos array defining the metrics for vertical writing. Its
		format is similar to the format of the array in w. It defines the x
		and y components of the position vector, and the y component
		of the displacement vector. The x component of the
		displacement vector is always 0. See Section 5.6.3 on character
		widths in CIDFonts in the PDF Reference for information on the
		format of this array. 
	*/
	CosObj w2;

	/** (Optional) Length of toUnicodeStm. */
	ASInt32 toUnicodeLen;

	/** (Optional) A stream containing a CMap that defines the mapping
		from character codes to Unicode values. This entry is
		recommended for fonts that do not use one of the predefined
		CMaps. If present, this allows strings in the encoding to convert
		to Unicode values for export to other applications or plug-ins.
		For more information, see Section 5.6 on Type 0 fonts in the
		PDF Reference. 
	*/
	ASStm toUnicodeStm;

	/** A stream contain the mapping from CID to glyphindex ("GID").
		The glyphindex for a particular CID value c is a 2-byte value
		stored in bytes 2*c and 2*c+1; the first byte is the high-order byte. 
	*/
	ASStm cidToGidMapStm;

	/** A 12-byte string containing the Family Class ID, Family
		SubClass ID, and 10 bytes for the Panose classification number
		for the font. For additional details on the Panose number, see
		Japanese TrueType Font Property Selection Guidelines by the
		TrueType Conference Technical Committee. 
	*/
	char *panoseNo;	

	/** A Cos dictionary identifying a subset of characters in a CIDFont.
		The values are dictionaries with entries that override the values
		in the FontDescriptor dictionary for the subset of characters.
		See Section 5.6 in the PDF Reference for more information. 
	*/
	CosObj fd;

	/** A stream identifying which CIDs are present in the CIDFont file.
		It is required if the CIDFont file is embedded and only contains a
		subset of the glyphs in the character collection defined by the
		CIDSystemInfo. If this entry is missing, then it is assumed that
		the CIDFont file contains all the glyphs for the character
		collection. The stream’s length should be rounded up to the
		nearest multiple of 8. The bits should be stored in bytes with the
		high-order bit first. Each bit corresponds to a CID. The first bit of
		the first byte corresponds to CID 0, the next bit corresponds to
		CID 1, and so on. If the subset contains a CID, the bit for that
		CID should be set. For compactness, the stream may use one of
		the compression filters to encode the data. For more
		information, see Section 5.6 in the PDF Reference. 
	*/
	ASStm cidSetStm;

	/** One of the PDEFontCreateFlags describing how to embed
		and subset the font. kPDEFontCreateEmbedded, kPDEFontWillSubset, kPDEFontDoNotEmbed 
	*/
	ASUns32 flags;	/* 	*/

	/** Pointer to multiple master font design vector. */
	ASFixed *mmDesignVec;

	/**  */
	ASAtom sourceFontType;	/* K2 */
	} PDEFontCreateParamsRec, *PDEFontCreateParams;


/**
	Data structure used with PDEFont creation. 
	@see PDEFontCreateFromSysFontWithParams 
*/
typedef struct _t_PDEFontCreateFromSysFontParams {

	/** Size of the data structure. Must be set to sizeof(PDEFontCreateFromSysFontParamsRec). */
	ASUns32		structSize;

	/** A bit mask of the PDEFontCreateFlags. */
	ASUns32		flags;

	/** The name of a multiple master snapshot. See PDF Reference
		for more information on snapshots. 
	*/
	ASAtom		snapshotName;

	/** Pointer to multiple master font design vector. */
	ASFixed		*mmDesignVec;

	/** Used to select a specific code page supported by the font. When
		a non-zero code page is supplied, embedding must be turned
		on and the kPDEFontEncodeByGID flag set. 
	*/
	long		ctCodePage;

	/** Used to specify which encoding to use with a CID font. Pass
		ASAtomNull to use the platform default.	 
	*/
	ASAtom		encoding;
	} PDEFontCreateFromSysFontParamsRec, *PDEFontCreateFromSysFontParams;

/**
	Data structure used for adding Unicode text
	@see PDETextAddGlyphs
*/
typedef struct _t_PDEGlyphDescription {

	/** The GlyphID of a glyph being added. */
	ASUns32			glyphID;

	/** Array of indices to Unicode chars, from the uniText member
		of the PDEGlyphRun struct, represented by this glyph.
	*/
	ASInt16			*charIndices;

	/** Count of indices in the charIndices array. */
	ASInt32			charIndicesLen;

	/** X position of where to place this glyph. */
	ASReal			xPosition;

	/** Y position of where to place this glyph. */
	ASReal			yPosition;
	} PDEGlyphDescription, *PDEGlyphDescriptionP;

/**
	Data structure used for adding Unicode text
	@see PDETextAddGlyphs
*/
typedef struct _t_PDEGlyphRun {

	/** Pointer to the array of PDEGlyphDescription items. */
	PDEGlyphDescriptionP	glyphs;

	/** Count of items in PDEGlyphDescription array. */
	ASInt32			glyphLen;

	/** Array of Unicode characters in UTF32HE format. */
	ASUTF32Val		*uniText;

	/** Count of characters in uniText array. */
	ASInt32			uniTextLen;
	} PDEGlyphRun, *PDEGlyphRunP;

/* Utility structures for passing around ActualText information
   related to the new PDETextAddGlyphs API. */

typedef struct _t_PDESpanItem {

	/** Index into the glyphs array in a PDEGlyphRun object. */
	ASInt32			firstGlyph;

	/** Number of glyphs in the span starting from firstGlyph.
		This can be zero, indicating there are no glyphs for
		this Unicode string. The firstGlyph indicates the
		glyph that precedes the Unicode string.
	*/
	ASInt32			nGlyphs;

	/** Index into the uniText array in a PDEGlyphRun object. */
	ASInt32			firstUniChar;

	/** Number of Unicode characters in the span starting from
		firstUniChar. This count can be zero, indicating there
		are no Unicode values for the range of glyphs.
	*/
	ASInt32			nUniChars;
} PDESpanItem, *PDESpanItemP;

typedef struct _t_PDESpanSet {

	/** Pointer to array of spans corresponding to the PDEGlyphRun
		object.
	*/
	PDESpanItemP		spans;

	/** Count of spans in the spans array. */
	ASInt32			spanCount;
} PDESpanSet, *PDESpanSetP;

/*------------------------------------------------------------------------
	PDFEdit Flags and Enums.
------------------------------------------------------------------------*/


/** Types of PDEObject, which is the superclass for PDEContent, PDEElement,
	PDEClip, and so on.
	@see PDEObjectGetType
*/
typedef enum {

	/** PDEContent object */
	kPDEContent,
	

	/** PDEText object */
	kPDEText,

	/** PDEPath object */
	kPDEPath,

	/** PDEImage object */
	kPDEImage,

	/** PDEForm object */
	kPDEForm,

	/** PDEPS object */
	kPDEPS,

	/** PDEXObject object */
	kPDEXObject,
	

	/** PDEClip object */
	kPDEClip,
	

	/** PDEFont object */
	kPDEFont,

	/** PDEColorSpace object */
	kPDEColorSpace,

	/** PDEExtGState object */
	kPDEExtGState,


	/** PDEPlace object */
	kPDEPlace,

	/** PDEContainer object */
	kPDEContainer,


	/** PDESysFont object */
	kPDSysFont,
	

	/** PDEPattern object */
	kPDEPattern,

	/** PDEDevineNColors object */
	kPDEDeviceNColors,


	/** PDEShading object */
	kPDEShading,

	/** PDEGroup object */
 	kPDEGroup,


	/** PDEUnknown object */
	kPDEUnknown,


	/** PDEBeginContainer object */
	kPDEBeginContainer,

	/** PDEEndContainer object */
	kPDEEndContainer,


	/** PDEBeginGroup object */
	kPDEBeginGroup,

	/** PDEEndGroup object */
	kPDEEndGroup,
	

	/** PDEXGroup object */
	kPDEXGroup,

	/** PDESoftMask object */
	kPDESoftMask,


	/** PDSysEcoding object */
	kPDSysEncoding,


	/** PDEDoc object */
	kPDEDoc,

	/** PDEPage object */
	kPDEPage,


	/** PDEReader object */
	kPDEReader,

	/** PDEWriter object */
	kPDEWriter,


	/** PDETextItem object */
	kPDETextItem,


	/** PDEImageFlate object */
	kPDEImageFlate,


	/** PDEImageJPX object */
	kPDEImageJPX,


	/** JPXColorSpace object */
	kJPXColorSpace,


	/** JPXPalette object */
	kJPXPalette,
	
	/** */
	kPDELastType
} PDEType;

/* Handy defines for inserting at the beginning and end of lists */
#define kPDEBeforeFirst	((ASInt32) -1)
#define kPDEAfterLast	(MAXInt32 - 1)

/* Bit field for the PDEContentToCosObj method, indicating the type of object to create
	and how it is created.
	@see PDEContentToCosObj  
*/
typedef enum {

	/** Create page contents. */
	kPDEContentToPage                = 0x0001,

	/** Create a form. */
	kPDEContentToForm                = 0x0002,

	/** Create charprocs. */
	kPDEContentToCharProc            = 0x0004,

	/** Currently unused. */
	kPDEContentRev1Compat            = 0x0008,

	/** Currently unused. */
	kPDEContentDoNotResolveForms     = 0x0010,

	/** Currently unused. */
	kPDEContentDoNotResolveType3     = 0x0020,

	/** Emit calibrated RGB and gray	information using the PDF 1.0
		compatible mechanism. In this case, generate rg and k page operators and
		place DefaultGray and DefaultRGB color space arrays in the Resources
		dictionary, as described in Section 4.5.7 in the PDF Reference. 
	*/
	kPDEContentEmitDefaultRGBAndGray = 0x0040,

	/**  */
	kPDEContentInheritState          = 0x0080,
	
	/** Prevents the emission of the content compatibility operators, BX/EX, which
        cause issues for some verstions of PDF/A or X
    */
	kPDEContentDoNotEmitBXEX         = 0x0100

	} PDEContentToCosObjFlags;
	

/** Bit field for PDEEnumElements method.	
	@see PDEEnumElements
*/
typedef enum {

	/** Indicates whether Marked Content is ignored in the enumeration. This may
		be useful when generating elements purely for display purposes. 
	*/
	kPDEContentIgnoreMarkedContent = 0x0001
} PDEEnumElementsFlags;


/** Bit field for PDEContentAttrs.
	@see PDEContentGetResources
*/
typedef enum {

	/** Obtain font resources. */
	kPDEGetFonts,

	/** Obtain Xobject resources. */
	kPDEGetXObjects, 

	/**
		Obtain color space resources.
	*/
	kPDEGetColorSpaces
	} PDEContentGetResourceFlags;
	

/** Bit field for PDEContentAttrs.
	@see PDEContentGetAttrs
	@see PDEContentToCosObj
*/
typedef enum {

	/** if set, cacheDevice contains 6 cache device values */
	kPDESetCacheDevice	= 0x0001,	

	/** if set, cacheDevice contains 2 charwidth values */
	kPDESetCharWidth	= 0x0002,	

	/** if set, formMatrix contains a valid matrix */
	kPDEFormMatrix		= 0x0004	
	} PDEContentFlags;
	

/** Bit field used in PDEText methods.
	@see PDETextAdd
	@see PDETextGetFont
	@see PDETextGetText
*/
typedef enum {

	/** Text run. */
	kPDETextRun = 0x0001, 

	/** Character (text run with only one character). */
	kPDETextChar = 0x0002,

	/** Obtain the advance width in page space. */
	kPDETextPageSpace = 0x0004,

	/** Fill in the left and right bounds of the text run’s bounding box. */
	kPDETextGetBounds = 0x0008
	} PDETextFlags;


/** Flags indicating text rendering mode set by the Tr operator.
	@see PDETextCreate
*/	
typedef enum {

	/** Fill text. */ 
	kPDETextFill,

	/** Stroke text. */ 
	kPDETextStroke,

	/** Fill and stroke text. */ 
	kPDETextFillAndStroke,

	/**
		Text with no fill and no stroke (invisible).
	*/ 
	kPDETextInvisible
	} PDETextRenderMode;
	 

/** Enumerated data type for path segment operators in PDEPath elements.
	@see PDEPathAddSegment
	@see PDEPathCreate
	@see PDEPathSetData
*/
typedef enum {

	/** Designates m (moveto) operator, which moves the current point. */
	kPDEMoveTo,

	/** Designates l (lineto) operator, which appends a straight line
		segment from the current point. 
	*/
	kPDELineTo,

	/** Designates c (curveto) operator, which appends a Bezier curve
		to the path. 
	*/
	kPDECurveTo,

	/** Designates v (curveto) operator, which appends a Bezier curve
		to the current path when the first control point coincides with
		initial point on the curve. 
	*/
	kPDECurveToV,

	/** Designates y (curveto) operator, which appends a Bezier curve
		to the current path when the second control point coincides with
		final point on the curve. 
	*/
	kPDECurveToY,

	/** Designates re operator, which adds a rectangle to the current path. */
	kPDERect,

	/** Designates h (closepath) operator, which closes the current sub-path. */
	kPDEClosePath,

	/** */
	kPDEPathLastType
	} PDEPathElementType;
	

/** Flags for paint operators in a PDEPath.
	@see PDEPathGetPaintOp
	@see PDEPathSetPaintOp
*/
typedef enum {

	/** Path is neither stroked nor filled, so it is invisible. */
	kPDEInvisible			= 0x00,

	/** Stroke the path, as with the S (stroke) operator. */
	kPDEStroke				= 0x01,

	/** Fills the path, using the nonzero winding number rule to
		determine the region to fill, as with the f (fill) operator. 
	*/
	kPDEFill				= 0x02,

	/** Fills the path, using the even–odd rule to determine the region to
		fill, as with the f* (eofill) operator. 
	*/
	kPDEEoFill				= 0x04
	} PDEPathOpFlags;


/** Flags for PDEImageAttrs. See Section 4.8.4 in the PDF Reference for more
	information on image attributes.
	@see PDEImageCreate
	@see PDEImageGetAttrs
	@see PDEImageGetData
*/
typedef enum {

	/** image is an XObject */
	kPDEImageExternal		= 0x0001,	

	/** image is an imagemask */
	kPDEImageIsMask			= 0x0002,	

	/** interpolate is true */
	kPDEImageInterpolate	= 0x0004,	

	/** we have a decode array */
	kPDEImageHaveDecode		= 0x0008,	

	/** uses an indexed color space */
	kPDEImageIsIndexed		= 0x0010,	

	/** image has a Mask key containing an ImageMask stream */
	kPDEImageMaskedByPosition	= 0x0020,	

	/** image has a Mask key containing an array of color values */
	kPDEImageMaskedByColor		= 0x0040	
	} PDEImageAttrFlags;


/** Flags for PDEImageGetData, PDEImageGetDataStm, PDEImageSetData, and
	PDEImageSetDataStm.
	@see PDEImageGetData
	@see PDEImageGetDataStm
	@see PDEImageSetData
	@see PDEImageSetDataStm
	@see PDEImageCreate
*/
typedef enum {

	/** Indicates filter is active; data is encoded. */
	kPDEImageEncodedData = 0x0001,
	kPDEImageAllowDelayedRead = 0x0002
	} PDEImageDataFlags;
	

/** Flags for PDEPSAttrs */
typedef enum {

	/** PS is an XObject */
	kPDEPSExternal			= 0x0001	
	} PDEPSFlags;


/** Bitfield for PDEElementCopy 
	@see PDEElementCopy
*/
typedef enum {

	/** copied element does not need gstate or clip */
	kPDEElementCopyForClip = 0x0001,	

	/** acquire the clip path and put it in the copied object */
	kPDEElementCopyClipping = 0x0002	
	} PDEElementCopyFlags;


/** Flags for PDEFontCreateFromSysFont.
	If you want to subset a font, set both the
	kPDEFontCreateEmbedded and kPDEFontWillSubset flags.
	@see PDEFontCreateFromSysFont
	@see PDEFontCreateFromSysFontAndEncoding
	@see PDEFontCreateFromSysFontWithParams
	@see kPDEFontWillSubset
 */
typedef enum {

	/** Embed the font. Create an embedded font. By itself, this will not subset the font.*/
	kPDEFontCreateEmbedded		= 0x0001,		

	/** Subset the font. If you want to subset a font, set both the kPDEFontCreateEmbedded
		and kPDEFontWillSubset flags. You must call PDEFontSubsetNow to actually subset
		the font. Both embedding and sub-setting a font creates a CFF font. 
	*/
	kPDEFontWillSubset			= 0x0002,		

	/** Do not embed the font. You cannot set both
		this and the kPDEFontWillSubset flags.
		Nor can you set kPDEFontCreateEmbedded.
	*/
	kPDEFontDoNotEmbed			= 0x0004,		

	/** Create a CIDFont with identity (GID) encoding */
	kPDEFontEncodeByGID			= 0x0008,		

	/** Wait to get widths until later (affects Type0 fonts only) */
	kPDEFontDeferWidths			= 0x0010,		

	/** */
	kPDEFontCreateSubset		= kPDEFontWillSubset,

	/** PDFLib will convert cp to gid with identity embedded */
	kPDEFontCreateGIDOverride	= 0x0020,		

	/** Create ToUnicode cmap */
	kPDEFontCreateToUnicode		= 0x0040,		

	/** Supply entire widths table (affects Type0 fonts only) */
	kPDEFontCreateAllWidths		= 0x0080,		

	/** Embed an OpenType style font subset if appropriate */
	kPDEFontCreateEmbedOpenType	= 0x0100		
} PDEFontCreateFlags;


/** Flags for PDEFontGetCreateNeedFlags */
typedef enum {

	/** need to create width */
	kPDEFontCreateNeedWidths	= 0x00010000,	

	/** need to create ToUnicode stream */
	kPDEFontCreateNeedToUnicode	= 0x00020000,	

	/** need to embed it */
	kPDEFontCreateNeedEmbed		= 0x00040000	
} PDEFontCreateNeedFlags;


/** Flags for protection of PDEFontAttrs - embedding is not allowed */
#define kPDEFontNoEmbedding				0x00000001	

/** Flags for protection of PDEFontAttrs - editable embedding is not allowed */
#define	kPDEFontNoEditableEmbedding		0x00000002

/* Flags for packageType of PDEFontAttrs */
typedef enum {
	kPDSysFontUnknown = 0,
	kPDSysFontType1,
	kPDSysFontTrueType,
	kPDSysFontCID,
	kPDSysFontATC,
	kPDSysFontOCF,
	kPDSysFontOpenTypeCFF,
	kPDSysFontOpenTypeCID,
	kPDSysFontOpenTypeTT
} PDSysFontPackageType;


/** Flags for use with PDESoftMaskCreate.
	@see PDESoftMaskCreate
*/
typedef enum {

	/** Specifies how the mask is to be computed. */
    kPDESoftMaskTypeLuminosity  = 0x0001,

	/** Specifies how the mask is to be computed. */
    kPDESoftMaskTypeAlpha       = 0x0002
	} PDESoftMaskCreateFlags;	


/** Enumerated data type used to specify the type of transparency group to create.
	@see PDESoftMaskCreate
*/
typedef enum {

	/** Creates a transparency XGroup object. */
    kPDEXGroupTypeTransparency  = 0x0001
	} PDEXGroupCreateFlags;	


/**
	Callback for PDEEnumElements. It is called once for each 
	PDEElement in a page's Contents Stream or Resources dictionary. 
	
	@param elem The PDEElement. 
	@param clientData User-supplied data that was specified 
	in the call to PDEEnumElements.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see PDEEnumElements 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEElementEnumProc)(
			IN	PDEElement elem,
			IN	void *clientData);

/**
	Callback for PDEClipFlattenedEnumElems, which enumerates 
	all of a PDEClip's PDEElements in a flattened manner. 
	@param elem IN/OUT? The PDEElement currently being enumerated. 
	
	@param clientData IN/OUT? User-supplied data that was passed in 
	the call to PDEClipFlattenedEnumElems. 
	@return If false, enumeration halts. If true, enumeration continues. 
	
	@see PDEClipFlattenedEnumElems 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEClipEnumProc)(
			IN	PDEElement elem,
			IN	void *clientData);

/*------------------------------------------------------------------------
	PDFEdit Dump.
------------------------------------------------------------------------*/


/**
	Callback for PDELogDump or PDEObjectDump. It is called once 
	for each PDEObject, its children, and their attributes for 
	the specified number of levels. 
	@param obj IN/OUT? The PDEObject. 
	@param dumpInfo IN/OUT? Contains information about an object. Information 
	fields are delimited by tabs. There are no newline characters 
	in this string. 
	@param clientData IN/OUT? User-supplied data that was specified 
	in the call to PDELogDump or PDEObjectDump. 
	@see PDELogDump 
	@see PDEObjectDump 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDEObjectDumpProc)(
			IN	PDEObject obj,
			IN	char *dumpInfo,
			IN	void *clientData);

/**
	Callback for PDEAttrEnumTable. It is called once for each 
	attribute in a table. 
	@param attrHdrP An opaque pointer to the attribute. The 
	actual attribute type is not specified in this function, 
	since the storage mechanism only knows the size of the attribute, 
	not its type. 
	@param refCount Reference count of the attribute. 
	@param size Size of attrHdrP, in bytes. 
	@param clientData User-supplied data that was specified 
	in the call to PDEAttrEnumTable.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see PDEAttrEnumTable 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEAttrEnumProc)(
			IN	void *attrHdrP,
			IN	ASUns32 refCount,
			IN	ASUns16 size,
			IN	void *clientData);
/*------------------------------------------------------------------------
	PDFEdit Colorspace Struct.
------------------------------------------------------------------------*/

/** */
typedef struct {
    float min;
    float max;
} PDEColorRangeFlt;
/** */
typedef struct {
    float x;
    float y;
    float z;
} PDEXYZColorFlt;
/**
	Structure describing a white point in a calibrated color 
	space. 
	@see PDEColorSpaceCreate
*/
typedef PDEXYZColorFlt PDEWhitePointFlt;
/**
	Structure describing a black point in a calibrated color 
	space. 
	@see PDEColorSpaceCreate
*/
typedef PDEXYZColorFlt PDEBlackPointFlt;


/**
	Structure describing a CalGray color space. 

	Default PDEGrayCalFlt: PDEGrayCalFlt calGray = {{0, 0, 0}, 
	{0, 0, 0}, 1}; 
	@see PDEColorSpaceCreate
*/
typedef struct _t_PDEGrayCalFlt {

	/** */
    PDEWhitePointFlt 	whitePoint;

	/** */
    PDEBlackPointFlt 	blackPoint;

	/** */
    float				gamma;
} PDEGrayCalFlt;


/**
	Structure describing a CalRGB color space. Same as AGMRGBCalFlt 
	(Only available as part of the PDF Library SDK). 

	Default is {{0, 0, 0}, {0, 0, 0}, 1, 1, 1, {1, 0, 0, 0, 
	1, 0, 0, 0, 1}}; 
	@see PDEColorSpaceCreate
*/
typedef struct _t_PDERGBCalFlt {

	/** */
	PDEWhitePointFlt	whitePoint;

	/** */
    PDEBlackPointFlt	blackPoint;

	/** */
    float 				redGamma;

	/** */
    float 				greenGamma;

	/** */
    float 				blueGamma;

	/** */
    float 				matrix[9];
} PDERGBCalFlt;


/**
	Structure describing a L*a*b* color space. 

	Default is {{0, 0, 0}, {0, 0, 0}, {-100, 100}, {-100, 100}}; 
	
	@see PDEColorSpaceCreate
*/
typedef struct _t_PDELabCalFlt {

	/** */
    PDEWhitePointFlt	whitePoint;

	/** */
    PDEBlackPointFlt	blackPoint;

	/** */
    PDEColorRangeFlt	rangeA, rangeB;
} PDELabCalFlt;

/* DeviceGray, DeviceRGB, DeviceCMYK - pass NULL for PDEColorSpaceStruct */



/**
	A PDEColorSpace that describes a Pattern color space. 
	@see PDEColorSpaceCreate
*/
typedef PDEColorSpace PDEPatternColorSpace;


/** ICCBased ColorSpace */
typedef struct _t_PDEICCBasedColorData {

	/** size = sizeof(PDEICCColorData) */
	ASSize_t		size;			

	/** stream containing ICC Profile */
	ASStm			iccstream;		

	/** Number of color components (1, 3, or 4) */
	ASUns32			nComps;			

	/** Alternate ColorSpace (optional) */
	PDEColorSpace	altCs;			
} PDEICCBasedColorData;


/**
	Structure describing an indexed color space. 
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDEIndexedColorData {

	/** size = sizeof(PDEIndexedColorData) */
	ASSize_t		size;			

	/** Base colorspace. */
	PDEColorSpace	baseCs;			

	/** Highest color value. */
	ASUns16			hival;			

	/** Indexed color lookup data. */
	char			*lookup;		

	/** Number of bytes in lookup data. */
	ASUns32			lookupLen;		
} PDEIndexedColorData;


/**
	Structure describing a separation color space. 
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDESeparationColorData {

	/** Size of the data structure. Must be set to 
		sizeof(PDESeparationColorData). 
	*/
	ASSize_t		size;			

	/** Name of separation or colorant. */
	ASAtom			name;			

	/** Alternative colorspace. */
	PDEColorSpace	alt;			

	/** The tintTransform dictionary or function. 
		See Section 4.5.5 in the PDF Reference. 
	*/
	CosObj			tintTransform;	
} PDESeparationColorData;


/**
	Structure describing a DeviceRGB or DeviceCMYK color space. 
	
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDEDeviceNColorData {

	/** Size of the data structure. Must be set to 
		sizeof(PDEDeviceNColorData). 
	*/
	ASSize_t		size;			

	/** Names of colorants. */
	ASAtom			*names;			

	/** Number of colorants. */
	ASUns32			nNames;			

	/** Alternative colorspace. */
	PDEColorSpace	alt;			

	/** The tintTransform dictionary or function. 
		See Section 4.5.5 in the PDF Reference for more information. 
	*/
	CosObj			tintTransform;	
} PDEDeviceNColorData;


/* A color space structure for PDEColorSpaceCreate. See Section 4.5 in the PDF Reference
	for information on color spaces.
	@see PDEColorSpaceCreate
*/
typedef union {

	/** */
	PDEGrayCalFlt			*calGray;

	/** */
	PDERGBCalFlt			*calRGB;

	/** */
	PDELabCalFlt			*lab;

	/** */
	PDEICCBasedColorData	*icc;

	/** */
	PDEIndexedColorData		*indexed;

	/** */
	PDEPatternColorSpace	patternbase;

	/** */
	PDESeparationColorData	*sep;

	/** */
	PDEDeviceNColorData		*devn;
} PDEColorSpaceStruct;

/* codePage supported for PDSysEncodingCreateFromCodePage */
enum	{
		/* Windows code pages	*/
	kPDCodePageWinEastEuropeanRoman		=		1250,
	kPDCodePageWinCyrillic				=		1251,
	kPDCodePageWinGreek					=		1253,
	kPDCodePageWinTurkish				=		1254,
	kPDCodePageWinHebrew				=		1255,
	kPDCodePageWinArabic				=		1256,
	kPDCodePageWinBaltic				=		1257,
	
		/* Macintosh pseudo code pages	*/
	kPDCodePageMacCentralEuropean		=		-9994,
	kPDCodePageMacCroatian				=		-9993,
	kPDCodePageMacRomanian				=		-9992,
	kPDCodePageMacCyrillic				=		-9991,
	kPDCodePageMacUkrainian				=		-9990,
	kPDCodePageMacGreek					=		-9989,
	kPDCodePageMacTurkish				=		-9988,
	kPDCodePageMacHebrew				=		-9987,
	kPDCodePageMacArabic				=		-9986
};



/*
**   Attribute definitions for JPX objects
*/

/**
** Attributes of a JPX image
*/
typedef struct _t_PDEImageJPXAttrs {
   ASUns32 flags;    /* PDEImageAttrFlags */
   ASInt32 width;
   ASInt32 height;
   ASInt32 tileWidth;
   ASInt32 tileHeight;
   ASInt32 nResolutions;
   ASInt32 nComponents;
   ASInt32 bpc[24];
   ASFixed decode[24];
} PDEImageJPXAttrs, *PDEImageJPXAttrsP;

    

/*
** JPX Color Space types
*/
typedef enum {
    kJPXCSUnknown              = 0x0000,
    kJPXCSEnumerated           = 0x0001,
    kJPXCSRestrictedICC        = 0x0002,
    kJPXCSAnyICC               = 0x0003,
    kJPXCSVenderColor          = 0x0004
} JPXColorSpaceType;	


/**
** Attributes of a JPXCSEnumerated JPXColorSpace.
*/
typedef struct _t_JPXCSEnumAttrs {
	/** enumerated color space number */
	ASUns32 enumNumber;        
	/** standard illuminant is used   */
	ASBool  stdIlluminant;     
	/** 4 byte illuminant value       */
	ASUns32 illuminant;        
	/** L, A, and B range values      */
	ASUns32 labRange[3];       
	/** L, A, and B offset values     */
	ASUns32 labOffset[3];      
} JPXCSEnumAttrs, *JPXCSEnumAttrsP;


#ifdef __cplusplus
}
#endif


#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_PEExpT */


