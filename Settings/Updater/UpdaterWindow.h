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

    void CreateMenu();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    std::wstring _availableStr = L"Update Available";
    std::wstring _updateVersStr = L"3RVX {1}";
    std::wstring _menuInstallStr = L"Install";
    std::wstring _menuNotesStr = L"Release Notes";
    std::wstring _menuIgnoreStr = L"Ignore version";
    std::wstring _menuRemindStr = L"Remind me later";

    static const int MENU_INSTALL = 0;
    static const int MENU_IGNORE = 1;
    static const int MENU_REMIND = 2;

};