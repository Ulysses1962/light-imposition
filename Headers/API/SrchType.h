/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 srchtype.h

*********************************************************************/

#ifndef _H_SrchType
#define _H_SrchType


#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

/** @name SearchWordOptionsFlags	
*/
/*@{*/
/** The search is case-sensitive. */
#define kWordOptionCase			0x00000001L
/** Find not only the specified word, but other words that have the same stem (for
	example, run and ran have the same stem).
*/
#define kWordOptionStemming		0x00000002L
/** Consider the proximity of
	results when using the AND operator to look for more
	than one word in a document. Without this option, ANDed
	terms can be anywhere in a document. Searching for
	"red" and "blue," for example, finds a document where
	"red" is the first word on the first page and where "blue" is
	the last word on the last page. With this option, however,
	ANDed terms must be within two or three pages of each
	other to be found. Also, the closer ANDed terms appear
	together, the higher the relevance ranking of the
	document that contains them. 
*/
#define kWordOptionProximity	0x00000010L
/** Do not search the entire list of
	indices, but only the documents that matched the
	previous search. This is used to refine the results of the
	previous search.
*/
#define kWordOptionRefine		0x00000020L
/**	*/
#define kWordOptionWholeWord	0x00000040L
/**	*/
#define kWordOptionIgnoreFH		0x00000080L

/**	*/
#define kWordOptionIgnoreDiacritics		0x00000100L

/*@}*/

/** @name SearchWordOptions
*/
/*@{*/
/** 
The client must supply all the SearchWordOptions in the scope parameter
by doing an OR (|) operation on all options where search needs to be executed.
For example, if the client needs to search on attachments and within attachment
search needs to be done on document text and XMP data, then it should be 
specified like:
	scope = kSearchDocumentText | kSearchDocumentXMP | kSearchPDFAttachments;

In most of the cases, however, the client might want to search on all the places 
possible, so it would suffice to write:
	scope = kSearchEveryWhere;

*/
typedef ASUns32 SearchWordOptions;
/** */
#define kSearchDocumentText		0x00000001L
/**		*/
#define kSearchBookmarks		0x00000002L
/**		*/
#define kSearchMarkup			0x00000004L
/**		*/
#define kSearchDocumentXMP		0x00000008L
/**		*/
#define kSearchSignatures		0x00000010L
/**		*/
#define kSearchDocInfo			0x00000020L
/**		*/
#define kSearchJPEGExif			0x00000040L
/**	
Including kSearchUserProperties option in your search
will search the object level data, which is same as that
seen by Menu item Tools -> Object Data -> Object Data Tool
in Acrobat viewer. This is NOT the metadata associated with
individual COS level objects in the PDF.
*/
#define kSearchUserProperties	0x00000080L
/**     */
#define kSearchPDFAttachments		0x00000100L
/**		*/
#define kSearchEveryWhere		ASMAXUns32
/*@}*/

/** */
typedef ASUns32 SearchScope;

/** */
typedef enum
{
	/** */
	kSearchActiveDoc,
	/** */
	kSearchFolder,
	/** */
	kSearchIndex,
	/** */
	kSearchActiveIndexes
} SearchType;

/**	Search Match Options	*/
typedef enum 
{
/**		*/	
	kMatchPhrase,
/**		*/	
	kMatchAllWords,
/**		*/	
	kMatchAnyWords,
/**		*/	
	kBooleanQuery
} SearchMatchOption;

/** */
typedef struct _t_SearchQueryDataRec {
	/** Size of the data structure. Must be set to sizeof(SearchQueryDataRec). */
	ASSize_t			size;		

	/** Text to be searched.  */
	ASText				query;		

	/** Location to search in */
	SearchType			type;
	
	/** How the "query" text should be matched in the document. */
	SearchMatchOption	match;		

	/** Search Options */
	SearchWordOptions	options;	

	/** Content in the PDFs that should be searched. 
	   Should be an OR of values specified in SearchScope. */
	SearchScope			scope;		

	/** The path of the folder or index. 
	   Required only when type parameter is kSearchFolder or kSearchIndex. */
	ASPathName			path;		

	/** The ASFileSys of the folder or index. 
	   Required only when type parameter is kSearchFolder or kSearchIndex. */
	ASFileSys			fs;			

	/** The maximum number of documents to display in the Results window. If
		more documents than this have hits, only the first maxDocs are
		displayed. maxDocs must be no greater than 999.
	*/
	ASUns16				maxDocs;	

	/** Page number to start the search; 0=first page */
	ASUns32				startPage;

	/** Range of words for proximity searches */
	ASUns32				proximityRange;
} SearchQueryDataRec;


/* opaque types for primitive types */
/** */
typedef void *SearchIndexListPtr;
/** */
typedef void *SearchIndexPtr;


/** @name SearchIndexFlags */
/*@{*/
/**	The index is available for searching.
	Indices that are not available appear grayed out in the Search
	plug-in’s user interface.	*/
#define kIndexAvailableFlag		(1L << 0)
/**	The index is used for searching.
	Indices that are selected appear with a filled in checkbox in the
	Search plug-in's user interface.	*/
#define kIndexSelectedFlag		(1L << 1)
/**	(Read only - cannot be set) The
	index cannot be located; it does not exist.
	In practice, kIndexAvailableFlag should always be set.	*/
#define kIndexPtrInvalidFlag	(1L << 31)
/*@}*/

#define kAddIndexFailed		((SearchIndexPtr)0)
#define kIndexExists		((SearchIndexPtr)-1)

/* deprecated */

/* parser types */
#define kParserSimple	0
#define kParserCQL		1
#define kParserBPlus	2

/** @name SearchWordOptionsFlags */
/*@{*/
/** Find not only the specified word, but other words that sound like it.
*/
#define kWordOptionSoundsLike	0x00000004L
/** Find not only the specified	word, but other words that have the same meaning. 
*/
#define kWordOptionThesaurus	0x00000008L
/*@}*/

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif

