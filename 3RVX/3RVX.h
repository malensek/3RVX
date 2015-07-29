#pragma once

#include <Windows.h>
#include <unordered_map>

#include "HotkeyInfo.h"
#include "KeyboardHotkeyProcessor.h"
#include "Window.h"

class EjectOSD;
class VolumeOSD;

class HotkeyManager;

class _3RVX : public Window {
public:
    _3RVX(HINSTANCE hInstance);

private:
    VolumeOSD *_vOSD;
    EjectOSD *_eOSD;

    HotkeyManager *_hkManager;
    KeyboardHotkeyProcessor _kbHotkeyProcessor;
    std::unordered_map<int, HotkeyInfo> _hotkeys;

    void Initialize();
    void ProcessHotkeys(HotkeyInfo &hki);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

public:
    static HWND MasterHwnd() {
        return FindWindow(CLASS_3RVX, CLASS_3RVX);
    }

    static HWND MasterSettingsHwnd() {
        return FindWindow(CLASS_3RVX_SETTINGS, CLASS_3RVX_SETTINGS);
    }

    static void Message(WPARAM wParam, LPARAM lParam) {
        HWND masterWnd = MasterHwnd();
        SendMessage(masterWnd, WM_3RVX_CTRL, wParam, lParam);
    }

    static void SettingsMessage(WPARAM wParam, LPARAM lParam) {
        HWND masterWnd = MasterSettingsHwnd();
        SendMessage(masterWnd, WM_3RVX_SETTINGSCTRL, wParam, lParam);
    }

public:
    static const wchar_t *CLASS_3RVX;
    static const wchar_t *CLASS_3RVX_SETTINGS;

    static const UINT WM_3RVX_CTRL;
    static const UINT WM_3RVX_SETTINGSCTRL;

public:
    /* 3RVX Messages */
    static const int MSG_LOAD = WM_APP + 100;
    static const int MSG_SETTINGS = WM_APP + 101;
    static const int MSG_EXIT = WM_APP + 102;
    static const int MSG_HIDEOSD = WM_APP + 103;

    /* Settings App Messages */
    static const int MSG_ACTIVATE = WM_APP + 200;
    static const int MSG_LANGCHANGE = WM_APP + 201;

};