#include "SliderWnd.h"

#include <windowsx.h>

SliderWnd::SliderWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title) :
MeterWnd(hInstance, className, title) {
    long styles = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, styles);
}

void SliderWnd::Show() {
    POINT p;
    GetCursorPos(&p);
    Move(p.x, p.y);

    MeterWnd::Show();
}

LRESULT SliderWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
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

