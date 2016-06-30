/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDMetadataProcs.h

*********************************************************************/

/**********************************************************************
 * PDF XAP Metadata API
 *
 * General remarks
 *
 * These procedures let you store and access descriptive metadata
 * associated with a PDF document and with components within a PDF
 * document.  The metadata is expressed in the Adobe XAP format, which is
 * an application of RDF (Resource Description Format) as defined by the
 * World Wide Web Consortium (W3C).
 *
 * Constructing XAP metadata and accessing information within XAP
 * metadata are the responsibility of the client.  The only exception
 * is in the case of the descriptive entries that are store in the Info
 * dictionary in a PDF document. These entries have corresponding entries in the
 * XAP metadata for a document, and the internals of Acrobat maintain compatibility
 * between the two representations by copying and translating information
 * as necessary.  The implementation of the existing API calls
 * PDDocSetMetadata and PDDocGetMetadata has been updated to maintain
 * both copies of the information as well.
 * 
 * The PDF XAP Metadata API consists of three pairs of setting and getting
 * procedures, plus one notification for initiating recalculation of
 * metadata items.
 *
 * PDDocGetXAPMetadata and PDDocSetMetadata deal with metadata that
 * describes the PDF document as a whole.
 *
 * CosDictGetXAPMetadata and CosDictSetXAPMetadata deal with metadata
 * that describes a particular object within a PDF document.
 * The PDF 1.4 update to the PDF Language Reference describes which
 * kinds of objects may have XAP metadata, but the API makes no attempt
 * to enforce restrictions.  Many objects that are actually represented
 * as Cos dictionaries or streams (such as pages, for example), have
 * special PD layer types that represent them more abstractly (the PDPage type
 * in the case of pages).  To use the PDF XAP metadata API on such an
 * object, you deal with its representation as a Cos dictionary.
 * In the case of a PDPage, you call PDPageGetCosObj to get this representation.
 *
 * PDEContainerGetXAPMetadata and PDEContainerSetXAPMetadata deal with
 * metadata that describes a particular portion of graphic stream content.
 * Inline images are an important kind of page content for which you might
 * want to maintain a metadata description (Image XObjects are Cos dictionaries,
 * and can have metadata attached via the CosDict procedures mentioned above).
 * You access metadata on graphic stream content by means of the PDFEdit
 * object type PDEContainer, which can contain other kinds of PDFEdit object.
 * If you want to attach XAP metadata to a PDFEdit object that isn't directly
 * contained in a PDEContainer, you use PDFEdit to create a PDEContainer
 * and place the object within.
 *
 * All the XAP metadata setting procedures take ASText objects as
 * arguments, and all the XAP metadata getting procedures return ASText
 * objects as results.  Since XAP metadata can potentially contain
 * arbitrary Unicode character codes, you should in general extract
 * the metadata from the ASText object as Unicode or UTF-8: other
 * encodings may not be able to represent all the characters that can
 * exist in XAP text.
 *
 * The remaining piece of the PDF XAP metadata API deals with the fact
 * that some applications require the calculation of metadata based on
 * the actual contents of the PDF document.  Clients that maintain
 * metadata items that have to be recalculated should register for the
 * notification PDDocCalculateMetadata with a callback procedure of
 * type PDImplicitMetadataProc (declared in PDMetadataExpT.h).
 * The procedure should obtain the metadata with which it's concerned,
 * change it, and put the changed metadata back on the object from
 * which it was obtained.
 *
 * The notification PDDocCalculateMetadat is issued when a document is saved;
 * it can also be issued explicitly via the PDF XAP metadata API procedure
 * PDDocCalculateImplicitMetadata.  A client that requires that all
 * metadata be up-to-date at a time when the document isn't being
 * saved should call this procedure.
 */
/**********************************************************************/
#if !PLUGIN 
#undef XNPROC
#if !READER
#define XNPROC NPROC
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#endif
#endif
 /* Document-level metadata calls */


