#pragma once

#include <unordered_map>
#include "../../3RVX/Window.h"

#include "Control.h"
class Dialog : public Window {
public:
    Dialog(LPCWSTR className, LPCWSTR dlgTemplate);

    HWND DialogHandle();

protected:
    static INT_PTR CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    HWND _dlgHwnd;

    /// <summary>Maps control IDs to their respective instances.</summary>
    std::unordered_map<int, Control *> _controlMap;

};