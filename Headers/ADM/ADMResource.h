/***********************************************************************/
/*                                                                     */
/* ADMResource.h                                                       */
/* ADM Resource IDs                                                    */
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
/*                                                                     */
/***********************************************************************/

#ifndef __ADMResource__
#define __ADMResource__


// -----------------------------------------------------------------------------
//	ADM picture IDs

#define kADMAttentionPictureID					-1000
#define kADMErrorPictureID						-1001

#define kADMSliderPictureID						-16000
#define kADMSliderSelectedPictureID				-16001
#define kADMSliderDisabledPictureID				-16002
#define kADMNewEntryPictureID					-16010
#define kADMNewEntryDisabledPictureID			-16012
#define kADMDeleteEntryPictureID				-16020
#define kADMDeleteEntryDisabledPictureID		-16022
#define kADMUpSpinArrowPictureID				-16040
#define kADMUpSpinArrowDisabledPictureID		-16042
#define kADMDownSpinArrowPictureID				-16050
#define kADMDownSpinArrowDisabledPictureID		-16052
#define kADMLeftSpinArrowPictureID				-16060
#define kADMLeftSpinArrowDisabledPictureID		-16062
#define kADMRightSpinArrowPictureID				-16070
#define kADMRightSpinArrowDisabledPictureID		-16072
#define kADMRightPopupArrowPictureID			-16080
#define kADMRightPopupArrowDisabledPictureID	-16082
#define kADMDownPopupArrowPictureID				-16090
#define kADMDownPopupArrowDisabledPictureID		-16092
#define kADMHierarchyListExpandPictureID		-16094
#define kADMHierarchyListCollapsePictureID		-16095
		

// -----------------------------------------------------------------------------
//	ADM cursor IDs

#define kADMIBeamCursorID						-1
#define kADMCrossCursorID						-2
#define kADMWaitCursorID						-3
#define kADMArrowCursorID						-4
#define kADMCancelCursorID						-5
#define kADMFingerCursorID						-6
#define kADMFistCursorID						-7
#define kADMFistPlusCursorID					-8
#define kADMHostControlsCursorID				-9

#define kADMSmallestCursorID					kADMHostControlsCursorID
#define kADMLargestCursorID						kADMIBeamCursorID


// -----------------------------------------------------------------------------
//	OR this flag with the dialog style of a Windows dialog resource and the
//	dialog's units are read as Pixel Units instead of Dialog Units.

#define kADMPixelUnitsDialogStyle				0x0020		


// -----------------------------------------------------------------------------
//	Popup Control Dialog IDs

#define kADMPopupSliderDialogID					-2000		//1900 - 2000 reserved for ADM Sliders
#define kADMPopupDialDialogID					-2010       //2001 - 2010 reserved for ADM Dials 


#endif	// __ADMResource__
