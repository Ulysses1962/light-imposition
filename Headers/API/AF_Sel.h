/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AF_Sel.h

 - Selectors for all AcroForm HFT functions.

*********************************************************************/

#ifndef _H_AF_SEL
#define _H_AF_SEL

#include "AF_ExpT.h"

enum
{	AcroFormFirst_SEL  = 0,
	IsPDDocAcroForm_SEL,
	AFPDDocLoadPDFields_SEL,
	AFPDDocEnumPDFields_SEL,
	AFPDDocGetPDFieldFromName_SEL,
	AFPDFieldFromCosObj_SEL,
	AFPDFieldGetCosObj_SEL,
	AFPDFieldIsValid_SEL,
	AFPDFieldIsTerminal_SEL,
	AFPDFieldGetValue_SEL,
	AFPDFieldGetFlags_SEL,
	AFPDFieldGetName_SEL,
	AFPDFieldIsAnnot_SEL,
	AFPDFieldSetValue_SEL,
	AFPDFieldSetFlags_SEL,
	AFPDFieldSetOptions_SEL,
	AFPDFieldReset_SEL,
	ExportAsFDF_SEL,
	ExportAsHtml_SEL,
	ImportAnFDF_SEL,
	ResetForm_SEL,
	AcroFormRegisterObserver_SEL,
	AcroFormUnregisterObserver_SEL,
	AFGetScriptingContext_SEL,
	ExportAsFDFEx_SEL,
	ExportAsHtmlEx_SEL,
	AssembleFormAndImportFDF_SEL,
	ExportAsFDFWithParams_SEL,
	AFPDFormFromPage_SEL,
	/* this call is obsoleted; see the new AFLayoutNew */
	AFLayoutNewOBSOLETE_SEL,
	AFLayoutDelete_SEL,
	/* this call is obsoleted; see the new AFLayoutCreateStream */
	AFLayoutCreateStreamOBSOLETE_SEL,
	AFLayoutBorder_SEL,
	AFLayoutText_SEL,
	AFPDFieldValueChanged_SEL,
	AFPDWidgetGetRotation_SEL,
	AFPDFieldGetDefaultTextAppearance_SEL,
	AFPDFieldSetDefaultTextAppearance_SEL,
	AFPDWidgetGetBorder_SEL,
	AFPDWidgetSetBorder_SEL,
	AFPDWidgetGetAreaColors_SEL,
	AFPDWidgetSetAreaColors_SEL,
	AFImportAppearance_SEL,
	AFLayoutTextEx_SEL,
	AFLayoutIconText_SEL,
	AFGetScriptingContextEx_SEL,
	AFExecuteThisScript_SEL,
	/* new anyfont-able AFLayout calls here */
	AFLayoutNew_SEL,
	AFLayoutCreateStream_SEL,
	AFCalculateFields_SEL,
	AFDrawText_SEL,
	AFPDFieldGetDefaultTextAppearanceEx_SEL,
	AFPDFieldSetDefaultTextAppearanceEx_SEL,
	AcroFormLast_SEL  };

#define AcroFormNum_SEL  AcroFormLast_SEL - 1
#define AcroFormHFT_NAME "Forms"
#define AcroFormHFT_LATEST_VERSION (0x00010002)

/**
	Indicates whether a PDDoc contains an Acrobat form. 
	@param doc The PDDoc to test. 
	@return true if doc contains a form, false otherwise. 
	@see AFPDFieldIsValid 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *IsPDDocAcroForm_SELPROTO)
		(PDDoc doc);

/**
	Makes sure every PDField in the given PDDoc exists. 

	NOTE: It is no longer necessary to call this method. It 
	still exists for backwards compatibility, but its purpose 
	is now automatically taken care of internally. 
	@param doc The PDDoc for the form whose PDFields are loaded.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDDocLoadPDFields_SELPROTO)
		(PDDoc doc);

/**
	Enumerates all PDFields that exist in a PDDoc. 
	@param doc The PDDoc whose PDFields are enumerated. 
	@param terminals If true, only PDFields without children 
	are enumerated. 
	@param parameterIgnored This parameter is no longer used (gets ignored).
	@param proc A user-supplied callback that is called for 
	each PDField. The enumeration terminates if proc returns 
	false. 
	@param clientData Pointer to user-specified data that 
	is passed to proc each time it is called.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDDocEnumPDFields_SELPROTO)
		(PDDoc doc, ASBool terminals, ASBool parameterIgnored, AFPDFieldEnumProc proc, void *clientData);

/**
	Retrieves a PDField with a given name from a PDDoc. (If 
	multiple fields have the same name, a change to any of them 
	affects all of them.) 
	@param doc The PDDoc containing the field. 
	@param name Name of the field to retrieve. 
	@return The PDField specified by name. Returns NULL if there is 
	no PDField with the given name in the PDDoc. 
	@see AFPDFieldGetName 
*/
typedef ACCBPROTO1 PDField (ACCBPROTO2 *AFPDDocGetPDFieldFromName_SELPROTO)
		(PDDoc doc, char* name);

