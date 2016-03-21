// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Dialog.h"

#include "../../3RVX/Logger.h"
#include "../UITranslator.h"
#include "Control.h"

Dialog::Dialog() {

}

Dialog::Dialog(HWND parent, LPCWSTR dlgTemplate) :
_parent(parent),
_template(dlgTemplate) {
//    _dlgHwnd = CreateDialogParam(
//        NULL,
//        dlgTemplate,
//        parent,
//        StaticDialogProc,
//        (LPARAM) this);

//    if (_dlgHwnd == NULL) {
//        Logger::LogLastError();
//    }

//    UITranslator::TranslateWindowText(_dlgHwnd);
}

void Dialog::Show() {
    DialogBoxParam(NULL, _template, _parent, StaticDialogProc, (LPARAM) this);
    //UITranslator::TranslateWindowText(_dlgHwnd);
}

void Dialog::Close(int result) {
    EndDialog(_dlgHwnd, result);
}

void Dialog::AddControl(Control *control) {
    if (control == nullptr) {
        return;
    }

    int id = control->ID();
    _controlMap[id] = control;
}

HWND Dialog::DialogHandle() {
    return _dlgHwnd;
}

HWND Dialog::ParentHandle() {
    return _parent;
}

INT_PTR Dialog::StaticDialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Dialog *dlg;

    if (uMsg == WM_INITDIALOG) {
        dlg = (Dialog *) lParam;
        dlg->_dlgHwnd = hwndDlg;
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

    unsigned short nCode, ctrlId;

    switch (uMsg) {
    case WM_INITDIALOG:
        Initialize();
        return FALSE;

    case WM_CLOSE: {
        Close();
        break;
    }

    case WM_COMMAND: {
        nCode = HIWORD(wParam);
        ctrlId = LOWORD(wParam);
        if (_controlMap.count(ctrlId) > 0) {
            return _controlMap[ctrlId]->Command(nCode);
        } else {
            return FALSE;
        }
    }

    case WM_NOTIFY: {
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
        HWND scrollHandle = (HWND) lParam;
        ctrlId = GetDlgCtrlID(scrollHandle);

        if (_controlMap.count(ctrlId) > 0) {
            return _controlMap[ctrlId]->Scroll(
                (uMsg == WM_HSCROLL), request, position);
        } else {
            return FALSE;
        }
    }
    }

    return FALSE;
}
