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

 CosProcs.h

 - Catalog of functions exported by Cos.

*********************************************************************/

/**
	Tests whether two Cos objects are equal. Cos objects are 
	equal when all of the following conditions are true: 
	<ul>
	<li> They are either both direct or both indirect. 
	<li> They have the same type. 
	<li> If they are indirect, they have the same generation number. 
	<li> If they are scalars, they have the same value. (Two null 
	objects are equal.) 
	<li> If they are nonscalar, they reference the same value. 
	</ul>

	The last condition implies that the comparison is "shallow." 
	For example: 

	CosObj a, b, c; a = CosNewString (doc, "XYZ"); b = CosNewString 
	(doc, "XYZ"); c = b; 

	In this case, CosObjEqual(a,b) is false, but CosObjEqual(b,c) 
	is true. 
	@param obj1, obj2 Objects to compare.
	@return true if obj1 and obj2 are equal, false otherwise. 
	@see CosObjCmp 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosObjEqual, (CosObj obj1, CosObj obj2))

/**
	Gets an object's type. 
	@param obj The object whose type is obtained.
	@return The object's type. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosType, CosObjGetType, (CosObj obj))

/**
	Tests whether an object is indirect. 
	@param obj The object to test.
	@return true if obj is indirect, false if obj is direct. 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosObjIsIndirect, (CosObj obj))

/**
	Enumerates the elements of a Cos object by calling a user-supplied 
	procedure for each component of the object. 
	@param obj The object whose elements are enumerated.
	<ul>
	<li> For scalars or strings, the proc is not called, and CosObjEnum
	returns true.
	<li> For dictionaries, proc is called for each key-value pair. The order in 
	which the key-value pairs are enumerated is undefined.
	<li> For arrays, proc is called with each element as the first paramater to proc,
	and the null object as the second parameter. Array elements 
	are enumerated in ascending order of index.
	<li> For streams, proc is called once,
	with the stream's dictionary as the first parameter to the
	proc and the null object as the second parameter.
	</ul>	
	@param proc User-supplied callback to call for each element 
	of obj.
	Neither proc nor any routine called by proc may modify 
	obj. Doing so can produce undefined results or errors. For 
	example, if obj is an array, proc must not call CosArrayRemove; 
	if obj is a dictionary, proc must not call CosDictPut. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return Returns true if every call to proc returned true.
	As soon as any call to proc returns false, the enumeration stops
	and CosObjEnum returns false.
	@see CosArrayGet 
	@see CosDictGet 
	@see CosDocEnumEOFs 
	@see CosDocEnumIndirect 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosObjEnum, (CosObj obj, CosObjEnumProc proc, void *clientData))

/**
	Gets the CosDoc containing the specified object. This is 
	defined only for indirect or non-scalar objects. 
	@param obj The object whose CosDoc is obtained.
	@return The object's CosDoc. 
	@exception Raises cosErrInvalidObj if the object is a direct scalar 
	object. 
	@see PDDocGetCosDoc 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosDoc, CosObjGetDoc, (CosObj obj))

/**
	Returns a direct object of type CosNull. This null object 
	is said to be invalid. You can compare an object to null 
	using either of the following methods (the second is more
	efficient):

	CosObjEqual (obj, CosNewNull());
	CosObjGetType(obj) == CosNull; 
	
	In general, use CosNewNull only to initialize a local variable 
	or pass a parameter. Null objects may be stored as array 
	elements, but not as dictionary values. The following statements 
	are equivalent: 

	CosDictPut (dict, key, CosNewNull()); 
	CosDictRemove (dict, key); 

	@return A null Cos object. 
	@see CosObjGetType 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewNull, (void))

/**
        Creates a new 32-bit integer object associated with the specified 
        document and having the specified value. 
        @param dP IN The document in which the integer is used. 
        @param indirect IN If true, creates the integer object as 
        an indirect object, and sets the document dP's PDDocNeedsSave 
        flag (see PDDocFlags). If false, creates the integer as 
        a direct object. 
        @param value IN The value, represented as a 32-bit integer.
        @return An object of type CosInteger.
        @see CosIntegerValue 
        @see CosNewFixed 
		@see CosNewFloat
        @see CosObjDestroy 
        @since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewInteger, (CosDoc dP, ASBool indirect, ASInt32 value))

/**
	Creates a new real-number object from a fixed-point number, 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If true, creates the real-number object 
	as an indirect object, and sets the document dP's PDDocNeedsSave 
	flag (see PDDocFlags). If false, creates the number 
	as a direct object. 
	@param value The real number, represented as a fixed-point number.
	@return A Cos object of type CosReal (CosFixed).
	@see CosFixedValue 
	@see CosNewInteger
	@see CosNewFloat
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewFixed, (CosDoc dP, ASBool indirect, ASFixed value))

/**
    Creates a new boolean object associated with the specified 
    document and having the specified value. 
    @param dP IN The document in which the boolean is used. 
    @param indirect IN If true, creates the boolean object as 
    an indirect object, and sets the document dP's PDDocNeedsSave 
    flag (see PDDocFlags). If false, creates the boolean as 
    a direct object. 
    @param value IN The value the new boolean will have. 
    @return A Cos boolean object. 
    @see CosBooleanValue 
    @see CosObjDestroy 
    @since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewBoolean, (CosDoc dP, ASBool indirect, ASBool value))


/**
	Creates a new name object associated with the specified 
	document and having the specified value. 
	@param dP The document in which the new name is used. 	
	@param indirect If true, creates the name as an indirect 
	object, and sets the document's PDDocNeedsSave flag (see 
	PDDocFlags) flag. If false, creates the name as a direct 
	object. 
	@param name The ASAtom corresponding to the name to create. 
	A C string can be converted to an ASAtom using ASAtomFromString.
	Note that a name object can be created directly from a C string,
	without creating an ASAtom, by using CosNewNameFromString.
	@return The newly created name Cos object. 
	@see CosNameValue
	@see CosNewNameFromString
	@see CosCopyNameStringValue
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewName, (CosDoc dP, ASBool indirect, ASAtom name))


/**
	Creates and returns a new Cos string object. 
	@param dP The document in which the string is used. 
	@param indirect If true, creates the string as an indirect 
	object, and sets the document dP's PDDocNeedsSave flag (see 
	PDDocFlags). If false, creates the string as a direct object. 
	
	@param str The value that the new string will have. It 
	is not a C string, since Cos strings can contain NULL characters. 
	The data in str is copied, that is, if str was dynamically 
	allocated, it can be freed after this call. 
	@param nBytes The length of str.
	@return The newly created string Cos object. 
	@see CosStringValue 
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewString, (CosDoc dP, ASBool indirect, char *str, ASTArraySize nBytes))


/**
	Creates and returns a new array Cos object. 
	@param dP The document in which the array is used. 
	@param indirect If true, creates the array as an indirect 
	Cos object, and sets the document's PDDocNeedsSave flag 
	(see PDDocSetFlags). If false, creates the array as a direct 
	object. 
	@param nElements The number of elements that will be in 
	the array. nElements is only a hint; Cos arrays grow dynamically 
	as needed.
	@return The newly created array Cos object. 
	@see CosObjDestroy 
	@see CosArrayGet 
	@see CosArrayInsert 
	@see CosArrayLength 
	@see CosArrayPut 
	@see CosArrayRemove 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewArray, (CosDoc dP, ASBool indirect, ASTArraySize nElements))


/**
	Creates a new dictionary. 

	See the PDF Reference for information on dictionary objects 
	that are part of standard PDF, such as annotations or page 
	objects. 
	@param dP The document in which the dictionary is used. 
	
	@param indirect If true, creates the dictionary as an 
	indirect Cos object, and sets dP's PDDocNeedsSave flag (see 
	PDDocFlags). If false, creates the dictionary as a direct 
	object. 
	@param nEntries Number of entries in the dictionary. This 
	value is only a hint - Cos dictionaries grow dynamically as 
	needed.
	@return The newly created dictionary Cos object. 
	@see CosDictGet 
	@see CosDictKnown 
	@see CosDictPut 
	@see CosDictRemove 
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewDict, (CosDoc dP, ASBool indirect, ASTArraySize nEntries))


/**
	Creates a new Cos stream, using data from an existing ASStm. 
	The data is copied, so the source stream may be closed after 
	CosNewStream returns. 

	This method creates a Cos stream object by writing its PDF 
	representation to an intermediate file, in the format specified 
	by PDF Reference:\n\n
	<</Length ... /Filter ... /DecodeParms ...>>\n
	stream\n
	... data, possibly encoded ...\n
	endstream\n\n

	This occurs in four steps: 

	1. Writing the attribute dictionary. 

	If attributesDict is a valid Cos dictionary, the method 
	writes that dictionary to the intermediate file. Otherwise, 
	it creates a new direct dictionary, determining a Length 
	key according to the sourceLength value:
	<ul>
	<li> If sourceLength is negative, or if the source data is to
	be encoded (see below), the value of the Length key is a reference to a 
	new indirect object, whose value will be set in Step 4. 
	<li> Otherwise, Length is a direct scalar representing sourceLength. 
	</ul>

	The dictionary that is written becomes the new stream's 
	attribute dictionary. 

	2. Reading the data. 

	sourceStart determines where in the source stream to begin 
	reading, and whether the source is seekable.
	
	<ul>
	<li> If sourceStart is a negative number, the source is assumed to be non-seekable 
	but positioned at the point where reading should start. 
	<li> Otherwise, the source is assumed to be seekable, and reading 
	starts at the position indicated by sourceStart. If sourceStart 
	is zero, data is read from the beginning of the source stream. 
	Positive values for sourceStart may be used, for instance, 
	to skip over initial data in the stream. 
	</ul>

	3. Encoding the data 
	
	If attributesDict is a valid Cos dictionary, and 
	it contains a Filter key, and encodeTheSourceData is true, 
	the method encodes the data after reading it from the source 
	stream and before writing it to the intermediate file. 

	The attributesDict is used as the new stream's dictionary. 
	The Filter entry in this dictionary indicates how the data 
	in the resulting Cos stream object will be subsequently 
	decoded; the value may be the name of a decoding filter 
	or an array of such names. Specify multiple filters in the 
	order they should be applied to decode the data. (If parameters 
	are needed to decode the data, they are specified as the 
	value of the DecodeParms key in attributesDict. See the 
	PDF Reference for details.) For each decoding filter, there 
	is a corresponding encoding filter, which the method applies 
	to the source data during this step. 

	If parameters are needed to encode the data, they must be 
	specified in the call by encodeParms. (Often, the encoding 
	parameters are different from the decoding parameters.) 
	The encodeParms parameter is optional for all encoding filters 
	except DCTDecode and JBIG2Decode. See the encodeParms field 
	of PDEFilterSpec. 

	If an array of filters is supplied, and at least one of 
	them requires encoding parameters, then a corresponding 
	array of encoding parameters is also required. Use the null 
	object to represent default parameters for filters that 
	have defaults.
	
	In any other case, the method copies the 
	source data directly into the Cos stream with no encoding. 
	If sourceLength is negative, it reads bytes until the source 
	reaches its EOF. Otherwise, sourceLength indicates how many 
	bytes to read from the source, and an exception is raised 
	if the source reaches EOF before that. 

	4. Writing the data 

	After the data is written, if the value of the Length key 
	in the attributes dictionary was an indirect reference (either 
	because it was supplied that way in attributesDict, or because 
	it was created that way in Step 1) the value of that indirect 
	object is set to the number of bytes actually written (that 
	is, the encoded length if the data was encoded). An indirect 
	Length key is useful for one-pass writing, when the size 
	of the written data is not known in advance, either because 
	the data was to be encoded, or because there was no way 
	to know how much data there would be before the source reached 
	its EOF. 

	NOTE: CosNewStream sets the document PDDocNeedsSave flag 
	(see PDDocFlags). 

	NOTE: You cannot call CosStreamPos on a stream created with 
	CosNewStream until the file has been saved. 
	@param dP The Cos document in which the newly created 
	stream will be used.
	@param indirect Must always be true, specifying that the 
	Cos stream is created as an indirect object (all streams 
	are indirect). This also sets the document's PDDocNeedsSave 
	flag (see PDDocFlags). 
	@param stm The source stream containing the data to copy 
	into the new stream. The caller is responsible for closing 
	stm after CosNewStream returns. The source stream can be 
	any readable ASStm. Typical sources are:
	<ul>
	<li> From files (ASFileStmRdOpen) 
	or from memory (ASMemStmRdOpen). These streams are always 
	seekable.
	<li> From arbitrary procedures (ASProcStmRdOpen or 
	ASProcStmRdOpenEx), or from other Cos streams (CosStreamOpenStm). 
	These streams are always non-seekable. 
	</ul>
	@param sourceStart The byte offset into stm from which 
	data reading starts for a seekable stream. If the value 
	is negative, specifies that the stream is not seekable. 
	
	@param encodeTheSourceData Whether the data in stm should 
	be encoded using filters specified in attributesDict before 
	it is written to the Cos stream. See the description of 
	the encoding step above. If attributesDict is a null object 
	or if the dictionary has no Filter key, this value is ignored. 
	
	@param attributesDict Either the null Cos object, or a 
	direct Cos dictionary containing stream attributes, such 
	as the length of the Cos stream data and a list of decoding 
	filters and parameters to apply to the data, as defined 
	in Section 3.2.7 in the PDF Reference. See the encoding 
	step in the description above. 
	@param encodeParms The parameters to be used by the filters 
	if the source data is encoded before it is written to the 
	file. The parameters follow the structure for the value 
	of the DecodeParms stream attribute described in Table 3.4 
	in the PDF Reference. See the encoding step in the description 
	above. If no encoding parameters are needed, this value 
	is ignored. 
	@param sourceLength The amount of data to be read from 
	the source. If negative (typically -1), data is read from 
	the source until it reaches its EOF. See Step 1 in the description 
	above.
	@return The newly created stream Cos object. 
	@exception Raises an exception if attributesDict is neither the null 
	object nor a direct Cos dictionary. 
	@exception Raises an exception if sourceStart is nonnegative but the 
	source is not seekable. 
	@exception Raises an exception if sourceLength is nonnegative but the 
	source stream reaches EOF before that many bytes have been read. 
	@see CosObjDestroy 
	@see CosNewStream64
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewStream, (CosDoc dP, ASBool indirect, ASStm stm, CosStreamStartAndCode sourceStart, ASBool encodeTheSourceData, CosObj attributesDict, CosObj encodeParms, CosByteMax sourceLength))

/**
	Destroys a Cos object. This method does nothing if obj is 
	a direct scalar object, such as the null object. 

	If a composite object (array, dictionary or stream) is destroyed: 
	<ul>
	<li> all the direct objects in it are automatically destroyed 
	<li> the indirect objects in it are not destroyed 
	</ul>
	@param obj The object to destroy.
	@see CosNewArray 
	@see CosNewBoolean 
	@see CosNewDict 
	@see CosNewFixed 
	@see CosNewInteger 
	@see CosNewName 
	@see CosNewStream 
	@see CosNewString 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosObjDestroy, (CosObj obj))

/**
	Gets the 32-bit integer value of a specified number object. 
	@param obj The object whose integer value is obtained. 
	It must have type CosInteger or CosReal (CosFixed). If it is CosReal, 
	its value is rounded to the nearest integer. The result 
	is undefined if the real value is outside the range of ASInt32 
	numbers.
	@return The 32-bit integer value of obj. 
	@exception Raises an exception if the given object has the wrong Cos 
	type. 
	@see CosNewInteger 
	@see CosNewFixed 
	@see CosNewFloat
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASInt32, CosIntegerValue, (CosObj obj))

/**
	Gets the value of obj as a fixed-point real number. 
	@param obj The object whose value is obtained. It must 
	have type CosInteger or CosReal (CosFixed). The result is undefined 
	if the real value is outside the range of ASFixed numbers.
	@return The numeric value of obj, represented as a fixed-point number. 
	@exception Raises an exception if the given object has the wrong Cos 
	type. 
	@see CosIntegerValue 
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASFixed, CosFixedValue, (CosObj obj))

/**
	Gets the value of the specified boolean object. 
	@param obj The boolean Cos object whose value is obtained.
	@return The value of obj. 
	@exception Raises an exception if obj has the wrong Cos type. 
	@see CosNewBoolean 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosBooleanValue, (CosObj obj))


/**
	Gets the value of a name object. 
	@param obj The object of type CosName whose value is obtained.
	@return The ASAtom corresponding to the specified name object. An ASAtom 
	can be converted to a string using ASAtomGetString.
	Note that CosCopyNameStringValue can be used to obtain the name as a
	string, without creating an ASAtom. (ASAtoms consume global memory
	that is not deallocated.)
	@exception Raises an exception if obj has the wrong type, if storage 
	is exhausted, or if file access fails. 
	@see CosNewName 
	@see CosNewNameFromString
	@see CosCopyNameStringValue
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASAtom, CosNameValue, (CosObj obj))

/**
	Gets the value of string Cos object, and the string's length. 
	

	NOTE: The pointer returned from this method is not guaranteed 
	to remain valid if CosStringValue is called again. It is 
	recommended that you use CosStringValueSafe or CosCopyStringValue 
	instead; these methods place the string into a user-allocated 
	buffer. 

	NOTE: The caller must immediately copy the returned string. 
	The memory pointed to be the return value may become invalid 
	if any memory-allocating calls are made. In particular, 
	consider the sequence: 
		str1 = CosStringValue(...); 
		str2 = CosStringValue(...); 
	In this case, the contents of str1 
	may be invalid by the time the second CosStringValue call 
	returns. 

	NOTE: The returned value is not a C-style string. Cos string 
	objects can contain NULL bytes. Standard C string-handling 
	functions may not work as expected. 
	@param obj IN The object whose value is obtained. 
	@param nBytes OUT (Filled by the method) The length of the 
	string, in bytes. Must be a non-NULL pointer.
	@return The value of obj. 
	@exception Raises an exception if the type of obj is not CosString. 
	
	@see CosNewString 
	@see CosCopyStringValue 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(char *, CosStringValue, (CosObj obj, ASTCount *nBytes))


/**
	Gets the value of the specified key in the specified dictionary. 
	If called with a stream object instead of a dictionary object, 
	this method gets the value of the specified key from the 
	stream's attributes dictionary. 

	NOTE: Use CosObjEnum to list all key-value pairs in a dictionary. 
	
	@param dict The dictionary or stream from which a value 
	is obtained. 
	@param key The key whose value is obtained, repesented as an ASAtom.
	See the PDF 
	Reference Manual to obtain the names of keys in dictionary objects 
	that are part of standard PDF, such as annotations or page 
	objects. Example: CosDictGet(dict, ASAtomFromString("Length")).

	Note that strings can be used directly as keys, by calling
	CosDictGetKeyString. Example: CosDictGetKeyString(dict, "Length").
	This method is preferred, because it avoids the creation of new ASAtoms.

	NOTE ON KEY NAMES: Even though key names in a PDF file are written with
	a leading slash (e.g., <</Length 42>>), the slash is omitted
	when creating an ASAtom to be used as a key, or when using the
	string directly as a key, as in the examples above.

	Cos name objects can also be used as keys, by calling CosDictGetKey.
	This method will also avoid the creation of new ASAtoms and is often
	more convenient than using ASAtoms or strings.

	@return The object associated with the specified key. If key is 
	not present or if its value is null (which is equivalent), returns 
	a null Cos object (a Cos object of type CosNull.) 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut 
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDictGet, (CosObj dict, ASAtom key))

/**
	Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present. Sets the PDDocNeedsSave 
	flag (see PDDocSetFlags) of dict's CosDoc if dict is indirect 
	or is a direct object with an indirect composite object 
	at the root of its container chain. 

	This method can also be used with a stream object. In that 
	case, the key-value pair is added to the stream's attributes 
	dictionary. 

	It is not safe to call CosDictPut during a call to CosObjEnum
	on that same dictionary, i.e., from within the callback procedure.

	NOTE: A dictionary entry whose value is null is equivalent 
	to an absent entry; using CosDictPut to put a null value 
	in a dictionary has the same effect as calling CosDictRemove 
	to remove it from the dictionary. 

	@param dict The dictionary or stream in which a value 
	is set. 
	@param key The key whose value is set, represented as an ASAtom.
	See the PDF Reference Manual
	to obtain the names of keys in dictionary objects that are 
	part of standard PDF, such as annotations or page objects. 
	(See CosDictGet for NOTE ON KEY NAMES.)

	Note that strings can be used directly as keys, by calling
	CosDictPutKeyString. Example: CosDictPutKeyString(dict, "Length", lenObj).
	This method is preferred, because it avoids the creation of new ASAtoms.

	Cos name objects can also be used as keys, by calling CosDictPutKey.
	This method will also avoid the creation of new ASAtoms and is often
	more convenient than using ASAtoms or strings.

	@param val The value to set.
	@exception Raises an exception if val is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if dict and val belong to different documents.
	
	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDictPut, (CosObj dict, ASAtom key, CosObj val))

/**
	Removes a key-value pair from a dictionary. Sets the PDDocNeedsSave 
	flag (see PDDocSetFlags) of dict's CosDoc if the dictionary 
	is indirect or has an indirect composite object at the root 
	of its container chain.

	If called with a stream object instead of a dictionary object, 
	this method removes the value of the specified key from the 
	stream's attributes dictionary. 

	It is not safe to call CosDictRemove during a call to CosObjEnum
	on that same dictionary, i.e., from within the callback procedure.

	If the key is not present in the dictionary, CosDictRemove has no effect.

	@param dict The dictionary from which the key-value pair is removed. 
	@param key The key to remove, represented as an ASAtom. 
	See the PDF Reference Manual to 
	obtain the names of keys in dictionary objects that are 
	part of standard PDF, such as annotations or page objects. 
	(See CosDictGet for NOTE ON KEY NAMES.)

	Note that strings can be used directly as keys, by calling
	CosDictRemoveString. Example: CosDictRemoveString(dict, "Length").
	This method is preferred, because it avoids the creation of new ASAtoms.

	Cos name objects can also be used as keys, by calling CosDictRemoveKey.
	This method will also avoid the creation of new ASAtoms and is often
	more convenient than using ASAtoms or strings.

	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosDictRemoveKey
	@see CosDictRemoveKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDictRemove, (CosObj dict, ASAtom key))

/**
	Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from CosDictGet is a null Cos object. 

	If called with a stream object instead of a dictionary object, 
	this method tests whether the specified key is found in the 
	stream's attributes dictionary. 

	@param dict The dictionary or stream in which to look for key.
	@param key The key to find. See the PDF Reference Manual to obtain 
	the names of keys in dictionary objects that are part of 
	standard PDF, such as annotations or page objects. 
	(See CosDictGet for NOTE ON KEY NAMES.)

	Note that strings can be used directly as keys, by calling
	CosDictKnownKeyString. Example: CosDictKnownKeyString(dict, "Length").
	This method is preferred, because it avoids the creation of new ASAtoms.

	Cos name objects can also be used as keys, by calling CosDictKnownKey.
	This method will also avoid the creation of new ASAtoms and is often
	more convenient than using ASAtoms or strings.

	@return true if the value of a key is known (exists and is not null) 
	in dict; false otherwise. 
	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosDictKnown, (CosObj dict, ASAtom key))

/**
	Gets the specified element from an array. 
	@param array The array from which an element is obtained. 
	
	@param index The array element to obtain. The first element 
	in an array has an index of zero. 
	@return The Cos object occupying the indexth element of array. Returns 
	a null Cos object if index is outside the array bounds. 
	
	@see CosArrayLength 
	@see CosArrayPut 
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosArrayGet, (CosObj array, ASTArraySize index))


/**
	Puts the specified object into the specified location in 
	an array. The array is extended as much as necessary and 
	null objects are stored in empty slots. Sets the PDDocNeedsSave 
	flag (see PDDocSetFlags) flag of array's CosDoc if array 
	is indirect or is a direct object with an indirect composite 
	object at the root of its container chain.

	It is not safe to call CosArrayPut during a call to CosObjEnum
	on that same array, i.e., from within the callback procedure, if
	doing so would extend the length of the array.

	@param array The array in which obj is stored. 
	@param index The location in array to store obj. The first 
	element of an array has an index of zero. 
	@param obj The Cos object to insert into array.
	@exception Raises an exception if object to insert is a direct object 
	that is already contained in another 
	@exception object or if object to insert belongs to another document. 
	
	@see CosArrayLength 
	@see CosArrayGet 
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayPut, (CosObj array, ASTArraySize index, CosObj obj))

/**
	Inserts an object into an array. 
	@param array The array into which the object is inserted. 
	
	@param pos The location in the array to insert the object. 
	The object is inserted before the specified location. The 
	first element in an array has a pos of zero. If pos ³ CosArrayLength(array), 
	obj is added at the end of the array. The length of the 
	array always increases by 1.

	It is not safe to call CosArrayInsert during a call to CosObjEnum
	on that same array, i.e., from within the callback procedure.

	@param obj The object to insert.
	@exception Raises an exception if object to insert is a direct object 
	that is already contained in another 
	object or if object to insert belongs to another document. 
	
	@see CosArrayLength 
	@see CosArrayRemove 
	@see CosArrayGet 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayInsert, (CosObj array, ASTArraySize pos, CosObj obj))

/**
	Finds the first element, if any, equal to the specified 
	object and removes it from the array. CosObjEqual is used 
	to determine whether an array element is equal to the specified 
	object. 

	The array is compressed after removing the element. The 
	compression is accomplished by moving each element following 
	the deleted element to the slot with the next smaller index 
	and decrementing the array's length by 1. 

	It is not safe to call CosArrayRemove during a call to CosObjEnum
	on that same dictionary, i.e., from within the callback procedure.

	@param array The array from which obj is removed. 
	@param obj The object to remove.
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayRemove, (CosObj array, CosObj obj))

/**
	Gets the number of elements in array. 
	@param array IN/OUT? The array for which the number of elements 
	is determined. 
	@return The number of elements in array. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTArraySize, CosArrayLength, (CosObj array))


/**
	Gets the length of a Cos stream from the Length key in the 
	stream's attributes dictionary. This specifies the length 
	of the undecoded data; that is, the number of bytes in the 
	stream before the Filter (if any) is applied.

	This has the same effect as calling
	CosIntegerValue(CosDictGetKeyString(stream, "Length"))

	@param stream The stream whose length is obtained.
	@return The length of the stream. 
	@exception Raises an exception if the Length key is not found in the 
	attributes dictionary, if its value is 
	not an integer, or if its value is outside the
	range of 32-bit integers.
	@see CosStreamDict 
	@see CosStreamPos
	@see CosStreamLength64
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTArraySize, CosStreamLength, (CosObj stream))

/**
	Gets a stream's attributes dictionary. 
	@param stream IN/OUT? The stream whose attributes dictionary is 
	obtained. 
	@return The stream's attributes dictionary Cos object. 
	@see CosStreamLength 
	@see CosStreamPos 
	@see CosDictGet 
	@see CosDictPut 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosStreamDict, (CosObj stream))

/**
	Creates a new, non-seekable ASStm for reading data from 
	a Cos stream. The data in the Cos stream may be filtered 
	and encrypted. After opening the Cos stream, data can be 
	read from it into memory using ASStmRead. When reading is 
	completed, close the stream using ASStmClose. 
	@param stream The Cos stream object for which an ASStm 
	is opened. 
	@param mode Must be one of the CosStreamOpenMode values.
	@return The newly-opened ASStm. 
	@see ASStmRead 
	@see ASStmWrite 
	@see CosNewStream 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASStm, CosStreamOpenStm, (CosObj stream, CosStreamOpenMode mode))

/**
	Gets the byte offset of the start of a Cos stream's data 
	in the PDF file (that is, the byte offset of the beginning 
	of the line following the 'stream' token). Use this method 
	to obtain the file location of any private data in a stream 
	that you need to read directly rather than letting it pass 
	through the normal Cos mechanisms. For example, a QuickTime 
	video embedded in a PDF file. 

	CosStreamPos is only valid when called on a stream that 
	is already stored in a PDF document. If the stream was created 
	using CosNewStream, the new stream is stored in the document's 
	temp file, and you cannot invoke CosStreamPos on it. After 
	the file has been saved, you can use CosStreamPos on the 
	stream. 
	@param stream The stream whose current position is obtained.
	@return The byte offset of the start of the Cos stream's data in 
	the PDF file. 
	@exception Raises cosErrInvalidObj if the stream object has not yet 
	been saved to the PDF file. In 
	@exception other words, before you can call CosStreamPos on a newly 
	created stream, you must first save the PDF file. 
	@see CosStreamDict 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTCount, CosStreamPos, (CosObj stream))

/**
	Gets the Catalog (the root object) for the specified document. 
	See Section 3.6.1 in the PDF Reference for a description 
	of the Catalog. 
	@param dP IN/OUT? The document whose Catalog is obtained. 
	@return The document's Catalog dictionary Cos object. 
	@see CosDocGetInfoDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDocGetRoot, (CosDoc dP))

/**
	Gets the specified document's Info dictionary. In general, 
	access the document's Info dictionary using PDDocGetInfo 
	and PDDocSetInfo wherever possible. 
	@param dP IN/OUT? The document whose Info dictionary is obtained. 
	
	@return The document's Info dictionary Cos object. 
	@see CosDocGetRoot 
	@see PDDocGetInfo 
	@see PDDocSetInfo 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDocGetInfoDict, (CosDoc dP))

/* The following method API's are in PICrypt.h */

/**
	Decrypts data in a buffer using the specified encryption 
	key. The standard Acrobat viewer encryption/decryption algorithm 
	(RC4 from RSA Data Security, Inc.) is used. 
	@param src The buffer containing the data to decrypt. 
	
	@param len The number of bytes in src. 
	@param dst (Filled by the method) The buffer into which 
	the decrypted data will be placed. This may point to the 
	same location as src. 
	@param cryptData The encryption key. 
	@param cryptDataLen Length of the encryption key, in bytes. 
	It cannot be greater than 5.
	@exception Raises an exception if encryption encounters an internal 
	error. 
	@see CosEncryptData 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDecryptData, (void *src, ASTArraySize len, void *dst, char *cryptData, ASTArraySize cryptDataLen))

/**
	Encrypts data in a buffer using the specified encryption 
	key. The standard Acrobat viewer encryption/decryption algorithm 
	(RC4 from RSA Data Security, Inc.) is used. 
	@param src The buffer containing the data to encrypt. 
	
	@param len The number of bytes in src. 
	@param dst (Filled by the method) The buffer into which 
	the encrypted data will be placed. This may point to the 
	same location as src. 
	@param cryptData The encryption key. 
	@param cryptDataLen Length of the encryption key, in bytes. 
	It cannot be greater than 5.
	@exception Raises an exception if encryption encounters an internal 
	error. 
	@see CosDecryptData 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosEncryptData, (void *src, ASTArraySize len, void *dst, char *cryptData, ASTArraySize cryptDataLen))

/* Acrobat 3.0 additions */