/**
	Gets the XMP metadata associated with a document. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). For more 
	information on this protocol, see the Adobe XMP specification. 
	

	Returns an ASText whose text is the XML text of the XMP 
	metadata associated with the document pdDoc. The ASText 
	becomes the property of the client, who is free to alter 
	or destroy it. 

	The XMP metadata returned always represents all the properties 
	in pdDoc's Info dictionary, and can also contain properties 
	not present in the Info dictionary. This call is preferred 
	to PDDocGetInfo, which only returns properties that are 
	in the Info dictionary (although the older function is supported 
	for compatibility). 
	@param pdDoc The document containing the metadata.
	@return An ASText object containing the XMP metadata associated 
	with the document pdDoc. 
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@see PDDocGetXAPMetadataProperty 
	@see PDDocSetXAPMetadata 
	@see PDDocSetXAPMetadataProperty 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(ASText,
      PDDocGetXAPMetadata, (IN  PDDoc pdDoc))


/**
	Sets the XMP metadata associated with a document. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). For more 
	information on this protocol, see the Adobe XMP specification. 
	

	Replaces the XMP metadata associated with the document pdDoc 
	with the XMP metadata stored in metadataASText. 

	The contents of metadataASText must be well-formed XML and 
	Resource Description Format (RDF) as defined by the W3C 
	(see http://www.w3.org/RDF) that also forms valid XMP. If 
	metadataASText is ill-formed, an error is raised. 

	The call does not destroy metadataASText or alter its text. 
	This method copies the textual information it needs, so 
	subsequent alteration or destruction of metadataASText does 
	not affect the document XMP metadata. 

	Calling PDDocSetXAPMetadata changes the contents of pdDoc's 
	Info dictionary to reflect the values of corresponding metadata 
	properties represented in metadataASText. The XMP metadata 
	can also contain properties that are not reflected in the 
	Info dictionary. 

	NOTE: This method raises an exception if the user does not 
	have permission to change the document. 

	NOTE: If you use this method to set metadata that does not 
	respect the requirement that aliased metadata items (such 
	as pdf:Title and xap:Title) be equal, then the mechanism 
	that maintains this equality when you set metadata via PDDocSetInfo 
	is disabled. 
	@param pdDoc The document whose metadata is to be set. 
	
	@param metadataASText An ASText object containing the 
	metadata to be stored in the document.
	@notify PDDocXAPMetadataDidChange
	@exception ErrSysPDModel 
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@exception pdErrOpNotPermitted if pdDoc is not writable 
	@see PDDocGetXAPMetadata 
	@see PDDocSetXAPMetadataProperty 
	@see PDDocSetInfo 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(void,
      PDDocSetXAPMetadata, (IN PDDoc  pdDoc,
                            IN ASText metadataASText))

/**********************************************************************/
/* Component-level metadata calls */


/**
	Gets the XMP metadata associated with a Cos dictionary or 
	stream. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). 

	If there is XMP metadata, it is returned as an ASText in 
	the output parameter metadataASText. The ASText returned 
	becomes the property of the client, who is free to alter 
	or destroy it. 

	NOTE: CosDictGetXAPMetadata will not attempt to verify that 
	obj is one of the objects that is specified in the PDF Reference 
	to allow XMP metadata. 
	@param obj A dictionary or stream CosObj. 
	@param metadataASText (Filled by the method) The ASText 
	object from which the XMP metadata will be obtained.
	@return True if obj has associated XMP metadata, false if it does 
	not. Also returns false if obj is not a dictionary or stream. 
	Returns true exactly when the Cos object obj has XMP metadata. 
	
	@exception ErrSysPDModel 
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@see CosDictSetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(ASBool,
      CosDictGetXAPMetadata, (IN  CosObj    obj,
                              OUT ASText * metadataASText))

/**
	Sets the XMP metadata associated with a Cos dictionary or 
	stream. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). 

	Replaces the XMP metadata associated with the Cos object 
	obj with the XMP metadata stored in metadataASText. 

	The contents of metadataASText must be well-formed XML and 
	Resource Description Format (RDF) as defined by the W3C 
	(see http://www.w3.org/RDF) that also forms valid XMP. CosDictSetXAPMetadtata 
	will not destroy metadataASText or alter its text. 

	NOTE: CosDictSetXAPMetadtata will raise an exception if 
	the user does not have permission to change the document. 
	

	NOTE: CosDictSetXAPMetadtata will not attempt to verify 
	that obj is one of the objects that is specified in the 
	PDF Reference to allow XMP metadata. 
	@param obj The dictionary or stream Cos object whose associated 
	XMP metadata is to be set. 
	@param metadataASText The ASText object containing the 
	metadata to be associated with obj.
	@notify CosDictXAPMetadataDidChange
	@exception ErrSysPDModel 
	@see CosDictGetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(void,
      CosDictSetXAPMetadata, (IN CosObj  obj,
                              IN ASText metadataASText))

/**********************************************************************/
/* Implicit metadata calls */


