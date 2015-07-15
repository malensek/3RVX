#include "EjectOSD.h"

#include <Dbt.h>

#include "..\HotkeyInfo.h"
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
        _mWnd.EnableGlass(skin->ejectMask);
    }

    _mWnd.Update();

    Settings *settings = Settings::Instance();
    _mWnd.AlwaysOnTop(settings->AlwaysOnTop());
    _mWnd.HideAnimation(settings->HideAnim(), settings->HideSpeed());
    _mWnd.VisibleDuration(settings->HideDelay());

    UpdateWindowPositions(ActiveMonitors());
}

EjectOSD::~EjectOSD() {

}

void EjectOSD::EjectDrive(std::wstring driveLetter) {
    std::wstring name = L"\\\\.\\" + driveLetter + L":";
    CLOG(L"Ejecting %s", name.c_str());

    HANDLE dev = CreateFile(name.c_str(),
        GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (dev == INVALID_HANDLE_VALUE) {
        CLOG(L"Failed to get device handle");
        return;
    }

    DWORD bytesReturned = 0;
    bool success = DeviceIoControl(dev, FSCTL_LOCK_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL)
    && DeviceIoControl(dev, FSCTL_DISMOUNT_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL)
    && DeviceIoControl(dev, IOCTL_STORAGE_EJECT_MEDIA,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    if (success) {
        std::wstring rootPath = driveLetter + L":\\";
        if (GetDriveType(rootPath.c_str()) != DRIVE_CDROM) {
            int driveBit = (int) pow(2, (driveLetter.at(0) - 65));
            _ignoreDrives |= driveBit;
            CLOG(L"Added drive bit %d to ignore list", driveBit);
        }
        HideOthers(Eject);
        _mWnd.Show();
    }

    CloseHandle(dev);
}

void EjectOSD::Hide() {
    _mWnd.Hide(false);
}

void EjectOSD::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {
    case HotkeyInfo::EjectDrive:
        if (hki.args.size() > 0) {
            EjectDrive(hki.args[0]);
        }
        break;

    case HotkeyInfo::EjectLastDisk:
        if (_latestDrive != 0) {
            wchar_t letter = MaskToDriveLetter(_latestDrive);
            std::wstring lStr(1, letter);
            EjectDrive(lStr);
            _latestDrive = 0;
        }

        break;
    }
}

void EjectOSD::UpdateWindowPositions(std::vector<Monitor> &monitors) {
    PositionWindow(monitors[0], _mWnd);
}

LRESULT
EjectOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEREMOVECOMPLETE) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;

            DWORD driveMask = lpdbv->dbcv_unitmask;
            CLOG(L"Eject notification received for drive %c:",
                MaskToDriveLetter(driveMask));

            if (driveMask & _ignoreDrives) {
                CLOG(L"Drive already ejected by a hotkey; not displaying OSD.");
                _ignoreDrives ^= driveMask;
            } else {
                HideOthers(Eject);
                _mWnd.Show();
            }

            if (_latestDrive == driveMask) {
                _latestDrive = 0;
            }
        }
    }

    if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEARRIVAL) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;

            if (lpdbv->dbcv_flags != DBTF_NET) {
                _latestDrive = lpdbv->dbcv_unitmask;

                CLOG(L"Media inserted in drive %c:",
                    MaskToDriveLetter(_latestDrive));
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD EjectOSD::DriveLetterToMask(wchar_t letter) {
    if (letter < 65 || letter > 90) {
        return 0;
    }

    return (DWORD) pow(2, (letter - 65));
}

wchar_t EjectOSD::MaskToDriveLetter(DWORD mask) {
    return (wchar_t) (log2(mask) + 65);
}
