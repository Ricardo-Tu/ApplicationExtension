
// ggre.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CggreApp:
// See ggre.cpp for the implementation of this class
//

class CggreApp : public CWinApp
{
public:
	CggreApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CggreApp theApp;
