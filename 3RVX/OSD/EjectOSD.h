// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <thread>

#include "../DriveInfo.h"
#include "OSD.h"

class NotifyIcon;

class EjectOSD : public OSD {
public:
    EjectOSD();
    ~EjectOSD();

    virtual void Hide();
    void HideIcon();

    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    DWORD _ignoreDrives;
    DWORD _latestDrive;
    MeterWnd _mWnd;

    NotifyIcon *_icon;
    HICON _iconImage;

    HMENU _menu;
    UINT _menuFlags;
    std::vector<DriveInfo> _menuItems;
    std::thread _menuThread;

    void UpdateDriveMenu();

    void EjectDrive(const std::wstring &driveLetter);

    DWORD DriveLetterToMask(wchar_t letter);
    wchar_t MaskToDriveLetter(DWORD mask);

    virtual void OnDisplayChange();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};