/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright 1994-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

  Speller_Sel.h
  
 - Selectors for all Spelling HFT functions.

*********************************************************************/

#ifndef _H_Speller_SEL
#define _H_Speller_SEL

#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

enum SpellProcSelectors
{	SpellerFirst_SEL  = 0,

	SpellCheck_SEL,				/* show Spell Check dialog */
	SpellCheckText_SEL,			/* spell check a text buffer */
	SpellCheckWord_SEL,			/* spell check a word */

	SpellDomainNames_SEL,		/* return the array of Domain names */
	SpellAddDomain_SEL,			/* add a search Domain */
	SpellRemoveDomain_SEL,		/* remove a search Domain */

	SpellDictionaryNames_SEL,	/* return an array of dictionary names of the currently available dictionaries */
	SpellAddDictionary_SEL,		/* *** OBSOLETE *** */
	SpellRemoveDictionary_SEL,	/* *** OBSOLETE *** */

	SpellUserWords_SEL,			/* return an array of words added or removed from a dictionary */
	SpellAddWord_SEL,			/* add a word to a dictionary */
	SpellRemoveWord_SEL,		/* remove a word from a dictionary */

	SpellUserDictionaryOrder_SEL,	/* get the array of search order dictionary names the user has set in the Spelling Preferences Panel */
	SpellGetDocDictionaryOrder_SEL,	/* get the array of dictionary names for the document search order */
	SpellSetDocDictionaryOrder_SEL,	/* set an array of dictionary names for the document search order */

	SpellCountKnownWords_SEL,	/* count the known words in text buffer using an array of dictionaries */

	SpellIgnoreAll_SEL,			/* ignore this word in the current document */

	SpellHyphenateWord_SEL,		/* hyphenate a word */
	SpellGetNextWord_SEL,		/* scan buffer for next Roman word */

	SpellCheckRTF_SEL,			/* show Spell Check dialog with rich text change call back */

	SpellLanguages_SEL,				/* return an array of ISO 639-2 and 3166 language codes of the currently available dictionaries */
	SpellUserLanguageOrder_SEL,		/* get the array of search order ISO 639-2 and 3166 language codes the user has set in the Spelling Preferences Panel */
	SpellGetDocLanguageOrder_SEL,	/* get the array of ISO 639-2 and 3166 language codes for the document search order */
	SpellSetDocLanguageOrder_SEL,	/* set an array of ISO 639-2 and 3166 language codes for the document search order */

	SpellCustomDictionaryClose_SEL,		/* close a custom dictionary opened by SpellCustomDictionaryOpen */
	SpellCustomDictionaryCreate_SEL,	/* create a custom dictionary */
	SpellCustomDictionaryDelete_SEL,	/* delete a custom dictionary */
	SpellCustomDictionaryExport_SEL,	/* export a custom dictionary */
	SpellCustomDictionaryOpen_SEL,		/* open a custom dictionary and add it to the top of the search order */

	SpellerLast_SEL	};

#define SpellerNum_SEL				SpellerLast_SEL - 1
#define SpellerHFT_NAME				"Spell"
#define SpellerHFT_VERSION_1_1		0x00010001
#define SpellerHFT_VERSION_1_2		0x00010002
#define SpellerHFT_VERSION_1_3		0x00010003
#define SpellerHFT_LATEST_VERSION	SpellerHFT_VERSION_1_3

#define kSpellLastBuffer			true
#define kSpellNotLastBuffer			false
#define kSpellMaxName				48

/**  Possible results of the spell check when the SpellCheck method is called.
	@see SpellCheck
*/
enum SpellDialogResult
{
	/**	The user clicked the "Done" button to dismiss the dialog and
		did not complete the spell check, but may have made
		changes to the text buffer. 
	*/
	kSpellDone = 0,				
								
	/**	The user completed spell checking of the input buffer, or all
		words were correct. A new text buffer is returned if
		corrections where made.
	*/
	kSpellCompleted,			
								
	/**	The spell check dialog failed due to an internal error. */
	kSpellFailed = -1,			

	/**	*/
	kSpellResultLast	
};

/** SpellingDomainFlags
	Valid values for the scFlags field of the SpellCheckParam block. For Acrobat 5 and
	higher, all clients should set this field to kSpellFlagAllDomain. All other bits in this
	flag are reserved for future use. Spelling clients’ private data and flags can be stored in the
	scClientData field.
*/
enum SpellDomainFlags
{
	/**	Default domain behavior. */
	kSpellFlagNone		= 0x0000,	
	/**	For Acrobat 5 and higher, all registered spelling domains
		should set this flag. Only the "All" domains are called by
		spelling when the user clicks the "Start" button on the
		Spell Check dialog.
	*/
	kSpellFlagAllDomain	= 0x0001,
	/** Announce to the client that spell dialog has been just popped */
	kSpellCheckStart	= 0x0002,
	/**	*/
	kSpellFlagLast		= 0xFFFF	
};


