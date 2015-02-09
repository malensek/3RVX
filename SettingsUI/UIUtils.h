#pragma once

#include "stdafx.h"
#include <string>
#include <vector>

#define CHECKED(checkbox) (checkbox.GetCheck() == 1) ? true : false

class UIUtils {
public:
    static std::vector<CPropertyPage *> pages;

    static CString Capitalize(CString in);
    static CString Capitalize(std::wstring in);
    static int TextToInt(CWnd &wnd);
    static void SaveSettings(CPropertyPage &page);
};