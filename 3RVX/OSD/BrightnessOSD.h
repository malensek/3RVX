// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "OSD.h"

class BrightnessOSD : public OSD {
public:
    BrightnessOSD();

    virtual void Hide();
    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    MeterWnd _mWnd;

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};