/**
	Opens a Cos document. The document does not need to be a 
	PDF document. In params, the client specifies a fileSys 
	and pathName from which to open the document. The client 
	may also specify a header string other than '%PDF-'. For 
	example, a client might want to open a private file type, 
	for example, '%FDF-'. 

	If the doRepair flag is set in the open flags, a minimal 
	document can be opened. A minimal document contains the 
	header string and a trailer dictionary. It may contain indirect 
	objects before the trailer dictionary, and the trailer dictionary 
	can refer to those objects. For example: 

	%FDF-1.0\n
	1 0 obj\n
	<< /Version /1.5\n
	/FDF << /F 20 0 R /JavaScript 5 0 R >>\n
	>>\n
	trailer\n
	<<\n
	/Root 1 0 R\n
	>> 
	@param params Specifies how to open the document.
	@return A Cos document. 
	@exception Various 
	@see CosDocClose 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(CosDoc, CosDocOpenWithParams, (CosDocOpenParams params))

/**
	Closes a Cos document. You should only call this method 
	with a document obtained via CosDocOpenWithParams to release 
	resources used by the Cos document. 
	@param cosDoc IN/OUT? The document to close. 
	@see CosDocOpenWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocClose, (CosDoc cosDoc))

/**
	Creates an empty Cos document. 
	@param createFlags An inclusive OR of bit flags that specify 
	the attributes of a CosDoc when created by CosDocCreate. 
	The only flag currently defined is:
	<ul>
	<li> cosDocCreateInfoDict (0x01): Creates an Info dictionary for the document.
	</ul>
	@return An empty Cos document. 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(CosDoc, CosDocCreate, (ASFlagBits createFlags))

/**
	Saves a Cos document to a file handle. CosDocSaveToFile 
	will not generate an XRef table in the saved file. If you 
	want the XRef to be generated, then you have to use CosDocSaveWithParams, 
	which generates the XRef table by default. 
	@param cosDoc IN/OUT? Document to save. 
	@param asFile IN/OUT? File to which the document is written; must 
	be open in write mode. This file is not necessarily position-
	able. 
	@param saveFlags IN/OUT? An OR of the values listed in CosDocSave 
	Flags specifying how to save the document. 
	@param saveParams IN/OUT? Optional parameters for use when saving 
	a document, as described in CosDocSaveParams. 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
#if READER || READER_PLUGIN
NOPROC(CosDocSaveToFile)
#else

/**
	Saves a Cos document to a file. CosDocSaveToFile will not 
	generate a cross-reference index (table or stream) in the 
	saved file. If you want the index to be generated, then 
	you have to use CosDocSaveWithParams, which generates it 
	by default. 
	@param cosDoc Document to save. 
	@param asFile File to which the document is written; must 
	be open in write mode. This file is not necessarily positionable. 
	
	@param saveFlags An OR of the CosDocSaveFlags bit flag 
	values specifying how to save the document. 
	@param saveParams Optional parameters for use when saving 
	a document, as described in CosDocSaveParams.
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocSaveToFile, (CosDoc cosDoc, ASFile asFile, CosDocSaveFlags saveFlags, CosDocSaveParams saveParams))

#endif

/**
	Sets a Cos document's dirty flag to a given boolean value. 
	If this flag is true when the document is closed, it indicates 
	that the document must be saved to preserve changes. 
	@param cosDoc The Cos document whose dirty flag is set. 
	
	@param isDirty true if dirty, false otherwise.
	@see CosDocSaveToFile 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocSetDirty, (CosDoc cosDoc, ASBool isDirty))

/* Acrobat 4.0 additions */

