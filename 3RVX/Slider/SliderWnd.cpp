#include "SliderWnd.h"

#include <windowsx.h>

#include "SliderKnob.h"

SliderWnd::SliderWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
MeterWnd(className, title, hInstance),
_dragging(false) {
    long styles = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, styles);
}

void SliderWnd::Show() {
    PositionWindow();
    MeterWnd::Show();
}

void SliderWnd::Knob(SliderKnob *knob) {
    _knob = knob;
    AddMeter(knob);
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

bool SliderWnd::MouseOverKnob(int x, int y) {
    if (x >= _knob->X() && x <= _knob->X() + _knob->Width()
        && y >= _knob->Y() && y <= _knob->Y() + _knob->Height()) {
        return true;
    } else {
        return false;
    }
}

/*    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProc(_hWnd, message, wParam, lParam);
        /* Make the client area appear to be the window title bar:
        if (hit == HTCLIENT) {
            hit = HTCAPTION;
        }
        return hit;
    }
*/


LRESULT SliderWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KILLFOCUS:
        //Hide();
        break;

    case WM_MOUSEMOVE: {
        if (_dragging == false) {
            break;
        }

        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        int knobMax = _knob->TrackX() + _knob->TrackWidth() - _knob->Width();
        if (x > knobMax) {
            _knob->X(knobMax);
        } else if (x - _dragOffset < _knob->TrackX()) {
            _knob->X(_knob->TrackX());
        } else {
            _knob->X(x - _dragOffset);
        }

        Update();
    }
    break;

    case WM_LBUTTONDOWN: {
        if (_knob == NULL) {
            break;
        }

        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (MouseOverKnob(x, y)) {
            _dragging = true;
            _dragOffset = x - _knob->X();
            SetCapture(_hWnd);
        }
    }
    break;

    case WM_LBUTTONUP:
        _dragging = false;
        ReleaseCapture();
    }

    return MeterWnd::WndProc(message, wParam, lParam);
}

