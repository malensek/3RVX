#include "SettingsTab.h"

SettingsTab::SettingsTab(
        HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title) :
TabPage(hInstance, tabTemplate, title) {

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