/**
	Notifies all registered implicit metadata calculators to 
	run. 

	Issues the notification PDDocCalculateMetadata, passing 
	pdDoc to all registered handlers. 
	@param pdDoc The document for which implicit metadata 
	is calculated.
	@notify PDDocCalculateMetadata 
	@see PDDocSave 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(void,
      PDDocCalculateImplicitMetadata, (IN PDDoc pdDoc))

/**********************************************************************/
/* Marked Content metadata calls */


/**
	Gets the XMP metadata associated with a PDEContainer. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). 

	If there is XMP metadata, it is returned as an ASText in 
	the output parameter metadataASText. The ASText returned 
	becomes the property of the client, who is free to alter 
	or destroy it. 
	@param pdeContainer The container whose metadata is retrieved. 
	
	@param metadataASText (Filled by the method) If there 
	is XMP metadata, it is returned as an ASText object in this 
	parameter. The ASText object returned becomes the property 
	of the client, who is free to alter or destroy it.
	@return true exactly when the pdeContainer has XMP metadata. 
	@exception ErrSysPDModel 
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@exception peErrWrongPDEObjectType 
	@exception cosErrInvalidObj 
	@see PDEContainerSetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(ASBool,
      PDEContainerGetXAPMetadata, (IN PDEContainer pdeContainer,
                                   OUT ASText * metadataASText))

/**
	Sets the XMP metadata associated with a PDEContainer. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). 

	Replaces the XMP metadata associated with pdeContainer with 
	the XMP metadata stored in metadataASText. The contents 
	of metadataASText must be well-formed XML and Resource Description 
	Format (RDF) as defined by the W3C (see http://www.w3.org/RDF) 
	that also forms valid XMP. PDEContainerSetXAPMetadtata will 
	not destroy metadataASText or alter its text. 
	@param pdeContainer The container whose metadata is set. 
	
	@param pdDoc The document containing pdeContainer. 
	@param metadataASText Well-formed XML and RDF that also 
	forms valid XMP.
	@notify PDEContainerXAPMetadataDidChange
	@exception ErrSysPDModel 
	@exception peErrWrongPDEObjectType 
	@exception cosErrInvalidObj 
	@see PDEContainerGetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00050000
*/
XNPROC(void,
      PDEContainerSetXAPMetadata, (IN PDEContainer pdeContainer,
                                   IN PDDoc pdDoc,
                                   IN ASText metadataASText))

/***********************************************************************
 * New in Acrobat 6
 */

