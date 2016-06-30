/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDExpT.h

 - Types, macros, structures, etc. required to use the PDModel HFT.

*********************************************************************/

#ifndef _H_PDExpT
#define _H_PDExpT

#include "ASExpT.h"
#include "CosExpT.h"
#include "PDBasicExpT.h"
#include "ASExtraExpT.h"

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

/**
	A 0-based page number for use in AVPageView and AVDoc methods. 
	Negative for special values. 
	@see AVDocGetPageText 
	@see AVDocGetViewDef 
	@see AVPageViewGetFirstVisiblePageNum 
	@see AVPageViewGetLastVisiblePageNum 
	@see AVPageViewGetNextView 
	@see AVPageViewGetPageNum 
	@see AVPageViewGetSelectedAnnotPageNum 
	@see AVPageViewGetVisibleAnnotPage 
	@see AVPageViewGoTo 
	@see AVPageViewPageNumIsVisible 
	@see AVPageViewSetPageNum 
	@see AVDocSelectionGetAVRectProc 
	@see AVSelectionPageRangeEnumProc
*/
typedef ASInt32 PDPageNumber; 

/**
	A flag value for use in PDDocInsertPagesParams. 
	@see PDDocInsertPages
*/
typedef ASUns16 PDSmallFlagBits;

/** 
	unsigned measurement of a font characteristic (e.g. width) 
*/
typedef ASUns16 PDFontMetric; 

/**
	An italic angle value in degrees, for use in PDFontMetrics. 
*/
typedef ASInt16 PDFontAngle; 

/**
	A font offset value, for use in PDFontMetrics. 
*/
typedef ASInt16 PDFontOffset;

/**
	A font metric value (never negative), for use in PDFontMetrics. 	
*/
typedef ASInt16 PDiFontMetric; 

/**
	A signed measurement of an image offset, for use in PDImageAttrs. 	
*/
typedef ASInt32 PDImageScalar; 

/** signed int for historical reasons, never negative */
typedef ASInt16 PDDocVersion; 

/**
	signed int for historical reasons, never negative.
	@see PDDocClearFlags 
	@see PDDocGetFlags 
	@see PDDocSetFlags 
*/
typedef ASInt32 PDDocFlags; 

/** */
typedef ASUns16 PDCharOffset;

/**
	A numeric count value for use in PDImageAttrs. 
*/
typedef ASInt32 PDCount; 


/************************************************************************************\
|*																					*|
|*									PDAction										*|
|*																					*|
\************************************************************************************/

/**
	Actions are what happens when a user clicks on a link or bookmark. In addition, the
	Acrobat viewer allows a document to have an action that is executed automatically
	when the document is opened. Applications can also support actions in custom
	annotation types they add.

	@see PDActionNew
	@see PDActionNewFromDest
	@see PDActionNewFromFileSpec
	@see PDLinkAnnotGetAction
	@see PDBookmarkGetAction
	@see PDDocGetOpenAction
	@see PDActionDestroy
*/
typedef OPAQUE_64_BITS PDAction;
/* Forward declarations */
typedef struct _t_PDActionHandler *PDActionHandler;
typedef struct _t_PDActionClipboardData *PDActionClipboardData;

/**
	Used to store PDAction data for copy and paste operations. 
	
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc
*/
typedef void *PDActionHandlerData;

/**
	Callback for PDActionHandler. Returns an ASAtom indicating 
	the action type for which the handler is responsible. Types 
	are defined by the client when registering the action handler. 
	
	@param pdah The action handler whose type is returned.
	@return The action type for which this handler is responsible. 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PDActionHandlerGetTypeProc)(PDActionHandler pdah);

/**
	(Optional) Callback for PDActionHandler. Returns true if 
	the copy operation is expected to succeed. Tests, for example, 
	whether copying is allowed by document permissions. 

	NOTE: The handler is not expected to test other actions 
	(if any) in the action chain. 
	@param pdah The action handler. 
	@param action The action object.
	@return true if the action can be copied, false if it cannot. 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCanCopy 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDActionHandlerCanCopyProc)(
	PDActionHandler pdah, PDAction action);

/**
	(Optional) Callback for PDActionHandler. Copies data from 
	an action object to a new data structure, from which it 
	can be pasted to a new document. The PDActionHandlerData 
	does not store any information related to a Next action. 
	Rebuilding the action chain is the responsibility of the 
	caller and can be ignored by the PDActionHandler. 
	@param pdah The action handler. 
	@param action The action object.
	@return The new structure containing the action data. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCopy 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 PDActionHandlerData(ACCBPROTO2 *PDActionHandlerCopyProc)(
	PDActionHandler pdah, PDAction action);

/**
	(Optional) Callback for PDActionHandler. Returns true if 
	the paste operation is expected to succeed. Tests, for example, 
	whether pasting is allowed by document permissions. 

	NOTE: The handler is not expect to test other actions (if 
	any) in the action chain. 
	@param pdah The action handler. 
	@param dest The destination document. 
	@param data The action data to test.
	@return true if the action can be pasted to the document, false 
	if it cannot. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCanPaste 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDActionHandlerCanPasteProc)(
	PDActionHandler pdah, PDDoc dest, PDActionHandlerData data);

/**
	(Optional) Callback for PDActionHandler. Creates a new PDAction 
	in the destination document using data that was placed in 
	a PDActionClipboardData structure using the PDActionCopy 
	function, and returns the new action object. 
	@param pdah The action handler. 
	@param dest The destination document. 
	@param data The action data to paste.
	@return The new action object containing the pasted data and associated 
	with the document. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionPaste 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 PDAction (ACCBPROTO2 *PDActionHandlerPasteProc)(
	PDActionHandler pdah, PDDoc dest, PDActionHandlerData data);

/**
	Callback for PDActionHandler. Destroys data copied into 
	an action clipboard data structure after it has been successfully 
	pasted to a new document. 
	@param pdah The action handler. 
	@param data The action data to destroy.
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerPasteProc 
	@see PDActionPaste 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDActionHandlerDestroyDataProc)(
	PDActionHandler pdah, PDActionHandlerData data);

/**
	(Optional) Callback for PDActionHandler. Destroys the action 
	handler structure when the application no longer requires 
	it. The handler should destroy any dynamic memory stored 
	in the userData field. 
	@param pdah The action handler to destroy.
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDActionHandlerDestroyProc)(
	PDActionHandler pdah);

/**
	Data structure containing callbacks that implement an action 
	manager. The callbacks provide copy-and-paste functionality 
	for a particular type of action. 
	@see PDRegisterActionHandler 
*/
typedef struct _t_PDActionHandler {

	/** Size of the data structure. Must be set to sizeof(PDAnnotHandlerRec). */
	ASSize_t size;

	/** Pointer to a block of user-supplied data. */
	void *userData;

	/** */
	PDActionHandlerGetTypeProc GetType;

	/** */
	PDActionHandlerCanCopyProc CanCopy;

	/** */
	PDActionHandlerCopyProc Copy;

	/** */
	PDActionHandlerCanPasteProc CanPaste;

	/** */
	PDActionHandlerPasteProc Paste;

	/** */
	PDActionHandlerDestroyDataProc DestroyData;

	/** */
	PDActionHandlerDestroyProc Destroy;
} PDActionHandlerRec;

/************************************************************************************\
|*																					*|
|*							PDAnnot, PDTextAnnot, PDLinkAnnot						*|
|*																					*|
|*	Opaque designators for PDF annotation objects.
|*	You may freely copy or destroy an instance of PDAnnot.
|*	Two instances of a PDAnnot designating the same annotation are guaranteed to
|*	contain identical bits.															*|
|*																					*|
\************************************************************************************/

/**
	An annotation on a page in a PDF file. Acrobat viewers have two built-in annotation
	types: PDTextAnnot and PDLinkAnnot. Physical attributes of the annotation can
	be set and queried. Plug-ins add movie and Widget (form field) annotations.
	Developers can define new annotation subtypes by creating new annotation handlers.
	
	@see AVPageViewIsAnnotAtPoint
	@see PDAnnotFromCosObj
	@see PDPageAddNewAnnot
	@see PDPageCreateAnnot
	@see PDPageGetAnnot
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDAnnot;

/**
	A PDF text annotation on a page in a PDF file. You can use any PDAnnot method on
	a PDTextAnnot.
	Applications can:
	
	- Get and set attributes including the rectangle, textual contents, and whether or not
	the annotation is open.
	- Create new text annotations and delete or move existing ones using PDAnnot
	methods.
	- Manipulate the behavior of text annotations by modifying the Text Annotation
	Handler.
	
	To obtain, use any of the PDAnnot calls, followed by CastToPDTextAnnot. The annotation
	passed to CastToPDTextAnnot must be a text annotation, it will not convert other
	annotation types into text annotations.
	
	@see CastToPDTextAnnot 
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDTextAnnot;

/**
	A link annotation on a page in a PDF file. You can use any PDAnnot method on a
	PDLinkAnnot. Applications can:
	
	- Get and set the bounding rectangle and color, using PDAnnot methods.
	- Get and set the action that occurs when the link is activated, and the link's border,
	using PDLinkAnnot methods.
	- Create new link annotations and delete existing ones, using the PDPage methods.

	To obtain, use any of the PDAnnot calls, followed by CastToPDLinkAnnot. The annotation passed
	to CastToPDLinkAnnot must be a link annotation: other annotation types are not
	converted into link annotations.
		
	@see CastToPDLinkAnnot
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDLinkAnnot;

/**@name PDAnnotFlags
Flags for PDAnnotSet/GetFlags corresponding to an annotation object's F key. 
*/
/*@{*/

/** If no annot handler, annot is invisible. */
#define pdAnnotInvisible		0x01		

/** Annot is not visible and does not print. */
#define pdAnnotHidden			0x02		

/** Annot prints. */
#define pdAnnotPrint			0x04		

/** Annot does not zoom with the view. */
#define pdAnnotNoZoom			0x08		

/** Annot does not rotate with page. */
#define pdAnnotNoRotate			0x10		

/** Annot does not view but can print. */
#define pdAnnotNoView			0x20		

/** Annot does not interact with the user. */
#define pdAnnotReadOnly			0x40		

/** Annot does not move, resize with the view. Currently only form fields respect this flag.
	If the annot is locked, the user can not delete, move and change its associated form field's properties.
*/
#define pdAnnotLock				0x80		

/** Mouse over / selection causes the noView bit to toggle */
#define pdAnnotToggleNoView		0x100		

/** place holder, used only at runtime - do not set this bit in PDF/FDF files */
#define pdAnnotSequenceAdjust	0x80000000	
											
/*@}*/

/**
	Casts a link annotation or a text annotation to a generic 
	annotation. 
	@param a The link annotation or text annotation to cast.
	@see CastToPDLinkAnnot 
	@see CastToPDTextAnnot 
*/
#define CastToPDAnnot(a) *(PDAnnot *)&(a)
/**
	Casts a link annotation or a generic annotation to a text 
	annotation. 
	@param a The link annotation or generic annotation to 
	cast.
	@see CastToPDAnnot 
	@see CastToPDLinkAnnot 
*/
#define CastToPDTextAnnot(a) *(PDTextAnnot *)&(a)
/**
	Casts a generic annotation or a text annotation to a link 
	annotation. 
	@param a The generic annotation or text annotation to 
	cast.
	@see CastToPDAnnot 
	@see CastToPDTextAnnot 
*/
#define CastToPDLinkAnnot(a) *(PDLinkAnnot *)&(a)

/** @name AnnotationOperations
	Operations allowed on this annotation. 
*/
/*@{*/

/** OK to summarize annotations*/
#define PDAnnotOperationSummarize	0x0001 

/** OK to filter annotations*/
#define PDAnnotOperationFilter	0x0002 

/** OK to manage annotations*/
#define PDAnnotOperationManager	0x0004 

/** Allow modifying this annotation type in a write-protected document*/
#define PDAnnotIgnorePerms		0x0008 

/** All operations are allowed. */
#define PDAnnotOperationAll		0xFFFF
/*@}*/

/**
	Information associated with an annotation. 
	@see PDAnnotHandlerDeleteAnnotInfoProc 
	@see PDAnnotHandlerGetAnnotInfoProc 
	@see PDRegisterAnnotHandler 
*/
typedef struct _PDAnnotInfoRec {
	/** Size of the data structure. Must be set to sizeof(PDAnnotInfoRec). */
	ASSize_t size;

	/** Operations allowed.
		@see AnnotationOperations
	*/
	ASFlagBits nOperationFlags;

	/** Author of this annotation. */
	unsigned char *cAuthor;	

	/** Associated text for this annotation. */
	unsigned char *cContents;

	/** Modification date of this annotation. */
	unsigned char *cDate;	

	/** The layer of this annot */
	ASFixed fxLayer;			
} PDAnnotInfoRec, *PDAnnotInfo;

/* It is assumed that the cAuthor and cContents passed back by the call
** will be in either PDFDocEncoding or UnicodeEncoding (with the 0xFEFF
** prefix). The date should be the standard date format as specified in
** section 7.2, "Date", of the PDF Reference Manual. */
#define PDAnnotInfoInit(x)				\
	do { \
	ASmemset(x, 0, sizeof(PDAnnotInfoRec));	\
	x->size = sizeof(PDAnnotInfoRec); \
	x->fxLayer = fixedTwo; \
	} while (0)

/**
	Data structure containing callbacks that implement an annotation 
	manager. The callbacks implement the annotation manager 
	functions. For example, view, delete, or export the annotations 
	of a document as a list, sorted by type, author, or date. 
	
	To fully use a PDAnnotHandler, the AVAnnotHandler associated 
	with this annotation must have its AVAnnotHandlerGetInfoProc 
	and AVAnnotHandlerDeleteInfoProc callbacks defined. 
	@see PDRegisterAnnotHandler 
*/
typedef struct _t_PDAnnotHandler *PDAnnotHandler;

/**
	Used to store PDAnnot data for copy and paste operations. 
	
	@see PDAnnotCanPaste 
	@see PDAnnotCopy 
	@see PDAnnotDestroyClipboardData 
	@see PDAnnotPaste
*/
typedef struct _t_PDAnnotClipboardData *PDAnnotClipboardData;

/** PDAnnotHandlerClipboardData is opaque data used by PDAnnotHandlers. */
typedef void *PDAnnotHandlerClipboardData;

/**
	PDAnnotPrintOp is passed to the PDAnnotHandlerGetPrintAppearanceProc
	callback to specify the type of print operation being performed.
	
	@see PDAnnotHandlerGetPrintAppearanceProc
*/
enum {

	/** Standard print operation */
	kPDAnnotPrintStandard = 1,	

	/** User selected "Form Fields Only" */
	kPDAnnotPrintVariableData	
};
typedef ASEnum16 PDAnnotPrintOp;

/**
	Callback for PDAnnotHandler. Gets the annotation information
	for an annotation.
	@param pdanh IN/OUT? The annotation handler responsible for this
	annotation.
	@param pdan IN/OUT? The annotation for which information is obtained.

	@param pdpage IN/OUT? The page associated with the annotation for
	which information is obtained. If the page associated with
	the annotation is not known, pass NULL.
	@return Annotation information, described in a PDAnnotInfo structure.

	@see AVAnnotHandlerGetInfoProc
	@see PDAnnotHandlerDeleteAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 PDAnnotInfo (ACCBPROTO2 *PDAnnotHandlerGetAnnotInfoProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, PDPage pdpage);

/**
	(Optional) Callback for PDAnnotHandler. Deletes information
	associated with an annotation. Frees all the memory associated
	with the annotation information.
	@param pdanh IN/OUT? The annotation handler responsible for this
	annotation.
	@param info IN/OUT? Information associated with the annotation.

	@see AVAnnotHandlerDeleteInfoProc
	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDeleteAnnotInfoProc)(
	PDAnnotHandler pdanh, PDAnnotInfo info);

/**
	Callback for PDAnnotHandler. Gets an ASAtom indicating the
	annotation type for which the handler is responsible. This
	corresponds to the annotation's Subtype key in the PDF file.

	@param pdanh IN/OUT? The annotation handler whose type is returned.

	@return The annotation type for which this handler is responsible.

	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PDAnnotHandlerGetTypeProc)(PDAnnotHandler pdanh);

/**
	Callback for PDAnnotHandler. Determines whether an annotation
	is exported or not.

	NOTE: This is a different callback than PDDocWillImportAnnotCallback.

	@param pdanh IN/OUT? The annotation handler of the annotation type
	to export.
	@param src IN/OUT? The annotation that may be exported.
	@param dst IN/OUT? Copy of src, which is actually exported.
	@return true to export dst, false to not export dst.
	@see PDAnnotWillPrintProc
	@see PDDocWillImportAnnotProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillExportAnnotProc)(PDAnnotHandler pdanh,
	PDAnnot src, PDAnnot dst);

/**
	Callback for PDAnnotHandler. Determines whether an annotation
	will be imported or not.

	NOTE: This is a different callback than PDDocWillImportAnnotCallback.

	@param pdanh IN/OUT? The annotation handler of the annotation type
	to import.
	@param doc IN/OUT? The document into which annotations may be imported.
	@param pdpage IN/OUT? The page in which the annotation may be imported.
	@param annot IN/OUT? The annotation that may be imported.
	@return true to import annot, false to not import annot.
	@see PDAnnotWillPrintProc
	@see PDDocWillExportAnnotProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillImportAnnotProc)(PDAnnotHandler pdanh,
	PDDoc doc, PDPage pdpage, PDAnnot annot);

/**
	Callback for PDAnnotHandler. This method is called to determine 
	whether an annotation is printed or not. 
	@param pdanh The annotation handler of the annotation 
	type to print. 
	@param annot The annotation to print.
	@return true if the annotation is printed, false if the annotation 
	is not printed. 
	@see PDDocWillExportAnnotProc 
	@see PDDocWillImportAnnotProc 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotWillPrintProc)(PDAnnotHandler pdanh,
	PDAnnot annot);

/**
	Callback for PDAnnotHandler. Gets the annotation handler
	info flags, which indicate the operations allowed with annotations
	of this type.
	@param pdanh IN/OUT? The annotation handler responsible for the
	annotation.
	@param pdan IN/OUT? The annotation.
	@return Operations allowed. Is an OR of the following flags:
	
	-  PDAnnotOperationSummarize - OK to summarize annotations 
	-  PDAnnotOperationFilter - OK to filter annotations 
	-  PDAnnotOperationManager - OK to manage annotations 
	-  PDAnnotIgnorePerms - Allow modifying this annotation type
	in a writeprotected document 
	-  PDAnnotOperationAll - All operations are allowed 
	

	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *PDAnnotHandlerGetAnnotInfoFlagsProc)(
	PDAnnotHandler pdanh, PDAnnot pdan);

/** (Optional) Callback for PDAnnotHandler. Returns true if the copy operation is expected
	to succeed. Tests, for example, whether copying is allowed by document permissions.
	@param pdanh IN/OUT? The annotation handler responsible for this annotation.
	@param sourcePage The page the annotation is on.
	@param pdan IN/OUT? The annotation object.
	@return true if the annotation can be copied, false if it cannot.

	@see PDAnnotHandlerCanPasteProc
	@see PDAnnotHandlerCopyProc
	@see PDAnnotHandlerPasteProc
	@see PDAnnotHandlerDestroyDataProc
	@see PDAnnotCanCopy
	@see PDRegisterAnnotHandlerCanCopy  
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotHandlerCanCopyProc)(
	PDAnnotHandler pdanh, PDPage sourcePage, PDAnnot pdan);

/**
	(Optional) Callback for PDAnnotHandler. Copies data from 
	the annotation object to a new clipboard structure, and 
	returns the clipboard structure. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param sourcePage The page containing the annotation to 
	be copied. 
	@param pdan The annotation object.
	@return The clipboard structure containing the copied data. 
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotCopy 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 PDAnnotHandlerClipboardData(ACCBPROTO2 *PDAnnotHandlerCopyProc)(
	PDAnnotHandler pdanh, PDPage sourcePage, PDAnnot pdan);

/**
	(Optional) Callback for PDAnnotHandler. Returns true if 
	the paste operation is expected to succeed. Tests whether 
	data from an annotation that has been copied to a clipboard 
	can be pasted to a location on a page. Pasting can be disallowed 
	by document permissions, or because the annotation cannot 
	be accurately reproduced in the destination document. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param destPage The page to which the annotation would 
	be pasted. 
	@param center The location for the center of the annotation 
	on the destination page, or a NULL pointer to center the 
	annotation on the destination page's crop box. 
	@param data The copied annotation data to test.
	@return true if the annotation data can be pasted, false otherwise. 
	
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotCanPaste 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotHandlerCanPasteProc)(
	PDAnnotHandler pdanh, PDPage destPage, const ASFixedPoint *center, PDAnnotHandlerClipboardData data);

/**
	(Optional) Callback for PDAnnotHandler. Creates a new annotation 
	on the specified page using clipboard data generated by 
	PDAnnotCopy. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param destPage The page to which the annotation is pasted. 
	@param center The location for the center of the annotation 
	on the destination page, or a NULL pointer to center the 
	annotation in the destination page's crop box. 
	@param data The copied annotation data to paste.
	@return The new annotation object associated with the page's document. 
	
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotCopy 
	@see PDAnnotPaste 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 PDAnnot (ACCBPROTO2 *PDAnnotHandlerPasteProc)(
	PDAnnotHandler pdanh, PDPage destPage, const ASFixedPoint *center, PDAnnotHandlerClipboardData data);

/**
	(Optional) Callback for PDAnnotHandler. Destroys data from 
	an annotation that has been copied to a clipboard. This 
	callback may be executed regardless of how many times (if 
	any) the annotation was pasted. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param data The copied annotation data to destroy.
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotDestroyClipboardData 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDestroyDataProc)(
	PDAnnotHandler pdanh, PDAnnotHandlerClipboardData data);

/**
	(Optional) Callback for PDAnnotHandler. Destroys the annotation 
	handler structure when the application no longer requires 
	it. The handler should destroy any dynamic memory stored 
	in the userData field. 
	@param pdanh The annotation handler to destroy.
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDestroyProc)(
	PDAnnotHandler pdanh);

/**
	Callback for PDAnnotHandler. Gets the heel point (the focus 
	or starting point) for the annotation. For a rectangular 
	annotation, this is usually the top left corner. 
	@param pdanh The annotation handler whose heel point is 
	obtained. 
	@param pdan The annotation object. 
	@param point (Filled by the method) The heel point for 
	the annotation.
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerGetHeelPointProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, ASFixedPoint *point);

/**
	Callback for PDAnnotHandler. Called by the host application 
	to obtain a print appearance (a Form XObject). If this callback 
	is not implemented the default appearance (if any) is used. 
	
	@param pdanh The annotation handler whose print appearance 
	is obtained. 
	@param pdan The annotation object. 
	@param frAnnot The rectangle in user space in which the 
	annotation will be printed. The handler is free to modify 
	it. 
	@param op The print operation being performed. This is 
	kPDAnnotPrintStandard for a standard print operation, or 
	kPDAnnotPrintVariableData if the user selected 'Form Fields 
	Only.'
	@return The Form XObject representing the print appearance of the 
	annotation. 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *PDAnnotHandlerGetPrintAppearanceProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, ASFixedRect *frAnnot, PDAnnotPrintOp op);


/**
	Data structure containing callbacks that implement an annotation 
	manager. The callbacks implement the annotation manager 
	functions. For example, view, delete, or export the annotations 
	of a document as a list, sorted by type, author, or date. 

	To fully use a PDAnnotHandler, the AVAnnotHandler associated 
	with this annotation must have its AVAnnotHandlerGetInfoProc 
	and AVAnnotHandlerDeleteInfoProc callbacks defined. 
	@see PDRegisterAnnotHandler 
*/
typedef struct _t_PDAnnotHandler {

	/** Size of the data structure. Must be set to sizeof(PDAnnotHandlerRec). */
	ASSize_t size;

	/** userData Pointer to a block of user-supplied data. */
	void* userData;

	/** */
	PDAnnotHandlerGetTypeProc GetType;

	/** */
	PDAnnotHandlerGetAnnotInfoProc GetAnnotInfo;

	/** */
	PDAnnotHandlerDeleteAnnotInfoProc DeleteAnnotInfo;

	/** */
	PDDocWillExportAnnotProc WillExportAnnot;

	/** */
	PDDocWillImportAnnotProc WillImportAnnot;

	/** */
	PDAnnotWillPrintProc WillPrintAnnot;

	/** */
	PDAnnotHandlerGetAnnotInfoFlagsProc GetAnnotInfoFlags;

	/** */
	PDAnnotHandlerCanCopyProc CanCopy;

	/** */
	PDAnnotHandlerCopyProc Copy;

	/** */
	PDAnnotHandlerCanPasteProc CanPaste;

	/** */
	PDAnnotHandlerPasteProc Paste;

	/** */
	PDAnnotHandlerDestroyDataProc DestroyData;

	/** */
	PDAnnotHandlerDestroyProc Destroy;

	/** */
	PDAnnotHandlerGetHeelPointProc GetHeelPoint;

	/** */
	PDAnnotHandlerGetPrintAppearanceProc GetPrintAppearance;
} PDAnnotHandlerRec;


