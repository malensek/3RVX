#pragma once

#include <Windows.h>
#include <string>

class HotkeyPrompt {
public:
    static void Show(HWND parent, HINSTANCE hInstance = NULL);

protected:
    static HotkeyPrompt *_instance;
    static HINSTANCE _hInstance;
    static HWND _hWnd;
    static HWND _parent;

    HotkeyPrompt();
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    static const wchar_t *CLASS_NAME;
};