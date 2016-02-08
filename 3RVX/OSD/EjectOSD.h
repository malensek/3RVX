// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

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

    void EjectDrive(std::wstring driveLetter);

    DWORD DriveLetterToMask(wchar_t letter);
    wchar_t MaskToDriveLetter(DWORD mask);

    virtual void OnDisplayChange();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};