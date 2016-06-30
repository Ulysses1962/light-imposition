/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PIPokes.h

 - List of prototypes for notifications. Plug-in clients that register
   for notifications have to register functions that match the prototypes
   listed below. All notification-callbacks return void.

*********************************************************************/

/* POKE(name, notifier formals, notifiee formals, notifiee actuals) */


/* AvApp-related notifications */

/** 
	The Acrobat viewer has finished initializing and is about 
	to enter its event loop. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppWillQuit 
*/
POKE(AVAppDidInitialize, (void), (void *clientData), (cell->clientData))

/**
	The Acrobat viewer is quitting. All documents have been 
	closed. To access or enumerate documents when the application 
	is quitting, replace the AVAppCanQuit method, access or 
	enumerate documents in your replacement for that procedure, 
	and return true to allow the Acrobat viewer to continue 
	quitting. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppDidInitialize 
*/
POKE(AVAppWillQuit, (void), (void *clientData), (cell->clientData))

/**
	The front-most AVDoc has changed. 

	NOTE: This notification is not broadcast for external windows, 
	such as OLE applications or PDF files being displayed in 
	Netscape. 
	@param doc The document that was brought to the front. 
	NULL if there is no front-most document (for example, the 
	previous front-most document was just closed). 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidActivate 
	@notify AVDocDidDeactivate 
	@see AVWindowBringToFront 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVAppFrontDocDidChange, (AVDoc doc), (AVDoc doc, void *clientData), (doc, cell->clientData))

/* AVDoc-related notifications */

/**
	An AVDoc will be opened from a file. 
	@param fileName The ASPathName for the file that will 
	be opened. 
	@param fileSys The file system responsible for the file 
	to open. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidOpen 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
*/
POKE(AVDocWillOpenFromFile, (ASPathName fileName, ASFileSys fileSys), (ASPathName fileName, ASFileSys fileSys, void *clientData), (fileName, fileSys, cell->clientData))

/**
	A document has been opened. 

	Calling AVDocClose within this notification is forbidden. 
	
	@param doc The document that was opened. 
	@param error Error code. error is set to 0 if no errors 
	occurred while opening the file. If an error occurred, error 
	contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocWillOpenFromFile 
	@notify AVDocWillOpenFromPDDoc 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
*/
POKE(AVDocDidOpen, (AVDoc doc, Int32 error), (AVDoc doc, Int32 error, void *clientData), (doc, error, cell->clientData))

/**
	An AVDoc has activated. At the time this notification is 
	broadcast, it is possible that the window being activated 
	has not yet been brought to the front. For this reason, 
	the AVAppFrontDocDidChange notification is often more useful. 
	

	NOTE: AVAppGetActiveDoc will not necessarily return the 
	AVDoc returned in this notification. For instance, if there 
	is an AVDoc in an external window (such as a Web browser's) 
	that becomes active, the AVDoc returned by this notification 
	will not match what AVAppGetActiveDoc returns. 

	NOTE: This notification is not broadcast for external windows, 
	such as OLE applications or PDF files being displayed in 
	Netscape. 
	@param doc The document that was activated. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidDeactivate 
	@notify AVAppFrontDocDidChange 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVDocDidActivate, (AVDoc doc), (AVDoc doc, void *clientData), (doc, cell->clientData))

/**
	A document was deactivated. 

	NOTE: This notification is not broadcast for external windows, 
	such as OLE applications or PDF files being displayed in 
	Netscape. 
	@param doc The document that was deactivated. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidActivate 
	@notify AVAppFrontDocDidChange 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVDocDidDeactivate, (AVDoc doc), (AVDoc doc, void *clientData), (doc, cell->clientData))

/**
	An AVDoc will be closed. Neither this notification nor AVDocDidClose 
	are broadcast if the user selects 'Cancel' when prompted 
	to save a modified document as it is closed. 

	NOTE: Reads made from AVDocWillClose on an in-browser document 
	will fail if the data is not already downloaded. 
	@param doc The document that will be closed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidClose AVDocClose 
*/
POKE(AVDocWillClose, (AVDoc doc),(AVDoc doc, void *clientData), (doc, cell->clientData))

/**
	A document has been closed. Although an AVDoc is passed 
	to the routine called by this notification, the document 
	has already been closed but not freed. As a result, all 
	the routine can really do is manipulate any private data 
	in the underlying PDF file at the time this notification 
	occurs. 
	@param doc The document that was closed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocWillClose AVDocClose 
*/
POKE(AVDocDidClose, (AVDoc doc),(AVDoc doc, void *clientData), (doc, cell->clientData))

/**
	A document's selection is about to be cleared. 
	@param doc The document whose selection will be cleared. 
	
	@param selType The ASAtom corresponding to the current 
	selection type. 
	@param selData Pointer to the current selection data. 
	The format and contents of selData depend on selType. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidAddToSelection 
	@notify AVDocDidClearSelection 
	@notify AVDocDidSetSelection 
	@see AVDocClearSelection 
	@see AVDocDeleteSelection 
	@see AVDocSetSelection 
*/
POKE(AVDocWillClearSelection, (AVDoc doc, ASAtom selType, void* selData), (AVDoc doc, ASAtom selType, void* selData, void *clientData), (doc, selType, selData, cell->clientData))

/**
	The document's selection has been set. 
	@param doc The document whose selection was set. 
	@param selType The ASAtom corresponding to the current 
	selection type. See Selection Types for a list of selection 
	types. 
	@param selData Pointer to the current selection data. 
	The format and contents of selData depend on selType. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidAddToSelection 
	@notify AVDocDidClearSelection 
	@notify AVDocWillClearSelection 
	@see AVDocClearSelection 
	@see AVDocDeleteSelection 
	@see AVDocSetSelection 
*/
POKE(AVDocDidSetSelection, (AVDoc doc, ASAtom selType, void* selData), (AVDoc doc, ASAtom selType, void* selData, void *clientData), (doc, selType, selData, cell->clientData))

/**
	An action is about to be performed. 
	@param doc The document containing the action that will 
	be performed. 
	@param action The action that will be performed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidPerformAction 
	@see AVDocPerformAction 
	@see The following methods broadcast this notification if the 
	document has an open action: 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVDocWillPerformAction, (AVDoc doc, PDAction action), (AVDoc doc, PDAction action, void* clientData), (doc, action, cell->clientData))

/**
	An action was performed. 
	@param doc The document containing the action that was 
	performed. 
	@param action The action that was performed. 
	@param err Error code. error is set to 0 if no errors 
	occurred while performing the action. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocWillPerformAction 
	@see AVDocPerformAction 
	@see The following methods broadcast this notification if the 
	document has an open action: 
	@see AVDocOpenFromASFileWithParamString 
	@see AVDocOpenFromFile 
	@see AVDocOpenFromASFileWithParams 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVDocDidPerformAction, (AVDoc doc, PDAction action, Int32 err), (AVDoc doc, PDAction action, Int32 err, void* clientData), (doc, action, err, cell->clientData))

/* AVPageView-related notifications */

/**
	Redrawing occurred in the page view section of the window. 
	
	@param pageView The AVPageView in which drawing occurred. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVPageViewDidChange 
	@notify AVPageViewWillDraw 
	@see AVPageViewDrawNow 
*/
POKE(AVPageViewDidDraw, (AVPageView pageView), (AVPageView pageView, void *clientData), (pageView, cell->clientData))

/**
	The page view has changed. Zero or more of the following 
	events has occurred: 
	<ul>
	<li> The page number has changed.
	<li> The zoom factor has changed.
	<li> The window has been resized.
	<li> The page has been scrolled.
	</ul>

	NOTE: If continuous scrolling is turned on (available in 
	Acrobat 3.0 or later) and more than one page is displayed 
	in the AVPageView, alternating mouse clicks in the different 
	pages displayed does not constitute a change to the AVPageView. 
	
	@param pageView The AVPageView that has changed. 
	@param how Specifies how the page view did change. how 
	is an OR of zero or more of the following (see AVExpT.h): 
	PAGEVIEW_UPDATE_SCROLL - The view has been scrolled. PAGEVIEW_UPDATE_PAGENUM 
	 -  The page number has changed. PAGEVIEW_UPDATE_PAGESIZE 
	 -  A new view has been created. PAGEVIEW_UPDATE_ZOOM - The 
	zoom has been changed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocActivePageViewDidChange 
	@notify AVPageViewDidDraw 
	@notify AVPageViewWasCreated 
	@notify AVPageViewWillDestroy 
	@see AVPageViewZoomTo 
	@see AVPageViewScrollTo 
	@see AVPageViewScrollToRect 
	@see AVPageViewGoTo 
	@see AVPageViewReadPageDown 
	@see AVPageViewReadPageUp 
	@see AVPageViewGoBack 
	@see AVPageViewGoForward 
	@see AVPageViewUseDestInfo 
	@see AVPageViewUseThisDestination
*/
POKE(AVPageViewDidChange, (AVPageView pageView, ASInt16 how), (AVPageView pageView, ASInt16 how, void *clientData), (pageView, how, cell->clientData))

/* PDDoc-related notifications */

/**
	One or more pages will be inserted. 
	@param doc The document into which pages will be inserted. 
	
	@param insertAfterThisPage Page number (in doc) after 
	which pages will be inserted. 
	@param srcDoc The document that provides the pages to 
	insert. This is NULL when a new blank page is created and 
	inserted into a document. This is NULL for a notification 
	broadcast by PDDocCreatePage. 
	@param srcFromPage The page number (in srcDoc) of the 
	first page that will be inserted. Not valid when a new blank 
	page is created and inserted into a document. This is NULL 
	for a notification broadcast by PDDocCreatePage. 
	@param srcToPage The page number (in srcDoc) of the last 
	page that will be inserted. Not valid when a new blank page 
	is created and inserted into a document. This is NULL for 
	a notification broadcast by PDDocCreatePage. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidInsertPages 
	@notify PDDocWillChangePages 
	@see PDDocCreatePage 
	@see PDDocInsertPages 
*/
POKE(PDDocWillInsertPages, \
	(PDDoc doc, Int32 insertAfterThisPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage), \
	(PDDoc doc, Int32 insertAfterThisPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, void *clientData), \
	(doc, insertAfterThisPage, srcDoc, srcFromPage, srcToPage, cell->clientData))

