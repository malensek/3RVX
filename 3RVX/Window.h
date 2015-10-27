// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>
#include <Windows.h>

/// <summary>
/// Abstracts away the underlying win32 API details for creating and managing
/// windows.
/// </summary>
class Window {
public:
    class Builder;

    /// <summary>
    /// Creates a window and registers a class for it. If the window cannot be
    /// created or the class cannot be registered, an error message will be
    /// displayed. Only the window title is required; if not provided, the rest
    /// of the window parameters take on their default values.
    /// </summary>
    Window(LPCWSTR className, LPCWSTR title = L"",
        HINSTANCE hInstance = NULL,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
        int width = 0, int height = 0,
        UINT classStyle = NULL, DWORD style = NULL, DWORD exStyle = NULL,
        HWND parent = NULL, HMENU menu = NULL,
        HICON icon = NULL, HCURSOR cursor = NULL,
        HBRUSH background = (HBRUSH) (COLOR_WINDOW + 1));
    ~Window();

    /// <summary>
    /// Retrieves the class name this Window was registered with.
    /// </summary>
    LPCWSTR ClassName();

    /// <summary>This Window's hWnd</summary>
    HWND Handle();

    /// <summary>hInstance associated with this Window.</summary>
    HINSTANCE InstanceHandle();

    /// <summary>
    /// Retrieves the window title supplied when this Window was created.
    /// </summary>
    LPCWSTR Title();

protected:
    /// <summary>
    /// The static window procedure for Window implementations. When a window
    /// is created, a pointer to its instance is passed as a parameter to
    /// CreateWindowEx() so that the virtual implementation of the window
    /// procedure can be called.
    /// </summary>
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

    /// <summary>Window procedure for this Window.</summary>
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    std::wstring _className;
    HINSTANCE _hInstance;
    HWND _hWnd;
    std::wstring _title;
};

class Window::Builder {
public:
    Builder(LPCWSTR className) :
    _className(className),
    _title(L""),
    _hInstance(NULL),
    _x(CW_USEDEFAULT),
    _y(CW_USEDEFAULT),
    _width(0),
    _height(0),
    _classStyle(NULL),
    _style(NULL),
    _exStyle(NULL),
    _parent(NULL),
    _menu(NULL),
    _icon(NULL),
    _cursor(NULL),
    _background((HBRUSH) (COLOR_WINDOW + 1)) {

    }

    Builder &Title(LPCWSTR title) {
        _title = title;
        return *this;
    }

    Builder &InstanceHandle(HINSTANCE hInstance) {
        _hInstance = hInstance;
        return *this;
    }

    Builder &X(int x) {
        _x = x;
        return *this;
    }

    Builder &Y(int y) {
        _y = y;
        return *this;
    }

    Builder &Width(int width) {
        _width = width;
        return *this;
    }

    Builder &Height(int height) {
        _height = height;
        return *this;
    }

    Builder &ClassStyle(UINT classStyle) {
        _classStyle = classStyle;
        return *this;
    }

    Builder &WindowStyle(DWORD style) {
        _style = style;
        return *this;
    }

    Builder &ExtendedStyle(DWORD exStyle) {
        _exStyle = exStyle;
        return *this;
    }

    Builder &Parent(HWND parent) {
        _parent = parent;
        return *this;
    }

    Builder &Menu(HMENU menu) {
        _menu = menu;
        return *this;
    }

    Builder &Icon(HICON icon) {
        _icon = icon;
        return *this;
    }

    Builder &Cursor(HCURSOR cursor) {
        _cursor = cursor;
        return *this;
    }

    Builder &Background(HBRUSH bgBrush) {
        _background = bgBrush;
        return *this;
    }

    Window Build() {
        return Window(_className, _title, _hInstance,
            _x, _y, _width, _height,
            _classStyle, _style, _exStyle,
            _parent,
            _menu, _icon, _cursor, _background);
    }

private:
    LPCWSTR _className;
    LPCWSTR _title;
    HINSTANCE _hInstance;
    int _x, _y, _width, _height;
    UINT _classStyle;
    DWORD _style, _exStyle;
    HWND _parent;
    HMENU _menu;
    HICON _icon;
    HCURSOR _cursor;
    HBRUSH _background;
};