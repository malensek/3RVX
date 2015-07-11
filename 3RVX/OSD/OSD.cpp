#include "OSD.h"

#include <algorithm>

#include "..\3RVX.h"
#include "..\DisplayManager.h"
#include "..\Error.h"
#include "..\HotkeyInfo.h"
#include "..\Monitor.h"

OSD::OSD(LPCWSTR className, HINSTANCE hInstance) :
_className(className) {

    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }
    _hInstance = hInstance;

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = &OSD::StaticWndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _className;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        throw SYSERR_REGISTERCLASS;
    }

    _hWnd = CreateWindowEx(NULL,
        className, className,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, hInstance, this);

    if (_hWnd == NULL) {
        throw SYSERR_CREATEWINDOW;
    }

    _masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");
}

OSD::~OSD() {
    DestroyWindow(_hWnd);
    UnregisterClass(_className, _hInstance);
}

void OSD::HideOthers(OSDType except = All) {
    SendMessage(_masterWnd, WM_3RVX_CONTROL, MSG_HIDEOSD, except);
}

std::vector<Monitor> OSD::ActiveMonitors() {
    std::vector<Monitor> monitors;
    std::wstring monitorStr = Settings::Instance()->Monitor();

    if (monitorStr == L"") {
        /* Primary Monitor */
        monitors.push_back(DisplayManager::Primary());
        CLOG(L"Monitor: (Primary)");

    } else if (monitorStr == L"*") {
        /* All Monitors */
        std::unordered_map<std::wstring, Monitor> map
            = DisplayManager::MonitorMap();

        for (auto it = map.begin(); it != map.end(); ++it) {
            CLOG(L"Monitor: %s", it->first.c_str());
            monitors.push_back(it->second);
        }
    } else {
        /* Specific Monitor */
        std::unordered_map<std::wstring, Monitor> map 
            = DisplayManager::MonitorMap();

        for (auto it = map.begin(); it != map.end(); ++it) {
            if (monitorStr == it->first) {
                CLOG(L"Monitor: %s", it->first.c_str());
                monitors.push_back(it->second);
                break;
            }
        }
    }

    return monitors;
}

void OSD::PositionWindow(Monitor monitor, LayeredWnd &lWnd) {
    Settings::OSDPos pos = Settings::Instance()->OSDPosition();

    if (pos == Settings::OSDPos::Custom) {
        int customX = Settings::Instance()->OSDX();
        int customY = Settings::Instance()->OSDY();
        lWnd.X(customX);
        lWnd.Y(customY);
        return;
    }

    int offset = Settings::Instance()->OSDEdgeOffset();

    /* Edge cases ;-) */
    switch (pos) {
    case Settings::TopLeft:
        lWnd.X(monitor.X() + offset);
        lWnd.Y(monitor.Y() + offset);
        return;

    case Settings::TopRight:
        lWnd.X(monitor.X() + monitor.Width() - lWnd.Width() - offset);
        lWnd.Y(monitor.Y() + offset);
        return;

    case Settings::BottomLeft:
        lWnd.X(monitor.X() + offset);
        lWnd.Y(monitor.Y() + monitor.Height() - lWnd.Height() - offset);
        return;

    case Settings::BottomRight:
        lWnd.X(monitor.X() + monitor.Width() - lWnd.Width() - offset);
        lWnd.Y(monitor.Y() + monitor.Height() - lWnd.Height() - offset);
        return;
    }

    /* Center */
    CenterWindowX(monitor, lWnd);
    CenterWindowY(monitor, lWnd);
    if (pos == Settings::OSDPos::Center) {
        return;
    }

    /* We're centered. Now adjust based on top, bottom, left, or right: */

    switch (pos) {
    case Settings::Top:
        lWnd.Y(monitor.Y() + offset);
        return;

    case Settings::Bottom:
        lWnd.Y(monitor.Y() + monitor.Height() - lWnd.Height() - offset);
        return;

    case Settings::Left:
        lWnd.X(monitor.X() + offset);
        return;

    case Settings::Right:
        lWnd.X(monitor.X() + monitor.Width() - lWnd.Width() - offset);
        return;
    }
}

void OSD::CenterWindowX(Monitor monitor, LayeredWnd &lWnd) {
    lWnd.X(monitor.X() + monitor.Width() / 2 - lWnd.Width() / 2);
}

void OSD::CenterWindowY(Monitor monitor, LayeredWnd &lWnd) {
    lWnd.Y(monitor.Y() + monitor.Height() / 2 - lWnd.Height() / 2);
}

void OSD::ProcessHotkeys(HotkeyInfo &hki) {

}

LRESULT CALLBACK
OSD::StaticWndProc(
HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    OSD *osd;

    if (message == WM_CREATE) {
        osd = (OSD *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) osd);
    } else {
        osd = (OSD *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!osd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return osd->WndProc(hWnd, message, wParam, lParam);
}

LRESULT
OSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}