/**
	Gets the local master index for an indirect object. For 
	indirect objects, the local master index is the same as 
	the indirect object index that appears in the PDF file. 
	
	@param obj IN/OUT? The indirect CosObj for which the ID is obtained. 
	A CosObj can be determined to be indirect using CosObjIsIndirect. 
	
	@return The ID of obj. 
	@exception Raises cosErrInvalidObj if the object is not valid or is 
	not indirect. 
	@see CosDocGetObjByID 
	@see CosObjGetGeneration 
	@see CosObjIsIndirect 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosID, CosObjGetID,(CosObj obj))

/**
	Gets the generation number of an indirect Cos object. See 
	Section 3.2.9 in the PDF Reference for more information. 
	
	@param obj IN/OUT? The indirect CosObj for which the generation 
	number is obtained. A CosObj can be determined to be indirect 
	using CosObjIsIndirect. 
	@return The generation number of cosObj. 
	@exception Raises cosErrInvalidObj if the object is not valid or is 
	not indirect. 
	@see CosObjGetID 
	@see CosObjIsIndirect 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosGeneration, CosObjGetGeneration, (CosObj obj))

/**
	Gets the indirect CosObj with the latest generation number. 
	
	@param dP The CosDoc to search for the matching Cos object. 
	
	@param objNum The local master index for the indirect 
	Cos object to return.
	@return The CosObj with the latest generation number whose ID (object 
	number) equals objNum, or the null object if there is no 
	object with this ID. 
	@see CosObjGetID 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosObj, CosDocGetObjByID, (CosDoc dP, CosID objNum))

/**
	Saves a Cos document, optionally to a new file handle. Generates 
	an XRef table by default. 
	@param cosDoc IN/OUT? The CosDoc for the document to save. 
	@param asFile IN/OUT? (Optional) If saving to the same file, do 
	not pass in an ASFile. If saving to a different file, specify 
	the file to which the document is written; it must be open 
	in write mode. If NULL, this method saves to the ASFile 
	originally associated with the CosDoc. 
	@param saveFlags IN/OUT? A bit field composed of the CosDocSaveFlags
	specifying how to save the document. 
	@param saveParams IN/OUT? (Optional) CosDocSaveParams parameters 
	for use when saving the CosDoc document. 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00040000
*/
#if READER || READER_PLUGIN
NOPROC(CosDocSaveWithParams)
#else

