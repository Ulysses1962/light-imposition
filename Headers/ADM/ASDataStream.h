#ifndef __ASDataStream__
#define __ASDataStream__

/***********************************************************************/
/*                                                                     */
/* Copyright 1996-2003 Adobe Systems Incorporated.                     */
/* All Rights Reserved.                                                */
/*                                                                     */
/* Patents Pending                                                     */
/*                                                                     */
/* NOTICE: All information contained herein is the property of Adobe   */
/* Systems Incorporated. Many of the intellectual and technical        */
/* concepts contained herein are proprietary to Adobe, are protected   */
/* as trade secrets, and are made available only to Adobe licensees    */
/* for their internal use. Any reproduction or dissemination of this   */
/* software is strictly forbidden unless prior written permission is   */
/* obtained from Adobe.                                                */
/*                                                                     */
/***********************************************************************/


/*******************************************************************************
 **
 **	Imports
 **
 **/

#ifndef __ASTypes__
#include "ASTypes.h"
#endif

#ifndef __ASNameSpace__
#include "ASNamespace.h"
#endif

#ifndef _SPFiles__
#include "SPFiles.h"
#endif

#ifndef __SPPlugins__
#include "SPPlugs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


/*******************************************************************************
 **
 **	Constants
 **
 **/

#define kASDataStreamSuite		"AS Data Filter Suite"
#define kASDataStreamSuiteVersion	2


#define kASDataStreamErr			'DFER'


/*******************************************************************************
 **
 **	Types
 **
 **/
 
#ifndef ASDataStreamRef
typedef struct t_ASDataStream *ASDataStreamRef;
#endif

/*******************************************************************************
 **
 **	Suite
 **
 **/

typedef struct {

	ASAPI ASErr (*LinkDataStream) ( ASDataStreamRef prev, ASDataStreamRef next );
	ASAPI ASErr (*UnlinkDataStream) ( ASDataStreamRef next, ASDataStreamRef *prev );
	ASAPI ASErr (*ReadDataStream) ( ASDataStreamRef filter, char *store, long *count );
	ASAPI ASErr (*WriteDataStream) ( ASDataStreamRef filter, char *store, long *count );
	ASAPI ASErr (*SeekDataStream) ( ASDataStreamRef filter, long *count );
	ASAPI ASErr (*MarkDataStream) ( ASDataStreamRef filter, long *count );
	ASAPI ASErr (*NewFileDataStream) ( SPPlatformFileSpecification *spec, char *mode, long creator, long type, ASDataStreamRef *filter );
	ASAPI ASErr (*NewBufferDataStream) ( long size, ASDataStreamRef *filter );
	ASAPI ASErr (*NewHexdecDataStream) ( char *state, long shift, ASDataStreamRef *filter );
	ASAPI ASErr (*NewBlockDataStream) ( void *address, long size, ASDataStreamRef *filter );
	ASAPI ASErr (*NewResourceDataStream) ( SPPluginRef plugin, long type, long id, char *name, ASDataStreamRef *filter);

} ASDataStreamSuite;


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif
