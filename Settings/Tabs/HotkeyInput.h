// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Controls/Dialog.h"
#include "../resource.h"

class HotkeyInput : public Dialog {
public:

protected:
    virtual INT_PTR CALLBACK DialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
};