// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "OSD.h"

class KeyboardOSD : public OSD {
public:
    KeyboardOSD();
    ~KeyboardOSD();

    virtual void Hide();
    void HideIcon();

    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    MeterWnd _mWnd;

    virtual void OnDisplayChange();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};
