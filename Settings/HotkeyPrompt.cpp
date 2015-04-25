#include "HotkeyPrompt.h"

#include "../3RVX/Error.h"
#include "../3RVX/Logger.h"

#include "KeyGrabber.h"
#include "resource.h"

HotkeyPrompt *HotkeyPrompt::_instance = NULL;
const wchar_t *HotkeyPrompt::CLASS_NAME = L"3RVX Hotkey Input Dialog";
HWND HotkeyPrompt::_hWnd = NULL;
HWND HotkeyPrompt::_parent = NULL;
HINSTANCE HotkeyPrompt::_hInstance = NULL;

HotkeyPrompt::HotkeyPrompt() {

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

INT_PTR CALLBACK HotkeyPrompt::DialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    unsigned short nCode, ctrlId;

    switch (uMsg) {
    case WM_INITDIALOG:
        KeyGrabber::Instance()->SetHwnd(hwndDlg);
        KeyGrabber::Instance()->Grab();
        break;

    case WM_CLOSE:
        CLOG(L"Closing hotkey prompt.");
        EnableWindow(_parent, TRUE);
        EndDialog(hwndDlg, 0);
        DestroyWindow(_hWnd);
        break;

    case WM_COMMAND:
        nCode = HIWORD(wParam);
        ctrlId = LOWORD(wParam);
        if (ctrlId == BTN_CANCEL && nCode == BN_CLICKED) {
            SendMessage(hwndDlg, WM_CLOSE, NULL, NULL);
        }
        break;
    }

    return 0;
}

LRESULT CALLBACK HotkeyPrompt::WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    return DefWindowProc(hWnd, message, wParam, lParam);
}