// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "UITranslator.h"

#include <CommCtrl.h>

#include "../3RVX/Settings.h"
#include "../3RVX/LanguageTranslator.h"
#include "../3RVX/Logger.h"

void UITranslator::TranslateControlText(HWND hWnd, int dlgItem) {
    wchar_t text[1024];
    GetDlgItemText(hWnd, dlgItem, text, 1024);
    std::wstring str(text);
    TranslateControlText(hWnd, dlgItem, str);
}

void UITranslator::TranslateControlText(
        HWND hWnd, int dlgItem, std::wstring &str) {
    std::wstring trans = Settings::Instance()->Translator()->Translate(str);
    SetDlgItemText(hWnd, dlgItem, trans.c_str());
}

void UITranslator::TranslateWindowText(HWND hWnd) {
    wchar_t text[1024];
    GetWindowText(hWnd, text, 1024);
    std::wstring str(text);
    std::wstring trans = Settings::Instance()->Translator()->Translate(str);
    SetWindowText(hWnd, trans.c_str());
}