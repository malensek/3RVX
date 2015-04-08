#include "UIContext.h"
#include <windowsx.h>

#define MAX_COMBOSTR 1024

UIContext::UIContext(HWND hWnd) :
_hWnd(hWnd) {

}

bool UIContext::GetCheck(int chkId) {
    return IsDlgButtonChecked(_hWnd, chkId) == BST_CHECKED;
}

bool UIContext::SetCheck(int chkId, bool checked) {
    return CheckDlgButton(
        _hWnd, chkId, checked ? BST_CHECKED : BST_UNCHECKED) == TRUE;
}

void UIContext::AddComboItem(int cmbId, std::wstring item) {
    SendDlgItemMessage(_hWnd, cmbId, CB_ADDSTRING, NULL, (LPARAM) item.c_str());
}

int UIContext::SelectComboItem(int cmbId, std::wstring item) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    return ComboBox_SelectString(cmbWnd, -1, item.c_str());
}

std::wstring UIContext::GetComboSelection(int cmbId) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    wchar_t text[MAX_COMBOSTR];
    ComboBox_GetText(cmbWnd, text, MAX_COMBOSTR);
    return std::wstring(text);
}

bool UIContext::SetText(int id, std::wstring text) {
    HWND wnd = GetDlgItem(_hWnd, id);
    return SetWindowText(wnd, text.c_str()) == TRUE;
}

void UIContext::Enable(int id) {
    HWND wnd = GetDlgItem(_hWnd, id);
    EnableWindow(wnd, TRUE);
}

void UIContext::Disable(int id) {
    HWND wnd = GetDlgItem(_hWnd, id);
    EnableWindow(wnd, FALSE);
}
