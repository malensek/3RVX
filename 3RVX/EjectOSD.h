#pragma once

#include <Windows.h>

#include "OSD.h"
#include "Settings.h"

class EjectOSD : public OSD {
public:
    EjectOSD(HINSTANCE hInstance, Settings &settings);

    virtual void Hide();
    virtual void Show();

private:
    MeterWnd *_mWnd;

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};