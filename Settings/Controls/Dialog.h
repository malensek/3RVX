// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <unordered_map>

#include "../../3RVX/Window.h"
#include "DialogBase.h"

class Control;

class Dialog : DialogBase, public Window {
public:
    Dialog(LPCWSTR className, LPCWSTR dlgTemplate);

    void AddControl(Control *control);

    HWND DialogHandle();

protected:
    HWND _dlgHwnd;

    /// <summary>Maps control IDs to their respective instances.</summary>
    std::unordered_map<int, Control *> _controlMap;

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
};