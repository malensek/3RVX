#include "UpdaterWindow.h"

#include "../3RVX/3RVX.h"
#include "../3RVX/Logger.h"
#include "../3RVX/CommCtl.h"
#include "../3RVX/NotifyIcon.h"
#include "resource.h"

UpdaterWindow::UpdaterWindow() :
Window(L"3RVX-UpdateWindow") {
    HRESULT hr = LoadIconMetric(
        Window::InstanceHandle(),
        MAKEINTRESOURCE(IDI_MAINICON),
        LIM_SMALL,
        &_icon);
}

LRESULT UpdaterWindow::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == _3RVX::WM_3RVX_SETTINGSCTRL) {
        switch (wParam) {
        case _3RVX::MSG_UPDATEICON:
            CLOG(L"Received request to activate window from external program");
            new NotifyIcon(Window::Handle(), L"Update Available", _icon);
            break;
        }

        return Window::WndProc(hWnd, message, wParam, lParam);
    }
}
