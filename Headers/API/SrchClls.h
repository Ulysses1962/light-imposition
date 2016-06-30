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

 SrchClls.h

*********************************************************************/

#ifndef _H_SrchClls
#define _H_SrchClls

#ifdef DEBUG_EXTENSIONS
#undef PRODUCT
#define PRODUCT "Plugin.h"
#endif

#include "SrchType.h"
#include "SrchHFT.h"

#define SearchExecuteQuery (*((SearchExecuteQuerySELPROTO)(gSearchHFT[SearchExecuteQuerySEL])))
#define SearchGetIndexList (*((SearchGetIndexListSELPROTO)(gSearchHFT[SearchGetIndexListSEL])))
#define SearchCountIndexList (*((SearchCountIndexListSELPROTO)(gSearchHFT[SearchCountIndexListSEL])))
#define SearchGetNthIndex (*((SearchGetNthIndexSELPROTO)(gSearchHFT[SearchGetNthIndexSEL])))
#define SearchGetIndexByPath (*((SearchGetIndexByPathSELPROTO)(gSearchHFT[SearchGetIndexByPathSEL])))
#define SearchAddIndex (*((SearchAddIndexSELPROTO)(gSearchHFT[SearchAddIndexSEL])))
#define SearchRemoveIndex (*((SearchRemoveIndexSELPROTO)(gSearchHFT[SearchRemoveIndexSEL])))
#define SearchGetIndexFlags (*((SearchGetIndexFlagsSELPROTO)(gSearchHFT[SearchGetIndexFlagsSEL])))
#define SearchSetIndexFlags (*((SearchSetIndexFlagsSELPROTO)(gSearchHFT[SearchSetIndexFlagsSEL])))
#define SearchGetIndexTitle (*((SearchGetIndexTitleSELPROTO)(gSearchHFT[SearchGetIndexTitleSEL])))
#define SearchGetIndexPath (*((SearchGetIndexPathSELPROTO)(gSearchHFT[SearchGetIndexPathSEL])))

#define SearchGetIndexByPathEx (*((SearchGetIndexByPathExSELPROTO)(gSearchHFT[SearchGetIndexByPathExSEL])))
#define SearchAddIndexEx (*((SearchAddIndexExSELPROTO)(gSearchHFT[SearchAddIndexExSEL])))
#define SearchGetIndexTitleEx (*((SearchGetIndexTitleExSELPROTO)(gSearchHFT[SearchGetIndexTitleExSEL])))
#define SearchGetIndexPathEx (*((SearchGetIndexPathExSELPROTO)(gSearchHFT[SearchGetIndexPathExSEL])))
#define SearchGetIndexFileSys (*((SearchGetIndexFileSysSELPROTO)(gSearchHFT[SearchGetIndexFileSysSEL])))

#define SearchIsLegacySearchAvailable (*((SearchIsLegacySearchAvailableSELPROTO)(gSearchHFT[SearchIsLegacySearchAvailableSEL])))
#define SearchExecuteQueryEx  (*((SearchExecuteQueryExSELPROTO)(gSearchHFT[SearchExecuteQueryExSEL])))

#endif


