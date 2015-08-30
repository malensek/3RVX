#pragma once

#include "../Controls/Dialog.h"
#include "../resource.h"

class ProgressWindow : public Dialog {
public:
    ProgressWindow() :
    Dialog(L"3RVX-UpdateProgress", MAKEINTRESOURCE(IDD_DOWNLOAD)) {
        ShowWindow(Dialog::DialogHandle(), SW_SHOWNORMAL);
    }

};