/**
	Saves a Cos document, optionally to a new file. Generates 
	a cross-reference index (table or stream) by default. 
	@param cosDoc The CosDoc for the document to save. 
	@param asFile The file to which the document will be written. 
	This file must already be open in write mode. If you pass 
	NULL, cosDoc is saved to the file with which it was originally 
	associated. 
	@param saveFlags An OR of the CosDocSaveFlags bit flag 
	values specifying how to save the document. 
	@param saveParams CosDocSaveParams parameters for use 
	when saving the CosDoc document. 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(void, CosDocSaveWithParams, (CosDoc cosDoc, ASFile asFile, CosDocSaveFlags saveFlags, CosDocSaveParams saveParams))

#endif

/**
	Calls the specified procedure for each EOF in a given CosDoc, 
	where the EOF is a position in a PDF file after a %%EOF 
	keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. Not every %%EOF keyword fits these 
	criteria. For example, the first %%EOF in a linearized file 
	does not, so its position is not be passed to proc. 

	If cosDoc was created in memory (using CosDocCreate), or 
	if it was damaged and needed to be repaired, the procedure 
	is not called at all. 
	@param cosDoc The CosDoc in which the EOF's are enumerated. 
	
	@param proc The CosDocEnumEOFsProc to call for each EOF. 
	
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return true if all of the calls to proc return true. false as soon 
	as a call to proc returns false. 
	@see CosDocEnumIndirect 
	@see CosDocEnumEOFs64
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosDocEnumEOFs, (CosDoc cosDoc, CosDocEnumEOFsProc proc, void * clientData))

/**
	Sets the hex flag of the CosString. The hex flag specifies 
	whether the CosString should be written out as hex when 
	writing the Cos Object to file. 
	@param cosObj The CosString for which the hex flag is 
	set. 
	@param setHex The value to set for the flag.
	@return The value of setHex. 
	@exception cosErrExpectedString 
	@see CosStringGetHexFlag 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosStringSetHexFlag, (CosObj cosObj, ASBool setHex))

/**
	Gets the hex flag of the CosString. The hex flag specifies 
	whether the CosString should be written out as hex when 
	writing the Cos Object to file. 
	@param cosObj IN/OUT? The CosString for which the hex flag is obtained. 
	
	@return The current value of the flag. 
	@exception cosErrExpectedString 
	@see CosStringSetHexFlag 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosStringGetHexFlag, (CosObj cosObj))

/**
	Gets a 32-bit hash-code for the given CosObj. 

	Two CosObjects with equal hash-codes are not necessarily 
	equal, however. Use CosObjEqual to determine the equality 
	of Cos objects. 
	@param obj The CosObj for which to obtain a hash-code.
	@return 32-bit hash-code for the given CosObj, or CosNewNull if 
	there is no object with this ID. 
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosHashCode, CosObjHash, (CosObj obj))

/**
	Copies a CosObj from one document to another (or the same 
	document). 
	@param srcObj The CosObj to copy. 
	@param destDoc The CosDoc for the document into which 
	the CosObj is copied. 
	@param copyIndirect true if all indirectly referenced 
	objects from srcObj are copied to destDoc, false otherwise.
	@return The CosObj which has been copied to the destination document. 
	
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosObj, CosObjCopy, (CosObj srcObj, CosDoc destDoc, ASBool copyIndirect))

/**
	Checks whether the position is within the array bounds and 
	then removes it from the array and moves each subsequent 
	element to the slot with the next smaller index and decrements 
	the array's length by 1. Sets the 'dirty' flag of array's 
	CosDoc. 
	@param array IN/OUT? The CosArray to remove the member from. 
	@param pos IN/OUT? The index for the array member to remove. Array 
	indices start at 0. 
	@see CosArrayRemove 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(void, CosArrayRemoveNth, (CosObj array, ASTArraySize pos))

/**
	Enumerates all the indirect objects of a given CosDoc. 

	The objects are enumerated in no particular order. Successive 
	enumerations of the same Cos document are not guaranteed 
	to enumerate objects in the same order. 

	This method does not enumerate invalid objects; this includes 
	objects that are defined as null, objects that are not defined 
	at all (that is, have no cross-reference entry), and objects 
	that are on the free list (see the PDF Reference). 
	@param dP The CosDoc whose indirect objects are enumerated. 
	
	@param proc User-supplied callback to call for each indirect 
	object in dP. Enumeration ends when proc returns false or 
	all indirect objects have been enumerated. The value parameter 
	returned in proc is always the null Cos object. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return true if all of the calls to proc returned true. false as 
	soon as a call to proc returns false. 
	@exception Re-raises any exception that proc raises. 
	@see CosObjEnum 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosDocEnumIndirect, (CosDoc dP, CosObjEnumProc proc, void * clientData))

/* Acrobat 4.05 additions */

/**
	Gets the current version number of the encryption algorithm 
	supported. 
	@return The current version number of encryption supported. 
	@see CosDecryptGetMaxKeyBytes 
	@see CosEncryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(ASTVersion, CosCryptGetVersion,       ())

/**
	Gets the maximum number of the decryption key length, in 
	bytes, for the specified cryptVersion. 
	@param cryptVersion IN/OUT? The Cos crypt version - the version 
	of the algorithm that is used to encrypt and decrypt document 
	data. cryptVersion equal to 0 is treated as cryptVersion 
	equal to 1 to maintain backward compatibility. 
	@return The maximum number of key length, in bytes, for the specified 
	cryptVersion. If cryptVersion is not currently supported, 
	returns -1. 
	@see CosCryptGetVersion 
	@see CosEncryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(CosByteMax, CosDecryptGetMaxKeyBytes, (ASTVersion cryptVersion))

/**
	Gets the maximum number of the encryption key length, in 
	bytes, for the specified cryptVersion. 
	@param cryptVersion IN/OUT? The Cos crypt version - the version 
	of the algorithm that is used to encrypt and decrypt document 
	data. cryptVersion equal to 0 is treated as cryptVersion 
	equal to 1 to maintain backward compatibility. 
	@return The maximum number of key length, in bytes, for the specified 
	cryptVersion. If cryptVersion is not currently supported, 
	it returns -1. 
	@see CosCryptGetVersion 
	@see CosDecryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(CosByteMax, CosEncryptGetMaxKeyBytes, (ASTVersion cryptVersion))

/* Acrobat 5.0 additions */

