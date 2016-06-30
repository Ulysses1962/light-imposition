/***********************************************************************/
/*                                                                     */
/* SPMacResourceChain.h                                                */
/*                                                                     */
/* Copyright 2003 Adobe Systems Incorporated.                          */
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


// The contents of this file are only intended for the Mac platform
#ifdef MAC_ENV


#ifndef __SPMacResourceChain__
#define __SPMacResourceChain__


/*******************************************************************************
 **
 **	Imports
 **
 **/

#include <Errors.h>
#include <Files.h>


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 **
 **	Constants
 **
 **/

#define kSPMacResourceChainSuite			"SP Mac Resource Chain Suite"
#define kSPMacResourceChainSuiteVersion		1


/*******************************************************************************
 **
 **	Suite
 **
 **/

typedef struct SPMacResourceChainSuite {

	SPAPI OSErr (*OpenResChain)( FSSpec *spec, SignedByte permission, SPMacResChain *resources );
	SPAPI void (*CloseResChain)( SPMacResChain *resources );
	SPAPI void (*CurResChain)( SPMacResChain *resources );
	SPAPI void (*UseResChain)( SPMacResChain *resources );

} SPMacResourceChainSuite;


SPAPI OSErr SPOpenResChain( FSSpec *spec, SignedByte permission, SPMacResChain *resources );
SPAPI void SPCloseResChain( SPMacResChain *resources );
SPAPI void SPCurResChain( SPMacResChain *resources );
SPAPI void SPUseResChain( SPMacResChain *resources );


/*******************************************************************************
 **
 **	Errors
 **
 **/

#include "SPErrorCodes.h"

#ifdef __cplusplus
}
#endif

#endif	// __SPMacResourceChain__


#endif	// MAC_ENV