/**
	One or more pages have been inserted. 
	@param doc The document into which pages were inserted. 
	
	@param insertAfterThisPage Page number (in doc) after 
	which pages were inserted. 
	@param srcDoc The document that provided the pages that 
	were inserted. This is NULL when a new blank page is created 
	and inserted into a document. This is NULL for a notification 
	broadcast by PDDocCreatePage. 
	@param srcFromPage The page number (in srcDoc) of the 
	first page that was inserted. Not valid when a new blank 
	page is created and inserted into a document. This is NULL 
	for a notification broadcast by PDDocCreatePage. 
	@param srcToPage The page number (in srcDoc) of the last 
	page that was inserted. Not valid when a new blank page 
	is created and inserted into a document. This is NULL for 
	a notification broadcast by PDDocCreatePage. 
	@param error Error code. error is set to 0 if no errors 
	occurred while inserting the pages. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillInsertPages 
	@notify PDDocDidChangePages 
	@see PDDocCreatePage 
	@see PDDocInsertPages 
*/
POKE(PDDocDidInsertPages, \
	(PDDoc doc, Int32 insertAfterThisPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, Int32 error), \
	(PDDoc doc, Int32 insertAfterThisPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, Int32 error, void *clientData), \
	(doc, insertAfterThisPage, srcDoc, srcFromPage, srcToPage, error, cell->clientData))

/**
	One or more pages will be replaced. 
	@param doc The document in which pages will be replaced. 
	
	@param fromPage The page number (in doc) of the first 
	page that will be replaced. 
	@param toPage The page number (in doc) of the last page 
	that will be replaced. 
	@param srcDoc The document that provides the replacement 
	pages. 
	@param srcFromPage The page number (in srcDoc) of the first 
	replacement page. 
	@param srcToPage The page number (in srcDoc) of the last 
	replacement page. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidReplacePages 
	@notify PDDocWillChangePages 
	@see PDDocReplacePages 
*/
POKE(PDDocWillReplacePages, \
	(PDDoc doc, Int32 fromPage, Int32 toPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage), \
	(PDDoc doc, Int32 fromPage, Int32 toPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, void *clientData), \
	(doc, fromPage, toPage, srcDoc, srcFromPage, srcToPage, cell->clientData))

/**
	One or more pages have been replaced. 
	@param doc The document in which pages have been replaced. 
	
	@param fromPage The page number (in doc) of the first 
	page that was replaced. 
	@param toPage The page number (in doc) of the last page 
	that was replaced. 
	@param srcDoc The document that provided the replacement 
	pages. 
	@param srcFromPage The page number (in srcDoc) of the first 
	replacement page. 
	@param srcToPage The page number (in srcDoc) of the last 
	replacement page. 
	@param error Error code. error is set to 0 if no errors 
	occurred while replacing pages. If an error occurred, error 
	contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillReplacePages 
	@notify PDDocDidChangePages 
	@see PDDocReplacePages 
*/
POKE(PDDocDidReplacePages, \
	(PDDoc doc, Int32 fromPage, Int32 toPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, Int32 error), \
	(PDDoc doc, Int32 fromPage, Int32 toPage, PDDoc srcDoc, Int32 srcFromPage, Int32 srcToPage, Int32 error, void *clientData), \
	(doc, fromPage, toPage, srcDoc, srcFromPage, srcToPage, error, cell->clientData))


/**
	One or more pages will be moved. 
	@param doc The document in which pages will be moved. 
	
	@param moveAfterThisPage The page number after which the 
	moved pages will be placed. 
	@param fromPage The page number of the first page to move. 
	
	@param toPage The page number of the last page to move. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidMovePages 
	@notify PDDocWillChangePages 
	@see PDDocMovePage 
*/
POKE(PDDocWillMovePages, \
	(PDDoc doc, Int32 moveAfterThisPage, Int32 fromPage, Int32 toPage), \
	(PDDoc doc, Int32 moveAfterThisPage, Int32 fromPage, Int32 toPage, void *clientData), \
	(doc, moveAfterThisPage, fromPage, toPage, cell->clientData))

/**
	One or more pages were moved. 
	@param doc The document in which pages were moved. 
	@param moveAfterThisPage The page number after which the 
	moved pages were placed. 
	@param fromPage The page number of the first page that 
	was moved. 
	@param toPage The page number of the last page that was 
	moved. 
	@param error Error code. error is set to 0 if no errors 
	occurred while moving pages. If an error occurred, error 
	contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillMovePages 
	@notify PDDocDidChangePages 
	@see PDDocMovePage 
*/
POKE(PDDocDidMovePages, \
	(PDDoc doc, Int32 moveAfterThisPage, Int32 fromPage, Int32 toPage, Int32 error), \
	(PDDoc doc, Int32 moveAfterThisPage, Int32 fromPage, Int32 toPage, Int32 error, void *clientData), \
	(doc, moveAfterThisPage, fromPage, toPage, error, cell->clientData))

/**
	One or more pages will be deleted. 
	@param doc The document from which pages will be deleted. 
	
	@param fromPage The page number of the first page that 
	will be deleted. 
	@param toPage The page number of the last page that will 
	be deleted. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidDeletePages 
	@notify PDDocDidChangePages 
	@see PDDocDeletePages 
*/
POKE(PDDocWillDeletePages, (PDDoc doc, Int32 fromPage, Int32 toPage), (PDDoc doc, Int32 fromPage, Int32 toPage, void *clientData), (doc, fromPage, toPage, cell->clientData))

/**
	One or more pages were deleted. 
	@param doc The document from which pages were deleted. 
	
	@param fromPage The page number of the first page that 
	was deleted. 
	@param toPage The page number of the last page that was 
	deleted. 
	@param error Error code. error is set to 0 if no errors 
	occurred while deleting the pages. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillDeletePages 
	@notify PDDocDidChangePages 
	@see PDDocDeletePages 
*/
POKE(PDDocDidDeletePages, (PDDoc doc, Int32 fromPage, Int32 toPage, Int32 error), (PDDoc doc, Int32 fromPage, Int32 toPage, Int32 error, void *clientData), (doc, fromPage, toPage, error, cell->clientData))

/**
	Pages will be inserted, deleted, moved, or modified. 
	@param doc The document in which pages will be changed. 
	
	@param op The change that will be made. op will be one 
	of the PDOperation values. 
	@param fromPage The page number of the first page that 
	will be modified. 
	@param toPage The page number of the last page that will 
	be modified. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidChangePages 
	@see PDDocDeletePages 
	@see PDPageSetRotate 
	@see PDPageSetMediaBox 
	@see PDPageSetCropBox 
*/
POKE(PDDocWillChangePages, (PDDoc doc, PDOperation op, Int32 fromPage, Int32 toPage), (PDDoc doc, PDOperation op, Int32 fromPage, Int32 toPage, void *clientData), (doc, op, fromPage, toPage, cell->clientData))

/**
	Pages have been inserted, deleted, moved, or modified. 
	@param doc The document in which pages have been changed. 
	
	@param op The change that was made. op will be one of 
	the PDOperation values. 
	@param fromPage The page number of the first page that 
	was modified. For page insertion, this is the number of 
	the page before the first inserted page. For page deletion, 
	this is the page number of the first deleted page. 
	@param toPage The page number of the last page that was 
	modified. If broadcast by PDDocCreatePage, this is the number 
	of the newly created page. When broadcast by PDDocInsertPages, 
	toPage is the number of pages in the document post-insertion; 
	that is, it points to a page not in the document. If broadcast 
	by PDDocDeletePages, toPage is the number of pages in the 
	PDDoc prior to deletion, pointing to a page not in the document. 
	
	@param error Error code. error is set to 0 if no errors 
	occurred while changing the pages. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillChangePages 
	@see PDDocCreatePage 
	@see PDDocInsertPages 
	@see PDDocReplacePages 
	@see PDDocMovePage 
	@see PDPageAddCosResource 
	@see PDPageRemoveCosResource 
	@see PDPageAddCosContents 
	@see PDPageRemoveCosContents 
	@see PDDocDeletePages 
	@see PDPageSetRotate 
	@see PDPageSetMediaBox 
	@see PDPageSetCropBox
*/
POKE(PDDocDidChangePages, (PDDoc doc, PDOperation op, Int32 fromPage, Int32 toPage, Int32 error), (PDDoc doc, PDOperation op, Int32 fromPage, Int32 toPage, Int32 error, void *clientData), (doc, op, fromPage, toPage, error, cell->clientData))

/**
	Thumbnail images have been added or removed. In addition 
	to the expected ways in which this can occur, it can also 
	occur if pages are inserted into a file. 
	@param doc The document in which thumbnail images have 
	been changed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidChangePages 
	@notify PDDocDidDeletePages 
	@notify PDDocDidInsertPages 
	@notify PDDocDidReplacePages 
	@notify PDDocWillChangePages 
	@notify PDDocWillDeletePages 
	@notify PDDocWillInsertPages 
	@notify PDDocWillReplacePages 
	@see PDDocCreatePage 
	@see PDDocCreateThumbs 
	@see PDDocDeleteThumbs 
	@see PDDocInsertPages 
*/
POKE(PDDocDidChangeThumbs, (PDDoc doc), (PDDoc doc, void *clientData), (doc, cell->clientData))

