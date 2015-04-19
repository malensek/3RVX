#include "ListBox.h"

#include <CommCtrl.h>

void ListBox::AddColumn(std::wstring colName, int width) {
    InsertColumn(_columns, colName, width);
}

void ListBox::InsertColumn(int index, std::wstring colName, int width) {
    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_WIDTH | LVCF_TEXT;
    lvc.cx = width;
    lvc.pszText = &colName[0];
    int result = ListView_InsertColumn(_hWnd, index, &lvc);
    if (result != -1) {
        _columns++;
    }
}

void ListBox::AddItem(std::wstring item) {
    LVITEM lvi = { 0 };
    lvi.mask = LVIF_TEXT;
    lvi.iItem = 0;
    lvi.pszText = L"test";

    ListView_InsertItem(_hWnd, &lvi);
}
