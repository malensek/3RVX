// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <thread>

#include "OSD.h"

class NotifyIcon;

class EjectOSD : public OSD {
public:
    EjectOSD();

    virtual void Hide();
    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    DWORD _ignoreDrives;
    DWORD _latestDrive;
    MeterWnd _mWnd;

    NotifyIcon *_icon;
    std::vector<HICON> _iconImages;

    HMENU _menu;
    std::thread _menuThread;

    void UpdateDriveMenu();

    void EjectDrive(std::wstring driveLetter);

    DWORD DriveLetterToMask(wchar_t letter);
    wchar_t MaskToDriveLetter(DWORD mask);
    std::wstring DriveFileName(wchar_t driveLetter);
    std::wstring DriveFileName(std::wstring &driveLetter);


    virtual void OnDisplayChange();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};