/**
	Enumerated data type. Specifies the color space in which a color value is specified
	(for example, RGB or grayscale).
*/
enum {

	/** Grayscale. Requires 1 value entry to specify the color. */
	PDDeviceGray,

	/** Red-Green-Blue color specification. Requires 3 value entries to specify the color.*/
	PDDeviceRGB,

	/** Cyan-Magenta-Yellow-Black color specification. Requires 4 value entries to specify the color.*/
	PDDeviceCMYK
};
typedef ASEnum8 PDColorSpace;

/*
** PDColorValue
**
** Value elements must be interpreted according to the color space.
**   PDDeviceGray	- 1 value
**   PDDeviceRGB	- 3 values
**   PDDeviceCMYK	- 4 values
*/

/**
	Data structure representing a color. The number of elements 
	needed in the value field depends on the color space type 
	(specified in the space field). See PDColorSpace for more 
	information. 

	See also AVPrefsType. 
	@see AVAppBeginFullScreen 
	@see AVPageViewGetColor 
	@see AVPageViewSetColor 
	@see PDAnnotGetColor 
	@see PDAnnotSetColor 
	@see PDStyleGetColor
*/
typedef struct _t_PDColorValueRec {

	/** */
	PDColorSpace	space;

	/** */
	ASFixed			value[4];
} PDColorValueRec, *PDColorValue;

typedef const struct _t_PDColorValueRec *PDConstColorValue;

/** Enumerated data type. Specifies the type of changes that occurred for the
	PDDocPrintingTiledPage and PDDocDidChangePages notifications. Not all
	Did notifications have corresponding Will notifications. 
	@see PDDocDeletePages
	@see PDDocInsertPages
	@see PDDocMovePage
	@see PDPageAddCosContents
	@see PDPageAddCosResource
	@see PDPageRemoveCosContents
	@see PDPageRemoveCosResource
	@see PDPageSetRotate
	@see PDPageSetCropBox
	@see PDPageSetMediaBox
*/
enum {
	/** Page insertion */
	pdOpInsertPages,

	/** Page deletion */
	pdOpDeletePages,

	/** Page replacement */
	pdOpReplacePages,

	/** Page rearrangment */
	pdOpMovePages,

	/** Page rotation */
	pdOpRotatePages,

	/** Page cropping */
	pdOpCropPages,

	/** Only PDDocDidChangePages exists, not PDDocWillChangePages.*/
	pdOpAddResources,

	/** Only PDDocDidChangePages exists, not PDDocWillChangePages.*/
	pdOpRemoveResources,

	/** Only PDDocDidChangePages exists, not PDDocWillChangePages. */
	pdOpAddContents,

	/** Only PDDocDidChangePages exists, not PDDocWillChangePages. */
	pdOpRemoveContents,

	/** Page media box modification. */
	pdOpSetMediaBox,

	/** Page bleed box modification. */
	pdOpSetBleedBox,

	/** Page trim box modification */
	pdOpSetTrimBox,

	/** Page art box modification */
	pdOpSetArtBox,

	/** Page tab order modification */
	pdOpSetTabOrder
};
typedef ASEnum8 PDOperation;

/* "defines" for two old (incorrect) PDOperation names previously used in the plug-in API */
#define pdOpRemoveResource pdOpRemoveResources
#define pdOpAddResource pdOpAddResources

#define PDAnnotMaxDashes			10

/**
	The border's dash pattern is specified by dashArray and 
	dashArrayLen. This is a normal PostScript dash pattern (an 
	array of on/off stroke lengths used cyclically) except that 
	zero is always used as the offset ("phase") and the number 
	of array entries is limited to PDAnnotMaxDashes. The number 
	of valid dashArray entries is specified by dashArrayLen - a 
	dashArrayLen of zero specifies a solid border. 
	@see PDLinkAnnotGetBorder 
	@see PDLinkAnnotSetBorder
*/
typedef struct _t_PDLinkAnnotBorder
{

  /** */
  ASInt32 hCornerRadius;

  /** */
  ASInt32 vCornerRadius;

  /** */
  ASInt32 width;

  /** */
  ASInt32	dashArrayLen;

  /** */
  ASFixed	dashArray[PDAnnotMaxDashes];
} PDLinkAnnotBorder;

/************************************************************************************\
|*																					*|
|*									PDBookmark										*|
|*																					*|
|*	You may freely copy or destroy an instance of PDBookmark.						*|
|*	Two instances of a PDBookmark designating the same bookmark are guaranteed to 	*|
|*	contain identical bits.															*|
|*																					*|
\************************************************************************************/

/**
	A bookmark on a page in a PDF file. Each bookmark has a title that appears on
	screen, and an action that specifies what happens when a user clicks on the
	bookmark. Bookmarks can either be created interactively by the user through the
	Acrobat viewer’s user interface or programmatically generated. The typical action for a
	user-created bookmark is to move to another location in the current document,
	although any action (see PDAction) can be specified.

	@see PDDocGetBookmarkRoot
	@see PDBookmarkAddNewSibling
	@see PDBookmarkAddNewChild
	@see PDBookmarkFromCosObj
	@see PDBookmarkGetByTitle
	@see PDBookmarkGetParent
	@see PDBookmarkGetFirstChild
	@see PDBookmarkGetLastChild
	@see PDBookmarkGetNext
	@see PDBookmarkGetPrev
	@see PDBookmarkDestroy
	@see PDBookmarkUnlink
*/
typedef OPAQUE_64_BITS PDBookmark;

/* 
	Constants that are returned from PDBookmarkGetFlags 
	@see PDBookmarkGetFlags
*/
enum 
{
	kPDBookmarkFontNormal		= 0x00,	
	kPDBookmarkFontItalic		= 0x01,	
	kPDBookmarkFontBold			= 0x02,	
	kPDBookmarkFontBoldItalic	= 0x03
};
typedef ASEnum8 PDBookmarkFlags;

/************************************************************************************\
|*																					*|
|*									PDFileSpec										*|
|*																					*|
\************************************************************************************/

/**
	The PDF file specification object. It is used to specify a file in an action (see
	PDAction). A file specification in a PDF file can take two forms:
	
	-  A single platform-independent pathname.
	-  A data structure containing one or more alternative ways to locate the file on different platforms.
	
	PDFileSpecs can be created from ASPathNames or from Cos objects.

	@see PDActionGetFileSpec
	@see PDFileSpecNewFromASPath
	@see PDFileSpecFromCosObj
*/
typedef OPAQUE_64_BITS PDFileSpec;

/************************************************************************************\
|*																					*|
|*									PDFileSpecHandler								*|
|*																					*|
\************************************************************************************/

/**
	Callback for PDFileSpecHandler. Creates a file specification 
	from an ASPath. 
	@param fileSpecHandlerObj User-supplied data passed in 
	the call to PDRegisterFileSpecHandler. 
	@param pdDoc The PDDoc in which the file specification 
	is created. 
	@param path The ASPathName for which a corresponding file 
	specification is created. 
	@param relativeToThisPath A pathname relative to which 
	path is interpreted. If NULL, path is assumed to be an absolute, 
	not a relative, path.
	@return The file specification corresponding to the specified ASPathName. 
	
	@see PDFileSpecNewFromASPath 
*/
typedef ACCBPROTO1 PDFileSpec (ACCBPROTO2 *PDFileSpecNewFromASPathProc)(void *fileSpecHandlerObj, PDDoc pdDoc, ASPathName path, ASPathName relativeToThisPath);

/**
	Callback for PDFileSpecHandler. Acquires the ASPath corresponding
	to a file specification.
	@param fileSpecHandlerObj IN/OUT? User-supplied data passed in
	the call to PDRegisterFileSpecHandler.
	@param fileSpec IN/OUT? The PDFileSpec for which an ASPath is acquired.

	@param relativeToThisPath IN/OUT? A pathname relative to which
	the PDFileSpec is interpreted. If NULL, fileSpec is assumed
	to be an absolute, not a relative, path.
	@return The ASPathName corresponding to the specified path.
	@see PDFileSpecAcquireASPath
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *PDFileSpecAcquireASPathProc)(void *fileSpecHandlerObj, PDFileSpec fileSpec, ASPathName relativeToThisPath);

/**
	(Optional) Callback for PDFileSpecHandler. Launches a specified
	file. Called when the Acrobat viewer encounters a Launch
	(GoTo File) action. If this callback is NULL, no launch
	action is performed.
	@param fileSpecHandlerObj IN/OUT? The registered PDFileSpecHandler.
	@param pdDoc IN/OUT? The document containing the Launch action.
	@param pdAction IN/OUT? The action dictionary.
	@return true if the handler can do the Launch, false otherwise.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDLaunchActionProc)(void *fileSpecHandlerObj, PDDoc pdDoc, PDAction pdAction);

/**
	Data structure that implements a file specification handler. It contains callbacks that
	implement the filespec handler’s functions (converting from a file specification to an
	ASPathName, creating a new file specification from an ASPathName, and launching
	the specified file).
	@see PDRegisterFileSpecHandlerByName
*/
typedef struct _t_PDFileSpecHandler {
	/** sizeof(PDFileSpecHandlerRec) */
	ASSize_t size;	

	/** */
	PDFileSpecNewFromASPathProc	NewFromASPath;

	/** */
	PDFileSpecAcquireASPathProc	AcquireASPath;

	/** */
	PDLaunchActionProc			LaunchAction;

	/** filesys that's to be used with this filespec handler */
	ASFileSys					fileSys; 
} PDFileSpecHandlerRec, *PDFileSpecHandler;

/**
	Callback used by PDDocOpen. It is called when an encrypted 
	document is being opened to determine whether the user is 
	authorized to open the file. 

	This callback implements whatever authorization strategy 
	you choose and calls the callbacks of the appropriate security 
	handler (the one that was used to secure the document) to 
	obtain and check authorization data. 

	The PDAuthProc must call the security handler's PDCryptGetAuthDataProc 
	to obtain whatever authorization data is needed (such as 
	a password), then call PDDocAuthorize (which is mostly a 
	call to the security handler's PDCryptAuthorizeProc) to 
	determine whether this data authorizes access to the file 
	(for example, did the user provide the correct password). 
	The PDAuthProc must also free the authorization data by 
	calling the security handler's PDCryptFreeAuthDataProc (or 
	ASfree, if the handler does not have a PDCryptFreeAuthDataProc.) 
	
	For Acrobat 3.0 and earlier, the correct way to obtain the 
	security handler in a PDAuthProc is to call PDDocGetNewCryptHandler, 
	relying on the fact that it returns the security handler 
	if the document has no new security handler, and the fact 
	that at the time the file is opened, it cannot yet have 
	a new security handler. (In the future, one or more new 
	methods may be added to make this procedure more straightforward.)The 
	Acrobat viewer's built-in authorization procedure works 
	as follows: 

	Call the security handler's PDCryptAuthorizeProc with NULL 
	authorization data to automatically handle the case where 
	no authorization data is needed (for example, the file has 
	a NULL password). 

	If PDCryptAuthorizeProc returns true open the file 

	If PDCryptAuthorizeProc returns false then { Loop for i 
	= 1 to 3 { Call the security handler's PDCryptGetAuthDataProc 
	
	If PDCryptGetAuthDataProc returns true { Call PDDocAuthorize 

	If returns true { /- We got authorization -/ Call the security 
	handler's PDCryptFreeAuthDataProc exit the loop and return 
	from PDAuthProc }Call the security handler's PDCryptFreeAuthDataProc 
	}/- Failed to get authorization after three attempts -/ 
	Display a dialog box indicating that user is not authorized 
	to open the file. }return from PDAuthProc 
	@param pdDoc The PDDoc to open.
	@return true if the user is authorized to open the document, false 
	otherwise. 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAuthProc)(PDDoc pdDoc);

/**
	Callback used by PDDocOpenEx. It is called when an encrypted 
	document is opened, to determine whether the user is authorized 
	to open the file. 

	This callback implements whatever authorization strategy 
	you choose and calls the callbacks of the appropriate security 
	handler (the one that was used to secure the document) to 
	obtain and check authorization data. 

	The PDAuthProcEx should obtain the authorization data (usually 
	a password) and call PDDocAuthorize. PDDocAuthorize in turn 
	calls the document encryption handler's Authorize function, 
	which returns the permissions that the authorization data 
	enables. PDDocAuthorize adds these permissions to those 
	currently allowed, and returns the new set of allowed permissions. 
	
	@param pdDoc The PDDoc to open. 
	@param clientData User-supplied data that was passed in 
	the call to PDDocOpenEx.
	@return true if the user is authorized to open the document, false 
	otherwise. 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAuthProcEx)(PDDoc pdDoc, void *clientData);

/** @name PDPermsFlags
	Flags that describe permissions wanted and granted from PDDocGetPermissions.

	Not all permissions will be granted if document is protected or if document
	is newer version than the app knows about.
*/
/*@{*/

/**	The user can open and decrypt the document. */
#define pdPermOpen		0x01

/**	The user can change the document's security settings. */
#define pdPermSecure	0x02

/**	The user can print the document. Page Setup access is unaffected 
	by this permission, since that affects Acrobat's preferences - not 
	the document's. In the Document Security dialog, this corresponds 
	to the Printing entry. 
*/
#define pdPermPrint		0x04

/**	The user can edit the document more than adding or modifying 
	text notes (see also pdPermEditNotes). In the Document Security 
	dialog, this corresponds to the Changing the Document entry. 
*/
#define pdPermEdit		0x08

/**	The user can copy information from the document to the clipboard. 
	In the Document Security dialog, this corresponds to the 
	Content Copying or Extraction entry. 
*/
#define pdPermCopy		0x10

/**	The user can add, modify, and delete text notes (see also 
	pdPermEdit). In the Document Security dialog, this corresponds 
	to the Authoring Comments and Form Fields entry. 	
*/
#define pdPermEditNotes	0x20

/**	The user can perform a 'Save As...' If both pdPermEdit and 
	pdPermEditNotes are disallowed, 'Save' will be disabled 
	but 'Save As...' is enabled. The 'Save As...' menu item is not 
	necessarily disabled even if the user is not permitted to 
	perform a 'Save As...'. NOTE: Not settable by clients.
*/
#define pdPermSaveAs	0x40

/** */
#define pdPermExt		0x80
/* Added perm bits (PV2) used for standard security handler
** These bits only applies to standard security handler and they are
** used only when kenLength > 5 (40bits)
** Note: When keyLength > 5 is specified, standard security handler willl
** Encrypt Dict with Revision 3.  Rev 3 is support by Acrobat 5.0 and up.
*/

/** override other PDPerm bits.  It allows user to fill-in
	or sign existing form or signature fields.
*/
#define pdPrivPermFillandSign	0x100

/** Override pdPermCopy to enable Accessibility API.
	If a document is saved in Rev2 format (Acrobat 4.0 compatible),
	only pdPermCopy bit is checked to determine Accessibility API state.
*/
#define pdPrivPermAccessible	0x200

/** Override various pdPermEdit bit and allow the following
	operations; page insert/delete/rotate and create bookmark and thumbnail.
*/
#define pdPrivPermDocAssembly	0x400

/** This bit is supplement to pdPermPrint.  If it is clear
	(disabled) only low quality printing (Print As Image) is allowed.  Under UNIX
	platforms, where "Print As Image" doesn't exist, printing is disabled.
*/
#define pdPrivPermHighPrint		0x800

/* Added perm bit (PV3) used for standard security handler
** These bits only apply to standard security handler Revision 4
** R4 is support by Acrobat 6.0 and up.
*/

/** The user is permitted to perform all operations, regardless of
	the permissions specified by the document. Unless this
	permission is set, the document’s permissions will be reset to
	those in the document after a full save.
*/
#define pdPermOwner		0x8000

/** should be set if user can submit forms outside of the browser. This bit is
	supplement to pdPrivPermFillandSign
*/
#define pdPrivPermFormSubmit       0x10000

/** should be set if user can spawn template pages. This bit will allow
	page template spawning even if pdPermEdit and pdPermEditNotes are clear
*/
#define pdPrivPermFormSpawnTempl   0x20000

/** */
#define pdPermAll		0xFFFFFFFF

/** The OR of all operations that can be set by the user in the
	Standard Security dialog (pdPermPrint + pdPermEdit + pdPermCopy + pdPermEditNotes).
*/
#define pdPermSettable	(pdPermPrint + pdPermEdit + pdPermCopy + pdPermEditNotes)

/** All permissions.*/
#define pdPermUser		(pdPermAll - pdPermOpen - pdPermSecure)
/*@}*/


/** Constant values that specify permissions which allow operations on a document file.
	@see AVCryptGetPassword 
	@see PDDocAuthorize 
	@see PDDocGetPermissions (obsolete) 
	@see PDDocPermRequest 
	@see PDCryptAuthorizeProc 
	@see PDCryptGetAuthDataProc 
*/
typedef ASUns32 PDPerms;

/************************************************************************************\
|*																					*|
|*									PDDocSaveWithParams								*|
|*																					*|
\************************************************************************************/


/** Flags for PDDocSave saveFlags parameter
	All undefined flags should be set to zero.
	@see PDDocSave
	@see PDSaveFlags2
*/
enum {
	/** Save only those portions of the document that have changed. 
		(This is provided only as the "opposite" of PDSaveFull, since there
		is no bit-value 0.) */
	PDSaveIncremental	= 0x00,	

	/** Save the entire document.
		Plug-ins that set PDSaveFull are also encouraged to set
		PDSaveCollectGarbage.
	*/
	PDSaveFull			= 0x01,		

	/** Save a copy of the document.
		(That is, the PDDoc continues to use the old file.)
		This flag is ignored if PDSaveFull is off.
	*/
	PDSaveCopy			= 0x02,	

	/** Write the PDF file in a format that is optimized for page-served
		remote (network) access ("linearized").
		This flag is ignored if PDSaveFull is off.

		(Prior to Acrobat 7, linearizing a file caused Cos objects
		to be renumbered, which required that some plug-ins use
		notifications to release and re-acquire objects. In
		Acrobat 7, objects are no longer renumbered.)
	*/
	PDSaveLinearized	= 0x04,	

	/** (Obsolete. In effect, always off.) Write a PostScript header as part of the saved file.*/
	PDSaveWithPSHeader	= 0x08,		

	/** (Obsolete. In effect, always on.) It is OK to store binary data in the PDF file.*/
	PDSaveBinaryOK		   = 0x10,	

	/** Remove unreferenced objects, often reducing file
		size. Plug-ins are encouraged to use this flag.
		This flag is ignored if PDSaveFull is off.
	*/
	PDSaveCollectGarbage   = 0x20,	

	/** Perform an incremental save even if the save is to a
		different file or the document’s version number has
		changed.
	*/
	PDSaveForceIncremental = 0x40,	

	/** Don’t update ModDate in InfoDict.*/
 	PDSaveKeepModDate      = 0x80	
};
typedef ASEnum8 PDSaveFlags;


/** More flags for PDDocSave saveFlags parameter (PDSaveFlags2).
	All undefined flags should be set to zero.
	The first three flags, PDSaveUncompressed, PDSaveCompressed, and
	PDSaveCompressStructureOnly, are mutually exclusive; they can all
	be off, but at most one can be on.
	@see PDDocSave
	@see PDSaveFlags
*/
enum	
{
	/**	Do not use object streams when saving the document.
		("Decompress" all objects.) 
		The result is compatible with all versions of PDF and Acrobat. 
		This flag is ignored if PDSaveFull is off.
	*/
	PDSaveUncompressed			= 1 << 0,

	/**	Compress objects, without restrictions about which objects to compress.
		The result is compatible only with PDF 1.5 (Acrobat 6) or greater. 
		This flag is ignored if PDSaveFull is off.
	*/
	PDSaveCompressed			= 1 << 1,	

	/**	Compress the objects, and ONLY the objects, that are 
		related to logical structure (e.g., tagged PDF).
		The result is compatible with any version of PDF or Acrobat,
		but the compressed objects are usable only in PDF 1.5 (Acrobat 6). 
		This flag is ignored if PDSaveFull is off.
	*/
	PDSaveCompressStructureOnly	= 1 << 2,

	/**	Remove ASCII85 filters from all streams.
		This flag is ignored if PDSaveFull is off. 
	*/
	PDSaveRemoveASCIIFilters	= 1 << 3,

	/**	Encode any unencoded stream with Flate, except for Metadata streams,
		which are never encoded, and for streams that would be larger if
		encoded.
		This flag is ignored if PDSaveFull is off. 
	*/
	PDSaveAddFlate				= 1 << 4,

	/**	Replace all LZW filters with FlateEncode filters.
		This flag is ignored if PDSaveFull is off. 
	*/
	PDSaveReplaceLZW			= 1 << 5,

	/**	Merge identical forms and images, as determined by an MD5 hash of 
		their contents.  (Causes OptimizeXObjects to be called.) 
	*/
	PDSaveOptimizeXObjects					= 1 << 6,

	/**	Look for common initial subsequences among content streams (the sequences
		of marking operators), and generate "substreams" that can be shared. 
		(Causes OptimizeGraphics to be called.) 
	*/
	PDSaveOptimizeContentStreams			= 1 << 7,

	/**	Merge identical font descriptors and encodings. Does not merge the top-level
		font dict.  (Causes OptimizeFonts to be called.) 
	*/
	PDSaveOptimizeFonts						= 1 << 8,

	/**	Delete symbols specific to deleted images from JBIG2 dictionaries that
		could not be processed at the time of image deletion.  Currently only
		effective after deleting pages or extracting pages. 
		(Causes OptimizeMarkedJBIG2Dictionaries to be called.) 
	*/
	PDSaveOptimizeMarkedJBIG2Dictionaries	= 1 << 9,

	/**	(Obsolete. In effect, always off.) 
	*/
	PDSaveEnsure7bitASCII					= 1 << 10,

	/** The PDSaveAutoSave flag is used to indicate that the save that occurred is an autosave event.
		It is only set when an autosave occurs.  It is a read-only flag.
	*/
	PDSaveAutoSave							= 1 << 11
};
/**
	Bitwise flags that specify save options. 
	@see PDDocSaveWithParams 
	@see PDDocInsertPages
*/
typedef ASFlagBits PDSaveFlags2;

/** Structure used to flag Cos objects you wish to access while a PDDoc is being saved.
	It contains a pointer to a CosObjSetCallbackFlagProc.
	In your pre-save callback, you can use this exposed proc to set a flag in the
	CosObjs that you care about, so that you will be called back during the save
	and given their offset and length.

	@see CosObjSetCallbackFlagProc
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocPreSaveInfo {

	/** Size of structure. Set to sizeof(PDDocPreSaveInfoRec). */
	ASSize_t size;

	/** */
	CosObjSetCallbackFlagProc  callbackProc;
} PDDocPreSaveInfoRec, *PDDocPreSaveInfo;

/**
	Callback in the PDDocSaveParams structure used by PDDocSaveWithParams.
	Use this callback to flag Cos objects you wish to access
	while a PDDoc is being saved.
	@param pdDoc IN/OUT? The PDDoc to be saved.
	@param preSaveInfo IN/OUT? A PDDocPreSaveInfo structure containing
	information to use during processing before the save.
	@param clientData IN/OUT? User-supplied data that was specified
	in preSaveProcClientData of the PDDocSaveParams structure.

	@see PDDocSaveWithParams
*/
typedef ACCBPROTO1 void  (ACCBPROTO2 *PDDocPreSaveProc)(
	PDDoc pdDoc, PDDocPreSaveInfo preSaveInfo, void *clientData);

