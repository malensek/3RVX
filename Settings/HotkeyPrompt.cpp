#include "HotkeyPrompt.h"

#include "../3RVX/Error.h"
#include "resource.h"

HotkeyPrompt::HotkeyPrompt(HINSTANCE hInstance, std::wstring className) {
    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }
    _hInstance = hInstance;

    const wchar_t *cls = className.c_str();

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = &HotkeyPrompt::StaticWndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = cls;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        throw SYSERR_REGISTERCLASS;
    }

    _hWnd = CreateWindowEx(NULL, cls, cls, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, hInstance, this);

    if (_hWnd == NULL) {
        throw SYSERR_CREATEWINDOW;
    }
}

void HotkeyPrompt::Show() {
    DialogBox(_hInstance, MAKEINTRESOURCE(IDD_HOTKEYPROMPT), _hWnd, NULL);
}

LRESULT CALLBACK HotkeyPrompt::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HotkeyPrompt *prompt;

    if (message == WM_CREATE) {
        prompt = (HotkeyPrompt *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) prompt);
    } else {
        prompt = (HotkeyPrompt *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!prompt) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return prompt->WndProc(hWnd, message, wParam, lParam);
}

LRESULT HotkeyPrompt::WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}