/**
	Retrieves the PDField object for which a Cos object is the 
	dictionary. 
	@param dict The Cos object for which to retrieve the corresponding 
	PDField object.
	@return The PDField corresponding to dict. Returns NULL if the Cos 
	object is not a PDField. 
	@see AFPDFieldGetCosObj 
*/
typedef ACCBPROTO1 PDField (ACCBPROTO2 *AFPDFieldFromCosObj_SELPROTO)
		(CosObj dict);

/**
	Retrieves the Cos object which is the field object of a 
	PDField object. 
	@param fldP The PDField object for which to retrieve the 
	corresponding Cos object.
	@return Cos object corresponding to the PDField object fldP. 
	@see AFPDFieldGetCosObj 
	@see AFPDFieldGetDefaultTextAppearance 
	@see AFPDFieldGetName 
	@see AFPDFieldGetValue 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *AFPDFieldGetCosObj_SELPROTO)
		(PDField fldP);

/**
	Determines whether a field is valid or not. 

	NOTE: This method is intended only to ensure that the field 
	has not been deleted, not to ensure that all necessary information 
	is present and the field has valid values. 
	@param fldP The field to test.
	@return true if fldP is a valid PDField; false otherwise. 
	@see AFPDFieldIsAnnot 
	@see AFPDFieldIsTerminal 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFPDFieldIsValid_SELPROTO)
		(PDField fldP);

/**
	Determines whether a field is terminal: the field has no 
	children or it has the same name as its children. 
	@param fldP The PDField to test. 
	@return If true, fldP is valid and all its children (if any) have 
	the same name as fldP. Otherwise all have unique names. 
	
	@see AFPDFieldIsAnnot 
	@see AFPDFieldIsValid 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFPDFieldIsTerminal_SELPROTO)
		(PDField fldP);

/**
	Retrieves the value from a PDField. The value is stored 
	in the field entry with the V key. 

	Values can be inherited. If a PDField Cos object does not 
	have a V key, its value is inherited from a parent's V key 
	(the parent must have the same name as the PDField). For 
	example, radio buttons inherit values this way. 

	NOTE: Retrieving the value of a radio button or combo box 
	requires Cos-level programming. See the PDF Reference for 
	details. 
	@param fldP The PDField whose value is retrieved.
	@return The value of the field as a CosObj. If the field has no 
	value, a NULL CosObj is returned. 
	@see AFPDFieldGetCosObj 
	@see AFPDFieldGetDefaultTextAppearance 
	@see AFPDFieldGetName 
	@see AFPDFieldSetValue 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *AFPDFieldGetValue_SELPROTO)
		(PDField fldP);

/**
	Retrieves the flags of a PDField for a given flag type. 
	
	@param fldP The PDField whose flags are obtained. 
	@param flagType Type of flags to obtain.
	@return The value of flags of flagType. 
	@see AFPDFieldGetName 
	@see AFPDFieldGetValue 
	@see AFPDFieldSetFlags 
*/
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFPDFieldGetFlags_SELPROTO)
		(PDField fldP, AF_Flags_t flagType);

/**
	Gets the name of a PDField object. 
	@param fldP The PDField whose name is retrieved. 
	@return A C program NULL-terminated string. 

	NOTE: Do not modify or free this string. To use this string 
	indefinitely, copy it to a private buffer. 
	@see AFPDFieldGetDefaultTextAppearance 
	@see AFPDFieldGetValue 
*/
typedef ACCBPROTO1 char* (ACCBPROTO2 *AFPDFieldGetName_SELPROTO)
		(PDField fldP);

