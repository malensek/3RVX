#include "EjectOSD.h"

#include <Dbt.h>

#include "..\Monitor.h"
#include "..\Skin.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher"),
_mWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD") {

    Skin *skin = Settings::Instance()->CurrentSkin();

    if (skin->HasOSD("eject") == false) {
        return;
    }

    Gdiplus::Bitmap *bg = skin->OSDBgImg("eject");
    _mWnd.BackgroundImage(bg);

    Gdiplus::Bitmap *mask = skin->OSDMask("eject");
    if (mask != NULL) {
        CLOG(L"Applying glass to eject OSD");
        _mWnd.GlassMask(mask);
    }

    _mWnd.Update();
    _mWnd.VisibleDuration(Settings::Instance()->HideDelay());

    HMONITOR monitor = Monitor::Primary();
    PositionWindow(monitor, _mWnd);
}

EjectOSD::~EjectOSD() {
}

void EjectOSD::Hide() {
    _mWnd.Hide(false);
}

LRESULT
EjectOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DEVICECHANGE
        && wParam == DBT_DEVICEREMOVECOMPLETE) {

        CLOG(L"Device removal notification received");
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;
            if (lpdbv->dbcv_flags & DBTF_MEDIA) {
                CLOG(L"Media volume has been removed: eject notification");
                HideOthers(Eject);
                _mWnd.Show();
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}