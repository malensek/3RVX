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
    psp.pfnDlgProc = StaticDialogProc;
    psp.lParam = (LPARAM) this;

    _pageHandle = CreatePropertySheetPage(&psp);
}
