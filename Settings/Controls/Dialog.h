#pragma once

#include "../../3RVX/Window.h"

class Dialog : public Window {
public:
    Dialog(LPCWSTR className, LPCWSTR dlgTemplate);

protected:
    static INT_PTR CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
};