#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>

#include "DialogBase.h"

class Control;

class TabPage : public DialogBase {
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