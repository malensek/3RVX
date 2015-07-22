#include "Window.h"

#include "Error.h"

Window::Window(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance,
        int x, int y, int width, int height,
        UINT classStyle, DWORD style, DWORD exStyle,
        HWND parent, HMENU menu,
        HICON icon, HCURSOR cursor, HBRUSH background) :
_className(className),
_title(title) {

    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }
    _hInstance = hInstance;

    if (cursor == NULL) {
        cursor = LoadCursor(NULL, IDC_ARROW);
    }

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = classStyle;
    wcex.lpfnWndProc = &Window::StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = icon;
    wcex.hCursor = cursor;
    wcex.hbrBackground = background;
    wcex.lpszClassName = _className;
    wcex.hIconSm = NULL;

    if (!RegisterClassEx(&wcex)) {
        Error::ErrorMessageDie(SYSERR_CREATEWINDOW, title);
    }

    LPCWSTR wndTitle = title;
    if (lstrcmp(title, L"") == 0) {
        wndTitle = className;
    }

    _hWnd = CreateWindowEx(
        exStyle, className, title, style,
        x, y, width, height,
        parent,
        menu,
        hInstance,
        this);

    if (_hWnd == NULL) {
        Error::ErrorMessageDie(SYSERR_CREATEWINDOW, title);
    }
}

Window::~Window() {
    DestroyWindow(_hWnd);
    UnregisterClass(_className, _hInstance);
}

LPCWSTR Window::ClassName() {
    return _className;
}

HWND Window::Handle() {
    return _hWnd;
}

HINSTANCE Window::InstanceHandle() {
    return _hInstance;
}

LPCWSTR Window::Title() {
    return _title;
}

LRESULT CALLBACK
Window::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window *window;

    if (message == WM_CREATE) {
        window = (Window *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) window);
    } else {
        window = (Window *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!window) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return window->WndProc(hWnd, message, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}
