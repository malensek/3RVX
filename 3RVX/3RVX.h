// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
    void ToggleOSDs();
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

public:
    static HWND MasterHwnd() {
        return FindWindow(CLASS_3RVX, CLASS_3RVX);
    }

    static HWND MasterSettingsHwnd() {
        return FindWindow(CLASS_3RVX_SETTINGS, CLASS_3RVX_SETTINGS);
    }

    static HWND UpdaterHwnd() {
        return FindWindow(CLASS_3RVX_UPDATER, CLASS_3RVX_UPDATER);
    }

    static void Message(WPARAM wParam, LPARAM lParam, bool post = false) {
        HWND masterWnd = MasterHwnd();

        if (post) {
            PostMessage(masterWnd, WM_3RVX_CTRL, wParam, lParam);
        } else {
            SendMessage(masterWnd, WM_3RVX_CTRL, wParam, lParam);
        }
    }

    static void SettingsMessage(
            WPARAM wParam, LPARAM lParam, bool post = false) {
        HWND masterWnd = MasterSettingsHwnd();

        if (post) {
            PostMessage(masterWnd, WM_3RVX_SETTINGSCTRL, wParam, lParam);
        } else {
            SendMessage(masterWnd, WM_3RVX_SETTINGSCTRL, wParam, lParam);
        }
    }

public:
    static constexpr const wchar_t *CLASS_3RVX = L"3RVXv3";
    static constexpr const wchar_t *CLASS_3RVX_SETTINGS = L"3RVXv3-Settings";
    static constexpr const wchar_t *CLASS_3RVX_UPDATER = L"3RVXv3-Updater";

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
    static const int MSG_UPDATEICON = WM_APP + 202;

private:
    static const int TIMER_FIRSTUPDATE = 100;
    static const int TIMER_UPDATE = 101;

    static const int FIRSTUPDATE_INTERVAL = 1800000; /* 30 minutes (ms) */
    static const int UPDATE_INTERVAL = 86410000; /* 24 hours, 10 secs (ms) */
};