/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CatHFT.h

 - Catalog of functions exported by Acrobat Catalog.

*********************************************************************/

#ifndef _H_CatHFT
#define _H_CatHFT

#define CatalogHFT_NAME		"Catalog"
#define CatalogHFT_LATEST_VERSION		(0L)
#define CatalogHFT_Error 1

#define Init_CatalogHFT ASExtensionMgrGetHFT(ASAtomFromString(CatalogHFT_NAME), CatalogHFT_LATEST_VERSION)

/* Extension name : "Catalog"*/

/* Enumerate the selectors */
enum {
	CatalogBAD_SELECTOR,
	CatalogActivateSEL,
	CatalogLoadIndexSEL,
	CatalogBuildIndexSEL,
	CatalogPurgeIndexSEL,
	CatalogGiveStatusSEL,
	CatalogNUMSELECTORSminusOne
};


/**
	Returns for the Current State of Catalog.
	If Catalog is currently busy in performing some operation, it returns the relevant 
	state, which could be "CatalogBuilding", "CatalogPurging" or "CatalogWaiting".
*/
typedef enum
{
/** */
CatalogIdle = 0,
/** */
CatalogBuilding,
/** */
CatalogPurging,
/** */
CatalogWaiting
}
CatalogStatus;

/**
	Returns for the current state of the Index after a call to Purge or Build or Load an index. 
*/
typedef enum
{
/** Input path is not correct, or Catalog was not able to load the index. */
	IndexInvalid = 0,
/** Catalog was unable to save the index at the path given. */
	IndexCouldNotBeSaved,
/** The build operation was valid and was completed. */
	IndexBuildDone,
/** The purge operation was valid and was completed. */
	IndexPurgeDone,
/** The index was loaded successfully. */
	IndexLoadDone,
/** Catalog is currently busy and could not process the request. */
	CatalogBusy,
/** */
	IndexJobAdded
} IndexStatus;


#define CatalogNUMSELECTORS (CatalogNUMSELECTORSminusOne + 1)

extern HFT gCatalogHFT;
extern ASBool SetUpCatalogHFTServer(void);

/** Launch Catalog.
	If Catalog is already launched, its window is activated.
	@return None
	@exception None
	@see CatalogGiveStatus 
	@see CatalogLoadIndex
*/
typedef ACCB1 void (ACCB2 CatalogActivatePROTO)(void);
typedef CatalogActivatePROTO *CatalogActivateSELPROTO;
#define CatalogActivate	(*((CatalogActivateSELPROTO)(gCatalogHFT[CatalogActivateSEL])))

/**Opens an already existing index. If Catalog is not already 
	open, it is launched. 
	@param szIndex Full path of the index (including the .pdx 
	extension). If it is invalid, the method does nothing. 
	@return Status after the index definition dialog is closed. If Catalog 
	is busy, returns CatalogBusy. 
	@see CatalogActivate 
	@see CatalogPurgeIndex 
	@exception None
*/
typedef ACCB1 IndexStatus (ACCB2 CatalogLoadIndexPROTO)(char *szIndex);
typedef CatalogLoadIndexPROTO *CatalogLoadIndexSELPROTO;
#define CatalogLoadIndex (*((CatalogLoadIndexSELPROTO)(gCatalogHFT[CatalogLoadIndexSEL])))


/**
	Builds an index. If Catalog is not already open, it is launched. 
	
	@param szIndex Destination path of the index (including 
	the .pdx extension). 
	@return Status after the build is done. If Catalog is busy, returns 
	CatalogBusy. 
	@see CatalogActivate 
	@see CatalogLoadIndex 
	@see CatalogPurgeIndex 
*/
typedef ACCB1 IndexStatus (ACCB2 CatalogBuildIndexPROTO)(char *szIndex);
typedef CatalogBuildIndexPROTO *CatalogBuildIndexSELPROTO;
#define CatalogBuildIndex (*((CatalogBuildIndexSELPROTO)(gCatalogHFT[CatalogBuildIndexSEL])))

/** Purges an already existing index.
	If Catalog is not already open, it is launched.
	@param szIndex IN Full path of the index (including the .pdx extension). If it is invalid, the method does nothing.
	@return Status after the purge is done. If Catalog is busy, returns CatalogBusy.
	@exception None
	@see CatalogActivate 
	@see CatalogBuildIndex 
	@see CatalogPurgeIndex 
*/
typedef ACCB1 IndexStatus (ACCB2 CatalogPurgeIndexPROTO)(char *szIndex);
typedef CatalogPurgeIndexPROTO *CatalogPurgeIndexSELPROTO;
#define CatalogPurgeIndex (*((CatalogPurgeIndexSELPROTO)(gCatalogHFT[CatalogPurgeIndexSEL])))

/** Returns the current status of Catalog.
	@return The current status of Catalog.
*/
typedef ACCB1 CatalogStatus (ACCB2 CatalogGiveStatusPROTO)(void);
typedef CatalogGiveStatusPROTO *CatalogGiveStatusSELPROTO;
#define CatalogGiveStatus (*((CatalogGiveStatusSELPROTO)(gCatalogHFT[CatalogGiveStatusSEL])))


#endif

