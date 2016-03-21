// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "ProgressWindow.h"

#include <chrono>

#include "../../3RVX/Logger.h"
#include "../../3RVX/StringUtils.h"
#include "../Controls/Controls.h"
#include "../resource.h"
#include "../UITranslator.h"

ProgressWindow::ProgressWindow(HWND parent, Version version) :
Dialog(parent, MAKEINTRESOURCE(IDD_DOWNLOAD)),
_version(version) {

}

void ProgressWindow::Initialize() {
    UITranslator::TranslateWindowText(DialogHandle());

    _cancel = new Button(BTN_CANCEL, *this);
    _progress = new ProgressBar(PRG_DOWNLOAD, *this);

    _cancel->OnClick = [this]() {
        Dialog::Close();
        return true;
    };

    _dlThread = std::thread(&ProgressWindow::Download, this);
    _dlThread.detach();
}

void ProgressWindow::Download() {
    INT_PTR result = 0;

    CLOG(L"Starting download thread");
    std::wstring path = Updater::DownloadVersion(_version, _progress);
    if (path == L"") {
        result = 1;
    } else {
        using namespace std::literals;
        std::this_thread::sleep_for(250ms);

        std::wstring ext = StringUtils::FileExtension(path);
        CLOG(L"File extension: %s", ext.c_str());
        if (ext == L"zip") {
            size_t slash = path.find_last_of(L"/\\");
            if (slash != std::wstring::npos) {
                std::wstring dir = path.substr(0, slash);
                CLOG(L"Launching download folder: %s", dir.c_str());
                ShellExecute(NULL, L"open", dir.c_str(), 0, 0, SW_SHOWNORMAL);
            }
        } else {
            ShellExecute(NULL, L"open", path.c_str(), 0, 0, SW_SHOWNORMAL);
        }
    }

    Dialog::Close(result);
}
