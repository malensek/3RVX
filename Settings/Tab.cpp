#include "Tab.h"

#include "UIContext.h"

Tab::Tab() {

}

Tab::~Tab() {
    delete _ctxt;
}

DLGPROC Tab::TabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    unsigned short nCode, ctrlId;
    LPNMHDR lpnmhdr;

    switch (message) {
    case WM_INITDIALOG:
        _hWnd = hDlg;
        _ctxt = new UIContext(hDlg);
        LoadSettings();
        break;

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hDlg), NULL);
        nCode = HIWORD(wParam);
        ctrlId = LOWORD(wParam);
        Command(nCode, ctrlId);
        break;

    case WM_NOTIFY:
        lpnmhdr = (NMHDR FAR *) lParam;
        if (lpnmhdr->code == PSN_APPLY) {
            SaveSettings();
        }
        break;
    }

    return FALSE;
}