/**
	This notification is broadcast when printing begins, before 
	any pages are printed. 

	NOTE: Page resources and contents cannot be modified reliably 
	at the time this notification is broadcast. 
	@param doc The document from which pages will be printed. 
	
	@param fromPage The page number of the first page that 
	will be printed. 
	@param toPage The page number of the last page that will 
	be printed. 
	@param psLevel When printing to a PostScript printer, 
	psLevel is either 1 or 2, representing the PostScript level 
	available on the printer. When printing to a non-PostScript 
	printer, psLevel is 0. psLevel is useful in determining 
	whether the output device is a PostScript printer. In addition, 
	when printing to a PostScript printer, psLevel is useful 
	to determine the operators that can be sent in any printing 
	code downloaded using the PDDocWillPrintDoc, PDDocWillPrintPage, 
	and PDDocDidPrintPage notifications. 
	@param binaryOK Valid only when printing to a PostScript 
	printer. Indicates whether binary data can be sent. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillPrintDoc 
	@notify PDDocWillPrintPage 
	@notify PDDocDidPrintPages 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocWillPrintPages, (PDDoc doc, Int32 fromPage, Int32 toPage, Int32 psLevel, ASBool binaryOK),
	(PDDoc doc, Int32 fromPage, Int32 toPage, Int32 psLevel, ASBool binaryOK, void *clientData),
	(doc, fromPage, toPage, psLevel, binaryOK, cell->clientData))

/**
	This notification is broadcast after printing ends. 
	@param doc The document from which pages were printed. 
	
	@param fromPage The page number of the first page that 
	was printed. 
	@param toPage The page number of the last page that was 
	printed. 
	@param error Error code. error is set to 0 if no errors 
	occurred while printing the pages. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillPrintPages 
	@notify PDDocDidPrintPage 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocDidPrintPages, (PDDoc doc, Int32 fromPage, Int32 toPage, Int32 error), (PDDoc doc, Int32 fromPage, Int32 toPage, Int32 error, void *clientData), (doc, fromPage, toPage, error, cell->clientData))

/**
	This notification is broadcast once per page that is printed, 
	before any marks are made on the page. When printing to 
	a PostScript printer, printing commands can also be sent 
	that will be placed on the page before any other marks. 
	

	NOTE: Page resources and contents cannot be modified reliably 
	at the time this notification is broadcast. 
	@param doc The document from which a page is about to 
	be printed. 
	@param page The page number of the page that is about 
	to be printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, or NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm with ASStmWrite to 
	add marks to the printed page before any other marks have 
	been made. In the 2.x Acrobat viewers, the page printing 
	sequence to a PostScript printer is:\n
	page setup (including setpagedevice) ... save\n
	... gsave ... save ... begin ... begin ... begin\n
	... PDDocWillPrintPage ... page contents ...\n
	PDDocDidPrintPage ... end ... end ... end ... restore ...\n
	restore ... showpage\n
	This sequence must not be relied on, 
	and depends on the printer driver in use. However, by the 
	time the PDDocWillPrintPage notification is broadcast, it 
	is too late to perform any setpagedevice operations. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidPrintPage 
	@notify PDDocWillPrintPages 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocWillPrintPage, (PDDoc doc, Int32 page, ASStm stm), (PDDoc doc, Int32 page, ASStm stm, void *clientData), (doc, page, stm, cell->clientData))

/**
	This notification is broadcast once per page that is printed, 
	after all marks have been made on the page. When printing 
	to a PostScript printer, printing commands can also be sent 
	that will be placed on the page after all other marks. 
	@param doc The document from which a page was printed. 
	
	@param page The page number of the page that was printed. 
	
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to the printed page after all other marks have 
	been made. See PDDocWillPrintPage for a description of the 
	sequence of operations when printing a page to a PostScript 
	printer. 
	@param error Error code. error is set to 0 if no errors 
	occurred while printing the page. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillPrintPage 
	@notify PDDocDidPrintPages 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocDidPrintPage, (PDDoc doc, Int32 page, ASStm stm, Int32 error), (PDDoc doc, Int32 page, ASStm stm, Int32 error, void *clientData), (doc, page, stm, error, cell->clientData))

/**
	A thread has been added to a document. 
	@param doc The document to which a thread was added. 
	@param thread The thread that was added. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDThreadDidChange 
	@see PDDocAddThread 
	@see PDDocInsertPages 
*/
POKE(PDDocDidAddThread, (PDDoc doc, PDThread thread), (PDDoc doc, PDThread thread, void* clientData), (doc, thread, cell->clientData))

/**
	A thread was removed from a document. 
	@param doc The document from which a thread was removed. 
	
	@param index The index of the thread that was removed. 
	Because the thread has already been removed, it is not possible 
	to access it using index. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillRemoveThread 
	@see PDDocRemoveThread 
*/
POKE(PDDocDidRemoveThread, (PDDoc doc, Int32 index), (PDDoc doc, Int32 index, void* clientData), (doc, index, cell->clientData))

/**
	A document will be saved. 

	The PDDocWillSave notification takes place just before the 
	save operation begins. At the time of a PDDocWillSave notification, 
	the current PDDoc is valid. 

	See PDDocWillSaveEx for important information on releasing 
	objects derived from the PDDoc before it is saved, and PDDocDidSave 
	for information on reacquiring objects from the PDDoc after 
	it has been saved. 
	@param doc The document that will be saved. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidSave 
	@notify PDDocWillSaveEx 
	@see PDDocSave 
	@see PDDocSaveWithParams 
*/
POKE(PDDocWillSave, (PDDoc doc), (PDDoc doc, void* clientData), (doc, cell->clientData))

/**
	A document has been saved. 

	The PDDocDidSave notification takes place just after the 
	save operation finishes. At the time of a PDDocDidSave notification, 
	a client or application can reacquire resources from the 
	PDDoc if needed. It should examine the error code err associated 
	with the save. If the save was not successful, the error 
	code is non-zero. In the case of a unsuccessful save, a 
	client or application should not attempt to do anything 
	further with this PDDoc. 

	See PDDocWillSaveEx for important information on releasing 
	objects derived from the PDDoc before it is saved. 
	@param doc The document that was saved. 
	@param err Error code. error is set to 0 if no errors 
	occurred while saving the file. If an error occurred, error 
	contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocWillSave 
	@notify PDDocWillSaveEx 
	@see CosObjRefreshAfterLinearizedSave 
	@see PDDocSave 
*/
POKE(PDDocDidSave, (PDDoc doc, Int32 err), (PDDoc doc, Int32 err, void* clientData), (doc, err, cell->clientData))


/**
	The contents of a page have changed and the page will be 
	redrawn. 
	@param page The page whose contents changed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageContentsDidChangeEx 
	@see PDPageNotifyContentsDidChange 
	@see PDPageNotifyContentsDidChangeEx 
*/
POKE(PDPageContentsDidChange, (PDPage page),(PDPage page, void *clientData), (page, cell->clientData))

/**
	An annotation will be added to a page. 
	@param page The page to which the annotation will be added. 
	
	@param addAfter The index in the page's annotation array 
	after which the annotation will be added. 
	@param annot The annotation that will be added. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidAddAnnot 
	@see PDPageAddAnnot 
	@see PDPageAddNewAnnot 
*/
POKE(PDPageWillAddAnnot, (PDPage page, Int32 addAfter, PDAnnot annot), (PDPage page, Int32 addAfter, PDAnnot annot, void *clientData), (page, addAfter, annot, cell->clientData))

/**
	An annotation was added to a page. 
	@param page The page to which the annotation was added. 
	
	@param annot The annotation that was added. 
	@param error Error code. error is set to 0 if no errors 
	occurred while adding the annotation. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageWillAddAnnot 
	@see PDPageAddAnnot 
	@see PDPageAddNewAnnot 
*/
POKE(PDPageDidAddAnnot, (PDPage page, PDAnnot annot, Int32 error), (PDPage page, PDAnnot annot, Int32 error, void *clientData), (page, annot, error, cell->clientData))

/**
	An annotation will be removed from a page. 
	@param page The page from which an annotation will be 
	removed. 
	@param annotIndex The index (in the page's annotation 
	array) of the annotation that will be removed. Use PDPageGetAnnot 
	to obtain the annotation from its index. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidRemoveAnnot 
	@see PDPageRemoveAnnot 
*/
POKE(PDPageWillRemoveAnnot, (PDPage page, Int32 annotIndex), (PDPage page, Int32 annotIndex, void *clientData), (page, annotIndex, cell->clientData))

/**
	An annotation has been removed from a page. 

	NOTE: Superceded by PDPageDidRemoveAnnotEx in Acrobat 6.0. 
	
	@param page The page from which an annotation was removed. 
	
	@param annotIndex The index (in the page's annotation 
	array) of the annotation that was removed. Because the annotation 
	has already been removed from the array, it is not possible 
	to access the annotation using annotIndex. 
	@param error Error code. error is set to 0 if no errors 
	occurred while removing the annotation. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageWillRemoveAnnot 
	@see PDPageRemoveAnnot 
*/
POKE(PDPageDidRemoveAnnot, (PDPage page, Int32 annotIndex, Int32 error), (PDPage page, Int32 annotIndex, Int32 error, void *clientData), (page, annotIndex, error, cell->clientData))


/**
	An annotation was created. 
	@param annot The annotation that was created. 
	@param page The page to which the annotation was added. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDAnnotDidChange 
	@notify PDAnnotWillChange 
	@see PDPageCreateAnnot 
	@see PDPageAddNewAnnot 
*/
POKE(PDAnnotWasCreated, (PDAnnot annot, PDPage page), (PDAnnot annot, PDPage page, void *clientData), (annot, page, cell->clientData))

/**
	An annotation will change in the specified way. 
	@param annot The annotation that will change. 
	@param key The ASAtom specifying how the annotation will 
	change. The ASAtom corresponding to the key that changed 
	in the annotation's Cos dictionary. See Section 8.4 on annotations 
	in the PDF Reference for information on the keys. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDAnnotDidChange 
	@see PDAnnotNotifyWillChange 
	@see PDAnnotSetRect 
	@see PDTextAnnotSetOpen 
	@see PDAnnotSetColor 
	@see PDAnnotSetTitle 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDTextAnnotSetContents 
	@see PDLinkAnnotSetBorder 
	@see PDLinkAnnotSetAction 
	@see AVPageViewSetAnnotLocation 
*/
POKE(PDAnnotWillChange, (PDAnnot annot, ASAtom key), (PDAnnot annot, ASAtom key, void* clientData), (annot, key, cell->clientData))

/**
	An annotation changed in the specified way. 
	@param annot The annotation that changed. 
	@param key The ASAtom specifying how the annotation changed. 
	The ASAtom corresponding to the key that changed in the 
	annotation's Cos dictionary. See Section 8.4 on annotations 
	in the PDF Reference for information on the keys. 
	@param error Error code. error is set to 0 if no errors 
	occurred while changing the annotation. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDAnnotWillChange 
	@see PDAnnotNotifyDidChange 
	@see PDAnnotSetRect 
	@see PDTextAnnotSetOpen 
	@see PDAnnotSetColor 
	@see PDAnnotSetTitle 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDTextAnnotSetContents 
	@see PDLinkAnnotSetBorder 
	@see PDLinkAnnotSetAction 
	@see AVPageViewSetAnnotLocation 
*/
POKE(PDAnnotDidChange, (PDAnnot annot, ASAtom key, Int32 error), (PDAnnot annot, ASAtom key, Int32 error, void* clientData), (annot, key, error, cell->clientData))


/**
	A thread was changed. 
	@param thread The thread that changed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidAddThread 
	@see PDThreadSetInfo 
*/
POKE(PDThreadDidChange, (PDThread thread), (PDThread thread, void* clientData), (thread, cell->clientData))


/**
	A bookmark was created. 
	@param bookmark The bookmark that was created. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkDidDestroy 
	@notify PDBookmarkWillDestroy 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddNewSibling 
*/
POKE(PDBookmarkWasCreated, (PDBookmark bookmark), (PDBookmark bookmark, void *clientData), (bookmark, cell->clientData))

/**
	One or more bookmarks have been moved. 
	@param bookmark The bookmark that was moved. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddChild 
	@see PDBookmarkAddSubtree 
*/
POKE(PDBookmarkDidChangePosition, (PDBookmark bookmark), (PDBookmark bookmark, void* clientData), (bookmark, cell->clientData))

/**
	A bookmark will be opened/closed, its action will be changed, 
	its title will be changed, or children will be added to 
	it. 
	@param bookmark The bookmark that will be changed. 
	@param key The ASAtom specifying the change that will 
	occur. key will be an ASAtom corresponding to one of the 
	following:
	<ul>
	<li> Count - Children will be added, or bookmark 
	will be opened/closed.
	<li> A - Action will be changed.
	<li> Title - Title will be changed.
	<li> C - Color will be changed (PDF 1.4)
	<li> F - Flags will be changed (PDF 1.4) 
	</ul>	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkDidChange 
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkSetOpen 
	@see PDBookmarkSetAction 
	@see PDBookmarkSetTitle 
	@see PDBookmarkAddSubtree 
*/
POKE(PDBookmarkWillChange, (PDBookmark bookmark, ASAtom key), (PDBookmark bookmark, ASAtom key, void* clientData), (bookmark, key, cell->clientData))

/**
	A bookmark has been opened/closed, its action has been changed, 
	its title has been changed, or children have been added 
	to it. 
	@param bookmark The bookmark that will be changed. 
	@param key The ASAtom specifying the change that will 
	occur. See PDBookmarkWillChange for a list of the ASAtoms 
	and their meaning. 
	@param err Error code. error is set to 0 if no errors 
	occurred while changing the bookmark. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkSetOpen 
	@see PDBookmarkSetAction 
	@see PDBookmarkSetTitle 
	@see PDBookmarkAddSubtree 
*/
POKE(PDBookmarkDidChange, (PDBookmark bookmark, ASAtom key, Int32 err), (PDBookmark bookmark, ASAtom key, Int32 err, void* clientData), (bookmark, key, err, cell->clientData))

/**
	A bookmark will be destroyed. 
	@param bookmark The bookmark that will be destroyed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkDidDestroy 
	@see PDBookmarkDestroy 
*/
POKE(PDBookmarkWillDestroy, (PDBookmark bookmark), (PDBookmark bookmark, void* clientData), (bookmark, cell->clientData))

/**
	A bookmark was destroyed. 
	@param bookmark The bookmark that was destroyed. Because 
	the bookmark has been destroyed, it is not possible to access 
	it. 
	@param err Error code. error is set to 0 if no errors 
	occurred while destroying the bookmark. If an error occurred, 
	error contains the error code. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkWillDestroy PDBookmarkDestroy 
*/
POKE(PDBookmarkDidDestroy, (PDBookmark bookmark, Int32 err), (PDBookmark bookmark, Int32 err, void* clientData), (bookmark, err, cell->clientData))

/* added 9/30/94 */


/**
	A thread will be removed from a document. 
	@param doc The document from which a thread will be removed. 
	
	@param index The index of the thread that will be removed. 
	Use PDDocGetThread to obtain the thread from its index. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidRemoveThread 
	@see PDDocRemoveThread 
*/
POKE(PDDocWillRemoveThread, (PDDoc doc, Int32 index), (PDDoc doc, Int32 index, void* clientData), (doc, index, cell->clientData))

/* Acrobat 2.1 additions */

/**
	The contents of a page changed. Unlike PDPageContentsDidChange, 
	this notification specifies whether the page is redrawn 
	immediately. 
	@param page The page whose contents changed. 
	@param invalidateViews If true, the page is redrawn immediately. 
	If false, redrawing is suppressed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageContentsDidChange 
	@see PDPageNotifyContentsDidChange 
	@see PDPageNotifyContentsDidChangeEx 
*/
POKE(PDPageContentsDidChangeEx, (PDPage page, ASBool invalidateViews),(PDPage page, ASBool invalidateViews, void *clientData), (page, invalidateViews, cell->clientData))

/* Acrobat 3.0 additions */

/**
	An AVDoc will be opened from a PDF file. 
	@param pdDoc The PDDoc for the file that will be opened. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidOpen 
	@see AVDocOpenFromPDDoc 
	@see AVDocOpenFromPDDocWithParams 
*/
POKE(AVDocWillOpenFromPDDoc, (PDDoc pdDoc), (PDDoc pdDoc, void* clientData), (pdDoc, cell->clientData))

/**
	The document's selection has been added to or had something 
	removed. 
	@param doc The document containing the selection. 
	@param selType The ASAtom corresponding to the current 
	selection type. See Selection Types for a list of selection 
	types. 
	@param selData Pointer to the current selection data after 
	the selection has been added. The format and contents of 
	selData depend on selType. 
	@param addData Pointer to the added selection data. The 
	format and contents of addData depend on selType. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidClearSelection 
	@notify AVDocDidSetSelection 
	@notify AVDocDidRemoveFromSelection 
	@notify AVDocWillClearSelection 
	@see AVDocClearSelection 
	@see AVDocDeleteSelection 
	@see AVDocSetSelection 
*/
POKE(AVDocDidAddToSelection, (AVDoc doc, ASAtom selType, void* selData, void* addData), (AVDoc doc, ASAtom selType, void* selData, void* addData, void *clientData), (doc, selType, selData, addData, cell->clientData))

/**
	The document's selection has had something removed. 
	@param doc The document whose selection was removed. 
	@param selType The ASAtom corresponding to the current 
	selection type. See Selection Types for a list of selection 
	types. 
	@param selData Pointer to the current selection data after 
	the selection has been deleted. The format and contents 
	of selData depend on selType.. 
	@param remData The item removed from the selection. The 
	format and contents of addData depend on selType. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidSetSelection 
	@notify AVDocDidAddToSelection 
	@notify AVDocDidClearSelection 
	@notify AVDocWillClearSelection 
	@see AVDocClearSelection 
	@see AVDocDeleteSelection 
	@see AVDocSetSelection 
*/
POKE(AVDocDidRemoveFromSelection, (AVDoc doc, ASAtom selType, void* selData, void* remData), (AVDoc doc, ASAtom selType, void* selData, void* remData, void *clientData), (doc, selType, selData, remData, cell->clientData))


/**
	An AVDoc's file stream has been terminated by the AVDocSetDead 
	method. 
	@param doc The AVDoc whose file stream has been terminated. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidClose AVDocSetDead 
*/
POKE(AVDocWantsToDie, (AVDoc doc), (AVDoc doc, void* clientData), (doc, cell->clientData))

/**
	This notification is broadcast before a document is printed, 
	before any marks are made on the first page. When printing 
	to a PostScript printer, printing commands can also be sent 
	that are placed on the page before any other marks. For 
	example, a setpagedevice operator could be placed in the 
	print stream. 

	NOTE: Page resources and contents cannot be modified reliably 
	at the time this notification is broadcast. 
	@param doc The document that is about to be printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	In the 2.x Acrobat viewers, the page printing sequence to 
	a PostScript printer is:\n
	page setup (including setpagedevice) ... save ...\n
	gsave ... save ... begin ... begin ... begin ... PDDocWillPrintPage ... page\n
	contents ... PDDocDidPrintPage ... end ... end ... end ... restore ... restore ...\n
	showpage.\n
	This sequence must not be relied on, and it is 
	to some extent dependent on the printer driver in use. Nevertheless, 
	it is true that by the time the PDDocWillPrintPage notification 
	is broadcast, it is too late to perform any setpagedevice 
	operations. 
	@param psLevel When printing to a PostScript printer, 
	psLevel is either 1 or 2, representing the PostScript level 
	available on the printer. When printing to a non-PostScript 
	printer, psLevel is 0. psLevel is useful in determining 
	whether the output device is a PostScript printer. In addition, 
	when printing to a PostScript printer, psLevel is useful 
	to determine the operators that can be sent in any printing 
	code downloaded using the PDDocWillPrintDoc, PDDocWillPrintPage, 
	and PDDocDidPrintPage notifications. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidPrintPage 
	@notify PDDocWillPrintPages 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams
*/
POKE(PDDocWillPrintDoc, (PDDoc doc, ASStm stm, Int32 psLevel), (PDDoc doc, ASStm stm, Int32 psLevel, void *clientData), (doc, stm, psLevel, cell->clientData))


/**
	A document will be saved. 

	The PDDocWillSaveEx notification takes place just before 
	the save operation begins. At the time of a PDDocWillSaveEx 
	notification, the current PDDoc is valid. 

	At this time, clients should look at the save flags field 
	saveFlags in params. In the case of a full save, they should 
	release any objects that they have acquired from the PDDoc 
	doc with methods, such as PDPageRelease. During this notification, 
	plug-in's should also forget any PD-level or Cos-level objects 
	that had been derived from the PDDoc; these will not be 
	valid after the save. 

	See PDDocDidSave for information on reacquiring objects 
	from the PDDoc after it has been saved. 
	@param doc The document that will be saved. 
	@param params The PDDocSaveParams parameters used when 
	saving a file using PDDocSaveWithParams. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidSave 
	@notify PDDocWillSave 
	@see PDDocSave 
	@see PDDocSaveWithParams 
*/
POKE(PDDocWillSaveEx, (PDDoc doc, PDDocSaveParams params), (PDDoc doc, PDDocSaveParams params, void* clientData), (doc, params, cell->clientData))

/* Acrobat 3.1 additions */

/**
	This notification is broadcast before a document is printed,
	before any marks are made on the first page.
	@param doc IN/OUT? The document that is about to be printed.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify AVDocDidPrint
	@see AVDocPrintPages
	@see AVDocPrintPagesWithParams

*/
POKE(AVDocWillPrint, (AVDoc doc), (AVDoc doc, void* clientData), (doc, cell->clientData))

/**
	This notification is broadcast after printing ends.
	@param doc IN/OUT? The document that was printed.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify AVDocWillPrint
	@see AVDocPrintPages
	@see AVDocPrintPagesWithParams

*/
POKE(AVDocDidPrint, (AVDoc doc), (AVDoc doc, void* clientData), (doc, cell->clientData))

/* Acrobat 4.0 additions */

/**
	A PDDoc will be closed. A PDDoc is closed only if its reference 
	count is zero. 

	Neither this notification, PDDocDidClose, AVDocWillClose, 
	nor AVDocDidClose are broadcast if the user selects 'Cancel' 
	when prompted to save a modified document as it is closed. 
	
	@param doc The document to close. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed in by the calling application when 
	this notification was registered for using AVAppRegisterNotification.
	@notify PDDocDidClose 
	@see AVDocClose 
	@see PDDocClose 
*/
POKE(PDDocWillClose, (PDDoc doc),(PDDoc doc, void *clientData), (doc, cell->clientData))

