// LightImposition.h : main header file for the LightImposition DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLightImpositionApp
// See LightImposition.cpp for the implementation of this class
//

class CLightImpositionApp : public CWinApp
{
public:
	CLightImpositionApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
