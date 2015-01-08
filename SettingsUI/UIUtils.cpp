#include "UIUtils.h"

#include <sstream>

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