// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <unordered_map>

class Control;

class Dialog {
public:
    Dialog();
    Dialog(HWND parent, LPCWSTR dlgTemplate);

    void AddControl(Control *control);
    HWND DialogHandle();
    HWND ParentHandle();

    virtual int Show();
    virtual void Close(int result = 0);

protected:
    HWND _dlgHwnd;
    HWND _parent;
    LPCWSTR _template;

    /// <summary>Maps control IDs to their respective instances.</summary>
    std::unordered_map<int, Control *> _controlMap;

    /// <summary>
    /// Performs intitialization for the dialog, similar to a constructor.   
    /// Since dialogs are created when shown, this method could be called much
    /// later than the constructor.
    /// </summary>
    virtual void Initialize() = 0;

    static INT_PTR CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
};