/**
	Determines whether or not a PDField is an annotation (PDAnnot), 
	that is, whether the field dictionary is also an annotation. 
	If this is the case, the value of Subtype is Widget. 
	@param fldP The field in question. 
	@return true if the PDField is an Acrobat annotation, false otherwise. 
	
	@see AFPDFieldIsTerminal 
	@see AFPDFieldIsValid 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFPDFieldIsAnnot_SELPROTO)
		(PDField fldP);

/**
	Sets the value for a PDField. The value is stored in the 
	field entry with the V key. Updates the display of the field 
	and its namesakes; that is, fields with the same fully qualified 
	names, if any. Changing the field dictionary's value for 
	the V entry directly does not change the appearance of the 
	field, which is represented by the AP key. 

	NOTE: Setting the value of a radio button or combo box requires 
	Cos-level programming. See the PDF Reference for details. 
	
	@param fldP The field whose value is set. 
	@param val The field's new value. If it is different than 
	the previous value, all previous instances of value keys 
	(V) in the field's dictionary and those of its namesakes 
	(fields with the same fully qualified name) are removed.
	@see AFPDFieldGetValue 
	@see AFPDFieldSetFlags 
	@see AFPDFieldSetOptions 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFPDFieldSetValue_SELPROTO)
		(PDField fldP, CosObj val);

/**
	Sets the flags of type flagType for a PDField. 

	The flags of a field's children (if any) are also set. 
	@param fldP The PDField for which to set flags and children's 
	flags. 
	@param flagType The type of the flags to set. If flagType 
	is Flags_Annot and the PDField is not an annotation, no 
	flags are changed. 
	@param flags The value of the flags. 
	@see AFPDFieldGetDefaultTextAppearance 
	@see AFPDFieldSetOptions 
	@see AFPDFieldSetValue 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldSetFlags_SELPROTO)
		(PDField fldP, AF_Flags_t flagType, AFPDFieldFlags_t flags);

/**
	Sets the options entry for a field. The options entry has 
	the key Opt and represents a list of options for a choice 
	field. 

	If the field is not valid, remove the options entry. 
	@param fldP The PDField whose options are set. 
	@param array The value to set in the options entry.
	@return Good if the operation succeeded, Bad otherwise. 
	@see AFPDFieldSetFlags 
	@see AFPDFieldSetValue 
*/
typedef ACCBPROTO1 RetCode (ACCBPROTO2 *AFPDFieldSetOptions_SELPROTO)
		(PDField fldP, CosObj array);

/**
	Sets a PDField's value to its default state. This is the 
	value associated with the DV key for the field. If there 
	is no DV key for the field, set the field's value to the 
	NULL Cos object; or, if the field has options, the default 
	DV is the first element of the Opt array. 

	A DV entry's value can be inherited from a parent (just 
	like V). 

	The PDField's value is set only if it is terminal; see AFPDFieldIsTerminal. 
	
	@param fldP The PDField to reset. 
	@see ResetForm 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldReset_SELPROTO)
		(PDField fldP);

/**
	Exports form data to a CosDoc, which can be written to an 
	FDF file. See the PDF Reference for a description of this 
	format. 

	To create an FDF file from this CosDoc, call the Core API 
	method CosDocSaveToFile. 

	NOTE: ExportAsFDFWithParams provides the same functionality 
	with additional options. 
	@param pdForm The PDDoc for the form for which we want 
	to export the data. 
	@param rgIncExcFlds If rgIncExcFlds is CosNewNull, then 
	all fields are exported, and bIncl is ignored. If rgIncExcFlds 
	is a CosArray, then the array elements may be: a) names 
	of fields (the names may be of non-terminal fields, which 
	is a fast and easy way to cause all their children to be 
	included in the FDF file). b) arrays, whose first element 
	is a field name, and the rest of the elements are field 
	dictionary key names whose values should be exported in 
	the FDF file. For example: [ (My listbox) /AP /Opt ]. This 
	variety of rgIncExcFlds array element can only be used if 
	bIncl is true (see below). c) If rgIncExcFlds contains a 
	single element, which is itself an array as described in 
	b) above, and its first element, which corresponds to the 
	field name, is NULL, then the FDF file will include the 
	requested field properties of all fields. For example: [ 
	null /F /Ff ]. 
	@param bIncl If true, rgIncExcFlds is an array of the 
	fields to export. Otherwise, rgIncExcFlds is an array of 
	the fields to exclude from exporting. 
	@param bEmpty If true, all fields selected per the above 
	criteria are exported. If false, exclude fields that have 
	no value. 
	@param bMenu If true, suppresses saving text fields that 
	have the "password' flag set, and does not force filling-in 
	required fields when creating an FDF file. 
	@param bLoadFields IN Not used.
	@param fdfPath IN The path where the FDF file will be saved 
	(by the client of ExportAsFDF) after it is produced. You need 
	this in order to create an FDF file with an F key that gives t
	he relative path to the form, from the location where the FDF 
	file will be saved. Pass NULL if an absolute pathname is desired.
	@return The FDF CosDoc containing the exported data.
	@exception Raises gAFpdErrExportFdf if it cannot export field data. 
	The viewer may raise other exceptions.
	@see ExportAsFDFEx 
	@see ExportAsFDFWithParams 
	@see ImportAnFDF 
	@see AssembleFormAndImportFDF 
*/
typedef ACCBPROTO1 CosDoc (ACCBPROTO2 *ExportAsFDF_SELPROTO)
		(PDDoc pdForm, CosObj rgIncExcFlds, ASBool bIncl, ASBool bEmpty,
		 ASBool bMenu, ASBool bLoadFields, ASPathName fdfPath);