/**
	A range of pages' labels changed in a PDDoc.
	@param doc IN/OUT? The document containing the pages whose labels
	changed.
	@param firstPage IN/OUT? The number of the first page whose label
	changed.
	@param lastPage IN/OUT? The number of the last page whose label
	changed.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@see PDDocSetPageLabel

*/
POKE(PDDocPageLabelDidChange, (PDDoc doc, ASInt32 firstPage, ASInt32 lastPage),(PDDoc doc, ASInt32 firstPage, ASInt32 lastPage, void *clientData),(doc, firstPage, lastPage, cell->clientData))


/**
	The annotations of a document will be exported.
	@param doc IN/OUT? The document whose annotations will be exported.

	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidExportAnnots
	@notify PDDocDidImportAnnots
	@notify PDDocWillImportAnnots
	@see PDDocExportNotes

*/
POKE(PDDocWillExportAnnots, (PDDoc doc), (PDDoc doc, void* clientData), (doc, cell->clientData))

/**
	The annotations from one document will be imported into
	another document.
	@param doc IN/OUT? The document into which annotations will be
	imported.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidExportAnnots
	@notify PDDocDidImportAnnots
	@notify PDDocWillImportAnnots
	@see PDDocImportCosDocNotes
	@see PDDocImportNotes

*/
POKE(PDDocWillImportAnnots, (PDDoc doc), (PDDoc doc, void* clientData), (doc, cell->clientData))

/**
	The annotations of a document were exported.
	@param doc IN/OUT? The document whose annotations were exported.

	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidImportAnnots
	@notify PDDocWillExportAnnots
	@notify PDDocWillImportAnnots
	@see PDDocExportNotes

*/
POKE(PDDocDidExportAnnots, (PDDoc doc), (PDDoc doc, void* clientData), (doc, cell->clientData))

/* PS print additions */

/**
	This notification is broadcast after the beginning of the
	PostScript Prolog (immediately after writing %% BeginPrologue)
	during the printing of a document to a PostScript printer
	with the methods PDFLPrintDoc (only available with PDF Library
	SDK) or PDDocPrintPages (only available with PDF Library
	SDK). The Prolog is a set of application-specific procedure
	definitions that an application may emit in a PostScript
	stream.

	At this point nothing should be added to the PostScript
	print stream that modifies the graphics state or puts marks
	on the page. Callers should only emit procset resources.

	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintAfterBeginSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterBeginProlog, (PDDoc doc, ASStm stm), (PDDoc doc, ASStm stm, void *clientData), (doc,  stm, cell->clientData))

/**
	This notification is broadcast after the beginning of the
	Document Setup (immediately after writing %% BeginSetup)
	during the printing of a page to a PostScript printer with
	the methods PDFLPrintDoc (only available with PDF Library
	SDK) or PDDocPrintPages (only available with PDF Library
	SDK). During Document Setup, fonts may be downloaded, setpagedevice
	may be called, procsets may be initialized, the graphics
	state may be initialized, and so forth.
	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintBeforeEndSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterBeginSetup, (PDDoc doc, ASStm stm), (PDDoc doc, ASStm stm, void *clientData), (doc, stm, cell->clientData))

/**
	This notification is broadcast before the end of Document
	Setup (immediately before writing %% EndSetup) during the
	printing of a page to a PostScript printer with the methods
	PDFLPrintDoc (only available with PDF Library SDK) or PDDocPrintPages
	(only available with PDF Library SDK). At this point all
	of the job level resources and procsets have been added
	to the print stream.
	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite. Some
	printer drivers (Windows PS4) may not allow additions to
	the PostScript stream at this point.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintAfterBeginSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintBeforeEndSetup, (PDDoc doc, ASStm stm), (PDDoc doc, ASStm stm, void *clientData), (doc, stm, cell->clientData))

/**
	This notification is broadcast after the beginning of the
	Page Setup (immediately after writing %% BeginPageSetup)
	during the printing of a page to a PostScript printer with
	the methods PDFLPrintDoc (only available with PDF Library
	SDK) or PDDocPrintPages (only available with PDF Library
	SDK). At this point it is possible to use setpagedevice
	and set the graphics state but marks cannot be made on the
	page.
	@param doc IN/OUT? The document from which a page is printed.
	@param page IN/OUT? Page number of the page being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite. Some
	printer drivers (Windows PS4) may not allow additions to
	the PostScript stream at this point.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintAfterPageTrailer
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterBeginPageSetup, (PDDoc doc, ASInt32 page, ASStm stm), (PDDoc doc, ASInt32 page, ASStm stm, void *clientData), (doc, page, stm, cell->clientData))

/**
	This notification is broadcast after the page trailer is
	emitted (immediately after writing %% PageTrailer) during
	the printing of a page to a PostScript printer with the
	methods PDFLPrintDoc (only available with PDF Library SDK)
	or PDDocPrintPages (only available with PDF Library SDK).
	At this point it is possible to resolve comments (at end)
	and emit cleanup code.
	@param doc IN/OUT? The document from which a page is printed.
	@param page IN/OUT? Page number of the page being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintAfterBeginPageSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterPageTrailer, (PDDoc doc, ASInt32 page, ASStm stm), (PDDoc doc, ASInt32 page, ASStm stm, void *clientData), (doc, page, stm, cell->clientData))

/**
	This notification is broadcast after the DSC trailer is
	emitted (immediately after writing %% Trailer) during the
	printing of a page to a PostScript printer with the methods
	PDFLPrintDoc (only available with PDF Library SDK) or PDDocPrintPages
	(only available with PDF Library SDK). At this point it
	is possible to resolve comments (at end) and emit cleanup
	code.
	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintBeforeEndSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterTrailer, (PDDoc doc, ASStm stm), (PDDoc doc, ASStm stm, void *clientData), (doc, stm, cell->clientData))

/**
	This notification is broadcast after the DSC page-level
	comments that apply to all pages have been emitted (immediately
	before writing %% EndComments) during the printing of a
	page to a PostScript printer with the methods PDFLPrintDoc
	(only available with PDF Library SDK) or PDDocPrintPages
	(only available with PDF Library SDK).
	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintAfterBeginSetup
	@notify PSPrintBeforeEndSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintBeforeEndComments, (PDDoc doc, ASStm stm), (PDDoc doc,  ASStm stm, void *clientData), (doc,  stm, cell->clientData))

/**
	This notification is broadcast after extended graphics state
	parameters are emitted while printing to a PostScript printer
	with the methods PDFLPrintDoc (only available with PDF Library
	SDK) or PDDocPrintPages (only available with PDF Library
	SDK).

	These parameters are typically device-dependent. For information
	on extended graphics state, see Section 4.3.4 on extended
	graphics states in PDF Reference.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE(PSPrintAfterEmitExtGState, (ASStm stm), (ASStm stm, void *clientData), (stm, cell->clientData))

/* New named destination addition */

/**
	Acrobat executed an action to go to a named destination. 
	
	@param doc The document containing the named destination. 
	
	@param NameObj The Cos object corresponding to the named 
	destination. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE(AVDocDidClickName, (AVDoc doc, CosObj NameObj), ( AVDoc doc, CosObj NameObj, void* clientData ), (doc, NameObj, cell->clientData ) )


/**
	The annotations from one document were imported into another
	document.
	@param doc IN/OUT? The document into which annotations were imported.

	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidImportAnnots
	@notify PDDocWillExportAnnots
	@notify PDDocWillImportAnnots
	@see PDDocImportCosDocNotes
	@see PDDocImportNotes

*/
POKE(PDDocDidImportAnnots, (PDDoc doc), (PDDoc doc, void* clientData), (doc, cell->clientData))

/**
	All AVDocs will be closed.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify AVDocDidClose
	@notify AVDocWillClose

*/
POKE(AVAppWillCloseAllInternalDocs, (void), (void *clientData), (cell->clientData))

/* New PDNameTree additions */

/**
	An entry was added to a name tree. 
	@param NameTree The name tree to which an entry was added. 
	
	@param Key Cos object of the key for the entry. This object 
	is a Cos integer. 
	@param Value Cos object for the value associated with 
	key. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed in by the calling application when 
	this notification was registered for using AVAppRegisterNotification.
	@notify PDNameTreeNameRemoved 
	@see PDNameTreeNotifyNameAdded 
*/
POKE(PDNameTreeNameAdded, (PDNameTree NameTree, CosObj Key, CosObj Value), ( PDNameTree NameTree, CosObj Key, CosObj Value, void* clientData ), (NameTree, Key, Value, cell->clientData ) )

/**
	An entry was removed from a name tree. 
	@param NameTree The name tree from which an entry was 
	removed. 
	@param Key The Cos object of the key for the entry. This 
	object is a Cos integer. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed in by the calling application when 
	this notification was registered for using AVAppRegisterNotification.
	@notify PDNameTreeNameAdded 
	@see PDNameTreeNotifyNameRemoved 
*/
POKE(PDNameTreeNameRemoved, (PDNameTree NameTree, CosObj Key), ( PDNameTree NameTree, CosObj Key, void* clientData ), (NameTree, Key, cell->clientData ) )


/**
	A PDDoc closed. A PDDoc is closed only if its reference 
	count is zero. 

	Neither this notification, PDDocWillClose, AVDocWillClose, 
	nor AVDocDidClose are broadcast if the user selects Cancel 
	when prompted to save a modified document as it is closed. 
	
	@param doc The document that closed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed in by the calling application when 
	this notification was registered for using AVAppRegisterNotification.
	@notify PDDocWillClose 
	@see AVDocClose 
	@see PDDocClose 
*/
POKE(PDDocDidClose, (PDDoc doc),(PDDoc doc, void *clientData), (doc, cell->clientData))

/* New InsertPages notifications */

/**
	Pages were inserted into a document. This notification occurs
	after the PDDocDidInsertPages notification.
	@param params IN/OUT? A structure describing how pages were inserted.

	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidInsertPages
	@notify PDDocWillInsertPages
	@notify PDDocWillInsertPagesEx
	@see PDDocCreatePage
	@see PDDocInsertPages

*/
POKE(PDDocDidInsertPagesEx, (PDDocInsertPagesParams params), (PDDocInsertPagesParams params, void* clientData), (params, cell->clientData))

/**
	Pages will be inserted into a document. This notification
	occurs after the PDDocWillInsertPages notification.
	@param params IN/OUT? A structure describing how pages will be
	inserted.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PDDocDidInsertPages
	@notify PDDocDidInsertPagesEx
	@notify PDDocWillInsertPages
	@see PDDocCreatePage
	@see PDDocInsertPages

*/
POKE(PDDocWillInsertPagesEx, (PDDocInsertPagesParams params), (PDDocInsertPagesParams params, void* clientData), (params, cell->clientData))

