/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 SrchPrcs.h

 - Catalog of functions exported by Acrobat Search.

*********************************************************************/

/**
	Executes a specified query, using the set of indices currently 
	on the shelf. The search results are displayed in the Acrobat 
	Search plug-in's Results window. 

	NOTE: Superceded in Acrobat 6.0 by SearchExecuteQueryEx, 
	which allows a broader range of search locations. 
	@param cQuery The query. Its format is the same as what 
	a user would type into the Search query dialog, and depends 
	on the search language specified by parserID. 
	@param nParserID The search language used in the query. 
	Must be one of the following values (see SrchType.h). 
	- kParserSimple: Allows only simple phrase searches; does 
	not allow boolean searching. 
	- kParserCQL: Allows boolean 
	searches using AND, OR, and NOT, as described in the Acrobat 
	Search plug-in's online help file. 
	- kParserBPlus: The Verity BooleanPlus query language. Contact Verity for further 
	information on this language. 
	@param nSortCount The number of entries in the sortSpec 
	list. 
	@param cSortNames An array of strings, each specifying a 
	key whose value is used to sort the search results. The 
	first entry is the primary sort key, the second is the secondary 
	sort key, and so forth. Each string may be any field that 
	appears in the index, plus Score (which sorts results by 
	relevance ranking). Some common fields are Title, ModificationDate, 
	CreationDate, and Keywords. 
	@param bSortWays An array of booleans indicating sort order, 
	corresponding to the array of sortSpecs with true = ascending, 
	false = descending.
	@param nWordOptions Search options. Must be an OR of the 
	following values (see SrchType.h). 
	- kWordOptionCase: The search is case-sensitive. 
	- kWordOptionStemming: Find not 
	only the specified word, but other words that have the same 
	stem (for example, run and ran have the same stem). 
	- kWordOptionSoundsLike: Find not only the specified word, but other words that 
	sound like it.
	- kWordOptionThesaurus: Find not only the 
	specified word, but other words that have the same meaning. 
	- kWordOptionProximity: Consider the proximity of results 
	when using the AND operator to look for more than one word 
	in a document. Without this option, ANDed terms can be anywhere 
	in a document. Searching for 'red' and 'blue,' for example, 
	finds a document where 'red' is the first word on the first 
	page and where 'blue' is the last word on the last page. 
	With this option, however, ANDed terms must be within two 
	or three pages of each other to be found. Also, the closer 
	ANDed terms appear together, the higher the relevance ranking 
	of the document that contains them. 
	- kWordOptionRefine: Do not search the entire list of indices, but only the 
	documents that matched the previous search. This is used 
	to refine the results of the previous search. The manner 
	in which wordOptions is used depends on the value of overrideWordOptions. 
	
	@param bOverrideWordOptions Flag that indicates whether 
	wordOptions is ORed with the search options set in the user 
	interface, or used instead of them. If 0, wordOptions is 
	ORed with the user interface search options, and the resulting 
	value is used. If non-zero, wordOptions is used instead 
	of the user interface search options. 
	@param nMaxDocs The maximum number of documents to display 
	in the Results window. If more documents than this have 
	hits, only the first maxDocs are displayed. maxDocs must 
	be no greater than 9999.
	@return true on success, otherwise false. 
	Executes a specified query, using the given set of search 
	parameters. The search results are displayed in the Acrobat 
	Search plug-in's Results window. 

	NOTE: Supercedes SearchExecuteQuery in Acrobat 6.0. 
	@return true if successful, false otherwise. 
	@see maxDocs The maximum number of documents to display in the 
	Results window. If more documents than this have hits, only 
	the first maxDocs are displayed. maxDocs must be no greater 
	than 999.
*/
NPROC(ASInt32, SearchExecuteQuery, (char *cQuery, ASInt16 nParserID, ASUns16 nSortCount, char **cSortNames, ASBool *bSortWays, ASUns32 nWordOptions, ASBool bOverrideWordOptions, ASUns16 nMaxDocs))
/**
	Gets a list of the indices currently on the shelf. 
	@return The list of indices currently on the shelf. This value can 
	subsequently be used by other Search plug-in methods to 
	obtain information about a specific index, the number of 
	indices on the shelf, and so forth.
*/
NPROC(SearchIndexListPtr, SearchGetIndexList, (void))
/**
	Gets the number of indices currently on the shelf. 
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList.
	@return The number of indices on the shelf. 
*/
NPROC(ASUns32, SearchCountIndexList, (SearchIndexListPtr list))
/**
	Gets the nth index on the shelf. The index can be passed 
	to other Search plug-in API methods to remove it from the 
	shelf, obtain its title, and so forth. 
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList. 
	@param n The index to get. The first index on the shelf 
	is index zero.
	@return The nth index on the shelf. This value may be used in subsequent 
	calls to remove the index, obtain its title, and so forth. 
	
*/
NPROC(SearchIndexPtr, SearchGetNthIndex, (SearchIndexListPtr list, ASUns32 n))
/**
	Gets the index that has the specified path. The index must 
	already be on the shelf. The index can be passed to other 
	Search plug-in API methods to remove it from the shelf, 
	obtain its title, and so forth. 

	NOTE: Superceded by SearchGetIndexPathEx in Acrobat 6.0. 
	
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList. 
	@param fullPath A platform-dependent path to the index. 
	On the Mac OS, it is of the form MyDisk:TopFolder:BottomFolder:Strange.pdx. 
	In Windows, it is of the form C:\\LVL1\\MYFILES\\INDEX. In 
	Windows, the .PDX extension is automatically added to the 
	specified pathname.
	@return The specified index. This value may be used in subsequent 
	calls to remove the index, obtain its title, and so forth. 
*/
NPROC(SearchIndexPtr, SearchGetIndexByPath, (SearchIndexListPtr list, char *fullPath))
/**
	Adds a specified index to the shelf. 

	N OTE : Superceded in Acrobat 6.0 by SearchAddIndexEx . 
	
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList . 
	@param fullPath A platform-dependent path to the index. 
	- On the Mac OS, it is of the form MyDisk:TopFolder:BottomFolder:Strange.pdx 
	- In Windows, it is of the form C:\\LVL1\\MYFILES\\INDEX. In UNIX and Windows, the .PDX extension is automatically 
	added to the specified pathname. 
	@param flags Flags that indicate the state of the index. 
	Must be an OR of the following values (see SrchType.h). 
	- kIndexAvailableFlag: The index is available for searching. 
	Indices that are not available appear grayed out in the 
	Search plug-in's user interface.
	- kIndexSelectedFlag: The index is used for searching. Indices that are selected 
	appear with a filled in checkbox in the Search plug-in's 
	user interface.
	- kIndexPtrInvalidFlag: (Read only: cannot 
	be set) The index cannot be located; it does not exist. 
	In practice, kIndexAvailableFlag should always be set.
	@return The index that was added. This value may be used in subsequent 
	calls to remove the index, obtain its title, and so forth. 
*/
NPROC(SearchIndexPtr, SearchAddIndex, (SearchIndexListPtr list, char *fullPath, ASUns32 flags))
/**	Removes the specified index from the shelf.

    @param list IN The list of indices on the shelf, obtained using SearchGetIndexList.
	@param index IN The index to be removed. The index may be obtained using
					SearchGetIndexByPath, SearchGetNthIndex, or	SearchAddIndex.
*/
NPROC(void, SearchRemoveIndex, (SearchIndexListPtr list, SearchIndexPtr index))
/**
	Gets the flags for a specified index. 
	@param index The index whose flags are to be obtained 
	(set). The index may be obtained using SearchGetIndexByPath, 
	SearchGetNthIndex, or SearchAddIndex. 
	@return The flags returned are the actual values set, and may not 
	always be the same as the requested value. 
	Gets a list of the indices currently on the shelf. 
	@return The list of indices currently on the shelf. This value can 
	subsequently be used by other Search plug-in methods to 
	obtain information about a specific index, the number of 
	indices on the shelf, and so forth.
*/
NPROC(ASUns32, SearchGetIndexFlags, (SearchIndexPtr index))
/**
	Sets the flags for a specified index. 
	@param index The index whose flags are to be set. The 
	index may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex. 
	@param flags Flags that indicate the status of the index. 
	flags must be an OR of the values (see SrchType.h). 
	- kIndexAvailableFlag: The index is available for searching. Indices that are 
	not available appear grayed out in the Search plug-in's 
	user interface.
	- kIndexSelectedFlag: The index is used 
	for searching. Indices that are selected appear with a filled 
	in checkbox in the Search plug-in's user interface. 
	- kIndexPtrInvalidFlag: (Read only: cannot be set) The index cannot be located; 
	it does not exist. In practice, kIndexAvailableFlag should 
	always be set.
	@return The flags returned are the actual values set, and may not 
	always be the same as the requested value. 
*/
NPROC(ASUns32, SearchSetIndexFlags, (SearchIndexPtr index, ASUns32 flags))
/**
	Gets the title of a specified index. 

	NOTE: Superceded in Acrobat 6.0 by SearchGetIndexTitleEx. 
	
	@param index The index whose title is obtained. The index 
	may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex.
	@return The title of the specified index. 
*/
NPROC(char *, SearchGetIndexTitle, (SearchIndexPtr index))
/**
	Gets the platform-dependent path for a specified index. 
	

	NOTE: Superceded in Acrobat 6.0 by SearchGetIndexPathEx. 
	
	@param index The index whose path is obtained. The index 
	may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex.
	@return A platform-dependent path to the index. On the Mac OS, it 
	is of the form: MyDisk:TopFolder:BottomFolder:Strange.pdx. 
	In Windows, it is of the form C:\\LVL1\\MYFILES\\INDEX. In 
	Windows, the .PDX extension is automatically added to the 
	specified pathname. 
*/
NPROC(char *, SearchGetIndexPath, (SearchIndexPtr index))
/**
	Gets the index that has the specified path. The index must 
	already be on the shelf. The index can be passed to other 
	Search plug-in API methods to remove it from the shelf, 
	obtain its title, and so forth. 

	NOTE: Supercedes SearchGetIndexPath in Acrobat 6.0. 
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList. 
	@param fileSys The file system on which the index is found. 
	
	@param indexPath The path to the index.
	@return The specified index. This value may be used in subsequent 
	calls to remove the index, obtain its title, and so forth. 
	
*/
NPROC(SearchIndexPtr, SearchGetIndexByPathEx, (SearchIndexListPtr list, ASFileSys fileSys, ASPathName indexPath))
/**
	Adds a specified index to the shelf, using the Acrobat 6.0 
	file system and path mechanism. See the Acrobat Core API 
	Reference for information on the ASFileSys and ASPathName 
	structures. 

	NOTE: Supercedes SearchAddIndex in Acrobat 6.0. 
	@param list The list of indices on the shelf, obtained 
	using SearchGetIndexList. 
	@param fileSys The file system on which the index is found. 
	
	@param indexPath The path to the index. 
	@param flags Flags that indicate the state of the index. 
	Must be an OR of the following values (see SrchType.h). 
	- kIndexAvailableFlag: The index is available for searching. 
	Indices that are not available appear grayed out in the 
	Search plug-in's user interface. 
	- kIndexSelectedFlag: The index is used for searching. Indices that are selected 
	appear with a filled in checkbox in the Search plug-in's 
	user interface. 
	- kIndexPtrInvalidFlag: (Read only: cannot 
	be set) The index cannot be located; it does not exist. 
	In practice, kIndexAvailableFlag should always be set.
	@return The index that was added. This value may be used in subsequent 
	calls to remove the index, obtain its title, and so forth. 
	
	Returns kAddIndexFailed if the specified index could not 
	be added to the shelf. Returns kIndexExists if the index 
	is already on the shelf. 
*/
NPROC(SearchIndexPtr, SearchAddIndexEx, (SearchIndexListPtr list, ASFileSys fileSys, ASPathName indexPath, ASUns32 flags))
/**
	Gets the title of a specified index as an ASText object. 
	

	NOTE: Supercedes SearchGetIndexTitle in Acrobat 6.0. 
	@param index The index whose title is obtained. The index 
	may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex.
	@return The title of the specified index as an ASText object. 
*/
NPROC(ASText, SearchGetIndexTitleEx, (SearchIndexPtr index))
/**
	Gets the path for a specified index as an ASPathName object. 
	

	NOTE: Supercedes SearchGetIndexPath in Acrobat 6.0. 
	@param index The index whose path is obtained. The index 
	may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex.
	@return The ASPathName object. 
*/
NPROC(ASPathName, SearchGetIndexPathEx, (SearchIndexPtr index))
/**
	Gets the file system for a specified index as an ASFileSys 
	object. 
	@param index The index whose path is obtained. The index 
	may be obtained using SearchGetIndexByPath, SearchGetNthIndex, 
	or SearchAddIndex.
	@return The ASFileSys object. 	
*/
NPROC(ASFileSys, SearchGetIndexFileSys, (SearchIndexPtr index))

/**
	Tests whether the search mechanism (Search5) for previous 
	Acrobat versions (prior to 6.0) is available for the current 
	system. When Search5 is available, you can search indexes 
	from Acrobat 5.0 and earlier. 
	@return true if legacy searches are available, false otherwise.
*/
NPROC(ASBool, SearchIsLegacySearchAvailable, (void))
/**
	Executes a specified query, using the given set of search 
	parameters. The search results are displayed in the Acrobat 
	Search plug-in's Results window. 

	NOTE: Supercedes SearchExecuteQuery in Acrobat 6.0. 
	@param queryData A pointer to the structure containing 
	the search parameters. 
	@return true if successful, false otherwise. 
	@see maxDocs The maximum number of documents to display in the 
	Results window. If more documents than this have hits, only 
	the first maxDocs are displayed. maxDocs must be no greater 
	than 999.
*/
NPROC(ASBool, SearchExecuteQueryEx, (const SearchQueryDataRec *queryData))
