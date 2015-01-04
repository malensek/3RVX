#include "EjectOSD.h"

#include <Dbt.h>

#include "..\Monitor.h"
#include "..\Skin.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher") {
    _mWnd = new MeterWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD");

    Skin *skin = Settings::Instance()->CurrentSkin();

    if (skin->HasOSD("eject") == false) {
        return;
    }

    Gdiplus::Bitmap *bg = skin->OSDBgImg("eject");
    _mWnd->BackgroundImage(bg);
    _mWnd->Update();

    _mWnd->VisibleDuration(800);

    HMONITOR monitor = Monitor::Default();
    PositionWindow(monitor, *_mWnd);
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