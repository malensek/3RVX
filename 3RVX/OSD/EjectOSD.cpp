// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "EjectOSD.h"

#include <Dbt.h>

#include "../DriveInfo.h"
#include "../HotkeyInfo.h"
#include "../Monitor.h"
#include "../NotifyIcon.h"
#include "../Skin/OSDComponent.h"
#include "../Skin/Skin.h"
#include "../Skin/SkinManager.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher"),
_mWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD") {

    SkinManager *skin = SkinManager::Instance();
    _mWnd.BackgroundImage(skin->EjectOSD()->background);
    _mWnd.EnableGlass(skin->EjectOSD()->mask);
    _mWnd.Update();

    OSD::InitMeterWnd(_mWnd);

    if (_settings->EjectIconEnabled()) {
        _iconImage = skin->EjectIcon();
        if (_iconImage != nullptr) {
            _icon = new NotifyIcon(Window::Handle(), L"Eject", _iconImage);
        }
    }

    _menu = CreatePopupMenu();
    /* Menu accepts both left and right clicks on its items: */
    _menuFlags = TPM_RIGHTBUTTON;
    if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
        _menuFlags |= TPM_RIGHTALIGN;
    } else {
        _menuFlags |= TPM_LEFTALIGN;
    }

    UpdateDriveMenu();
}

void EjectOSD::UpdateDriveMenu() {
    /* Remove any drives in the menu */
    for (unsigned int i = 0; i < _menuItems.size(); ++i) {
        RemoveMenu(_menu, 0, MF_BYPOSITION);
    }
    _menuItems.clear();

    int menuItem = 0;

    DWORD drives = GetLogicalDrives();
    /* Get the most significant bit of the drive bitset */
    DWORD msb = (DWORD) log2(drives);
    for (DWORD i = 0; i < msb; ++i, drives >>= 1) {
        if (drives & 0x1) {
            wchar_t letter = (wchar_t) i + 65;
            wchar_t drivePath[] = L" :\\";
            DriveInfo di(letter);
            if (di.IsHotPluggable() || di.HasRemovableMedia()) {
                CLOG(L"Removable Drive: %c %s %s",
                    di.DriveLetter(),
                    di.ProductID().c_str(),
                    di.VendorID().c_str());

                std::wstring menuStr = L"[" + std::wstring(1, di.DriveLetter()) + L":] - " + di.ProductID();
                InsertMenu(_menu, -1, MF_ENABLED, menuItem++, menuStr.c_str());
                _menuItems.push_back(di);
            }
        }
    }
}

void EjectOSD::EjectDrive(std::wstring driveLetter) {
    std::wstring name = DriveInfo::DriveFileName(driveLetter);
    CLOG(L"Ejecting %s", name.c_str());

    HANDLE dev = CreateFile(
        name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        NULL,
        NULL);

    if (dev == INVALID_HANDLE_VALUE) {
        CLOG(L"Failed to get device handle");
        return;
    }

    DWORD bytesReturned = 0;

    DeviceIoControl(dev, FSCTL_LOCK_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    PREVENT_MEDIA_REMOVAL pmr = { 0 };
    pmr.PreventMediaRemoval = FALSE;
    DeviceIoControl(dev, IOCTL_STORAGE_MEDIA_REMOVAL,
        &pmr, sizeof(PREVENT_MEDIA_REMOVAL), NULL, 0, &bytesReturned, NULL);

    DeviceIoControl(dev, FSCTL_DISMOUNT_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    BOOL ejected = FALSE;
    ejected = DeviceIoControl(dev, IOCTL_STORAGE_EJECT_MEDIA,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    if (ejected != FALSE) {
        std::wstring rootPath = driveLetter + L":\\";
        if (GetDriveType(rootPath.c_str()) != DRIVE_CDROM) {
            int driveBit = (int) pow(2, (driveLetter.at(0) - 65));
            _ignoreDrives |= driveBit;
            CLOG(L"Added drive bit %d to ignore list", driveBit);
        }
        if (OSD::Enabled()) {
            HideOthers(Eject);
            _mWnd.Show();
        }
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
            } else if (OSD::Enabled()) {
                HideOthers(Eject);
                _mWnd.Show();
            }

            if (_latestDrive == driveMask) {
                _latestDrive = 0;
            }
        }
    } else if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEARRIVAL) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;
            
            if (lpdbv->dbcv_flags != DBTF_NET) {
                _latestDrive = lpdbv->dbcv_unitmask;

                CLOG(L"Media inserted in drive %c:",
                    MaskToDriveLetter(_latestDrive));
            }
        }
    } else if (message == MSG_NOTIFYICON) {
        if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP) {
            POINT p;
            GetCursorPos(&p);
            SetForegroundWindow(hWnd);
            TrackPopupMenuEx(_menu, _menuFlags, p.x, p.y,
                Window::Handle(), NULL);
            PostMessage(hWnd, WM_NULL, 0, 0);
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

void EjectOSD::OnDisplayChange() {
    InitMeterWnd(_mWnd);
}
