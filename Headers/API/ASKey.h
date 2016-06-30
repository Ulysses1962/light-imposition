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

 ASKey.h

 - Definition of standard key codes for Mac and Windows.

*********************************************************************/

#ifndef _H_ASKey
#define _H_ASKey

#include "Environ.h"
#if WIN_PLATFORM
#include <windows.h>
#endif

#define ASKEY_ESCAPE	27
#define ASKEY_SPACE		32
#define ASKEY_TAB		9
#define ASKEY_ARROW_R	29
#define ASKEY_ARROW_D	31
#define ASKEY_ARROW_L	28
#define ASKEY_ARROW_U	30
#define ASKEY_PAGE_U	11
#define ASKEY_PAGE_D	12
#define ASKEY_HELP		5

#if WIN_PLATFORM
#define ASKEY_ENTER		13
#define ASKEY_DEL		127	/* Deprecated. Please use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE */
#define ASKEY_RIGHT_DELETE 127
#define ASKEY_BACKSPACE	8
#define ASKEY_HOME		4
#define ASKEY_END		1
#define ASKEY_MENU		2
#endif

#if MAC_PLATFORM
#define ASKEY_ENTER		3
#define ASKEY_DEL		8	/* Deprecated. Please use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE */
#define ASKEY_RIGHT_DELETE 127
#define ASKEY_BACKSPACE	8
#define ASKEY_HOME		1
#define ASKEY_END		4
#define ASKEY_CR		13
#define ASKEY_CLEAR		27
#endif

#if UNIX_PLATFORM
#define ASKEY_ENTER             10
#define ASKEY_CR                13
#define ASKEY_CLEAR             27
#define ASKEY_DEL				8	/* Deprecated. Please use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE */
#define ASKEY_RIGHT_DELETE 		127
#define ASKEY_BACKSPACE			8
#define ASKEY_HOME              1
#define ASKEY_END               4

#define ASKEY_MENU              2
#define ASKEY_INSERT            17

#endif

#endif /* _H_ASKey */
