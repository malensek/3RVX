#include "UIContext.h"
#include <windowsx.h>

UIContext::UIContext(HWND hWnd) :
_hWnd(hWnd) {

}

bool UIContext::SetCheck(int chkId, bool checked) {
    return CheckDlgButton(_hWnd, chkId, checked ? BST_CHECKED : BST_UNCHECKED);
}

void UIContext::AddComboItem(int cmbId, std::wstring item) {
    SendDlgItemMessage(_hWnd, cmbId, CB_ADDSTRING, NULL, (LPARAM) item.c_str());
}

int UIContext::SelectComboItem(int cmbId, std::wstring item) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    return ComboBox_SelectString(cmbWnd, -1, item.c_str());
}
