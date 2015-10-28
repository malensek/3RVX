#pragma once

#include <Windows.h>

class DialogBase {
public:
    static INT_PTR CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam) = 0;
};