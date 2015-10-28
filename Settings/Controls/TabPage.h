#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>

#include "DialogBase.h"

class Control;

class TabPage : public DialogBase {
public:
    TabPage(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title = L"");

protected:
    HWND _hWnd;
    HINSTANCE _hInstance;
    HPROPSHEETPAGE _pageHandle;
    std::wstring _title;
};