/**	This parameter block is used for communication between the Spelling plug-in and a client
plug-in. The client must allocate it, initialize all fields, and pass it to the Spelling plug-in
when adding a domain with SpellAddDomain. Spelling passes it back to the client when
the SCEnableProc, SCGetTextProc, and SCCompletionProc are called.
It is passed to the client’s SCGetTextProc from the Spelling plug-in when it needs to
request a text buffer from the client. After the user has completed the spell check on the
scInBuffer, this same parameter block is passed to the client’s
SCCompletionProc with the result of the spell check in scOutBuffer.
When the SCGetTextProc is called, scPageNum, scIndex and scAVDoc are filled in
by the Spelling plugin.
The SCGetTextProc should fill in scInBuffer and clear scOutBuffer if it is not
already NULL.
When the spell check is completed, SCCompletionProc is called with scOutBuffer if
the user made changes to scInBuffer.
NOTE: The spelling client is responsible for all memory allocated including the
scOutBuffer returned from the Spelling plug-in.
The client is responsible for all parameters except where indicated.
*/
typedef struct SpellCheckParam		SpellCheckParam;
/**	*/
typedef SpellCheckParam *			SpellCheckParamPtr;


/* ---- Spell Check CallBack Procs ---- */
/** */
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCInitProc)(void* clientData);
/**	*/
typedef ACCBPROTO1 void		(ACCBPROTO2 *SCTermProc)(void* clientData);

/**
	Called by the Spelling plug-in to determine whether the 
	Spelling menu items and toolbar button should be enabled. 
	An SCEnableProc must be provided for each domain a plug-in 
	registers with SpellAddDomain. 
	@param scp The Spelling plug-in passes this SpellCheckParam 
	parameter block (which the client set up in SpellAddDomain) 
	to the SCEnableProc after the spell check is complete. The 
	result of the spell check is in scOutBuffer.
	@return Should return true if spell checking can be performed on 
	the scName domain. 
	@see SCCompletionProc 
	@see SCGetTextProc 
	@see SpellAddDomain 
	@see SpellRemoveDomain 
*/
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCEnableProc)(SpellCheckParamPtr scp);

/**
	This procedure is called by Spelling to request a text buffer 
	to be checked by the user in the Spelling Dialog. 
	@param scp The Spelling plug-in passes this SpellCheckParam 
	parameter block, which the client set up in SpellAddDomain, 
	to the SCGetTextProc to request a text buffer. The Spelling 
	plug-in will fill in the scAVDoc, scPageNum, and scIndex 
	members. SCGetTextProc should return a text buffer in scInBuffer.
	@return This callback should pass back a text buffer to be checked 
	in the scInBuffer member of scp, and return true. Spelling 
	will call the client's SCCompletionProc after the user has 
	processed this buffer. 

	If there is no more data to be checked on this page, SCGetTextProc 
	should return false and set scInBuffer to NULL. 
	@see SCCompletionProc 
	@see SCEnableProc 
	@see SpellAddDomain 
	@see SpellRemoveDomain 
*/
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCGetTextProc)(SpellCheckParamPtr scp);

/**
	An SCCompletionProc must be provided for each domain a plug-in 
	registers with SpellAddDomain. It is called by the Spelling 
	plug-in when the user has completed the spell check. 
	@param scp The SpellCheckParam parameter block (which 
	the client set up in SpellAddDomain) passed to the client 
	by the Spelling plug-in after the spell check of scInBuffer 
	is complete. This procedure will be called with scOutBuffer 
	filled in by the Spelling plug-in if the user made changes 
	to scInBuffer. (scOutBuffer could be NULL).
	@return true if the changes were successfully committed. 

	NOTE: In the current version of the Spelling plug-in, this 
	value is ignored. 
	@see SCEnableProc 
	@see SCGetTextProc 
	@see SpellAddDomain 
	@see SpellRemoveDomain 
*/
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCCompletionProc)(SpellCheckParamPtr scp);

/** Called by the Spelling plug-in each time scOutBufferAStext is about to change.
	An SCChangeProc is optional.
	@param scp IN The SpellCheckParam parameter block (which the client set up in
	SpellAddDomain) passed to the client by the Spelling plug-in when
	the user makes a change to the scInBufferASText.  When called, scOutBufferAStext
	is about to be changed and scChangeStart and scChangeEnd are
	set to the the character offsets of the change from the start of scOutBufferAStext,
	and scChangeText is the new text that will replace the current text in the range.
	@return TBD
	@exception None
	@see	SCEnableProc
	@see	SCGetTextProc
	@see	SpellAddDomain
	@see	SpellRemoveDomain
	@see	SCCompletionProc
*/
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCChangeProc)(SpellCheckParamPtr scp);

/**	Call back proc for SpellCheckRTF.
	This proc is called each time the user changes something in the original input buffer.
	This allows the caller of SpellCheckRTF to track text changes to a rich text buffer and
	overlay the plain text change onto the rich text spans to perserve formatting.

	@param	vReserved IN	- reserved for future use.
	@param	astNewText IN	- the new plain text of the change
	@param	nStartIndex IN	- character offset from the start of the current plain text buffer of the change start
	@param	nEndIndex IN	- character offset from the start of the current plain text buffer of the change end
	@param	vData IN		- client data
	@return true to continue the dialog with user, false to terminate
	@exception None
	@see	SpellCheckRTF
*/
typedef ACCBPROTO1 ASBool	(ACCBPROTO2 *SCRTFChangeProc)(void* vReserved, ASConstText astNewText, ASInt32 nStartIndex, ASInt32 nEndIndex, void *vData);

#define kSCparam_VERSION_1			0x0001
#define kSCparam_VERSION_2			0x0002			/* Acrobat 6 and earlier */
#define kSCparam_VERSION_3			0x0003			/* Acrobat 7: scMenuText added */
#define kSCparam_LATEST_VERSION		kSCparam_VERSION_3

