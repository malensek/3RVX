#include "Tab.h"

#include "UIContext.h"

Tab::Tab() {

}

Tab::~Tab() {
    delete _ctxt;
}

DLGPROC Tab::TabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    unsigned short nCode, ctrlId;

    switch (message) {
    case WM_INITDIALOG:
        _hWnd = hDlg;
        _ctxt = new UIContext(hDlg);
        LoadSettings();
        return FALSE;

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hDlg), NULL);
        nCode = HIWORD(wParam);
        ctrlId = LOWORD(wParam);
        return Command(nCode, ctrlId);

    case WM_NOTIFY:
        NMHDR *nHdr = (NMHDR *) lParam;
        return Notification(nHdr);
    }

    return FALSE;
}