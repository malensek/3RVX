#include "HotkeyPrompt.h"

#include "../3RVX/Error.h"
#include "resource.h"

HotkeyPrompt::HotkeyPrompt(HINSTANCE hInstance, std::wstring className) {
    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }
void HotkeyPrompt::Show(HWND parent, HINSTANCE hInstance) {
    _hInstance = hInstance;
    _parent = parent;

    if (_hInstance == NULL) {
        _hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }

    if (_hWnd == NULL) {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = NULL;
        wcex.lpfnWndProc = &HotkeyPrompt::WndProc;
        wcex.cbClsExtra = NULL;
        wcex.cbWndExtra = NULL;
        wcex.hInstance = _hInstance;
        wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = CLASS_NAME;
        wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wcex)) {
            throw SYSERR_REGISTERCLASS;
        }

        _hWnd = CreateWindowEx(NULL, CLASS_NAME, CLASS_NAME, NULL, NULL, NULL,
            NULL, NULL, parent, NULL, _hInstance, NULL);

        if (_hWnd == NULL) {
            throw SYSERR_CREATEWINDOW;
        }

        EnableWindow(parent, FALSE);
    }

    DialogBox(_hInstance, MAKEINTRESOURCE(IDD_HOTKEYPROMPT),
        parent, &HotkeyPrompt::DialogProc);
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