/** Spell Check parameters
	This parameter block is used for communication between the Spelling plug-in and a
	client plug-in. The client must allocate it, initialize all fields, and pass it to the Spelling
	plug-in when adding a domain with SpellAddDomain. Spelling passes it back to the
	client when the SCEnableProc, SCGetTextProc, and SCCompletionProc are
	called.
	It is passed to the clientís SCGetTextProc from the Spelling plug-in when it needs to
	request a text buffer from the client. After the user has completed the spell check on
	the scInBuffer, this same parameter block is passed to the clientís
	SCCompletionProc with the result of the spell check in scOutBuffer.
	When the SCGetTextProc is called, scPageNum, scIndex and scAVDoc are filled
	in by the Spelling plugin.
	The SCGetTextProc should fill in scInBuffer and clear scOutBuffer if it is not
	already NULL.
	When the spell check is completed, SCCompletionProc is called with scOutBuffer
	if the user made changes to scInBuffer.
	NOTE: The spelling client is responsible for all memory allocated including the
	scOutBuffer returned from the Spelling plug-in.
	The client is responsible for all parameters except where indicated.
*/
struct SpellCheckParam {
	/** Version number of this structure (kSCparam_LATEST_VERSION). */
	ASInt16 			scVersion;				
	/** Domain control flags; see SpellDomainFlags.  Should normally be set to kSpellFlagAllDomain. */
	ASInt16 			scFlags;				
	/** This name will be returned by SpellDomainNames. */
	char 				scName[kSpellMaxName];	

	/** */
	SCEnableProc		scEnableProc;			

	/** */
	SCGetTextProc		scGetTextProc;		
	/** */
	SCCompletionProc	scCompletionProc;		

	/** (Passed by Spelling plug-in) The active AVDoc during this Spell Check session. */
	AVDoc				scAVDoc;				
	/** (Passed by Spelling plug-in)PDDoc page number of scAVDoc, zero based. */
	ASInt32 			scPageNum;				
	/** (Passed by Spelling plug-in) The index of this domain item, zero based.
		Spelling plug-in will set scIndex to zero to start/restart and request the first text buffer from
		this domain on scPageNum.
		Spelling plug-in will increment scIndex after each call to scGetTextProc.
		The client can increment scIndex if desired to track non-sequential domain items. */
	ASInt32 			scIndex;				
	/** The text buffer from client to Spelling plug-in when scGetTextProc is called by the Spelling plug-in */
	char * 				scInBuffer;				
	/** (Passed by Spelling plug-in) The buffer returned from the
		Spelling plug-in to scCompletionProc.
		NOTE: The client must release this memory. 
	*/
	char * 				scOutBuffer;			
	/** Can be used by the client to store private state data. The
		client is responsible for allocating memory where necessary. 
	*/
	void *				scClientData;			

/* new with version kSCparam_VERSION_2... */
	/** The text buffer from client to Spelling plug-in when scGetTextProc is called by the Spelling plug-in. (client must release this memory) */
	ASText				scInBufferASText;		
	/** Returned buffer from Spelling PI to scCompletionProc. (this memory is owned by the Spelling plug-in, do not free it) */
	ASConstText			scOutBufferAStext;		
	/** This optional callback will be invoked each time scOutBufferAStext is about to change. */
	SCChangeProc		scChangeProc;			
	/** New text that will replace the text from scChangeStart to scChangeEnd.  (this memory is owned by the Spelling plug-in, do not free it) */
	ASConstText			scChangeText;			
	/** Character offset of the start of the change when scChangeProc is called */
	ASInt32				scChangeStart;			
	/** Character offset of the end of the change when scChangeProc is called */
	ASInt32				scChangeEnd;

	/*new with version kSCparam_VERSION_3... */
	/** Menu title in case this is the only enabled domain.  e.g.:  "In Form Fields..." or "In Comments..." */
	ASConstText			scMenuTitle;
};


/* ======================================= */
/* ======== Spelling HFT services ======== */
/* ======================================= */

/* =============================== */
/* === Spelling check services === */
/* =============================== */

/**
	Calls the Spelling plug-in to have it check a text buffer 
	and interact with the user. This service will display the 
	Spell Check dialog. 
	@param avd The current active document. 
	@param textBuffer The input text buffer to be checked 
	for spelling. 
	@param dialogResult (Filled by the method) See enum SpellDialogResult 
	for possible dialog results. If dialogResult is kSpellDone, 
	the client should terminate any domain search loops. 
	@param bReserved Reserved for future use. 
	@param dictionaryArray Optional array of dictionary names 
	to be searched. When provided, this dictionary list overrides 
	the default dictionary search list for this document. 
	@param dictionaryCount Count of dictionary names in dictionaryArray.
	@return Anew text buffer if the user made any changes to the input 
	text buffer, otherwise NULL. 
	@see SpellCheckRTF 
	@see SpellCheckText 
	@see SpellCheckWord 
*/
typedef ACCBPROTO1 char* (ACCBPROTO2 *SpellCheck_SELPROTO)
		(AVDoc avd, const char* textBuffer, ASInt16* dialogResult, ASBool bReserved, char** dictionaryArray, ASInt32 dictionaryCount);

