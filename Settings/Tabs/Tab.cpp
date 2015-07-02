#include "Tab.h"

Tab::Tab() {

}

Tab::~Tab() {

}

DLGPROC Tab::TabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    unsigned short nCode, ctrlId;

    switch (message) {
    case WM_INITDIALOG:
        _hWnd = hDlg;
        Initialize();
        LoadSettings();
        return FALSE;

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hDlg), NULL);
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

    return FALSE;
}