/**
	Returns a newly allocated buffer containing a copy of the 
	Cos object's string value. Upon return nBytes contains the 
	number of bytes in the original Cos string. CosCopyStringValue 
	never returns NULL; it raises an exception if the allocation 
	fails. The client is responsible for freeing the result 
	by calling ASfree. 

	CosCopyStringValue allocates extra memory past the end of 
	the string and writes zeros into these extra bytes to ensure 
	that the string is null-terminated whether viewed as a UTF-16 
	(Unicode) string or as a C string. (These bytes are not 
	included in the number returned in nBytes). If the Cos string 
	has 0 length, nBytes will be 0, and a pointer to newly allocated 
	memory containing some zero bytes is returned (that is, 
	CosCopyStringValue still returns a null-terminated string 
	but with zero length). 

	NOTE: In general, the returned value is not a null-terminated 
	C string. Cos string objects are binary and can contain 
	arbitrary byte sequences, including NULL characters. Standard 
	C string-handling functions may not work as expected. 
	@param obj IN The Cos object whose string value is copied 
	and returned. 
	@param nBytes OUT (Filled by the method) The length of the 
	original Cos string in bytes. Can be NULL if you don't care 
	how many bytes were in the original string.
	@return A copy of the Cos object's string value. 
	@exception Raises an out-of-memory exception if insufficient memory 
	is available. Can also raise any 
	@exception exception that CosStringValue can raise. 
	@see CosStringValueSafe 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(char *, CosCopyStringValue, (CosObj obj, ASTCount *nBytes))

/**
	Copies at most bufferLen bytes from obj's string value into 
	buffer, and stores the actual length of the Cos string in 
	*nBytes. If bufferLen is greater than the length of the 
	Cos string, the remaining bytes in buffer have undefined 
	values upon return. 

	NOTE: In general, the returned value is not a null-terminated 
	C string. Cos string objects are binary data and can contain 
	any arbitrary byte sequence, including embedded NULL characters. 
	Standard C string-handling functions may not work as expected. 
	@param obj The Cos object whose string value is copied. 
	@param buffer The buffer into which the Cos string value 
	is copied or NULL. 
	@param bufferSize The length of buffer or 0. 
	@param nBytes (Filled by the method) The length of the 
	original Cos string in bytes (which may be more than bufferLen). 
	Must be a non-NULL pointer.
	@return A copy of the Cos string value or an exception. Will never 
	return NULL. 
	@exception Will raise a bad-parameter exception if bufferLen is less 
	than 0, or nBytes is NULL. Can 
	@exception also raise any exception that CosStringValue can raise. 
	
	@see CosCopyStringValue 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(char *, CosStringValueSafe, (CosObj obj, char *buffer, ASTArraySize bufferSize, ASTCount *nBytes))


/**
	Returns two ID byte arrays identifying the CosDoc. The client 
	should copy these arrays before making the next call to 
	Acrobat. 
	@param dP IN/OUT? The CosDoc whose ID byte arrays are returned. 
	
	@param pInstanceID IN/OUT? (Filled by the method) The instance 
	ID. 
	@param pPermaID IN/OUT? (Filled by the method) The permanent ID. 
	
	@param instIDLength IN/OUT? The length of pInstanceID, in bytes. 
	
	@param permIDLength IN/OUT? The length of pPermaID, in bytes. 
	@return true if the ID is returned, false otherwise. 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(ASBool, CosDocGetID, (CosDoc dP, CosByte **pInstanceID, CosByte **pPermaID, ASTCount *instIDLength, ASTCount *permIDLength))

/**
	Compares the two CosObjs. The result is 0 if and only if 
	CosObjEqual(obj1, obj2) is true. Otherwise, the result is 
	either -1 or 1. The result is useful for ordering or sorting 
	Cos objects. No other significance should be attached to 
	the result. In particular, a nonzero result indicates nothing 
	about the type of either object. 

	The result is valid only within a single instance of the 
	document. That is, if CosObjCmp returns a nonzero value 
	and the document is closed and then reopened, there is no 
	guarantee that it will return the same nonzero value for 
	those same objects. 

	The following conditions apply: 
	<ul>
	<li> If CosObjCmp (a, b) == 0, then CosObjCmp (b, a) == 0. 
	<li> If CosObjCmp (a, b) > 0, then CosObjCmp (b, a) < 0. 
	<li> If CosObjCmp (a, b) < 0, then CosObjCmp (b, a) > 0. 
	<li> If CosObjCmp (a, b) == 0, and CosObjCmp (b, c) == 0, then 
	CosObjCmp ( a, c ) == 0, then CosObjCmp (a, c) == 0. 
	<li> If CosObjCmp (a, b) > 0, and CosObjCmp (b, c) > 0, then 
	CosObjCmp (a, c) > 0. 
	<li> If CosObjCmp (a, b) < 0, and CosObjCmp (b, c) < 0, then 
	CosObjCmp (a, c) < 0. 
	</ul>
	@param obj1 The first CosObj to compare. 
	@param obj2 The second CosObj to compare.
	@return Returns zero if the two objects are equal, -1 if obj1 is 
	less than obj2, 1 if obj1 is greater than obj2. 
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(ASInt32, CosObjCmp, (CosObj obj1, CosObj obj2))

/**
	Puts a limit on the amount of memory (RAM) that can be used to store Cos
	objects. The default limit is 2MB, and this method can be used only to
	increase the limit. Beyond the limit, Cos objects may be stored on disk.
	NOTE: The limit applies only to fixed-size data in Cos objects, not to
	variable data stored in strings, arrays, dictionaries and streams. In some
	cases, objects may need to stay in memory, even if the limit is exceeded.
	@param maxMemory The maximum amount of RAM (in bytes) that will be used to
	store fixed-size Cos objects.
	@since PI_COS_VERSION >= 0x00050001
*/
NPROC(void, CosSetMaxDocStorage, (ASInt32 maxMemory))

/* Acrobat 6.0 additions */

/**
	Tests whether the definition of a specified Cos object, 
	in the file associated with the object's CosDoc, begins 
	within any of a set of byte ranges. The test is inclusive; 
	that is the object may begin at the first or last byte of 
	a range. 
	@param obj The Cos object (must be indirect). 
	@param byteRanges An array containing pairs of byte offsets 
	within the document. Each pair is a start and end offset 
	from the beginning of the document. 
	@param numEntries The number of byte offsets (not pairs) 
	in the byteRanges array.
	@return true if the object begins within any of the given ranges 
	and has not been modified, false otherwise. 
	@exception Raises an exception if obj is a direct object or numEntries 
	is an odd number. 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocObjIsWithinRange, (CosObj obj, Int32 byteRanges[], Int32 numEntries))

/**
	Tests whether an object is compressed - that is, part of a 
	CosObjCollection. 
	@param obj The object to test.
	@return true if obj is compressed, false otherwise. 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjIsCompressed, (CosObj obj))

/**
	Creates a new object collection for objects in a document. 
	
	@param dP The document whose objects are collected, or 
	NULL to create a null collection. (A null collection is 
	not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type CosObjCollection.)
	@return The newly created Cos object collection. 
	@see CosObjAddToCollection 
	@see CosObjCollectionEnum 
	@see CosObjGetCollection 
	@see CosObjCollectionIsNull 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(CosObjCollection, CosNewObjCollection, (CosDoc dP))

/**
	Tests whether an object collection is null. A null collection 
	is not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type CosObjCollection. 
	@param coll The object collection to test.
	@return true if coll is null, false otherwise. 
	@see CosNewObjCollection 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionIsNull, (CosObjCollection coll))

/**
	Gets the CosObjCollection containing the specified object. 
	If the object is not in a collection, the method raises 
	an exception. 
	@param obj The object whose CosObjCollection is obtained.
	@return The CosObjCollection to which the object belongs. 
	@exception Raises an error if obj is not in a collection.. 
	@see CosObjAddToCollection 
	@see CosObjIsCompressed 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(CosObjCollection, CosObjGetCollection, (CosObj obj))

/**
	Adds a Cos object to a collection; see CosObjCollection 
	for requirements of these collections. This method sets 
	the dirty flag of the collection's Cos document. 
	@param coll The Cos object collection. 
	@param item The object to add.
	@return true if obj was successfully added to the collection, false 
	otherwise. 
	@exception Raises an exception if the collection and the object belong 
	to different Cos documents. 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@see CosObjRemoveFromCollection 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjAddToCollection, (CosObjCollection coll, CosObj item))

/**
	Removes a Cos object from the CosObjCollection to which 
	is belongs. 
	@param obj The object to remove.
	@exception Raises an exception if the object is not in the collection. 
	@see CosObjAddToCollection 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjRemoveFromCollection, (CosObj obj))

/**
	Controls whether a Cos object can be compressed. A compressible 
	object can be added to a CosObjCollection. 

	If you set the compressibility to false, calling CosObjAddToCollection 
	on that object has no effect. If the object is already compressed, 
	it is removed from the object collection to which it belongs 
	and then marked as incompressible. 

	This method does nothing if applied to a direct object, 
	a stream, or an object whose generation number is not zero. 
	Objects of these types are never compressible. 
	@param obj The object whose compressibility is set. 
	@param compressible true if the object can be made part 
	of a CosObjCollection, false otherwise.
	@return true if obj is marked as compressible, false otherwise. 
	
	@see CosObjAddToCollection 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjSetCompressibility, (CosObj obj, ASBool compressible))

/**
	Tests whether an object is compressible. A compressible 
	object can be added to a CosObjCollection. An object is 
	compressible only if all of the following conditions are 
	true: 
	<ul>
	<li> It is indirect. 
	<li> It has a generation number of zero. 
	<li> It is not a stream. 
	<li> It has not been marked as incompressible by CosObjSetCompressibility. 
	</ul>

	@param obj The object to test.
	@return true if obj is compressible, false otherwise. 
	@see CosObjIsCompressed 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjGetCompressibility, (CosObj obj))

/**
	Gets the number of objects in an object collection. The 
	size of a null collection is zero. 
	@param coll The object collection whose size is obtained.
	@return The number of objects in the collection. 
	@see CosObjAddToCollection 
	@see CosObjRemoveFromCollection 
	@see CosObjCollectionEnum 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASUns32, CosObjCollectionSize, (CosObjCollection coll))

/**
	Tests whether two Cos object collections are the same collection. 
	Two null collections are always equal. (A null collection 
	is not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type CosObjCollection.) 
	@param c1, c2 Object collections to compare.
	@return true if c1 and c2 are the same collection, false otherwise. 
	
	@see CosNewObjCollection 
	@see CosObjGetCollection 
	@see CosObjCollectionIsNull 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionEqual, (CosObjCollection c1, CosObjCollection c2))

/**
	Enumerates the members of a Cos object collection, calling 
	a user-supplied procedure for each member object. The order 
	in which the objects are enumerated is undefined. 
	@param coll The object collection whose members are enumerated. 
	
	@param proc User-supplied callback to call for each member 
	object of coll. Enumeration ends if proc returns false. 
	The callback must not modify the collection (by adding or 
	removing objects, for example). Doing so produces undefined 
	results or errors. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return Returns the value that proc returned (that is, returns true 
	if all the member objects were enumerated, false if enumeration 
	was terminated at the request of proc). 
	@see CosObjGetCollection 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionEnum, (CosObjCollection coll, CosObjEnumProc proc,
								    void *clientData))

/**
	In version 6, CosObjRefreshAfterLinearizedSave
	updates an indirect Cos object after a linearized save operation. 
	Linearizing renumbers all indirect objects; this function 
	returns the new renumbered Cos object, which should be used 
	from this point on. This call is only valid from within 
	notification callbacks responding to the PDDocDidSave notification. 
	If called from outside this context, or if the passed Cos 
	object is direct, the function does not modify the object.

	In version 7 and later, linearizing does not renumber objects,
	and CosObjRefreshAfterLinearizedSave has no effect.
	
	@param obj A pointer to the object to refresh. The object 
	is updated by the method. 
	@param doc The document that was saved.
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjRefreshAfterLinearizedSave, (CosObj *obj, CosDoc doc))

/**
	Tests whether the Cos document is fully compressed. In a 
	fully compressed document, most objects are stored in object 
	streams, which are normally Flate-encoded to reduce the 
	size of the PDF file. Cross-reference information for these 
	objects is stored in cross-reference streams, which are 
	also normally Flate-encoded. See the PDF Reference. 

	NOTE: Fully compressed files are not compatible with PDF 
	1.4 and earlier viewers. 
	@param doc The document whose compression is checked.
	@return true if the document is fully compressed, false otherwise. 
	
	@see CosDocHasPartialCompression 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocHasFullCompression, (CosDoc doc))

/**
	Tests whether the Cos document is partially compressed. 
	In a partially compressed file, the size of the logical 
	structure information is reduced; however, this information 
	is unavailable to pre-PDF 1.5 viewers, while the document 
	can still be viewed and printed. PDF 1.5 viewers (such as 
	Acrobat 6) have full access to the structure information. 

	In a partially compressed document, objects related to logical 
	structure are stored in object streams, which are normally 
	Flate-encoded to compress the document. Their cross-reference 
	information is stored twice: in a cross-reference stream, 
	to which there is a reference in the trailer of an update 
	section; and in the main cross-reference table, which indicates 
	that the objects are on the free list. See the PDF Reference. 
	
	@param doc The document whose compression is checked.
	@return true if the document is partially compressed, false otherwise. 
	
	@see CosDocHasFullCompression 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocHasPartialCompression, (CosDoc doc))

/**
	Acrobat 7 additions
*/

