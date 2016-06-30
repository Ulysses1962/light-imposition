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

 MacPlatform.h

 - PLATFORM file for Macintosh development.

*********************************************************************/

#ifndef _H_MacPlatform
#define _H_MacPlatform

#define AS_LITTLEENDIAN 0
#define IEEEFLOAT 0
#define IEEESOFT 0
#define UNSIGNEDCHARS 0

#define ACCB1 pascal
#define ACCB2

#define ACEX1 pascal
#define ACEX2

#define ACCBPROTO1 pascal
#define ACCBPROTO2

#define MAC_PLATFORM 1

/*
//	I have no idea why this code was here in the first place,
//	since I can't figure out what it could ever do.  But I've
//	left it in in case someone far smarter than I can see a use
//	for it.

#if DEBUG
 	#ifndef DEBUG
 		#define DEBUG 1
 	#endif
#endif
*/

#endif /* defined(_H_MacPlatform) */