/**
	Calls the Spelling plug-in to have it check a text object 
	and receive a callback for each change as the user interacts 
	with the Spell Check dialog. This service will display the 
	Spell Check dialog. 
	@param avd The current active document. 
	@param vReserved Reserved for future use. 
	@param astPlainText The text object to be checked for 
	spelling. 
	@param dictionaryArray Optional array of dictionary names 
	to be searched. When provided, this dictionary list overrides 
	the default dictionary search list for this document. 
	@param dictionaryCount Count of dictionary names in dictionaryArray. 
	
	@param pRTFchangeProc The callback procedure to call with 
	each user change as it occurs. 
	@param vClientData A pointer to data to pass to the callback.
	@return A SpellDialogResult value. 
	@see SpellCheck 
	@see SpellCheckText 
	@see SpellCheckWord 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *SpellCheckRTF_SELPROTO)
		(AVDoc avd, void* vReserved, ASText astPlainText, char** dictionaryArray, ASInt32 dictionaryCount, SCRTFChangeProc pRTFchangeProc, void* vClientData);

/**
	Checks the spelling of the words in a text buffer starting 
	at startOffset and ending at endOffset. 

	NOTE: It does not display the Spell Check dialog. Typically 
	you would call this function when you want to silently check 
	the spelling of a text buffer. AcroForms uses this call 
	to underline unknown words in a form text field. 
	@param avd The document to check. 
	@param textBuffer Text buffer to be checked for spelling. 
	
	@param startOffset Set this value to the starting offset 
	(zero based) in textBuffer where Spelling should start searching 
	for misspelled words. (Pass zero to check all text in the 
	buffer.) The method sets this value to the offset of the 
	first misspelled word found. 
	@param endOffset Set this value to the ending offset (zero 
	based) in textBuffer where Spelling should stop searching 
	for misspelled words. (Pass the length of textBuffer to 
	check to the end of the buffer.) The method sets this value 
	to the offset of the end of the first misspelled word found. 
	
	@param dictionaryArray Optional array of dictionary names 
	to be searched. When provided, this dictionary list overrides 
	the default dictionary search list for the current active 
	document. 
	@param dictionaryCount Count of dictionary names in dictionaryArray
	@return Returns true if all words in the buffer were in the current 
	dictionaries. 
	Returns false if a misspelled word was found and returns 
	the startOffset and endOffset of the first misspelled word 
	found in the text buffer. 
	@see SpellCheck 
	@see SpellCheckRTF 
	@see SpellCheckWord
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCheckText_SELPROTO)
		(AVDoc avd, const char* textBuffer, ASUns32* startOffset, ASUns32* endOffset, char** dictionaryArray, ASInt32 dictionaryCount);

/**
	Calls the Spelling plug-in to have it check a word. 

	NOTE: It does not display the Spell Check dialog. Typically 
	you would call this function when you want to silently check 
	the spelling of a word. 
	@param avd The current active document. 
	@param cWord A string containing the word to be checked 
	for spelling. 
	@param dictionaryArray Optional array of dictionary names 
	to be searched. When provided, this dictionary list overrides 
	the default dictionary search list for the current active 
	document. 
	@param dictionaryCount Count of dictionary names in dictionaryArray 
	
	@param alternativeArrayPtr (Filled by the method) Optional 
	pointer for an array of alternative spellings of an incorrect 
	word if any. NOTE: The caller is responsible for freeing 
	this memory. 
	@param alternativeCount Count of alternative words in 
	alternativeArray
	@return true if the word in the buffer is correct, false if the 
	word is unknown. 
	@see SpellCheck 
	@see SpellCheckRTF 
	@see SpellCheckText 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCheckWord_SELPROTO)
		(AVDoc avd, const char* cWord, char** dictionaryArray, ASInt32 dictionaryCount, char*** alternativeArrayPtr, ASInt32 *alternativeCount);

/**
	Calls the Spelling plug-in to count the words in a text 
	buffer that are contained in each of a set of dictionaries. 
	Typically you would call this function to attempt to discover 
	the language of a text buffer. 

	NOTE: It does not display the Spell Check dialog. 
	@param textBuffer The text buffer. 
	@param dictionaryCount Count of dictionary names in dictionaryArray 
	and counters in counterArray 
	@param dictionaryArray Array of dictionary names to be 
	searched 
	@param counterArray An array of counters. For each dictionary 
	in dictionaryArray, the Spelling plug-in increments the 
	corresponding counter in this array when a word from the 
	text buffer is found in that dictionary. NOTE: All four of these parameters are required. 
	@return The index of the dictionary with the highest correct count; 
	if more than one dictionary has the highest correct count, 
	the index of the first one encountered. -1 on error. 
	@see SpellCheckWord 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *SpellCountKnownWords_SELPROTO)
		(const char* textBuffer, ASInt32 dictionaryCount, char** dictionaryArray, ASInt32* counterArray);

/* ======================================= */
/* === Spelling client domain handling === */
/* ======================================= */