/**
	Callback in the PDDocSaveParams structure. Invoked by PDDocSaveWithParams 
	immediately before a PDDoc is saved to disk. 
	@param pdDoc The PDDoc to be saved. 
	@param clientData User-supplied data that was specified 
	in preWriteProcClientData of the PDDocSaveParams structure.
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 void  (ACCBPROTO2 *PDDocPreWriteProc)(
	PDDoc pdDoc, void *clientData);

/**
	Parameters used when saving a file with PDDocSaveWithParams 
	and returned by the PDDocWillSaveEx notification. Most of 
	these parameters are the same as the parameters for PDDocSave. 
	
	@see PDDocSaveWithParams 
*/
typedef struct _t_PDDocSaveParams {

	/** set this to be the size of this struct */
	ASSize_t			size;			

	/** Must be one of the PDSaveFlags values.
		@see PDSaveFlags
	*/
	PDSaveFlags			saveFlags;	

	/** The path to which the file is saved. A path must be
		specified when either PDSaveFull or PDSaveCopy are
		used for saveFlags. If PDSaveIncremental is
		specified in saveFlags, then newPath should be
		NULL.
		If PDSaveFull is specified and newPath is the same
		as the file’s original path, the new file is saved to a file
		system determined temporary path, then the old file is
		deleted and the new file is renamed to newPath.
	*/
	ASPathName			newPath;	

	/** File system. If NULL, uses the fileSys of the
		document’s current backing file.
		Implementation Restriction: Files can only be saved
		to the same file system, thus fileSys must be NULL or
		an error is raised.
	*/
	ASFileSys			fileSys;	

	/** Progress monitor. Use
		AVAppGetDocProgressMonitor to obtain the
		default. May be NULL.
		@see AVAppGetDocProgressMonitor
	*/
	ASProgressMonitor	mon;	

	/** Pointer to user-supplied data to pass to mon each time it
		is called. Must be NULL if mon is NULL. 
	*/
	void				*monClientData;

	/**	Callback to test whether an operation should be cancelled. A CancelProc 
		is typically passed to some method that takes a long time 
		to complete. At frequent intervals, the method calls the 
		CancelProc. If it returns true, then the method cancels 
		its operation; if false, it continues. 

		@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
		@see AVAppGetCancelProc 
*/
	ASCancelProc		cancelProc;	

	/** Pointer to user-specified data to pass to cancelProc
		each time it is called. Must be NULL if cancelProc is
		NULL.
	*/
	void				*cancelProcClientData;

	/** Callback to flag Cos objects you wish to access while
		the PDDoc is being saved. 
	*/
	PDDocPreSaveProc	preSaveProc;	

	/** Pointer to user-specified data to pass to preSaveProc
		each time it is called.
	*/
	void				*preSaveProcClientData;

	/** Callback to get information about Cos
		objects while the PDDoc is being saved.
	*/
	CosObjOffsetProc	offsetProc;	

	/** Pointer to user-specified data to pass to offsetProc
		each time it is called.
	*/
	void				*offsetProcClientData;

	/** Major PDF version number of the document. If major
		equals 0, both major and minor are ignored. Make
		sure that the document conforms to the version number
		you are setting.
	*/
	PDDocVersion		major;			

	/** Minor PDF version number of the document.*/
	PDDocVersion		minor;		

	/** If true, makes the XML Packet containing the
        XMP metadata for the document read-only and
        suppresses the generation of padding.  If false,
        the XML packet is made writable and the number of
        padding bytes specified by the value of
        the XMPPacketPaddingBytes field is included in
        the XML Packet. 
	*/
    ASBool              XMPPacketReadOnly;  

	/** Specifies the number of bytes of padding to
        include in the XML packet containing the XMP
        metadata for the document, when
        XMPPacketReadOnly is false (ignored when
        XMPPacketReaOnly is true).  Non-positive values
        specify the default padding of 2048 bytes;
        positive values specify the number of bytes of
        padding  directly. 
	*/
    ASInt32             XMPPacketPaddingBytes; 

	/** A callback that is called right before the doc is written to disk */
    PDDocPreWriteProc   preWriteProc;  

	/** Client data to pass to the pre-write proc. */
    void                *preWriteProcClientData;

	/** more flags for PDDocSave */
	PDSaveFlags2		saveFlags2;	/*	*/

	/** Number of incremental saves to "compact". Will eliminate
		the last numSubFilesToCompact subFiles (as opposed to update section, which
		does not always correspond to a valid subFile) and merge the changes in them into
		the current append save. 
		This parameter is taken into account only if:
		
		-  1.The current save operation is an incremental save AND 
		-  2.The current save operation is a saveAs. 
		
		More precisely, compaction of incremental saves will only happen if and only
		if:
		
		- 1. newPath != NULL AND 
		- 2. either PDSaveForceIncremental is set or the document has digital signatures in it or both. 
		
		Moreover, if there are any signatures added in any of the incremental saves that are compacted,
		these signatures will be invalidated. It is up to the client to figure out which incremental
		save they should stop compacting at to avoid this.
	*/
	ASUns32				numSubFilesToCompact;	
												
} PDDocSaveParamsRec;
typedef struct _t_PDDocSaveParams *PDDocSaveParams;


/**
	Structure used to pass information to PDDocWillInsertPagesEx 
	and PDDocDidInsertPagesEx notifications. 
	@see PDDocInsertPages 
*/
typedef struct _t_PDDocInsertPagesParams {

	/** Size of the data structure. Must be set to
		sizeof(PDDocInsertPagesParamsRec).
	*/
	ASSize_t	size;			

	/** The document into which pages are inserted. This document
		must have at least one page.
	*/
	PDDoc		targetDoc;

	/** The page number in targetDoc after which pages from
		srcDoc are inserted. The first page is 0. If
		PDBeforeFirstPage (see PDExpT.h) is used, the pages are
		inserted before the first page in targetDoc. Use PDLastPage
		to insert pages after the last page in targetDoc. 
	*/
	PDPageNumber		insertAfterThisPage;

	/** The document containing the pages that are inserted into targetDoc.*/
	PDDoc		srcDoc;

	/** The page number of the first page in srcDoc to insert into
		targetDoc. The first page is 0.
	*/
	PDPageNumber		srcFromPage;

	/** The page number of the last page in srcDoc to insert into targetDoc.*/
	PDPageNumber		srcToPage;

	/** Flags that determine what additional information is copied from
		srcDoc into targetDoc. Must be an OR of the following 
		
		-  PDInsertAll - Inserts the entire document srcDoc into the
		document targetDoc. This operation not only inserts the
		pages themselves, but also merges other document data from
		srcDoc into targetDoc. In particular, the following happens:
		1. The bookmark tree of srcDoc is merged into the bookmark
		tree of targetDoc by copying it as a new first-level sub-tree of
		targetDoc’s bookmark tree root, of which it becomes the last
		child. If targetDoc has no bookmark tree, it acquires one
		identical to the bookmark tree from srcDoc.
		2. Named destinations from srcDoc (of PDF 1.1 and later) are
		copied into targetDoc. If there are named destinations in
		srcDoc with the same name as some named destination in
		targetDoc, the ones in targetDoc retain their names and the
		copied named destinations are given new names based on the
		old ones with distinguishing digits added. Actions and
		bookmarks referring to the old names are made to refer to the
		new names after being copied into targetDoc.
		3. Document logical structure from srcDoc is copied into
		targetDoc. The top-level children of the structure tree root of
		srcDoc are copied as new top-level children of the structure
		tree root of targetDoc; a structure tree root is created in
		targetDoc if there was none before. The role maps of the two
		structure trees are merged, with name conflicts resolved in favor
		of the role mappings present in targetDoc. Attribute objects
		are not copied, nor is class map information from srcDoc
		merged into that for targetDoc.
		If PDInsertAll flag is not set, pages copied from srcDoc into
		targetDoc will have their structure back-pointer information
		stripped off. 
		-  PDInsertBookmarks - Inserts bookmarks as well as pages. 
		-  PDInsertThreads - Inserts threads as well as pages.
		
	*/
	PDSmallFlagBits		insertFlags;

	/** Error code, which is only valid for the PDDocDidInsertPagesEx notification.*/
	ASErrorCode		error;			
} PDDocInsertPagesParamsRec;
typedef struct _t_PDDocInsertPagesParams *PDDocInsertPagesParams;

/**
	Structure used by PDDocOpenWithParams to specify file open 
	information. The parameters are very similar to those in 
	PDDocOpenEx and PDDocOpenFromASFileEx. 
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocOpenParams *PDDocOpenParams;
/**
	Structure used by PDDocOpenWithParams to specify file open 
	information. The parameters are very similar to those in 
	PDDocOpenEx and PDDocOpenFromASFileEx. 
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocOpenParams {

	/** Size of the data structure. Must be set to sizeof(PDDocOpenParamsRec).*/
	ASSize_t		size;     

	/** Pathname to the file, specified in whatever format is correct for fileSys.*/
	ASFile			file;		

	/** Pathname to the file, specified in whatever format is correct for fileSys.*/
	ASPathName		fileName;	

	/** Pointer to an ASFileSysRec containing the file system in which the file resides.*/
	ASFileSys		fileSys;	

	/** Authorization callback, called only if the file is encrypted.
		This callback should obtain whatever information is
		needed to determine whether or not the user is
		authorized to open the file, then call PDDocAuthorize
		(which returns the permissions that the authentication
		data enables). If the file is encrypted and authProcEx is
		NULL or returns false, pdErrPassword is raised.
		The Acrobat viewer’s built-in authorization procedure
		requires the user to enter a password, and allows the
		user to try three times before giving up.
	*/
	PDAuthProcEx	authProcEx;

	/** Pointer to user-specified data to pass to authProcEx each time it is called.*/
	void			*authProcClientData;

	/** If true, attempt to repair the file if it is damaged; if false, do not attempt to repair the file if it is damaged.*/
	ASBool			doRepair;

	/** The permissions to remove from the document.*/
	PDPerms			restrictPerms; 
} PDDocOpenParamsRec;

/**
	Enumerated data type. Specifies whether or not thumbnail images or bookmarks are
	shown.
	@see AVDocGetViewMode
	@see AVDocSetViewMode
	@see PDDocGetPageMode
	@see PDDocSetPageMode
*/
enum {

	/** Leaves view mode as is. */
	PDDontCare,

	/** Displays document, but neither thumbnails nor bookmarks.*/
	PDUseNone,

	/** Displays document plus thumbnails.*/
	PDUseThumbs,

	/** Displays document plus bookmarks.*/
	PDUseBookmarks,

	/** Displays document in full-screen viewing mode. This is equivalent to AVAppBeginFullScreen. */
	PDFullScreen,

	/** */
	PDContents,

	/** */
	PDUseOC,

	/** Displays document plus attachments. */
	PDUseAttachments
};
typedef ASEnum8 PDPageMode;

/** 
		Structure that defines the layout of a document. The layout can be set as the viewer’s
		avpPageViewLayoutMode preference (set by AVAppSetPreference) or in a view
		of a document by the pageViewLayoutMode field in AVDocViewDef (set by
		AVDocGetViewDef).
		@see AVDocGetViewDef
		@see AVPageViewGetLayoutMode
		@see AVPageViewSetLayoutMode
*/
enum {

	/** (Default) Use the user preference when opening the
		file, as specified in the avpPageViewLayoutMode
		preference, set by AVAppSetPreference.
	*/
	PDLayoutDontCare,

	/** Use single-page mode, as in pre-Acrobat 3.0 viewers.*/
	PDLayoutSinglePage,

	/** Use one-column continuous mode.*/
	PDLayoutOneColumn,

	/** Use two-column continuous mode with first page on left.*/
	PDLayoutTwoColumnLeft,

	/** Use two-column continuous mode with first page on right.*/
	PDLayoutTwoColumnRight,

	/** */
	PDLayoutTwoPageLeft,

	/** */
	PDLayoutTwoPageRight
};
typedef ASEnum8 PDLayoutMode;

/** @name PDDOcFlags Enumerated data type. Specifies various file status attributes.
	These flags indicate the state of the document and whether it needs
	to be saved or deleted on close, etc.
	The flags are used as a bit field. More than one value may be set.
	Some flags may be set or get only. Most can be either set or get.
	@see PDDocClearFlags
	@see PDDocGetFlags
	@see PDDocSetFlags
*/
enum {

	/** Document has been modified and needs to be saved.(get/set) */
	PDDocNeedsSave			= 0x0001,	

	/** Document cannot be saved incrementally;
		when it is saved using PDDocSave, the
		PDSaveFull flag must be specified (see
		PDSaveFlags). (get/set)
	*/
	PDDocRequiresFullSave	= 0x0002,	

	/** Document has been modified slightly (such
		as bookmarks or text annotations have been
		opened or closed), but not in a way that
		warrants saving.(get only)
	*/
	PDDocIsModified			= 0x0004,	

	/** Document is based on a temporary file that
		must be deleted when the document is closed
		or saved.(get/set)
	*/
	PDDocDeleteOnClose		= 0x0008,	

	/** Document was repaired when it was opened.(get only) */
	PDDocWasRepaired		= 0x0010,	

	/** Document’s major version is newer than current.(get only) */
	PDDocNewMajorVersion	= 0x0020,	

	/** Document’s minor version is newer than current. (get only)*/
	PDDocNewMinorVersion	= 0x0040,	

	/** Document’s version is older than current.(get only) */
	PDDocOldVersion			= 0x0080,	

	/** Don’t display errors.(get/set) */
	PDDocSuppressErrors		= 0x0100,	

	/** Document is embedded in a compound document (OLE, OpenDoc).(get/set) */
	PDDocIsEmbedded			= 0x0200,	

	/** Document is linearized for page-served remote (network) access.(get only) */
	PDDocIsLinearized		= 0x0400,

	/** Document is optimized. If this flag is cleared,
		the Batch Optimizer plug-in and the Adobe
		PDF Library do not save the file optimized.
		You can, therefore, linearize a PDF file
		without optimizing it. Optimizing without
		linearizing is not allowed, however. (setonly) 
	*/
	PDDocIsOptimized		= 0x0800	
};


/** used by PDDocInsertPages 
	@see PDDOcInsertPages
*/
typedef enum {

	/** insert bookmarks as well as pages */
	PDInsertBookmarks		= 0x0001,	

	/** insert all Catalog and Info dict values as well as pages */
	PDInsertAll				= 0x1000,	

	/** insert articles as well */
	PDInsertThreads			= 0x0002	
} PDInsertFlags;

/*
** PDPageNumber specification defines -- use where a page number or range or count
** is required.
*/
#define PDBeforeFirstPage	((PDPageNumber) -1)
#define PDLastPage			((PDPageNumber) -2)
#define PDAllPages			((PDPageNumber) -3)
#define PDOddPagesOnly      ((PDPageNumber) -4)
#define PDEvenPagesOnly     ((PDPageNumber) -5)

/************************************************************************************\
|*																					*|
|*									PDFind											*|
|*																					*|
\************************************************************************************/
typedef struct _t_PDFind *PDFind;

/** PDFindFlags -- passed to PDFindText */
enum {

	/** find whole words only */
	PDFindWholeWords	= 0x0001,		

	/** do a case-sensitive search */
	PDFindCaseSens		= 0x0002,		

	/** do a reverse order search */
	PDFindReverse		= 0x0004,		

	/** return a PDTextSelect with all found words on the page */
	PDFindAllOnPage		= 0x0008,		

	/** do not do match full width/half Kana characters */
	PDFindIgnoreFH		= 0x0100,		

	/* ignore diacritics */
	PDFindIgnoreDiacritics = 0x0200,
	
	/** Reset to the beginning of the doc */
	PDFindReset			= 0x0800
	
};
typedef ASEnum8 PDFindFlags;


/************************************************************************************\
|*																					*|
|*									PDFont											*|
|*																					*|
\************************************************************************************/


/*
** These flags define the bit masks for the flags field in structure PDFontFlags.
*/
#define PDFONTFLAGS_USEDBYFORM 0x00000001

/**
	Data structure containing additional information about a font. 
	@see PDDocEnumFonts
	@see PDFontEnumProc 
*/
typedef struct _t_PDFontFlags {

	/** Not used. For backward compatibility. */
	ASInt32  notUsed;

	/** Must be an OR of the PDFONTFLAGS_ values. All unused flags must be off. */
	ASUns32 flags;				
} PDFontFlags;

/**
	Callback used by PDDocEnumFonts and PDDocEnumLoadedFonts. 
	It is called once for each font. 
	@param font The font currently being enumerated. 
	@param fontFlags PDFontFlags used when PDDocEnumFonts was called. 
	@param clientData User-supplied data passed in the call 
	to PDDocEnumFonts or PDDocEnumLoadedFonts.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see PDDocEnumFonts 
	@see PDDocEnumLoadedFonts 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDFontEnumProc)(PDFont font, PDFontFlags *fontFlags, void *clientData);

/*
** PSNAMESIZE is a typical maximum length for a font name.
*/
#define PSNAMESIZE 64

/*
** These flags define the bit masks for the flags field returned by PDFontGetFlags().
*/
#define     PD_FIXED_WIDTH      0x00000001
#define     PD_SERIF		    0x00000002
#define     PD_PI		     	0x00000004
#define     PD_SCRIPT		 	0x00000008
#define     PD_STD_ENCODING     0x00000020
#define     PD_ITALIC			0x00000040
#define     PD_ALL_CAP			0x00010000
#define     PD_SMALL_CAP		0x00020000
#define     PD_FORCE_BOLD       0x00040000


/** Enumerated data type. Specifies a font’s encoding. */
enum {

	/** The encoding specified internally in the font. In the
		case of a Type 1 or MMType 1 font, this is specified
		by the Encoding value in the font’s fontdict. In the
		case of TrueType fonts, this is the encoding specified
		by the default one-byte CMap for the platform.
	*/
	PDBuiltInEncoding = -1,

	/** MacRomanEncoding, as defined in Appendix D in
		the PDF Reference.
	*/
	PDMacRomanEncoding = 0,

	/** MacExpertEncoding, as defined in Appendix D in
		the PDF Reference.
	*/
	PDMacExpertEncoding = 1,

	/** WinAnsiEncoding, as defined in Appendix D in
		the PDF Reference.
	*/
	PDWinAnsiEncoding = 2,

	/** StandardEncoding, as defined in Appendix D in
		the PDF Reference.
	*/
	PDStdEncoding = 3,

	/** PDFDocEncoding, as defined in Appendix D in the
		PDF Reference. This will never be returned for a
		font; it is used internally.
	*/
	PDFDocEncoding = 4,		

	/** */
	PDLastKnownEncoding
};
typedef ASEnum8 PDFontEncoding;

#define PDLastOneByteEncoding PDLastKnownEncoding
#define	PDUnicodeEncoding PDLastKnownEncoding

/** Enumerated data type. Identifies the character set of a Type 1, multiple master Type 1,
	or TrueType font.
*/
enum {

	/** The font does not use Adobe Standard Encoding.*/
	PDUnknownCharSet = 0,

	/** The font uses Adobe Standard encoding. This is
		determined by the "Uses Adobe Standard
		Encoding" bit in the font descriptor. 
	*/
	PDStandardRomanCharSet = 1,

	/** Currently unused. */
	PDAdobeExpertCharSet = 2,

	/** */
	PDLastCharSet
};

typedef ASEnum8 PDCharSet;

/* Font Embedding -- segment types */
#define PD_SEGASCII  ((ASUns8) 1)
#define PD_SEGBINARY ((ASUns8) 2)
#define PD_SEGEOF    ((ASUns8) 3)

/**
	Data structure containing Panose and sFamily class values 
	for a font. Used in the PDFontMetrics structure. See Section 
	5.7.2 in the PDF Reference for more information. For additional 
	details on the Panose number, see Japanese TrueType Font 
	Property Selection Guidelines by the TrueType Conference 
	Technical Committee. 
	@see PDFontGetMetrics 
	@see PDFontSetMetrics 
*/
typedef struct _t_PDFontStyles {

	/** Number that identifies the font family and determines the
		meaning of the remaining Panose digits. Possible families
		are Latin, Kanji, Hebrew, and so forth.
	*/
	ASUns8 sFamilyClassID;	

	/** Number to identify the kind of family: text, decorative,
		handwritten, symbols, and so on.
	*/
	ASUns8 sFamilySubclassID;

	/** Number to identify the family type: text, decorative,
		handwritten, symbols, and so on.
	*/
	ASUns8 bFamilyType;	       

	/** Number that specifies the font’s serif style, such as cove,
		obtuse cove, square, bone, and so forth.
	*/
	ASUns8 bSerifStyle;

	/** Number that specifies the font’s weight, such as very
		light, heavy, black, and so on.
	*/
	ASUns8 bWeight;

	/** Number that specifies the font’s proportions, such as
		modern, expanded, condensed, mono-spaced, and so on.
	*/
	ASUns8 bProportion;
} PDFontStyles;

/**
	Data structure containing information about a font's metrics. 
	See Section 5.5.1 in the PDF Reference for more information 
	about font metrics.You also can find information about Adobe 
	Font Metrics (AFM) on the http://partners.adobe.com/asn 
	web site. 
	@see PDFontGetMetrics 
	@see PDFontSetMetrics 
*/
typedef struct _t_PDFontMetrics {

	/** Must be an OR of the Font Flags values. All unused flags must be off. */
	ASFlagBits			flags;				

	/** Font bounding box in 1000 EM units. (An EM is a typographic
		unit of measurement equal to the size of a font. In a 12-point
		font, an EM is 12 points.)	
	*/
	ASFixedRect			fontBBox;			

	/** Width of missing character (.notdef) */
	PDiFontMetric  		missingWidth;		

	/** Vertical stem width				*/
	PDiFontMetric  		stemV;				

	/** Horizontal stem width			*/
	PDiFontMetric  		stemH;				

	/** Capital height					*/
	PDiFontMetric  		capHeight;			

	/** X height							*/
	PDiFontMetric  		xHeight;			

	/** Max ascender height				*/
	PDiFontMetric  		ascent;				

	/** Max descender depth				*/
	PDiFontMetric  		descent;			

	/** Additional leading between lines	*/
	PDiFontMetric  		leading;			

	/** Maximum character width			*/
	PDiFontMetric  		maxWidth;			

	/** Average character width			*/
	PDiFontMetric  		avgWidth;			

	/** Italic angle in degrees, if any	*/
	PDFontAngle  		italicAngle;		

	/** Panose and sFamily class values  */
	PDFontStyles		style;				

	/** Baseline adjustment, which is a vertical adjustment for font
		baseline difference and writing mode 1 (vertical). This should
		only be used for CIDFontType 2 fonts with font substitution. 
	*/
	PDFontOffset		baseLineAdj;		
} PDFontMetrics, *PDFontMetricsP;

/************************************************************************************\
|*																					*|
|*						PDPage & Contents & Resources								*|
|*																					*|
\************************************************************************************/


/** Specifies page rotation, in degrees. Used for routines that
	set/get the value of a page's Rotate key.
	@see PDPageGetRotate
	@see PDPageSetRotate 
*/
enum {
	/** */
	pdRotate0 = 0,

	/** */
	pdRotate90 = 90,

	/** */
	pdRotate180 = 180,

	/** */
	pdRotate270 = 270
};
typedef ASEnum16 PDRotate;

/** All graphic objects that comprise page, charproc, and PDForm descriptions.
	@see PDGraphicEnumMonitor
 */
typedef struct _t_PDGraphic *PDGraphic,
							/* as well as the following subtypes of PDGraphic */
							*PDText,
							*PDPath,
							*PDInlineImage;

/** A superclass used for PDF XObjects. Acrobat currently uses two XObject subclasses:
	PDImage and PDForm. You can use any PDXObject method on these three objects.
	@see PDXObjectEnumFilters
	@see PDXObjectGetData 
*/
typedef struct _t_PDXObject *PDXObject;	/* Note that a PDFont is really a PDXObject */

/* Type 3 Font CharProc */
typedef struct _t_PDCharProc *PDCharProc;

/**
	A path object consists of a sequence of segment operators -- moveto, lineto, etc. --
	and a set of operations to be performed with the path.
	Note that the operations include doing nothing, closing, stroking, filling
	and using the path as a clip.
	@see PDPathPaintOp
*/
enum {
	/** */
	pdSegMoveTo,
	/** */
	pdSegLineTo,
	/** */
	pdSegCurveTo,
	/** */
	pdSegVCurveTo,
	/** */
	pdSegYCurveTo,
	/** */
	pdSegRect,
	/** */
	pdSegClosePath
};
typedef ASEnum8 PDPathSegmentOp;

/** 
	A path object consists of a sequence of segment operators -- moveto, lineto, etc. --
	and a set of operations to be performed with the path.
	Note that the operations include doing nothing, closing, stroking, filling
	and using the path as a clip.
	@see PDPathSegmentOp
	@see PDPathGetPaintOp 
*/
enum {
	/** Path is not painted. */
	pdPathNoPaint = 0,
	/** Path contains a closepath operator. */
	pdPathOpClose = 1,
	/** Path contains a stroke operator. */
	pdPathStroke = 2,
	/** Path contains a fill operator. */
	pdPathFill = 4,
	/** Path contains an eofill operator. */
	pdPathEOFill = 8,
	/** Path contains a clip operator. */
	pdPathClip = 16,
	/** Path contains an eoclip operator. */
	pdPathEOClip = 32
};
typedef ASEnum8 PDPathPaintOp;

/** Data structure containing information about the current graphics state.
	@see PDGraphicGetState
*/
typedef struct _t_PDGraphicState {
	/** */
	ASFixedMatrix		ctm;
	/** */
	ASFixed			fillColor[4];
	/** */
	ASFixed			strokeColor[4];
	/** */
	ASAtom			fillCSpace;
	/** */
	ASAtom			strokeCSpace;
	/** */
	ASFixed			flatness;
	/** */
	ASInt32			lineCap;
	/** */
	ASFixed			dashPhase;
	/** */
	ASTArraySize	dashLen;
	/** */
	ASFixed			dashes[10];
	/** */
	ASInt32			lineJoin;
	/** */
	ASFixed			lineWidth;
	/** */
	ASFixed			miterLimit;
} PDGraphicState, *PDGraphicStateP;

/** Data structure containing information about the current text state.
	@see PDTextGetState
*/
typedef struct _t_PDTextState {
	/** */
	PDFont			font;
	/** */
	ASFixed			charSpacing;
	/** */
	ASFixed			wordSpacing;
	/** */
	ASFixed			horizontalScale;
	/** */
	ASFixed			leading;
	/** */
	ASFixed			textRise;
	/** */
	ASFixed			textSize;
	/** */
	ASInt32			renderMode;
	/** */
	ASFixedMatrix		textMatrix;
} PDTextState, *PDTextStateP;

