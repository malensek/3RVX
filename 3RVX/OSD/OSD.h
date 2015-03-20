#pragma once

#include <Windows.h>

#include "..\MeterWnd\MeterWnd.h"
#include "..\Settings.h"
#include "OSDType.h"

class Monitor;

class OSD {
public:
    OSD(std::wstring className, HINSTANCE hInstance = NULL);
    ~OSD();

    virtual void Hide() = 0;
    virtual void ProcessHotkeys(HotkeyInfo &hki);

protected:
    HWND _hWnd;
    HWND _masterWnd;

    void HideOthers(OSDType except);

    virtual void UpdateWindowPositions(std::vector<Monitor> &monitors) = 0;
    std::vector<Monitor> ActiveMonitors();
    void PositionWindow(Monitor monitor, LayeredWnd &lWnd);
    void CenterWindowX(Monitor monitor, LayeredWnd &lWnd);
    void CenterWindowY(Monitor monitor, LayeredWnd &lWnd);

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};