/**
	Returns an array of the current domain names. 
	@param domainArrayPtr (Filled by the method) An array 
	of the domain names, returned by the Spelling plug-in NOTE: 
	The caller is responsible for freeing this memory. 
	@param domainCount (Filled by the method) The count of 
	names in the array, returned by the Spelling plug-in.
	@return true if successful; domain names if any are listed in the 
	output domainArray. 
	@see SpellAddDomain 
	@see SpellRemoveDomain 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellDomainNames_SELPROTO)(char*** domainArrayPtr, ASInt32 *domainCount);

/**
	Adds a spelling domain (search scope) to the Spell Check 
	dialog. This should be called during plug-in initialization 
	for each domain that Spelling should iterate through when 
	the user invokes the Spell Check dialog. 
	@param scp A pointer to a parameter block. See SpellCheckParam 
	for details.
	@return true if the domain name was successfully added, false otherwise. 
	
	@see SpellRemoveDomain 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellAddDomain_SELPROTO)(SpellCheckParamPtr scp);

/**
	Removes a spelling domain (search scope) from the Spell 
	Check dialog that was added with the SpellAddDomain call. 
	This should be called during plug-in termination for each 
	domain that was added by a call to SpellAddDomain. After 
	calling the function, the client should free all memory 
	associated with the SpellCheckParam block (scInBuffer, scOutBuffer, 
	and scClientData) and then free the SpellCheckParam block. 
	
	@param scp A pointer to a parameter block. See SpellCheckParam 
	for details.
	@return true if successful; domain name was removed; false if the 
	domain was already removed or never added. 
	@see SpellAddDomain 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellRemoveDomain_SELPROTO)(SpellCheckParamPtr scp);

/* ==================================== */
/* === Spelling dictionary handling === */
/* ==================================== */

/**
	Returns an array of the currently available dictionary names. 
	
	@param dictionaryArrayPtr (Filled by the method) A pointer 
	to an array of the dictionary names allocated and returned 
	by the Spelling plug-in. NOTE: The caller is responsible 
	for freeing this memory. 
	@param dictionaryCount (Filled by the method) A pointer 
	to the count of names in the array, returned by the Spelling 
	plug-in.
	@return true if successful; dictionary names if any are listed in 
	the output dictionaryArray. 
	@see SpellAddDictionary 
	@see SpellRemoveDictionary 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellDictionaryNames_SELPROTO)(char*** dictionaryArrayPtr, ASInt32 *dictionaryCount);

/**
	Adds a dictionary to the list of available dictionaries. 
	
	@param cName The dictionary name to be used in the Spelling 
	Preference Panel. It can be used as one of the dictionaryArray 
	elements in the SpellCheck, SpellCheckText, SpellCheckWord 
	and SpellCountKnownWords methods. It can be returned as 
	one of the dictionaryArray elements in the SpellDictionaryNames, 
	SpellGetDocDictionaryOrder, SpellSetDocDictionaryOrder and 
	SpellUserDictionaryOrder methods. 
	@param cFile Device-independent path of the dictionary 
	files. A dictionary actually consists of four files: DDDxxxxx.hyp, 
	DDDxxxxx.lex, DDDxxxxx.clx, and DDDxxxxx.env. (xxxxx may 
	be any value you like.) The cFile parameter must be the 
	device independent path of the .hyp file. For example "/c/temp/testdict/TST.hyp". 
	Spelling will look in the parent directory of the TST.hyp 
	file for the other three files. All four file names must 
	start with the same unique 3 characters to associate them 
	with each other, and they must end with the dot-three extensions 
	listed above, even on a Macintosh. 
	@param bShow If true, Spelling will combine the cName 
	parameter with "User: " and show that name in all lists 
	and dialogs. For example if cName is "Test", Spelling will 
	add "User: Test" to all lists and dialogs. When bShow is 
	false, Spelling will not show this custom dictionary in 
	any lists or dialogs.
	@return true if successful; the dictionary is now available for 
	use. 
	@see SpellDictionaryNames 
	@see SpellGetDocDictionaryOrder 
	@see SpellRemoveDictionary 
	@see SpellSetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellAddDictionary_SELPROTO)(char* cName, char *cFile, ASBool bShow);
/**
	Removes a user dictionary that was added via SpellAddDictionary. 
	
	@param cName The dictionary name; must be the same name 
	that was passed to SpellAddDictionary.
	@return true if successful; the dictionary is no longer available 
	for use. 
	@see SpellAddDictionary 
	Removes a spelling domain (search scope) from the Spell 
	Check dialog that was added with the SpellAddDomain call. 
	This should be called during plug-in termination for each 
	domain that was added by a call to SpellAddDomain. After 
	calling the function, the client should free all memory 
	associated with the SpellCheckParam block (scInBuffer, scOutBuffer, 
	and scClientData) and then free the SpellCheckParam block. 
	@return true if successful; domain name was removed; false if the 
	domain was already removed or never added. 
	@see SpellAddDomain 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellRemoveDictionary_SELPROTO)(char* cName);

/* =========================================== */
/* === Spelling custom dictionary handling === */
/* =========================================== */

