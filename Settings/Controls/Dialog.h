// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <unordered_map>

#include "../../3RVX/Window.h"

class Control;

class Dialog : public Window {
public:
    Dialog(LPCWSTR className, LPCWSTR dlgTemplate);

    void AddControl(Control *control);

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