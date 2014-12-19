#include "SliderWnd.h"

#include <windowsx.h>

#include "SliderKnob.h"

SliderWnd::SliderWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
MeterWnd(className, title, hInstance),
_dragging(false) {
    long styles = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, styles | WS_EX_TOPMOST);
}

void SliderWnd::Show() {
    PositionWindow();
    MeterWnd::Show(false);
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

bool SliderWnd::MouseOverTrack(int x, int y) {
    if (x >= _knob->TrackX()
        && x <= _knob->TrackX() + _knob->TrackWidth()
        && y >= _knob->TrackY()
        && y <= _knob->TrackY() + _knob->TrackHeight()) {

        return true;
    } else {
        return false;
    }
}

void SliderWnd::UpdateKnob(int x, int y) {
    int oldLoc, newLoc, drag, knobMax, knobMin;

    if (_vertical) {
        oldLoc = _knob->Y();
        drag = y;
        knobMax = _knob->TrackY() + _knob->TrackHeight() - _knob->Height();
        knobMin = _knob->TrackY();
    } else {
        oldLoc = _knob->X();
        drag = x;
        knobMax = _knob->TrackX() + _knob->TrackWidth() - _knob->Width();
        knobMin = _knob->TrackX();
    }

    if (drag - _dragOffset > knobMax) {
        newLoc = knobMax;
    } else if (drag - _dragOffset < knobMin) {
        newLoc = knobMin;
    } else {
        newLoc = drag - _dragOffset;
    }

    if (oldLoc != newLoc) {
        if (_vertical) {
            _knob->Y(newLoc);
        } else {
            _knob->X(newLoc);
        }

        MeterWnd::MeterLevels(_knob->Value());
        SliderChanged();
        Update();
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
        Hide();
        break;

    case WM_MOUSEMOVE:
        if (_dragging) {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            UpdateKnob(x, y);
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
            if (_vertical) {
                _dragOffset = y - _knob->Y();
            } else {
                _dragOffset = x - _knob->X();
            }
            SetCapture(_hWnd);
        } else if (MouseOverTrack(x, y)) {
            /* Simulate the mouse dragging to the clicked location: */
            UpdateKnob(x, y);
        }

        break;
    }

    case WM_LBUTTONUP:
        _dragging = false;
        ReleaseCapture();
        break;
    }

    return MeterWnd::WndProc(message, wParam, lParam);
}

