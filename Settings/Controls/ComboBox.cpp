#include "ComboBox.h"

#include <windowsx.h>

void ComboBox::AddItem(std::wstring item) {
    SendDlgItemMessage(_parent, _id, CB_ADDSTRING, NULL, (LPARAM) item.c_str());
}

int ComboBox::Select(std::wstring item) {
    return ComboBox_SelectString(_hWnd, -1, item.c_str());
}

void ComboBox::Select(int itemIdx) {
    ComboBox_SetCurSel(_hWnd, itemIdx);
}

std::wstring ComboBox::Selection() {
    wchar_t text[MAX_EDITSTR];
    ComboBox_GetText(_hWnd, text, MAX_EDITSTR);
    return std::wstring(text);
}

int ComboBox::SelectionIndex() {
    return ComboBox_GetCurSel(_hWnd);
}

DLGPROC ComboBox::Command(unsigned short nCode) {
    switch (nCode) {
    case CBN_SELCHANGE:
        if (OnSelectionChange) {
            return (DLGPROC) OnSelectionChange();
        }
    }

    return FALSE;
}
