#pragma once

#include <Windows.h>

#include "../HotkeyProcessor.h"
#include "../MeterWnd/MeterWnd.h"
#include "../Settings.h"
#include "../Window.h"
#include "OSDType.h"

class Monitor;

class OSD : HotkeyProcessor, protected Window {
public:
    OSD(LPCWSTR className, HINSTANCE hInstance = NULL);

    virtual void Hide() = 0;
    virtual void ProcessHotkeys(HotkeyInfo &hki);

    bool Enabled();
    bool Disabled();
    void Enable();
    void Disable();

protected:
    HWND _masterWnd;
    Settings *_settings;

    void HideOthers(OSDType except);

    void InitMeterWnd(MeterWnd &mWnd);

    std::vector<Monitor> ActiveMonitors();
    void PositionWindow(Monitor monitor, LayeredWnd &lWnd);
    void CenterWindowX(Monitor monitor, LayeredWnd &lWnd);
    void CenterWindowY(Monitor monitor, LayeredWnd &lWnd);

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    bool _disabled;
};