/**
	Gets the value of an XMP metadata property associated with 
	a document. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). For more 
	information on this protocol, see the Adobe XMP specification. 
	

	Returns an ASText whose text is the XML text of the value 
	of the specified property in the XMP metadata associated 
	with the document pdDoc. The ASText becomes the property 
	of the client, who is free to alter or destroy it. 

	The XMP metadata can represent all properties in pdDoc's 
	Info dictionary, as well as other properties. This call 
	is preferred to PDDocGetInfo, which only allows access to 
	properties that are in the Info dictionary (although the 
	latter is supported for compatibility). 
	@param pdDoc The document containing the metadata. 
	@param namespaceName The XML namespace URI for the schema 
	in which the property is to be found. 
	@param path The name of the desired simple property. NOTE: 
	XMP properties can have an XML substructure; this method 
	can only retrieve values from simple textual properties.
	@return An ASText object containing the XML text of the value of 
	the specified property in the XMP metadata associated with 
	the document pdDoc, or an empty ASText if no such property 
	is found. 
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@see PDDocSetXAPMetadataProperty 
	@see PDDocGetXAPMetadata 
	@see PDDocSetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00060000
*/
XNPROC(ASText,
      PDDocGetXAPMetadataProperty, (PDDoc pdDoc,
                                    ASText namespaceName,
                                    ASText path))
/**
	Sets the value of an XMP metadata property associated with 
	a document. 

	NOTE: The term'XAP' refers to an early internal code name 
	for Adobe's Extensible Metadata Platform (XMP). For more 
	information on this protocol, see the Adobe XMP specification. 
	

	The XMP metadata represents all the properties in pdDoc's 
	Info dictionary, and can also contain properties that are 
	not in the Info dictionary. This call is preferred to PDDocSetInfo, 
	which only allows access to properties that are in the Info 
	dictionary (although the older function is supported for 
	compatibility). 
	@param pdDoc The document containing the metadata. 
	@param namespaceName The XML namespace URI for the schema 
	in which the property is to be found. 
	@param namespacePrefix A brief string to be used as an 
	abbreviation when creating the XML representation of the 
	property. This string must not be empty. 
	@param path The name of the simple property to be modified. 
	NOTE: XMP properties can have an XML substructure; this 
	method can only set a value for simple textual properties. 
	
	@param newValue The new XML text value for the property.
	@exception pdMetadataErrCouldntCreateMetaXAP 
	@see PDDocGetXAPMetadataProperty 
	@see PDDocGetXAPMetadata 
	@see PDDocSetXAPMetadata 
	@since PI_PDMETADATA_VERSION >= 0x00060000
*/
XNPROC(void,
      PDDocSetXAPMetadataProperty, (PDDoc pdDoc,
                                    ASText namespaceName,
                                    ASText namespacePrefix,
                                    ASText path,
                                    ASText newValue))

/**
   Yields an ASText containing a semicolon-separated list of fields.
   The first such field is the entire content of the pdf:Keywords
   property of the document XMP; the remaining fields are the contents
   of successive items in the xmp:Keywords bag of keyword items.

   The document's metadata is not modified as a result of this call.

   NOTE: The term'XAP' refers to an early internal code name 
   for Adobe's Extensible Metadata Platform (XMP). For more 
   information on this protocol, see the Adobe XMP specification. 
	
   @param pdDoc The document containing the metadata from which to
   extract the merged list of keywords
   @return An ASText containing the textual representation of the merged
   list of keywords.   The ASText returned becomes the sole property of the caller.
   @since PI_PDMETADATA_VERSION >= 0x00070000
*/

XNPROC(ASText,
	   PDDocGetMergedXAPKeywords, (PDDoc pdDoc))

/**
   Causes a string produced as by PDDocGetMergedXAPKeywords to be
   stored as the new value of the pdf:Keywords property, and the
   former value of the pdf:Keywords property to be stored as an item
   in the xmp:Keywords bag of keyword items.

   The algorithm used to compute merged keywords lists detects the case
   in which the keywords lists have already been merged and makes no
   changes to the XMP metadata in this case.

   NOTE: The term'XAP' refers to an early internal code name 
   for Adobe's Extensible Metadata Platform (XMP). For more 
   information on this protocol, see the Adobe XMP specification. 
	
   @param pdDoc The document containing the metadata that is to be modified
   to make the pdf:Keywords and xmp:Keywords properties conform.
   @since PI_PDMETADATA_VERSION >= 0x00070000
*/

XNPROC(void,
	   PDDocMergeXAPKeywords, (PDDoc pdDoc))

#undef XNPROC
