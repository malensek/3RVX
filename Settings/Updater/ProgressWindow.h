#pragma once

#include "../Controls/Dialog.h"
#include "../resource.h"

#include "../Controls/Button.h"
#include "../Controls/ProgressBar.h"

#include "Updater.h"
#include "Version.h"

class ProgressWindow : public Dialog {
public:
    ProgressWindow() :
    Dialog(L"3RVX-UpdateProgress", MAKEINTRESOURCE(IDD_DOWNLOAD)) {
        ShowWindow(Dialog::DialogHandle(), SW_SHOWNORMAL);

        _cancel = new Button(BTN_CANCEL, *this);
        _progress = new ProgressBar(PRG_DOWNLOAD, *this);
        _progress->Range(0, 100);
        _progress->Position(79);
        //_progress->Marquee(true);

        //_cancel.Enabled(false);
        _cancel->OnClick = []() {
            return true;
        };

        Updater::DownloadVersion({ 2, 9, 1 }, _progress);
    }

private:
    Button *_cancel;
    ProgressBar *_progress;


};