/* New PDNumTree additions */

/**
	An entry was added to a name tree.
	@param numTree IN/OUT? The name tree to which an entry was added.
	@param key IN/OUT? The key for the entry.
	@param value IN/OUT? Cos object for the value associated with key.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.
	@notify PDNumTreeNumRemoved
	@see PDNumTreePut
*/
POKE(PDNumTreeNumAdded, (PDNumTree numTree, ASInt32 key, CosObj value), ( PDNumTree numTree, ASInt32 key, CosObj value, void* clientData ), (numTree, key, value, cell->clientData ) )

/**
	An entry was removed from a name tree.
	@param numTree IN/OUT? The name tree from which an entry was removed.
	@param key IN/OUT? The key for the entry.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.
	@notify PDNumTreeNumAdded
	@see PDNumTreePut
	@see PDNumTreeRemove
*/
POKE(PDNumTreeNumRemoved, (PDNumTree numTree, ASInt32 key), ( PDNumTree numTree, ASInt32 key, void* clientData ), (numTree, key, cell->clientData ) )

/* New bookmark notification */

/**
	A bookmark was unlinked from the bookmark tree. 
	@param bookmark The bookmark that was unlinked. Because 
	the bookmark has not been destroyed, it is possible to access 
	it. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkDidChangePosition PDBookmarkUnlink 
*/
POKE(PDBookmarkDidUnlink, (PDBookmark bookmark), (PDBookmark bookmark, void* clientData), (bookmark, cell->clientData))

/* 5.0 */


/**
	Called when a user deletes the current selection. 
	@param doc The document containing the selection. 
	@param selType The selection's type. See Selection Types 
	for a list of the those available. 
	@param selData Server-dependent selection data. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocDidAddToSelection 
	@notify AVDocDidSetSelection 
	@see AVDocDeleteSelection 
*/
POKE(AVDocDidDeleteSelection, (AVDoc doc, ASAtom selType, void* selData), (AVDoc doc, ASAtom selType, void* selData, void *clientData), (doc, selType, selData, cell->clientData))

/**
	This notification is broadcast before a tiled page is printed. 
	Tiled pages can be requested by the user from the Advanced 
	print dialog; clients are made aware of the selection via 
	the tiled page notifications. 
	@param doc The document containing the tiled page. 
	@param page The page to be printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	
	@param whichTile The sheet to be printed. 
	@param numTiles The total number of sheets to be printed. 
	
	@param cropRegion A pointer to an ASFixedRect that represents 
	the region of the PDPage being 'cropped to' for this sheet. 
	
	@param tile The PDTile currently in use. The fields in 
	this structure can be modified as necessary to affect tiling 
	output. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidPrintTiledPage 
	@notify PDDocPrintingTiledPage 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocWillPrintTiledPage, (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 whichTile, ASInt32 numTiles, ASFixedRect *cropRegion, PDTile tile), (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 whichTile, ASInt32 numTiles, ASFixedRect *cropRegion, PDTile tile, void *clientData), (doc, page, stm, whichTile, numTiles, cropRegion, tile, cell->clientData))

/**
	Clients who register for PDDocPrintingTilePage will be called 
	just after the page contents have been emitted for this 
	tile. Tiled pages can be requested by the user from the 
	Advanced print dialog; clients are made aware of the selection 
	via the tiled page notifications. 
	@param doc The document containing the tiled page. 
	@param page The page being printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	
	@param whichRow The row that is being printed. 
	@param whichColumn The column that is being printed. 
	@param numRows The number of rows to be printed. 
	@param numColumns The number of columns to be printed. 
	
	@param cropRegion A pointer to an ASFixedRect that represents 
	the region of the PDPage being 'cropped to' for this sheet. 
	
	@param tile The PDTile currently in use. The fields in 
	this structure can be modified as necessary to affect tiling 
	output. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidPrintTiledPage 
	@notify PDDocWillPrintTiledPage 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams
*/
POKE(PDDocPrintingTiledPage, (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 whichRow, ASInt32 whichColumn, ASInt32 numRows, ASInt32 numColumns, ASFixedRect *cropRegion, PDTile tile), (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 whichRow, ASInt32 whichColumn, ASInt32 numRows, ASInt32 numColumns, ASFixedRect *cropRegion, PDTile tile, void *clientData), (doc, page, stm, whichRow, whichColumn, numRows, numColumns, cropRegion, tile, cell->clientData))

/**
	Clients who register for PDDocDidPrintTiledPage will be 
	called after the tile marks (if any) have been emitted for 
	this tiled page. Tiled pages can be requested by the user 
	from the Advanced print dialog; clients are made aware of 
	the selection via the tiled page notifications. 
	@param doc The document containing the tiled page. 
	@param page The page being printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	@param error Error code. error is set to 0 if no errors 
	occurred while printing the pages. If an error occurred, 
	error contains the error code. 
	@param whichTile The sheet to be printed. 
	@param numTiles The number of sheets to be printed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocPrintingTiledPage 
	@notify PDDocWillPrintTiledPage 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocDidPrintTiledPage, (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 error, ASInt32 whichTile, ASInt32 numTiles), (PDDoc doc, ASInt32 page, ASStm stm, ASInt32 error, ASInt32 whichTile, ASInt32 numTiles, void *clientData), (doc, page, stm, error, whichTile, numTiles, cell->clientData))

/**
	Clients who register for PDPageWillPrintAnnots will be called 
	before the printed representations of any annotations have 
	been emitted. If a page has no annotations, this will not 
	be called. If a page has annotations, this will be called. 
	There may not be any code emitted for the annotations on 
	that page, however, since they may not have any appearance 
	for printing. The status parameter passed in the PDPageDidPrintAnnot 
	will indicate this. 
	@param doc The document containing the annotations. 
	@param page The page to be printed. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidPrintAnnot 
	@notify PDPageDidPrintAnnots 
	@notify PDPageWillPrintAnnot 
	@see Numerous 
*/
POKE(PDPageWillPrintAnnots, (PDDoc doc, ASInt32 page, ASStm stm), (PDDoc doc, ASInt32 page, ASStm stm, void *clientData), (doc, page, stm, cell->clientData))

/**
	Annotations were printed. 
	@param doc The document containing the annotations. 
	@param page The page containing the annotations. 
	@param stm The PostScript print stream when printing to 
	a PostScript printer, and NULL when printing to a non-PostScript 
	printer. When printing to a PostScript printer, clients 
	can write printing commands into stm (using ASStmWrite) 
	to add marks to pages before any other marks have been made. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidPrintAnnot 
	@notify PDPageWillPrintAnnot 
	@notify PDPageWillPrintAnnots 
	@see Numerous 
*/
POKE(PDPageDidPrintAnnots, (PDDoc doc, ASInt32 page, ASStm stm), (PDDoc doc, ASInt32 page, ASStm stm, void *clientData), (doc, page, stm, cell->clientData))


/**
	A document was opened. 
	@param doc The document. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocPermsReady 
	@see Numerous 
*/
POKE(PDDocDidOpen, (PDDoc doc), (PDDoc doc, void *clientData), (doc, cell->clientData))

/**
	The client is requested to calculate and set metadata items 
	that depend on the state of the document. Issued when a 
	document is saved. Can also be issued explicitly via PDDocCalculateImplicitMetadata. 
	
	@param doc The document for which implicit metadata 
	is to be calculated. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see Numerous, but especially PDDocCalculateImplicitMetadata 
	
*/
POKE(PDDocCalculateMetadata, (PDDoc doc), (PDDoc doc, void *clientData), (doc, cell->clientData))


/**
	A new-style preference changed. 

	NOTE: As of Acrobat 5.0, most of the methods used to set 
	and get the new style preferences have not been exported 
	to the public API. 
	@param section The section. 
	@param key The key. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppOldPrefDidChange 
	@see Numerous 
*/
POKE(AVAppPrefDidChange,
	 (const char *section, const char *key),
	 (const char *section, const char *key, void *clientData),
	 (section, key, cell->clientData))


/**
	An old-style AVPrefsType was changed. 
	@param preference Preference type that was changed. You 
	can call AVAppGetPreference to find out the new value. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppPrefDidChange 
	@see AVAppSetPreference 
*/
POKE (AVAppOldPrefDidChange,
	 (AVPrefsType preference),
	 (AVPrefsType preference, void *clientData),
	 (preference, cell->clientData))


/**
	Page areas changed. 
	@param pdDoc The document in which the page areas changed. 
	
	@param areaMask The area mask. 
	@param firstPage The first page. 
	@param lastPage The last page. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidChangePages 
	@see Numerous 
*/
POKE (PDDocDidChangePageAreas,
	 (PDDoc pdDoc, ASInt32 areaMask, ASInt32 firstPage, ASInt32 lastPage),
	 (PDDoc pdDoc, ASInt32 areaMask, ASInt32 firstPage, ASInt32 lastPage, void *clientData),
	 (pdDoc, areaMask, firstPage, lastPage, cell->clientData))

/**
	Clients that register for this notification will be called
	back during PostScript printing - just prior to emission
	of the Acrobat procsets.
	@param doc IN/OUT? The document that is being printed.
	@param stm IN/OUT? The PostScript print stream. PostScript commands
	can be added to the print stream using ASStmWrite.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@notify PSPrintBeforeEndSetup
	@see PDDocPrintPages (Only available with PDF Library SDK)
	@see PDFLPrintDoc (Only available with PDF Library SDK)

*/
POKE (PSPrintBeforeAcrobatProcsets,
	 (PDDoc doc, ASStm stm),
	 (PDDoc doc, ASStm stm, void *clientData),
	 (doc, stm, cell->clientData))

/**
	Redrawing will occur in the page view section of the window. 
	
	@param pageView The AVPageView in which drawing will occur. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVPageViewDidChange 
	@notify AVPageViewDidDraw 
	@see AVPageViewDrawNow 
*/
POKE(AVPageViewWillDraw,
	 (AVPageView pageView),
	 (AVPageView pageView, void *clientData),
	 (pageView, cell->clientData))


