#include "EjectOSD.h"

#include <Dbt.h>

#include "..\Monitor.h"
#include "..\Skin.h"
#include "..\SkinManager.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher"),
_mWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD") {

    Skin *skin = SkinManager::Instance()->CurrentSkin();

    if (skin->HasOSD("eject") == false) {
        return;
    }

    /* TODO: NULL check*/
    _mWnd.BackgroundImage(skin->ejectBackground);

    if (skin->ejectMask != NULL) {
        _mWnd.GlassMask(skin->ejectMask);
    }

    _mWnd.Update();
    _mWnd.VisibleDuration(Settings::Instance()->HideDelay());

    UpdateWindowPositions(ActiveMonitors());
}

EjectOSD::~EjectOSD() {
}

void EjectOSD::Hide() {
    _mWnd.Hide(false);
}

void EjectOSD::UpdateWindowPositions(std::vector<Monitor> monitors) {
    PositionWindow(monitors[0], _mWnd);
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