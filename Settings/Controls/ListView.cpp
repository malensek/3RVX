#include "ListView.h"

#include <CommCtrl.h>

void ListView::AddListExStyle(DWORD style) {
    ListView_SetExtendedListViewStyleEx(_hWnd, style, style);
}

void ListView::AddColumn(std::wstring colName, int width) {
    InsertColumn(_columns, colName, width);
}

void ListView::InsertColumn(int index, std::wstring colName, int width) {
    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_WIDTH | LVCF_TEXT;
    lvc.cx = width;
    lvc.pszText = &colName[0];
    int result = ListView_InsertColumn(_hWnd, index, &lvc);
    if (result != -1) {
        _columns++;
    }
}

int ListView::AddItem(std::wstring item) {
    return InsertItem(Size(), item);
}

int ListView::InsertItem(int index, std::wstring item) {
    LVITEM lvi = { 0 };
    lvi.mask = LVIF_TEXT;
    lvi.iItem = index;
    lvi.pszText = &item[0];
    return ListView_InsertItem(_hWnd, &lvi);
}

void ListView::ItemState(int index, unsigned int state, unsigned int mask) {
    ListView_SetItemState(_hWnd, index, state, mask);
}

std::wstring ListView::ItemText(int index, int subIndex) {
    std::wstring str;
    str.reserve(MAX_EDITSTR);
    ListView_GetItemText(_hWnd, index, subIndex, &str[0], MAX_EDITSTR);
    return str;
}

void ListView::ItemText(int index, int subIndex, std::wstring text) {
    ListView_SetItemText(_hWnd, index, subIndex, &text[0]);
}

int ListView::Selection() {
    return ListView_GetSelectionMark(_hWnd);
}

void ListView::Selection(int index) {
    if (index < 0) {
        index = 0;
    }
    if (index > Size() - 1) {
        index = Size() - 1;
    }

    ItemState(index, LVIS_SELECTED, LVIS_SELECTED);
    ListView_SetSelectionMark(_hWnd, index);
    ListView_EnsureVisible(_hWnd, index, FALSE);
}

int ListView::Size() {
    return ListView_GetItemCount(_hWnd);
}

DLGPROC ListView::Notification(NMHDR *nHdr) {
    switch (nHdr->code) {
    case LVN_ITEMCHANGED:
        if (OnItemChange) {
            NMLISTVIEW *lv = reinterpret_cast<NMLISTVIEW *>(nHdr);
            OnItemChange(lv);
        }
    }

    return FALSE;
}
