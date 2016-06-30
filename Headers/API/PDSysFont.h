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

 PDSysFont.h

 - PDSysFont header file.

*********************************************************************/

#ifdef NPROC /* may be already defined */
#undef NPROC
#endif

#define NPROC(returnType, name, params)		\
		ACCB1 returnType ACCB2 name params;


/**
	Enumerates all of the system fonts with a user-supplied 
	procedure. 

	The PDSysFont must be acquired during the enumeration if 
	the font is needed beyond the enumProc. 

	Developers should not assume that the enumProc will get 
	called. If no system fonts are found (for example, if the 
	PSRESOURCEPATH environment variable is not set on UNIX platforms), 
	enumProc is never called, and PDEnumSysFonts does not raise 
	an exception. 

	NOTE: The font names that are returned from the methods 
	PDEnumSysFonts and PDSysFontGetAttrs are different in 5.0 
	(compared to 4.05). The differences are shown in the table 
	below. 

	Acrobat 4.05 Acrobat 5.0 

	Name PSname Name Psname 

	MS-Mincho NULL MSMincho MS-Mincho 

	MS-Gothic NULL MSGothic MS-Gothic 

	MS-PMincho NULL MSPMincho MS-PMincho 

	MS-PGothic NULL MSPGothic MS-PGothic 

	MS-UIGothic NULL MSUIGothic MS-UIGothic 
	@param enumProc IN/OUT? User-supplied callback to call once for 
	each system font. Enumeration continues until all fonts 
	have been enumerated, or until enumProc returns false. 
	@param clientData IN/OUT? Pointer to user-supplied data to pass 
	to enumProc each time it is called. 
	@see PDFindSysFont 
	@see PDFindSysFontForPDEFont 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDEnumSysFonts, (
		IN		PDSysFontEnumProc enumProc, 
		IN		void *clientData)
	)


/**
	Finds a system font that matches the requested attributes. 
	

	The method gets the PDSysFont rather than acquires it, so 
	do not call PDERelease on the returned PDSysFont when done 
	with it. 
	@param attrs IN/OUT? Pointer to a PDEFontAttrs structure with the 
	attributes of the font you are searching for. 
	@param attrsSize IN/OUT? Size of the attrs buffer, in bytes. 
	@param flags IN/OUT? Flags from PDSysFontMatchFlags. 
	@return The desired system font. 
	@see PDEnumSysFonts 
	@see PDFindSysFontForPDEFont 
	@see PDFindSysFontEx 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(PDSysFont, PDFindSysFont, (
		IN		PDEFontAttrsP attrs,
		IN		ASUns32 attrsSize,
		IN		ASUns32 flags)
	)


/**
	Finds a system font that matches the requested attributes. 
	

	If the requested font is a multiple master font instance, 
	the base font is returned, and the specified design vector 
	is decoded and returned in mmDesignVector. 

	The method gets the PDSysFont rather than acquires it, so 
	do not call PDERelease on the returned PDSysFont when done 
	with it. 
	@param attrs IN/OUT? Pointer to a PDEFontAttrs structure with the 
	attributes of the font you are searching for. 
	@param attrsSize IN/OUT? Size of the attrs buffer, in bytes. 
	@param flags IN/OUT? Flags from PDSysFontMatchFlags. 
	@param mmDesignVector IN/OUT? (Filled by the method) If the requested 
	font is a multiple master font instance, the specified design 
	vector is decoded and returned in mmDesignVector. 
	@param designVecLength IN/OUT? (Filled by the method) Pass the 
	length of mmDesignVector. This parameter also returns the 
	number of elements filled in mmDesignVector (maximum = 4). 
	
	@return The desired system font. 
	@see PDEnumSysFonts 
	@see PDFindSysFont 
	@see PDFindSysFontForPDEFont 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(PDSysFont, PDFindSysFontEx, (
		IN		PDEFontAttrsP attrs,
		IN		ASUns32 attrsSize,
		IN		ASUns32 flags,
		OUT		ASFixed *mmDesignVector,
		OUT		ASInt32 *designVecLength)
	)


/**
	Find a system font that matches the requested PDEFont. 

	The method gets the PDSysFont rather than acquires it, so 
	do not call PDERelease on the returned PDSysFont when done 
	with it. 
	@param font IN/OUT? A PDEFont whose matching system font is found. 
	
	@param flags IN/OUT? Bit field comprised of PDSysFontMatchFlags 
	values.
	<ul>
	<li> kPDSysFontMatchNameAndCharSet
	<li> kPDSysFontMatchFontType 
	<li> PDSysFontMatchFlags Passing zero matches font by name 
	only. 
	</ul>	
	@return The system font corresponding to font. 
	@exception peErrCantGetAttrs 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDFindSysFont 
	@since PI_PDSYSFONT_VERSION >= 0x00050000
*/
NPROC(PDSysFont, PDFindSysFontForPDEFont, (
		IN		PDEFont font, 
		IN		ASUns32 flags)
	)


