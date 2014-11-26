#pragma once

#include <Windows.h>

#include "MeterWnd\Animations\FadeOut.h"
#include "MeterWnd\MeterWnd.h"
#include "Settings.h"

class OSD {
public:
    OSD(HINSTANCE hInstance, std::wstring className, Settings &settings);

    void Hide();
    void Show();

protected:
    Animation *_animIn;
    Animation *_animOut;
    HWND _hWnd;
    HWND _masterWnd;
    Settings &_settings;

    void AnimateIn();
    void AnimateOut();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};