/**
	Creates a new custom user dictionary and adds it to the 
	list of available dictionaries. You can add words to the 
	new dictionary using SpellAddWord. When it is complete, 
	you can export it using SpellCustomDictionaryExport. 

	This dictionary can be passed or returned as one of the 
	dictionaryArray elements in Spelling functions. 

	A PDF Form can add a custom dictionary to its dictionary 
	search order by opening it with SpellCustomDictionaryOpen. 
	
	@param astName The dictionary name to be used in the Spelling 
	Preference Panel, as an ASText object.
	@param astLanguage Language of the dictionary.
	@param bShow When true, Spelling combines astName with 
	"User: " and shows that name in all lists and menus. For 
	example if astName is "Test", Spelling adds "User: Test" 
	to all lists and menus. When false, Spelling does not show 
	this custom dictionary in any lists or menus.
	@return true if successful; the dictionary is now available for 
	use. 
	@see SpellCustomDictionaryClose 
	@see SpellCustomDictionaryDelete 
	@see SpellCustomDictionaryExport 
	@see SpellCustomDictionaryOpen 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCustomDictionaryCreate_SELPROTO)(ASText astName, ASText astLanguage, ASBool bShow);

/**
	Opens a custom user dictionary and adds it to the dictionary 
	search order. Generally used by a PDF Form. 

	This dictionary can be passed or returned as one of the 
	dictionaryArray elements in Spelling functions. 
	@param astName The dictionary name to be used in the Spelling 
	Preference Panel, as an ASText object. 
	@param filePath The dictionary folder path, as an ASPathName 
	object. If the user does not have read/write permission 
	in this folder, the method fails. 
	@param bShow When true, Spelling combines astName with 
	"User: " and shows that name in all lists and menus. For 
	example if astName is "Test", Spelling adds "User: Test" 
	to all lists and menus. When false, Spelling does not show 
	this custom dictionary in any lists or menus.
	@return true if successful; the dictionary is now available for 
	use. 
	@see SpellCustomDictionaryClose 
	@see SpellCustomDictionaryCreate 
	@see SpellCustomDictionaryDelete 
	@see SpellCustomDictionaryExport 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCustomDictionaryOpen_SELPROTO)(ASText astName, ASPathName filePath, ASBool bShow);

/**
	Removes a custom user dictionary from the dictionary search 
	order, but does not delete the dictionary file. Generally 
	used by a PDF Form. 
	@param astName The dictionary name as an ASText object; 
	must be the same name that was passed to SpellCustomDictionaryCreate 
	or SpellCustomDictionaryOpen.
	@return true if successful; the dictionary is no longer available 
	for use. 
	@see SpellCustomDictionaryCreate 
	@see SpellCustomDictionaryDelete 
	@see SpellCustomDictionaryExport 
	@see SpellCustomDictionaryOpen 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCustomDictionaryClose_SELPROTO)(ASText astName);

/**
	Deletes a custom user dictionary object, and deletes its 
	file from disk. 
	@param astName The dictionary name as an ASText object; 
	must be the same name that was passed to the creation function.
	@return true if successful; the dictionary is no longer available 
	for use. 

	false on failure; the method fails if the user does not 
	have read/write permission for the dictionary file. 
	@see SpellCustomDictionaryClose 
	@see SpellCustomDictionaryCreate 
	@see SpellCustomDictionaryExport 
	@see SpellCustomDictionaryOpen 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCustomDictionaryDelete_SELPROTO)(ASText astName);

/**
	Exports an open custom user dictionary to the specified 
	folder. 
	@param astName The dictionary name, as an ASText object. 
	This becomes the file name of the exported dictionary file. 
	
	@param folderPath The folder path where the dictionary 
	file will be created, as an ASPathName object. If the user 
	does not have read/write permission in this folder, the 
	method fails. 
	@param reserved Reserved for future use.
	@return true if successful; the dictionary has been exported. 
	@see SpellCustomDictionaryClose 
	@see SpellCustomDictionaryCreate 
	@see SpellCustomDictionaryDelete 
	@see SpellCustomDictionaryOpen 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellCustomDictionaryExport_SELPROTO)(ASText astName, ASPathName folderPath, void* reserved);

/* ========================== */
/* === Spelling Languages === */
/* ========================== */

/**
	Returns an array of ISO 639-2 and 3166 language codes that 
	identify the currently available dictionaries. 
	@param languageArrayPtr (Filled by the method) Spelling 
	allocates and returns a char* array of the language codes. 
	The caller is responsible for freeing the array and its 
	elements. 
	@param languageCount (Filled by the method) A pointer 
	to the number of elements in the languageArrayPtr array.
	@return true if successful. 
	@see SpellGetDocLanguageOrder 
	@see SpellSetDocLanguageOrder 
	@see SpellUserLanguageOrder 
	@see SpellGetDocDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellLanguages_SELPROTO)(char*** languageArrayPtr, ASInt32 *languageCount);

/**
	Returns the user's dictionary search order, if any, as an 
	array of ISO 639-2 and 3166 language codes, allowing you 
	to identify a dictionary by language rather than by name. 
	

	This is the order defined by the user in the Spelling Preferences 
	panel. Spelling searches dictionaries in the document's 
	order first, followed by the user's order. 
	@param languageArrayPtr (Filled by the method) An array 
	of the language codes, allocated and returned by the Spelling 
	plug-in. The order in the list is the dictionary search 
	order for this document. NOTE: The caller is responsible 
	for freeing this memory. 
	@param languageCount (Filled by the method) Spelling returns 
	the count of language codes in the array
	@return true if successful. 
	@see SpellGetDocLanguageOrder 
	@see SpellLanguages 
	@see SpellSetDocLanguageOrder 
	@see SpellGetDocDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellUserLanguageOrder_SELPROTO)(char*** languageArrayPtr, ASInt32 *languageCount);

/**
	Returns the document's dictionary search order, if any, 
	as an array of ISO 639-2 and 3166 language codes, allowing 
	you to identify a dictionary by language rather than by 
	name. 

	If this array is NULL, Spelling uses the order defined by 
	the user in the Spelling Preferences panel. Spelling searches 
	dictionaries in the document's order first, followed by 
	the user's order. 
	@param avd The document. 
	@param languageArrayPtr (Filled by the method) An array 
	of the language codes, allocated and returned by the Spelling 
	plug-in. The order in the list is the dictionary search 
	order for this document. NOTE: The caller is responsible 
	for freeing this memory. 
	@param languageCount (Filled by the method) The spelling 
	plugin will return the count of language codes in the array
	@return true if successful. 
	@see SpellLanguages 
	@see SpellSetDocLanguageOrder 
	@see SpellUserLanguageOrder 
	@see SpellGetDocDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellGetDocLanguageOrder_SELPROTO)(AVDoc avd, char*** languageArrayPtr, ASInt32 *languageCount);

/**
	Sets the document's dictionary search order from an array 
	of ISO 639-2 and 3166 language codes, allowing you to identify 
	a dictionary by language rather than by name. Spelling searches 
	dictionaries in the document's order first, followed by 
	the user's order. 
	@param avd The document. 
	@param languageArray A char* array of ordered language 
	codes for dictionaries to be searched before searching the 
	list specified by the user in the Spelling Preferences Panel.
	@param languageCount The number of language codes in the 
	array. Pass zero to clear the document's dictionary order 
	list.
	@return true if successful. 
	@see SpellGetDocLanguageOrder 
	@see SpellLanguages 
	@see SpellUserLanguageOrder 
	@see SpellGetDocDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellSetDocLanguageOrder_SELPROTO)(AVDoc avd, char** languageArray, ASInt32 languageCount);

/* =================================== */
/* === Spelling user word handling === */
/* =================================== */