/**
    Creates a new 64-bit integer object associated with the specified 
    document and having the specified value. 
    @param dP IN The document in which the integer is used. 
    @param indirect IN If true, creates the integer object as 
    an indirect object, and sets the document dP's PDDocNeedsSave 
    flag (see PDDocFlags). If false, creates the integer as 
    a direct object. 
    @param value IN The value, represented as a 64-bit integer.
    @return An object of type CosInteger.
    @see CosInteger64Value
	@see CosNewInteger
    @see CosNewFixed 
    @see CosNewFloat 
    @since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewInteger64, (CosDoc dP, ASBool indirect, ASInt64 value))

/**
	Gets the 64-bit integer value of a specified number object. 
	@param obj The object whose integer value is obtained. 
	It must have type CosInteger or CosReal (CosFixed). If it is CosReal, 
	its value is rounded to the nearest integer. The result 
	is undefined if the real value is outside the range of ASInt64 
	numbers.
	@return The 64-bit integer value of obj. 
	@exception Raises an exception if the given object has the wrong Cos 
	type. 
	@see CosNewInteger64
	@see CosIntegerValue
	@see CosFixed Value
	@see CosFloatValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASInt64, CosInteger64Value, (CosObj obj))

/**
	Creates a new real-number object from a single-precision floating-point number, 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If true, creates the real-number object 
	as an indirect object, and sets the document dP's PDDocNeedsSave 
	flag (see PDDocFlags). If false, creates the number 
	as a direct object. 
	@param value The real number, represented as a single-precision floating-point number.
	@return A Cos object of type CosReal (CosFixed).
	@see CosFloatValue 
	@see CosNewInteger
	@see CosNewInteger64
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewFloat, (CosDoc dP, ASBool indirect, float value))

/**
	Gets the value of obj as a single-precision floating-point real number. 
	@param obj The object whose value is obtained. It must 
	have type CosInteger or CosReal (CosFixed). The result is undefined 
	if the real value is outside the range of floating-point numbers.
	@return The numeric value of obj, represented as a floating-point number. 
	@exception Raises an exception if the given object has the wrong Cos 
	type.
	See CosNewFloat
	@see CosIntegerValue
	@see CosInteger64Value
	@see CosFixedValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(float, CosFloatValue, (CosObj obj))

/**
	Gets the value of the specified key in the specified dictionary. 

	For more details, see CosDictGet.

	@param dict The dictionary or stream from which a value is obtained. 
	@param key The key whose value is obtained, represented as a Cos name object. 
	@return The object associated with the specified key. If key is 
	not present, returns a null Cos object. 
	@see CosDictGet
	@see CosDictGetKeyString
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC( CosObj, CosDictGetKey,	(CosObj dict, CosObj key))

/**
	Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from CosDictGetKey is a null Cos object. 

	For more details, see CosDictKnown.

	@param dict The dictionary or stream in which to look for key.
	@param key The key to find, represented as a Cos name object.

	@return true if the value of a key is known (exists and is not null) 
	in dict; false otherwise. 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( ASBool, CosDictKnownKey,	(CosObj dict, CosObj key))

/**
	Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present.

	For more details, see CosDictPut.

	It is not safe to call CosDictPutKey during a call to CosObjEnum
	on that same dictionary, i.e., from within the callback procedure.

	@param dict The dictionary or stream in which a value is set. 
	@param key The key whose value is set, represented as a Cos name object.

	@param val The value to set.
	@exception Raises an exception if val is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if dict and val belong to different documents.
	
	@see CosDictPut
	@see CosDictPutKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictPutKey,	(CosObj dict, CosObj key, CosObj val))

/**
	Removes a key-value pair from a dictionary.

	For more details, see CosDictRemove.

	@param dict The dictionary from which the key-value pair 
	is removed. 
	@param key The key to remove, represented as a Cos name object.

	@see CosDictRemove 
	@see CosDictRemoveKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictRemoveKey,(CosObj dict, CosObj key))

/**
	Gets the value of the specified key in the specified dictionary. 

	For more details, see CosDictGet.

	@param dict The dictionary or stream from which a value is obtained. 
	@param key The key whose value is obtained, represented as a string. 
	@return The object associated with the specified key. If key is 
	not present, returns a null Cos object. 
	@see CosDictGet
	@see CosDictGetKey
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC( CosObj, CosDictGetKeyString,		(CosObj dict, const char* key))

/**
	Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from CosDictGetKeyString is a null Cos object. 

	For more details, see CosDictKnown.

	@param dict The dictionary or stream in which to look for key.
	@param key The key to find, represented as a string.

	@return true if the value of a key is known (exists and is not null) 
	in dict; false otherwise. 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( ASBool, CosDictKnownKeyString,	(CosObj dict, const char* key))

/**
	Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present.

	For more details, see CosDictPut.

	It is not safe to call CosDictPutKey during a call to CosObjEnum
	on that same dictionary, i.e., from within the callback procedure.

	@param dict The dictionary or stream in which a value is set. 
	@param key The key whose value is set, represented as a string.

	@param val The value to set.
	@exception Raises an exception if val is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if dict and val belong to different documents.
	
	@see CosDictPut
	@see CosDictPutKey
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictPutKeyString,		(CosObj dict, const char* key, CosObj val))

/**
	Removes a key-value pair from a dictionary.

	For more details, see CosDictRemove.

	@param dict The dictionary from which the key-value pair is removed. 
	@param key The key to remove, represented as a string.

	@see CosDictRemove 
	@see CosDictRemoveKey
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictRemoveKeyString,	(CosObj dict, const char* key))


/**
	"Weak" references and "strong" references.

	When a Cos document is saved in full-save mode, objects that are
	not accessible from the root of the document are destroyed. This
	process uses a mark-and-sweep garbage collector: the root is marked,
	and then every object to which it refers is marked, and so on. At
	the end of this marking phase, objects that are not marked are
	destroyed.

	A so-called "weak" reference changes this policy: during the marking
	phase, a reference that has been declared to be "weak" will not be
	marked. For example, when a dictionary is marked, all its keys and
	values are normally also marked. But if a certain key has been set as a weak
	reference, then the corresponding value will not be marked. Consequently, if there
	are no other references to that value, it will be destroyed.

	A so-called "strong" reference also changes this policy, but in the
	opposite direction. An object for which there is a strong reference
	*will* be marked (and therefore *not* garbage-collected), even if
	there is no path to the object from the root of the document, and even
	if a weak reference exists for it.

	CosDictSetWeakReference establishes or removes a weak reference from a dictionary.
	@param dict The dictionary containing the weak reference.
	@param key The name of a key in the dictionary.
	@param isWeak If 'isWeak' is true, then the object stored in 'dict' under 'key' at the time
	of every subsequent full-save GC will not be marked as a component of the 
	dictionary. If there is no
	other path to that object from the root of the document, then it will be garbage-
	collected (destroyed) by GC.
	It is not an error if there is no such value at the time of GC or
	at the time of the call to this function.

	If 'isWeak' is false (the default condition), then there is no such behavior,
	and the value, if any, will be marked in the normal manner. 
	The case where 'isWeak' is specified as false is intended primarily to reverse the 
	effect of a previous call in which 'isWeak' was true.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosDictSetWeakReference, (CosObj dict, const char* key, ASBool isWeak))

/**
	Gets the state of a weak reference. For details, see CosDictSetWeakReference.
	@param dict A dictionary
	@param key The name of a key
	@return Return the value of the 'isWeak' parameter in the most recent call
	to CosDictSetWeakReference with these parameters, or false if there has been
	no such call.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDictIsWeakReference, (CosObj dict, const char* key))

/**
	Establishes or removes a weak reference from an array.

	For a description of weak references, see CosDictSetWeakReference.

	@param array An array
	@param n The index of the element that is the weak reference. Note
	that the weak reference "travels" with the element; that is, if
	an item is marked as a weak reference, and an item is subsequently
	inserted before that item, the weak reference applies to the same 
	element as it did previously.
	@param isWeak Sets a weak reference for an array.
	@since PI_COS_VERSION >= 0x00070000

*/
NPROC(void, CosArraySetWeakReference, (CosObj array, ASInt32 n, ASBool isWeak))

