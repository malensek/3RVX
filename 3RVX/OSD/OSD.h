#pragma once

#include <Windows.h>

#include "..\MeterWnd\MeterWnd.h"
#include "..\Settings.h"
#include "OSDType.h"

class OSD {
public:
    OSD(std::wstring className, HINSTANCE hInstance = NULL);

    virtual void Hide() = 0;

protected:
    HWND _hWnd;
    HWND _masterWnd;
    Settings &_settings;

    void HideOthers(OSDType except);

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};