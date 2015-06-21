#include "ComboBox.h"

#include <windowsx.h>

void ComboBox::AddItem(std::wstring item) {
    ComboBox_AddString(_hWnd, &item[0]);
}

std::vector<std::wstring> ComboBox::Items() {
    std::vector<std::wstring> items;
    for (int i = 0; i < Count(); ++i) {
        wchar_t buf[MAX_EDITSTR];
        ComboBox_GetLBText(_hWnd, i, buf);
        items.push_back(buf);
    }
    return items;
}

void ComboBox::Clear() {
    ComboBox_ResetContent(_hWnd);
}

int ComboBox::Count() {
    return ComboBox_GetCount(_hWnd);
}

void ComboBox::Select(int itemIdx) {
    ComboBox_SetCurSel(_hWnd, itemIdx);
}

int ComboBox::Select(std::wstring item) {
    int idx = ComboBox_FindStringExact(_hWnd, -1, item.c_str());
    if (idx != CB_ERR) {
        Select(idx);
    }
    return idx;
}

int ComboBox::SelectPrefix(std::wstring item) {
    return ComboBox_SelectString(_hWnd, -1, item.c_str());
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
