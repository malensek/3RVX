#include "UIUtils.h"

bool UIUtils::SetCheck(HWND hWnd, int chkId, bool checked) {
    HWND chkWnd = GetDlgItem(hWnd, chkId);
    if (chkWnd == NULL) {
        return false;
    }
    int state = checked ? BST_CHECKED : BST_UNCHECKED;
    SendMessage(chkWnd, BM_SETCHECK, (WPARAM) state, NULL);
}