// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "ListBox.h"

#include <windowsx.h>

int ListBox::AddItem(std::wstring item) {
    return ListBox_AddString(_hWnd, item.c_str());
}

int ListBox::InsertItem(int index, std::wstring item) {
    return ListBox_InsertString(_hWnd, index, item.c_str());
}

BOOL CALLBACK ListBox::Command(unsigned short nCode) {
    switch (nCode) {
    case LBN_SELCHANGE:
        if (OnSelectionChange) {
            return OnSelectionChange();
        }
    }

    return FALSE;
}
