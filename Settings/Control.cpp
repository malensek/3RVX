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

bool Control::Text(std::wstring text) {
    return SetDlgItemText(_parent, _id, text.c_str()) == TRUE;
}

bool Control::Text(int value) {
    return Text(std::to_wstring(value));
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

void Control::AddWindowExStyle(long exStyle) {
    long exs = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
    exs |= exStyle;
    SetWindowLongPtr(_hWnd, GWL_EXSTYLE, exs);
}

RECT Control::Dimensions() {
    RECT r;
    GetWindowRect(_hWnd, &r);
    return r;
}