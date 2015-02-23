#include "UIUtils.h"

#include <sstream>

#include "3RVX.h"
#include "Settings.h"

std::vector<CPropertyPage *> UIUtils::pages = { };

CString UIUtils::Capitalize(std::wstring in) {
    CString cstr(in.c_str());
    return UIUtils::Capitalize(cstr);
}

CString UIUtils::Capitalize(CString in) {
    CString newString(in);
    CString firstLetter = newString.Left(1).MakeUpper();
    newString = firstLetter + newString.Mid(1);
    return newString;
}

int UIUtils::TextToInt(CWnd &wnd) {
    int num;
    CString text;
    wnd.GetWindowTextW(text);
    std::wistringstream wistr((LPCWSTR) text);
    wistr >> num;
    return num;
}

void UIUtils::SaveSettings(CPropertyPage &page) {
    HWND hWnd = NULL;
    for (CPropertyPage *page : pages) {
        HWND test = page->m_hWnd;
        if (IsWindow(test)) {
            hWnd = test;
        }
    }

    if (hWnd == page.m_hWnd) {
        OutputDebugString(L"Saving Settings XML\n");
        Settings::Instance()->Save();
    }


    HWND masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");
    SendMessage(masterWnd, WM_3RVX_CONTROL, MSG_LOAD, NULL);
}