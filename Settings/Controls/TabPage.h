// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>

#include "Dialog.h"

class Control;

class TabPage : public Dialog {
public:
    TabPage(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title = L"");

    HPROPSHEETPAGE PageHandle();

protected:
    HINSTANCE _hInstance;
    HPROPSHEETPAGE _pageHandle;
    std::wstring _title;

    static INT_PTR CALLBACK StaticTabProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
};