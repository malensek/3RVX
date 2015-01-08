#pragma once

#include "stdafx.h"
#include <string>

#define CHECKED(checkbox) (checkbox.GetCheck() == 1) ? true : false

class UIUtils {
public:
    static CString Capitalize(CString in);
    static CString Capitalize(std::wstring in);
    static int TextToInt(CWnd &wnd);
};