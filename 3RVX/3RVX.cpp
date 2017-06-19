// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "3RVX.h"
#include "HideWin10VolumeOSD.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Wtsapi32.lib")

#include <ctime>
#include <gdiplus.h>
#include <iostream>
#include <Wtsapi32.h>

#include "DisplayManager.h"
#include "HotkeyManager.h"
#include "Logger.h"
#include "OSD/OSD.h"
#include "OSD/BrightnessOSD.h"
#include "OSD/EjectOSD.h"
#include "OSD/KeyboardOSD.h"
#include "OSD/VolumeOSD.h"
#include "Settings.h"
#include "Skin/AccentColor.h"
#include "Skin/SkinManager.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    Logger::Start();

    QCLOG(L"  _____ ______     ____  _______ ");
    QCLOG(L" |___ /|  _ \\ \\   / /\\ \\/ /___ / ");
    QCLOG(L"   |_ \\| |_) \\ \\ / /  \\  /  |_ \\ ");
    QCLOG(L"  ___) |  _ < \\ V /   /  \\ ___) |");
    QCLOG(L" |____/|_| \\_\\ \\_/   /_/\\_\\____/ ");
    QCLOG(L"");

    HANDLE mutex;
    mutex = CreateMutex(NULL, FALSE, L"Local\\3RVX");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (mutex) {
            ReleaseMutex(mutex);
        }

        CLOG(L"A previous instance of the program is already running.\n"
            L"Requesting Settings dialog.");
        _3RVX::Message(_3RVX::MSG_SETTINGS, NULL);

#if defined(ENABLE_3RVX_LOG) && (defined(ENABLE_3RVX_LOGTOFILE) == FALSE)
        CLOG(L"Press [enter] to terminate");
        std::cin.get();
#endif

        return EXIT_SUCCESS;
    }

    CLOG(L"App directory: %s", Settings::AppDir().c_str());

    using namespace Gdiplus;
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    _3RVX mainWnd(hInstance);

    HRESULT hr = CoInitializeEx(NULL,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (hr != S_OK) {
        CLOG(L"Failed to initialize the COM library.");
        return EXIT_FAILURE;
    }

    /* Tell the program to initialize */
    PostMessage(mainWnd.Handle(), _3RVX::WM_3RVX_CTRL, _3RVX::MSG_LOAD, NULL);

    /* Register for session change notifications */
    WTSRegisterSessionNotification(mainWnd.Handle(), NOTIFY_FOR_THIS_SESSION);

    /* Start the event loop */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    CoUninitialize();

    Logger::Stop();

    return (int) msg.wParam;
}

_3RVX::_3RVX(HINSTANCE hInstance) :
Window(_3RVX::CLASS_3RVX, _3RVX::CLASS_3RVX, hInstance) {

}

void _3RVX::Initialize() {
    CLOG(L"Initializing...");

    delete _vOSD;
    delete _eOSD;
    delete _bOSD;
    delete _kOSD;
    _vOSD = nullptr;
    _eOSD = nullptr;
    _bOSD = nullptr;
    _kOSD = nullptr;

    Settings *settings = Settings::Instance();
    settings->Load();

    KillTimer(Window::Handle(), TIMER_FIRSTUPDATE);
    KillTimer(Window::Handle(), TIMER_UPDATE);
    if (settings->AutomaticUpdates()) {
        SetTimer(
            Window::Handle(),
            TIMER_FIRSTUPDATE,
            FIRSTUPDATE_INTERVAL,
            NULL);
    }

    SkinManager::Instance()->LoadSkin(settings->SkinXML());

    DisplayManager::UpdateMonitorMap();

    /* OSDs */
    _eOSD = new EjectOSD();
    _vOSD = new VolumeOSD();
    _bOSD = new BrightnessOSD();
    _kOSD = new KeyboardOSD();

    _osds.clear();
    _osds.push_back(_eOSD);
    _osds.push_back(_vOSD);
    _osds.push_back(_bOSD);
    _osds.push_back(_kOSD);

    /* Hotkey setup */
    if (_hkManager != NULL) {
        _hkManager->Shutdown();
    }
    _hkManager = HotkeyManager::Instance(Handle());

    _hotkeys = Settings::Instance()->Hotkeys();
    for (auto it = _hotkeys.begin(); it != _hotkeys.end(); ++it) {
        /* Enable arg caching */
        it->second.EnableArgCache();

        int combination = it->first;
        _hkManager->Register(combination);
    }
}

void _3RVX::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
    case HotkeyInfo::Mute:
    case HotkeyInfo::VolumeSlider:
        if (_vOSD) {
            _vOSD->ProcessHotkeys(hki);
        }
        break;

    case HotkeyInfo::EjectDrive:
    case HotkeyInfo::EjectLastDisk:
        if (_eOSD) {
            _eOSD->ProcessHotkeys(hki);
        }
        break;

    case HotkeyInfo::IncreaseBrightness:
    case HotkeyInfo::DecreaseBrightness:
    case HotkeyInfo::SetBrightness:
        if (_bOSD) {
            _bOSD->ProcessHotkeys(hki);
        }
        break;

    case HotkeyInfo::MediaKey:
    case HotkeyInfo::VirtualKey:
        _kbHotkeyProcessor.ProcessHotkeys(hki);
        break;

    case HotkeyInfo::Run:
        if (hki.HasArgs()) {
            ShellExecute(NULL, L"open", hki.args[0].c_str(),
                NULL, NULL, SW_SHOWNORMAL);
        }
        break;

    case HotkeyInfo::DisableOSD:
        ToggleOSDs();
        break;

    case HotkeyInfo::Settings:
        ShellExecute(NULL, L"open", Settings::SettingsApp().c_str(),
            NULL, NULL, SW_SHOWNORMAL);
        break;

    case HotkeyInfo::Exit:
        SendMessage(Handle(), WM_CLOSE, NULL, NULL);
        break;
    }
}

void _3RVX::ToggleOSDs() {
    for (OSD *osd : _osds) {
        osd->Enabled(!(osd->Enabled()));
    }
}

LRESULT _3RVX::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_HOTKEY: {
        CLOG(L"Hotkey: %d", (int) wParam);
        HotkeyInfo hki = _hotkeys[(int) wParam];
        ProcessHotkeys(hki);
        break;
    }

    case WM_DISPLAYCHANGE: {
        CLOG(L"Detected display change");
        DisplayManager::UpdateMonitorMap();
        for (OSD *osd : _osds) {
            osd->OnDisplayChange();
        }
        break;
    }

    case WM_DWMCOLORIZATIONCOLORCHANGED: {
        /* While this message contains the colorization color, we need to
         * refresh here to handle situations where the user has requested a
         * different color (undocumented colorization color, for example). */
        CLOG(L"Received DWM colorization change notification: %x", wParam);
        AccentColor::Instance()->Refresh();
    }

    case WM_WTSSESSION_CHANGE: {
        CLOG(L"Detected session change");
        break;
    }

    case WM_CLOSE: {
        CLOG(L"Shutting down");
        HideWin10VolumeOSD::ShowOSD();
        HotkeyManager::Instance()->Shutdown();
        _vOSD->HideIcon();
        _eOSD->HideIcon();
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    case WM_TIMER:
        if (wParam == TIMER_FIRSTUPDATE || wParam == TIMER_UPDATE) {
            CLOG(L"Received updater timer notification");
            Settings *settings = Settings::Instance();
            long long checkTime = settings->LastUpdateCheck();
            if ((std::time(nullptr) - checkTime) > (UPDATE_INTERVAL / 1000)) {
                /* Enough time has elapsed since the last update check */
                std::wstring settingsApp = Settings::SettingsApp();
                CLOG(L"Launching update task: %s %s",
                    settingsApp.c_str(), L"-update");
                ShellExecute(NULL, L"open",
                    Settings::SettingsApp().c_str(), L"-update", NULL, SW_HIDE);
            }

            if (wParam == TIMER_FIRSTUPDATE) {
                CLOG(L"Starting long-term update timer");
                /* If this was the first update check (30 min after launch),
                 * then kill the first update timer and start the main timer
                 * (checks on 24-hour intervals) */
                KillTimer(Window::Handle(), TIMER_FIRSTUPDATE);
                SetTimer(Window::Handle(), TIMER_UPDATE,
                    UPDATE_INTERVAL, NULL);
            }
        }
        break;
    }

    if (message == _3RVX::WM_3RVX_CTRL) {
        switch (wParam) {
        case _3RVX::MSG_LOAD:
            Initialize();
            HideWin10VolumeOSD::Init();
            break;

        case _3RVX::MSG_SETTINGS:
            Settings::LaunchSettingsApp();
            break;

        case _3RVX::MSG_HIDEOSD:
            int except = (OSDType) lParam;
            switch (except) {
            case Volume:
                if (_eOSD) { _eOSD->Hide(); }
                if (_bOSD) { _bOSD->Hide(); }
                break;

            case Eject:
                if (_vOSD) { _vOSD->Hide(); }
                if (_bOSD) { _bOSD->Hide(); }
                break;

            case Brightness:
                if (_vOSD) { _vOSD->Hide(); }
                if (_eOSD) { _eOSD->Hide(); }
            }

            break;
        }
    }

    return Window::WndProc(hWnd, message, wParam, lParam);
}
