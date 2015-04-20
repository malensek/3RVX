#include "ListView.h"

#include <CommCtrl.h>

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

void ListView::ItemText(int index, int subIndex, std::wstring text) {
    ListView_SetItemText(_hWnd, index, subIndex, &text[0]);
}

int ListView::Size() {
    return ListView_GetItemCount(_hWnd);
}
