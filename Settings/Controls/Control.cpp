#include "Control.h"

#include <sstream>

Control::Control() {

}

Control::Control(int id, HWND parent) :
_id(id),
_parent(parent) {
    _hWnd = GetDlgItem(parent, id);
}

Control::~Control() {

}

RECT Control::Dimensions() {
    RECT r;
    GetWindowRect(_hWnd, &r);
    return r;
}

void Control::Enable() {
    EnableWindow(_hWnd, TRUE);
}

void Control::Disable() {
    EnableWindow(_hWnd, FALSE);
}

bool Control::Enabled() {
    return (IsWindowEnabled(_hWnd) == TRUE);
}

void Control::Enabled(bool enabled) {
    if (enabled == true) {
        Enable();
    } else {
        Disable();
    }
}

bool Control::Text(std::wstring text) {
    return SetDlgItemText(_parent, _id, text.c_str()) == TRUE;
}

bool Control::Text(int value) {
    return SetDlgItemInt(_parent, _id, value, TRUE) == TRUE;
}

std::wstring Control::Text() {
    wchar_t text[MAX_EDITSTR];
    GetDlgItemText(_parent, _id, text, MAX_EDITSTR);
    return std::wstring(text);
}

int Control::TextAsInt() {
    std::wstring str = Text();
    int num;
    std::wistringstream wistr(str);
    wistr >> num;
    return num;
}

bool Control::Visible() {
    return IsWindowVisible(_hWnd) != FALSE;
}

void Control::Visible(bool visible) {
    ShowWindow(_hWnd, visible ? SW_SHOW : SW_HIDE);
}

void Control::AddWindowExStyle(long exStyle) {
    long exs = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    exs |= exStyle;
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, exs);
}

DLGPROC Control::Command(unsigned short nCode) {
    /* By default, indicate that we did not process the message: */
    return FALSE;
}

DLGPROC Control::Notification(NMHDR *nHdr) {
    /* By default, indicate that we did not process the message: */
    return FALSE;
}