/**
	Data structure containing information about the attributes 
	of an image. See Section 4.8 in the PDF Reference for more 
	information. 
	@see PDImageGetAttrs 
	@see PDInlineImageGetAttrs 
*/
typedef struct _t_PDImageAttrs {
	/** (Required) Width of the source image in samples. */
	PDImageScalar	width;
	/** (Required) Height of the source image in samples. */
	PDImageScalar	height;
	/** (Required) The number of bits used to represent each color component. */
	PDCount			bitsPerComponent;
	/** (Optional) true if the image should be treated as a mask; false otherwise.*/
	ASBool			imageMask;
	/** (Optional) true if interpolation is performed; false otherwise.
		Interpolation attempts to smooth transitions between sample values.
	*/
	ASBool			interpolate;
	/** true if decode is used; false otherwise.*/
	ASBool			haveDecode;
	/** (Optional) An array of numbers specifying the mapping from
		sample values in the image to values appropriate for the current color space.
	*/
	ASFixed			decode[8];
	/** ASAtom representing the color space name.*/
	ASAtom			colorSpaceName;
	/** true if the color space is indexed; false otherwise.*/
	ASBool			isIndexed;
	/** (Optional) Used if isIndexed is true. Colors are specified by integers in the range 0 to hival.*/
	PDCount			hiVal;
	/** (Required for images, not allowed for image masks) Cos object of the color space used for the image samples.*/
	CosObj			colorSpace;
	/** Length of sample data, in bytes. */
	ASTArraySize	dataLen;
	/* Added in Acrobat 4.0 */
	/** Number of components in colorSpace. For instance, comps is 3 for an RGB color space.*/
	PDCount			comps;
} PDImageAttrs, *PDImageAttrsP;

/**
	Callback for PDResourceEnumMonitor. Procedure called for
	font resources.
	@param font IN/OUT? The font.
	@param name IN/OUT? The name of the font as it appears in the Resources
	dictionary.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDFormEnumResources.
	@return true to continue enumeration, false to halt enumeration.
	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumFontProc)		(PDFont font, char *name, void *clientData);

/**
	Callback for PDResourceEnumMonitor. Procedure called for
	XObject resources.
	@param xObject IN/OUT? The XObject.
	@param name IN/OUT? The name of the XObject as it appears in the
	Resources dictionary.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDFormEnumResources.
	@return true to continue enumeration, false to halt enumeration.
	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumXObjectProc)	(PDXObject xObject, char *name, void *clientData);

/**
	Callback for PDResourceEnumMonitor. Procedure called for
	ProcSet resources.
	@param name IN/OUT? The name of the ProcSet as it appears in the
	Resources dictionary.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDFormEnumResources.
	@return true to continue enumeration, false to halt enumeration.

	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumProcSetProc)	(char *name, void *clientData);

/**
	Callback for PDResourceEnumMonitor. It is called for color
	space resources.
	@param name IN/OUT? Color space name.
	@param colorSpace IN/OUT? The name of the color space as it appears
	in the Resources dictionary.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDFormEnumResources.
	@return true to continue enumeration, false to halt enumeration.

	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumColorSpaceProc)	(char *name, CosObj colorSpace, void *clientData);

/**
	Data structure containing callbacks used when enumerating 
	the resources of a form with PDFormEnumResources or PDPageEnumResources. 
	

	NOTE: PDPageEnumResources is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page resources. 
	@see PDFormEnumResources 
*/
typedef struct _t_PDResourceEnumMonitor *PDResourceEnumMonitor;
/**
	Data structure containing callbacks used when enumerating 
	the resources of a form with PDFormEnumResources or PDPageEnumResources. 

	NOTE: PDPageEnumResources is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page resources. 
	@see PDFormEnumResources 
*/
typedef struct _t_PDResourceEnumMonitor {
	/** Size of the data structure. Must be set to sizeof(PDResourceEnumMonitorRec). */
	ASSize_t size;
	/** */
	PDResourceEnumFontProc			EnumFont;
	/** */
	PDResourceEnumXObjectProc		EnumXObject;
	/** */
	PDResourceEnumProcSetProc		EnumProcSet;
	/** */
	PDResourceEnumColorSpaceProc	EnumColorSpace;
} PDResourceEnumMonitorRec;

/**
	Callback for PDGraphicEnumMonitor. Called for every text
	operator.
	@param obj IN/OUT? The text object.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumTextProc)			(PDText obj, void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every path
	operator.
	@param obj IN/OUT? The path data.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumPathProc)			(PDPath obj, void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every image
	operator.
	@param obj IN/OUT? Image data.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumImageProc)			(PDInlineImage obj, void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every XObject
	(Do) operator.
	@param name IN/OUT? The XObject's name.
	@param bbox IN/OUT? The XObject's bounding box, describing the
	bounding box of the XObject in user space. This is only
	the case for top-level XObjects. If a Form XObject refers
	to another XObject, the second XObject's bounding box is
	the 'infinity' bounding box.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumXObjectRefProc)	(char *name, ASFixedRect *bbox, void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every Q (save)
	operator.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumSaveProc)			(void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every Q (restore)
	operator.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumRestoreProc)		(void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every d0 (
	that is, setcharwidth) operator.
	@param width IN/OUT? Array of numbers containing the two parameters
	passed to the d0 operator.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.
	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumCharWidthProc)		(ASFixedPoint width, void *clientData);

/**
	Callback for PDGraphicEnumMonitor. Called for every d1 (
	that is, setcachedevice) operator.
	@param parms IN/OUT? Array of numbers containing the 6 parameters
	passed to the d1 operator.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumCacheDeviceProc)	(ASFixed *parms, void *clientData);/* Added in Acrobat 3.0 */

/**
	Callback for PDGraphicEnumMonitor. Gets the current matrix
	for the subsequent XObject. Called immediately before PDGraphicEnumXObjectRefProc.

	@param matrix IN/OUT? (Filled by the callback) The current transformation
	matrix for the subsequent XObject whose name is obtained
	by PDGraphicEnumXObjectRefProc.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageEnumContents.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumXObjectRefMatrixProc)	(ASFixedMatrix *matrix, void *clientData);

typedef struct _t_PDGraphicEnumMonitor *PDGraphicEnumMonitor;
#if PLUGIN || ACROBAT_LIBRARY || TOOLKIT_LITE || PDFL_EXTENSION
/**
	An array of callbacks to pass to PDCharProcEnum, PDFormEnumPaintProc 
	or PDPageEnumContents. One of the callbacks is called for 
	every renderable object in the page contents. Pass NULL 
	for a callback to not enumerate that type of object. Each 
	array element must be either NULL or a valid callback. Enumeration 
	of the page contents halts if the callback returns false. 
	

	NOTE: PDPageEnumContents is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page contents. 

	NOTE: In versions at least through Acrobat 2.1, enumeration 
	does not stop even if a method returns false. 
	@see PDCharProcEnum 
	@see PDFormEnumPaintProc 
	@see PDPageEnumContents 
*/
typedef struct _t_PDGraphicEnumMonitor {

	/** Size of the data structure. Must be set to sizeof(PDGraphicEnumMonitorRec). */
	ASSize_t size;

	/** */
	PDGraphicEnumTextProc			EnumText;

	/** */
	PDGraphicEnumPathProc			EnumPath;

	/** */
	PDGraphicEnumImageProc			EnumImage;

	/** */
	PDGraphicEnumXObjectRefProc		EnumXObjectRef;

	/** */
	PDGraphicEnumSaveProc			EnumSave;

	/** */
	PDGraphicEnumRestoreProc		EnumRestore;

	/** */
	PDGraphicEnumCharWidthProc		EnumCharWidth;

	/** */
	PDGraphicEnumCacheDeviceProc	EnumCacheDevice;
	/* Added in Acrobat 3.0 */

	/** If non-NULL, EnumXObjectRefMatrix is called immediately before EnumXObjectRef.
		It returns the current matrix for the subsequent XObject ref.
	*/
	PDGraphicEnumXObjectRefMatrixProc	EnumXObjectRefMatrix;
} PDGraphicEnumMonitorRec;
#endif

/**
	Callback for PDPathEnumMonitor. Called for every m operator.

	@param p1 IN/OUT? The one point needed to specify the location
	to move to.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathMoveToProc)	(ASFixedPoint *p1, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every l operator.

	@param p1 IN/OUT? The one point needed to specify the line's ending
	point.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathLineToProc)	(ASFixedPoint *p1, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every c operator.

	@param p1 First point needed to specify curve.
	@param p2 Second point needed to specify curve.
	@param p3 Third point needed to specify curve.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, ASFixedPoint *p3, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every v operator.

	@param p1 First of two points needed to specify the curve.
	@param p2 Second of two points needed to specify the curve.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathVCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every y operator.

	@param p1 First of two points needed to specify the curve.
	@param p2 Second of two points needed to specify the curve.
	@param clientData IN/OUT? User-supplied data passed in the call
	to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathYCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every re operator.

	@param p1 First of two points needed to specify the rectangle.
	@param p2 Second of two points needed to specify the rectangle.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathRectProc)		(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	Callback for PDPathEnumMonitor. Called for every path closing
	operator.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPathEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathClosePathProc)(void *clientData);

typedef struct _t_PDPathEnumMonitor *PDPathEnumMonitor;

/**
	Data structure containing callbacks used by PDPathEnum. 
	One callback is called for each path operator encountered; 
	the callback to call depends on the operator. 
	@see PDPathEnum 
*/
typedef struct _t_PDPathEnumMonitor {
	/** Size of the data structure. Must be set to sizeof(PDPathEnumMonitorRec). */
	ASSize_t size;
	/** */
	PDPathMoveToProc		MoveTo;
	/** */
	PDPathLineToProc		LineTo;
	/** */
	PDPathCurveToProc		CurveTo;
	/** */
	PDPathVCurveToProc		VCurveTo;
	/** */
	PDPathYCurveToProc		YCurveTo;
	/** */
	PDPathRectProc			Rect;
	/** */
	PDPathClosePathProc		ClosePath;
} PDPathEnumMonitorRec;

/**
	Callback for PDTextEnum. Called once for each string in
	a text object.
	@param font IN/OUT? The font used for string.
	@param string IN/OUT? The string. This string may be converted
	using PDFontXlateToHost or PDFontXlateToUCS.
	@param stringLen IN/OUT? The number of bytes in string.
	@param delta IN/OUT? The difference, in thousandths of an EM, from
	the end of the previous string to the beginning of the current
	string. (An EM is a typographic unit of measurement equal
	to the size of a font. For example, in a 12-point font,
	an EM is 12 points.) See the description of the TJ operator
	in Section 5.3.2 in the PDF Reference.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDTextEnum.
	@return true to continue enumeration, false to halt enumeration.

	@see PDTextEnum
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDStringEnumProc)(PDFont font,
	char *string, ASInt32 stringLen, ASFixed delta, void *clientData);

/**
	Callback for PDXObjectEnumFilters. Called once for each
	filter that has been applied to an XObject's data.
	@param filter IN/OUT? The filter's name.
	@param decodeParms IN/OUT? The dictionary Cos object containing
	the filter's decode parameters.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDXObjectEnumFilters.
	@return true to continue enumeration, false to halt enumeration.

	@see PDXObjectEnumFilters
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDXObjectFilterEnumProc)(char *filter,
	CosObj decodeParms, void *clientData);

/**
	Callback for PDFontEnumCharProcs. It is called once for
	each character in a Type 3 font.
	@param name IN/OUT? The name of the current character.
	@param obj IN/OUT? Stream Cos object containing the PDF drawing
	operators that draw the character.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDFontEnumCharProcs.
	@return true to continue enumerating, false to halt enumeration.

	@see PDFontEnumCharProcs
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDCharProcEnumProc)(char *name, PDCharProc obj,
	void *clientData);

/**
	Callback for PDXObjectGetData. It is passed the XObject's
	data. Currently, the XObject's data is read 1 kB at a time
	and passed to this callback.
	@param data IN/OUT? Buffer containing the XObject's data.
	@param lenData IN/OUT? The amount of data in data, in bytes.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDXObjectGetData.
	@return true to continue reading the XObject's data, false to halt
	it.
	@see PDXObjectGetData
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDGetDataProc)(char *data, ASUns32 lenData, void *clientData);


/************************************************************************************\
|*																					*|
|*								PDTextSelect										*|
|*																					*|
\************************************************************************************/

/**
	Data structure representing a single entry (starting location and length) in a highlight
	list.
	@see PDTextSelectCreatePageHilite
	@see PDTextSelectCreateWordHilite
*/
typedef struct _t_HiliteEntry {
	/** */
	PDCharOffset	offset;
	/** */
	PDCharOffset	length;
} HiliteEntry;

/*
** PDTextSelectRange
**
** Word- and character-oriented text selection.  
*/

/**
	Data structure used to specify a range of text in a text 
	selection. 

	Use 0 for ofsStart and ofsEnd for whole-word selections. 
	Nonzero values for ofsStart and ofsEnd are supported by 
	PDText but are currently ignored by the Acrobat viewer's 
	user interface code (which highlights only whole-word selections). 
	If ofsEnd is 0, end is the first word not selected. 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectGetRange 
*/
typedef struct _t_PDTextSelectRange {
	/** word containing start of selection */
	ASInt32	start;			
	/** word containing end of selection */
	ASInt32	end;			
	/** offset into word at start of selection */
	ASInt32	ofsStart;		
	/** offset into word at end of selection */
	ASInt32	ofsEnd;			
} PDTextSelectRangeRec, *PDTextSelectRange;

/**
	Callback for PDTextSelectEnumQuads. Called once for each
	quad in a text selection.
	@param procObj IN/OUT? User-supplied data that was passed in the
	call to PDTextSelectEnumQuads.
	@param page IN/OUT? The page on which the text selection is located.

	@param quad IN/OUT? The quad being enumerated.
	@return true to continue enumeration, false to halt enumeration.

	@see PDTextSelectEnumQuads
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumQuadProc)(void *procObj,
	ASInt32 page, ASFixedQuad *quad);

/**
	Callback for PDTextSelectEnumText and PDTextSelectEnumTextUCS.
	Called once for each text run (text in the same font, size,
	color, and on the same line) in a text selection.
	@param procObj IN/OUT? User-supplied data that was passed in the
	call to PDTextSelectEnumText or PDTextSelectEnumTextUCS.

	@param font IN/OUT? The text's font.
	@param size IN/OUT? The text's size, in points.
	@param color IN/OUT? The text's color.
	@param text IN/OUT? The text in the current run. NOTE: This string
	is not necessarily null-terminated.
	@param textLen IN/OUT? The number of bytes in text.
	@return true to continue enumeration, false to halt enumeration.

	@see PDTextSelectEnumText
	@see PDTextSelectEnumTextUCS
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumTextProc)(void *procObj,
	PDFont font, ASFixed size, PDColorValue color, char *text, ASInt32 textLen);
	
/** */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumRTFTextProc)(void *procObj,
	PDFont font, ASFixed size, PDColorValue color, char *text, ASUns32 rtfCntFlag, ASInt32 textLen);

/************************************************************************************\
|*																					*|
|*							PDThread & PDBead										*|
|*																					*|
\************************************************************************************/

/**
	An article in the Acrobat viewer’s user interface, and contains an ordered sequence of
	rectangles that bound the article. Each rectangle is called a bead. Threads can be
	created interactively by the user, or programmatically.
	@see PDDocGetThread
	@see PDThreadNew
	@see PDThreadFromCosObj
	@see PDBeadGetThread
	@see PDDocRemoveThread
	@see PDThreadDestroy
*/
typedef OPAQUE_64_BITS PDThread;

/**
	A single rectangle in an article thread. (Article threads are known simply as articles in
	the Acrobat viewer’s user interface.) A bead remains valid as long as a thread is
	"current and active."
	@see AVPageViewGetActiveBead
	@see AVPageViewIsBeadAtPoint
	@see PDBeadNew
	@see PDBeadGetNext
	@see PDBeadGetPrev
	@see PDThreadGetFirstBead
	@see PDBeadDestroy
*/
typedef OPAQUE_64_BITS PDBead;

/************************************************************************************\
|*																					*|
|*									PDThumb											*|
|*																					*|
\************************************************************************************/

/*
** PDThumbCreationServer
** The create thumb server is passed in at thumb creation time.
*/

/**
	(Optional) Callback for PDThumbCreationServer. Called before
	processing each page. May be NULL.
	@param pageNum IN/OUT? The page for which to create a thumbnail
	image.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDDocCreateThumbs.
	@return true to continue thumbnail image creation, false to halt
	thumbnail image creation.
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDThumbCreationNotifyPageProc)(ASInt32 pageNum,
	void *clientData);

/**
	(Optional) Callback for PDThumbCreationServer. Called for
	each page that does not currently contain a thumbnail image.
	May be NULL. If it is NULL, the thumbnail data is generated
	by the default thumbnail generator.
	@param page IN/OUT? The page for which to create a thumbnail image.

	@param thumbScale IN/OUT? The scale to map from the page size to
	the thumbnail size - the thumbnail size is either 1/ 8 of
	the page size, or is limited to MAX_THUMBPAGE_WIDTH and
	MAX_THUMBPAGE_HEIGHT, whichever is smaller.
	@param width IN/OUT? The width of the thumbnail image to create.

	@param height IN/OUT? The height of the thumbnail image to create.

	@param thumbData IN/OUT? A buffer into which the thumbnail data
	is copied. This buffer has the size: rowBytes = (width *
	bitsPerPixel + 7) / 8; size = rowBytes * height; where bitsPerPixel
	is specified as numComponents x bitsPerComponent. numComponents
	is dependent upon the color space. For DeviceRGB, numComponents
	is 3. For an indexed color space, numComponents is 1.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDDocCreateThumbs.
	@return true to continue thumbnail image creation, false to halt
	thumbnail image creation.
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDThumbCreationGetThumbDataProc)(
	PDPage page, ASFixed thumbScale, ASInt32 width, ASInt32 height, void *thumbData,
	void *clientData);

/**
	(Optional) Callback for PDThumbCreationServer. Called after
	PDThumbCreationGetThumbDataProc and after a PDThumb has
	been created. Gives the server a chance to draw the thumbnail
	image in a status window. May be NULL.
	@param thumb IN/OUT? The thumbnail image to draw.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDDocCreateThumbs.
	@see PDThumbCreationGetThumbDataProc
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDThumbCreationDrawThumbProc)(PDThumb thumb,
	void *clientData);

typedef struct _t_PDThumbCreationServer *PDThumbCreationServer;
/**
	Data structure containing callbacks that implement a creation server. The callbacks
	implement the creation server functions.
	@see PDDocCreateThumbs
*/
typedef struct _t_PDThumbCreationServer {
	/** Set this field to sizeof(PDThumbCreationServerRec) */
	ASSize_t size; 
	/** */
	PDThumbCreationNotifyPageProc	NotifyPage;
	/** */
	PDThumbCreationGetThumbDataProc	GetThumbData;
	/** */
	PDThumbCreationDrawThumbProc	DrawThumb;
} PDThumbCreationServerRec;

/************************************************************************************\
|*																					*|
|*							PDViewDestination										*|
|*																					*|
|*  Opaque designator for a view destination object. Note that this API does not    *|
|*  export routines that construct or acquire view destination objects, just the    *|
|*  definition of the object and its methods. The client may freely copy or destroy *|
|*  an instance of a PDViewDestination; two instances of a PDViewDestination        *|
|*  designating the same view destination are guaranteed to contain the identical   *|
|*  bits.                                                                           *|
|*																					*|
\************************************************************************************/

/**
	A particular view of a page in a document. It contains a reference to a page, a
	rectangle on that page, and information specifying how to adjust the view to fit the
	window’s size and shape. It corresponds to a PDF Dest array and can be considered
	a special form of a PDAction.
	@see AVPageViewToViewDest
	@see PDActionGetDest
	@see PDViewDestCreate
	@see PDViewDestFromCosObj
	@see PDViewDestResolve
	@see PDViewDestDestroy
*/
typedef OPAQUE_64_BITS PDViewDestination;

/*
** Value indicating a null entry for VDXYZ destination attributes
*/
#define PDViewDestNULL		fixedNegativeInfinity


/************************************************************************************\
|*																					*|
|*									PDWord											*|
|*																					*|
\************************************************************************************/

/**
	Extracts words from a PDF file, and enumerates the words on a single page or on all
	pages in a document.
	@see PDDocCreateWordFinder
	@see PDDocCreateWordFinderUCS
	@see PDDocGetWordFinder
	@see PDWordFinderDestroy
	@see PDWordFinderEnumWords
*/
typedef struct _t_PDWordFinder *PDWordFinder;

/**
	A word in a PDF file. Each word contains a sequence of characters in one or more
	styles (see PDStyle).
	@see PDWordFinderGetNthWord
	@see PDWordFinderEnumWords
	@see PDWordFinderEnumWords
*/
typedef struct _t_PDWord *PDWord;

/**
	Provides access to information about the fonts, font sizes, and colors used in a
	PDWord.
	@see PDWordGetNthCharStyle
*/
typedef struct _t_PDStyle *PDStyle;

/**
	Callback for PDWordFinderEnumWords. Called once for each
	word.
	@param wObj IN/OUT? The word finder.
	@param wInfo IN/OUT? The current word in the enumeration.
	@param pgNum IN/OUT? The page number on which wInfo is located.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDWordFinderEnumWords.
	@return true to continue enumeration, false to halt enumeration.

	@see PDWordFinderEnumWords
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDWordProc)( PDWordFinder wObj, PDWord wInfo,
	ASInt32 pgNum, void *clientData );

/**
** PDFindTranslateStringProc -- passed to PDFindText()
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDFindTranslateStringProc)(char *string,
	ASInt32 stringLength, PDWord pdWord, void *clientData);

/* Wordy Character types */

/** @name CharacterTypeCodes
	Constants that specify a character’s type (uppercase, lowercase, number,
	punctuation, and so forth).
	@see PDWordGetCharacterTypes
	@see PDWordSplitString
	@see PDWordFilterString
*/
/*@{*/

/** A control code.*/
#define W_CNTL             0x1

/** A lowercase letter.	*/
#define W_LETTER           0x2

/** An uppercase letter. */
#define W_UPPERCASE        0x4

/** A digit. */
#define W_DIGIT            0x8

/** A punctuation mark. */
#define W_PUNCTUATION      0x10

/** A hyphen. */
#define W_HYPHEN           0x20

/** A hyphen that is only present because a word is broken across two lines of text. */
#define W_SOFT_HYPHEN      0x40

/** A ligature. */
#define W_LIGATURE         0x80

/** A white space glyph. */
#define W_WHITE            0x100

/** A comma (commas and periods are treated separately
	from other punctuation marks because they are used
	both as word punctuation marks and as de-limiters in
	numbers, and need to be treated differently in the two
	cases).
*/
#define W_COMMA            0x200

/** A period. */
#define W_PERIOD           0x400

/** An accent mark. */
#define W_ACCENT           0x800

/** A glyph that cannot be represented in the destination font encoding. */
#define W_UNMAPPED         0x1000

/** An end-of-phrase glyph (for example, ".", "?", "!", ":", and ";"). */
#define W_END_PHRASE       0x2000

/** A wildcard glyph (for example, "*" and "?") that should not be treated as a normal punctuation mark.*/
#define W_WILD_CARD        0x4000

/** A glyph that acts as a delimiter between words 
	@see GlyphNamesofWordSeparators 
*/
#define W_WORD_BREAK       0x8000
/*@}*/