/**
	Goes out when an annotation has performed a focus operation - for 
	example, kAVAnnotAcceptFocus or kAVAnnotLostFocus. No notification 
	is issued for kAVAnnotDefaultAction or kAVAnnotShowMenu. 
	
	@param pageView The page view containing the annotation. 
	
	@param pdAnnot The annotation that performed the operation. 
	
	@param annotOp The operation. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see AVPageViewFocusAnnotPerformOp 
*/
POKE(AVPageViewAnnotDidPerformOp,
	 (AVPageView pageView, PDAnnot pdAnnot, AVAnnotOp annotOp),
	 (AVPageView pageView, PDAnnot pdAnnot, AVAnnotOp annotOp, void *clientData),
	 (pageView, pdAnnot, annotOp, cell->clientData))

/**
	Clients who register for this notification will be notified 
	just before an annotation is expected to print. This notification 
	allows clients that manage annotations to prepare the appearance 
	of the annotation for printing purposes. There is no requirement 
	that the annot referred to in this parameter list actually 
	have a print appearance. 
	@param annot The annotation to print. 
	@param page The page on which the annotation occurs. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidPrintAnnot 
	@notify PDPageDidPrintAnnots 
	@notify PDPageWillPrintAnnot 
	@see Numerous 
*/
POKE(PDPageWillPrintAnnot,
	(PDAnnot annot, ASInt32 page),
	(PDAnnot annot, ASInt32 page, void *clientData),
	(annot, page, cell->clientData))


/**
	Clients who register for PDPageDidPrintAnnot will be called 
	after the annotation has printed. status indicates whether 
	Acrobat tried to print any representation of this annotation. 
	
	@param annot The annotation that Acrobat tried to print. 
	
	@param page The page on which the annotation occurs. 
	@param status A status of 0 indicates that no representation 
	of the annot was found. A status of 1 indicates the annot 
	was printed. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageDidPrintAnnot 
	@notify PDPageWillPrintAnnot 
	@notify PDPageWillPrintAnnots 
	@see Numerous 
*/
POKE(PDPageDidPrintAnnot,
	(PDAnnot annot, ASInt32 page, ASInt32 status),
	(PDAnnot annot, ASInt32 page, ASInt32 status, void *clientData),
	(annot, page, status, cell->clientData))


/**
	Used to let clients know when pdDoc is undergoing batch
	processing.

	NOTE: This notification is set for any pdDoc that serves
	as input to batch processing, including pdDocs that are
	attached to AVDocs when batching is invoked on the currently
	open documents.
	@param context IN/OUT? Placeholder. Reserved for future releases.
	Always NULL.
	@param pdDoc IN/OUT? Document undergoing batch processing.
	@param docInUse IN/OUT? When the document is about to be batch processed,
	the notification is sent with isUsing set to true. When
	batch processing has finished with the document, the notification
	is again sent but with isUsing set to false.
	@param clientData IN/OUT? Pointer to a block of user-supplied data
	that was passed in by the calling application when this
	notification was registered for using AVAppRegisterNotification.

	@see Numerous

*/
POKE(AVAppUsingPDDocForBatch,
	(AVBatchContext context, PDDoc pdDoc, ASBool docInUse),
	(AVBatchContext context, PDDoc pdDoc, ASBool docInUse, void *clientData),
	(context, pdDoc, docInUse, cell->clientData))
     /* 6.0 */

/**
	Called when the document’s page view changes.
	@param avDoc The AVDoc whose page view has changed.
	@param newPageView The new AVPageView.
	@param oldPageView The old AVPageView.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVPageViewDocDidChange 
	@notify AVPageViewWasCreated 
	@notify AVPageViewWillDestroy
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews 
	@see AVDocGetPageView 
*/
POKE(AVDocActivePageViewDidChange,
	 (AVDoc avDoc, AVPageView newPageView, AVPageView oldPageView),
	 (AVDoc avDoc, AVPageView newPageView, AVPageView oldPageView, void *clientData),
	 (avDoc, newPageView, oldPageView, cell->clientData))

/**
	Sent when a page view is created.
	@param pageView The AVPageView that was created.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocActivePageViewDidChange 
	@notify AVPageViewDocDidChange
	@notify AVPageViewDidChange
	@notify AVPageViewWillDestroy
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews 
	@see AVDocGetPageView
	@see AVPageViewGetAVDoc
*/
POKE(AVPageViewWasCreated,
	 (AVPageView pageView),
	 (AVPageView pageView, void *clientData),
	 (pageView, cell->clientData))

/**
	Sent before a page view is destroyed.
	@param pageView The AVPageView that will be destroyed.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocActivePageViewDidChange 
	@notify AVPageViewDocDidChange
	@notify AVPageViewDidChange
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews 
	@see AVDocGetPageView
	@see AVPageViewGetAVDoc
*/	 
POKE(AVPageViewWillDestroy,
	 (AVPageView pageView),
	 (AVPageView pageView, void *clientData),
	 (pageView, cell->clientData))

/**
	The page view has changed. Zero or more of the following 
	events has occurred: 
	<ul>
	<li> The page number has changed.
	<li> The zoom factor has changed.
	<li> The window has been resized.
	<li> The page has been scrolled.
	</ul>

	NOTE: If continuous scrolling is turned on (available in 
	Acrobat 3.0 or later) and more than one page is displayed 
	in the AVPageView, alternating mouse clicks in the different 
	pages displayed does not constitute a change to the AVPageView. 
	
	@param pageView The AVPageView that has changed. 
	@param how Specifies how the page view did change. how 
	is an OR of zero or more of the following (see AVExpT.h): 
	PAGEVIEW_UPDATE_SCROLL - The view has been scrolled. PAGEVIEW_UPDATE_PAGENUM 
	 -  The page number has changed. PAGEVIEW_UPDATE_PAGESIZE 
	 -  A new view has been created. PAGEVIEW_UPDATE_ZOOM - The 
	zoom has been changed.
	@param rect Pointer to the scroll rectangle for the pageView
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocActivePageViewDidChange 
	@notify AVPageViewDidDraw 
	@notify AVPageViewWasCreated 
	@notify AVPageViewWillDestroy 
	@see AVPageViewZoomTo 
	@see AVPageViewScrollTo 
	@see AVPageViewScrollToRect 
	@see AVPageViewGoTo 
	@see AVPageViewReadPageDown 
	@see AVPageViewReadPageUp 
	@see AVPageViewGoBack 
	@see AVPageViewGoForward 
	@see AVPageViewUseDestInfo 
	@see AVPageViewUseThisDestination
*/
POKE(AVPageViewDidChangeEx,
	 (AVPageView pageView, ASInt16 how, const AVDevRectP rect),
	 (AVPageView pageView, ASInt16 how, const AVDevRectP rect, void *clientData),
	 (pageView, how, rect, cell->clientData))

/* 6.0 */
/* existing PDBookmarkWill/DidChange notifications don't distinguish between new bookmark and open/close */

/**
	A bookmark will be opened or closed.
	@param bookmark The bookmark that will be opened or closed.
	@param open When true, the bookmark will be opened. When false, it will be closed.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkSetOpen 
*/
POKE(PDBookmarkWillChangeOpenState, (PDBookmark bookmark, ASBool open), (PDBookmark bookmark, ASBool open, void* clientData), (bookmark, open, cell->clientData))

/**
	A bookmark was opened or closed.
	@param bookmark The bookmark that was opened or closed.
	@param open When true, the bookmark was opened. When false, it was closed.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkSetOpen 
*/
POKE(PDBookmarkDidChangeOpenState, (PDBookmark bookmark, ASBool open), (PDBookmark bookmark, ASBool open, void* clientData), (bookmark, open, cell->clientData))

/**
	Sent on a system running Windows XP when the user does a
	Switch User (Fast User Switching) operation to log on or switch
	to another user account. Typically used to release shared resources,
	such a fonts or multimedia ports.
	@param switchState The switch state. \n
	<ul>
	<li> WTS_CONSOLE_CONNECT when a switched out session is switched back in.
	<li> WTS_CONSOLE_DISCONNECT when a session is switched out.
	</ul>
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE(AVAppSystemLogonSessionSwitched, (const char* switchState), (const char* switchState, void *clientData), (switchState, cell->clientData))

/* BEGIN Optional Content notifications */

/**
	An optional-content context is changing in a way that could affect the
	visibility state of content.
	@param ocContext The PDOCContext whose visibility state will change.
	@param whatWillHappen The type of change that will occur.
	@param objects A pointer to the object that will change. The kind of object this
	can be depends on the type of change that will occur:\n
	- kPDOCGCreate: PDOCG
	- kPDOCGDestroy: NULL
	- kPDOCGProperties: PDOCG
	- kPDOCConfigChange: PDOCConfig
	- kPDOCConfigCreate: PDOCConfig
	- kPDOCConfigDestroy: NULL
	- kPDOCGReplace: PDOCG (the replacement)
	- kPDDocRemoveOC: NULL
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE(PDOCContextWillChange,
	(PDOCContext ocContext, PDOCContextChangeType whatWillHappen, void* objects),
	(PDOCContext ocContext, PDOCContextChangeType whatWillHappen, void* objects, void *clientData),
	(ocContext, whatWillHappen, objects, cell->clientData))
	
/**
	An optional-content context changed in a way that could affect
	the visibility state of content.
	@param ocContext The PDOCContext whose visibility state changed.
	@param whatHappened The type of change that occurred.
	@param objects A pointer to the object that changed. The kind of object this
	can be depends on the type of change that occurred:\n
	- kPDOCGCreate: PDOCG
	- kPDOCGDestroy: NULL
	- kPDOCGProperties: PDOCG
	- kPDOCConfigChange: PDOCConfig
	- kPDOCConfigCreate: PDOCConfig
	- kPDOCConfigDestroy: NULL
	- kPDOCGReplace: PDOCG (the replacement)
	- kPDDocRemoveOC: NULL
	@param error If non-zero, indicates an error that occurred while setting the optional-content context.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE(PDOCContextDidChange,
	(PDOCContext ocContext, PDOCContextChangeType whatHappened, void* objects, ASErrorCode error),
	(PDOCContext ocContext, PDOCContextChangeType whatHappened, void* objects, ASErrorCode error, void *clientData),
	(ocContext, whatHappened, objects, error, cell->clientData))

/**
	An optional-content context is changing in a PDDoc in a way that could affect the
	visibility state of content.
	@param doc The document containing the optional-content context whose visibility state is changing.
	@param whatWillHappen The type of change that will occur.
	@param object A pointer to the object that will change. The kind of object this
	can be depends on the type of change that will occur:\n
	- kPDOCGCreate: PDOCG
	- kPDOCGDestroy: NULL
	- kPDOCGProperties: PDOCG
	- kPDOCConfigChange: PDOCConfig
	- kPDOCConfigCreate: PDOCConfig
	- kPDOCConfigDestroy: NULL
	- kPDOCGReplace: PDOCG (the replacement)
	- kPDDocRemoveOC: NULL
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocOCDidChange
	@notify PDOCContextDidChange
	@notify PDOCContextWillChange 
*/	
POKE(PDDocOCWillChange,
	(PDDoc doc, PDDocOCChangeType whatWillHappen, void* object),
	(PDDoc doc, PDDocOCChangeType whatWillHappen, void* object, void *clientData),
	(doc, whatWillHappen, object, cell->clientData))

