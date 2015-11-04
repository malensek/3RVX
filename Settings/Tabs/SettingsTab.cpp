// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SettingsTab.h"

#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Settings.h"

SettingsTab::SettingsTab(
        HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title) :
TabPage(hInstance, tabTemplate,
    Settings::Instance()->Translator()->Translate(title).c_str()) {

}

SettingsTab::~SettingsTab() {

}

INT_PTR SettingsTab::DialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_INITDIALOG:
        _dlgHwnd = hwndDlg;
        Initialize();
        LoadSettings();
        return FALSE;

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hwndDlg), NULL);
        break;
    }

    return DialogBase::DialogProc(hwndDlg, uMsg, wParam, lParam);
}