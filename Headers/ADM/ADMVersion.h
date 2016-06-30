/***********************************************************************/
/*                                                                     */
/* ADMVersion.h                                                        */
/* ADM Version Number                                                  */
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

#ifndef __ADMVERSION_H__
#define __ADMVERSION_H__


// 3/20/00 dhearst - We are beginning a new method for describing the version
// of a particular build. First of all, the "x" delimiter for the build number
// will be replaced by "m" for main. When we branch for a particular product,
// this delimiter will be a branch descriptor (e.g. "ai" or "ps). In addition,
// our plan is to to prepend "10" to the build number for any build that is
// not on the main branch. We are still working out the way this will be
// reflected in the binary representation of the version, and what is
// returned by GetADMVersion().
//
// EX) last build before branch for Illustrator was 2.80x35
//		---> first build on main branch after 2.80x35 is 2.80m36
//              ---> first build on Illustrator branch after 2.80x35 is 2.80ai1036


// -----------------------------------------------------------------------------
//	The version number is coded as BCD. It is available as symbols for each
//	component, a string, or a single long-int value. Be sure to update all
//	three in sync!

// The following denotes version 2.82:

#define ADMVMajor 0x02
#define ADMVMinor 0x82

// ADMVBuildType of 0x80 denotes a release stage of "final" and has nothing to
// do with the version number. (See the definition of the 'vers' resource in
// MacTypes.r.)

#define ADMVBuildType 0x80

// The build number is not BCD; it's just a straight number.

#define ADMVBuildNumber 21

// ADMVFull is needed for the windows PiPL file. Here, the entire build number
// is expressed in BCD.

#define ADMVFull 0x000282021L

#define ADMVString "2.82ac21"


#endif	// __ADMVERSION_H__
