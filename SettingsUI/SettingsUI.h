#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "UIUtils.h"

static const UINT WM_3RVX_SETTINGSCTRL
    = RegisterWindowMessage(L"WM_3RVX_SETTINGSCTRL");

class CSettingsUIApp : public CWinApp {
public:
    CSettingsUIApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()

private:
    HANDLE _mutex;
};

extern CSettingsUIApp theApp;