/**
	Gets the attributes of a system font. 

	The attributes will be returned in the buffer pointed to 
	by attrsP. 

	No more than attrsSize bytes will be written to the buffer. 
	

	This call can be expensive to execute, as it may involve 
	parsing the font in order to determine attributes. 

	NOTE: The font names that are returned from the methods 
	PDEnumSysFonts and PDSysFontGetAttrs are different in 5.0 
	(compared to 4.05). The differences are shown in the table 
	below. 

	Acrobat 4.05 Acrobat 5.0 

	Name PSname Name Psname 

	MS-Mincho NULL MSMincho MS-Mincho 

	MS-Gothic NULL MSGothic MS-Gothic 

	MS-PMincho NULL MSPMincho MS-PMincho 

	MS-PGothic NULL MSPGothic MS-PGothic 

	MS-UIGothic NULL MSUIGothic MS-UIGothic 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose attributes are obtained. 
	@param attrsP IN/OUT? (Filled by the method) Pointer to a PDEFontAttrs 
	structure with the attributes of a system font. 
	@param attrsSize IN/OUT? Size of the attrsP buffer, in bytes. 
	@exception peErrCantGetAttrs 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetInfo 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetAttrs, (
		IN		PDSysFont sysFont, 
		OUT		PDEFontAttrsP attrsP, 
		IN		ASUns32 attrsSize)
	)


/**
	Gets the widths of a single byte encoded system font. 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose widths are obtained. 
	@param widthsP IN/OUT? (Filled by the method) Pointer to widths 
	array. widthsP must have room for 256 entries. 
	@exception peErrCantGetWidths 
	@see PDSysFontGetType0Widths 
	@see PDSysFontGetWidthsEx 
	@see PDFontGetWidths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetWidths, (
		IN		PDSysFont sysFont, 
		OUT		ASInt16 *widthsP)
	)


/**
	Gets the widths of a single byte encoded system font. 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose widths are obtained. 
	@param widthsP IN/OUT? (Filled by the method) Pointer to widths 
	array. widthsP must have room for 256 entries. 
	@param mmDesignVector IN/OUT? If sysFont is a multiple master font, 
	points to the design vector, whose length must equal the 
	number of design axes of sysFont. 
	@exception peErrCantGetWidths 
	@see PDSysFontGetType0Widths 
	@see PDSysFontGetWidths 
	@see PDFontGetWidths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetWidthsEx, (
		IN		PDSysFont sysFont, 
		OUT		ASInt16 *widthsP,
		IN		ASFixed *mmDesignVector)
	)


/**
	Gets the encoding of a single byte encoded system font. 
	

	The returned encoding must be freed via a call to ASfree. 
	
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose encoding is obtained. 
	@param encodingNameP IN/OUT? (Filled by the method) An encoding 
	name if the return value of PDSysFontGetEncoding is zero. 
	If encodingNameP is the NULL ASAtom, the font uses its default 
	encoding. 
	@return An encoding array of 256 C strings. Each entry in the array 
	either contains a glyph name or NULL. If it is NULL, the 
	corresponding entry uses the font's built in encoding value. 
	

	If the return value is zero, encodingNameP contains the 
	name of the encoding. 
	<ul>
	<li> For a Type 1 font, the default encoding is that specified 
	by the Encoding value in the font dictionary. 
	<li> For a TrueType font, the default encoding is that specified 
	in the single byte CMAP table. 
	</ul>
	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetInfo 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(Uns8 **, PDSysFontGetEncoding, (
		IN		PDSysFont sysFont, 
		OUT		ASAtom *encodingNameP)
	)


/**
	Gets high-level information about a system font. 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose information is obtained. 
	@param infoP IN/OUT? (Filled by the method) Pointer to PDEFontInfoRec 
	structure to fill with font information for sysFont. No 
	more than infoSize bytes are written to this buffer. 
	@param infoSize IN/OUT? Size of the infoP buffer, in bytes. 
	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetInfo, (
		IN		PDSysFont sysFont, 
		OUT		PDEFontInfoP infoP, 
		IN		ASUns32 infoSize)
	)


/**
	Gets the PostScript or TrueType styled name for a system 
	font. 
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font whose name is obtained. 
	@return The ASAtom for the system font's name. 
	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetInfo 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00050000
*/
NPROC(ASAtom, PDSysFontGetName, (
		IN		PDSysFont sysFont)
	)