/* WEChar encoding info flags (8-bit) */
#define WXE_ENC_UNMAPPED	0x01	/* The font has no useful encoding information. Word Finder copied
			the original character string from the PDF content. In this case, Word Finder always assumes
			the original string is single-byte characters. If the Word Finder is extracting the text in Unicode,
			it inserts additional bytes, 0x00s, to form 16-bit characters. (i.e. "0x41, 0x42" a "0x00, 0x41,
			0x00, 0x42") A common example of this case is a Symbol font without ToUnicode table.Since there is
			no valid Unicode values are available, during the word-finding process, these characters are treated
			as symbolic characters and each character becomes a word. */
#define WXE_ENC_MISSING		0x02	/* The character code is not available in the output encoding space.
									   This character is replaced by a space character. */
#define WXE_ENC_NO_UCS		0x04	/* Word Finder is not able to find a reliable Unicode value
			from the character. The character encoding is determined by "guessing" due to insufficient
			encoding information available in the PDF file. */
#define WXE_FROM_ACTUALT	0x08	/* The character comes from an ActualText, rather than the page content. */


/* Word's Flags (Group ID = 0) */

/** @name WordAttributes
	Constants that specify various attributes of words.
	@see PDWordGetAttr
*/
/*@{*/

/** The word contains a character outside the range
of A-Z, a-Z, 0-9.*/
#define WXE_HAS_NONALPHANUM     0X1

/** The word contains a character between A-Z or az.*/
#define WXE_HAS_LETTER			0x2

/** The word contains a character between A-Z.*/
#define WXE_HAS_UPPERCASE		0x4

/** One or more characters in the word are digits.*/
#define WXE_HAS_DIGIT           0x8

/** One or more characters in the word are
	punctuation marks. Other flag bits can be
	checked to test whether the punctuation was at
	the beginning of the word
	(WXE_HAS_LEADING_PUNC), the end of the word
	(WXE_HAS_TRAILING_PUNC), or elsewhere in
	the word.
*/
#define WXE_HAS_PUNCTUATION     0x10

/** There is a hyphen in the word.*/
#define WXE_HAS_HYPHEN          0x20

/** There is a soft hyphen in the word.*/
#define WXE_HAS_SOFT_HYPHEN     0x40

/** The word contains a ligature.*/
#define WXE_HAS_LIGATURE        0x80

/** The first character in the word is a punctuation
	mark. If this bit is set, WXE_HAS_PUNCTUATION
	will also be set.
*/
#define WXE_HAS_LEADING_PUNC    0x100

/** The last character in the word is a punctuation
	mark. If this bit is set, WXE_HAS_PUNCTUATION
	will also be set.
*/
#define WXE_HAS_TRAILING_PUNC   0x200

/** One or more characters in the word cannot be
	represented in the output font encoding.
*/
#define WXE_HAS_UNMAPPED_CHAR   0x400

/** The character following the end of the word is a
	space (either an explicit space character encoded
	in a string, or one that appears implicitly because
	the drawing point was moved). 
*/
#define WXE_ADJACENT_TO_SPACE   0x800
/* Extended wFlags */

/** The writing direction of the word is not in a
	multiple of 90 degrees or the bounding box of the
	text is skewed. This flag indicates that the quads
	of the word should be used to specify the highlight
	area correctly.
*/
#define WXE_ROTATED   			0x1000

/** The writing direction of the word is either 90 or
	180 degrees. This flag ignores the page rotation
	parameter of the page dictionary. Therefore, if the
	page is rotated 90 degrees, this flag will be set on
	each word that appears horizonally on the screen.
*/
#define WXE_VERTICAL_FLOW       0x2000

/** */
#define WXE_WBREAK_WORD         0x4000

/** The word is at the end of the current text line (i.e.,
	the word is followed by a line break).
*/
#define WXE_LAST_WORD_ON_LINE   0x8000
/*@}*/

/* Word's Flags (Group ID = 1) */
#define WXE_FRONT_TAB				0x01	/* Insert a tab character before this word */
#define WXE_ENCODING_WARNING		0x02	/* Unreliable encoding conversion happened in this word.
											   Check the encoding flags of each character for detail. */
#define WXE_REVERSE_DIRECTION		0x04	/* The writing direction is right-left or top-down */
#define WXE_WORD_IS_UNICODE			0x08	/* The text is in Unicode format. */
#define WXE_EXT_CHAR_OFFSETS		0x10	/* The word has extened character offset information and
											   can be used for character based PDWord methods like
											   PDWordGetCharOffsetEx(), PDWordGetCharQuad(),
											   PDWordGetNumHiliteChar(), and PDWordGetByteIdxFromHiliteChar(). */
/* 0x0100 ~ 0x8000 are reserved for Acrobat internal usage. */


/* Context  flags */
/**
*/
#define  WXE_STREAM          0x1

/**
*/
#define  WXE_PDF_ORDER       0x2

/**
*/
#define  WXE_XY_SORT         0x4

/**
*/
#define  WXE_RD_ORDER_SORT   0x8

/** To obtain latest available version
*/
#define WF_LATEST_VERSION     0

/** Version used for Acrobat 3.x, 4.x
*/
#define WF_VERSION_2	2	

/** For Acrobat 5.0 without Accessibility enabled 
*/
#define WF_VERSION_3	3

/** For Acrobat 5.0 with Accessibility enabled 
*/
#define WF_VERSION_4	4	

/**
	Callback for PDEnumDocs. It is called once for each open
	PDDoc.
	@param pdDoc IN/OUT? The PDDoc currently being enumerated.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDEnumDocs.
	@return true to continue enumeration, false to halt enumeration.

	@see PDEnumDocs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocEnumProc)(PDDoc pdDoc, void *clientData);

/**
	A word finder configuration that customizes the way the 
	extraction is performed. In the default configuration, all 
	options are false. 
	@see PDDocCreateWordFinderEx 
*/
typedef struct _t_PDWordFinderConfig {

	/** Always, sizeof(PDWordFinderConfigRec) */
	ASSize_t		recSize;	
	/** When true, disables Tagged PDF 
		support and treats the document as non-Tagged PDF. Use this 
		to keep the word finder in legacy mode when it is created 
		with the latest algorithm version (WF_LATEST_VERSION).
	*/
	ASBool			disableTaggedPDF;	

	/** When true, disables generating an XY-ordered 
		word list. This option replaces the sort order flags in 
		the older version of the word finder creation command (PDDocCreateWordFinder). 
		Setting this option is equivalent to omitting the WXE_XY_SORT 
		flag. 
	*/
	ASBool			noXYSort;
	/** When true, the word finder preserves 
		space characters during word breaking. Otherwise, spaces 
		are removed from output text. When false (the default) you 
		can add spaces later by considering the word attribute flag 
		WXE_ADJACENT_TO_SPACE, but there is no way to restore the 
		exact number of consecutive space characters.
	*/
	ASBool			preserveSpaces;		

	/** When true, disables the expansion 
		of ligatures using the default ligatures. Default ligatures 
		are: fi, ff, fl, ffi, ffl, ch, cl, ct, ll, ss, fs, st, oe, 
		OE. 
	*/
	ASBool			noLigatureExp;		

	/** When true, disables guessing encoding 
		of fonts that have unknown or custom encoding, when there 
		is no ToUnicode table. Inappropriate encoding conversions 
		can cause the word finder to mistakenly recognize non-Roman 
		single-byte fonts as Standard Roman encoding fonts and extract 
		the text in an unusable format. When this option is selected, 
		the word finder avoids such unreliable encoding conversions 
		and tries to provide the original characters without any 
		encoding conversion for a client with its own encoding handling. 
		Use the PDWordGetCharEncFlags method to detect such characters. 
	*/
	ASBool			noEncodingGuess;	

	/** When true, assumes any font with 
		unknown or custom encoding to be Standard Roman. This option 
		overrides the noEncodingGuess option.
	*/
	ASBool			unknownToStdEnc;	

	/** When true, disables converting large 
		character gaps to space characters, so that the word finder 
		reports a character space only when a space character appears 
		in the original PDF content. This option has no effect on 
		Tagged PDF. 
	*/
	ASBool			ignoreCharGaps;		

	/** When true, disables treating vertical 
		movements as line breaks, so that the word finder determines 
		a line break only when a line break character or special 
		tag information appears in the original PDF content. This 
		option has no effect on Tagged PDF. 
	*/
	ASBool			ignoreLineGaps;		

	/** When true, disables extracting text from 
		text annotations. Normally, the word finder extracts text 
		from the normal appearances of text annotations that are 
		inside the page crop box. 
	*/
	ASBool			noAnnots;			

	/** When true, disables finding and 
		removing soft hyphens in non-Tagged PDF, so that the word 
		finder trusts hard hyphens as non-soft hyphens. This option 
		has no effect on Tagged PDF files. Normally, the word finder 
		does not differentiate soft and hard hyphen characters in 
		non-Tagged PDF files, because these are often misused. 
	*/
	ASBool			noHyphenDetection;	

	/** When true, disables treating non-breaking 
		space characters as regular space characters in non-Tagged 
		PDF, so that the word finder preserves the space without 
		breaking the word. This option has no effect on Tagged PDF 
		files. Normally, the word finder does not differentiate 
		breaking and non-breaking space characters in non-Tagged 
		PDF files, because these are often misused. 
	*/
	ASBool			trustNBSpace;		

	/** When true, disables generating 
		extended character offset information to improve text extraction 
		performance. The extended character offset information is 
		necessary to determine exact character offset for character-by-character 
		text selection. The beginning character offset of each word 
		is always available regardless of this option, and can be 
		used for word-by-word text selection with reasonable accuracy. 
		When a client has no need for the detailed character offset 
		information, it can use this option to improve the text 
		extraction efficiency. There is a minor difference in the 
		text extraction performance, and less memory is needed for 
		the extracted word list.
	*/
	ASBool			noExtCharOffset;	

	/** When true, disables generating character 
		style information to improve text extraction performance 
		and memory efficiency. When you select this option, you 
		cannot use PDWordGetNthCharStyle and PDWordGetStyleTransition 
		with the output of the word finder.
	*/
	ASBool			noStyleInfo;		

	/**
		A custom UTF-16 decomposition table. 
		This table can be used to expand Unicode ligatures not included 
		in the default ligature list. Each decomposition record 
		contains a UTF-16 character code (either 16-bit or 32-bit 
		surrogate), a replacement UTF16 string, and the delimiter 
		0x0000. For example: const ASUns16 myDecompTbl[] = {0x00b2, 
		0x0032, 0x0000, 0x00c6, 0x0061, 0x0065, 0x0000, 0xFB01, 
		0xFB01, 0x0000}; This replaces superscript '2' with '2', 
		the 'AE' ligature with 'a' and 'e', and disables the default 
		handling of the 'fi' ligature. The word finder applies this 
		substitution after identifying the character types, so the 
		word-breaking process uses the character attributes of the 
		original, rather than the replacement characters. See charTypeTble 
		below. 
	*/
	const ASUns16	*decomposeTbl;		

	/** Size of the decomposeTbl in bytes. */
	ASSize_t		decomposeTblSize;	

	/**
		A custom character type table to enhance 
		word breaking quality. Each character type record contains 
		a region start value, a region end value, and a character 
		type flag as defined in PDExpT.h. A character code is in 
		UTF-16, either 16-bit or 32- bit surrogate. For example: 
		const ASUns16 myCharTypeTbl[] = {0x0082, 0x0082, W_CNTL+W_WORD_BREAK, 
		0x00b2, 0x00b3, W_DIGIT} This identifies 0x0082 as a control-plus-word 
		break character, and 0x00b2 and b3 as digits. If you need 
		to change a character's type along with its character code, 
		you must define the original character code in the custom 
		character type table. For example, if 'a' is transformed 
		to '1' in the decomposition table, 'a' should be transformed 
		to W_DIGIT here, so that the word finder can recognize the 
		replaced character, '1', as a number. 
	*/
	const ASUns16	*charTypeTbl;		

	/** Size of the charTypeTbl in bytes */
	ASSize_t		charTypeTblSize;	
} PDWordFinderConfigRec, *PDWordFinderConfig;

/** passed to PDWordFinderSetCtrlProc()
	This is the callback function called by Word Finder when its page enumeration process takes longer
	than the specified time (in seconds). Return TRUE to continue the enumeration process, or return FALSE
	to stop. startTime is the value that was set by ASGetSecs() when the Word Finder started processing
	the current page.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDWordFinderCtrlProc)(ASUns32 startTime, void *clientData);

/*------------------------------------------------------------------------
	Color calibration types.
------------------------------------------------------------------------*/

/**
	Data structure containing a monitor’s chromaticity, for use in displaying deviceindependent
	color.
	x and y are the two values needed to specify the chromaticity.
	@see PDColorCalP
	@see PDPrefGetColorCal
	@see PDPrefSetColorCal
*/
typedef struct _t_PDChromaticity
{

	/** The x-axis component of the monitor’s chromaticity. See figure. Must be >= 0 and < 1. x + y must be <= 1.*/
	ASFixed	x;

	/** The y-axis component of the monitor’s chromaticity. See figure. Must be > 0 and <= 1. x + y must be <= 1. */
	ASFixed	y;
} PDChromaticity;

/** Data structure used to represent the characteristics of an output device; needed for
	device-independent color.
	@see PDPrefGetColorCal
	@see PDPrefSetColorCal
*/
typedef struct _t_PDColorCal
{
	/** */
	PDChromaticity 	whiteChrom;
	/** */
	PDChromaticity 	redChrom;
	/** */
	PDChromaticity 	greenChrom;
	/** */
	PDChromaticity 	blueChrom;
	/** */
	ASFixed		redGamma;
	/** */
	ASFixed		greenGamma;
	/** */
	ASFixed		blueGamma;
} PDColorCal, *PDColorCalP;

/*------------------------------------------------------------------------
	PDPageStm types
------------------------------------------------------------------------*/

#define kPDPageStmStringMax	256

/* Flag values for PDPageStmGetToken */
#define kPDPageStmSkipComments	0x0001

/* Flag values for PDPageStmGetInlineImage */
/* -- none defined yet -- */

/* Flag values for PDPageStmToken */
#define kPDPageStmTokenHexString	0x0001

/**
	Data structure used by PDPageStmGetToken. It contains information about the
	current page contents token.

	@see PDPageStmGetToken
*/
typedef struct _t_PDPageStmToken {
	/** size of this record; to be filled in by caller */
	ASSize_t	size;	
	/** type of token */
	CosType	type;		
	/** additional information about token, such as if hex string */
	ASUns32	flags;		
	/** value of token if CosInteger, ASFixed, or Name */
	ASInt32	iVal;		
	/** value of token if CosString */
	char	sVal[kPDPageStmStringMax];	
	/** number of bytes in sVal */
	ASSize_t	sValLen;	
} PDPageStmTokenRec, *PDPageStmToken;

/**
	Callback used by PDPageStmGetToken. It is called when the
	length of a string token exceeds kPDPageStmStringMax bytes
	(see PDExpT. h) in PDPageStmGetToken.
	@param sVal IN/OUT? The string value read so far.
	@param sValLen IN/OUT? Length of sVal, in bytes.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageStmGetToken.
	@see PDPageStmGetToken
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDPageStmStringOverflowProc)(char *sVal,
	ASSize_t sValLen, void *clientData);

/**
	Callback for PDPageStmGetInlineImage. Should be called when
	inline image data is encountered in PDPageStmGetToken. This
	method may be called multiple times for one inline image.
	If so, each call provides sequential data for the image.

	@param data IN/OUT? The image data read so far.
	@param dataLen IN/OUT? Length of data, in bytes.
	@param clientData IN/OUT? User-supplied data that was passed in
	the call to PDPageStmGetInlineImage (which may have been
	passed in the PDPageStmGetToken method).
	@return true to continue reading the image's data, false to stop
	reading.
	@see PDPageStmGetInlineImage
	@see PDPageStmGetToken
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPageStmImageDataProc)(ASUns8 *data,
	ASSize_t dataLen, void *clientData);

/**
	A transition to a page. The Trans key in a Page dictionary specifies a Transition
	dictionary, which describes the effect to use when going to a page and the amount of
	time the transition should take.
	@see PDPageGetTransition
	@see PDTransFromCosObj
	@see PDTransNew
	@see PDTransNewFromCosDoc
	@see PDTransNull
*/
typedef OPAQUE_64_BITS PDTrans;

#define fxDefaultPageDuration	-fixedOne
#define fxDefaultTransDuration	fixedOne

/*------------------------------------------------------------------------
	PDDocReadAhead types
------------------------------------------------------------------------*/

/* Flag values for PDDocReadAhead */
/**	Allows the AcroForm client to request that 
	all the AcroForm data be read 'ahead,' before the viewer 
	needs it. This flag is ignored if the PDF file does not 
	contain a Forms hint table. See Section F.3.5 in the PDF 
	Reference for information about the Forms hint table. 
	
*/
#define kPDDocReadAheadAcroForms	0x0001

/**	Requests that the PDF file's Forms Template data be read 
	'ahead,' before the viewer needs it. There is currently 
	no Template hint table defined, so this flag simply causes 
	the rest of the file to be read. 
	
*/
#define kPDDocReadAheadTemplates	0x0002

/**	Requests that the PDF file's page label data be read 'ahead,' 
	before the viewer needs it. There is currently no page label 
	hint table defined, so this flag simply causes the rest 
	of the file to be read. 
	
*/
#define kPDDocReadAheadPageLabels	0x0004

/**	Requests that the PDF file's logical structure data be read 
	'ahead,' before the viewer needs it. There is currently 
	no logical structure hint table defined, so this flag simply 
	causes the rest of the file to be read.
*/
#define kPDDocReadAheadStructure	0x0008

/**
*/
#define kPDDocReadAheadRenditions	0x0010

/*------------------------------------------------------------------------
	PD Related Security and Encryption: New permission methods
------------------------------------------------------------------------*/

/** 
	An enumerated data type that provides the status of PDDoc-related permissions
	methods.
	@see PDDocPermRequest
	@see PDCryptAuthorizeExProc
	@see PDCryptGetAuthDataExProc
*/
enum {
	/** -1 Request was denied.*/
	PDPermReqDenied = -1,
	/** 0 Request was granted.*/
	PDPermReqGranted = 0,
	/** 1 The object is unknown.*/
	PDPermReqUnknownObject = 1,
	/** 2 The operation is unknown.*/
	PDPermReqUnknownOperation = 2,
	/** 3 The operation is not applicable for the specified object.*/
	PDPermReqOperationNA = 3,	/* Not applicable operation for specified object */
	/** The handler doesn't have enough info to answer 
        at this point. Try again later*/
    PDPermReqPending = 4        
};
typedef ASInt16	PDPermReqStatus;

/* The version of the PDPermReqObj and PDPermReqOpr */
#define PDPermReqVersion    0x0003

/**	Document object type. Applicable operations: 
	-  PDPermReqOprAll 
	-   PDPermReqOprModify (Doc Info, open action, page label, modifying document ) 
	-   PDPermReqOprCopy (Copy to clipboard) 
	-  PDPermReqOprAccessible 
	-   PDPermReqOprSelect (Selection only) 
	-  PDPermReqOprOpen 
	-   PDPermReqOprSecure 
	-   PDPermReqOprPrintHigh 
	-   PDPermReqOprPrintLow 
	-   PDPermReqOprFullSave 
	-   PDPermReqOprImport (Non-PDF) 
	-   PDPermReqOprExport (Non-PDF & text extract API, Search & Catalog) 
	-  PDPermReqOprAny 
*/
enum {
	/** */
	PDPermReqObjDoc = 1,
	/** */
	PDPermReqObjPage,
	/** */
	PDPermReqObjLink,
	/** */
	PDPermReqObjBookmark,
	/** */
	PDPermReqObjThumbnail,
	/** */
	PDPermReqObjAnnot,
	/** Form fields, other then signature form fields */
	PDPermReqObjForm,
	/** signature form fields */
	PDPermReqObjSignature,

	/* Acrobat 7.0 additions */
	/** Named embedded files */
	PDPermReqObjEF,

	/** Used to for checking cache size */
	PDPermReqObjLast	
};
typedef ASUns32 PDPermReqObj;

/** Enumerated data type used to describe the target operation of a permissions request.
	@see PDCryptAuthorizeExProc
	@see PDCryptGetAuthDataExProc
	@see PDDocPermRequest
*/
enum {
	/** Check all operations */
	PDPermReqOprAll = 1,	
	/** generic */
	PDPermReqOprCreate, 
	/** Delete */
	PDPermReqOprDelete,
	/** Modify */
	PDPermReqOprModify,
	/** Copy */
	PDPermReqOprCopy,
	/** For Accessibility use */
	PDPermReqOprAccessible,	
	/** For doc or page, selecting (not copying) text or graphics */
	PDPermReqOprSelect, 
	/** For document open */
	PDPermReqOprOpen, 
	/** for doc to changing security settings */
	PDPermReqOprSecure, 
	/** for doc, Regular printing */
	PDPermReqOprPrintHigh, 
	/** for doc, low quality printing */
	PDPermReqOprPrintLow, 
	/** Form fill-in or Sign existing field*/
	PDPermReqOprFillIn,	
	/** Rotate */
	PDPermReqOprRotate,
	/** Crop */
	PDPermReqOprCrop,
	/** For summarize notes */
	PDPermReqOprSummarize,	
	/** Insert */
	PDPermReqOprInsert,
	/** for page */
	PDPermReqOprReplace,	
	/** for page */
	PDPermReqOprReorder,	
	/** For doc */
	PDPermReqOprFullSave,	
	/** For notes & Image */
	PDPermReqOprImport, 
	/** For notes. ExportPS should check print */
	PDPermReqOprExport, 
	/** Used for checking to see if any operation is allowed */
	PDPermReqOprAny,		
	/** Used for error checking */
	PDPermReqOprUnknownOpr,	

	/* Acrobat 5.1 additions.  */
	/** Submit forms outside of the browser */
	PDPermReqOprSubmitStandalone,	
	/** allow form to spawn template page*/
	PDPermReqOprSpawnTemplate,		
	/** This should be always the last item  */

	/* Acrobat 7.0 additions */
	/** For annots & form, enabling online functionality */
	PDPermReqOprOnline,
	/** For annots, enabling summary view of annots in Reader */
	PDPermReqOprSummaryView,
	/** For forms, enables form appearance to be rendered as plaintext barcode */
	PDPermReqOprBarcodePlaintext,

	PDPermReqOprLast	
};
typedef ASUns32 PDPermReqOpr;

/*------------------------------------------------------------------------
	PD Related Security and Encryption
------------------------------------------------------------------------*/

/* Flags used by PDDocGetNewSecurityInfo */

/** @name SecurityInfoFlags
	Constants used to specify various information about the Acrobat viewer’s security and
	permissions.
	@see PDDocGetNewSecurityInfo
*/
/*@{*/

/** The document has a user password.*/
#define pdInfoHasUserPW		pdPermOpen

/** The document has an owner password.*/
#define pdInfoHasOwnerPW	pdPermSecure

/** The document can be printed.*/
#define pdInfoCanPrint		pdPermPrint

/** The document can be modified, for example by adding notes, links, or bookmarks 
	@see pdInfoCanEditNotes
*/
#define pdInfoCanEdit		pdPermEdit

/** The document text and graphics can be copied to the clipboard. */
#define pdInfoCanCopy		pdPermCopy

/** The document’s notes, but nothing else, can be modified
	@see pdInfoCanEdit
*/
#define pdInfoCanEditNotes	pdPermEditNotes
/*@}*/

/* This is the standard data used to fill in the Encrypt dict.
 */

#define MAX_PWCHARS 255

typedef char StdPassword[MAX_PWCHARS+1];

/**
	Structure describing the data for the standard security 
	handler provided in the Acrobat viewer. 
*/
typedef struct _t_StdSecurityData {
	/** size of this stucture */
	ASSize_t size;			
	/** true if the user password should be changed */
	ASBool newUserPW;		
	/** true if there is a user password */
	ASBool hasUserPW;		
	/** the user password */
	StdPassword userPW;	
	/** true if the owner password should be changed, false otherwise.*/
	ASBool newOwnerPW;
	/** true if an owner password is provided, false otherwise. */
	ASBool hasOwnerPW;
	/** The owner password. */
	StdPassword ownerPW;
	/** permissions to allow */
	PDPerms perms;			
	/** Encryption key length in byte. new for Acrobat 5.0 */
	Int32	keyLength;		
	/** indicate /R value. New for Acrobat 6.0 */
	Int32	revision;
	/** flag that indicates whether document metadata will be encrypted */
	ASBool	encryptMetadata;
	/** New for Acrobat 7.0 */
	/** method of encryption for filters to use
	 ** only valid if revision is 4 or greater
	 ** Valid Values are defined immediately below
	 ** STDSEC_METHOD_RC4_V2 - use RC4 algorithm for encryption
	 ** STDSEC_METHOD_AES_V1 - use AES algorithm for encryption with a zero initialized iv
	 ** STDSEC_METHOD_AES_V2 - use AES algorithm for encryption with a random initialized iv
     **/
	Int32	encryptMethod;
	/** flag to indicate that only Attachments are encrypted - encryptMetadata and encryptAttachmentsOnly cannot both be true */
	ASBool encryptAttachmentsOnly;
} StdSecurityDataRec, *StdSecurityData;

#define STDSEC_METHOD_RC4_V2 2
#define STDSEC_METHOD_AES_V1 5
#define STDSEC_METHOD_AES_V2 6

/**
	Callback for PDCryptHandler. Called by PDDocAuthorize when 
	a user tries to set security for an encrypted document and 
	by a PDAuthProc when a user tries to open a file. 

	It must decide, based on the contents of the authorization 
	data structure, whether the user is permitted to open the 
	file, and what permissions the user has for this file. The 
	authorization data structure is available in making this 
	decision. Alternate implementations may not require authorization 
	data and may, for example, make authorization decisions 
	based on data contained in the security data structure (use 
	PDCryptNewSecurityDataProc). 

	This callback must not obtain the authorization data (for 
	example, by displaying a user interface into which a user 
	can type a password). Obtaining authorization data is handled 
	by the security handler's PDCryptGetAuthDataProc, which 
	must be called before this callback. Instead, PDCryptAuthorizeProc 
	must work with whatever authorization data is passed to 
	it. 

	It is legitimate for this callback to be called with NULL 
	authorization data; the Acrobat viewer's built-in authProc 
	does this in order to support authorization methods that 
	do not require authorization data. 

	When this callback is invoked to determine whether a user 
	is permitted to open a file, permWanted is set to pdPermOpen. 
	In this case, the file's contents are not yet decrypted 
	(since this callback is being asked to permit decryption), 
	and some calls must be avoided. For example, a call that 
	causes a page to be parsed results in an error, since the 
	encrypted contents are parsed. In general, it is safe to 
	obtain information about the presence or absence of things, 
	or the number of things, and to examine any part of a document 
	at the Cos level. 
	@param pdDoc The document for which authorized permissions 
	are being requested. 
	@param authData Authorization data. Its format is security 
	handler-specific; each handler can select its own authorization 
	data format. 
	@param permWanted The permissions being requested. Is 
	either pdPermOpen (if the file is being opened) or pdPermSecure 
	(if a request is being made to change the document's security 
	settings).
	@return The permissions granted based on the authData. For opening, 
	the permissions returned usually should be pdPermOpen and 
	some or all of pdPermPrint, pdPermEdit, and pdPermCopy. 
	For setting security, permissions returned should be pdPermAll. 
	However, if authorization fails, 0 should be returned. 
	@see PDCryptAuthorizeExProc 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx
*/
typedef ACCBPROTO1 PDPerms (ACCBPROTO2 *PDCryptAuthorizeProc)(PDDoc pdDoc,
	void *authData, PDPerms permWanted);

/**
	(Optional) Callback for PDCryptHandler. Creates a new empty 
	authorization data structure. This structure is subsequently 
	filled by PDCryptGetAuthDataProc, then passed to PDCryptAuthorizeProc 
	and eventually to ASfree. 

	This callback is not called by the Acrobat viewer, but a 
	security handler may use it if it wishes. The Acrobat viewer's 
	standard security handler does not use this method. 
	@param pdDoc The document for which a new authorization 
	data structure is created.
	@return The newly created authorization data structure. 
	@see PDCryptFreeAuthDataProc 
	@see PDCryptGetAuthDataProc 
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptNewAuthDataProc)(PDDoc pdDoc);

/**
	Callback for PDCryptHandler. This callback is called from 
	a PDAuthProc when a file is opened after PDCryptNewSecurityDataProc 
	is called. 

	The callback must determine the user's authorization properties 
	for the document by obtaining authorization data, such as 
	a user interface log in or password entry. It populates 
	an authorization data structure with this data. 

	This callback may call the security handler's PDCryptNewAuthDataProc 
	to allocate the authorization data structure. Use of an 
	authorization data structure is optional (an implementation 
	may wish to contain authorization data within the security 
	data structure). The authorization data structure is subsequently 
	used by the security handler's PDCryptAuthorizeProc to determine 
	whether the user is authorized to open the file. 

	A security handler can specify the standard password dialog 
	by using AVCryptGetPassword. In this case, the authData 
	is a char*. 
	@param pdDoc The document to open. 
	@param permWanted Either pdPermOpen or pdPermSecure. Since 
	this value is also passed to PDCryptAuthorizeProc, it may 
	not be necessary for this callback to use permWanted. 
	@param authDataP Pointer to authorization data structure. 
	Set to NULL if not used.
	@return true unless the operation should be canceled (for example, 
	if the user cancels a dialog), false otherwise. 
	@see PDCryptGetAuthDataExProc 
	@see PDCryptNewAuthDataProc 
	@see PDDocOpen 
	@see PDDocOpen 
	@see PDDocOpenEx 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptGetAuthDataProc)(PDDoc pdDoc,
	PDPerms permWanted, void **authDataP);

/**
	(Optional) Callback for PDCryptHandler. Creates and populates 
	a new structure that contains whatever security-related 
	information the security handler requires (for example, 
	permissions, whether the file has owner and/or user passwords, 
	owner and/or user passwords, or other data used internally 
	by the security handler). If encryptDict is not NULL, the 
	structure should be populated based on encryptDict's contents. 
	This method is intended only to initialize the security 
	data structure. 

	This callback is called under two circumstances: 
	
	-  When a document is opened, it is called with encryptDict 
	set to the document's Encryption dictionary. The handler 
	should then populate the new security data structure with 
	data that is obtained from the Encryption dictionary. 
	-  When the user chooses a new encryption method, it is called 
	without an encryptDict. The handler should return a security 
	data structure with default values. 
	
	If a security handler does not have this callback, the document's 
	newSecurityData field is set to NULL. 

	If a file is to be saved, then PDCryptUpdateSecurityDataProc 
	is subsequently called to allow user interface modification 
	of the contents. 

	Security data is freed using PDCryptFreeSecurityDataProc. 
	If PDCryptFreeSecurityDataProc is not defined, ASfree is 
	used. 
	@param pdDoc The document for which a new security data 
	structure is created. 
	@param encryptDict If encryptDict is a dictionary, this 
	callback must initialize the security data so that it corresponds 
	to the dictionary. Otherwise, it must set up default values 
	in the security data structure. 
	@return The newly created security data structure. 
	@see PDCryptFreeSecurityDataProc 
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptNewSecurityDataProc)(PDDoc pdDoc,
	CosObj encryptDict);

/**
	(Optional) Callback for PDCryptHandler. Validates the security
	data structure, which specifies the user's permissions.
	This callback may modify the security data structure, for
	example because the user is not authorized to change the
	security as they requested. A client may have called PDDocNewSecurityData
	to obtain a new security data structure, then modified it,
	and then called PDDocSetNewSecurityData to change the document
	security. This callback should be called before actually
	setting the document's security data.

	This callback is not called automatically by the Acrobat
	viewer. It must be called, if desired, by the security handler's
	PDCryptUpdateSecurityDataProc.
	@param pdDoc IN/OUT? The document whose security data is validated.

	@param secData IN/OUT? (May be modified by the callback) The document's
	security data.
	@see PDCryptUpdateSecurityDataProc
	@see PDDocNewSecurityData
	@see PDDocSetNewSecurityData
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptValidateSecurityDataProc)(PDDoc pdDoc,
	void *secData);

/**
	Callback for PDCryptHandler. Updates the security data structure 
	that was created by PDCryptNewSecurityDataProc. This structure 
	can be obtained by calling PDDocGetNewSecurityData. The 
	security data structure of the previously saved file can 
	be obtained with a call to PDDocGetSecurityData. 

	The security data structure should be updated to reflect 
	the encryption parameters that will be used when saving 
	the file. (This information is usually obtained via dialogs.) 
	The encryption parameters are transferred to the Encrypt 
	dictionary by a subsequent callback to PDCryptFillEncryptDictProc. 
	
	The security data should be allocated by ASmalloc or a related 
	function. Security data is freed using PDCryptFreeSecurityDataProc. 
	If PDCryptFreeSecurityDataProc is not defined, ASfree is 
	used. 

	The callback can also update the security handler itself. 
	For example, the standard encryption handler switches to 
	no encryption if no passwords or permissions are set in 
	the security dialog box. Return ASAtomNull in cryptHandler 
	if no encryption is used in the saved file. 
	@param pdDoc The document whose security data is updated. 
	
	@param cryptHandler The current security handler for pdDoc. 
	Can be modified to change the security handler. Encryption 
	is turned off if ASAtomNull is set. 
	@param secDataP (Required) Security data structure. Its 
	content and organization is up to the security handler.
	@return true unless the operation should be canceled (for example, 
	the user clicked on the Cancel button). 
	@see PDCryptValidateSecurityDataProc 
	@see PDDocGetSecurityData 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptUpdateSecurityDataProc)(PDDoc pdDoc,
	ASAtom *cryptHandler, void **secDataP);

/**
	Callback for PDCryptHandler. Sets up the key to be passed
	to initialize the RC4 cipher for encryption and decryption
	of a PDF file. It is called when an encrypted document is
	opened or saved.
	@param pdDoc IN/OUT? The document for which the key is set.
	@param cryptData IN/OUT? (Filled by the callback) The key. cryptData
	must be allocated by ASmalloc because the Acrobat viewer
	will free it using ASfree.
	@param cryptDataLen IN/OUT? (Filled by the callback) The number
	of bytes in cryptData. Cannot be greater than 5 bytes.
	@see PDCryptNewAuthDataProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptNewCryptDataProc)(PDDoc pdDoc,
	char **cryptData, ASInt32 *cryptDataLen);

/**
	Callback for PDCryptHandler. Called when an encrypted document
	is saved. Fills the document's Encryption dictionary with
	whatever information the security handler wants to store
	in the document.

	Normally this callback is called after PDCryptUpdateSecurityDataProc.
	The security data structure can be obtained with a call
	to PDDocGetNewSecurityData, and the encryptDict is filled
	based on this data.

	The sequencing of events that the viewer performs during
	creation of the encryptDict is as follows:
	
	-  the viewer creates the encryptDict
	-  the viewer adds the Filter attribute to the dictionary
	-  calls this PDCryptFillEncryptDictProc to allow the security
	handler to add its own attributes to the dictionary
	-  calls the PDCryptNewCryptDataExProc (then PDCryptNewCryptDataProc
	if unsuccessful) to get the algorithm version, key, and
	key length
	-  checks if the V attribute has been added to the dictionary
	and, if not, then sets V to the algorithm version
	-  sets the Length attribute if V is 2 or greater
	-  adds the encryptDict to the document
	
	@param pdDoc IN/OUT? The document to save.
	@param encryptDict IN/OUT? A dictionary Cos object to fill with
	whatever information the security handler wants to store
	in the PDF file. Unlike all other strings and streams, direct
	object elements of the encryptDict are not encrypted automatically.
	If you want them encrypted, you must encrypt them before
	inserting them into the dictionary.
	@see PDDocSave
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *PDCryptFillEncryptDictProc)(PDDoc pdDoc,
	CosObj encryptDict);

/**
	(Optional) Callback for PDCryptHandler. Called by PDDocGetNewSecurityInfo. 
	Extracts the security information from the security data 
	structure, and returns the security information. 

	This function is also used after a 'Save As...' to reset the 
	permissions according to the current document. 

	A default set of permissions is used if this callback is 
	absent: 

	pdInfoCanPrint|pdInfoCanEdit|pdInfoCanCopy|pdInfoCanEditNotes 
	

	See PDPerms. 
	@param pdDoc The document whose security info is obtained. 
	
	@param secInfo (Filled by the callback) The document's 
	security info. The value must be an OR of the Security Info 
	Flags. All unused bits in must be set to 1.
	@see PDDocGetNewSecurityInfo 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptGetSecurityInfoProc)(PDDoc pdDoc,
	ASUns32 *secInfo);

/**
	(Optional) Callback for PDCryptHandler. Used to free security
	data acquired via PDCryptNewSecurityDataProc. If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree.
	@param pdDoc IN/OUT? The document whose security data is freed.

	@param secData IN/OUT? (Filled by the callback) Pointer to the
	document's security data.
	@see PDCryptNewSecurityDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeSecurityDataProc)(PDDoc pdDoc,
	void *secData);

/**
	(Optional) Callback for PDCryptHandler. Used to free authorization
	data acquired via PDCryptNewAuthDataProc. If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree.
	@param pdDoc IN/OUT? The document whose authorization data is freed.

	@param authData IN/OUT? (Filled by the callback) Pointer to the
	document's authorization data.
	@see PDCryptNewAuthDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeAuthDataProc)(PDDoc pdDoc, void *authData);

/**
	(Optional) Callback for PDCryptHandler. Used to free authorization
	data acquired via PDCryptNewCryptDataProc. If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree.
	@param pdDoc IN/OUT? The document whose encryption/ decryption
	data is freed.
	@param cryptData IN/OUT? (Filled by the callback) Pointer to the
	document's encryption/ decryption data.
	@see PDCryptNewCryptDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeCryptDataProc)(PDDoc pdDoc, char *cryptData);

/**
	Callback for PDCryptHandler. Sets up the key to be passed
	to initialize the RC4 cipher for encryption and decryption
	of a PDF file. It is called when an encrypted document is
	opened or saved.

	The key is truncated when the length is greater than the
	viewer currently supports. Data is freed by PDCryptFreeCryptDataProc
	if provided. Otherwise, ASfree is used.
	@param pdDoc IN/OUT? The document for which the key is set.
	@param cryptData IN/OUT? (Filled by the callback) The key. cryptData
	must be allocated by ASmalloc because the Acrobat viewer
	will free it using ASfree.
	@param cryptDataLen IN/OUT? (Filled by the callback) The number
	of bytes in cryptData. Cannot be greater than 5 bytes.
	@param cryptVersion IN/OUT? The Cos crypt version - the version
	of the algorithm that is used to encrypt and decrypt document
	data. cryptVersion equal to 0 is treated as cryptVersion
	equal to 1 to maintain backward compatibility.
	@see PDCryptFreeCryptDataProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptNewCryptDataExProc)(PDDoc pdDoc,
	char **cryptData, ASInt32 *cryptDataLen, ASInt32 *cryptVersion);

/**
	Replaces PDCryptAuthorizeProc. PDPerms are now obsolete 
	because Acrobat 5.0 introduces new permission controls. 
	However, Acrobat still supports old security handlers. 

	Called whenever Acrobat needs to get authorization data 
	and/or check permissions for operations. 
	@param pdDoc The document for which the request is made. 
	
	@param reqObj Object type that is the focus of the request: 
	one of the PDPermReqObj values. 
	@param reqOpr Operation type that is the focus of the 
	request: one of the PDPermReqOpr values. 
	@param authData Authorization data. Its format is security 
	handler-specific.
	@return The status of the request. One of the PDPermReqStatus values. 
	
	@see PDCryptAuthorizeProc 
	@see PDDocPermRequest 
*/
typedef ACCBPROTO1 PDPermReqStatus (ACCBPROTO2 *PDCryptAuthorizeExProc)(PDDoc pdDoc, PDPermReqObj reqObj,
	PDPermReqOpr reqOpr, void *authData);

/**
	Replaces PDCryptGetAuthDataProc. Called whenever Acrobat 
	needs to get authorization data and/or check permissions 
	for operations. 

	PDPerms are now obsolete because Acrobat 5.0 introduces 
	new permission controls. However, Acrobat still supports 
	old security handlers. 
	@param pdDoc The document for which the request is made. 
	
	@param reqObj Object type that is the focus of the request: 
	one of the PDPermReqObj values. 
	@param reqOpr Operation type that is the focus of the 
	request: one of the PDPermReqOpr values. 
	@param authDataP Pointer to an authorization data structure. 
	Its format is security handler specific.
	@return true unless the operation should be canceled (for example, 
	if the user cancels a dialog), false otherwise.. 
	@see PDCryptGetAuthDataProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptGetAuthDataExProc)(PDDoc pdDoc, PDPermReqObj reqObj,
	PDPermReqOpr reqOpr, void **authDataP);

/**
	Called when the app needs to open rolled back portion of the original document. Since rolled back document
	still requires authData which should be identical to the original document's. However, authData 
	structure is unique to each security handler; therefore, it cannot be duplicated by the app. This callback 
	is intended for opening rolled back document silently by asking security handler to provide authdata for it.
	Security handler should be able to duplicate security data associated with original doc and supply for rolledback doc.
	The callee is expected to authorize for sebsequent callbacks, including for Crypt Filters.

	If this callback is not provided, security handler is asked for authData via normal call (e.g. PDCryptGetAuthDataExProc).
	The side effect might include security handler prompting for password for rolled back document.

	**"rolled back" document is original portion of the document when it is digitally signed. This 
	functionality is used for document modification detection.

	@param pdDoc The document for which the request is made. (rolled back document)
	@param encryptDict encrypt dict for pdDoc, it's here just in case security handler might need it
	@param alreadyOpenedDoc original PDDoc which rolledback document is extracted from. It is provided, so that security handler
	can get security data out for rolled back document.
	@param openedEncryptDict encrypt dict of the original PDDoc
	@param authDataP to return security data for rolled back doc
	@return true unless the security handler cannot provide authData for requested doc. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptNewSecurityDataFromOriginalDocProc)(PDDoc pdDoc, CosObj encryptDict, 
	PDDoc alreadyOpenedDoc, CosObj openedEncryptDict, void **authDataP);

/**
	Called when the security handler should bring up a document
	(security) info dialog with the current settings. It also
	should return true when the user wants to change the settings.

	If this callback is not supplied, the default info dialog
	is displayed with PDPerms bits information (Acrobat 4. x
	equivalent dialog).
	@param pdDoc IN/OUT? The document whose info is displayed.
	@param cryptHandler IN/OUT? The registered name of the handler.

	@return true if the handler wishes a call back to change the settings,
	false otherwise.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptDisplaySecurityDataProc)(PDDoc pdDoc,	ASAtom cryptHandler);

/**
	(Optional) Callback for PDCryptHandler. Used by Acrobat
	WebBuy proprietary method of passing crypt data.
*/
typedef ACCBPROTO1 void * (ACCBPROTO2 *PDCryptReservedProc)(void);

/**
	(Optional) This call is used to provide PDCrypt handler interoperability.  
	When an encrypted document is being opened and the the security handler 
	specified in the encryption dictionary is not present, this callback is 
	used to determine if one of the registered security handlers can be used 
	to open the document.

	@param pdDoc The document being opened.
	@param encryptDict The encryption dictionary.

	@return true if the handler supports the format of encryptDict.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptCanParseEncryptDictProc)(
	PDDoc pdDoc,
	CosObj encryptDict);

/**
	Callback for PDCryptHandler. This function should extract 
	and return information about the document permissions for 
	display to the user: whether the user can print, edit, copy 
	text and graphics, edit notes and do form fill in and signing. 
	

	The permissions returned are logically ANDed with the document 
	permissions returned by any other permissions handlers and 
	displayed to the user. All crypt handlers should implement 
	this call so that consolidated permissions can be displayed. 
	To display your own crypt handler's permissions, implement 
	PDCryptDisplaySecurityDataProc. 

	If this callback is absent, Acrobat assumes that all the 
	operations on the document are allowed. 
	@param pdDoc The document whose permissions are obtained. 
	
	@param perms (Filled by the callback) An array of the 
	document's permissions. For each combination of PDPermReqObj 
	and PDPermReqOpr, the value is true if the operation is 
	allowed for the object, false if it is not. 
	@param version (Filled by the callback) Pointer to the 
	version number of PDPermReqObj and PDPermReqOpr with which 
	this crypt handler is compatible (specified by the constant 
	PDPermReqVersion).
	@see PDDocPermRequest 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptGetDocPermsProc) (
    PDDoc pdDoc, ASBool perms[PDPermReqObjLast][PDPermReqOprLast], ASInt16 *version );

/**
	(Optional) Callback for PDCryptHandler.
	Determines whether a document's metadata will be encrypted.  If this call is not 
	implemented, the metadata is always encrypted.  Note that documents with plaintext 
	metadata can be opened only by Acrobat versions 6.0 and later.

	@param pdDoc IN The document being encrypted

	@return true if document metadata should be encrypted.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptEncryptDocMetadata)(PDDoc pdDoc);

/**
	Callback for PDCryptBatchHandler. This callback puts up
	a dialog that allows a user to enter data that will be used
	to batch secure a series of files. The data is stored in
	an ASCab which is part of a batch sequence file. The actual
	security data, including password( s), should be stored
	as a pointer in the ASCab so that password information is
	not serialized to disk. Pointers are not serialized from
	ASCabs, but ASTexts, ASInt32s, and ASBools are serialized.

	@param settings IN/OUT? An object of type ASCab.
	@return true indicates success.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchShowDialogProc)(ASCab settings);

/**
	Callback for PDCryptBatchHandler. Developer should provide
	information about the current batch settings for the security
	handler. Batch settings are provided as a read-only ASCab
	that is passed to the function. A writable ASCab is also
	provided, which should be used to store parameter information
	about the security handler. The description information
	should be stored starting in the paramDesc ASCab using ASText
	objects starting with key " 1". Example: key=" 1", value="
	Title: API Reference" (ASText object)
	@param settings IN/OUT? Batch settings.
	@param paramDesc IN/OUT? Description information.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchParamDescProc)(const ASCab settings, ASCab paramDesc);

/**
	Callback for PDCryptBatchHandler. Different from the regular
	PDCryptHandler NewAuthData function. Create and return a
	void* which is the authorization data for the batch security
	handler. This data should be used to both batch open files
	and batch secure files. Therefore, make sure to provide
	password information for both the pdPermOpen and pdPermSecure
	cases. This data will be passed to the PDCryptBatchAuthorizeProc
	callback and eventually to PDCryptBatchFreeAuthDataProc
	to free the data. This authorization data is collected before
	any files are opened in the batch sequence. It is permitted
	to display a UI at this point since the batch operation
	has not started yet. The data applies to all files, and
	therefore could represent one or more passwords which can
	be enumerated in the BatchAuthorize function which receives
	the batch authData.
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptBatchNewAuthDataProc)(void);

/**
	Callback for PDCryptBatchHandler. Called when PDF file is
	opened. First called with NULL authData for the case without
	a user password. Called again with authorization data provided
	for the security handler that matches the one used in the
	PDF file.

	NOTE: This function is called a batch operation and therefore
	should not display any user interface. During a batch operation,
	a file will first be opened with the pdPermSecure bit set.
	It will then be opened with the pdPermOpen bit set.
	@param pdDoc IN/OUT? The document being opened.
	@param reqObj IN/OUT? Object type that is the focus of the request:
	one of the PDPermReqObj values.
	@param reqOpr IN/OUT? Operation type that is the focus of the request:
	one of the PDPermReqOpr values.
	@param authData IN/OUT? Authorization data. Its format is security
	handler-specific.
	@return PDPermReqStatus
*/
typedef ACCBPROTO1 PDPermReqStatus (ACCBPROTO2 *PDCryptBatchAuthorizeProc)(PDDoc pdDoc,
	PDPermReqObj reqObj, PDPermReqOpr reqOpr, void *authData);

/**
	Callback for PDCryptBatchHandler. If provided, must be used
	to free authData acquired via BatchGetAuthData or BatchNewAuthData.
	If no BatchFreeAuthData function is provided, a default
	one will be used which calls ASfree on the authData if it
	is non-NULL.
	@param authData IN/OUT? Authorization data. Its format is security
	handler-specific.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchFreeAuthDataProc)(void *authData);

/**
	Callback for PDCryptBatchHandler. This function is called
	at the beginning of a batch sequence before any files have
	been opened. This allows a security handler to be called
	back with the ASCab of settings that were filled out by
	the BatchShowDialog function, or by an ASCab that was read
	in from disk. Pointers of security information are not serialized
	to disk, and therefore a security information structure
	may need to be regenerated based on other security information
	in the ASCab. It is permitted for the BatchPreSequence callback
	to put up a UI asking the user for more information since
	the batch sequence has not started yet. If this function
	returns false, the viewer will assume that the command cannot
	be executed and will cancel the sequence.
	@param settings IN/OUT? Batch settings.
	@return See above.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchPreSequenceProc)(ASCab settings);

/**
	Callback for PDCryptBatchHandler. This function is called
	at the end of a batch sequence after all files have been
	processed. Any memory that was allocated in the BatchPreSequence
	call should be cleaned up in this callback.
	@param settings IN/OUT? Batch settings.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchPostSequenceProc)(ASCab settings);

/**
	Callback for PDCryptBatchHandler. This function should update 
	the crypt handler's security data without bringing up a 
	dialog. This data is provided by a PDCryptBatchShowDialogProc. 
	The current security data can be obtained by calling PDDocGetNewSecurityData. 
	This function should return true unless there is a problem 
	with the batch data for this security handler. 

	NOTE: This function is called a batch operation and therefore 
	should not display any user interface. 
	@param pdDoc The document whose data is updated. 
	@param settings An object of type ASCab. 
	@param cryptHandler An object of type ASAtom. 
	@param secDataP Pointer to the document's security data.
	@return See above. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchUpdateSecurityDataProc)(
	PDDoc pdDoc, ASCab settings, ASAtom *cryptHandler, void **secDataP);

/* Call back routine prototypes for Crypt Filter support */

/**
	(Optional) Callback for PDCryptFilterHandler. Callbacks 
	that conform to this prototype are called to encrypt or 
	decrypt streams from a document. 

	The first call to a procedure of this type must fill out 
	an ASCryptStmRec structure with pointers to callback routines 
	for various types of stream access; see ASCryptStmProcs. 
	
	@param dP The document containing the stream. 
	@param filterName The name of the security filter in use. 
	
	@param stm The security streamstructure, containing the 
	stream itself, access information, and an ASCryptStmRec. 
	This callback function is called upon opening the stream; 
	the first call must initialize the stream. 
	@param handOff true if the method should close the base 
	stream when it closes the filter, false otherwise. 
	@param params A Cos object containing parameters for the 
	operation, as specified for the filter. 
	@param stmLength For a decryption operation, the requested 
	number of bytes; for an encryption operation, the number 
	of bytes in the stream contained in stm. 
	@see PDCryptFilterAuthorizeProc 
	@see PDCryptFilterGetDataProc 
	@see PDCryptFilterStringProc 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFilterStreamProc)(
	CosDoc dP, ASAtom filterName, ASCryptStm stm, ASBool handOff, CosObj params, ASInt32 stmLength);

/**
	(Optional) Callback for PDCryptFilterHandler. Acrobat's 
	security mechanism calls this method to determine whether 
	the user should have access to this filter. 
	@param dP The document for which to perform authorization. 
	
	@param filterName The name of the security filter. 
	@param encryptDict The encryption dictionary to use. 
	@param bEncrypt true if authorization is for an encryption 
	operation, false if it is for a decryption operation. 
	@param bUIAllowed true if the caller can bring up the 
	security dialogs as needed, false otherwise.
	@return true if the user has access to this filter, false otherwise. 
	
	@see PDCryptFilterGetDataProc 
	@see PDCryptFilterStreamProc 
	@see PDCryptFilterStringProc 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptFilterAuthorizeProc)(
	CosDoc dP, ASAtom filterName, CosObj encryptDict, ASBool bEncrypt, ASBool bUIAllowed);

/**
	(Optional) Callback for PDCryptFilterHandler. Acrobat's 
	security mechanism calls this method to retrieve the encryption/decryption 
	key for this filter. Called only when the filter's encryption 
	method is V2. 
	@param dP The document whose data is retrieved. 
	@param filterName The name of the security filter. 
	@param key (Filled by the method) A pointer to the encryption/decryption 
	key. 
	@param bNewKey true if you want to retrieve a new key 
	(changed since opening the file), false otherwise. 
	@param bUIAllowed true if the caller can bring up the 
	security dialogs as needed, false otherwise.
	@see PDCryptFilterAuthorizeProc 
	@see PDCryptFilterStreamProc 
	@see PDCryptFilterStringProc 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDCryptFilterGetDataProc)(
	CosDoc dP, ASAtom filterName, char **key, ASBool bNewKey, ASBool bUIAllowed);

#define PDCryptFilterStringProc	CosCryptStringProc

/**   PDCrypt Filter support

	- Ecnryption callback routines are optional. However, security handler is responsible for not allowing app to cause 
	Full Save.
	- String encryption/decryption support is not required.
	- DecryptStream callback can be NULL if crypt filter handler does not perform encryption/decryption on its own.  
	In such case, GetDataProc must be supplied.
	- Crypt Filter handlers are expected to return 0 length buffer (or EOF) when unauthorized access is made.

	CryptStringProcs - string callback is expected to return desired buffer size (required destination buffer length) 
	when NULL is passed as destination buffer.  This mechanism is to allow different buffer length upon 
	encryption/decryption.

	About ASCryptStm in callback: Upon the first call (back) to Encrypt/Decrypt stream procs, handler is expected to 
	fill out ASCryptStmRec. ASCryptStm has pointer to callback routines for various stream access. Subsequent stream 
	access are made to these callback routines.  See ASExpT.h for ASCryptStmRec definition.
*/
typedef struct _t_PDCryptFilterHandler {
	/** Set this to sizeof(PDCryptFilterHandlerRec). */
	ASSize_t size;
	/** */
	PDCryptFilterAuthorizeProc	Authorize;
	/** Called to get crypt data when app's built-in method is used */
	PDCryptFilterGetDataProc	GetData;	
	/** */
	PDCryptFilterStreamProc		DecryptStream;	
	/** */
	PDCryptFilterStreamProc		EncryptStream;
	/** */
	PDCryptFilterStringProc		DecryptString;
	/** */
	PDCryptFilterStringProc		EncryptString;
} PDCryptFilterHandlerRec;

typedef struct _t_PDCryptFilterHandler *PDCryptFilterHandler;

/**
	Callbacks used to open secured files and to modify security 
	settings while batch is processing a list of files. These 
	callbacks are not called while opening files through the 
	UI. In addition, the regular PDCryptHandler functions are 
	not called during batch operations. 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
*/
typedef struct _t_PDCryptBatchHandler {
	/** Set this to sizeof(PDCryptBatchHandlerRec).*/
	ASSize_t size;

	/** This function should put up a dialog that allows a user to enter data
		that will be used to batch secure a series of files.  The data is stored in an
		ASCab which is part of a Batch sequence file.  The actual security data, including
		password(s), should be stored as a pointer in the ASCabinet so that password information
		is not serialized to disk.  Pointers are not serialized from ASCabs, but ASTexts, ASInt32s,
		and ASBools are serialized.
	*/
	PDCryptBatchShowDialogProc BatchShowDialog;

	/** Developer should provide information about the current batch settings for the security handler.
		Batch settings are provided as a read-only ASCab which is passed in to the function.  A
		writeable ASCab is also provided, which should be used to store parameter information about
		the security handler.  The description information should be stored starting in the
		paramDesc ASCab using ASText objects starting with key "1".
		
		Example:  key="1", value="Title: API Reference" (ASText object)
	*/
	PDCryptBatchParamDescProc BatchParamDesc;

	/** Different from the regular PDCryptHandler NewAuthData function.
		Create and return a void* which is the authorization data for the batch security handler.
		This data should be used to both batch open files and batch secure files.  Therefore,
		make sure to provide password information for both the pdPermOpen and pdPermSecure cases.
		and passed to Authorize and eventually to ASfree.
		
		This authorization data is collected before any files are opened in the Batch sequence.
		It is permitted to display a UI at this point since the Batch operation has not started yet.
		The data applies to all files, and therefore could represent one or more passwords which
		can be enumerated in the BatchAuthorize function which receives the Batch authData.
	*/
	PDCryptBatchNewAuthDataProc BatchNewAuthData;

	/** Called when PDF file is opened.  First called with NULL authData for the case without a
		user password.  Called again with authorization data provided for the security handler
		that matches the one used in the PDF file.
		
		IMPORTANT: This function is called a Batch operation and therefore should not display any UI.
		
		During a Batch operation, a file will first be opened with the pdPermSecure bit set.  It
		will then be opened with the pdPermOpen bit set.
	*/
	PDCryptBatchAuthorizeProc BatchAuthorize;

	/** If provided, must be used to free authData acquired via
		BatchGetAuthData or BatchNewAuthData.  If no BatchFreeAuthData function is provided,
		a default one will be used which calls ASfree on the authData if it is non-NULL.
	*/
	PDCryptBatchFreeAuthDataProc BatchFreeAuthData;

	/** This function should update the crypt handler's security data without bringing
		up a dialog.  This data is provided by a PDCryptShowBatchDialogProc.
		The current security data can be obtained by calling PDDocGetNewSecurityData.
		This function should return true unless there is a problem with the batch data
		for this security handler.
		
		IMPORTANT: This function is called a Batch operation and therefore should not display any UI.
	*/
	PDCryptBatchUpdateSecurityDataProc BatchUpdateSecurityData;

	/** This function is called at the beginning of a Batch sequence before any files have been
		opened.  This allows a security handler to be called back with the ASCab of settings that
		were filled out by the BatchShowDialog function, or by an ASCabinet that was read in
		from disk.  Pointers of security information are not serialized to disk, and therefore
		a security information structure may need to be regenerated based on other security
		information in the ASCabinet.
		
		It is permitted for the BatchPreSequence callback to put up a UI asking the user for
		more information since the Batch sequence has not started yet.
		
		If this function returns false, the viewer will assume that the command cannot be executed.
		and will cancel the sequence.
	*/
	PDCryptBatchPreSequenceProc BatchPreSequence;

	/** This function is called at the end of a Batch sequence after all files have been
		processed.  Any memory that was allocated in the BatchPreSequence call should be
		cleaned up in this callback.
	*/
	PDCryptBatchPostSequenceProc BatchPostSequence;
} PDCryptBatchHandlerRec;

typedef struct _t_PDCryptBatchHandler *PDCryptBatchHandler;


/**
	Data structure containing callbacks that implement a security handler. The callbacks
	implement the security handler functions. For example, get authorization data such as
	a password from the user, set permissions, read and write security-related data in a
	PDF file, and so on.
	@see PDRegisterCryptHandler
	@see PDRegisterCryptHandlerEx
*/
typedef struct _t_PDCryptHandler {
	/** Set this to sizeof(PDCryptHandlerRec).  PDRegisterCryptHandler
		uses this to determine if the caller was compiled with an old
		version of this structure declaration.  PDRegisterCryptHandler
		will raise genErrBadParam if the size does not correspond to
		a known size of this struct.
	*/
	ASSize_t size;
		
	/** This function will be called when a user tries to open or set
		security for an encrypted document.  PermsWanted will be either
		pdPermOpen or pdPermSecure.  This function should return the
		permissions granted based on the authData.  For opening, the
		permissions returned usually should be pdPermOpen and some or
		all of pdPermPrint, pdPermEdit, and pdPermCopy.  For setting
		security, permissions returned should be pdPermAll.  However,
		if authorization fails, 0 should be returned.  The function is
		first called with authData NULL.  If that fails, GetAuthData
		is called and the authData from it is passed to Authorize.
		
		If this function is called to authorize Open, decryption will
		not yet have been installed.  So while any part of the document
		may be examined, some calls must be avoided.  For example, a
		call that causes a page to be parsed will probably result in an
		error since the encrypted contents will be parsed.  In general,
		it is safe to obtain information about the presence or absence
		of things, or the number of things, and to examine any part of
		a document at the Cos level.
	*/
	PDCryptAuthorizeProc Authorize;

	/** Create and return a new struct that can be filled out
		and passed to Authorize and eventually to ASfree.
		This function is not called by the standard security mechanism
		but may be called by extensions that want to gain access
		to a protected document.  This function need not be
		implemented if clients can simply allocate data using
		ASmalloc.  In fact, the standard CryptHandler does not.
	*/
	PDCryptNewAuthDataProc NewAuthData;
		
	/** This function obtains authorization data from the user.  As for
		Authorize, permsWanted will be either pdPermOpen or
		pdPermSecure.  This function should allocate authData to be used
		by Authorize.  The function
		should return true unless the operation should be cancelled
		(for example, if the user cancels a dialog).
		A crypt handler can specify the standard password dialog by using
		AVCryptGetPassword.  In this case, the authData will be a char *
		that should be freed using ASfree after Authorize is called.
	*/
	PDCryptGetAuthDataProc GetAuthData;

	/** Create new struct that contains info corresponding to info
		in security dialog.  If encryptDict is a dict, initialize
		security data to correspond to the dict.  Otherwise, set up
		defaults.  This function will be called when opening a doc
		with encryptDict set to the doc's encryptDict.  It will also
		be called when a user chooses new encryption.
	*/
	PDCryptNewSecurityDataProc NewSecurityData;

	/** Validate the security data, modifying it as necessary. A client
		may have called PDDocNewSecurityData to obtain a new security
		data structure, then modified it, and then called
		PDDocSetNewSecurityData to change the document security.
		Validate is called before actually setting the document's
		security data.
	*/
	PDCryptValidateSecurityDataProc ValidateSecurityData;		

	/** This function should update the crypt handler's security data,
		usually by bringing up a dialog.  The current security data
		can be obtained by calling PDDocGetNewSecurityData.
		Like GetAuthData, this function should return true unless cancelled.
		The security data should be created with ASmalloc so that it can
		later be freed by ASfree.
		The function can also update the cryptHandler itself.
		For example, the built-in encryption
		switches to no encryption if no passwords or permissions are
		set in the security dialog.
	*/
	PDCryptUpdateSecurityDataProc UpdateSecurityData;

	/** Set up the key to be passed to initialize the RC4 cipher for
		encryption and decryption.  The length may not be greater than
		5 to satisfy the current export control regulations.
		Data should be allocated by ASmalloc or a relative
		so that it may be freed by ASfree.
	*/
	PDCryptNewCryptDataProc NewCryptData;

	/** This function should fill the encryptDict with whatever
		information is to be stored in the document.  Unlike all other
		strings and streams, direct object elements of the encryptDict
		are not encrypted automatically.  They must be encrypted before
		they are inserted into the dictionary.
	*/
	PDCryptFillEncryptDictProc FillEncryptDict;
	
	/** This function should return information about security for
		display to the user:  whether the document has owner and
		user passwords and whether the user password enables
		printing, editing, copying text and graphics, and editing notes.
		See PDexpt.h for possible permissions.  All other bits in secInfo
		should be set to 1.
		This function is also used after a SaveAs to reset the
		permissions according to the current document.
	*/
	PDCryptGetSecurityInfoProc GetSecurityInfo;
		
	/* New calls for Acrobat 3.0 are below. */

	/** If provided, must be used to free securityData acquired via
		NewSecurityData.
	*/
	PDCryptFreeSecurityDataProc FreeSecurityData;
		
	/** If provided, must be used to free authData acquired via
		GetAuthData or NewAuthData.
	*/
	PDCryptFreeAuthDataProc FreeAuthData;
		
	/** If provided, must be used to free cryptData acquired via
		NewCryptData.
	*/
	PDCryptFreeCryptDataProc FreeCryptData;
		
	/* New call for Acrobat 4.05 are below. */

	/** Set up the key to be passed to initialize the RC4 cipher and
		the version of algorithm for encryption and decryption.  The
		key will be truncated when the length is greater than the viewer
		currently supports.
		Data will be freed by FreeCryptData if provided.  Otherwise,
		ASfree is used.
	*/
	PDCryptNewCryptDataExProc NewCryptDataEx;		

	/* New call for Acrobat 5.0 and later. */

	/** This was added to replace PDCryptGetAuthDataProc.  We now have new permission
		controls.  PDPerms are obsolete. Yet, the viewer still supports old security
		handler. It is called whenever viewer need to get authorize data and/or check
		permission for operation(s).
	*/
	PDCryptAuthorizeExProc AuthorizeEx;
		
	/** This was added to replace PDCryptGetAuthDataProc.  We now have new permission
		controls.  PDPerms are obsolete. Yet, the viewer still supports old security
		handler. It is called whenever viewer need to get authorize data and/or check
		permission for operation(s).
	*/
	PDCryptGetAuthDataExProc GetAuthDataEx;

	/** When security handler get called, it should pop up an dialog with current
		permission settings.  This callback was added to provide security handler with
		way to display its custom permission settings. It should return true, if it wants
		a call back to modify security settings.  Otherwise, it should return false.
	*/
	PDCryptDisplaySecurityDataProc DisplaySecurityData;

	/**
		Used for Acrobat WebBuy proprietary method of passing crypt data.
	*/
	PDCryptReservedProc GetReservedData;

	/** Pointer to PDCryptBatchHandler structure.  If this parameter is non-NULL, then
		the security handler will work in a Batch environment, either for decrypting
		PDF files or encrypting them on Save with a new security handler.
	*/
	PDCryptBatchHandler CryptBatchHandler;
		
	/* New call for Acrobat 6.0 and later */

	/** This call is used to provide PDCrypt handler interoperability.  When an encrypted 
		ducument is being opened and the the security handler specified in the encryption
		dictionary is not present, the viewer will call this function on existing security
		handlers to see if one of them can be used to open the document 
	*/
	PDCryptCanParseEncryptDictProc CanParseEncryptDict;

	/** Pointer to PDCryptFilterHandler structure.  If this parameter is non-NULL, then
		the security handler support Crypt Filter.
	*/
	PDCryptFilterHandler CryptFilterHandler;		

	/** This function should return information about the document permissions
		for display to the user: whether the user can print, edit, copy text and 
        graphics, edit notes and do form fill in and signing. See PDexpt.h for 
        possible permissions.
	*/
	PDCryptGetDocPermsProc GetDocPerms;

	/** Determines whether a document's metadata will be encrypted.  If this call is not 
	    implemented, the metadata is always encrypted.
	*/
	PDCryptEncryptDocMetadata EncryptDocMetadata;

	/** New call added for Acrobat 7.0
	    Used for openeing rolled back document (original portion of signed document). If 
		this call is not implemented, security handler is asked for authorization. (Since 
		authorization is already asked for the document, this would be second authorization call
		for rolled back part.)
		Callee is expected to authorize opening of this document using already opened document, 
		including subsequent callback to authorize Crypt Filter(s).
	*/
	PDCryptNewSecurityDataFromOriginalDocProc	NewSecurityDataFromOrgDoc;
} PDCryptHandlerRec;

typedef struct _t_PDCryptHandler *PDCryptHandler;

/*------------------------------------------------------------------------
	PDNameTree
------------------------------------------------------------------------*/

/**
	The dictionary used to store all of the Named Destinations in a PDF file. A name tree
	is used to map Cos strings to Cos objects just as a Cos dictionary is used to map Cos
	names to Cos objects. However, a name tree can have many more entries than a Cos
	dictionary can. You create a PDNameTree and locate it where you think is appropriate
	(perhaps under a page, but most often right under the catalog).
	Name trees use Cos-style strings (not null-terminated C strings), which may use
	Unicode encoding, and these may contain bytes with zeroes in them (high bytes of
	ASCII characters).
	@see PDDocCreateNameTree
	@see PDNameTreeNew
	@see PDNameTreeFromCosObj
	@see PDNameTreeEnum
*/
typedef OPAQUE_64_BITS PDNameTree;

/*------------------------------------------------------------------------
	PDNumTree
------------------------------------------------------------------------*/

/**
	An object that points to the root node of a number tree inside a PDF file. A number
	tree is used to map integers to arbitrary Cos objects just as a Cos dictionary is used
	to map Cos names to Cos objects. However, a number tree can have many more
	entries than a Cos dictionary can.
	@see PDNumTreeNew
	@see PDNumTreeFromCosObj
	@see PDNumTreeEnum
*/
typedef OPAQUE_64_BITS PDNumTree;

/*------------------------------------------------------------------------
    PDPageLabel
--------------------------------------------------------------------------*/

/**
A label used to describe a page. This is used to allow for non-sequential page
numbering or the addition of arbitrary labels for a page (such as the inclusion of
Roman numerals at the beginning of a book). A PDPageLabel specifies the
numbering style to use (for example, upper- or lower-case Roman, decimal, and so
forth), the starting number for the first page, and an arbitrary prefix to be preappended
to each number (for example, "A-" to generate "A-1", "A-2", "A-3", and so
forth.)
@see PDDocGetPageLabel
@see PDDocGetLabelForPageNum
@see PDPageLabelFromCosObj
@see PDPageLabelNew
@see PDDocRemovePageLabel
*/
typedef OPAQUE_64_BITS PDPageLabel;

/*------------------------------------------------------------------------
    PDPageRange
	Used to specify a range of pages when printing
--------------------------------------------------------------------------*/

/**
	Specifies a range of pages in a document. Page numbers begin 
	with 0. 
*/
typedef struct _t_PDPageRange {
	/** Starting page number.*/
	ASInt32				startPage;
	/** Ending page number.*/
	ASInt32				endPage;
	/** Pages in the range to print. Must be one of: PDAllPages, PDEvenPagesOnly, or PDOddPagesOnly.*/
	ASInt32				pageSpec;	
} PDPageRange;

/**
	passed to the PDDocWillPrintDocInMode notification
	to specify the type of print operation being performed.
	
	@see PDDocWillPrintDocInMode
*/
enum {

	/** Print only the document. */
	PDPrintWhat_DOCUMENT, 

	/** Print the document and associated annotations. */
	PDPrintWhat_DOCUMENT_AND_COMMENTS, 

	/** Print the document and stamp annots (new default for 7) */
	PDPrintWhat_DOCUMENT_AND_STAMPS, 

	/** Print only the data within form fields. */
	PDPrintWhat_FORM_FIELDS_ONLY, 

	/** */
	PDPrintWhat_COUNT,

	/** */
	PDPrintWhat_MIN=PDPrintWhat_DOCUMENT
};
typedef ASEnum8 PDPrintWhat;

/*------------------------------------------------------------------------

	Optional Content API typedefs, defines, etc. PDF 1.5, Acrobat 6.0

--------------------------------------------------------------------------*/
/** A PDOCG represents a named object whose state can be toggled in a UI to affect
	changes in visibility of content.
 */
typedef struct _t_PDOCG *PDOCG;

/** A PDOCMD is an object that is attached to content to indicate membership in
	an OCG or group of OCGs.
 */
typedef struct _t_PDOCMD *PDOCMD;

/** A PDOCContext is an object that keeps track the on/off states of all of the OCGs
	in a document. There can be more than one PDOCContext object, representing
	different combinations of OCG states. The PDDoc contains an internal PDOCContext
	that is used for on-screen drawing and as the default state used for any other
	drawing or content enumeration. Clients can change the states of OCGs within
	any PDOCContext. Clients can build (and save in the PDF file) PDOCContexts with their
	own combination of OCG states, and issue drawing or enumeration commands using
	their own PDOCContext instead of the document's internal PDOCContext.
	All discussion of "visibility" of content is therefore meant to be with respect
	to the OCG states stored in a specific PDOCContext.
 */
typedef struct _t_PDOCContext *PDOCContext;

/** A PDOCConfig represents a set of states and other information that is saved in
	a PDF file for future use. There is a document default configuration, saved
	in the /D entry in the OCProperties dict, and a list of other client configurations,
	saved as an array of configurations in the /Configs entry in the OCProperties dict.
	PDOCConfigs are typically used to initialize the OCG states for a client's
	PDOCContext.
 */
typedef struct _t_PDOCConfig *PDOCConfig;

/** PDOCMDVisPolicy represents the 4 legal values for the /P key in an OCMD dictionary.
	They specify the visibility of content with respect to the on/off state of the OCGs
	listed in the OCMD.
 */
enum {
	/** */
	kOCMDVisibility_AllOn,
	/** */
	kOCMDVisibility_AnyOn,
	/** */
	kOCMDVisibility_AnyOff,
	/** */
	kOCMDVisibility_AllOff
};
typedef ASUns8 PDOCMDVisPolicy;

/** PDOCContextInitPolicy is used to specify how to initialize the states of OCGs
	when calling PDOCContextNew or PDOCContextInit
 */
enum {
	/** */
	kOCCInit_OFF,
	/** */
	kOCCInit_ON,
	/** */
	kOCCInit_FromOtherContext,
	/** */
	kOCCInit_FromConfig
};
typedef ASUns8 PDOCContextInitPolicy;

/** PDOCBaseState enumerates the 3 legal values for the BaseState key in an optional
	content configuration dictionary (PDOCConfig) When initializing a PDOCContext
	using KOCCInit_FromConfig, this enumeration represents the starting state of the
	OCGs before the contents of the config's ON and OFF ocg lists are processed.
	If the BaseState is Unchanged, and the PDOCConfig is just being constructed,
	the current states of the ocgs from the PDDoc's own PDOCConfig are used.
 */
enum {
	/** */
	kPDOCBaseState_OFF,
	/** */
	kPDOCBaseState_ON,
	/** */
	kPDOCBaseState_Unchanged
};
typedef ASUns8 PDOCConfigBaseState;

/** PDOCDrawEnumType controls drawing or enumerating the page wrt optional content.
	Enumerated type that, together with the NonOCDrawing value, 
	controls drawing or enumerating content on a page with optional 
	content. 
	
	-  Content that is marked as optional content is drawn or 
	not drawn according to the PDOCDrawEnumType and the visibility 
	state as determined by the OCGs and OCMDs. 
	-  Content that is not marked as optional content is drawn 
	when NonOCDrawing is true, and not drawn when NonOCDrawing 
	is false. 
	
	@see PDOCContextGetOCDrawEnumType 
	@see PDOCContextSetOCDrawEnumType 
	@see PDOCContextGetNonOCDrawing 
	@see PDOCContextSetNonOCDrawing 
*/
enum {
	/**  Draw or enumerate optional content 
		that is visible, according to the current state of OCGs 
		and OCMDs. This is the normal default mode. 
	*/
	kPDOC_VisibleOC = 0,
	/** Draw or enumerate all optional content, 
		regardless of its visibility state. If the context's NonOCDrawing 
		is true, all contents of document are shown. 
	*/
	kPDOC_AllOC,		
	/**	Draw or enumerate no optional content, 
		regardless of its visibility state. If the context's NonOCDrawing 
		is false, nothing is drawn, resulting in a blank page.
	*/
	kPDOC_NoOC,		
	/** */
	kPDOC_LastDrawEnumType = kPDOC_NoOC
};
typedef ASUns8 PDOCDrawEnumType;

/**	Optional-content group state changing. 
*/
enum {
	/** OCGs' states changing */
	kPDOCGState,    
	/** PDOCContext's PDDrawEnumType changing */
	kPDOCContextDrawEnumType, 
	/** PDOCContext's non-OC drawing changing */
	kPDOCContextNonOCDrawing, 
	/** PDOCContext's intent changing */
	kPDOCContextIntent,      
	/** PDOCContext being reset using PDOCContextInit */
	kPDOCContextInit,        
	/** */
	kPDOC_LastContextChangeType = kPDOCContextInit
};
typedef ASUns8 PDOCContextChangeType;

/** PDDocOCChangeType is an enumeration of types of changes to the OC structures
	of a PDDoc. These types of changes may effect visibility in *all* PDOCContexts.
	This enumeration is used in the PDDocOC{Will,Did}Change notifications.
	These notifications typically passes in the effected page, or PDAllPages if all
	pages may be affected.
 */
enum {
	/** OCG(s) created
	*/
	kPDOCGCreate,				
	/** OCG properties changed
	*/
	kPDOCGProperties,			
	/** OCG replaced by another
	*/
	kPDOCGReplace,				
	/** OCG destroyed
	*/
	kPDOCGDestroy,				
	/** content made optional
	*/
	kPDOCMDAttach,				
	/** content made optional
	*/
	kPDOCMDRemove,				
	/** oc config created
	*/
	kPDOCConfigCreate,			
	/** oc config changed
	*/
	kPDOCConfigChange,			
	/** oc config destroyed
	*/
	kPDOCConfigDestroy,			
	/** oc removed from doc
	*/
	kPDDocRemoveOC,				
	/**
	*/
	kPDOC_LastDocChangeType = kPDDocRemoveOC
};
typedef ASUns8 PDDocOCChangeType;

/* These callbacks are used for enumerating PDOCGs, PDOCMDs and PDOCConfigs.
 * Enumeration stops when all PDOCGs (PDOCMDs, PDOCConfigs) have been enumerated,
 * or if the callback returns false.
 */
/**
	A callback used for enumerating optional-content groups (OCGs). Enumeration stops when
	all OCGs have been enumerated, or when the callback returns false.
	@param ocg IN/OUT? The optional-content group object
	@param clientData IN/OUT? Pointer to user-supplied data to pass to proc each time it is called.
	@return true to continue enumeration, false to halt enumeration.
	@see PDDocEnumOCGs
	@see PDPageEnumOCGs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDOCGEnumProc )(PDOCG ocg, void *clientData);
/**
	A callback used for enumerating optional-content configurations. 
	Enumeration stops when all configurations have been enumerated, 
	or when the callback returns false. 
	@param occonfig The optional-content configuration object 
	
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return true to continue enumeration, false to halt enumeration. 
	
	@see PDDocEnumOCConfigs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDOCConfigEnumProc)(PDOCConfig occonfig, void *clientData);
/* Controlling Acrobat trap presets */
enum {

/** */
kPDJoinMiter,

/** */
kPDJoinRound,

/** */
kPDJoinBevel
};
typedef ASEnum8 PDJoinStyle;
enum {

/** */
kPDEndMiter,

/** */
kPDEndOverlap
};
typedef ASEnum8 PDEndStyle;

enum {
kPDPlacementCenter,
kPDPlacementChoke,
kPDPlacementNeutralDensity,
kPDPlacementSpread
};
typedef ASEnum8 PDPlacementTypes;

typedef struct {
	ASBool noTrap;
	ASBool defaultTrap;
	float trapWidth;
	float blackWidth;
	float imageTrapWidth;
	ASUns32 trapJoinStyle;
	ASUns32 trapEndStyle;
	ASUns32 stepLimit;
	ASUns32 blackColorLimit;
	float blackDensityLimit;
	ASUns32 slidingTrapLimit;
	ASUns32 trapColorScaling;
	ASUns32 trapPlacement;
	ASBool imageToImageTrapping;
	ASBool imageToObjectTrapping;
	ASBool imageInternalTrapping;
	ASBool imagemaskTrapping;
	
} PDTrapPresetRec, *PDTrapPreset;

/*------------------------------------------------------------------------
	Controlling acrobat-based separations
--------------------------------------------------------------------------*/
enum { /* options for font inclusion */
	/** Embed no fonts.
	*/
	kHSEmitFontNoFonts,	
	/** Emit all embedded fonts
	*/
	kHSEmitFontEmbeddedFonts,	
	/** Emit all fonts
	*/
	kHSEmitFontAllFonts		
};

enum { /* Options for what to do with a plate when doing separations */
	/**
	*/
	kEmitPlate,
	/**
	*/
	kDontEmitPlate,
	/**
	Represents an ink used on a page. 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
	@see PDPageEnumInks 
*/
	kConvertToProcess
};

/** ink types */
enum { 
	/** */
	kNormal,
	/** */
	kTransparent,
	/** */
	kOpaqueInk,
	/** */
	kOpaqueIgnore
};

enum {	/* marksStyle choices */
	/** No flags == InDesign style printer marks */
	kPDDefaultMarkType = 0,		/* Acrobat defaults to InDesign style marks */
	kPDInDesignJ1MarkType,		/* InDesignJ1 */
	kPDInDesignJ2MarkType,		/* InDesignJ2 */
	kPDIllustratorMarkType,  	/* Illustrator */
	kPDIllustratorJ,			/* IllustratorJ */
	kPDQuarkXPress				/* QuarkXPress */
};
/**
	Represents an ink used on a page. 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
	@see PDPageEnumInks 
*/
typedef struct _t_PDPageInkRec {
	/** The size of the structure. Must be set to
		sizeof(PDPageInkRec). 
	*/
	ASSize_t size;			
	/** Name of colorant from Separation or DeviceN colorspace, or
		process color name.
	*/
	ASAtom colorantName;		
	/** true if this is a process color, false if it is a spot color.
	*/
	ASBool isProcessColor;		
	/** How to handle the colorant for a separation preview. Value can be:
		 kEmitColorant
		 kDontEmitColorant
		 kConvertToProcess
	*/
	ASUns8 whatToDo;            
	/** RGB values for on-screen display of a colorswatch.
	*/
	ASUns8 r, g, b;				
	/** The frequency for this ink.
	*/
	float	frequency;			
	/** The angle for this ink.
	*/
	float angle;				
	/** ink type for this ink.
	*/
	ASUns8 inkType;				
	/** trap sequence for this ink.
	*/
	ASInt32 trapSequence;		
	/** neutral density for this ink.
	*/
	/* CMYK values for spot inks, provided so that neutral density can be calculated */
	float cyan;
	float magenta;
	float yellow;
	float black;

	float density;				
	/** alias this spot to another.
	*/
	ASAtom alias;				

} PDPageInkRec, *PDPageInk;

/** PDPageEnumInksCallback is used for enumerating the inks on a page via PDPageEnumInks */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPageEnumInksCallback) (PDPageInk ink, void *clientData);

