// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "DialogBase.h"

#include "Control.h"

void DialogBase::AddControl(Control *control) {
    if (control == nullptr) {
        return;
    }

    int id = control->ID();
    _controlMap[id] = control;
}

HWND DialogBase::DialogHandle() {
    return _dlgHwnd;
}

INT_PTR DialogBase::StaticDialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

INT_PTR DialogBase::DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam) {

    unsigned short nCode, ctrlId;

    switch (uMsg) {
    case WM_INITDIALOG:
        _dlgHwnd = hwndDlg;
        return FALSE;

    case WM_COMMAND:
        nCode = HIWORD(wParam);
        ctrlId = LOWORD(wParam);
        if (_controlMap.count(ctrlId) > 0) {
            return _controlMap[ctrlId]->Command(nCode);
        } else {
            return FALSE;
        }

    case WM_NOTIFY:
        NMHDR *nHdr = (NMHDR *) lParam;
        ctrlId = nHdr->idFrom;
        if (_controlMap.count(ctrlId) > 0) {
            return _controlMap[ctrlId]->Notification(nHdr);
        } else {
            return FALSE;
        }
    }

    case WM_HSCROLL:
    case WM_VSCROLL: {
        WORD request = LOWORD(wParam);
        WORD position = HIWORD(wParam);
        return FALSE;
    }
    }

    return FALSE;
}
