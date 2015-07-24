#pragma once

#include <string>
#include <Windows.h>

/// <summary>
/// Abstracts away the underlying win32 API details for creating and managing
/// windows.
/// </summary>
class Window {
public:
    /// <summary>
    /// Creates a window and registers a class for it. If the window cannot be
    /// created or the class cannot be registered, a std::runtime_error
    /// exception will be thrown. Only the window title is required; if not
    /// provided, the rest of the window parameters take on their default
    /// values.
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
    LPCWSTR _className;
    HINSTANCE _hInstance;
    HWND _hWnd;
    LPCWSTR _title;
};