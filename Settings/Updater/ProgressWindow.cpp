#include "ProgressWindow.h"

#include <chrono>

#include "../../3RVX/Logger.h"
#include "../Controls/Controls.h"
#include "../resource.h"

ProgressWindow::ProgressWindow(Version version) :
Dialog(L"3RVX-UpdateProgress", MAKEINTRESOURCE(IDD_DOWNLOAD)),
_version(version) {
    ShowWindow(Dialog::DialogHandle(), SW_SHOWNORMAL);

    _cancel = new Button(BTN_CANCEL, *this);
    _progress = new ProgressBar(PRG_DOWNLOAD, *this);
    _progress->Marquee(true);

    _cancel->OnClick = [this]() {
        SendMessage(Window::Handle(), WM_CLOSE, 0, 0);
        return true;
    };

    _dlThread = std::thread(&ProgressWindow::Download, this);
}


void ProgressWindow::Download() {
    CLOG(L"Starting download thread");
    std::wstring path = Updater::DownloadVersion(_version, _progress);
    if (path == L"") {
        CLOG(L"Error downloading file!");
        return;
    } else {
        using namespace std::literals;
        std::this_thread::sleep_for(250ms);

        ShellExecute(NULL, L"open", path.c_str(), 0, 0, SW_SHOWNORMAL);
        SendMessage(Window::Handle(), WM_CLOSE, 0, 0);
    }
}

LRESULT ProgressWindow::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
    }

    return Dialog::WndProc(hWnd, message, wParam, lParam);
}