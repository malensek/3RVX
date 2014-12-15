#pragma once

#include <Windows.h>

#include "OSD.h"

class EjectOSD : public OSD {
public:
    EjectOSD(Settings &settings);

    virtual void Hide();

private:
    MeterWnd *_mWnd;

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};