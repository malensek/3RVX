#pragma once

#include <Windows.h>

#include "MeterWnd\MeterWnd.h"
#include "Settings.h"

class OSD {
public:
    OSD(HINSTANCE hInstance, std::wstring className, Settings &settings);

    void Hide();
    void Show();

protected:
    HWND _hWnd;
    HWND _masterWnd;
    Settings &_settings;

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};