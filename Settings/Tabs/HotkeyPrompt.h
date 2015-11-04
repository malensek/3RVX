// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <string>

// TODO: this should extend Window
class HotkeyPrompt {
public:
    static void Show(HWND parent, HINSTANCE hInstance = NULL);

protected:
    static HotkeyPrompt *_instance;
    static HINSTANCE _hInstance;
    static HWND _hWnd;
    static HWND _parent;

    HotkeyPrompt();
    static void Translate(HWND hwndDlg);

    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    static const wchar_t *CLASS_NAME;
};