/**
	Exports data from a form to a file in HTML format. 
	@param pdForm The PDDoc for the form whose data is exported. 
	@param rgIncExcFlds If rgIncExcFlds is CosNewNull, then 
	all fields are exported, and bIncl is ignored. If it is 
	a CosArray, then the array elements must be indirect references 
	to field dictionaries. 
	@param bIncl If true, rgIncExcFlds is an array of the 
	fields to export. Otherwise, rgIncExcFlds is an array of 
	the fields to exclude from exporting. 
	@param bEmpty If true, all fields selected per the above 
	criteria are exported. If false, exclude fields that have 
	no value. 
	@param Hfile File to which the HTML data is written.
	@see ExportAsHtmlEx 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ExportAsHtml_SELPROTO)
		(PDDoc pdForm, CosObj rgIncExcFlds, ASBool bIncl, ASBool bEmpty, ASFile Hfile);

/**
	Imports data from an FDF file into a PDDoc's form. See the 
	PDF Reference for a description of this format. 
	@param pdForm The PDDoc for the form into which we want 
	to import the data. 
	@param cdFDF The CosDoc for the FDF file containing the 
	data.
	@return true if the fields in the FDF file only contained values 
	or flags (that is, V, F, Ff, ClrF, ClrFf, SetF, SetFf), 
	false if any field contained other attributes, such as appearances 
	(that is, AP), actions (that is, A), and so forth. 
	@exception Raises gAFpdErrBadFdf if the FDF file is invalid. 
	@exception Raises an exception if memory cannot be allocated for FDF 
	file data. 
	@see AssembleFormAndImportFDF 
	@see ExportAsFDF 
	@see ExportAsFDFEx 
	@see ExportAsFDFWithParams 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ImportAnFDF_SELPROTO)
		(PDDoc pdForm, CosDoc cdFDF);

/**
	Resets the indicated fields of a PDDoc's form to their default 
	values. 

	A PDField's value is reset only if it is terminal; see AFPDFieldIsTerminal. 
	
	@param pdForm The PDDoc for the form whose fields are 
	reset. 
	@param rgIncExcFlds If rgIncExcFlds is CosNewNull, then 
	all fields are reset, and bIncl is ignored. If it is a CosArray, 
	then the array elements must be names of fields. The names 
	may be of non-terminal fields, which is a fast and easy 
	way to cause all their children to be reset. 
	@param bIncl If true, rgIncExcFlds is an array of the 
	fields to reset. Otherwise, rgIncExcFlds is an array of 
	the fields to exclude from resetting.
	@see AFPDFieldSetDefaultTextAppearance 
	Callback used by AFPDDocEnumPDFields. It is called once 
	for each PDField in a form. 
	@return true to continue the enumeration, false to halt enumeration.
	@see AFPDDocEnumPDFields 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ResetForm_SELPROTO)
		(PDDoc pdForm, CosObj rgIncExcFlds, ASBool bIncl);

/** */
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *AcroFormRegisterObserver_SELPROTO)
		(AF_NotificationSelector_t notificationID, AF_NotificationProc notifyProc,
		AF_NotificationFailureProc notifyFailure, void *clientData);

/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AcroFormUnregisterObserver_SELPROTO)
		(AF_NotificationSelector_t notificationID, ASInt32 observerID);

/** */		
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFGetScriptingContext_SELPROTO)
		(void** pcx, void** pobj);

