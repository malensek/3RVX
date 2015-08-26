#include "UpdaterWindow.h"

#include "../3RVX/3RVX.h"
#include "../3RVX/Logger.h"
#include "../3RVX/CommCtl.h"
#include "../3RVX/NotifyIcon.h"
#include "../3RVX/Settings.h"
#include "resource.h"
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

LRESULT UpdaterWindow::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == _3RVX::WM_3RVX_SETTINGSCTRL) {
        switch (wParam) {
        case _3RVX::MSG_UPDATEICON:
            _settings = Settings::Instance();
            _settings->Load();
            std::pair<int, int> newVersion = Updater::RemoteVersion();
            _versionString = Updater::VersionToString(newVersion);
            CLOG(L"Creating update icon");
            _notifyIcon = new NotifyIcon(
                Window::Handle(),
                L"Update Available",
                _smallIcon);

            CLOG(L"Launching balloon notification");
            _notifyIcon->Balloon(L"Update Available", L"3RVX 3.2", _largeIcon);
            break;
        }

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