/** PDHostSepsPlateRec is used to specify per-plate options when doing separations */
typedef struct _t_PDHostSepsPlateRec {
	/** The size of the structure. Must be set to
		sizeof(PDHostSepsPlateRec).
	*/
	ASSize_t size;				
	/** Name of colorant from Separation or DeviceN
		colorspace, or a spot or process color name (Cyan,
		Magenta, Yellow, Black).
	*/
	ASAtom colorantName;		
	/** How to handle the colorant. Value can be:
		kEmitPlate
		kDontEmitPlate
		kConvertToProcess
	*/
	ASUns8 whatToDo;			
	/** Set internally. Used to determine whether marks
		were made on this plate to aid in blank detection.
	*/
	ASUns32 wasColorSet;		
	/** Set internally. Stream into which Acrobat emits
		PostScript for this plate. NULL if whatToDo is not
		set to kEmitPlate.
	*/
	ASStm epsStm;				
	/** Set internally. Used only when separating
		documents, to close the file afterward.
	*/
	ASFile file;				
	/** Set internally. Used only when separating
		documents, to reopen the stream on the next page.
	*/
	ASPathName path;			
	/** The frequency for this ink. -1 to use the default value.
	*/
	float	frequency;			
	/** The angle for this ink. -1 to use the default value.
	*/
	float	angle;				
	/** ink type for this ink.
	*/
	ASUns8 inkType;				
	/** trap sequence for this ink.
	*/
	ASInt32 trapSequence;		
	/** neutral density for this ink.
	*/
	float density;				
	/** alias this spot to another.
	*/
	ASAtom alias;				
} PDHostSepsPlateRec, *PDHostSepsPlate;