/**
	Exports form data to a CosDoc, which can be written to an 
	FDF file. See the PDF Reference for a description of this 
	format. 

	To create an FDF file from this CosDoc, call the Acrobat 
	viewer plug-in API method CosDocSaveToFile. 

	NOTE: This method is the same as ExportAsFDF, with the exception 
	of the additional parameter submitBtnName. ExportAsFDFWithParams 
	provides the same functionality with additional options. 
	
	@param pdForm A PDDoc for the form whose data is exported. 
	
	@param rgIncExcFlds If rgIncExcFlds is CosNewNull, then 
	all fields are exported, and bIncl is ignored. If rgIncExcFlds 
	is a CosArray, then the array elements may be: a) names 
	of fields (the names may be of non-terminal fields, which 
	is a fast and easy way to cause all their children to be 
	included in the FDF file). b) arrays, whose first element 
	is a field name, and the rest of the elements are field 
	dictionary key names whose values should be exported in 
	the FDF file. For example: [ (My listbox) /AP /Opt ]. This 
	variety of rgIncExcFlds array element can only be used if 
	bIncl is true (see below). c) If rgIncExcFlds contains a 
	single element, which is itself an array as described in 
	b) above, and its first element, which corresponds to the 
	field name, is NULL, then the FDF file will include the 
	requested field properties of all fields. For example: [ 
	null /F /Ff ]. 
	@param bIncl If true, rgIncExcFlds is an array of the 
	fields to export. Otherwise, rgIncExcFlds is an array of 
	the fields to exclude from exporting. 
	@param bEmpty If true, all fields selected per the above 
	criteria are exported. If false, exclude fields that have 
	no value.
	@param bMenu IN If true, suppresses saving text fields that 
	have the “password” flag set, and doesn’t force filling-in 
	required fields when creating an FDF file.
	@param bLoadFields IN Not used.
	@param fdfPath IN The path where the FDF file will be saved 
	(by the client of ExportAsFDF) after it is produced. You need 
	this in order to create an FDF file with an F key that gives 
	the relative path to the form, from the location where the FDF 
	file will be saved. Pass NULL if an absolute pathname is desired.
	@param submitBtnName IN A NULL-terminated string containing 
	the name of the button used to submit. If the value passed is 
	not NULL, then the FDF file will include a field dictionary 
	corresponding to the submit button, which will only contain 
	one key, namely T.
	NOTE:This dictionary is no different than the one you get when 
	an AcroForm has an empty text field (that is, no value), and 
	parameter bEmpty is true.
	@return The FDF CosDoc containing the exported data.
	@exception Raises gAFpdErrExportFdf if it cannot export field data. 
	The viewer may raise other exceptions.
	@see ExportAsFDF 
	@see ExportAsFDFWithParams 
	@see ImportAnFDF 
	@see AssembleFormAndImportFDF 
*/
typedef ACCBPROTO1 CosDoc (ACCBPROTO2 *ExportAsFDFEx_SELPROTO)
		(PDDoc pdForm, CosObj rgIncExcFlds, ASBool bIncl, ASBool bEmpty,
		 ASBool bMenu, ASBool bLoadFields, ASPathName fdfPath,
		 const char* submitBtnName);

/**
	Exports data from a form to a file in HTML format. 

	NOTE: This method is the same as ExportAsHtml, with the 
	exception of the additional parameter submitBtnName. 
	@param pdForm The PDDoc for the form whose data is exported. 
	
	@param rgIncExcFlds If rgIncExcFlds is CosNewNull, then 
	all fields are exported, and bIncl is ignored. If it is 
	a CosArray, then the array elements must be names of fields. 
	The names may be of non-terminal fields, which is a fast 
	and easy way to cause all their children to be exported. 
	
	@param bIncl If true, rgIncExcFlds is an array of the 
	fields to export. Otherwise, rgIncExcFlds is an array of 
	the fields to exclude from exporting. 
	@param bEmpty If true, all fields selected per the above 
	criteria are exported. If false, exclude fields that have 
	no value. 
	@param Hfile File to which the HTML data is written. 
	@param submitBtnName A NULL-terminated string containing 
	the name of the button used to submit. If the value passed 
	is not NULL, then include "...&submitBtnName=&..." as part 
	of the generated x-www-form-urlencoded output. NOTE: This 
	type of output is the same one you get when an AcroForm 
	has an empty text field (that is, no Value), and parameter 
	bEmpty is true.
	@see ExportAsHtml 
	@see ImportAnFDF
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ExportAsHtmlEx_SELPROTO)
		(PDDoc pdForm, CosObj rgIncExcFlds, ASBool bIncl, ASBool bEmpty,
		 ASFile Hfile, const char* submitBtnName);

/**
	Constructs an Acrobat form from templates and imports an 
	FDF file. 
	@param pdCurrForm The current form being viewed, if any, 
	at the time cdFDF is being imported. This parameter can 
	be NULL; if not NULL, then cdFDF can refer to templates 
	in the current form, by omitting the F key in the TRef dictionary. 
	Even if the F key is not NULL, it can be a relative path 
	(as opposed to an absolute path), as long as pdCurrForm 
	is not NULL. 
	@param cdFDF The FDF file being imported. 
	@param bAddToCurr If true (and pdCurrForm is not NULL), 
	then instead of creating a new form, the templates spawn 
	pages that are appended at the end of pdCurrForm (and the 
	function returns pdCurrForm).
	@return The PDDoc for the newly-created form (or pdCurrForm, if 
	bAddToCurr is true). 
	@see ExportAsFDF 
	@see ExportAsFDFEx 
	@see ExportAsFDFWithParams 
	@see ImportAnFDF 
*/
typedef ACCBPROTO1 PDDoc (ACCBPROTO2 *AssembleFormAndImportFDF_SELPROTO)
		(PDDoc pdCurrForm, CosDoc cdFDF, ASBool bAddToCurr);