/**
	Returns an array of words from the user's "is a word" dictionary, 
	or from the user's "not a word" dictionary. 
	@param bAdded If true, the list of added words is returned. 
	if false, the list of removed words is returned. 
	@param wordArrayPtr (Filled by the method) An array of 
	the added/removed words, allocated and returned by the Spelling 
	plug-in. NOTE: The caller is responsible for freeing this 
	memory. 
	@param wordCount (Filled by the method) The count of words 
	in the array, returned by the Spelling plug-in.
	@return true if successful; the user words if any are listed in 
	the output wordArray. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellUserWords_SELPROTO)(ASBool bAdded, char*** wordArrayPtr, ASInt32 *wordCount);

/**
	Adds a word to the user's dictionary. 
	@param cWord The word to be added to the user's dictionary. 
	NOTE: Internally the Spelling plug-in will scan the user 
	"Not-A-Word" dictionary and remove the word if it is listed 
	there. Otherwise, the word is added to the user dictionary.
	@return true if successful; the word has been added. 
	@see SpellRemoveWord 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellAddWord_SELPROTO)(char *cWord);

/**
	Removes a word from the user's dictionary. 
	@param cWord The word to be removed from the user's dictionary. 
	NOTE: Internally the Spelling plug-in will scan the user 
	dictionary and remove the previously added word if it is 
	there. Otherwise the word will be added to the user's "Not-A-Word" 
	dictionary.
	@return true if successful; the word has been removed. 
	@see SpellAddWord 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellRemoveWord_SELPROTO)(char *cWord);

/**
	Ignore all occurrences of a word in a document when spell-checking. 
	
	@param avd The document in which to ignore the word. 
	@param cWord The word to ignore. A NULL-terminated string 
	as PDText, which is either a big-endian Unicode string prepended 
	with the bytes 0xFE 0xFF, or a string with PDFDocEncoding.
	@return true if successful; the word will be ignored. 
	@see SpellAddWord 
	@see SpellRemoveWord 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellIgnoreAll_SELPROTO)(AVDoc avd, char *cWord);

/**
	Scans a text buffer and returns the next word. 
	@param inBuffer The text buffer. 
	@param nStart (Filled by the method) On input, the character 
	offset in the buffer at which to start the search. On return, 
	the offset to the start of the word, if found. 
	@param nEnd (Filled by the method) On input, the character 
	offset at which to terminate the scan, or zero to scan to 
	the end of the text buffer. On return, the offset to the 
	character past the last character of the word found. To 
	scan for the next word, set nStart to this new offset. 
	@param bFilter If true, apply standard ignore filters 
	to the search, ignoring the following:
	- single character words
	- long words (48 characters max)
	- words that have digits
	- all UPPERCASE
	- roman numerals
	- words that have non-roman unicode (CJK) characters
	@return The next word as an ASText object. 
	@see SpellAddWord 
	@see SpellRemoveWord 
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *SpellGetNextWord_SELPROTO)(ASConstText inBuffer, ASInt32* nStart, ASInt32* nEnd, ASBool bFilter);

/* ======================================== */
/* === Spelling dictionary search order === */
/* ======================================== */

