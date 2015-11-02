#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <malloc.h>
#include <memory.h>
#include <prsht.h>
#include <stdlib.h>
#include <tchar.h>
#include <vector>

#include "../3RVX/3RVX.h"
#include "resource.h"

class About;
class Display;
class General;
class Hotkeys;
class OSD;
class SettingsTab;

class SettingsUI : public Window {
public:
    SettingsUI(HINSTANCE hInstance);

    INT_PTR LaunchPropertySheet();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    std::vector<SettingsTab *> _tabs;
    General *_general;
    Display *_display;
    OSD *_osd;
    Hotkeys *_hotkeys;
    About *_about;

private:
    /* Startup x/y location offsets */
    static const int XOFFSET = 70;
    static const int YOFFSET = 20;

};

/* Forward Declarations */
int CALLBACK PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam);