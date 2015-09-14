// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "UpdaterWindow.h"

#include "../../3RVX/3RVX.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/CommCtl.h"
#include "../../3RVX/NotifyIcon.h"
#include "../../3RVX/Settings.h"
#include "../resource.h"
#include "ProgressWindow.h"
#include "Updater.h"

UpdaterWindow::UpdaterWindow() :
Window(L"3RVX-UpdateWindow") {
    HRESULT hr;
    hr = LoadIconMetric(
        Window::InstanceHandle(),
        MAKEINTRESOURCE(IDI_MAINICON),
        LIM_SMALL,
        &_smallIcon);
    if (hr != S_OK) {
        CLOG(L"Could not load notification icon");
    }

    hr = LoadIconMetric(
        Window::InstanceHandle(),
        MAKEINTRESOURCE(IDI_MAINICON),
        LIM_LARGE,
        &_largeIcon);
    if (hr != S_OK) {
        CLOG(L"Could not load large notification icon");
    }

    _menu = CreatePopupMenu();

    InsertMenu(_menu, -1, MF_ENABLED, MENU_INSTALL, L"Install");
    InsertMenu(_menu, -1, MF_ENABLED, MENU_IGNORE, L"Ignore version");
    InsertMenu(_menu, -1, MF_ENABLED, MENU_REMIND, L"Remind me later");

    _menuFlags = TPM_RIGHTBUTTON;
    if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
        _menuFlags |= TPM_RIGHTALIGN;
    } else {
        _menuFlags |= TPM_LEFTALIGN;
    }
}

UpdaterWindow::~UpdaterWindow() {
    delete _notifyIcon;
    DestroyIcon(_smallIcon);
    DestroyIcon(_largeIcon);
}

void UpdaterWindow::InstallUpdate() {
    delete _notifyIcon;
    _notifyIcon = nullptr;

    ProgressWindow *pw = new ProgressWindow(_version);
}

LRESULT UpdaterWindow::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == _3RVX::WM_3RVX_SETTINGSCTRL) {
        switch (wParam) {
        case _3RVX::MSG_UPDATEICON:
            _settings = Settings::Instance();

            /* Set that we just checked for updates now */
            _settings->LastUpdateCheckNow();
            _settings->Save();

            _version = Updater::RemoteVersion();
            if (_version.Major() <= 0
                    || _version.ToString() == _settings->IgnoreUpdate()) {
                SendMessage(Window::Handle(), WM_CLOSE, NULL, NULL);
                break;
            }

            CLOG(L"Creating update icon");
            _notifyIcon = new NotifyIcon(
                Window::Handle(),
                L"Update Available",
                _smallIcon);

            CLOG(L"Launching balloon notification");
            _notifyIcon->Balloon(L"Update Available",
                L"3RVX " + _version.ToString(), _largeIcon);
            break;
        }
    } else if (message == MSG_NOTIFYICON) {
        if (lParam == WM_LBUTTONUP
            || lParam == WM_RBUTTONUP
            || lParam == NIN_BALLOONUSERCLICK) {
            POINT p;
            GetCursorPos(&p);
            SetForegroundWindow(hWnd);
            TrackPopupMenuEx(_menu, _menuFlags, p.x, p.y,
                Window::Handle(), NULL);
            PostMessage(hWnd, WM_NULL, 0, 0);
        }
    } else if (message == WM_COMMAND) {
        int menuItem = LOWORD(wParam);
        switch (menuItem) {
        case MENU_INSTALL:
            InstallUpdate();
            break;

        case MENU_IGNORE:
            _settings->IgnoreUpdate(_version.ToString());
            _settings->Save();
            SendMessage(Window::Handle(), WM_CLOSE, NULL, NULL);
            break;

        case MENU_REMIND:
            SendMessage(Window::Handle(), WM_CLOSE, NULL, NULL);
            break;
        }
    } else if (message == WM_DESTROY) {
        PostQuitMessage(0);
    }

    return Window::WndProc(hWnd, message, wParam, lParam);
}

void UpdaterWindow::DoModal() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}