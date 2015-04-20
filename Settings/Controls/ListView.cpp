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


int ListView::Size() {
    return ListView_GetItemCount(_hWnd);
}
