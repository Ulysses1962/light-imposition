/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PIHeaders.h

 - A general include file for Acrobat Headers. This file is included
   in the Macintosh precompiled header file "PIHeaders.pch".

*********************************************************************/
#ifndef _H_PIHEADERS
#define _H_PIHEADERS

#define NEW_PDFEDIT_HFTS 1
#define NEW_PDSEDIT_HFTS 1
#define PDMETADATA_HFT 1

#ifdef WIN_PLATFORM
#include "WinPIHeaders.h"
#endif
#ifdef MAC_PLATFORM
#include "MacPIHeaders.h"
#include "SafeResources.h"
#endif

#include "PIExcept.h"

#include "ASTypes.h"
#include "AcroColorCalls.h"
#include "ASCalls.h"
#include "ASExtraCalls.h"
#include "AVCalls.h"
#include "CorCalls.h"
#include "CosCalls.h"
#include "DigSigHFT.h"
#include "FormsHFT.h"
#include "PagePDECntCalls.h"
#include "PDCalls.h"
#include "PDMetadataCalls.h"
#include "PDSReadCalls.h"
#include "PDSWriteCalls.h"
#include "PERCalls.h"
#include "PEWCalls.h"
#include "PSFCalls.h"
#include "SpellerHFT.h"
#include "SrchHFT.h"
#include "TtsHFT.h"
#include "WLHFT.h"
#include "CatHFT.h"
#include "ConsHFT.h"


// Acrobat 6.0 Headers
#include "AcroColorCalls.h"
#include "ASKey.h"
#include "CAVAlert.h"
#include "PSFCalls.h"
#include "PubSecHFT.h"

#ifdef WIN_PLATFORM
#include "WinCalls.h"
#endif
#ifdef MAC_PLATFORM
#include "MacCalls.h"
#endif



#ifdef UNIX_PLATFORM
#include "UnixCalls.h"
#endif

#ifndef UNIX_PLATFORM /* No ADM on Unix */
#include "AVSPCalls.h"
#include "ADMAcroSDK.h"
#endif

#endif
