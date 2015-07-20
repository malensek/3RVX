#include "OSD.h"

#include <algorithm>

#include "..\3RVX.h"
#include "..\DisplayManager.h"
#include "..\Error.h"
#include "..\HotkeyInfo.h"
#include "..\Monitor.h"

OSD::OSD(LPCWSTR className, HINSTANCE hInstance) :
Window(className, className, hInstance),
_settings(Settings::Instance()) {
    _masterWnd = FindWindow(_3RVX::CLASS_3RVX, _3RVX::CLASS_3RVX);
}

OSD::~OSD() {

}

void OSD::HideOthers(OSDType except = All) {
    SendMessage(_masterWnd, _3RVX::WM_3RVX_CTRL, _3RVX::MSG_HIDEOSD, except);
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

LRESULT OSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}