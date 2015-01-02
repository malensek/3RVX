#include "OSD.h"

#include <algorithm>

#include "..\3RVX.h"
#include "..\Monitor.h"

#define DISPLAY_EDGE_OFFSET 140

OSD::OSD(std::wstring className, HINSTANCE hInstance) :
_settings(*Settings::Instance()) {

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

void OSD::HideOthers(OSDType except = All) {
    SendMessage(_masterWnd, WM_3RVX_CONTROL, MSG_HIDEOSD, except);
}

void OSD::PositionWindow(HMONITOR monitor, MeterWnd &mWnd) {
    std::wstring pos = _settings.GetText("position");
    if (pos == L"") {
        pos = L"bottom";
    }
    std::transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

    const int mWidth = Monitor::Width(monitor);
    const int mHeight = Monitor::Height(monitor);

    if (pos == L"custom") {
        int customX = _settings.GetInt("positionX");
        int customY = _settings.GetInt("positionY");
        mWnd.X(customX);
        mWnd.Y(customY);
        return;
    }

    CenterWindowX(monitor, mWnd);
    CenterWindowY(monitor, mWnd);
    if (pos == L"center") {
        return;
    }

    /* We're centered. Now adjust based on top, bottom, left, or right: */
    int offset = DISPLAY_EDGE_OFFSET;
    if (_settings.HasSetting("positionOffset")) {
        offset = _settings.GetInt("positionOffset");
    }
    if (pos == L"top") {
        mWnd.Y(offset);
    } else if (pos == L"bottom") {
        mWnd.Y(mHeight - mWnd.Height() - offset);
    } else if (pos == L"left") {
        mWnd.X(offset);
    } else if (pos == L"right") {
        mWnd.X(mWidth - mWnd.Width() - offset);
    }
}

void OSD::CenterWindowX(HMONITOR monitor, MeterWnd &mWnd) {
    const int mWidth = Monitor::Width(monitor);
    mWnd.X(mWidth / 2 - mWnd.Width() / 2);
}

void OSD::CenterWindowY(HMONITOR monitor, MeterWnd &mWnd) {
    const int mHeight = Monitor::Height(monitor);
    mWnd.Y(mHeight / 2 - mWnd.Height() / 2);
}

LRESULT CALLBACK
OSD::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    OSD* osd;

    if (message == WM_CREATE) {
        osd = (OSD*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) osd);
    } else {
        osd = (OSD*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
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