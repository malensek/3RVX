#include "Dialog.h"

Dialog::Dialog(LPCWSTR className, LPCWSTR dlgTemplate) :
Window(className) {
    _dlgHwnd = CreateDialogParam(
        Window::InstanceHandle(),
        dlgTemplate,
        Window::Handle(),
        StaticDialogProc,
        (LPARAM) this);
}

HWND Dialog::DialogHandle() {
    return _dlgHwnd;
}

INT_PTR Dialog::StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam) {
    Dialog *dlg;

    if (uMsg == WM_INITDIALOG) {
        dlg = (Dialog *) lParam;
        SetWindowLongPtr(hwndDlg, DWLP_USER, (LONG_PTR) dlg);
    } else {
        dlg = (Dialog *) GetWindowLongPtr(hwndDlg, DWLP_USER);
        if (!dlg) {
            return FALSE;
        }
    }

    return dlg->DialogProc(hwndDlg, uMsg, wParam, lParam);
}

INT_PTR Dialog::DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam) {

    return FALSE;
}