/**
	Exports form data to a CosDoc, which can be written to an 
	FDF file. See the PDF Reference for a description of this 
	format. 
	@param params An ExportAsFDFParamsRec structure.
	@return The FDF CosDoc containing the exported data. 
	NOTE: Call CosDocSaveToFile to create an FDF file from this 
	CosDoc. 
	@exception Raises gAFpdErrExportFdf if cannot export field data. The 
	viewer may raise other exceptions. 
	@see ExportAsFDF 
	@see ExportAsFDFEx 
	@see ImportAnFDF 
	@see AssembleFormAndImportFDF 
*/
typedef ACCBPROTO1 CosDoc (ACCBPROTO2 *ExportAsFDFWithParams_SELPROTO)
		(ExportAsFDFParams params);

/**
	Creates an XObject form from a PDF page. A form XObject 
	is a content stream that can be treated as a single graphics 
	object. Use this method for importing PDF graphics into 
	documents. 
	@param cd The CosDoc that the XObject will be created 
	in. 
	@param pdp The PDPage that you want to create the XObject 
	from. 
	@return A Cos object pointing to the XObject on the PDF page. 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *AFPDFormFromPage_SELPROTO)(CosDoc cd, PDPage pdp);

/* AFLayoutNew has been revved */
/**
	Create a new layout context for annotations. Use PDAnnotGetRect 
	to get the annotation's bounding box, then use this method 
	to define new layout context. 
	@param frBbox The bounding box of the area for text and 
	border data to flow into. 
	@param annotRotation The rotation of the annotation. 
	@param cd The CosDoc.
	@return A new layout. 
	@see AFLayoutBorder 
	@see AFLayoutText 
	@see AFLayoutCreateStream 
*/
typedef ACCBPROTO1 void* (ACCBPROTO2 *AFLayoutNew_SELPROTO)(ASFixedRectP frBbox, PDRotate annotRotation, CosDoc cd);

/**
	Frees the layout context. 
	@param vlayout The layout of the annotation to remove.
	@see AFLayoutNew 
	@see AFLayoutCreateStream 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFLayoutDelete_SELPROTO)(void* vlayout);

/* AFLayoutCreateStream has been revved */
/**
	Creates a layout stream that can be used as an annotation 
	appearance. 
	@param vlayout The layout of the annotation. Use AFLayoutNew 
	to create a new layout before calling this method. 
	@return A stream CosObj. 
	@see AFLayoutNew 
	@see AFLayoutDelete 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *AFLayoutCreateStream_SELPROTO)(void* vlayout);

/**
	Draws a border into the layout context. 
	@param vlayout The layout of the annotation. Use AFLayoutNew 
	to create a new layout before calling this method. 
	@param border A pointer to a structure containing information 
	about the appearance a border. 
	@param pdcvBrdr A PDColorValue structure representing 
	the color of the annotations border. 
	@param pdcvBg A PDColorValue structure representing the 
	color of the annotations background. 
	@param bDown A boolean specifying whether the background 
	should be drawn as it is drawn in forms while being "pressed' 
	(clicked by the mouse) or not. If true, draws as if it is 
	a field that is being pressed.
	@see AFLayoutNew 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFLayoutBorder_SELPROTO)(void* vlayout, AFPDWidgetBorder border, 
															   PDColorValue pdcvBrdr, PDColorValue pdcvBg,
															   ASBool bDown);

/**
	Sets the text layout for the annotation. 

	NOTE: Before calling this method, you should call AFLayoutNew 
	to create a new layout, as well as AFLayoutBorder. 
	@param vlayout The layout of the annotation. 
	@param bMultline If true, the text can use multiple lines 
	in a text field. Otherwise, text is single line. 
	@param bWrap If true, the text will wrap. 
	@param border The border appearance that defines the width 
	and line style of a border. The border of the annotation 
	should be the same as in your call to AFLayoutBorder. 
	@param ta A pointer to structure containing font, point 
	size, and color information. You should initialize the structure 
	with the SetDefaultTextAppearanceP macro which defaults 
	to Helvetica. 
	@param cBytes The text string for the layout.
	@exception Raises an exception if the field is a radio box or button. 
	
	@see AFLayoutNew 
	@see AFLayoutBorder 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFLayoutText_SELPROTO)(void* vlayout, ASBool bMultline, ASBool bWrap,
															 AFPDWidgetBorder border, TextAppearanceP ta, char* cBytes);

/** 
	Sets the text layout for the annotation. 

	NOTE: Before calling this method, you should call AFLayoutNew 
	to create a new layout, as well as AFLayoutBorder. 
	@param vlayout The layout of the annotation. 
	@param bMultline If true, the text can use multiple lines 
	in a text field. Otherwise, text is single line. 
	@param bWrap If true, the text will wrap. 
	@param border The border appearance that defines the width 
	and line style of a border. The border of the annotation 
	should be the same as in your call to AFLayoutBorder. 
	@param ta A pointer to structure containing font, point 
	size, and color information. You should initialize the structure 
	with the SetDefaultTextAppearanceP macro which defaults 
	to Helvetica. 
	@param cBytes The text string for the layout.
	@param fxMinFontSize The minimum font size, when using autosizing.
	@param fxMaxFontSize The maximum font size, when using autosizing..
	@exception Raises an exception if the field is a radio box or button. 
	
	@see AFLayoutNew 
	@see AFLayoutBorder 

*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFLayoutTextEx_SELPROTO)(void* vlayout, ASBool bMultline, ASBool bWrap,
															 AFPDWidgetBorder border, TextAppearanceP ta, char* cBytes,
															 ASFixed fxMinFontSize, ASFixed fxMaxFontSize);

/**
	Lays out the text and creates a Cos XObject with the content stream
	for the text and required Resources. This API supports Unicode text. 
	If required different 	fonts may be used for different characters in 
	the text. It has support for western scripts, Arabic, Hebrew, Thai, Vietnamese, 
	Chinese, Japanese, and Korean. Other scripts may be added in the future.

	@return A stream CosObj. If the operation fails, then CosNewNull
	is returned.

	@param cd The CosDoc.
	@param frBbox The bounding box of the area for text and 3
	border data to flow into. 
	@param annotRotation The rotation of the annotation. 
	@param bWrap If true, the text will wrap. 
	@param border The border appearance that defines the width 
	@param fxMinFontSize The minimum font size, when using autosizing.
	@param fxMaxFontSize The maximum font size, when using autosizing..
	and line style of a border.  
	@param textAttrs A pointer to structure containing font, point 
	size,color information, alignment, and writing direction. 

	@param asText The text string for the layout.
	
	
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *AFDrawText_SELPROTO)(CosDoc cd,  ASFixedRectP frBbox, PDRotate annotRotation, 
														  ASBool bWrap, AFPDWidgetBorder border, 
														  AFTextAttributesP textAttrs, ASConstText asText, 
														  ASFixed fxMinFontSize, ASFixed fxMaxFontSize);

/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AFLayoutIconText_SELPROTO)(void* vLayout, AFPDWidgetPosition nPosition,
	CosObj coIcon, AFPDWidgetBorder border, TextAppearanceP ta, char *cBytes);

/** */
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldValueChanged_SELPROTO)(PDDoc pdd, PDField fldP);