/** PDHostSepsSpecRec s used to control the generation of separations from Acrobat */
typedef struct _t_PDHostSepsSpecRec {
	/**The size of the structure. Must be set to
		sizeof(PDHostSepsSpecRec).
	*/
	ASSize_t size;						

	/** The PostScript printing level. 2 means emit as level
		2, 3 means level 3.
		Used if the emitToPrinter or emitToFile
		print parameter is true.
	*/
	ASUns32 psLevel;					
	/** true if a binary channel to the printer is
		supported, false otherwise.
		Used if the emitToPrinter or emitToFile
		print parameter is true.
	*/
	ASBool binaryOK;					

	/** When true, emit annotations.
	*/
	ASBool emitAnnots;
	/** When true, emit halftones.
	*/
	ASBool emitHalftones;
	/** When true, emit transfer functions.
	*/
	ASBool emitTransferFuncs;
	/** When true, emit separable images only.
	*/
	ASBool emitSeparableImagesOnly;

	/** When true, suppress CJK substitution.
	*/
	ASBool suppressCJKSubstitution;
	/** Font output options. Can be:
		kHSEmitFontNoFonts: Embed no fonts.
		kHSEmitFontEmbeddedFonts: Emit all embedded fonts.
		kHSEmitFontAllFonts: Emit all fonts.
	*/
	ASEnum8 emitFontOption;				
	/** When true, send TrueType fonts as TrueType
		fonts (level 3 and most level 2 PS printers). When
		false, convert TT to T1 (typically desirable only
		for Level 1 PS where no TT handling is present).
	*/
	ASBool TTasT42;						
	/** true means do not include far-east fonts.
	*/
	ASBool  printerHasFarEastFonts;		

	/** The transparency level, 0-100.
	*/
	ASUns32 transparencyLevel;			

	/** When true, color manage DeviceCMYK. When
		false, pass it directly onto the process plates.
	*/
	ASBool	useCMYKWorkingColorspace;	

	/** The profile description of a valid CMYK profile, such
		as the strings seen in the Color Management panel
		of the General Preferences dialog; for example,
		"U.S. Web Coated (SWOP) v2".
	*/
	char    destProfile[256];			

	/** When true, use overprint preview (OPP) for
		convert to process, better simulating what would
		happen if the spot ink were really used.
	*/
	ASBool convertToProcessUsingOPP;	

	/** Number of items in the plates array.
	*/
	ASUns32 numPlates;					
	/** List of the colorant names and what to do with
		them for separations.
	*/
	PDHostSepsPlate *plates;		
	/** The optional-content context to use for visibility
		state information, or NULL to use the document's
		current states in the default context.
	*/
	PDOCContext ocContext;				
	/** When true, apply print-specific visibility state
		settings from the optional-content group.
	*/
	ASBool      applyOCGPrintOverrides; 
	/** When true, invert the plate.
	*/
	ASBool		negative;				
	/** One of the following constants:
		kPDPrintFlipNone = 0x01,
		kPDPrintFlipX = 0x02,
		kPDPrintFlipY = 0x04,
		kPDPrintFlipXY = 0x08
		Mirroring is done in the PostScript output stream.
	*/
	ASEnum8		mirrorprint;			
	/** Page mark indication. A bit-wise OR of the
		PDPageMarkFlags values.
	*/
	ASUns32		whichMarks;				
	/** When true, use western style for page marks.
	*/
	ASBool		western;	/* obsolete */		
	ASBool		doInRipTrapping;	/* enable In RIP trapping */
	ASInt32		marksStyle;	/* 0 == default, indicates printer marks style to use */
} PDHostSepsSpecRec, *PDHostSepsSpec;

