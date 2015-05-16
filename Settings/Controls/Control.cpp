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

RECT Control::ScreenDimensions() {
    RECT r;
    GetWindowRect(_hWnd, &r);
    return r;
}

RECT Control::ClientDimensions() {
    RECT r;
    GetWindowRect(_hWnd, &r);
    int width = r.right - r.left;
    int height = r.bottom - r.top;

    POINT p = { r.left, r.top };
    int ret = ScreenToClient(_parent, &p);
    r.left = p.x;
    r.top = p.y;
    r.right = r.left + width;
    r.bottom = r.top + height;

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

int Control::EmSize() {
    /* Determine the width of the text */
    HDC dc = GetDC(_hWnd);
    SIZE sz = { 0 };
    GetTextExtentPoint32(dc, L"M", 1, &sz);
    return sz.cx;
}

void Control::Move(int x, int y) {
    RECT r = ClientDimensions();
    MoveWindow(_hWnd, x, y, r.right - r.left, r.bottom - r.top, TRUE);
}

void Control::PlaceRightOf(Control &control) {
    RECT otherRect = control.ClientDimensions();
    int otherHeight = otherRect.bottom - otherRect.top;
    /* Vertical center point of the other control: */
    int vCenter = otherRect.top + otherHeight / 2;
    
    RECT myRect = ClientDimensions();
    int myHeight = myRect.bottom - myRect.top;

    int x = otherRect.right + EmSize();
    int y = vCenter - myHeight / 2;
    Move(x, y);
}

void Control::Resize(int width, int height) {
    RECT r = ClientDimensions();
    MoveWindow(_hWnd, r.left, r.top, width, height, TRUE);
}

int Control::X() {
    RECT r = ClientDimensions();
    return r.left;
}

int Control::Y() {
    RECT r = ClientDimensions();
    return r.top;
}

int Control::Width() {
    RECT r = ClientDimensions();
    return r.right - r.left;
}

int Control::Height() {
    RECT r = ClientDimensions();
    return r.bottom - r.top;
}

void Control::X(int x) {
    Move(x, Y());
}

void Control::Y(int y) {
    Move(X(), y);
}

void Control::Width(int width) {
    Resize(width, Height());
}

void Control::Height(int height) {
    Resize(Width(), height);
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

void Control::RemoveWindowExStyle(long exStyle) {
    long exs = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    exs &= ~exStyle;
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
