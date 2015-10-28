#include "DialogBase.h"

INT_PTR DialogBase::StaticDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DialogBase *dlg;

    if (uMsg == WM_INITDIALOG) {
        dlg = (DialogBase *) lParam;
        SetWindowLongPtr(hwndDlg, DWLP_USER, (LONG_PTR) dlg);
    } else {
        dlg = (DialogBase *) GetWindowLongPtr(hwndDlg, DWLP_USER);
        if (!dlg) {
            return FALSE;
        }
    }

    return dlg->DialogProc(hwndDlg, uMsg, wParam, lParam);
}
