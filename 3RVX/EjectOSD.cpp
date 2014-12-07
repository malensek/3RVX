#include "EjectOSD.h"

#include <Dbt.h>

#include "Monitor.h"
#include "Skin.h"

EjectOSD::EjectOSD(HINSTANCE hInstance, Settings &settings) :
OSD(hInstance, L"3RVX-EjectDispatcher", settings) {
    _mWnd = new MeterWnd(hInstance, L"3RVX-EjectOSD", L"3RVX-EjectOSD",
        NULL, NULL, 800);

    Skin skin(_settings.SkinXML());
    Gdiplus::Bitmap *bg = skin.OSDBgImg("eject");
    _mWnd->BackgroundImage(bg);
    _mWnd->Update();

    HMONITOR monitor = Monitor::Default();
    const int mWidth = Monitor::Width(monitor);
    const int mHeight = Monitor::Height(monitor);
    _mWnd->X(mWidth / 2 - _mWnd->Width() / 2);
    _mWnd->Y(mHeight - _mWnd->Height() - 140);
}

void EjectOSD::Hide() {
    _mWnd->Hide(false);
}

LRESULT
EjectOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DEVICECHANGE) {
        if (wParam == DBT_DEVICEREMOVECOMPLETE) {
            CLOG(L"Device removal notification received");
            _mWnd->Show();
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}