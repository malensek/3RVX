#include "SliderWnd.h"

#include <windowsx.h>

SliderWnd::SliderWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
MeterWnd(className, title, hInstance) {
    long styles = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, styles);
}

void SliderWnd::Show() {
    PositionWindow();
    MeterWnd::Show();
}

void SliderWnd::PositionWindow() {
    POINT p;
    GetCursorPos(&p);

    HMONITOR monitor = MonitorFromPoint(p, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mInfo = {};
    mInfo.cbSize = sizeof(mInfo);
    GetMonitorInfo(monitor, &mInfo);
    RECT mRect = mInfo.rcWork;

    /* Default location is the bottom of the window centered above cursor. */
    POINT loc;
    loc.x = p.x - _size.cx / 2;
    loc.y = p.y - _size.cy;

    /* Reposition the window if it's falling off the monitor somewhere. */
    if (loc.y < mRect.top) {
        loc.y = mRect.top;
    }

    if (loc.x < mRect.left) {
        loc.x = mRect.left;
    }

    if (p.y > mRect.bottom) {
        loc.y = mRect.bottom - _size.cy;
    }

    if (p.x > mRect.right - _size.cx) {
        loc.x = mRect.right - _size.cx;
    }

    Move(loc.x, loc.y);
}

LRESULT SliderWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KILLFOCUS:
        //Hide();
        break;

    case WM_NCHITTEST:
        LRESULT hit = DefWindowProc(_hWnd, message, wParam, lParam);
        /* Make the client area appear to be the window title bar: */
        if (hit == HTCLIENT) {
            hit = HTCAPTION;
        }
        return hit;
    }

    return MeterWnd::WndProc(message, wParam, lParam);
}

