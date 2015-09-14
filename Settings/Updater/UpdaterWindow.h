// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../../3RVX/Window.h"
#include "Version.h"

class NotifyIcon;
class Settings;

class UpdaterWindow : public Window {
public:
    UpdaterWindow();
    ~UpdaterWindow();

    void DoModal();

private:
    HICON _smallIcon;
    HICON _largeIcon;
    NotifyIcon *_notifyIcon;

    HMENU _menu;
    UINT _menuFlags;

    Settings *_settings;

    Version _version;

    void InstallUpdate();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    static const int MENU_INSTALL = 0;
    static const int MENU_IGNORE = 1;
    static const int MENU_REMIND = 2;

};