/*------------------------------------------------------------------------*/

/**
	Callback for PDDocExportNotes. Determines whether an annotation
	is exported or not.

	NOTE: This is a different callback than PDDocWillExportAnnotProc.

	@param doc IN/OUT? The document from which annotations may be exported.

	@param pdpage IN/OUT? The page from which the annotation may be
	exported.
	@param src IN/OUT? The annotation that may be exported.
	@param dict IN/OUT? Copy of annot in a Cos object.
	@return true to export annot, false to not export annot.
	@see PDDocWillImportAnnotCallback
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillExportAnnotCallback)(
	PDDoc doc, PDPage pdpage, PDAnnot src, CosObj dict);

/**
	Callback for PDDocImportCosDocNotes and PDDocImportNotes.
	Determines whether an annotation will be imported or not.


	NOTE: This is a different callback than PDDocWillImportAnnotProc.

	@param doc IN/OUT? The document into which annotations may be imported.

	@param pdPage IN/OUT? The page in which the annotation may be imported.

	@param annot IN/OUT? The annotation that may be imported.
	@return true to import annot, false to not import annot.
	@see PDDocWillExportAnnotCallback
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillImportAnnotCallback)(
	PDDoc doc, PDPage pdPage, PDAnnot annot);

/**
	Structure used by PDDocCopyToFile to specify file copy information. 
	
	@see PDDocCopyToFile 
*/
typedef struct _t_PDDocCopyParams {

	/** Size of the data structure. Must be set to sizeof(PDDocCopyParamsRec).  */
	ASSize_t			size;	

	/** Pathname to copy to, specified in whatever format is correct for fileSys.  */
	ASPathName			newPath;	

	/** Pointer to an ASFileSysRec containing the file system that does the copy. */
	ASFileSys			fileSys;	

	/** A progress monitor. May be NULL */
	ASProgressMonitor	progMon;	

	/** Pointer to user-supplied data to pass to progMon each time it is
		called Must be NULL if progMon is NULL. */
	void				*progMonData;

	/** A cancel procedure. May be NULL. */
	ASCancelProc		cancelProc;	

	/** Pointer to user-specified data to pass to cancelProc each time it is called. Must be NULL 
		if cancelProc is NULL. 
		*/
	void				*cancelProcData;

	/**  Whether changes should be saved if 
		the document is dirty. When true, if the document is dirty 
		and the product is Acrobat, save all in-memory changes to 
		the new copy. Otherwise, just copy the on-disk bytes. Ignored 
		in Adobe Reader.
	*/
	ASBool				saveChanges;	
} PDDocCopyParamsRec, *PDDocCopyParams;

/** @name PrinterEmitFlags for PDTile->marksflags 
*/
/*@{*/

/** nothing */
#define kPDEmitNoMarks			0	

/** tile marks*/
#define	kPDEmitWesternTileMarks	0x0001	

/** tile marks*/
#define	kPDEmitEasternTileMarks	0x0002	

/** emit info about document, name, page #, etc. */
#define kPDEmitSlug			0x0004	
/*@}*/

/** Printing flags 

@see PDDocDidPrintTiledPage
@see PDDocPrintingTiledPage
@see PDDocWillPrintTiledPage
*/
typedef struct {

	/** # of points to overlap (UI units may anything, clients convert to points */
	ASUns32	overlap;	

	/** center the pages' contents on the physical paper */
	ASBool	center;		

	/** what printer marks do we emit?
		@see PrinterEmitFlags
	*/
	ASInt32	marksflags;		

	/** width of paper (pts), client-provided, since client has PPD access */
	ASInt32 paperWidth;	

	/** height of paper (pts)*/
	ASInt32 paperHeight; 

	/** title string for slug (optional) */
	char *docTitle;		

	/** date string for slug (optional) */
	char *docDate;		

	/** time string for slug (optional) */
	char *docTime;		
	/* the remaining fields are for communicating during print time
	the current page's state; which page is being printed, etc. */

	/** current col (0 - numcols-1) */
	ASInt32 col;		

	/** current row */
	ASInt32 row;		

	/** numCols for this page*/
	ASInt32 numCols;	

	/** numRows */
	ASInt32 numRows;	

	/** amount to shift right 1'st tile to center entire image on sheets */
	ASInt32 xOffset;	

	/** amount to shift down */
	ASInt32 yOffset;	
}PDTileRec, *PDTile;

/*****************************
** Definitions for logical page areas
******************************/

/** Different logical areas on a page. */
enum {

	/** */
	kPDPageArea,

	/** */
	kPDClipArea,

	/** */
	kPDNumAreas
};
typedef ASEnum16 PDPageArea;

/**
	Used by PDDocExportSomeNotes; represents an array of PDAnnots. 
	
	@see PDDocExportSomeNotes 
*/
typedef struct _s_PDAnnotArray
{

	/** Annotation count */
	ASTArraySize annotCount;

	/** Pointer to an array of PDAnnots. */
	PDAnnot *annots;
} PDAnnotArrayRec, *PDAnnotArray;


/************************************************************
 ***
 *** Drawing params for PDPageDrawWithParams, PDDrawCosObjWithParams. Acrobat 6.0
 ***
 ************************************************************/
/**
	Parameters used for optional-content drawing control. The 
	parameters are the same as those passed to the original 
	version of the method, with the addition of an optional-content 
	context that determines what contents are visible. 
	@see PDDrawCosObjWithParams 
	@see PDPageEnumContents 
*/
typedef struct _t_PDDrawParams
{
	/** Size of the data structure.
	*/
	ASUns32 size;			
	/** Pointer to a platform-dependent window object
		(WindowPtr or CWindowPtr in Mac OS, or HWND in
		Windows). In Mac OS, to draw into an offscreen
		GWorld, pass NULL in window and pass the
		GWorldPtr in displayContext. In Windows, to draw
		into an offscreen DC, pass NULL for window.
	*/
	void *window;			
	/** A platform-dependent display context structure
		(GWorldPtr in Mac OS,HDC in Windows). In Mac OS,
		displayContext is ignored if window is non-NULL
		NOTE: Note: displayContext cannot be reliably
		used as the hDC for a printer device.
	*/
	void *displayContext;	
	/** Pointer to the matrix to concatenate onto the default
		page matrix. It is useful for converting from page to
		window coordinates and for scaling.
	*/
	ASFixedMatrix matrix;	
	/** See above. The only value you should use is 0.
	*/
	ASUns32 flags;			
	/** A rectangle represented by the coordinates of its four sides.
	*/
	ASFixedRect updateRect;	
	/**
		Procedure called periodically to check for user cancel of
		the drawing operation. The default cancel procedure
		can be obtained using AVAppGetCancelProc. May
		be NULL in which case no cancel procedure is used.
	*/
	CancelProc cancelProc;	
	/** Pointer to user-supplied data to pass to cancelProc
		each time it is called. Should be NULL if cancelProc
		is NULL.
	*/
	void *cancelProcClientData;

	/** An optional-content context that determines what
		contents are visible. NULL uses the document's
		optional-content context, as returned by
		PDDocGetOCContext(pddoc), which is equivalent
		to calling the version of the method without optionalcontent
		parameters.
		This context is copied and the copy is used in drawing.
		This allows a client to change its copy of the context
		without raising an exception.
	*/
	PDOCContext clientOCContext;
} PDDrawParamsRec, *PDDrawParams;

/************************************************************
 ***
 *** Enumeration params for PDFormEnumPaintProcWithParams, PDCharProcEnumWithParams,
 *** PDPageEnumContentsWithParams.
 ***
 ************************************************************/

	/**
	Enumeration parameters used for optional-content drawing 
	control in PDFormEnumPaintProcWithParams and PDCharProcEnumWithParams. 
	The parameters are the same as those passed to the original 
	versions of these methods (PDFormEnumPaintProc and PDCharProcEnum) 
	with the addition of an optional-content context that determines 
	what contents are visible. 
	@see PDFormEnumPaintProcWithParams 
	@see PDCharProcEnumWithParams 
*/
typedef struct _t_PDGraphicEnumParams {
	/**Size of the data structure. 
	*/
	ASUns32 size;			     
	/** An optional-content context that 
		determines what contents are visible. NULL uses the document's 
		optional-content context, as returned by PDDocGetOCContext(pddoc), 
		which is equivalent to calling the version of the method 
		without optional-content parameters. This context is copied 
		and the copy is used in drawing. This allows a client to 
		change its copy of the context without raising an exception.
	*/
	PDOCContext clientOCContext;  

	/** Filled by the method with the context 
		that will be used during enumeration. This is a copy of 
		the context specified by clientOCContext. 
	*/
	PDOCContext usedOCContext;    

	/** The graphic enumeration monitor. 
	*/
	PDGraphicEnumMonitor mon;     
	/** Pointer to user-supplied data to pass to the monitor.
	*/
	void *monObj;                 
} PDGraphicEnumParamsRec, *PDGraphicEnumParams;


/************************************************************/
/************************************************************
 ***
 *** Callbacks for PDDocRequestPages and PDDocRequestEntireFile
 ***
 ************************************************************/

/** Tell the callback why we are calling them back */
enum {
	/** Request still being processed 
	*/
	kPDDocRequestUnderway = 0, 
	/** Requested data has arrived
	*/
	kPDDocRequestComplete, 
	/** Request is cancelled due to file being closed
	*/
	kPDDocRequestCancelled, 
	/** An error occurred 
	*/
	kPDDocRequestError		
};
typedef ASEnum8	PDDocRequestReason;

/** Callback for PDDocRequestPages */
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDDocRequestPagesProc)
(PDDoc pdDoc,  ASInt32 startPage,  ASInt32 nPages, PDDocRequestReason reason,  void *clientData);

/**
	Callback used by PDDocRequestEntireFile. Use this callback 
	to process a document file. 
	@param pdDoc The PDDoc to be saved.
	@param reason A constant representing the status 
	of the request. One of:
	
	-  kPDDocRequestUnderway
	-  kPDDocRequestComplete 
	-  kPDDocRequestCancelled
	-  kPDDocRequestError 
	
	@param clientData User-supplied data passed in the PDDocRequestPages 
	method.
	@return 0 when successful, otherwise a non-zero error code. 
	@see PDDocRequestEntireFile 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDDocRequestEntireFileProc)
(PDDoc pdDoc,  PDDocRequestReason reason, void *clientData);

/************************************************************/

/************************************************************
 ***
 *** Params structure for PDDocCreateWithParams
 ***
 ************************************************************/

enum {
    kPDCreateWithoutMetaData = 0x00000001
};

typedef ASFlagBits PDCreateFlags;

    
typedef struct _t_PDDocCreateParams {
    ASSize_t   size;
    PDCreateFlags createFlags;
} PDDocCreateParamsRec, *PDDocCreateParams;
        

/************************************************************/

/* This typedef appears in both PDExpT.h and PEExpT.h now, so avoid multiple typedef's here */
#ifndef _T_PDEFONT_
#define _T_PDEFONT_
typedef struct _t_PDEFont *PDEFont;
#endif

enum {
	kPDHorizLeft = 0, 
	kPDHorizCenter, 
	kPDHorizRight
};
typedef ASEnum8	PDHorizAlign;

enum {
	kPDVertTop = 0, 
	kPDVertCenter, 
	kPDVertBottom
};
typedef ASEnum8	PDVertAlign;

/**
	Parameters used for describing text-based watermarks.
	@see PDDocAddWatermarkFromText
*/
typedef struct _t_PDDocWatermarkTextParams {
	/** Size of the data structure. 
	*/
	ASSize_t		size;

	/** The text to be used when generating at text-based watermark.
	*/
	ASText			srcText;

	/** The alignment to be used when justifying a text-based watermark.
	*/
	PDHorizAlign	textAlign;

	/** The PDEFont to be used when generating a text-based watermark.
		If NULL, then font specified by sysFontName will be used.
	*/
	PDEFont			pdeFont;

	/** The name of a system font to be used when generating a text-based
		watermark.  The font will be embedded/subsetted when possible.
		This parameter is ignored if pdeFont is non-NULL.
	*/
	ASAtom			sysFontName;

	/** The size of the font in points to be used when generating a text-based
		watermark.
	*/
	float			fontSize;

	/** The color to be used when generating a text-based watermark.
	*/
	PDColorValueRec	color;
} PDDocWatermarkTextParamsRec, *PDDocWatermarkTextParams;


/**
	Parameters used for adding and describing watermarks.
	@see PDDocAddWatermarkFromPDPage
	@see PDDocAddWatermarkFromText
*/
typedef struct _t_PDDocAddWatermarkParams {
	/** Size of the data structure. 
	*/
	ASSize_t		size;

	/** The page range of the document to which the watermark should be added.
	*/
	PDPageRange		targetRange;

	/** A boolean specifying whether or not this watermark is a FixedPrint watermark.
		FixedPrint watermarks maintain their size and position regardless of the
		dimensions of the target media.
	*/
	ASBool			fixedPrint;

	/** A boolean specifying where in the page z-order the watermark should be added.
		If true, then the watermark is added to the front of the page, otherwise it is
		added as a background.
		This parameter is ignored if fixedPrint is true, as all FixedPrint watermarks are
		added to the front of the page.
	*/
	ASBool			zOrderTop;

	/** A boolean specifying whether or not the watermark should be visible when viewing
		on-screen.
	*/
	ASBool			showOnScreen;

	/** A boolean specifying whether or not the watermark should be printed.
	*/
	ASBool			showOnPrint;

	/** The horizontal alignment to be used when adding the watermark to a page.
	*/
	PDHorizAlign	horizAlign;

	/** The vertical alignment to be used when adding the watermark to a page.
	*/
	PDVertAlign		vertAlign;

	/** The horizontal offset value to be used when adding the watermark on a page.  If
		percentageVals is true, then this value is a percentage of the page width,
		with 1.0 being 100%.  Otherwise this value is in UserUnits.
	*/
	float			horizValue;

	/** The vertical offset value to be used when adding the watermark on a page.  If
		percentageVals is true, then this value is a percentage of the page height,
		with 1.0 being 100%.  Otherwise this value is in UserUnits.
	*/
	float			vertValue;

	/** A boolean specifying the units of horizValue and vertValue.  If true, then
		horizValue and vertValue represent percentages of the page dimensions.
		Otherwise horizValue and vertValue are in user units.
	*/
	ASBool			percentageVals;

	/** The scale factor to be used when adding the watermark, with 1.0 being 100%.
	*/
	float			scale;

	/** The rotation (counter-clockwise) in degrees to be used when adding the watermark.
	*/
	float			rotation;

	/** The opacity to be used when adding the watermark, with 0.0 being fully
		transparent and 1.0 being fully opaque.
	*/
	float			opacity;

	/** The progress monitor to be updated when adding the watermark.
		May be NULL.
	*/
	ASProgressMonitor	progMon;

	/** The private data to be passed to progMon.
		This parameter is ignored if progMon is NULL.
	*/
	void*				progMonData;

	/** The cancel procedure to be checked when adding the watermark.
		May be NULL.
	*/
	ASCancelProc		cancelProc;

	/** The private data to be passed to cancelProc.
		This parameter is ignored if cancelProc is NULL.
	*/
	void*				cancelProcData;
} PDDocAddWatermarkParamsRec,  *PDDocAddWatermarkParams;


#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_PDExpT */