/**
	Gets the rotation of the annotation. 
	@param pdan The annotation widget.
	@return Returns a ASAtom object representing the annotation's rotation 
	with respect to the page. 
*/
typedef ACCBPROTO1 PDRotate (ACCBPROTO2 *AFPDWidgetGetRotation_SELPROTO)(PDAnnot pdan);

/**  
    This routine is deprecated, please use AFPDFieldGetDefaultTextAppearanceEx.

	Gets the default text appearance of a field. Use this method 
	to get the font, size, color, and so forth; values that were 
	set through the field properties dialog box. 
	@param fldP The PDField object for which to retrieve the 
	text appearance. 
	@param aP A pointer to a structure describing the text 
	appearance. Returns font, size, quadding, text color, and 
	so forth upto and including the nameFont field in the text appearance structure. 
	The remaining fields will be uninitialized.
	@see AFPDWidgetSetAreaColors
	@see AFPDFieldGetDefaultTextAppearanceEx
*/

typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldGetDefaultTextAppearance_SELPROTO)(PDField fldP, TextAppearanceP aP);

/**
    This routine is deprecated, please use AFPDFieldSetDefaultTextAppearanceEx.

	Sets the default text appearance of a field. Use this method 
	to set the font, size, color, and so forth. 
	@param fldP The PDField object for which to set the text 
	appearance. 
	@param aP A pointer to a structure describing the text 
	appearance to set. This routine will use the values upto 
	and including the nameFont field in the text appearance structure. 
	@see AFPDWidgetSetAreaColors
	@see AFPDFieldSetDefaultTextAppearanceEx
*/

typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldSetDefaultTextAppearance_SELPROTO)(PDField fldP, TextAppearanceP aP);