/**
	Acquires platform-specific data for use by user interface 
	code. Must be released when finished by PDSysFontReleasePlatformData. 
	
	@param sysFont IN/OUT? A PDSysFont object referencing a system 
	font returned by either PDFindSysFont or PDFindSysFontForPDEFont. 
	
	@return Pointer to a platform-dependent structure PDSysFontPlatData 
	containing information relating to a system font. Returns 
	NULL if out of memory. 
	@see PDSysFontReleasePlatformData 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(PDSysFontPlatDataP, PDSysFontAcquirePlatformData, (
        IN      PDSysFont sysFont)
	)


/**
	Releases platform-specific data for the specified PDSysFont. 
	
	@param platDataP IN/OUT? A pointer to a PDSysFontPlatDataP structure 
	containing platform-specific data. 
	@see PDSysFontAcquirePlatformData 
	Creates a new attribute object with the specified owner. 
	
	@param pdDoc IN/OUT? Document in which the attribute object is 
	created. 
	@param owner IN/OUT? Owner of the new attribute object. 
	@param indirect IN/OUT? If true, creates the attribute object as 
	an indirect Cos object and sets pdDoc's PDDocNeedsSave flag 
	(see PDDocFlags). If false, creates the attribute object 
	as a direct object. 
	@param attrObj IN/OUT? (Filled by the method) The newly-created 
	attribute object. 
	@exception Various 
	@see PDSAttrObjCreateFromStream 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontReleasePlatformData, (
        IN  PDSysFontPlatDataP platDataP)
	)

#if !POQUTIO
/**
	Returns a PDScript value for the specified PDSysFont
	@param sysFont font to acquire script from
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
NPROC(PDScript, PDSysFontGetScript, (
        IN  PDSysFont sysFont)
	)
#endif


/**
	If there is a font on the system that matches this PDEFont, 
	embed the full font, regardless of whether it was subsetted 
	or not embedded at all in the first place. This will not 
	work for CID fonts, because they must be subsetted. 

	The matching is based on the PDSysFontMatchFlags. 

	Only the font object itself is modified - no content streams 
	are changed. 

	NOTE: This method does not change the reference count of 
	the font. 
	@param font IN/OUT? A PDEFont object returned from one of the PDEFontCreate 
	methods. 
	@param flags IN/OUT? Flags from PDSysFontMatchFlags that determine 
	matches. 
	@param cosDoc IN/OUT? Currently unused. 
	@exception Raises peErrFontToEmbedNotOnSys if there is no system font 
	that matches this 
	@exception PDEFont. 
	@exception Raises genErrBadParm if the PDEFont is a CID font. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@see PDEFontCreateFromSysFont 
	@see PDFindSysFontForPDEFont 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDEmbedSysFontForPDEFont, (
		IN	PDEFont	font,
		IN	ASUns32	flags,
		IN	CosDoc	cosDoc)
)


/**
	Derives the registry, ordering, and supplement information 
	of a multi-byte system font. This information can be used 
	to create a PDEFont from a system font. For more information 
	on CID fonts, see PDFontGetCIDSystemInfo. 
	@param sysFont IN/OUT? A PDSysFont object referencing a multibyte 
	system font. 
	@param registry IN/OUT? (Filled by the method) The ASAtom representing 
	the CIDFont's Registry information, as in ÒAdobeÓ. 
	@param ordering IN/OUT? (Filled by the method) The ASAtom representing 
	the CIDFont's Ordering information, for example, ÒJapan1Ó. 
	
	@param supplement IN/OUT? (Filled by the method) The SystemSupplement 
	field from the CIDFont. 
	@see PDFontGetCIDSystemInfo 
	@see PDFontGetCIDSystemSupplement 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetCIDSystemInfo, (
  		IN		PDSysFont sysFont, 
		OUT		ASAtom* registry, 
		OUT		ASAtom* ordering, 
		OUT		ASInt32* supplement)
	)


/**
	Gets width information from a Type 0 system font. This information 
	can be used to create a PDEFont from a system font. 

	NOTE: In general, you are discouraged from using this method. 
	Instead use PDEFontCreateFromSysFontAndEncoding followed 
	by PDEFontCreateWidthsNow to create the W entry in a font. 
	
	@param sysFont IN/OUT? A PDSysFont object referencing a multibyte 
	system font. 
	@param ordering IN/OUT? ASAtom representing the CIDFont's Ordering 
	information. Used to get a CMap object for sysFont. 
	@param hasdw IN/OUT? (Filled by the method) true if sysFont has 
	a valid dw value; false otherwise. 
	@param dw IN/OUT? (Filled by the method) Default width for glyphs 
	in a CIDFont. Currently, always 1000. See Section 5.6 on 
	CIDFontType 0 in the PDF Reference for more information. 
	
	@param w IN/OUT? (Filled by the method) A Cos array of a set of 
	lists that define the widths for the glyphs in the CIDFont. 
	Each list can specify individual widths for consecutive 
	CIDs, or one width for a range of CIDs. See Section 5.6.3 
	on character widths in CIDFonts in the PDF Reference for 
	information on the format of this array. 
	@param hasdw2 IN/OUT? (Filled by the method) true if sysFont has 
	a valid dw2 value. Default is false. 
	@param dw2 IN/OUT? (Filled by the method) The default metrics for 
	writing mode 1. This entry is an array of two ASInt32 numbers: 
	the y component of the position vector and the y component 
	of the displacement vector for writing mode 1. The x component 
	of the position vector is always half the width of the character. 
	The x component of the displacement vector is always 0. 
	The default value is [ 880 -1000]. For information on writing 
	mode 1, see Section 5.6.3 on vertical writing in the PDF 
	Reference. 
	@param w2 IN/OUT? (Filled by the method) A Cos array defining the 
	metrics for vertical writing. Its format is similar to the 
	format of the array in w. It defines the x and y components 
	of the position vector, and the y component of the displacement 
	vector. The x component of the displacement vector is always 
	0. See Section 5.6.3 on character widths in CIDFonts in 
	the PDF Reference for information on the format of this 
	array. 
	@see PDSysFontGetWidths 
	@see PDSysFontGetWidthsEx 
	@see PDFontGetWidths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
NPROC(void, PDSysFontGetType0Widths, (
  		IN		PDSysFont sysFont,
		IN		ASAtom ordering,
		OUT		ASBool* hasDW,
		OUT		ASInt32* dw, 
		OUT		CosObj* w, 
		OUT		ASBool* hasDW2,
		OUT		ASInt32* dw2, 
		OUT		CosObj* w2)
	)



#undef NPROC
