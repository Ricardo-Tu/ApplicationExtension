
// cs1.6.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"        // main symbols


// Ccs16App:
// See cs1.6.cpp for the implementation of this class
//

class Ccs16App : public CWinApp
{
public:
    Ccs16App();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern Ccs16App theApp;
