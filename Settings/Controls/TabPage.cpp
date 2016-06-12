// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "TabPage.h"

TabPage::TabPage(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title) :
_title(title) {

    if (hInstance == NULL) {
        hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }
    _hInstance = hInstance;

    PROPSHEETPAGE psp = { 0 };
    psp.dwSize = sizeof(PROPSHEETPAGE);
    if (_title != L"") {
        psp.dwFlags |= PSP_USETITLE;
        psp.pszTitle = &_title[0];
    }
    psp.hInstance = _hInstance;
    psp.pszTemplate = tabTemplate;
    psp.pszIcon = NULL;
    psp.pfnDlgProc = StaticTabProc;
    psp.lParam = (LPARAM) this;

    _pageHandle = CreatePropertySheetPage(&psp);
}

INT_PTR TabPage::StaticTabProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    TabPage *tab;

    if (uMsg == WM_INITDIALOG) {
        tab = (TabPage *) ((LPPROPSHEETPAGE) lParam)->lParam;
        tab->_dlgHwnd = hwndDlg;
        SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR) tab);
    } else {
        tab = (TabPage *) GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
        if (!tab) {
            return FALSE;
        }
    }

    return tab->DialogProc(hwndDlg, uMsg, wParam, lParam);
}


HPROPSHEETPAGE TabPage::PageHandle() {
    return _pageHandle;
}
