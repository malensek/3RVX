#include "SliderWnd.h"

SliderWnd::SliderWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title) :
MeterWnd(hInstance, className, title) {
    long styles = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, styles);
}