/**
	An optional-content context changed in a PDDoc in a way that could affect
	the visibility state of content.
	@param doc The document containing the optional-content context whose visibility state changed.
	@param whatHappened The type of change that occurred.
	@param object A pointer to the object that changed. The kind of object this
	can be depends on the type of change that occurred:\n
	- kPDOCGCreate: PDOCG
	- kPDOCGDestroy: NULL
	- kPDOCGProperties: PDOCG
	- kPDOCConfigChange: PDOCConfig
	- kPDOCConfigCreate: PDOCConfig
	- kPDOCConfigDestroy: NULL
	- kPDOCGReplace: PDOCG (the replacement)
	- kPDDocRemoveOC: NULL
	@param error If non-zero, indicates an error that occurred while setting the optional-content context.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocOCWillChange
	@notify PDOCContextDidChange
	@notify PDOCContextWillChange 
*/
POKE(PDDocOCDidChange,
	(PDDoc doc, PDDocOCChangeType whatHappened, void* object, ASErrorCode error),
	(PDDoc doc, PDDocOCChangeType whatHappened, void* object, ASErrorCode error, void *clientData),
	(doc, whatHappened, object, error, cell->clientData))
/* END Optional Content notifications */

/**
	An annotation has been removed from a page.
	NOTE: Supercedes PDPageDidRemoveAnnot in Acrobat 6.0.
	@param page The page from which an annotation was removed.
	@param removedAnnot The annotation object that was removed.
	@param error Error code. error is set to 0 if no errors occurred
	while removing the annotation. If an error occurred, error contains
	the error code.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDPageWillRemoveAnnot
	@see PDPageRemoveAnnot 
*/
POKE(PDPageDidRemoveAnnotEx,
	 (PDPage page, PDAnnot removedAnnot, ASInt32 error),
	 (PDPage page, PDAnnot removedAnnot, Int32 error, void *clientData),
	 (page, removedAnnot, error, cell->clientData))


/**
	Sent before a document is printed, before any marks are 
	made on the first page. Page resources and contents may 
	be modified at the time this notification is broadcast. 
	
	@param doc The document that is about to be printed. 
	@param printMode A constant that describes what is being 
	printed - the document only, the document with annotations, 
	or the form data only. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify PDDocDidPrintPages 
	@notify PDDocWillPrintDoc 
	@see AVDocPrintPages 
	@see AVDocPrintPagesWithParams 
*/
POKE(PDDocWillPrintDocInMode,
    (PDDoc doc, PDPrintWhat printMode),
    (PDDoc doc, PDPrintWhat printMode, void *clientData),
    (doc, printMode, cell->clientData))



/**
	Sent when a page view becomes associated with an AVDoc. 
	When a cross-document link is being performed, the same 
	page view may be re-used with a different AVDoc. In this 
	case this notification is sent twice, once when the old 
	AVDoc is closed and the page view's AVDoc becomes NULL, 
	and again when the new AVDoc is opened and associated with 
	the page view. 
	@param pageView The AVPageView for which the document 
	changed. 
	@param newDoc The new document associated with the page 
	view. 
	@param oldDoc The document that was previously associated 
	with the page view. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocActivePageViewDidChange 
	@notify AVPageViewDidChange 
	@notify AVPageViewWasCreated 
	@notify AVPageViewWillDestroy 
	@see AVDocGetNthPageView 
	@see AVDocGetNumPageViews 
	@see AVDocGetPageView 
	@see AVPageViewGetAVDoc 
*/
POKE(AVPageViewDocDidChange,
	 (AVPageView pageView, AVDoc newDoc, AVDoc oldDoc),
	 (AVPageView pageView, AVDoc newDoc, AVDoc oldDoc, void *clientData),
	 (pageView, newDoc, oldDoc, cell->clientData))



 /**
	A document' s selection has been cleared. 
	@param doc The document whose selection was cleared. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVDocWillClearSelection 
	@see AVDocClearSelection 
	@see AVDocDeleteSelection 
	@see AVDocSetSelection 
*/
POKE(AVDocDidClearSelection,
	(AVDoc doc),
	(AVDoc doc, void *clientData),
	(doc, cell->clientData))


/**
	The Acrobat viewer has finished initializing extensions. 
	
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppWillTerminateExtensions 
*/
POKE(AVAppDidInitExtensions, (void), (void *clientData), (cell->clientData))


/**
	The Acrobat viewer will terminate extensions. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@notify AVAppDidInitExtensions 
	@notify AVAppWillQuit 
*/
POKE(AVAppWillTerminateExtensions, (void), (void *clientData), (cell->clientData))


/**
	A document's active tool changed. 
	@param activeDoc The document 
	@param prevTool The previously active tool. 
	@param curTool The currently active tool. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see AVDocGetActiveTool 
	@see AVDocSetActiveTool 
*/
POKE(AVAppToolDidChange,
	 (AVDoc activeDoc, AVTool prevTool, AVTool curTool),
	 (AVDoc activeDoc, AVTool prevTool, AVTool curTool, void *clientData),
	 (activeDoc, prevTool, curTool, cell->clientData))

/**
	A document was opened and its permissions (if present) have 
	been validated. 
	@param doc The document. 
	@param clientData Pointer to a block of user-supplied 
	data that was passed in by the calling application when 
	this notification was registered for using AVAppRegisterNotification.
	@notify PDDocDidOpen 
	@see numerous 
*/
POKE(PDDocPermsReady, (PDDoc doc), (PDDoc doc, void *clientData), (doc, cell->clientData))

/**
	A new window was created for a document. This will be
	called for all windows created for a document, including
	the initial window. It will arrive before AVDocDidOpen.

	@param doc The document
	@param window The new AVWindow
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see AVDocGetAVWindow
	@see AVDocWindowWasRemoved
*/
POKE(AVDocWindowWasAdded,
	 (AVDoc doc, AVWindow window),
	 (AVDoc doc, AVWindow window, void* clientData),
	 (doc, window, cell->clientData))

/**
	A window was removed from a document. This is called
	after the association between the window and document
	has been severed but before the window is destroyed.

	@param doc The document
	@param window The AVWindow being removed
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see AVDocGetAVWindow
	@see AVDocWindowWasAdded
*/
POKE(AVDocWindowWasRemoved,
	 (AVDoc doc, AVWindow window),
	 (AVDoc doc, AVWindow window, void* clientData),
	 (doc, window, cell->clientData))

/**
	The active window associated with a document has changed. This
	is the window returned by AVDocGetAVWindow.

	@param doc The document
	@param oldWindow The previous active window for the document (which may be NULL)
	@param newWindow The new active window for the document
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see AVDocGetAVWindow
*/
POKE(AVDocAVWindowDidChange,
	 (AVDoc doc, AVWindow oldWindow, AVWindow newWindow),
	 (AVDoc doc, AVWindow oldWindow, AVWindow newWindow, void* clientData),
	 (doc, oldWindow, newWindow, cell->clientData))

/* An entry will be removed from the PDNameTree */
POKE(PDNameTreeNameWillRemove,
	 (PDNameTree tree, const char *key, ASInt32 keyLen),
	 (PDNameTree tree, const char *key, ASInt32 keyLen, void *clientData),
	 (tree, key, keyLen, cell->clientData))

/**
	Sent when the page direction of the
	document changes. The page direction
	is determined by the Direction key
	in the ViewerPreferences dictionary.
	@param doc The document
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE(PDDocPageDirectionDidChange,
	(PDDoc doc),
	(PDDoc doc, void* clientData),
	(doc, cell->clientData))


/**
	The contents of a popup note have change but have not yet been 
	set in the PDAnnot, thus the change is intermediate. The contents
	will be changed when input focus is lost by the popup note.

	@param doc		The AVDoc in which the annot lives
	@param annot	The PDAnnot representing the popup (possibly the parent of the popup)
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE( AVDocPopupContentsWillChange, ( AVDoc doc, PDAnnot annot ), ( AVDoc doc, PDAnnot annot, void *clientData ), ( doc, annot, cell->clientData ) )

/**
	The visibility of the AVPanel with the specified name changed.

	@param doc		The AVDoc for which the panel's visiblity changed. If NULL, the panel is floating
	@param name		The const char* name of the panel for which the visibility has changed
	@param visible	The new visiblity state (true, for visible, false otherwise)
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
*/
POKE( AVAppPanelVisibilityDidChangeForAVDoc, ( AVDoc doc, const char* name, ASBool visible ), ( AVDoc doc, const char* name, ASBool visible, void *clientData ), ( doc, name, visible, cell->clientData ) )

/**
	The XMP metadata describing the document as a whole has changed

	@param pdDoc  The PDDoc whose describing XMP metadata has changed.
	@param newMetadata A serialized representation of the new describing XMP metadata.
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see PDDocSetXAPMetadata
*/
POKE( PDDocXAPMetadataDidChange,
	  (PDDoc pdDoc, ASText newMetadata),
	  (PDDoc pdDoc, ASText newMetadata, void *clientData),
	  (pdDoc, newMetadata, cell->clientData))

/**
	The XMP metadata describing the object represented by a Cos dictionary
	has changed

	@param dict The Cos dictionary or stream representing an object whose describing XMP metadata has changed.
	@param newMetadata A serialized representation of the new describing XMP metadata
	@param clientData Pointer to a block of user-supplied 
	data that was passed when the client registered for this 
	notification using AVAppRegisterNotification.
	@see CosDictSetXAPMetadata
*/
POKE( CosDictXAPMetadataDidChange,
	  (CosObj dict, ASText newMetadata),
	  (CosObj dict, ASText newMetadata, void *clientData),
	  (dict, newMetadata, cell->clientData))

/**
   The XMP metadata describing a marked content sequence has changed.

   @param container The PDEContainer representing a marked content sequence  whose describing XMP metadata has changed.
   @param newMetadata A serialized representation of the new describing XMP metadata
   @see PDEContainerSetXAPMetadata

*/
/*
POKE( PDEContainerXAPMetadataDidChange,
	  (PDEContainer container, ASText newMetadata),
	  (PDEContainer container, ASText newMetadata, void *clientData),
	  (container, newMetadata, cell->clientData))
	
*/