/**
	Returns the user's dictionary search order from the Spelling 
	Preference Panel. 
	@param dictionaryArrayPtr (Filled by the method) An array 
	of dictionary names, allocated and returned by the Spelling 
	plug-in. The array is ordered by the dictionary search order 
	from the Spelling Preferences panel. NOTE: The caller is 
	responsible for freeing this memory. 
	@param dictionaryCount (Filled by the method) The count 
	of names in the array, returned by the Spelling plug-in.
	@return true if successful. 
	@see SpellGetDocDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellGetDocLanguageOrder 
	@see SpellLanguages 
	@see SpellSetDocLanguageOrder 
	@see SpellUserLanguageOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellUserDictionaryOrder_SELPROTO)(char*** dictionaryArrayPtr, ASInt32 *dictionaryCount);

/**
	Returns the document's dictionary search order, if any. 
	If this array is NULL, Spelling will use the order defined 
	by the user in the Spelling Preferences panel. Spelling 
	will search dictionaries in the document order first followed 
	by the user's order. 
	@param avd The document. 
	@param dictionaryArrayPtr (Filled by the method) An array 
	of the dictionary names, allocated and returned by the Spelling 
	plug-in. The order in the list is the dictionary search 
	order for this document. NOTE: The caller is responsible 
	for freeing this memory. 
	@param dictionaryCount (Filled by the method) The spelling 
	plugin will return the count of names in the array
	@return true if successful; dictionary search order names are listed 
	in the output dictionaryArray. 
	@see SpellUserDictionaryOrder 
	@see SpellSetDocDictionaryOrder 
	@see SpellGetDocLanguageOrder 
	@see SpellLanguages 
	@see SpellSetDocLanguageOrder 
	@see SpellUserLanguageOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellGetDocDictionaryOrder_SELPROTO)(AVDoc avd, char*** dictionaryArrayPtr, ASInt32 *dictionaryCount);

/**
	Sets the document's dictionary search order. If this array 
	is NULL, Spelling uses the order defined by the user in 
	the Spelling Preferences panel. Spelling searches dictionaries 
	in the document order first, followed by the user's order. 
	
	@param avd The document. 
	@param dictionaryArray An array of ordered dictionary 
	names to be searched before searching the list specified 
	by the user in Spelling Preferences Panel. Use SpellDictionaryNames 
	to obtain a list of the currently available dictionaries. 
	
	@param dictionaryCount The number of dictionaries in the 
	dictionaryArray. Pass zero to clear the document order list.
	@return true if successful; the dictionary search order for this 
	document has been set. 
	@see SpellGetDocDictionaryOrder 
	@see SpellUserDictionaryOrder 
	@see SpellGetDocLanguageOrder 
	@see SpellLanguages 
	@see SpellSetDocLanguageOrder 
	@see SpellUserLanguageOrder 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *SpellSetDocDictionaryOrder_SELPROTO)(AVDoc avd, char** dictionaryArray, ASInt32 dictionaryCount);

/* ================================== */
/* === Hyphenation check services === */
/* ================================== */

/** types of hyphenations */
enum {
	/**		*/
	kHyphen_all = -1,				
	/**		*/
	kHyphenType_preferred = 0,
	/**		*/
	kHyphenType_normal,
	/**		*/
	kHyphenType_nonpreferred
};
typedef ASEnum16 eHyphenType;

/**
	hyphenation record.
	One of these is returned by HyphenateWord in the optional hyphenation array for each known hyphenation
*/
struct _t_HyphenationRecord {
	/** type of hyphenation: preffered, normal, or non-preferred. */
	eHyphenType			eType;					
	/** hyphen index.  The hyphen should follow this character. */
	ASInt32				nPosition;				
	/** word part before the hyphen */
	ASText				astLeft;				
	/** word part after the hyphen */
	ASText				astRight;				
};
/** */
typedef struct _t_HyphenationRecord		HyphenationRecord;
/** */
typedef HyphenationRecord *				HyphenationRecordPtr;

/**	Call the Spelling PI to have it hyphenate a word.

 	Sets the document search order.  If this array is NULL then Spelling will use the order defined by the user
 	in the Spelling Preference Panel.  Spelling will search the document order first followed by the users' order.
	   Example usage:
	   <pre>

		HyphenationRecordPtr hyphenArray = NULL;
		ASInt32 nHyphenations = SpellHyphenateWord(avd, cWord, NULL, 0, &hyphenArray);
		for (ASInt32 nIndex = 0; nIndex < nHyphenations; nIndex++) {
			HyphenationRecordPtr pHyphenRecord = hyphenArray[nIndex];
			if (pHyphenRecord->eType == kHyphenType_preferred) {
				char* cLeft = ASTextGetEncodedCopy(pHyphenRecord->astLeft, ASHostEncoding(PDGetHostEncoding()));
				char* cRight = ASTextGetEncodedCopy(pHyphenRecord->astRight, ASHostEncoding(PDGetHostEncoding()));
			}
		}	//  don't forget to free cLeft, cRight, each element of hyphenArray and the hyphenArray itself.
		</pre>

 	@param avd IN The document.
	@param astWord IN ASText containing the word to be hyphenated.
 	@param dictionaryArray IN Optional array of dictionary names to be used for hyphenation.
 						  When provided, this dictionary list overrides the default dictionary list for this document.
 						  Use SpellDictionaryNames to obtain a list of the currently available dictionaries.
 						  The dictionary names are PDText which is either a big-endian Unicode string pre-pended with the
 						  bytes 0xFE 0xFF or a string with PDFDocEncoding.  In either case the
 						  string is expected to have the appropriate NULL-termination.
 	@param dictionaryCount	IN Count of dictionary names in dictionaryArray
 	@param hyphenationArrayPtr IN Optional pointer for an an arrray of HyphenationRecord  pointers if any.
 						  NOTE: The caller is responsible for freeing this memory.
 
 	@return the count of hyphenations for this word.
 	@exception None
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *SpellHyphenateWord_SELPROTO)
		(AVDoc avd, const ASText astWord, char** dictionaryArray, ASInt32 dictionaryCount, HyphenationRecordPtr* hyphenationArrayPtr);

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif	/* _H_Speller_SEL */
