#include "OSD.h"

#include <algorithm>

#include "..\3RVX.h"
#include "..\DisplayManager.h"
#include "..\HotkeyInfo.h"
#include "..\Monitor.h"

#define DISPLAY_EDGE_OFFSET 140

OSD::OSD(std::wstring className, HINSTANCE hInstance) {

    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }

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
    wcex.lpszClassName = className.c_str();
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);
    /* throw exception if failed? */

    _hWnd = CreateWindowEx(
        NULL, className.c_str(), className.c_str(),
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, hInstance, this);

    _masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");
}

OSD::~OSD() {
    DestroyWindow(_hWnd);
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

void OSD::PositionWindow(Monitor monitor, MeterWnd &mWnd) {
    Settings::OSDPos pos = Settings::Instance()->OSDPosition();

    if (pos == Settings::OSDPos::Custom) {
        int customX = Settings::Instance()->OSDX();
        int customY = Settings::Instance()->OSDY();
        mWnd.X(customX);
        mWnd.Y(customY);
        return;
    }

    int offset = Settings::Instance()->OSDEdgeOffset();

    /* Edge cases ;-) */
    switch (pos) {
    case Settings::TopLeft:
        mWnd.X(monitor.X() + offset);
        mWnd.Y(monitor.Y() + offset);
        return;

    case Settings::TopRight:
        mWnd.X(monitor.X() + monitor.Width() - mWnd.Width() - offset);
        mWnd.Y(monitor.Y() + offset);
        return;

    case Settings::BottomLeft:
        mWnd.X(monitor.X() + offset);
        mWnd.Y(monitor.Y() + monitor.Height() - mWnd.Height() - offset);
        return;

    case Settings::BottomRight:
        mWnd.X(monitor.X() + monitor.Width() - mWnd.Width() - offset);
        mWnd.Y(monitor.Y() + monitor.Height() - mWnd.Height() - offset);
        return;
    }

    /* Center */
    CenterWindowX(monitor, mWnd);
    CenterWindowY(monitor, mWnd);
    if (pos == Settings::OSDPos::Center) {
        return;
    }

    /* We're centered. Now adjust based on top, bottom, left, or right: */

    switch (pos) {
    case Settings::Top:
        mWnd.Y(monitor.Y() + offset);
        return;

    case Settings::Bottom:
        mWnd.Y(monitor.Y() + monitor.Height() - mWnd.Height() - offset);
        return;

    case Settings::Left:
        mWnd.X(monitor.X() + offset);
        return;

    case Settings::Right:
        mWnd.X(monitor.X() + monitor.Width() - mWnd.Width() - offset);
        return;
    }
}

void OSD::CenterWindowX(Monitor monitor, MeterWnd &mWnd) {
    mWnd.X(monitor.X() + monitor.Width() / 2 - mWnd.Width() / 2);
}

void OSD::CenterWindowY(Monitor monitor, MeterWnd &mWnd) {
    mWnd.Y(monitor.Y() + monitor.Height() / 2 - mWnd.Height() / 2);
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