/**
	Return the state of a weak reference in an array.
	See CosDictIsWeakReference for details.
	@param array An array
	@param n The index of an item in the array.
	@return Return the value of the 'isWeak' parameter in the most recent call
	to CosArraySetWeakReference with these parameters, or false if there
	has been no such call.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosArrayIsWeakReference, (CosObj array, ASInt32 n))

/**
	Create a "strong reference" for an object. 
	For a description of strong references, see CosDictSetWeakReference.

	For indirect objects and direct nonscalars, CosObjAcquire increments an internal
	reference count for obj. The reference count is used
	by the garbage collector, which is invoked during a full-save of the document. 
	If the refcount is positive at the time of GC (it is initially 0), then the object
	will not be garbage-collected, regardless of whether the object is accessible from
	the root of the document.
	@param obj A Cos object.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosObjAcquire, (CosObj obj))

/**
	Remove a "strong reference" for an object. 
	For a description of strong references, see CosDictSetWeakReference.

	For indirect objects and direct nonscalars, CosObjRelease decrements an internal
	reference count for obj. The reference count is used
	by the garbage collector, which is invoked during a full-save of the document. 
	If the refcount is positive at the time of GC (it is initially 0), then the object
	will not be garbage-collected, regardless of whether the object is accessible from
	the root of the document.
	@param obj A Cos object.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosObjRelease, (CosObj obj))

/**
	Creates a new name object associated with the specified 
	document and having the specified value. 
	@param dP The document in which the new name is used. 	
	@param indirect If true, creates the name as an indirect 
	object, and sets the document's PDDocNeedsSave flag (see 
	PDDocFlags) flag. If false, creates the name as a direct 
	object. 
	@param namestring The name to create.
	This routine will not create an ASAtom corresponding to 'namestring'
	and is generally more efficient than CosNewName. 
	(ASAtoms consume global memory that is not deallocated.)
	@return The newly created name Cos object. 
	@see CosNewName
	@see CosNameValue
	@see CosCopyNameStringValue
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC(CosObj, CosNewNameFromString, (CosDoc dP, ASBool indirect, const char* namestring))

/**
	Returns a newly allocated buffer containing a copy of the 
	Cos object's name as a null-terminated string. Upon return, nBytes contains the 
	number of bytes in the string. CosCopyNameStringValue 
	never returns NULL; it raises an exception if the allocation 
	fails. The client is responsible for freeing the result 
	by calling ASfree. 

	Unlike Cos strings, the strings corresponding to Cos names are
	null-terminated.

	This routine will avoid creating an ASAtom corresponding to the object's
	name and is generally more efficient than copying the value returned by
	ASAtomGetString(CosNameValue(obj)).
	(ASAtoms consume global memory that is not deallocated.)

	@param obj IN A Cos name object. 
	@param nBytes OUT The length of the name
	of the Cos object, and therefore the length of the returned
	string. nBytes may be NULL if you don't care 
	how many bytes are in the name.
	@return A copy of the Cos object's name, as a null-terminated string. 
	@exception Raises an out-of-memory exception if insufficient memory 
	is available.
	@see CosNewName
	@see CosNewNameFromString
	@see CosNameValue
	@see CosCopyStringValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(char *, CosCopyNameStringValue, (CosObj obj, ASTCount *nBytes))



/**
	Calls the specified procedure for each EOF in a given CosDoc.
	For details, see CosDocEnumEOFs. This is the same as CosDocEnumEOFs, 
	except that the callback proc takes a
	64-bit file position instead of a 32-bit file position.
	@param cosDoc The CosDoc in which the EOF's are enumerated. 
	
	@param proc The CosDocEnumEOFsProc64 to call for each EOF. 
	
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return true if all of the calls to proc return true. false as soon 
	as a call to proc returns false. 
	@see CosDocEnumIndirect 
	@see CosDocEnumEOFs64
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDocEnumEOFs64, (CosDoc cosDoc, CosDocEnumEOFsProc64 proc, void * clientData))


/**
	Tests whether the value of a Cos number is inside the range of 32-bit integers,
	[-2147483648, +2147483647]. 
	If so, the 32-bit value may be obtained by calling CosIntegerValue.
	If not, the 64-bit value may be obtained by calling CosIntegerValue64.

	Raises an exception if obj is not a number (CosInteger or CosReal).
	@param obj A Cos integer or real number.
	@return true if the value of the number is in the range of 32-bit integers,
	false otherwise.
	@see CosIntegerValue
	@see CosIntegervalue64
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosNumberIsWithinASInt32Range, (CosObj obj))


/**
	Tests whether the value of a Cos number is inside the range of ASFixed numbers,
	[-32768.0, +32768.0). 
	If so, the ASFixed value may be obtained by calling CosFixedValue.
	If not, the floating-point value may be obtained by calling CosFloatValue.

	Raises an exception if obj is not a number (CosInteger or CosReal).
	@param obj A Cos integer or real number.
	@return true if the value of the number is in the range of ASFixed,
	false otherwise.
	@see CosFixedValue
	@see CosFloatValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosNumberIsWithinASFixedRange, (CosObj obj))


/**
	Tests whether the definition of a specified Cos object, 
	in the file associated with the object's CosDoc, begins 
	within any of a set of byte ranges. For details, see CosDocObjIsWithinRange.
	This is the same as CosDocObjIsWithinRange, except that the byte-ranges are
	64-bit file positions instead of a 32-bit file positions.
	@param obj The Cos object (must be indirect). 
	@param byteRanges An array containing pairs of byte offsets 
	within the document. Each pair is a start and end offset 
	from the beginning of the document. 
	@param numEntries The number of byte offsets (not pairs) 
	in the byteRanges array.
	@return true if the object begins within any of the given ranges 
	and has not been modified, false otherwise. 
	@exception Raises an exception if obj is a direct object or numEntries 
	is an odd number. 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDocObjIsWithinRange64, (CosObj obj, 
										 ASFilePos64 byteRanges[],
										 ASInt32 numEntries))

/**
	Creates a new Cos stream, using data from an existing ASStm. 
	 
	
	For details, see CosNewStream.

	This is the same as CosNewStream, except that decodeLength is a
	64-bit value instead of a 32-bit value, and allowDelayedRead enables
	the implementation to avoid making an intermediate copy the stream data.
	This is useful when creating very large streams of data.

	@param dP The Cos document in which the newly created stream will be used.
	@param indirect Must always be true, specifying that the 
	Cos stream is created as an indirect object. 
	@param stm The source stream containing the data to copy 
	into the new stream. 
	@param stmStartPos Starting position for the Stm.  Default is 0.
	@param stmDataIsDecoded A boolean value indicating whether the data in stm should 
	be encoded using filters specified in attributesDict. 
	@param attributesDict Either the null Cos object, or a 
	direct Cos dictionary containing stream attributes. 
	@param encodeParms The parameters to be used by the filters 
	if the source data is to be encoded. 
	@param sourceLength The amount of data to be read from the source.
	@param allowDelayedRead If this is
	true and stm is seekable, then the data from stm will not be read during
	this call, but rather at a subsequent time, and it may be read more
	than once. In this case, THE CALLER MUST NOT CLOSE STM until it is established, through
	some independent mechanism, that the data will not be read again.
	(See ASProcStmRdOpenEx for further details on this feature.)
	If allowDelayedRead is false, the source data is copied during this call,
	so the source stream may be closed after 
	CosNewStream64 returns.
	@return The newly created stream Cos object. 
	@see CosNewStream
	@see ASProcStmRdOpenEx
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewStream64, (CosDoc dP,
							   ASBool indirect,
							   ASStm stm,
							   ASInt64 stmStartPos, 
                               ASBool stmDataIsDecoded,
							   CosObj attributesDict,
							   CosObj encodeParms,
							   ASInt64 sourceLength,
							   ASBool allowDelayedRead))

/**
	Gets the length of a Cos stream from the Length key in the 
	stream's attributes dictionary. See CosStreamLength for details.
	This is the same as CosStreamLength, except that the return value is a
	64-bit integer instead of a 32-bit integer.

	This has the same effect as calling
	CosInteger64Value(CosDictGetKeyString(stream, "Length"))

	@param stream The stream whose length is obtained.
	@return The length of the stream. 
	@exception Raises an exception if the Length key is not found in the 
	attributes dictionary, or if its value is 
	not an integer.
	@see CosStreamDict 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASInt64, CosStreamLength64, (CosObj stream))

/**
	Gets the byte offset of the start of a Cos stream's data 
	in the PDF file. For details, see CosStreamPos.
	This is the same as CosStreamPos, except that the return value is a
	64-bit file position instead of a 32-bit file position.
	@param stream The stream whose current position is obtained.
	@return The byte offset of the start of the Cos stream's data in 
	the PDF file. 
	@exception Raises cosErrInvalidObj if the stream object has not yet 
	been saved to the PDF file. 
	@see CosStreamPos 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASFilePos64, CosStreamPos64, (CosObj stream))




