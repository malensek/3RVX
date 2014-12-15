#include "EjectOSD.h"

#include <Dbt.h>

#include "..\Monitor.h"
#include "..\Skin.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher") {
    _mWnd = new MeterWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD");

    Skin skin(_settings.SkinXML());
    Gdiplus::Bitmap *bg = skin.OSDBgImg("eject");
    _mWnd->BackgroundImage(bg);
    _mWnd->Update();

    _mWnd->VisibleDuration(800);

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
            HideOthers(Eject);
            _mWnd->Show();
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}