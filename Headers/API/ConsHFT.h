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

 ConsHFT.h

 - Catalog of functions exported by PDF Consultant.

*********************************************************************/

#ifndef __CONSHFT_H__
#define __CONSHFT_H__

#include "ConsExpT.h"
#include "ASCalls.h"

/* ****************************** HFT Stuff ************************************** */
#ifdef __cplusplus
extern "C"
{
#endif

extern HFT gConsultantHFT;

#define PDFCONSULTANT_HFT_NAME		"PDFConsultant"
#define PDFCONSULTANT_HFT_VER_1		0x00010000
#define PDFCONSULTANT_HFT_VER_2		0x00020000
#define PDFCONSULTANT_HFT_LATEST	PDFCONSULTANT_HFT_VER_2

#define Init_PDFConsultantHFT ASExtensionMgrGetHFT( ASAtomFromString( PDFCONSULTANT_HFT_NAME ), PDFCONSULTANT_HFT_LATEST )


#if PDFCONSULTANT_HFT_LATEST != 0


/* HFT Function selectors */
enum
{
	ConsultantBAD_SELECTOR = 0,
	
	ConsultantCreateSEL,
	ConsultantDestroySEL,
	ConsultantTraverseFromSEL,
	ConsultantRegisterAgentSEL,
	ConsultantSetStartSEL,
	ConsultantNextObjSEL,
	ConsultantGetPercentDoneSEL,
	ConsultantGetNumDirectVisitedSEL,
	ConsultantGetNumIndirectVisitedSEL,
	ConsultantSuspendSEL,
	ConsultantResumeSEL,
	ConsStackGetCountSEL,
	ConsStackIndexGetObjSEL,
	ConsStackIndexGetTypeCountSEL,
	ConsStackIndexGetTypeAtSEL,
	ConsStackIndexIsDictSEL,
	ConsStackIndexIsArraySEL,
	ConsStackIndexGetDictKeySEL,
	ConsStackIndexGetArrayIndexSEL,
	PDFObjTypeGetSuperclassSEL,
	ConsultantGetNumUniqueIndirectsVisitedSEL,

	ConsultantNUMSELECTORSPlusOne
};

#define ConsultantNUMSELECTORS ( ConsultantNUMSELECTORSPlusOne - 1 )

/**
	Allocates and intializes a new Consultant object. Use the 
	returned object to call the other Consultant API functions. 
	When you are finished with this object, you must destroy 
	it using the ConsultantDestroy function. 
	@param pPercentDoneCallBack A function pointer to be called back with progress 
	updates. May be NULL.
	@return The Consultant object that was created. 
	@exception Raises an Acrobat exception on failure. 
	@see ConsultantDestroy 
*/
typedef ACCBPROTO1 Consultant ( ACCBPROTO2 *ConsultantCreateSELPROTO )( ConsAgentPercentDoneCallback pPercentDoneCallBack );
#define ConsultantCreate ( *( ( ConsultantCreateSELPROTO )( gConsultantHFT[ ConsultantCreateSEL ] ) ) )

/* ConsultantDestroy

   Halts traversal by the given Consultant, detatches all Agents, and destroys it.
   Must be called on all Consultants created via ConsultantCreate.

   Raises on failure.
*/
/**
	Detaches all Agents and destroys the given Consultant object, 
	invalidating its handle. You must never call this on a Consultant 
	that is currently running. 
	@param hConsultantToDestroy A valid Consultant object 
	handle as returned by ConsultantCreate. Handle is invalid 
	after the call returns.
	@exception Raises an Acrobat exception on failure. 
*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantDestroySELPROTO )( Consultant hConsultantToDestroy );
#define ConsultantDestroy ( *( ( ConsultantDestroySELPROTO )( gConsultantHFT[ ConsultantDestroySEL ] ) ) )

/**
	Starts the given Consultant object traversing at the given 
	Cos object. It traverses and processes all objects beneath 
	obj, classifying the type of objects based on the fact that 
	obj is of the given ObjType. 

	It is never legal to destroy a Consultant object that is 
	currently executing a call to ConsultantTraverseFrom. To 
	properly destroy a running Consultant, you must call ConsultantSuspend 
	first. ConsultantTraverseFrom raises an exception under 
	any other conditions, and may also raise an exception as 
	the result of a registered Agent's raising an exception 
	during the operation. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate. The Consultant with which the Agent 
	will be registered. 
	@param hObj Object at which to start traversal. 
	@param kObjType The object type of the specified start 
	object. May be PT_NULL, in which case the Consultant attempts 
	to determine the type of the object itself. You should specify 
	a value other than PT_NULL whenever possible.
	@exception Raises an Acrobat exception if the Consultant has been started 
	and is not in a suspended state. 

*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantTraverseFromSELPROTO )( Consultant hConsultant, CosObj hObj, PDFObjType kObjType );
#define ConsultantTraverseFrom ( *( ( ConsultantTraverseFromSELPROTO )( gConsultantHFT[ ConsultantTraverseFromSEL ] ) ) )

/**
	Suspends the Consultant, even if it is currently executing 
	a call to ConsultantCreate or ConsultantResume. This function 
	causes currently executing calls to ConsultantTraverseFrom 
	to return. It is legal to call this function from within 
	the ScrubPercentDoneCallback passed to the Consultant on 
	ConsultantCreate. Calls to ConsultantTraverseFrom that are 
	currently in progress will return when ConsultantSuspend 
	is called. 

	To resume, call ConsultantResume. 

	- You can call ConsultantNextObj on a suspended Consultant, 
	which removes the suspension and causes the Consultant to 
	process the next object. 
	- You can destroy a Consultant that has been suspended. 
	- If you call ConsultantTraverseFrom on a suspended Consultant 
	it will reset the operation of the Consultant, but the Consultant 
	will remain in a suspended state and will not process the 
	document further. 

	This function does nothing if you call it on a Consultant 
	object that is already suspended, or was never started. 
	
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantSuspendSELPROTO )( Consultant hConsultant );
#define ConsultantSuspend ( *( ( ConsultantSuspendSELPROTO )( gConsultantHFT[ ConsultantSuspendSEL ] ) ) )

/**
	Resumes a previously suspended Consultant at the point in 
	the traversal where it stopped. This function does not return 
	from traversing and notifying Agents until the traversal 
	is complete or ConsultantSuspend is called. The function 
	does nothing if the Consultant object is already running 
	or has not been started. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return None. 
*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantResumeSELPROTO )( Consultant hConsultant );
#define ConsultantResume ( *( ( ConsultantResumeSELPROTO )( gConsultantHFT[ ConsultantResumeSEL ] ) ) )

/**
	Registers the given agent with the given consultant, so 
	that the agent is called when the consultant encounters 
	objects of interest. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate. The Consultant with which the Agent 
	will be registered. 
	@param pAgent The Agent to register, of a 
	type derived from the ConsultantAgentObj base class. 
	@param kFlag Flag indicating the mode that the Consultant 
	should operate in. 
	@return None. 
	@exception Raises an Acrobat exception if the Consultant has been started 
	and is not in a suspended state. 
*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantRegisterAgentSELPROTO )( Consultant hConsultant, const ConsultantAgent* pAgent, RegAgentFlag kFlag );
#define ConsultantRegisterAgent ( *( ( ConsultantRegisterAgentSELPROTO )( gConsultantHFT[ ConsultantRegisterAgentSEL ] ) ) )

/**
	Resets the suspended Consultant and starts a new traversal 
	from the given starting object. 

	If you do not know the type of the object, the Consultant 
	will attempt to determine it. This function does not return 
	until the entire path beneath the starting object has been 
	traversed. The Consultant passes to the registered Agents 
	all objects it encounters that have been registered as interesting. 
	
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate. The Consultant with which the Agent 
	will be registered. 
	@param hObjStart Object at which to restart traversal. 
	Usually, for traversing an entire document, this is the 
	Catalog. 
	@param kInitType The object type of the specified start 
	object. May be PT_NULL, in which case the Consultant attempts 
	to determine the type of the object itself. You should specify 
	a value other than PT_NULL whenever possible In most cases, 
	for traversing the entire document, the starting object 
	is the Catalog so the type is PT_CATALOG.
	@exception Raises an Acrobat exception if the Consultant has been started 
	and is not in a suspended state. 
*/
typedef ACCBPROTO1 void ( ACCBPROTO2 *ConsultantSetStartSELPROTO )( Consultant hConsultant, CosObj hObjStart, PDFObjType kInitType );
#define ConsultantSetStart ( *( ( ConsultantSetStartSELPROTO )( gConsultantHFT[ ConsultantSetStartSEL ] ) ) )

/**
	Instructs the Consultant to process the next object in the 
	current traversal. Assumes that the Consultant has been 
	suspended and reset with calls to ConsultantSuspend and 
	ConsultantSetStart. This function does not unsuspend a Consultant, 
	so you can call it repeatedly. It returns after all registered 
	Agents have processed the object. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return true if the process is done or there has been a problem, 
	false otherwise. 
	@exception Raises an Acrobat exception if you call it on a running 
	Consultant. 
*/
typedef ACCBPROTO1 ASBool ( ACCBPROTO2 *ConsultantNextObjSELPROTO )( Consultant hConsultant );
#define ConsultantNextObj ( *( ( ConsultantNextObjSELPROTO )( gConsultantHFT[ ConsultantNextObjSEL ] ) ) )

/**
	Returns an estimate (from 0 - 100) of what percentage of 
	the current document has been processed by the Consultant. 
	You can call this function at any time. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return Number between 0 - 100. 
*/
typedef ACCBPROTO1 ASReal ( ACCBPROTO2 *ConsultantGetPercentDoneSELPROTO )( Consultant hConsultant );
#define ConsultantGetPercentDone ( *( ( ConsultantGetPercentDoneSELPROTO )( gConsultantHFT[ ConsultantGetPercentDoneSEL ] ) ) )

/**
	Returns the number of direct objects that the Consultant 
	has processed so far. This count may include some objects 
	twice, depending on revisitation of objects.This count is 
	reset on calls to ConsultantTraverseFrom and ConsultantSetStart.	
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return The number of direct objects the Consultant has visited 
	so far. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsultantGetNumDirectVisitedSELPROTO )( Consultant hConsultant );
#define ConsultantGetNumDirectVisited ( *( ( ConsultantGetNumDirectVisitedSELPROTO )( gConsultantHFT[ ConsultantGetNumDirectVisitedSEL ] ) ) )

/**
	Returns the number of indirect objects that the Consultant 
	has processed so far. This count may include some objects 
	twice, depending on revisitation of objects.This count is 
	reset on calls to ConsultantTraverseFrom and ConsultantSetStart.
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return The number of indirect objects the Consultant has visited 
	so far. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsultantGetNumIndirectVisitedSELPROTO )( Consultant hConsultant );
#define ConsultantGetNumIndirectVisited ( *( ( ConsultantGetNumIndirectVisitedSELPROTO )( gConsultantHFT[ ConsultantGetNumIndirectVisitedSEL ] ) ) )

/**
	Returns the number of unique indirect objects that the Consultant 
	has processed so far. This count is reset on calls to ConsultantTraverseFrom 
	and ConsultantSetStart. Visited objects are not counted 
	more than once; if an object is revisited, the count is 
	not incremented. 
	@param hConsultant A valid Consultant object handle as returned 
	by ConsultantCreate.
	@return The number of unique indirect objects the Consultant has 
	visited so far. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsultantGetNumUniqueIndirectsVisitedSELPROTO )( Consultant hConsultant );
#define ConsultantGetNumUniqueIndirectsVisited ( *( ( ConsultantGetNumUniqueIndirectsVisitedSELPROTO )( gConsultantHFT[ ConsultantGetNumUniqueIndirectsVisitedSEL ] ) ) )

/**
	Returns the number of objects currently on Consultant's 
	traversal stack. The stack includes the objects that the 
	Consultant has visited on its path to the current object, 
	or, in other words, all parents of the current object, but 
	not the object itself. 
	@param hConsultant The Consultant's traversal stack.
	@return The number of objects on the Consultant.'s traversal stack. 
	@exception Raises an Acrobat exception on error. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsStackGetCountSELPROTO )( ConsStack hConsultant );
#define ConsStackGetCount ( *( ( ConsStackGetCountSELPROTO )( gConsultantHFT[ ConsStackGetCountSEL ] ) ) )

/**
	Gets the size of the type hierarchy at the given index into 
	the stack. 
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex The object in question.
	@return Size of the type hierarchy. 
	Tests whether the given index into the stack is a CosArray. 	
	@return true if the object found at the index point is an array, 
	false otherwise. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsStackIndexGetTypeCountSELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexGetTypeCount ( *( ( ConsStackIndexGetTypeCountSELPROTO )( gConsultantHFT[ ConsStackIndexGetTypeCountSEL ] ) ) )

/**
	Gets a type from the type array at each index in the stack. 
	Since there are potentially multiple types for each object, 
	you can access the type classifications one at a time. 
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex The position in the stack of the object in 
	question. 
	@param iTypeIndex The type classification of the object. 
	0 is the most specific type classification. The higher the 
	number, the more general the type classification.
	@return One type of an object at a particular location in the traversal 
	stack. 
*/
typedef ACCBPROTO1 PDFObjType ( ACCBPROTO2 *ConsStackIndexGetTypeAtSELPROTO )( ConsStack hConsultant, ASUns32 iIndex, ASUns32 iTypeIndex );
#define ConsStackIndexGetTypeAt ( *( ( ConsStackIndexGetTypeAtSELPROTO )( gConsultantHFT[ ConsStackIndexGetTypeAtSEL ] ) ) )

/**
	Gets the the Cos object at the given index into the stack. 
	
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex Point at which to find the object.
	@return The object at the specified point in the Consultant's traversal 
	stack. 
*/
typedef ACCBPROTO1 CosObj ( ACCBPROTO2 *ConsStackIndexGetObjSELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexGetObj ( *( ( ConsStackIndexGetObjSELPROTO )( gConsultantHFT[ ConsStackIndexGetObjSEL ] ) ) )

/**
	Tests whether the object at the given index into the stack 
	is a CosDict object. 
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex Index in the stack where the object in question 
	is located.
	@return true if the object found at the index point is a dictionary, 
	false otherwise. 
*/
typedef ACCBPROTO1 ASBool ( ACCBPROTO2 *ConsStackIndexIsDictSELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexIsDict ( *( ( ConsStackIndexIsDictSELPROTO )( gConsultantHFT[ ConsStackIndexIsDictSEL ] ) ) )

/**
	Tests whether the given index into the stack is a CosArray. 
	
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex Index in the stack where the object in question 
	is located.
	@return true if the object found at the index point is an array, 
	false otherwise. 
*/
typedef ACCBPROTO1 ASBool ( ACCBPROTO2 *ConsStackIndexIsArraySELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexIsArray ( *( ( ConsStackIndexIsArraySELPROTO )( gConsultantHFT[ ConsStackIndexIsArraySEL ] ) ) )

/**
	Gets the key string atom of the object at the given index 
	into the stack (that is, the key that led from the given 
	object to the next object in the traversal). It is only 
	valid to call this function on an index if ConsStackIndexIsDict 
	returns true for that index. 
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex Index in the stack where the object in question 
	is located.
	@return The key that led from the object at the given index in the 
	stack to the next object in the Consultant's traversal path. 
	
	@exception Raises an Acrobat exception on error. 
*/
typedef ACCBPROTO1 ASAtom ( ACCBPROTO2 *ConsStackIndexGetDictKeySELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexGetDictKey ( *( ( ConsStackIndexGetDictKeySELPROTO )( gConsultantHFT[ ConsStackIndexGetDictKeySEL ] ) ) )

/**
	Get the array index of the object at the given index into 
	the stack (that is, the index that led from the given object 
	to the next object in the traversal). It is only valid to 
	call this function on an index if ConsStackIndexIsArray 
	returns true for that index. 
	@param hConsultant The Consultant's traversal stack. 
	@param iIndex Index in the stack where the object in question 
	is located.
	@return The array index that led from the object at the given index 
	in the stack to the next object in the Consultant's traversal 
	path. 
*/
typedef ACCBPROTO1 ASUns32 ( ACCBPROTO2 *ConsStackIndexGetArrayIndexSELPROTO )( ConsStack hConsultant, ASUns32 iIndex );
#define ConsStackIndexGetArrayIndex ( *( ( ConsStackIndexGetArrayIndexSELPROTO )( gConsultantHFT[ ConsStackIndexGetArrayIndexSEL ] ) ) )

/**
	Gets the superclass, if any, of the given PDFObjType. 
	@param kType The type that might have a superclass.
	@return The superclass of the given type or DT_NULL if no superclass 
	exists. 
*/
typedef ACCBPROTO1 PDFObjType ( ACCBPROTO2 *PDFObjTypeGetSuperclassSELPROTO )( PDFObjType kType );
#define PDFObjTypeGetSuperclass ( *( ( PDFObjTypeGetSuperclassSELPROTO )( gConsultantHFT[ PDFObjTypeGetSuperclassSEL ] ) ) )

#endif /* PDFCONSULTANT_HFT_LATEST != 0 */

#ifdef __cplusplus
}
#endif

#endif /* __CONSHFT_H__ */
