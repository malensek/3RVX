#pragma once

#include <string>
#include <Windows.h>

class Window {
public:
    Window(LPCWSTR className, LPCWSTR title = L"",
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
        int width = 0, int height = 0,
        HINSTANCE hInstance = NULL,
        UINT classStyle = NULL, DWORD style = NULL, DWORD exStyle = NULL,
        HWND parent = NULL, HMENU menu = NULL,
        HICON icon = NULL, HCURSOR cursor = NULL,
        HBRUSH background = (HBRUSH) (COLOR_WINDOW + 1));
    ~Window();

    LPCWSTR ClassName();
    HWND Handle();

protected:
    LPCWSTR _className;
    HINSTANCE _hInstance;
    HWND _hWnd;

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};