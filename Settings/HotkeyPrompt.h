#pragma once

#include <Windows.h>
#include <string>

class HotkeyPrompt {
public:
    HotkeyPrompt(HINSTANCE hInstance = NULL,
        std::wstring className = L"3RVX Hotkey Input Dialog");

    void Show();
    static void Show(HWND parent, HINSTANCE hInstance = NULL);

protected:
    HINSTANCE _hInstance;
    HWND _hWnd;

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};