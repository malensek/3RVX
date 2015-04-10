#pragma once

#include <Windows.h>
#include <string>

class Control {
public:
    Control();
    Control(int id, HWND parent);
    ~Control();

    RECT Dimensions();

    void Enable();
    void Disable();
    void Enabled();
    void Enabled(int id, bool enabled);

    std::wstring Text();
    int TextAsInt();
    bool Text(std::wstring text);
    bool Text(int value);

    void WindowExStyle();
    void WindowExStyle(long exStyle);
    void AddWindowExStyle(long exStyle);
    void RemoveWindowExStyle(long exStyle);

protected:
    int _id;
    HWND _hWnd;
    HWND _parent;

protected:
    static const int MAX_EDITSTR = 0x4000;
};