/**
	Gets the default text appearance of a field. Use this method 
	to get the font, size, color, and other values that were 
	set through the field properties dialog box. 
	@param fldP The PDField object for which to retrieve the 
	text appearance. 
	@param aP A pointer to a structure describing the text 
	appearance. Returns font, size, alignment, text color, and 
	so forth. 
	@param size The size in bytes of the structure referenced by aP. Fields in the 
	structure up to this size will be initialized. 
	@see AFPDWidgetSetAreaColors 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldGetDefaultTextAppearanceEx_SELPROTO)(PDField fldP, TextAppearanceP aP, size_t size);

/**
	Sets the default text appearance of a field. Use this method 
	to set the font, size, color, and so forth. 
	@param fldP The PDField object for which to set the text 
	appearance. 
	@param aP A pointer to a structure describing the text 
	appearance to set.
	@param size The size in bytes of the structure referenced by aP. Fields in the 
	structure up to this size will be used. If there are additional fields after this
	size, default values will be used. 
	@see AFPDWidgetSetAreaColors 
*/

typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDFieldSetDefaultTextAppearanceEx_SELPROTO)(PDField fldP, TextAppearanceP aP, size_t size);
/**
	Gets the border of an annotation. 
	@param pdan The annotation. 
	@param pdwb (Filled by the method) A pointer to a structure 
	describing the form field appearance definitions for the 
	outside border of an annotation. 
	@return true if successful getting border, false otherwise. 
	@see AFPDWidgetSetAreaColors 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFPDWidgetGetBorder_SELPROTO)(PDAnnot pdan, AFPDWidgetBorder pdwb);

/**
	Sets the border of an annotation. 
	@param pdan The annotation whose change border appearance 
	will be set. 
	@param pdwb A pointer to a structure describing the form 
	field appearance definitions for the outside border of an 
	annotation. Possible border types are solid, dashed, beveled, 
	inset, underline.
	@see AFPDWidgetSetAreaColors 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDWidgetSetBorder_SELPROTO)(PDAnnot pdan, AFPDWidgetBorder pdwb);

/**
	Gets the border and background colors of an annotation. 
	
	@param pdan The annotation. 
	@param borderP (Filled by the method) A pointer to a structure 
	representing the border color of the annotation. 
	@param bkgndP (Filled by the method) A pointer to a structure 
	representing the background color of the annotation. 
	@see AFPDWidgetSetAreaColors 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDWidgetGetAreaColors_SELPROTO)(PDAnnot pdan, PDColorValue borderP, PDColorValue bkgndP);

/**
	Sets the border and background color of the annotation. 
	
	@param pdan The annotation. 
	@param borderP A pointer to a structure representing the 
	border color of the annotation. 
	@param bkgndP A pointer to a structure representing the 
	background color of the annotation. 
	@see AFPDWidgetGetAreaColors 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AFPDWidgetSetAreaColors_SELPROTO)(PDAnnot pdan, PDColorValue borderP, PDColorValue bkgndP);

/**
	Opens the dialog box that allows the user to select a PDF 
	to use as the icon for a button. 
	@param cd The CosDoc that contains the appearance you 
	are trying to import. 
	@param coIcon If AFImportAppearance is successful, coIcon 
	is a pointer to a CosObj that will contain the Cos representation 
	of the appearance. 
	@param avd The AVDoc that you want the window to be the 
	child of. You can pass NULL if you do not have an AVDoc. 
	
	@param cTitle The window title of the dialog box when 
	it appears. You can pass NULL if you want the title of the 
	dialog to be the same as it is when brought up through the 
	field properties dialog.
	@return true if appearance was imported properly, false otherwise. 
	
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFImportAppearance_SELPROTO)(CosDoc cd, CosObj *coIcon, AVDoc avd, char *cTitle);

/* Obsolete. */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFGetScriptingContextEx_SELPROTO)
		(ScriptingData data);

/**AcroForm Methods 
	Executes a JavaScript script. 
	@param pdd The PDDoc in which the script is to be executed. 
	
	@param cScript A string containing the text of the script 
	to be executed. If Unicode, the string must begin with 0xFEFF 
	and end with 2 null bytes. If this is not the case, it is 
	assumed to be in the application's language encoding, as 
	returned by AVAppGetLanguageEncoding. 
	@param pRetValue To get a return value from the execution 
	of the script, pass a non-NULL value for this parameter. 
	If on return *pRetVal is non-NULL, the caller should dispose 
	of the string by calling ASFree. If present, the value will 
	be in host encoding. NOTE: The script sets this value by 
	assigning it to event.value. See the Acrobat JavaScript 
	Object Specification for more information.
	@return The JavaScript value of event.rc. This function pre-initializes 
	it to true; a script may set it to false if desired. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFExecuteThisScript_SELPROTO)
		(PDDoc pdd, const char* cScript, char** pRetValue);

/** */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFCalculateFields_SELPROTO)
		(PDDoc pdd, ASCab asc);

#endif /* _H_AF_SEL  } */
